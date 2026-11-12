#!/usr/bin/env python3
"""Name the functions and data Black's twins call or load, from the symbols the same calls name in pokebw2.

A twin (build/twins_b_us.tsv, from tools/twins.py) has the same code in Black and in pokebw2, so its relocations sit
at the same offsets: Black's target there (relocs.txt, symbols.txt) is the symbol pokebw2's compiled object names
(compiled into --work, the cache twins.py uses). Each such pair is a vote, from twins that are surely the same function:
their code tells their targets (all functions with that code, in pokebw2 and in Black, have the same ones), their
relocations agree in kind, and no clear majority outvotes them at any target. A Black symbol gets the name when
- it is still a dsd placeholder (func_/data_) of the same kind as pokebw2's symbol,
- every vote for it names the same pokebw2 symbol (a label on a function counts as the function, and is added too),
- the name is swan's (in pokebw2's symbols.txt, not in its config/names.txt), or a public SDK name (with an SDK
  prefix, in the headers of pokebw2's lib/nitro, lib/nnsys, lib/twl) or one of MWCC's runtime functions,
- and the name lands on no other Black symbol, here or already in symbols.txt.
  (--apply also skips a name that a C file using the placeholder already defines.)

    python3 tools/callee_names.py            # prints the counts, writes build/callee_names.tsv (every symbol voted on)
    python3 tools/callee_names.py --apply    # also renames them in config/, src/, lib/ and include/

Calls into "overlays(a,b)" (dsd can't tell which overlay) and data loads with an addend are not used. Twins in files
already in C are not in Black's delinked objects, so they give no votes.
"""
import argparse
import bisect
import csv
import glob
import os
import re
import struct
from collections import Counter, defaultdict

import twins

RUNTIME = re.compile(r"(__aeabi_\w+|_(s32|u32|ll|ull)_\w+|_[fd][a-z0-9]{1,5})$")  # MWCC's
SDK = re.compile(r"[A-Z]{2,}i?_")  # NNS_, OSi_, FX_, SDK_...
PLACEHOLDER = re.compile(r"(func|data)_|\.|.*(?=[0-9a-f]{0,6}\d)[0-9a-f]{7}")  # dsd's, or with an address
ABS32 = 2


def relocations(path):
    """Returns {function: [(offset, type, symbol, addend)]} of a relocatable ARM ELF's .text relocations."""
    d = open(path, "rb").read()
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", d, 0x2E)
    sh = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    syms, funcs, rels = [], defaultdict(list), defaultdict(list)
    for s in sh:
        if s[1] == 2:  # SYMTAB
            strtab = sh[s[6]][4]
            for i in range(s[5] // 16):
                name, value, size, info, _, ndx = struct.unpack_from("<IIIBBH", d, s[4] + 16 * i)
                name = d[strtab + name:d.index(b"\0", strtab + name)].decode()
                syms.append(name)
                if info & 15 == 2 and size:
                    funcs[ndx].append((value, size, name))
    for s in sh:
        if s[1] in (4, 9):  # RELA, REL
            for o in range(s[4], s[4] + s[5], 12 if s[1] == 4 else 8):
                off, info = struct.unpack_from("<II", d, o)
                add = struct.unpack_from("<i", d, o + 8)[0] if s[1] == 4 else 0
                rels[s[7]].append((off, info & 255, syms[info >> 8], add))
    out = {}
    for ndx, fs in funcs.items():
        for value, size, name in fs:
            out[name] = [(off - value, t, sym, add) for off, t, sym, add in rels[ndx] if value <= off < value + size]
    return out


def module_of(path):
    mod = os.path.basename(os.path.dirname(path))
    return "main" if mod == "arm9" else mod


def read_symbols(root):
    """Returns {(module, address): [(name, kind)]} of a symbols.txt tree."""
    out = defaultdict(list)
    for path in glob.glob(f"{root}/**/symbols.txt", recursive=True):
        for line in open(path):
            m = re.match(r"(\S+) kind:(\w+).* addr:(0x\w+)", line)
            if m:
                out[module_of(path), int(m[3], 16)].append((m[1], m[2]))
    return out


def read_relocs(root):
    """Returns {module: [(from, to, addend, target module or None, kind)]} sorted by address."""
    out = {}
    for path in glob.glob(f"{root}/**/relocs.txt", recursive=True):
        rows = []
        for line in open(path):
            f = dict(t.split(":", 1) for t in line.split())
            if "to" not in f:  # link_time_const
                continue
            mod = f.get("module", "")
            m = re.match(r"(\w+)\((\d+)\)$", mod)
            if m and m[1] == "overlay":
                mod = f"ov{int(m[2]):03d}"
            elif m:
                mod = f"{m[1]}_{m[2]}"
            elif mod not in ("main", "itcm", "dtcm"):
                mod = None  # overlays(a,b,...) or none
            rows.append((int(f["from"], 16), int(f["to"], 16), int(f.get("add", "0"), 16), mod, f["kind"]))
        out[module_of(path)] = sorted(rows)
    return out


def main():
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--bw2", default=os.path.expanduser("~/ref/pokebw2"), help="pokebw2 checkout (read only)")
    ap.add_argument("--tools", default=f"{root}/tools", help="directory with wibo and mwccarm/")
    ap.add_argument("--work", default=f"{root}/build/twins_work", help="cache of the compiled objects")
    ap.add_argument("--twins", default=f"{root}/build/twins_b_us.tsv")
    ap.add_argument("--delinks", default=f"{root}/build/b_us/delinks", help="Black's delinked objects")
    ap.add_argument("--config", default=f"{root}/config/b_us/arm9", help="Black's symbols.txt and relocs.txt tree")
    ap.add_argument("--tsv", default=f"{root}/build/callee_names.tsv", help="the names found, one row each")
    ap.add_argument("--apply", action="store_true", help="rename in config/, src/, lib/ and include/")
    ap.add_argument("--extra", action="append", metavar="COMPILER:FLAGS:DIRS",
                    help="twins.py's --extra the table was made with (default: dsi/1.2:-nothumb:src/gfl)")
    ap.add_argument("-j", type=int, default=os.cpu_count())
    args = ap.parse_args()

    # pokebw2's objects: the game code with each compiler that found a twin first, the libraries with their own
    rows = list(csv.DictReader(open(args.twins), delimiter="\t"))
    # (a library's twin built as Black builds its libraries is of that build: twins.py --black-libs)
    first = lambda r: r["compilers"].split(",")[0]
    tag = lambda r: first(r) if first(r) in twins.BLACK_LIB else "lib" if r["bw2_source"].startswith("lib/") else first(r)
    tags = {tag(r) for r in rows} - {"lib"} - set(twins.BLACK_LIB)
    extras = [e for e in map(twins.parse_extra, args.extra or ["dsi/1.2:-nothumb:src/gfl"]) if e[0] in tags]
    black_libs = root if any(tag(r) in twins.BLACK_LIB for r in rows) else None
    jobs = twins.compile_jobs(args.bw2, args.tools, args.work, sorted(tags - {e[0] for e in extras}), extras,
                              black_libs)
    failed, _ = twins.compile_all(args.bw2, jobs, args.j)
    print(f"{len(jobs)} pokebw2 objects, {len(failed)} failed to compile")

    # A twin is used only if its code alone tells its targets: every pokebw2 function and every Black function with
    # the same code (relocations masked) has the same targets. A wrapper that calls something else, or a getter of
    # another struct, would otherwise pair with whichever one twins.py found first.
    b2_relocs, b2_targets = {}, defaultdict(set)
    for t, src, obj, _, _ in jobs:
        if os.path.exists(obj):
            rel = b2_relocs[t, src[len(args.bw2) + 1:]] = relocations(obj)
            for name, _, _, _, h in twins.functions(obj):
                b2_targets[h].add(tuple((off, sym, add) for off, _, sym, add in rel[name]))
    syms = read_symbols(args.config)
    relocs = read_relocs(args.config)
    starts = {mod: [r[0] for r in rs] for mod, rs in relocs.items()}

    def black_relocs(mod, start, size):
        """Returns {offset: (to, addend, target module, kind)} of the relocations in a Black function."""
        i = bisect.bisect_left(starts.get(mod, []), start)
        j = bisect.bisect_left(starts.get(mod, []), start + size)
        return {fr - start: tuple(rest) for fr, *rest in relocs.get(mod, [])[i:j]}

    black_targets, black_hash = defaultdict(set), {}
    for mod, name, addr, size, _, h in twins.black_functions(args.delinks, args.config)[0]:
        if addr is not None:
            black_targets[h].add(tuple(sorted(black_relocs(mod, addr, size).items())))
            black_hash[mod, addr] = h

    # pokebw2's names: swan's, pokebw2's own (names.txt), and the public SDK's (its library headers)
    b2_syms = read_symbols(f"{args.bw2}/config/b2_us/arm9")
    b2_name = {n: (key, kind) for key, syms in b2_syms.items() for n, kind in syms}
    guesses = {line.split()[2] for line in open(f"{args.bw2}/config/names.txt") if line[0] != "#" and line.strip()}
    sdk = set()
    for lib in ("nitro", "nnsys", "twl"):
        for path in glob.glob(f"{args.bw2}/lib/{lib}/include/**/*.h", recursive=True):
            sdk |= set(re.findall(r"\w+", open(path, errors="replace").read()))

    def allowed(name):
        if name not in b2_name or PLACEHOLDER.match(name):
            return False
        return name not in guesses or name in sdk and SDK.match(name) or RUNTIME.match(name)

    # Each twin's relocations, paired with Black's by offset. A twin whose relocations differ in kind (a call against
    # a load, a function against data) is two other functions that happen to share their code, and gives no votes.
    def kind_class(kind):
        return "data" if kind in ("data", "bss") else "function"  # labels mark code

    def b2_class(name):
        if re.match(r"OVERLAY_\d+_ID$", name):
            return "overlay ID"  # a constant in Black's relocs.txt, never a relocation
        return name in b2_name and kind_class(b2_name[name][1])

    twin_pairs = {}  # (module, Black function) -> [((module, address), pokebw2 name)]
    stats = Counter()
    for r in rows:
        b2 = b2_relocs.get((tag(r), r["bw2_source"]), {}).get(r["bw2_function"])
        h = black_hash.get((r["module"], int(r["address"], 16)))
        if b2 is None or h is None:
            stats["twins not found"] += 1
            continue
        if len(b2_targets[h]) != 1 or len(black_targets[h]) != 1:
            stats["twins whose code doesn't tell their targets"] += 1
            continue
        black = black_relocs(r["module"], int(r["address"], 16), int(r["size"]))
        if black.keys() != {off for off, *_ in b2}:
            stats["twins with relocations at other offsets"] += 1
            continue
        pairs, unlike = [], False
        for off, typ, name, b2_add in b2:
            to, add, mod, kind = black[off]
            unlike |= (kind == "load") != (typ == ABS32)
            if mod is None or typ == ABS32 and (b2_add or add not in (0, 1)):
                continue  # in an unknown overlay, or a field of the data
            if (mod, to) not in syms and to & 1:  # a Thumb function's address
                to -= 1
            if (mod, to) in syms and name[0] not in ".@":  # not a section or a string
                black_kind = next((k for _, k in syms[mod, to] if k != "label"), "label")
                unlike |= b2_class(name) not in (False, kind_class(black_kind))
                pairs.append(((mod, to), name))
        if unlike:
            stats["twins with relocations of other kinds"] += 1
        else:
            twin_pairs[r["module"], r["black_symbol"]] = pairs

    # Votes. A twin outvoted at one of its targets (or sending a name where the other votes for it don't) is two
    # other functions with the same code too, and loses all its votes; without a clear majority, every twin there does.
    def losers(groups):
        """Returns the twins of all but a clear majority of groups ({label: {twin}}), or of all if there is none."""
        top = max(groups.values(), key=len)
        rest = set().union(*(t for t in groups.values() if t is not top))
        return rest if len(top) >= 3 and len(top) >= 2 * len(rest) else rest | top

    outvoted = set()
    while True:
        votes = defaultdict(lambda: defaultdict(set))  # (module, address) -> {pokebw2 name: {twin}}
        for twin, pairs in twin_pairs.items():
            for key, name in pairs:
                if twin not in outvoted:
                    votes[key][name].add(twin)
        lost = set()
        by_id = defaultdict(lambda: defaultdict(set))  # pokebw2 symbol -> {(module, address): {twin}}
        for key, names in votes.items():
            ids = defaultdict(set)
            for name, t in names.items():
                ids[b2_name[name][0] if name in b2_name else name] |= t
            for i, t in ids.items():
                by_id[i][key] |= t
            if len(ids) > 1:
                lost |= losers(ids)
        for keys in by_id.values():
            if len(keys) > 1:
                lost |= losers(keys)
        if lost <= outvoted:
            break
        outvoted |= lost
    stats["twins outvoted"] = len(outvoted)
    print(f"{len(rows)} twins, of which not used:", ", ".join(f"{v} {k}" for k, v in stats.items()))

    # Resolve, one row per Black symbol voted on. The name is pokebw2's symbol of the same kind at the address the
    # votes name; votes for a label there (MWCC's runtime: _dadd on __aeabi_dadd) add that label to Black's too.
    taken = {n for s in syms.values() for n, _ in s}
    result = {}  # (module, address) -> [Black name, kind, name, labels, votes, outcome]
    for key, names in votes.items():
        black_name, black_kind = next((s for s in syms[key] if s[1] != "label"), syms[key][0])
        ids = {b2_name[n][0] if n in b2_name else n for n in names}
        at = b2_syms.get(next(iter(ids)), []) if black_kind != "label" else []
        name = next((n for n, k in at if k != "label" and kind_class(k) == kind_class(black_kind)), "")
        labels = sorted(n for n in names if b2_name.get(n, (0, ""))[1] == "label" and n not in taken)
        if len(ids) > 1:
            outcome = "votes disagree"
        elif not name:
            outcome = "pokebw2's symbol is of another kind or unknown"
        elif not all(map(allowed, [name] + labels)):
            outcome = "not ours to use"
        elif black_name == name and not labels:
            outcome = "already so named"
        elif black_name != name and not re.match(r"(func|data)_", black_name):
            outcome = "already named otherwise"
        else:
            outcome = "named"
        shown = ",".join(f"{n}:{len(t)}" for n, t in sorted(names.items(), key=lambda nt: -len(nt[1])))
        result[key] = [black_name, black_kind, name, ",".join(labels), shown, outcome]
    count = Counter(r[2] for r in result.values() if r[5] == "named")
    for r in result.values():
        if r[5] == "named" and r[0] != r[2] and (count[r[2]] > 1 or r[2] in taken):
            r[5] = "name on two Black symbols"
    with open(args.tsv, "w") as f:
        f.write("module\taddress\tblack_symbol\tkind\tname\tlabels\tvotes\toutcome\n")
        for (mod, addr), r in sorted(result.items()):
            f.write(f"{mod}\t{addr:#010x}\t" + "\t".join(r) + "\n")
    named = {key: r for key, r in result.items() if r[5] == "named"}
    print(f"{len(result)} Black symbols voted on:", dict(Counter(r[5] for r in result.values())), "; named",
          dict(Counter(kind_class(r[1]) for r in named.values())), "and",
          sum(bool(r[3]) for r in named.values()), "labels added")
    if args.apply:
        apply(named, root, args.config)


def apply(named, root, config):
    """Renames Black's symbols as named says ({(module, address): [Black name, kind, name, labels, ...]}) in config/,
    src/, lib/ and include/, adding the labels to symbols.txt."""
    rename = {r[0]: r[2] for r in named.values()}
    word = re.compile(r"\b(?:func|data)_\w+")
    paths = glob.glob(f"{config}/**/symbols.txt", recursive=True)
    for d in ("src", "lib", "include"):
        paths += [p for p in glob.glob(f"{root}/{d}/**/*", recursive=True) if re.search(r"\.(c|h|inc|s)$", p)]
    # A C file that uses the placeholder and already defines the name (for another function) can't take it: the
    # rename would give the file two definitions of it.
    for path in paths:
        if not path.endswith(".c"):
            continue
        text = open(path).read()
        for old in sorted(set(word.findall(text)) & rename.keys()):
            new = rename[old]
            if re.search(rf"^[A-Za-z_][^;{{}}#]*?\b{new}\s*\([^;{{}}]*\)\s*\{{", text, re.M) or \
                    re.search(rf"^(?!extern)[A-Za-z_][^;(){{}}\n]*\b{new}\b[^;(){{}}\n]*[=;\[]", text, re.M):
                print(f"not renaming {old} to {new}: {os.path.relpath(path, root)} already defines {new}")
                del rename[old]
    changed = 0
    for path in paths:
        text = open(path).read()
        new = word.sub(lambda m: rename.get(m[0], m[0]), text)
        for (mod, addr), r in named.items():
            if r[3] and r[0] in rename and path.endswith("symbols.txt") and module_of(path) == mod:
                line = re.search(rf"^{r[2]} kind:\w+\((\w+).*\n", new, re.M)
                labels = "".join(f"{label} kind:label({line[1]}) addr:{addr:#010x}\n" for label in r[3].split(","))
                new = new[:line.end()] + labels + new[line.end():]
        if new != text:
            open(path, "w").write(new)
            changed += 1
    print(f"renamed {len(rename)} symbols in {changed} files")


if __name__ == "__main__":
    main()
