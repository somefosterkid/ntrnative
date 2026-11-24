#!/usr/bin/env python3
"""tools/hg_declscan.py: find functions of the HeartGold/SoulSilver port build that are DECLARED differently in different files.

The decomp style (and mwccarm's tolerance) re-declares functions locally: `void *sub_02015530(u8 x);` in one file while the
definition returns u8, `void f();` (K&R, no prototype) called with fewer arguments than the definition takes, a header that says
`int` where the body returns a pointer. On the DS every one of these works, the value is just r0. On the 64-bit host the callee
leaves garbage in the upper bits of rax / the unset argument registers, and the caller reads them (a table index, a pointer): a
crash that depends on what rdi/rax held (the Psybeam / Tail Glow crash of overlay_07_0223192C_02231C20.c).

The tool parses every HG/SS file of the port build with libclang (the port's compile line from a compile_commands.json), collects
every declaration of every external (non-static) function (file scope, in headers, and block-scope re-declarations in function
bodies) with its return/parameter ABI classes, and every call through an unprototyped declaration with its argument count and
classes. A function's TRUTH is its definition in C (when one exists), otherwise the declaration most files agree on. Each other
declaration is compared with it and ranked by how likely it is to crash on x86-64:

  5  a u8/u16/u32 definition read as a pointer, float vs int, a call through a K&R declaration (or a prototype) with fewer
     arguments than the definition takes
  4  a pointer returned through an int the caller truncates; struct by value of another size; a definition returning void read
     as a pointer/64-bit value; a return widened to 64 bits
  3  a definition returning void read as a 32-bit value; an int passed where a pointer is wanted
  2  narrower/wider integer width mismatches; signedness only (1)
  0  harmless differences (the declaration ignores a return value, extra arguments)

  python3 tools/hg_declscan.py --cc COMPILE_COMMANDS.json [-j N] [--facts FILE] [--out FILE] [--min-rank N] [--match REGEX] [FILES...]
  python3 tools/hg_declscan.py --from-facts FILE [--out FILE]   # re-rank saved facts without parsing

--cc: a build's compile_commands.json (build/port/linux/); the files are mapped from the build's checkout to this one
(--src-root). --facts saves what the parse found (JSON lines) so a ranking change does not re-parse ~10k files."""
import argparse, collections, concurrent.futures as cf, json, os, re, shlex, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LIB = '/usr/lib/llvm-18/lib/libclang-18.so.1'
SEEN = set()           # header declarations this worker already reported
SKIP_ARGS = {'-o', '-MF', '-MQ', '-MT', '-c'}


def init_lib():
    import clang.cindex as ci
    for p in (os.environ.get('LIBCLANG'), LIB, '/usr/lib/x86_64-linux-gnu/libclang-18.so.1'):
        if p and os.path.exists(p):
            try:
                ci.Config.set_library_file(p)
            except Exception:
                pass
            break
    return ci


# ---------------------------------------------------------------------------------------------------------- classes
def tcls(ci, t):
    """ABI class of a type on the host: 'v' void, 'p' pointer, 'i4s'/'i1u' integer (size + signedness), 'f4' / 'f8', 's16' struct."""
    TK = ci.TypeKind
    t = t.get_canonical()
    k = t.kind
    if k == TK.VOID:
        return 'v'
    if k in (TK.POINTER, TK.CONSTANTARRAY, TK.INCOMPLETEARRAY, TK.BLOCKPOINTER, TK.FUNCTIONPROTO, TK.FUNCTIONNOPROTO):
        return 'p'
    if k in (TK.FLOAT, TK.DOUBLE, TK.LONGDOUBLE):
        return 'f%d' % t.get_size()
    if k == TK.RECORD:
        return 's%d' % t.get_size()
    if k == TK.BOOL:
        return 'i1u'
    if k == TK.ENUM:
        return 'i4s'
    if k in (TK.CHAR_S, TK.SCHAR, TK.SHORT, TK.INT, TK.LONG, TK.LONGLONG, TK.INT128):
        return 'i%ds' % t.get_size()
    if k in (TK.CHAR_U, TK.UCHAR, TK.USHORT, TK.UINT, TK.ULONG, TK.ULONGLONG, TK.UINT128, TK.WCHAR):
        return 'i%du' % t.get_size()
    sz = t.get_size()
    return 'i%ds' % sz if sz > 0 else '?'


def csize(c):
    return 8 if c == 'p' else (int(re.match(r'[ifs](\d+)', c).group(1)) if re.match(r'[ifs]\d', c) else 0)


# ------------------------------------------------------------------------------------------------------------- parse
def argv_for(cmd, path, mapp):
    a = shlex.split(cmd)
    argv = []
    i = 1
    while i < len(a):
        x = a[i]
        if x in SKIP_ARGS:
            i += 2
            continue
        if x in ('ccache', 'clang') or x.startswith(('-fdiagnostics-color', '-MD', '-MMD', '-fcolor')) or (x.endswith('.c') and not x.startswith('-')):
            i += 1
            continue
        if x == '-include-pch':
            i += 2
            continue
        if mapp:
            x = x.replace(mapp[0], mapp[1])
        argv.append(x)
        i += 1
    return argv


def pch_args(ci, idx, argv):
    """the forced prefix (port_hgss.h: SDL2 + libc, 75k lines) parses once per distinct command line into a precompiled AST that
    libclang's own -include-pch reads (the build's PCH is clang 21's); every file then parses in ~0.05 s."""
    import hashlib, tempfile
    if '-include' not in argv or 'port_hgss.h' not in argv:
        return argv
    key = hashlib.sha1(' '.join(argv).encode()).hexdigest()[:12]
    pch = os.path.join(tempfile.gettempdir(), 'hg_declscan_%d_%s.pch' % (os.getuid(), key))
    rest = []
    i = 0
    while i < len(argv):
        if argv[i] == '-include' and argv[i + 1] == 'port_hgss.h':
            i += 2
            continue
        rest.append(argv[i])
        i += 1
    if not os.path.exists(pch):
        empty = pch + '.c'
        open(empty, 'w').close()
        tu = idx.parse(empty, args=argv)
        tmp = '%s.%d' % (pch, os.getpid())
        tu.save(tmp)
        os.replace(tmp, pch)
    return rest + ['-include-pch', pch]


def scan_file(job):
    path, cmd, cwd, mapp, root = job
    ci = init_lib()
    CK = ci.CursorKind
    LK = ci.LinkageKind
    decls, calls, errs = [], [], 0
    old = os.getcwd()
    try:
        os.chdir(cwd if os.path.isdir(cwd) else root)
        idx = ci.Index.create()
        argv = argv_for(cmd, path, mapp) + ['-fsyntax-only', '-Wno-everything', '-Werror=implicit-function-declaration']
        tu = idx.parse(path, args=pch_args(ci, idx, argv))
    except Exception as ex:
        os.chdir(old)
        return path, [], [], ['parse failed: %s: %r' % (os.path.relpath(path, root), ex)], {}
    os.chdir(old)
    impl = []
    for d in tu.diagnostics:
        if d.severity >= 3:
            errs += 1
            if 'implicit declaration' in d.spelling:
                impl.append('%s:%d %s' % (os.path.relpath(d.location.file.name, root) if d.location.file else '?', d.location.line, d.spelling))

    def rel(f):
        return os.path.relpath(f.name, root) if f else '?'

    def decl(c):
        if c.linkage == LK.INTERNAL:
            return
        t = c.type
        proto = t.kind == ci.TypeKind.FUNCTIONPROTO
        pa = [tcls(ci, x) for x in t.argument_types()] if proto else []
        decls.append((c.spelling, tcls(ci, c.result_type), proto, pa, bool(proto and t.is_function_variadic()),
                      c.is_definition(), rel(c.location.file), c.location.line, c.result_type.spelling,
                      [x.spelling for x in t.argument_types()] if proto else []))

    refs = {}

    def walk(c, disc=False):
        k = c.kind
        if k == CK.FUNCTION_DECL:
            decl(c)
            if c.is_definition():
                for x in c.get_children():
                    walk(x)
            return
        if k == CK.CALL_EXPR:
            ref = c.referenced
            if ref is not None and ref.kind == CK.FUNCTION_DECL and ref.linkage != LK.INTERNAL:
                r = refs.setdefault((ref.spelling, rel(ref.location.file), ref.location.line), [0, 0])
                r[0] += 1
                r[1] += 0 if disc else 1
                if ref.type.kind == ci.TypeKind.FUNCTIONNOPROTO:
                    args = list(c.get_arguments())
                    calls.append((ref.spelling, len(args), [tcls(ci, a.type) for a in args], rel(c.location.file), c.location.line))
        ch = list(c.get_children())
        n = len(ch)
        for i, x in enumerate(ch):
            d = False                    # the value of this child is discarded (a statement)
            if k == CK.COMPOUND_STMT:
                d = True
            elif k in (CK.IF_STMT, CK.WHILE_STMT, CK.SWITCH_STMT):
                d = i >= 1
            elif k == CK.FOR_STMT:
                d = i == n - 1
            elif k == CK.DO_STMT:
                d = i == 0
            elif k in (CK.CASE_STMT, CK.DEFAULT_STMT, CK.LABEL_STMT):
                d = i == n - 1
            elif k == CK.CSTYLE_CAST_EXPR and c.type.kind == ci.TypeKind.VOID:
                d = True
            walk(x, d)

    mainf = os.path.abspath(path)
    for c in tu.cursor.get_children():
        f = c.location.file
        inmain = f is not None and os.path.abspath(f.name) == mainf
        if c.kind == CK.FUNCTION_DECL and not inmain:
            key = (c.spelling, f.name if f else '', c.location.line)
            if key not in SEEN:
                SEEN.add(key)
                decl(c)
        elif inmain:
            walk(c)
    return path, decls, calls, impl, refs


# ----------------------------------------------------------------------------------------------------------- ranking
def ret_rank(T, D):
    """T: the truth's return class, D: the declaration's. How badly does a caller that believes D misread T's result."""
    if T == D:
        return 0, ''
    if D == 'v':
        return 0, 'decl ignores the result'
    if T == 'v':
        return (4, 'definition returns void, declared to return a pointer/64-bit value') if csize(D) == 8 else \
               (3, 'definition returns void, declared to return a value')
    if T[0] == 'f' or D[0] == 'f':
        return 5, 'float class vs %s' % ('int/pointer' if T[0] != D[0] else 'float of another width')
    if T[0] == 's' or D[0] == 's':
        return 4, 'struct by value of another class/size'
    if T == 'p':
        return (4, 'pointer returned, declared as a %d-byte int (truncated)' % csize(D)) if csize(D) < 8 else (1, 'pointer returned, declared as a 64-bit int')
    if D == 'p':
        return (5, 'a %d-byte int returned, declared as a pointer (upper bits are garbage)' % csize(T)) if csize(T) < 8 else (1, '64-bit int returned, declared as a pointer')
    ts, ds = csize(T), csize(D)
    if ds > ts:
        return (4 if ds == 8 else 2), 'return widened %d -> %d bytes' % (ts, ds)
    if ds < ts:
        return 2, 'return narrowed %d -> %d bytes' % (ts, ds)
    return 1, 'signedness differs'


def par_rank(T, D):
    """the parameter class T in the definition, D in the declaration the caller used."""
    if T == D:
        return 0, ''
    if T == 'v' or D == 'v':
        return 3, 'void parameter'
    if T[0] == 'f' or D[0] == 'f':
        return 5, 'float class vs %s parameter' % ('int/pointer' if T[0] != D[0] else 'float of another width')
    if T[0] == 's' or D[0] == 's':
        return 4, 'struct by value of another class/size'
    if T == 'p':
        return 3, 'pointer wanted, an int is passed (%d bytes)' % csize(D)
    if D == 'p':
        return 1, 'int wanted, a pointer is passed'
    ts, ds = csize(T), csize(D)
    if ts > ds:
        return 2, 'parameter wider in the definition (%d vs %d bytes)' % (ts, ds)
    if ts < ds:
        return 0, ''
    return 1, 'signedness differs'


def sig(d):
    n, r, proto, pa, va, isdef, f, l, rs, ps = d
    return '%s(%s%s)' % (rs, ', '.join(ps), ', ...' if va else '') if proto else '%s()K&R' % rs


def analyse(decls, calls, refs):
    by = collections.defaultdict(dict)       # name -> (file, line) -> decl tuple
    for d in decls:
        by[d[0]][(d[6], d[7])] = d
    cby = collections.defaultdict(list)
    for c in calls:
        cby[c[0]].append(c)
    hits = []
    nsym = 0
    for name, dd in sorted(by.items()):
        nsym += 1
        dl = list(dd.values())
        sigs = collections.Counter((d[1], d[2], tuple(d[3]), d[4]) for d in dl)
        if len(sigs) < 2 and not cby.get(name):
            continue
        defs = [d for d in dl if d[5]]
        if defs:
            T = defs[0]
            why = 'definition'
        else:
            # no C definition: the prototype most declarations agree on
            pool = [d for d in dl if d[2]] or dl
            cnt = collections.Counter((d[1], tuple(d[3]), d[4]) for d in pool)
            best = cnt.most_common(1)[0][0]
            T = next(d for d in pool if (d[1], tuple(d[3]), d[4]) == best)
            why = 'consensus'
        for d in dl:
            if (d[1], d[2], tuple(d[3]), d[4]) == (T[1], T[2], tuple(T[3]), T[4]):
                continue
            n, used = refs.get((name, d[6], d[7]), (0, 0))
            rr, nr = ret_rank(T[1], d[1])         # what a caller reading the result gets
            rp, npn = 0, ''                       # what the callee gets as arguments
            if d[2] and T[2]:
                if len(d[3]) < len(T[3]):
                    rp, npn = 5, 'declaration has %d parameters, %s has %d (missing arguments)' % (len(d[3]), why, len(T[3]))
                elif len(d[3]) > len(T[3]):
                    rp, npn = 1, 'declaration has %d parameters, %s has %d (extra)' % (len(d[3]), why, len(T[3]))
                else:
                    for i, (a, b) in enumerate(zip(T[3], d[3])):
                        prk, pn = par_rank(a, b)
                        if prk > rp:
                            rp, npn = prk, 'parameter %d: %s' % (i, pn)
            elif not d[2] and T[2]:
                rp, npn = 1, 'K&R declaration of a prototyped function'
            note = []
            if rr and used == 0:
                note.append('result never read through this declaration (%s)' % nr)
                rr = 0
            elif rr:
                note.append(nr)
            if rp and n == 0:
                note.append('no call resolves to this declaration (%s)' % npn)
                rp = 0
            elif rp:
                note.append(npn)
            rk = max(rr, rp)
            if note:
                hits.append((rk, name, why, T, d, '; '.join(note), n, used))
        for (n, na, acl, f, l) in cby.get(name, ()):
            if not T[2]:
                continue
            tp = T[3]
            if na < len(tp):
                hits.append((5, name, why, T, None, 'call at %s:%d passes %d arguments, %s takes %d (missing arguments)' % (f, l, na, why, len(tp)), 1, 1))
            else:
                worst = (0, '')
                for a, b in zip(tp, acl):
                    if b[0] == 'f':
                        b = 'f8'
                    r = par_rank(a, b)
                    if r[0] > worst[0]:
                        worst = r
                if worst[0] >= 2:
                    hits.append((worst[0], name, why, T, None, 'call at %s:%d: %s' % (f, l, worst[1]), 1, 1))
        if not T[2] and cby.get(name):
            cnts = collections.Counter(c[1] for c in cby[name])
            if len(cnts) > 1:
                hits.append((2, name, why, T, None, 'K&R calls with differing argument counts: ' + ', '.join('%d args x%d' % (k, v) for k, v in sorted(cnts.items())), 1, 1))
    return hits, nsym


def report(decls, calls, impl, refs, out, min_rank, nfiles):
    hits, nsym = analyse(decls, calls, refs)
    hits.sort(key=lambda h: (-h[0], h[1]))
    w = sys.stdout if out == '-' else open(out, 'w')
    for rk, name, why, T, d, note, nc, nu in hits:
        if rk < min_rank:
            continue
        loc = '%s:%d' % (d[6], d[7]) if d else '(call)'
        w.write('%d %s: %s | %s %s:%d %s | vs %s %s | calls %d, result used %d\n' % (rk, name, note, why, T[6], T[7], sig(T), loc, sig(d) if d else '', nc, nu))
    r = collections.Counter(h[0] for h in hits)
    perr = [x for x in impl if x.startswith('parse failed')]
    impl = [x for x in impl if not x.startswith('parse failed')]
    w.write('# files %d (%d did not parse), symbols %d, implicit-declaration errors %d, hits %d (rank 5: %d, 4: %d, 3: %d, 2: %d, 1: %d, 0: %d), distinct symbols with hits %d\n' % (
        nfiles, len(perr), nsym, len(impl), len(hits), r[5], r[4], r[3], r[2], r[1], r[0], len({h[1] for h in hits})))
    for x in impl[:50]:
        w.write('# implicit: %s\n' % x)
    for x in perr[:60]:
        w.write('# %s\n' % x)
    return hits


# -------------------------------------------------------------------------------------------------------- emit-fixes
FIX_ARGS = ['-Werror=int-conversion', '-Werror=incompatible-pointer-types', '-Werror=int-to-pointer-cast',
            '-Werror=incompatible-function-pointer-types', '-Werror=implicit-function-declaration']
# what may stand between the start of a statement and a call for the call to be the first thing the function does
PREFIX_OK = re.compile(r'^(?:return\s+|\(void\)\s*|(?:if|switch)\s*\(\s*!?\s*|(?:[A-Za-z_][\w\s\*]*?[\s\*])?\*?[A-Za-z_]\w*\s*=\s*(?:\([\w\s\*]+\)\s*)?)?$')
_FIXER = {}


class Fixer:
    """one process's view of the build: parses a file (optionally with edited text) and tells how a def looks."""

    def __init__(self, cc_path, src_root, match):
        self.ci = init_lib()
        self.CK = self.ci.CursorKind
        cc = json.load(open(cc_path))
        self.root = os.path.abspath(src_root)
        self.old_root = os.path.dirname(os.path.dirname(os.path.dirname(cc[0]['directory'])))
        self.mapp = (self.old_root, self.root) if self.old_root != self.root else None
        self.cmd = {}
        for e in cc:
            f = e['file'] if os.path.isabs(e['file']) else os.path.normpath(os.path.join(e['directory'], e['file']))
            if self.mapp:
                f = f.replace(self.mapp[0], self.mapp[1])
            if f.endswith('.c') and re.search(match, f) and os.path.exists(f):
                self.cmd[os.path.relpath(f, self.root)] = (e['command'], e['directory'].replace(self.old_root, self.root))
        self.idx = self.ci.Index.create()
        self.defs = {}

    def text(self, rel):
        return open(os.path.join(self.root, rel), 'rb').read().decode('latin-1')

    def parse(self, rel, text=None):
        cmd, cwd = self.cmd[rel]
        path = os.path.join(self.root, rel)
        old = os.getcwd()
        os.chdir(cwd if os.path.isdir(cwd) else self.root)
        try:
            argv = argv_for(cmd, path, self.mapp) + ['-fsyntax-only', '-Wno-everything'] + FIX_ARGS
            argv = pch_args(self.ci, self.idx, argv)
            un = [(path, text)] if text is not None else None
            return self.idx.parse(path, args=argv, unsaved_files=un)
        finally:
            os.chdir(old)

    @staticmethod
    def nerr(tu):
        return sum(1 for d in tu.diagnostics if d.severity >= 3)

    def defsig(self, rel, name, line):
        """the definition of name at rel:line: prototype text, return-type text, parameter classes/names, parameters the body reads"""
        key = (rel, name, line)
        if key in self.defs:
            return self.defs[key]
        res = None
        if rel in self.cmd:
            text = self.text(rel)
            tu = self.parse(rel)
            CK = self.CK
            for c in tu.cursor.get_children():
                if c.kind == CK.FUNCTION_DECL and c.spelling == name and c.is_definition() and c.location.line == line \
                        and c.location.file and os.path.abspath(c.location.file.name) == os.path.join(self.root, rel):
                    body = next((x for x in c.get_children() if x.kind == CK.COMPOUND_STMT), None)
                    if body is None:
                        break
                    pro = text[c.extent.start.offset:body.extent.start.offset].strip()
                    nt = next((t for t in c.get_tokens() if t.spelling == name), None)
                    ret = text[c.extent.start.offset:nt.extent.start.offset].strip() if nt else ''
                    params = list(c.get_arguments())
                    names = [p.spelling for p in params]
                    used = set()
                    for x in body.walk_preorder():
                        if x.kind == CK.DECL_REF_EXPR and x.referenced is not None and x.referenced.kind == CK.PARM_DECL \
                                and x.referenced.spelling in names:
                            used.add(names.index(x.referenced.spelling))
                    res = dict(proto=re.sub(r'\s+', ' ', pro), ret=re.sub(r'\s+', ' ', ret), names=names,
                               classes=[tcls(self.ci, p.type) for p in params], used=used, nparams=len(params),
                               bad=bool(re.search(r'\b(static|inline|__attribute__|asm)\b', pro)))
                    break
        self.defs[key] = res
        return res


def apply_edits(text, edits):
    for s, e, new in sorted(edits, key=lambda x: -x[0]):
        text = text[:s] + new + text[e:]
    return text


def call_edit(F, tu, text, mainpath, name, line, dsig):
    """((pos, pos, args), why): the caller's own leading parameters, for an argument-less K&R call that is the first thing
    the caller does (r0..r3 still hold the caller's own arguments there, on the DS and so in the C)"""
    CK = F.CK
    k = dsig['nparams']
    for fn in tu.cursor.get_children():
        if fn.kind != CK.FUNCTION_DECL or not fn.is_definition() or not fn.location.file or os.path.abspath(fn.location.file.name) != mainpath:
            continue
        if not (fn.extent.start.line <= line <= fn.extent.end.line):
            continue
        body = next((x for x in fn.get_children() if x.kind == CK.COMPOUND_STMT), None)
        if body is None:
            return None, 'no body'
        call = None
        for x in body.walk_preorder():
            if x.kind == CK.CALL_EXPR and x.spelling == name and x.location.line == line and not list(x.get_arguments()):
                call = x
                break
        if call is None:
            return None, 'call not found'
        first = None
        for s in body.get_children():
            if s.kind == CK.DECL_STMT and not any(y.kind.is_expression() for v in s.get_children() for y in v.get_children()):
                continue
            first = s
            break
        if first is None or not (first.extent.start.offset <= call.extent.start.offset and call.extent.end.offset <= first.extent.end.offset):
            return None, 'not in the first statement'
        pre = re.sub(r'\s+', ' ', text[first.extent.start.offset:call.extent.start.offset]).strip()
        if not PREFIX_OK.match(pre):
            return None, 'something runs before the call (%s)' % pre[:40]
        params = list(fn.get_arguments())
        if len(params) < k:
            return None, 'caller has %d parameters, callee %d' % (len(params), k)
        pn = [p.spelling for p in params[:k]]
        if not all(pn):
            return None, 'unnamed caller parameter'
        pc = [tcls(F.ci, p.type) for p in params[:k]]
        for i, (a, b) in enumerate(zip(dsig['classes'], pc)):
            if (a == 'p') != (b == 'p') or a[0] in 'fs' or b[0] in 'fs' or a == 'v':
                return None, 'parameter %d class differs (%s vs %s)' % (i, a, b)
        end = call.extent.end.offset
        if text[end - 1] != ')':
            return None, 'call text'
        return (end - 1, end - 1, ', '.join(pn)), 'ok'
    return None, 'caller not found'


def decl_cands(F, tu, text, mainpath, name, line):
    """(start, end, original text, text of the declaration from its name on) of the declaration of name at line"""
    CK = F.CK
    for c in tu.cursor.get_children():
        if c.kind == CK.FUNCTION_DECL and c.spelling == name and not c.is_definition() and c.location.line == line \
                and c.location.file and os.path.abspath(c.location.file.name) == mainpath:
            s, e = c.extent.start.offset, c.extent.end.offset
            orig = text[s:e]
            nt = next((t for t in c.get_tokens() if t.spelling == name), None)
            if nt is None or re.search(r'\b(static|inline|extern)\b', orig):
                return None
            return s, e, orig, text[nt.extent.start.offset:e]
    return None


def fix_file(job):
    """worker: every decl / call task of one file -> (edits that verified, [report rows])"""
    cc_path, src_root, match, rel, dtasks, ctasks = job
    F = _FIXER.get('F')
    if F is None:
        F = _FIXER['F'] = Fixer(cc_path, src_root, match)
    rows = []
    try:
        if rel not in F.cmd:
            return rel, [], [('listed', t[0], rel, t[1], 'file not in the build') for t in dtasks + ctasks]
        text = F.text(rel)
        if not text.isascii():
            return rel, [], [('listed', t[0], rel, t[1], 'non-ASCII source') for t in dtasks + ctasks]
        mainpath = os.path.join(F.root, rel)
        tu = F.parse(rel)
        base = F.nerr(tu)
        applied = []
        by = collections.defaultdict(lambda: {'d': [], 'c': []})
        for t in dtasks:
            by[t[0]]['d'].append(t)
        for t in ctasks:
            by[t[0]]['c'].append(t)
        for name, g in by.items():
            anyt = (g['d'] or g['c'])[0]
            dsig = F.defsig(anyt[4], name, anyt[5])
            if dsig is None or dsig['bad']:
                rows += [('listed', name, rel, t[1], 'definition not parseable here') for t in g['d'] + g['c']]
                continue
            need = max(dsig['used']) + 1 if dsig['used'] else 0
            for t in g['c']:
                if t[2] >= need:
                    rows.append(('benign', name, rel, t[1], 'the definition never reads the missing parameters'))
                    continue
                ed, why = call_edit(F, tu, text, mainpath, name, t[1], dsig)
                if ed is None:
                    rows.append(('listed', name, rel, t[1], 'argument-less call: ' + why))
                elif F.nerr(F.parse(rel, apply_edits(text, applied + [ed]))) <= base:
                    applied.append(ed)
                    rows.append(('fixed-call', name, rel, t[1], 'passes the caller parameters (%s)' % ed[2]))
                else:
                    rows.append(('listed', name, rel, t[1], 'argument-less call: adding the parameters does not compile'))
            for t in g['d']:
                dc = decl_cands(F, tu, text, mainpath, name, t[1])
                if dc is None:
                    rows.append(('listed', name, rel, t[1], 'declaration not found'))
                    continue
                s, e, orig, tail = dc
                ret = dsig['ret'].rstrip()
                retonly = ret + ('' if ret.endswith('*') else ' ') + tail
                done = False
                for kind, new in (('proto', dsig['proto']), ('ret', retonly)):
                    if re.sub(r'\s+', '', new) == re.sub(r'\s+', '', orig):
                        continue
                    if F.nerr(F.parse(rel, apply_edits(text, applied + [(s, e, new)]))) <= base:
                        applied.append((s, e, new))
                        rows.append(('fixed-decl-' + kind, name, rel, t[1], '%s  ->  %s' % (re.sub(r'\s+', ' ', orig), new)))
                        done = True
                        break
                if not done:
                    rows.append(('listed', name, rel, t[1], 'declaration: neither the definition prototype nor its return type compiles here (' + t[6][:90] + ')'))
        return rel, applied, rows
    except Exception as ex:
        return rel, [], [('listed', t[0], rel, t[1], 'tool error %r' % ex) for t in dtasks + ctasks]


def emit_fixes(o, decls, calls, refs):
    hits, _ = analyse(decls, calls, refs)
    dtasks = collections.defaultdict(list)     # file -> (name, line, ndeclparams, -, deffile, defline, note)
    ctasks = collections.defaultdict(list)     # file -> (name, line, nargs, -, deffile, defline, '')
    listed = []
    seen = set()
    for rk, name, why, T, d, note, n, used in hits:
        if rk < 4:
            continue
        if why != 'definition':
            listed.append(('listed', name, d[6] if d else '?', d[7] if d else 0, 'no C definition (consensus truth): ' + note))
            continue
        if d is None:
            m = re.match(r'call at (\S+):(\d+) passes (\d+) arguments', note)
            if m and (m.group(1), int(m.group(2))) not in seen:
                seen.add((m.group(1), int(m.group(2))))
                ctasks[m.group(1)].append((name, int(m.group(2)), int(m.group(3)), None, T[6], T[7], ''))
            continue
        if d[6].endswith('.h'):
            listed.append(('listed', name, d[6], d[7], 'header declaration: ' + note))
            continue
        if d[5]:
            listed.append(('listed', name, d[6], d[7], 'a second definition: ' + note))
            continue
        if (d[6], d[7], name) in seen:
            continue
        seen.add((d[6], d[7], name))
        dtasks[d[6]].append((name, d[7], len(d[3]), None, T[6], T[7], note))
    files = sorted(set(dtasks) | set(ctasks))
    jobs = [(o.cc, o.src_root, o.match, f, dtasks[f], ctasks[f]) for f in files]
    allrows = list(listed)
    allfix = {}
    print('emit-fixes: %d files, %d declaration tasks, %d call tasks' % (len(files), sum(map(len, dtasks.values())), sum(map(len, ctasks.values()))), file=sys.stderr)
    with cf.ProcessPoolExecutor(o.j) as ex:
        for rel, edits, rows in ex.map(fix_file, jobs, chunksize=4):
            allrows += rows
            if edits:
                allfix[rel] = edits
    return allfix, allrows


def write_fixes(o, allfix, allrows):
    import difflib
    out = o.emit_fixes
    os.makedirs(out, exist_ok=True)
    root = os.path.abspath(o.src_root)
    pre = 'build/pokeheartgold/'
    pdiff, ddiff = [], []
    for rel in sorted(allfix):
        text = open(os.path.join(root, rel), 'rb').read().decode('latin-1')
        new = apply_edits(text, allfix[rel])
        p = rel[len(pre):] if rel.startswith(pre) else rel
        pdiff += list(difflib.unified_diff(text.splitlines(True), new.splitlines(True), 'a/' + p, 'b/' + p, n=3))
        # the decomp form: the DS declaration stays, the host form goes under SDK_PORT / NONMATCHING; a call that gains its
        # arguments is the same code on the DS (r0..r3 there), so it is written as is
        body = text
        for s, e, repl in sorted(allfix[rel], key=lambda x: -x[0]):
            if s == e:
                body = body[:s] + repl + body[e:]
            else:
                body = body[:s] + '#if defined(SDK_PORT) || defined(NONMATCHING)\n' + repl + '\n#else\n' + body[s:e] + '\n#endif' + body[e:]
        ddiff += list(difflib.unified_diff(text.splitlines(True), body.splitlines(True), 'a/' + p, 'b/' + p, n=3))

    def git(lines):
        res = []
        for ln in lines:
            if ln.startswith('--- a/'):
                res.append('diff --git %s b/%s\n' % (ln[4:].strip(), ln[6:].strip()))
            res.append(ln if ln.endswith('\n') else ln + '\n')
        return ''.join(res)
    open(os.path.join(out, 'port.patch'), 'wb').write(git(pdiff).encode('latin-1'))
    open(os.path.join(out, 'decomp.patch'), 'wb').write(git(ddiff).encode('latin-1'))
    cnt = collections.Counter(r[0] for r in allrows)
    with open(os.path.join(out, 'rows.txt'), 'w') as w:
        for r in sorted(allrows):
            w.write('%s\t%s\t%s:%s\t%s\n' % r)
    print('files edited %d; %s' % (len(allfix), dict(cnt)), file=sys.stderr)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--cc')
    ap.add_argument('--src-root', default=ROOT, help='this checkout (the build dir of the compile_commands.json may be another one)')
    ap.add_argument('--match', default=r'/build/pokeheartgold/')
    ap.add_argument('--from-facts')
    ap.add_argument('--facts')
    ap.add_argument('--min-rank', type=int, default=1)
    ap.add_argument('-j', type=int, default=8)
    ap.add_argument('--out', default='-')
    ap.add_argument('--limit', type=int, default=0)
    ap.add_argument('--emit-fixes', metavar='DIR', help='with --from-facts and --cc: write DIR/port.patch (the fixes that verify), DIR/decomp.patch, DIR/rows.txt')
    ap.add_argument('files', nargs='*')
    o = ap.parse_args()
    decls, calls, impl, nfiles = [], [], [], 0
    refs = collections.defaultdict(lambda: [0, 0])
    if o.from_facts:
        for ln in open(o.from_facts):
            k, v = json.loads(ln)
            if k == 'd':
                decls.append(tuple(v))
            elif k == 'c':
                calls.append(tuple(v))
            elif k == 'i':
                impl.append(v)
            elif k == 'r':
                refs[(v[0], v[1], v[2])] = [v[3], v[4]]
            elif k == 'n':
                nfiles = v
    else:
        cc = json.load(open(o.cc))
        root = os.path.abspath(o.src_root)
        # the checkout the build dir belongs to: ".../build/port/linux" -> three levels up
        old_root = os.path.dirname(os.path.dirname(os.path.dirname(cc[0]['directory'])))
        mapp = (old_root, root) if old_root != root else None
        jobs = []
        for e in cc:
            f = e['file'] if os.path.isabs(e['file']) else os.path.normpath(os.path.join(e['directory'], e['file']))
            if mapp:
                f = f.replace(mapp[0], mapp[1])
            if not f.endswith('.c') or not re.search(o.match, f):
                continue
            if o.files and not any(f.endswith(x) for x in o.files):
                continue
            if not os.path.exists(f):
                continue
            jobs.append((f, e['command'], e['directory'].replace(old_root, root), mapp, root))
        if o.limit:
            jobs = jobs[:o.limit]
        nfiles = len(jobs)
        print('scanning %d files' % nfiles, file=sys.stderr)
        seen = set()
        with cf.ProcessPoolExecutor(o.j) as ex:
            for i, (p, ds, cs, im, rf) in enumerate(ex.map(scan_file, jobs, chunksize=8)):
                for d in ds:
                    k = (d[0], d[6], d[7])
                    if k not in seen:
                        seen.add(k)
                        decls.append(tuple(d))
                calls += [tuple(c) for c in cs]
                impl += im
                for k2, v2 in rf.items():
                    refs[k2][0] += v2[0]
                    refs[k2][1] += v2[1]
                if i % 1000 == 999:
                    print('  %d files' % (i + 1), file=sys.stderr)
        if o.facts:
            with open(o.facts, 'w') as w:
                w.write(json.dumps(['n', nfiles]) + '\n')
                for d in decls:
                    w.write(json.dumps(['d', d]) + '\n')
                for c in calls:
                    w.write(json.dumps(['c', c]) + '\n')
                for x in impl:
                    w.write(json.dumps(['i', x]) + '\n')
                for k2, v2 in refs.items():
                    w.write(json.dumps(['r', list(k2) + v2]) + '\n')
    report(decls, calls, impl, refs, o.out, o.min_rank, nfiles)
    if o.emit_fixes:
        allfix, allrows = emit_fixes(o, decls, calls, refs)
        write_fixes(o, allfix, allrows)


if __name__ == '__main__':
    main()
