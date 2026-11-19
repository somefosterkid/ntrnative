#!/usr/bin/env python3
"""Game switching in one process through the launcher, with the rows' fills compared (port/src/asset_load.c).

    python3 tools/launcher_switch_test.py [--exe EXE] [--hops N] [GAME ...]   # default: heartgold soulsilver, 2 hops

A fresh app root (/tmp/launcher_switch), hidden Xvfb: the launcher imports each game's ROM (orig/), plays the first, and
cfgscript `games=1` every 6000 frames goes back to it, which plays the next (BWPORT_LAUNCHER_AGAIN="game=next;play"): with
the default, HeartGold, SoulSilver, HeartGold. BWPORT_ASSET_DUMP records every fill of the rows' tables; the test passes when
each game filled its rows each time it started and every fill of a game holds the bytes of its first (HeartGold and
SoulSilver share the tables, so HG after SS must fill them again), and the process ran every hop without a crash.
Prints one line per fill and per check, PASS or FAIL; the log and the shots go to the root's out/.
"""
import argparse
import os
import shutil
import subprocess
import sys
import time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'tools'))
import port_run  # noqa: E402

ROMS = {'heartgold': 'baserom_hg_us.nds', 'soulsilver': 'baserom_ss_us.nds', 'platinum': 'baserom_plat_us.nds',
        'black': 'baserom_b_us.nds', 'white': 'baserom_w_us.nds', 'b2': 'baserom_b2_us.nds', 'w2': 'baserom_w2_us.nds'}
CODES = {'heartgold': 'IPKE', 'soulsilver': 'IPGE', 'platinum': 'CPUE', 'black': 'IRBO', 'white': 'IRAO', 'b2': 'IREO',
         'w2': 'IRDO'}
ORDER = ['black', 'white', 'b2', 'w2', 'platinum', 'heartgold', 'soulsilver']   # the launcher table's (its list's order)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('games', nargs='*', default=['heartgold', 'soulsilver'])
    ap.add_argument('--exe', default=os.path.join(REPO, 'build/port/linux/bwport'))
    ap.add_argument('--hops', type=int, default=2, help='Game > Games this many times (default 2)')
    ap.add_argument('--root', default='/tmp/launcher_switch')
    ap.add_argument('--secs', type=int, default=900)
    a = ap.parse_args()
    games = sorted(a.games, key=ORDER.index)
    exe = os.path.abspath(a.exe)   # (the run's cwd is the root)
    root, out = a.root, os.path.join(a.root, 'out')
    dump = os.path.join(out, 'dump.txt')
    shutil.rmtree(root, ignore_errors=True)
    os.makedirs(out)
    imp = ';'.join(f'importrom={REPO}/orig/{ROMS[g]};idle' for g in games)
    cfg = 'f1:ff=1;f6000:shot=t;p200:games=1;' + 'p6000:shot=t;p200:games=1;' * (a.hops - 1) + 'p6000:shot=t;p200:quit=1'
    xvfb, display = port_run.start_xvfb()
    env = dict(os.environ, DISPLAY=display, SDL_VIDEODRIVER='x11', WAYLAND_DISPLAY='', SDL_AUDIODRIVER='dummy',
               BWPORT_FRAMESKIP='4', BWPORT_BENCH_CLOCK='2026-10-29T12:00:00', BWPORT_BENCH_SEED='100',
               BWPORT_MUSICNORMAL='off', BWPORT_ASSET_DUMP=dump,
               BWPORT_LAUNCHER_TEST=f'layout=library;{imp};wait=1;game=0;play',
               BWPORT_LAUNCHER_AGAIN='wait=1;game=next;play', BWPORT_CFGSCRIPT=cfg)
    for k in ('BWPORT_ROM', 'BWPORT_DIR', 'BWPORT_GAME'):
        env.pop(k, None)
    t0 = time.time()
    log = os.path.join(out, 'run.log')
    try:
        with open(log, 'w') as f:
            rc = subprocess.run([exe], cwd=root, env=env, stdout=f, stderr=subprocess.STDOUT, timeout=a.secs,
                                stdin=subprocess.DEVNULL).returncode
    except subprocess.TimeoutExpired:
        rc = 'timeout'
    finally:
        xvfb.kill()
    print(f'launcher_switch_test: rc={rc} {time.time() - t0:.0f} s, log {log}')
    text = open(log, errors='replace').read()
    for line in text.splitlines():
        if line.startswith('assets:') or 'CRASH' in line or 'HANG' in line:
            print('  ' + line[:240])
    # the dump: one block per fill, its tables in registration order (a row's name can be several tables: the first
    # table's name again starts the next fill of the same game)
    blocks = []
    if os.path.exists(dump):
        for line in open(dump):
            code, name, _, data = line.split(' ', 3)
            if not blocks or blocks[-1][0] != code or name == blocks[-1][2]:
                blocks.append((code, {}, name))
            blocks[-1][1][(name, len(blocks[-1][1]))] = data
    blocks = [(c, d) for c, d, _ in blocks]
    want = [CODES[games[i % len(games)]] for i in range(a.hops + 1)]
    fails = []
    got = [c for c, _ in blocks]
    print(f'  fills {" ".join(f"{c}({len(d)})" for c, d in blocks)}; the games played {" ".join(want)}')
    if got != want:
        fails.append(f'the fills {got} are not the games played {want}')
    first = {}
    for i, (c, d) in enumerate(blocks):
        if c not in first:
            first[c] = d
        elif first[c] != d:
            fails.append(f'fill {i} of {c} differs from its first')
        else:
            print(f'  fill {i} {c}: the bytes of its first fill ({len(d)} tables)')
    codes = list(first)
    for i in range(len(codes)):
        for j in range(i + 1, len(codes)):
            da, db = first[codes[i]], first[codes[j]]
            shared = [n for n in da if n in db]   # (the same row at the same place in both games' registrations)
            print(f'  {codes[i]} vs {codes[j]}: {len(shared)} tables in both, {sum(da[n] != db[n] for n in shared)} hold other bytes')
    if rc != 0 or 'CRASH' in text:
        fails.append(f'the process ended with {rc}')
    for d, _, fs in os.walk(root):
        for f in fs:
            if f.endswith('.png') and d != out and 'cover' not in f and 'artwork' not in f and 'icon' not in f:
                shutil.copy(os.path.join(d, f), os.path.join(out, os.path.relpath(os.path.join(d, f), root).replace('/', '_').replace(' ', '')))
    print('\n'.join('FAIL ' + f for f in fails) or 'PASS')
    return 1 if fails else 0


if __name__ == '__main__':
    sys.exit(main())
