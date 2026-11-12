#!/usr/bin/env python3
"""What a port may have changed: each function's tables against the original's, each macro against pokebw2's text.

    python3 tools/port_audit.py                    # every file tools/port_file.py ported (its first line says so)
    python3 tools/port_audit.py lib/nitrowifi/src/ssl/md5.c ...
    python3 tools/port_audit.py --open             # only the functions open in docs/nonmatching.md (and their files)
    python3 tools/port_audit.py --build            # `ninja` the files' objects first (else the build's, maybe stale)

Wave 29 found two ways a ported file's C had left its source: md5.c read the padding table where the original's pool
loads the word index (port_file.py had swapped the two tables' definitions without their comments, and a later
version followed the comments), and md5's and sha1's round macros were expressions where pokebw2 has statements,
which changes the order of the adds (pokebw2 rewrote them after the port: a port copies the pokebw2 of its day).

Tables: the data each function's constant pools load (R_ARM_ABS32 relocations in .text), in the compiled object
(build/b_us/<file>.o) and in dsd's delinked original (build/b_us/delinks/<file>.o), each target taken to the symbol of
Black's symbols.txt it lands in (a symbol by its name, an unnamed one, a string, where the link would place it). A
datum one side loads and the other doesn't is a row:

    lib/nitrowifi/src/ssl/md5.c func_ov009_02162ff0 (open): loads data_ov009_021a1de8 where the original loads
        data_ov009_021a1da8

A function that matches can't differ (tools/compiler_probe.py compares the relocations' symbols); an open one that
does reads another table than the original, or the same one at another offset (said so: `another offset`), or a
string that lands elsewhere (strings are left out while the file's data isn't where Black has it).

Macros: each function-like #define of the file against pokebw2's current text of the source the first line names, the
parameters by position and other names through renames (consistent within the macro): a statement-form macro
(`{ ... }`, `do ... while`) that the file has as an expression, or another order of the terms, is a row with
pokebw2's text. Also pokebw2's function-like macros that neither the file nor a header of this repo defines (pokebw2
wrote new ones since), and the file's that pokebw2 no longer has.
"""
import argparse
import bisect
import os
import re
import subprocess
import sys
import types
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from file_ranges import ROOT, read_delinks  # noqa: E402
from port_file import CONFIG, module_bytes, place, read_names, read_object  # noqa: E402

BS = chr(92)
HEADER = re.compile(r"^// From pokebw2 \([^)]*\), (\S+), ported by tools/port_file\.py\.", re.M)
ROW = re.compile(r"^\| `(\w+)` \| `0x[0-9a-f]+` \| `([^`]+)` \|", re.M)
ABS32 = 2
KEYWORDS = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
            "float", "for", "goto", "if", "inline", "int", "long", "register", "return", "short", "signed", "sizeof",
            "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "u8", "u16",
            "u32", "u64", "s8", "s16", "s32", "s64", "vu8", "vu16", "vu32", "BOOL", "TRUE", "FALSE", "NULL", "fx32",
            "fx16"}


def ported_files(roots=("src", "lib")):
    """{file: pokebw2's source} of the C files whose first line says tools/port_file.py ported them."""
    out = {}
    for root in roots:
        for d, _, names in os.walk(os.path.join(ROOT, root)):
            for n in names:
                if n.endswith(".c"):
                    path = os.path.join(d, n)
                    with open(path, errors="replace") as f:
                        m = HEADER.match(f.read(400))
                    if m:
                        out[os.path.relpath(path, ROOT)] = m[1]
    return dict(sorted(out.items()))


def open_rows(path=None):
    """{(file, function)} of docs/nonmatching.md's rows."""
    text = open(path or os.path.join(ROOT, "docs", "nonmatching.md")).read()
    return {(f, n) for n, f in ROW.findall(text)}


# --- Tables -------------------------------------------------------------------------------------------------------

class Names:
    """Black's symbols (symbols.txt, not labels): a name's address and the symbol an address lands in. A label's
    name has its address too (_u32_div_f is a label at __aeabi_uidivmod: one function), but no datum starts at it."""

    def __init__(self, config=CONFIG):
        self.at, self.where = read_names(config)
        self.labels = read_names(config, labels=True)[1]
        self.sorted = defaultdict(list)
        for (mod, a) in self.at:
            self.sorted[mod].append(a)
        for v in self.sorted.values():
            v.sort()

    def locate(self, name, mod):
        """(module, address) of a symbols.txt name, mod's first, then main's; None if no module has it."""
        places = self.where.get(name) or self.labels.get(name)
        if not places:
            return None
        return next((p for p in places if p[0] == mod), None) or next((p for p in places if p[0] == "main"),
                                                                     None) or places[0]

    def datum(self, mod, addr):
        """(name, offset) of the symbol of mod that addr lands in, or None before the first."""
        addrs = self.sorted.get(mod) or []
        i = bisect.bisect_right(addrs, addr) - 1
        if i < 0:
            return None
        return self.at[mod, addrs[i]], addr - addrs[i]


class Loads:
    """What each function of an object loads from its literal pools: {function: {key: (name, module, address, symbol,
    anonymous)}}, the key the start of the datum (symbols.txt's) the target lands in, without the Thumb bit, so that
    two names of one address are one load (an alias, dsd's name of a function pointer from another overlay), as
    tools/compiler_probe.py compares them. A named target goes by its name, an unnamed one (a string, a section) by
    where the link places it; symbol: the index of the object's own symbol (its bytes), anonymous: unnamed. Also
    {function: the unrelocated words of its pools} (dsd leaves some addresses unrelocated), and the linker's
    OVERLAY_<n>_ID words (a number in the original's) are left out."""

    def __init__(self, path, ranges, mod, names):
        d, secs, syms, relocs = self.object = read_object(path)
        at = place(secs, ranges)
        self.at = at
        rel = not any(s[1] == 4 for s in secs)  # REL: the addend is the word in place
        funcs, marks, holders = defaultdict(list), defaultdict(list), defaultdict(dict)
        for si, (n, ndx, v, size, _, func) in enumerate(syms):
            if func and size and ndx in at and secs[ndx][0] == ".text" and n[:1] != "$":
                funcs[ndx].append((v, v + size, n))
            if n in ("$a", "$t", "$d"):
                marks[ndx].append((v, n))
            elif not func and size and n and n[0] not in ".$@" and ndx in at:
                holders[ndx].update((v + k, (si, k)) for k in range(size))
        self.loads = {n: {} for v in funcs.values() for _, _, n in v}
        self.words = {n: set() for n in self.loads}
        self.slots = {n: {} for n in self.loads}  # {function: {offset in it: key}}: the pool words one by one
        for sec, spans in funcs.items():
            spans.sort()
            starts = [s[0] for s in spans]
            entries = relocs.get(sec, [])
            relocated = {e[0] for e in entries}
            for off, kind, si, add in entries:
                i = bisect.bisect_right(starts, off) - 1
                if kind != ABS32 or i < 0 or off >= spans[i][1]:
                    continue
                if rel:
                    add = int.from_bytes(d[secs[sec][2] + off:secs[sec][2] + off + 4], "little", signed=True)
                name, ndx, value = syms[si][0], syms[si][1], syms[si][2]
                if re.fullmatch(r"OVERLAY_\d+_ID", name):
                    continue
                if (not name or name[0] in ".$@") and ndx in at:  # a section's symbol: the object's symbol there
                    inner = holders.get(ndx, {}).get(value + add)
                    if inner is not None:
                        si, add = inner
                        name, ndx, value = syms[si][0], syms[si][1], syms[si][2]
                named = bool(name) and name[0] not in ".$@" and names.locate(name, mod)
                loc = named or ((mod, at[ndx] + value) if ndx in at else None)
                own = si if ndx in at and syms[si][3] and not syms[si][5] else None
                if not loc:
                    self.loads[spans[i][2]][name or "?"] = (name or "?", None, None, own, False, None)
                    self.slots[spans[i][2]][off - spans[i][0]] = name or "?"
                    continue
                addr = loc[1] + add
                found = names.datum(loc[0], addr)
                start = addr - found[1] if found else addr
                shown = name if own is not None and name and name[0] not in ".$@" else \
                    found[0] if found else f"{addr:#010x}"
                there = None  # where the link puts our own symbol's target: its datum's key
                if own is not None and ndx in at:
                    landed = names.datum(mod, at[ndx] + value + add)
                    there = (at[ndx] + value + add - (landed[1] if landed else 0)) & ~1
                self.loads[spans[i][2]][start & ~1] = (shown, loc[0], addr, own, not named, there)
                self.slots[spans[i][2]][off - spans[i][0]] = start & ~1
            # the words of the pools that no relocation names: the data between a $d and the next $a/$t
            kinds = sorted(marks.get(sec, []))
            for j, (o, k) in enumerate(kinds):
                if k != "$d":
                    continue
                end = kinds[j + 1][0] if j + 1 < len(kinds) else secs[sec][3]
                for w in range(o + (-o % 4), end - 3, 4):
                    i = bisect.bisect_right(starts, w) - 1
                    if i >= 0 and w < spans[i][1] and w not in relocated:
                        self.words[spans[i][2]].add(int.from_bytes(d[secs[sec][2] + w:secs[sec][2] + w + 4],
                                                                   "little"))

    def symbol_bytes(self, si):
        """(bytes, offsets that a relocation fills) of the object's own symbol si; None for .bss."""
        d, secs, syms, relocs = self.object
        _, ndx, value, size, _, _ = syms[si]
        if secs[ndx][1] != 1:
            return None
        off = secs[ndx][2] + value
        mask = {o - value + k for o, *_ in relocs.get(ndx, ()) if value <= o < value + size for k in range(4)}
        return d[off:off + size], mask


class Rom:
    """The ROM's bytes of each module (tools/port_file.py's module_bytes)."""

    def __init__(self, delinks):
        self.r, self.cache = types.SimpleNamespace(delinks=delinks), {}

    def at(self, mod, addr, size):
        if mod not in self.cache:
            try:
                self.cache[mod] = module_bytes(self.r, mod)
            except (OSError, KeyError):
                self.cache[mod] = (b"", 0)
        data, base = self.cache[mod]
        return data[addr - base:addr - base + size] if 0 <= addr - base else b""


def compare(ours, theirs, same_bytes=None):
    """What one function's pools load otherwise than the original's: ours (its Loads entry, {key: size of our own
    symbol}, its unrelocated pool words[, its slots]) and theirs (its Loads entry, its unrelocated pool words[, its
    slots]). With the relocations at the same offsets of both (the function's layout is the original's) the words
    pair one by one (a function that matches reads its tables in place of the original's: tools/compiler_probe.py
    doesn't compare a static's relocation, it has no global name), else as sets. Returns (ours only
    by name, ours only by place, theirs only, another offset, same bytes): names, names, names, [(ours, theirs)],
    [(ours, theirs)]. By place: a datum of ours that no name of symbols.txt tells (a string, pokebw2's name of a
    static), where the link would put it, which is Black's place only where the file's data is in Black's order.
    Another offset: a datum of ours that the original's target lies inside; same bytes: a datum of the object's whose
    bytes are the ROM's at the original's (same_bytes(ours entry, theirs entry) says so), one table under two names (a
    string placed elsewhere, twin tables that carry each other's names). A target one side has as a relocation and the
    other as a plain word of its pool (dsd relocates some numbers, leaves some addresses) is no difference."""
    (mine_loads, mine_sizes, mine_words, *mine_slots), (orig_loads, words, *orig_slots) = ours, theirs
    shifted, same = [], []

    def inside(k, o):  # the original's target lies in our datum k
        size = mine_sizes.get(k)
        return bool(size) and isinstance(k, int) and orig_loads[o][2] is not None and k <= orig_loads[o][2] < k + size

    def word(loads, k, other):  # a target the other side has as a plain word of its pool
        addr = loads[k][2]
        return addr is not None and bool({addr, addr | 1, addr & ~1} & other)

    if mine_slots and orig_slots and mine_slots[0] and set(mine_slots[0]) == set(orig_slots[0]):
        # the pools have their relocations at the same offsets (the function's layout is the original's): word by word
        mine, orig, placed, seen = [], [], [], set()
        for off in sorted(mine_slots[0]):
            k, o = mine_slots[0][off], orig_slots[0][off]
            if k == o or (k, o) in seen:
                continue
            seen.add((k, o))
            if inside(k, o):
                shifted.append((mine_loads[k][0], orig_loads[o][0]))
            elif mine_loads[k][5] == o or same_bytes and same_bytes(mine_loads[k], orig_loads[o]):
                same.append((mine_loads[k][0], orig_loads[o][0]))
            elif mine_loads[k][4]:
                placed.append(f"{mine_loads[k][0]} (the original's {orig_loads[o][0]})")
            else:
                mine.append(mine_loads[k][0])
                orig.append(orig_loads[o][0])
        return mine, placed, orig, shifted, same
    mine = {k for k in mine_loads if k not in orig_loads}
    orig = {k for k in orig_loads if k not in mine_loads}
    for side, loads, other in ((mine, mine_loads, words), (orig, orig_loads, mine_words)):
        for k in list(side):
            if word(loads, k, other):
                side.discard(k)
    for k in sorted(mine, key=str):
        hit = next((o for o in sorted(orig, key=str) if inside(k, o)), None)
        if hit is not None:
            shifted.append((mine_loads[k][0], orig_loads[hit][0]))
        else:
            hit = next((o for o in sorted(orig, key=str) if mine_loads[k][5] == o or
                        same_bytes and same_bytes(mine_loads[k], orig_loads[o])), None)
            if hit is not None:
                same.append((mine_loads[k][0], orig_loads[hit][0]))
        if hit is not None:
            mine.discard(k)
            orig.discard(hit)
    placed = {k for k in mine if mine_loads[k][4]}
    return (sorted(mine_loads[k][0] for k in mine - placed), sorted(mine_loads[k][0] for k in placed),
            sorted(orig_loads[k][0] for k in orig), shifted, same)


def table_rows(dest, mod, ranges, names, rom, only=None):
    """[(function, message, counts)] for one file; only: these functions (default all). counts: whether the C reads
    another table (not only another offset of one, or the same bytes under another name)."""
    obj = os.path.join(ROOT, "build", "b_us", os.path.splitext(dest)[0] + ".o")
    orig = os.path.join(ROOT, "build", "b_us", "delinks", os.path.splitext(dest)[0] + ".o")
    if not (os.path.exists(obj) and os.path.exists(orig)):
        return [("", "no object (ninja it, or --build)", False)]
    ours, theirs = Loads(obj, ranges, mod, names), Loads(orig, ranges, mod, names)

    def same_bytes(a, b):
        found = ours.symbol_bytes(a[3]) if a[3] is not None and b[1] is not None and b[2] is not None else None
        if not found or not found[0]:
            return False
        data, mask = found
        start = names.datum(b[1], b[2])
        rom_data = rom.at(b[1], b[2] - (start[1] if start else 0), len(data))
        return len(rom_data) == len(data) and all(k in mask or data[k] == rom_data[k] for k in range(len(data)))

    rows = []
    for f in sorted(set(ours.loads) & set(theirs.loads)):
        if only is not None and f not in only:
            continue
        sizes = {k: ours.object[2][v[3]][3] for k, v in ours.loads[f].items() if v[3] is not None}
        mine, placed, orig_only, shifted, same = compare((ours.loads[f], sizes, ours.words[f], ours.slots[f]),
                                                         (theirs.loads[f], theirs.words[f], theirs.slots[f]),
                                                         same_bytes)
        parts = []
        if mine and orig_only:
            parts.append(f"loads {', '.join(mine)} where the original loads {', '.join(orig_only)}")
        elif mine:
            parts.append(f"loads {', '.join(mine)}, which the original doesn't")
        elif orig_only:
            parts.append(f"doesn't load {', '.join(orig_only)}, which the original does")
        if placed:
            parts.append(f"placed elsewhere (no name of symbols.txt): {', '.join(placed)}")
        if shifted:
            parts.append("another offset: " + ", ".join(f"{a} (the original's {b})" for a, b in shifted))
        if same:
            parts.append("the same bytes under another name: " + ", ".join(f"{a} = {b}" for a, b in same))
        if parts:
            rows.append((f, "; ".join(parts), bool(mine or orig_only and not placed)))
    return rows


# --- Macros -------------------------------------------------------------------------------------------------------

def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    return re.sub(r"//[^\n]*", "", text)


def macros(text):
    """{name: (parameters or None, body)} of the text's #defines, continuation lines joined, comments dropped."""
    lines, out, i = strip_comments(text).split("\n"), {}, 0
    while i < len(lines):
        line = lines[i]
        while line.rstrip().endswith(BS) and i + 1 < len(lines):
            line = line.rstrip()[:-1] + " " + lines[i + 1]
            i += 1
        m = re.match(r"\s*#\s*define\s+(\w+)(\(([^)]*)\))?(.*)", line)
        if m:
            out[m[1]] = ([p.strip() for p in m[3].split(",")] if m[2] is not None else None, " ".join(m[4].split()))
        i += 1
    return out


def tokens(body):
    return re.findall(r"\w+|\S", body)


def form(body):
    return "statement" if re.match(r"\{|do\b", body.strip()) else "expression"


def same_macro(ours, theirs):
    """Whether two macros are one: the parameters by position, other names through a renaming consistent within the
    macro (Black's names for pokebw2's), everything else as written."""
    (po, bo), (pt, bt) = ours, theirs
    if (po is None) != (pt is None) or po is not None and len(po) != len(pt):
        return False
    to, tt = tokens(bo), tokens(bt)
    if len(to) != len(tt):
        return False
    io, it = {p: i for i, p in enumerate(po or ())}, {p: i for i, p in enumerate(pt or ())}
    rename, back = {}, {}
    for a, b in zip(to, tt):
        if a in io or b in it:
            if io.get(a, -1) != it.get(b, -2):
                return False
        elif a != b:
            free = all(re.fullmatch(r"[A-Za-z_]\w*", x) and x not in KEYWORDS for x in (a, b))
            if not free or rename.setdefault(a, b) != b or back.setdefault(b, a) != a:
                return False
    return True


def header_macros(roots):
    """The names #defined in any header under roots."""
    out = set()
    for root in roots:
        for d, _, names in os.walk(root):
            for n in names:
                if n.endswith(".h"):  # a #define in a comment defines nothing
                    with open(os.path.join(d, n), errors="replace") as f:
                        out.update(re.findall(r"^\s*#\s*define\s+(\w+)", strip_comments(f.read()), re.M))
    return out


def short(s, limit=160):
    return s if len(s) <= limit else s[:limit - 3] + "..."


def macro_rows(text, theirs_text, ours_headers=(), theirs_headers=()):
    """[message] for one file: its function-like macros against pokebw2's text of its source."""
    ours, theirs = macros(text), macros(theirs_text)
    rows = []
    for name in sorted(set(ours) & set(theirs)):
        if theirs[name][0] is None and ours[name][0] is None:
            continue
        if not same_macro(ours[name], theirs[name]):
            fo, ft = form(ours[name][1]), form(theirs[name][1])
            article = {"statement": "a", "expression": "an"}
            how = f"pokebw2's is {article[ft]} {ft}, the file's {article[fo]} {fo}" if fo != ft else \
                "written otherwise"
            head = f"{name}({', '.join(theirs[name][0])})" if theirs[name][0] is not None else name
            rows.append(f"{name}: {how}; pokebw2: #define {head} {short(theirs[name][1])}; "
                        f"ours: {short(ours[name][1])}")
    new = sorted(n for n, m in theirs.items() if m[0] is not None and n not in ours and n not in ours_headers)
    if new:
        rows.append(f"pokebw2's macros the file and this repo's headers lack: {', '.join(new)}")
    gone = sorted(n for n, m in ours.items() if m[0] is not None and n not in theirs and n not in theirs_headers)
    if gone:
        rows.append(f"the file's macros pokebw2 no longer has: {', '.join(gone)}")
    return rows


# --- Main ---------------------------------------------------------------------------------------------------------

def module_of(dest, delinks):
    for mod, (_, files) in delinks.items():
        if dest in files:
            return mod, files[dest]
    return None, None


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("files", nargs="*", help="C files (default: every file port_file.py ported)")
    ap.add_argument("--open", action="store_true", help="only the functions open in docs/nonmatching.md")
    ap.add_argument("--build", action="store_true", help="ninja the files' objects first")
    ap.add_argument("--bw2", default=os.path.expanduser("~/ref/pokebw2"), help="pokebw2 checkout (read only)")
    ap.add_argument("--no-macros", action="store_true", help="tables only")
    ap.add_argument("--no-tables", action="store_true", help="macros only")
    args = ap.parse_args()
    ported = ported_files()
    files = {f: ported.get(f) for f in args.files} if args.files else ported
    opened = defaultdict(set)
    for f, n in open_rows():
        opened[f].add(n)
    if args.open:
        files = {f: s for f, s in files.items() if f in opened}
    delinks = read_delinks(CONFIG)
    if args.build:
        targets = [f"build/b_us/{os.path.splitext(f)[0]}.o" for f in files]
        subprocess.run(["ninja", "-k", "0"] + targets, cwd=ROOT, stdout=subprocess.DEVNULL)
    names = None if args.no_tables else Names()
    rom = Rom(delinks)
    ours_h = header_macros([os.path.join(ROOT, d) for d in ("include", "lib", "src")])
    theirs_h = header_macros([os.path.join(args.bw2, d) for d in ("include", "lib", "src")])
    flagged, flagged_open, macro_files, open_list = 0, 0, 0, []
    for dest, src in files.items():
        out = []
        mod, ranges = module_of(dest, delinks)
        if names and mod:
            for f, msg, counts in table_rows(dest, mod, ranges, names, rom, opened[dest] if args.open else None):
                tag = " (open)" if f in opened[dest] else ""
                out.append(f"  tables: {f}{tag}: {msg}")
                flagged += counts
                flagged_open += counts and bool(tag)
                if counts and tag:
                    open_list.append(f"{dest} {f}")
        if not args.no_macros and src and os.path.exists(os.path.join(args.bw2, src)):
            text = open(os.path.join(ROOT, dest), errors="replace").read()
            theirs = open(os.path.join(args.bw2, src), errors="replace").read()
            found = macro_rows(text, theirs, ours_h, theirs_h)
            out += [f"  macros: {m}" for m in found]
            macro_files += bool(found)
        if out:
            print(dest + (f" (pokebw2's {src})" if src else ""))
            print("\n".join(out))
    print(f"{len(files)} files: {flagged} functions load other data than the original ({flagged_open} open); "
          f"{macro_files} files' macros differ from pokebw2's")
    if open_list:
        print("open rows that load other data:\n  " + "\n  ".join(open_list))


if __name__ == "__main__":
    main()
