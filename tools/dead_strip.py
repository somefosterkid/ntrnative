#!/usr/bin/env python3
"""Make the link dead-strip as the original's did: keep everything Black has, drop what only our C has.

    python3 tools/dead_strip.py build/b_us/arm9.lcf build/b_us/objects.txt build/b_us/arm9.ds.lcf build/b_us/objects.ds.txt

The original link dropped what nothing reached: a function nothing calls is gone from the ROM, but its initializers and
string literals took their places in its file's sorted data before the link (tools/data_order.py), so its C is how a
file gets Black's data order (delivery_beacon.c's "" before its file name). The build links with dead-stripping, and
this writes the linker script with every symbol Black has forced in (FORCE_ACTIVE, as nothing reaches some of them: proc
tables only other overlays load, unused const tables): every global of dsd's delinked objects, and every global of our
compiled objects but the functions symbols.txt lacks and the data Black's file lacks (foreign(): a global past the
file's range, or in the place of another of its symbols), which the link drops when nothing uses them. A delinked
section whose first bytes no global symbol covers (main's crt0, autoloads) gets one, `_keep_<n>`, in a copy of its
object under build/b_us/keep, which the objects list names instead. A label of symbols.txt (`kind:label`, a second name
inside or at the start of a function: fp.c's _dadd at __aeabi_dadd, _ll_sdiv inside _ll_mod) is in dsd's delinked object
but not in our compiled one, as an asm function's labels are its own: the copy of a compiled object gets it too, at its
offset in the function that holds it, unless an object defines it already.

It also defines the SDK lcf's symbols that dsd's script lacks: the stack sizes and arenas (LINKER_SYMBOLS), and the
autoload lists' addresses that crt0.c's module parameters hold (autoload_symbols(): SDK_AUTOLOAD_LIST, _LIST_END and the
LTD module's SDK_LTDAUTOLOAD_START, _LIST, _LIST_END), computed from the sections as the SDK's lcf does.
"""
import glob
import os
import re
import struct
import sys
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from file_ranges import read_delinks  # noqa: E402
from port_file import CONFIG, read_object  # noqa: E402
from twin_files import module_of  # noqa: E402

KEPT = (".text", ".init", ".rodata", ".data", ".bss", ".ctor")


def add_symbols(path, out, symbols):
    """Writes a copy of a relocatable ELF with global symbols [(name, section index[, offset])], at the sections'
    starts or the offsets: a new .strtab and .symtab after the old data, their section headers pointing there."""
    d = bytearray(open(path, "rb").read())
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum, _ = struct.unpack_from("<HHH", d, 0x2E)
    sh = [list(struct.unpack_from("<10I", d, shoff + i * shentsize)) for i in range(shnum)]
    symtab = next(i for i, s in enumerate(sh) if s[1] == 2)
    strtab = sh[symtab][6]
    names = d[sh[strtab][4]:sh[strtab][4] + sh[strtab][5]]
    syms = d[sh[symtab][4]:sh[symtab][4] + sh[symtab][5]]
    for name, section, *offset in symbols:
        syms += struct.pack("<IIIBBH", len(names), offset[0] if offset else 0, 0, 0x10, 0, section)  # GLOBAL NOTYPE
        names += name.encode() + b"\0"
    tail = len(d)
    sh[strtab][4], sh[strtab][5] = tail, len(names)
    sh[symtab][4], sh[symtab][5] = tail + len(names), len(syms)
    d += names + syms
    d[shoff:shoff + shnum * shentsize] = b"".join(struct.pack("<10I", *s) for s in sh)  # headers stay where they are
    open(out, "wb").write(d)


# The TwlSDK's linker-script symbols Black's code loads as values (its SDK's lcf defines them): the system and IRQ
# stack sizes, the DTCM and extended arenas' starts and the count of NTR overlays (os_thread.c, os_arena.c, fs_overlay.c)
LINKER_SYMBOLS = {"SDK_SYS_STACKSIZE": 0x1800, "SDK_IRQ_STACKSIZE": 0x800, "SDK_SECTION_ARENA_DTCM_START": 0x2fe00c0,
                  "SDK_SECTION_ARENA_EX_START": 0xd000000, "SDK_OVERLAY_NUMBER": 0xed}


# Each autoload's entry in a module's autoload list: its address, its size, its static initializers' start and its .bss
# size (a TWL list; NitroSDK's has no initializers word), as the ROM builder writes it after the module
AUTOLOAD_ENTRY = 16


def ltd_start(lcf):
    """Where the LTD module's autoload blocks start: after the ARM9i static module (TwlSDK's .ltdmain at 0x02400000,
    Black's only its 4-byte .ctor end), which the ROM build takes from extract/<version>/dsi as it is. None without
    a DSi part."""
    version = os.path.basename(os.path.dirname(os.path.abspath(lcf)))
    dsi = os.path.join("extract", version, "dsi")
    if not os.path.exists(os.path.join(dsi, "dsi.yaml")) or not os.path.exists(os.path.join(dsi, "arm9i.bin")):
        return None
    base = re.search(r"^arm9i:\n(?:  .*\n)*?  base_address: (\d+)", open(os.path.join(dsi, "dsi.yaml")).read(), re.M)
    return base and int(base[1]) + os.path.getsize(os.path.join(dsi, "arm9i.bin"))


def autoload_symbols(text, ltd=None):
    """The SDK's lcf symbols for the modules' autoload lists, which crt0's module parameters hold (BuildInfo,
    the LTD module's): SDK_AUTOLOAD_LIST and _END, after the autoload blocks that follow the static module's .data
    (ARM9_DATA_END), and SDK_LTDAUTOLOAD_START, _LIST and _END for the LTD module's (ltd: where its blocks start). A
    block's size is its section's .data part, from its first label to its .bss (dsd's lcf: ITCM_TEXT_START,
    DTCM_DATA_START ... X_BSS_START). Lines to put at the end of SECTIONS, where the labels are defined."""
    blocks = {"main": [], "ltd": []}
    for m in re.finditer(r"^\s*\.\w+ : \{\n(.*?)^\s*\} > (\w+)$", text, re.M | re.S):
        if m[2] == "ARM9" or re.fullmatch(r"OV\d+", m[2]):
            continue
        start = re.search(r"^\s*(\w+_START) = \.;", m[1], re.M)
        bss = re.search(r"^\s*(\w+_BSS_START) = \.;", m[1], re.M)
        if start and bss:
            blocks["ltd" if m[2].startswith("LTD_") else "main"].append(f"({bss[1]} - {start[1]})")
    out = []
    if blocks["main"]:
        out += [f"SDK_AUTOLOAD_LIST = ARM9_DATA_END + {' + '.join(blocks['main'])};",
                f"SDK_AUTOLOAD_LIST_END = SDK_AUTOLOAD_LIST + {len(blocks['main']) * AUTOLOAD_ENTRY:#x};"]
    if blocks["ltd"] and ltd is not None:
        out += [f"SDK_LTDAUTOLOAD_START = {ltd:#x};",
                f"SDK_LTDAUTOLOAD_LIST = SDK_LTDAUTOLOAD_START + {' + '.join(blocks['ltd'])};",
                f"SDK_LTDAUTOLOAD_LIST_END = SDK_LTDAUTOLOAD_LIST + {len(blocks['ltd']) * AUTOLOAD_ENTRY:#x};"]
    return out


DATA = (".rodata", ".data", ".bss")


def ltdmain_bss_sections(d):
    """The indices of a relocatable ELF's .ltdmain.bss sections, which read_object() names .bss: the link puts them
    after the file's .bss (ltdmain_bss()), in the LTD module's .bss when the file's .bss is main's (card_rom.c)."""
    if b".ltdmain.bss" not in d:
        return set()
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", d, 0x2E)
    names = struct.unpack_from("<10I", d, shoff + shstrndx * shentsize)[4]
    return {i for i in range(shnum) if d.startswith(b".ltdmain.bss\0", names + struct.unpack_from(
        "<I", d, shoff + i * shentsize)[0])}


def foreign(secs, syms, ranges, at, black, defined, ltd=()):
    """The global data of a compiled object that Black's file lacks, which Black's link dropped as nothing uses it
    (foundation.c's DWCi_A_SCR, t37e): each global of a data section of its own (MWCC's) under a name symbols.txt
    (black) lacks, placed in the file's range (ranges: {section: (start, end)}) as place() does, ends past the range or
    starts where another symbol of the module (at: {address: [names]}) that our objects define (defined) is. Its
    section takes no room, as the link drops it; the others are Black's, which the link keeps, used or not. The
    sections ltd (.ltdmain.bss, after the file's .bss) are Black's."""
    out, end, own, held = set(), {}, {x[0] for x in syms}, defaultdict(list)
    for x in syms:
        if x[0] and x[0][0] not in ".$":
            held[x[1]].append(x)
    for i, (name, kind, _, size, align) in enumerate(secs):
        if kind not in (1, 8) or name not in DATA or name not in ranges or i in ltd:
            continue
        start = -(-end.get(name, ranges[name][0]) // max(4, align)) * max(4, align)
        one = held[i][0] if len(held[i]) == 1 else None
        if one and one[4] and not one[5] and one[0] not in black and (start + size > ranges[name][1] or any(
                n != one[0] and (n in defined or n in own) for n in at.get(start, ()))):
            out.add(one[0])
            continue
        end[name] = start + size
    return out


def labels(paths):
    """symbols.txt's global labels by the function of their module that holds them: {function: [(label, offset)]}."""
    out = defaultdict(list)
    for p in paths:
        funcs, found = [], []
        for line in open(p):
            f = line.split()
            addr = next((int(x[5:], 16) for x in f if x.startswith("addr:")), None)
            if len(f) < 3 or addr is None:
                continue
            size = re.search(r"size=(0x[0-9a-fA-F]+)", f[1])
            if f[1].startswith("kind:function") and size:
                funcs.append((addr, addr + int(size.group(1), 16), f[0]))
            elif f[1].startswith("kind:label") and "local" not in f[2:] and re.fullmatch(r"[A-Za-z_]\w*", f[0]):
                found.append((f[0], addr))
        for name, addr in found:
            holder = [(a, n) for a, e, n in funcs if a <= addr < e]
            if len(holder) == 1:
                out[holder[0][1]].append((name, addr - holder[0][0]))
    return out


def ltdmain_bss(text):
    """The linker script with each object's .ltdmain.bss after its .bss in the LTD autoloads: TwlSDK's
    twl/ltdmain_begin.h sends the uninitialised data after it there (`#pragma define_section LTDMAIN ".ltdmain"
    ".ltdmain.bss"`), and dsd's script lists only .bss, so the link left it out (t34d). The file's .bss range in
    delinks.txt holds both, its .bss first, as tools/port_file.py's read_object() has them."""
    def block(m):
        ltd = set(re.findall(r"^\s*((?!_dsd_gap@)\S+\.o)\(\.ltdmain\)$", m[0], re.M))  # our files with LTD code
        return re.sub(r"^(\s*)(\S+\.o)\(\.bss\)$", lambda b: f"{b[0]}\n{b[1]}{b[2]}(.ltdmain.bss)" if b[2] in ltd
                      else b[0], m[0], flags=re.M)
    return re.sub(r"^\s*\.ltd_\w+ : \{\n.*?^\s*\} > LTD_\w+$", block, text, flags=re.M | re.S)


def main():
    lcf, objects, out_lcf, out_objects = sys.argv[1:5]
    # the version's own config and delinked objects (build/<version>/arm9.lcf): Black's, or White's (docs/white.md)
    version = os.path.basename(os.path.dirname(os.path.abspath(lcf)))
    config = CONFIG.replace("/config/b_us/", f"/config/{version}/")
    tables = glob.glob(f"{config}/**/symbols.txt", recursive=True)
    black = {line.split()[0] for p in tables for line in open(p) if line.split()}
    held = labels(tables)
    paths = [line.strip().strip('"') for line in open(objects) if line.strip()]
    objs = [(path, read_object(path)) for path in paths]
    defined = {x[0] for _, (_, secs, syms, _) in objs for x in syms if x[4] and 0 < x[1] < len(secs)}
    at, files = defaultdict(lambda: defaultdict(list)), defaultdict(list)
    for p in tables:
        for line in open(p):
            addr = next((int(x[5:], 16) for x in line.split() if x.startswith("addr:")), None)
            if addr is not None:
                at[module_of(p)][addr].append(line.split()[0])
    for mod, (_, sources) in read_delinks(config).items():
        for src, ranges in sources.items():
            files[os.path.splitext(src)[0]].append((mod, ranges))
    force, listed, kept = set(), [], 0
    for path, (d, secs, syms, relocs) in objs:
        delinked = path.startswith(f"build/{version}/delinks/")
        stem = re.match(r"build/[^/]+/(.*)[.]o$", path)
        mods = [] if delinked or not stem else files.get(stem[1], [])
        # a section two modules' ranges have (a two-module file's) is neither's to judge
        dropped = set().union(*(foreign(secs, syms, {k: v for k, v in ranges.items() if not any(
            k in other for m, other in mods if m != mod)}, at[mod], black, defined, ltdmain_bss_sections(d))
            for mod, ranges in mods))
        for name, i, value, size, glob_, func in syms:
            if glob_ and name and 0 < i < len(secs) and re.fullmatch(r"[A-Za-z_]\w*", name) and \
                    (delinked or name in black or not func and name not in dropped):
                force.add(name)
        bare = [i for i, s in enumerate(secs) if delinked and s[0] in KEPT and s[3] and not any(
            x[1] == i and x[2] == 0 and x[4] and re.fullmatch(r"[A-Za-z_]\w*", x[0]) for x in syms)]
        aliases = [] if delinked else [(label, i, value + offset) for name, i, value, size, glob_, func in syms
                                         if glob_ and func and 0 < i < len(secs) for label, offset in held.get(name, ())
                                         if label not in defined]
        defined |= {n for n, *_ in aliases}
        if bare or aliases:
            keep = [(f"_keep_{kept + k}", i) for k, i in enumerate(bare)]
            kept += len(keep)
            keep += aliases
            copy = os.path.join(os.path.dirname(out_objects), "keep", path)
            os.makedirs(os.path.dirname(copy), exist_ok=True)
            add_symbols(path, copy, keep)
            force |= {k[0] for k in keep}
            path = copy
        listed.append(path)
    text = ltdmain_bss(open(lcf).read()).replace("SECTIONS {", "FORCE_ACTIVE {\n" + ",\n".join(f"    {n}" for n in sorted(force))
                                    + "\n}\n\nSECTIONS {\n"
                                    + "".join(f"    {k} = {v:#x};\n" for k, v in LINKER_SYMBOLS.items()), 1)
    # the autoload lists' symbols, after the sections that define the labels they're made of
    end = text.rstrip().rfind("}")
    text = text[:end] + "".join(f"    {line}\n" for line in autoload_symbols(text, ltd_start(lcf))) + text[end:]
    open(out_lcf, "w").write(text)
    open(out_objects, "w").write("".join(f'"{p}"\n' for p in listed))


if __name__ == "__main__":
    main()
