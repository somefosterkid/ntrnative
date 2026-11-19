#!/usr/bin/env python3
"""Tests for tools/port_assets.py: the pointer-name list (asset_ptrs.tsv) round trip, a placeholder's pointer fields, and
the source rewrite of each declaration shape an assets.tsv row is included from (a static table with an alignment, the
first member of a static union, an automatic array, an automatic struct whose initialiser goes on after the table).

    python3 tools/port_assets_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import port_assets as pa  # noqa: E402


def check(name, cond):
    print(("ok   " if cond else "FAIL ") + name)
    return cond


def rewritten(src):
    with tempfile.TemporaryDirectory() as d:
        p = Path(d) / "t.c"
        p.write_text(src)
        found = []
        return pa.rewrite(p, [], found), found


def tests():
    ok = True
    with tempfile.TemporaryDirectory() as d:
        ptrs = Path(d) / "asset_ptrs.tsv"
        ptrs.write_text("name\tfield\tc (each record's)\nt\t1\tNULL func_a &data_b\n")
        got = pa.read_ptrs(ptrs)
        ok &= check("ptrs: one line per field, the records' texts in order",
                    got == {("t", 0, 1): "NULL", ("t", 1, 1): "func_a", ("t", 2, 1): "&data_b"})
        ph = pa.placeholder("b_us", ("t", "ov001", 0x100, 0x118, "u32,ptr"), got)
        ok &= check("placeholder: zeros and the listed pointers",
                    ph.splitlines()[-3:] == ["    { 0, NULL },", "    { 0, func_a },", "    { 0, &data_b },"])

    out, found = rewritten('static const u16 tab[] ATTRIBUTE_ALIGN(4) = {\n#include "asset/a.inc"\n};\n')
    ok &= check("static table with an alignment: const dropped, alignment kept",
                "static  u16 tab [] ATTRIBUTE_ALIGN(4)  __attribute__" in out and found[0][:2] == ("a", "tab"))

    out, found = rewritten('#define X 1\nstatic const struct {\n    u16 a;\n} s[] = {\n#include "asset/d.inc"\n};\n')
    ok &= check("static table of an anonymous struct: the declaration from `static`",
                "static  struct {\n    u16 a;\n} s [] " in out and found[0][:2] == ("d", "s"))

    out, found = rewritten('int x;\nstatic const union { u8 raw[8]; u16 t[4]; } v_raw = { {\n#include "asset/b.inc"\n} };\n')
    ok &= check("first member of a static union: the union is the table",
                "static  union { u8 raw[8]; u16 t[4]; } v_raw " in out and found[0][:2] == ("b", "v_raw"))

    src = 'void f(void) {\n    const Rect rects[4] = {\n#include "asset/c.inc"\n    };\n    g(rects);\n}\n'
    out, found = rewritten(src)
    ok &= check("automatic array: a static table before the function, a copy where the declaration was",
                out.index("static  Rect bwasset_local_rects_") < out.index("void f(void)") and
                "__typeof__(bwasset_local_rects_" in out and "__builtin_memcpy(&rects, &bwasset_local_rects_" in out and
                out.count("\n") >= src.count("\n") and found[0][0] == "c")

    src = 'void f(void) {\n    T s = {\n#include "asset/e.inc"\n        A, B\n    };\n    g(&s);\n}\n'
    out, found = rewritten(src)
    ok &= check("automatic struct with more fields after the table: the whole initialiser moves",
                "#include \"asset/e.inc\"\n        A, B\n" in out.split("void f(void)")[0] and
                "__builtin_memcpy(&s, &bwasset_local_s_" in out and found[0][0] == "e")

    src = 'u16 f(u16 v) {\n    int i;\n    static const struct {\n        u16 k;\n        u16 v;\n    } t[] = {\n#include "asset/g.inc"\n};\n    return t[v].v;\n}\n'
    out, found = rewritten(src)
    ok &= check("static of an anonymous struct in a function: the declaration from `static`, before the function",
                "static   struct {\n        u16 k;\n        u16 v;\n    } bwasset_local_t_" in out.split("u16 f(u16 v)")[0] and
                found[0][0] == "g")

    ok &= check("n8: a 4-byte value of the row in a 1-byte slot (host 5 bytes, DS 8 per record)",
                pa.host_layout("n8,u8*4") == (5, 8) and pa.host_size("n8,u8*4", 0, 656) == 410)

    ok &= check("pokebw2's files and its stand-ins take its include dirs, Black's Black's",
                pa.B2_ROOT / "generated" in pa.inc_dirs(pa.B2_ROOT / "src/ov011/x.c") and
                pa.B2_ROOT / "generated" in pa.inc_dirs(pa.ROOT / "port/extern_b2w2/src/system/x.c") and
                pa.inc_dirs(pa.ROOT / "src/x.c") == [pa.ROOT / "port/include", pa.ROOT / "include"])

    name = pa.rows("b_us")[0][0]
    if not pa.rows("w_us"):  # config/w_us/assets.tsv is written from the White ROM (ignored by git): the White block has no rows
        print("skip registration: no config/w_us/assets.tsv")
        return ok
    with tempfile.TemporaryDirectory() as d:
        src, out = Path(d) / "t.c", Path(d) / "o.c"
        src.write_text(f'static const u8 t[] = {{\n#include "asset/{name}.inc"\n}};\n')
        pa.write_src(src, out, Path(d) / "o.d")
        reg = out.read_text().split('#line 1 "tools/port_assets.py"')[1]
        blocks = [b for b in reg.splitlines() if b.startswith("#")]
        ok &= check("registration: a block per version, Black's last (#else), each its game's code",
                    blocks == ["#if defined(BLACK2)", "#elif defined(WHITE2)", "#elif defined(WHITE)", "#else", "#endif"] and
                    f'PORT_AssetAdd("IRBO", "{name}"' in reg.split("#else")[1] and
                    f'PORT_AssetAdd("IRAO", "{name}"' in reg.split("#elif defined(WHITE)")[1].split("#else")[0])
    return ok


if __name__ == "__main__":
    sys.exit(0 if tests() else 1)
