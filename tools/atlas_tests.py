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
    # a scripted door (tools/story.py scripted_edges): after the warps and edges, with its token
    data["scripted"] = [{"from": 2, "to": 0, "tok": "G:npc:3", "how": "actor uid 3"}, {"from": 0, "to": 1,
                                                                                         "tok": "G:trig:0", "how": "T0"}]
    assert A.path(data, 2, 0) == [(2, "script G:npc:3: actor uid 3", 0)]
    assert A.path(data, 0, 1)[0][1].startswith("edge"), "the edge, not the script"


def test_path_skips_dead_warps():
    # zone 0's door has two warps, one into each version's interior (1 or 2); this game's atlas marks the other dead
    w = lambda i, to, **k: dict({"i": i, "to": to, "to_warp": 0, "x": 5, "z": 5}, **k)   # noqa: E731
    zones = [{"warps": [w(0, 1, dead=True), w(1, 2)]}, {"warps": [w(0, 3)]}, {"warps": [w(0, 3)]}, {"warps": []}]
    data = {"zones": zones, "edges": {}}
    assert [b for _, _, b in A.path(data, 0, 3)] == [2, 3]
    assert A.path(data, 0, 1) is None
    zones[0]["warps"][0].pop("dead")
    assert [b for _, _, b in A.path(data, 0, 3)] == [1, 3]



def test_few_cuts_to_a_pointer_to_all():
    lines = ["w%d" % i for i in range(11)]
    assert A.few(lines, True, "warps") == lines
    out = A.few(lines, False, "warps")
    assert len(out) == A.BRIEF + 1 and out[-1] == "  ... 3 more warps: --all", out
    assert A.few(lines[:4], False, "warps") == lines[:4]


def test_rom():
    if not (A.ROOT / "orig" / "baserom_b_us.nds").exists():
        print("skip test_rom: no orig/baserom_b_us.nds")
        return
    data = A.load("b")
    assert len(data["zones"]) == 427 and data["entity_size_mismatch"] == 0
    assert [b for _, _, b in A.path(data, 397, 6)] == [320, 319, 6]


def events4():
    bg = struct.pack("<HH3iHH", 8001, 2, 10, 20, 0, 0, 0)
    npc = struct.pack("<6Hh3Hhh2Hi", 3, 11, 15, 1, 0, 3001, 1, 4, 0, 0, 0, 0, 12, 34, 0)
    warp = struct.pack("<4HI", 7, 9, 320, 1, 0)
    trig = struct.pack("<H2h5H", 2, 5, 6, 3, 1, 0, 1, 16509)
    return b"".join(struct.pack("<I", 1) + r for r in (bg, npc, warp, trig))


def test_gen4_events():
    bg, npcs, warps, trigs, ok = A.atlas_gen4.parse_events4(events4())
    assert ok
    assert not A.atlas_gen4.parse_events4(events4() + b"\1")[4], "a file longer than its records"
    assert (bg[0]["x"], bg[0]["z"], bg[0]["type"], "item" in bg[0]) == (10, 20, 2, True)
    assert (npcs[0]["uid"], npcs[0]["x"], npcs[0]["z"], npcs[0]["sight"], npcs[0]["scr"]) == (3, 12, 34, 4, 3001)
    assert (warps[0]["to"], warps[0]["to_warp"], warps[0]["x"], warps[0]["z"], warps[0]["w"]) == (320, 1, 7, 9, 1)
    assert (trigs[0]["work"], trigs[0]["value"], trigs[0]["x"], trigs[0]["w"]) == (16509, 1, 5, 3)
    zone = {"zone": 0, "matrix": 0, "entities": 0, "script": 1, "init": 2, "text": 3, "enc": 0, "parent": 0xFFFF,
            "name": 0, "type": 0, "chunks": [], "bg": bg, "npcs": npcs, "warps": warps, "triggers": trigs}
    out = A.show_zone({"zones": [zone], "edges": {}}, 0)
    assert "init 2" in out and "W0 7,9 -> zone 320 warp 1\n" in out and "b0 10,20 scr 8001 type 2" in out, out


def test_gen4_matrix_and_walk():
    # 2 x 1 with header ids, altitudes and a 3-letter name; the second chunk has no land data
    m = A.atlas_gen4.parse_matrix4(bytes([2, 1, 1, 1, 3]) + b"abc" + struct.pack("<2H", 10, 11) + bytes(2)
                                   + struct.pack("<2H", 5, 0xFFFF))
    assert (m["w"], m["chunks"], m["zones"]) == (2, [5, 0xFFFFFFFF], [10, 11])
    m = A.atlas_gen4.parse_matrix4(bytes([1, 1, 0, 0, 0]) + struct.pack("<H", 7))
    assert (m["chunks"], m["zones"]) == ([7], None)
    land = bytes(0x14) + b"".join(struct.pack("<H", 0x8000 if i % 2 else 0) for i in range(1024))
    w = A.atlas_gen4.chunk_walk4(land, 0x14)
    assert w[0] and not w[1] and len(w) == 1024
    assert A.atlas_gen4.chunk_walk4(land[:0x100], 0x14) is None


def test_gen4_header_table():
    # junk, then three Platinum headers (matrix, scripts, init, events within the archives' counts), then junk
    rec = lambda mat, ev: struct.pack("<BB11H", 0, 0, mat, 1, 2, 3, 0, 0, 0, ev, 0, 0, 0)  # noqa: E731
    image = b"\xff" * 0x30 + rec(0, 0) + rec(1, 2) + rec(0, 1) + b"\xff" * 0x30
    assert len(rec(0, 0)) == 0x18
    assert A.atlas_gen4.header_table(image, "pt", 2, 3, 4) == (0x30, 3)
    h = A.atlas_gen4.parse_header4(rec(1, 2), "pt", 1)
    assert (h["matrix"], h["script"], h["init"], h["text"], h["entities"]) == (1, 1, 2, 3, 2)


def test_gen4_rom():
    """Platinum: Twinleaf (411) -> Route 201 (342) -> Sandgem (418); HeartGold: New Bark (60) -> Route 29 (33) ->
    Cherrygrove (67); route.py's G:zone expansion over the Platinum atlas (zone ids: pret's map header enums)."""
    for game, n, a, b, via in (("pt", 594, 411, 418, [342, 418]), ("hg", 540, 60, 67, [33, 67])):
        rom = "baserom_%s_us.nds" % A.atlas_gen4.GEN4[game]["rom"]
        if not (A.ROOT / "orig" / rom).exists():
            print("skip test_gen4_rom %s: no orig/%s" % (game, rom))
            continue
        data = A.load(game)
        assert len(data["zones"]) == n and data["entity_size_mismatch"] == 0, (game, len(data["zones"]))
        steps = A.path(data, a, b)
        assert [z for _, _, z in steps] == via and all(h.startswith("edge") for _, h, _ in steps), steps
        if game == "pt":
            import route
            pilot, _ = route.expand_zones(game, "G:zone:%d-%d" % (a, b), None, data)
            assert pilot == "G:warp:342 G:warp:418", pilot
            assert [w["to"] for w in data["zones"][a]["warps"]] == [412, 414, 416, 417]   # the four houses' doors


for name, fn in list(globals().items()):
    if name.startswith("test_"):
        fn()
        print("ok", name)
