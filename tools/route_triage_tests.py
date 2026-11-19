#!/usr/bin/env python3
"""Tests for tools/route_triage.py (a failed leg in 1-3 lines), no game needed: python3 tools/route_triage_tests.py
(prints "ok" per test, exits 1 on a failure). The logs are small excerpts of real failures (no game text)."""
import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import route_triage as T  # noqa: E402

HEAD = "map: B zone {z} player {p} y 0 facing 1 (V-blank 396, stuck) matrix 2x2 chunks; tiles 0..11 x 0..5 (gen5)\n"
GRID = """        0
    0 ############
    1 ##...D....##
    2 ##........##
    3 ##.....N..##
    4 ##........##
    5 ############
"""
GATE = GRID.replace("    2 ##........##", "    2 ######N#####")   # the only way to row 1 is the actor at 6,2
LISTS = """warps (1, 1 shown):
  D0 5,1 1x1 -> zone 7 warp 0 (camera, arrive facing 1)
actors (2, 2 shown):
  N uid 8 at 6,2 (from 6,2) code 85 move 0
  N uid 3 at 7,3 (from 7,3) code 0 move 3
triggers (1, 1 shown):
  T0 3,3 1x1 script 11 (work 0x40b4 = 1, type 0)
story (B zone 6; the snapshot at V-blank 6 in zone 6)
changed since the snapshot (1):
  work 0x4001 = 2 (was 1; V-blank 80, zone 6)
"""


def run(verdict, tokens, where="", log="", dump="", game=""):
    return T.triage("b", dict(name="x", pilot=tokens), verdict, dict(where=where, log=log, game=game, dump=dump))


def test_ok_and_look_say_nothing():
    assert run("ok", "Pa") == [] and run("look", "Pa") == []


def test_crash_stub_and_a_port_hang_are_port_bugs():
    out = run("crash", "Pa", where="zone 6 1,2 (crash SIGSEGV in Foo (a.c:3) <- Bar (b.c:9), frame 40) 3s")
    assert len(out) == 2 and "port bug" in out[0] and "SIGSEGV in Foo (a.c:3)" in out[0] and "do not work around" in out[1]
    out = run("crash", "Pa", where="zone 6 1,2 (stub Foo_Bar called from x.c:5, frame 9) 2s")
    assert "stub" in out[0] and "x.c:5" in out[0]
    out = run("hang", "Pa", where="zone 30 12,8 (hang in Foo (m.c:85), frame 35243 (no frame for 20 s)) 187s")
    assert "port bug" in out[0] and "m.c:85" in out[0]


def test_no_path_blocked_by_an_actor_names_its_uid_and_tile():
    log = "pilot: no path: no path from 3,4 to warp:7 (the zone's warps: 1 goal tiles, none reachable); warps reached lead to: zone 8 (1,1) (G token G:warp:7)\n"
    out = run("stuck", "Pa G:warp:7 Wf", log=log, dump=HEAD.format(z=6, p="3,4") + GATE + LISTS)
    assert 2 <= len(out) <= 3, out
    assert "G:warp:7: blocked by uid 8 at 6,2" in out[0] and "G:npc:8" in out[1], out


def test_no_path_ride_reason_is_not_a_window_problem():
    for why in ("ride:3: switch T1 wrong for spot 3 (car 1 runs path 2: G:trig:1 throws it)",
                "ride:13: the ride starts at 4,14 on floor y 20, the player is at y 0 (another ride first)"):
        out = run("stuck", "Pa G:ride:3", log=f"pilot: no path: {why} (G token G:ride:3)\n", dump=HEAD.format(z=63, p="3,4") + GRID + LISTS)
        assert why.split(": ", 1)[1][:30] in out[0] and "window" not in out[0], out


def test_no_path_honey_wall_names_g_wall():
    lists = LISTS.replace("D0 5,1 1x1 -> zone 7", "D0 5,1 1x1 -> zone 99").replace("uid 8 at 6,2 (from 6,2) code 85", "uid 8 at 6,2 (from 6,2) code 0")
    log = "pilot: no path: no path from 3,4 to warp:7 (the zone's warps: 1 goal tiles, none reachable) (G token G:warp:7)\n"
    out = run("stuck", "Pa G:warp:7", log=log, dump=HEAD.format(z=29, p="3,4") + GATE + lists)
    assert "honey wall" in out[0] and "G:wall:8" in out[1], out


def test_no_path_target_beyond_the_dump_but_in_planner_range_says_unreachable():
    log = "pilot: no path: no path from 3,4 to G:npc:40 (the tiles facing it: 3 goal tiles, none reachable) (G token G:npc:40)\n"
    out = run("stuck", "G:npc:40", log=log, dump=HEAD.format(z=97, p="3,4") + GRID + LISTS)   # uid 40 is not in the dump's lists
    assert "unreachable" in out[0] and "3 tiles" in out[0] and "window" not in out[0], out
    log = "pilot: no path: G:npc:40: no free tile (0 goal tiles, none reachable) (G token G:npc:40)\n"
    out = run("stuck", "G:npc:40", log=log, dump=HEAD.format(z=97, p="3,4") + GRID + LISTS)
    assert "window" in out[0], out   # still the old verdict when the planner saw no tile either


def test_no_path_target_not_loaded_says_walk_closer():
    log = "pilot: no path: no path from 3,4 toward 90,4 (no tile nearer reachable) (G token G:90,4)\n"
    out = run("stuck", "G:90,4", log=log, dump=HEAD.format(z=6, p="3,4") + GRID + LISTS)
    assert "not on the loaded map" in out[0] and "G:X,Z" in out[1], out


def test_no_path_scenery_actor_and_unknown_uid_and_wall_closed():
    log = "pilot: no path: A facing 1 at 5,3 started no event (3 tries) (G token G:npc:8)\n"
    out = run("stuck", "G:npc:8", log=log, dump=HEAD.format(z=6, p="5,3") + GRID + LISTS)
    assert "started no event" in out[0] and "script 6 ID" in out[1] and "uid 8" in out[1], out
    dump = HEAD.format(z=6, p="5,3") + GRID + LISTS + "actors (3 NPC entries):\n  uid 9 at 1,1 script 4 ev 0 flag 0x021b set: hidden; not on the map\n"
    log = "pilot: no path: G:npc:9: no actor of uid 9 in zone 6 (the actors' uids: 1 2) (G token G:npc:9)\n"
    out = run("stuck", "G:npc:9", log=log, dump=dump)
    assert "hidden" in out[0] and "who-sets flag 0x021b =0" in out[1], out
    out = run("stuck", "G:wall:4", log="pilot: no path: wall 4 closed (no step in 120 frames at 3,4, key 1) (G token G:wall:4)\n")
    assert "wall 4 closed" in out[0] and "switches" in out[1], out


def test_stuck_walk_and_warp_that_does_not_take():
    log = "pilot: G: 3,4 -> waypoint 5,4 (3 steps to go)\npilot: stuck at 3,4 (G: 6 re-plans without a step)\n"
    out = run("stuck", "G:9,4", log=log, dump=HEAD.format(z=6, p="3,4") + GRID + LISTS)
    assert "6 re-plans" in out[0] and "toward 5,4" in out[0], out
    out = run("stuck", "G:warp:7", log="pilot: stuck at 5,2 (G: the warp does not take)\n", dump=HEAD.format(z=6, p="5,2") + GRID + LISTS)
    assert "warp does not take" in out[0] and "scripted door" in out[1] and "who-warps 6" in out[1], out


def test_waiting_yes_no_scene_battle_ws_and_py():
    hang = "pilot: hang: Wf:300, event on, zone 427 39,741, no progress in 3000 frames"
    out = run("hang", "Pa Wf:300", game="pilot: event on at V-blank 9\n" + hang + ", a yes/no list is up (Py or Pn answers it) at V-blank 99\n",
              dump=HEAD.format(z=427, p="3,4") + GRID + LISTS)
    assert "unanswered yes/no" in out[0] and "Py (Yes) or Pn" in out[1], out
    out = run("hang", "Pa Wf:300", game="pilot: event on at V-blank 9\n" + hang + " at V-blank 99\n", dump=HEAD.format(z=6, p="3,4") + GRID + LISTS)
    assert "a scene never ended" in out[0] and "event on since V-blank 9" in out[0], out
    out = run("hang", "Pa G:npc:2", game="pilot: hang: G:npc:2, event battle, zone 6 3,4, no progress in 12000 frames at V-blank 99\n")
    assert out[0].startswith("  > battle hang"), out
    out = run("timeout", "Pa Ws:0x4001=3", game="pilot: timeout: Ws:0x4001=3 not done in 20000 frames\n", dump=HEAD.format(z=6, p="3,4") + GRID + LISTS)
    assert "never became true" in out[0] and "work 0x4001 is 2 now" in out[0] and "who-sets work 0x4001 =3" in out[1], out
    out = run("hang", "Pa Py", game="pilot: hang: Py: no yes/no list in 3000 frames, zone 6 3,4 at V-blank 99\n")
    assert "Py waited 3000 frames" in out[0], out


def test_off_route_names_the_trigger_that_did_not_fire():
    log = "pilot: G:trig:0 done at 3,3 zone 6 (V-blank 50)\n"
    out = run("off-route", "Pa G:trig:0 Wf S W30", where="zone 6 3,3 (want zone 7 1,2) 5s", log=log, dump=HEAD.format(z=6, p="3,3") + GRID + LISTS)
    assert "ended zone 6 3,3, wanted zone 7 1,2" in out[0] and "T0 needs work 0x40b4 = 1" in out[0], out
    assert "who-sets work 0x40b4 =1" in out[1]
    out = run("off-route", "Pa S", where="zone 6 3,3 (want zone 7 1,2) 5s")
    assert out[0] == "  > off-route: ended zone 6 3,3, wanted zone 7 1,2" and len(out) == 2


def test_unclassified_and_no_path_zone_and_save():
    out = run("weird", "Pa", log="pilot: foo at V-blank 1\npilot: bar at V-blank 2\n")
    assert out[0].startswith("  > unclassified (weird): foo at V-blank 1 | bar at V-blank 2"), out
    out = run("no-path", "G:zone:5-9", where="G:zone:5-9: no atlas path from zone 5 to zone 9")
    assert "no atlas path" in out[0] and "atlas.py b path" in out[1]
    out = run("save-failed", "Pa S W30", where="zone 6 1,1 (save.bin unchanged)", log="pilot: S: X at V-blank 3\n")
    assert "the save did not finish" in out[0], out
    assert all(len(line) <= 240 for line in out)


def test_load_logs_reads_the_run_work_files_and_the_newest_dump():
    w = Path(tempfile.mkdtemp())
    (w / "b-x.log").write_text("L")
    (w / "b-x.game.log").write_text("G")
    (w / "b-x-map").mkdir()
    (w / "b-x-map" / "mapdump-01-a.txt").write_text("one")
    (w / "b-x-map" / "mapdump-02-b.txt").write_text("two")
    assert T.load_logs(w, "b", "x", "W") == dict(where="W", log="L", game="G", dump="two")
    assert T.load_logs(w, "b", "none") == dict(where="", log="", game="", dump="")


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
