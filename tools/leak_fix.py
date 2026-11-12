#!/usr/bin/env python3
"""Moves the ROM-matching tables of vendored Black 2 / White 2 files to asset rows and says what still leaks.

    python3 tools/leak_fix.py [--version b2_us] [--allow-code] FILE...     # FILE: src/ov036/foo.c or the full path

Per file: asset.py --move-all (every table of 12 numbers or names), then tools/rom_leak_check.py --where, then --move of
the tables the findings name that move-all left (smaller ones), rescanning, until the file is clean or a round moves
nothing. What is left is listed with its lines and the reason it could not move:

    LEFT src/system/pokedex_savedata.c [u16] 237B L723-726 unovaUnks: not in main's symbols.txt (static local ...)
    LEFT src/system/mcss.c [u32] 57B L528-550 (code): ... numbers in function bodies, not a table

A finding that lies in code (outside any initialiser) is decompiled code's own constants: nothing to move. --allow-code
appends the allow line for such files (tools/rom_leak_allow.txt, with its reason) when nothing else is left in them.
Exit status 1 when a file still leaks. The moves are made in the working tree and are not committed.
"""
import argparse
import os
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asset  # noqa: E402
import rom_leak_check as R  # noqa: E402

PREFIX = "third_party/pokebw2/"
CODE_REASON = "decompiled code's own constants (a literal-pool coincidence), not a table"


def scan(paths, cache, jobs):
    """{path: [(kind, bytes_total, where runs)]} of the files over their allow-list limit."""
    roms = [r for r in R.DEFAULT_ROMS if os.path.exists(r[1]) and (r[2] is None or os.path.isdir(r[2]))]
    allow = R.load_allow()
    res = R.scan_cached(paths, roms, cache, jobs, True, False)
    bad = sorted({r[0] for r in res if r[3] > allow.get(r[0], -1)})
    found = R.where_files(bad, roms, cache, jobs) if bad else {}
    return {p: [(k, runs) for k, runs in found[p]] for p in bad}


def tables_of(found):
    return list(dict.fromkeys(t for _, runs in found for r in runs for t in r[3]))


def reason_for(table, skipped):
    return skipped.get(table) or "not tried (smaller than 12 numbers) or already moved: it still matches"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="+")
    ap.add_argument("--version", default="b2_us")
    ap.add_argument("--allow-code", action="store_true", help="add allow lines for files whose findings are all in code")
    ap.add_argument("--cache", default=os.environ.get("ROMLEAK_CACHE", os.path.join(R._main_checkout(), "orig", "leak_cache")))
    a = ap.parse_args()
    paths = [f if f.startswith(PREFIX) or a.version not in asset.TWIN else PREFIX + f.removeprefix("./")
             for f in (f.removeprefix(R.REPO + "/") for f in a.files)]   # Black's own files: no prefix
    jobs = int(os.environ.get("BW_JOBS", "8"))
    skipped = {p: {} for p in paths}
    for p in paths:
        s, n = asset.move_all(a.version, p)
        skipped[p].update(s)
        print(f"leak_fix {p}: moved {n}, left {len(s)}")
    left = scan(paths, a.cache, jobs)
    for rnd in range(3):   # the tables a finding names that move-all did not try (small ones), until a round moves nothing
        moved = 0
        for p, found in left.items():
            todo = [t for t in tables_of(found) if t not in skipped[p]]
            if todo:
                s, n = asset.move_all(a.version, p, only=set(todo))
                skipped[p].update(s)
                moved += n
        if not moved:
            break
        left = scan(list(left), a.cache, jobs)
    code_only = []
    for p, found in sorted(left.items()):
        for kind, runs in found:
            for first, last, n, tables in runs:
                where = f"L{first}" + (f"-{last}" if last != first else "") + " " + (",".join(dict.fromkeys(tables)) or "(code)")
                ts = list(dict.fromkeys(tables))
                why = "; ".join((f"{t}: " if len(ts) > 1 else "") + reason_for(t, skipped[p]) for t in ts) or CODE_REASON
                print(f"LEFT {p.removeprefix(PREFIX)} [{kind}] {n}B {where}: {why}")
        if all(not r[3] for _, runs in found for r in runs):
            code_only.append((p, max(sum(r[2] for r in runs) for _, runs in found)))
    for p, limit in code_only:
        line = f"{p} {limit}  # {CODE_REASON}"
        if a.allow_code:
            with open(os.path.join(R.REPO, "tools", "rom_leak_allow.txt"), "a") as f:
                f.write(line + "\n")
        print(("allowed: " if a.allow_code else "allow line: ") + line)
    print(f"leak_fix: {len(paths) - len(left)} of {len(paths)} files clean")
    return 1 if left and not (a.allow_code and len(code_only) == len(left)) else 0


if __name__ == "__main__":
    sys.exit(main())
