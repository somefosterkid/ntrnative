#!/usr/bin/env python3
"""Tests for tools/route_add.py (route.py GAME add), no game needed: python3 tools/route_add_tests.py   (prints "ok" per test,
exits 1 on a failure)"""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import route_add as A  # noqa: E402

SPEC = "gym-out | b-badge3 | Pa G:warp:31 Wf S W30 | ? | b-r4-0"
DUMP = """map: B zone 31 player 17,62
story (B zone 31; the snapshot at V-blank 6 in zone 31)
changed since the snapshot (4):
  flag 0x0280 = 0 (was 1; V-blank 400, zone 31)
  flag 0x0320 = 1 (was 0; V-blank 400, zone 31)
  work 0x40af = 3 (was 2; V-blank 410, zone 31)
  flag 0x0101 = 1 (was 0; V-blank 420, zone 31) FLAG_SOMETHING
temp flags set now: none
"""


def test_parse_spec():
    s = A.parse_spec(SPEC)
    assert s == dict(name="gym-out", start="b-badge3", tokens="Pa G:warp:31 Wf S W30", end="?", save="b-r4-0", flags="")
    assert A.parse_spec("a | NEWGAME | Pa S | zone 7 12,4 | - | encounters quick")["flags"] == "encounters quick"
    assert A.parse_spec("a | s | Pa | zone 29 -3,4 | -")["end"] == "zone 29 -3,4"
    for bad in ("a | b | c | d", "a b | s | Pa | ? | -", "a | s | | ? | -", "a | s | Pa | nowhere | -", "a | s | Pa | ? | x y", "a|b|c|?|-|f|g"):
        try:
            A.parse_spec(bad)
        except ValueError:
            continue
        raise AssertionError("accepted " + bad)


def test_parse_spec_file():
    legs = A.parse_spec_file("# one\n\ndesc: first leg\na | s | Pa | ? | x\n#two\nb | x | Pa | ? | -\n")
    assert legs == [("a | s | Pa | ? | x", ["# one"], "first leg"), ("b | x | Pa | ? | -", ["#two"], None)]


def routes(text):
    d = Path(tempfile.mkdtemp())
    (d / "tools/routes").mkdir(parents=True)
    (d / "tools/routes/b.txt").write_text(text)
    return d, d / "tools/routes/b.txt"


def test_write_leg_adds_after_the_last_leg_and_keeps_a_trailing_note():
    d, p = routes("# head\nold | s | Pa | zone 1 1,1 | x\n# NEXT: a note\n")
    spec = A.parse_spec(SPEC)
    assert A.write_leg(p, spec, ["why", "# more"]) == "added"
    assert p.read_text() == ("# head\nold | s | Pa | zone 1 1,1 | x\n# why\n# more\n"
                             "gym-out | b-badge3 | Pa G:warp:31 Wf S W30 | zone 0 0,0 | b-r4-0\n# NEXT: a note\n")


def test_write_leg_replaces_a_leg_and_its_comments_only_when_given_new_ones():
    d, p = routes("a | s | Pa | zone 1 1,1 | x\n# about b\nb | s | Pa | zone 2 2,2 | y | quick\nc | s | Pa | zone 3 3,3 | z\n")
    spec = dict(A.parse_spec("b | t | Pa W1 | zone 5 5,5 | y2"))
    assert A.write_leg(p, spec, []) == "replaced"   # comments stay
    assert p.read_text() == "a | s | Pa | zone 1 1,1 | x\n# about b\nb | t | Pa W1 | zone 5 5,5 | y2\nc | s | Pa | zone 3 3,3 | z\n"
    assert A.write_leg(p, spec, ["new"]) == "replaced"   # the old comment block goes
    assert p.read_text() == "a | s | Pa | zone 1 1,1 | x\n# new\nb | t | Pa W1 | zone 5 5,5 | y2\nc | s | Pa | zone 3 3,3 | z\n"
    A.write_leg(p, A.parse_spec("c | t | Pa | ? | -  | encounters"), [])
    assert p.read_text().splitlines()[-1] == "c | t | Pa | zone 0 0,0 | - | encounters"
    # a name that is another's prefix is not it
    A.write_leg(p, A.parse_spec("b | u | Pa | ? | -"), [])
    assert "\nb | u | Pa | zone 0 0,0 | -\n" in p.read_text() and p.read_text().count("\nb | ") == 1


def test_readme_add_once():
    d = Path(tempfile.mkdtemp())
    (d / "README").write_text("b-old   something")   # no final newline
    assert A.readme_add(d, "b-r4-0", "b-r4-0   text") is True
    assert A.readme_add(d, "b-r4-0", "b-r4-0   other") is False
    assert A.readme_add(d, "b-old", "b-old   other") is False
    assert (d / "README").read_text() == "b-old   something\nb-r4-0   text\n"
    assert A.readme_line("b-r4-0", "Black: out", A.parse_spec(SPEC), "b") == "b-r4-0".ljust(14) + " Black: out   (route b gym-out from b-badge3)"


def test_story_changes_lists_named_then_set_flags_then_work_then_cleared():
    assert A.story_changes(DUMP) == ["flag 0x0101 = 1 FLAG_SOMETHING", "flag 0x0320 = 1", "work 0x40af = 3 (was 2)", "flag 0x0280 = 0"]
    assert A.story_changes("changed since the snapshot (0):\ntemp flags set now: none\n") == []
    assert A.story_changes("") == []


OUT_OK = """b gym-out: look zone 31 17,62 4s v=1237 map=/w/b-gym-out-map/ (end learned: zone 31 17,62)
--- /w/b-gym-out-map/mapdump-02-end.txt
""" + DUMP + """
0/1 ok in 3 s; 0 game frames in 2 leg seconds (0 frames/s)
"""
OUT_REP = """b gym-out: ok zone 31 17,62 4s v=1237
--- /w/b-gym-out-map/mapdump-02-end.txt
""" + DUMP + """b gym-out: repeat x3 SAME zone 31 17,62 v=1237 save=a0cfd4145e
1/1 ok in 9 s; 3000 game frames in 12 leg seconds (250 frames/s)
"""


def test_parse_output():
    r = A.parse_output(OUT_OK, "b", "gym-out")
    assert (r["verdict"], r["learned"], r["repeat"], r["dump"]) == ("look", "zone 31 17,62", None, "/w/b-gym-out-map/mapdump-02-end.txt")
    assert "changed since the snapshot (4)" in r["dump_text"] and "ok in" not in r["dump_text"]
    r = A.parse_output(OUT_REP, "b", "gym-out")
    assert (r["verdict"], r["same"], r["learned"]) == ("ok", True, None)
    assert "temp flags" in r["dump_text"] and "repeat" not in r["dump_text"]
    r = A.parse_output(OUT_REP.replace("SAME zone 31 17,62 v=1237 save=a0cfd4145e", "DIFFERENT v 1 / 2 / 3"), "b", "gym-out")
    assert r["same"] is False and r["repeat"].endswith("v 1 / 2 / 3")
    assert A.parse_output("route: no such leg: x\n", "b", "x")["line"] is None


def test_why_prefers_the_pilots_reason_then_a_crash():
    w = Path(tempfile.mkdtemp())
    (w / "b-x.log").write_text("pilot: event off\npilot: no path: no path from 1,2 to warp:9 (G token G:warp:9)\npilot: stuck: no path at V-blank 5\n")
    assert A.why(w, "b", "x") == "pilot: no path: no path from 1,2 to warp:9 (G token G:warp:9)"
    (w / "b-y.log").write_text("pilot: event off\nCRASH at foo.c:12\npilot: stuck\n")
    assert A.why(w, "b", "y") == "CRASH at foo.c:12"
    (w / "b-z.log").write_text("pilot: event off\npilot: stuck\n")
    assert A.why(w, "b", "z") == "pilot: stuck"
    assert A.why(w, "b", "none") == ""


class Fake:
    """a route.py stand-in: scripted outputs, the argv of each call kept"""

    def __init__(self, outs):
        self.outs, self.calls = list(outs), []

    def __call__(self, argv):
        self.calls.append(argv)
        return self.outs.pop(0)


def ctx_for(d, store_msg="stored X: add its line to the README"):
    work = d / "work"
    work.mkdir()
    saves = d / "saves"
    saves.mkdir()
    stored = []

    def store(save, out, replace):
        stored.append((save, str(out), replace))
        return store_msg
    return dict(root=d, saves=saves, work=work, store=store), stored


def test_add_leg_learns_then_repeats_then_stores_and_writes_the_readme():
    d, p = routes("")
    ctx, stored = ctx_for(d)
    f = Fake([OUT_OK, OUT_REP])
    ok, out = A.add_leg("b", A.parse_spec(SPEC), ["why"], "Black: out of the gym", False, ctx, runner=f)
    assert ok and len(out) <= 8, out
    assert f.calls == [["gym-out", "--map", "--learn"], ["gym-out", "--map", "--no-build", "--repeat", "2"]]
    assert out[0].startswith("b gym-out: ok zone 31 17,62") and out[0].endswith("(added, end learned)") and "map=" not in out[0]
    assert out[1].startswith("story: 4 changed: flag 0x0101 = 1 FLAG_SOMETHING; flag 0x0320 = 1;")
    assert out[2] == "repeat x3 SAME zone 31 17,62 v=1237 save=a0cfd4145e"
    assert out[3] == "stored b-r4-0.bin + README line" and out[4].startswith("end dump: ")
    assert stored == [("b-r4-0", str(ctx["work"] / "b-gym-out.save.bin"), "")]
    assert (ctx["saves"] / "README").read_text() == "b-r4-0".ljust(14) + " Black: out of the gym   (route b gym-out from b-badge3)\n"
    assert p.read_text() == "# why\ngym-out | b-badge3 | Pa G:warp:31 Wf S W30 | zone 0 0,0 | b-r4-0\n"   # the placeholder end: the fake route.py does not learn


def test_add_leg_with_an_end_runs_once_and_does_not_learn():
    d, p = routes("")
    ctx, stored = ctx_for(d, "not stored: b-r4-0.bin exists (--replace b-r4-0 works for b2-* and w2-* only)")
    f = Fake([OUT_REP])
    ok, out = A.add_leg("b", A.parse_spec(SPEC.replace("?", "zone 31 17,62")), [], "", False, ctx, runner=f)
    assert f.calls == [["gym-out", "--map", "--repeat", "2"]]
    assert ok and out[0].endswith("(added)") and out[-2].startswith("NOT STORED: b-r4-0.bin exists"), out
    assert not (ctx["saves"] / "README").exists()


def test_add_leg_failures_say_why_and_where_the_dump_is():
    d, p = routes("")
    ctx, stored = ctx_for(d)
    (ctx["work"] / "b-gym-out.log").write_text("pilot: no path: no path from 1,2 to warp:9 (G token G:warp:9)\npilot: stuck: no path at V-blank 5\n")
    stuck = "b gym-out: stuck zone 29 32,5 2s map=/w/b-gym-out-map/\n--- /w/b-gym-out-map/mapdump-01-stuck.txt\n" + DUMP + "\n0/1 ok in 3 s\n"
    ok, out = A.add_leg("b", A.parse_spec(SPEC), [], "", False, ctx, runner=Fake([stuck]))
    assert not ok and len(out) <= 8
    assert out[0] == "b gym-out: stuck zone 29 32,5 2s (added)"
    assert out[-3].startswith("  > no path: G:warp:9: ") and out[-2].startswith("  > try: ")   # tools/route_triage.py's lines
    assert out[-1] == "FAILED (stuck): dump /w/b-gym-out-map/mapdump-01-stuck.txt"
    assert stored == []
    ok, out = A.add_leg("b", A.parse_spec(SPEC), [], "", False, ctx, runner=Fake(["route: no such leg: gym-out\n"]))
    assert not ok and out[0].startswith("no verdict from route.py: route: no such leg")


def test_add_leg_does_not_store_a_repeat_that_differs():
    d, p = routes("")
    ctx, stored = ctx_for(d)
    diff = OUT_REP.replace("SAME zone 31 17,62 v=1237 save=a0cfd4145e", "DIFFERENT save a / b / c")
    ok, out = A.add_leg("b", A.parse_spec(SPEC.replace("?", "zone 31 17,62")), [], "", False, ctx, runner=Fake([diff]))
    assert not ok and out[-1].startswith("NOT STORED") and stored == []


def test_add_leg_look_leg_and_no_save_and_replace():
    d, p = routes("")
    ctx, stored = ctx_for(d)
    look = "b lk: look zone 31 1,1 3s map=/w/b-lk-map/\n--- /w/b-lk-map/mapdump-01.txt\n" + DUMP + "\n"
    ok, out = A.add_leg("b", A.parse_spec("lk | s | Pa Md C | zone 0 0,0 | -"), [], "", False, ctx, runner=Fake([look]))
    assert ok and out[-1] == "look leg: the map dump is /w/b-lk-map/mapdump-01.txt"
    rep = OUT_REP.replace("gym-out", "ns")
    ok, out = A.add_leg("b", A.parse_spec("ns | s | Pa | zone 31 17,62 | -"), [], "", False, ctx, runner=Fake([rep]))
    assert ok and out[-1].startswith("no save (save is -)")
    ok, out = A.add_leg("b", A.parse_spec("ns | s | Pa | zone 31 17,62 | w2-x"), [], "", True, ctx, runner=Fake([rep]))
    assert stored == [("w2-x", str(ctx["work"] / "b-ns.save.bin"), "w2-x")]   # --replace passes the save's name to route.py's store


def test_final_dump_finds_the_repeat_s_copy():
    w = Path(tempfile.mkdtemp())
    (w / "b-x-r2-map").mkdir()
    (w / "b-x-r2-map/mapdump-02.txt").write_text("m")
    assert A.final_dump(w, "b", "x", str(w / "b-x-map/mapdump-02.txt")) == str(w / "b-x-r2-map/mapdump-02.txt")
    (w / "b-x-map").mkdir()
    (w / "b-x-map/mapdump-02.txt").write_text("m")
    assert A.final_dump(w, "b", "x", str(w / "b-x-map/mapdump-02.txt")) == str(w / "b-x-map/mapdump-02.txt")


def test_route_py_dispatches_add_and_rejects_a_bad_spec_before_running_anything():
    r = subprocess.run([sys.executable, os.path.join(HERE, "route.py"), "b", "add", "only | three | fields"], capture_output=True, text=True)
    assert r.returncode == 2 and "need name | start | tokens | end | save" in r.stdout, (r.stdout, r.stderr)


def test_add_honours_bwport_routes_dir():
    """BWPORT_ROUTES_DIR set: the leg goes to GAME.txt there (created when missing) and tools/routes/ stays untouched"""
    d, p = routes("")
    other = Path(tempfile.mkdtemp())
    before = p.read_text()
    old = os.environ.get("BWPORT_ROUTES_DIR")
    os.environ["BWPORT_ROUTES_DIR"] = str(other)
    try:
        ctx, _ = ctx_for(d)
        f = Fake([OUT_OK, OUT_REP])
        ok, out = A.add_leg("b", A.parse_spec(SPEC), [], "", False, ctx, runner=f)
        assert ok, out
        assert (other / "b.txt").read_text() == "gym-out | b-badge3 | Pa G:warp:31 Wf S W30 | zone 0 0,0 | b-r4-0\n"
        assert p.read_text() == before
        del os.environ["BWPORT_ROUTES_DIR"]
        assert A.routes_file(d, "b") == d / "tools/routes/b.txt"
    finally:
        if old is None:
            os.environ.pop("BWPORT_ROUTES_DIR", None)
        else:
            os.environ["BWPORT_ROUTES_DIR"] = old


def test_look_passes_full_map_to_route():
    """route.py GAME look SAVE --full-map: the flag reaches the route.py run (it was an unknown argument before)"""
    seen = []

    class R:
        stdout = ""

    def fake(cmd, **kw):
        seen.append(cmd)
        return R()

    d = tempfile.mkdtemp()
    Path(d, "s1.bin").write_bytes(b"x")
    old = A.subprocess.run
    A.subprocess.run = fake
    try:
        for extra, want in (([], False), (["--full-map"], True)):
            seen.clear()
            rc = A.look("b", ["s1", "Pa"] + extra + ["--no-build"], HERE + "/..", d, d, None)
            assert rc == 1 and len(seen) == 1 and ("--full-map" in seen[0]) == want and "--map" in seen[0], seen
    finally:
        A.subprocess.run = old


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
