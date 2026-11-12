#!/usr/bin/env python3
"""Port a pokebw2 source file that Black has too: copy it with Black's names, add it to delinks.txt and build it.

    python3 tools/port_file.py src/gfl/assert.c main
    python3 tools/port_file.py src/ov294/intro_mcss.c ov204 .text:0x021bf594-0x021bf8dc .rodata:0x021c08f0-0x021c0908

Without ranges, they come from tools/file_ranges.py (its notes are printed). The file goes where tools/file_ranges.py
says (src/<module>/, lib/gfl/src/ for pokebw2's src/gfl/). Each pokebw2 symbol in it gets the Black symbol at the same
place, found from the twins (build/twins_b_us.tsv), the order of the file's functions between twins, the offsets of
its data in its sections, and the relocations of functions of the same size:
- pokebw2's own guesses (its config/names.txt) and placeholders become Black's name, with pokebw2's in a comment
  above the definitions;
- swan's names (and the SDK's) replace Black's dsd placeholder, in symbols.txt and everywhere in src/, lib/, include/;
- pokebw2's overlay numbers (OVERLAY_ID(n)) become Black's, of the overlay that has most of that one's twins.
A B2 function that Black lacks (none between two paired functions), which nothing else in the file calls, is dropped.
After a first compile, the file's static data take the dsd names of the addresses they land on (placed as the link
places the object's sections), and a static function or datum that code elsewhere reaches becomes global. No name is
given that the file has already or that two of its symbols land on (their data moved in Black).

Then the object is checked against the ROM, as it will be linked: each section's size against its range, the bytes of
its data, the target of each relocation (relocs.txt), and what code outside the file reaches in its ranges. What the
ROM tells is fixed: an initializer's values that differ from Black's (pokebw2's tables often do) are rewritten from
the ROM's bytes, with the types from the object's debug info; a name Black has elsewhere or not at all gives way to
Black's (or, swan's, goes to Black's placeholder); globals Black lacks go, in a section too big (one the code still names, a `const` scalar that Black's code has as a
literal, shinka_demo_view.c's SHINKA_DEMO_HELIX_TOP, becomes a #define); dsd's false pointers
from other modules' byte tables into the file's data go from their relocs.txt.

The file goes in delinks.txt incomplete. When it compiles, its object is the ROM's and every function matches
(tools/compiler_probe.py), it is marked complete and `ninja` must pass; otherwise it stays incomplete (and the build
broken, while it doesn't compile) and the script prints what failed: missing headers, the declarations pokebw2's
headers have and Black's lack, compile errors, data the delinks entry lacks, what the object has otherwise than the
ROM, functions that don't match, the build's error or the first byte of the module that differs. Headers are not
ported here: `tools/port_decls.py --apply --file <file>` declares what the file uses. Once that is fixed, the same
command builds the file again and marks it complete if it matches; given ranges, the file takes them first. The
structs the file defines itself get port_decls.py's notes: Black 2's, but for the fields its twinned functions access.
Copying from pokebw2 is allowed: GPL-3.0, https://github.com/fuddlesworth/pokebw2.
"""
import argparse
import bisect
import csv
import glob
import os
import re
import shutil
import struct
import subprocess
import sys
from collections import defaultdict

from callee_names import PLACEHOLDER, read_relocs
from file_ranges import CODE, DATA, ROOT, TINY, Ranges, black_path, code_section, module_dir, propose, read_rows

CONFIG = f"{ROOT}/config/b_us/arm9"
# --pairs: {B2 (module, address): Black (module, address)} that no twin shows
PAIRS = {}
INCLUDE_DIRS = ["include"] + sorted(p[len(ROOT) + 1:] for p in glob.glob(f"{ROOT}/lib/*/include"))


def read_names(root, labels=False):
    """Returns {(module, address): name} (functions and data, and labels too if asked) and {name: [(module,
    address)]}."""
    at, where = {}, defaultdict(list)
    for path in glob.glob(f"{root}/**/symbols.txt", recursive=True):
        mod = os.path.basename(os.path.dirname(path))
        mod = "main" if mod == "arm9" else mod
        for line in open(path):
            m = re.match(r"(\S+) kind:(\w+).* addr:(0x\w+)", line)
            if m and (labels or m[2] != "label"):
                at[mod, int(m[3], 16)] = m[1]
                where[m[1]].append((mod, int(m[3], 16)))
    return at, where


def read_object(path):
    """A relocatable ELF: its bytes, sections [(name, type, offset, size, alignment)], symbols by index [(name,
    section, value, size, global, function)] and relocations {section: [(offset, type, symbol, addend)]}."""
    return parse_object(open(path, "rb").read())


def parse_object(d):
    """read_object's tuple from an object's bytes (an archive's member: tools/prebuilt_map.py)."""
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", d, 0x2E)
    sh = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    cstr = lambda off: d[off:d.index(b"\0", off)].decode(errors="replace")
    # TwlSDK's twl/ltdmain_begin.h sends the uninitialised data after it to .ltdmain.bss, which the link puts with the
    # file's .bss (tools/dead_strip.py): its range is the file's .bss range
    secs = [({".ltdmain.bss": ".bss"}.get(n, n), *rest) for n, *rest in
            ((cstr(sh[shstrndx][4] + s[0]), s[1], s[4], s[5], s[8]) for s in sh)]
    syms, relocs = [], defaultdict(list)
    for s in sh:
        if s[1] == 2:  # SYMTAB
            for i in range(s[5] // 16):
                name, value, size, info, _, ndx = struct.unpack_from("<IIIBBH", d, s[4] + 16 * i)
                syms.append((cstr(sh[s[6]][4] + name), ndx, value, size, info >> 4 == 1, info & 15 == 2))
    for s in sh:
        if s[1] in (4, 9):  # RELA, REL
            for o in range(s[4], s[4] + s[5], 12 if s[1] == 4 else 8):
                off, info = struct.unpack_from("<II", d, o)
                add = struct.unpack_from("<i", d, o + 8)[0] if s[1] == 4 else 0
                relocs[s[7]].append((off, info & 255, info >> 8, add))
    return d, secs, syms, relocs


def place(secs, ranges):
    """Where the link puts each section of the object that has a range: {section: address}. MWCC gives each function,
    and some data, a section of its own; dsd's linker script keeps their order and aligns each to 4 (ALIGNALL)."""
    at, out = {}, {}
    for i, (name, kind, _, size, align) in enumerate(secs):
        if kind in (1, 8) and name in ranges:  # PROGBITS, NOBITS
            align = max(4, align)
            out[i] = -(-at.get(name, ranges[name][0]) // align) * align
            at[name] = out[i] + size
    return out


def module_bytes(r, mod, built=False):
    """A module's bytes, the ROM's (extracted) or the build's, and its base address: where its first section starts
    (main's and the overlays' .text; DTCM's and the unknown autoloads' .data, which have no .text). KeyError for a
    module without sections, as for one not in the delinks (callers such as fit_data.holes() go on without the ROM)."""
    secs = r.delinks[mod][0]
    if not secs:
        raise KeyError(f"{mod}: no sections in its delinks.txt")
    return open(f"{ROOT}/{module_path(mod, built)}", "rb").read(), min(s for s, _ in secs.values())


def module_path(mod, built=False):
    """The module's file, the ROM's (extracted) or the build's, from the checkout: main, an autoload, the DSi's LTD
    autoload or an overlay."""
    name = {"main": "arm9"}.get(mod, mod)
    if mod in ("main", "itcm", "dtcm"):
        path = f"build/b_us/build/{name}.bin" if built else f"extract/b_us/arm9/{name}.bin"
    elif re.fullmatch(r"(ltd_)?autoload_\d+", mod):  # the LTD one is the DSi's, the others unknown autoloads
        path = f"build/b_us/build/{mod}.bin" if built else \
            f"extract/b_us/dsi/{mod}.bin" if mod.startswith("ltd_") else f"extract/b_us/arm9/unk_{mod}.bin"
    else:
        path = f"build/b_us/build/arm9_{mod}.bin" if built else f"extract/b_us/arm9_overlays/{mod}.bin"
    return path


def uleb(b, i):
    v = shift = 0
    while True:
        v, shift, i = v | (b[i] & 0x7F) << shift, shift + 7, i + 1
        if b[i - 1] < 0x80:
            return v, i


def debug_types(d, secs, syms, relocs):
    """{name: type} of the object's file-scope variables, from MWCC's DWARF 2: ("int", size, signed, {value:
    enumerator}), ("pointer", 4), ("array", element, count), ("struct" or "union", size, [(offset, type, bit size, bit
    offset)]), or None for what isn't told here (floats)."""
    index = {s[0]: i for i, s in enumerate(secs)}
    if ".debug_info" not in index or ".debug_abbrev" not in index:
        return {}
    _, _, o, n, _ = secs[index[".debug_info"]]
    info = d[o:o + n]
    _, _, o, n, _ = secs[index[".debug_abbrev"]]
    ab, abbrev, i = d[o:o + n], {}, 0
    while i < len(ab):
        code, i = uleb(ab, i)
        if code:
            tag, i = uleb(ab, i)
            spec, children, i = [], ab[i], i + 1
            while True:
                a, i = uleb(ab, i)
                f, i = uleb(ab, i)
                if not a and not f:
                    break
                spec.append((a, f))
            abbrev[code] = tag, children, spec
    # MWCC's references (DW_FORM_ref_addr) are relocations, their target in the addend
    refs = {off: syms[s][2] + add for off, _, s, add in relocs.get(index[".debug_info"], [])}
    fixed = {0x01: 4, 0x05: 2, 0x06: 4, 0x07: 8, 0x0B: 1, 0x0C: 1, 0x10: 4, 0x11: 1, 0x12: 2, 0x13: 4, 0x14: 8}
    dies = {}  # offset -> (tag, {attribute: value}, [children])

    def parse(i):
        out = []
        while i < len(info):
            off, (code, i) = i, uleb(info, i)
            if not code:
                return out, i
            tag, children, spec = abbrev[code]
            attrs = {}
            for a, f in spec:
                if f in fixed:
                    attrs[a] = refs.get(i, 0) if f == 0x10 else int.from_bytes(info[i:i + fixed[f]], "little")
                    i += fixed[f]
                elif f == 0x08:  # string
                    j = info.index(b"\0", i)
                    attrs[a], i = info[i:j].decode(errors="replace"), j + 1
                elif f in (0x03, 0x04, 0x0A):  # blocks
                    k = {0x03: 2, 0x04: 4, 0x0A: 1}[f]
                    size, i = int.from_bytes(info[i:i + k], "little"), i + k
                    attrs[a], i = info[i:i + size], i + size
                elif f in (0x0F, 0x0D):  # udata, sdata (only small positive values here)
                    attrs[a], i = uleb(info, i)
                else:
                    raise ValueError(f"DWARF form {f:#x}")
            kids = []
            if children:
                kids, i = parse(i)
            dies[off] = tag, attrs, kids
            out.append(off)
        return out, i

    top, _ = parse(11)  # after the compile unit's header

    def type_of(off, seen=()):
        if off not in dies or off in seen:
            return None
        tag, a, kids = dies[off]
        if tag in (0x16, 0x26, 0x35):  # typedef, const, volatile
            return type_of(a.get(0x49), seen + (off,))
        if tag == 0x24:  # base type; encoding 4 is a float, 5 and 6 signed
            if a.get(0x3E) == 4:
                return "float", a.get(0x0B, 4)
            return "int", a.get(0x0B, 0), a.get(0x3E) in (5, 6), {}
        if tag == 0x04:  # enum
            return "int", a.get(0x0B, 4), True, {dies[k][1].get(0x1C): dies[k][1].get(0x03) for k in kids}
        if tag in (0x0F, 0x15):  # pointer, function
            return "pointer", 4
        if tag in (0x13, 0x17):  # struct, union
            members = []
            for k in kids:
                tag_k, m, _ = dies[k]
                if tag_k == 0x0D:
                    loc = m.get(0x38, b"\x23\x00")  # DW_OP_plus_uconst or DW_OP_const4u
                    at = uleb(loc, 1)[0] if loc[0] == 0x23 else int.from_bytes(loc[1:5], "little")
                    members.append((at, type_of(m.get(0x49), seen + (off,)), m.get(0x0D), m.get(0x0C)))
            return "struct" if tag == 0x13 else "union", a.get(0x0B, 0), members
        if tag == 0x01:  # array, one subrange per dimension
            t = type_of(a.get(0x49), seen + (off,))
            for k in reversed([k for k in kids if dies[k][0] == 0x21]):
                t = "array", t, dies[k][1].get(0x2F, -1) + 1
            return t
        return None

    unit = [dies[k] for k in dies[top[0]][2]] if top else []
    return {a[0x03]: type_of(a.get(0x49)) for tag, a, _ in unit if tag == 0x34 and 0x03 in a}  # variables


def size_of(t):
    return t[1] if t[0] in ("int", "float", "struct", "union") else 4 if t[0] == "pointer" else size_of(t[1]) * t[2]


def definition(text, name):
    """A file-scope definition with an initializer: (start, initializer, end after its `;`). The initializer is a
    leaf, the (start, end) of its expression in text, or for `{...}` a list of items, leaves and lists. None if there
    is none, or it has designators."""
    m = re.search(rf"^(?!extern\b)[^\s#/][^;{{}}\n]*\b{name}\b\s*(?:\[[^\]]*\]\s*)*=\s*", text, re.M)
    if not m:
        return None
    masked = re.sub(r"//[^\n]*|/\*.*?\*/", lambda c: " " * len(c[0]), text, flags=re.S)
    if masked[m.end()] != "{":
        end = masked.index(";", m.end())
        leaf = (m.end(), m.end() + len(masked[m.end():end].rstrip()))
        return (m.start(), leaf, end + 1) if "," not in masked[m.end():end] else None

    def group(j):  # masked[j] == "{"
        items, start, depth, sub = [], j + 1, 0, None
        j += 1
        while True:
            c = masked[j]
            if c in "\"'":
                j = re.compile(rf"{c}(?:\\.|[^\\{c}])*{c}").match(masked, j).end()
                continue
            if c == "{" and not depth:
                sub, j = group(j)
                continue
            depth += (c in "([") - (c in ")]")
            if not depth and c in ",}":
                lo, hi = start, j
                while lo < hi and masked[lo].isspace():
                    lo += 1
                while hi > lo and masked[hi - 1].isspace():
                    hi -= 1
                if sub is not None:
                    items.append(sub)
                elif lo < hi:
                    if masked[lo] in ".[":
                        raise ValueError("designator")
                    items.append((lo, hi))
                sub, start = None, j + 1
                if c == "}":
                    return items, j + 1
            j += 1

    try:
        items, j = group(m.end())
        return m.start(), items, masked.index(";", j) + 1
    except (ValueError, AttributeError, IndexError):
        return None


def definition_span(text, name):
    """Where a file-scope definition is with the comments that tell of it: (start, end after its `;`). The comment
    lines right above it (`//` lines or a `/* */` block of their own, no blank line between) go with it, its
    `// pokebw2's` name too, None if it has no initializer (definition())."""
    found = definition(text, name)
    if not found:
        return None
    start, _, end = found
    while start:
        prev = text.rfind("\n", 0, start - 1) + 1
        line = text[prev:start].strip()
        if line.startswith("//"):
            start = prev
        elif line.startswith("/*") and line.endswith("*/"):
            start = prev
        elif line.endswith("*/"):  # the last line of a block: from the line its `/*` opens
            opening = text.rfind("/*", 0, prev)
            head = text.rfind("\n", 0, opening) + 1
            if opening < 0 or text[head:opening].strip() or "*/" in text[opening:prev]:
                break
            start = head
        else:
            break
    return start, end


def swap_definitions(text, swaps):
    """Puts the definition of swaps[name] where name's is, for each name (both have initializers), each with the
    comments right above it: md5.c's padding and word-index tables, of one size, traded places while their comments
    stayed, so that each table's comment told of the other one (and a later version read the wrong one)."""
    spans = {name: definition_span(text, name) for name in swaps}
    chunks = {name: text[s:e] for name, (s, e) in spans.items()}
    for name, (s, e) in sorted(spans.items(), key=lambda x: -x[1][0]):
        text = text[:s] + chunks[swaps[name]] + text[e:]
    return text


def fix_initializer(text, name, t, ours, theirs, mask):
    """Gives the values of a definition's initializer that differ from the ROM's bytes (theirs) the ROM's, from its
    type and the compiled bytes (ours; mask: the offsets of relocated bytes). Returns the new text, or None if a value
    has no place in the C (a member left out, a string, a float)."""
    found = definition(text, name)
    if t is None or found is None:
        return None
    item, leaves = found[1], []

    def walk(t, items, i, off):
        """The leaf, or the aggregate's leaves, of items[i:] for a t at off. Returns the next item."""
        if t is None:
            raise ValueError("type")
        if t[0] in ("array", "struct", "union"):
            if i < len(items) and isinstance(items[i], list):  # braced
                fill(t, items[i], 0, off)
                return i + 1
            return fill(t, items, i, off)  # braces left out: the aggregate takes as many items as it has members
        it = items[i] if i < len(items) else None
        while isinstance(it, list):
            it = it[0] if it else None
        leaves.append((off, t, it))
        return i + 1

    def fill(t, items, i, off):
        if t[0] == "array":
            chars = t[1] and t[1][:2] == ("int", 1)
            if chars and i < len(items) and not isinstance(items[i], list) and text[items[i][0]] == '"':
                leaves.append((off, ("string", t[2]), items[i]))  # a char array's string
                return i + 1
            for k in range(t[2]):
                i = walk(t[1], items, i, off + k * size_of(t[1]))
            return i
        for at, mt, bits, top in t[2] if t[0] == "struct" else t[2][:1]:
            i = walk(("bits", mt, bits, top) if bits else mt, items, i, off + at)
        return i

    try:
        walk(t, [item], 0, 0)
    except (ValueError, IndexError):
        return None
    edits = []
    for off, t, it in leaves:
        bits, top = (t[2], t[3]) if t[0] == "bits" else (None, None)
        t = t[1] if bits else t
        # Pointers are relocated (their names are checked apart); strings, floats and members the C leaves out (zero)
        # aren't rewritten here, and what still differs is reported after the next compile
        if t is None or t[0] != "int" or it is None or any(k in mask for k in range(off, off + t[1])):
            continue
        size, signed, enums = t[1], t[2], t[3]
        width = bits or size * 8
        o, w = (int.from_bytes(b[off:off + size], "little") for b in (ours, theirs))
        if bits:
            low = size * 8 - top - bits
            o, w = (o >> low) & ((1 << bits) - 1), (w >> low) & ((1 << bits) - 1)
        if o == w:
            continue
        if signed and w >= 1 << (width - 1):
            w -= 1 << width
        old = text[it[0]:it[1]]
        if w in enums and re.fullmatch(r"[A-Za-z_]\w*", old):
            new = enums[w]
        elif re.fullmatch(r"'.'", old) and 0x20 <= w < 0x7F and chr(w) not in "'\\":
            new = f"'{chr(w)}'"
        elif re.match(r"-?0[xX]", old) or not re.fullmatch(r"-?\d+[uUlL]*", old) and abs(w) >= 10:
            new = f"-{-w:#x}" if w < 0 else f"{w:#x}"
        else:
            new = str(w)
        edits.append((it, new))
    for (lo, hi), new in sorted(edits, reverse=True):
        text = text[:lo] + new + text[hi:]
    return text if edits else None


def reached(r, mod, ranges):
    """{address: [(module, from)]} of what code and data outside the file reach in its ranges, from relocs.txt, also
    through relocations to several overlays at one address (module:overlays(24,119)), which dsd links to the first."""
    inside = lambda a: any(s <= a < e for s, e in ranges.values())
    out = defaultdict(list)
    for m, rows in r.relocs.items():
        for fr, to, _, tm, _ in rows:
            if tm == mod and inside(to) and not (m == mod and inside(fr)):
                out[to & ~1].append((m, fr))
    if re.match(r"ov\d+$", mod):
        for path in glob.glob(f"{CONFIG}/**/relocs.txt", recursive=True):
            m = os.path.basename(os.path.dirname(path))
            rows = re.findall(r"^from:(0x\w+) .*to:(0x\w+) module:overlays\((\d+)[,)]", open(path).read(), re.M)
            for fr, to, first in rows:
                if int(first) == int(mod[2:]) and inside(int(to, 16)):
                    out[int(to, 16) & ~1].append(("main" if m == "arm9" else m, int(fr, 16)))
    return out


def check(r, mod, ranges, dest, obj, bw2):
    """Compares the compiled object, placed as the link will place it, with the ROM: the size and bytes of each data
    section, the target of each relocation, and what code outside the file reaches in its ranges. Fixes what the ROM
    tells: the C's initializer values that differ (their types from the object's debug info), and names Black has at
    another address or not at all (swan's names go to Black's placeholder, as in port()). Returns the problems left
    and whether the C changed."""
    d, secs, syms, relocs = read_object(f"{ROOT}/{obj}")
    at = place(secs, ranges)
    rom, base = module_bytes(r, mod)
    black_at, black_where = read_names(CONFIG)[0], read_names(CONFIG, labels=True)[1]  # a label is a name to link to
    black = {fr: (to, add, tm) for fr, to, add, tm, _ in r.relocs[mod]}  # to:<table> add:-0x2 is table - 2
    header = r.delinks[mod][0]
    problems, text, renames, replace = [], open(f"{ROOT}/{dest}").read(), {}, {}

    # Every function where Black has one of its size, and each of Black's there, or nothing else can be compared
    own_code = start, end = ranges[code_section(ranges)]
    sizes = {a: s for a, s, _ in r.funcs[mod] if start <= a < end}
    placed = {at[i] + (v & ~1): (n, s) for n, i, v, s, _, f in syms if f and s and i in at}
    moved = [f"{n} at {a:#x}" for a, (n, s) in placed.items() if sizes.get(a) != s]
    moved += [f"none at {a:#x}" for a in sizes if a not in placed]
    if moved:
        return [f"functions not where Black has them, one missing or of another size: {', '.join(moved[:4])}"], False

    # Sizes. A file at the end of a section may stop short of it: the rest is the section's padding
    too_big = []
    for sec, (s0, s1) in ranges.items():
        end = max([at[i] + secs[i][3] for i in at if secs[i][0] == sec], default=s0)
        tail = sec == ".bss" or not any(rom[end - base:s1 - base])
        if end > s1 or -(-end // 4) * 4 < s1 and not (tail and s1 == header[sec][1]):
            zeros = f", and the ROM's last {s1 - end:#x} are zeros (a table's tail?)" if s1 > end and tail else ""
            problems.append(f"{sec} is {end - s0:#x} bytes, delinks.txt gives {s1 - s0:#x}{zeros}")
            too_big += [sec] if end > s1 else []

    # Relocations: to the file's own symbols, where they land; to others', by name
    relocated, layout, wrong = set(), [], defaultdict(set)
    func_at = lambda a: next((n for n, i, v, s, _, f in syms if f and i in at and at[i] <= a < at[i] + s), "?")
    objs = {k: (n, i, at[i] + v, s) for k, (n, i, v, s, _, f) in enumerate(syms)
            if i in at and not f and s and secs[i][0] not in CODE}  # the data objects: name, section, address, size
    theirs_at = {}  # data object -> where Black's code and data reach it
    for i, rows in relocs.items():
        for off, kind, s, add in rows if i in at else ():
            a, (name, ndx, value, _, _, _) = at[i] + off, syms[s]
            add = add if kind == 2 else 0  # R_ARM_ABS32's addend is an offset, a branch's the pipeline's -4
            relocated.update(range(a, a + 4))
            if a not in black:
                continue
            to, black_add, tm = black[a]
            theirs = black_at.get((tm, to)) or black_at.get((tm, to & ~1)) or f"{to:#x}"
            target = to
            to += black_add
            if ndx in at:
                ours = at[ndx] + value + add
                k = next((k for k, (_, i2, a2, s2) in objs.items() if i2 == ndx and a2 <= ours < a2 + s2), None)
                if k is not None:
                    theirs_at[k] = to - (ours - objs[k][2])
                if ours & ~1 != to & ~1:
                    layout.append(f"{func_at(a)} at {a:#x} reaches {name}+{add:#x} at {ours:#x}, "
                                  f"Black's {theirs} at {to:#x}")
            elif ndx:
                layout.append(f"{name} is in {secs[ndx][0]}, which delinks.txt gives the file no range for")
            elif tm == mod and secs[i][0] not in CODE and own_code[0] <= to < own_code[1]:
                continue  # a table of the file's own functions in another order in Black: the names are right
            elif tm and not any((x + add) & ~1 == to & ~1 for m, x in black_where.get(name, []) if m == tm) and \
                    not linked_name(r, tm, target, name):
                wrong[name].add(theirs)
    problems += layout[:4]

    # What code outside the file reaches in its ranges must be the file's globals, by Black's names. A pointer from
    # another file's data into the middle of an object is dsd reading a byte table as one: it goes from relocs.txt
    defined = {(at[i] + v) & ~1: n for n, i, v, _, g, _ in syms if g and i in at}
    starts = {(at[i] + v) & ~1 for n, i, v, s, _, _ in syms if i in at and s}
    in_data = lambda m, a: any(s <= a < e for sec, (s, e) in r.delinks[m][0].items() if sec in (".rodata", ".data"))
    outside, false = reached(r, mod, ranges), defaultdict(set)
    for a, refs in sorted(outside.items()):
        name = black_at.get((mod, a)) or black_at.get((mod, a | 1))
        if not name or defined.get(a) == name:
            continue
        if a not in starts and all(in_data(m, fr) for m, fr in refs):
            for m, fr in refs:
                false[m].add(fr)
        elif a in defined:
            wrong[defined[a]].add(name)
        else:
            problems.append(f"{name} at {a:#x}, which {refs[0][0]} reaches from {refs[0][1]:#x}, is not one of the "
                            f"file's globals")

    # Names: swan's go to Black's placeholders, as in port(); a name Black lacks, or has elsewhere, gives way
    guesses = {l.split()[2] for l in open(f"{bw2}/config/names.txt") if l.strip() and l[0] != "#"}
    swan = set(read_names(f"{bw2}/config/b2_us/arm9")[1]) - guesses
    for name, targets in wrong.items():
        theirs = next(iter(targets))
        if len(targets) > 1 or theirs.startswith("0x"):
            problems.append(f"{name} is Black's {', '.join(sorted(targets))}")
        elif name not in black_where and PLACEHOLDER.match(theirs) and name in swan and \
                not used_elsewhere(name, dest):
            renames[theirs] = name
        elif name not in black_where:
            renames[name] = theirs  # a name Black doesn't have: nothing else can link to it either
        else:
            replace[name] = theirs  # Black's at another address: this file's pairing was wrong
    if renames or replace:
        print(f"names: {', '.join(f'{a} -> {b}' for a, b in {**renames, **replace}.items())}, from the relocations")
    for m, froms in false.items():
        path = f"{CONFIG}/{module_dir(m)}/relocs.txt"
        lines = open(path).read().split("\n")
        keep = [l for l in lines if not (l.startswith("from:") and int(l.split()[0][5:], 16) in froms)]
        open(path, "w").write("\n".join(keep))
        r.relocs[m] = [row for row in r.relocs[m] if row[0] not in froms]
        print(f"relocs: {len(lines) - len(keep)} of {m}'s pointers into the file's data dropped, which are bytes "
              f"of its tables: {', '.join(f'{fr:#x}' for fr in sorted(froms))}")

    # Data Black lacks: in a section too big, globals that nothing here reaches, under a name Black lacks (so nothing
    # there reaches them either)
    used = {s for i, rows in relocs.items() if i in at for _, _, s, _ in rows}  # not the debug info's
    b2_only = [n for k, (n, i, v, s, g, f) in enumerate(syms) if i in at and secs[i][0] in too_big and g and not f and s
               and k not in used and n not in black_where]
    defines = []
    for name in b2_only:
        found = definition(text, name)
        if found:
            start = found[0]
            while start and text[text.rfind("\n", 0, start - 1) + 1:start].startswith("//"):
                start = text.rfind("\n", 0, start - 1) + 1
            after = text[found[2]:].lstrip("\n")
            # a constant the code still names is folded into it, as MWCC does with a const global (shinka_demo_view.c's
            # SHINKA_DEMO_HELIX_TOP, which Black's code has as a literal): Black's source had a #define
            value = re.fullmatch(rf"const\s+(?:[us](?:8|16|32)|f32|fx(?:16|32)|int)\s+{name}\s*=\s*([^;]+);",
                                 text[found[0]:found[2]])
            if value and re.search(rf"\b{name}\b", text[:start] + after):
                number = value[1].strip()
                text = text[:start] + (f"#define {name} {number}" if re.fullmatch(r"[\w.+-]+", number) else
                                       f"#define {name} ({number})") + "\n" + after
                defines.append(name)
            else:
                text = text[:start] + after
    if b2_only:
        gone = [n for n in b2_only if n not in defines]
        print("data: " + (f"dropped {', '.join(gone)}, which Black doesn't have" if gone else "")
              + ("; " if gone and defines else "")
              + (f"{', '.join(defines)} made #defines, as Black's code has them as literals" if defines else ""))

    # Objects of one size in another order than Black's. MWCC heapsorts a section's objects by size, so two of a size
    # trade places when their definitions do: each slot gets the definition of the object Black has where the slot's
    # lands. Where Black's code doesn't tell, the bytes do
    count = defaultdict(int)
    for n, i, a, s in objs.values():
        count[i] += 1
    groups = defaultdict(list)  # (section, size, a section of its own) -> objects
    for k, (n, i, a, s) in objs.items():
        groups[secs[i][0], s, count[i] == 1].append(k)
    swaps = {}
    for g in groups.values() if layout and not too_big and not b2_only else ():
        lands = {objs[k][2]: k for k in g}
        want = {k: theirs_at[k] for k in g if theirs_at.get(k) in lands}
        for k in g:
            n, i, a, s = objs[k]
            ours = d[secs[i][2] + a - at[i]:secs[i][2] + a - at[i] + s]
            same = [x for x in sorted(lands) if x not in want.values() and rom[x - base:x - base + s] == ours]
            if k not in want and same and secs[i][1] == 1:  # not .bss
                want[k] = same[0]
        found = {objs[k][0]: definition(text, objs[k][0]) for k in g}
        if len(g) > 1 and len(set(want.values())) == len(g) and all(found.values()):
            swaps.update({objs[k][0]: objs[next(j for j in g if want[j] == objs[k][2])][0] for k in g})
    swaps = {a: b for a, b in swaps.items() if a != b}
    if swaps:
        text = swap_definitions(text, swaps)
        print(f"data: definitions reordered to Black's order: {', '.join(f'{b} for {a}' for a, b in swaps.items())}")

    # Data: the bytes of each symbol against the ROM's, and the initializers' values from the ROM where they differ
    types = None
    for name, ndx, value, size, _, func in syms:
        if ndx not in at or func or not size or secs[ndx][1] != 1 or secs[ndx][0] in CODE or b2_only:
            continue
        a = at[ndx] + value
        ours, theirs = d[secs[ndx][2] + value:secs[ndx][2] + value + size], rom[a - base:a - base + size]
        mask = {k for k in range(size) if a + k in relocated}
        diff = [k for k in range(size) if k not in mask and ours[k:k + 1] != theirs[k:k + 1]]
        if not diff:
            continue
        if layout:
            problems.append(f"{name}: {len(diff)} bytes differ from the ROM, first at +{diff[0]:#x}")
            continue
        if types is None:
            try:
                types = debug_types(d, secs, syms, relocs)
            except (ValueError, KeyError, IndexError):  # debug info this reader doesn't know
                types = {}
        new = fix_initializer(text, name, types.get(name), ours, theirs, mask) if name in types else None
        if new is None or new == text:
            problems.append(f"{name}: {len(diff)} bytes differ from the ROM, first at +{diff[0]:#x}: "
                            f"{ours[diff[0]:diff[0] + 8].hex(' ')} for {theirs[diff[0]:diff[0] + 8].hex(' ')}")
        else:
            print(f"data: {name}'s values from the ROM")
            text = new
    changed = text != open(f"{ROOT}/{dest}").read() or bool(renames or replace)
    if replace:
        text = re.sub(r"\b(" + "|".join(map(re.escape, replace)) + r")\b", lambda m: replace[m[0]], text)
    if changed:
        open(f"{ROOT}/{dest}", "w").write(text)
    if renames:
        rename_everywhere(renames)
        notes = {b: n for n, b in renames.items() if not PLACEHOLDER.match(n)}  # pokebw2's name over the definition
        text = comment_definitions(open(f"{ROOT}/{dest}").read(), notes)
        open(f"{ROOT}/{dest}", "w").write(text)
    if renames or replace:
        # a borrowed prototype (local_driver.py's borrow) under the Black name a header declares now is a second one
        new = set(renames.values()) | set(replace.values())
        words = set()
        for d in INCLUDE_DIRS:
            for p in glob.glob(f"{ROOT}/{d}/**/*.h", recursive=True):
                words |= new & set(re.findall(r"\w+", open(p, errors="replace").read()))
        text = open(f"{ROOT}/{dest}").read()
        pruned = prune_borrowed(text, lambda line: not words & set(re.findall(r"\w+", line)))
        if pruned != text:
            open(f"{ROOT}/{dest}", "w").write(pruned)
    return problems, changed


# Above the prototypes tools/local_driver.py's borrow() gives a file from pokebw2's headers
BORROWED_NOTE = ("// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until "
                 "tools/port_file.py\n// gives them Black's from the relocations")


def prune_borrowed(text, keep):
    """The C text with only the lines under BORROWED_NOTE that keep(line) holds, and without the note if none is."""
    at = text.find(BORROWED_NOTE)
    if at < 0:
        return text
    start = end = at + len(BORROWED_NOTE) + 1
    while end < len(text) and text[end] != "\n":  # one a line, up to the blank line after them
        end = text.index("\n", end) + 1
    kept = [line for line in text[start:end].splitlines(True) if keep(line)]
    if kept:
        return text[:start] + "".join(kept) + text[end:]
    return text[:at].rstrip("\n") + "\n" + text[end:]


LISTINGS = {}  # delinks.txt's text by module, for linked_name


def linked_name(r, mod, addr, name):
    """Whether the link finds name at Black's address addr of module mod though Black's symbols.txt names it otherwise:
    a complete C file here (linked from its object, not the delinked one) holds the address in its ranges and its
    object defines name there. mailsys.c defines MAILSYS_PROC_FUNCTIONS where Black has data_ov136_021c7768: check()
    took it for a name Black doesn't have and renamed it everywhere, mailsys.c's definition too, which finish.py left
    out of its commit, and the tree no longer linked (t39n)."""
    for f, ranges in r.delinks.get(mod, ({}, {}))[1].items():
        if not any(s <= addr & ~1 < e for s, e in ranges.values()):
            continue
        obj = f"{ROOT}/build/b_us/{os.path.splitext(f)[0]}.o"
        if mod not in LISTINGS:
            try:
                LISTINGS[mod] = chr(10) + open(f"{CONFIG}/{module_dir(mod)}/delinks.txt").read()
            except OSError:
                LISTINGS[mod] = ""
        if f"\n{f}:\n    complete\n" not in LISTINGS[mod] or not os.path.exists(obj):
            return False
        _, secs, syms, _ = read_object(obj)
        at = place(secs, ranges)
        return any(n == name and g and i in at and (at[i] + v) & ~1 == addr & ~1 for n, i, v, _, g, _ in syms)
    return False


def used_elsewhere(name, dest):
    """Whether a C file of src/ or lib/ other than dest has the name: it is some other Black symbol's."""
    found = subprocess.run(["grep", "-rlw", name, "--include=*.c", "src", *glob.glob(f"{ROOT}/lib/*/src")],
                           cwd=ROOT, capture_output=True, text=True).stdout.split()
    return any(os.path.abspath(f"{ROOT}/{f}") != os.path.abspath(f"{ROOT}/{dest}") for f in found)


def pair_by_order(b2, black, anchors):
    """Pairs the untwinned functions between twins when there are as many on both sides: [(B2 index, Black index)]."""
    out = []
    marks = [(-1, -1)] + sorted(anchors) + [(len(b2), len(black))]
    for (i0, j0), (i1, j1) in zip(marks, marks[1:]):
        if i1 - i0 == j1 - j0 and i1 > i0:
            out += [(i0 + k, j0 + k) for k in range(1, i1 - i0)]
    return out


class Names:
    """Black's symbol for each pokebw2 symbol the file uses."""

    def __init__(self, r, src, mod, ranges, bw2):
        self.black_at, self.black_where = read_names(CONFIG)
        self.b2_at, self.b2_where = read_names(f"{bw2}/config/b2_us/arm9")
        self.guesses = {l.split()[2] for l in open(f"{bw2}/config/names.txt") if l.strip() and l[0] != "#"}
        b2_mod, b2_secs = r.b2_files[src]
        self.b2_mod = b2_mod
        pairs = {}  # B2 (module, address) -> Black (module, address)

        # this file's twins, then its other functions in order between them
        twins = defaultdict(list)  # (pokebw2 source, B2 function) -> [Black (module, address)]
        for (bmod, addr), fns in r.twins.items():
            for s, fn in fns:
                twins[s, fn].append((bmod, addr))
        start, end = ranges[code_section(ranges)]
        for fn in (n for n, _ in r.b2_order(src)):
            inside = [c for c in twins[src, fn] if c[0] == mod and start <= c[1] < end]
            if len(inside) == 1 and self.b2_key(fn):
                pairs[self.b2_key(fn)] = inside[0]
        b2_code = b2_secs.get(code_section(b2_secs), (0, 0))  # (B2's file: .text, or .ltdmain as Black's)
        b2_funcs = [(a, sz) for a, sz, _ in r.b2_funcs[b2_mod] if b2_code[0] <= a < b2_code[1]]
        funcs = [(a, sz) for a, sz, _ in r.funcs[mod] if start <= a < end]
        index = {a: j for j, (a, _) in enumerate(funcs)}
        anchors = [(i, index[pairs[b2_mod, a][1]]) for i, (a, _) in enumerate(b2_funcs)
                   if pairs.get((b2_mod, a), ("", -1))[0] == mod and pairs[b2_mod, a][1] in index]
        ordered = pair_by_order(b2_funcs, funcs, anchors)
        for i, j in ordered:
            pairs[b2_mod, b2_funcs[i][0]] = (mod, funcs[j][0])
        self.defined = {(b2_mod, a) for a, _ in b2_funcs}
        # B2's functions with no Black function between two that have one, when every Black function of the range
        # has its B2 one (at the edges, the range may be short)
        have = [i for i, (a, _) in enumerate(b2_funcs) if (b2_mod, a) in pairs]
        paired = {pairs[b2_mod, b2_funcs[i][0]] for i in have}
        self.b2_only = [self.b2_at[b2_mod, a] for i, (a, _) in enumerate(b2_funcs) if (b2_mod, a) not in pairs
                        and have and have[0] < i < have[-1]] if all((mod, a) in paired for a, _ in funcs) else []

        # its data at the same offsets in their sections, if they are of the same size: MWCC orders data by size
        for sec, (s0, s1) in b2_secs.items():
            if sec not in CODE and sec in ranges and s1 - s0 == ranges[sec][1] - ranges[sec][0]:
                for (m, a), name in self.b2_at.items():
                    if m == b2_mod and s0 <= a < s1 and (mod, ranges[sec][0] + a - s0) in self.black_at:
                        pairs[m, a] = (mod, ranges[sec][0] + a - s0)
                        self.defined.add((m, a))

        # the targets of the relocations of its functions where B2's and Black's have the same size
        b2_relocs, relocs = read_relocs(f"{bw2}/config/b2_us/arm9"), r.relocs
        for i, j in anchors + ordered:
            (a, sz), (b, bsz) = b2_funcs[i], funcs[j]
            if sz != bsz:
                continue
            theirs = {f - a: (t, tm) for f, t, _, tm, _ in self.slice(b2_relocs[b2_mod], a, sz) if tm}
            for f, t, _, tm, _ in self.slice(relocs[mod], b, bsz):
                if f - b in theirs and tm:
                    bt, btm = theirs[f - b]
                    key = next(((btm, x) for x in (bt, bt - 1) if (btm, x) in self.b2_at), None)
                    val = next(((tm, x) for x in (t, t - 1) if (tm, x) in self.black_at), None)
                    if key and val and key not in self.defined:
                        pairs[key] = val
        # --pairs', which no twin shows (a file Black 2 builds as Thumb and Black as ARM: overlay 9's NitroWiFi)
        for key, val in PAIRS.items():
            if val in self.black_at:
                pairs.setdefault(key, val)
        # the twins anywhere else, but a tiny one can be any getter
        size = {(m, a): sz for m, fs in r.funcs.items() for a, sz, _ in fs}
        # but not, for a function of this file, a twin in another file of delinks.txt: code of the same shape, as
        # btl_server_flow_sub.c's func_ov167_021b02a0 and poke_type_pair.c's PokeTypePair_IsMonotype (two calls and a
        # compare), whose name then clashed with its header's prototype
        others = [(m, secs[code_section(secs)]) for m, (_, files) in r.delinks.items()
                  for f, secs in files.items() if code_section(secs)]
        for (s, fn), copies in twins.items():
            key = self.b2_key(fn, r.b2_files[s][0]) if s in r.b2_files else None
            if key in self.defined and copies and any(m == copies[0][0] and a <= copies[0][1] < b
                                                      for m, (a, b) in others):
                continue
            if key and len(copies) == 1 and size.get(copies[0], 0) >= TINY:
                pairs.setdefault(key, copies[0])
        self.pairs = pairs

    @staticmethod
    def slice(rows, start, size):
        froms = [row[0] for row in rows]
        return rows[bisect.bisect_left(froms, start):bisect.bisect_left(froms, start + size)]

    def b2_key(self, name, mod=None):
        """pokebw2's (module, address) of a name: in the given module (or the file's), else in main, else the one."""
        where = self.b2_where.get(name, [])
        where = [w for w in where if w[0] == (mod or self.b2_mod)] or [w for w in where if w[0] == "main"] or where
        return where[0] if len(where) == 1 else None


def rename_everywhere(renames):
    """Renames Black's symbols (whole words) in config/**/symbols.txt and in src/, lib/, include/. The files it changes
    go in build/renamed_files.txt, as tools/rename.py's: finish.py commits the C files among them with the file (t39n:
    the header's new name went in, mailsys.c's definition was left out and the tree did not link). Returns them."""
    paths = glob.glob(f"{CONFIG}/**/symbols.txt", recursive=True)
    for d in ("src", "lib", "include"):
        # not lib/<lib>/sdk/: another decompilation's headers, as they are (tools/port_lib.py)
        paths += [p for p in glob.glob(f"{ROOT}/{d}/**/*", recursive=True) if re.search(r"\.(c|h|inc|s)$", p)
                  and not re.search(r"/lib/[^/]+/sdk/", p)]
    word = re.compile(r"\b(" + "|".join(map(re.escape, renames)) + r")\b")
    changed = []
    for path in paths:
        text = open(path).read()
        new = word.sub(lambda m: renames[m[0]], text)
        if new != text:
            open(path, "w").write(new)
            changed.append(os.path.relpath(path, ROOT))
    if changed:
        os.makedirs(f"{ROOT}/build", exist_ok=True)
        with open(f"{ROOT}/build/renamed_files.txt", "a") as f:
            f.writelines(c + chr(10) for c in changed)
    return changed


def drop_function(text, name):
    """Removes a function's definition (with the comment lines above it) and its prototypes."""
    text = re.sub(rf"(?:^//[^\n]*\n)*^[^\n;]*\b{name}\s*\([^;{{]*\)\s*{{\n.*?^}}\n\n?", "", text, flags=re.M | re.S)
    return re.sub(rf"^[^\n]*\b{name}\s*\([^;{{]*\);\n", "", text, flags=re.M)


def comment_definitions(text, notes):
    """Puts `// pokebw2's <name>` above the definition of each renamed symbol: {Black name: pokebw2 name}."""
    lines = text.split("\n")
    for black, theirs in notes.items():
        for i, line in enumerate(lines):
            if re.match(rf"[^\s#/].*\b{black}\b", line) and not line.startswith("extern") and (
                    line.rstrip().endswith("{") or not re.search(rf"\b{black}\s*\(", line)):
                lines.insert(i, f"// pokebw2's {theirs}")
                break
    return "\n".join(lines)


# What a header declares: macros, functions, types, tags, variables, fields and enum constants (not parameters)
DECLARED = re.compile(r"#define\s+(\w+)|(\w+)(?=\s*[(;\[])|\(\s*\*\s*(\w+)\s*\)|(?:struct|union|enum)\s+(\w+)|"
                      r"^\s*(\w+)\s*(?:=|,|$)", re.M)


KEYWORDS = {"break", "case", "char", "const", "continue", "default", "do", "else", "enum", "extern", "for", "goto", "if",
            "inline", "int", "long", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
            "union", "unsigned", "void", "volatile", "while"}


def missing_declarations(text, theirs, bw2):
    """The names the C uses that pokebw2's headers declare and Black's don't: ["name (pokebw2's header)"]."""
    black = set()
    for d in INCLUDE_DIRS:
        for path in glob.glob(f"{ROOT}/{d}/**/*.h", recursive=True):
            black |= set(re.findall(r"\w+", open(path, errors="replace").read()))
    where = {}
    for path in sorted(glob.glob(f"{bw2}/include/**/*.h", recursive=True) +
                       glob.glob(f"{bw2}/lib/*/include/**/*.h", recursive=True)):
        for w in {w for ws in DECLARED.findall(open(path, errors="replace").read()) for w in ws} - {""}:
            where.setdefault(w, re.sub(r".*/include/", "", path))
    out = []
    for w in sorted(set(re.findall(r"\b[A-Za-z_]\w*\b", text)) - KEYWORDS):
        if w not in black and theirs.get(w, w) in where:
            out.append(f"{w} ({where[theirs.get(w, w)]})")
    return out[:16] + ([f"{len(out) - 16} more"] if len(out) > 16 else [])


def run(command):
    p = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    return p.returncode, p.stdout + p.stderr


def overlay_ids(r):
    """{pokebw2's overlay number: Black's}, for each pokebw2 overlay that has most of its twins, at least 3, in one of
    Black's overlays (pokebw2's 176 to 180, irc_compatible.c's procs, are Black's 102 to 106)."""
    votes = defaultdict(lambda: defaultdict(int))
    for (mod, _), pairs in r.twins.items():
        for src, _ in pairs:
            if src in r.b2_files:
                votes[r.b2_files[src][0]][mod] += 1
    out = {}
    for b2, theirs in votes.items():
        mod, n = max(theirs.items(), key=lambda kv: kv[1])
        if re.fullmatch(r"ov\d+", b2) and re.fullmatch(r"ov\d+", mod) and n >= 3 and 2 * n > sum(theirs.values()):
            out[int(b2[2:])] = int(mod[2:])
    return out


def ported(r, src, mod, ranges, dest, bw2, names=None):
    """pokebw2's C of the file (going to dest) with Black's names (the pairing of names, a Names, else this file's):
    (the text, {pokebw2's name: Black's}, {Black's placeholder: swan's name}, which rename_everywhere gives Black's
    symbols)."""
    names = names or Names(r, src, mod, ranges, bw2)
    text = open(f"{bw2}/{src}").read()
    replace, comments, renames, unknown = {}, {}, {}, []
    for n in sorted(set(re.findall(r"\b[A-Za-z_]\w*\b", text))):
        key = names.b2_key(n)
        if key is None:
            continue
        black = names.black_at.get(names.pairs.get(key))
        if black is None:
            if n not in names.black_where and re.search(r"[A-Z_]", n):
                unknown.append(n)
            continue
        if black == n:
            continue
        if n in names.guesses or PLACEHOLDER.match(n):
            replace[n] = black
            if key in names.defined and not PLACEHOLDER.match(n):
                comments[black] = n
        elif PLACEHOLDER.match(black) and n not in names.black_where and not used_elsewhere(n, dest):
            renames[black] = n  # swan's name, which Black takes
        else:
            replace[n] = black
            print(f"names: pokebw2's {n} is Black's {black}")
    # Not onto a name the file keeps, or one that two of its own symbols land on: their data moved in Black
    words = set(re.findall(r"\b[A-Za-z_]\w*\b", text))
    own = [b for n, b in replace.items() if names.b2_key(n) in names.defined]
    replace = {n: b for n, b in replace.items() if (b not in words or b in replace) and
               not (names.b2_key(n) in names.defined and own.count(b) > 1)}
    comments = {b: n for b, n in comments.items() if n in replace}
    # Functions only B2 has, which nothing else in the file calls
    dropped = [n for n in names.b2_only if not re.search(rf"\b{n}\b", drop_function(text, n))]
    for n in dropped:
        text = drop_function(text, n)
    if dropped:
        print(f"names: dropped {', '.join(dropped)}, which Black doesn't have")
    if replace:
        text = re.sub(r"\b(" + "|".join(map(re.escape, replace)) + r")\b", lambda m: replace[m[0]], text)
    # pokebw2's overlay numbers, which the procs it loads and their tables give, are Black's where the twins tell
    ids = overlay_ids(r)
    used = sorted({int(n) for n in re.findall(r"\bOVERLAY_ID\((\d+)\)", text)})
    moved = {n: ids[n] for n in used if n in ids and ids[n] != n}
    if moved:
        text = re.sub(r"\bOVERLAY_ID\((\d+)\)", lambda m: f"OVERLAY_ID({moved.get(int(m[1]), int(m[1]))})", text)
        pairs = ", ".join(f"{a} is Black's {b}" for a, b in moved.items())
        print(f"overlays: pokebw2's {pairs}, from the twins")
    text = comment_definitions(text, comments)
    head = f"// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), {src}, ported by tools/port_file.py.\n"
    if comments:
        head += "// The names are Black's; pokebw2's own guesses are in the comments.\n"
    unknown = [n for n in unknown if n not in dropped]
    print(f"names: {len(replace)} of pokebw2's replaced by Black's, {len(renames)} of swan's given to Black's "
          f"symbols" + (f"; not found in Black: {', '.join(unknown)}" if unknown else ""))
    return head + text, replace, renames


def note_structs(dest, src, bw2):
    """Notes the structs the C file defines itself as tools/port_decls.py notes those it copies into headers: Black 2's,
    but for the fields the file's twinned functions access (pokebw2's C of a twin compiles to Black's code)."""
    import port_decls as pd
    rows = [row for path in pd.default_twins() for row in csv.DictReader(open(path), delimiter="\t")]
    twinned = {row["bw2_function"] for row in rows if row["bw2_source"] == src and (
        int(row["size"]) >= pd.TINY or row["black_symbol"] == row["bw2_function"])}
    evidence = {"fields": defaultdict(set), "records": {}, "sizes": [], "names": []}
    ast = pd.clang_ast(bw2, src) if shutil.which("clang") else None
    if ast:
        owner, evidence["records"], _ = pd.ast_records(ast)
        for fields, _, sizes in pd.ast_functions(ast, owner, twinned).values():
            for record, field in fields:
                evidence["fields"][record].add(field)
            evidence["sizes"] += sorted(sizes)
    planner = pd.Planner({}, {}, evidence)
    text = open(f"{ROOT}/{dest}").read()
    for item in pd.parse(f"{ROOT}/{dest}"):
        if item.struct and not item.fwd and "{" in item.code() and "Black 2's" not in item.comment:
            note = planner.note(item)
            if note and text.count(item.text) == 1:
                text = text.replace(item.text, f"{note}\n{item.text}")
    open(f"{ROOT}/{dest}", "w").write(text)


def port(r, src, mod, ranges, dest, bw2):
    """Writes the C with Black's names and adds it to delinks.txt, incomplete. Returns what the headers lack."""
    text, replace, renames = ported(r, src, mod, ranges, dest, bw2)
    os.makedirs(os.path.dirname(f"{ROOT}/{dest}"), exist_ok=True)
    open(f"{ROOT}/{dest}", "w").write(text)
    note_structs(dest, src, bw2)
    text = open(f"{ROOT}/{dest}").read()
    if renames:
        rename_everywhere(renames)

    sections = [f"{s}:{a:#010x}-{b:#010x}" for s, (a, b) in ranges.items()]
    code, out = run([sys.executable, "tools/add_source_file.py", dest, module_dir(mod), *sections, "--incomplete"])
    if code:
        sys.exit(out)
    run([sys.executable, "configure.py"])
    missing = [h for h in re.findall(r'^#include\s+["<]([^">]+)[">]', text, re.M)
               if not any(os.path.exists(f"{ROOT}/{d}/{h}") for d in INCLUDE_DIRS + [os.path.dirname(dest)])]
    return missing, missing_declarations(text, {v: k for k, v in replace.items()}, bw2)


def compile_errors(out):
    return [f"    {l.strip()[:160]}" for l in out.splitlines() if re.match(r"\S+\.c:\d+: (?!warning)", l)]


def trim_initializers(dest, out):
    """A struct Black has shorter, or a table it has shorter, than pokebw2's: the last value of each initializer that
    has too many goes (the check against the ROM's bytes then fixes the rest). btlv_effvm.c's VMInitParam, whose
    extraCommands, extraCommandCount and extraCommandStart are Black 2's. Returns whether it trimmed one."""
    bad = sorted({int(n) for n in re.findall(r"^\S+\.c:(\d+): too many initializers", out, re.M)}, reverse=True)
    if not bad:
        return False
    text = open(f"{ROOT}/{dest}").read()
    masked = re.sub(r"//[^\n]*|/\*.*?\*/|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'",
                    lambda c: re.sub(r"[^\n]", " ", c[0]), text, flags=re.S)
    starts = [0] + [m.end() for m in re.finditer(r"\n", masked)]
    trimmed = False
    for n in bad:
        if n > len(starts):
            continue
        line = masked[starts[n - 1]:starts[n] if n < len(starts) else len(masked)]
        k = line.find("{")
        if k < 0:  # the brace list that holds the line's start
            k, depth = starts[n - 1], 0
            while k > 0 and depth >= 0:
                k -= 1
                depth += {"}": 1, "{": -1}.get(masked[k], 0)
            if depth >= 0:
                continue
        else:
            k += starts[n - 1]
        depth, commas, j = 0, [], k
        for j in range(k, len(masked)):
            c = masked[j]
            depth += {"{": 1, "(": 1, "[": 1, "}": -1, ")": -1, "]": -1}.get(c, 0)
            if c == "," and depth == 1:
                commas.append(j)
            if depth == 0:
                break
        body_end = j
        while commas and not masked[commas[-1] + 1:body_end].strip():  # a trailing comma
            body_end = commas.pop()
        if not commas:
            continue
        while masked[body_end - 1].isspace():  # the space before the brace stays
            body_end -= 1
        text, masked = text[:commas[-1]] + text[body_end:], masked[:commas[-1]] + masked[body_end:]
        trimmed = True
    if trimmed:
        open(f"{ROOT}/{dest}", "w").write(text)
        print(f"initializers: the last value of those at lines {', '.join(map(str, bad))} dropped, too many for Black")
    return trimmed


def build(r, mod, ranges, dest, bw2, missing=(), undeclared=()):
    """Compiles the file, gives its statics Black's names, checks its data and relocations against the ROM (fixing
    what the ROM tells), and marks it complete if it matches. Returns 0 if so."""
    obj = f"build/b_us/{os.path.splitext(dest)[0]}.o"
    code, out = run(["ninja", obj])
    for _ in range(8):
        if not code or not trim_initializers(dest, out):
            break
        code, out = run(["ninja", obj])
    if code:
        errors = compile_errors(out)
        print(f"FAILED {dest}: does not compile ({len(errors)} errors)")
        if missing:
            print(f"    missing headers: {', '.join(missing)}")
        if undeclared:
            print(f"    in pokebw2's headers, not in Black's: {', '.join(undeclared)}")
        print("\n".join(errors[:6]))
        return 1

    for _ in range(4):
        # Static data take the dsd names of where they land; statics that code and data elsewhere reach become global
        black_at = read_names(CONFIG)[0]
        _, secs, syms, _ = read_object(f"{ROOT}/{obj}")
        sizes = defaultdict(int)
        for name, kind, _, size, _ in secs:
            sizes[name] += size if kind in (1, 8) else 0
        lost = [f"{sec} ({n:#x} bytes)" for sec, n in sizes.items() if n and sec in DATA and sec not in ranges]
        if lost:
            print(f"FAILED {dest}: delinks.txt gives it no {', '.join(lost)}; the link would drop them")
            return 1
        at = place(secs, ranges)
        later = {}
        for name, ndx, value, size, is_global, is_func in syms:
            black = black_at.get((mod, at[ndx] + value)) if ndx in at and not is_func else None
            if black and black != name and not is_global and re.match(r"[A-Za-z_]\w*$", name):
                later[name] = black
        # Not a name the file keeps, or one that two of its statics land on: their data moved in Black. The file's
        # text too: a static the compile dropped, its users gone, has no symbol but keeps its name
        # (btl_server_flow_sub.c's item table landed where its EV table, unused then, has its name: two
        # data_ov093_021efdc0)
        words = set(re.findall(r"\b[A-Za-z_]\w*\b", open(f"{ROOT}/{dest}").read()))
        later = {n: b for n, b in later.items() if (b not in {s[0] for s in syms} and b not in words or b in later)
                 and list(later.values()).count(b) == 1}
        outside = reached(r, mod, ranges)
        exported = sorted({later.get(n, n) for n, i, v, _, g, _ in syms
                           if not g and i in at and (at[i] + v) & ~1 in outside and re.match(r"[A-Za-z_]\w*$", n)})
        if later or exported:
            text = open(f"{ROOT}/{dest}").read()
            if later:
                text = re.sub(r"\b(" + "|".join(map(re.escape, later)) + r")\b", lambda m: later[m[0]], text)
            text = comment_definitions(text, {b: n for n, b in later.items() if not PLACEHOLDER.match(n)})
            for name in exported:
                text = re.sub(rf"^static\s+(?=[^;=\n]*\b{name}\b)", "", text, flags=re.M)
            open(f"{ROOT}/{dest}", "w").write(text)
            print(f"names: {len(later)} static data take Black's names; made global: {', '.join(exported) or 'none'}")
            code, out = run(["ninja", obj])
            if code:
                print(f"FAILED {dest}: does not compile after renaming its data\n" + "\n".join(compile_errors(out)[:6]))
                return 1

        # Data and relocations against the ROM, fixed where it tells how
        problems, changed = check(r, mod, ranges, dest, obj, bw2)
        if not changed:
            break
        code, out = run(["ninja", obj])
        if code:
            print(f"FAILED {dest}: the fixes from the ROM break the compile\n" + "\n".join(compile_errors(out)[:6]))
            return 1

    # Functions, the rest of the object, then the whole build
    code, out = run([f"{ROOT}/.venv/bin/python", "tools/compiler_probe.py", dest])
    lines = out.splitlines()
    k = max((i for i, l in enumerate(lines) if l.startswith("function")), default=len(lines))
    rows = [l.split(None, 1) for l in lines[k + 1:] if l.strip()]
    # A static function that nothing calls and Black doesn't have isn't emitted, as in pokebw2: it stays for its data
    start, end = ranges[code_section(ranges)]
    theirs = {n for (m, a), n in read_names(CONFIG)[0].items() if m == mod and start <= a < end}
    rows = [(n, s) for n, s in rows if s.strip() != "not emitted" or n in theirs]
    bad = [f"{n} {s.strip()}" for n, s in rows if s.strip() != "MATCH"]
    if bad or not rows:
        print(f"FAILED {dest}: {len(rows) - len(bad)}/{len(rows)} functions match; the others:\n    "
              + "\n    ".join(bad or out.splitlines()[-5:]))
        return 1
    if problems:
        print(f"FAILED {dest}: every function matches, but not the object: {problems[0][:150]}" +
              "".join(f"\n    {p[:200]}" for p in problems[1:12]))
        return 1
    run([sys.executable, "tools/add_source_file.py", dest, module_dir(mod)])
    code, out = run(["ninja"])
    if code:
        delinks = f"{CONFIG}/{module_dir(mod)}/delinks.txt"
        lines = open(delinks).read().split("\n")
        del lines[lines.index(f"{dest}:") + 1]  # complete
        open(delinks, "w").write("\n".join(lines))
        # The linker's or dsd's error (a dsd backtrace follows some), and for a module that differs, the file's own's
        # first difference
        shown = [l for l in out.splitlines() if not l.startswith(("[", "FAILED:", "   ")) and re.search(
            r"ndefined|panicked|rror|didn't pass", l)][:8] or out.splitlines()[-12:]
        if os.path.exists(f"{ROOT}/build/b_us/build/rom_config.yaml") and "Checking modules" in out:
            built, rom = module_bytes(r, mod, built=True)[0], module_bytes(r, mod)[0]
            base = r.delinks[mod][0][".text"][0]
            k = next((k for k in range(min(len(built), len(rom))) if built[k] != rom[k]), min(len(built), len(rom)))
            syms = sorted((a, n) for (m, a), n in read_names(CONFIG)[0].items() if m == mod and a <= base + k)
            shown.insert(0, f"{mod} differs first at {base + k:#x} ({syms[-1][1] if syms else '?'}), "
                            f"{sum(a != b for a, b in zip(built, rom))} bytes; {len(built):#x} bytes, the ROM's "
                            f"{len(rom):#x}" if built != rom else f"{mod} is the ROM's: another module differs")
        print(f"FAILED {dest}: every function matches, but not the build: {shown[0][:150] if shown else '?'}" +
              "".join(f"\n    {l[:200]}" for l in shown[1:]))
        return 1
    print(f"MATCHED {len(rows)}/{len(rows)} {dest}: complete, and ninja passes")
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("source", help="pokebw2 source, e.g. src/gfl/assert.c")
    ap.add_argument("module", help="Black's module: main or ovNNN (or its directory, . or overlays/ovNNN)")
    ap.add_argument("ranges", nargs="*", help="section ranges as for add_source_file.py; default: file_ranges.py's")
    ap.add_argument("--bw2", default=os.path.expanduser("~/ref/pokebw2"), help="pokebw2 checkout (read only)")
    ap.add_argument("--pairs", help="a TSV of B2 module, B2 address, Black module, Black address: symbols to pair "
                    "that no twin shows, such as those of a file whose code is Thumb in Black 2 and ARM in Black")
    args = ap.parse_args()
    if args.pairs:
        for line in open(args.pairs):
            b2_mod, b2_addr, mod_, addr = line.split()
            PAIRS[b2_mod, int(b2_addr, 16)] = (mod_, int(addr, 16))

    mod = {".": "main"}.get(args.module, os.path.basename(args.module))
    r = Ranges(args.bw2, CONFIG, f"{ROOT}/build/twins_b_us.tsv")
    dest = black_path(args.source, mod)
    if os.path.exists(f"{ROOT}/{dest}"):  # ported before: build it again, after a worker's fixes (or in new ranges)
        if args.ranges:
            code, out = run([sys.executable, "tools/add_source_file.py", dest, module_dir(mod), *args.ranges,
                             "--incomplete"])
            if code:
                sys.exit(out)
            run([sys.executable, "configure.py"])
            r = Ranges(args.bw2, CONFIG, f"{ROOT}/build/twins_b_us.tsv")
        return build(r, mod, r.delinks[mod][1][dest], dest, args.bw2)
    if args.ranges:
        ranges = {}
        for a in args.ranges:
            m = re.match(r"(\.\w+):(0x\w+)-(0x\w+)$", a)
            ranges[m[1]] = (int(m[2], 16), int(m[3], 16))
    else:
        rows = read_rows(f"{ROOT}/build/twin_files.tsv")
        if args.source not in rows:
            sys.exit(f"{args.source}: no row in build/twin_files.tsv; give the ranges")
        found, ranges, notes = propose(r, rows, args.source)
        if found != mod:
            sys.exit(f"{args.source}: twin_files.tsv puts it in {found}, not {mod}")
        for n in notes:
            print(f"ranges: {n}")
    return build(r, mod, ranges, dest, args.bw2, *port(r, args.source, mod, ranges, dest, args.bw2))


if __name__ == "__main__":
    sys.exit(main())
