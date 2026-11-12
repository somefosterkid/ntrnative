#!/usr/bin/env python3
"""Compile a C file and compare each of its functions against Black's original code.

Relocated bytes (calls and pointers) are compared by their symbols instead, since the object is not linked: Black's
come from dsd's delinked object of the file (build/b_us/delinks), and a call or address of another symbol than the
original's counts too, as does one of a symbol that neither a symbols.txt nor an object of the link defines; the address of a static datum or string, which no relocation names (its name is often another
table's), by where it lands in the file's delinks.txt ranges (or by its bytes, when the file's data isn't in Black's
order yet). Prints one row per function and compiler (MATCH, the count of differing bytes or symbols, or
both sizes), then an aligned diff of each function that doesn't match under the first compiler: only the hunks that
differ, so that one instruction more or less shifts nothing after it. Literal pools are listed as `.word`s, with the
symbol of a relocated one. Functions are looked up by name in config/b_us/arm9/**/symbols.txt, so the C must use Black's symbol names. A
file compiles with the compiler and flags configure.py gives it: the game's, or its library's (lib/*/library.toml).

    .venv/bin/python tools/compiler_probe.py src/ov198/b_plist_anm.c
    .venv/bin/python tools/compiler_probe.py src/... --compilers 1.1p1,1.1,1.6sp2 --functions func_ov198_0220b460
    .venv/bin/python tools/compiler_probe.py lib/nitro/src/... --compilers 1.1p1,2.0/sp2p2 --flags="-O4,s -thumb"

Needs `ninja` to have run once (for extract/) and the packages in requirements.txt. From pokebw2 (GPL-3.0,
https://github.com/fuddlesworth/pokebw2), tools/scripts/compiler_probe.py, trimmed to one version.
"""
import argparse
import bisect
import difflib
import os
import re
import shlex
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

import capstone
import yaml
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / "tools"
# The version judged against: Black, or another whose ROM is in orig/ (BW_VERSION=w_us: White, docs/white.md)
VERSION = os.environ.get("BW_VERSION") or "b_us"
sys.path.insert(0, str(ROOT))
import bwcache  # noqa: E402
from configure import INCLUDE_DIRS, VERSIONS, compiler_of  # noqa: E402

# the version's define and the directory its assets are in, as configure.py compiles it
VERSION_FLAGS = [arg for d in VERSIONS[VERSION]["defines"] for arg in ("-d", d)] + ["-include", "constants/version.h"]
VERSION_INCLUDES = [*INCLUDE_DIRS[:-1], VERSIONS[VERSION]["asset_root"]]

SYMBOL_RE = re.compile(r"^(\S+) kind:function\((\w+),size=(0x[0-9a-f]+)\S*\) addr:(0x[0-9a-f]+)")


# Caches for a caller that probes many times in one process (tools/local_driver.py): the modules, their bytes, the
# symbols (read again when a symbols.txt changes) and the delinked objects (when one changes)
MODULES: dict[str, tuple[Path, int]] = {}
BINARIES: dict[Path, bytes] = {}
INDEX: dict = {}
DELINKED: dict[Path, tuple[int, dict]] = {}


def symbol_index(refresh: bool = False) -> dict[str, tuple[str, str]]:
    """{function: (the directory of its symbols.txt, its line)}; refresh: read again if a symbols.txt changed."""
    if INDEX and not refresh:
        return INDEX["index"]
    files = INDEX.get("files") or sorted((ROOT / "config" / VERSION / "arm9").rglob("symbols.txt"))
    INDEX["files"] = files  # the modules don't change; their symbols do
    key = tuple((str(p), p.stat().st_mtime_ns) for p in files)
    if INDEX.get("key") != key:
        token = bwcache.files_token(files, __file__)  # kept in build/cache while no symbols.txt changes
        index = bwcache.get("symbol_index", str(ROOT), token)
        if index is None:
            index: dict[str, tuple[str, str]] = {}
            for p in files:
                for line in p.read_text().splitlines():
                    if " kind:function(" in line:
                        index.setdefault(line.split(" ", 1)[0], (str(p.parent), line))
            bwcache.put("symbol_index", str(ROOT), token, index)
        INDEX.update(key=key, index=index)
        ADDRESSES.clear()
    return INDEX["index"]


def load_modules() -> dict[str, tuple[Path, int]]:
    """Returns the binary and base address of every module, keyed by its config directory."""
    if MODULES:
        return MODULES
    extract = ROOT / "extract" / VERSION
    token = bwcache.files_token(sorted(extract.rglob("*.yaml")), __file__)  # the yaml is slow: kept in build/cache
    found = bwcache.get("modules", str(extract), token)
    if found is not None:
        MODULES.update(found)
        return MODULES
    config = ROOT / "config" / VERSION / "arm9"
    arm9 = yaml.safe_load((extract / "arm9" / "arm9.yaml").read_text())
    modules = {str(config): (extract / "arm9" / "arm9.bin", arm9["base_address"])}
    for name in ("itcm", "dtcm"):
        info = yaml.safe_load((extract / "arm9" / f"{name}.yaml").read_text())
        modules[str(config / name)] = (extract / "arm9" / f"{name}.bin", info["base_address"])
    overlays = yaml.safe_load((extract / "arm9_overlays" / "overlays.yaml").read_text())["overlays"]
    for overlay in overlays:
        path = extract / "arm9_overlays" / overlay["file_name"]
        modules[str(config / "overlays" / f"ov{overlay['id']:03d}")] = (path, overlay["base_address"])
    # the other autoloads and the DSi's LTD ones (TwlSDK's photo signature, poke_net), as config.yaml names them
    rom = yaml.safe_load((extract / "config.yaml").read_text())
    for prefix, autoloads in (("autoload", rom.get("unknown_autoloads") or []),
                              ("ltd_autoload", (rom.get("dsi") or {}).get("ltd_autoloads") or [])):
        for autoload in autoloads:
            info = yaml.safe_load((extract / autoload["config"]).read_text())
            modules[str(config / f"{prefix}_{autoload['index']}")] = (extract / autoload["bin"], info["base_address"])
    bwcache.put("modules", str(extract), token, modules)
    MODULES.update(modules)
    return modules


def find_function(name: str, modules) -> tuple[bytes, int, bool] | None:
    """Returns the original bytes, address and Thumb flag of a function."""
    entry = symbol_index().get(name)
    match = entry and SYMBOL_RE.match(entry[1])
    if not match:
        return None
    directory, line = entry
    mode, size, addr = match.group(2), int(match.group(3), 16), int(match.group(4), 16)
    if "dsprot=" in line:
        # DS Protect's functions are encrypted in the ROM; dsd decrypts them into the delinked objects
        data = delinked_function(name)
        if data is None:
            print(f"{name}: encrypted, and not in build/{VERSION}/delinks; run ninja first", file=sys.stderr)
            return None
        return data[:size], addr, mode == "thumb"
    if directory not in modules:
        return None
    binary, base = modules[directory]
    if binary not in BINARIES:
        BINARIES[binary] = binary.read_bytes()
    return BINARIES[binary][addr - base : addr - base + size], addr, mode == "thumb"


DELINKED: dict[str, tuple[Path, int, int, int]] = {}  # {symbol: (object, section, value, size)}, the first of each
DELINKED_SCANNED: list[bool] = []


def delinked_symbols() -> dict[str, tuple[Path, int, int, int]]:
    """The defined symbols of all the delinked objects (build/<version>/delinks), the first of each name in the order of
    the objects: read with struct, once. pyelftools took 3 s for the symbols of every object, for each function asked
    (tools/ghidra_draft.py passes() asks for 7: 25 s)."""
    if not DELINKED_SCANNED:
        for path in (ROOT / "build" / VERSION / "delinks").rglob("*.o"):  # (the order the first match was found in)
            data = path.read_bytes()
            if data[:4] != b"\x7fELF":
                continue
            shoff = struct.unpack_from("<I", data, 0x20)[0]
            shentsize, shnum = struct.unpack_from("<HH", data, 0x2E)
            sections = [struct.unpack_from("<10I", data, shoff + i * shentsize) for i in range(shnum)]
            symtab = next((s for s in sections if s[1] == 2), None)
            if not symtab:
                continue
            strtab = sections[symtab[6]]
            strings = data[strtab[4]:strtab[4] + strtab[5]]
            for n, value, size, _, _, shndx in struct.iter_unpack("<IIIBBH", data[symtab[4]:symtab[4] + symtab[5] // 16 * 16]):
                if shndx in (0, 0xFFF1):  # SHN_UNDEF, SHN_ABS
                    continue
                name = strings[n:strings.find(bytes(1), n)].decode(errors="replace")
                DELINKED.setdefault(name, (path, shndx, value, size))
        DELINKED_SCANNED.append(True)
    return DELINKED


def delinked_function(name: str) -> bytes | None:
    """Returns a function's bytes from the delinked objects, which hold DS Protect's code decrypted."""
    found = delinked_symbols().get(name)
    if not found:
        return None
    path, shndx, value, size = found
    with path.open("rb") as f:
        elf = ELFFile(f)
        start = value & ~1
        data = elf.get_section(shndx).data()
        return data[start : start + size] if size else data[start:]


def object_functions(obj: Path, statics: dict | None = None) -> dict[str, tuple[bytes, dict[int, str | None], list[tuple[int, str]]]]:
    """Every function in an object file: its bytes, {relocated offset: the symbol's name, with the addend of a word in
    the literal pool, or None for a section or local symbol}, and its mapping symbols [(offset, "$a", "$t" or "$d")].
    The 32-bit little-endian ELF is read here with struct, not with pyelftools, which took 8 s for a delinked object
    of 28,000 symbols; each function's relocations and marks are found by bisection in the section's sorted lists.
    statics: filled with {function: {offset: (section index, offset in it, name)}} of the pool words that hold the
    address of a static datum or a string (a local symbol or a section's, None in the relocations), for
    static_misses()."""
    data = obj.read_bytes()
    shoff = struct.unpack_from("<I", data, 0x20)[0]
    shentsize, shnum = struct.unpack_from("<HH", data, 0x2E)
    # (name, type, flags, addr, offset, size, link, info, addralign, entsize)
    sections = [struct.unpack_from("<10I", data, shoff + i * shentsize) for i in range(shnum)]
    symtab = next(s for s in sections if s[1] == 2)
    strtab = sections[symtab[6]]
    strings = data[strtab[4]:strtab[4] + strtab[5]]
    names: dict[int, str] = {}
    shstrndx = struct.unpack_from("<H", data, 0x32)[0]

    def name_of_section(i):
        at = sections[shstrndx][4] + sections[i][0]
        return data[at:data.find(bytes(1), at)].decode(errors="replace")

    def name_of(offset):
        if offset not in names:
            names[offset] = strings[offset:strings.find(bytes(1), offset)].decode()
        return names[offset]
    # (name, value, size, info, other, shndx)
    symbols = list(struct.iter_unpack("<IIIBBH", data[symtab[4]:symtab[4] + symtab[5] // 16 * 16]))
    relocations: dict[int, list] = {}  # section -> [(offset, symbol index, type, addend or None)] in file order
    for s in sections:
        if s[1] not in (4, 9):
            continue
        if sections[s[6]] != symtab:
            raise ValueError(f"{obj}: relocations against section {s[6]}")
        width = 12 if s[1] == 4 else 8
        body = data[s[4]:s[4] + s[5] // width * width]
        relocations.setdefault(s[7], []).extend(
            (e[0], e[1] >> 8, e[1] & 0xFF, e[2] if s[1] == 4 else None)
            for e in struct.iter_unpack("<IIi" if s[1] == 4 else "<II", body))
    for entries in relocations.values():
        entries.sort(key=lambda e: e[0])  # stable: a later relocation of one offset still wins
    keys = {i: [e[0] for e in entries] for i, entries in relocations.items()}
    marks: dict[int, list[tuple[int, str]]] = {}
    for name, value, _, _, _, shndx in symbols:
        if shndx not in (0, 0xFFF1):
            n = name_of(name)
            if n[:2] in ("$a", "$t", "$d"):
                marks.setdefault(shndx, []).append((value, n[:2]))
    for entries in marks.values():
        entries.sort()
    mark_keys = {i: [m[0] for m in entries] for i, entries in marks.items()}
    functions = {}
    for name, value, size, info, _, index in symbols:
        if info & 0xF != 2 or index in (0, 0xFFF1):  # STT_FUNC, defined
            continue
        n = name_of(name)
        if n.startswith("$"):
            continue
        start = value & ~1
        code = data[sections[index][4] + start:sections[index][4] + start + size]
        relocs = {}
        for offset, sym, kind, addend in relocations.get(index, ())[
                bisect.bisect_left(keys.get(index, ()), start):bisect.bisect_left(keys.get(index, ()), start + size)]:
            o = offset - start
            kind_of = symbols[sym][3]  # st_info: the binding above the type
            # a symbol, not a section: a global one, or a static function by its name (compared by its address, so that
            # a call of another static function counts), not an unnamed `@123`. Not a static table by its name: the C's
            # statics often carry another table's address in their name (btl_main.c, poke_tool.c: complete files that
            # link byte for byte): a static datum is compared by where it lands instead (statics, static_misses())
            target = name_of(symbols[sym][0])
            named = kind_of & 0xF != 3 and (kind_of >> 4 != 0 or kind_of & 0xF == 2 and bool(target) and target[0] not in "@$.")
            if kind != 2:
                addend = 0
            elif addend is None:  # REL: the addend is in the word
                addend = int.from_bytes(code[o:o + 4], "little")
            if kind == 2 and addend & 1 and named and (kind_of & 0xF == 2 or "(thumb" in symbol_index().get(
                    target, ("", ""))[1]):
                # a word into a Thumb function: MWCC's addend counts from its symbol's address with the Thumb bit,
                # which the link adds (+0x7f is +0x80), dsd's from its start (os_reset_itcm.c's OSi_VerifyHmac key)
                addend += 1
            relocs[o] = target + (f"{addend:+#x}" if addend else "") if named else None
            sec = symbols[sym][5]
            data_section = 0 < sec < len(sections) and sections[sec][1] in (1, 8) and not sections[sec][2] & 4
            if statics is not None and not named and kind == 2 and kind_of & 0xF != 2 and data_section:
                where = symbols[sym][1] + addend  # (PROGBITS or NOBITS, not code)
                label = (target + (f"{addend:+#x}" if addend else "") if kind_of & 0xF != 3 and target
                         else f"{name_of_section(sec)}+{where:#x}")
                statics.setdefault(n, {})[o] = (sec, where, label)
        low, high = (bisect.bisect_left(mark_keys.get(index, ()), x) for x in (start, start + size))
        functions[n] = (code, relocs, [(v - start, k) for v, k in marks.get(index, ())[low:high]])
    return functions


def delinked_object(source: Path) -> Path | None:
    """dsd's delinked object of a source file, which holds Black's code with its relocations and mapping symbols."""
    try:
        relative = source.resolve().relative_to(ROOT)
    except ValueError:
        return None
    path = ROOT / "build" / VERSION / "delinks" / relative.with_suffix(".o")
    return path if path.exists() else None


ADDRESSES: dict[str, int] = {}


def symbol_address(name: str) -> int | None:
    """The address of `symbol` or `symbol+0x4` from symbols.txt, the mode bit cleared; None if it isn't there."""
    if not ADDRESSES:  # emptied by symbol_index() when a symbols.txt changed
        for symbols in (ROOT / "config" / VERSION / "arm9").rglob("symbols.txt"):
            ADDRESSES.update((m[1], int(m[2], 16)) for m in re.finditer(r"^(\S+) kind:\S+ addr:(0x[0-9a-f]+)",
                                                                          symbols.read_text(), re.M))
    m = re.fullmatch(r"(.+?)([+-]0x[0-9a-f]+)?", name)
    return None if m[1] not in ADDRESSES else (ADDRESSES[m[1]] + int(m[2] or "0", 16)) & ~1


def datum_address(name: str) -> int | None:
    """symbol_address() of a datum's symbol: an odd address is a byte's (field_actor_tool.c's data_ov010_021714c1),
    not a Thumb function's mode bit."""
    symbol_address(name)  # (reads the symbols.txt files once)
    m = re.fullmatch(r"(.+?)([+-]0x[0-9a-f]+)?", name)
    return None if m[1] not in ADDRESSES else ADDRESSES[m[1]] + int(m[2] or "0", 16)


VENEER_TARGETS: dict[str, dict[str, int]] = {}  # {a symbols.txt's directory: {veneer: its target's address}}


def same_place(ours: str, theirs: str, functions=()) -> bool:
    """Whether two relocations' symbols are one: the same address, or Black's is a linker veneer that jumps to ours
    (tools/veneers.py): the link makes the veneer for the call of the far function, so the C calls that. functions:
    the functions the C file defines; one of them that Black's symbols lack is not the place of a known symbol
    (field_buildmodel.c's FieldPropSystem_InitResource called a second copy of Black's 0x0218bc10 and matched)."""
    a, b = symbol_address(ours), symbol_address(theirs)
    if a is None and b is not None and re.sub(r"[+-]0x[0-9a-f]+$", "", ours) in functions:
        return False
    if a is None or b is None or a == b:
        return True
    entry = symbol_index().get(re.sub(r"[+-]0x[0-9a-f]+$", "", theirs))
    if not entry:
        return False
    if entry[0] not in VENEER_TARGETS:
        import veneers
        VENEER_TARGETS[entry[0]] = {}
        try:
            mod = veneers.Module(veneers.mod_of_dir(Path(entry[0])))
        except (KeyError, FileNotFoundError):
            mod = None
        for v in mod.veneers if mod else ():
            if mod.caller(v) and v[3] is not None:
                VENEER_TARGETS[entry[0]].update((n, v[3] & ~1) for n in v[2])
    return VENEER_TARGETS[entry[0]].get(theirs) == a


LINK: dict = {}  # link_names()'s {name: objects}, once a process


def object_names(path: Path) -> set[str]:
    """The names a relocatable object defines and the link sees: global or weak (MWCC's weak binds 14), not local."""
    data = path.read_bytes()
    shoff = struct.unpack_from("<I", data, 0x20)[0]
    shentsize, shnum = struct.unpack_from("<HH", data, 0x2E)
    sections = [struct.unpack_from("<10I", data, shoff + i * shentsize) for i in range(shnum)]
    symtab = next((s for s in sections if s[1] == 2), None)
    if symtab is None:
        return set()
    strtab = sections[symtab[6]]
    strings = data[strtab[4]:strtab[4] + strtab[5]]
    out = set()
    for name, _, _, info, _, shndx in struct.iter_unpack("<IIIBBH", data[symtab[4]:symtab[4] + symtab[5] // 16 * 16]):
        if shndx and info >> 4 and name:
            n = strings[name:strings.find(bytes(1), name)].decode(errors="replace")
            if n[:1] not in ("$", "@", ".", ""):
                out.add(n)
    return out


def link_names() -> dict[str, tuple[str, ...]] | None:
    """{name: the objects that define it} over the link's objects, build/b_us/objects.txt (paths as it writes them,
    relative to the repo), and the names the linker scripts define (OVERLAY_<n>_ID, ARM9_BSS_END ...: by "lcf");
    None before a build wrote objects.txt. Cached in build/cache by the objects' stamps."""
    if "names" in LINK:
        return LINK["names"]
    listing = ROOT / "build" / VERSION / "objects.txt"
    if not listing.exists():
        LINK["names"] = None
        return None
    objects = [l.strip().strip(chr(34)) for l in listing.read_text().splitlines() if l.strip()]
    scripts = [ROOT / "build" / VERSION / n for n in ("arm9.lcf", "arm9.o.lcf")]
    token = bwcache.files_token([listing, *scripts, *(ROOT / o for o in objects)], __file__)
    names = bwcache.get("link_names", str(ROOT), token)
    if names is None:
        found: dict[str, list[str]] = {}
        for o in objects:
            if (ROOT / o).exists():
                for n in object_names(ROOT / o):
                    found.setdefault(n, []).append(o)
        for lcf in scripts:
            if lcf.exists():
                for n in re.findall(r"^\s+(\w+)\s*=\s*[^;:]*;", lcf.read_text(), re.M):
                    found.setdefault(n, []).append("lcf")
        names = {n: tuple(os_) for n, os_ in found.items()}
        bwcache.put("link_names", str(ROOT), token, names)
    LINK["names"] = names
    return names


def own_objects(source: Path) -> set[str]:
    """The paths objects.txt gives a source's compiled and delinked objects, which an older C made."""
    try:
        relative = source.resolve().relative_to(ROOT).with_suffix(".o")
    except ValueError:
        return set()
    return {f"build/{VERSION}/{relative}", f"build/{VERSION}/delinks/{relative}"}


def undefined(name: str, defined=(), own=()) -> bool:
    """Whether a relocation's symbol is one the link can't resolve: not in a symbols.txt, not defined by the compiled
    object (defined: its names), nor by an object of the link other than the file's own (own: own_objects()), nor by
    the linker scripts. bsubway_scr.c called Black 2's func_ov033_0217c11c, which nothing defines, and matched 32/32:
    the bytes of a call are relocated, so only its symbol can tell. False when there is no build to look in."""
    base = re.sub(r"[+-]0x[0-9a-f]+$", "", name)
    if base in defined or symbol_address(base) is not None:
        return False
    names = link_names()
    return names is not None and not any(o not in own for o in names.get(base, ()))


def symbol_value(name: str) -> int | None:
    """What a relocated word holds once linked: an address, or the number of the linker's OVERLAY_<n>_ID."""
    m = re.fullmatch(r"OVERLAY_(\d+)_ID", name)
    return int(m[1]) if m else symbol_address(name)


def is_address(value: int) -> bool:
    return 0x01FF8000 <= value < 0x02800000


def defined_functions(source: Path) -> list[str]:
    """The functions a C file defines at the top level, other than inline ones, which aren't emitted on their own."""
    text = source.read_text(errors="replace")
    pattern = r"^(?![ \t#/])(?!.*\binline\b)[^;{}()=\n]*?\b(\w+)\([^;{}]*\)\s*\{"
    return [m.group(1) for m in re.finditer(pattern, text, re.M)]


PC_LOAD = re.compile(r"^ldr\w*\s+\w+, \[pc, #(-?(?:0x[0-9a-f]+|\d+))\]$")


def pool_marks(data: bytes, address: int, thumb: bool) -> list[tuple[int, str]]:
    """Mapping symbols for code that has none (Black's, without a delinked object): the words pc-relative loads read
    are data, the rest code."""
    code = "$t" if thumb else "$a"
    words: set[int] = set()
    for _ in range(4):  # a pool word read as an instruction may look like a load itself; drop those and look again
        marks = sorted({(w, "$d") for w in words} | {(w + 4, code) for w in words if w + 4 not in words} | {(0, code)})
        found = set()
        for start, end, kind in segments(marks, len(data)):
            if kind == "$d":
                continue
            md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if thumb else capstone.CS_MODE_ARM)
            md.skipdata = True
            for x in md.disasm(data[start:end], address + start):
                m = PC_LOAD.match(f"{x.mnemonic} {x.op_str}")
                if m:
                    pc = (x.address + 4) & ~3 if thumb else x.address + 8
                    target = pc + int(m[1], 0) - address
                    if 0 <= target <= len(data) - 4 and target % 4 == 0:
                        found.add(target)
        if found == words:
            break
        words = found
    return sorted({(w, "$d") for w in words} | {(w + 4, code) for w in words if w + 4 not in words} | {(0, code)})


def segments(marks: list[tuple[int, str]], size: int) -> list[tuple[int, int, str]]:
    marks = [(o, k) for o, k in marks if o < size]
    return [(o, marks[i + 1][0] if i + 1 < len(marks) else size, k) for i, (o, k) in enumerate(marks)
            if (marks[i + 1][0] if i + 1 < len(marks) else size) > o]


def normalize(ins: str) -> str:
    """An instruction as the diff compares it: addresses and branch targets as #ADDR, literal pool offsets left out,
    and bl against blx ignored (the call is relocated, and the linker makes it a blx when the callee is in the other
    mode)."""
    ins = re.sub(r"#0x[0-9a-f]{7}\b", "#ADDR", ins)
    ins = re.sub(r"^blx #ADDR$", "bl #ADDR", ins)
    return re.sub(r"\[pc, #-?(?:0x[0-9a-f]+|\d+)\]", "[pc]", ins)


def inner_data(name: str, relocs: dict[int, str | None], marks: list[tuple[int, str]] | None, size: int,
               thumb: bool) -> list[tuple[int, str]] | None:
    """marks with data from each offset of a Thumb function that a relocated word of it points to (name+0x80, even:
    code's would hold the Thumb bit) where no mark is: data the function holds inside its code, which dsd's object
    marks as code (os_reset_itcm.c's OSi_VerifyHmac key, whose pool word dsd relocates but no $d marks)."""
    if not thumb or not marks:
        return marks
    at = {o for o, _ in marks}
    inside = re.compile(re.escape(name) + r"\+0x([0-9a-f]+)")
    found = {int(m[1], 16) for m in (inside.fullmatch(x or "") for x in relocs.values()) if m}
    found = {o for o in found if o % 2 == 0 and o < size and o not in at}
    return sorted(marks + [(o, "$d") for o in found]) if found else marks


def disassemble(data: bytes, address: int, thumb: bool, relocs: dict[int, str | None] | None = None,
                marks: list[tuple[int, str]] | None = None) -> list[tuple[str, str, str, str | None]]:
    """[(address, key, text, symbol)]: the instructions, and the literal pool's words as `.word`, which capstone would
    otherwise read as instructions (two Thumb ones a word, a pointer's zero word in our object as `movs r0, r0`). A
    relocated call or word shows its symbol; the key is what the diff compares, where any address is the same."""
    relocs = relocs or {}
    if not marks or not any(k == "$d" for _, k in marks):
        marks = pool_marks(data, address, thumb)
    elif marks[0][0]:
        marks = [(0, "$t" if thumb else "$a")] + marks
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if thumb else capstone.CS_MODE_ARM)
    # Words capstone can't decode are skipped as data, so that the listing goes on to the end of the function
    md.skipdata = True
    lines = []
    for start, end, kind in segments(marks, len(data)):
        if kind == "$d":
            for o in range(start, end, 4):
                n = min(4, end - o)
                value = int.from_bytes(data[o : o + n], "little")
                symbol = relocs.get(o)
                text = f".word {symbol}" if symbol else f".word {value:#x}" if n == 4 else f".short {value:#x}"
                # an address: relocated, or one that dsd left unrelocated in Black's pool; an overlay's ID by its number
                linked = symbol_value(symbol) if symbol else None
                key = f".word {linked:#x}" if linked is not None and not is_address(linked) else \
                    ".word ADDR" if o in relocs or n == 4 and is_address(value) else text
                lines.append((f"{address + o:08x}", key, text, symbol))
            continue
        for x in md.disasm(data[start:end], address + start):
            o = x.address - address
            hit = next((r for r in relocs if o <= r < o + x.size), None)
            symbol = relocs[hit] if hit is not None else None
            key = normalize(f"{x.mnemonic} {x.op_str}")
            text = re.sub(r"^blx? #ADDR$", f"bl {symbol}", key) if symbol else key
            lines.append((f"{x.address:08x}", key, text, symbol))
    return lines


def aligned_diff(ours: list, theirs: list, functions=()) -> tuple[list[str], int]:
    """Returns the hunks that differ between two disassemblies, aligned by difflib so that an instruction added or
    left out shifts nothing after it, and how many calls or addresses name another symbol than the original's where
    the instructions are the same at the same offset (both symbols known, at different addresses), which are hunks as
    well."""
    a, b = [line[1] for line in ours], [line[1] for line in theirs]
    lines, renamed = [], 0

    def hunk(i1, i2, j1, j2):
        lines.append(f"@@ ours {ours[i1][0] if i1 < len(ours) else '-'} / original {theirs[j1][0] if j1 < len(theirs) else '-'}")
        for k in range(max(i2 - i1, j2 - j1)):
            lines.append(f"  {ours[i1 + k][2] if i1 + k < i2 else '':40s} | {theirs[j1 + k][2] if j1 + k < j2 else ''}")

    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if tag != "equal":
            hunk(i1, i2, j1, j2)
            continue
        for k in range(i2 - i1):
            x, y = ours[i1 + k][3], theirs[j1 + k][3]
            if x and y and x != y and ours[i1 + k][0] == theirs[j1 + k][0] and not same_place(x, y, functions):
                renamed += 1
                hunk(i1 + k, i1 + k + 1, j1 + k, j1 + k + 1)
    return lines, renamed


def masked_hunks(data: bytes, orig_data: bytes, address: int, thumb: bool, relocs: dict[int, str | None]) -> list[str]:
    """The instructions whose bytes differ where the diff shows them the same: a branch's target, which normalize()
    masks (br_net.c's func_ov186_021ff72c: a forward `b` to another `movs r5, #1`, and an empty diff). Each with its
    real operands, a target inside the function as an offset into it."""
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if thumb else capstone.CS_MODE_ARM)
    md.skipdata = True
    listing = lambda d: {x.address - address: (x.size, f"{x.mnemonic} {x.op_str}") for x in md.disasm(d, address)}
    ours, theirs = listing(data), listing(orig_data)
    inside = lambda m: f"func+{int(m[1], 16) - address:#x}" if 0 <= int(m[1], 16) - address < len(data) else m[0]
    rel = lambda t: re.sub(r"#0x([0-9a-f]{7,8})\b", inside, t)
    masked = {o + k for o in relocs for k in range(4)}
    out = [f"  {rel(ours[o][1]):40s} | {rel(theirs[o][1])}" for o in sorted(ours)
           if o in theirs and data[o:o + ours[o][0]] != orig_data[o:o + theirs[o][0]]
           and not any(o + k in masked for k in range(ours[o][0]))]
    return ["@@ the same instructions, but these bytes differ (branch targets as offsets into the function):"] + out \
        if out else []


def renamed_in_place(relocs: dict[int, str | None], orig_relocs: dict[int, str | None], functions=()) -> int:
    """How many relocations at the same offset name symbols at different addresses (both in symbols.txt), a call
    of a far function where Black's calls a veneer to it aside (same_place)."""
    count = 0
    for o, x in relocs.items():
        y = orig_relocs.get(o)
        if x and y and x != y:
            count += not same_place(x, y, functions)
    return count


def source_ranges(source: Path) -> tuple[str | None, dict]:
    """(module, {section: (start, end)}) of a source file in the delinks.txt files, or (None, {})."""
    from file_ranges import read_delinks
    try:
        relative = str(source.resolve().relative_to(ROOT))
    except ValueError:
        return None, {}
    for mod, (_, files) in read_delinks(str(ROOT / "config" / VERSION / "arm9")).items():
        if relative in files:
            return mod, files[relative]
    return None, {}


def rom_bytes(mod: str | None, address: int, size: int) -> bytes | None:
    """Black's bytes at an address, of the module (else main's), or None where no module's binary holds them (.bss)."""
    from file_ranges import module_dir
    config = ROOT / "config" / VERSION / "arm9"
    modules = load_modules()
    for key in ([os.path.normpath(str(config / module_dir(mod)))] if mod else []) + [str(config)]:
        if key in modules:
            binary, base = modules[key]
            if binary not in BINARIES:
                BINARIES[binary] = binary.read_bytes()
            if base <= address and address + size <= base + len(BINARIES[binary]):
                return BINARIES[binary][address - base:address - base + size]
    return None


def static_misses(obj: Path, source: Path, statics: dict, originals: dict, sizes: dict) -> dict[str, list[str]]:
    """{function: [a hunk's two lines for each pool word whose static datum lands elsewhere than Black's word points]}
    (gaps.md 13m). A static is named in no relocation (the C's statics often carry another table's address in their
    name: poke_tool.c's data_0209e298 is Black's stat_nature_adjust, 0x0209e2a4), so it is judged by where it lands:
    its section's place in the file's delinks.txt ranges (port_file.place(), as dsd's linker script lays the object
    out there, against the address of Black's symbol at the same offset of the function. Where they
    differ, the read is of another datum when the section is laid out as Black's (every other read of the object into
    it lands on Black's, and one more does) and another of our data lands where Black's word points (md5.c's
    CPSi_md5_result reading the other u8[64] table, a complete file). Else (an incomplete file whose data is in
    another order: scrcmd_shop.c's .rodata 6 bytes off from a datum on) the bytes at both places decide, the datum's
    (to the end of the local symbol that holds it, relocated words left out) against the ROM's: a function's own
    initializer (@123) with other values than Black's table is no match (pokemontrade_2d.c's func_ov119_021e60bc, its
    pairs swapped), but a static of Black's name, or whose name is the address Black's word points at, is the datum
    Black reads, its bytes the file's data to fix (trainer_card.c's data_ov111_021c8544, research_graph.c's
    data_ov217_021c3ac8+0x3 for data_ov217_021c3acb). With no bytes to compare (.bss), it passes as before.
    sizes: {function: its size in our object}; a function of another size than Black's is not looked at."""
    from port_file import place, read_object
    d, secs, syms, relocs = read_object(obj)
    mod, ranges = source_ranges(source)
    where = place(secs, ranges)
    reads, same, other = [], {}, {}
    for name, found in statics.items():
        orig = originals.get(name)
        if not orig or len(orig[0]) != sizes.get(name):
            continue
        for o, (sec, off, label) in sorted(found.items()):
            theirs = orig[1].get(o)
            b = datum_address(theirs) if theirs else None
            if b is None:
                continue
            a = where[sec] + off if sec in where else None
            reads.append((name, o, sec, off, a, b, label, theirs))
            if a is not None:
                tally = same if a == b else other
                tally[sec] = tally.get(sec, 0) + 1
    landed = {where[i] + v for x, i, v, *_ in syms if i in where and x and x[0] not in "$."}  # our data's places
    out: dict[str, list[str]] = {}
    for name, o, sec, off, a, b, label, theirs in reads:
        if a == b:
            continue
        laid = a is not None and other.get(sec, 0) == 1 and same.get(sec, 0) >= 1 and b in landed
        if not laid:
            if secs[sec][1] != 1:  # NOBITS: no bytes to tell
                continue
            end = next((v + n for _, i, v, n, *_ in syms if i == sec and n and v <= off < v + n), off + 4)
            end = min(end, secs[sec][3])
            ours = bytearray(d[secs[sec][2] + off:secs[sec][2] + end])
            black = rom_bytes(mod, b, len(ours))
            if not ours or black is None:
                continue
            black = bytearray(black)
            for r, *_ in relocs.get(sec, ()):  # an address in our datum is relocated, Black's linked: not compared
                for k in range(max(r, off), min(r + 4, end)):
                    ours[k - off] = black[k - off] = 0
            base = [re.sub(r"[+-]0x[0-9a-f]+$", "", x) for x in (label, theirs)]
            if ours == black or label[0] not in "@." and (base[0] == base[1] or datum_address(label) == b):
                continue
        at = f"({a:#x})" if a is not None else "(not placed)"
        out.setdefault(name, []).extend([f"@@ the pool word at +{o:#x}: a static that lands elsewhere than Black's",
                                         f"  {'.word ' + label + ' ' + at:40s} | .word {theirs} ({b:#x})"])
    return out


def delinked_functions(path: Path | None) -> dict:
    if path is None:
        return {}
    mtime = path.stat().st_mtime_ns
    if DELINKED.get(path, (None,))[0] != mtime:
        DELINKED[path] = (mtime, object_functions(path))
    return DELINKED[path][1]


class ProbeTimeout(Exception):
    pass


PROBE_TIMEOUT = int(os.environ.get("BW_PROBE_TIMEOUT", "300"))  # seconds one probe may take (0: no limit)


def probe_source(*args, **kwargs):
    """_probe_source under a time limit (BW_PROBE_TIMEOUT, 300 s): a probe that runs past it raises ProbeTimeout, which
    local_driver.probe_file reports as a failed compile, so one stuck variant costs its search a candidate, not a
    worker's brief its whole run (func_ov230_02188a48: every probe of a DS Protect function scanned every delinked
    object with pyelftools, 3.5 s each, and a tweak search made hundreds). Only where SIGALRM can be had: the main
    thread of the process (a pool worker is one)."""
    import signal
    import threading
    if not PROBE_TIMEOUT or threading.current_thread() is not threading.main_thread() or not hasattr(signal, "setitimer"):
        return _probe_source(*args, **kwargs)

    def expire(signum, frame):
        raise ProbeTimeout(f"probe timed out after {PROBE_TIMEOUT} s")
    old = signal.signal(signal.SIGALRM, expire)
    signal.setitimer(signal.ITIMER_REAL, PROBE_TIMEOUT)
    try:
        return _probe_source(*args, **kwargs)
    finally:
        signal.setitimer(signal.ITIMER_REAL, 0)
        signal.signal(signal.SIGALRM, old)


def _probe_source(source: Path, compilers: list[str] | None = None, flags: str = "", functions: set[str] | None = None,
                 full: bool = False, cwd: Path | None = None, original: Path | None = None):
    """Compiles a C file with each compiler and compares its functions: ({function: {compiler: status}}, [diff lines],
    {compiler: its output, for one that failed}). original: the file whose delinked object holds Black's relocations
    (for a copy of a file under another name); cwd: where the compiler runs, which a relative source is relative to."""
    here = source if source.is_absolute() else Path(cwd or Path.cwd()) / source
    compiler, file_flags = compiler_of(original or source)  # a copy (brief.py --variants) compiles as its file does
    compilers_dir = TOOLS / "mwccarm" / "dsi"
    compilers = compilers or [compiler.removeprefix("dsi/")]
    modules = load_modules()
    symbol_index(refresh=True)  # read again if a symbols.txt changed since the last call
    originals = delinked_functions(delinked_object(original or here))

    results: dict[str, dict[str, str]] = {}
    diffs: list[str] = []
    failed: dict[str, str] = {}
    with tempfile.TemporaryDirectory() as tmp:
        for compiler in compilers:
            obj = Path(tmp) / f"{compiler.replace('/', '_')}.o"
            command = [
                str(TOOLS / "wibo"),
                str((TOOLS / "mwccarm" / compiler if "/" in compiler else compilers_dir / compiler) / "mwccarm.exe"),
                *shlex.split(" ".join(file_flags)),
                *flags.split(),
                *VERSION_FLAGS,
                "-w", "off",
                *(arg for d in VERSION_INCLUDES for arg in ("-i", str(ROOT / d))),
                "-o", str(obj),
                str(source),
            ]
            proc = subprocess.run(command, capture_output=True, text=True, cwd=cwd)
            if proc.returncode != 0:
                failed[compiler] = proc.stdout + proc.stderr
                continue
            statics: dict = {}
            compiled = object_functions(obj, statics)
            here_names, own = object_names(obj) | set(compiled), own_objects(original or here)
            missed = static_misses(obj, original or here, statics, originals,
                                   {n: len(v[0]) for n, v in compiled.items()}) if statics else {}
            # MWCC drops a static function that nothing references, which would otherwise pass unnoticed
            for name in defined_functions(here):
                if name not in compiled and (functions is None or name in functions):
                    results.setdefault(name, {})[compiler] = "not emitted"
            for name, (data, relocs, marks) in compiled.items():
                if functions is not None and name not in functions:
                    continue
                original_function = find_function(name, modules)
                if original_function is None:
                    results.setdefault(name, {})[compiler] = "unknown"
                    continue
                orig_data, addr, thumb = original_function
                # Black's relocations and pool from dsd's object when it has the function under this name (the
                # bytes always from the ROM)
                _, orig_relocs, orig_marks = originals.get(name, (None, {}, None))
                # (set for every function: a size mismatch skips the check, and the diff below reads it)
                unresolved = []
                if len(orig_data) != len(data):
                    status = f"size {len(data):#x}/{len(orig_data):#x}"
                else:
                    masked = {o + k for o in relocs for k in range(4)}
                    count = sum(1 for i in range(len(data)) if i not in masked and data[i] != orig_data[i])
                    # bytes the same, but a call or address of another symbol than the original's, or of one
                    # that nothing defines
                    unresolved = [o for o, x in sorted(relocs.items()) if x and undefined(x, here_names, own)]
                    renamed = 0 if count else (renamed_in_place(relocs, orig_relocs, compiled)
                                               + len(missed.get(name, ())) // 2 + len(unresolved))
                    status = "MATCH" if count == renamed == 0 else f"{count} bytes" if count else f"{renamed} symbols"
                results.setdefault(name, {})[compiler] = status

                if compiler == compilers[0] and (status != "MATCH" or full == "all"):  # "all": a match's too
                    ours = disassemble(data, addr, thumb, relocs, inner_data(name, relocs, marks, len(data), thumb))
                    theirs = disassemble(orig_data, addr, thumb, orig_relocs,
                                         inner_data(name, orig_relocs, orig_marks, len(orig_data), thumb))
                    diffs.append(f"--- {name} ({compiler}): ours | original")
                    diffs += missed.get(name, [])
                    for o in unresolved:
                        theirs_name = orig_relocs.get(o)
                        diffs += [f"@@ ours {addr + o:08x}: {relocs[o]} is in no symbols.txt and no object of the "
                                  "link defines it (the link would say Undefined): "
                                  + (f"Black's is {theirs_name}" if theirs_name else "call or name Black's")]
                    if not full:
                        diffs += aligned_diff(ours, theirs, compiled)[0] or masked_hunks(data, orig_data, addr, thumb, relocs)
                        continue
                    for i in range(max(len(ours), len(theirs))):
                        a = f"{ours[i][0]}: {ours[i][2]}" if i < len(ours) else ""
                        b = f"{theirs[i][0]}: {theirs[i][2]}" if i < len(theirs) else ""
                        mark = " " if i < min(len(ours), len(theirs)) and ours[i][1] == theirs[i][1] else "*"
                        diffs.append(f"{mark} {a:45s} | {b}")
    return results, diffs, failed


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("source", type=Path)
    parser.add_argument("--compilers", help="comma-separated dsi compiler versions, or 'all'; other builds by their "
                        "directory under tools/mwccarm, as in 2.0/sp2p2. The diff is of the first. Default: the file's")
    parser.add_argument("--flags", default="", help="flags added after the file's, where the last of an option wins; "
                        "written --flags=\"...\" since they start with -")
    parser.add_argument("--functions", help="comma-separated functions to report and diff; all by default")
    parser.add_argument("--full", action="store_true", help="show the whole side-by-side listing instead of the "
                        "aligned hunks that differ")
    args = parser.parse_args()

    functions = set(args.functions.split(",")) if args.functions else None
    compilers_dir = TOOLS / "mwccarm" / "dsi"
    compilers = sorted(p.name for p in compilers_dir.iterdir()) if args.compilers == "all" else \
        args.compilers.split(",") if args.compilers else [compiler_of(args.source)[0].removeprefix("dsi/")]
    results, diffs, failed = probe_source(args.source, compilers, args.flags, functions, args.full)
    for compiler, output in failed.items():
        print(f"{compiler}: compile failed\n{output}")
    if diffs:
        print("\n".join(diffs))
    width = max((len(n) for n in results), default=8)
    print(f"{'function':{width}s}  " + "  ".join(f"{c:>12s}" for c in compilers))
    for name, row in results.items():
        print(f"{name:{width}s}  " + "  ".join(f"{row.get(c, '-'):>12s}" for c in compilers))
    return 0 if all(s == "MATCH" for row in results.values() for s in row.values()) else 1


if __name__ == "__main__":
    sys.exit(main())
