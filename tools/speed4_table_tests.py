#!/usr/bin/env python3
"""speed4_table_tests.py: tools/speed4_table.sh never deletes a caller's exported OUT and leaves no temp dirs behind (pfx1 11-25, crv4 #11)."""
import os, shutil, subprocess, sys, tempfile

root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
with tempfile.TemporaryDirectory() as d:
    repo, tmp, out = f'{d}/repo', f'{d}/tmp', f'{d}/keepme'
    for p in (f'{repo}/tools', tmp, out):
        os.makedirs(p)
    shutil.copy(f'{root}/tools/speed4_table.sh', f'{repo}/tools/')
    open(f'{repo}/tools/speed4_measure.sh', 'w').write('echo "row $1 $4$5 out=${OUT:-none}"\n')   # a fake measure: one line per run
    open(f'{out}/sentinel', 'w').write('x')
    subprocess.run(['git', 'init', '-q', repo], check=True)
    env = dict(os.environ, TMPDIR=tmp, OUT=out, PAR='3', HOME=f'{d}/home')
    r = subprocess.run(['bash', 'tools/speed4_table.sh'], cwd=repo, env=env, capture_output=True, text=True)
    fail = []
    if r.returncode != 0:
        fail.append(f'rc={r.returncode} {r.stderr[-200:]}')
    if not os.path.exists(f'{out}/sentinel'):
        fail.append('the caller\'s OUT was deleted')
    if os.listdir(tmp):
        fail.append(f'temp dirs left: {os.listdir(tmp)}')
    rows = [l for l in r.stdout.splitlines() if l.startswith('row ')]
    if len(rows) != 18 or any(f'out={out}' not in l for l in rows):
        fail.append(f'rows: {len(rows)}; OUT not passed on')
    env.pop('OUT')
    r = subprocess.run(['bash', 'tools/speed4_table.sh'], cwd=repo, env=env, capture_output=True, text=True)
    dflt = f'{d}/home/.cache/bwport/speed4/table.'   # without OUT: the table's own per-run dir, left in place for its logs
    if len([l for l in r.stdout.splitlines() if l.startswith('row ') and f'out={dflt}' in l]) != 18 or os.listdir(tmp):
        fail.append('without OUT: rows wrong or temp dirs left')
print('speed4_table_tests:', 'FAIL ' + '; '.join(fail) if fail else 'ok')
sys.exit(1 if fail else 0)
