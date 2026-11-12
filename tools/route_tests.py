#!/usr/bin/env python3
"""Tests for the parts of tools/route.py that need no game: python3 tools/route_tests.py   (prints "ok" per test, exits 1 on a failure)"""
import os
import struct
import sys
import tempfile
import zlib
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import route as R  # noqa: E402


def png(rows, ftype=0, bpp=4):
    """rows: lists of pixel byte-strings (bpp bytes each); every row filtered with ftype (0 none, 1 Sub, 2 Up, 3 Average, 4 Paeth)"""
    raw, prev = b"", bytes(len(rows[0]) * bpp)
    for row in rows:
        cur = b"".join(row)
        out = bytearray()
        for i, x in enumerate(cur):
            a = cur[i - bpp] if i >= bpp else 0
            b = prev[i]
            c = prev[i - bpp] if i >= bpp else 0
            p = a + b - c
            pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
            pred = [0, a, b, (a + b) >> 1, a if pa <= pb and pa <= pc else b if pb <= pc else c][ftype]
            out.append((x - pred) & 255)
        raw += bytes([ftype]) + bytes(out)
        prev = cur

    def chunk(kind, body):
        return struct.pack(">I", len(body)) + kind + body + struct.pack(">I", zlib.crc32(kind + body))
    ihdr = struct.pack(">IIBBBBB", len(rows[0]), len(rows), 8, 6, 0, 0, 0)
    return b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", ihdr) + chunk(b"IDAT", zlib.compress(raw)) + chunk(b"IEND", b"")


def image(w, h, seed):
    return [[bytes([(x * 7 + y * 13 + seed) & 255, (x * 3 + seed) & 255, (y * 5) & 255, 255]) for x in range(w)] for y in range(h)]


def test_game_hash_ignores_the_panel():
    """the picture right of `width` (the debug panel and its statistics) is not in the hash, in every PNG row filter"""
    base = image(40, 12, 1)
    other = [row[:30] + [bytes([200 + x, 9, 9, 255]) for x in range(10)] for row in base]   # same left 30 columns
    changed = [[bytes([1, 2, 3, 255])] + row[1:] for row in base]                            # the first pixel differs
    with tempfile.TemporaryDirectory() as d:
        for ftype in range(5):
            hashes = []
            for name, rows in (("a", base), ("b", other), ("c", changed)):
                p = Path(d) / f"{name}{ftype}.png"
                p.write_bytes(png(rows, ftype))
                hashes.append(R.game_hash(p, 30))
            assert hashes[0] == hashes[1], ftype
            assert hashes[0] != hashes[2], ftype
        # the filter does not change the hash, and a width past the picture's is the whole picture
        p0, p4 = Path(d) / "a0.png", Path(d) / "a4.png"
        assert R.game_hash(p0, 30) == R.game_hash(p4, 30)
        assert R.game_hash(p0, 999) != R.game_hash(p0, 30)


def test_game_hash_width_per_platform():
    """the Linux window's game half is not the Mac's 1016 pixels: the default width follows the platform"""
    assert R.GAME_WIDTH[True] != R.GAME_WIDTH[False]
    assert R.GAME_WIDTH[R.LINUX] == R.game_width()


def test_compare_runs():
    """SAME = verdict, end position, end frame and save bytes; the screen hash is info only"""
    run = {"verdict": "ok", "pos": "zone 1 2,3", "v": 500, "png": None, "save": "abc"}
    assert R.compare_runs([run, dict(run), dict(run)]) == (True, "")
    assert R.compare_runs([run, dict(run, png="h1"), dict(run, png="h2")]) == (True, "")   # animated screens: not compared
    for field, val in (("pos", "zone 1 2,4"), ("v", 501), ("save", "abd"), ("verdict", "stuck")):
        ok, why = R.compare_runs([run, dict(run), dict(run, **{field: val})])
        assert not ok and why.startswith(field + " "), (field, why)
    ok, why = R.compare_runs([run, dict(run, v=499), dict(run)])
    assert not ok and why == "v 500 / 499 / 500"
    ok, why = R.compare_runs([run, dict(run, v=499, pos="zone 1 9,9")])
    assert not ok and why.count(";") == 1
    assert R.png_info([run, run]) == "" and R.png_info([dict(run, png="a"), dict(run, png="a")]) == " png=a"
    assert R.png_info([dict(run, png="a"), dict(run, png="b")]) == " png=a/b (info only)"


def test_chain_table():
    t = R.chain_table([("a", 1000, 12.34, "ok"), ("b", None, 600.0, "hang"), ("c", 250, 3.0, "ok")])
    assert t.splitlines() == ["leg\tframes\tseconds\tresult", "a\t1000\t12.3\tok", "b\t-\t600.0\thang", "c\t250\t3.0\tok",
                              "total\t1250\t615.3\t2/3 ok"]


def test_chain_table_empty():
    assert R.chain_table([]).splitlines() == ["leg\tframes\tseconds\tresult", "total\t0\t0.0\t0/0 ok"]


def test_legs_parse():
    """every route file parses, and a leg's end is a position"""
    for game in R.GAMES:
        for leg in R.read_legs(game):
            assert leg["end"].startswith("zone "), (game, leg["name"])
            assert set(leg["flags"]) <= {"-", "encounters", "noskip", "realtime", "quick", "defaultopts"}, (game, leg["name"])


LEG = {"name": "t", "start": "NEWGAME", "pilot": "W1", "end": "zone 5 1,2", "save": "-", "flags": []}
DONE_LOG = "pilot: script done at V-blank 7\npos: black Z zone 5 1,2\n"


def run_fake(leg, log_text, dump=False, map_kept=False, tag=""):
    """R.run_leg with the game faked: (verdict, where, the env the game got, the map dir)"""
    import argparse
    import subprocess
    real, seen = subprocess.run, []

    def fake(cmd, env=None, **kw):
        if cmd[0] == "cp":
            return real(cmd, **kw)
        seen.append(dict(env))
        run = Path(cmd[cmd.index("--dir") + 1]) if R.LINUX else Path(cmd[-2])
        logf = run / "out/log.txt" if R.LINUX else run / "log.txt"
        logf.parent.mkdir(parents=True, exist_ok=True)
        logf.write_text(log_text)
        if dump:
            (Path(env["BWPORT_MAPDUMP"]) / "mapdump-01-end.txt").write_text("dump")

    args = argparse.Namespace(chain=False, skip=8, cap=60, hang=20, fast_opts=True, cheat=True, gpu=False, map=map_kept, shot=0)
    with tempfile.TemporaryDirectory() as d:
        base = Path(d) / "base"
        base.mkdir()
        saved = (R.WORK, R.base_root, subprocess.run)
        R.WORK, R.base_root, subprocess.run = Path(d) / "work", lambda g: base, fake
        try:
            v, where, _ = R.run_leg("b", leg, args, {}, tag=tag)
            mapdir = R.WORK / f"b-{leg['name']}{tag}-map"
            return v, where, seen[0], mapdir.exists()
        finally:
            R.WORK, R.base_root, subprocess.run = saved


def test_every_run_has_a_map_dump_and_keeps_it_on_failure():
    v, where, env, kept = run_fake(LEG, DONE_LOG)
    assert v == "ok" and not kept and "map=" not in where and env["BWPORT_MAPDUMP"].endswith("b-t-map")
    v, where, env, kept = run_fake(LEG, DONE_LOG, map_kept=True)   # --map keeps it for ok too
    assert v == "ok" and kept
    v, where, env, kept = run_fake(LEG, "pilot: script done at V-blank 7\npos: black Z zone 5 9,9\n")
    assert v == "off-route" and kept and "(want zone 5 1,2)" in where and "map=" in where and where.rstrip().endswith("-map/")


def test_repeat_runs_use_the_same_cfgscript_without_a_shot():
    import inspect
    assert "shot" not in inspect.signature(R.run_leg).parameters
    a = run_fake(LEG, DONE_LOG)[2]["BWPORT_CFGSCRIPT"]
    assert a == run_fake(LEG, DONE_LOG, tag="-r1")[2]["BWPORT_CFGSCRIPT"] and "shot" not in a


def test_look_leg_with_a_dump():
    look = dict(LEG, end=R.LOOK)
    assert R.judge(look, DONE_LOG, "", "zone 5 1,2", False, True) == ("look", "")
    assert R.judge(look, DONE_LOG, "", "zone 5 1,2", False, False) == ("off-route", "want zone 0 0,0")
    assert R.judge(LEG, DONE_LOG, "", "zone 5 9,9", False, True)[0] == "off-route"
    assert R.judge(look, "pilot: hang: G:warp:3 at V-blank 5\n", "", "zone 5 1,2", False, True)[0] == "hang"
    v, where, _, kept = run_fake(look, DONE_LOG, dump=True)
    assert v == "look" and kept and "map=" in where


def fake_atlas():
    """zones 0 -> 1 -> 2 by warps, 2 -> 3 by a matrix edge; 4 is not connected"""
    w = lambda to: [{"i": 0, "to": to, "to_warp": 0, "x": 1, "z": 1}]   # noqa: E731
    return {"zones": [{"warps": w(1)}, {"warps": w(2)}, {"warps": []}, {"warps": []}, {"warps": []}],
            "edges": {"2>3": [{"dir": "up", "x": 5, "z": [1, 3], "open": 3}]}}


def test_expand_zones():
    data = fake_atlas()

    def start():
        raise ValueError("no start")
    pilot, notes = R.expand_zones("b", "Pa G:zone:0-3 W5", start, data)   # explicit start: start() not called
    assert pilot == "Pa G:warp:1 G:warp:2 G:warp:3 W5" and len(notes) == 1 and "from zone 0" in notes[0] and "edge up" in notes[0]
    asked = []
    pilot, _ = R.expand_zones("b", "G:zone:2 G:zone:3", lambda: asked.append(1) or 0, data)   # a later token starts at the previous target
    assert pilot == "G:warp:1 G:warp:2 G:warp:3" and asked == [1]
    assert R.expand_zones("b", "G:warp:2 G:zone:3", start, data)[0] == "G:warp:2 G:warp:3"   # after G:warp:2 the player is in zone 2
    assert R.expand_zones("b", "G:zone:1-1", start, data)[0] == ""
    for bad in ("G:zone:3-0", "G:zone:0-4"):
        try:
            R.expand_zones("b", bad, start, data)
        except ValueError as e:
            assert "no atlas path" in str(e)
        else:
            raise AssertionError(bad)


def test_zone_leg_reads_the_start_zone_with_one_probe_run():
    if (R.ORIG / "baserom_b_us.nds").exists():   # the real atlas: Accumula Town (397) -> the gate -> Route 2 -> zone 6
        calls, real = [], R.run_leg

        def probe(game, leg, args, produced, st=None, tag=""):
            calls.append(leg)
            st["pos"] = "zone 397 780,658"
            return "look", "zone 397 780,658", None
        R.run_leg = probe
        try:
            out = R.zone_leg("b", dict(LEG, start="sv", pilot="Pa G:zone:6"), None, {})
        finally:
            R.run_leg = real
        assert out["pilot"] == "Pa G:warp:320 G:warp:319 G:warp:6" and out["zones"] and len(calls) == 1 and calls[0]["pilot"] == "W1"
    try:
        R.zone_leg("b", dict(LEG, pilot="G:zone:2"), None, {})
    except ValueError as e:
        assert "NEWGAME" in str(e)
    else:
        raise AssertionError("NEWGAME has no zone")
    plain = dict(LEG, pilot="Pa W1")
    assert R.zone_leg("b", plain, None, {}) is plain


if __name__ == "__main__":
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            try:
                fn()
                print("ok", name)
            except Exception as e:  # noqa: BLE001
                bad += 1
                print("FAIL", name, repr(e))
    sys.exit(1 if bad else 0)
