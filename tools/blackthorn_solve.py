#!/usr/bin/env python3
"""Blackthorn Gym platform solver (HeartGold / SoulSilver): an offline BFS over the three platforms' (x, z, rotation).

The rules are transcribed from the decomp's field/gymmick/blackthorn.c (numbers only, no text): see docs/hgss.md "Blackthorn Gym".
Input: the pilot's "bt:" dump (BWPORT_BTDUMP=1 on a run in the gym: the raw zone map, '#' collision bit, 'M' magma, '.' other) and
the start position. Output: the buttons to step onto, one G:X,Z token each, with Wf:60 between the presses.

    python3 tools/blackthorn_solve.py DUMPFILE [--start X,Z] [--goal X,Z]

Platforms start where InitBlackthornGym puts them: P0 13,75 rot 0; P1 9,58 rot 1; P2 14,32 rot 0 (P0 and P2 are 5 x 7, P1 is 4 x 8).
"""
import argparse
import re
import sys
from collections import deque

INIT = [(13, 75, 0), (9, 58, 1), (14, 32, 0)]
EXT_MAIN = {0: [10, 10, 13, 18, 25, 38, 51, 64, 64, 23, 28, 35, 45, 55],
            1: [8, 9, 10, 14, 19, 26, 37, 51, 64, 17, 19, 23, 28, 35, 46, 55, 64, 64, 64, 30, 35, 41, 49, 57]}
EXT_EDGE = {0: [64, 64, 64, 64, 42], 1: [64, 20, 30, 46]}
TRAINERS = {(11, 66), (8, 61), (20, 54), (21, 45), (13, 40)}   # stationary actors: not walked through
HOLES = {(5, 9), (23, 41), (3, 62)}                            # the falling triggers (script 3)


def rot(p, o, r):
    dx, dz = p[0] - o[0], p[1] - o[1]
    for _ in range(r % 4):
        dx, dz = -dz, dx
    return (o[0] + dx, o[1] + dz)


def shape_of(i):
    return 1 if i == 1 else 0


def geometry(i, st):
    """the platform's point sets for state (x, z, r): the game's BlackthornPlatform_Init for rot 0, then rotated by r about x,z"""
    x, z, r = st
    t = shape_of(i)
    w, h = (5, 7) if t == 0 else (4, 8)
    g = {}
    if t == 0:
        g['right'] = [(x + 3, z - 2 + k) for k in range(7)]
        g['left'] = [(x - 3, z - 2 + k) for k in range(7)]
        g['bottom'] = [(x - 2 + k, z + 5) for k in range(5)]
        g['top'] = [(x - 2 + k, z - 3) for k in range(5)]
        g['area'] = [(x - 4 - a, z + 4 - b) for a in range(2) for b in range(7)]
    else:
        g['right'] = [(x + 2, z - 2 + k) for k in range(8)]
        g['left'] = [(x - 3, z - 2 + k) for k in range(8)]
        g['bottom'] = [(x - 2 + k, z + 6) for k in range(4)]
        g['top'] = [(x - 2 + k, z - 3) for k in range(4)]
        g['area'] = [(x - 4 - a, z + 5 - b) for a in range(3) for b in range(8)]
    g['edge'] = [(x + 3, z - 2 + k) for k in range(w)]
    g['floor'] = [(x - 1 + j, z - 1 + k) for k in range(6) for j in range(3)]
    g['rot_btn'] = (x, z)
    g['mr_btn'] = rot((x + 1, z), (x, z), r)    # "left" button: ACTION_MOVE_RIGHT, moves toward direction r
    g['ml_btn'] = rot((x - 1, z), (x, z), r)    # "right" button: ACTION_MOVE_LEFT, moves toward direction r + 2
    for k in ('right', 'left', 'bottom', 'top', 'area', 'edge', 'floor'):
        g[k] = [rot(p, (x, z), r) for p in g[k]]
    return g, w, h


class Grid:
    def __init__(self, rows):
        self.rows = rows

    def at(self, x, z):
        if 0 <= z < len(self.rows) and 0 <= x < len(self.rows[z]):
            return self.rows[z][x]
        return '#'

    def free(self, p):   # the platform's test: no collision bit and a magma tile
        return self.at(*p) == 'M'


DV = [(1, 0), (0, 1), (-1, 0), (0, -1)]


def move(grid, i, st, which):
    """MOVE_RIGHT (which 0) / MOVE_LEFT (1): the new state, or None (no change: a bonk at once or after k steps and back)"""
    x, z, r = st
    g, w, h = geometry(i, st)
    wall = g['right'] if which == 0 else g['left']
    d = DV[(r + (0 if which == 0 else 2)) % 4]
    for k in range(w):
        if not all(grid.free((p[0] + d[0] * k, p[1] + d[1] * k)) for p in wall):
            return None   # k = 0 nothing happens, k > 0 the platform goes back to its start: either way no change
    return (x + d[0] * w, z + d[1] * w, r)


def rotate(grid, i, st):
    x, z, r = st
    g, w, h = geometry(i, st)
    t = shape_of(i)
    if not (all(grid.free(p) for p in g['bottom']) and all(grid.free(p) for p in g['top']) and all(grid.free(p) for p in g['left'])
            and all(grid.free(p) for p in g['right'][:w - 1])):
        return None
    for pts, ext in ((g['area'], EXT_MAIN[t]), (g['edge'], EXT_EDGE[t])):
        for p, e in zip(pts, ext):
            if not grid.free(p) and e < 64:
                return None   # a partial turn that comes back: no change
    return (x, z, (r + 1) % 4)


def walk_region(grid, plats, start):
    """tiles the player reaches from start on foot: a platform floor or a non-magma free tile; button tiles and trainers are not walked over"""
    floors, buttons = set(), set()
    for i, st in enumerate(plats):
        g, _, _ = geometry(i, st)
        floors |= set(g['floor'])
        buttons |= {g['rot_btn'], g['mr_btn'], g['ml_btn']}
    seen = {start}
    q = deque([start])
    while q:
        p = q.popleft()
        for d in DV:
            n = (p[0] + d[0], p[1] + d[1])
            if n in seen or n in TRAINERS or n in HOLES or n in buttons:
                continue
            c = grid.at(*n)
            if n in floors or c == '.':
                seen.add(n)
                q.append(n)
    return seen, buttons


def solve(grid, start, goal, limit=200000):
    s0 = (tuple(INIT), start)
    prev = {s0: None}
    q = deque([s0])
    while q:
        cur = q.popleft()
        plats, pos = cur
        reach, _ = walk_region(grid, plats, pos)
        if goal in reach:
            path, fin = [], cur
            while prev[cur] is not None:
                cur, step = prev[cur]
                path.append((cur[0], cur[1], step))   # the platforms and the player before the press, and the press
            return path[::-1], fin   # the presses, and the platforms and the player where the goal is reachable
        # a button is pressed from a reachable neighbour: the step onto it
        for i, st in enumerate(plats):
            g, w, h = geometry(i, st)
            for kind, btn in (('rot', g['rot_btn']), ('mr', g['mr_btn']), ('ml', g['ml_btn'])):
                if not any((btn[0] + d[0], btn[1] + d[1]) in reach for d in DV):
                    continue
                if kind == 'rot':
                    ns, np_ = rotate(grid, i, st), btn
                else:
                    ns = move(grid, i, st, 0 if kind == 'mr' else 1)
                    np_ = btn if ns is None else (btn[0] + ns[0] - st[0], btn[1] + ns[1] - st[1])
                if ns is None:
                    continue
                nplats = tuple(ns if j == i else s for j, s in enumerate(plats))
                key = (nplats, np_)
                if key not in prev:
                    prev[key] = (cur, (i, kind, btn, np_))
                    q.append(key)
        if len(prev) > limit:
            break
    return None


def waypoints(grid, plats, pos, btn):
    """the corner tiles of a walk from pos onto the button btn over walkable tiles only (never across another button: a step onto one
    starts its platform; never next to nothing): the pilot's planner walks a straight segment between two corners as it is; a button
    pressed again is left and re-entered, so pos == btn walks off first"""
    floors, buttons = set(), set()
    for i, st in enumerate(plats):
        g, _, _ = geometry(i, st)
        floors |= set(g['floor'])
        buttons |= {g['rot_btn'], g['mr_btn'], g['ml_btn']}

    def ok(n):
        return n not in TRAINERS and n not in HOLES and n not in buttons and (n in floors or grid.at(*n) == '.')
    prev = {pos: None}
    q = deque([pos])
    while q:
        p = q.popleft()
        for d in DV:
            n = (p[0] + d[0], p[1] + d[1])
            if n == btn and p != btn:
                tiles = [btn]
                while p is not None:
                    tiles.append(p)
                    p = prev[p]
                tiles = tiles[::-1]
                out = [tiles[0]]
                for a, b, c in zip(tiles, tiles[1:], tiles[2:]):
                    if (b[0] - a[0], b[1] - a[1]) != (c[0] - b[0], c[1] - b[1]):
                        out.append(b)
                out.append(tiles[-1])
                return out[1:]   # the corners after the start
            if n in prev or not ok(n):
                continue
            prev[n] = p
            q.append(n)
    return [btn]


def read_dump(paths):
    """the raw rows of one or more dumps merged: the pilot knows only the chunks near the player ('?' elsewhere), so a run per
    chunk (BWPORT_HG_WARP z 80, 48, 16) is merged here, a known tile winning over '?'"""
    rows = {}
    for path in paths:
        for line in open(path, errors='replace'):
            m = re.match(r'bt: raw (\d+) (\S{32})\s*$', line.strip())
            if m:
                z, new = int(m.group(1)), m.group(2)
                old = rows.get(z, '?' * 32)
                rows[z] = ''.join(o if n == '?' else n for o, n in zip(old, new))
    if not rows:
        sys.exit('no "bt: raw" rows in ' + ' '.join(paths))
    return Grid([rows.get(z, '?' * 32) for z in range(max(rows) + 1)])


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('dump', nargs='+')
    ap.add_argument('--start', default='13,86')
    ap.add_argument('--goal', default='12,4')
    a = ap.parse_args()
    start = tuple(int(v) for v in a.start.split(','))
    goal = tuple(int(v) for v in a.goal.split(','))
    grid = read_dump(a.dump)
    res = solve(grid, start, goal)
    if res is None:
        sys.exit('no solution')
    path, fin = res
    toks = []
    for plats, pos, (i, kind, btn, end) in path:
        print('# P%d %s: onto %d,%d, the player ends at %d,%d' % (i, kind, btn[0], btn[1], end[0], end[1]))
        for w in waypoints(grid, plats, pos, btn)[:-1]:
            toks.append('G:%d,%d' % w)
        toks.append('G:%d,%d Wf:60' % btn)
    # the walk from the last press to the goal: the corners over the free tiles
    for w in waypoints(grid, fin[0], fin[1], goal):
        toks.append('G:%d,%d' % w)
    print(' '.join(toks))


if __name__ == '__main__':
    main()
