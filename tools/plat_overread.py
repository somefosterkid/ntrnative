#!/usr/bin/env python3
"""plat_overread: loops / indexes that read past the end of a table (the BerryPatches_Init bug class), for Platinum.

Adapted from admin-79's HG/SS scanner (tools/hg/overread.py, hk300), credit to it: the lexical scanner is theirs, ours (GPL-3.0)
changes the paths, the defaults and the ROM lookup (--follow) for Platinum, whose tables are C arrays (no asm data labels).

BerryPatches_Init loops `i < 128` over a 118-entry table: on the DS the memory after the table is whatever the linker put next
in the module, on the PC whatever .rodata the linker put next. This scanner (lexical, no clang; parse once, cache per file)
finds, per use `T[sub]`:
  - a loop whose counter bound (constant / macro / enum / NELEMS(other) / sizeof(a)/sizeof(a[0])) makes max(sub) >= count,
  - a constant index >= count, `T[i - c]` with i starting below c, and `T[x - 1]` with no visible x == 0 guard.
Tables: C arrays (static / global / function-local; explicit dim or initializer count), extern decls (explicit dim, or the
definition behind them). Classes: REAL (bound and table size proven, no exit / guard in the loop), SUSP (guarded / early exit /
ambiguous / x-1 without a visible guard). Ranked REAL first, then by overshoot.
Default root: the PRISTINE pret tree (third_party/pokeplatinum): the port's patches (pokeplatinum-saveinit.patch) fix hits and
would hide them; gen/ headers come from build/pokeplatinum (macros only).

  python3 tools/plat_overread.py [--root DIR] [--jobs N] [--tsv OUT] [--all] [--no-cache] [FILES...]
  python3 tools/plat_overread.py --follow NAME   (where the table is in the ROM and the bytes after it: config/plat_us/symbols.tsv,
                                                  extract/plat_us module images)
"""
import os, re, sys, bisect, pickle, argparse, time
from multiprocessing import Pool

CACHE = os.environ.get('OVERREAD_CACHE', '/tmp/plat_overread/cache.pkl')
PRIM = {'u8': 1, 's8': 1, 'char': 1, 'bool8': 1, 'BOOL8': 1, 'u16': 2, 's16': 2, 'short': 2, 'fx16': 2, 'u32': 4, 's32': 4,
        'int': 4, 'BOOL': 4, 'fx32': 4, 'f32': 4, 'float': 4, 'long': 4, 'u64': 8, 's64': 8, 'double': 8, 'vu8': 1, 'vu16': 2,
        'vu32': 4, 'vs8': 1, 'vs16': 2, 'vs32': 4, 'size_t': 4, 'wchar_t': 2, 'HeapID': 4, 'ecword_t': 2, 'u_int': 4}
QUAL = {'static', 'const', 'extern', 'volatile', 'register', 'signed', 'unsigned', 'struct', 'union', 'enum', 'inline', 'auto'}
KW = {'return', 'else', 'sizeof', 'case', 'do', 'goto', 'if', 'while', 'for', 'switch'}
NOTYPE = KW | {'break', 'continue', 'default'}
CMP = {'<', '>', '<=', '>=', '==', '!=', '&&', '||'}
OPEN = {'{': '}', '(': ')', '[': ']'}
CMT = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/|//[^\n]*', re.S)
DIR = re.compile(r'^[ \t]*#[^\n]*(?:\\\n[^\n]*)*', re.M)
DEF = re.compile(r'#\s*define\s+(\w+)(?!\()[ \t]+(.*)', re.S)
TOK = re.compile(r'[A-Za-z_]\w*|0[xX][0-9A-Fa-f]+[uUlL]*|\d+[uUlL]*|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|->|<<=|>>=|[-+*/%&|^<>=!]=|<<|>>|&&|\|\||\+\+|--|\S')
ID = re.compile(r'[A-Za-z_]')
CONSTISH = re.compile(r'[A-Z_][A-Z0-9_]*$|\d')
COUNTERS = ('NELEMS', 'ARRAY_COUNT', 'ARRAY_SIZE', 'ARRAY_LENGTH', 'COUNT_OF')


def blank(s):
    return re.sub(r'[^\n]', ' ', s)


def cond_val(c, defined):
    """True / False / None (unknown) for a #if / #ifdef condition."""
    c = c.strip()
    if c in ('0', '(0)'): return False
    if c in ('1', '(1)'): return True
    m = re.fullmatch(r'defined\s*\(?\s*(\w+)\s*\)?', c)
    if m: return defined.get(m.group(1))
    m = re.fullmatch(r'!\s*defined\s*\(?\s*(\w+)\s*\)?', c)
    if m:
        v = defined.get(m.group(1)); return None if v is None else not v
    parts = re.split(r'\s*\|\|\s*', c)
    if len(parts) > 1:
        vs = [cond_val(p, defined) for p in parts]
        return True if True in vs else (False if all(v is False for v in vs) else None)
    return None


def preprocess(text, defined):
    """Comments out; directive lines out (inactive #if branches blanked); simple object-like #defines collected."""
    text = CMT.sub(lambda m: (blank(m.group()) if m.group()[0] == '/' else m.group()), text)
    macros, out, last, stack, skip = {}, [], 0, [], False   # frame = [parent_skip, state of the taken-so-far branch]
    for m in DIR.finditer(text):
        d = m.group()
        seg = text[last:m.start()]
        out.append(blank(seg) if skip else seg)
        last = m.end()
        w = re.match(r'[ \t]*#\s*(\w+)\s*(.*)', d, re.S)
        k, rest = (w.group(1), w.group(2).strip()) if w else ('', '')
        if k in ('if', 'ifdef', 'ifndef'):
            if k == 'if': v = cond_val(rest, defined)
            else:
                nm = rest.split()[0] if rest.split() else ''
                v = defined.get(nm)
                if k == 'ifndef' and v is not None: v = not v
            stack.append([skip, v]); skip = skip or v is False
        elif k == 'else' and stack:
            par, v = stack[-1]; skip = True if v is True else par
        elif k == 'elif' and stack:
            par, v = stack[-1]
            if v is True: skip = True
            else:
                v2 = cond_val(rest, defined); stack[-1][1] = v2; skip = par or v2 is False
        elif k == 'endif' and stack:
            skip = stack.pop()[0]
        elif k == 'define' and not skip:
            dm = DEF.match(d.strip())
            if dm: macros[dm.group(1)] = re.sub(r'\\\n', ' ', dm.group(2)).strip()
        out.append(blank(d))
    seg = text[last:]
    out.append(blank(seg) if skip else seg)
    return ''.join(out), macros


def prim_size(tt):
    """DS byte size of a declared element type (tokens) or None."""
    if tt and tt[-1] == '*': return 4
    ws = [t for t in tt if t not in QUAL]
    if len(ws) == 1: return PRIM.get(ws[0])
    if ws and ws[-1] in ('int', 'short', 'char'): return PRIM[ws[-1]]
    return None


def scan_c(args):
    path, defined = args
    try:
        raw = open(path, encoding='latin-1').read()
    except OSError:
        return path, None
    text, macros = preprocess(raw, defined)
    nl = [m.start() for m in re.finditer('\n', text)]
    line = lambda p: bisect.bisect_left(nl, p) + 1
    toks, pos = [], []
    for m in TOK.finditer(text):
        toks.append(m.group()); pos.append(m.start())
    n = len(toks)
    match, stack = {}, []
    for i, t in enumerate(toks):
        if t in OPEN: stack.append(i)
        elif t in (')', ']', '}') and stack: match[stack.pop()] = i
    F = dict(defs=[], uses=[], loops=[], enums=[], typedefs={}, members=[], funcs=[], macros=macros, plain=set())

    def init_count(a):                       # a = index of '{'
        e, cnt, i, has = match.get(a, n), 0, a + 1, False
        while i < e:
            t = toks[i]
            if t in OPEN and i in match: has = True; i = match[i] + 1; continue
            if t == ',':
                cnt += has; has = False
            else: has = True
            i += 1
        cnt += has
        if a + 1 < e and toks[a + 1] == '[':                 # designated: [K] = v
            ks, i = [], a + 1
            while i < e:
                if toks[i] == '[' and (i == a + 1 or toks[i - 1] == ','):
                    ks.append(' '.join(toks[i + 1:match.get(i, i)]))
                i = match[i] + 1 if toks[i] in OPEN and i in match else i + 1
            return ('desig', tuple(ks), cnt)
        return cnt

    def decl(s, e, scope, in_func):
        """one array declarator in tokens [s, e): a defs row or None."""
        j, k = s, None
        while j < e:
            t = toks[j]
            if t == '=': break
            if t == '{' and j in match: j = match[j] + 1; continue          # `struct {..} name[N];`
            if t == '[' and j > s and ID.match(toks[j - 1]): k = j; break
            if t == '(' and j > s and toks[j - 1] != '*' and not (j + 1 < e and toks[j + 1] == '*'): return None
            j += 1
        if k is None: return None
        name = toks[k - 1]
        tl, q = [], s
        while q < k - 1:
            if toks[q] == '{' and q in match: tl.append('struct'); q = match[q] + 1; continue
            if toks[q] != '(': tl.append(toks[q])
            q += 1
        tt = tuple(tl)
        if not tt or tt[0] == 'typedef' or tt[0] in NOTYPE: return None
        if not all(ID.match(x) or x == '*' for x in tt): return None
        if in_func is not None and not any(ID.match(x) and x not in QUAL for x in tt): return None
        dims, p = [], k
        while p < e and toks[p] == '[' and p in match:
            dims.append(' '.join(toks[p + 1:match[p]])); p = match[p] + 1
        while p < e and toks[p] == ')': p += 1
        while p < e and toks[p] == '(' and p in match: p = match[p] + 1
        ic = None
        if p < e and toks[p] == '=':
            q = p + 1
            if q < e and toks[q] == '{': ic = init_count(q)
            elif q < e and toks[q][0] == '"': ic = len(toks[q]) - 1
        base = tuple(x for x in tt if x not in QUAL and x != '*')
        ptr = '*' in tt or any(x == '(' for x in toks[s:k])
        esz = 4 if ptr else prim_size(tt)
        return (name, tuple(dims), ic, esz, base[-1] if base else '', 'static' in tt, 'extern' in tt, line(pos[k]), scope)

    def struct_size(a):
        """DS byte size of the struct/union body at tokens a.. when every member is a primitive / pointer / known type, else None."""
        e = match.get(a, n); i = s0 = a + 1; off = al = 0; un = toks[a - 1] == 'union' or (a > 1 and toks[a - 2] == 'union')
        top = 0
        while i < e:
            if toks[i] in OPEN and i in match: i = match[i] + 1; continue
            if toks[i] == ';':
                seg = toks[s0:i]; s0 = i + 1
                if not seg or ':' in seg or '{' in seg or '(' in seg: return None
                k = len(seg); mult = 1
                while k > 0 and seg[k - 1] == ']':
                    q = k - 1
                    while q > 0 and seg[q] != '[': q -= 1
                    d = ' '.join(seg[q + 1:k - 1])
                    if not re.fullmatch(r'\d+|0[xX][0-9A-Fa-f]+', d): return None
                    mult *= int(d, 0); k = q
                if k < 2 or not ID.match(seg[k - 1]): return None
                tt = tuple(seg[:k - 1]); sz = prim_size(tt)
                if sz is None:
                    ws = [t for t in tt if t not in QUAL]
                    sz = F['typedefs'].get(ws[-1]) if len(ws) == 1 else None
                    if sz and sz % 4: return None
                if not sz: return None
                a8 = sz if sz in (1, 2, 4) else 4
                if un: top = max(top, sz * mult)
                else:
                    off = (off + a8 - 1) // a8 * a8 + sz * mult
                al = max(al, a8)
            i += 1
        if un: return (top + al - 1) // al * al if al else None
        return (off + al - 1) // al * al if al else None

    def members(a):
        e = match.get(a, n); i = s = a + 1
        while i < e:
            if toks[i] in OPEN and i in match:
                if toks[i] == '{': members(i)
                i = match[i] + 1; continue
            if toks[i] == ';':
                d = decl(s, i, None, 0)
                if d: F['members'].append((d[0], d[1], d[3]))
                else:
                    ids = [x for x in toks[s:i] if ID.match(x) and x not in QUAL]
                    if ids: F['plain'].add(ids[-1])
                s = i + 1
            i += 1

    def enum_body(a):
        e = match.get(a, n); i = a + 1; items = []
        while i < e:
            j = i
            while j < e and toks[j] != ',':
                j = match[j] if toks[j] in OPEN and j in match else j
                j += 1
            seg = toks[i:j]
            if seg and ID.match(seg[0]):
                items.append((seg[0], ' '.join(seg[2:]) if len(seg) > 2 and seg[1] == '=' else None))
            i = j + 1
        F['enums'].append(tuple(items))

    def stmt_end(p):
        t = toks[p] if p < n else ';'
        if t == '{': return match.get(p, n) + 1
        if t in ('if', 'for', 'while') and p + 1 < n and p + 1 in match:
            e = stmt_end(match[p + 1] + 1)
            if t == 'if' and e < n and toks[e] == 'else': e = stmt_end(e + 1)
            return e
        if t == 'else': return stmt_end(p + 1)
        if t == 'do':
            e = stmt_end(p + 1)
            return match.get(e + 1, e) + 2 if e < n and toks[e] == 'while' else e
        j = p
        while j < n and toks[j] != ';':
            j = match[j] if toks[j] in OPEN and j in match else j
            j += 1
        return j + 1

    def cond_vars(c0, c1):                   # ([(var, op, bound_text)], compound?) of a loop condition
        parts, cur, j, ors, ands = [], [], c0, False, False
        while j < c1:
            t = toks[j]
            if t in OPEN and j in match: cur += toks[j:match[j] + 1]; j = match[j] + 1; continue
            if t in ('&&', '||'):
                ors, ands = ors or t == '||', ands or t == '&&'; parts.append(cur); cur = []
            else: cur.append(t)
            j += 1
        if ors and ands: return [], True
        parts.append(cur); out = []
        flip = {'<': '>', '<=': '>=', '>': '<', '>=': '<=', '!=': '!='}
        for c in parts:
            for oi, op in enumerate(c):
                if op in flip and 0 < oi < len(c) - 1:
                    l, r = c[:oi], c[oi + 1:]
                    if len(l) == 1 and ID.match(l[0]): out.append((l[0], op, ' '.join(r), ors))
                    elif len(r) == 1 and ID.match(r[0]): out.append((r[0], flip[op], ' '.join(l), ors))
                    break
        return out, len(parts) > 1 and not ors

    def step_of(b0, b1, v):
        for j in range(b0, b1 - 1):
            if toks[j] == v:
                if toks[j + 1] in ('++', '+='): return 1
                if toks[j + 1] in ('--', '-='): return -1
                if toks[j + 1] == '=' and j + 3 < b1 and toks[j + 2] == v and toks[j + 3] in ('+', '-'): return 1 if toks[j + 3] == '+' else -1
            elif toks[j] in ('++', '--') and toks[j + 1] == v: return 1 if toks[j] == '++' else -1
        return 0

    def guard(b0, b1, v):
        return any(toks[j] == v and (toks[j - 1] in CMP or (j + 1 < n and toks[j + 1] in CMP)) for j in range(b0, b1))

    def func(a, name, j0=None):
        b = match.get(a, n); loops = []; fid = len(F['funcs']); fe = [name, line(pos[a]), [], [], []]; F['funcs'].append(fe)
        if j0 is not None and j0 in match:                   # parameters: (name, pointee type tokens) of the pointer / array ones
            cur, j = [], j0 + 1
            for j in range(j0 + 1, match[j0] + 1):
                if toks[j] in (',', ')'):
                    ids = [x for x in cur if ID.match(x) and x not in QUAL]
                    if ids and ('*' in cur or '[' in cur):
                        pv = [x for x in cur[:cur.index(ids[-1])] if x != '*' and x not in QUAL]
                        fe[2].append((ids[-1], pv[-1] if pv else ''))
                    elif ids: fe[2].append((ids[-1], None))
                    cur = []
                else: cur.append(toks[j])
        i, done = a + 1, set()
        while i < b:
            t = toks[i]
            if t not in ('for', 'while', 'do') or i in done or i + 1 >= b: i += 1; continue
            init = incr = None
            if t == 'do':
                be = stmt_end(i + 1)
                if not (be < n and toks[be] == 'while' and be + 1 in match): i += 1; continue
                done.add(be); bs, cs, ce = i + 1, be + 2, match[be + 1]
            elif toks[i + 1] == '(' and i + 1 in match:
                bs = match[i + 1] + 1; be = stmt_end(bs); cs, ce = i + 2, match[i + 1]
            else: i += 1; continue
            if t == 'for':
                segs, cur, j = [], cs, cs
                while j < ce:
                    if toks[j] in OPEN and j in match: j = match[j] + 1; continue
                    if toks[j] == ';': segs.append((cur, j)); cur = j + 1
                    j += 1
                segs.append((cur, ce))
                if len(segs) != 3: i += 1; continue
                init, (cs, ce), incr = segs[0], segs[1], segs[2]
            cv, compound = cond_vars(cs, ce)
            ex = compound or any(toks[j] in ('break', 'return', 'goto') for j in range(bs, be))
            for (v, op, bt, isor) in cv:
                start, step = None, 0
                if t == 'for':
                    step = step_of(incr[0], incr[1] + 1, v)
                    for j in range(init[0], init[1] - 1):
                        if toks[j] == v and toks[j + 1] == '=':
                            q, r = j + 2, []
                            while q < init[1] and toks[q] != ',': r.append(toks[q]); q += 1
                            start = ' '.join(r); break
                else:
                    step = step_of(bs, be, v)
                    for j in range(i - 1, max(a, i - 150), -1):
                        if toks[j] == v and toks[j + 1] == '=' and toks[j - 1] in (';', '{', '}'):
                            q, r = j + 2, []
                            while q < i and toks[q] != ';': r.append(toks[q]); q += 1
                            start = ' '.join(r); break
                if step: loops.append((bs, be, v, start, op, bt, step, ex, guard(bs, be, v), line(pos[i]), isor))
            i += 1
        F['loops'].append(loops)
        rm = {v: k for k, v in match.items() if a < k < b}
        i = a + 1
        while i < b:                                         # calls, returned variables
            t = toks[i]
            if t == 'return' and i + 2 < b and ID.match(toks[i + 1]) and toks[i + 2] == ';': fe[4].append(toks[i + 1])
            elif ID.match(t) and t not in KW and toks[i + 1] == '(' and i + 1 in match and toks[i - 1] not in ('.', '->'):
                args, cur, j, e2 = [], [], i + 2, match[i + 1]
                while j < e2:
                    if toks[j] in OPEN and j in match: cur += toks[j:match[j] + 1]; j = match[j] + 1; continue
                    if toks[j] == ',': args.append(cur); cur = []
                    else: cur.append(toks[j])
                    j += 1
                if cur or args: args.append(cur)
                q, tg = i - 1, None
                if toks[q] == ')' and q in rm: q = rm[q] - 1
                if toks[q] == '=' and q >= 1 and ID.match(toks[q - 1]): tg = toks[q - 1]
                fe[3].append((t, args, tg, line(pos[i])))
            i += 1
        i = a + 1
        while i < b:                                         # uses  NAME[sub]..
            if toks[i] == '[' and i in match and ID.match(toks[i - 1]) and toks[i - 1] not in KW:
                nm, pv = toks[i - 1], toks[i - 2]
                if pv in ('.', '->'): nm = '.' + nm
                elif (ID.match(pv) and pv not in NOTYPE) or (
                        pv == '*' and (toks[i - 3] == '(' or ID.match(toks[i - 3]) and toks[i - 3] not in NOTYPE)):
                    i += 1; continue
                subs, p = [], i
                while p < n and toks[p] == '[' and p in match and len(subs) < 2:
                    subs.append(' '.join(toks[p + 1:match[p]])); p = match[p] + 1
                s1 = subs[0]; st = s1.split()
                lv = {lp[2] for lp in loops if lp[0] <= i < lp[1]}
                ok = st and all(re.fullmatch(r'[A-Za-z_]\w*|0[xX][0-9A-Fa-f]+[uUlL]*|\d+[uUlL]*|[-+*/<>|&()]+', x) and x != '->' for x in st)
                ids = [x for x in st if ID.match(x)]
                ok = ok and all(x in lv or CONSTISH.match(x) for x in ids) and s1 not in ('0', '1')
                mm = re.fullmatch(r'\(?\s*([A-Za-z_]\w*)\s*\)?\s*-\s*1', s1)
                g = 0
                if not ok and mm and mm.group(1) not in lv:
                    x = mm.group(1); pre = ' '.join(toks[a:i]); ok = True
                    g = int(bool(re.search(r'\b%s\s*(==|!=|>|<=|<|>=)\s*[01]\b|!\s*%s\b|\bif\s*\(\s*%s\s*\)|\bASSERT\w*\s*\([^;]*\b%s\b|\b%s\s*-\s*1\s*(<|>|>=|<=)|\b%s\s*&&|&&\s*%s\b' % ((x,) * 7), pre)))
                if ok:
                    lps = tuple(k for k, lp in enumerate(loops) if lp[0] <= i < lp[1])
                    F['uses'].append((nm, tuple(subs), line(pos[i]), fid, lps, g))
            i += 1
        i = s = a + 1                                        # function-local array definitions
        while i < b:
            t = toks[i]
            if t in OPEN and t != '{' and i in match: i = match[i] + 1; continue
            if t in (';', '{', '}'):
                if t == ';' and s < i:
                    d = decl(s, i, fid, name)
                    if d: F['defs'].append(d)
                s = i + 1
            i += 1

    i = s = 0                                                # file level
    pend = None
    while i < n:
        t = toks[i]
        if t == '{':
            if i > 0 and toks[i - 1] == ')':
                j0 = next((j for j in range(s + 1, i) if toks[j] == '('), None)
                nm = toks[j0 - 1] if j0 else '?'
                func(i, nm, j0); i = match.get(i, n) + 1; s = i; continue
            prev = toks[s:i]
            if '=' not in prev:
                kind = next((x for x in prev if x in ('struct', 'union', 'enum')), None)
                if kind == 'enum': enum_body(i)
                elif kind:
                    members(i); sz = struct_size(i)
                    if sz and toks[i - 1] != kind and ID.match(toks[i - 1]): F['typedefs'][toks[i - 1]] = sz
                    pend = sz
            i = match.get(i, n) + 1; continue
        if t in OPEN: i = match.get(i, n) + 1; continue
        if t == ';':
            if toks[s] == 'typedef':
                ws = toks[s + 1:i]
                if len(ws) == 2 and ws[0] in PRIM: F['typedefs'][ws[1]] = PRIM[ws[0]]
                elif pend and ID.match(toks[i - 1]): F['typedefs'][toks[i - 1]] = pend
                pend = None
            else:
                d = decl(s, i, None, None)
                if d: F['defs'].append(d)
            s = i + 1
        i += 1
    return path, F


# ------------------------------------------------------------------------------------------------ asm labels
LAB = re.compile(r'^\s*([A-Za-z_@$.][\w@$.]*)\s*:')
DIRV = re.compile(r'^\s*\.(\w+)\s*(.*)$')


def num(x):
    try: return int(x.strip(), 0)
    except ValueError: return None


def scan_asm(path):
    """[(label, data bytes up to the next label (alignment padding excluded), size_is_lower_bound)] of one .s file."""
    res, pend, off, dend = [], [], 0, 0

    def flush(last, nxt=None):
        for nm, o in pend:
            if dend > o: res.append((nm, dend - o, last, nxt))
        pend.clear()
    for ln in open(path, encoding='latin-1'):
        ln = ln.split(';')[0].rstrip()
        if not ln.strip(): continue
        m = LAB.match(ln)
        if m:
            if pend and dend > pend[0][1]: flush(False, m.group(1))
            pend.append((m.group(1), off)); ln = ln[m.end():]
            if not ln.strip(): continue
        d = DIRV.match(ln)
        if not d:
            pend.clear(); continue                           # an instruction: code labels carry no data
        k, a = d.group(1), d.group(2)
        if k in ('text', 'rodata', 'data', 'bss', 'section', 'arm', 'thumb', 'code'):
            if k in ('text', 'rodata', 'data', 'bss', 'section'): flush(True); off = dend = 0
            continue
        sz = None
        if k in ('byte',): sz = a.count(',') + 1
        elif k in ('short', '2byte', 'hword'): sz = 2 * (a.count(',') + 1)
        elif k in ('word', '4byte', 'long', 'float'): sz = 4 * (a.count(',') + 1)
        elif k in ('space', 'skip', 'zero'): sz = num(a.split(',')[0])
        elif k in ('ascii', 'asciz', 'string'):
            sm = re.findall(r'"((?:\\.|[^"\\])*)"', a)
            sz = sum(len(re.sub(r'\\.', 'x', s)) + (1 if k != 'ascii' else 0) for s in sm)
        elif k in ('balign', 'align'):
            al = num(a.split(',')[0])
            if al and k == 'balign': off = (off + al - 1) // al * al
            continue
        if sz: off += sz; dend = off
    flush(True)
    return res


# ------------------------------------------------------------------------------------------------ evaluation
class Unk(Exception):
    pass


class Ctx:
    """Name resolution: macros, enums, table counts (per file)."""
    def __init__(self, macros, enums, typedefs, tables):
        self.macros, self.enums, self.typedefs, self.tables = macros, enums, typedefs, tables
        self.cur = None
        self.memo = {}

    def table(self, name):
        return self.tables(name, self.cur)

    def ieval(self, text, env=None, depth=0):
        if depth > 10: raise Unk
        env = env or {}
        toks = TOK.findall(text)
        pos = [0]
        ctx = self

        def peek(): return toks[pos[0]] if pos[0] < len(toks) else None

        def take(): pos[0] += 1; return toks[pos[0] - 1]

        def typeish(seq):
            return seq and all(x in PRIM or x in QUAL or x in ctx.typedefs or x == '*' for x in seq)

        def prim():
            t = take()
            if t == '(':
                j, d = pos[0], 1
                while j < len(toks) and d: d += (toks[j] == '(') - (toks[j] == ')'); j += 1
                if typeish(toks[pos[0]:j - 1]):                  # a cast
                    pos[0] = j; return prim()
                v = expr(0); take(); return v
            if t in ('-', '+', '~', '!'):
                v = prim()
                if t == '+': return v
                if v[0] != v[1]: return (-v[1], -v[0]) if t == '-' else (_ for _ in ()).throw(Unk())
                return ({'-': -v[0], '~': ~v[0], '!': int(not v[0])}[t],) * 2
            if re.fullmatch(r'(0[xX][0-9A-Fa-f]+|\d+)[uUlL]*', t):
                v = int(re.sub(r'[uUlL]+$', '', t), 0); return (v, v)
            if t[0] == "'" and len(t) >= 3: return (ord(t[1]),) * 2 if t[1] != '\\' else (0, 0)
            if t == 'sizeof':
                take(); j, d = pos[0], 1
                while j < len(toks) and d: d += (toks[j] == '(') - (toks[j] == ')'); j += 1
                inner = [x for x in toks[pos[0]:j - 1] if x not in ('(', ')')]
                pos[0] = j
                if len(inner) == 1 and (inner[0] in PRIM or inner[0] in ctx.typedefs):
                    v = PRIM.get(inner[0], ctx.typedefs.get(inner[0])); return (v, v)
                nm = next((x for x in inner if ID.match(x)), None)
                tb = ctx.table(nm) if nm else None
                if not tb: raise Unk
                if inner[0] == '*' or '[' in inner:
                    if not tb['esz']: raise Unk
                    return (tb['esz'] * tb['inner'],) * 2
                if tb['bytes'] is None: raise Unk
                return (tb['bytes'],) * 2
            if ID.match(t):
                if t in env: return env[t]
                if t in COUNTERS and peek() == '(':
                    take(); nm = take(); take(); tb = ctx.table(nm)
                    if not tb or tb['count'] is None: raise Unk
                    return (tb['count'],) * 2
                if t in ctx.enums and ctx.enums[t] is not None: return (ctx.enums[t],) * 2
                mv = ctx.macros.get(t)
                if mv: return ctx.ieval(mv, env, depth + 1)
                raise Unk
            raise Unk

        PREC = [('|',), ('^',), ('&',), ('<<', '>>'), ('+', '-'), ('*', '/', '%')]

        def expr(lv):
            if lv == len(PREC): return prim()
            v = expr(lv + 1)
            while peek() in PREC[lv]:
                op = take(); w = expr(lv + 1)
                if op == '+': v = (v[0] + w[0], v[1] + w[1])
                elif op == '-': v = (v[0] - w[1], v[1] - w[0])
                elif op == '*':
                    c = [a * b for a in v for b in w]; v = (min(c), max(c))
                else:
                    if v[0] != v[1] or w[0] != w[1]: raise Unk
                    a, b = v[0], w[0]
                    try:
                        r = {'/': lambda: int(a / b), '%': lambda: a % b, '<<': lambda: a << b, '>>': lambda: a >> b,
                             '&': lambda: a & b, '|': lambda: a | b, '^': lambda: a ^ b}[op]()
                    except ZeroDivisionError: raise Unk
                    v = (r, r)
            return v
        v = expr(0)
        if pos[0] != len(toks): raise Unk
        return v

    def const(self, text, env=None):
        try:
            v = self.ieval(text, env); return v[0] if v[0] == v[1] else None
        except (Unk, IndexError, KeyError, RecursionError):
            return None

    def interval(self, text, env):
        try: return self.ieval(text, env)
        except (Unk, IndexError, KeyError, RecursionError): return None


def load_cache(no_cache):
    if no_cache or not os.path.exists(CACHE): return {}
    try:
        with open(CACHE, 'rb') as f: return pickle.load(f)
    except Exception:
        return {}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('files', nargs='*')
    ap.add_argument('--root', default=os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'third_party', 'pokeplatinum'))
    ap.add_argument('--extra', action='append', default=None, help='more trees read for macros / enums (default: build/pokeplatinum/gen)')
    ap.add_argument('--jobs', type=int, default=4); ap.add_argument('--tsv'); ap.add_argument('--all', action='store_true')
    ap.add_argument('--no-cache', action='store_true'); ap.add_argument('--follow')
    ap.add_argument('--limit', type=int, default=60)
    ap.add_argument('--members', action='store_true', help='also member names declared in several structs with different sizes (thousands of SUSP)')
    a = ap.parse_args()
    root = os.path.abspath(a.root); t0 = time.time()
    defined = {'SDK_PORT': False, 'NONMATCHING': False, 'PLATINUM': True}
    a.game = 'PL'
    cfiles = []
    extra = a.extra if a.extra is not None else [os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'build', 'pokeplatinum', 'gen')]
    for base, ds in [(root, ('src', 'include', 'lib'))] + [(os.path.abspath(x), ('.',)) for x in extra]:
        for d in ds:
            for dp, _, fs in os.walk(os.path.join(base, d)):
                cfiles += [os.path.join(dp, f) for f in fs if f.endswith(('.c', '.h'))]
    cfiles.sort()
    cache = load_cache(a.no_cache)
    me = os.stat(os.path.abspath(__file__)).st_mtime_ns
    key = lambda p: (os.stat(p).st_size, os.stat(p).st_mtime_ns, a.game, me)
    todo = [p for p in cfiles if cache.get(p, (None,))[0] != key(p)]
    if todo:
        with Pool(a.jobs) as pool:
            for p, F in pool.imap_unordered(scan_c, [(p, defined) for p in todo], chunksize=16):
                cache[p] = (key(p), F)
        os.makedirs(os.path.dirname(CACHE), exist_ok=True)
        with open(CACHE + '.tmp', 'wb') as f: pickle.dump(cache, f, protocol=4)
        os.replace(CACHE + '.tmp', CACHE)
    sys.stderr.write('c: %d files, %d parsed, %.1fs\n' % (len(cfiles), len(todo), time.time() - t0))
    FS = {p: cache[p][1] for p in cfiles if cache.get(p, (0, None))[1]}
    # asm labels
    asmf = []
    for dp, _, fs in os.walk(os.path.join(root, 'asm')):
        asmf += [os.path.join(dp, f) for f in fs if f.endswith('.s')]
    asmlab, asmfile = {}, {}
    for p in sorted(asmf):
        for nm, sz, last, nxt in scan_asm(p):
            asmlab.setdefault(nm, []).append((sz, last, p, nxt))
    sys.stderr.write('asm: %d files, %d data labels, %.1fs\n' % (len(asmf), len(asmlab), time.time() - t0))
    # macros / enums / typedefs / members (global)
    macros, enums_l, typedefs, members, plain = {}, set(), {}, {}, set()
    for F in FS.values():
        for k, v in F['macros'].items():
            if k in macros and macros[k] != v: macros[k] = None
            else: macros[k] = v
        enums_l.update(F['enums']); typedefs.update(F['typedefs'])
        for nm, dims, esz in F['members']: members.setdefault(nm, set()).add((dims, esz))
        plain |= F['plain']
    macros = {k: v for k, v in macros.items() if v}
    EV = {}
    ctx = Ctx(macros, EV, typedefs, None)
    for _ in range(3):
        for items in enums_l:
            prev = -1
            for nm, ex in items:
                v = (ctx.const(ex) if ex is not None else prev + 1) if (ex is not None or prev is not None) else None
                if ex is not None and v is None and nm in EV: v = EV[nm]
                if nm not in EV or EV[nm] is None: EV[nm] = v
                prev = v
    # global defs
    gdefs, edecl, hdefs = {}, {}, {}
    for p, F in FS.items():
        for d in F['defs']:
            name, dims, ic, esz, base, st, ex, ln, scope = d
            if ex: edecl.setdefault(name, []).append((p, d))
            elif scope is None and not st: gdefs.setdefault(name, []).append((p, d))
            elif scope is None and p.endswith('.h'): hdefs.setdefault(name, []).append((p, d))

    def cnt_of(d):
        name, dims, ic, esz, base, st, ex, ln, scope = d
        if dims and dims[0] != '':
            c = ctx.const(dims[0]); return c
        if isinstance(ic, tuple):
            ks = [ctx.const(k) for k in ic[1]]
            return max(ks) + 1 if ks and None not in ks else None
        if ic is not None and dims and dims[0] == '': return ic
        return None

    def mk(d, src, conf=1):
        name, dims, ic, esz, base, st, ex, ln, scope = d
        c = cnt_of(d)
        inner = 1
        for x in dims[1:]:
            v = ctx.const(x); inner *= v if v else 1
        e = esz if esz else typedefs.get(base)
        if c is None: return None
        return dict(count=c, esz=e, inner=inner, bytes=(c * e * inner) if e else None, src=src, last=False, conf=conf)

    LB, PA = {}, {}

    def member_table(nm):
        ms = members.get(nm)
        cs = []
        for dims, esz in ms or ():
            c = ctx.const(dims[0]) if dims and dims[0] != '' else None
            inner = 1
            for x in dims[1:]:
                v = ctx.const(x); inner *= v if v else 1
            if c: cs.append((c, esz, inner))
        if not cs: return None
        c, esz, inner = min(cs, key=lambda x: x[0])
        return dict(count=c, esz=esz, inner=inner, bytes=None, src='member', last=False,
                    conf=int(len(set(cs)) == 1 and nm not in plain and len(ms) == 1))

    def tables(name, cur):
        if not cur: return None
        if name[0] == '.': return member_table(name[1:])
        p, fid = cur
        F = FS[p]
        best = None
        for d in F['defs']:                                  # function-local, then this file
            if d[0] == name and not d[6] and d[8] == fid and fid is not None: best = mk(d, 'c-local')
        if not best and fid is not None:                     # a heap block / a parameter bound to a table by its callers
            best = LB.get((p, fid), {}).get(name) or PA.get((p, fid), {}).get(name)
        if not best:
            for d in F['defs']:
                if d[0] == name and not d[6] and d[8] is None:
                    r = mk(d, 'c-file')
                    if r: best = r
        if not best and name in gdefs:
            rs = [r for r in (mk(d, 'c-global') for _, d in gdefs[name]) if r]
            if rs:
                best = min(rs, key=lambda r: r['count'])
                if len({r['count'] for r in rs}) > 1: best['conf'] = 0
        if not best and name in hdefs:                       # a static table of a header the file includes
            rs = [r for r in (mk(d, 'c-header') for _, d in hdefs[name]) if r]
            if rs:
                best = min(rs, key=lambda r: r['count'])
                if len({r['count'] for r in rs}) > 1: best['conf'] = 0
        ed = [d for q, d in edecl.get(name, []) if q == p] or [d for q, d in edecl.get(name, [])]
        if not best:
            for d in ed:
                if d[1] and d[1][0] != '':
                    r = mk(d, 'extern-dim')
                    if r: best = r; break
        if not best and name in asmlab:
            sz, last, _, nxt = asmlab[name][0]
            e = next((x[3] or typedefs.get(x[4]) for x in ed if x[3] or typedefs.get(x[4])), None)
            inner = 1
            if ed:
                for x in ed[0][1][1:]:
                    v = ctx.const(x); inner *= v if v else 1
            best = dict(count=(sz // (e * inner)) if e else None, esz=e or 1, inner=inner, bytes=sz, src='asm', last=last,
                        conf=0 if (last or len(asmlab[name]) > 1) else 1, estk=bool(e), nxt=nxt,
                        nxtref=bool(nxt and (nxt in edecl or nxt in gdefs)))
        if not best:
            for d in ed:
                r = mk(d, 'extern-dim')
                if r: best = r; break
        return best
    ctx.tables = tables
    ALLOC = ('Heap_Alloc', 'Heap_AllocAtEnd')

    def alloc_of(args):
        """(count, element size, src) of a Heap_Alloc size argument `sizeof(T) * N` / `N * sizeof(T)`, else None."""
        if len(args) < 2: return None
        t = ' '.join(args[1])
        m = re.fullmatch(r'sizeof \( ([^()]+) \) \* (.+)', t)
        m2 = re.fullmatch(r'(.+) \* sizeof \( ([^()]+) \)', t)
        if m: ty, cn = m.group(1), m.group(2)
        elif m2: ty, cn = m2.group(2), m2.group(1)
        else: return None
        c = ctx.const(cn)
        if not c or c < 2: return None
        return dict(count=c, esz=ctx.const('sizeof ( %s )' % ty), inner=1, bytes=None, src='heap', last=False, conf=1)
    RB = {}
    for p, F in FS.items():
        for fe in F['funcs']:
            rs = []
            for r in fe[4]:
                for (callee, args, tg, ln) in fe[3]:
                    if tg == r and callee in ALLOC:
                        rs.append(alloc_of(args)); break
                else: rs.append(None)
            if rs and rs[0] and all(x and x['count'] == rs[0]['count'] for x in rs):
                if fe[0] in RB and RB[fe[0]]['count'] != rs[0]['count']: RB[fe[0]] = None
                else: RB[fe[0]] = rs[0]
    for p, F in FS.items():
        for fid, fe in enumerate(F['funcs']):
            d = LB.setdefault((p, fid), {})
            for (callee, args, tg, ln) in fe[3]:
                if not tg: continue
                r = alloc_of(args) if callee in ALLOC else RB.get(callee)
                if r and (tg not in d or d[tg] and d[tg]['count'] == r['count']): d[tg] = r
                elif tg in d and d[tg] and not (r and d[tg]['count'] == r['count']): d[tg] = None
    FDEF = {}
    for p, F in FS.items():
        for fid, fe in enumerate(F['funcs']): FDEF.setdefault(fe[0], []).append((p, fid))

    def bind_pass():
        new = {}
        for p, F in FS.items():
            for gfid, fe in enumerate(F['funcs']):
                for (callee, args, tg, ln) in fe[3]:
                    defs = FDEF.get(callee) or []
                    cand = [x for x in defs if x[0] == p] or (defs if len(defs) == 1 else [])
                    if len(cand) != 1: continue
                    q, ffid = cand[0]
                    params = FS[q]['funcs'][ffid][2]
                    for k, ta in enumerate(args):
                        if k >= len(params): break
                        pname, pty = params[k]
                        if not pty or pty == 'void': continue
                        cast = False
                        if ta and ta[0] == '(' and ')' in ta:
                            ci = ta.index(')'); cast = True; ta = ta[ci + 1:]
                        if len(ta) == 1 and ID.match(ta[0]): nm_ = ta[0]
                        elif len(ta) == 5 and ta[0] == '&' and ta[2] == '[' and ta[3] == '0': nm_ = ta[1]
                        else: continue
                        tb = tables(nm_, (p, gfid))
                        if not tb or tb['count'] is None or tb['inner'] > 1: continue
                        psz = PRIM.get(pty) or typedefs.get(pty); esz = tb['esz']; cnt = tb['count']; conf = tb['conf']
                        if esz and psz and esz != psz: cnt = cnt * esz // psz
                        elif cast and not (esz and psz): conf = 0
                        elif not esz or not psz: conf = 0 if (esz or psz) else conf
                        new.setdefault((q, ffid), {}).setdefault(pname, []).append((cnt, conf, psz or esz or 0, tb['src']))
        res = {}
        for key, ps in new.items():
            for pn, lst in ps.items():
                cs = {x[0] for x in lst}
                res.setdefault(key, {})[pn] = dict(count=min(cs), esz=lst[0][2], inner=1, bytes=None, src='alias:' + lst[0][3], last=False,
                                                  conf=int(len(cs) == 1 and all(x[1] for x in lst)))
        return res
    for _ in range(2):
        PA.clear(); PA.update(bind_pass())
    sys.stderr.write('index: %d macros, %d enums, %d defs, %d externs, %.1fs\n' % (len(macros), len(EV), len(gdefs), len(edecl), time.time() - t0))

    if a.follow:
        nm = a.follow
        for p, F in FS.items():
            for d in F['defs']:
                if d[0] == nm: print(os.path.relpath(p, root), d)
        repo = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
        rows = [l.rstrip('\n').split('\t') for l in open(os.path.join(repo, 'config/plat_us/symbols.tsv'))][1:]
        base = dict(l.split() for l in list(open(os.path.join(repo, 'config/plat_us/modules.tsv')))[1:])
        for r in rows:
            if r[1].split('.')[-1] != nm: continue
            mod, st, en = r[2], int(r[3], 16), int(r[4], 16)
            img = os.path.join(repo, 'extract/plat_us', 'arm9/arm9.bin' if mod == 'main' else 'arm9_overlays/%s.bin' % mod)
            data = open(img, 'rb').read(); off = st - int(base[mod], 16)
            nxt = sorted((int(x[3], 16), x[1]) for x in rows if x[2] == mod and int(x[3], 16) >= en)[:1]
            print('ROM %s %s 0x%08x..0x%08x (%d bytes); after it: %s; next symbol %s' % (r[0].split('/')[-1], mod, st, en, en - st,
                  data[off + en - st:off + en - st + 24].hex(' '), nxt))
        return

    only = {os.path.abspath(f) for f in a.files}
    hits, seen = [], set()
    for p, F in FS.items():
        if only and p not in only: continue
        rel = os.path.relpath(p, root)
        for (nm, subs, ln, fid, lps, g) in F['uses']:
            ctx.cur = (p, fid)
            tb = tables(nm, ctx.cur)
            if not tb: continue
            if tb['src'] == 'member' and not tb['conf'] and not a.members: continue   # one name, several structs: no proof
            loops = F['loops'][fid]; env, used, unsafe, gd, desc = {}, [], False, False, ''
            for k in lps:
                bs, be, v, start, op, bt, step, ex, gu, lln, isor = loops[k]
                B = ctx.const(bt); S = ctx.const(start) if start is not None else None
                if B is None: continue
                if step > 0:
                    hi = B if op == '<=' else B - 1
                    if isor and v in env: hi = max(hi, env[v][1])
                    env[v] = (S if S is not None else 0, hi)
                elif S is not None:
                    env[v] = (B if op == '>=' else B + 1, S)
                else: continue
                used.append((v, ex, gu, lln, start, op, bt, S is not None))
            s1 = subs[0]
            dm = re.fullmatch(r'\(?\s*([A-Za-z_]\w*)\s*\)?\s*-\s*1', s1)
            dec1 = bool(dm) and dm.group(1) not in env and not CONSTISH.match(dm.group(1))
            iv = (-1, -1) if dec1 else ctx.interval(s1, env)
            if iv is None: continue
            lo, hi = iv
            kind = 'const' if not any(re.search(r'\b%s\b' % re.escape(u[0]), s1) for u in used) else 'loop'
            if dec1 and g: continue
            if kind == 'const' and not dec1 and ctx.const(s1) is None: continue
            cnt, bts, esz = tb['count'], tb['bytes'], tb['esz']
            over = False
            if cnt is not None and hi >= cnt: over = True
            elif tb['src'] == 'asm' and bts is not None and (hi + 1) * (esz or 1) > bts: over = True
            neg = lo < 0 and kind == 'loop' and all(u[7] for u in used if re.search(r'\b%s\b' % re.escape(u[0]), s1))
            if len(subs) > 1 and not over and tb['inner'] > 1:
                iv2 = ctx.interval(subs[1], env)
                if iv2 and iv2[1] >= tb['inner'] and tb['src'] != 'asm': over = True; hi = iv2[1]; cnt = tb['inner']
            if dec1: over = False
            if not (over or neg or dec1): continue
            loopinfo = ''.join('for %s=%s %s %s%s%s%s' % (u[0], u[4], u[5], u[6], ' @%d' % u[3], ' exit' if u[1] else '', ' guard' if u[2] else '') for u in used if re.search(r'\b%s\b' % re.escape(u[0]), s1))
            if dec1: cls, why = 'SUSP', 'x-1 unguarded (count %s)' % cnt
            elif not over and neg: cls, why = 'SUSP', 'negative index'
            else:
                bad = [u for u in used if re.search(r'\b%s\b' % re.escape(u[0]), s1) and (u[1] or u[2])]
                # an asm label with an unknown element size, or room for one element only, is one entry of a table
                # the decomp split at referenced addresses: not a proof (SUSP)
                real = tb['conf'] and not bad and not (tb['src'] == 'asm' and not (tb['estk'] and (tb['count'] or 0) >= 2))
                cls = 'REAL' if real else 'SUSP'
                if cls == 'REAL' and tb['src'] == 'c-local' and re.match(r'(sp[0-9A-Fa-f]*|param_\d+|local_\w+|arg\d+|v\d+|a\d+|puVar\d+)$', nm): cls = 'LOCAL'
                why = 'bound' if kind == 'loop' else 'const'
                if bad: why += ' exit/guard'
                if not tb['conf']: why += ' ambiguous/lowerbound'
                if tb['src'] == 'asm': why += ' next=%s%s' % (tb['nxt'], '(C-declared)' if tb['nxtref'] else '(unreferenced)')
            if True:
                k = (rel, ln, nm, s1)
                if k in seen: continue
                seen.add(k)
                hits.append((cls, rel, ln, F['funcs'][fid][0], nm, s1, hi, cnt if cnt is not None else -1, bts if bts is not None else -1,
                             tb['src'], why, loopinfo, esz or 0))
    order = {'REAL': 0, 'SUSP': 1, 'LOCAL': 2}
    hits.sort(key=lambda h: (order[h[0]], h[10].startswith('x-1'), -(h[6] - h[7]) if h[7] >= 0 else 0, h[1], h[2]))
    if a.tsv:
        with open(a.tsv, 'w') as f:
            f.write('class\tfile\tline\tfunc\ttable\tsub\tmaxidx\tcount\tbytes\tsource\twhy\tloop\tesz\n')
            for h in hits: f.write('\t'.join(map(str, h)) + '\n')
    cn = {c: sum(1 for h in hits if h[0] == c) for c in order}
    print('%d hits: %d REAL, %d SUSP, %d LOCAL (stack-array drafts) (%.1fs)' % (len(hits), cn['REAL'], cn['SUSP'], cn['LOCAL'], time.time() - t0))
    for h in hits[:a.limit if not a.all else None]:
        print('%s %s:%d %s %s[%s] maxidx %d vs %s (%s bytes %s, %s) %s %s' % (h[0], h[1], h[2], h[3], h[4], h[5], h[6],
              h[7] if h[7] >= 0 else '?', h[8], h[9], 'esz%d' % h[12], h[10], h[11]))


if __name__ == '__main__':
    main()
