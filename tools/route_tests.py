#!/usr/bin/env python3
"""Tests for the parts of tools/route.py and tools/route_port.py that need no game: python3 tools/route_tests.py   (prints "ok" per test, exits 1 on a failure)"""
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
            assert set(leg["flags"]) <= {"-", "encounters", "noskip", "realtime", "quick", "defaultopts", "expect-hang"}, (game, leg["name"])


LEG = {"name": "t", "start": "NEWGAME", "pilot": "W1", "end": "zone 5 1,2", "save": "-", "flags": []}
DONE_LOG = "pilot: script done at V-blank 7\npos: black Z zone 5 1,2\n"


def run_fake(leg, log_text, dump=False, map_kept=False, tag="", shot=0):
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

    args = argparse.Namespace(chain=False, skip=8, cap=60, hang=20, fast_opts=True, cheat=True, gpu=False, map=map_kept, shot=shot)
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


def run_cmds(leg, runs, gdb_flag=False):
    """R.run_leg with port_run faked: runs[i] = (the game's log, port_run's verdict.txt) for its i-th launch; (the leg's
    verdict, where, the commands it was launched with)"""
    import argparse
    import subprocess
    real, cmds = subprocess.run, []

    def fake(cmd, env=None, **kw):
        if cmd[0] == "cp":
            return real(cmd, **kw)
        text, pr = runs[len(cmds)]
        cmds.append(cmd)
        out = Path(cmd[cmd.index("--dir") + 1]) / "out"
        out.mkdir(parents=True, exist_ok=True)
        (out / "log.txt").write_text(text)
        (out / "verdict.txt").write_text(pr + "\n")

    args = argparse.Namespace(chain=False, skip=8, cap=60, hang=20, fast_opts=True, cheat=True, gpu=False, map=False, shot=0,
                              gdb=gdb_flag)
    with tempfile.TemporaryDirectory() as d:
        base = Path(d) / "base"
        base.mkdir()
        saved = (R.WORK, R.base_root, subprocess.run)
        R.WORK, R.base_root, subprocess.run = Path(d) / "work", lambda g: base, fake
        try:
            v, where, _ = R.run_leg("b", leg, args, {})
            return v, where, cmds
        finally:
            R.WORK, R.base_root, subprocess.run = saved


def test_legs_run_without_gdb_and_a_crash_or_hang_runs_again_under_gdb():
    if not R.LINUX:
        return
    crash = "crash SIGSEGV in f (x.c:3), frame 9"
    v, where, cmds = run_cmds(LEG, [(DONE_LOG, "reached-frame 5 in 1 s (5 fps)")])
    assert v == "ok" and len(cmds) == 1 and "--no-gdb" in cmds[0]
    v, where, cmds = run_cmds(LEG, [("pos: x\n", "crash SIGSEGV in ?, frame 9"), ("pos: x\n", crash)])   # the stop says where
    assert v == "crash" and f"({crash})" in where and len(cmds) == 2 and "--no-gdb" in cmds[0] and "--no-gdb" not in cmds[1]
    v, where, cmds = run_cmds(LEG, [("pos: x\n", "hang in ?, frame 9 (no frame for 20 s)")] * 2)   # no pilot hang line: again
    assert v == "hang" and len(cmds) == 2 and "--no-gdb" not in cmds[1]
    v, where, cmds = run_cmds(LEG, [("pilot: hang: no progress at V-blank 5\n", "hang in ?, frame 9")])   # the pilot said where
    assert v == "hang" and len(cmds) == 1
    v, where, cmds = run_cmds(LEG, [("pos: x\n", "crash SIGABRT in ?, frame 9"), (DONE_LOG, "reached-frame 5 in 1 s (5 fps)")])
    assert v == "crash" and "not reproduced under gdb: ok" in where and len(cmds) == 2
    v, where, cmds = run_cmds(LEG, [("pos: x\n", crash)], gdb_flag=True)   # --gdb: gdb every time, no second run
    assert v == "crash" and len(cmds) == 1 and "--no-gdb" not in cmds[0]


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


def test_shot_list_has_a_cfgscript_step_per_shot():
    assert "f1996:ff=0;f2000:shot=route;f2004:ff=1" in run_fake(LEG, DONE_LOG, shot=2000)[2]["BWPORT_CFGSCRIPT"]   # one: route.png
    two = run_fake(LEG, DONE_LOG, shot="2400,2200")[2]["BWPORT_CFGSCRIPT"]   # a list, in order: routeN.png each
    assert "f2196:ff=0;f2200:shot=route2200;f2204:ff=1;f2396:ff=0;f2400:shot=route2400;f2404:ff=1" in two


def test_look_leg_with_a_dump():
    look = dict(LEG, end=R.LOOK)
    assert R.judge(look, DONE_LOG, "", "zone 5 1,2", False, True) == ("look", "")
    assert R.judge(look, DONE_LOG, "", "zone 5 1,2", False, False) == ("off-route", "want zone 0 0,0")
    assert R.judge(LEG, DONE_LOG, "", "zone 5 9,9", False, True)[0] == "off-route"
    assert R.judge(look, "pilot: hang: G:warp:3 at V-blank 5\n", "", "zone 5 1,2", False, True)[0] == "hang"
    v, where, _, kept = run_fake(look, DONE_LOG, dump=True)
    assert v == "look" and kept and "map=" in where


def test_open_files_guard():
    """the exe's `openfiles:` lines (port/src/run_frames.c): a leak line makes an otherwise good leg a failure, the final
    line gives the leg's peak, port_run's verdict for the checker's stop is a crash"""
    ok = DONE_LOG + "openfiles: now 118 peak 121\n"
    leak = "openfiles: leak: 300 host files open at frame 4096 (peak 300, bound 256): x\n" + ok
    assert R.judge(LEG, ok, "", "zone 5 1,2", False, False) == ("ok", "")
    v, note = R.judge(LEG, leak, "", "zone 5 1,2", False, False)
    assert v == "leak" and "300 host files" in note and v not in R.PASS
    assert R.judge(LEG, leak, "openfiles: leak: 300 host files open", "zone 5 1,2", False, False)[0] == "crash"
    assert R.OPENFILES.search(ok).group(2) == "121" and R.OPENFILES.search(DONE_LOG) is None


def fake_atlas():
    """zones 0 -> 1 -> 2 by warps, 2 -> 3 by a matrix edge; 4 is not connected"""
    w = lambda to: [{"i": 0, "to": to, "to_warp": 0, "x": 1, "z": 1}]   # noqa: E731
    return {"zones": [{"warps": w(1)}, {"warps": w(2)}, {"warps": []}, {"warps": []}, {"warps": []}],
            "edges": {"2>3": [{"dir": "up", "x": 5, "z": [1, 3], "open": 3}]}}


def test_expand_repeats():   # (TOKENS)*N in a leg's pilot field (read_legs)
    assert R.expand_repeats("K0:5 (Ka:6 T:1,2:3)*2 Wf") == "K0:5 Ka:6 T:1,2:3 Ka:6 T:1,2:3 Wf"
    assert R.expand_repeats("Pa G:warp:62 Wf") == "Pa G:warp:62 Wf"


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
    data["zones"][3]["warps"] = []   # scripted doors: 3 -> 4 by talking to actor 7, 4 -> 0 by trigger 2
    data["scripted"] = [{"from": 3, "to": 4, "tok": "G:npc:7", "how": "actor uid 7"},
                        {"from": 4, "to": 0, "tok": "G:trig:2", "how": "trigger 2"}]
    pilot, notes = R.expand_zones("b", "G:zone:2-0 C", start, data)
    assert pilot == "G:warp:3 G:npc:7 G:trig:2 Wf C" and "script G:npc:7: actor uid 7" in notes[0], (pilot, notes)
    del data["scripted"]
    for bad in ("G:zone:3-0", "G:zone:0-4"):
        try:
            R.expand_zones("b", bad, start, data)
        except ValueError as e:
            assert "no atlas path" in str(e)
        else:
            raise AssertionError(bad)


def test_expand_zones_skips_the_other_versions_warps():
    """Reversal Mountain: the entity file has a warp into each version's interior on the same door tile; the atlas marks the
    one this game never takes dead (story.version_dead_warps), and the path and the planner's warp hints go by the other"""
    data = fake_atlas()
    data["zones"][0]["warps"] = [{"i": 0, "to": 3, "to_warp": 0, "x": 1, "z": 1, "dead": True},   # the other version's interior
                                 {"i": 1, "to": 1, "to_warp": 0, "x": 1, "z": 1}]
    data["zones"][3]["warps"] = [{"i": 0, "to": 4, "to_warp": 0, "x": 2, "z": 2}]
    assert R.expand_zones("w2", "G:zone:0-2", None, data)[0] == "G:warp:1 G:warp:2"
    assert R.expand_zones("w2", "G:zone:0-4", None, data)[0] == "G:warp:1 G:warp:2 G:warp:3 G:warp:4"   # the long way round
    del data["zones"][0]["warps"][0]["dead"]   # the same warp live (the other game): the short way
    assert R.expand_zones("b2", "G:zone:0-4", None, data)[0] == "G:warp:3 G:warp:4"
    data["zones"][0]["warps"][0]["dead"] = True
    assert R.warp_hints("w2", "G:warp:3", data) == ""   # no hint toward the door of the other version's interior
    assert R.warp_hints("w2", "G:warp:1", data) == "1=0:1,1"


def test_expand_zones_adds_the_known_waypoints():
    data = fake_atlas()
    R.WAYPOINTS[("w2", 1, 2)] = "G:7,52 G:40,57"
    try:
        assert R.expand_zones("w2", "G:zone:0-2", None, data)[0] == "G:warp:1 G:7,52 G:40,57 G:warp:2"   # the hop 1 -> 2 only
        assert R.expand_zones("b2", "G:zone:0-2", None, data)[0] == "G:warp:1 G:warp:2"   # other game: none
    finally:
        del R.WAYPOINTS[("w2", 1, 2)]
    assert R.WAYPOINTS[("w2", 539, 542)] == "G:7,52 G:40,57"   # White 2 Reversal Mountain (the proven detour)
    assert ("b2", 233, 234) not in R.WAYPOINTS   # the frozen crater: the planner reads the ROM terrain now (pz19), no waypoints


def test_f_tokens_are_documented():
    """every field-move word the pilot parses (route_fieldmove.h's strcmp(word, "...")) has a header line there and a mention in
    docs/route-writer.md: F:waterfall (11-13) was a gap the Victory Road writers would have hit"""
    src = (R.ROOT / "port/src/route_fieldmove.h").read_text()
    words = set(re.findall(r'strcmp\(word, "(\w+)"\)', src)) | set(re.findall(r'strncmp\(tok, "F:(\w+):"', src))
    assert {"cut", "smash", "strength", "push", "stone", "surf", "waterfall", "fly", "teach"} <= words, words
    doc = (R.ROOT / "docs/route-writer.md").read_text()
    head = src.split("static int FmDir")[0]
    for w in words:
        assert "F:" + w in head, "F:%s has no line in the header of route_fieldmove.h" % w
        assert "F:" + w in doc, "F:%s is not in docs/route-writer.md" % w


def test_hg_route_patch_hooks_are_defined():
    """every PortRoute4_ function pokeheartgold-route.patch's added lines call is defined in route_gen4.c's HEARTGOLD half, with
    the patch's argument count (hgfly1: F:fly's party_menu.c / fly_map.c hooks; a gap is a link error only the HG build shows)"""
    patch = (R.ROOT / "port/subprojects/pokeheartgold-route.patch").read_text()
    calls = {}
    for name, args in re.findall(r'^\+.*?\b(PortRoute4_\w+)\(([^;{]*?)\)\s*[;{)]', patch, re.M):
        calls.setdefault(name, set()).add(0 if args.strip() in ("", "void") else args.count(",") + 1)
    src = (R.ROOT / "port/game/route_gen4.c").read_text()
    hg = src[src.index("#else   // HEARTGOLD"):]
    defs = {n: 0 if a.strip() in ("", "void") else a.count(",") + 1
            for n, a in re.findall(r'^\w[\w *]*?\b(PortRoute4_\w+)\(([^)]*)\)\s*\{', hg, re.M)}
    assert {"PortRoute4_PartyMenu", "PortRoute4_FlyMap", "PortRoute4_TownMapEnd"} <= set(calls), sorted(calls)
    for name, counts in calls.items():
        assert name in defs, "%s: called by the patch, not defined in route_gen4.c's HEARTGOLD half" % name
        assert counts == {defs[name]}, (name, counts, defs[name])


def test_pk_token_parse():
    """route_test.c's PkTokenMode, compiled on its own: Pk and Pk:pw (White 2's frigate password, pz12 11-13) are the naming-screen
    tokens, nothing else that starts with Pk is (skipped without a C compiler)"""
    import shutil
    import subprocess
    cc = shutil.which("cc") or shutil.which("gcc")
    if cc is None:
        return
    src = (R.ROOT / "port/src/route_test.c").read_text()
    m = re.search(r"static int PkTokenMode\(const char \*tok\) \{.*?\n\}\n", src, re.S)
    assert m, "PkTokenMode is gone from port/src/route_test.c"
    cases = {"Pk": 1, "Pk Wf:60": 1, "Pk:pw": 2, "Pk:pw Wf:60": 2, "Pk:pwx": 0, "Pk:p": 0, "Pkx": 0, "Pa": 0, "Py Pk": 0}
    body = "".join('    if (PkTokenMode("%s") != %d) { puts("%s"); bad = 1; }\n' % (t, v, t) for t, v in cases.items())
    prog = "#include <stdio.h>\n#include <string.h>\n" + m.group(0) + "int main(void) {\n    int bad = 0;\n" + body + "    return bad;\n}\n"
    with tempfile.TemporaryDirectory() as d:
        c, exe = os.path.join(d, "pk.c"), os.path.join(d, "pk")
        Path(c).write_text(prog)
        subprocess.run([cc, "-o", exe, c], check=True)
        r = subprocess.run([exe], capture_output=True, text=True)
        assert r.returncode == 0, "PkTokenMode is wrong for: " + r.stdout.replace("\n", " ")


def test_warp_hints():
    data = fake_atlas()
    data["zones"][3]["warps"] = [{"i": 0, "to": 2, "to_warp": 0, "x": 30, "z": 40}, {"i": 1, "to": 2, "to_warp": 0, "rail": [1, 2, 0]}]
    assert R.warp_hints("b", "Pa G:5,6 W5", data) == ""   # no G:warp: the atlas is not even loaded
    assert R.warp_hints("b", "G:warp:2 G:warp:xx G:warp:20", data) == "2=1:1,1;2=3:30,40"   # a rail warp has no tile; 20 is not 2
    assert R.warp_hints("b", "G:warp:3", data) == ""   # no warp into 3 (a matrix edge)


def test_save_waits():
    def leg(name, start, save="-"):
        return ("b2", dict(LEG, name=name, start=start, save=save))
    jobs = [leg("new", "NEWGAME"), leg("make", "NEWGAME", "made"), leg("use", "made"), leg("lib", "stored"),
            leg("early", "late"), leg("late", "NEWGAME", "late")]
    # "use" waits for "make"; "lib" starts from the library; "early" would wait for a leg after it: no wait (no deadlock)
    assert R.save_waits(jobs, {"stored"}) == {("b2", "use"): "made"}
    assert R.save_waits(jobs, {"stored", "made"}) == {}   # the library has it: the library's save, as before


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


# ---- route_port.py: the version port (b -> w, b2 -> w2) ----
import argparse  # noqa: E402
import re  # noqa: E402
import tempfile  # noqa: E402
import route_port as P  # noqa: E402


def test_port_save_names_and_readme_text():
    """b-X and black-X -> w-X, b2-X -> w2-X, other names get the prefix; a README text changes the game name, the save names and `route`"""
    assert P.map_save("b2-v7", "b2", "w2") == "w2-v7"
    assert P.map_save("b-gym1a", "b", "w") == "w-gym1a" and P.map_save("black-accum", "b", "w") == "w-accum"
    assert P.map_save("caught", "b", "w") == "w-caught" and P.map_save("-", "b", "w") == "-"
    assert P.map_save("w2-v7", "w2", "b2") == "b2-v7" and P.map_save("b-x", "b2", "w2") == "w2-b-x"   # only SRC's own prefix is a prefix
    t = "Black 2: Pokestar lobby after the film (route b2 pokestar-movie from b2-v7, rw7; Black too)"
    assert P.swap_game(t, "b2", "w2") == "White 2: Pokestar lobby after the film (route w2 pokestar-movie from w2-v7, rw7; Black too)"
    assert P.swap_game("Black: Route 1 (route b catch from b-route1, Black 2 is another game)", "b", "w") == \
        "White: Route 1 (route w catch from w-route1, Black 2 is another game)"


def test_port_dst_leg_chain_and_line():
    leg = {"name": "gym-door", "start": "b-striaton-bros", "pilot": "Pa W1", "end": "zone 7 12,43", "save": "b-gym1a", "flags": ["quick", "noskip"]}
    d = P.dst_leg(leg, "b", "w", {"b-striaton-bros": "w-striaton-bros"})
    assert d == dict(leg, start="w-striaton-bros", save="w-gym1a", flags=["noskip"])
    assert P.line_of(d) == "gym-door | w-striaton-bros | Pa W1 | zone 7 12,43 | w-gym1a | noskip"
    assert P.dst_leg(leg, "b", "w", {"b-other": "w-other"}) is None   # a start outside the chain
    assert P.line_of(dict(d, flags=[])) == "gym-door | w-striaton-bros | Pa W1 | zone 7 12,43 | w-gym1a"
    assert P.line_of(dict(d, flags=[], save="-")) == "gym-door | w-striaton-bros | Pa W1 | zone 7 12,43"
    assert P.line_of(dict(d, flags=["quick"], save="-")) == "gym-door | w-striaton-bros | Pa W1 | zone 7 12,43 | - | quick"


PORT_DUMP = """story (B2 zone 568; the snapshot at V-blank 6 in zone 568)
changed since the snapshot (3):
  flag 0x02bd = 1 (was 0; V-blank 3389, zone 568)
  flag 0x0988 = 0 (was 1; V-blank 4265, zone 568)
  work 0x40ab = 4 (was 3; V-blank 3389, zone 568)
temp flags set now: none
"""


def test_port_story_changes_and_zone():
    a = P.story_changes(PORT_DUMP)
    assert a == {("flag", "0x02bd"): 1, ("flag", "0x0988"): 0, ("work", "0x40ab"): 4}
    assert P.story_diff(a, P.story_changes(PORT_DUMP.replace("V-blank 3389", "V-blank 77"))) == ""   # the V-blank is not compared
    assert P.story_diff(a, {k: v for k, v in a.items() if k[1] != "0x02bd"}) == "flag 0x02bd: SRC 1, DST -"
    assert P.story_diff(a, {**a, ("work", "0x40ab"): 5}) == "work 0x40ab: SRC 4, DST 5"
    assert P.same_zone("zone 568 14,18", "zone 568 14,12") and not P.same_zone("zone 568 14,18", "zone 448 14,18")
    assert not P.same_zone(None, "zone 568 14,12") and not P.same_zone(None, None)


class PortWorld:
    """a fake route.run_leg and zone_leg over a temp saves library, work dir and routes dir. runs: (game, tag) or game ->
    (verdict, end position, V-blank, save bytes hash); a run that ends off the leg's end is off-route; map dumps are written"""

    def __init__(self, d, runs, dumps):
        self.d, self.runs, self.dumps, self.log = Path(d), runs, dumps, []
        self.saved = {"SAVES": R.SAVES, "WORK": R.WORK, "run_leg": R.run_leg, "zone_leg": R.zone_leg, "ROUTES": P.ROUTES}
        for sub in ("saves", "work", "routes"):
            (self.d / sub).mkdir()
        (self.d / "routes/w2.txt").write_text("# the file\n")
        (self.d / "saves/README").write_text("b2-v8           Black 2: lobby   (route b2 pokestar-movie from b2-v7, rw7)\n")
        R.SAVES, R.WORK, P.ROUTES = self.d / "saves", self.d / "work", self.d / "routes"
        R.run_leg, R.zone_leg = self.run_leg, lambda game, leg, args, produced: leg

    def restore(self):
        for k, v in self.saved.items():
            setattr(P if k == "ROUTES" else R, k, v)

    def run_leg(self, game, leg, args, produced, st=None, tag=""):
        verdict, pos, v, save = self.runs.get((game, tag), self.runs[game])
        self.log.append((game, tag, leg["end"]))
        mapdir = R.WORK / f"{game}-{leg['name']}-map"
        mapdir.mkdir(exist_ok=True)
        (mapdir / "mapdump-01-end.txt").write_text(self.dumps.get(game, ""))
        if st is not None:
            st.update(pos=pos, v=v, save=save, secs=1.0)
        if verdict == "ok" and pos != leg["end"]:
            verdict = "off-route"
        out = None
        if verdict == "ok" and leg["save"] != "-":
            out = R.WORK / f"{game}-{leg['name']}{tag}.save.bin"
            out.write_bytes(b"save")
        return verdict, f"{pos} 1s", out


PORT_SAME = ("ok", "zone 568 14,12", 4816, "aa")
PORT_LEG = {"name": "pokestar-movie", "start": "b2-v7", "pilot": "Pa G:npc:0 Wf S W30", "end": "zone 568 14,12", "save": "b2-v8", "flags": []}


def port(runs, dumps=None, before=None):
    """port_one over the fake game: (ok, printed lines, the routes file, the README, the world)"""
    w = PortWorld(tempfile.mkdtemp(), runs, dumps or {})
    try:
        if before:
            before(w)
        ok, lines = P.port_one(argparse.Namespace(src="b2", dst="w2", repeat=2), argparse.Namespace(map=False), PORT_LEG, P.dst_leg(PORT_LEG, "b2", "w2", {"b2-v7": "w2-v7"}))
        return ok, lines, (w.d / "routes/w2.txt").read_text(), (w.d / "saves/README").read_text(), w
    finally:
        w.restore()


def test_port_ok_is_ported_and_stored():
    ok, lines, routes, readme, w = port({"b2": PORT_SAME, "w2": PORT_SAME})
    assert ok and len(lines) == 1 and lines[0].startswith("w2 pokestar-movie: ported zone 568 14,12 (repeat SAME) stored w2-v8"), lines
    assert routes.endswith("pokestar-movie | w2-v7 | Pa G:npc:0 Wf S W30 | zone 568 14,12 | w2-v8\n") and "# " not in routes.split("\n", 1)[1]
    assert (w.d / "saves/w2-v8.bin").read_bytes() == b"save"
    assert re.search(r"^w2-v8 +White 2: lobby   \(route w2 pokestar-movie from w2-v7, rw7\)   \[ported from b2-v8", readme, re.M) and readme.count("\n") == 2
    assert [x[1] for x in w.log] == ["", "-r1", "-r2"]   # the run and two repeats, no SRC run


def test_port_learns_an_end_in_the_same_zone_with_the_same_story():
    ok, lines, routes, readme, w = port({"b2": PORT_SAME, "w2": ("ok", "zone 568 14,18", 4816, "aa")}, {"b2": PORT_DUMP, "w2": PORT_DUMP})
    assert ok and "learned zone 568 14,18" in lines[0], lines
    assert "# White 2: ends at zone 568 14,18, Black 2 at zone 568 14,12" in routes
    assert routes.rstrip().endswith("| zone 568 14,18 | w2-v8")
    assert [x[0] for x in w.log] == ["w2", "b2", "w2", "w2", "w2"]   # DST, SRC for the story, DST with the end taken, two repeats


def test_port_stops():
    ok, lines, routes, readme, w = port({"b2": PORT_SAME, "w2": ("ok", "zone 448 14,18", 4816, "aa")})   # another zone
    assert not ok and "another zone" in lines[0] and "map dump:" in lines[2] and routes == "# the file\n" and "w2-v8" not in readme
    assert "line (not written): pokestar-movie | w2-v7 | " in lines[-1] and not (w.d / "saves/w2-v8.bin").exists()
    # the same zone, other story changes: the difference is in the report
    ok, lines, routes, readme, w = port({"b2": PORT_SAME, "w2": ("ok", "zone 568 14,18", 4816, "aa")}, {"b2": PORT_DUMP, "w2": PORT_DUMP.replace("= 4 (", "= 5 (")})
    assert not ok and "other story changes" in lines[0] and "story changes differ: work 0x40ab: SRC 4, DST 5" in lines[2] and routes == "# the file\n"
    # a crash, a hang, a stuck or timed-out leg
    for verdict in ("crash", "hang", "stuck", "timeout"):
        ok, lines, *_ = port({"b2": PORT_SAME, "w2": (verdict, "zone 568 14,12", 1, "aa")})
        assert not ok and f"STOP {verdict}" in lines[0], lines
    # SRC's own leg failing: no story to compare
    ok, lines, *_ = port({"b2": ("crash", "zone 1 1,1", 1, "aa"), "w2": ("ok", "zone 568 14,18", 4816, "aa")}, {"b2": PORT_DUMP, "w2": PORT_DUMP})
    assert not ok and "the b2 leg is crash itself" in lines[0]


def test_port_repeats():
    # a repeat that ends elsewhere stops; one that differs in the end frame or the save bytes only is accepted with a comment line
    ok, lines, routes, *_ = port({"b2": PORT_SAME, "w2": PORT_SAME, ("w2", "-r2"): ("ok", "zone 568 14,13", 4816, "aa")})
    assert not ok and "STOP the repeats differ" in lines[0] and "pos zone 568 14,12 / zone 568 14,12 / zone 568 14,13" in lines[0] and routes == "# the file\n"
    ok, lines, routes, *_ = port({"b2": PORT_SAME, "w2": PORT_SAME, ("w2", "-r1"): ("ok", "zone 568 14,12", 4830, "bb")})
    assert ok and "(repeat differs)" in lines[0] and "# White 2: --repeat differs in v, save" in routes


def test_port_save_exists_is_not_overwritten():
    ok, lines, routes, readme, w = port({"b2": PORT_SAME, "w2": PORT_SAME}, before=lambda w: (w.d / "saves/w2-v8.bin").write_bytes(b"mine"))
    assert not ok and "not stored" in lines[0] and (w.d / "saves/w2-v8.bin").read_bytes() == b"mine" and routes == "# the file\n"
def test_save_state_decode():
    """route_state on stored saves (skipped when the library lacks them): w-route1 = 1 Pokemon + 5 balls, w-caught = 2 Pokemon,
    b-badge3 = 3 badges, money grows b-cas1 -> b-cas6; the saves' species / levels / badge counts are sane"""
    import route_state
    def have(n):
        return (R.SAVES / (n + ".bin")).exists()
    if not (have("w-route1") and have("w-caught") and have("b-badge3") and have("b-cas1") and have("b-cas6")):
        return
    a, b = route_state.load("w", "w-route1"), route_state.load("w", "w-caught")
    assert len(a.party) == 1 and len(b.party) == 2 and a.bag["items"] == {4: 5}, (a.party, b.party, a.bag)
    assert route_state.load("b", "b-badge3").badges == 3
    assert route_state.load("b", "b-cas6").money > route_state.load("b", "b-cas1").money
    d = "\n".join(route_state.diff_lines("w", "w-route1", a, "w-caught", b))
    assert "party: 1:" in d and "bag items" in d, d
    assert len(route_state.state_lines("w", "w-route1", a)) <= 12
    moves = a.party[0][5]   # the starter's moves: (move, PP) x4, the first one known, every number a move's (1..559)
    assert moves[0][0] and moves[0][1] and all(0 <= m < 560 and pp < 64 for m, pp in moves), moves


def test_story_names_platinum():
    """route_story_names.read_pt (skipped without the pokeplatinum submodule): named ids from pret's generated/vars_flags.txt:
    an alias keeps the first real name, placeholders are left out, vars start at 0x4000, script vars at 0x8000"""
    import route_story_names as N
    if not (N.ROOT / "third_party/pokeplatinum/generated/vars_flags.txt").exists():
        return
    pt = N.read_pt()
    assert pt[0x4106] == "VAR_LUNAR_WING_EVENT_STATE" and pt[0x800C] == "VAR_RESULT" and pt[0x09B0] == "FLAG_FIRST_ARRIVAL_TWINLEAF_TOWN"
    assert not any(N.PLACEHOLDER.search(n) for n in pt.values()) and max(i for i in pt if i < 0x4000) < 4096


def gen4_pkm(species, level, moves, hp, item=0, pid=0x12345678):
    """a stored Gen 4 party Pokemon (0xEC bytes), the inverse of route_state.decode_pkm"""
    import route_state as S
    buf = bytearray(0xEC)
    struct.pack_into("<IHH", buf, 0, pid, 0, 0xBEEF)
    a, b = (8 + 0x20 * S.ORDERS[(pid >> 13 & 31) % 24].index(i) for i in (0, 1))
    struct.pack_into("<2H", buf, a, species, item)
    struct.pack_into("<4H", buf, b, *[m for m, _ in moves])
    struct.pack_into("<4B", buf, b + 8, *[pp for _, pp in moves])
    struct.pack_into("<BBHH", buf, 0x8C, level, 0, hp, hp)
    S._crypt(buf, 8, 0x88, 0xBEEF)
    S._crypt(buf, 0x88, 0xEC, pid)
    return bytes(buf)


def gen4_save(counter, money, badges, party, items, tmhm, var, flags, zone, tile, slot=0):
    """a Platinum save (two 0x40000 slots) whose slot `slot` holds the data, footers of save counter `counter` (other slot: counter-1)"""
    d = bytearray(0x80000)
    for s, n in ((0, 0), (0x40000, 1)):   # both slots get a valid footer; slot `slot` the data and the larger counter
        c = counter if s == slot else counter - 1
        struct.pack_into("<4IB", d, s + 0xCF18, c, c, 0xCF2C, 0x20060623, 0)
    o = slot
    struct.pack_into("<4i", d, o + 0x1280, zone, 0, tile[0], tile[1])
    struct.pack_into("<I", d, o + 0x7C, money)
    d[o + 0x82] = badges
    struct.pack_into("<ii", d, o + 0x98, 6, len(party))
    for i, p in enumerate(party):
        d[o + 0xA0 + 0xEC * i: o + 0xA0 + 0xEC * (i + 1)] = p
    for off, pocket in ((0x630, items), (0x630 + 860, tmhm)):
        for i, (item, n) in enumerate(pocket):
            struct.pack_into("<2H", d, o + off + 4 * i, item, n)
    struct.pack_into("<H", d, o + 0xDAC + 2 * (var[0] - 0x4000), var[1])
    for f in flags:
        d[o + 0xDAC + 576 + f // 8] |= 1 << (f % 8)
    return bytes(d)


def test_save_state_gen4_platinum_fixture():
    """a Platinum-layout fixture save (pret's save table offsets): the newer slot wins, money, badges, party with moves and PP,
    bag pockets, HMs, vars from 0x4000, flags, the player's zone and tile; a Gen 5 save is still read at its own offsets"""
    import route_state as S
    mon = gen4_pkm(393, 5, ((33, 35), (45, 40), (0, 0), (0, 0)), 20, item=4)
    new = gen4_save(5, 3000, 0b10000101, [mon, gen4_pkm(399, 7, ((33, 30),) + ((0, 0),) * 3, 25)], [(4, 5), (17, 2)],
                    [(420, 1), (432, 1)], (0x4001, 7), [0x0A, 2911], 71, (200, 313), slot=0x40000)
    old = gen4_save(4, 100, 0, [mon], [(4, 1)], [], (0x4001, 1), [], 9, (1, 2), slot=0)
    s = S.Save("pt", new)
    assert (s.zone, s.tile, s.money, s.badges, s.badge_bits) == (71, (200, 313), 3000, 3, 0b10000101)
    assert [p[:5] for p in s.party] == [(393, 5, 20, 20, 4), (399, 7, 25, 25, 0)] and s.party[0][5][:2] == ((33, 35), (45, 40))
    assert s.bag["items"] == {4: 5, 17: 2} and s.bag["tmhm"] == {420: 1, 432: 1} and not s.bag["balls"]
    assert s.works == {0x4001: 7} and s.flags == {0x0A, 2911}
    # one fixture carries both slots: the older slot with a valid footer loses, an invalid footer is skipped
    both = bytearray(new)
    both[0:0xCF18] = old[0:0xCF18]
    both[0xCF18:0xCF2C] = old[0xCF18:0xCF2C]
    assert S.Save("pt", bytes(both)).money == 3000
    broken = bytearray(new)   # the newer slot's footer size is wrong: that slot does not count, the older one is read
    struct.pack_into("<I", broken, 0x40000 + 0xCF18 + 8, 1)
    broken[0:0xCF2C] = old[0:0xCF2C]
    assert S.Save("pt", bytes(broken)).money == 100
    try:
        S.Save("pt", bytes(0x80000))
        raise AssertionError("an empty save is no save")
    except ValueError:
        pass
    text = "\n".join(S.state_lines("pt", "x", s))
    assert "badges 3" in text and "money 3000" in text and "HMs held: 420" in text and "TMs held: 1" in text, text
    assert "#393 moves (move:PP): 33:35 45:40" in text, text
    d = "\n".join(S.diff_lines("pt", "a", S.Save("pt", old), "b", s))
    assert "money: 100 -> 3000" in d and "place:" in d and "party:" in d, d


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
