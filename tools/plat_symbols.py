#!/usr/bin/env python3
"""Platinum's table addresses for tools/asset.py: pret pokeplatinum has no symbols.txt (no dsd config), so the places of
its C tables in the ROM are found by their bytes.

    python3 tools/plat_symbols.py [--rom ROM] FILE...      # FILE: third_party/pokeplatinum/src/... .c

Each file is compiled to an object with tools/pokeplat_cc.py's clang line made 32-bit (-m32, SDK_32BIT: a table of
fixed-width integers and pointers is laid out as MWCC lays it out, a pointer in 4 bytes). Each data symbol of the object (a file-scope table, a static local `Func.name`; a local array's
initialiser is a private .L constant without a symbol: asset.py finds it by its values) with at least 12 bytes not all alike besides its pointers is
searched in Platinum's module images (main and the overlays, BLZ-decompressed: tools/rom_modules.py writes
extract/plat_us), a pointer (a relocation of the object) matching any 4 bytes; the one place that holds its bytes is
its row:

    config/plat_us/symbols.tsv    file, symbol, module, start, end (the C name: a local's after the last dot)
    config/plat_us/modules.tsv    module, the address of its image's first byte (main's load address, an overlay's RAM
                                  address from the ROM's overlay table)

A table at several places takes the one in the file's module (where its other tables are, at one place each); a name
two tables of one file share, a table at no place or at several of its module gets no row. The rows of
the files given are replaced; the others stay. asset.py --version plat_us reads both files as it reads a dsd
symbols.txt and delinks.txt (the module of a file, a symbol's address, the end of its bytes).
"""
import argparse
import multiprocessing
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import pokeplat_cc  # noqa: E402
import rom_modules  # noqa: E402

VERSION = "plat_us"
CONFIG = ROOT / "config" / VERSION
HEADER = "file\tsymbol\tmodule\tstart\tend\n"
MIN_BYTES = 12


def object_tables(obj):
    """[(name, bytes, holes)] of an object's data symbols: holes, the offsets of its pointers (4-byte relocations)."""
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    out = []
    with open(obj, "rb") as f:
        elf = ELFFile(f)
        relocs = {}
        for s in elf.iter_sections():
            if isinstance(s, RelocationSection):
                relocs.setdefault(s["sh_info"], []).extend(r["r_offset"] for r in s.iter_relocations())
        symtab = elf.get_section_by_name(".symtab")
        for sym in symtab.iter_symbols() if symtab else []:
            if sym["st_info"]["type"] != "STT_OBJECT" or not isinstance(sym["st_shndx"], int) or sym["st_size"] < MIN_BYTES:
                continue
            sec = elf.get_section(sym["st_shndx"])
            if sec["sh_type"] == "SHT_NOBITS":
                continue
            lo, hi = sym["st_value"], sym["st_value"] + sym["st_size"]
            holes = tuple(sorted(r - lo for r in relocs.get(sym["st_shndx"], []) if lo <= r < hi))
            data = sec.data()[lo:hi]
            if len(fixed(data, holes)) < MIN_BYTES:
                continue
            out.append((sym.name.rsplit(".", 1)[-1], data, holes))
    return out


def fixed(data, holes):
    """The bytes of a table that are not its pointers."""
    keep = bytearray(data)
    for h in holes:
        keep[h:h + 4] = b"\0" * 4
    return bytes(b for i, b in enumerate(keep) if not any(h <= i < h + 4 for h in holes))


def args32(args, tmp):
    """pokeplat_cc's 64-bit host line as a 32-bit one: pointers in 4 bytes, as on the DS."""
    stubs = Path(tmp) / "inc" / "gnu"
    stubs.mkdir(parents=True, exist_ok=True)
    (stubs / "stubs-32.h").write_text("")   # (glibc's 32-bit stubs header: no i386 libc is installed, none is linked)
    return ([{"-DSDK_64BIT": "-DSDK_32BIT"}.get(x, x) for x in args if x != "-fPIC"] +
            ["-m32", "-idirafter/usr/include/x86_64-linux-gnu", "-idirafter" + str(stubs.parent)])


def compile_one(task):
    src, args, tmp = task
    obj = os.path.join(tmp, f"{abs(hash(src))}.o")
    r = subprocess.run(["clang", *args, src, "-c", "-o", obj], cwd=pokeplat_cc.PLAT,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return src, (object_tables(obj) if r.returncode == 0 else None)


def module_images():
    """{module: (base, image)} of Platinum's main and overlays (written from the ROM when missing)."""
    ext = ROOT / "extract" / VERSION
    bases = dict((m, int(a, 16)) for m, a in (l.split("\t") for l in (CONFIG / "modules.tsv").read_text().splitlines()[1:]))
    out = {"main": (bases["main"], (ext / "arm9/arm9.bin").read_bytes())}
    for p in sorted((ext / "arm9_overlays").glob("ov*.bin")):
        out[p.stem] = (bases[p.stem], p.read_bytes())
    return out


def write_modules(rom_path):
    """extract/plat_us's images and config/plat_us/modules.tsv from the ROM."""
    rom = Path(rom_path).read_bytes()
    if not (ROOT / "extract" / VERSION / "arm9/arm9.bin").exists():
        rom_modules.write_images(VERSION, rom_path)
    rows = [("main", rom_modules.struct.unpack_from("<I", rom, 0x28)[0])]
    _, _, ovt, ovt_size = rom_modules.struct.unpack_from("<4I", rom, 0x48)
    rows += [(f"ov{rom_modules.struct.unpack_from('<I', rom, e)[0]:03d}", rom_modules.struct.unpack_from("<I", rom, e + 4)[0])
             for e in range(ovt, ovt + ovt_size, 32)]
    CONFIG.mkdir(parents=True, exist_ok=True)
    (CONFIG / "modules.tsv").write_text("module\tbase\n" + "".join(f"{m}\t{a:#010x}\n" for m, a in rows))


def places(data, images, holes=()):
    """[(module, address)] of each place of the images that holds data (any 4 bytes at each hole)."""
    cuts = sorted({0, len(data)} | {c for h in holes for c in (h, h + 4)})
    runs = [(a, b) for a, b in zip(cuts, cuts[1:]) if not any(h <= a < h + 4 for h in holes)]
    a0, b0 = max(runs, key=lambda r: r[1] - r[0])   # the longest run without a pointer finds the candidates
    hits = []
    for mod, (base, image) in images.items():
        i = image.find(data[a0:b0])
        while i >= 0:
            at = i - a0
            if 0 <= at and at + len(data) <= len(image) and all(image[at + a:at + b] == data[a:b] for a, b in runs):
                hits.append((mod, base + at))
            i = image.find(data[a0:b0], i + 1)
    return hits


NAME_ADDRESS = re.compile(r"_([0-9A-Fa-f]{8})$")   # pret names a table it has no name for by its address: Unk_ov61_0222E550


def at_its_name(name, hits):
    """The hits at the address the name gives (two identical tables sit side by side: each is at its own name)."""
    m = NAME_ADDRESS.search(name)
    return [h for h in hits if m and h[1] == int(m[1], 16)]


def file_module(unique, many):
    """The module a file's code is in: where its tables with one place are; failing that, the one module that holds a place
    of more of its tables than any other (many: the hits of each table at several places); else None."""
    if unique:
        return max(set(unique), key=unique.count)
    count = {}
    for hits in many:
        for mod in {m for m, _ in hits}:
            count[mod] = count.get(mod, 0) + 1
    best = sorted(count.values(), reverse=True)
    return max(count, key=count.get) if best and (len(best) == 1 or best[0] > best[1]) else None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="+")
    ap.add_argument("--rom", default=str(ROOT / "orig" / "baserom_plat_us.nds"))
    a = ap.parse_args()
    write_modules(a.rom)
    images = module_images()
    files = [str(Path(f).resolve().relative_to(ROOT)) if Path(f).exists() else f for f in a.files]
    with tempfile.TemporaryDirectory() as tmp, multiprocessing.get_context("fork").Pool(int(os.environ.get("BW_JOBS", "8"))) as pool:
        args = args32(pokeplat_cc.plat_args(), tmp)
        found = dict(pool.map(compile_one, [(str(ROOT / f), args, tmp) for f in files]))
    rows, failed, n_none, n_many = [], [], 0, 0
    for f in files:
        tables = found[str(ROOT / f)]
        if tables is None:
            failed.append(f)
            continue
        names = [n for n, _, _ in tables]
        many, mods = [], []
        for name, data, holes in tables:
            if names.count(name) > 1 or len(set(fixed(data, holes))) < 2:
                continue
            hits = places(data, images, holes)
            n_none += not hits
            if len(hits) == 1:
                rows.append((f, name, hits[0][0], hits[0][1], hits[0][1] + len(data)))
                mods.append(hits[0][0])
            elif hits:
                many.append((name, data, hits))
        own = file_module(mods, [h for _, _, h in many])
        for name, data, hits in many:   # a table at several places: the one its name gives, else the first in the file's module
            hits = at_its_name(name, hits) or sorted(h for h in hits if h[0] == own)[:1]
            n_many += len(hits) != 1
            if len(hits) == 1:
                rows.append((f, name, hits[0][0], hits[0][1], hits[0][1] + len(data)))
    table = CONFIG / "symbols.tsv"
    done = set(files) - set(failed)   # (a file that does not compile keeps its rows)
    keep = [l for l in (table.read_text().splitlines()[1:] if table.exists() else []) if l.split("\t")[0] not in done]
    new = [f"{f}\t{n}\t{m}\t{s:#010x}\t{e:#010x}" for f, n, m, s, e in rows]
    table.write_text(HEADER + "".join(l + "\n" for l in sorted(keep + new)))
    print(f"plat_symbols: {len(rows)} tables placed in {len(files) - len(failed)} files ({n_none} at no place, "
          f"{n_many} at several); {len(failed)} files do not compile" + (": " + " ".join(failed[:10]) if failed else ""))


if __name__ == "__main__":
    main()
