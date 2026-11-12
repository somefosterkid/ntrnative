#!/usr/bin/env python3
"""One row per function and file in docs/nonmatching.md.

A merge that keeps both sides of a conflicted table (tools/merge_worker.sh) or a hunk applied twice leaves a function
with two or more rows (wave 12: gpiBuffer.c, field_rail.c, bmp.c; dwc_match.c's rows came 12 times). Every writer of the
table updates all of a function's rows alike, so the copies only grow. The row kept is the best: the fewest lines of
diff (a number beats "not written"), then the longest tried text (the newest adds to it), then the last. It stays at the
place of the function's first row.

    python3 tools/nonmatching_dedupe.py          # lists the duplicates, exit 1 when there are any
    python3 tools/nonmatching_dedupe.py --fix    # drops the worse copies
"""
import re
import sys
from pathlib import Path

NM = Path(__file__).resolve().parents[1] / "docs" / "nonmatching.md"
ROW = re.compile(r"^\| `([^`]+)` \| `0x[0-9a-f]+` \| `([^`]+)` \|")


def key(line):
    m = ROW.match(line)
    return (m[1], m[2]) if m else None


def quality(line):
    """Sorts the better row last."""
    m = re.search(r"\| (\d+) lines of diff", line)
    return (-int(m[1]) if m else float("-inf"), len(line))


def dedupe(text):
    """(text with one row per (function, file), the number of rows dropped). The tables of the assembly in the original and
    of the nonmatching C are one namespace: a function is in one of them."""
    lines = text.split("\n")
    best = {}
    for i, l in enumerate(lines):
        k = key(l)
        if k and (k not in best or quality(l) >= quality(lines[best[k]])):
            best[k] = i
    first = {}
    for i, l in enumerate(lines):
        if (k := key(l)):
            first.setdefault(k, i)
    out, dropped = [], 0
    for i, l in enumerate(lines):
        k = key(l)
        if not k:
            out.append(l)
        elif i == first[k]:
            out.append(lines[best[k]])
        else:
            dropped += 1
    return "\n".join(out), dropped


def duplicates(text):
    """[(function, file, rows)] of the functions with more than one row."""
    seen = {}
    for l in text.split("\n"):
        if (k := key(l)):
            seen[k] = seen.get(k, 0) + 1
    return [(*k, n) for k, n in seen.items() if n > 1]


def main():
    text = NM.read_text()
    dups = duplicates(text)
    if "--fix" in sys.argv:
        new, n = dedupe(text)
        if n:
            NM.write_text(new)
        print(f"{n} duplicate rows dropped ({len(dups)} functions)")
        return 0
    for fn, f, n in dups:
        print(f"{n} rows: {fn} in {f}")
    if dups:
        print(f"{len(dups)} functions with more than one row in docs/nonmatching.md: python3 tools/nonmatching_dedupe.py --fix")
    return 1 if dups else 0


if __name__ == "__main__":
    sys.exit(main())
