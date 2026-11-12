#!/usr/bin/env python3
"""The struct and union layouts of two PC builds compared, by their DWARF (llvm-dwarfdump): each named type's size and
its members' offsets (bit offsets for bitfields). Both targets are 64-bit with 8-byte pointers, so the game's types
should come out the same; what differs is a type of the host's ABI in the game's C: `long` (8 bytes on Linux, 4 on
win64 and on the DS), wchar_t (4 and 2), bitfields next to 64-bit integers (win64 without -mno-ms-bitfields).

    python3 tools/port_layout.py build/port/linux/bwport build/port/win64/bwport.exe    # tools/port_win64.sh layout
    python3 tools/port_layout.py A B --all       # the host's types too (libc, C++, SDL, enet), which differ by design

Prints each type that differs (its sizes and the first member that moved) and a count; exit 1 when a type differs.
"""
import argparse
import re
import subprocess
import sys
from collections import defaultdict

DIE = re.compile(r"^(0x[0-9a-f]+):(\s+)(DW_TAG_\w+)")
ATTR = re.compile(r"^\s+(DW_AT_\w+)\s+\((.*)\)\s*$")
REF = re.compile(r"^(0x[0-9a-f]+)")
STRUCT_TAGS = {"DW_TAG_structure_type": "struct ", "DW_TAG_union_type": "union "}
# the host's headers and libntr's simulator: their types are the host's by design
HOST = re.compile(r"^/usr/|/c\+\+/|SDL2/|enet/|libraries/sim|imgui")


def num(v):
    v = v.strip()
    return int(v, 0) if re.fullmatch(r"-?(0x[0-9a-fA-F]+|\d+)", v) else v


def parse(exe):
    """{offset: (tag, attrs, parent offset)} from llvm-dwarfdump"""
    p = subprocess.Popen(["llvm-dwarfdump", "--debug-info", exe], stdout=subprocess.PIPE, text=True, errors="replace")
    dies, stack, cur = {}, [], None
    for line in p.stdout:
        m = DIE.match(line)
        if m:
            off, depth, tag = m.group(1), len(m.group(2)), m.group(3)
            while stack and stack[-1][0] >= depth:
                stack.pop()
            cur = (tag, {}, stack[-1][1] if stack else None)
            dies[off] = cur
            stack.append((depth, off))
            continue
        a = ATTR.match(line)
        if a and cur is not None:
            cur[1][a.group(1)] = a.group(2)
    if p.wait() != 0:
        sys.exit(f"llvm-dwarfdump failed on {exe}")
    return dies


def layouts(dies, host):
    members = defaultdict(list)
    for off, (tag, attrs, parent) in dies.items():
        if tag == "DW_TAG_member" and parent is not None:
            members[parent].append(attrs)

    def target(off, depth=0):
        """a typedef's struct or union, through typedefs and qualifiers"""
        if off not in dies or depth > 20:
            return None
        tag, attrs, _ = dies[off]
        if tag in STRUCT_TAGS:
            return off
        r = REF.match(attrs.get("DW_AT_type", ""))
        if tag in ("DW_TAG_typedef", "DW_TAG_const_type", "DW_TAG_volatile_type") and r:
            return target(r.group(1), depth + 1)
        return None

    out = defaultdict(set)  # name -> {(size, members)}
    files = defaultdict(set)
    for off, (tag, attrs, _) in dies.items():
        if "DW_AT_name" not in attrs:
            continue
        if tag in STRUCT_TAGS:
            name, t = STRUCT_TAGS[tag] + attrs["DW_AT_name"].strip('"'), off
        elif tag == "DW_TAG_typedef":
            name, t = attrs["DW_AT_name"].strip('"'), target(off)
            if t is None:
                continue
        else:
            continue
        tattrs = dies[t][1]
        if "DW_AT_declaration" in tattrs or "<" in name or "::" in name:
            continue
        f = attrs.get("DW_AT_decl_file", "").strip('"')
        if not host and HOST.search(f):
            continue
        mem = tuple((m.get("DW_AT_name", "?").strip('"'), num(m.get("DW_AT_data_member_location", "-1")),
                     num(m.get("DW_AT_data_bit_offset", "-1")), num(m.get("DW_AT_bit_size", "0")))
                    for m in members.get(t, []))
        out[name].add((num(tattrs.get("DW_AT_byte_size", "-1")), mem))
        f = re.sub(r"^.*/build/port/[^/]+/\.\./\.\./\.\./", "", f)  # libntr's, relative to a build dir
        files[name].add(re.sub(r"^/.*?/((src|include|lib|port)/)", r"\1", f))
    return out, files


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("a")
    ap.add_argument("b")
    ap.add_argument("--all", action="store_true", help="the host's types too")
    args = ap.parse_args()
    A, fa = layouts(parse(args.a), args.all)
    B, fb = layouts(parse(args.b), args.all)
    both = sorted(set(A) & set(B))
    differ = 0
    for n in both:
        if A[n] == B[n]:
            continue
        differ += 1
        sa, sb = sorted({s for s, _ in A[n]}), sorted({s for s, _ in B[n]})
        print(f"{n}: size {', '.join(map(hex, sa))} | {', '.join(map(hex, sb))}  ({', '.join(sorted(fa[n] | fb[n]))})")
        if len(A[n]) == 1 and len(B[n]) == 1:
            (_, ma), = A[n]
            (_, mb), = B[n]
            def at(m):
                return f"{m[0]} at {m[1]:#x}" if m[2] == -1 else f"{m[0]} at bit {m[2]}"
            for x, y in zip(ma, mb):
                if x != y:
                    print(f"    first moved: {at(x)} | {at(y)}")
                    break
    print(f"{len(both)} types in both, {differ} differ ({args.a} | {args.b})", file=sys.stderr)
    return 1 if differ else 0


sys.exit(main())
