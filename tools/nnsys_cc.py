#!/usr/bin/env python3
"""Compile every lib/nnsys C file with a game file's PC compile line, libntrsystem's include dirs swapped for
lib/nnsys/include, the pointer-width warnings on (docs/nnsys-port.md: the inventory of what the PC build of our
NitroSystem needs). Run from the repo root after a Linux port build (build/port/linux/compile_commands.json).

    python3 tools/nnsys_cc.py -Iport/include_nnsys -include port/include_nnsys/nnsys_lib.h -ferror-limit=0

The extra arguments go to clang (the ones above are what the PC build of lib/nnsys adds). Prints ok/FAIL and the
error/warning counts per file; every diagnostic goes to $NNSYS_CC_OUT/all.txt (default build/nnsys_cc/all.txt).
The 64-bit spots: grep all.txt for pointer-to-int-cast, int-to-pointer-cast, int-to-void-pointer-cast.
"""
import json, os, re, shlex, subprocess, sys, multiprocessing
from pathlib import Path

ROOT = Path.cwd()
OUT = Path(os.environ.get('NNSYS_CC_OUT', 'build/nnsys_cc')); OUT.mkdir(parents=True, exist_ok=True)
cc = json.load(open(ROOT / 'build/port/linux/compile_commands.json'))
ent = next(e for e in cc if '/src/system/' in e['file'] and e['file'].endswith('.c'))
args = shlex.split(ent['command'])
new = []
skip = False
for a in args:
    if skip:
        skip = False; continue
    if a in ('-o', '-MF', '-MQ', '-c'):
        skip = True; continue
    if a == ent['file'] or a.endswith(ent['file']):
        continue
    if 'libntrsystem' in a:
        continue
    if a.startswith('-MD') or a == '-w' or a.startswith('-Wno-'):
        continue
    new.append(a)
extra = [str(ROOT / a) if a.startswith('port/') else ('-I' + str(ROOT / a[2:]) if a.startswith('-Iport/') else a)
         for a in sys.argv[1:]]
new += ['-I' + str(ROOT / 'lib/nnsys/include'), '-Wno-macro-redefined', '-Wpointer-to-int-cast', '-Wint-to-pointer-cast',
        '-Wint-conversion', '-Wincompatible-pointer-types', '-Wimplicit-function-declaration'] + extra
files = sorted(str(p) for p in (ROOT / 'lib/nnsys/src').rglob('*.c'))


def one(f):
    r = subprocess.run(new + ['-fno-color-diagnostics', '-c', f, '-o', '/dev/null'], cwd=ent['directory'],
                       capture_output=True, text=True)
    return f, r.returncode, r.stderr


with multiprocessing.get_context('fork').Pool(int(os.environ.get('BW_JOBS', '8'))) as p:
    res = p.map(one, files)
with open(OUT / 'all.txt', 'w') as log:
    for f, rc, err in res:
        log.write(f'=== {f} rc={rc}\n{err}\n')
        e = len(re.findall(r': error:', err)); w = len(re.findall(r': warning:', err))
        print(f'{"FAIL" if rc else "ok  "} e={e:3d} w={w:3d} {os.path.relpath(f, ROOT)}')
print(f'{sum(1 for r in res if not r[1])} of {len(res)} compile; diagnostics in {OUT / "all.txt"}')
