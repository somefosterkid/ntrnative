#!/usr/bin/env python3
"""tools/hg_offscan.py: find raw "pointer + constant" struct offsets in HeartGold/SoulSilver's C that are right only on the DS.

The decomp (and our decompilation) reach struct members as `*(T *)((u8 *)p + 0x18)`, `p + 0x18` on a void * or u8 *, `&((u8 *)p)[0x18]`.
The constant is the member's DS offset (4-byte pointers). On the 64-bit host a struct with a pointer member before that offset is
laid out differently, so the access lands in another member (a write corrupts a pointer: docs/hgss.md, the Foresight animation work block).

The tool parses each file of the port's working copy (build/pokeheartgold, the port's compile line) with libclang, finds every such
expression, takes the struct the base points to (its static type, or the type the same variable is cast to elsewhere in the function),
computes that struct's DS layout (pointer and long 4 bytes, long long / double 8 aligned 8: the ARM EABI the DS uses) and the host
layout, and reports the member the constant means on the DS and where it is on the host. Hits are ranked: writes first, pointer
members, then reads.

  python3 tools/hg_offscan.py --cc COMPILE_COMMANDS.json [--map OLD=NEW] [--match REGEX] [-j N] [--out FILE] [FILES...]

--cc: a build's compile_commands.json (the HG/SS port build, e.g. build/port/linux/); --map rewrites a path prefix in it (a build dir
made in another checkout); --match selects files by regex (default: files of the working copy's src/ that contain "+ 0x" or "[0x").
"""
import argparse, concurrent.futures as cf, json, os, re, shlex, sys

import clang.cindex as ci
from clang.cindex import CursorKind as CK, TypeKind as TK

LIB = '/usr/lib/llvm-18/lib/libclang-18.so.1'
SKIP_ARGS = {'-o', '-MF', '-MQ', '-MT', '-c'}

BYTE = (TK.CHAR_S, TK.CHAR_U, TK.SCHAR, TK.UCHAR, TK.VOID)


def init_lib():
    for p in (os.environ.get('LIBCLANG'), LIB, '/usr/lib/x86_64-linux-gnu/libclang-18.so.1'):
        if p and os.path.exists(p):
            try:
                ci.Config.set_library_file(p)
            except Exception:
                pass
            break


# ---------------------------------------------------------------------------------------------------------- layouts
class Leaf:
    __slots__ = ('path', 'dso', 'dsz', 'ho', 'hsz', 'ptr')

    def __init__(self, path, dso, dsz, ho, hsz, ptr):
        self.path, self.dso, self.dsz, self.ho, self.hsz, self.ptr = path, dso, dsz, ho, hsz, ptr


class Lay:
    """a type's DS size/align, its host size, and its leaf members (scalars and pointers)."""
    __slots__ = ('dsz', 'dal', 'hsz', 'leaves', 'differs')

    def __init__(self, dsz, dal, hsz, leaves, differs):
        self.dsz, self.dal, self.hsz, self.leaves, self.differs = dsz, dal, hsz, leaves, differs


_cache = {}


def lay(t):
    t = t.get_canonical()
    key = t.spelling
    if t.kind == TK.RECORD:
        d = t.get_declaration()
        key = '%s@%s:%s' % (t.spelling, d.location.file.name if d.location.file else '', d.location.line)
    r = _cache.get(key)
    if r is None:
        r = _cache[key] = _lay(t)
    return r


def _scalar(t, size, hsz, ptr=False):
    return Lay(size, min(size, 8), hsz, [Leaf('', 0, size, 0, hsz, ptr)], size != hsz)


def _lay(t):
    k = t.kind
    if k == TK.POINTER:
        return _scalar(t, 4, t.get_size(), True)
    if k in (TK.LONG, TK.ULONG):
        return _scalar(t, 4, t.get_size())
    if k == TK.CONSTANTARRAY:
        e = lay(t.element_type)
        n = t.element_count
        lv = []
        if n <= 64:
            for i in range(n):
                for l in e.leaves:
                    lv.append(Leaf('[%d]%s' % (i, l.path), i * e.dsz + l.dso, l.dsz, i * e.hsz + l.ho, l.hsz, l.ptr))
        else:
            for i in (0, 1, n - 1):
                for l in e.leaves:
                    lv.append(Leaf('[%d]%s' % (i, l.path), i * e.dsz + l.dso, l.dsz, i * e.hsz + l.ho, l.hsz, l.ptr))
        return Lay(e.dsz * n, e.dal, t.get_size(), lv, e.differs)
    if k == TK.INCOMPLETEARRAY:
        e = lay(t.element_type)
        return Lay(0, e.dal, 0, [], e.differs)
    if k == TK.RECORD:
        d = t.get_declaration()
        union = d.kind == CK.UNION_DECL
        bp = 0                # bit position in the DS layout
        al = 1
        lv = []
        mx = 0
        for f in t.get_fields():
            fl = lay(f.type)
            al = max(al, fl.dal)
            if f.is_bitfield():
                w = f.get_bitfield_width()
                sz = fl.dsz
                if (bp % (8 * sz)) + w > 8 * sz:
                    bp = (bp + 8 * sz - 1) // (8 * sz) * (8 * sz)
                if union:
                    mx = max(mx, (w + 7) // 8)
                else:
                    bp += w
                continue
            a = fl.dal
            if union:
                o = 0
            else:
                bp = (bp + 7) // 8 * 8
                o = (bp // 8 + a - 1) // a * a
                bp = o * 8
            name = f.spelling
            try:
                hoff = f.get_field_offsetof() // 8
            except Exception:
                hoff = t.get_offset(name) // 8 if name else 0
            for l in fl.leaves:
                lv.append(Leaf(('.' + name if name else '') + l.path, o + l.dso, l.dsz, hoff + l.ho, l.hsz, l.ptr))
            if union:
                mx = max(mx, fl.dsz)
            else:
                bp += fl.dsz * 8
        size = mx if union else (bp + 7) // 8
        size = (size + al - 1) // al * al
        hsz = t.get_size()
        return Lay(size, al, hsz, lv, size != hsz or any(l.dso != l.ho for l in lv))
    if k == TK.ENUM:
        return _scalar(t, t.get_size(), t.get_size())
    s = t.get_size()
    if s < 0:
        s = 0
    return _scalar(t, s, s)


def host_of(S, K, width=0):
    """members of S that sit at DS offset K, with their host offsets: [(path, dso, ho, ptr, dsz)]"""
    L = lay(S)
    out = []
    for l in L.leaves:
        if l.dso <= K < l.dso + max(l.dsz, 1):
            out.append(l)
    # an exact start beats containing
    ex = [l for l in out if l.dso == K]
    return ex or out


# ---------------------------------------------------------------------------------------------------------- AST helpers
def strip(c):
    """drop parens / unexposed wrappers (implicit casts); keep explicit casts"""
    while c is not None and c.kind in (CK.PAREN_EXPR, CK.UNEXPOSED_EXPR):
        ch = list(c.get_children())
        if len(ch) != 1:
            break
        c = ch[0]
    return c


def tok(c):
    try:
        return [t.spelling for t in c.get_tokens()]
    except Exception:
        return []


def intlit(c):
    c = strip(c)
    if c is None:
        return None
    if c.kind == CK.INTEGER_LITERAL:
        t = tok(c)
        if t:
            s = t[0].rstrip('uUlL')
            try:
                return int(s, 0)
            except ValueError:
                return None
    return None


def binop(c):
    """operator spelling of a BINARY_OPERATOR / COMPOUND_ASSIGNMENT_OPERATOR cursor"""
    ch = list(c.get_children())
    if len(ch) != 2:
        return None, ch
    lend = ch[0].extent.end.offset
    for t in c.get_tokens():
        if t.extent.start.offset >= lend and t.kind.name == 'PUNCTUATION' and t.spelling not in (')',):
            return t.spelling, ch
    return None, ch


def pointee_record(t):
    """the type a pointer's pointee is for the check (record, or array of records), or None"""
    t = t.get_canonical()
    if t.kind != TK.POINTER:
        return None
    p = t.get_pointee().get_canonical()
    return p


def is_bytep(t):
    t = t.get_canonical()
    return t.kind == TK.POINTER and t.get_pointee().get_canonical().kind in BYTE


def is_intlike(t):
    return t.get_canonical().kind in (TK.INT, TK.UINT, TK.LONG, TK.ULONG, TK.LONGLONG, TK.ULONGLONG, TK.SHORT, TK.USHORT)


def base_types(e, fn_info):
    """the structs the pointer expression e (already stripped of byte casts) may point to: [(type, how)]"""
    e = strip(e)
    out = []
    # peel explicit casts to byte pointers / integers
    while e is not None and e.kind == CK.CSTYLE_CAST_EXPR:
        ct = e.type
        ch = list(e.get_children())
        inner = strip(ch[-1]) if ch else None
        if inner is None:
            break
        if pointee_record(ct) is not None and pointee_record(ct).kind in (TK.RECORD, TK.CONSTANTARRAY) and not is_bytep(ct):
            out.append((pointee_record(ct), 'cast', ct.get_pointee().spelling))
            return out
        e = inner
    t = e.type if e is not None else None
    if t is not None:
        p = pointee_record(t)
        if p is not None and p.kind in (TK.RECORD, TK.CONSTANTARRAY):
            out.append((p, 'type', t.get_pointee().spelling))
            return out
    # a variable of an untyped kind: the types it is cast to elsewhere in the function
    if e is not None and e.kind == CK.DECL_REF_EXPR:
        d = e.referenced
        if d is not None:
            for ty, sp in fn_info.get(d.hash, []):
                out.append((ty, 'castelsewhere', sp))
    elif e is not None and e.kind == CK.MEMBER_REF_EXPR:
        pass
    return out


def collect_casts(fn):
    """var hash -> set of record types the variable is cast to / assigned to in the function body"""
    info = {}
    seen = {}

    def add(var, ty, sp=''):
        if ty is None or ty.kind not in (TK.RECORD, TK.CONSTANTARRAY):
            return
        key = ty.spelling
        lst = info.setdefault(var.hash, [])
        if key not in seen.setdefault(var.hash, set()):
            seen[var.hash].add(key)
            lst.append((ty, sp))

    def walk(c):
        if c.kind == CK.CSTYLE_CAST_EXPR:
            ch = list(c.get_children())
            inner = strip(ch[-1]) if ch else None
            pr = pointee_record(c.type)
            if inner is not None and inner.kind == CK.DECL_REF_EXPR and pr is not None and not is_bytep(c.type):
                add(inner.referenced, pr, c.type.get_pointee().spelling)
        elif c.kind == CK.VAR_DECL:
            ch = [x for x in c.get_children() if x.kind not in (CK.TYPE_REF, CK.PARM_DECL)]
            if ch:
                inner = strip(ch[-1])
                pr = pointee_record(c.type)
                if inner is not None and inner.kind == CK.DECL_REF_EXPR and pr is not None and not is_bytep(c.type):
                    add(inner.referenced, pr, c.type.get_pointee().spelling)
        elif c.kind == CK.BINARY_OPERATOR:
            op, ch = binop(c)
            if op == '=' and len(ch) == 2:
                lhs, rhs = strip(ch[0]), strip(ch[1])
                if rhs is not None and rhs.kind == CK.DECL_REF_EXPR and lhs is not None:
                    pr = pointee_record(lhs.type)
                    if pr is not None and not is_bytep(lhs.type):
                        add(rhs.referenced, pr, lhs.type.get_pointee().spelling)
        for x in c.get_children():
            walk(x)
    walk(fn)
    return info


# ---------------------------------------------------------------------------------------------------------- scan one file
def deref_width(anc):
    """the width of the access and its kind: walks the ancestors (innermost last) of the offset expression.
    returns (kind, width, store) with kind 'deref' | 'addr' | 'plain'"""
    store = False
    kind = 'plain'
    width = 0
    dnode = None
    # look through parens / casts / unary * / member of cast, to the access
    i = len(anc) - 1
    cur_ptr_type = None
    while i >= 0:
        a = anc[i]
        if a.kind in (CK.PAREN_EXPR, CK.UNEXPOSED_EXPR):
            i -= 1
            continue
        if a.kind == CK.CSTYLE_CAST_EXPR:
            cur_ptr_type = a.type
            i -= 1
            continue
        break
    if i >= 0 and anc[i].kind == CK.UNARY_OPERATOR:
        t = tok(anc[i])
        if t and t[0] == '*':
            kind = 'deref'
            dnode = anc[i]
            if cur_ptr_type is not None and cur_ptr_type.get_canonical().kind == TK.POINTER:
                width = cur_ptr_type.get_canonical().get_pointee().get_size()
            # the deref result used as an lvalue?
            j = i - 1
            while j >= 0 and anc[j].kind in (CK.PAREN_EXPR, CK.UNEXPOSED_EXPR):
                j -= 1
            if j >= 0 and anc[j].kind in (CK.BINARY_OPERATOR, CK.COMPOUND_ASSIGNMENT_OPERATOR):
                op, ch = binop(anc[j])
                if op in ('=', '+=', '-=', '|=', '&=', '^=', '<<=', '>>=') and ch and strip(ch[0]) is not None:
                    # is anc[i] the lhs?
                    lhs = ch[0]
                    if lhs.extent.start.offset <= anc[i].extent.start.offset and anc[i].extent.end.offset <= lhs.extent.end.offset:
                        store = True
            if j >= 0 and anc[j].kind == CK.UNARY_OPERATOR:
                t2 = tok(anc[j])
                if t2 and (t2[0] in ('++', '--') or t2[-1] in ('++', '--')):
                    store = True
        elif t and t[0] == '&':
            kind = 'addr'
    elif i >= 0 and anc[i].kind == CK.MEMBER_REF_EXPR:
        kind = 'deref'
    elif i >= 0 and anc[i].kind in (CK.CALL_EXPR,):
        kind = 'arg'
    return kind, width, store, dnode


def under_ptr_cast(anc):
    i = len(anc) - 1
    while i >= 0 and anc[i].kind in (CK.PAREN_EXPR, CK.UNEXPOSED_EXPR):
        i -= 1
    return i >= 0 and anc[i].kind == CK.CSTYLE_CAST_EXPR and anc[i].type.get_canonical().kind == TK.POINTER


def src_slice(c):
    e = c.extent
    if not e.start.file or not e.end.file:
        return None
    return (e.start.file.name, e.start.offset, e.end.offset)


def outer_prefix(S, K):
    """the shortest member path of S whose first byte is DS offset K (the aggregate starting there)"""
    L = lay(S)
    for l in L.leaves:
        if l.dso == K:
            parts = re.findall(r'\.[A-Za-z_0-9]+|\[\d+\]', l.path)
            if ''.join(parts) != l.path:
                return None
            pre = ''
            for q in parts:
                pre += q
                st = min(x.dso for x in L.leaves if x.path.startswith(pre) and (len(x.path) == len(pre) or x.path[len(pre)] in '.['))
                if st == K:
                    return pre
            return None
    return None


def base_src(sb):
    """the source text of the pointer expression with byte casts peeled"""
    e = strip(sb)
    while e is not None and e.kind == CK.CSTYLE_CAST_EXPR:
        ct = e.type
        ch = list(e.get_children())
        if not (is_bytep(ct) or is_intlike(ct)) or not ch:
            break
        e = strip(ch[-1])
    if e is None:
        return None
    t = ''.join(tok(e))
    if '#' in t or not t:
        return None
    return t if re.fullmatch(r'[A-Za-z_][A-Za-z_0-9]*', t) else '(' + t + ')'


def make_fix(node, dnode, base, S2, sname, l, K2, kind, width, isarray):
    """(file, start, end, replacement, kind) that reaches the member by name, or None"""
    if isarray or not sname or K2 != l.dso or '(' in l.path or ' ' in l.path:
        return None
    B = base_src(base)
    if B is None:
        return None
    cast = '((%s *)%s)' % (sname, B)
    if kind in ('arg', 'plain', 'addr'):
        pre = outer_prefix(S2, K2)
        sl = src_slice(node)
        if pre is None or sl is None:
            return None
        return (sl[0], sl[1], sl[2], '&%s->%s' % (cast, pre[1:] if pre.startswith('.') else pre), 'addr')
    if kind == 'deref' and dnode is not None and width == l.dsz and not (l.ptr and width != 8):
        sl = src_slice(dnode)
        if sl is None:
            return None
        return (sl[0], sl[1], sl[2], '%s->%s' % (cast, l.path[1:] if l.path.startswith('.') else l.path), 'value')
    return None


def scan_file(args):
    path, cmd, cwd, mapp = args
    out = []
    try:
        init_lib()
        old = os.getcwd()
        os.chdir(cwd)
        a = shlex.split(cmd)
        argv = []
        i = 1
        while i < len(a):
            x = a[i]
            if x in SKIP_ARGS or x == '-o':
                i += 2
                continue
            if x in ('ccache', 'clang') or x.startswith('-fdiagnostics-color') or x == path or x.endswith(('.c',)) and not x.startswith('-'):
                i += 1
                continue
            if x.startswith(('-MD', '-MMD', '-fcolor')):
                i += 1
                continue
            if x == '-include-pch':
                # libclang cannot read the build's precompiled header: include its source header instead
                pre = next((y[2:] for y in a if y.startswith('-I') and y.endswith('port/include_hgss')), None)
                if pre:
                    argv += ['-include', os.path.join(os.path.dirname(pre), 'pch', os.path.basename(a[i + 1])[:-4])]
                i += 2
                continue
            if mapp:
                x = x.replace(mapp[0], mapp[1])
            argv.append(x)
            i += 1
        idx = ci.Index.create()
        tu = idx.parse(path, args=argv + ['-fsyntax-only', '-Wno-everything'],
                       options=ci.TranslationUnit.PARSE_SKIP_FUNCTION_BODIES * 0)
        os.chdir(old)
    except Exception as ex:
        return path, [], 'parse: %r' % ex
    errs = sum(1 for d in tu.diagnostics if d.severity >= 3)
    for fn in tu.cursor.get_children():
        if fn.kind != CK.FUNCTION_DECL or not fn.is_definition():
            continue
        if not fn.location.file or os.path.realpath(fn.location.file.name) != os.path.realpath(path):
            continue
        try:
            out += scan_fn(fn, path)
        except Exception as ex:
            out.append(dict(err='%s: %r' % (fn.spelling, ex)))
    return path, out, 'diag errors: %d' % errs


def scan_fn(fn, path):
    hits = []
    casts = None
    anc = []

    def lazy_casts():
        nonlocal casts
        if casts is None:
            casts = collect_casts(fn)
        return casts

    def emit(node, base, K, anc_here):
        kind, width, store, dnode = deref_width(anc_here)
        cands = base_types(base, {})
        if not cands:
            cands = base_types(base, lazy_casts())
        if not cands:
            hits.append(dict(file=path, line=node.location.line, fn=fn.spelling, K=K, S=None, kind=kind, store=store,
                             text=' '.join(tok(node))[:120], how='untyped'))
            return
        for S, how, sname in cands:
            if S.kind == TK.CONSTANTARRAY:
                # array of records: K walks the elements
                el = S.element_type.get_canonical()
                e = lay(el)
                if e.dsz <= 0:
                    continue
                idx, rem = divmod(K, e.dsz)
                S2, K2, ho_base = el, rem, idx * e.hsz
            else:
                S2, K2, ho_base = S, K, 0
            if lay(S2).dsz <= 0 and S2.kind == TK.RECORD:
                pass
            ls = host_of(S2, K2, width)
            if not ls:
                hits.append(dict(file=path, line=node.location.line, fn=fn.spelling, K=K, S=S2.spelling, kind=kind, store=store,
                                 text=' '.join(tok(node))[:120], how=how + '/nofield'))
                continue
            for l in ls:
                ho = ho_base + l.ho + (K2 - l.dso)
                hits.append(dict(file=path, line=node.location.line, fn=fn.spelling, K=K, S=S2.spelling, kind=kind, store=store,
                                 width=width, leaf=l.path, dso=l.dso, ho=ho, ptr=l.ptr, dsz=l.dsz, how=how,
                                 text=' '.join(tok(node))[:120], div=(ho != K), ncand=len(cands),
                                 fix=make_fix(node, dnode, base, S2, sname, l, K2, kind, width, S.kind == TK.CONSTANTARRAY)))

    def walk(c):
        anc.append(c)
        k = c.kind
        if k == CK.BINARY_OPERATOR:
            op, ch = binop(c)
            if op == '+' and len(ch) == 2:
                l, r = ch
                K = intlit(r)
                b = l
                if K is None:
                    K = intlit(l)
                    b = r
                if K is not None and K > 0:
                    sb = strip(b)
                    bt = sb.type if sb is not None else None
                    if bt is not None and is_bytep(bt):
                        emit(c, sb, K, anc[:-1])
                    elif bt is not None and is_intlike(bt) and under_ptr_cast(anc[:-1]):
                        # an integer holding an address: (T *)(addr + 0x18)
                        emit(c, sb, K, anc[:-1])
        elif k == CK.ARRAY_SUBSCRIPT_EXPR:
            ch = list(c.get_children())
            if len(ch) == 2:
                K = intlit(ch[1])
                sb = strip(ch[0])
                if K is not None and K > 0 and sb is not None and is_bytep(sb.type):
                    # the subscript result is the access; treat as deref of width = elem size
                    emit(c, sb, K, anc[:-1])
        for x in c.get_children():
            walk(x)
        anc.pop()

    walk(fn)
    return hits


# ---------------------------------------------------------------------------------------------------------- main
def rank(h):
    if h.get('how') == 'untyped':
        return 0
    s = 0
    if h.get('div'):
        s += 10
        if h.get('store'):
            s += 8
        elif h.get('kind') in ('addr', 'arg'):
            s += 5
        if h.get('ptr'):
            s += 4
        if h.get('how') == 'type':
            s += 1
    return s


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--cc', required=True)
    ap.add_argument('--map')
    ap.add_argument('--root', default='build/pokeheartgold')
    ap.add_argument('--match', default=r'src/overlay_07_|src/battle')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--noprefilter', action='store_true', help='parse every selected file (default: files with a "+ 0x" / "[0x" / "+ N)" in the text)')
    ap.add_argument('-j', type=int, default=6)
    ap.add_argument('--out', default='-')
    ap.add_argument('--fixes', help='write the mechanical rewrites (file, start, end, text) as JSON; apply with --apply')
    ap.add_argument('--apply', help='apply a --fixes JSON to the files it names, then stop')
    ap.add_argument('--limit', type=int, default=0)
    ap.add_argument('files', nargs='*')
    o = ap.parse_args()
    mapp = tuple(o.map.split('=', 1)) if o.map else None
    if o.apply:
        by = {}
        for f, a, b, t, kind in json.load(open(o.apply)):
            by.setdefault(f, []).append((a, b, t))
        n = 0
        for f, lst in by.items():
            data = open(f, 'rb').read()
            for a, b, t in sorted(set(lst), reverse=True):
                data = data[:a] + t.encode() + data[b:]
                n += 1
            open(f, 'wb').write(data)
        print('applied %d rewrites in %d files' % (n, len(by)))
        return
    cc = json.load(open(o.cc))
    root = os.path.abspath(o.root)
    jobs = []
    for e in cc:
        f = e['file']
        if mapp:
            f = f.replace(mapp[0], mapp[1])
        if not f.endswith('.c') or '/pokeheartgold/' not in f:
            continue
        if o.files:
            if not any(f.endswith(x) for x in o.files):
                continue
        elif not o.all and not re.search(o.match, f):
            continue
        if not os.path.exists(f):
            continue
        txt = open(f, errors='replace').read()
        if not re.search(r'\+ ?0x|\[0x|\+ ?\d+\)', txt) and not o.files and not o.noprefilter:
            continue
        jobs.append((f, e['command'], e['directory'], mapp))
    if o.limit:
        jobs = jobs[:o.limit]
    print('scanning %d files' % len(jobs), file=sys.stderr)
    res = []
    notes = []
    with cf.ProcessPoolExecutor(o.j) as ex:
        for p, hs, note in ex.map(scan_file, jobs, chunksize=4):
            res += hs
            if 'errors: 0' not in note:
                notes.append((p, note))
    res = [h for h in res if 'err' not in h]
    res.sort(key=lambda h: (-rank(h), h['file'], h['line']))
    w = sys.stdout if o.out == '-' else open(o.out, 'w')
    for h in res:
        if h.get('how') == 'untyped':
            continue
        rel = os.path.relpath(h['file'], root)
        w.write('%2d %s:%d %s K=0x%X %s -> %s %s ds=0x%X host=0x%X %s%s%s%s  | %s\n' % (
            rank(h), rel, h['line'], h['fn'], h['K'], h['S'], h.get('leaf', '?'), h['how'], h.get('dso', 0), h.get('ho', 0),
            h['kind'], ' STORE' if h['store'] else '', ' PTR' if h.get('ptr') else '',
            ' FIX' if h.get('fix') and h.get('div') else '', h['text']))
    if o.fixes:
        fx = {}
        for h in res:
            f = h.get('fix')
            if f and h.get('div') and h.get('ncand') == 1:
                fx[(f[0], f[1], f[2])] = f
        json.dump([list(v) for v in fx.values()], open(o.fixes, 'w'))
    un = [h for h in res if h.get('how') == 'untyped']
    w.write('# untyped: %d\n' % len(un))
    for h in un:
        w.write('U %s:%d %s K=0x%X %s%s | %s\n' % (os.path.relpath(h['file'], root), h['line'], h['fn'], h['K'], h['kind'],
                                                   ' STORE' if h['store'] else '', h['text']))
    n_div = sum(1 for h in res if h.get('div'))
    w.write('# files %d, offset expressions %d, typed %d, divergent %d, untyped %d, files with parse errors %d\n' % (
        len(jobs), len(res), len(res) - len(un), n_div, len(un), len(notes)))
    for p, n in notes[:20]:
        w.write('# %s %s\n' % (os.path.relpath(p, root), n))


if __name__ == '__main__':
    main()
