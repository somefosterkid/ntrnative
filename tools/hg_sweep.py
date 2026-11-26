#!/usr/bin/env python3
"""hg_sweep.py: the HG/SS crash sweep: the player on every map, a walk there, every crash ahead of the story (docs/hgss.md
"Crash sweep"). Crash finding only: the debug warp is never used in tools/routes chains or for milestones of the save library.

    python3 tools/hg_sweep.py --maps all -j 6 --out /tmp/sweep            # every MAP_ constant (541)
    python3 tools/hg_sweep.py --maps 60-70,33 -j 3 --out DIR               # ids, ranges
    python3 tools/hg_sweep.py --summary --out DIR                          # only (re)write the summary files

Per map: tools/port_run.py --game heartgold --no-build from a library save (--save, default hg-room1), the reload's keys
(the title, CONTINUE, the summary, B closes the X menu the save was made in), then BWPORT_HG_WARP=MAP:X:Y:END: at the first
free field frame port/game/route_gen4.c warps the player there with the game's own fade warp (tile X,Y: the map's first warp
event, else an object's, a BG event's or a coord event's tile; zone_event JSON), then a walk seeded by the map id (random
directions held 1-3 tiles, A and B presses) for --frames V-blanks, and the exe exits 0 at END. No wall watchdog (a slow run is
never killed, never a finding): the frame cap END + 6000 ends a run still in a battle or a script (verdict exit); --hang 30 (no
frame for 30 s) ends a stalled run with gdb's backtrace (tools/sweep_wall.py).
Verdicts: ok (END reached on the field), crash (signal: the top 6 frames), hang, stub, no-field (the warp's map never came up
free and nothing crashed), stuck-walk (the player stood on one tile for the last 1200 V-blanks of the walk; top = `MAP X,Z`),
exit (the frame cap: a battle or a script was still running at END), timeout (any other end).
Build: --build (default build/port/linux-hgss-cov, meson -Dhgss_cov=true: function coverage, port/game/hg_cov.c; any HG build
works, without coverage the coverage files stay empty). The tool works at any pin: maps, warps and objects are read from the
generated tree (build/pokeheartgold: include/constants/maps.h for the names); the warps and objects come from the player's ROM
(orig/baserom_hg_us.nds, _ss_ with --game soulsilver: zone event archive a/0/3/2, tools/atlas_gen4.py's parser).

Out dir (resumable: maps already in sweep.tsv are skipped; --redo runs them again):
    sweep.tsv          one line per map: map id, name, verdict, frame, top function, backtrace id, last zone, zones, wall s
    crash_groups.tsv   by top function: top function, map (the first), count, backtrace, save, all maps
    summary.txt        verdict counts; crash groups by count
    executed_funcs.txt every HG function any map reached, with the number of maps (count<TAB>name, by count)
    maps/<id>/         warp.txt (the hook's lines), cov.txt.gz (the functions reached), log.txt.gz (crash / hang / stub only)
"""
import argparse
import bisect
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
import sweep_wall  # noqa: E402  (no wall watchdog, stuck-walk)
TREE = os.path.join(REPO, 'build/pokeheartgold')
# the reload from a library save: START and A pulses from f200 skip the intro and the title, CONTINUE, the summary (the
# field is free at ~f630, against room-load.txt's f10500 with START at f7700); B closes the X menu the save was made in.
# The hook warps at the first free frame (~f630, the map up ~30 frames later)
RELOAD = ' '.join(f'start@f{f} a@f{f + 75}' for f in range(200, 900, 150)) + ' b@f1000 b@f1100'
WALK_FROM = 1600   # V-blank: the warp is over by then
FRAME_RE = re.compile(r'^#(\d+)\s+(?:0x[0-9a-f]+ in )?(\S+) \(.*?\)(?: at (\S+):(\d+))?(?: from (\S+))?$')


def rom_events(game):
    """{zone id: {'warps', 'objects', 'bgs', 'coords': [{x, z}]}} read from the player's ROM (orig/baserom_hg_us.nds or _ss_):
    the zone event archive a/0/3/2 through the ROM's own map header table (the data is not in the tree: only the player's ROM has it)"""
    import atlas
    import atlas_gen4
    from rom_modules import arm9
    kind = 'ss' if game.startswith('soul') else 'hg'
    rom_path = os.path.join(REPO, 'orig', f'baserom_{kind}_us.nds')
    if not os.path.exists(rom_path):
        sys.exit(f'hg_sweep: {rom_path} is needed (the map sweep reads the zone events from the ROM)')
    rom = open(rom_path, 'rb').read()
    g = atlas_gen4.GEN4[kind]
    mats, ents, scripts = (atlas.arc_path(rom, g[k]) for k in ('matrix', 'events', 'scripts'))
    image = arm9(rom)
    off, n = atlas_gen4.header_table(image, 'hg', len(mats), len(ents), len(scripts))
    out = {}
    for z in range(n):
        h = atlas_gen4.parse_header4(image[off + 0x18 * z:off + 0x18 * (z + 1)], 'hg', z)
        bg, npcs, warps, trigs, _ok = atlas_gen4.parse_events4(ents[h['entities']])
        out[z] = {'warps': warps, 'objects': npcs, 'bgs': bg, 'coords': trigs}
    return out


def read_maps(game='heartgold'):
    """{id: (name, code, events dict or None)}: names from the generated tree's maps.h, events from the ROM (rom_events)"""
    maps = {}
    for line in open(os.path.join(TREE, 'include/constants/maps.h')):
        m = re.match(r'#define (MAP_\w+)\s+(\d+)\s*(?://\s*(\w+))?', line)
        if m:
            maps[int(m.group(2))] = [m.group(1), m.group(3) or '', None]
    events = rom_events(game)
    for k, v in maps.items():
        v[2] = events.get(k)
    return {k: tuple(v) for k, v in maps.items()}


def spawn(events):
    """a tile to warp to: the first warp event's, else an object's (the tile below it), a BG event's, a coord event's.
    events: read_maps' dict (or the path of a zone_event JSON)"""
    if isinstance(events, dict):
        d = events
    else:
        try:
            d = json.load(open(events))
        except (OSError, TypeError, ValueError):
            return None
    for key, dz in (('warps', 0), ('objects', 1), ('bgs', 1), ('coords', 0)):
        for e in d.get(key) or []:
            if 'x' in e and 'z' in e:
                return int(e['x']), int(e['z']) + dz
    return None


def walk_keys(seed, frames):
    """the deterministic walk: directions held 1-3 tiles, A presses (talk, signs, items), now and then B"""
    rng = random.Random(seed)
    f, out = WALK_FROM, []
    while f < WALK_FROM + frames:
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


APPS_TAP = False   # --apps-tap: icons by a stylus tap (keys' cursor crashed in Sprite_GetNumAnimSeqs at the hgsw1 pin)
APPS_MAP = 63   # the apps pass: the house's 1F (the save's own map), warped onto its first warp event's tile


def apps_keys(frames, places=range(7)):
    """the apps pass: X opens the field menu (the touch screen's icons; keys move the cursor over the shown ones), then
    for each place (0-6): N downs, A opens the app, a few A presses inside, B closes it (several: menus nest), B the menu.
    hg_sweep runs one place per row (-1 .. -7: place 0 .. 6), so a crash in one app does not hide the next"""
    f, out = WALK_FROM, []
    for i in places:
        out.append(f'x@f{f}')
        f += 60
        if APPS_TAP:
            # a tap on the icon (stylus: the DS pixel, no cursor): the touch menu's rects (ov27 data 0x0225CF68, y0 y1 x0 x1):
            # 4 icons down the left (x 16-76, y 22-54 / 62-94 / 102-134 / 142-174), 3 on the right (x 96-156, the first 3 rows)
            x, y = ((46, 38 + 40 * i) if i < 4 else (126, 38 + 40 * (i - 4)))
            out.append(f'stylus:{x},{y}@f{f}')
        else:
            for _ in range(i):
                out.append(f'down@f{f}')
                f += 20
            out.append(f'a@f{f}')
        f += 400
        for _ in range(3):
            out += [f'right@f{f}', f'a@f{f + 30}']
            f += 90
        for _ in range(5):
            out.append(f'b@f{f}')
            f += 80
        f += 200
    return ' '.join(out), max(frames, f - WALK_FROM + 300)


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


class Symbols:
    """the exe's function symbols (nm, cached by the exe's size and mtime): address -> name"""
    def __init__(self, exe, cache_dir):
        st = os.stat(exe)
        cache = os.path.join(cache_dir, f'nm_{st.st_size}_{int(st.st_mtime)}.txt')
        if not os.path.exists(cache):
            r = subprocess.run(['nm', '--defined-only', exe], capture_output=True, text=True)
            rows = []
            for line in r.stdout.splitlines():
                p = line.split()
                if len(p) == 3 and p[1] in 'tTwW':
                    rows.append((int(p[0], 16), p[2]))
            rows.sort()
            with open(cache + '.tmp', 'w') as f:
                f.writelines(f'{a:x} {n}\n' for a, n in rows)
            os.replace(cache + '.tmp', cache)
        self.addr, self.name = [], []
        for line in open(cache):
            a, n = line.split()
            self.addr.append(int(a, 16))
            self.name.append(n)
        # port/game/hg_cov.c's dump starts with its own address
        self.base = self.addr[self.name.index('HgCov_Dump')] \
            if 'HgCov_Dump' in self.name else None

    def funcs(self, cov_bin):
        """the names of the functions a BWPORT_HG_COV file holds"""
        try:
            data = open(cov_bin, 'rb').read()
        except OSError:
            return set()
        if len(data) < 8 or self.base is None:
            return set()
        words = [int.from_bytes(data[i:i + 8], 'little') for i in range(0, len(data) - 7, 8)]
        delta = self.base - words[0]
        out = set()
        for pc in words[1:]:
            i = bisect.bisect_right(self.addr, pc + delta) - 1   # (a function's entry: its symbol's address)
            if i >= 0:
                out.add(self.name[i])
        return out


def run_map(a, mid, name, tile, slot, syms):
    d = os.path.join(a.out, 'maps', str(mid))
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
    run_dir = os.path.join(a.out, 'run', f'w{slot}')
    out_dir = os.path.join(run_dir, 'out')
    walk = mid >= 0   # (the apps pass has no walk)
    if mid < 0:   # the apps pass: row -1 - N opens the menu's place N
        keys, frames = apps_keys(a.frames, [-1 - mid])
        mid = APPS_MAP
    else:
        keys, frames = walk_keys(mid * 7919 + a.seed, a.frames), a.frames
    end = WALK_FROM + frames
    env = dict(os.environ, BWPORT_BUILD=a.build, BWPORT_HG_WARP=f'{mid}:{tile[0]}:{tile[1]}:{end}',
               BWPORT_HG_WARP_LOG=os.path.join(d, 'warp.txt'), BWPORT_HG_COV=os.path.join(run_dir, 'cov.bin'),
               PYTHONDONTWRITEBYTECODE='1')
    # straight to the exe (port/src/keys_script.c takes KEY*fN@fF holds; port_run's --keys parser does not)
    env['BWPORT_KEYS'] = a.reload + ' ' + keys
    cmd = ['python3', os.path.join(REPO, 'tools/port_run.py'), '--game', a.game, '--no-build', '--save', a.save,
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
    m = re.search(r'frame (\d+)', line)
    frame = m.group(1) if m else ''
    top = text = bid = ''
    if line.startswith('heapcheck:'):   # BWPORT_HEAPCHECK (--heapcheck): the cause column names the block's allocator
        verdict = 'heap'
        mo = re.search(r'heap (?:overrun|misuse)( after \S+| \(ow\wer [^)]*\))', line)
        top = 'heap overrun' + (mo.group(1) if mo else '')
        text = line
        bid = hashlib.sha1(top.encode()).hexdigest()[:8]
    elif line.startswith('crash'):
        verdict = 'crash'
        top, text, bid = crash_info(log)
    elif line.startswith('hang'):
        verdict = 'hang'
        top, text, bid = sweep_wall.frozen_top(log)
    elif line.startswith('harness'):   # port_run itself died (sweep_wall.verdict_line): not a game finding
        verdict, top, text = 'harness', line[:100], line
    elif line.startswith('stub'):
        verdict = 'stub'
        top = line.split()[1]
        text = line
    elif re.search(r'^end \d+', warp, re.M) and mid in zones:
        verdict = 'ok'
        frame = re.search(r'^end (\d+)', warp, re.M).group(1)
    elif mid not in zones:
        verdict = 'no-field'
    elif line.startswith('exit'):
        verdict = 'exit'
    else:
        verdict = 'timeout'
    stuck = sweep_wall.stuck_walk(warp, end, WALK_FROM) if walk and verdict in ('ok', 'exit') else None
    if stuck:   # on one tile for the last STUCK_FRAMES V-blanks of the walk: a finding with its place
        verdict, top, text, frame = 'stuck-walk', f'{name} {stuck[0]},{stuck[1]}', f'on ({stuck[0]},{stuck[1]}) since V-blank {stuck[2]}', ''
    if verdict in ('crash', 'hang', 'stub', 'heap', 'stuck-walk'):
        with gzip.open(os.path.join(d, 'log.txt.gz'), 'wt') as f:
            f.write(log)
        open(os.path.join(d, 'bt.txt'), 'w').write(line + '\n' + text + '\n')
    funcs = syms.funcs(os.path.join(run_dir, 'cov.bin')) if syms else set()
    with gzip.open(os.path.join(d, 'cov.txt.gz'), 'wt') as f:
        f.writelines(n + '\n' for n in sorted(funcs))
    for p in ('cov.bin',):
        if os.path.exists(os.path.join(run_dir, p)):
            os.remove(os.path.join(run_dir, p))
    shutil.rmtree(out_dir, ignore_errors=True)
    last = zones[-1] if zones else ''
    note = ''
    why = entered_wrong(mid, name, warp) if verdict != 'ok' else None
    if why:
        verdict, note = 'entered-wrong', f'{verdict}: {why}'
    return [os.path.basename(d), name, verdict, frame, top, bid, last, ','.join(map(str, dict.fromkeys(zones))),
            f'{wall:.0f}', note], text


def parse_maps(spec, maps):
    if spec == 'all':
        return sorted(maps)
    out = []
    for part in spec.split(','):
        if part == 'all':
            out += sorted(maps)
        elif part == 'apps':
            out += list(range(-1, -8, -1))
        elif '-' in part:
            lo, hi = part.split('-')
            out += [i for i in range(int(lo), int(hi) + 1) if i in maps]
        elif part:
            out.append(int(part))
    return out


HEAD = ['map', 'name', 'verdict', 'frame', 'top', 'bt_id', 'last_zone', 'zones', 'wall_s', 'note']

# Harness artefacts (hk542): maps the sweep's direct warp enters without the state the game's own entry gives them. A
# failing verdict (crash, hang, stub, heap, timeout, no-field) on one of these is reported as `entered-wrong` with the
# reason in the `note` column, kept out of crash_groups.tsv: it says nothing about the port. An `ok` row stays ok.
# entered_wrong() adds every elevator map and a last logged position outside the map matrix (an off-map walk).
ENTERED_WRONG = {
    2: 'Union Room: fieldSystem->unk84 is NULL without the entry tasks (a direct warp skips them)',
    3: 'Underground: a comm / session map (needs the link session)',
    4: 'Wi-Fi single battle area (Colosseum-like): comm / session map',
    5: 'Wi-Fi multi battle area (Colosseum-like): comm / session map',
    266: 'Battle Tower elevator: frontierFsys is NULL on a direct warp (the tower entry creates it)',
    306: 'Hall of Fame: mons[0].mon is NULL, the sweep save (hg-room1) has no party (hg-r30c: ok)',
    442: "Celadon department store elevator: the sweep's warp id has no warp event on that map",
    516: 'Wi-Fi Plaza warp room: comm / session map (the plaza needs the Wi-Fi session)',
}


def entered_wrong(mid, name, warp):
    """The reason a failing run on map MID is a harness artefact (None: a real finding): the ENTERED_WRONG table, every
    elevator (a direct warp lands in a car the entry never set up), a last position off the map matrix (the random walk
    left the map: warp.txt's `pos F X Z off`, port/game/route_gen4.c Pr4_Sweep)."""
    if mid in ENTERED_WRONG:
        return ENTERED_WRONG[mid]
    if 'ELEVATOR' in name:
        return 'elevator: a direct warp enters the car without the entry that sets up its floor and state (hk541)'
    pos = re.findall(r'^pos (\d+) (-?\d+) (-?\d+) (in|off)$', warp, re.M)
    if pos and pos[-1][3] == 'off':
        return f'off-map walk: the player stood at tile ({pos[-1][1]},{pos[-1][2]}) outside the map matrix at V-blank {pos[-1][0]}'
    return None


def summarize(a, maps):
    path = os.path.join(a.out, 'sweep.tsv')
    rows = [l.rstrip('\n').split('\t') for l in open(path)][1:] if os.path.exists(path) else []
    verdicts = collections.Counter(r[2] for r in rows)
    groups = collections.OrderedDict()
    for r in sorted(rows, key=lambda r: int(r[0])):
        if r[2] in ('crash', 'hang', 'stub', 'heap', 'stuck-walk'):
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
            f.write(f'{top}\t{m0} {(maps.get(m0, ("?",))[0] if m0 >= 0 else f"APPS_{-1 - m0}")}\t{len(g["maps"])}\t{g["bt"]}\t{a.save}\t{kind}\t'
                    f'{",".join(g["maps"])}\n')
    counts = collections.Counter()
    for r in rows:
        p = os.path.join(a.out, 'maps', r[0], 'cov.txt.gz')
        if os.path.exists(p):
            counts.update(set(gzip.open(p, 'rt').read().split()))
    with open(os.path.join(a.out, 'executed_funcs.txt'), 'w') as f:
        f.writelines(f'{n}\t{fn}\n' for fn, n in sorted(counts.items(), key=lambda kv: (-kv[1], kv[0])))
    lines = [f'hg_sweep: {len(rows)} maps, save {a.save}, {a.frames} walk frames; '
             + ', '.join(f'{k} {v}' for k, v in verdicts.most_common()),
             f'functions executed (union): {len(counts)}', '', 'count  kind   top function   (first map)']
    for (kind, top), g in order:
        m0 = int(g['maps'][0])
        lines.append(f'{len(g["maps"]):5}  {kind:5}  {top}   ({m0} {(maps.get(m0, ("?",))[0] if m0 >= 0 else f"APPS_{-1 - m0}")}; maps {",".join(g["maps"][:12])}'
                     f'{" ..." if len(g["maps"]) > 12 else ""})')
    open(os.path.join(a.out, 'summary.txt'), 'w').write('\n'.join(lines) + '\n')
    print('\n'.join(lines[:3] + lines[3:30]))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--maps', default='all', help="all | ids and ranges: 60-70,33 | apps (the X menu's 7 places: rows -1..-7)")
    ap.add_argument('--out', required=True)
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('--save', default='hg-room1', help='the save (from the save library) the runs load')
    ap.add_argument('--game', default='heartgold')
    ap.add_argument('--build', default='build/port/linux-hgss-cov')
    ap.add_argument('--frames', type=int, default=3000, help='the walk, V-blanks after the warp')
    ap.add_argument('--seed', type=int, default=0)
    ap.add_argument('--skip', type=int, default=4, help="port_run's --skip (frame skip)")
    sweep_wall.ignore_t(ap)
    ap.add_argument('--reload', default=RELOAD, help="the keys from the save to the free field (BWPORT_KEYS items)")
    ap.add_argument('--walk-from', type=int, default=WALK_FROM, help='the V-blank the walk starts at (after the warp)')
    ap.add_argument('--apps-tap', action='store_true', help="the apps pass opens each place by a stylus tap on its icon, not N downs + A")
    ap.add_argument('--heapcheck', nargs='?', const='1', default=None, metavar='LEVEL',
                    help="BWPORT_HEAPCHECK=LEVEL for every map (default 1): a damaged expanded heap stops the run, the row's verdict is "
                         "heap and its top column `heap overrun after FUNC` (the allocator of the block before the damage)")
    ap.add_argument('--redo', action='store_true', help='run maps already in sweep.tsv again')
    ap.add_argument('--summary', action='store_true', help='only write the summary files')
    ap.add_argument('--keep-run', action='store_true', help="keep DIR/run (the slots' run dirs, ~250 MB each) at the end")
    a = ap.parse_args()
    a.out = os.path.abspath(a.out)
    if a.heapcheck is not None:
        os.environ['BWPORT_HEAPCHECK'] = a.heapcheck
    globals()['APPS_TAP'] = a.apps_tap
    globals()['WALK_FROM'] = a.walk_from   # (walk_keys, apps_keys)
    os.makedirs(os.path.join(a.out, 'maps'), exist_ok=True)
    maps = read_maps(a.game)
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
    exe = os.path.join(REPO, a.build, 'bwport')
    syms = Symbols(exe, a.out)
    try:
        cov_on = any(o['name'] == 'hgss_cov' and o['value'] for o in json.load(open(os.path.join(
            REPO, a.build, 'meson-info/intro-buildoptions.json'))))
    except (OSError, ValueError):
        cov_on = False
    if syms.base is None or not cov_on:
        print(f'hg_sweep: {a.build} has no coverage (meson -Dhgss_cov=true): executed_funcs stays empty')
        syms = None
    slots = queue.Queue()
    for i in range(a.j):
        slots.put(i)
    lock = threading.Lock()
    t0 = time.time()
    n = [0]

    def job(mid):
        name, _code, events = maps.get(mid if mid >= 0 else APPS_MAP, ('?', '', None))
        name = name if mid >= 0 else f'APPS_{-1 - mid}'
        tile = spawn(events) if events else None
        if tile is None:
            row, text = [mid, name, 'no-spawn', '', '', '', '', '', '0', ''], ''
        else:
            slot = slots.get()
            try:
                row, text = run_map(a, mid, name, tile, slot, syms)
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
