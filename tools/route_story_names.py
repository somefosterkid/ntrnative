#!/usr/bin/env python3
"""Writes port/src/route_story_names.c: the names of the event flags and work values the decomp has, for the route
pilot's story readout (port/src/route_story.c: the map dump's story section, the Ws: token). Black and White: Black's
include/constants/flags.h and vars.h (a block marked "Black 2's, not checked against Black" is left out); Black 2 and
White 2: pokebw2's generated constants (third_party/pokebw2/generated/constants/). Placeholder names (EVENT_FLAG_0x0960,
EVENT_WORK_0x4041 ...) are left out: the readout prints the number anyway.

    python3 tools/route_story_names.py          # rewrites the file (run it after the constants change)
"""
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
        "// pokebw2's generated constants (GPL-3.0, https://github.com/fuddlesworth/pokebw2). Do not edit by hand.",
        "",
        "typedef struct {",
        "    unsigned short id;",
        "    const char *name;",
        "} PortStoryName;",
        "",
    ]
    lines += table("gPortStoryNamesBW", bw) + table("gPortStoryNamesB2W2", b2)
    out = ROOT / "port/src/route_story_names.c"
    out.write_text("\n".join(lines))
    print(f"{out.relative_to(ROOT)}: {len(bw)} Black/White names, {len(b2)} Black 2/White 2 names")


if __name__ == "__main__":
    sys.exit(main())
