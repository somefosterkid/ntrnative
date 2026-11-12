#!/usr/bin/env python3
"""The structs whose size Black's allocations contradict: a constant size Black's code allocates for a struct, against
its sizeof here.

Black's sizes come from the Ghidra drafts (build/ghidra, tools/ghidra_draft.py), which give each allocation's size as a
constant. Its struct comes from either side:
- the draft stores the result in a variable of a struct's pointer type (GFL_HeapAllocate, GFL_ProcInitSubsystem): its
  types are this repo's, from the prototypes it was given;
- the C here passes `sizeof(X)` in the same call of the same function (GFL_HeapAllocate, GFL_ProcInitSubsystem,
  GameEvent_Create), the calls paired in order. That reaches functions that don't compile or aren't emitted too. A
  function not written here that is a near twin (tools/near_twins.py) has pokebw2's C instead.
A size that is a multiple of the struct's may be an array of it and isn't listed. Complete files are left out: their
sizes are Black's. clang lays the structs out as the C files see them (cached in build/alloc_sizes.json by the text of
the files and headers; tools/check_layouts.py's file_key).

    .venv/bin/python tools/alloc_sizes.py          # each struct Black allocates at another size, with its functions
"""
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_layouts as cl  # noqa: E402  (re-runs this in the venv)
import near_twins  # noqa: E402
import port_decls as pd  # noqa: E402

ROOT = cl.ROOT
DRAFTS = ROOT / "build" / "ghidra"
CACHE = ROOT / "build" / "alloc_sizes.json"
SIZE_ARG = {"GFL_HeapAllocate": 1, "GFL_ProcInitSubsystem": 1, "GameEvent_Create": 3}
TYPED = ("GFL_HeapAllocate", "GFL_ProcInitSubsystem")  # whose result is the struct
CALL = re.compile(r"\b(" + "|".join(SIZE_ARG) + r")\s*\(")


def arguments(text, start):
    """The arguments of the call whose "(" is at start, as text."""
    out, depth, cur = [], 0, start + 1
    for i in range(start, len(text)):
        c = text[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
            if depth == 0:
                out.append(text[cur:i].strip())
                return out
        elif c == "," and depth == 1:
            out.append(text[cur:i].strip())
            cur = i + 1
    return out


def calls(text):
    """[(function called, size argument, the call's start)] in a function's text."""
    out = []
    for m in CALL.finditer(text):
        args = arguments(text, m.end() - 1)
        if len(args) > SIZE_ARG[m[1]]:
            out.append((m[1], args[SIZE_ARG[m[1]]], m.start()))
    return out


def number(arg):
    arg = re.sub(r"^\(\w+\)\s*", "", arg)  # a cast
    return int(arg, 0) if re.fullmatch(r"0x[0-9a-fA-F]+|\d+", arg) else None


def defined_structs():
    """{short name: (file, record)} of the structs the headers and C files of src/ define."""
    out = {}
    # the game's headers first (a name both have is the game's), then the libraries' (DWCMatchState in lib/dwc's
    # base.h was invisible to struct_map.py, sx9)
    headers = pd.header_files(ROOT)
    paths = [p for p in headers if not pd.is_library(p)] + [p for p in headers if pd.is_library(p)]
    # src/ headers too (src/ov135/worldtrade_local.h: WorldTradeWork was invisible to the struct tools, sx8)
    for path in paths + sorted(str(p.relative_to(ROOT)) for p in (ROOT / "src").rglob("*.[hc]")):
        for item in pd.parse(ROOT / path):
            if item.struct and not item.fwd and "{" in item.code():
                key = item.tag or item.names[-1]
                out.setdefault(key.split()[-1], (path, key))
                for n in item.names:
                    out.setdefault(n, (path, key))
    return out


def pairs():
    """[(Black's function, its C file, short struct name, Black's size, how it was paired)] of the allocations."""
    files = cl.c_files()
    file_of = {}
    for o in (ROOT / "build" / "b_us" / "delinks").rglob("*.o"):
        rel = str(o.relative_to(ROOT / "build" / "b_us" / "delinks"))[:-2] + ".c"
        if rel in files:
            for name, _, _ in near_twins.functions(o):
                file_of[name] = rel
    ours = {}  # function: (its text, "C"), in the incomplete files; else pokebw2's of a near twin
    for f, complete in files.items():
        if not complete:
            for item in pd.parse(ROOT / f):
                if item.kind == "func":
                    ours[item.names[0]] = (item.text, "C")
    sources = defaultdict(dict)
    for t in near_twins.near_twins():
        if t["file"] in files and not files[t["file"]] and t["black"] not in ours:
            if t["source"] not in sources:
                sources[t["source"]] = {it.names[0]: it.text for it in pd.parse(near_twins.BW2 / t["source"])
                                        if it.kind == "func"}
            if t["bw2"] in sources[t["source"]]:
                ours[t["black"]] = (sources[t["source"]][t["bw2"]], "pokebw2")
    out = []
    for draft in sorted(DRAFTS.glob("*/*.c")):
        fn = draft.stem
        f = file_of.get(fn)
        if f is None or files[f]:
            continue
        text = draft.read_text(errors="replace")
        theirs = calls(text)
        for name, arg, start in theirs:
            size = number(arg)
            if size is None or name not in TYPED:
                continue
            line = text[text.rfind("\n", 0, start) + 1:start]
            m = re.match(r"\s*(\w+)\s*=\s*(?:\([^()]*\)\s*)?$", line) or \
                re.match(r"\s*return\s+(?:\([^()]*\)\s*)?$", line)
            if not m:
                continue
            decl = re.search(rf"^\s+(?:struct\s+)?(\w+)\s*\*\s*{m[1]};", text, re.M) if m.groups() else \
                re.search(rf"^(?:struct\s+)?(\w+)\s*\*\s*{fn}\(", text, re.M)
            if decl:
                out.append((fn, f, decl[1], size, "draft"))
        mine, how = ours.get(fn, ("", None))
        mine = calls(mine)
        for name in SIZE_ARG:
            a = [c for c in theirs if c[0] == name]
            b = [c for c in mine if c[0] == name]
            if len(a) != len(b):
                continue
            for (_, x, _), (_, y, _) in zip(a, b):
                m = re.fullmatch(r"sizeof\s*\(\s*(?:struct\s+)?(\w+)\s*\)", y)
                if m and number(x) is not None:
                    out.append((fn, f, m[1], number(x), how))
    return out


def sizes(names, defined):
    """{short name: sizeof here} of the structs, from clang (cached)."""
    by_file = defaultdict(set)
    for n in names:
        if n in defined:
            by_file[defined[n][0]].add(n)
    cache = json.loads(CACHE.read_text()) if CACHE.exists() else {}
    out = {}
    headers = sorted(f for f in by_file if f.endswith(".h"))
    jobs = [(f, sorted(ns)) for f, ns in by_file.items() if f.endswith(".c")]
    if headers:  # one file that includes them all
        (ROOT / "build" / "alloc_sizes.c").write_text("".join(f'#include "{pd.include_name(h)}"\n' for h in headers))
        jobs.append(("build/alloc_sizes.c", sorted(n for h in headers for n in by_file[h])))
    for f, ns in jobs:
        key = cl.file_key(f)
        hit = cache.get(f)
        if not hit or hit["key"] != key or set(ns) - set(hit["sizes"]):
            records = {defined[n][1] for n in ns}
            lay = cl.layouts(f, records)
            hit = {"key": key, "sizes": {n: lay[defined[n][1]][0] for n in ns if defined[n][1] in lay}}
            cache[f] = hit
        out.update({n: s for n, s in hit["sizes"].items() if n in ns})
    CACHE.write_text(json.dumps(cache))
    return out


def mismatches():
    """[(file, function, struct, Black's size, ours, how)] of the allocations whose size isn't the struct's here."""
    found = pairs()
    defined = defined_structs()
    ours = sizes({p[2] for p in found}, defined)
    out = []
    for fn, f, name, black, how in sorted(set(found)):
        size = ours.get(name)
        if size and black != size and black % size:
            out.append((f, fn, defined[name][1], black, size, how))
    return out


def main():
    rows = mismatches()
    by = defaultdict(list)
    for f, fn, record, black, ours, how in rows:
        by[(record, ours)].append((black, fn, f, how))
    for (record, ours), hits in sorted(by.items()):
        blacks = sorted({h[0] for h in hits})
        print(f"{record}: {ours:#x} here, Black allocates {', '.join(f'{b:#x}' for b in blacks)}: "
              + ", ".join(f"{fn} ({how})" for _, fn, _, how in hits[:4]) + (" ..." if hits[4:] else ""))
    print(f"{len(by)} structs, {len({r[0] for r in rows})} files", file=sys.stderr)


if __name__ == "__main__":
    main()
