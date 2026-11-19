#!/usr/bin/env python3
"""Writes port/src/route_story_names.c: the names of the event flags and work values the decomp has, for the route
pilot's story readout (port/src/route_story.c: the map dump's story section, the Ws: token). Black and White: Black's
include/constants/flags.h and vars.h (a block marked "Black 2's, not checked against Black" is left out); Black 2 and
White 2: pokebw2's generated constants (third_party/pokebw2/generated/constants/). Platinum: pret's generated/vars_flags.txt
(third_party/pokeplatinum: the names in order, a line `NAME = other` sets the value). HeartGold and SoulSilver (one table): pret
pokeheartgold's include/constants/flags.h and vars.h (that tree is no registered submodule yet: POKEHEARTGOLD=DIR, default
third_party/pokeheartgold). Placeholder names (EVENT_FLAG_0x0960, EVENT_WORK_0x4041, FLAG_UNK_0x0FFF ...) are left out: the
readout prints the number anyway.

    python3 tools/route_story_names.py          # rewrites the file (run it after the constants change)
"""
import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEFINE = re.compile(r"#define\s+(EVENT_(?:FLAG|WORK)_\w+)\s+(0x[0-9a-fA-F]+|\d+)\s*$")
PLACEHOLDER = re.compile(r"_0x[0-9a-fA-F]+$")


def read(path, skip_unchecked):
    names, skipping = {}, False
    for line in (ROOT / path).read_text().splitlines():
        if skip_unchecked and line.startswith("//") and "Black 2's" in line and "not checked" in line:
            skipping = True
        if not line.strip():
            skipping = False
        m = DEFINE.match(line)
        if m and not skipping and not PLACEHOLDER.search(m.group(1)) and not m.group(1).startswith("EVENT_FLAG_TEMP_"):
            names.setdefault(int(m.group(2), 0), m.group(1))
    return names


def read_pt():
    """Platinum: generated/vars_flags.txt: `NAME` takes the next value, `NAME = ref` the value of ref (or a number)"""
    names, values, nxt = {}, {}, 0
    for line in (ROOT / "third_party/pokeplatinum/generated/vars_flags.txt").read_text().splitlines():
        name, _, ref = (p.strip() for p in line.partition("="))
        if not name:
            continue
        nxt = (values[ref] if ref in values else int(ref, 0)) if ref else nxt
        values[name] = nxt
        if re.match(r"(FLAG|VAR)_", name) and not PLACEHOLDER.search(name):
            names.setdefault(nxt, name)
        nxt += 1
    return names


def read_hg():
    """HeartGold/SoulSilver: include/constants/flags.h and vars.h: `#define FLAG_X 123` / VAR_X (plain numbers only)"""
    tree = Path(os.environ.get("POKEHEARTGOLD", ROOT / "third_party/pokeheartgold"))
    if not (tree / "include/constants/flags.h").exists():
        sys.exit("pokeheartgold tree not found (POKEHEARTGOLD=DIR): %s" % tree)
    names = {}
    for f in ("flags.h", "vars.h"):
        for line in (tree / "include/constants" / f).read_text().splitlines():
            m = re.match(r"#define\s+((?:FLAG|VAR)_\w+)\s+(0x[0-9a-fA-F]+|\d+)\s*(?://.*|/\*.*)?$", line)
            if m and not PLACEHOLDER.search(m.group(1)) and not m.group(1).startswith(("FLAG_ACTION_", "FLAG_TEMP_")):
                names.setdefault(int(m.group(2), 0), m.group(1))
    return names


def table(name, names):
    out = [f"const PortStoryName {name}[] = {{"]
    out += [f'    {{0x{i:04x}, "{n}"}},' for i, n in sorted(names.items())]
    out += ["    {0, \"\"},", "};", f"const int {name}Count = {len(names)};", ""]
    return out


def main():
    bw = read("include/constants/flags.h", True) | read("include/constants/vars.h", True)
    b2 = read("third_party/pokebw2/generated/constants/flags.h", False)
    b2 |= read("third_party/pokebw2/generated/constants/vars.h", False)
    lines = [
        "// Written by tools/route_story_names.py: the event flag and work value names of the decomp's constants, for the",
        "// route pilot's story readout (port/src/route_story.c). Black, White: include/constants/; Black 2, White 2:",
        "// pokebw2's generated constants (GPL-3.0, https://github.com/fuddlesworth/pokebw2); Platinum, HeartGold, SoulSilver:",
        "// pret's pokeplatinum / pokeheartgold constants (names only). Do not edit by hand.",
        "",
        "typedef struct {",
        "    unsigned short id;",
        "    const char *name;",
        "} PortStoryName;",
        "",
    ]
    pt, hg = read_pt(), read_hg()
    lines += table("gPortStoryNamesBW", bw) + table("gPortStoryNamesB2W2", b2)
    lines += table("gPortStoryNamesPT", pt) + table("gPortStoryNamesHG", hg)
    out = ROOT / "port/src/route_story_names.c"
    out.write_text("\n".join(lines))
    print(f"{out.relative_to(ROOT)}: {len(bw)} Black/White names, {len(b2)} Black 2/White 2 names, {len(pt)} Platinum, {len(hg)} HeartGold/SoulSilver")


if __name__ == "__main__":
    sys.exit(main())
