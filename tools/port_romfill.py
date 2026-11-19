#!/usr/bin/env python3
"""The PC build's game data from the player's ROM: zero it in the linked image, write where it comes from.

A matching decompilation's compiled data IS the ROM's data: every table, string and constant of the game's C that the
DS build puts into the ROM's .data/.rodata compiles, on the PC, to the same bytes in a different layout (pointers 8
bytes wide). This step runs after the link, on the whole image, so it needs no knowledge of any C type:

1. The linker script (tools/port_overlays.py, romdata_script) gathers the read-only data of each group of objects
   into an output section of its own, so the linker merges strings and constants only within a group: .bwrom_b
   (Black's game C), .bwrom_w (White's), .bwrom_s (the C both games share: the two shared game files, libntr's SDK
   libraries but libsim, lib/nnsys (libnnsys.a), port/acore's ARM7 sound tables). Writable data and const data with
   pointers stay where they were (.data, .ovldata, .machdata, .data.rel.ro); the link map (-Wl,-Map) says which
   object each of their input sections comes from.
2. In each such input section, the image's own relocations (ELF .rela.dyn/.relr.dyn, PE .reloc, Mach-O chained fixups) mark the pointer
   slots: they stay as linked. The bytes between them are matched against the game's ROM modules as the DS loads
   them (ARM9 static module and overlays BLZ-decompressed, the ARM7), first at the place the last match ended (a
   table's next element), then in the DS object of the same C file (config/<version>/**/delinks.txt), then
   anywhere in the game's modules, longest match first. A match is a copy (module, offset, length) from the ROM
   to the image; its bytes are zeroed in the image. Bytes that are in no module stay as compiled (host-only data).
3. The copies, per game, go into the image's .bwfill section (port/src/rom_fill.c reserves it), with a hash of the
   bytes each game's copies give: port/src/rom_fill.c makes the pages writable, copies, checks the hash (a ROM of
   another revision fails) and puts the protection back, before the game boots (asset_load.c, PORT_AssetsLoad).

Correct by construction: a copy's ROM bytes ARE the image's bytes it replaces (the step checks it: it applies its
own table to the zeroed image and compares with the linked one), and no runtime-relocated byte is ever replaced.

    port_romfill.py IN OUT --map MAP --game IRBO:b:ROM:config/b_us [--game IRAO:w:ROM:config/w_us] [--report F]
    port_romfill.py --stats IMAGE --map MAP ...    # what would be filled, nothing written
"""
import argparse
import bisect
import hashlib
import os
import pickle
import re
import struct
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import rom_modules  # noqa: E402

MAGIC = b"BWFL"
VERSION = 1
# The games, by group: (ROM header code, the PC link's archives of its game C). A game is one row here, its ROM and
# config dir a --game argument (port/meson.build), its archives' .rodata a section .bwrom_<group> (tools/port_overlays.py
# reads this table). Black 2 and White 2 (pokebw2's C, docs/b2w2.md) link as b2.o / w2.o: on macOS and iOS (Mach-O, below)
# their groups come from those objects in the map; on the PC their rodata is a group of its own, their data is filled as White's.
GAMES = {
    "b": ("IRBO", ["libgame.a", "libentry.a"]),
    "w": ("IRAO", ["libgame_w.a", "libentry_w.a"]),
    "b2": ("IREO", ["b2.o"]),   # Black 2 / White 2: one relocatable object each (tools/port_relink.py), not archives
    "w2": ("IRDO", ["w2.o"]),
    "pl": ("CPUE", ["pl.o"]),   # Platinum (docs/platinum.md): pret's C, one relocatable object, pl.o
    "hg": ("IPKE", ["hg.o"]),   # HeartGold and SoulSilver (IPGE: the same module, the same group; docs/hgss.md): its C's and
                                # asm data's tables the blanks left (laid out as in the ROM, small tables next to each other
                                # made runs of up to 13 KB in the Mac binary)
}
# Group s: what every game of the binary runs. The game files Black's archive holds for both (tools/port_rename.py
# --shared), the SDK's libraries (libntr's, not libsim, the host; libnnsys.a) and port/acore (the ARM7's sound tables).
SHARED_FILES = ("src/main/main_0207e3d0.c", "src/main/main_02083644.c")
SHARED_FROM = "libgame.a"
SDK_ARCHIVES = ["subprojects/libntr/libraries/*.a", "libnnsys.a", "libacore.a"]
# macOS and iOS (ld64): Black's overlay files are an archive of their own and White links as one relocatable object
# (port/meson.build), as Black 2 and White 2 do
MACHO_ARCHIVES = {"b": ["libgame_ovl.a"], "w": ["white.o"]}
# ld64 has no linker script: the game's read-only data (const data with pointers too) is in a writable section per
# group (port/machine/macos_machine_section.h, libntr-ios-romfill.patch), so iOS's signed __TEXT is never written; the
# group of an atom is its object's (group_of), as for the ELF and PE data sections
MACHO_SECTIONS = {"__DATA,__bwrom_" + g for g in list(GAMES) + ["s"]} | {"__DATA,__machdata", "__DATA,__ovldata",
                                                                          "__DATA,__data", "__DATA,__const"}
# (__DATA,__const: HG's asm data, tools/pokehg_asm.py's .data.rel.ro)
# What the compiler makes itself (a local array's initialiser, l___const.F.x; switch tables) the pragma does not move: the
# link renames __TEXT,__const and __DATA_CONST,__const to writable sections (port/meson.build), filled by object as above.
# String literals: ld64 keeps one copy of equal strings from any objects, so __TEXT,__cstring (renamed too) is filled
# only where no host object has the string (cstring_owners), for the games whose objects have it. The same for the 4, 8
# and 16-byte literals (ld64 merges __literal4/8/16 into __const, one copy of each value for all objects).
MACHO_SECTIONS |= {"__DATA,__bwconst", "__DATA,__bwconstp"}
MACHO_CSTRINGS = "__DATA,__bwcstring"
HOST_OBJECTS = r"[/_]port_(src|game)_"     # port/src's and port/game's files in a game archive: the host's own
LOCAL_WINDOW = 512        # bytes after the last match where the next piece is looked for first
MIN_GLOBAL = 1            # a piece shorter than this is only looked for near its object
GRAM = 8                  # the n-gram index's width (pieces longer than this are split at the longest match)


# ------------------------------------------------------------------------------------------------ the image
class Image:
    """Sections (name, vaddr, file offset, size, file size), pointer slots and page protections of an ELF64 or PE32+."""

    def __init__(self, data):
        self.data = data
        if data[:4] == b"\x7fELF":
            self._elf()
        elif data[:2] == b"MZ":
            self._pe()
        elif data[:4] == b"\xcf\xfa\xed\xfe":
            self._macho()
        else:
            sys.exit("port_romfill: not an ELF, PE or Mach-O 64 image")
        self.emutls = self._keep_emutls()

    def _keep_emutls(self):
        """Emulated TLS (win64's -femulated-tls, port/meson.build; Android below API 29): each __thread variable has a
        control object __emutls_v.X (size, align, index, template: 4 words) and maybe a template __emutls_t.X, the
        compiler's, which __emutls_get_address reads at a thread's first touch of X. That can come before rom_fill.c's
        copies (libntr's threads made before the game's tables are loaded): a size and align the step zeroed gave those
        threads 0-byte, 8-aligned blocks (win64, 2026-11-13: os_thread.c's jmp_buf, msvcrt's setjmp faulting on its
        movdqa, then heap corruption 0xC0000374). They are never game data: kept as linked, as pointer slots are."""
        d = self.data
        found = []
        if self.kind == "pe":
            pe = struct.unpack_from("<I", d, 0x3C)[0]
            symptr, nsym = struct.unpack_from("<II", d, pe + 12)
            strtab = symptr + nsym * 18
            k = 0
            while symptr and k < nsym:
                o = symptr + k * 18
                name = d[o:o + 8]
                value, secnum, _, _, naux = struct.unpack_from("<IhHBB", d, o + 8)
                k += 1 + naux
                if name[:4] == b"\0\0\0\0":
                    so = strtab + struct.unpack_from("<I", name, 4)[0]
                    name = d[so:d.index(b"\0", so)]
                else:
                    name = name.rstrip(b"\0")
                if name.startswith(b"__emutls_v.") and 0 < secnum <= len(self.sections):
                    found.append(self.sections[secnum - 1]["addr"] + value)
        elif self.kind == "elf":
            sym, strs = self.section(".symtab"), self.section(".strtab")
            if sym and strs:
                for o in range(sym["off"], sym["off"] + sym["size"] - 23, 24):
                    nm = struct.unpack_from("<I", d, o)[0]
                    if d[strs["off"] + nm:strs["off"] + nm + 11] == b"__emutls_v.":
                        found.append(struct.unpack_from("<Q", d, o + 8)[0])
        for va in found:
            off = self.va_off(va)
            if off is None:
                continue
            size, _, _, templ = struct.unpack_from("<QQQQ", d, off)
            for k in range(0, 32, 8):
                self.slots[va + k] = 8
            if templ and size < (1 << 20):
                for k in range(0, size, 8):
                    self.slots[templ + k] = 8
        return len(found)

    def _elf(self):
        d = self.data
        self.kind = "elf"
        (self.etype, mach, _, _, phoff, shoff, _, _, phentsize, phnum, shentsize, shnum, shstrndx) = \
            struct.unpack_from("<HHIQQQIHHHHHH", d, 16)
        self.base = 0
        secs = []
        for i in range(shnum):
            nm, typ, flags, addr, off, size, link, info, align, entsize = struct.unpack_from("<IIQQQQIIQQ", d, shoff + i * shentsize)
            secs.append([nm, typ, flags, addr, off, size])
        stro = secs[shstrndx][4]
        self.sections = []
        for nm, typ, flags, addr, off, size in secs:
            name = d[stro + nm:d.index(b"\0", stro + nm)].decode()
            self.sections.append(dict(name=name, type=typ, flags=flags, addr=addr, off=off, size=size,
                                      fsize=0 if typ == 8 else size))
        self.loads, relro = [], None
        for i in range(phnum):
            typ, flags, off, vaddr, paddr, filesz, memsz, align = struct.unpack_from("<IIQQQQQQ", d, phoff + i * phentsize)
            if typ == 1:
                self.loads.append((vaddr, vaddr + memsz, flags))
            elif typ == 0x6474E552:
                relro = (vaddr, vaddr + memsz)
        self.relro = relro
        slots = {}
        for s in self.sections:
            if s["type"] == 4 and s["flags"] & 2:      # SHT_RELA, alloc: the dynamic relocations
                for o in range(s["off"], s["off"] + s["size"], 24):
                    r_off, r_info = struct.unpack_from("<QQ", d, o)
                    if r_info & 0xFFFFFFFF:
                        slots[r_off] = 8
            elif s["type"] == 9 and s["flags"] & 2:    # SHT_REL
                for o in range(s["off"], s["off"] + s["size"], 16):
                    r_off, r_info = struct.unpack_from("<QQ", d, o)
                    if r_info & 0xFFFFFFFF:
                        slots[r_off] = 8
            elif s["type"] == 19:                       # SHT_RELR
                where = 0
                for o in range(s["off"], s["off"] + s["size"], 8):
                    e = struct.unpack_from("<Q", d, o)[0]
                    if e & 1 == 0:
                        slots[e] = 8
                        where = e + 8
                    else:
                        for b in range(63):
                            if e >> (b + 1) & 1:
                                slots[where + b * 8] = 8
                        where += 63 * 8
            elif s["type"] in (0x60000001, 0x60000002):  # Android's packed relocations
                sys.exit("port_romfill: packed relocations (SHT_ANDROID_REL[A]) are not read: link without "
                         "--pack-dyn-relocs=android")
        self.slots = slots

    def _pe(self):
        d = self.data
        self.kind = "pe"
        pe = struct.unpack_from("<I", d, 0x3C)[0]
        if d[pe:pe + 4] != b"PE\0\0":
            sys.exit("port_romfill: no PE signature")
        mach, nsec, _, symptr, nsym, optsize, _ = struct.unpack_from("<HHIIIHH", d, pe + 4)
        opt = pe + 24
        if struct.unpack_from("<H", d, opt)[0] != 0x20B:
            sys.exit("port_romfill: not PE32+")
        self.base = struct.unpack_from("<Q", d, opt + 24)[0]
        ndir = struct.unpack_from("<I", d, opt + 108)[0]
        reloc_rva, reloc_size = struct.unpack_from("<II", d, opt + 112 + 5 * 8) if ndir > 5 else (0, 0)
        strtab = symptr + nsym * 18 if symptr else 0
        self.sections = []
        self.loads = []
        sh = opt + optsize
        for i in range(nsec):
            raw = d[sh + i * 40:sh + i * 40 + 8].rstrip(b"\0").decode()
            vsize, va, rsize, rptr = struct.unpack_from("<IIII", d, sh + i * 40 + 8)
            ch = struct.unpack_from("<I", d, sh + i * 40 + 36)[0]
            if raw.startswith("/") and strtab:
                o = strtab + int(raw[1:])
                raw = d[o:d.index(b"\0", o)].decode()
            addr = self.base + va
            self.sections.append(dict(name=raw, type=1, flags=ch, addr=addr, off=rptr, size=vsize,
                                      fsize=min(vsize, rsize)))
            prot = (4 if ch & 0x40000000 else 0) | (2 if ch & 0x80000000 else 0) | (1 if ch & 0x20000000 else 0)
            # (ELF's PF_R=4, PF_W=2, PF_X=1)
            self.loads.append((addr, addr + vsize, prot))
        self.relro = None
        slots = {}
        if reloc_rva:
            o = self.va_off(self.base + reloc_rva)
            end = o + reloc_size
            while o + 8 <= end:
                page, bsize = struct.unpack_from("<II", d, o)
                if bsize < 8:
                    break
                for k in range(o + 8, o + bsize, 2):
                    e = struct.unpack_from("<H", d, k)[0]
                    t, off = e >> 12, e & 0xFFF
                    if t == 10:
                        slots[self.base + page + off] = 8
                    elif t == 3:
                        slots[self.base + page + off] = 4
                o += bsize
        self.slots = slots

    def _macho(self):
        """A Mach-O 64 (ld64's arm64 executable): sections are named SEGMENT,SECTION; the pointer slots are the chained
        fixups' (LC_DYLD_CHAINED_FIXUPS: rebases and binds alike), or LC_DYLD_INFO's rebase and bind opcodes."""
        d = self.data
        self.kind = "macho"
        ncmds, sizeofcmds = struct.unpack_from("<II", d, 16)
        self.base = 0
        self.sections, self.loads, self.relro = [], [], None
        segs = []          # (vmaddr, fileoff) in load command order: the fixups' segment index
        chained = dyld_info = None
        o = 32
        for _ in range(ncmds):
            cmd, size = struct.unpack_from("<II", d, o)
            if cmd == 0x19:                                   # LC_SEGMENT_64
                segname = d[o + 8:o + 24].rstrip(b"\0").decode()
                vmaddr, vmsize, fileoff, filesize, maxprot, initprot, nsects, flags = struct.unpack_from("<QQQQiiII", d, o + 24)
                if segname == "__TEXT":
                    self.base = vmaddr
                segs.append((vmaddr, fileoff))
                if segname != "__PAGEZERO":
                    # (Mach-O's VM_PROT R=1, W=2, X=4 as ELF's PF_ bits: R=4, W=2, X=1)
                    self.loads.append((vmaddr, vmaddr + vmsize,
                                       (4 if initprot & 1 else 0) | (initprot & 2) | (1 if initprot & 4 else 0)))
                for k in range(nsects):
                    so = o + 72 + k * 80
                    sect = d[so:so + 16].rstrip(b"\0").decode()
                    seg = d[so + 16:so + 32].rstrip(b"\0").decode()
                    addr, ssize, soff = struct.unpack_from("<QQI", d, so + 32)
                    sflags = struct.unpack_from("<I", d, so + 64)[0]
                    zerofill = (sflags & 0xFF) in (1, 0xC, 0x12)
                    self.sections.append(dict(name=seg + "," + sect, type=sflags & 0xFF, flags=sflags, addr=addr,
                                              off=soff, size=ssize, fsize=0 if zerofill else ssize))
            elif cmd == 0x80000034:                           # LC_DYLD_CHAINED_FIXUPS
                chained = struct.unpack_from("<II", d, o + 8)
            elif cmd in (0x22, 0x80000022):                   # LC_DYLD_INFO(_ONLY)
                dyld_info = struct.unpack_from("<10I", d, o + 8)
            o += size
        self.segs = segs
        self.slots = {}
        if chained:
            self._chained(*chained)
        if dyld_info:
            rebase_off, rebase_size, bind_off, bind_size, weak_off, weak_size, lazy_off, lazy_size = dyld_info[:8]
            for off, n, bind in ((rebase_off, rebase_size, False), (bind_off, bind_size, True),
                                 (weak_off, weak_size, True), (lazy_off, lazy_size, True)):
                if n:
                    self._opcodes(off, n, bind)

    def _chained(self, dataoff, datasize):
        d = self.data
        starts = dataoff + struct.unpack_from("<I", d, dataoff + 4)[0]
        seg_count = struct.unpack_from("<I", d, starts)[0]
        for i in range(seg_count):
            so = struct.unpack_from("<I", d, starts + 4 + 4 * i)[0]
            if so == 0:
                continue
            si = starts + so
            size, page_size, fmt, seg_off, max_ptr, page_count = struct.unpack_from("<IHHQIH", d, si)
            if fmt in (2, 6):            # DYLD_CHAINED_PTR_64(_OFFSET): next in 4-byte strides, 12 bits
                stride, nbits = 4, 0xFFF
            elif fmt in (1, 9, 12):      # arm64e: 8-byte strides, 11 bits
                stride, nbits = 8, 0x7FF
            else:
                sys.exit(f"port_romfill: chained fixups pointer format {fmt} is not read")
            for p in range(page_count):
                start = struct.unpack_from("<H", d, si + 22 + 2 * p)[0]
                if start == 0xFFFF:
                    continue
                if start & 0x8000:
                    sys.exit("port_romfill: chained fixups with several starts a page are not read")
                va = self.base + seg_off + p * page_size + start   # (seg_off: the segment's from the image's start)
                while True:
                    o = self.va_off(va)
                    if o is None:
                        sys.exit(f"port_romfill: a fixup at {va:#x} is outside the image's file")
                    self.slots[va] = 8
                    nxt = (struct.unpack_from("<Q", d, o)[0] >> 51) & nbits
                    if nxt == 0:
                        break
                    va += nxt * stride

    def _opcodes(self, off, n, bind):
        """LC_DYLD_INFO's rebase or bind opcodes: the slots they write (pointers, 8 bytes)."""
        d = self.data
        end = off + n
        seg = soff = 0
        count = 0

        def uleb(o):
            v, sh = 0, 0
            while True:
                b = d[o]
                o += 1
                v |= (b & 0x7F) << sh
                sh += 7
                if not b & 0x80:
                    return v, o
        o = off
        while o < end:
            b = d[o]
            o += 1
            op, imm = b & 0xF0, b & 0x0F
            here = lambda: self.segs[seg][0] + soff
            if op == 0x00:
                if not bind:
                    break
                continue
            if bind:
                if op in (0x10, 0x30):                 # dylib ordinal imm / special
                    continue
                if op == 0x20:                         # dylib ordinal uleb
                    _, o = uleb(o)
                elif op == 0x40:                       # symbol name
                    o = d.index(b"\0", o) + 1
                elif op == 0x50:                       # type
                    continue
                elif op == 0x60:                       # addend sleb
                    while d[o] & 0x80:
                        o += 1
                    o += 1
                elif op == 0x70:
                    seg = imm
                    soff, o = uleb(o)
                elif op == 0x80:
                    v, o = uleb(o)
                    soff += v
                elif op == 0x90:
                    self.slots[here()] = 8
                    soff += 8
                elif op == 0xA0:
                    self.slots[here()] = 8
                    v, o = uleb(o)
                    soff += 8 + v
                elif op == 0xB0:
                    self.slots[here()] = 8
                    soff += 8 + imm * 8
                elif op == 0xC0:
                    cnt, o = uleb(o)
                    skip, o = uleb(o)
                    for _ in range(cnt):
                        self.slots[here()] = 8
                        soff += 8 + skip
                else:
                    sys.exit(f"port_romfill: bind opcode {op:#x}")
            else:
                if op == 0x10:                         # type
                    continue
                if op == 0x20:
                    seg = imm
                    soff, o = uleb(o)
                elif op == 0x30:
                    v, o = uleb(o)
                    soff += v
                elif op == 0x40:
                    soff += imm * 8
                elif op == 0x50:
                    for _ in range(imm):
                        self.slots[here()] = 8
                        soff += 8
                elif op == 0x60:
                    cnt, o = uleb(o)
                    for _ in range(cnt):
                        self.slots[here()] = 8
                        soff += 8
                elif op == 0x70:
                    self.slots[here()] = 8
                    v, o = uleb(o)
                    soff += 8 + v
                elif op == 0x80:
                    cnt, o = uleb(o)
                    skip, o = uleb(o)
                    for _ in range(cnt):
                        self.slots[here()] = 8
                        soff += 8 + skip
                else:
                    sys.exit(f"port_romfill: rebase opcode {op:#x}")

    def section(self, name):
        for s in self.sections:
            if s["name"] == name:
                return s
        return None

    def va_off(self, va):
        for s in self.sections:
            if s["addr"] and s["addr"] <= va < s["addr"] + s["fsize"]:
                return s["off"] + va - s["addr"]
        return None

    def prot(self, va):
        """The page protection the running image has at va (ELF PF_ bits: R=4, W=2, X=1)."""
        if self.relro and self.relro[0] <= va < self.relro[1]:
            return 4
        for a, b, f in self.loads:
            if a <= va < b:
                return f & 7
        return 0


# ------------------------------------------------------------------------------------------------ the link map
def parse_map(path):
    """[(output section, input section, vaddr, size, object)] of a GNU ld (ELF or PE) or lld map."""
    text = Path(path).read_text(errors="replace")
    out = []
    if text.startswith("# Path:"):
        return parse_ld64_map(text)
    if text.startswith("             VMA") or "     VMA      LMA     Size Align Out     In      Symbol" in text[:400]:
        head = text.splitlines()[0]
        col_in = head.index("In")
        cur = None
        for line in text.splitlines()[1:]:
            m = re.match(r"\s*([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+(\d+) ", line)
            if not m:
                continue
            rest_at = m.end()
            rest = line[rest_at:]
            if not rest.startswith(" "):
                cur = rest.strip()
                continue
            r = rest.strip()
            if line[col_in - 1:col_in] == " " and len(line) - len(line.lstrip()) < col_in and ":(" in r and r.endswith(")"):
                k = r.rindex(":(")
                out.append((cur, r[k + 2:-1], int(m.group(1), 16), int(m.group(3), 16), r[:k]))
        return out
    OUT = re.compile(r"^(\.\S+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)")
    IN1 = re.compile(r"^ (\.\S+|COMMON)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (.+)$")
    IN0 = re.compile(r"^ (\.\S+|COMMON)$")
    CONT = re.compile(r"^\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (.+)$")
    cur, pend = None, None
    for line in text.splitlines():
        m = OUT.match(line)
        if m:
            cur, pend = m.group(1), None
            continue
        if line.startswith(".") and " " not in line.strip():
            cur, pend = line.strip(), None     # a long output section name, its address on the next line
            continue
        m = IN1.match(line)
        if m:
            out.append((cur, m.group(1), int(m.group(2), 16), int(m.group(3), 16), m.group(4).strip()))
            pend = None
            continue
        m = IN0.match(line)
        if m:
            pend = m.group(1)
            continue
        if pend:
            m = CONT.match(line)
            if m:
                out.append((cur, pend, int(m.group(1), 16), int(m.group(2), 16), m.group(3).strip()))
            pend = None
    return out


def parse_ld64_map(text):
    """ld64's -map: each atom (symbol, literal, padding) with its address, size and object; the output section is
    SEGMENT,SECTION (from the map's section list), the input section the atom's name."""
    objs, secs, out = {}, [], []
    part = None
    for line in text.splitlines():
        if line.startswith("# "):
            part = {"# Object files:": "o", "# Sections:": "s", "# Symbols:": "y"}.get(line.strip(), part if
                                                                                          not line.startswith("# Dead") else None)
            continue
        if part == "o":
            m = re.match(r"\[\s*(\d+)\] (.*)$", line)
            if m:
                objs[int(m.group(1))] = m.group(2)
        elif part == "s":
            f = line.split("\t")
            if len(f) >= 4 and f[0].startswith("0x"):
                secs.append((int(f[0], 16), int(f[1], 16), f[2] + "," + f[3]))
        elif part == "y":
            f = line.split("\t", 2)
            if len(f) < 3 or not f[0].startswith("0x"):
                continue
            va, size = int(f[0], 16), int(f[1], 16)
            m = re.match(r"\[\s*(\d+)\] ?(.*)", f[2])
            if not m:
                continue
            k = bisect.bisect_right(secs, (va, 1 << 64)) - 1
            if k < 0 or not secs[k][0] <= va < secs[k][0] + max(secs[k][1], 1):
                continue
            out.append((secs[k][2], m.group(2), va, size, objs.get(int(m.group(1)), "")))
    return out


def parse_ld64_objects(path):
    """The input files of ld64's -map ("# Object files:"): paths, or ARCHIVE(MEMBER)."""
    out = []
    for line in Path(path).read_text(errors="replace").splitlines():
        if line.startswith("# Sections:"):
            break
        m = re.match(r"\[\s*\d+\] (.*)$", line)
        if m:
            out.append(m.group(1))
    return out


def macho_object_cstrings(d):
    """Each NUL-terminated string (with its NUL) of a Mach-O 64 object's __cstring sections, and each literal of its
    __literal4/8/16 sections as ("L", bytes)."""
    if d[:4] != b"\xcf\xfa\xed\xfe":
        return []
    ncmds = struct.unpack_from("<I", d, 16)[0]
    out, o = [], 32
    for _ in range(ncmds):
        cmd, size = struct.unpack_from("<II", d, o)
        if cmd == 0x19:
            for k in range(struct.unpack_from("<I", d, o + 64)[0]):
                so = o + 72 + k * 80
                if d[so:so + 16].rstrip(b"\0") == b"__cstring":
                    addr, ssize, soff = struct.unpack_from("<QQI", d, so + 32)
                    sec = d[soff:soff + ssize]
                    i = 0
                    while i < len(sec):
                        j = sec.find(b"\0", i)
                        j = len(sec) - 1 if j < 0 else j
                        out.append(bytes(sec[i:j + 1]))
                        i = j + 1
                elif d[so:so + 16].rstrip(b"\0") in (b"__literal4", b"__literal8", b"__literal16"):
                    addr, ssize, soff = struct.unpack_from("<QQI", d, so + 32)
                    w = int(d[so + 9:so + 16].rstrip(b"\0"))
                    out += [("L", bytes(d[soff + i:soff + i + w])) for i in range(0, ssize, w)]
        o += size
    return out


def archive_members(path):
    """{member name: bytes} of a BSD or GNU ar archive."""
    d = Path(path).read_bytes()
    if d[:4] == b"\xca\xfe\xba\xbe":   # a universal file (clang's runtime for iOS): its arm64 archive
        for k in range(struct.unpack_from(">I", d, 4)[0]):
            cpu, sub, off, size = struct.unpack_from(">iiII", d, 8 + 20 * k)
            if cpu == 0x0100000C:
                d = d[off:off + size]
                break
    out, o = {}, 8
    while o + 60 <= len(d):
        name = d[o:o + 16].decode(errors="replace").strip()
        size = int(d[o + 48:o + 58].decode().strip() or 0)
        body = d[o + 60:o + 60 + size]
        if name.startswith("#1/"):
            n = int(name[3:])
            name, body = body[:n].rstrip(b"\0").decode(errors="replace"), body[n:]
        out[name.rstrip("/")] = body
        o += 60 + size + (size & 1)
    return out


def cstring_owners(objects):
    """{string bytes: the groups whose objects have it (None: a host object)} over the link's input objects."""
    owners, archives = {}, {}
    for obj in objects:
        m = re.match(r"(.*\.a)\((.*)\)$", obj)
        try:
            if m:
                if m.group(1) not in archives:
                    archives[m.group(1)] = archive_members(m.group(1))
                d = archives[m.group(1)].get(m.group(2), b"")
            elif obj.endswith(".o"):
                d = Path(obj).read_bytes()
            else:
                continue
        except OSError:
            d = None
        g = group_of(obj)
        if d is None or (m and m.group(2) not in archives.get(m.group(1), {})):
            sys.exit(f"port_romfill: can't read {obj} for its strings")
        for s in macho_object_cstrings(d):
            owners.setdefault(s, set()).add(g)
    return owners


GROUP_SECTIONS = {".bwrom_" + g: g for g in list(GAMES) + ["s"]}
DATA_SECTIONS = {".data", ".data.rel.ro", ".ovldata", ".machdata", ".bwhost", ".rdata"}


def mangled(c_file):
    return c_file.replace("/", "_") + ".o"


def group_of(obj):
    """b (Black's game C), w (White's), s (shared: both games' and the SDK's) or None (the host's own)."""
    o = obj.replace("\\", "/")
    base = re.split(r"[/(]", o.rstrip(")"))[-1]
    for grp, (code, archives) in GAMES.items():
        if any(re.search(r"(^|/)" + re.escape(a) + r"(\.p/|\(|$)", o)
               for a in archives + MACHO_ARCHIVES.get(grp, [])):
            if re.search(HOST_OBJECTS, base):
                return None
            if archives[0] == SHARED_FROM and any(base.endswith(mangled(f)) or base.endswith(mangled(f) + "bj")
                                                  for f in SHARED_FILES):
                return "s"
            return grp
    if "libsim.a" in o:
        return None
    for a in SDK_ARCHIVES:
        if re.search(r"(^|/)" + re.escape(a).replace(r"\*", r".*") + r"(\.p/|\(|$)", o):
            return "s"
    return None


# ------------------------------------------------------------------------------------------------ the ROM side
class Corpus:
    """A game's modules as the DS loads them, one buffer: 0 = ARM9 static, 1 = ARM7, 2 + N = overlay N."""

    def __init__(self, rom_path, config, cache=None):
        """cache: a directory where each game's modules and Matcher.match's answers are kept between runs (a relink
        changes the host's objects, hardly ever the game's data): the BLZ decompress and the matching were 80% of the
        step. Keyed by the ROM's sha1 and by this file's and rom_modules.py's, so another ROM or tool is a cold run."""
        rom = Path(rom_path).read_bytes()
        self.code = rom[12:16].decode()
        self.memo, self.used, self.misses = {}, {}, 0
        self.cache_file = self.stamp = None
        kept = None
        if cache is not None:
            self.cache_file = Path(cache) / f"{self.code}.pkl"
            h = hashlib.sha1(rom)
            for src in (__file__, rom_modules.__file__):
                h.update(Path(src).read_bytes())
            self.stamp = h.hexdigest()
            try:
                with open(self.cache_file, "rb") as f:
                    kept = pickle.load(f)
                if kept["stamp"] != self.stamp:
                    kept = None
            except Exception:   # absent, torn or from another version: a cold run
                kept = None
        if kept is not None:
            self.info, self.buf, self.memo = kept["info"], kept["buf"], kept["memo"]
            self.starts = [i[2] for i in self.info]
        else:
            mods = []
            a9 = rom_modules.arm9(rom)
            mods.append((0, struct.unpack_from("<I", rom, 0x28)[0], a9))
            off7, _, ram7, size7 = struct.unpack_from("<4I", rom, 0x30)
            mods.append((1, ram7, rom[off7:off7 + size7]))
            fat, _, ovt, ovt_size = struct.unpack_from("<4I", rom, 0x48)
            rams = {}
            for e in range(ovt, ovt + ovt_size, 32):
                oid, ram = struct.unpack_from("<II", rom, e)
                rams[oid] = ram
            for oid, img in rom_modules.overlays(rom):
                mods.append((2 + oid, rams[oid], img))
            parts, self.starts, self.info = [], [], []
            pos = 0
            for mid, base, img in mods:
                self.starts.append(pos)
                self.info.append((mid, base, pos, pos + len(img)))
                parts.append(img)
                pos += len(img)
            self.buf = b"".join(parts)
            self.misses = 1
        self.by_mid = {mid: (base, s, e) for mid, base, s, e in self.info}
        self.files = self.delinks(config)
        self._grams = None
        self._mangled = None

    def save(self):
        """Keep the modules and the answers this run used (a run that asked for something new, or left over a lot)."""
        if self.cache_file is None or not (self.misses or len(self.memo) > 2 * len(self.used) + 1000):
            return
        try:
            self.cache_file.parent.mkdir(parents=True, exist_ok=True)
            tmp = self.cache_file.with_name(f"{self.cache_file.name}.{os.getpid()}.tmp")
            with open(tmp, "wb") as f:
                pickle.dump({"stamp": self.stamp, "info": self.info, "buf": self.buf, "memo": self.used}, f, 4)
            os.replace(tmp, self.cache_file)
        except OSError:
            pass

    def mangled_files(self):
        """{mangled object name: C file} (the first file of delinks.txt for a name two files make)."""
        if self._mangled is None:
            self._mangled = {}
            for f in self.files:
                self._mangled.setdefault(mangled(f), f)
        return self._mangled

    def delinks(self, config):
        """{C file: [(corpus start, end)]} of its DS sections, from config/<version>/arm9 (main) and its overlays."""
        files = {}
        root = ROOT / config / "arm9"
        todo = [(root / "delinks.txt", 0)]
        for d in sorted((root / "overlays").glob("ov*/delinks.txt")):
            todo.append((d, 2 + int(d.parent.name[2:])))
        sec = re.compile(r"^\s+(\.\w+)\s+start:0x([0-9a-fA-F]+)\s+end:0x([0-9a-fA-F]+)")
        for path, mid in todo:
            if not path.exists() or mid not in self.by_mid:
                continue
            base, s, e = self.by_mid[mid]
            cur = None
            for line in path.read_text().splitlines():
                if line and not line[0].isspace():
                    cur = line.strip().rstrip(":")
                    continue
                m = sec.match(line)
                if m and cur is not None and m.group(1) != ".bss":
                    a, b = int(m.group(2), 16) - base + s, int(m.group(3), 16) - base + s
                    if s <= a < b <= e:
                        files.setdefault(cur, []).append((a, b))
        return files

    def where(self, pos):
        i = bisect.bisect_right(self.starts, pos) - 1
        return self.info[i]

    def inside(self, pos, n):
        mid, base, s, e = self.where(pos)
        return pos + n <= e

    def find(self, piece, lo=0, hi=None):
        hi = len(self.buf) if hi is None else hi
        i = self.buf.find(piece, lo, hi)
        while i >= 0 and not self.inside(i, len(piece)):
            i = self.buf.find(piece, i + 1, hi)
        return i

    def index(self):
        """Every GRAM-byte window of the modules, sorted: (windows as little-endian integers, their positions)."""
        if self._grams is None:
            import numpy as np
            a = np.frombuffer(self.buf, np.uint8)
            n = len(a) - GRAM + 1
            v = np.zeros(n, np.uint64)
            for k in range(GRAM):
                v |= a[k:k + n].astype(np.uint64) << np.uint64(8 * k)
            order = np.argsort(v, kind="stable")
            self._grams = (v[order], order)
        return self._grams

    def candidates(self, piece, cap=256):
        """Positions where piece's first GRAM bytes are (at most cap of them)."""
        import numpy as np
        g, order = self.index()
        key = np.uint64(int.from_bytes(piece[:GRAM], "little"))
        lo = int(np.searchsorted(g, key, "left"))
        if lo == len(g) or g[lo] != key:
            return []
        hi = int(np.searchsorted(g, key, "right"))
        return order[lo:min(hi, lo + cap)].tolist()

    def has_grams(self, piece):
        """For each position of piece, whether its next GRAM bytes are somewhere in the modules."""
        import numpy as np
        a = np.frombuffer(piece, np.uint8)
        n = len(a) - GRAM + 1
        if n <= 0:
            return []
        v = np.zeros(n, np.uint64)
        for k in range(GRAM):
            v |= a[k:k + n].astype(np.uint64) << np.uint64(8 * k)
        g = self.index()[0]
        i = np.searchsorted(g, v)
        i[i == len(g)] = 0
        return list(g[i] == v)

    def common(self, piece, p, pos):
        """How many bytes of piece[p:] equal the modules' from pos (within pos's module)."""
        end = self.where(pos)[3]
        top = min(len(piece) - p, end - pos)
        n, step = 0, 64
        buf = self.buf
        while n < top:
            s = min(step, top - n)
            if piece[p + n:p + n + s] == buf[pos + n:pos + n + s]:
                n += s
                step *= 2
            elif s == 1:
                break
            else:
                step = max(1, s // 2)
        return n


# ------------------------------------------------------------------------------------------------ matching
def obj_file(obj, corpus):
    """The C file (as delinks.txt names it) an object was compiled from: the longest whose mangled name ends it."""
    base = re.split(r"[/(]", obj.replace("\\", "/").rstrip(")"))[-1]
    names = corpus.mangled_files()
    best = None
    for i in range(len(base)):
        for tail in (base[i:], base[i:-2] if base.endswith("bj") else ""):   # m ends the name, or m + "bj" does
            f = names.get(tail)
            if f is not None and (best is None or len(f) > len(best)):
                best = f
    return best


class Matcher:
    def __init__(self, corpus):
        self.c = corpus
        self.cursor = None
        self.ranges = []
        self.file_cache = {}

    def set_object(self, obj):
        if obj not in self.file_cache:
            f = obj_file(obj, self.c)
            self.file_cache[obj] = self.c.files.get(f, []) if f else []
        self.ranges = self.file_cache[obj]
        self.cursor = None

    def local(self, piece):
        c = self.c
        if self.cursor is not None:
            i = c.find(piece, self.cursor, min(len(c.buf), self.cursor + LOCAL_WINDOW + len(piece)))
            if i >= 0:
                return i
        for a, b in self.ranges:
            i = c.find(piece, a, b)
            if i >= 0:
                return i
        return -1

    def near(self, pos):
        """0 if pos is where this object's bytes are expected (after the last match, in its DS object), else 1."""
        if self.cursor is not None and self.cursor <= pos < self.cursor + LOCAL_WINDOW:
            return 0
        for a, b in self.ranges:
            if a <= pos < b:
                return 0
        return 1

    def short(self, piece):
        """A piece shorter than GRAM: near first, then anywhere (MIN_GLOBAL bytes or more)."""
        i = self.local(piece)
        if i < 0 and len(piece) >= MIN_GLOBAL:
            i = self.c.find(piece)
        return i

    def match(self, piece):
        """search(piece), kept in the corpus's cache: the answer and where the cursor ends depend only on the piece, the
        cursor and the object's DS ranges."""
        h = hashlib.blake2b(repr((self.cursor, tuple(self.ranges))).encode(), digest_size=16)
        h.update(piece)
        k = h.digest()
        hit = self.c.memo.get(k)
        if hit is None:
            hit = (self.search(piece), self.cursor)
            self.c.misses += 1
        self.c.used[k] = hit
        self.cursor = hit[1]
        return hit[0]

    def search(self, piece):
        """[(offset in piece, corpus position, length)] covering what of piece the modules have: the longest match
        at each place (near ones first among equals), the gaps shorter than GRAM looked up on their own."""
        n = len(piece)
        i = self.local(piece)
        if i >= 0:
            self.cursor = i + n
            return [(0, i, n)]
        if n < GRAM:
            i = self.short(piece)
            if i >= 0:
                self.cursor = i + n
                return [(0, i, n)]
            return []
        out = []
        ok = self.c.has_grams(piece)
        p = 0
        while p + GRAM <= n:
            if not ok[p]:
                p += 1
                continue
            best = (0, 1, -1)
            for pos in self.c.candidates(piece[p:]):
                ln = self.c.common(piece, p, pos)
                key = (ln, -self.near(pos), -pos)
                if key > best:
                    best = key
            ln, pos = best[0], -best[2]
            if ln < GRAM:
                p += 1
                continue
            out.append((p, pos, ln))
            self.cursor = pos + ln
            p += ln
        # the gaps between the long matches: short pieces of their own
        full, k = [], 0
        for p0, pos, ln in out + [(n, 0, 0)]:
            gap = piece[k:p0]
            a = len(gap) - len(gap.lstrip(b"\0"))
            b = len(gap.rstrip(b"\0"))
            if a < b and b - a < GRAM:
                i = self.short(gap[a:b])
                if i >= 0:
                    full.append((k + a, i, b - a))
            if ln:
                full.append((p0, pos, ln))
            k = p0 + ln
        return full


def pieces(data, base, slots):
    """(vaddr, bytes) of the runs between pointer slots, leading and trailing zeros cut, split at 16+ zero bytes."""
    mask = bytearray(len(data))
    for va, w in slots:
        for k in range(max(0, va - base), min(len(data), va - base + w)):
            mask[k] = 1
    out = []
    i, n = 0, len(data)
    while i < n:
        if mask[i] or data[i] == 0:
            i += 1
            continue
        j = i
        zeros = 0
        last = i
        while j < n and not mask[j]:
            if data[j] == 0:
                zeros += 1
                if zeros >= 16:
                    break
            else:
                zeros = 0
                last = j
            j += 1
        out.append((base + i, bytes(data[i:last + 1])))
        i = last + 1
    return out


# ------------------------------------------------------------------------------------------------ the table
def uvar(v):
    out = bytearray()
    while True:
        b = v & 0x7F
        v >>= 7
        if v:
            out.append(b | 0x80)
        else:
            out.append(b)
            return bytes(out)


def zz(v):
    return (v << 1) ^ (v >> 63) if v < 0 else v << 1


def fnv(h, b):
    for x in b:
        h = ((h ^ x) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return h


def encode(table_va, regions, games):
    """games: [(code, [(dst va, module, module offset, length, shared)], hash, bytes)]"""
    head = bytearray(MAGIC + struct.pack("<III", VERSION, len(regions), len(games)))
    body = bytearray()
    reg = bytearray()
    for va, size, prot in regions:
        reg += struct.pack("<qII", va - table_va, size, prot)
    gh = bytearray()
    streams = []
    for code, entries, h, nbytes in games:
        s = bytearray()
        prev_dst, prev_src = table_va, 0
        for dst, mid, off, ln, shared in entries:
            s += uvar(zz(dst - prev_dst)) + uvar(ln) + uvar(mid << 1 | shared) + uvar(zz(off - prev_src))
            prev_dst, prev_src = dst + ln, off + ln
        streams.append(s)
    hdr_len = len(head) + len(reg) + len(games) * 32
    off = hdr_len
    for (code, entries, h, nbytes), s in zip(games, streams):
        gh += code.encode() + struct.pack("<IIIIQI", off, len(s), len(entries), nbytes, h, 0)
        off += len(s)
    out = head + reg + gh
    for s in streams:
        out += s
    return bytes(out)


# ------------------------------------------------------------------------------------------------ main
def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("input")
    ap.add_argument("output", nargs="?")
    ap.add_argument("--map", required=True)
    ap.add_argument("--game", action="append", default=[], help="CODE:GROUP:ROM:CONFIG (IRBO:b:orig/baserom_b_us.nds:config/b_us)")
    ap.add_argument("--report", help="per group: bytes filled, bytes left (host-only), pieces")
    ap.add_argument("--stats", action="store_true", help="nothing written")
    ap.add_argument("--cache", help="where the ROM side's work is kept between runs (default: romfill-cache next to the "
                                    "output); --cache '' or BWFILL_NOCACHE=1: none")
    a = ap.parse_args()
    cache = None
    if a.cache is None and not a.stats and not os.environ.get("BWFILL_NOCACHE"):
        cache = Path(a.output or a.input).resolve().parent / "romfill-cache"
    elif a.cache:
        cache = Path(a.cache)
    t0 = time.time()
    data = bytearray(Path(a.input).read_bytes())
    img = Image(data)
    fill = img.section("__DATA,__bwfill" if img.kind == "macho" else ".bwfill")
    if fill is None and not a.stats and not os.environ.get("BWFILL_NOTABLE"):
        sys.exit("port_romfill: the image has no .bwfill section (port/src/rom_fill.c)")
    if fill is not None and data[fill["off"]:fill["off"] + 4] != MAGIC:
        sys.exit("port_romfill: .bwfill does not start with BWFL")
    if fill is not None and struct.unpack_from("<I", data, fill["off"] + 8)[0] != 0:
        sys.exit("port_romfill: the image was filled already (link it again)")
    games = []
    for g in a.game:
        code, grp, rom, cfg = g.split(":", 3)
        if not Path(rom).exists():
            continue
        games.append((code, grp, Corpus(rom, cfg, cache)))
    if not games:
        sys.exit("port_romfill: no game ROM")
    for code, grp, c in games:
        if c.code != code:
            sys.exit(f"port_romfill: {code}'s ROM has the code {c.code}")
    users = {"s": [g for g in games]}
    for g in games:
        users.setdefault(g[1], []).append(g)
    # the input sections to fill: the group sections whole, the data sections' game objects
    work = []
    strings = None
    for out_sec, in_sec, va, size, obj in parse_map(a.map):
        if size == 0 or out_sec is None:
            continue
        if img.kind == "macho":
            literal = re.match(r"\d+-byte-literal$", in_sec)
            if out_sec == MACHO_CSTRINGS or (literal and out_sec in MACHO_SECTIONS):
                if strings is None:
                    strings = cstring_owners(parse_ld64_objects(a.map))
                o = img.va_off(va)
                key = bytes(data[o:o + size]) if o is not None else None
                grp = strings.get(("L", key) if literal else key, {None})
                if None in grp:
                    continue      # a host object has the string too (ld64 keeps one copy): it stays as compiled
                grp = "s" if "s" in grp else "+".join(sorted(grp))
                if grp not in users:
                    users[grp] = [g for g in games if g[1] in grp.split("+")]
            elif out_sec not in MACHO_SECTIONS:
                continue
            else:
                grp = group_of(obj)
            if grp is None:
                continue
        elif out_sec in GROUP_SECTIONS:
            grp = GROUP_SECTIONS[out_sec]
        elif out_sec in DATA_SECTIONS or (a.stats and out_sec == ".rodata" and ".str" not in in_sec and ".cst" not in in_sec):
            grp = group_of(obj)
            if grp is None:
                continue
        else:
            continue
        if grp not in users:
            continue
        # GNU ld 2.42 (Ubuntu 24.04) lists a merged string piece (.rodata.str1.1) at its section's end, past the bytes
        # the section has in the file: nothing of it is in the image, so there is nothing to fill or check
        if img.va_off(va) is None:
            continue
        work.append((va, size, grp, obj, out_sec))
    work.sort()
    # overlapping input sections (merged pools are listed at their first member's address): keep the first
    merged = []
    for w in work:
        if merged and w[0] < merged[-1][0] + merged[-1][1]:
            continue
        merged.append(w)
    work = merged
    slot_vas = sorted(img.slots)
    entries = {code: [] for code, _, _ in games}
    stats = {}
    zero = []
    regions = {}
    for va, size, grp, obj, out_sec in work:
        off = img.va_off(va)
        if off is None:
            continue
        sec_data = bytes(data[off:off + size])
        lo = bisect.bisect_left(slot_vas, va - 8)
        hi = bisect.bisect_left(slot_vas, va + size)
        slots = [(s, img.slots[s]) for s in slot_vas[lo:hi]]
        st = stats.setdefault((grp, out_sec), [0, 0, 0, 0])
        gs = users[grp]
        for pva, piece in pieces(sec_data, va, slots):
            st[3] += len(piece)
            # every game of the group must have it (the shared C runs with whichever ROM the player has)
            per_game = []
            covered = None
            for code, _, corpus in gs:
                mt = MATCHERS.setdefault(code, Matcher(corpus))
                if mt.obj != obj:
                    mt.set_object(obj)
                    mt.obj = obj
                ms = mt.match(piece)
                per_game.append((code, corpus, ms))
                cov = set()
                for p0, pos, ln in ms:
                    cov.update(range(p0, p0 + ln))
                covered = cov if covered is None else covered & cov
            if not covered:
                st[2] += len(piece)
                if LEFT is not None and len(piece) >= 16:
                    LEFT.write(f"{grp} {out_sec} {pva:#x} {len(piece)} {len(piece)} {obj[-60:]}\n")
                continue
            # the bytes every game covers; each game's copies cut to them
            for code, corpus, ms in per_game:
                for p0, pos, ln in ms:
                    k = p0
                    while k < p0 + ln:
                        if k not in covered:
                            k += 1
                            continue
                        e = k
                        while e < p0 + ln and e in covered:
                            e += 1
                        mid, base, s0, e0 = corpus.where(pos + (k - p0))
                        entries[code].append((pva + k, mid, pos + (k - p0) - s0, e - k, 1 if grp == "s" else 0))
                        k = e
            run = None
            for k in sorted(covered):
                if run is not None and run[1] == k:
                    run[1] = k + 1
                else:
                    run = [k, k + 1]
                    zero.append((pva + k, run))
            st[0] += len(covered)
            st[1] += 1
            st[2] += len(piece) - len(covered)
            if LEFT is not None and len(piece) - len(covered) >= 16:
                LEFT.write(f"{grp} {out_sec} {pva:#x} {len(piece)} {len(piece) - len(covered)} {obj[-60:]}\n")
            r = regions.setdefault(out_sec, [va, va + size])
            r[0], r[1] = min(r[0], va), max(r[1], va + size)
    # each game's copies in image order, adjacent ones joined
    games_out = []
    orig = bytes(data)
    for code, grp, corpus in games:
        es = sorted(entries[code])
        joined = []
        for e in es:
            if joined and joined[-1][0] + joined[-1][3] == e[0] and joined[-1][1] == e[1] and \
                    joined[-1][2] + joined[-1][3] == e[2] and joined[-1][4] == e[4]:
                joined[-1] = (joined[-1][0], e[1], joined[-1][2], joined[-1][3] + e[3], e[4])
            else:
                joined.append(e)
        h = 0xCBF29CE484222325
        nbytes = 0
        for dst, mid, moff, ln, shared in joined:
            base, s0, e0 = corpus.by_mid[mid]
            src = corpus.buf[s0 + moff:s0 + moff + ln]
            o = img.va_off(dst)
            if src != orig[o:o + ln]:
                sys.exit(f"port_romfill: internal error: a copy to {dst:#x} is not the image's bytes")
            h = fnv(h, src)
            nbytes += ln
        games_out.append((code, joined, h, nbytes))
    for va, (k0, k1) in zero:
        o = img.va_off(va)
        data[o:o + k1 - k0] = bytes(k1 - k0)
    # the self-check: each game's copies give back the linked image's bytes
    for code, joined, h, nbytes in games_out:
        corpus = [g[2] for g in games if g[0] == code][0]
        test = bytearray(data)
        for dst, mid, moff, ln, shared in joined:
            base, s0, e0 = corpus.by_mid[mid]
            o = img.va_off(dst)
            test[o:o + ln] = corpus.buf[s0 + moff:s0 + moff + ln]
        for va, size, grp, obj, out_sec in work:
            if code in [g[0] for g in users[grp]]:
                o = img.va_off(va)
                if test[o:o + size] != orig[o:o + size]:
                    sys.exit(f"port_romfill: self-check failed for {code} in {obj} at {va:#x}")
    lines = []
    for (grp, sec), (filled, n, left, total) in sorted(stats.items()):
        lines.append(f"{grp} {sec:14} {filled:9} bytes from the ROM in {n:6} pieces, {left:7} left of {total}")
    for code, joined, h, nbytes in games_out:
        lines.append(f"{code}: {len(joined)} copies, {nbytes} bytes")
    lines.append(f"emulated TLS: {img.emutls} control objects kept as linked")
    if fill is not None:
        reg = []
        for name, (lo, hi) in sorted(regions.items(), key=lambda x: x[1]):
            reg.append((lo, hi - lo, img.prot(lo)))
        table = encode(fill["addr"], reg, games_out)
        lines.append(f"table: {len(table)} of {fill['size']} bytes")
        if len(table) > fill["fsize"]:
            sys.exit(f"port_romfill: the table needs {len(table)} bytes, .bwfill has {fill['fsize']}: raise "
                     "BWFILL_SIZE in port/src/rom_fill.c")
        data[fill["off"]:fill["off"] + len(table)] = table
    for _, _, c in games:
        c.save()
    lines.append(f"{time.time() - t0:.1f} s")
    print("\n".join("port_romfill: " + l for l in lines))
    if a.report:
        Path(a.report).write_text("\n".join(lines) + "\n")
    if not a.stats:
        out = Path(a.output or a.input)
        tmp = out.with_name(out.name + ".tmp")
        tmp.write_bytes(data)
        os.chmod(tmp, os.stat(a.input).st_mode & 0o777)
        os.replace(tmp, out)
        if img.kind == "macho":   # the linker's ad hoc signature no longer fits the pages (the IPA is signed again later)
            subprocess.run(["codesign", "-f", "-s", "-", str(out)], check=True, capture_output=True)


MATCHERS = {}
Matcher.obj = None
LEFT = open(os.environ["BWFILL_LEFT"], "w") if os.environ.get("BWFILL_LEFT") else None

if __name__ == "__main__":
    main()
