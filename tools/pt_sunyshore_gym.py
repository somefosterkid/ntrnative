#!/usr/bin/env python3
"""Search for a way through Platinum's Sunyshore gym rooms (tools/routes/pt.txt legs ver51-ver55 were written from it).

Each room is a net of corridors whose crossings are gears (gym_features.c, SunyshoreGym_*): the arms are collision regions, a
rotation state 0..3 blocks six to fourteen of them, and a trigger on a gear's centre tile turns every gear of the room (a
button: NORMAL +1, REVERSE -1, DOUBLE +2). The planner already reads the game's own collision (route_gen4.c Pr4_DynTile), so
a G: hop plans in the state the room is in; what it cannot do is plan across a button. This tool searches (tile, state):

    python3 tools/pt_sunyshore_gym.py ROOM STATE SX,SZ GX,GZ [NPCX,NPCZ ...]    # ROOM 1..3, the room's rotation state at the start

and prints the corner tiles of the shortest way with the state after each (G:X,Z[sN]): one hop per corner, a Wf:60 after a
button tile. A room entered by its far door starts in state 2 (room 1), 1 (room 2), 0 (room 3: its only door is the near one;
persisted_map_features_init.c); by the near door in 0.
Inputs: the terrain collision of the room's chunk from the ROM (atlas data: the zone's matrix), the regions and the per-state
lists parsed from the decompiled gym_features.c in build/pokeplatinum, and (optional, needed for rooms 2 and 3 which have two
or three floor levels: a step of 20+ is a wall) the heights from the game: run any leg's look with BWPORT_HDUMP=0,0,31,31,6000
inside the room and pass its game.log as HDUMP=path. NPCs are walls (give their tiles).
"""
import json
import os
import re
import struct
import sys
from collections import deque

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import atlas  # noqa: E402
import atlas_gen4  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ZONE = {1: 154, 2: 155, 3: 156}
BUTTONS = {1: {(3, 8): "N", (13, 8): "N"}, 2: {(6, 13): "N", (6, 8): "R", (11, 8): "R"},
           3: {(6, 8): "N", (16, 13): "N", (6, 18): "D", (16, 18): "D"}}
DELTA = {"N": 1, "R": -1, "D": 2}
NUM = {1: 6, 2: 11, 3: 14}   # the live entries of each room's state lists (room 2's lists are padded)


def terrain(zone):
    from pathlib import Path
    rom = (Path(ROOT) / "orig" / "baserom_plat_us.nds").read_bytes()
    g = atlas_gen4.GEN4["pt"]
    mats = atlas.arc_path(rom, g["matrix"])
    land = atlas.arc_path(rom, g["land"])
    zones = json.load(open(os.path.join(ROOT, "build", "atlas", "pt.json")))["zones"]
    m = atlas_gen4.parse_matrix4(mats[zones[zone]["matrix"]])
    return struct.unpack_from("<1024H", land[m["chunks"][0]], g["attr"])


def tables(room):
    src = open(os.path.join(ROOT, "build", "pokeplatinum", "src", "overlay008", "gym_features.c")).read()
    m = re.search(r"sSunyshoreRoom%dCollisionRegions\[\d+\] = \{(.*?)\};" % room, src, re.S)
    regions = [tuple(int(v) for v in re.findall(r"= (\d+)", r)) for r in re.findall(r"\{[^{}]*\}", m.group(1))]
    states = []
    for s in range(4):
        m = re.search(r"sSunyshoreRoom%dState%dCollisions\[\] = \{(.*?)\};" % (room, s), src, re.S)
        states.append([int(v) for v in re.findall(r"\d+", m.group(1))][:NUM[room]])
    return regions, states


def heights(path):
    H = {}
    for line in open(path):
        p = line.split()
        if len(p) >= 18 and p[0] == "hdump:" and p[1].isdigit():
            H[(int(p[1]), int(p[2]))] = int(p[10])   # the height asked at 16
    return H


def solve(room, s0, start, goal, npcs, H=None):
    A = terrain(ZONE[room])
    regions, states = tables(room)
    blocked = []
    for lst in states:
        b = set()
        for i in lst:
            x, z, sx, sz = regions[i]
            b |= {(xx, zz) for xx in range(x, x + sx) for zz in range(z, z + sz)}
        blocked.append(b)
    btn = BUTTONS[room]
    seen = {(start, s0): None}
    q = deque([(start, s0)])
    while q:
        cur = q.popleft()
        (x, z), s = cur
        if (x, z) == goal:
            path = []
            while cur is not None:
                path.append(cur)
                cur = seen[cur]
            return path[::-1]
        for dx, dz in ((0, -1), (0, 1), (-1, 0), (1, 0)):
            nx, nz = x + dx, z + dz
            if not (0 <= nx < 32 and 0 <= nz < 32) or A[nz * 32 + nx] >> 15 or (nx, nz) in blocked[s] or (nx, nz) in npcs:
                continue
            if H is not None and abs(H[(nx, nz)] - H[(x, z)]) >= 20:
                continue
            ns = (s + DELTA[btn[(nx, nz)]]) % 4 if (nx, nz) in btn else s
            if ((nx, nz), ns) not in seen:
                seen[((nx, nz), ns)] = cur
                q.append(((nx, nz), ns))
    return None


def corners(path, btn):
    out = []
    for i in range(1, len(path)):
        p, s = path[i]
        if i == len(path) - 1 or p in btn:
            out.append((p, s))
            continue
        a, b = path[i - 1][0], path[i + 1][0]
        if (p[0] - a[0], p[1] - a[1]) != (b[0] - p[0], b[1] - p[1]):
            out.append((p, s))
    return out


def main(argv):
    if len(argv) < 5:
        print(__doc__)
        return 2
    room, s0 = int(argv[1]), int(argv[2])
    start = tuple(int(v) for v in argv[3].split(","))
    goal = tuple(int(v) for v in argv[4].split(","))
    npcs = {tuple(int(v) for v in a.split(",")) for a in argv[5:]}
    H = heights(os.environ["HDUMP"]) if os.environ.get("HDUMP") else None
    path = solve(room, s0, start, goal, npcs, H)
    if path is None:
        print("no way")
        return 1
    print("%d steps" % (len(path) - 1))
    print(" ".join("G:%d,%d[s%d]" % (p[0], p[1], s) for p, s in corners(path, BUTTONS[room])))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
