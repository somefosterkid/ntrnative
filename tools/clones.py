#!/usr/bin/env python3
"""Find Black's functions whose code a matching function of this repo has too ("clone twins"), and give their C.

Hashes every function of Black's delinked objects (build/b_us/delinks: the gaps and the original code of each file in
delinks.txt) as tools/twins.py does: ARM/Thumb mode and bytes, with the relocated fields zeroed. Functions with the
same hash are clones: the same C, with other callees or data. A clone that matches (in a file marked complete, or in
an incomplete file whose compiled object has the same code) gives its C to the others of its set, with the names of
what it calls and loads mapped to theirs from the relocations at the same offsets. The C is "checked" when every
relocation that differs is to a name the C spells out with the same addend; otherwise a macro, a field or a string
literal hides one and the C may call or load the wrong thing (compiler_probe.py catches that only for symbols it
finds in symbols.txt; port_file.py's check and ninja catch it all).

    python3 tools/clones.py                                # counts, and build/clones_b_us.tsv, one row a function
    python3 tools/clones.py --show func_ov021_0219a07c     # its clone's C with its names

Needs `ninja` to have run (the delinked objects, and the compiled ones of incomplete files). Reads objects, compiles
nothing: about 4 s, so rerun it after files complete. tools/local_driver.py tries a clone's C before asking the model.
"""
import argparse
import glob
import hashlib
import os
import re
import sys
from collections import Counter, defaultdict

import bwcache
from file_ranges import ROOT
from port_file import read_object
from twin_files import read_delinks
from twins import SHORT_RELOCS

CONFIG = f"{ROOT}/config/b_us/arm9"
DELINKS = f"{ROOT}/build/b_us/delinks"
TINY = 0x10  # bytes: clones under this are any getter or stub, the same C only by accident


def functions(path):
    """[(name, size, digest, [(offset, target, addend)])] of an object's functions, relocated fields zeroed. Read from
    build/cache/clones when the object (size and mtime) and this code are what they were; read_object is the cost."""
    token = (bwcache.stamp(path), bwcache.code_hash(__file__, read_object.__code__.co_filename,
                                                    sys.modules["twins"].__file__))
    out = bwcache.get("clones", path, token)
    if out is None:
        out = _functions(path)
        bwcache.put("clones", path, token, out)
    return out


def _functions(path):
    d, secs, syms, relocs = read_object(path)
    modes = {(s[1], s[2]): s[0][1] for s in syms if s[0] in ("$a", "$t")}
    out = []
    for name, ndx, value, size, _, is_func in syms:
        if not is_func or not size or name.startswith("$") or secs[ndx][1] != 1:
            continue
        off = secs[ndx][2]
        code = bytearray(d[off + value:off + value + size])
        refs = []
        for at, kind, sym, add in relocs.get(ndx, []):
            if value <= at < value + size:
                width = SHORT_RELOCS.get(kind, 4)
                code[at - value:at - value + width] = bytes(width)
                refs.append((at - value, syms[sym][0], add))
        mode = modes.get((ndx, value), "a")
        out.append((name, size, hashlib.sha1(mode.encode() + bytes(code)).digest(), sorted(refs)))
    return out


def symbol_addrs():
    """{(module, name): address} of Black's functions, from the symbols.txt files (build/cache/symbols while none of
    them changed)."""
    paths = sorted(glob.glob(f"{CONFIG}/**/symbols.txt", recursive=True))
    token = bwcache.files_token(paths, __file__)
    addr = bwcache.get("symbols", CONFIG, token)
    if addr is None:
        addr = {}
        for path in paths:
            mod = os.path.basename(os.path.dirname(path))
            mod = "main" if mod == "arm9" else mod
            for line in open(path):
                m = re.match(r"(\S+) kind:function\(.*\) addr:(0x\w+)", line)
                if m:
                    addr[mod, m[1]] = int(m[2], 16)
        bwcache.put("symbols", CONFIG, token, addr)
    return addr


def delinks_functions(paths):
    """{path: functions(path)} of the delinked objects, read as one file from build/cache while none of them changed
    (their sizes and mtimes): the most of black()'s time was a file read for each of 1,500 objects."""
    token = (tuple((p, bwcache.stamp(p)) for p in paths), bwcache.code_hash(__file__))
    out = bwcache.get("delinks_functions", DELINKS, token)
    if out is None:
        out = {p: functions(p) for p in paths}
        bwcache.put("delinks_functions", DELINKS, token, out)
    return out


def compiled_digests(path):
    """{function: digest} of a compiled object (an incomplete file's), cached by its size and mtime: the file the
    worker is writing changes every time, and this is all of it black() takes from it."""
    token = (bwcache.stamp(path), bwcache.code_hash(__file__, read_object.__code__.co_filename,
                                                    sys.modules["twins"].__file__))
    out = bwcache.get("compiled_digests", path, token)
    if out is None:
        out = {n: h for n, _, h, _ in _functions(path)}
        bwcache.put("compiled_digests", path, token, out)
    return out


def black():
    """Every function of Black: [(module, name, address, size, source or None, matches, digest, relocations)]."""
    addr = symbol_addrs()
    files = {os.path.splitext(src)[0]: (src, mod, done) for mod, src, done, _, _ in read_delinks(CONFIG)}
    out, seen, used = [], set(), []
    # the files' objects first: a gap's object can be older than a file taken out of it
    for path in sorted(glob.glob(f"{DELINKS}/**/*.o", recursive=True), key=lambda p: ("_dsd_gap@" in p, p)):
        rel = path[len(DELINKS) + 1:-2]
        gap = re.match(r"_dsd_gap@(.*)_\d+$", rel)
        if gap:
            used.append((path, rel, None, gap[1], False))
        elif rel in files:
            used.append((path, rel) + files[rel])
    static = delinks_functions([u[0] for u in used])
    for path, rel, src, mod, done in used:
        ours = {}
        if src and not done and os.path.exists(f"{ROOT}/build/b_us/{rel}.o"):  # an incomplete file's functions
            ours = compiled_digests(f"{ROOT}/build/b_us/{rel}.o")
        for name, size, h, refs in static[path]:
            if (mod, name) not in seen:
                seen.add((mod, name))
                out.append((mod, name, addr.get((mod, name)), size, src, done or ours.get(name) == h, h, refs))
    return out


def clones(rows):
    """{(module, name): (its row, the matching clone's row)} for every function that doesn't match and has a clone
    that does: the one in the same module if any."""
    sets = defaultdict(list)
    for r in rows:
        sets[r[6]].append(r)
    out = {}
    for members in sets.values():
        done = sorted((r for r in members if r[5]), key=lambda c: (c[0], c[2] or 0))
        for r in members:
            if done and not r[5]:
                out[r[0], r[1]] = r, next((c for c in done if c[0] == r[0]), done[0])
    return out


def definition(text, name):
    """A function's definition in C text, through its closing brace; None if the text has none."""
    m = re.search(rf"^(?![ \t#/])[^;{{}}\n]*\b{name}\s*\([^;{{}}]*\)\s*\{{", text, re.M)
    end = text.find("\n}", m.end()) if m else -1
    return text[m.start():end + 2] if end >= 0 else None


def reference(row, clone):
    """The clone's C with the function's names: (C, notes); notes empty when the mapping is checked."""
    text = open(f"{ROOT}/{clone[4]}", errors="replace").read()
    c = definition(text, clone[1])
    if c is None:
        return None, [f"{clone[1]} not found in {clone[4]}"]
    names, notes = {clone[1]: row[1]}, []
    if [o for o, _, _ in clone[7]] != [o for o, _, _ in row[7]]:
        notes.append("the relocations are at other offsets")
    for (o1, t1, a1), (o2, t2, a2) in zip(clone[7], row[7]):
        if t1 == t2 and a1 == a2:
            continue
        if a1 != a2 or names.setdefault(t1, t2) != t2:
            notes.append(f"+{o1:#x}: {t1}{a1:+#x} there, {t2}{a2:+#x} here")
        elif not re.search(rf"\b{re.escape(t1)}\b", c):
            notes.append(f"+{o1:#x}: {t1} there, {t2} here, but the C doesn't name {t1}")
    names = {k: v for k, v in names.items() if re.match(r"[A-Za-z_]\w*$", k) and re.match(r"[A-Za-z_]\w*$", v)}
    c = re.sub(r"\b(" + "|".join(map(re.escape, names)) + r")\b", lambda m: names[m[0]], c)
    return c + "\n", notes


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--tsv", default=f"{ROOT}/build/clones_b_us.tsv")
    ap.add_argument("--twins", default=f"{ROOT}/build/twins_b_us.tsv", help="pokebw2's twins, from tools/twins.py")
    ap.add_argument("--show", metavar="FUNCTION", help="print the C of a function's clone with its names, and exit")
    args = ap.parse_args()

    rows = black()
    found = clones(rows)
    if args.show:
        shown = [v for k, v in found.items() if k[1] == args.show]
        if not shown:
            sys.exit(f"{args.show}: no matching clone" + ("" if any(r[1] == args.show for r in rows) else
                                                           " (no such function)"))
        for row, clone in shown:
            c, notes = reference(row, clone)
            print(f"// {args.show}: the same code as {clone[1]} in {clone[4]}" +
                  ("; check these, which the names don't map:\n// " + "\n// ".join(notes) if notes else
                   ", names mapped"))
            print(c or "", end="")
        return 0

    twinned = {(l.split("\t")[1], l.split("\t")[0]) for l in list(open(args.twins))[1:]} \
        if os.path.exists(args.twins) else set()
    sets = Counter(r[6] for r in rows)
    complete = {src for _, src, done, _, _ in read_delinks(CONFIG) if done}
    print(f"Black: {len(rows)} functions, {sum(n for n in sets.values() if n > 1)} of them in "
          f"{sum(1 for n in sets.values() if n > 1)} sets of clones; {sum(1 for r in rows if r[5])} match, "
          f"{sum(1 for r in rows if r[5] and r[4] not in complete)} of them in incomplete files")
    out = []
    for r, clone in found.values():
        c, notes = reference(r, clone)
        out.append((r, clone, "checked" if c and not notes else "check"))
    for label, part in [("all", out), (f">= {TINY} bytes", [x for x in out if x[0][3] >= TINY])]:
        new = [x for x in part if (x[0][0], x[0][1]) not in twinned]
        print(f"{label}: {len(part)} functions ({sum(x[0][3] for x in part)} bytes) have a matching clone, "
              f"{sum(1 for x in part if x[2] == 'checked')} with checked names; "
              f"{len(new)} ({sum(x[0][3] for x in new)} bytes) have no pokebw2 twin; "
              f"{sum(1 for x in part if x[0][4])} are in files of delinks.txt")
    with open(args.tsv, "w") as f:
        f.write("black_symbol\tmodule\taddress\tsize\tfile\tclone\tclone_module\tclone_file\tnames\tpokebw2_twin\n")
        for r, c, names in sorted(out, key=lambda x: (x[0][0], x[0][2] or 0)):
            f.write(f"{r[1]}\t{r[0]}\t{r[2] or 0:#010x}\t{r[3]}\t{r[4] or '-'}\t{c[1]}\t{c[0]}\t{c[4]}\t{names}\t"
                    f"{'yes' if (r[0], r[1]) in twinned else 'no'}\n")
    print(f"written to {args.tsv}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
