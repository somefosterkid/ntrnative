#!/usr/bin/env python3
"""Make each downloaded Metrowerks compiler start with the x87 at 53-bit precision, as Windows starts a process.

wibo runs the compilers with the x87 at 64-bit precision. MWCC's switch lowering compares cases / range against the
double 0.4, and at exactly 0.4 (two cases 4 apart: 2/5, 4/10, 6/15) only 53-bit precision makes them equal: under
wibo the compare tree is rooted at another case than Black's (docs/lessons.md, demo3d_capture.c). Each .exe gets a
stub in the zero slack at the end of its code section, `push 0x27f; fldcw [esp]; add esp, 4; jmp <entry>`, and its
entry point moved to the stub. Idempotent: an .exe whose entry is already such a stub is left as it is.

    python3 tools/mwcc_fpu.py              # every tools/mwccarm/*/*.exe (configure.py runs it after the download)
    python3 tools/mwcc_fpu.py --check      # which compilers have it
"""
import argparse
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
STUB = bytes.fromhex("687f020000" "d92c24" "83c404") + b"\xe9"  # push 0x27f; fldcw [esp]; add esp, 4; jmp rel32


def patch(exe, check=False):
    """Returns "patched", "already" or why not."""
    b = bytearray(exe.read_bytes())
    pe = struct.unpack_from("<I", b, 0x3c)[0]
    if b[pe:pe + 4] != b"PE\0\0":
        return "not a PE file"
    nsec, opt_size = struct.unpack_from("<H", b, pe + 6)[0], struct.unpack_from("<H", b, pe + 20)[0]
    opt = pe + 24
    entry_off = opt + 16
    entry = struct.unpack_from("<I", b, entry_off)[0]
    sections = []
    for i in range(nsec):
        s = opt + opt_size + 40 * i
        vsize, va, raw_size, raw = struct.unpack_from("<IIII", b, s + 8)
        flags = struct.unpack_from("<I", b, s + 36)[0]
        sections.append((va, vsize, raw, raw_size, flags))

    def file_off(rva):
        return next((raw + rva - va for va, vsize, raw, raw_size, _ in sections if va <= rva < va + raw_size), None)

    at = file_off(entry)
    if at is not None and bytes(b[at:at + len(STUB)]) == STUB:
        return "already"
    if check:
        return "not yet"
    size = len(STUB) + 4
    for va, vsize, raw, raw_size, flags in sections:
        if not flags & 0x20000000:  # executable
            continue
        end = raw + raw_size  # the slack: zeros between the code's virtual size and the raw size
        start = raw + max(vsize, 0)
        start = (start + 15) & ~15
        if start + size <= end and not any(b[start:start + size]):
            rva = va + start - raw
            b[start:start + len(STUB)] = STUB
            struct.pack_into("<i", b, start + len(STUB), entry - (rva + size))
            struct.pack_into("<I", b, entry_off, rva)
            if vsize < start - raw + size:  # the loader maps only the virtual size
                struct.pack_into("<I", b, opt + opt_size + 40 * sections.index((va, vsize, raw, raw_size, flags))
                                 + 8, start - raw + size)
            exe.write_bytes(bytes(b))
            return "patched"
    return "no slack in a code section"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true", help="only say which compilers have the stub")
    args = ap.parse_args()
    exes = sorted((ROOT / "tools" / "mwccarm").glob("*/*/*.exe")) + sorted((ROOT / "tools" / "mwccarm").glob("*/*.exe"))
    for exe in exes:
        if exe.name.lower().startswith("mwcc"):
            print(f"{exe.relative_to(ROOT)}: {patch(exe, args.check)}")
    if not exes:
        sys.exit("no compilers in tools/mwccarm: run configure.py first")


if __name__ == "__main__":
    main()
