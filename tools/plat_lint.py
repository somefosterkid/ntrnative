#!/usr/bin/env python3
"""tools/plat_lint.py: static checks for Platinum's 64-bit layout classes, on the patched tree build/pokeplatinum
(tools/pokeplat_gen.py), before they crash. pret's C is written for the DS (4-byte pointers, 32-bit int/long); the host
is LP64, so every struct that holds a pointer is wider and 8-aligned. Each class below crashed (or nearly) once:

  C1  a DS-sized buffer (an integer array field, a u8 blob) cast to a struct that holds pointers: clang's -Wcast-align
      to alignment 8. A cast from a pointer-to-array whose bytes are fewer than the host struct's sizeof is a real
      overflow (Sprite's u32 animData[29], pokeplatinum-widesprite.patch); one that fits is silent. Any other source
      (u8 *, u32 *, a header's blob) needs a row in the allow-list saying why it is harmless.
  C2  a literal size passed to Heap_Alloc/Heap_AllocAtEnd whose result goes to a struct pointer: a literal smaller than
      the host struct's sizeof is a real overflow (HGSS crash 44: HBlankSystem_New(0x1C)); the destination type comes from
      libclang's AST, only for the files that have such a call. A destination that can't be resolved is listed.
  C3  a pointer read at a byte offset that is 4 mod 8, or a pointer array walked with a 4-byte stride
      (`*(T **)((u8 *)p + 4)`, `*(T **)(p + i * 4)`; the HGSS crashes 40 and 45), plus -Wcast-align casts to `T **`.
  C4  a pointer held in a 32-bit integer: -Wpointer-to-int-cast, -Wint-to-pointer-cast, -Wint-to-void-pointer-cast and
      -Wint-conversion (a pointer passed, assigned or initialising an int; NULL used as 0 is ignored), and a
      uintptr_t/intptr_t narrowed to 32 bits (-Wshorten-64-to-32: how a REG_*_ADDR reached a u32 parameter).

  C5  sizeof of a pointer taken for the array it points to: `SNELEMS(outFlags)` / sizeof(p) / sizeof(p[0]) with p a `T *`
      parameter, or an array-typed parameter (clang's -Wsizeof-pointer-div, -Wsizeof-array-argument,
      -Wsizeof-pointer-memaccess). The DS's pointer is 4 bytes, so pret's element counts and memset sizes come out right
      there (4 / 1 = 4 u8 flags) and 8 on the host: the loop writes 8 flags into a 4-byte buffer (Platinum's double
      battle: GetBattlerTargetFlags smashed its caller's frame, SIGSEGV in the target menu, round ptleg4).

  python3 tools/plat_lint.py [--no-cache] [--all] [-j N]    # exit 0: nothing outside the allow-list

Hits that are harmless are listed in tools/plat_lint_allow.txt (check, path, the source line's text, reason: tab-separated;
no line numbers, so a patch that moves the line doesn't break the row; a text starting with ~ is a regex, a path of * any file). The clang pass runs the port's own line
(tools/pokeplat_cc.py) over every file once (about a minute with the pool; MB of memory); its findings are cached in
build/plat_lint_cache.json keyed by the tree's stamp, the port's headers and this file, so an unchanged tree answers at once.
"""
import argparse, collections, hashlib, json, multiprocessing, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
PLAT = os.path.join(ROOT, 'build/pokeplatinum')
CACHE = os.path.join(ROOT, 'build/plat_lint_cache.json')
ALLOW = os.path.join(ROOT, 'tools/plat_lint_allow.txt')
FLAGS = ['-Wno-everything', '-Wcast-align', '-Wpointer-to-int-cast', '-Wint-to-pointer-cast', '-Wint-conversion', '-Wshorten-64-to-32',
         '-Wsizeof-pointer-div', '-Wsizeof-array-argument', '-Wsizeof-pointer-memaccess',
         '-fno-diagnostics-color', '-fno-caret-diagnostics', '-ferror-limit=0', '-fsyntax-only']
SKIP_WORDS = {'cast', 'from', 'to', 'const', 'increases', 'required', 'alignment', 'aka', 'struct', 'unsigned', 'char', 'int'}
HEAP_FNS = ('Heap_Alloc', 'Heap_AllocAtEnd')
HEADER = re.compile(r'^(.+?):(\d+):(\d+): (warning|error|fatal error|note): (.*)$')
ELEM_SIZE = {'u8': 1, 's8': 1, 'char': 1, 'unsigned char': 1, 'u16': 2, 's16': 2, 'short': 2, 'unsigned short': 2,
             'u32': 4, 's32': 4, 'int': 4, 'unsigned int': 4, 'BOOL': 4, 'fx32': 4}


def parse_diags(text):
    """clang's -fno-caret-diagnostics output -> one dict per warning/error: path, line, col, flag, msg, null (the
    diagnostic's notes say the expression is the NULL macro). Notes attach to the diagnostic before them."""
    out = []
    for l in text.splitlines():
        m = HEADER.match(l)
        if not m:
            continue
        kind = m.group(4)
        if kind == 'note':
            if out and "expanded from macro 'NULL'" in m.group(5):
                out[-1]['null'] = True
            continue
        msg = m.group(5)
        f = re.search(r' \[(-W[\w-]+)\]$', msg)
        out.append({'path': m.group(1), 'line': int(m.group(2)), 'col': int(m.group(3)), 'kind': kind,
                    'flag': f.group(1) if f else '', 'msg': re.sub(r' \[-W[\w-]+\]$', '', msg), 'null': False})
    return out


def classify(d):
    """The check a diagnostic belongs to (C1, C3, C4) or None."""
    f, msg = d['flag'], d['msg']
    if f == '-Wcast-align':
        m = re.search(r"cast from '(.*?)'(?: \(aka '.*?'\))? to '(.*?)'(?: \(aka '.*?'\))? increases required alignment from (\d+) to (\d+)", msg)
        if not m or m.group(4) != '8':
            return None
        return 'C3' if m.group(2).rstrip().endswith('**') else 'C1'
    if f in ('-Wpointer-to-int-cast', '-Wint-to-pointer-cast', '-Wint-to-void-pointer-cast'):
        return 'C4'
    if f in ('-Wsizeof-pointer-div', '-Wsizeof-array-argument', '-Wsizeof-pointer-memaccess'):
        return 'C5'
    if f == '-Wshorten-64-to-32' and re.search(r"loses integer precision: '(?:u|s)?intptr_t'", msg):
        return 'C4'                                   # a pointer-sized integer (a cast pointer) narrowed to 32 bits
    if f == '-Wint-conversion':
        if d['null'] or re.search(r"'(?:u|s)?intptr_t'|'(?:unsigned )?long(?: long)?'|'size_t'", msg):
            return None                               # NULL as 0, or a pointer to an integer as wide as it
        return 'C4'
    return None


# C3's text scans (HGSS hg24/ptr4.py, hg25/stride4.py): a pointer at a 4-mod-8 byte offset; a pointer array with a 4-byte stride
PTR4 = re.compile(r'\*\s*\(\s*(?:const\s+)?[A-Za-z_][\w ]*?\*\s*\*\s*\)\s*\(\s*(?:\(\s*(?:u8|char|unsigned char|s8)\s*\*\s*\)\s*)?\(?\s*'
                  r'[A-Za-z_][\w.\->]*\s*\)?\s*\+\s*(0x[0-9A-Fa-f]+|\d+)\s*\)')
STRIDE4 = [re.compile(r'\*\s*\(\s*(?:const\s+)?[A-Za-z_][\w ]*?\*\s*\*\s*\)\s*\([^;]*?\*\s*4\b'),
           re.compile(r'\*\s*\(\s*(?:const\s+)?[A-Za-z_][\w ]*?\*\s*\*\s*\)\s*\([^;]*?\b4\s*\*\s*\(?\w')]


def text_hits(line):
    """C3 messages for one source line."""
    out = []
    for m in PTR4.finditer(line):
        if int(m.group(1), 0) % 8 == 4:
            out.append('pointer at byte offset %s (4 mod 8)' % m.group(1))
    if any(p.search(line) for p in STRIDE4):
        out.append('pointer array walked with a 4-byte stride')
    return out


def scan_text(files):
    out = []
    for p in files:
        for n, l in enumerate(open(p, errors='replace'), 1):
            for msg in text_hits(l):
                out.append({'check': 'C3', 'path': p, 'line': n, 'msg': msg})
    return out


def eval_size(tokens):
    """A size argument made of numeric literals and operators only -> its value, else None."""
    s = ' '.join(tokens)
    if not re.fullmatch(r'[\s0-9a-fA-FxXuUlL()+*<>|&-]*', s) or not re.search(r'\d', s):
        return None
    s = re.sub(r'\b(0[xX][0-9a-fA-F]+|\d+)[uUlL]*\b', r'\1', s)
    try:
        return int(eval(s, {'__builtins__': {}}, {}))
    except Exception:
        return None


def heap_calls(path, args):
    """C2 in one file with libclang: [(line, literal, dest type name or None, host sizeof or None)] for every
    Heap_Alloc/Heap_AllocAtEnd whose size is a literal; dest = the pointee of the variable, cast or return it goes to."""
    import clang.cindex as ci
    K = ci.CursorKind
    tu = ci.Index.create().parse(path, args=args)
    res, ret = [], [None]

    def pointee(t):
        t = t.get_canonical() if t is not None else None
        if t is None or t.kind != ci.TypeKind.POINTER:
            return None
        return t.get_pointee().get_canonical()

    def visit(n, dest):
        if n.kind == K.CALL_EXPR and n.spelling in HEAP_FNS:
            a = list(n.get_arguments())
            if len(a) == 2:
                lit = eval_size([t.spelling for t in a[1].get_tokens()])
                if lit is not None and a[1].location.file and a[1].location.file.name == path:
                    p = pointee(dest)
                    rec = p is not None and p.kind == ci.TypeKind.RECORD
                    res.append((n.location.line, lit, p.spelling if rec else ('buffer' if p is not None else None),
                                p.get_size() if rec else None))
        kids = list(n.get_children())
        for i, c in enumerate(kids):
            if c.location.file and c.location.file.name != path and n.kind == K.TRANSLATION_UNIT:
                continue
            if n.kind == K.VAR_DECL:
                visit(c, n.type)
            elif n.kind == K.CSTYLE_CAST_EXPR:
                visit(c, n.type)
            elif n.kind == K.BINARY_OPERATOR and i == 1 and len(kids) == 2:
                visit(c, kids[0].type)
            elif n.kind in (K.UNEXPOSED_EXPR, K.PAREN_EXPR):
                visit(c, dest)
            elif n.kind == K.RETURN_STMT:
                visit(c, ret[0])
            elif n.kind == K.FUNCTION_DECL:
                ret[0] = n.result_type
                visit(c, None)
            else:
                visit(c, None)

    visit(tu.cursor, None)
    return res


def sizeof_names(path, args, names):
    """name -> host sizeof for typedef or struct names, in the file's translation unit."""
    import clang.cindex as ci
    tu = ci.Index.create().parse(path, args=args)
    out = {}
    for c in tu.cursor.get_children():
        if c.spelling in names and c.kind in (ci.CursorKind.TYPEDEF_DECL, ci.CursorKind.STRUCT_DECL):
            t = c.underlying_typedef_type if c.kind == ci.CursorKind.TYPEDEF_DECL else c.type
            if t.get_size() > 0:
                out[c.spelling] = t.get_size()
    return out


def heap_job(j):
    return j[0], heap_calls(j[0], j[1])


def size_job(j):
    return j[0], sizeof_names(j[0], j[1], set(j[2]))


def array_fits(d, size_of):
    """C1: the diagnostic's source is a pointer to an array: True when the array's bytes cover the host struct,
    False when not, None when the source is no array or a size is unknown."""
    m = re.search(r"cast from '(?:const )?([\w ]+?) \(\*\)\[(\d+)\]'.* to '(?:const )?(?:struct )?(\w+) \*'", d['msg'])
    if not m:
        return None
    es = ELEM_SIZE.get(m.group(1)) or size_of.get(m.group(1))
    host = size_of.get(m.group(3))
    return None if not es or not host else es * int(m.group(2)) >= host


def source_text(path, line, cache={}, strip=True):
    if path not in cache:
        try:
            cache[path] = open(path, errors='replace').read().splitlines()
        except OSError:
            cache[path] = []
    ls = cache[path]
    t = ls[line - 1] if 0 < line <= len(ls) else ''
    return t.strip() if strip else t


def rel(p):
    p = os.path.normpath(p)
    for base in (PLAT, ROOT):
        if p.startswith(base + os.sep):
            return os.path.relpath(p, base)
    return p


def clang_args():
    """The port's line for a Platinum file without its forced include of port_plat.h (the nitro/nnsys/global_pch headers
    every file shares: parsed once into a precompiled header, then each file takes -include-pch instead)."""
    import pokeplat_cc
    a = [x for x in pokeplat_cc.plat_args() if not x.startswith('-W') and x not in ('-ferror-limit=0', '-fno-diagnostics-color')]
    i = a.index('-include')
    assert a[i + 1] == 'port_plat.h'
    return a[:i] + a[i + 2:]


def one(job):
    src, args = job
    r = subprocess.run(['clang'] + args + FLAGS + [src], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                       errors='replace', cwd=PLAT)
    return src, r.stdout


def make_pch(args):
    """port_plat.h compiled once to build/plat_lint.pch (its own diagnostics come with it); returns (pch path, output)."""
    pch = os.path.join(ROOT, 'build/plat_lint.pch')
    hdr = os.path.join(ROOT, 'port/include_plat/port_plat.h')
    r = subprocess.run(['clang'] + args + [x for x in FLAGS if x != '-fsyntax-only'] + ['-x', 'c-header', hdr, '-o', pch],
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace', cwd=PLAT)
    if not os.path.exists(pch):
        sys.exit('plat_lint: the precompiled header failed:\n' + r.stdout[-2000:])
    return pch, r.stdout


def cache_key():
    h = hashlib.sha1(open(__file__, 'rb').read())
    h.update(open(os.path.join(PLAT, '.bwport-stamp'), 'rb').read())
    for d in ('port/include_plat', 'port/include_gen4', 'port/include', 'lib/nnsys/include', 'port/subprojects/libntr/include',
              'build/pokeplatinum/src', 'build/pokeplatinum/include', 'build/pokeplatinum/lib', 'build/pokeplatinum/gen'):
        for dp, _, fn in sorted(os.walk(os.path.join(ROOT, d))):
            for f in sorted(fn):
                s = os.stat(os.path.join(dp, f))
                h.update(('%s %d %d\n' % (os.path.join(dp, f), s.st_size, s.st_mtime_ns)).encode())
    return h.hexdigest()


def findings(jobs, use_cache=True):
    """All hits before the allow-list: [{check, path (relative), line, code, msg, extra}]; cached."""
    key = cache_key()
    if use_cache and os.path.exists(CACHE):
        c = json.load(open(CACHE))
        if c.get('key') == key:
            return c['hits'], c['bad'], True
    import pokeplat_gen
    pokeplat_gen.tree()
    import pokeplat_cc
    args = clang_args()
    files = pokeplat_cc.sources()
    pch, head = make_pch(args)
    with multiprocessing.get_context('fork').Pool(jobs) as p:
        res = p.map(one, [(f, args + ['-include-pch', pch]) for f in files], chunksize=4)
    res.append((os.path.join(ROOT, 'port/include_plat/port_plat.h'), head))
    args = args + ['-include', 'port_plat.h']    # libclang (another version than the clang binary) can't read the pch
    bad = [rel(s) for s, o in res if 'fatal error' in o]
    seen, diags = set(), []
    for _, o in res:
        for d in parse_diags(o):
            k = (d['path'], d['line'], d['col'], d['flag'], d['msg'])
            if k not in seen:
                seen.add(k)
                diags.append(d)
    hits = []
    for d in diags:
        if d['flag'] == '-Wint-conversion' and "'void *'" in d['msg'] and re.search(r'\bNULL\b', source_text(d['path'], d['line'])):
            d['null'] = True       # NULL used as 0 (clang's macro note isn't always printed, its column is the declarator's)
        c = classify(d)
        if c:
            d['check'] = c
            hits.append(d)
    # C1: pointer-to-array sources are judged by size (libclang, one parse per file that has such a cast)
    names = collections.defaultdict(set)
    for d in hits:
        if d['check'] == 'C1' and re.search(r"\(\*\)\[\d+\]", d['msg']):
            names[d['path']] |= set(re.findall(r"[A-Za-z_]\w*", d['msg'])) - SKIP_WORDS
    with multiprocessing.get_context('fork').Pool(jobs) as p:
        size_of = dict(p.map(size_job, [(f, args, sorted(n)) for f, n in names.items()]))
    keep = []
    for d in hits:
        if d['check'] == 'C1' and d['path'] in size_of:
            fits = array_fits(d, size_of[d['path']])
            if fits:
                continue
            if fits is False:
                d['msg'] += ' [array too small for the host struct]'
        keep.append(d)
    out = []
    for d in keep:
        out.append({'check': d['check'], 'path': rel(d['path']), 'line': d['line'], 'code': source_text(d['path'], d['line']),
                    'msg': d['msg']})
    # C2: the files that have a Heap_Alloc with a digit-only size
    cand = []
    for f in files:
        t = open(f, errors='replace').read()
        if re.search(r'\bHeap_Alloc(?:AtEnd)?\s*\([^;]*,\s*[0-9][^;,]*\)\s*;', t):
            cand.append(f)
    with multiprocessing.get_context('fork').Pool(jobs) as p:
        calls = p.map(heap_job, [(f, args + ['-w']) for f in cand])
    for f, cs in calls:
        for line, lit, dest, host in cs:
            if dest is None or (host is not None and lit < host):
                out.append({'check': 'C2', 'path': rel(f), 'line': line, 'code': source_text(f, line),
                            'msg': ('literal size %d, destination type unknown' % lit) if dest is None
                            else 'literal size %d < host sizeof(%s) = %d' % (lit, dest, host)})
    # C3's text scan, over the same files
    for h in scan_text(files):
        out.append({'check': 'C3', 'path': rel(h['path']), 'line': h['line'], 'code': source_text(h['path'], h['line']),
                    'msg': h['msg']})
    for h in out:
        h['code'] = re.sub(r'\s+', ' ', h['code'])
    json.dump({'key': key, 'hits': out, 'bad': bad}, open(CACHE, 'w'))
    return out, bad, False


def load_allow(path=ALLOW):
    rows = {}
    if os.path.exists(path):
        for l in open(path):
            if l.strip() and not l.startswith('#'):
                f = l.rstrip('\n').split('\t')
                if len(f) >= 4:
                    rows[(f[0], f[1], re.sub(r'\s+', ' ', f[2]))] = f[3]
    return rows


def find_allow(h, allow):
    """The allow-list row for a hit: the exact (check, path, code) or a regex row (code starting with '~', path '*' for any)."""
    k = (h['check'], h['path'], h['code'])
    if k in allow:
        return k
    for (c, p, code) in allow:
        if c == h['check'] and p in ('*', h['path']) and code.startswith('~') and re.search(code[1:], h['code']):
            return (c, p, code)
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--no-cache', action='store_true')
    ap.add_argument('--all', action='store_true', help='list the allowed hits too')
    ap.add_argument('-j', type=int, default=int(os.environ.get('BW_JOBS', '8')))
    o = ap.parse_args()
    hits, bad, cached = findings(o.j, not o.no_cache)
    allow = load_allow()
    used, left, n = set(), [], collections.Counter()
    for h in hits:
        k = find_allow(h, allow)
        n[h['check']] += 1
        if k:
            used.add(k)
            if o.all:
                print('allowed  %s %s:%d: %s   (%s)' % (h['check'], h['path'], h['line'], h['code'][:100], allow[k][:60]))
        else:
            left.append(h)
    for h in sorted(left, key=lambda h: (h['check'], h['path'], h['line'])):
        print('%s %s:%d: %s\n      %s' % (h['check'], h['path'], h['line'], h['msg'][:200], h['code'][:160]))
    stale = [k for k in allow if k not in used]
    for k in stale:
        print('stale allow-list row (no such hit any more): %s %s: %s' % (k[0], k[1], k[2][:80]))
    for p in bad:
        print('NOT PARSED (fatal error): %s' % p)
    allowed = sum(1 for h in hits if find_allow(h, allow))
    print('plat_lint: hits %s (%d); allowed %d; left %d; stale rows %d; unparsed files %d; %s' % (
        ' '.join('%s=%d' % (c, n[c]) for c in ('C1', 'C2', 'C3', 'C4')), sum(n.values()), allowed, len(left), len(stale),
        len(bad), 'cached' if cached else 'fresh'))
    return 1 if left or stale or bad else 0


if __name__ == '__main__':
    sys.exit(main())
