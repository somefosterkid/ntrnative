#!/usr/bin/env python3
"""The PC build's overlay map (port/src/ovl.c reads it): a linker script that gathers each overlay's writable data
into ranges the runtime can reset, with the DS's overlay table.

On the DS an overlay is loaded into memory it shares with others, and each load (FS_LoadOverlay) brings its .data
back to the values the ROM has and zeroes its .bss. The PC build links every overlay into the one exe, so nothing
resets them by itself. The script puts the .data and the .bss (with -fcommon's COMMON symbols) of each overlay's
objects (the C files config/b_us/arm9/overlays/ovNNN/delinks.txt lists) into two output sections in overlay order,
.ovldata and .ovlbss, and writes a table, .ovltab, of each overlay's offsets into them and its DS layout (address,
image size, bss size) that port/src/ovl.c reads: it copies .ovldata at startup, and FS_LoadOverlay copies an
overlay's range back and zeroes its .bss range.

It is an INSERT script: the linker's default script stays, `-Wl,-T,<script>` adds to it (GNU ld, ELF and mingw's
PE). An object is matched by the end of its name, meson's mangled source path (`libgame.a(_root_..._src_ov021_
field.c.o)`), so the patterns don't depend on the checkout or the build dir. Const data stays where it was: on ELF
PIE a const table of pointers is .data.rel.ro (read-only once relocated), on PE it is .rdata.

    python3 tools/port_overlays.py --target linux -o build/port/linux/ovl.ld
    python3 tools/port_overlays.py --target win64 -o build/port/win64/ovl.ld
    python3 tools/port_overlays.py --list                 # overlay, DS address, image and bss size, files
    python3 tools/port_overlays.py --check                # the DS layout against extract/'s overlay table (the ROM's)
    python3 tools/port_overlays.py --check-map MAP        # a link map (-Wl,-Map=MAP): every overlay object's .data
                                                          # and .bss in .ovldata/.ovlbss, and no other object's

port/meson.build passes the script to the link and -DBWPORT_OVL_SCRIPT to the C (without them ovl.c resets nothing).
"""
import argparse
import re
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ROM_TABLE = ROOT / "extract/b_us/arm9_overlays/overlays.yaml"
SECTION = re.compile(r"^\s+(\.\w+)\s+start:0x([0-9a-fA-F]+)\s+end:0x([0-9a-fA-F]+)")
MAGIC = 0x564F5742  # "BWOV"
VERSION = 1

# The writable input sections of a C object, by object format. ELF: .data, and with -fPIC/-fPIE .data.rel and
# .data.rel.local (pointers); .data.rel.ro is const and stays. PE: .data, .data$<name> (COMDAT, -fdata-sections).
# -fdata-sections on ELF (.data.<name>) isn't covered: --check-map would list such an object's data as missed.
DATA = {"linux": ".data .data.rel .data.rel.local", "win64": ".data .data$*"}
BSS = {"linux": ".bss .bss.* COMMON", "win64": ".bss .bss$* COMMON"}
# The read-only section the table goes after
RODATA = {"linux": ".rodata", "win64": ".rdata"}
# An output section's address: PE wants each at a multiple of the section alignment (the default script's BLOCK)
HEAD = {"linux": " : ALIGN(64)", "win64": " BLOCK(__section_alignment__) :"}
# The table's section. Its LONGs alone make it writable (on ELF .rodata's segment with it) and on PE give it no
# content flags; port/src/ovl.c's const anchor, an input section .ovltab after them, makes it read-only data
TABHEAD = {"linux": " : ALIGN(16)", "win64": " BLOCK(__section_alignment__) :"}


def overlays(version="b_us", root=ROOT):
    """[(id, ds_ram, ds_image_size, ds_bss_size, [C file])] in id order, from the overlays' delinks.txt (another
    version's: config/<version>, which tools/version_config.py writes; another tree's: root, as pokebw2's checkout for
    Black 2 and White 2, docs/b2w2.md)."""
    out = []
    for d in sorted((Path(root) / "config" / version / "arm9/overlays").iterdir()):
        m = re.fullmatch(r"ov(\d+)", d.name)
        if not m or not (d / "delinks.txt").exists():
            continue
        head, files, cur = {}, [], None
        for line in (d / "delinks.txt").read_text().splitlines():
            if not line.strip():
                continue
            if not line[0].isspace():
                cur = line.strip().rstrip(":")
                files.append(cur)
                continue
            s = SECTION.match(line)
            if s and cur is None:
                head[s[1]] = (int(s[2], 16), int(s[3], 16))
        start = min(a for a, b in head.values())
        bss = head.get(".bss", (max(b for a, b in head.values()),) * 2)
        out.append((int(m[1]), start, bss[0] - start, bss[1] - bss[0], files))
    out.sort()
    ids = [o[0] for o in out]
    if ids != list(range(len(ids))):
        sys.exit("port_overlays.py: the overlay ids are not 0..n-1: " + str(ids))
    return out


def mangled(c_file):
    """The C file's object name as meson writes it: the source path with '/' made '_', then '.o' (or '.obj')."""
    return c_file.replace("/", "_") + ".o"


def pattern(c_file):
    """The ld file-name pattern of the C file's object, absolute or relative path (a '/' or '_' before it)."""
    return "*[/_]" + mangled(c_file) + "*"


# Black 2's and White 2's objects (port/meson.build, port/machine/gnu_*_section.h) put their writable data in sections of their own:
# the overlay files' in .<tag>ovldata and .<tag>ovlbss (inside .ovldata and .ovlbss, after Black's range: the DS table covers only
# Black's, theirs is built at start-up from the game's own markers), the rest in .<tag>machdata and .<tag>machbss (the machine's
# range). (NOBITS for the bss ones, from the pragma.)
GAME_TAGS = ["b2", "w2"]
OVL_GAME_SECTIONS = {"data": [(f".{t}ovldata",) for t in GAME_TAGS], "bss": [(f".{t}ovlbss",) for t in GAME_TAGS]}
MACH_GAME_SECTIONS = {"data": [f".{t}machdata" for t in GAME_TAGS], "bss": [f".{t}machbss" for t in GAME_TAGS]}


def script(target, ovs):
    data, bss = DATA[target], BSS[target]
    n = len(ovs)
    w = [f"/* Generated by tools/port_overlays.py --target {target}: each overlay's .data and .bss, for port/src/ovl.c."
         " Don't edit. */", ""]
    w += ["SECTIONS", "{", f"  .ovldata{HEAD[target]}", "  {", "    __bwovl_data = .;"]
    for oid, *_, files in ovs:
        w.append(f"    __bwovl_d{oid:03d} = .;")
        w += [f"    {pattern(f)}({data})" for f in files]
        w.append("    . = ALIGN(16);")
    w += [f"    __bwovl_d{n:03d} = .;"]
    w += [f"    *({data_t[0]})" for data_t in OVL_GAME_SECTIONS["data"]] + ["    . = ALIGN(16);"]   # (Black 2 and White 2: their own tables, port/src/ovl.c)
    w += ["    __bwovl_data_end = .;", "  }", "}", "INSERT AFTER .data;", ""]
    w += ["SECTIONS", "{", f"  .ovlbss{HEAD[target]}", "  {", "    __bwovl_bss = .;"]
    for oid, *_, files in ovs:
        w.append(f"    __bwovl_b{oid:03d} = .;")
        w += [f"    {pattern(f)}({bss})" for f in files]
        w.append("    . = ALIGN(16);")
    w += [f"    __bwovl_b{n:03d} = .;"]
    w += [f"    *({bss_t[0]})" for bss_t in OVL_GAME_SECTIONS["bss"]] + ["    . = ALIGN(16);"]
    w += ["    __bwovl_bss_end = .;", "  }", "}", "INSERT AFTER .bss;", ""]
    w += machine_script(target)
    w += romdata_script(target)
    # The table: offsets (differences of two symbols of one section: no relocation, so PIE and ASLR are fine)
    w += ["SECTIONS", "{", f"  .ovltab{TABHEAD[target]}", "  {", "    __bwovl_tab = .;",
          f"    LONG({MAGIC:#x}) LONG({VERSION}) LONG({n}) LONG(__bwovl_d{n:03d} - __bwovl_data)"
          f" LONG(__bwovl_b{n:03d} - __bwovl_bss) LONG(0) LONG(0) LONG(0)"]
    for oid, ram, size, bss_size, files in ovs:
        a, b = f"{oid:03d}", f"{oid + 1:03d}"
        w.append(f"    LONG({oid}) LONG({ram:#010x}) LONG({size:#x}) LONG({bss_size:#x})"
                 f" LONG(__bwovl_d{a} - __bwovl_data) LONG(__bwovl_d{b} - __bwovl_d{a})"
                 f" LONG(__bwovl_b{a} - __bwovl_bss) LONG(__bwovl_b{b} - __bwovl_b{a})")
    w += ["    KEEP(*(.ovltab))", "  }", "}", f"INSERT AFTER {RODATA[target]};", ""]
    return "\n".join(w)


# The machine (port/src/machine.c, docs/launcher.md section 1): the objects whose writable data the in-process restart
# puts back, matched by the archive they come from (the meson build's static libraries, by path as the link gets them):
# the game's C (libgame.a, libentry.a: port/src's host files are in libporthost.a, outside), lib/nnsys (libnnsys.a), libntr's
# SDK libraries but not libsim (the host), but for sim_vars.c (the DS's memory and registers). Apple's linker has no
# scripts: there the C is compiled into sections of its own (port/machine/macos_machine_section.h).
MACH_ARCHIVES = ["*libgame.a:*", "*libentry.a:*", "*libgame_w.a:*", "*libentry_w.a:*",   # (_w: White, port/meson.build)
                 "*libnnsys.a:*"]   # our NitroSystem (port/meson.build's nnsys option; nnsys_port is the host's)
MACH_SIM = "*libsim.a:*sim_vars.c.o"
MACH_LIBS = ["*subprojects/libntr/libraries/*.a:*"]


def machine_script(target):
    """.machdata and .machbss, after the overlays' (so an overlay object stays the overlay's), with the symbols
    machine.c reads. GNU ld (ELF, PE) and lld take it as it is."""
    out = []
    for sec, names, sym in ((".machdata", DATA[target].split(), "data"), (".machbss", BSS[target].split(), "bss")):
        w = ["SECTIONS", "{", f"  {sec}{HEAD[target]}", "  {", f"    __bwmach_{sym}0 = .;"]
        for pat in MACH_ARCHIVES + [MACH_SIM]:
            w.append(f"    {pat}({' '.join(names)})")
        w += [f"    *({n})" for n in MACH_GAME_SECTIONS[sym]]
        for pat in MACH_LIBS:
            w.append(f"    {pat}({' '.join('EXCLUDE_FILE(*libsim.a:*) ' + n for n in names)})")
        w += ["    . = ALIGN(16);", f"    __bwmach_{sym}1 = .;", "  }", "}",
              f"INSERT AFTER {'.ovldata' if sym == 'data' else '.ovlbss'};", ""]
        out += w
    return out


# The game's read-only data by group (tools/port_romfill.py fills it from the player's ROM after the link, and its
# GAMES table names the groups): each group's .rodata (ELF; PE: .rdata) in an output section of its own, so the linker
# merges strings and constants only within a group and no host code shares their bytes. .bwrom_s: what every game of
# the binary runs (the game files Black's archive holds for both, the SDK's libraries but libsim, port/acore's ARM7
# tables); .bwrom_<group>: that game's C (port/src's and port/game's files in its archives are the host's, left out).
# Only plain .rodata of the SDK's libraries (no merged strings: libntr's host side may share them before a game runs).
RODATA_IN = {"linux": [".rodata", ".rodata.*"], "win64": [".rdata", ".rdata$*"]}
HOST_FILES = "*[/_]port_src_* *[/_]port_game_*"


def romdata_script(target):
    import port_romfill
    ro = RODATA_IN[target]
    plain = ro[0]

    def game(archive):
        if archive.endswith(".o"):   # Black 2 / White 2: one object (b2.o), no archive members, no host files in it
            return f"    *{archive}(" + " ".join(ro) + ")"
        return f"    *{archive}:*(" + " ".join(f"EXCLUDE_FILE({HOST_FILES}) {n}" for n in ro) + ")"

    w = ["SECTIONS", "{", f"  .bwrom_s{HEAD[target]}", "  {"]
    w += [f"    *{port_romfill.SHARED_FROM}:{pattern(f)}({' '.join(ro)})" for f in port_romfill.SHARED_FILES]
    for a in port_romfill.SDK_ARCHIVES:
        if a == "libacore.a":
            w.append(f"    *{a}:*({' '.join(ro)})")
        else:
            w.append(f"    *{a}:*(EXCLUDE_FILE(*libsim.a:*) {plain})")
    w.append("  }")
    for grp, (code, archives) in port_romfill.GAMES.items():
        w += [f"  .bwrom_{grp}{HEAD[target]}", "  {"] + [game(a) for a in archives] + ["  }"]
    w += ["}", f"INSERT AFTER {RODATA[target]};", ""]
    return w

def macho_layout(ovs, sources, root=ROOT):
    """ld64 has no linker scripts: the Mach-O build (port/meson.build, is_mac) compiles the overlay files into one
    archive, force-loaded, whose members sit in overlay order, each overlay's preceded by a marker object
    (ovlmarkNNN.c, see macho_files) whose data and bss atoms start the overlay's range in __DATA,__ovldata and
    __DATA,__ovlbss (the files are compiled with port/machine/macos_ovl_section.h). ld64 lays atoms out in the order
    of the objects loaded, so the ranges are contiguous. Lines: 'mark N' or a source path (absolute, as given)."""
    have = {}
    for s in sources:
        rel = Path(s).resolve() if Path(s).is_absolute() else Path(s)
        if rel.is_absolute():
            rel = rel.relative_to(Path(root).resolve() if rel.is_relative_to(Path(root).resolve()) else ROOT)
        # tools/port_sources.py's stand-ins keep their game file's place
        have[rel.as_posix().removeprefix("port/extern/").removeprefix("port/extern_b2w2/")] = s
    out = []
    for oid, *_, files in ovs:
        out.append(f"mark {oid}")
        out += [have[f] for f in files if f in have]
    out.append(f"mark {len(ovs)}")
    return out


def macho_files(ovs, outdir, gnu_tag=None):
    """ovlmarkNNN.c (NNN = 0..n: the last is the end) and ovltab.c: the DS layout and the marker addresses, which
    port/src/ovl.c turns into its table at startup (BWPORT_OVL_MACHO)."""
    outdir = Path(outdir)
    outdir.mkdir(parents=True, exist_ok=True)
    n = len(ovs)
    head = "/* Generated by tools/port_overlays.py. Don't edit. */\n"

    def put(name, text):
        f = outdir / name
        if not f.exists() or f.read_text() != text:
            f.write_text(text)

    for i in range(n + 1):
        pragma = (f'#pragma clang section bss=".{gnu_tag}ovlbss" data=".{gnu_tag}ovldata"\n' if gnu_tag
                  else '#pragma clang section bss="__DATA,__ovlbss" data="__DATA,__ovldata"\n')
        put(f"ovlmark{i:03d}.c", head + pragma +
            f"__attribute__((aligned(16), used)) unsigned char bwovl_d{i:03d}[16] = {{1}};\n"
            f"__attribute__((aligned(16), used)) unsigned char bwovl_b{i:03d}[16];\n")
    # The count and the DS layout are the host's, read before the ROM fill (port/src/ovl.c's PortOvl_SetGame builds a game's
    # table at start-up): in a section of their own (.bwhro: tools/port_romfill.py fills data and rodata only, by the sections it
    # knows), not in .rodata, where their bytes match the ROM's (a count of 0 left Black 2 with no overlay resets)
    w = [head, "#ifdef __APPLE__", "#define BWHRO", "#else", '#define BWHRO __attribute__((section(".bwhro")))', "#endif",
         "extern unsigned char"]
    w.append(",\n".join(f"    bwovl_d{i:03d}[], bwovl_b{i:03d}[]" for i in range(n + 1)) + ";")
    w.append(f"BWHRO const unsigned bwovl_count = {n};")
    w.append("// id, DS address, DS image size, DS bss size")
    w.append(f"BWHRO const unsigned bwovl_ds[{n}][4] = {{")
    w += [f"    {{{oid}, {ram:#010x}, {size:#x}, {b:#x}}}," for oid, ram, size, b, _ in ovs]
    w += ["};", f"unsigned char *const bwovl_dptr[{n + 1}] = {{"]
    w += [f"    bwovl_d{i:03d}," for i in range(n + 1)]
    w += ["};", f"unsigned char *const bwovl_bptr[{n + 1}] = {{"]
    w += [f"    bwovl_b{i:03d}," for i in range(n + 1)]
    w += ["};", ""]
    put("ovltab.c", "\n".join(w))


def check(ovs):
    """The DS layout from the config against the ROM's own overlay table (extract/, git-ignored)."""
    if not ROM_TABLE.exists():
        sys.exit(f"{ROM_TABLE.relative_to(ROOT)} is missing (extract the base ROM)")
    rom = {}
    cur = None
    for line in ROM_TABLE.read_text().splitlines():
        m = re.match(r"\s*-\s*id:\s*(\d+)", line)
        if m:
            cur = rom.setdefault(int(m[1]), {})
            continue
        m = re.match(r"\s+(\w+):\s*(\S+)", line)
        if m and cur is not None:
            cur[m[1]] = m[2]
    bad = 0
    for oid, ram, size, bss_size, files in ovs:
        r = rom.get(oid)
        got = (ram, size, bss_size)
        want = r and (int(r["base_address"]), int(r["code_size"]), int(r["bss_size"]))
        if got != want:
            bad += 1
            print(f"ov{oid:03d}: config {got}, ROM {want}")
    print(f"{len(ovs)} overlays, {bad} differ from the ROM's table")
    return bad


def check_map(ovs, path):
    """Where a link map put each object's writable input sections: an overlay object's in .ovldata/.ovlbss, nobody
    else's. Returns the number misplaced."""
    owner = {mangled(f): oid for oid, *_, files in ovs for f in files}
    is_writable = re.compile(r"^(\.data(\.rel(\.local)?)?|\.data\$.*|\.bss(\..*|\$.*)?|COMMON)$")
    out_sec, pending = None, None
    placed = Counter()  # (overlay or None, output section, input kind) -> bytes
    bad = []
    text = Path(path).read_text(errors="replace")
    for line in text[text.find("Linker script and memory map"):].splitlines():
        m = re.match(r"^(\.\S+)\s", line)
        if m:
            out_sec = m[1]
            continue
        m = re.match(r"^ (\.\S+|COMMON)\s*$", line)  # a long input section name: its numbers on the next line
        if m:
            pending = m[1]
            continue
        m = re.match(r"^ (?:(\.\S+|COMMON)\s+)?\s*0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)$", line)
        name, pending = (m[1] or pending, None) if m else (None, None)
        if not m or not name or not is_writable.match(name) or int(m[3], 16) == 0:
            continue
        obj = m[4].rstrip(")").split("(")[-1].rsplit("/", 1)[-1]
        oid = next((o for key, o in owner.items() if obj == key or obj.endswith("_" + key)
                    or obj == key + "bj" or obj.endswith("_" + key + "bj")), None)
        size = int(m[3], 16)
        placed[(oid is not None, out_sec)] += size
        if (oid is not None) != (out_sec in (".ovldata", ".ovlbss")):
            bad.append((obj, oid, name, out_sec, size))
    for (ovl, sec), size in sorted(placed.items(), key=str):
        print(f"{'overlay' if ovl else 'other  '} objects' {sec:9} {size:10d} bytes")
    for b in bad[:30]:
        print("misplaced:", *b)
    print(f"{len(bad)} misplaced")
    return len(bad)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--target", choices=sorted(DATA), default="linux")   # (android: the linux script, for lld)   # (android: the linux script, for lld)
    ap.add_argument("-o", "--output", help="the script (default: stdout)")
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--check-map", metavar="MAP")
    ap.add_argument("--macho-layout", nargs="+", metavar="SRC", help="ld64: print the overlay archive's member order")
    ap.add_argument("--macho-files", metavar="DIR", help="ld64: write ovlmarkNNN.c and ovltab.c into DIR")
    ap.add_argument("--gnu-tag", help="with --macho-files: GNU ld/lld/PE: the markers go in .<TAG>ovldata / .<TAG>ovlbss (b2, w2)")
    ap.add_argument("--version", default="b_us", help="with --macho-layout/--macho-files: the game (w_us: White's layout)")
    ap.add_argument("--root", default=ROOT, help="with --version: the tree whose config/ and sources these are (pokebw2's)")
    a = ap.parse_args()
    ovs = overlays(a.version, a.root)
    if a.macho_layout:
        print("\n".join(macho_layout(ovs, a.macho_layout, a.root)))
        return
    if a.macho_files:
        macho_files(ovs, a.macho_files, a.gnu_tag)
        return
    if a.check:
        sys.exit(1 if check(ovs) else 0)
    if a.check_map:
        sys.exit(1 if check_map(ovs, a.check_map) else 0)
    if a.list:
        for oid, ram, size, bss_size, files in ovs:
            print(f"ov{oid:03d} {ram:#010x} {size:#8x} {bss_size:#7x} {len(files):3d} files")
        return
    text = script(a.target, ovs)
    if a.output:
        out = Path(a.output)
        if not out.exists() or out.read_text() != text:  # unchanged: keep the timestamp (no relink)
            out.parent.mkdir(parents=True, exist_ok=True)
            out.write_text(text)
    else:
        sys.stdout.write(text)


if __name__ == "__main__":
    main()
