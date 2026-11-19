#!/usr/bin/env python3
"""hg_int_sweep.py: the HG/SS interaction sweep: on every map, every object event (a person), BG event (sign, hidden item,
plain) and coord event (a trigger tile) is used once: the player is warped next to it facing it, A is pressed (a trigger tile:
the step onto it), then A / B pulses run the script to its end. One row per (map, event). Crash finding only, as hg_sweep
(the debug warp is never used in routes or milestones of the save library). docs/hgss.md "Interaction sweep".

    python3 tools/hg_int_sweep.py --maps all -j 6 --out DIR --heapcheck          # every MAP_ constant (541)
    python3 tools/hg_int_sweep.py --maps 60-70,33 -j 3 --out DIR                 # ids, ranges
    python3 tools/hg_int_sweep.py --summary --out DIR                            # only (re)write the summary files

Per map one exe run (tools/port_run.py, the reload's keys, BWPORT_HG_WARP onto the map's first tile, then BWPORT_HG_INT in
port/game/route_gen4.c: the events in order, BWPORT_HG_INT_LOG's lines). A run ends at the segment's end (--maxn events), at a
battle's first move menu, at a script that never lets the field go (budget), or at a crash / hang / heap overrun; the next run
starts at the following event, so a map's rows are complete whatever happened. Resumable: events already in sweep.tsv are not run
again (--redo runs the maps again).

Row verdicts: ok (a script ran and the field was free again), no-script (A / the step began nothing: the person moved, the sign
faces the other way, the variable did not fire), absent (the person is not on the map: its flag), collected (a hidden item that is
gone), no-spot (no free tile next to it), battle (a battle reached its first move menu), timeout (the script still ran at the
budget), crash, hang (no frame for 30 s), heap, stub, wall-timeout (the run ended with no exit line, e.g. at the frame cap
maxn * budget + 6000; there is no wall watchdog, a slow run is never killed: tools/sweep_wall.py), warp-timeout. `ran` lists the
script ids the event began
(the first is the event's own; later ones are scripts it started).
Out dir: sweep.tsv, crash_groups.tsv (by top function and kind, with the script ids), summary.txt, maps/<id>/ (int_N.txt, bt.txt,
log.txt.gz for a crash)."""
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

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import hg_sweep as S  # noqa: E402
import sweep_wall  # noqa: E402  (no wall watchdog)

REPO = S.REPO
HEAD = ['map', 'name', 'ev', 'kind', 'x', 'z', 'script', 'verdict', 'ran', 'frame', 'top', 'bt_id', 'wall_s', 'note']
KINDS = {0: 'npc', 1: 'bg', 2: 'coord'}
BGSUB = {0: 'bg', 1: 'sign', 2: 'hidden'}


def kind_name(k, sub):
    return BGSUB.get(sub, 'bg') if k == 1 else KINDS.get(k, '?')


def skip_reason(mid, name):
    """maps the sweep does not run: comm / elevator / entered-wrong (hg_sweep.ENTERED_WRONG)"""
    if mid in S.ENTERED_WRONG:
        return S.ENTERED_WRONG[mid].split(':')[0]
    if 'ELEVATOR' in name:
        return 'elevator'
    return None


def parse_int_log(path):
    """{events: {i: (kind, sub, x, z, script, a, b, c)}, n, begun: [i], ends: {i: (frame, verdict, scripts, map)},
    scripts: {i: [(frame, id)]}, press: {i: frame}, exit: (frame, why) or None}"""
    r = {'n': None, 'events': {}, 'begun': [], 'ends': {}, 'scripts': collections.defaultdict(list), 'press': {}, 'exit': None}
    try:
        lines = open(path, errors='replace').read().splitlines()
    except OSError:
        return r
    for line in lines:
        p = line.split()
        if not p:
            continue
        try:
            if p[0] == 'count':
                r['n'] = int(p[1])
            elif p[0] == 'ev':
                r['events'][int(p[1])] = tuple(int(v) for v in p[2:])
            elif p[0] == 'begin':
                r['begun'].append(int(p[1]))
            elif p[0] == 'press':
                r['press'][int(p[1])] = int(p[2])
            elif p[0] == 'script':
                r['scripts'][int(p[1])].append((int(p[2]), int(p[3])))
            elif p[0] == 'end':
                r['ends'][int(p[1])] = (int(p[2]), p[3], p[4] if len(p) > 4 else '', p[5] if len(p) > 5 else '')
            elif p[0] == 'exit':
                r['exit'] = (int(p[1]), p[2] if len(p) > 2 else '')
        except (ValueError, IndexError):
            continue
    return r


class Rows:
    """sweep.tsv: append-only, read back on start (resume)"""
    def __init__(self, path):
        self.path = path
        self.lock = threading.Lock()
        self.rows = []
        if os.path.exists(path):
            self.rows = [l.rstrip('\n').split('\t') for l in open(path)][1:]
        else:
            with open(path, 'w') as f:
                f.write('\t'.join(HEAD) + '\n')

    def add(self, row):
        row = [str(v) for v in row]
        with self.lock:
            self.rows.append(row)
            with open(self.path, 'a') as f:
                f.write('\t'.join(row) + '\n')

    def drop_maps(self, ids):
        with self.lock:
            self.rows = [r for r in self.rows if int(r[0]) not in ids]
            with open(self.path, 'w') as f:
                f.write('\t'.join(HEAD) + '\n')
                f.writelines('\t'.join(r) + '\n' for r in self.rows)

    def map_state(self, mid):
        """(n or None, done event ids, finished) for map MID: the map-level row (ev -1) holds n in its note"""
        n, done, fin = None, set(), False
        for r in self.rows:
            if int(r[0]) != mid:
                continue
            ev = int(r[2])
            if ev == -1:
                m = re.match(r'n=(\d+)', r[13])
                if m:
                    n = int(m.group(1))
                if r[7] in ('skipped', 'no-field', 'no-events', 'map-crash'):
                    fin = True
            else:
                done.add(ev)
        if n is not None and len(done) >= n:
            fin = True
        return n, done, fin


def run_segment(a, mid, tile, slot, start):
    """one exe run from event START of MID. Returns (parsed int log, verdict line, crash tuple or None, log text, wall s)"""
    d = os.path.join(a.out, 'maps', str(mid))
    os.makedirs(d, exist_ok=True)
    run_dir = os.path.join(a.out, 'run', f'w{slot}')
    out_dir = os.path.join(run_dir, 'out')
    int_log = os.path.join(d, f'int_{start}.txt')
    env = dict(os.environ, BWPORT_BUILD=a.build, BWPORT_HG_WARP=f'{mid}:{tile[0]}:{tile[1]}',
               BWPORT_HG_WARP_LOG=os.path.join(d, 'warp.txt'), BWPORT_HG_INT=f'{start}:{a.budget}:{a.maxn}',
               BWPORT_HG_INT_LOG=int_log, PYTHONDONTWRITEBYTECODE='1', BWPORT_KEYS=a.reload)
    if a.no_force:
        env['BWPORT_HG_INT_NOFORCE'] = '1'
    if a.no_show:
        env['BWPORT_HG_INT_NOSHOW'] = '1'
    if a.heapcheck is not None:
        env['BWPORT_HEAPCHECK'] = a.heapcheck
    cmd = ['python3', os.path.join(REPO, 'tools/port_run.py'), '--game', a.game, '--no-build', '--save', a.save,
           '--dir', run_dir, '--out', out_dir, '-t', sweep_wall.NO_WALL_CAP, '--frames', str(a.maxn * a.budget + sweep_wall.END_SLACK),
           '--shots', '0', '--hang', '30', '--hang-stop',
           '--skip', str(a.skip)]
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
    shutil.rmtree(out_dir, ignore_errors=True)
    return parse_int_log(int_log), line, log, wall


def run_map(a, mid, name, tile, slot, rows):
    n, done, fin = rows.map_state(mid)
    d = os.path.join(a.out, 'maps', str(mid))
    start = 0
    segs = 0
    wall_total = 0
    while True:
        # the first event not done
        start = next((i for i in range(0, (n if n is not None else 1 << 30)) if i not in done), None) if n is not None else 0
        if start is None:
            return
        log_i, line, log, wall = run_segment(a, mid, tile, slot, start)
        wall_total += wall
        segs += 1
        ev = log_i['events']
        if n is None and log_i['n'] is not None:
            n = log_i['n']
            rows.add([mid, name, -1, '', '', '', '', 'map', '', '', '', '', f'{wall:.0f}', f'n={n}'])
        crash_verdict = None
        top = text = bid = ''
        if line.startswith('heapcheck:'):
            crash_verdict = 'heap'
            mo = re.search(r'heap (?:overrun|misuse)( after \S+| \(ow\wer [^)]*\))', line)
            top = 'heap overrun' + (mo.group(1) if mo else '')
            text = line
            bid = hashlib.sha1(top.encode()).hexdigest()[:8]
        elif line.startswith('crash'):
            crash_verdict = 'crash'
            top, text, bid = S.crash_info(log)
        elif line.startswith('hang'):
            crash_verdict = 'hang'
            top, text, bid = sweep_wall.frozen_top(log)
        elif line.startswith('harness'):   # port_run itself died (sweep_wall.verdict_line): not a game finding
            crash_verdict, top, text = 'harness', line[:100], line
        elif line.startswith('stub'):
            crash_verdict = 'stub'
            top = line.split()[1]
            text = line
        progressed = False
        for i, (fr, verdict, scripts, mp) in sorted(log_i['ends'].items()):
            if i in done or i not in ev:
                continue
            k, sub, x, z, script = ev[i][:5]
            v = 'battle' if verdict.startswith('battle') else verdict
            pf = log_i['press'].get(i, 0)
            ran = [str(sid) for f, sid in log_i['scripts'].get(i, []) if f >= pf]
            note = ''
            if v == 'ok' and not ran:
                note = 'free again, no script logged'
            rows.add([mid, name, i, kind_name(k, sub), x, z, script, v, ','.join(ran), fr, '', '', f'{wall:.0f}', f'{scripts} {mp} {note}'.strip()])
            done.add(i)
            progressed = True
        # the event the run died in: the last begun with no end
        open_ev = [i for i in log_i['begun'] if i not in done]
        if open_ev:
            i = open_ev[-1]
            if i in ev:
                k, sub, x, z, script = ev[i][:5]
                pf = log_i['press'].get(i, 0)
                ran = [str(sid) for f, sid in log_i['scripts'].get(i, []) if f >= pf]
                fr = re.search(r'frame (\d+)', line)
                v = crash_verdict or ('wall-timeout' if log_i['exit'] is None else 'warp-timeout')
                if log_i['exit'] is not None and log_i['exit'][1] in ('timeout', 'stuck-before-warp', 'warp-timeout'):
                    v = log_i['exit'][1]
                rows.add([mid, name, i, kind_name(k, sub), x, z, script, v, ','.join(ran), fr.group(1) if fr else '', top, bid,
                          f'{wall:.0f}', line[:120] if v in ('wall-timeout',) else ''])
                done.add(i)
                progressed = True
                if crash_verdict:
                    os.makedirs(d, exist_ok=True)
                    with gzip.open(os.path.join(d, f'log_{i}.txt.gz'), 'wt') as f:
                        f.write(log)
                    open(os.path.join(d, f'bt_{i}.txt'), 'w').write(line + '\n' + text + '\n')
        if n is None:   # the run never listed the events: the landing failed or the run died before it
            why = crash_verdict or (log_i['exit'][1] if log_i['exit'] else 'no-field')
            rows.add([mid, name, -1, '', '', '', '', 'map-crash' if crash_verdict else 'no-field', '', '', top, bid, f'{wall:.0f}', f'{why} {line[:100]}'])
            if crash_verdict:
                with gzip.open(os.path.join(d, 'log_map.txt.gz'), 'wt') as f:
                    f.write(log)
                open(os.path.join(d, 'bt_map.txt'), 'w').write(line + '\n' + text + '\n')
            return
        if n == 0:
            rows.add([mid, name, -1, '', '', '', '', 'no-events', '', '', '', '', f'{wall:.0f}', 'n=0'])
            return
        if all(i in done for i in range(n)):
            return
        if not progressed or segs > 400:   # (a run that finished nothing, twice: give up on the rest)
            if not progressed:
                i = next(i for i in range(n) if i not in done)
                k, sub, x, z, script = ev[i][:5] if i in ev else (0, 0, 0, 0, 0)
                rows.add([mid, name, i, kind_name(k, sub), x, z, script, 'no-progress', '', '', top, bid, f'{wall:.0f}', line[:120]])
                done.add(i)


def summarize(a, maps):
    path = os.path.join(a.out, 'sweep.tsv')
    rows = [l.rstrip('\n').split('\t') for l in open(path)][1:] if os.path.exists(path) else []
    ev_rows = [r for r in rows if int(r[2]) >= 0]
    verdicts = collections.Counter(r[7] for r in ev_rows)
    kinds = collections.Counter(r[3] for r in ev_rows)
    map_rows = [r for r in rows if int(r[2]) == -1]
    mv = collections.Counter(r[7] for r in map_rows)
    nmaps = len({r[0] for r in rows})
    reached = sum(1 for r in ev_rows if r[8])
    groups = collections.OrderedDict()
    for r in ev_rows + [m for m in map_rows if m[7] == 'map-crash']:
        if r[7] in ('crash', 'hang', 'stub', 'heap', 'map-crash'):
            kind = 'crash' if r[7] == 'map-crash' else r[7]
            g = groups.setdefault((kind, r[10]), {'rows': [], 'bt': ''})
            g['rows'].append(r)
            if not g['bt']:
                for bt in (f'bt_{r[2]}.txt', 'bt_map.txt'):
                    try:
                        g['bt'] = open(os.path.join(a.out, 'maps', r[0], bt)).read().split('\n')[1]
                        break
                    except (OSError, IndexError):
                        pass
    order = sorted(groups.items(), key=lambda kv: -len(kv[1]['rows']))
    with open(os.path.join(a.out, 'crash_groups.tsv'), 'w') as f:
        f.write('top_function\tkind\tcount\tmaps\tevent_kinds\tfirst (map ev script)\tscript_ids\tbacktrace\n')
        for (kind, top), g in order:
            rs = g['rows']
            f.write(f'{top}\t{kind}\t{len(rs)}\t{len({r[0] for r in rs})}\t'
                    + ','.join(f'{k}:{n}' for k, n in collections.Counter(r[3] for r in rs).most_common())
                    + f'\t{rs[0][0]} {rs[0][1]} ev {rs[0][2]} script {rs[0][6]}\t'
                    + ','.join(sorted({r[6] for r in rs if r[6]}, key=int)[:40]) + f'\t{g["bt"]}\n')
    lines = [f'hg_int_sweep: {nmaps} maps, {len(ev_rows)} event rows, save {a.save}, budget {a.budget} V-blanks, '
             f'heapcheck {a.heapcheck if a.heapcheck is not None else "off"}',
             'rows: ' + ', '.join(f'{k} {v}' for k, v in verdicts.most_common()),
             'event kinds: ' + ', '.join(f'{k} {v}' for k, v in kinds.most_common()),
             f'rows that ran a script: {reached}',
             'maps: ' + ', '.join(f'{k} {v}' for k, v in mv.most_common()), '',
             'count  kind   top function   (maps; first)']
    for (kind, top), g in order:
        rs = g['rows']
        lines.append(f'{len(rs):5}  {kind:5}  {top}   ({len({r[0] for r in rs})} maps; {rs[0][0]} {rs[0][1]} ev {rs[0][2]} {rs[0][3]} script {rs[0][6]})')
    open(os.path.join(a.out, 'summary.txt'), 'w').write('\n'.join(lines) + '\n')
    print('\n'.join(lines[:40]))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--maps', default='all', help='all | ids and ranges: 60-70,33')
    ap.add_argument('--out', required=True)
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('--save', default='hg-r30c', help='the save (from the save library) the runs load (a party: battles)')
    ap.add_argument('--game', default='heartgold')
    ap.add_argument('--build', default='build/port/linux-hgss')
    ap.add_argument('--budget', type=int, default=1500, help="V-blanks per event from the press (the script's time)")
    ap.add_argument('--maxn', type=int, default=40, help='events per exe run')
    ap.add_argument('--skip', type=int, default=4, help="port_run's --skip (frame skip)")
    sweep_wall.ignore_t(ap)
    ap.add_argument('--reload', default=S.RELOAD, help='the keys from the save to the free field (BWPORT_KEYS items)')
    ap.add_argument('--no-force', action='store_true', help="a coord event's variable stays as it is (default: set to the value that fires it)")
    ap.add_argument('--no-show', action='store_true', help='a person hidden by its event flag stays hidden (default: shown for its row)')
    ap.add_argument('--heapcheck', nargs='?', const='1', default=None, metavar='LEVEL', help='BWPORT_HEAPCHECK=LEVEL (default 1)')
    ap.add_argument('--redo', action='store_true', help='run the maps again')
    ap.add_argument('--redo-verdicts', metavar='V,V', help='run again the maps with a row of these verdicts (crash,heap,hang ...: after a re-pin), from the rows already in the out dir')
    ap.add_argument('--summary', action='store_true', help='only write the summary files')
    ap.add_argument('--keep-run', action='store_true')
    a = ap.parse_args()
    a.out = os.path.abspath(a.out)
    os.makedirs(os.path.join(a.out, 'maps'), exist_ok=True)
    maps = S.read_maps()
    if a.summary:
        summarize(a, maps)
        return
    rows = Rows(os.path.join(a.out, 'sweep.tsv'))
    todo = S.parse_maps(a.maps, maps)
    if a.redo:
        rows.drop_maps(set(todo))
    if a.redo_verdicts:
        want = set(a.redo_verdicts.split(','))
        again = {int(r[0]) for r in rows.rows if r[7] in want or (int(r[2]) == -1 and r[7] in want)}
        todo = [m for m in todo if m in again]
        rows.drop_maps(set(todo))
    todo = [m for m in todo if not rows.map_state(m)[2]]
    slots = queue.Queue()
    for i in range(a.j):
        slots.put(i)
    lock = threading.Lock()
    t0 = time.time()
    cnt = [0]

    def job(mid):
        name, _code, events = maps.get(mid, ('?', '', None))
        why = skip_reason(mid, name)
        tile = S.spawn(events) if events and not why else None
        if why:
            rows.add([mid, name, -1, '', '', '', '', 'skipped', '', '', '', '', '0', why])
        elif tile is None:
            rows.add([mid, name, -1, '', '', '', '', 'no-events', '', '', '', '', '0', 'no event to stand on'])
        else:
            slot = slots.get()
            try:
                run_map(a, mid, name, tile, slot, rows)
            except Exception as e:   # (the tool's own fault: a row, not a dead sweep)
                rows.add([mid, name, -1, '', '', '', '', 'tool-error', '', '', '', '', '0', repr(e)[:150]])
            finally:
                slots.put(slot)
        with lock:
            cnt[0] += 1
            n, done, fin = rows.map_state(mid)
            mine = collections.Counter(r[7] for r in rows.rows if int(r[0]) == mid and int(r[2]) >= 0)
            print(f'[{cnt[0]}/{len(todo)} {time.time() - t0:.0f}s] {mid} {name}: n={n} {dict(mine)}', flush=True)

    with concurrent.futures.ThreadPoolExecutor(a.j) as ex:
        list(ex.map(job, todo))
    if not a.keep_run:
        shutil.rmtree(os.path.join(a.out, 'run'), ignore_errors=True)
    summarize(a, maps)


if __name__ == '__main__':
    main()
