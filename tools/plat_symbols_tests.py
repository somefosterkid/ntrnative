#!/usr/bin/env python3
"""Tests for tools/plat_symbols.py: the tables an object holds (file-scope, static local; none with a pointer), their places in module images, and asset.py's reading of config/plat_us's rows.

    python3 tools/plat_symbols_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import asset  # noqa: E402
import asset_table  # noqa: E402
import plat_symbols as ps  # noqa: E402

C = r"""
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef struct { u8 a, b; u16 c; } T;
static const T sTable[4] = { { 1, 2, 300 }, { 4, 5, 600 }, { 7, 8, 900 }, { 10, 11, 1200 } };
static const u8 sSmall[4] = { 1, 2, 3, 4 };
static void f(void) {}
static const struct { u32 n; void (*fn)(void); } sPtrs[3] = { { 1, f }, { 2, f }, { 3, f } };
const void *g(int i) {
    static const u16 sLocal[8] = { 9, 8, 7, 6, 5, 4, 3, 2 };
    const u32 copy[16] = { 0x111, 0x222, 0x333, 0x444, 0x555, 0x666, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    volatile u32 x = copy[i];
    (void)x;
    return i ? (const void *)sLocal : i > 1 ? (const void *)sPtrs : (const void *)sTable;
}
const void *h(void) { return sSmall; }
"""


def check(name, cond):
    print(("ok   " if cond else "FAIL ") + name)
    return cond


def tests():
    ok = True
    with tempfile.TemporaryDirectory() as tmp:
        src, obj = Path(tmp) / "t.c", Path(tmp) / "t.o"
        src.write_text(C)
        subprocess.run(["clang", "-O0", "-m32", "-c", str(src), "-o", str(obj)], check=True)
        found = ps.object_tables(obj)
        tables = {n: d for n, d, _ in found}
        holes = {n: h for n, _, h in found}
    table = b"".join(struct.pack("<BBH", a, b, c) for a, b, c in ((1, 2, 300), (4, 5, 600), (7, 8, 900), (10, 11, 1200)))
    ok &= check("a file-scope table: its bytes as MWCC lays them out", tables.get("sTable") == table)
    ok &= check("a static local by its C name", tables.get("sLocal") == struct.pack("<8H", 9, 8, 7, 6, 5, 4, 3, 2))
    ok &= check("a local array's initialiser has no symbol (clang's private .L constant): not placed", "copy" not in tables)
    ok &= check("a table with pointers: their offsets are holes", holes.get("sPtrs") == (4, 12, 20))
    ok &= check("one under 12 bytes is left out", "sSmall" not in tables)
    ok &= check("args32: 32-bit pointers", ps.args32(["-DSDK_64BIT", "-fPIC", "-O0"], tmp)[:2] == ["-DSDK_32BIT", "-O0"])

    images = {"main": (0x02000000, b"\0" * 8 + table + b"\0" * 4), "ov001": (0x021d0000, b"\1" * 6 + table[:8])}
    ok &= check("places: module and address", ps.places(table, images) == [("main", 0x02000008)])
    ptrs = struct.pack("<6I", 1, 0x02001234, 2, 0x02005678, 3, 0x02009abc)
    pimg = {"main": (0x02000000, b"\5" * 4 + ptrs + struct.pack("<6I", 1, 9, 2, 9, 4, 9))}
    ok &= check("places: any 4 bytes at a pointer", ps.places(struct.pack("<6I", 1, 0, 2, 0, 3, 0), pimg, (4, 12, 20)) ==
                [("main", 0x02000004)])
    ok &= check("fixed: the bytes besides the pointers", ps.fixed(ptrs, (4, 12, 20)) == struct.pack("<3I", 1, 2, 3))
    ok &= check("places: every place of a part", ps.places(table[:8], images) == [("main", 0x02000008), ("ov001", 0x021d0006)])
    two = [("ov001", 0x021d0006), ("ov001", 0x021d0046), ("main", 0x02000008)]
    ok &= check("at_its_name: the address in an Unk_ name", ps.at_its_name("Unk_ov1_021D0046", two) == [("ov001", 0x021d0046)])
    ok &= check("at_its_name: a name with no address", ps.at_its_name("sTable", two) == [])
    ok &= check("file_module: where the tables with one place are", ps.file_module(["ov002", "ov001", "ov001"], []) == "ov001")
    ok &= check("file_module: no table with one place: the module most of the others have a place in",
                ps.file_module([], [[("main", 1), ("ov001", 2), ("ov001", 3)], [("ov001", 4)], [("main", 5), ("ov001", 6)]]) == "ov001")
    ok &= check("file_module: a tie is no module", ps.file_module([], [[("main", 1)], [("ov001", 2)]]) is None)
    ok &= check("file_module: no tables", ps.file_module([], []) is None)

    rows = [("third_party/pokeplatinum/src/a.c", "sTable", "ov001", 0x021d0010, 0x021d0020),
            ("third_party/pokeplatinum/src/a.c", "sOther", "ov001", 0x021d0040, 0x021d0048)]
    real = asset_table.plat_rows
    asset_table.plat_rows = lambda: rows
    try:
        ok &= check("asset.py: a Platinum file's module", asset.module_of("plat_us", rows[0][0]) == "ov001")
        ok &= check("asset.py: a symbol's address", asset.symbol_addr("plat_us", "ov001", "sOther", rows[0][0]) == 0x021d0040)
        ok &= check("asset.py: the end of a table's bytes", asset.section_end("plat_us", "ov001", rows[0][0], 0x021d0014) == 0x021d0020)
        asset_table.symbol_names.cache_clear()
        ok &= check("asset_table: the names of a module", asset_table.symbol_names("plat_us", "ov001") ==
                    {0x021d0010: ["sTable"], 0x021d0040: ["sOther"]})
    finally:
        asset_table.plat_rows = real
        asset_table.symbol_names.cache_clear()
    return ok


if __name__ == "__main__":
    sys.exit(0 if tests() else 1)
