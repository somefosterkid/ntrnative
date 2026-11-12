#!/usr/bin/env python3
"""tools/pokeplat_cc.py: compile pret pokeplatinum's C (build/pokeplatinum, tools/pokeplat_fetch.sh) as 64-bit host C
with the port's line for a game file (libntr's and lib/nnsys's headers and defines, from build/port/linux's
compile_commands.json), and classify the failures (docs/platinum.md, "The failure table").

  python3 tools/pokeplat_cc.py [--out DIR] [--obj] [FILES...]   # default: every src/**/*.c and lib/{spl,gds,crypto}

--obj compiles to objects in DIR/obj (for a link); the default is -fsyntax-only. Per-file logs in DIR/log/, the
summary in DIR/summary.txt (files OK / failed, errors by category and by normalized message).
"""
import argparse, collections, json, multiprocessing, os, re, shlex, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PLAT = os.path.join(ROOT, 'build/pokeplatinum')


def base_args():
    """The port's clang line for one Black game file, minus Black's own include dirs, prefix and sources."""
    cc = json.load(open(os.path.join(ROOT, 'build/port/linux/compile_commands.json')))
    ent = next(e for e in cc if e['file'].endswith('src/system/main.c') and 'pokebw2' not in e['file'])
    a = shlex.split(ent['command'])
    out, skip = [], False
    i = 1
    while i < len(a):
        x = a[i]
        if x in ('-o', '-MF', '-MQ', '-c'):
            i += 2 if x != '-c' else 2
            continue
        if x in ('-include', '-idirafter', '-isystem', '-I'):
            v = a[i + 1]
            out.append((x, v))
            i += 2
            continue
        out.append((x,))
        i += 1
    keep = []
    d = ent['directory']
    for t in out:
        x = t[0]
        if x == '-include':
            continue                       # Black's prefix (port_prefix.h ...): Platinum has its own
        if x.startswith('-I') or x in ('-idirafter', '-isystem', '-I'):
            p = t[1] if len(t) > 1 else x[2:]
            p = os.path.normpath(os.path.join(d, p))
            # libntr's and lib/nnsys's headers stay; Black's include/, lib/*/include (but nnsys), port/include go
            if ('subprojects' in p or '/lib/nnsys' in p or 'include_nnsys' in p) and 'pokebw2' not in p:
                keep += [x if len(t) > 1 else '-I', p] if len(t) > 1 else ['-I' + p]
            continue
        if x.startswith('-D') or x.startswith('-U') or x.startswith('-f') or x.startswith('-m') or x.startswith('-std') \
                or x.startswith('-W') or x.startswith('-O') or x.startswith('-g') or x.startswith('--target'):
            if x.startswith("-Werror") or x.startswith("-fdiagnostics-color") or x.startswith("-DBWPORT_") or x == '-DBWPORT_ENTRY':
                continue
            keep.append(x)
    return keep


def sources():
    s = []
    for top in ['src', 'lib/spl/src', 'lib/gds/src', 'lib/crypto/src']:
        for dp, dn, fn in os.walk(os.path.join(PLAT, top)):
            s += [os.path.join(dp, f) for f in fn if f.endswith('.c')]
    return sorted(s)


def plat_args():
    a = ['-I' + os.path.join(ROOT, 'port/include_plat'), '-I' + os.path.join(ROOT, 'port/include_gen4')]
    a += base_args()
    for d in ['include', 'asm', 'build', '.', 'lib/spl/include', 'lib/gds/include', 'lib/crypto/include',
              'lib/ppwlobby/include', 'build/res/text', 'build/generated']:
        a.append('-I' + os.path.join(PLAT, d))
    # a few files include a .naix by its bare name (pret's meson gives each such target its dir)
    dirs = set()
    for dp, dn, fn in os.walk(os.path.join(PLAT, 'build/res')):
        if any(f.endswith('.naix') for f in fn) and '.p' not in os.path.basename(dp):
            dirs.add(dp)
    a += ['-idirafter' + d for d in sorted(dirs)]
    a += ['-DPOKEPLATINUM_GENERATED_ENUM', '-DPM_KEEP_ASSERTS', '-DGAME_VERSION=VERSION_PLATINUM',
          '-DGAME_LANGUAGE=LANGUAGE_ENGLISH', '-DPOKEPLATINUM_REVISION=1', '-fsigned-char',
          '-idirafter', os.path.join(ROOT, 'port/include'),
          # Platinum's DWC, NitroWiFi and libvct headers (pret's subprojects, fetched by its meson setup): the online
          # side is stubbed, as Black's; only the declarations are needed
          '-idirafter', os.path.join(PLAT, 'subprojects/NitroDWC-2.2.30008/include'),
          '-idirafter', os.path.join(PLAT, 'subprojects/NitroDWC-2.2.30008/include/gs'),
          '-idirafter', os.path.join(PLAT, 'subprojects/NitroDWC-2.2.30008/include/base'),
          '-idirafter', os.path.join(PLAT, 'subprojects/NitroWiFi-2.1.30003/include'),
          '-idirafter', os.path.join(PLAT, 'subprojects/libvct-1.3.1/include'),   # (Black's types.h, which lib/nnsys's headers include)
          '-include', 'port_plat.h', '-ferror-limit=0', '-fno-diagnostics-color', '-Wno-everything',
          '-Wint-to-pointer-cast', '-Wpointer-to-int-cast', '-Wshorten-64-to-32', '-Wincompatible-pointer-types',
          '-Wimplicit-function-declaration', '-Wint-conversion', '-Wno-error']
    return a


ARGS = None
OUT = None
OBJ = False


def one(src):
    rel = os.path.relpath(src, PLAT)
    log = os.path.join(OUT, 'log', rel + '.log')
    os.makedirs(os.path.dirname(log), exist_ok=True)
    cmd = ['clang'] + ARGS + [src]
    if OBJ:
        o = os.path.join(OUT, 'obj', rel + '.o')
        os.makedirs(os.path.dirname(o), exist_ok=True)
        cmd += ['-c', '-o', o]
    else:
        cmd += ['-fsyntax-only']
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace', cwd=PLAT)
    open(log, 'w').write(r.stdout)
    return rel, r.returncode, r.stdout


CATS = [
    ('pointer size', r'(int-to-pointer|pointer-to-int|cast to smaller integer|incompatible integer to pointer|'
                     r'incompatible pointer to integer|static_assert|static assertion)'),
    ('inline asm / ARM-only', r'(\basm\b|__asm|invalid instruction|unknown register|register variable)'),
    ('generated header / res', r"'(res|generated)/[^']*' file not found"),
    ('missing header', r"file not found"),
    ('SDK/NNS undeclared name', r'(undeclared identifier|implicit declaration|unknown type name|undeclared function)'),
    ('SDK/NNS signature/struct', r'(too (few|many) arguments|no member named|incompatible pointer types|'
                                 r'conflicting types|redefinition|passing .* to parameter|incomplete)'),
]


def cat(msg):
    for n, p in CATS:
        if re.search(p, msg):
            return n
    return 'other'


def norm(m):
    m = re.sub(r"'[^']*'", "'X'", m)
    return re.sub(r'\d+', 'N', m)


def main():
    global ARGS, OUT, OBJ
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default='build/pokeplat_cc')
    ap.add_argument('--obj', action='store_true')
    ap.add_argument('--print-args', action='store_true')
    ap.add_argument('files', nargs='*')
    o = ap.parse_args()
    ARGS, OUT, OBJ = plat_args(), o.out, o.obj
    if o.print_args:
        print(' '.join(ARGS)); return
    files = [os.path.abspath(f) for f in o.files] or sources()
    os.makedirs(OUT, exist_ok=True)
    with multiprocessing.get_context('fork').Pool(int(os.environ.get('BW_JOBS', '8'))) as p:
        res = p.map(one, files, chunksize=4)
    ok = [r for r in res if r[1] == 0]
    bad = [r for r in res if r[1] != 0]
    bycat, bycat_files, bymsg, first = collections.Counter(), collections.defaultdict(set), collections.Counter(), {}
    for rel, rc, txt in res:
        for line in txt.splitlines():
            m = re.match(r'(.*?):(\d+):\d+: (?:fatal )?(error|warning): (.*)', line)
            if not m:
                continue
            if m.group(3) == 'warning' and 'pokeplatinum' not in os.path.abspath(os.path.join(PLAT, m.group(1))):
                continue
            m = (m, m.group(1), m.group(2), ('W ' if m.group(3) == 'warning' else '') + m.group(4))
            m = type('M', (), {'group': lambda self, i, m=m: m[i]})()
            c = cat(m.group(3))
            bycat[c] += 1
            bycat_files[c].add(rel)
            k = norm(m.group(3))
            bymsg[(c, k)] += 1
            first.setdefault((c, k), '%s:%s: %s' % (os.path.relpath(m.group(1), PLAT) if m.group(1).startswith('/') else m.group(1), m.group(2), m.group(3)))
    with open(os.path.join(OUT, 'summary.txt'), 'w') as f:
        f.write('files %d ok %d failed %d\n\n' % (len(res), len(ok), len(bad)))
        for c, n in bycat.most_common():
            f.write('%-28s %6d errors in %4d files\n' % (c, n, len(bycat_files[c])))
        f.write('\n')
        for (c, k), n in bymsg.most_common(150):
            f.write('%6d  [%s] %s\n        e.g. %s\n' % (n, c, k[:160], first[(c, k)][:200]))
        f.write('\nFAILED FILES\n' + '\n'.join(r[0] for r in bad) + '\n')
    print(open(os.path.join(OUT, 'summary.txt')).read()[:3000])


if __name__ == '__main__':
    main()
