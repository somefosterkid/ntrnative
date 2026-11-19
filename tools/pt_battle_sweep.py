#!/usr/bin/env python3
"""pt_battle_sweep.py: the Platinum battle sweep: every species as the wild opponent (pass A), every move used by our
Pokemon (pass B), through the test battle (port/game/route_gen4.c Pr4_TestBattle; docs/platinum.md "Battle sweep").
Crash finding only: throwaway runs from a library save, nothing is stored. HeartGold / SoulSilver's twin is
tools/hg_battle_sweep.py on the hgss branch (same out files, same verdicts).

    python3 tools/pt_battle_sweep.py --pass A --ids all -j 6 --out DIR --heapcheck     # species 1-493 (or 1-40,151)
    python3 tools/pt_battle_sweep.py --pass B --ids all -j 6 --out DIR --heapcheck     # moves 1-467, 4 per battle
    python3 tools/pt_battle_sweep.py --pass B1 --ids 33,52 --out DIR                    # moves one per battle
    python3 tools/pt_battle_sweep.py --summary --out DIR

Per battle: tools/port_run.py --game platinum --no-build --save pt-pgrunt (the reload's keys: pt_sweep.RELOAD), then the A
pulse every 20 V-blanks from f1700 (messages, the action menu's FIGHT, the move menu), BWPORT_PT_TESTBATTLE at the first free
field frame and BWPORT_PT_TESTEXIT: the exe exits 0 at the test's end. --frames is the game-frame cap (`exit-frame` = cap),
--hang 30 a stalled run (gdb's backtrace: no frame for 30 s); no wall watchdog, a slow run is never killed (tools/sweep_wall.py).
- A: our Mew L100 (Splash, Tackle, Shadow Ball, Psychic: slot 0 of the save's party is replaced) against species N at L50 with
  its own moves; BWPORT_PT_TESTSLOT=stop: the run ends at the first move menu (the send-out, the cry, both sprites, the intro
  messages, the action menu). --a-turns: pass A2 = cycle with BWPORT_TESTCHEAT=win instead (Splash on turn 1: the opponent's
  own move animation; then our hits one-shot it).
- B: our Mew L10 with moves 4k+1..4k+4 against Blissey L100 that knows only Splash; BWPORT_PT_TESTSLOT=cycle (each move once,
  then the end) and BWPORT_PT_TESTHIT (ours always hit, the damage stays real: Blissey outlives four moves). B1: one move
  per battle.
Verdicts: ok (the test's end), crash (signal: the top 6 frames), hang (no frame for 30 s), heap (--heapcheck), cap (the frame
cap: still in the battle), no-battle (the battle never started), timeout (any other end). A crash's move: the last `test: move` line.
Out (resumable; --redo): sweep.tsv, crash_groups.tsv (top function, first id, count, backtrace, kind, all ids), summary.txt
(the pin), runs/<pass>-<id>/ (bt.txt, log.txt.gz for crash / hang / heap / cap).
"""
import argparse
import collections
import concurrent.futures
import gzip
import hashlib
import os
import queue
import re
import shutil
import subprocess
import sys
import threading
import time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'tools'))
import pt_sweep  # noqa: E402  (RELOAD, crash_info)
import sweep_wall  # noqa: E402  (no wall watchdog)

MEW, BLISSEY, SPLASH = 151, 242, 150
A_MOVES = '150,33,247,94'   # Splash, Tackle, Shadow Ball (Ghost: Spiritomb, Shedinja), Psychic
MAX_SPECIES, MAX_MOVE = 493, 467
PULSE_FROM = 1700   # the first free field frame is ~f1656: no A in the field before the battle starts


def battles(pas, ids):
    """[(id, spec, env)] for a pass"""
    out = []
    if pas == 'A':
        for n in ids:
            out.append((n, f'{MEW}:100:{A_MOVES}:{n}/50', {'BWPORT_PT_TESTSLOT': 'stop'}))
    elif pas == 'A2':
        for n in ids:
            out.append((n, f'{MEW}:100:{A_MOVES}:{n}/50', {'BWPORT_PT_TESTSLOT': 'cycle', 'BWPORT_TESTCHEAT': 'win'}))
    elif pas == 'B':
        for k in sorted({(m - 1) // 4 for m in ids}):
            mv = [m for m in range(4 * k + 1, 4 * k + 5) if m <= MAX_MOVE]
            out.append((k, f'{MEW}:10:{",".join(map(str, mv))}:{BLISSEY}/100:{SPLASH}',
                        {'BWPORT_PT_TESTSLOT': 'cycle', 'BWPORT_PT_TESTHIT': '1'}))
    elif pas == 'B1':
        for m in ids:
            out.append((m, f'{MEW}:10:{m}:{BLISSEY}/100:{SPLASH}', {'BWPORT_PT_TESTSLOT': 'cycle', 'BWPORT_PT_TESTHIT': '1'}))
    return out


def run_one(a, pas, bid, spec, extra, slot):
    d = os.path.join(a.out, 'runs', f'{pas}-{bid}')
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
    run_dir = os.path.join(a.out, 'run', f'w{slot}')
    out_dir = os.path.join(run_dir, 'out')
    pulses = ' '.join(f'a@f{f}' for f in range(PULSE_FROM, a.frames, 20))
    env = dict(os.environ, BWPORT_BUILD=a.build, BWPORT_PT_TESTBATTLE=spec, BWPORT_PT_TESTEXIT='1', PYTHONDONTWRITEBYTECODE='1',
               BWPORT_KEYS=pt_sweep.RELOAD + ' ' + pulses, **extra)
    env.pop('BWPORT_PILOT', None)
    if 'BWPORT_TESTCHEAT' not in extra:
        env.pop('BWPORT_TESTCHEAT', None)
    cmd = ['python3', os.path.join(REPO, 'tools/port_run.py'), '--game', a.game, '--no-build', '--save', a.save, '--dir', run_dir,
           '--out', out_dir, '--frames', str(a.frames), '-t', sweep_wall.NO_WALL_CAP, '--shots', '0', '--hang', '30',
           '--hang-stop',
           '--skip', str(a.skip)] + (['--cheat'] if 'BWPORT_TESTCHEAT' in extra else ['--no-cheat'])
    if a.heapcheck:
        cmd += ['--heapcheck', a.heapcheck]
    t0 = time.time()
    r = subprocess.run(['nice', '-n', '10'] + cmd, env=env, capture_output=True, text=True, cwd=REPO)
    wall = time.time() - t0
    line = (r.stdout.strip().splitlines() or ['?'])[-1]
    try:
        line = sweep_wall.verdict_line(r, out_dir)
    except OSError:
        pass
    try:
        log = open(os.path.join(out_dir, 'log.txt'), errors='replace').read()
    except OSError:
        log = r.stdout + r.stderr
    tests = re.findall(r'^test: .*$', log, re.M)
    moves = re.findall(r'^test: move slot \d+ move (\d+)', log, re.M)
    done = any(t.startswith(('test: moves done', 'test: battle over')) for t in tests)
    started = any(t.startswith('test: wild battle') for t in tests)
    m = re.search(r'frame (\d+)', line)
    frame = m.group(1) if m else ''
    top = text = btid = ''
    if line.startswith('heapcheck:'):
        verdict = 'heap'
        mo = re.search(r'heap (?:overrun|misuse)( after \S+| \(owner [^)]*\))', line)
        top = 'heap overrun' + (mo.group(1) if mo else '')
        text = line
    elif line.startswith('crash'):
        verdict = 'crash'
        top, text, btid = pt_sweep.crash_info(log)
    elif line.startswith('hang'):
        verdict = 'hang'
        top, text, btid = sweep_wall.frozen_top(log)
    elif line.startswith('harness'):   # port_run itself died (sweep_wall.verdict_line): not a game finding
        verdict, top, text = 'harness', line[:100], line
    elif done:
        verdict = 'ok'
        mo = [t for t in tests if t.startswith(('test: moves done', 'test: battle over'))][-1]
        frame = re.search(r'V-blank (\d+)', mo).group(1)
    elif not started:
        verdict = 'no-battle'
    elif line.startswith('exit-frame'):
        verdict = 'cap'
        top, text, btid = sweep_wall.stuck_top(log)   # frames ran to the cap, the battle never ended: the exe's state line
    else:
        verdict = 'timeout'
        top = line[:80]
    if not btid and top:
        btid = hashlib.sha1(top.encode()).hexdigest()[:8]
    if verdict != 'ok':
        with gzip.open(os.path.join(d, 'log.txt.gz'), 'wt') as f:
            f.write(log)
    open(os.path.join(d, 'bt.txt'), 'w').write(line + '\n' + text + '\n' + '\n'.join(tests) + '\n')
    shutil.rmtree(out_dir, ignore_errors=True)
    return [pas, str(bid), spec, verdict, frame, top, btid, ','.join(moves), moves[-1] if moves and verdict != 'ok' else '',
            f'{wall:.0f}'], text


HEAD = ['pass', 'id', 'spec', 'verdict', 'frame', 'top', 'bt_id', 'moves_chosen', 'last_move', 'wall_s']


def pin():
    try:
        return subprocess.run(['git', '-C', os.path.join(REPO, 'third_party/pokeplatinum'), 'rev-parse', '--short=9', 'HEAD'],
                              capture_output=True, text=True).stdout.strip()
    except OSError:
        return '?'


def summarize(a):
    path = os.path.join(a.out, 'sweep.tsv')
    rows = [l.rstrip('\n').split('\t') for l in open(path)][1:] if os.path.exists(path) else []
    groups = collections.OrderedDict()
    for r in rows:
        if r[3] not in ('ok',):
            g = groups.setdefault((r[3], r[5]), {'ids': [], 'bt': '', 'ex': ''})
            g['ids'].append(f'{r[0]}{r[1]}')
            if not g['bt']:
                try:
                    g['bt'] = open(os.path.join(a.out, 'runs', f'{r[0]}-{r[1]}', 'bt.txt')).read().split('\n')[1]
                except (OSError, IndexError):
                    pass
                g['ex'] = f'{r[0]}{r[1]} {r[2]}' + (f' move {r[8]}' if r[8] else '')
    order = sorted(groups.items(), key=lambda kv: -len(kv[1]['ids']))
    with open(os.path.join(a.out, 'crash_groups.tsv'), 'w') as f:
        f.write('top_function\tfirst (pass id spec move)\tcount\tbacktrace\tsave\tkind\tall_ids\n')
        for (kind, top), g in order:
            f.write(f'{top}\t{g["ex"]}\t{len(g["ids"])}\t{g["bt"]}\t{a.save}\t{kind}\t{",".join(g["ids"])}\n')
    lines = [f'pt_battle_sweep at pokeplatinum pin {pin()} (exe {a.build}), save {a.save}, frame cap {a.frames}'
             + (f', heapcheck {a.heapcheck}' if a.heapcheck else '')]
    for pas in sorted({r[0] for r in rows}):
        v = collections.Counter(r[3] for r in rows if r[0] == pas)
        lines.append(f'pass {pas}: {sum(v.values())} battles; ' + ', '.join(f'{k} {n}' for k, n in v.most_common()))
    lines += ['', 'count  kind   top function   (first: pass+id spec [move]; ids)']
    for (kind, top), g in order:
        lines.append(f'{len(g["ids"]):5}  {kind:5}  {top}   ({g["ex"]}; {",".join(g["ids"][:12])}{" ..." if len(g["ids"]) > 12 else ""})')
    open(os.path.join(a.out, 'summary.txt'), 'w').write('\n'.join(lines) + '\n')
    print('\n'.join(lines[:40]))


def parse_ids(spec, top):
    if spec == 'all':
        return list(range(1, top + 1))
    out = []
    for part in spec.split(','):
        if '-' in part:
            lo, hi = part.split('-')
            out += range(int(lo), int(hi) + 1)
        elif part:
            out.append(int(part))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--pass', dest='pas', default='A', choices=['A', 'A2', 'B', 'B1'])
    ap.add_argument('--ids', default='all', help='species (A) or moves (B, B1): all | 1-40,151')
    ap.add_argument('--out', required=True)
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('--save', default='pt-pgrunt')
    ap.add_argument('--game', default='platinum')
    ap.add_argument('--build', default=pt_sweep.BUILD)
    ap.add_argument('--frames', type=int, default=0, help='the game-frame cap (default A 4000, A2 9000, B 12000, B1 6000)')
    ap.add_argument('--skip', type=int, default=4)
    sweep_wall.ignore_t(ap)
    ap.add_argument('--heapcheck', nargs='?', const='1', default=None, metavar='LEVEL')
    ap.add_argument('--redo', action='store_true')
    ap.add_argument('--summary', action='store_true')
    a = ap.parse_args()
    pt_sweep.exe_or_exit(a)
    a.out = os.path.abspath(a.out)
    a.frames = a.frames or {'A': 4000, 'A2': 9000, 'B': 12000, 'B1': 6000}[a.pas]
    os.makedirs(os.path.join(a.out, 'runs'), exist_ok=True)
    if a.summary:
        summarize(a)
        return
    path = os.path.join(a.out, 'sweep.tsv')
    done = {}
    if os.path.exists(path):
        for l in list(open(path))[1:]:
            p = l.split('\t')
            done[(p[0], p[1])] = l
    todo = [b for b in battles(a.pas, parse_ids(a.ids, MAX_SPECIES if a.pas.startswith('A') else MAX_MOVE))
            if a.redo or (a.pas, str(b[0])) not in done]
    if a.redo:
        done = {k: v for k, v in done.items() if (k[0], k[1]) not in {(a.pas, str(b[0])) for b in todo}}
    with open(path, 'w') as f:
        f.write('\t'.join(HEAD) + '\n')
        f.writelines(done[k] for k in sorted(done, key=lambda k: (k[0], int(k[1]))))
    slots = queue.Queue()
    for i in range(a.j):
        slots.put(i)
    lock = threading.Lock()
    t0 = time.time()
    n = [0]

    def job(b):
        slot = slots.get()
        try:
            row, _ = run_one(a, a.pas, b[0], b[1], b[2], slot)
        finally:
            slots.put(slot)
        with lock:
            with open(path, 'a') as f:
                f.write('\t'.join(row) + '\n')
            n[0] += 1
            print(f'[{n[0]}/{len(todo)} {time.time() - t0:.0f}s] {a.pas}{b[0]} {b[1]}: {row[3]} {row[4]} {row[5]} {row[8]}', flush=True)

    with concurrent.futures.ThreadPoolExecutor(a.j) as ex:
        list(ex.map(job, todo))
    shutil.rmtree(os.path.join(a.out, 'run'), ignore_errors=True)
    summarize(a)


if __name__ == '__main__':
    main()
