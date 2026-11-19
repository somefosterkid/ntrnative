#!/usr/bin/env python3
"""Tests for tools/crystal_solve.py (Chargestone's crystal puzzle) on synthetic dumps, no game: python3 tools/crystal_solve_tests.py
(prints "ok" per test, exits 1 on a failure)."""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import crystal_solve as C  # noqa: E402

# '#' wall, 'E' electric rock (a wall), '.' floor, 'S' a crystal on floor, 'N' an NPC on floor
ROOM = [
    "###E###",
    "###.###",
    "#..S...#",
    "#.#.###",
    "#.....#",
    "#######",
]


def dump(zone, rows, warps=(), npcs=()):
    h, w = len(rows), max(len(r) for r in rows)
    kind = lambda x, z: rows[z][x] if 0 <= z < h and 0 <= x < len(rows[z]) else "#"
    walk = lambda x, z: kind(x, z) in ".SN"
    out = ["map: B zone %d player 1,2 y 0 facing 0 (V-blank 1, end) matrix 1x1 chunks; tiles 0..%d x 0..%d (gen5)" % (zone, w - 1, h - 1)]
    out.append("classes (tile class hex, pass bits u d l r as a digit 0-15):")
    out.append("      " + " ".join("%6d" % x for x in range(w)))
    for z in range(h):
        cells = []
        for x in range(w):
            k = kind(x, z)
            if k == "E":
                cells.append("0032/0#")
            elif k == "#":
                cells.append("0001/0#")
            else:
                bits = sum(1 << d for d in range(4) if walk(x + C.DX[d], z + C.DZ[d]))
                cells.append("0010/%x." % bits)
        out.append("%5d " % z + " ".join(cells))
    out.append("warps (%d):" % len(warps))
    out += ["  D%d %d,%d 1x1 -> zone %d warp %d (door)" % (n, x, z, dz, dw) for n, (x, z, dz, dw) in enumerate(warps)]
    out.append("actors (%d):" % (sum(r.count("S") for r in rows) + len(npcs)))
    for z, r in enumerate(rows):
        for x, ch in enumerate(r):
            if ch == "S":
                out.append("  N uid 253 at %d,%d (from %d,%d) code 0 move 0" % (x, z, x, z))
    out += ["  N uid 4 at %d,%d (from %d,%d) code 1 move 0" % (x, z, x, z) for x, z in npcs]
    return "\n".join(out) + "\n"


def cave(text, *more):
    c = C.Cave()
    for t in (text,) + more:
        c.parse(t)
    return c


def test_push_up_clears_the_corridor():
    c = cave(dump(1, ROOM))
    assert len(c.stones) == 1
    seq = c.solve((1, 1, 2), (1, 5, 2))   # the stone at 3,2 blocks the way east; (3,3) is below it
    assert [(s, d, e) for s, d, e in seq] == [((1, 3, 2), 0, (1, 3, 1))], seq
    assert C.tokens(seq, 1) == ["F:stone:3,2:0"]


def test_already_open_and_impossible():
    open_room = [r.replace("S", ".") for r in ROOM]
    assert cave(dump(1, open_room)).solve((1, 1, 2), (1, 5, 2)) == []
    # no rock above the stone: the push does nothing, the way stays shut
    no_rock = [r.replace("E", "#") for r in ROOM]
    assert cave(dump(1, no_rock)).solve((1, 1, 2), (1, 5, 2)) is None


def test_fixed_next_to_a_rock_and_npcs_block():
    c = cave(dump(1, ROOM))
    s = [(1, 3, 1)]
    assert c.push(s, 0, 0) is None           # next to the rock: fixed
    c2 = cave(dump(1, ROOM, npcs=[(3, 1)]))   # an NPC in the stone's way up
    assert c2.solve((1, 1, 2), (1, 5, 2)) is None


def test_warps_link_zones_and_tokens_name_the_zone():
    z1 = ["#######", "#..#...#", "#######"]
    z2 = ["#######", "#.....#", "#######"]
    a = dump(1, z1, warps=[(3, 1, 2, 0)])   # the door at 3,1 (a wall tile) leads to zone 2's warp 0
    b = dump(2, ROOM, warps=[(0, 2, 1, 0)])   # zone 2's warp 0 at 0,2 leads back
    c = cave(a, b)
    assert len(c.dest) == 2
    # from zone 1's west room the way to zone 2's east end goes through the door and needs the push
    seq = c.solve((1, 1, 1), (2, 5, 2))
    assert seq is not None and C.tokens(seq, 1) == ["G:warp:2 Wf:60", "F:stone:3,2:0"], seq
    assert C.tokens(seq, 1, c, (1, 1, 1)) == ["G:warp:2 Wf:60", "F:stone:3,2:0"], seq
    del z2


def test_hops_between_zones_name_the_door_when_there_are_several():
    z1 = ["######", "#....#", "######"]
    a = dump(1, z1, warps=[(2, 0, 2, 0), (3, 0, 2, 1)])   # two doors from zone 1 to zone 2, above 2,1 and 3,1
    b = dump(2, ROOM, warps=[(0, 2, 1, 0), (0, 4, 1, 1)])
    c = cave(a, b)
    seq = c.solve((1, 1, 1), (2, 5, 2))
    assert seq is not None
    t = C.tokens(seq, 1, c, (1, 1, 1))
    assert t[0] in ("G:2,1", "G:3,1") and t[1:] == ["G:warp:2 Wf:60", "F:stone:3,2:0"], t   # the tile beside the door it uses


if __name__ == "__main__":
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            try:
                fn()
                print("ok", name)
            except AssertionError as e:
                bad += 1
                print("FAIL", name, e)
    sys.exit(1 if bad else 0)
