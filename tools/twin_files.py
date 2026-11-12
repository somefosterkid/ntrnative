#!/usr/bin/env python3
"""Propose Pokemon Black's source files from pokebw2's, using the twins found by tools/twins.py.

For every pokebw2 source file with a .text range, takes its B2 functions from pokebw2's symbols.txt and looks up the
Black functions whose code equals them (the twins TSV). The Black file goes in the module with the most twinned
functions, from the first twin's start to the end of the last twin (rounded up to 4 bytes, but not into the next
function). Writes one TSV row per pokebw2 file, best candidates first: every function twinned, contiguous and in B2's
order; then by the twinned fraction of max(B2 functions, Black functions in the range). Prints counts per class.

    python3 tools/twin_files.py --bw2 ~/ref/pokebw2 --twins build/twins_b_us.tsv --tsv build/twin_files.tsv

A B2 function whose code appears several times in Black (stubs, copies) gets the copy that sits closest to where the
file's other twins put it. twins.py keeps one B2 function per Black function, so of two identical B2 functions only
one ever has a twin.
"""
import argparse
import glob
import os
import re
from collections import Counter, defaultdict

import bwcache


def module_of(path):
    mod = os.path.basename(os.path.dirname(path))
    return "main" if mod == "arm9" else mod


def read_symbols(root):
    """Returns {module: sorted [(address, size, name)]} of the functions in a symbols.txt tree."""
    out = defaultdict(list)
    for path in glob.glob(f"{root}/**/symbols.txt", recursive=True):
        for line in open(path):
            m = re.match(r"(\S+) kind:function\(\w+,size=(0x\w+).* addr:(0x\w+)", line)
            if m:
                out[module_of(path)].append((int(m[3], 16), int(m[2], 16), m[1]))
    return {mod: sorted(funcs) for mod, funcs in out.items()}


def read_delinks(root):
    """Returns [(module, source, complete, start, end)] for every source file with a .text range. Read from
    build/cache/delinks when no delinks.txt of the tree changed (size and mtime)."""
    paths = sorted(glob.glob(f"{root}/**/delinks.txt", recursive=True))
    token = bwcache.files_token(paths, __file__)
    out = bwcache.get("delinks", root, token)
    if out is None:
        out = _read_delinks(paths)
        bwcache.put("delinks", root, token, out)
    return list(out)


def _read_delinks(paths):
    out = []
    for path in paths:
        src = None
        for line in open(path):
            if line.strip() and not line[0].isspace():
                src, done = line.strip().rstrip(":"), False
            elif line.strip() == "complete":
                done = True
            m = re.match(r"\s+\.text\s+start:(0x\w+) end:(0x\w+)", line)
            if m and src:
                out.append((module_of(path), src, done, int(m[1], 16), int(m[2], 16)))
    return out


def main():
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--bw2", default=os.path.expanduser("~/ref/pokebw2"), help="pokebw2 checkout (read only)")
    ap.add_argument("--config", default=f"{root}/config/b_us/arm9", help="Black's symbols.txt and delinks.txt tree")
    ap.add_argument("--twins", default=f"{root}/build/twins_b_us.tsv", help="written by tools/twins.py")
    ap.add_argument("--tsv", default=f"{root}/build/twin_files.tsv")
    args = ap.parse_args()

    b2_funcs = read_symbols(f"{args.bw2}/config/b2_us/arm9")
    black_funcs = read_symbols(args.config)
    black_files = read_delinks(args.config)
    twins = defaultdict(list)  # (bw2 source, bw2 function) -> [(module, address)]
    for line in list(open(args.twins))[1:]:
        _, mod, addr, _, src, fn, _ = line.rstrip("\n").split("\t")
        twins[src, fn].append((mod, int(addr, 16)))

    rows = []
    for b2_mod, src, done, start, end in read_delinks(f"{args.bw2}/config/b2_us/arm9"):
        funcs = [f for f in b2_funcs.get(b2_mod, []) if start <= f[0] < end]
        row = {"bw2_source": src, "bw2_complete": "yes" if done else "no", "bw2_functions": len(funcs),
               "module": "", "start": "", "end": "", "black_functions": 0, "twins": 0, "contiguous": "",
               "same_order": "", "overlaps": "", "fraction": 0.0}
        rows.append(row)
        votes = Counter(m for a, _, n in funcs for m in sorted({m for m, _ in twins[src, n]}))
        if not votes:
            continue
        mod = votes.most_common(1)[0][0]
        # Each twin implies where the Black file starts, weighted by size over copies so that a big unique function
        # outvotes a stub's many copies. The start with the most weight within 0x100 bytes wins; each B2 function
        # takes its copy nearest to it, and copies more than the B2 file's length away are dropped.
        cands = [(a - start, [x for m, x in twins[src, n] if m == mod], s) for a, s, n in funcs]
        cands = [(off, xs, s) for off, xs, s in cands if xs]
        votes = [(x - off, s / len(xs)) for off, xs, s in cands for x in xs]
        anchor = max(votes, key=lambda v: sum(w for t, w in votes if abs(t - v[0]) <= 0x100))[0]
        chosen = sorted((min(xs, key=lambda x: abs(x - off - anchor)), i) for i, (off, xs, _) in enumerate(cands))
        chosen = [(x, i) for x, i in chosen if abs(x - cands[i][0] - anchor) <= end - start]
        lo, last = chosen[0][0], chosen[-1][0]
        hi = last + next(s for a, s, _ in black_funcs[mod] if a == last)
        hi = min([(hi + 3) & ~3] + [a for a, _, _ in black_funcs[mod] if a >= hi])
        inside = [a for a, _, _ in black_funcs[mod] if lo <= a < hi]
        overlaps = [s for m, s, _, s0, s1 in black_files if m == mod and s0 < hi and lo < s1]
        row.update(module=mod, start=f"{lo:#010x}", end=f"{hi:#010x}", black_functions=len(inside),
                   twins=len(chosen), contiguous="yes" if len(inside) == len(chosen) else "no",
                   same_order="yes" if [i for _, i in chosen] == sorted(i for _, i in chosen) else "no",
                   overlaps=",".join(overlaps), fraction=len(chosen) / max(len(funcs), len(inside)))

    def kind(r):
        if not r["twins"]:
            return "none"
        if r["twins"] == r["bw2_functions"] == r["black_functions"] and r["same_order"] == "yes":
            return "full"
        return "partial"

    for r in rows:
        r["class"] = kind(r)
    rows.sort(key=lambda r: (r["class"] != "full", -r["fraction"], r["bw2_source"]))
    cols = ["class", "bw2_source", "bw2_complete", "bw2_functions", "module", "start", "end", "black_functions",
            "twins", "contiguous", "same_order", "overlaps"]
    with open(args.tsv, "w") as f:
        f.write("\t".join(cols) + "\n")
        for r in rows:
            f.write("\t".join(str(r[c]) for c in cols) + "\n")

    full = [r for r in rows if r["class"] == "full"]
    new = [r for r in full if not r["overlaps"]]
    size = lambda rs: sum(int(r["end"], 16) - int(r["start"], 16) for r in rs)
    print(f"{len(rows)} pokebw2 files with .text:", dict(Counter(r["class"] for r in rows)))
    print(f"full: {len(full)} files, {size(full)} bytes; {len(new)} files, {size(new)} bytes not yet in "
          f"Black's delinks; written to {args.tsv}")


if __name__ == "__main__":
    main()
