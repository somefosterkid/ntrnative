#!/usr/bin/env python3
"""Drops the rows of docs/nonmatching.md that are out of date: a function that matches now (its file complete, or not
in tools/brief.py's list of the file's functions that don't), of a file no delinks.txt lists (renamed), and all but the last row of a function listed twice.
Merges of parallel branches leave such rows behind.

    python3 tools/nonmatching_clean.py
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ROW = re.compile(r"\| `(\w+)`[^|]*\| `0x[0-9a-f]+` \| `((?:src|lib)/[^`]+\.c)` \|")


def main():
    complete, listed = set(), set()
    for d in (ROOT / "config").rglob("delinks.txt"):
        for entry in re.split(r"\n(?=\S)", d.read_text()):
            listed.add(entry.split(":")[0])
            if re.search(r"^\s+complete$", entry, re.M):
                complete.add(entry.split(":")[0])
    path = ROOT / "docs" / "nonmatching.md"
    lines = path.read_text().splitlines(True)
    rows = {i: m.groups() for i, l in enumerate(lines) if (m := ROW.match(l))}
    left = {}  # file: the functions brief.py lists as not matching
    for f in sorted({f for _, f in rows.values()} - complete):
        if f not in listed:
            continue  # renamed: its rows go
        out = subprocess.run([sys.executable, "tools/brief.py", f], cwd=ROOT, capture_output=True, text=True).stdout
        if re.search(r"functions match", out):
            left[f] = set(re.findall(r"^(\w+)\s+0x", out, re.M))
    last = {v: i for i, v in rows.items()}
    drop = {i for i, (fn, f) in rows.items()
            if f in complete or f not in listed or (f in left and fn not in left[f]) or last[fn, f] != i}
    path.write_text("".join(l for i, l in enumerate(lines) if i not in drop))
    print(f"{len(drop)} rows dropped, {len(rows) - len(drop)} left")


if __name__ == "__main__":
    main()
