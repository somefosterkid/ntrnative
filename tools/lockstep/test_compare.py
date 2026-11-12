#!/usr/bin/env python3
"""Tests for lstep_compare.py with synthetic dumps: python3 tools/lockstep/test_compare.py"""
import contextlib
import copy
import io
import json
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lstep_compare as lc

HEADER = {"lockstep": 1, "side": "melonds", "rom": "r.nds", "save": "town.bin", "inputs": "up@f1",
          "clock": "2026-10-29T12:00:00", "anchor_g": 100, "anchor_vblank": 120, "vars": "tools/lockstep/vars.txt"}


def grid(r, g, b):
    return ("%02x%02x%02x" % (r, g, b)) * 48


def rec(f):
    return {"f": f, "g": 100 + f, "vb": 120 + f, "lag": 0, "keys": "up" if f % 4 < 2 else "",
            "top": "%08x" % (f * 7 + 1), "bot": "%08x" % (f * 13 + 5), "gtop": grid(10, 20, 30), "gbot": grid(5, 6, 7),
            "vars": {"game.frame": 100 + f, "rng.lc": "0x0123456789abcdef", "player.x": 10 + f // 16,
                     "player.z": 4, "event.cb": None, "script.id": 0, "eventwork": "deadbeef", "party": "cafef00d",
                     "battle.main": None}}


def dump(n=40, side="melonds"):
    h = dict(HEADER, side=side)
    return h, [rec(f) for f in range(n)]


def write(path, h, recs):
    with open(path, "w") as fh:
        fh.write(json.dumps(h) + "\n")
        for r in recs:
            fh.write(json.dumps(r) + "\n")


class T(unittest.TestCase):
    def setUp(self):
        self.d = tempfile.mkdtemp()

    def run_cmp(self, a, b, *opts):
        pa, pb = os.path.join(self.d, "a.jsonl"), os.path.join(self.d, "b.jsonl")
        write(pa, *a)
        write(pb, *b)
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rc = lc.main([pa, pb] + list(opts))
        return rc, out.getvalue()

    def test_identical(self):
        rc, out = self.run_cmp(dump(), dump(side="port"))
        self.assertEqual(rc, 0)
        self.assertEqual(out.strip().splitlines()[-1], "SAME frames=40")

    def test_game_frame_ignored_by_default(self):
        b = dump(side="port")
        for r in b[1]:
            r["vars"]["game.frame"] += 300
        rc, out = self.run_cmp(dump(), b)
        self.assertEqual(rc, 0)
        rc, out = self.run_cmp(dump(), b, "--compare-frame")
        self.assertEqual(rc, 1)
        self.assertIn("game.frame", out.strip().splitlines()[-1])

    def test_overlay_alias_matches(self):
        a, b = dump(), dump(side="port")
        for r in a[1]:
            r["vars"]["event.cb"] = "func_ov021_021bee74|func_ov114_021bee74"
        for r in b[1]:
            r["vars"]["event.cb"] = "func_ov021_021bee74"
        self.assertEqual(self.run_cmp(a, b)[0], 0)
        b[1][5]["vars"]["event.cb"] = "func_ov021_021bee75"
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 1)
        self.assertIn("f=5", out.strip().splitlines()[-1])

    def test_skip_lag(self):
        a, b = dump(), dump(side="port")
        a[1][7]["lag"] = 3
        a[1][7]["vars"]["player.z"] = 99
        self.assertEqual(self.run_cmp(a, b)[0], 1)
        self.assertEqual(self.run_cmp(a, b, "--skip-lag")[0], 0)

    def test_var_diverges(self):
        a, b = dump(), dump(side="port")
        for r in b[1][17:]:
            r["vars"]["player.x"] += 1
            r["vars"]["rng.lc"] = "0x1"
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 1)
        last = out.strip().splitlines()[-1]
        self.assertTrue(last.startswith("DIVERGED f=17 g=117 kind=vars groups=position,rng"), last)
        self.assertIn("[position  ] player.x", out)
        self.assertIn("last 5 frames", out)
        # ignoring those vars lets it pass; --after skips the frame
        self.assertEqual(self.run_cmp(a, b, "--ignore-var", "player.x,rng.*")[0], 0)
        self.assertEqual(self.run_cmp(a, b, "--ignore-group", "position", "--ignore-group", "rng")[0], 0)
        rc, out = self.run_cmp(a, b, "--after", "30", "-q")
        self.assertEqual(rc, 1)
        self.assertTrue(out.startswith("DIVERGED f=31 "), out)
        self.assertEqual(len(out.strip().splitlines()), 1)

    def test_later_group_flags_party(self):
        a, b = dump(), dump(side="port")
        b[1][9]["vars"]["eventwork"] = "00000000"
        b[1][9]["vars"]["party"] = "00000001"
        rc, out = self.run_cmp(a, b, "-q")
        self.assertIn("f=9 ", out)
        self.assertIn("groups=flags/work,party", out)

    def test_screen_only(self):
        a, b = dump(), dump(side="port")
        # renderer difference: hash differs, grid within tolerance, from frame 5 on
        for r in b[1][5:]:
            r["top"] = "ffffffff"
            r["gtop"] = grid(11, 19, 32)
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 0)
        self.assertIn("renderer-only-frames=35", out)
        # a real picture difference at frame 12 (vars equal)
        for r in b[1][12:]:
            r["gbot"] = "ff0000" + grid(5, 6, 7)[6:]
            r["bot"] = "11111111"
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 1)
        last = out.strip().splitlines()[-1]
        self.assertTrue(last.startswith("DIVERGED f=12 g=112 kind=screens-only top=renderer(bad=0,worst=2) bot=DIFF(bad=1"), last)
        self.assertIn("only the picture differs", out)
        self.assertEqual(self.run_cmp(a, b, "--ignore-screens")[0], 0)
        self.assertEqual(self.run_cmp(a, b, "--tol", "250")[0], 0)

    def test_port_frame_skip(self):
        # the port drew only every 4th frame (LSTEP_SKIP=4): null screens elsewhere, compared only where both have them
        a, b = dump(), dump(side="port")
        for r in b[1]:
            if r["f"] % 4:
                r["top"] = r["bot"] = r["gtop"] = r["gbot"] = None
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 0)
        self.assertTrue(out.strip().splitlines()[-1].startswith("SAME frames=40 screens-compared=10"), out)
        self.assertIn("screens compared on 10 of them", out)
        # a picture difference on a drawn frame (f=8) is found; one on a frame the port did not draw (f=9) cannot be
        for r in b[1][8:]:
            if r["f"] % 4 == 0:
                r["gbot"] = "ff0000" + grid(5, 6, 7)[6:]
                r["bot"] = "11111111"
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 1)
        self.assertIn("DIVERGED f=8 ", out.strip().splitlines()[-1])
        # vars still compared on the frames without screens
        a, b = dump(), dump(side="port")
        for r in b[1]:
            if r["f"] % 4:
                r["top"] = r["bot"] = r["gtop"] = r["gbot"] = None
        b[1][6]["vars"]["player.z"] = 5
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 1)
        self.assertIn("DIVERGED f=6 g=106 kind=vars", out.strip().splitlines()[-1])
        self.assertIn("screens=not-drawn", out.strip().splitlines()[-1])

    def test_vars_and_screens(self):
        a, b = dump(), dump(side="port")
        b[1][3]["vars"]["player.z"] = 5
        b[1][3]["top"] = "0"
        rc, out = self.run_cmp(a, b, "-q")
        self.assertIn("kind=vars", out)
        self.assertIn("screens=renderer-only", out)

    def test_beam_race(self):
        a, b = dump(), dump(side="port")
        # frame 9: the top screen differs in the bottom row only (cells 40-47), the next frame agrees
        bad = grid(10, 20, 30)[:40 * 6] + grid(200, 20, 30)[:8 * 6]
        b[1][9]["gtop"] = bad
        b[1][9]["top"] = "deadbeef"
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 1)
        self.assertIn("DIVERGED f=9 ", out.strip().splitlines()[-1])
        rc, out = self.run_cmp(a, b, "--ignore-beam-race")
        self.assertEqual(rc, 0)
        self.assertIn("beam-race-frames=1", out.strip().splitlines()[-1])
        # the difference lasts two frames: a real one
        b[1][10]["gtop"] = bad
        b[1][10]["top"] = "deadbef0"
        rc, out = self.run_cmp(a, b, "--ignore-beam-race")
        self.assertEqual(rc, 1)
        self.assertIn("DIVERGED f=9 ", out.strip().splitlines()[-1])
        # a cell above the bottom row: a real one
        b2 = dump(side="port")
        b2[1][9]["gtop"] = grid(10, 20, 30)[:30 * 6] + grid(200, 20, 30)[:6] + grid(10, 20, 30)[:17 * 6]
        b2[1][9]["top"] = "deadbeef"
        self.assertEqual(self.run_cmp(a, b2, "--ignore-beam-race")[0], 1)
        # a cell above the bottom row that equals the port's own frame f+1 / f-1 cell: melonDS drew it before/after the write
        plain = grid(10, 20, 30)
        odd = plain[:30 * 6] + grid(200, 20, 30)[:6] + plain[:17 * 6]
        a5, b5 = dump(), dump(side="port")
        a5[1][9]["gtop"] = odd; a5[1][9]["top"] = "1"; a5[1][10]["gtop"] = odd; a5[1][10]["top"] = "2"
        b5[1][10]["gtop"] = odd; b5[1][10]["top"] = "2"
        self.assertEqual(self.run_cmp(a5, b5, "--ignore-beam-race")[0], 0)
        self.assertEqual(self.run_cmp(a5, b5)[0], 1)
        a6, b6 = dump(), dump(side="port")
        a6[1][9]["gtop"] = odd; a6[1][9]["top"] = "1"
        b6[1][8]["gtop"] = odd; b6[1][8]["top"] = "3"
        self.assertEqual(self.run_cmp(a6, b6, "--ignore-beam-race", "--after", "8")[0], 0)
        a7, b7 = dump(), dump(side="port")
        a7[1][9]["gtop"] = odd; a7[1][9]["top"] = "1"
        self.assertEqual(self.run_cmp(a7, b7, "--ignore-beam-race")[0], 1)
        # a var differing on that frame: a real one
        b3 = dump(side="port")
        b3[1][9]["gtop"] = bad
        b3[1][9]["top"] = "deadbeef"
        b3[1][9]["vars"]["player.z"] = 5
        self.assertEqual(self.run_cmp(a, b3, "--ignore-beam-race")[0], 1)
        # the last frame has no next frame to agree: not skipped
        b4 = dump(side="port")
        b4[1][39]["gtop"] = bad
        b4[1][39]["top"] = "deadbeef"
        self.assertEqual(self.run_cmp(a, b4, "--ignore-beam-race")[0], 1)

    def test_beam_race_whole_screen(self):
        # the bottom screen changes at f=9: melonDS shows the new picture in f=9, the port in f=10 (frame f+1 agrees)
        new_g = grid(200, 100, 50)
        a, b = dump(), dump(side="port")
        for r in a[1][9:]:
            r["gbot"] = new_g; r["bot"] = "aa"
        for r in b[1][10:]:
            r["gbot"] = new_g; r["bot"] = "aa"
        self.assertEqual(self.run_cmp(a, b)[0], 1)
        rc, out = self.run_cmp(a, b, "--ignore-beam-race")
        self.assertEqual(rc, 0, out)
        # a port-only blip at f=9 (melonDS static) is a real difference
        a, b = dump(), dump(side="port")
        b[1][9]["gbot"] = new_g; b[1][9]["bot"] = "bb"
        self.assertEqual(self.run_cmp(a, b, "--ignore-beam-race")[0], 1)

    def test_missing_frame(self):
        a, b = dump(), dump(side="port")
        del b[1][8]
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 1)
        self.assertIn("DIVERGED f=8 kind=missing-frame absent-in=port", out)
        self.assertEqual(self.run_cmp(a, b, "--allow-missing")[0], 0)

    def test_unequal_lengths(self):
        a, b = dump(40), dump(35, side="port")
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 1)
        self.assertIn("DIVERGED f=35 kind=length shorter=port melonds=40 port=35", out)
        rc, out = self.run_cmp(a, b, "--allow-short")
        self.assertEqual(rc, 0)
        self.assertIn("SAME frames=35", out)

    def test_bad_input(self):
        p = os.path.join(self.d, "bad.jsonl")
        with open(p, "w") as fh:
            fh.write("{not json\n")
        with contextlib.redirect_stderr(io.StringIO()), contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(lc.main([p, p]), 2)

    def test_header_note(self):
        a, b = dump(), dump(side="port")
        b[0]["save"] = "other.bin"
        rc, out = self.run_cmp(a, b)
        self.assertEqual(rc, 0)
        self.assertIn("note: header save differs", out)


if __name__ == "__main__":
    unittest.main()
