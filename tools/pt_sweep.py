#!/usr/bin/env python3
"""pt_sweep.py: the Platinum crash sweep: the player on every map, a walk there, every crash ahead of the story (docs/platinum.md
"Crash sweep"; the HG/SS twin is tools/hg_sweep.py on the hgss branch). Crash finding only: the debug warp is never used in
tools/routes chains or for the save library milestones.

    python3 tools/pt_sweep.py --maps all -j 6 --out /tmp/ptsweep       # every map header (593)
    python3 tools/pt_sweep.py --maps 60-70,411 -j 3 --out DIR --heapcheck   # ids, ranges; the heap checker on
    python3 tools/pt_sweep.py --summary --out DIR                           # only (re)write the summary files

Per map: tools/port_run.py --game platinum --no-build from a library save (--save, default pt-pgrunt), the reload's keys (START at
the title, A on CONTINUE), then BWPORT_PT_WARP=MAP:X:Z:END: at the first free field frame port/game/route_gen4.c warps the player
there with the game's own warp (ScrCmd_Warp's: tile X,Z = the map's first warp event, else an object's (the tile below it), a BG
event's or a coord event's; res/field/maps/<map>/events.json), then a walk seeded by the map id (directions held 1-3 tiles, A and
B presses) for --frames V-blanks, and the exe exits 0 at END. No wall watchdog (a slow run is never killed, never a finding): the
frame cap END + 6000 ends a run still in a battle or a script (verdict exit); --hang 30 (no frame for 30 s) ends a stalled run
with gdb's backtrace (tools/sweep_wall.py).
Verdicts: ok (END reached on the field), crash (signal: the top 6 frames), heap (--heapcheck: an exp heap was damaged; the top
column names the allocator of the block before the damage), hang, stub, no-field (the warp's map never came up free and nothing
crashed), no-spawn (the map has no event to stand on), exit (the frame cap: a battle or a script was still running at END),
timeout (any other end).

Out dir (resumable: maps already in sweep.tsv are skipped; --redo runs them again):
    sweep.tsv          one line per map: map id, name, verdict, frame, top function, backtrace id, last zone, zones, wall s
    crash_groups.tsv   by top function: top function, map (the first), count, backtrace, save, kind, all maps
    summary.txt        verdict counts; crash groups by count
    maps/<id>/         warp.txt (the hook's lines), run.txt (the tail of port_run's output), log.txt.gz + bt.txt (crash / heap /
                       hang / stub only)
"""
import argparse
import collections
import concurrent.futures
import gzip
import hashlib
import json
import os
import queue
import random
import re
import shutil
import subprocess
import sys
import threading
import time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'tools'))
import sweep_wall  # noqa: E402  (no wall watchdog)
TREE = os.path.join(REPO, 'build/pokeplatinum')
# The exe: build/port/linux has every game since the Gen 4 modules are on by default (port_run.GAME_BUILD). The old module dir
# build/port/linux-plat is left over in older checkouts and in remote build slots: as the default it ran a stale exe
# (11-16's on 11-18) where it existed and no exe where it did not (every row no-field in seconds) (plsw2, 11-18).
BUILD = os.environ.get('BWPORT_PLAT_BUILD', 'build/port/linux')


def exe_or_exit(a):
    """exit when a.build has no bwport (a sweep without an exe makes only tool-error rows); --summary needs none"""
    p = os.path.join(REPO, a.build, 'bwport')
    if not getattr(a, 'summary', False) and not os.path.exists(p):
        sys.exit(f'no exe {p}: build it first (python3 tools/port_run.py --game platinum -t 1) or name one with --build')
# the reload from a library save: START at f1000 skips the opening to the title (START at f200 or f300 did not; tools/route.py's
# SAVE_START waits to f6300), START at the title, A on CONTINUE. The hook warps at the first free field frame
RELOAD = 'start*f10@f1000 start*f10@f1300 a*f10@f1600'
WALK_FROM = 1800   # V-blank: the warp is over by then (the first free frame ~1660, the map up ~90 frames later) (--walk-from)
FRAME_RE = re.compile(r'^#(\d+)\s+(?:0x[0-9a-f]+ in )?(\S+) \(.*?\)(?: at (\S+):(\d+))?(?: from (\S+))?$')
HEAD = ['map', 'name', 'verdict', 'frame', 'top', 'bt_id', 'last_zone', 'zones', 'wall_s']
# the interaction sweep's landing where a map's first-visit scene needs another tile than the first warp's (ptc3, 11-16): the Fight
# Area's scene walks the player along a fixed path that only fits the western start (5 tiles west of the rival, same row); from
# the north gate's tile the path ends in Route 225's zone, which deletes the map's objects under the rival's running walk, and the
# script waits on that walk for good (the same code on the DS: the scene is not played from there)
INT_LANDING = {188: (623, 434), 468: (842, 657)}


def read_maps(tree=TREE):
    """{id: (name, events.json path or None)}: the map header enum is generated/map_headers.txt's order (the line index is the
    id, MAP_HEADER_COUNT and what follows are not maps); a map's directory is its name in lower case"""
    maps = {}
    for i, line in enumerate(open(os.path.join(tree, 'generated/map_headers.txt'))):
        name = line.strip()
        if name == 'MAP_HEADER_COUNT':
            break
        ev = os.path.join(tree, 'res/field/maps', name[len('MAP_HEADER_'):].lower(), 'events.json')
        maps[i] = (name, ev if os.path.exists(ev) else None)
    return maps


def spawn(events):
    """a tile to warp to: the first warp event's, else an object's (the tile below it), a BG event's, a coord event's"""
    try:
        d = json.load(open(events))
    except (OSError, TypeError, ValueError):
        return None
    for key, dz in (('warp_events', 0), ('object_events', 1), ('bg_events', 1), ('coord_events', 0)):
        for e in d.get(key) or []:
            if 'x' in e and 'z' in e:
                return int(e['x']), int(e['z']) + dz
    return None


def walk_keys(seed, frames, start=None):
    """the deterministic walk: directions held 1-3 tiles, A presses (talk, signs, items), now and then B"""
    rng = random.Random(seed)
    f, out = WALK_FROM if start is None else start, []
    end = f + frames
    while f < end:
        r = rng.random()
        if r < 0.70:
            hold = 16 * rng.randint(1, 3)
            out.append(f'{rng.choice(("up", "down", "left", "right"))}*f{hold}@f{f}')
            f += hold + 2
        elif r < 0.92:
            out.append(f'a@f{f}')
            f += rng.randint(20, 60)
        else:
            out.append(f'b@f{f}')
            f += rng.randint(20, 40)
    return ' '.join(out)


def frames_of(text):
    out = []
    for line in text.splitlines():
        m = FRAME_RE.match(line.strip())
        if m:
            out.append((m.group(2), m.group(3), m.group(4)))
    return out


def crash_info(log):
    """(top function, top 6 frames text, id) from port_run's gdb backtrace"""
    stop = log.split('\n== port_run: stop\n', 1)
    bt = frames_of(stop[1].split('== port_run: threads')[0]) if len(stop) > 1 else []
    if not bt:
        return '?', '', ''
    src = [fr for fr in bt if fr[1]]
    top = (src or bt)[0][0]
    text = ' <- '.join(f'{fr[0]} ({os.path.basename(fr[1])}:{fr[2]})' if fr[1] else fr[0] for fr in bt[:6])
    return top, text, hashlib.sha1(' '.join(fr[0] for fr in bt[:6]).encode()).hexdigest()[:8]


def classify(line, log, warp, mid):
    """(verdict, frame, top, backtrace text, id) of one run: port_run's verdict line, its log, the hook's warp.txt"""
    zones = [int(z) for z in re.findall(r'^zone (\d+)', warp, re.M)]
    m = re.search(r'frame (\d+)', line)
    frame = m.group(1) if m else ''
    if line.startswith('heapcheck:'):   # BWPORT_HEAPCHECK (--heapcheck): the cause column names the block's allocator
        mo = re.search(r'heap (?:overrun|misuse)( after \S+| \(owner [^)]*\))', line)
        top = 'heap overrun' + (mo.group(1) if mo else '')
        return 'heap', frame, top, line, hashlib.sha1(top.encode()).hexdigest()[:8]
    if line.startswith('crash'):
        return ('crash', frame) + crash_info(log)
    if line.startswith('hang'):
        return ('hang', frame) + sweep_wall.frozen_top(log)
    if line.startswith('harness'):   # port_run itself died (sweep_wall.verdict_line): not a game finding
        return 'harness', frame, line[:100], line, ''
    if line.startswith('stub'):
        return 'stub', frame, line.split()[1], line, ''
    if re.search(r'^end \d+', warp, re.M) and mid in zones:
        return 'ok', re.search(r'^end (\d+)', warp, re.M).group(1), '', '', ''
    if mid not in zones:
        return 'no-field', frame, '', '', ''
    return ('exit' if line.startswith('exit') else 'timeout'), frame, '', '', ''


def run_map(a, mid, name, tile, slot):
    d = os.path.join(a.out, 'maps', str(mid))
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
    run_dir = os.path.join(a.out, 'run', f'w{slot}')
    out_dir = os.path.join(run_dir, 'out')
    keys = walk_keys(mid * 7919 + a.seed, a.frames, a.walk_from)
    end = a.walk_from + a.frames
    env = dict(os.environ, BWPORT_BUILD=a.build, BWPORT_PT_WARP=f'{mid}:{tile[0]}:{tile[1]}:{end}',
               BWPORT_PT_WARP_LOG=os.path.join(d, 'warp.txt'), PYTHONDONTWRITEBYTECODE='1')
    # straight to the exe (port/src/keys_script.c takes KEY*fN@fF holds; port_run's --keys parser does not)
    env['BWPORT_KEYS'] = a.reload + ' ' + keys
    cmd = ['python3', os.path.join(REPO, 'tools/port_run.py'), '--game', 'platinum', '--no-build', '--save', a.save,
           '--dir', run_dir, '--out', out_dir, '-t', sweep_wall.NO_WALL_CAP, '--frames', str(end + sweep_wall.END_SLACK), '--shots', '0',
           '--hang', '30', '--hang-stop',
           '--skip', str(a.skip)]
    t0 = time.time()
    r = subprocess.run(['nice', '-n', '10'] + cmd, env=env, capture_output=True, text=True, cwd=REPO)
    wall = time.time() - t0
    open(os.path.join(d, 'run.txt'), 'w').write('\n'.join((r.stdout + r.stderr).splitlines()[-20:]) + '\n')
    line = (r.stdout.strip().splitlines() or ['?'])[-1]
    try:
        line = sweep_wall.verdict_line(r, out_dir)
    except OSError:
        pass
    try:
        log = open(os.path.join(out_dir, 'log.txt'), errors='replace').read()
    except OSError:
        log = r.stdout + r.stderr
    warp = open(os.path.join(d, 'warp.txt')).read() if os.path.exists(os.path.join(d, 'warp.txt')) else ''
    zones = [int(z) for z in re.findall(r'^zone (\d+)', warp, re.M)]
    verdict, frame, top, text, bid = classify(line, log, warp, mid)
    if verdict in ('crash', 'hang', 'stub', 'heap'):
        with gzip.open(os.path.join(d, 'log.txt.gz'), 'wt') as f:
            f.write(log)
        open(os.path.join(d, 'bt.txt'), 'w').write(line + '\n' + text + '\n')
    shutil.rmtree(out_dir, ignore_errors=True)
    last = zones[-1] if zones else ''
    return [os.path.basename(d), name, verdict, frame, top, bid, last, ','.join(map(str, dict.fromkeys(zones))),
            f'{wall:.0f}'], text


def parse_maps(spec, maps):
    if spec == 'all':
        return sorted(maps)
    out = []
    for part in spec.split(','):
        if '-' in part:
            lo, hi = part.split('-')
            out += [i for i in range(int(lo), int(hi) + 1) if i in maps]
        elif part:
            out.append(int(part))
    return out


def summarize(a, maps):
    path = os.path.join(a.out, 'sweep.tsv')
    rows = [l.rstrip('\n').split('\t') for l in open(path)][1:] if os.path.exists(path) else []
    verdicts = collections.Counter(r[2] for r in rows)
    groups = collections.OrderedDict()
    for r in sorted(rows, key=lambda r: int(r[0])):
        if r[2] in ('crash', 'hang', 'stub', 'heap'):
            g = groups.setdefault((r[2], r[4]), {'maps': [], 'bt': ''})
            g['maps'].append(r[0])
            if not g['bt']:
                try:
                    g['bt'] = open(os.path.join(a.out, 'maps', r[0], 'bt.txt')).read().split('\n')[1]
                except (OSError, IndexError):
                    pass
    order = sorted(groups.items(), key=lambda kv: -len(kv[1]['maps']))
    with open(os.path.join(a.out, 'crash_groups.tsv'), 'w') as f:
        f.write('top_function\tmap\tcount\tbacktrace\tsave\tkind\tall_maps\n')
        for (kind, top), g in order:
            m0 = int(g['maps'][0])
            f.write(f'{top}\t{m0} {maps.get(m0, ("?",))[0]}\t{len(g["maps"])}\t{g["bt"]}\t{a.save}\t{kind}\t'
                    f'{",".join(g["maps"])}\n')
    lines = [f'pt_sweep: {len(rows)} maps, save {a.save}, {a.frames} walk frames; '
             + ', '.join(f'{k} {v}' for k, v in verdicts.most_common()), '', 'count  kind   top function   (first map)']
    for (kind, top), g in order:
        m0 = int(g['maps'][0])
        lines.append(f'{len(g["maps"]):5}  {kind:5}  {top}   ({m0} {maps.get(m0, ("?",))[0]}; maps {",".join(g["maps"][:12])}'
                     f'{" ..." if len(g["maps"]) > 12 else ""})')
    open(os.path.join(a.out, 'summary.txt'), 'w').write('\n'.join(lines) + '\n')
    print('\n'.join(lines[:1] + lines[1:30]))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--maps', default='all', help='all | ids and ranges: 60-70,411')
    ap.add_argument('--out', required=True)
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('--save', default='pt-pgrunt', help='the library save the runs load')
    ap.add_argument('--build', default=BUILD)
    ap.add_argument('--frames', type=int, default=3000, help='the walk, V-blanks after the warp')
    ap.add_argument('--seed', type=int, default=0)
    ap.add_argument('--skip', type=int, default=4, help="port_run's --skip (frame skip)")
    sweep_wall.ignore_t(ap)
    ap.add_argument('--reload', default=RELOAD, help="the keys from the save to the free field (BWPORT_KEYS items)")
    ap.add_argument('--walk-from', type=int, default=WALK_FROM, help='the V-blank the walk starts at (after the warp)')
    ap.add_argument('--heapcheck', nargs='?', const='1', default=None, metavar='LEVEL',
                    help="BWPORT_HEAPCHECK=LEVEL for every map (default 1): a damaged expanded heap stops the run, the row's verdict is "
                         "heap and its top column `heap overrun after FUNC` (the allocator of the block before the damage)")
    ap.add_argument('--redo', action='store_true', help='run maps already in sweep.tsv again')
    ap.add_argument('--summary', action='store_true', help='only write the summary files')
    ap.add_argument('--keep-run', action='store_true', help="keep DIR/run (the slots' run dirs, ~250 MB each) at the end")
    a = ap.parse_args()
    exe_or_exit(a)
    a.out = os.path.abspath(a.out)
    if a.heapcheck is not None:
        os.environ['BWPORT_HEAPCHECK'] = a.heapcheck
    os.makedirs(os.path.join(a.out, 'maps'), exist_ok=True)
    maps = read_maps()
    if a.summary:
        summarize(a, maps)
        return
    path = os.path.join(a.out, 'sweep.tsv')
    done = {}
    if os.path.exists(path):
        for l in list(open(path))[1:]:
            done[int(l.split('\t')[0])] = l
    todo = [m for m in parse_maps(a.maps, maps) if a.redo or m not in done]
    if a.redo:
        done = {k: v for k, v in done.items() if k not in todo}
    with open(path, 'w') as f:
        f.write('\t'.join(HEAD) + '\n')
        f.writelines(done[k] for k in sorted(done))
    slots = queue.Queue()
    for i in range(a.j):
        slots.put(i)
    lock = threading.Lock()
    t0 = time.time()
    n = [0]

    def job(mid):
        name, events = maps.get(mid, ('?', None))
        tile = spawn(events) if events else None
        if tile is None:
            row, text = [mid, name, 'no-spawn', '', '', '', '', '', '0'], ''
        else:
            slot = slots.get()
            try:
                row, text = run_map(a, mid, name, tile, slot)
            finally:
                slots.put(slot)
        with lock:
            with open(path, 'a') as f:
                f.write('\t'.join(map(str, row)) + '\n')
            n[0] += 1
            print(f'[{n[0]}/{len(todo)} {time.time() - t0:.0f}s] {mid} {name}: {row[2]} {row[3]} {row[4]}', flush=True)

    with concurrent.futures.ThreadPoolExecutor(a.j) as ex:
        list(ex.map(job, todo))
    if not a.keep_run:   # the slots' run dirs: each holds the launcher's ROM import (~250 MB), made again by the next run
        shutil.rmtree(os.path.join(a.out, 'run'), ignore_errors=True)
    summarize(a, maps)


if __name__ == '__main__':
    main()
