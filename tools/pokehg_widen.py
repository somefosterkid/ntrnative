#!/usr/bin/env python3
"""tools/pokehg_widen.py: HeartGold/SoulSilver's pointer rules as rewrites of the working copy build/pokeheartgold
(tools/pokehg_gen.py), from which port/subprojects/pokeheartgold-widepointers.patch and -widepointers2.patch are made
(docs/hgss.md, "Pointer rules"). Systemic, never by hand:

  --tsv     port/UINTPTR.tsv's BASE-IN-INT rows (an address kept in an int: the decomp's Ghidra-typed C) whose holder is
            a parameter or a local: that parameter (in the definition and in every prototype of the function, in any
            file of src/, include/, lib/, port/nonmatching/: the position, not the name) or local becomes intptr_t
            (s32, int, long) or uintptr_t (u32, unsigned). Not done: an address loaded from memory as a number
            (NUM-HOLDS-PTR rows, "loaded as s32"): its offset is a DS layout, which the HGSS side lifts into struct
            fields first (port/OFFSETS.tsv); "root is s32" / "int base" / struct fields: no single holder to widen.
  --casts DIR  every cast of a pointer to a 32-bit integer clang reports in DIR's logs (tools/pokehg_cc.py --out DIR:
            -Wpointer-to-int-cast, "cast to smaller integer type") as an intptr_t / uintptr_t cast, at the column clang
            gives (as pokeplatinum-widepointers.patch): never worse than before, right wherever the address is computed
            on and cast back. A local the cast starts (`u32 x = (u32)p + n`, `x = (u32)p`) becomes intptr_t /
            uintptr_t too (pokeplatinum-widepointers2.patch's rule): it keeps the address and turns it back into one.
            The same logs' int-to-pointer casts (-Wint-to-pointer-cast, "from smaller integer type"): when the operand
            is a plain name (`(T *)(param_1 + 4)`, `(T *)base`; not `p[i]`, `p->f`, `f(x)`), the int local becomes
            intptr_t / uintptr_t, or the int parameter (definition and every prototype, by position, as --tsv).

  python3 tools/pokehg_widen.py --tsv            # prints what it changed and what it could not, by rule
  python3 tools/pokehg_widen.py --casts build/pokehg_cc
"""
import argparse, collections, csv, os, re, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pokegen4 as G4  # noqa: E402

HG = os.path.join(G4.ROOT, 'build/pokeheartgold')
TOPS = ('src', 'include', 'lib', 'port/nonmatching')
SIGNED = {'s32', 'int', 'long', 'signed', 'signed int', 'long int', 'signed long'}
UNSIGNED = {'u32', 'unsigned', 'unsigned int', 'unsigned long', 'unsigned long int', 'uint'}
INT_TYPE = re.compile(r'^(\s*(?:const\s+)?)(unsigned\s+long\s+int|unsigned\s+long|unsigned\s+int|signed\s+int|'
                      r'signed\s+long|long\s+int|unsigned|signed|s32|u32|int|long|uint)\b(?!\s*\*)(?!\s+(?:char|short)\b)')
NOT_TYPE = {'return', 'else', 'case', 'sizeof', 'goto', 'do', 'if', 'while', 'for', 'switch'}


def wide(t):
    t = ' '.join(t.split())
    return 'intptr_t' if t in SIGNED else 'uintptr_t' if t in UNSIGNED else None


def split_params(s):
    """s (the text between a function's parentheses) -> [(start, end)] of each parameter, commas at depth 0."""
    out, depth, st = [], 0, 0
    for i, c in enumerate(s):
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif c == ',' and depth == 0:
            out.append((st, i))
            st = i + 1
    out.append((st, len(s)))
    return out


def close_paren(text, i):
    """The index of the ')' that closes the '(' at text[i]."""
    depth = 0
    for j in range(i, len(text)):
        if text[j] == '(':
            depth += 1
        elif text[j] == ')':
            depth -= 1
            if depth == 0:
                return j
    return -1


def declarations(text, name):
    """[(open, close)] of the parameter lists of every declaration or definition of name in text (not the calls:
    the token before the name is a type, an identifier or '*', not an operator or a keyword)."""
    out = []
    for m in re.finditer(r'\b%s\s*\(' % re.escape(name), text):
        before = text[:m.start()].rstrip()
        prev = re.search(r'([A-Za-z_][A-Za-z_0-9]*|\*)$', before)
        if not prev or prev.group(1) in NOT_TYPE:
            continue
        o = m.end() - 1
        c = close_paren(text, o)
        if c < 0:
            continue
        nxt = text[c + 1:].lstrip()[:1]
        if nxt in (';', '{', ''):
            out.append((o, c))
    return out


def widen_param(text, name, index):
    """text with parameter index of every declaration of name widened (when it is a 32-bit integer); (text, count)."""
    n = 0
    for o, c in reversed(declarations(text, name)):
        inner = text[o + 1:c]
        ps = split_params(inner)
        if index >= len(ps):
            continue
        a, b = ps[index]
        p = inner[a:b]
        m = INT_TYPE.match(p)
        if not m or not wide(m.group(2)):
            continue
        p = p[:m.start(2)] + wide(m.group(2)) + p[m.end(2):]
        inner = inner[:a] + p + inner[b:]
        text = text[:o + 1] + inner + text[c:]
        n += 1
    return text, n


def param_index(text, func, param):
    """The position of param in func's definition, or None."""
    for o, c in declarations(text, func):
        if text[c + 1:].lstrip()[:1] != '{':
            continue
        inner = text[o + 1:c]
        for i, (a, b) in enumerate(split_params(inner)):
            if re.search(r'\b%s\s*$' % re.escape(param), inner[a:b]):
                return i
    return None


def body(text, func):
    """(start, end) of func's body in text, or None."""
    for o, c in declarations(text, func):
        rest = text[c + 1:]
        if rest.lstrip()[:1] != '{':
            continue
        st = c + 1 + rest.index('{')
        depth = 0
        for j in range(st, len(text)):
            if text[j] == '{':
                depth += 1
            elif text[j] == '}':
                depth -= 1
                if depth == 0:
                    return st, j
    return None


def widen_local(text, func, local):
    """text with func's local declaration `<int type> local` widened; (text, count)."""
    return widen_in(text, body(text, func), local)


def widen_in(text, sp, local):
    """text with the first `<int type> local` declaration inside text[sp[0]:sp[1]] widened; (text, count)."""
    if not sp:
        return text, 0
    b = text[sp[0]:sp[1]]
    pat = re.compile(r'(^|[;{]\s*|\n\s*)((?:const\s+)?)(unsigned\s+long|unsigned\s+int|unsigned|s32|u32|int|long)'
                     r'(\s+%s\s*[;=,\[])' % re.escape(local))
    m = pat.search(b)
    if not m or not wide(m.group(3)):
        return text, 0
    b = b[:m.start(3)] + wide(m.group(3)) + b[m.end(3):]
    return text[:sp[0]] + b + text[sp[1]:], 1


HOLDER = re.compile(r'\b(param|local) ([A-Za-z_][A-Za-z_0-9]*) is ([a-z][a-z0-9 ]*)$')


def tsv_rules(rows):
    """UINTPTR.tsv rows -> ({(file, func, param)}, {(file, func, local)}, Counter of what was not a rule)."""
    params, locs, skip = set(), set(), collections.Counter()
    for r in rows:
        if r['kind'] != 'BASE-IN-INT':
            skip['NUM-HOLDS-PTR (a load: after the offset lifting)'] += 1
            continue
        got = False
        for part in r['what'].split(', '):
            m = HOLDER.match(part)
            if m and wide(m.group(3)):
                (params if m.group(1) == 'param' else locs).add((r['file'], r['function'], m.group(2)))
                got = True
        if not got:
            w = r['what'].split(',')[0]
            skip[re.sub(r'\b(param|local|field) \S+', r'\1 X', w)] += 1
    return params, locs, skip


def files():
    for top in TOPS:
        for dp, dn, fn in os.walk(os.path.join(HG, top)):
            for f in fn:
                if f.endswith(('.c', '.h')):
                    yield os.path.join(dp, f)


def apply_tsv(work, tsv):
    rows = list(csv.DictReader(open(tsv), delimiter='\t'))
    params, locs, skip = tsv_rules(rows)
    return apply_rules(work, params, locs), skip


def apply_rules(work, params, locs):
    """params {(file, func, param)} and locs {(file, func, local)} widened in the working copy (a parameter in the
    definition and every declaration of the function, by position); Counter."""
    stats = collections.Counter()
    texts = {}

    def get(p):
        if p not in texts:
            texts[p] = open(p, errors='surrogateescape').read()
        return texts[p]

    # the parameter positions, from each function's definition
    pos = collections.defaultdict(set)
    for f, func, param in sorted(params):
        p = os.path.join(work, f)
        i = param_index(get(p), func, param) if os.path.exists(p) else None
        if i is None:
            stats['param: no definition found'] += 1
        else:
            pos[func].add(i)
    names = re.compile(r'\b(%s)\s*\(' % '|'.join(map(re.escape, sorted(pos)))) if pos else None
    for p in files():
        t = get(p)
        if not names or not names.search(t):
            continue
        for func in sorted({m.group(1) for m in names.finditer(t)}):
            for i in sorted(pos[func]):
                t, n = widen_param(t, func, i)
                stats['param: declarations widened'] += n
        texts[p] = t
    for f, func, local in sorted(locs):
        p = os.path.join(work, f)
        if not os.path.exists(p):
            stats['local: no file'] += 1
            continue
        t, n = widen_local(get(p), func, local)
        texts[p] = t
        stats['local: widened' if n else 'local: declaration not found'] += 1
    changed = 0
    for p, t in texts.items():
        if t != open(p, errors='surrogateescape').read():
            open(p, 'w', errors='surrogateescape').write(t)
            changed += 1
    stats['files changed'] = changed
    stats['params (unique)'] = len(params)
    stats['functions'] = len(pos)
    return stats


CAST_DIAG = re.compile(r"^(.*?):(\d+):(\d+): warning: cast to smaller integer type '([^']*)'(?: \(aka '([^']*)'\))?")
# the cast at clang's column: an int type, a typedef of one (fx32, HeapID) or an enum (Ghidra's (enum HeapID)ptr)
CAST = re.compile(r'\(\s*((?:const\s+)?(?:enum\s+|unsigned\s+|signed\s+)?[A-Za-z_][A-Za-z_0-9]*)\s*\)')


def cast_wide(spelled, diag_type, aka):
    """The intptr_t / uintptr_t for a cast spelled `spelled` that clang calls diag_type (aka aka), or None (u16, u8...)."""
    t = re.sub(r'^const\s+', '', aka or diag_type)
    return wide(re.sub(r'^const\s+', '', spelled)) or wide(t) or ('intptr_t' if t.startswith('enum ') else None)
# the cast is a local's whole start: `u32 x = (u32)p...` or `x = (u32)p...` (pokeplatinum-widepointers2.patch's rule)
HOLDS = re.compile(r'(?:^|[;{(]|\s)([A-Za-z_][A-Za-z_0-9]*)\s*=\s*$')


def top_block(text, pos):
    """(start, end) of the top-level {...} holding pos (a function body), or None. Comments and literals skipped."""
    depth, st, i, n = 0, None, 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith('//', i):
            j = text.find('\n', i)
            i = n if j < 0 else j
            continue
        if text.startswith('/*', i):
            j = text.find('*/', i + 2)
            i = n if j < 0 else j + 2
            continue
        if c in '"\'':
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                j += 2 if text[j] == '\\' else 1
            i = j + 1
            continue
        if c == '{':
            if depth == 0:
                st = i
            depth += 1
        elif c == '}' and depth:
            depth -= 1
            if depth == 0:
                if st <= pos <= i:
                    return st, i
                if st > pos:
                    return None
        i += 1
    return None


def widen_local_at(text, pos, local):
    """text with the `<int type> local` declared in the function body holding pos widened; (text, count)."""
    return widen_in(text, top_block(text, pos), local)


# an int-to-pointer cast: `(T *)name`, `(T *)(name + n)`; the int `name` it reads is what holds the address
I2P_DIAG = re.compile(r"^(.*?):(\d+):(\d+): warning: cast to '[^']*'(?: \(aka '[^']*'\))? from smaller integer type")
OPERAND = re.compile(r'[\s(]*([A-Za-z_]\w*)\s*(?!->)(?=[-+);,]|$)')


def cast_operand(s, col):
    """The name the cast whose '(' is at clang's column col (1-based) of line s reads, when the operand is `name` or
    starts with it (`(name + 4)`, `name - n`, not `name[i]`, `name->f`, `f(x)`); else None."""
    if col < 1 or col > len(s) or s[col - 1] != '(':
        return None
    c = close_paren(s, col - 1)
    m = OPERAND.match(s, c + 1) if c > 0 else None
    return m.group(1) if m and m.group(1) not in NOT_TYPE else None


def line_pos(text, ln):
    """The offset of line ln (1-based) in text."""
    i = 0
    for _ in range(ln - 1):
        i = text.index('\n', i) + 1
    return i


def enclosing_function(text, pos):
    """The name of the function whose top-level body holds pos, or None."""
    sp = top_block(text, pos)
    head = text[:sp[0]].rstrip() if sp else ''
    if not head.endswith(')'):
        return None
    depth = 0
    for j in range(len(head) - 1, -1, -1):
        depth += (head[j] == ')') - (head[j] == '(')
        if depth == 0:
            m = re.search(r'([A-Za-z_]\w*)\s*$', head[:j])
            return m.group(1) if m else None
    return None


def apply_casts(work, out):
    """Every pointer-to-int cast the logs in out/log report, widened at its column; the int local or parameter an
    int-to-pointer cast reads (`(T *)(param_1 + 4)`) too; Counter."""
    at = collections.defaultdict(set)
    i2p = collections.defaultdict(set)
    for dp, dn, fn in os.walk(os.path.join(out, 'log')):
        for f in fn:
            for line in open(os.path.join(dp, f), errors='replace'):
                m = CAST_DIAG.match(line)
                if m:
                    p = os.path.normpath(os.path.join(work, m.group(1)))
                    if p.startswith(work + os.sep) and '/gen/' not in p:
                        at[p].add((int(m.group(2)), int(m.group(3)), m.group(4), m.group(5)))
                    continue
                m = I2P_DIAG.match(line)
                if m:
                    p = os.path.normpath(os.path.join(work, m.group(1)))
                    if p.startswith(work + os.sep) and '/gen/' not in p:
                        i2p[p].add((int(m.group(2)), int(m.group(3))))
    stats = collections.Counter()
    params = set()
    for p in sorted(set(at) | set(i2p)):
        spots = at.get(p, ())
        lines = open(p, errors='surrogateescape').read().split('\n')
        reads = []
        for ln, col in sorted(i2p.get(p, ())):
            name = cast_operand(lines[ln - 1], col)
            if name:
                reads.append((ln, name))
            else:
                stats['int-to-pointer cast: operand not a plain name (an element, a field, a call)'] += 1
        holders = []
        for ln, col, ty, aka in sorted(spots, key=lambda x: x[:2], reverse=True):
            s = lines[ln - 1]
            m = CAST.match(s, col - 1)
            w = m and cast_wide(m.group(1), ty, aka)
            if not w:
                stats['cast: not a cast to a 32-bit int or enum at the column (a macro? u16?)'] += 1
                continue
            lines[ln - 1] = s[:m.start(1)] + w + s[m.end(1):]
            stats['cast: widened'] += 1
            h = HOLDS.search(s[:m.start()])
            if h:
                holders.append((ln, h.group(1)))
        text = '\n'.join(lines)
        for ln, name in holders:
            text, n = widen_local_at(text, line_pos(text, ln), name)
            stats['local holding the cast: widened' if n else 'local holding the cast: none (a global/field/param)'] += 1
        seen = set()
        for ln, name in reads:
            pos = line_pos(text, ln)
            sp = top_block(text, pos)
            if (sp and sp[0], name) in seen:
                continue
            seen.add((sp and sp[0], name))
            text, n = widen_local_at(text, pos, name)
            if n:
                stats['int-to-pointer cast: local widened'] += 1
                continue
            func = enclosing_function(text, pos)
            if func and param_index(text, func, name) is not None:
                params.add((os.path.relpath(p, work), func, name))
            else:
                stats['int-to-pointer cast: not an int local or parameter (a global, a u16...)'] += 1
        if text != open(p, errors='surrogateescape').read():
            open(p, 'w', errors='surrogateescape').write(text)
            stats['files changed'] += 1
    if params:
        for k, n in apply_rules(work, params, set()).items():
            stats['int-to-pointer cast, ' + k] += n
    return stats


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--tsv', action='store_true', help="port/UINTPTR.tsv's parameters and locals")
    ap.add_argument('--casts', metavar='DIR', help="the pointer casts tools/pokehg_cc.py's logs in DIR report")
    o = ap.parse_args()
    if o.tsv:
        stats, skip = apply_tsv(HG, os.path.join(HG, 'port/UINTPTR.tsv'))
        for k, n in sorted(stats.items()):
            print('%6d  %s' % (n, k))
        print('not rules here (rows):')
        for k, n in skip.most_common(12):
            print('%6d  %s' % (n, k))
    if o.casts:
        for k, n in sorted(apply_casts(HG, os.path.abspath(o.casts)).items()):
            print('%6d  %s' % (n, k))


if __name__ == '__main__':
    main()
