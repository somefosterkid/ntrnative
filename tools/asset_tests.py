#!/usr/bin/env python3
"""Tests for tools/asset.py and tools/asset_table.py: layouts, the .inc a table gets from bytes, a C initialiser's
records, the layout found from a C table and the ROM's bytes (with names mixed in), and the whole move of a table and
its row into a made-up tree.

    python3 tools/asset_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os
import struct
import sys
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import asset  # noqa: E402
import asset_table as at  # noqa: E402

SYMS = {0x02000100: ["func_a"], 0x02000200: ["data_b"], 0x02000300: ["func_c"]}


def check(name, cond):
    print(("ok   " if cond else "FAIL ") + name)
    return cond


def tests():
    ok = True
    spec = "u16,u8[0=NONE/255=END]*3,ptr,s32"
    cols = at.parse_layout(spec)
    ok &= check("layout round trip", at.layout_spec(cols) == spec and len(cols) == 6)
    offs, stride = at.offsets(cols)
    ok &= check("C alignment: u16 at 0, u8s at 2-4, ptr at 8, s32 at 12", offs == [0, 2, 3, 4, 8, 12] and stride == 16)
    ok &= check("trailing padding", at.offsets(at.parse_layout("u16,u8"))[1] == 4 and at.offsets(at.parse_layout("u8*3"))[1] == 3)

    data = struct.pack("<HBBBxxxIi", 0x155, 255, 3, 0, 0x02000101, -2) + struct.pack("<HBBBxxxIi", 7, 0, 0, 0, 0, 5)
    text = at.render(data, spec, SYMS)
    ok &= check("render: names, hex, thumb pointer by symbol, NULL, signed decimal",
                text == "    { 0x155, END, 0x03, NONE, func_a, -2 },\n    { 0x07, NONE, NONE, NONE, NULL, 5 },\n", )
    ok &= check("a ref is written &name", at.render(struct.pack("<I", 0x02000200), "ref", SYMS) == "    &data_b,\n")
    ok &= check("a single-field table is bare values", at.render(struct.pack("<2H", 1, 2), "u16[2=TWO]", SYMS) == "    0x01,\n    TWO,\n")

    recs = at.parse_init("{ { 1, 2, {3, 4} }, /* c */ { 5, END, {6, NULL} }, // x\n }")
    ok &= check("initialiser records are flattened", recs == [["1", "2", "3", "4"], ["5", "END", "6", "NULL"]])
    ok &= check("scalar elements are records of one", at.parse_init("{ 1, 2, -3 }") == [["1"], ["2"], ["-3"]])
    for bad in ("{ .x = 1 }", "{ {1, \"s\"} }", "#if X\n{ {1,2} }"):
        try:
            at.parse_init(bad)
            ok &= check("bails on " + bad[:12], False)
        except at.Bail:
            ok &= check("bails on " + repr(bad[:12]), True)

    rom = (struct.pack("<HBBI", 0x155, 1, 255, 0x02000101) + struct.pack("<HBBI", 7, 2, 255, 0) +
           struct.pack("<HBBI", 9, 3, 255, 0x02000200)) + b"\xaa" * 8
    recs = at.parse_init("{ {0x155, 1, END, func_a}, {7, 2, END, NULL}, {9, 3, END, data_b} }")
    spec2, stride2 = at.infer(recs, rom, SYMS, len(rom) - 8)
    ok &= check("infer: widths, the macro's value and pointer found", (spec2, stride2) == ("u16,u8,u8[255=END],ptr", 8))
    ok &= check("infer: the .inc gives back the C's names", at.render(rom[:24], spec2, SYMS).splitlines()[0] ==
                "    { 0x155, 0x01, END, func_a },")
    try:
        at.infer(at.parse_init("{ {1, 2}, {3, 4}, {5, 6}, {7, 99} }"), struct.pack("<8B", 1, 2, 3, 4, 5, 6, 7, 8), SYMS, 8)
        ok &= check("infer rejects values that are not the ROM's", False)
    except at.Bail:
        ok &= check("infer rejects values that are not the ROM's", True)
    # a literal too big for a field is not that field
    recs = at.parse_init("{ {0x155, 1}, {0x156, 2}, {0x157, 3}, {0x158, 4} }")
    ok &= check("a value wider than u8 makes its field u16", at.infer(recs, struct.pack("<4H4H", 0x155, 1, 0x156, 2, 0x157, 3, 0x158, 4)[:16], {}, 16)[0] == "u16,u8")
    # 152 is not an s8: a column with a negative is signed, and 0x98 in one byte is -104
    recs = at.parse_init("{ {152, -1}, {152, -1}, {152, -1}, {152, -1} }")
    try:
        r = at.infer(recs, struct.pack("<8h", 152, -1, 152, -1, 152, -1, 152, -1), {}, 16)
    except at.Bail:
        r = ("bail",)
    ok &= check("a signed column keeps its range", r[0] == "u8,s16")
    # two different names that read the same bytes are no field of names (a shifted column of zeros would fit)
    try:
        at.infer(at.parse_init("{ {A, 1}, {B, 2}, {A, 3}, {B, 4} }"), struct.pack("<8B", 0, 1, 0, 2, 0, 3, 0, 4), {}, 8)
        ok &= check("infer: names of one value are rejected", False)
    except at.Bail:
        ok &= check("infer: names of one value are rejected", True)
    # a wide record of fields that all fit every width must give up, not search 3^fields layouts
    import time
    t0 = time.process_time()   # CPU time: wall time fails on a loaded machine
    wide = at.parse_init("{ " + ", ".join("{" + ",".join(["0"] * 60) + "}" for _ in range(8)) + " }")
    try:
        at.infer(wide, bytes(60 * 8 * 2) + b"" * 16, {}, 60 * 8 * 2)
    except at.Bail:
        pass
    ok &= check("a wide all-zero record is decided in seconds", time.process_time() - t0 < 30)
    # constant names, expressions and [INDEX] = designators: values come from the file's #defines and enums
    import tempfile
    import c_consts
    with tempfile.TemporaryDirectory() as tmp:
        root = os.path.join(tmp, "third_party/pokebw2")
        os.makedirs(os.path.join(root, "include"))
        open(os.path.join(root, "include/k.h"), "w").write(
            "#define BASE 0x10\n#define LIM (BASE * 2 + 1)\n#ifdef BLACK2\n#define VER 2\n#else\n#define VER 9\n#endif\n"
            "#if defined(WHITE2)\n#define W 1\n#endif\nenum { E_A, E_B = 5, E_C, E_D = LIM | 0x100 };\n#define CAST ((u8)-1 & 0xf)\n")
        open(os.path.join(tmp, "x.c"), "w").write('#include "k.h"\n#include "asset/gone.inc"\n')
        old = c_consts.ROOT, c_consts.VENDORED
        c_consts.ROOT, c_consts.VENDORED = Path(tmp), Path(root)
        c_consts.consts.cache_clear()
        try:
            c = c_consts.consts("x.c", "b2_us")
            vals = [c.value(n) for n in ("BASE", "LIM", "VER", "W", "E_A", "E_B", "E_C", "E_D", "CAST", "NOPE", "BASE * 3", "'a'")]
            ok &= check("c_consts: defines, #ifdef, enums, expressions, casts", vals == [16, 33, 2, None, 0, 5, 6, 0x121, 15, None, 48, 97])
            ok &= check("c_consts: the other version's #ifdef", c_consts.Consts("x.c", "w2_us").value("VER") == 9)
            open(os.path.join(tmp, "y.c"), "w").write(
                '#include "k.h"\n#define GRID FX32_CONST(16)\nstatic const u8 sA[] = { 1, 2, (3), };\n'
                'static const u16 sB[4][2] = {0};\nstatic const u16 sC[] = {\n#include "asset/y_sC.inc"\n};\n')
            os.makedirs(os.path.join(tmp, "config/b2_us"))
            open(os.path.join(tmp, "config/b2_us/assets.tsv"), "w").write("name\tmodule\tstart\tend\ttype\ny_sC\tov1\t0x10\t0x1c\tu16,u8\n")
            c = c_consts.Consts("y.c", "b2_us")
            vals = [c.value(e) for e in ("FX32_CONST(1.5)", "FX32_CONST(-0.25f)", "77 * GRID", "FX16_CONST(BASE)",
                                         "GX_RGB(31, 0, 2)", "GX_RGBA(1, 1, 1, 1)", "GX_BG_SCRBASE(0x2000)",
                                         "GX_BG_CHARBASE(0x0c000)", "NELEMS(sA)", "NELEMS(sB)", "NELEMS(sC)", "-0.2f", "NELEMS(sZ)")]
            ok &= check("c_consts: FX32_CONST, GX_RGB, GX_BG_*, NELEMS (declared, counted, moved)",
                        vals == [6144, -1024, 77 * 65536, 65536, 31 | 2 << 10, 0x8421, 4, 3, 3, 4, 3, None, None] or print(vals))
        finally:
            c_consts.ROOT, c_consts.VENDORED = old
            c_consts.consts.cache_clear()
    names = {"A": 1, "B": 2, "C": 300}
    recs = at.parse_init("{ [2] = { A, B }, [3] = { C, (1 + 1) } }", names.get)
    ok &= check("designators: skipped indexes are zero records", recs == [["0", "0"], ["0", "0"], ["A", "B"], ["C", "(1 + 1)"]])
    rom = struct.pack("<8H", 0, 0, 0, 0, 1, 2, 300, 2)
    spec2, stride2 = at.infer(recs, rom, {}, len(rom), lambda t: {"A": 1, "B": 2, "C": 300, "(1 + 1)": 2}.get(t))
    ok &= check("infer: names evaluated give the layout", (spec2, stride2) == ("u16,u8", 4))
    try:
        at.parse_init("{ { 1, 2 }, { \"a\", 3 } }")
        ok &= check("strings are refused", False)
    except at.Bail:
        ok &= check("strings are refused", True)
    try:
        at.infer([], b"", {}, 0)
        ok &= check("an empty initialiser is a Bail, not an IndexError", False)
    except at.Bail:
        ok &= check("an empty initialiser is a Bail, not an IndexError", True)
    try:
        at.infer(at.parse_init("{ {1, sizeof(x)}, {2, 3}, {4, 5}, {6, 7} }"), bytes(32), {}, 32)
        ok &= check("an expression without a value is a Bail", False)
    except at.Bail as e:
        ok &= check("an expression without a value is a Bail", "no value" in str(e))
    ok &= tree_moves()
    # // comments inside a table must not hide the values after them
    ok &= check("comments: // ends at the line", at.strip_comments("{1, // x\n 2}") == "{1, \n 2}")
    return ok


def tree_moves():
    """The fallbacks in a made-up Black 2 / White 2 tree: a static local found by its bytes (one place per version, its
    twin at another address), a bitfield table turned into a union with the ROM's bytes, and what is refused."""
    import tempfile
    import c_consts
    ok = True
    with tempfile.TemporaryDirectory() as tmp:
        t = Path(tmp)
        local = struct.pack("<8H", 7, 300, 9, 1000, 11, 1200, 13, 1400)
        bits = bytes([0x21, 0x43, 0x65, 0x87, 0xa9, 0xcb, 0xed, 0x0f])
        for v, pad in (("b2_us", 0), ("w2_us", 8)):   # White 2's data sits 8 bytes later
            d = t / "third_party/pokebw2/config" / v / "arm9/overlays/ov001"
            d.mkdir(parents=True)
            (d / "delinks.txt").write_text("    .rodata     start:0x02100000 end:0x02100040 kind:rodata align:4\n\n"
                                          "src/ov001/x.c:\n    complete\n")
            (d / "symbols.txt").write_text(f"sBits kind:data(any) addr:{0x02100020 + pad:#x}\n"
                                           f"sAfter kind:data(any) addr:{0x02100028 + pad:#x}\n")
            img = t / "extract" / v / "arm9_overlays/ov001.bin"
            img.parent.mkdir(parents=True)
            img.write_bytes(bytes(4 + pad) + local + bytes(16 - len(local)) + bytes(12) + bits + bytes(0x18 - pad))
            (t / "config" / v).mkdir(parents=True)
            (t / "config" / v / "assets.tsv").write_text("name\tmodule\tstart\tend\ttype\n")
        c = t / "third_party/pokebw2/src/ov001/x.c"
        c.parent.mkdir(parents=True)
        c.write_text("typedef struct { u16 a : 4; u16 b : 12; } Bits;\nstatic const Bits sBits[];\nint g(void) { return sBits[0].b; }\n"
                     "static const Bits sBits[] = {\n    { 1, 0x432 }, { 5, 0x876 }, { 9, 0xcba }, { 13, 0x0fe },\n};\n"
                     "int f(int i) {\n    static const u16 table[] = { 7, 300, 9, 1000, 11, 1200, 13, 1400 };\n"
                     "    return table[i] + sBits[i].a + NELEMS(sBits);\n}\n")
        old = asset.ROOT, at.ROOT, c_consts.ROOT, c_consts.VENDORED
        asset.ROOT = at.ROOT = c_consts.ROOT = t
        c_consts.VENDORED = t / "third_party/pokebw2"
        at.symbol_names.cache_clear()
        c_consts.consts.cache_clear()
        try:
            cf = "third_party/pokebw2/src/ov001/x.c"
            skipped, n = asset.move_all("b2_us", cf, min_values=4)
            text = c.read_text()
            b2 = (t / "config/b2_us/assets.tsv").read_text().splitlines()[1:]
            w2 = (t / "config/w2_us/assets.tsv").read_text().splitlines()[1:]
            ok &= check("fallbacks: both tables moved", n == 2 and not skipped and "7, 300" not in text and "0x432" not in text)
            ok &= check("static local: found by its bytes in each version",
                        "x_table\tov001\t0x02100004\t0x02100014\tu16" in b2 and "x_table\tov001\t0x0210000c\t0x0210001c\tu16" in w2)
            ok &= check("bitfields: a union with the bytes up to the next symbol",
                        "x_sBits\tov001\t0x02100020\t0x02100028\tu8" in b2 and "x_sBits\tov001\t0x02100028\t0x02100030\tu8" in w2
                        and "static const union { u8 raw[8]; Bits t[4]; } sBits_raw = { {" in text
                        and "#define sBits (sBits_raw.t)" in text and "sBits[i].a" in text
                        and text.index("sBits_raw") < text.index("int g(void)") and text.count("Bits sBits") == 0)
            # a name two files' statics share: the one in the file's own sections (ov142's sItemNameOrder)
            d = t / "third_party/pokebw2/config/b2_us/arm9/overlays/ov001"
            (d / "delinks.txt").write_text("    .rodata     start:0x02100000 end:0x02100040 kind:rodata align:4\n\n"
                                          "src/ov001/w.c:\n    .rodata     start:0x02100000 end:0x02100010\n\n"
                                          "src/ov001/x.c:\n    .rodata     start:0x02100010 end:0x02100040\n")
            (d / "symbols.txt").write_text((d / "symbols.txt").read_text() + "sTwin kind:data(any) addr:0x02100004\n"
                                           "sTwin kind:data(any) addr:0x02100020\n")
            ok &= check("a shared static name: the file's own address", asset.symbol_addr("b2_us", "ov001", "sTwin", cf) == 0x02100020
                        and asset.symbol_addr("b2_us", "ov001", "sTwin") == 0x02100004)
            # refused: a table of two places (ambiguous), one with pointers
            c.write_text("static const u16 sTwice[] = { 0, 0, 0, 0, 0, 0 };\n"
                         "typedef struct { u16 *p; u16 x : 4; } P;\nstatic const P sBits[] = { { NULL, 1 }, { NULL, 2 } };\n")
            c_consts.consts.cache_clear()
            skipped, n = asset.move_all("b2_us", cf, min_values=1)
            ok &= check("refused: ambiguous bytes, pointers", n == 0 and "places" in skipped.get("sTwice", "")
                        and "pointers" in skipped.get("sBits", ""))
            # a table of an unnamed struct: the name follows the type's closing brace
            c.write_text("static const struct {\n    u16 a;\n    u16 b;\n} sAfter[] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };\n")
            c_consts.consts.cache_clear()
            skipped, n = asset.move_all("b2_us", cf, min_values=1)
            ok &= check("an unnamed struct's table is tried", "sAfter" in skipped or n == 1)
            if not ok:
                print(skipped, text, b2, w2, sep="\n")
        finally:
            asset.ROOT, at.ROOT, c_consts.ROOT, c_consts.VENDORED = old
            at.symbol_names.cache_clear()
            c_consts.consts.cache_clear()
    return ok


if __name__ == "__main__":
    sys.exit(0 if tests() else 1)
