#!/usr/bin/env python3
"""Tests for tools/leak_fix.py with asset.move_all and the scan stubbed (no ROM): the loop, the report lines, the
allow line for findings in code.

    python3 tools/leak_fix_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import contextlib
import io
import os
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import leak_fix as L  # noqa: E402


def run(files, scans, moves, extra=()):
    """main() with the scans (one per call) and move_all results (path -> list of (skipped, moved)) given."""
    calls = []
    old = L.scan, L.asset.move_all, sys.argv
    scans = list(scans)

    def fake_scan(paths, cache, jobs):
        return scans.pop(0) if len(scans) > 1 else scans[0]

    def fake_move(version, path, min_values=12, only=None):
        calls.append((path, None if only is None else sorted(only)))
        return moves[path].pop(0) if len(moves[path]) > 1 else moves[path][0]
    L.scan, L.asset.move_all, sys.argv = fake_scan, fake_move, ["leak_fix", *extra, *files]
    out = io.StringIO()
    try:
        with contextlib.redirect_stdout(out):
            rc = L.main()
    finally:
        L.scan, L.asset.move_all, sys.argv = old
    return rc, out.getvalue(), calls


def tests():
    ok = True

    def check(name, cond, detail=None):
        nonlocal ok
        print(("ok   " if cond else "FAIL ") + name + ("" if cond or detail is None else f"  {detail}"))
        ok &= bool(cond)

    P = "third_party/pokebw2/src/a/x.c"
    table = [("u16", [(10, 20, 62, ["sBig"])])]
    # clean after the first pass
    rc, out, calls = run(["src/a/x.c"], [{}], {P: [({}, 2)]})
    check("clean file: status 0", rc == 0 and "1 of 1 files clean" in out and calls == [(P, None)])
    # a small table the first pass did not try is moved by the second round, then the file is clean
    rc, out, calls = run(["src/a/x.c"], [{P: table}, {}], {P: [({}, 0), ({}, 1)]})
    check("small table: tried by name, then clean", rc == 0 and calls == [(P, None), (P, ["sBig"])], calls)
    # a table that cannot move is listed with its lines and reason; the loop stops
    rc, out, calls = run(["src/a/x.c"], [{P: table}], {P: [({"sBig": "not in main's symbols.txt"}, 0)]})
    check("stuck: LEFT line with lines and reason", rc == 1 and
          "LEFT src/a/x.c [u16] 62B L10-20 sBig: not in main's symbols.txt" in out and len(calls) == 1, out)
    # findings in code only: an allow line (written with --allow-code)
    code = [("u32", [(5, 9, 57, [])])]
    rc, out, calls = run(["src/a/x.c"], [{P: code}], {P: [({}, 0)]})
    check("code finding: allow line suggested", rc == 1 and "allow line: " + P + " 57  # decompiled code" in out, out)
    with tempfile.TemporaryDirectory() as tmp:
        old = L.R.REPO
        L.R.REPO = tmp
        os.makedirs(os.path.join(tmp, "tools"))
        try:
            rc, out, _ = run(["src/a/x.c"], [{P: code}], {P: [({}, 0)]}, ["--allow-code"])
            text = open(os.path.join(tmp, "tools", "rom_leak_allow.txt")).read()
            check("--allow-code writes the line", rc == 0 and text.startswith(P + " 57  # "), text)
        finally:
            L.R.REPO = old
    return ok


if __name__ == "__main__":
    sys.exit(0 if tests() else 1)
