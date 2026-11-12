#!/usr/bin/env python3
"""The ARM9 module images from the base ROM without dsd, and the asset .inc files from them (macOS and iOS builds).

dsd (ninja's extract step) is a Linux x86_64 program. This reads the .nds itself and writes what tools/asset.py reads
of dsd's extraction: extract/<version>/arm9/arm9.bin (main) and extract/<version>/arm9_overlays/ovNNN.bin, each
BLZ-decompressed. Only main and the overlays: the ARM9i (ltd_autoload_0) is modcrypted, and no file the port builds
includes its asset.

    python3 tools/rom_modules.py [--version b_us] [ROM]   # the images (default ROM: orig/baserom_<version>.nds)
    python3 tools/rom_modules.py --assets ROM_DIR [--version V] [--stamp F]  # port/meson.build's step: every version whose
        # ROM_DIR/baserom_<version>.nds exists and has no dsd extraction (extract/<version>/config.yaml): its images if
        # missing or older than the ROM or this file, then each main and ovNNN row of its assets.tsv to
        # <asset_root>/asset/<name>.inc (build/asset for Black, build/w_us/asset for White, as configure.py), each
        # file rewritten only when its text changes

The ROM's layout (GBATEK, "DS Cartridge Header" and "DS Cartridge NitroROM and NitroARM7/9 Overlays"): the header
gives the ARM9's ROM offset, load address and size, the ARM9 overlay table (32 bytes an entry: id, RAM address, RAM
size, BSS size, static init start and end, file id, and a word whose bit 24 says compressed and whose low 24 bits are
the compressed size) and the FAT (start and end offset per file id). The ARM9's module params end in the words
0xDEC00621 0x2106C0DE (this ROM has no nitro footer after the ARM9 to point at them); module params + 0x14 is
compressed_static_end, the load address where the ARM9's compressed part ends (0: not compressed).

BLZ, the DS's backward LZ (GBATEK "LZ Decompression Functions", ndstool's BLZ): the last 8 bytes of the compressed
part are a footer, a word of (header length << 24 | compressed length) and a word of how many bytes decompression
adds. Decompression runs from the end down: a flag byte, then 8 items, MSB first, each a literal byte (0) or a 2-byte
reference (1; high byte read first) of length (v >> 12) + 3 at distance (v & 0xFFF) + 3 above the output position.
Bytes below the compressed length are stored as they are.
"""
import argparse
import os
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import asset  # noqa: E402

# configure.py's VERSIONS[version]["asset_root"]; Black 2's and White 2's: port/meson.build's include dir per game
ASSET_ROOT = {"b_us": "build", "w_us": "build/w_us", "b2_us": "build/b2_us", "w2_us": "build/w2_us"}


def blz_decompress(data):
    """The decompressed bytes of a BLZ buffer whose footer is its last 8 bytes."""
    word, extra = struct.unpack_from("<II", data, len(data) - 8)
    hdr_len, enc_len = word >> 24, word & 0xFFFFFF
    out = bytearray(data) + bytearray(extra)
    src, dst, end = len(data) - hdr_len, len(out), len(data) - enc_len
    while src > end:
        src -= 1
        flags = out[src]
        for bit in range(8):
            if src <= end:
                break
            if flags & (0x80 >> bit):
                src -= 2
                pair = out[src + 1] << 8 | out[src]
                n, disp = (pair >> 12) + 3, (pair & 0xFFF) + 3
                for _ in range(n):
                    dst -= 1
                    out[dst] = out[dst + disp]
            else:
                src -= 1
                dst -= 1
                out[dst] = out[src]
    if dst != src:
        sys.exit(f"BLZ: the output ends at {dst:#x}, the input at {src:#x}: not a BLZ buffer")
    return bytes(out)


def arm9(rom):
    """Main's image: the ARM9 binary, its compressed part decompressed."""
    off, _, load, size = struct.unpack_from("<4I", rom, 0x20)
    params = rom.find(struct.pack("<II", 0xDEC00621, 0x2106C0DE), off, off + size) - 0x1C
    if params < off:
        sys.exit("no module params in the ARM9 (its magic 0xDEC00621 0x2106C0DE)")
    end = struct.unpack_from("<I", rom, params + 0x14)[0]   # compressed_static_end
    image = rom[off:off + size]
    return blz_decompress(image[:end - load]) + image[end - load:] if end else image


def overlays(rom):
    """(id, image) for each ARM9 overlay, decompressed."""
    fat, _, ovt, ovt_size = struct.unpack_from("<4I", rom, 0x48)
    for e in range(ovt, ovt + ovt_size, 32):
        ov_id, file_id, word = struct.unpack_from("<I", rom, e)[0], *struct.unpack_from("<II", rom, e + 24)
        start, stop = struct.unpack_from("<II", rom, fat + 8 * file_id)
        data = rom[start:stop]
        yield ov_id, blz_decompress(data[:word & 0xFFFFFF]) if word >> 24 & 1 else data


def write_images(version, rom_path):
    rom = Path(rom_path).read_bytes()
    out = ROOT / "extract" / version
    (out / "arm9").mkdir(parents=True, exist_ok=True)
    (out / "arm9_overlays").mkdir(exist_ok=True)
    (out / "arm9/arm9.bin").write_bytes(arm9(rom))
    n = 0
    for ov_id, image in overlays(rom):
        (out / f"arm9_overlays/ov{ov_id:03d}.bin").write_bytes(image)
        n += 1
    print(f"rom_modules: {out}: main and {n} overlays")


def write_assets(rom_dir, versions=("b_us", "w_us")):
    for version in versions:
        rom = Path(rom_dir) / f"baserom_{version}.nds"
        table = ROOT / "config" / version / "assets.tsv"
        if not rom.exists() or (ROOT / "extract" / version / "config.yaml").exists():
            continue
        if (ROOT / "config" / version / "delta.txt").exists():
            # White's assets.tsv is made from Black's and the delta (configure.py runs it on the PC; it writes only changes)
            subprocess.run([sys.executable, str(ROOT / "tools/version_config.py"), version], check=True, stdout=subprocess.DEVNULL)
        if not table.exists():
            continue
        main = ROOT / "extract" / version / "arm9/arm9.bin"
        if not main.exists() or main.stat().st_mtime < max(rom.stat().st_mtime, Path(__file__).stat().st_mtime):
            write_images(version, rom)
        written = 0
        for row in table.read_text().splitlines()[1:]:
            name, mod, start, end, kind = row.split("\t")
            if not (mod == "main" or mod.startswith("ov")):
                continue
            out = ROOT / ASSET_ROOT[version] / "asset" / f"{name}.inc"
            tmp = out.with_suffix(".tmp")
            asset.write_asset(version, mod, int(start, 16), int(end, 16), kind, tmp)   # plain elements or a table's records
            if out.exists() and out.read_bytes() == tmp.read_bytes():
                tmp.unlink()
            else:
                os.replace(tmp, out)
                written += 1
        if written:
            print(f"rom_modules: {written} asset files written to {ROOT / ASSET_ROOT[version] / 'asset'}")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("rom", nargs="?", help="the .nds, or with --assets the directory of baserom_<version>.nds")
    ap.add_argument("--version", help="the images' version (default b_us); with --assets only this version's (default: "
                    "Black's and White's; Black 2's and White 2's only so, port/meson.build's step per game)")
    ap.add_argument("--assets", action="store_true")
    ap.add_argument("--stamp", help="with --assets: a file to touch after (the meson step's output)")
    a = ap.parse_args()
    if a.assets:
        write_assets(a.rom or ROOT / "orig", [a.version] if a.version else ("b_us", "w_us"))
        if a.stamp:
            Path(a.stamp).touch()
    else:
        version = a.version or "b_us"
        write_images(version, a.rom or ROOT / "orig" / f"baserom_{version}.nds")


if __name__ == "__main__":
    main()
