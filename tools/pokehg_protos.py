#!/usr/bin/env python3
"""HGSS: a function's declarations and its definition that disagree on int vs pointer in a slot (tools/pokehg_protos.py).

pret's HGSS C declares many functions in other files (or headers) with an int where the definition takes or returns
a pointer, or the reverse: on the DS both are 4 bytes; on a 64-bit host the int slot truncates the pointer (the caller
passes 32 bits through an int prototype, or the callee reads its pointer argument as s32). A first-order host fix, as
the widening patches: in every slot where the definition and a declaration disagree (one says pointer, one a 32-bit
int), each 32-bit int of that slot (the definition's and every declaration's) becomes uintptr_t: the value passes
whole both ways (uintptr_t and a pointer are the same register class). The int locals the value then flows through
are the HGSS side's (or a later widening rule's).

  python3 tools/pokehg_protos.py [--tree DIR] [--report OUT.tsv]   scan: counts (and every mismatch, for the HGSS side)
  python3 tools/pokehg_protos.py --tree DIR --write [--chains B]    rewrite the slots in DIR (a clean patched tree);
      --chains: then each 32-bit local a uintptr_t / intptr_t is stored into (clang -Wshorten-64-to-32 with build dir
      B's compile commands, DIR in place of build/pokeheartgold; DIR/gen must exist) becomes intptr_t / uintptr_t
      (pokehg_widen.py's local rule), until no warning names a new one
pokeheartgold-wideprotos.patch is made with --write on a fresh tree (all the other patches applied), then `git diff -U2`.
--write also runs the extern data pass (--externs alone: only it): `extern s32 X;` where another file has `extern T *X;`
(17 globals on 95d5f7f1d, e.g. _021D0EB0, the OAM managers: SIGSEGV in g2d_OAM.c at the intro movie) -> uintptr_t.
In wideprotos.patch since hg11's re-pin to bc7fa4386 (made with `--write --chains build/port/linux-hgss`: 4,002 slots,
261 locals, 4 extern declarations; upstream fixed the other externs).
--write first puts back the STALE widenings (stale()): a generated patch's hunk is carried over to a new pin as it is
as long as its context applies; where upstream then fixed the definition or the declaration its own way, no pointer is
left in the slot and the old uintptr_t is a `conflicting types` build error (hgsw5: sub_0205DD94 at 50a4b94b0). Only declarations in
headers and in the definition's own file are put back (those are the ones that conflict).
--arity OUT.tsv: the declarations with another parameter count than the definition (report only; 95d5f7f1d: 781 with
fewer, the crash class of zhostfix.patch's sub_02013424).
"""
import argparse, glob, os, re, subprocess, sys
from collections import defaultdict

INT32 = re.compile(r'\b(unsigned\s+int|signed\s+int|unsigned|signed|s32|u32|int|fx32|undefined4|BOOL|vu32|vs32|'
                   r'int32_t|uint32_t)\b')
OTHER_INT = re.compile(r'\b(u16|s16|u8|s8|short|char|long|u64|s64|size_t|uintptr_t|intptr_t|bool|vu16|vu8)\b')
COMMENT = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"', re.S)
FUNC = re.compile(r'^([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)\s*\(([^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*([;{])', re.M)
KW = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'else', 'do', 'case', 'goto'}


def kind(t):
    """'ptr', 'int32', 'other' or None (void / empty / ...)."""
    s = re.sub(r'\b(const|volatile|static|extern|inline|register|__inline)\b', ' ', t).strip()
    if not s or s == 'void' or s == '...':
        return None
    if '(' in s or '[' in s or '*' in s:
        return 'ptr'
    if re.match(r'(struct|enum|union)\b', s):
        return 'other'
    m = INT32.match(s)
    if m and not OTHER_INT.match(s):
        rest = s[m.end():].split()
        if len(rest) <= 1 and not (rest and rest[0] in ('long', 'short', 'char')):
            return 'int32'
    return 'other'


def split_params(s, base):
    """[(text, start, end)] of a parameter list s that starts at offset base."""
    if s.strip() in ('', 'void'):
        return []
    out, depth, st = [], 0, 0
    for i, c in enumerate(s):
        if c == ',' and depth == 0:
            out.append((s[st:i], base + st, base + i))
            st = i + 1
        elif c == '(':
            depth += 1
        elif c == ')':
            depth -= 1
    out.append((s[st:], base + st, base + len(s)))
    return out


def scan(tree):
    files = sorted(glob.glob(tree + '/src/**/*.c', recursive=True) + glob.glob(tree + '/port/nonmatching/*.c') +
                   glob.glob(tree + '/include/**/*.h', recursive=True))
    occ = defaultdict(list)    # name -> [(is_def, static, file, [(kind, start, end) per slot: return first])]
    ndefs = defaultdict(int)
    for f in files:
        raw = open(f, encoding='latin-1').read()
        txt = COMMENT.sub(lambda m: re.sub(r'[^\n]', ' ', m.group(0)), raw)
        for m in FUNC.finditer(txt):
            ret, name, ps, end = m.groups()
            r = ret.strip()
            if name in KW or not r or re.search(r'\b(typedef|return|else|define)\b', r) or r.startswith('#'):
                continue
            slots = [(kind(ret), m.start(1), m.end(1))]
            slots += [(kind(p), a, b) for p, a, b in split_params(ps, m.start(3))]
            is_def = end == '{'
            static = bool(re.search(r'\bstatic\b', r))
            occ[name].append((is_def, static, os.path.relpath(f, tree), slots))
            if is_def:
                ndefs[name] += 1
    return occ, ndefs


def conflicts(occ, ndefs):
    """[(name, slot, def_kind, [occurrences to rewrite])]: slots where the definition and a declaration disagree."""
    out = []
    for name, os_ in occ.items():
        if ndefs[name] != 1:
            continue
        d = next(o for o in os_ if o[0])
        if d[1]:     # a static definition: declarations elsewhere are other functions
            os_ = [o for o in os_ if o[2] == d[2]]
        decls = [o for o in os_ if not o[0]]
        for i in range(len(d[3])):
            ks = {o[3][i][0] for o in os_ if len(o[3]) == len(d[3])}
            if 'ptr' in ks and 'int32' in ks:
                rew = [o for o in os_ if len(o[3]) == len(d[3]) and o[3][i][0] == 'int32']
                out.append((name, i, d[3][i][0], d, decls, rew))
    return out

WIDE = re.compile(r'\b(uintptr_t|intptr_t)\b')


def stale(tree, occ, ndefs):
    """[(file, (start, end), the text it becomes)]: widenings a re-pin carried over (the generated patch's hunk still
    applies, its context is unchanged) that upstream has since made wrong by fixing the other side: the one definition
    says a 32-bit int in a slot, a declaration says uintptr_t / intptr_t, and no occurrence of that slot is a pointer any
    more (hgsw5, sub_0205DD94 at 50a4b94b0: the definition used to return a pointer, the header said int; the hunk made
    the header uintptr_t; upstream then made both an int: `conflicting types`, and a caller reading rax would take
    garbage). The declaration goes back to the definition's own type word."""
    out, cache = [], {}

    def seg(o, i):
        if o[2] not in cache:
            cache[o[2]] = open(os.path.join(tree, o[2]), encoding='latin-1').read()
        s, e = o[3][i][1:]
        return cache[o[2]][s:e]

    for name, os_ in sorted(occ.items()):
        if ndefs[name] != 1:
            continue
        d = next(o for o in os_ if o[0])
        if d[1]:
            os_ = [o for o in os_ if o[2] == d[2]]
        same = [o for o in os_ if len(o[3]) == len(d[3])]
        for i in range(len(d[3])):
            if d[3][i][0] != 'int32' or any(o[3][i][0] == 'ptr' for o in same):
                continue
            word = INT32.search(seg(d, i))
            for o in same:
                t = seg(o, i)
                # headers and the definition's own file only: a declaration a .c file makes for itself (widepointers2's
                # ov96_021EAA00 in five port/nonmatching files) is a deliberate widening of what that caller reads
                if (o is not d and (o[2].endswith('.h') or o[2] == d[2]) and o[3][i][0] == 'other' and WIDE.search(t)
                        and word):
                    out.append((o[2], o[3][i][1:], WIDE.sub(word.group(0), t, count=1)))
    return out


def unwiden(tree, occ, ndefs):
    """Applies stale()'s edits to the tree; the number of slots put back."""
    edits = defaultdict(dict)
    for f, span, new in stale(tree, occ, ndefs):
        edits[f][span] = new
    for f, spans in edits.items():
        p = os.path.join(tree, f)
        raw = open(p, encoding='latin-1').read()
        for (s, e), new in sorted(spans.items(), reverse=True):
            raw = raw[:s] + new + raw[e:]
        open(p, 'w', encoding='latin-1').write(raw)
    return sum(len(v) for v in edits.values())


# ---- extern data: a global that one file declares `extern s32 X;` and another `extern T *X;` (hg9) ----
EXTERN = re.compile(r'^extern\s+((?:const\s+|volatile\s+)*[A-Za-z_][\w ]*?)\s*(\*+)?\s*(?:const\s+)?([A-Za-z_]\w*)\s*;',
                    re.M)


def externs(tree):
    """{file: [(start, end)]}: the type spans of the 32-bit int `extern` declarations of a global that another
    declaration (or the definition's header) gives a pointer type; the int side does byte arithmetic on it
    (`(T *)(X + 0x1c)`), so uintptr_t keeps that and the whole pointer."""
    files = sorted(glob.glob(tree + '/src/**/*.c', recursive=True) + glob.glob(tree + '/include/**/*.h', recursive=True))
    occ = defaultdict(list)
    for f in files:
        raw = open(f, encoding='latin-1').read()
        for m in EXTERN.finditer(raw):
            k = 'ptr' if m.group(2) else kind(m.group(1))
            occ[m.group(3)].append((k, os.path.relpath(f, tree), m.start(1), m.end(1)))
    edits = defaultdict(list)
    for name, os_ in occ.items():
        if {'ptr', 'int32'} <= {o[0] for o in os_}:
            for k, f, s, e in os_:
                if k == 'int32':
                    edits[f].append((s, e))
    return edits


def write_externs(tree):
    edits, n = externs(tree), 0
    for f, spans in edits.items():
        p = os.path.join(tree, f)
        raw = open(p, encoding='latin-1').read()
        for s, e in sorted(spans, reverse=True):
            seg = raw[s:e]
            new = INT32.sub('uintptr_t', seg, count=1)
            if new != seg:
                raw, n = raw[:s] + new + raw[e:], n + 1
        open(p, 'w', encoding='latin-1').write(raw)
    print('extern data: rewrote %d int declarations of pointer globals in %d files' % (n, len(edits)))

# ---- --chains: the int locals a widened (uintptr_t / intptr_t) value is stored into ----
SHORTEN = re.compile(r"^(.*?):(\d+):(\d+): warning: implicit conversion loses integer precision: "
                     r"'(?:u?intptr_t)'(?: \(aka '[^']*'\))? to '[^']*'")
ASSIGN = re.compile(r'^\s*(?:(?:const\s+)?(?:unsigned\s+int|unsigned|s32|u32|int)\s+)?([A-Za-z_]\w*)\s*[-+|&^]?=(?!=)')


def _syntax(job):
    cmd, cwd = job
    r = subprocess.run(cmd, shell=True, cwd=cwd, capture_output=True, text=True, errors='replace')
    return r.stderr


def chains(tree, builddir, rounds=8, only=None):
    """Compile tree's C (builddir's compile_commands.json, its build/pokeheartgold paths pointed at tree) with only
    -Wshorten-64-to-32; each warning that stores a uintptr_t / intptr_t into a plain 32-bit local
    (`v1 = arg0 + 0x20;`, `s32 v = p;`) widens that local; again until nothing changes. only: the files to compile
    first (the ones the slot rewrite changed: a full pass over 10k files takes ~20 min)."""
    import json, multiprocessing
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import pokehg_widen as WD
    tree = os.path.abspath(tree)
    hg = os.path.realpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'build/pokeheartgold'))
    hg_alias = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'build/pokeheartgold')
    cc = json.load(open(os.path.join(builddir, 'compile_commands.json')))
    jobs = {}
    for e in cc:
        f = os.path.realpath(e['file'])
        if not f.startswith(hg + '/') or not f.endswith('.c'):
            continue
        rel = os.path.relpath(f, hg)
        cmd = e['command']
        for h in {hg, os.path.normpath(hg_alias), e['file'].split('/build/pokeheartgold/')[0] + '/build/pokeheartgold'}:
            cmd = cmd.replace(h + '/', tree + '/')
        cmd = re.sub(r' -w(?= )', ' ', cmd)    # the module builds with -w: no warning would show
        cmd = re.sub(r' -o \S+', ' ', cmd)
        cmd = re.sub(r' -M[DF]? ?\S*| -MQ \S+', ' ', cmd)
        cmd += ' -fsyntax-only -fno-color-diagnostics -Wno-everything -Wshorten-64-to-32'
        jobs[rel] = (cmd, e['directory'])
    todo = sorted(r for r in jobs if only is None or r in only)
    total = 0
    for rnd in range(rounds):
        with multiprocessing.get_context('fork').Pool(int(os.environ.get('BW_JOBS', '8'))) as p:
            outs = p.map(_syntax, [jobs[r] for r in todo], chunksize=8)
        want = defaultdict(set)    # file -> {(line, local)}
        for out in outs:
            for l in out.splitlines():
                m = SHORTEN.match(l)
                if not m:
                    continue
                f = os.path.realpath(m.group(1))
                if not f.startswith(tree + '/'):
                    continue
                want[os.path.relpath(f, tree)].add(int(m.group(2)))
        changed = []
        n = 0
        for rel, lines in want.items():
            p_ = os.path.join(tree, rel)
            text = open(p_, encoding='latin-1').read()
            src = text.split('\n')
            k0 = n
            for ln in sorted(lines, reverse=True):
                a = ASSIGN.match(src[ln - 1]) if ln - 1 < len(src) else None
                if not a or a.group(1) in WD.NOT_TYPE:
                    continue
                text, c = WD.widen_local_at(text, WD.line_pos(text, ln), a.group(1))
                n += c
            if n > k0:
                open(p_, 'w', encoding='latin-1').write(text)
                changed.append(rel)
        total += n
        print('chains round %d: %d files compiled, %d warnings in %d files, %d locals widened in %d files'
              % (rnd + 1, len(todo), sum(len(v) for v in want.values()), len(want), n, len(changed)), flush=True)
        if not changed:
            break
        # a widened local can make more warnings in its own file only (locals are file-local)
        todo = sorted(c for c in changed if c in jobs)
    return total


def arity(tree, out):
    """--arity: every declaration whose parameter count differs from the (one) definition's. Fewer (and not 0, an
    unprototyped `()`) is the crash class: on the DS a missing argument is whatever its register held (often the
    caller's own argument there), on the host garbage (hg10: sub_02013424 from sub_02010F34). Report only: each call
    needs the right value."""
    occ, ndefs = scan(tree)
    rows = []
    for name, os_ in occ.items():
        if ndefs[name] != 1:
            continue
        d = next(o for o in os_ if o[0])
        if d[1]:
            os_ = [o for o in os_ if o[2] == d[2]]
        for o in os_:
            if not o[0] and len(o[3]) != len(d[3]):
                raw = open(os.path.join(tree, o[2]), encoding='latin-1').read()
                rows.append((name, len(d[3]) - 1, len(o[3]) - 1, d[2], o[2], raw.count('\n', 0, o[3][0][1]) + 1))
    with open(out, 'w') as f:
        f.write('function\tdefinition params\tdeclaration params\tdefinition\tdeclaration\tline\n')
        f.writelines('%s\t%d\t%d\t%s\t%s\t%d\n' % r for r in sorted(rows))
    print('declarations with another arity: %d (fewer, not 0: %d), functions %d' % (
        len(rows), sum(0 < r[2] < r[1] for r in rows), len({r[0] for r in rows})))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--tree', default='build/pokeheartgold')
    ap.add_argument('--report')
    ap.add_argument('--arity', metavar='OUT.tsv', help='only list the declarations whose parameter count differs from '
                    "the definition's (hg10: a missing argument is garbage on the host)")
    ap.add_argument('--write', action='store_true')
    ap.add_argument('--chains', metavar='BUILDDIR', help="after --write: widen the int locals the values are stored in "
                    "(-Wshorten-64-to-32 with BUILDDIR's compile_commands.json, e.g. build/port/linux-hgss)")
    ap.add_argument('--stale', action='store_true', help="only list the widenings a re-pin carried over that upstream's "
                    "own fix has made wrong (--write puts them back first)")
    ap.add_argument('--externs', action='store_true', help="only the extern data pass (--write runs it too): an "
                    "`extern s32 X;` of a global another file declares a pointer becomes uintptr_t")
    a = ap.parse_args()
    if a.externs:
        write_externs(a.tree)
        return
    if a.arity:
        arity(a.tree, a.arity)
        return
    occ, ndefs = scan(a.tree)
    if a.write or a.stale:
        st = stale(a.tree, occ, ndefs)
        print('stale widenings (definition int, a declaration uintptr_t / intptr_t, no pointer): %d' % len(st))
        for f, span, new in st if a.stale else []:
            print('  %s: -> %s' % (f, new.strip()))
        if a.write and st:
            print('put back %d slots' % unwiden(a.tree, occ, ndefs))
            occ, ndefs = scan(a.tree)
    if a.stale:
        return
    cs = conflicts(occ, ndefs)
    print('slots in conflict: %d (def pointer, a decl int: %d; def int, a decl pointer: %d; return slots %d), functions %d'
          % (len(cs), sum(c[2] == 'ptr' for c in cs), sum(c[2] == 'int32' for c in cs), sum(c[1] == 0 for c in cs),
             len({c[0] for c in cs})))
    if a.report:
        with open(a.report, 'w') as o:
            o.write('function\tslot (0 = return)\tdefinition (file)\tits kind\tdeclarations with the other kind\n')
            for name, i, dk, d, decls, rew in sorted(cs):
                other = [x[2] for x in decls if len(x[3]) == len(d[3]) and x[3][i][0] not in (dk, None, 'other')]
                o.write('%s\t%d\t%s\t%s\t%s\n' % (name, i, d[2], dk, ' '.join(sorted(set(other)))))
    if not a.write:
        return
    edits = defaultdict(dict)    # file -> {(start, end): the type to write}
    for name, i, dk, d, decls, rew in cs:
        # a definition already widened to intptr_t (widepointers2, kept at a re-pin) takes its declarations along:
        # uintptr_t there made a header disagree with it (phs1: ov05_0221E9C4, `conflicting types` at 69c1d06be)
        s, e = d[3][i][1:]
        dseg = open(os.path.join(a.tree, d[2]), encoding='latin-1').read()[s:e]
        wide = 'intptr_t' if re.search(r'\bintptr_t\b', dseg) else 'uintptr_t'
        for o in rew:
            edits[o[2]][o[3][i][1:]] = wide
    n = 0
    for f, spans in edits.items():
        p = os.path.join(a.tree, f)
        raw = open(p, encoding='latin-1').read()
        for (s, e), wide in sorted(spans.items(), reverse=True):
            seg = raw[s:e]
            new = INT32.sub(wide, seg, count=1)
            if new != seg:
                raw = raw[:s] + new + raw[e:]
                n += 1
        open(p, 'w', encoding='latin-1').write(raw)
    print('rewrote %d slots in %d files' % (n, len(edits)))
    write_externs(a.tree)
    if a.chains:
        print('chains: %d locals widened' % chains(a.tree, a.chains, only=set(edits)))


if __name__ == '__main__':
    main()
