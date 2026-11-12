#!/usr/bin/env python3
"""Tests of tools/nonmatching_dedupe.py, and the check on docs/nonmatching.md itself.   python3 tools/nonmatching_dedupe_tests.py"""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import nonmatching_dedupe as nd  # noqa: E402


def row(fn, d, tried="t", f="src/a.c"):
    return f"| `{fn}` | `0x02000000` | `{f}` | {d} | {tried} |"


class Dedupe(unittest.TestCase):
    def test_identical(self):
        t = "\n".join(["# x", row("a", "5 lines of diff"), row("b", "not written"), row("a", "5 lines of diff"), ""])
        out, n = nd.dedupe(t)
        self.assertEqual(n, 1)
        self.assertEqual(out, "\n".join(["# x", row("a", "5 lines of diff"), row("b", "not written"), ""]))

    def test_best_kept_at_first_place(self):
        t = "\n".join([row("a", "not written"), row("b", "1 lines of diff"), row("a", "9 lines of diff"),
                      row("a", "4 lines of diff", "t; more")])
        out, n = nd.dedupe(t)
        self.assertEqual(n, 2)
        self.assertEqual(out.split("\n"), [row("a", "4 lines of diff", "t; more"), row("b", "1 lines of diff")])

    def test_same_diff_longer_text(self):
        out, _ = nd.dedupe("\n".join([row("a", "4 lines of diff", "t; more"), row("a", "4 lines of diff")]))
        self.assertEqual(out, row("a", "4 lines of diff", "t; more"))

    def test_other_file_is_another_row(self):
        t = "\n".join([row("a", "1 lines of diff"), row("a", "1 lines of diff", f="src/b.c")])
        self.assertEqual(nd.dedupe(t), (t, 0))
        self.assertEqual(nd.duplicates(t), [])

    def test_notes_row(self):
        t = "\n".join([row("-", "FAILED x"), row("-", "FAILED x")])
        self.assertEqual(nd.dedupe(t)[1], 1)


class RealFile(unittest.TestCase):
    def test_no_function_has_two_rows(self):
        self.assertEqual(nd.duplicates(nd.NM.read_text()), [], "python3 tools/nonmatching_dedupe.py --fix")


if __name__ == "__main__":
    unittest.main()
