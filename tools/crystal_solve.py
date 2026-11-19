#!/usr/bin/env python3
"""Chargestone Cave's crystal puzzle solver (Black, White, Black 2, White 2: the same gimmick, ov051/ov107 field_gimmick_d06):
reads the pilot's map dumps and prints the pushes that open the way to a target as route tokens.

    python3 tools/crystal_solve.py --start ZONE,X,Z --target ZONE,X,Z DUMP.txt [DUMP.txt ...]

The dumps: one per window of a zone (the dump is 64 x 64 around the player; a zone of 3 x 2 chunks needs two), made with the
classes section on: `BWPORT_MAPDUMP_H=x0,z0,x1,z1 python3 tools/route.py GAME look SAVE "X:warp:ZONE:X:Z Wf:60 Md" --full-map`
(X:warp stands the player at a walkable tile of a zone; a test save only, docs/route-writer.md). The actors, warps and
classes of every dump are merged; zones are linked through the dumps' warps lists ("D2 66,50 1x1 -> zone 196 warp 1": zone 196's
dump says where its warp 1 is). START is where the player stands, TARGET a tile (or a door's tile: a neighbour of it counts).

The rule (the game's StartPush, ProbeElectricRock): a crystal (actor uid 253, all alike) slides in the pushed direction over free
tiles, at most 32, and moves only when the first blocked tile after them is an electric rock (tile class 0x32); it stops in front
of it, and a crystal already next to a rock cannot be pushed at all: so each crystal moves at most once, in at most 4 ways. The
player pushes from the tile behind the crystal and stays there. Search: breadth first over the sets of crystal places, a push
allowed when the tile behind the crystal is reachable (walk, one-way ledge hops, warps; NPCs and crystals block), until TARGET is
reachable: the shortest list. Output: `G:warp:Z Wf:60` where the next push is in another zone, then `F:stone:X,Z:DIR` (the pilot's
token: port/src/route_fieldmove.h). Plan the walk to TARGET yourself afterwards (G:warp, G:trig: the scene triggers on the way)."""
import argparse
import collections
import re
import sys

DX = (0, 0, -1, 1)
DZ = (-1, 1, 0, 0)
MAX_PUSH = 32


class Cave:
    def __init__(self, stone_uid=253, electric=0x32):
        self.cells = {}      # (zone, x, z) -> (class, pass bits, kind)
        self.stones = []     # (zone, x, z)
        self.npcs = set()
        self.warps = {}      # (zone, index) -> [tiles]
        self.dest = {}       # (zone, index) -> (zone, index)
        self.stone_uid = stone_uid
        self.electric = electric

    def parse(self, text):
        lines = text.split("\n")
        zone = int(re.search(r"zone (\d+) player", lines[0]).group(1))
        i = 0
        while i < len(lines):
            ln = lines[i]
            if ln.startswith("classes"):
                xs = [int(t) for t in lines[i + 1].split()]
                i += 2
                while i < len(lines) and lines[i].strip() and re.match(r"^\s*-?\d+ ", lines[i]):
                    tk = lines[i].split()
                    if len(tk) - 1 == len(xs):
                        for x, t in zip(xs, tk[1:]):
                            if t != "?":
                                self.cells[(zone, x, int(tk[0]))] = (int(t[:4], 16), int(t[5], 16), t[6])
                    i += 1
                continue
            m = re.match(r"\s+N uid (\d+) at (-?\d+),(-?\d+)", ln)
            if m and ln.find("rail") < 0:
                p = (zone, int(m.group(2)), int(m.group(3)))
                if int(m.group(1)) == self.stone_uid:
                    if p not in self.stones:
                        self.stones.append(p)
                else:
                    self.npcs.add(p)
            m = re.match(r"\s+D(\d+) (-?\d+),(-?\d+) (\d+)x(\d+) -> zone (\d+) warp (\d+)", ln)
            if m:
                n, x, z, w, h, dz_, dw = (int(v) for v in m.groups())
                self.warps[(zone, n)] = [(zone, x + a, z + b) for a in range(w) for b in range(h)]
                self.dest[(zone, n)] = (dz_, dw)
            i += 1

    def walk(self, p):
        return p in self.cells and self.cells[p][2] in ".,"

    def elec(self, p):
        return p in self.cells and self.cells[p][0] == self.electric

    @staticmethod
    def nb(p, d):
        return (p[0], p[1] + DX[d], p[2] + DZ[d])

    def links(self):
        """pairs (door tiles of one warp, the walkable tiles beside the door it leads to)"""
        out = []
        for w, (dz_, dw) in self.dest.items():
            if (dz_, dw) not in self.warps:
                continue
            src = {self.nb(t, d) for t in self.warps[w] for d in range(4) if self.walk(self.nb(t, d))}
            dst = {self.nb(t, d) for t in self.warps[(dz_, dw)] for d in range(4) if self.walk(self.nb(t, d))}
            out.append((src, dst))
        return out

    def reach(self, start, stones, par=None):
        """the tiles the player can walk to from start with the crystals at stones; par (a dict) gets tile -> the tile before it"""
        block = set(stones) | self.npcs
        seen = {start}
        q = [start]
        while q:
            p = q.pop()
            for src, dst in self.links_cache:
                if p in src:
                    for n in dst:
                        if n not in seen and n not in block:
                            seen.add(n)
                            q.append(n)
                            if par is not None:
                                par[n] = p
            c = self.cells.get(p)
            if c is None:
                continue
            for d in range(4):
                if not (c[1] >> d) & 1:
                    continue
                n = self.nb(p, d)
                if n in seen or n in block:
                    continue
                if not self.walk(n):
                    if self.cells.get(n, (0, 0, "#"))[2] == "^v<>"[d]:   # a ledge jumped that way: lands one tile beyond
                        n = self.nb(n, d)
                        if n in seen or n in block or not self.walk(n):
                            continue
                    else:
                        continue
                seen.add(n)
                q.append(n)
                if par is not None:
                    par[n] = p
        return seen

    def path(self, start, stones, goal):
        """the tiles walked from start to goal with the crystals at stones (zone crossings are consecutive tiles of two zones), or None"""
        par = {}
        if goal not in self.reach(start, stones, par):
            return None
        out = [goal]
        while out[-1] != start:
            out.append(par[out[-1]])
        return out[::-1]

    def push(self, stones, i, d):
        """where crystal i stops when pushed in direction d, or None (no move)"""
        s = stones[i]
        if any(self.elec(self.nb(s, dd)) for dd in range(4)):
            return None   # next to a rock: fixed
        others = set(stones) - {s}
        p, k = s, 0
        while k < MAX_PUSH:
            n = self.nb(p, d)
            if n not in self.cells or self.cells[n][2] == "#":
                return p if self.elec(n) and k > 0 else None
            if n in others or n in self.npcs:
                return None
            p, k = n, k + 1
        return None

    def solve(self, start, target):
        """the shortest push list [(crystal, dir, stop)] after which target is reachable; None; [] when it is reachable now"""
        self.links_cache = self.links()
        tgt = {target} | {self.nb(target, d) for d in range(4)}
        st0 = tuple(self.stones)
        seen = {st0: None}
        q = collections.deque([st0])
        while q:
            st = q.popleft()
            r = self.reach(start, st)
            if tgt & r:
                seq = []
                while seen[st] is not None:
                    st, mv = seen[st]
                    seq.append(mv)
                return seq[::-1]
            for i, s in enumerate(st):
                for d in range(4):
                    if (s[0], s[1] - DX[d], s[2] - DZ[d]) not in r:   # the tile behind it
                        continue
                    e = self.push(st, i, d)
                    if e is None:
                        continue
                    ns = st[:i] + (e,) + st[i + 1:]
                    if ns not in seen:
                        seen[ns] = (st, (s, d, e))
                        q.append(ns)
        return None


def hops(cave, path):
    """the tokens for the zone crossings of a walked path: G:warp:Z; with G:X,Z (the tile beside the door) first when the zone has
    several doors to Z (G:warp takes the nearest one)"""
    out = []
    for a, b in zip(path, path[1:]):
        if a[0] != b[0]:
            if sum(1 for (zn, _), (dz, _) in cave.dest.items() if zn == a[0] and dz == b[0]) > 1:
                out.append("G:%d,%d" % (a[1], a[2]))
            out.append("G:warp:%d Wf:60" % b[0])
    return out


def tokens(seq, start_zone, cave=None, start=None):
    """the push list as route tokens. With the cave and the start tile it also walks between zones (the way to the tile behind a
    crystal may lead through another zone's doors); without them only a G:warp:Z where the next crystal is in another zone"""
    out, zone, pos = [], start_zone, start
    stones = list(cave.stones) if cave else []
    for s, d, e in seq:
        if cave is not None and start is not None:
            behind = (s[0], s[1] - DX[d], s[2] - DZ[d])
            out += hops(cave, cave.path(pos, stones, behind) or [])
            stones[stones.index(s)] = e
            pos = behind
        elif s[0] != zone:
            out.append("G:warp:%d Wf:60" % s[0])
            zone = s[0]
        out.append("F:stone:%d,%d:%d" % (s[1], s[2], d))
    return out


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--start", required=True)
    ap.add_argument("--target", required=True)
    ap.add_argument("--uid", type=int, default=253)
    ap.add_argument("--electric", type=lambda v: int(v, 0), default=0x32)
    ap.add_argument("dumps", nargs="+")
    a = ap.parse_args(argv)
    cave = Cave(a.uid, a.electric)
    for f in a.dumps:
        cave.parse(open(f).read())
    start = tuple(int(v) for v in a.start.split(","))
    target = tuple(int(v) for v in a.target.split(","))
    print("%d tiles, %d crystals, %d other actors, %d warps" % (len(cave.cells), len(cave.stones), len(cave.npcs), len(cave.dest)))
    seq = cave.solve(start, target)
    if seq is None:
        print("no solution: the dumps may not cover the way (or a ledge/NPC blocks it)")
        return 1
    print("%d pushes:" % len(seq))
    for s, d, e in seq:
        print("  zone %d: crystal %d,%d, direction %d, stops at %d,%d" % (s[0], s[1], s[2], d, e[1], e[2]))
    print(" ".join(tokens(seq, start[0], cave, start)))
    tgt = [target] + [cave.nb(target, d) for d in range(4)]
    stones = list(cave.stones)
    pos = start
    for s, d, e in seq:
        stones[stones.index(s)] = e
        pos = (s[0], s[1] - DX[d], s[2] - DZ[d])
    for t in tgt:
        p = cave.path(pos, stones, t)
        if p is not None:
            print("then:", " ".join(hops(cave, p)) or "(the target is in this zone)", "-> walk to", "%d,%d,%d" % t)
            break
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
