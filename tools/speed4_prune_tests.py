#!/usr/bin/env python3
"""speed4_prune_tests.py: tools/speed4_prune.sh keeps the newest 5 runs and drops what is older than 2 days, leaves runs in progress
(touched < 1 h ago), and speed4_table.sh prunes its default dir but never a caller's OUT."""
import os, shutil, subprocess, sys, tempfile, time

root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
fail = []


def mk(d, name, age_h, kind='dir'):
    p = f'{d}/{name}'
    if kind == 'dir':
        os.makedirs(p)
        open(f'{p}/big', 'w').write('x')
    else:
        open(p, 'w').write('x')
    t = time.time() - age_h * 3600
    os.utime(p, (t, t))


def names(d):
    return sorted(os.listdir(d))


with tempfile.TemporaryDirectory() as d:
    k = f'{d}/speed4'
    os.makedirs(k)
    for i in range(8):   # 8 runs, 3h .. 10h old: the newest 5 stay, with their .run/.log
        mk(k, f'run_t{i}', 3 + i)
        mk(k, f't{i}.run', 3 + i, 'file'); mk(k, f't{i}.log', 3 + i, 'file')
    mk(k, 'run_old', 80)             # older than 2 days: gone although it would be a 9th
    mk(k, 'table.1', 0.2)            # in progress: kept
    mk(k, 'run_live', 0.1)           # in progress: kept
    mk(k, 'stray.log', 100, 'file')  # loose old file
    r = subprocess.run(['bash', f'{root}/tools/speed4_prune.sh', k], capture_output=True, text=True)
    got = names(k)
    want = sorted(['run_live', 'table.1'] + [f'run_t{i}' for i in range(3)] + [f't{i}.{e}' for i in range(3) for e in ('run', 'log')])
    # newest 5 by mtime: run_live(0.1h), table.1(0.2h), run_t0..t2 (3..5h); the rest (t3..t7, old) goes
    if r.returncode != 0 or got != want:
        fail.append(f'prune: rc={r.returncode} got {got} want {want}')
    # a dir that does not exist is fine
    if subprocess.run(['bash', f'{root}/tools/speed4_prune.sh', f'{d}/none']).returncode != 0:
        fail.append('missing dir')
    # speed4_table.sh: its default dir is pruned, a caller's OUT is not
    repo, tmp, out, home = f'{d}/repo', f'{d}/tmp', f'{d}/keepme', f'{d}/home'
    for p in (f'{repo}/tools', tmp, out, f'{home}/.cache/bwport/speed4'):
        os.makedirs(p)
    for f in ('speed4_table.sh', 'speed4_prune.sh'):
        shutil.copy(f'{root}/tools/{f}', f'{repo}/tools/')
    open(f'{repo}/tools/speed4_measure.sh', 'w').write('echo "row $1"\n')
    subprocess.run(['git', 'init', '-q', repo], check=True)
    for base in (out, f'{home}/.cache/bwport/speed4'):
        mk(base, 'run_ancient', 100)
    env = dict(os.environ, TMPDIR=tmp, PAR='3', HOME=home)
    env.pop('OUT', None)
    subprocess.run(['bash', 'tools/speed4_table.sh'], cwd=repo, env=env, capture_output=True, text=True)
    if os.path.exists(f'{home}/.cache/bwport/speed4/run_ancient'):
        fail.append('table without OUT did not prune its default dir')
    mk(f'{home}/.cache/bwport/speed4', 'run_ancient', 100)
    subprocess.run(['bash', 'tools/speed4_table.sh'], cwd=repo, env=dict(env, OUT=out), capture_output=True, text=True)
    if not os.path.exists(f'{out}/run_ancient'):
        fail.append("a caller's OUT was pruned")
    if os.path.exists(f'{home}/.cache/bwport/speed4/run_ancient') is False:
        fail.append('a caller OUT run pruned the default dir (should not touch anything)')
print('FAIL: ' + '; '.join(fail) if fail else 'speed4_prune_tests: ok')
sys.exit(1 if fail else 0)
