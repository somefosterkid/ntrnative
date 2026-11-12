#!/usr/bin/env python3
"""Tests for atlas.py on made-up records (no game data), and one check of Black's ROM when orig/ has it:

    python3 tools/atlas_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import atlas as A  # noqa: E402


def entities():
    bg = struct.pack("<4H3i", 8001, 5, 1, 0, 10, 20, 0)
    npc = struct.pack("<12HIHHi", 3, 11, 15, 1, 0, 3001, 1, 4, 0, 0, 1, 1, 0, 12, 34, 0)
    warp = struct.pack("<HHBBHhhhHHH", 320, 1, 3, 4, 0, 7 * 16 + 8, 0, 9 * 16 + 8, 1, 2, 0)
    trig = struct.pack("<5HHHHHhH", 2, 1, 16509, 0, 0, 5, 6, 3, 1, 0, 0)
    body = bytes([1, 1, 1, 1]) + bg + npc + warp + trig
    return struct.pack("<I", len(body)) + body


def test_entities():
    bg, npcs, warps, trigs, ok = A.parse_entities(entities())
    assert ok
    assert (bg[0]["x"], bg[0]["z"], bg[0]["flag"], "item" in bg[0]) == (10, 20, 5, True)
    assert (npcs[0]["uid"], npcs[0]["x"], npcs[0]["z"], npcs[0]["sight"]) == (3, 12, 34, 4)
    assert (warps[0]["to"], warps[0]["to_warp"], warps[0]["x"], warps[0]["z"], warps[0]["h"]) == (320, 1, 7, 9, 2)
    assert (trigs[0]["work"], trigs[0]["value"], trigs[0]["x"], trigs[0]["w"]) == (16509, 1, 5, 3)


def test_matrix_and_walk():
    m = A.parse_matrix(struct.pack("<4H", 1, 0, 2, 1) + struct.pack("<4I", 5, 6, 10, 11))
    assert (m["w"], m["chunks"], m["zones"]) == (2, [5, 6], [10, 11])
    assert A.parse_matrix(struct.pack("<4H", 0, 0, 1, 1) + struct.pack("<I", 5))["zones"] is None
    tiles = b"".join(struct.pack("<HHI", 0, 0, (1 << 16) if i % 2 else 0) for i in range(1024))
    chunk = struct.pack("<HHIIII", 0x4257, 3, 0x14, 0x14, 0, 0) + struct.pack("<HH", 32, 32) + tiles
    w = A.chunk_walk(chunk)
    assert w[0] and not w[1] and len(w) == 1024
    assert A.chunk_walk(struct.pack("<H", 0x474E) + bytes(20)) is None


def test_path():
    zones = [{"warps": []}, {"warps": [{"i": 0, "to": 2, "to_warp": 0, "x": 1, "z": 2}]}, {"warps": []}]
    data = {"zones": zones, "edges": {"0>1": [{"x": 31, "z": [3, 5], "dir": "right", "open": 3}]}}
    steps = A.path(data, 0, 2)
    assert [(a, b) for a, _, b in steps] == [(0, 1), (1, 2)], steps
    assert A.path(data, 2, 0) is None


def test_rom():
    if not (A.ROOT / "orig" / "baserom_b_us.nds").exists():
        print("skip test_rom: no orig/baserom_b_us.nds")
        return
    data = A.load("b")
    assert len(data["zones"]) == 427 and data["entity_size_mismatch"] == 0
    assert [b for _, _, b in A.path(data, 397, 6)] == [320, 319, 6]


for name, fn in list(globals().items()):
    if name.startswith("test_"):
        fn()
        print("ok", name)
