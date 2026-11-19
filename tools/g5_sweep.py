#!/usr/bin/env python3
"""g5_sweep.py: the Gen 5 crash sweep (Black, White, Black 2, White 2): the player on every zone, a walk there with wild
encounters on, every crash ahead of the story (the Platinum / HG/SS twins: tools/pt_sweep.py, tools/hg_sweep.py). Crash finding
only: the pilot's X:warp is a test cheat, never used in tools/routes chains or for the save library's milestones.

    python3 tools/g5_sweep.py b --zones all -j 4 --out /tmp/g5b --heapcheck   # every zone header of Black
    python3 tools/g5_sweep.py b2 --zones 60-70,411 -j 2 --out DIR             # ids, ranges
    python3 tools/g5_sweep.py b --summary --out DIR                           # only (re)write the summary files

Per zone: one route leg (tools/route.py with BWPORT_ROUTES_DIR = DIR/routes, ROUTE_WORK = DIR/work) from a late library save
(--save; defaults: b-post3, w-post3, b2-champ, w2-champ), `Pa X:warp:ZONE:X:Z Wf:60` (the game's own map change to the zone's
first warp tile, else the tile below an actor, a BG event or a trigger; rail-only zones are no-spawn), then a walk seeded by the
zone id (directions held 1-3 tiles, A and B presses, --walk V-blanks), `C`. The leg is flagged `encounters`: a walk in grass, a
cave or water fights wild battles (the test cheat wins them; the pilot presses A), so the sweep is the wild-battle sweep too.
route.py runs a crashed or hung leg again under gdb; the backtrace is in DIR/work/GAME-zN.game.log.
Verdicts are route.py's (look = the walk finished: a pass; crash, hang, timeout, stuck) plus heap (--heapcheck: an expanded
heap was damaged) and no-spawn.

--trainers is the battle pass: one leg per trainer actor of the zones (scripts 3000-6999): its trainer flag cleared (X:flag: the
id + 0x58c, Black 2 0x5f0, as TrainerFlagGet), the warp onto the tile it faces, G:npc:UID (its sight or the talk starts the battle; the test cheat wins
it), Wf:60, C. An unreachable trainer ends `stuck` / `no path`: not a finding. Its out dir is its own (legs tZONE-UID).

Out dir: sweep.tsv (leg, verdict, top function, backtrace top 3, battles, the route line), crash_groups.tsv (by top function +
the backtrace's top 3: count, zones), summary.txt (counts, groups).
"""
import argparse
import collections
import json
import os
import random
import re
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SAVES = {'b': 'b-post3', 'w': 'w-post3', 'b2': 'b2-champ', 'w2': 'w2-champ'}
FRAME_RE = re.compile(r'^#(\d+)\s+(?:0x[0-9a-f]+ in )?(\S+) \(.*?\)(?: at (\S+):(\d+))?(?: from (\S+))?$')
LINE_RE = re.compile(r'^(\w+) ([zt][\d-]+): (\S+) ?(.*)$')
HEAD = ['leg', 'verdict', 'top', 'bt3', 'hits', 'line']


def parse_ranges(spec, nzones):
    if spec == 'all':
        return list(range(nzones))
    out = []
    for part in spec.split(','):
        a, _, b = part.partition('-')
        out += list(range(int(a), int(b) + 1)) if b else [int(a)]
    return out


def landing(z):
    """(x, z) of the zone's landing tile, or None: the first warp (its own tile, the way a door's arrival lands), else the tile
    below an actor, a BG event or a trigger; entities placed on rails are skipped (X:warp is a grid map change)"""
    for w in z.get('warps', []):
        if 'rail' not in w:
            return w['x'], w['z']
    for kind in ('npcs', 'bg', 'triggers'):
        for e in z.get(kind, []):
            if 'rail' not in e and 'x' in e:
                return e['x'], e['z'] + 1
    return None


def walk(zone, frames, game):
    rng = random.Random(zone * 7919 + sum(map(ord, game)))
    out, f = [], 0
    while f < frames:
        r = rng.random()
        if r < 0.8:
            n = 16 * rng.randint(1, 3)
            out.append(f'K{rng.choice("UDLR")}:{n}')
        elif r < 0.9:
            n = 6
            out.append('Ka:6 K0:20')
            n = 26
        else:
            out.append('Kb:6 K0:10')
            n = 16
        f += n
    return ' '.join(out)


def frames_of(text):
    return [(m.group(2), m.group(3), m.group(4)) for m in (FRAME_RE.match(l.strip()) for l in text.splitlines()) if m]


def crash_info(text):
    """(top function, top 3 frames) from the gdb stop in a game log (port_run's `== port_run: stop` section)"""
    stop = text.split('\n== port_run: stop\n', 1)
    bt = frames_of(stop[1].split('== port_run: threads')[0]) if len(stop) > 1 else frames_of(text)
    if not bt:
        return '?', ''
    src = [fr for fr in bt if fr[1]]
    top = (src or bt)[0][0]
    return top, ' <- '.join(f'{fr[0]} ({os.path.basename(fr[1])}:{fr[2]})' if fr[1] else fr[0] for fr in bt[:3])


def legkey(name):
    return [int(x) for x in re.findall(r'\d+', name)]


def read_rows(out):
    rows = {}
    p = os.path.join(out, 'sweep.tsv')
    if os.path.exists(p):
        for line in open(p).read().splitlines()[1:]:
            f = line.split('\t')
            rows[f[0]] = f
    return rows


def classify(game, leg, verdict, rest, work):
    gl = os.path.join(work, f'{game}-{leg}.game.log')
    text = open(gl, errors='replace').read() if os.path.exists(gl) else ''
    battles = len(re.findall(r'^cheat: damage', text, re.M))   # the test cheat's damage lines: hits in battles (0: no battle ran)
    top, bt3 = '', ''
    if 'heapcheck' in rest:
        verdict = 'heap'
        m = re.search(r'heap (?:overrun|misuse)( after \S+| \([^)]*\))', rest)
        top = 'heap overrun' + (m.group(1) if m else '')
        bt3 = rest[:200]
    elif verdict in ('crash', 'hang'):
        top, bt3 = crash_info(text)
        if top == '?':
            top = rest[:80]
    return [leg, verdict, top, bt3, str(battles), rest[:300].replace('\t', ' ')]


def summary(out, game):
    rows = read_rows(out)
    counts = collections.Counter(r[1] for r in rows.values())
    groups = collections.defaultdict(list)
    for r in rows.values():
        if r[1] not in ('look', 'ok', 'no-spawn', 'off-route'):   # off-route: the walk left the zone (a door), no dump: a pass
            groups[(r[1], r[2], r[3])].append(r[0])
    g = sorted(groups.items(), key=lambda kv: -len(kv[1]))
    with open(os.path.join(out, 'crash_groups.tsv'), 'w') as f:
        f.write('verdict\ttop\tbt3\tcount\tzones\n')
        for (v, top, bt3), zs in g:
            f.write(f'{v}\t{top}\t{bt3}\t{len(zs)}\t{",".join(sorted(zs, key=legkey))}\n')
    lines = [f'{game}: {len(rows)} legs: ' + ', '.join(f'{k} {v}' for k, v in counts.most_common()),
             f'battle hits (the test cheat damage lines): {sum(int(r[4] or 0) for r in rows.values())}']
    for (v, top, bt3), zs in g:
        lines.append(f'{len(zs):4d} {v:8s} {top}  [{bt3}]  zones {",".join(sorted(zs, key=legkey)[:12])}{" ..." if len(zs) > 12 else ""}')
    open(os.path.join(out, 'summary.txt'), 'w').write('\n'.join(lines) + '\n')
    print('\n'.join(lines))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('game', choices=sorted(SAVES))
    ap.add_argument('--zones', default='all')
    ap.add_argument('--out', required=True)
    ap.add_argument('-j', type=int, default=2)
    ap.add_argument('--save', default='')
    ap.add_argument('--walk', type=int, default=1500, help='V-blanks of walk tokens after the warp (default 1500)')
    ap.add_argument('--heapcheck', nargs='?', const='1', default=None, metavar='LEVEL')
    ap.add_argument('--no-build', action='store_true')
    ap.add_argument('--redo', action='store_true')
    ap.add_argument('--summary', action='store_true')
    ap.add_argument('--trainers', action='store_true', help='the battle pass: one leg per trainer actor (its flag cleared, G:npc)')
    a = ap.parse_args()
    out = os.path.abspath(a.out)
    os.makedirs(out, exist_ok=True)
    if a.summary:
        return summary(out, a.game)
    atlas = os.path.join(REPO, f'build/atlas/{a.game}.json')
    if not os.path.exists(atlas):
        subprocess.run([sys.executable, os.path.join(REPO, 'tools/atlas.py'), a.game, 'zone', '0'], stdout=subprocess.DEVNULL, check=True)
    zones = json.load(open(atlas))['zones']
    rows = {} if a.redo else read_rows(out)
    save = a.save or SAVES[a.game]
    legs = []
    flag0 = 0x58c if a.game in ('b', 'w') else 0x5f0   # TrainerFlagGet's base (src/ov010/trainer_script.c, pokebw2's ov012)
    for zid in parse_ranges(a.zones, len(zones)):
        if zid >= len(zones):
            continue
        land = landing(zones[zid])
        if a.trainers:   # one leg per trainer: its flag cleared, the warp, G:npc (walk up: its sight or the talk starts the battle)
            for n in zones[zid].get('npcs', []):
                if 'trainer' not in n or 'rail' in n or not 3000 <= n['scr'] < 7000 or land is None:
                    continue
                name = f't{zid}-{n["uid"]}'
                tid = n['scr'] - (3000 if n['scr'] < 5000 else 5000)
                # the landing: the tile the trainer faces (dir 0 up 1 down 2 left 3 right), the player facing it: its sight
                # starts the battle on arrival (a door tile far from it left most trainers unreachable: `no path`)
                dx, dz = ((0, -1), (0, 1), (-1, 0), (1, 0))[n['dir'] & 3]
                tx, tz, back = n['x'] + dx, n['z'] + dz, (1, 0, 3, 2)[n['dir'] & 3]
                if name not in rows:
                    legs.append(f'{name} | {save} | Pa X:flag:{tid + flag0}:0 X:warp:{zid}:{tx}:{tz}:{back} To:3000 Wf:60 '
                                f'To:6000 G:npc:{n["uid"]} Wf:60 C | zone 0 0,0 | - |')
            continue
        name = f'z{zid}'
        if name in rows:
            continue
        if land is None:
            rows[name] = [name, 'no-spawn', '', '', '0', '']
            continue
        x, z = land
        legs.append(f'{name} | {save} | Pa X:warp:{zid}:{x}:{z} To:3000 Wf:60 {walk(zid, a.walk, a.game)} C | zone 0 0,0 | - | encounters')
    rdir = os.path.join(out, 'routes')
    os.makedirs(rdir, exist_ok=True)
    open(os.path.join(rdir, f'{a.game}.txt'), 'w').write('\n'.join(legs) + '\n')
    env = dict(os.environ, BWPORT_ROUTES_DIR=rdir, ROUTE_WORK=os.path.join(out, 'work'))
    if a.heapcheck is not None:
        env['BWPORT_HEAPCHECK'] = a.heapcheck
    cmd = [sys.executable, os.path.join(REPO, 'tools/route.py'), a.game, '-j', str(a.j)] + (['--no-build'] if a.no_build else [])
    print(f'g5_sweep: {a.game}: {len(legs)} legs, save {save}, -j {a.j}', flush=True)
    with open(os.path.join(out, 'route.out'), 'a') as log:
        p = subprocess.Popen(cmd if legs else ['true'], cwd=REPO, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        for line in p.stdout:
            log.write(line)
            log.flush()
            m = LINE_RE.match(line.strip())
            if m and m.group(1) == a.game:
                rows[m.group(2)] = classify(a.game, m.group(2), m.group(3), m.group(4), env['ROUTE_WORK'])
                with open(os.path.join(out, 'sweep.tsv'), 'w') as f:
                    f.write('\t'.join(HEAD) + '\n')
                    for k in sorted(rows, key=legkey):
                        f.write('\t'.join(rows[k]) + '\n')
        p.wait()
    with open(os.path.join(out, 'sweep.tsv'), 'w') as f:
        f.write('\t'.join(HEAD) + '\n')
        for k in sorted(rows, key=legkey):
            f.write('\t'.join(rows[k]) + '\n')
    summary(out, a.game)


if __name__ == '__main__':
    main()
