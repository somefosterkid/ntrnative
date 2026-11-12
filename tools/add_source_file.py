#!/usr/bin/env python3
"""Add a source file to a module's delinks.txt, or mark a file already listed there complete.

    tools/add_source_file.py src/ov198/b_plist_anm.c overlays/ov198 .text:0x0220b460-0x0220b4b0
    tools/add_source_file.py src/ov198/b_plist_anm.c overlays/ov198 .text:0x0220b460-0x0220b4b0 --incomplete
    tools/add_source_file.py src/ov198/b_plist_anm.c overlays/ov198

The module is its directory under config/b_us/arm9 (`.` for the main module); overlays share addresses, so it can't be
told from them. A .text range ends at the end of its last function or at the start of the next one. A new file is
marked complete, which links its compiled object in place of the original code, unless --incomplete. Without ranges,
the file already listed is marked complete; with them, it takes them instead of its own (and stays complete unless
--incomplete). A new file's entry goes beside its neighbour by address (insert_entry()). Adapted from pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
tools/scripts/add_source_file.py and mark_complete.py.
"""
import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pool_check  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]


def check_pool(delinks, before, source, module):
    """tools/pool_check.py after the file's ranges are set: a .text range that starts after the constant pool of its
    first function would make dsd dis panic for everyone; move its start to the pool, or put delinks.txt back and
    refuse."""
    mod = "main" if module == "." else Path(module).name
    why = pool_check.guard(mod, source)
    if why:
        delinks.write_text(before)
        sys.exit(f"{why}\n{delinks.relative_to(ROOT)} is as it was")


RANGE = re.compile(r"^\s+(\.\w+)\s+start:0x(\w+) end:0x(\w+)")


def insert_entry(lines, entry):
    """The delinks.txt lines with the new file's entry (its header line, then its own) beside its neighbour by
    address: after the file whose range of the new one's code section (.text or .ltdmain, else its first section)
    starts last before the new one's, else before the one that starts first after it, else at the end. It went at
    the end, and t36k moved the new files by hand; a delinks.txt isn't in address order as a whole (main's fx_vec.c
    comes first), so the neighbour is the file of the nearest address, not a place in a sorted list."""
    mine = {m[1]: int(m[2], 16) for m in map(RANGE.match, entry) if m}
    key = next((s for s in (".text", ".ltdmain") if s in mine), next(iter(mine), None))
    before = after = None  # (start, the block's first line, past its last)
    for i, line in enumerate(lines):
        if not line or line[0].isspace() or not line.endswith(":") or key is None:
            continue
        j = i + 1
        while j < len(lines) and lines[j].startswith(" "):
            j += 1
        theirs = {m[1]: int(m[2], 16) for m in map(RANGE.match, lines[i + 1:j]) if m}
        if key not in theirs:
            continue
        if theirs[key] < mine[key] and (before is None or theirs[key] > before[0]):
            before = (theirs[key], i, j)
        if theirs[key] > mine[key] and (after is None or theirs[key] < after[0]):
            after = (theirs[key], i, j)
    if before is not None:
        j = before[2]
        return lines[:j] + [""] + entry + lines[j:]
    if after is not None:
        i = after[1]
        while i and lines[i - 1].lstrip().startswith(("//", "#")):  # (its comment lines stay with it)
            i -= 1
        return lines[:i] + entry + [""] + lines[i:]
    return "\n".join(lines).rstrip("\n").split("\n") + [""] + entry


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("source", help="path of the source file, e.g. src/ov198/b_plist_anm.c")
    parser.add_argument("module", help="module directory under config/b_us/arm9, e.g. overlays/ov198 or . for main")
    parser.add_argument("sections", nargs="*", help="section ranges, e.g. .text:0x0220b460-0x0220b4b0")
    parser.add_argument("--incomplete", action="store_true", help="do not mark the new file complete")
    args = parser.parse_args()

    delinks = ROOT / "config" / "b_us" / "arm9" / args.module / "delinks.txt"
    before = delinks.read_text()
    lines = before.splitlines()
    header = f"{args.source}:"

    if not args.sections:
        if header not in lines:
            sys.exit(f"{args.source} is not in {delinks.relative_to(ROOT)}; give its section ranges to add it")
        i = lines.index(header)
        if lines[i + 1].strip() == "complete":
            sys.exit(f"{args.source} is already complete")
        lines.insert(i + 1, "    complete")
        delinks.write_text("\n".join(lines) + "\n")
        print(f"{args.source}: complete")
        return

    entry = []
    for section in args.sections:
        match = re.match(r"^(\.\w+):(0x[0-9a-fA-F]+)-(0x[0-9a-fA-F]+)$", section)
        if not match:
            sys.exit(f"invalid section range {section!r}, expected e.g. .text:0x0220b460-0x0220b4b0")
        name, start, end = match.group(1), int(match.group(2), 16), int(match.group(3), 16)
        entry.append(f"    {name:<11} start:{start:#010x} end:{end:#010x}")
    if header in lines:
        # A file already listed takes the new ranges, and stays complete unless --incomplete
        i = j = lines.index(header)
        while j + 1 < len(lines) and lines[j + 1].startswith(" "):
            j += 1
        complete = "    complete" in lines[i + 1:j + 1] and not args.incomplete
        lines[i + 1:j + 1] = (["    complete"] if complete else []) + entry
        delinks.write_text("\n".join(lines) + "\n")
        print(f"{args.source}: its ranges in {delinks.relative_to(ROOT)} replaced{', complete' if complete else ''}")
        check_pool(delinks, before, args.source, args.module)
        return
    entry = [header] + ([] if args.incomplete else ["    complete"]) + entry
    delinks.write_text("\n".join(insert_entry(lines, entry)).rstrip("\n") + "\n")
    print(f"{args.source}: added to {delinks.relative_to(ROOT)}{'' if args.incomplete else ', complete'}")
    check_pool(delinks, before, args.source, args.module)


if __name__ == "__main__":
    main()
