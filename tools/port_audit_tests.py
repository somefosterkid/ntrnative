#!/usr/bin/env python3
"""Tests for tools/port_audit.py: md5.c's port read the padding table where the original's pool loads the word index,
and its round macros were expressions where pokebw2 has statements (s816, s808).

    python3 tools/port_audit_tests.py      # "ok" per test, exit 1 on a failure
"""
import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import port_audit as A  # noqa: E402

BS = chr(92)
OURS = ("#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))\n"
        "#define FF(a, b, c, d, x, t, s) ((a) = (b) + ROTL((a) + F(b, c, d) + (x) + (t), s))\n"
        "#define S11 7\n"
        "#define OLD(p) ((p)[0])\n")
THEIRS = ("#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))\n"
          "#define FF(a, b, c, d, x, t, s)        " + BS + "\n"
          "    {                                  " + BS + "\n"
          "        (a) += F(b, c, d) + (x) + (t); " + BS + "\n"
          "        (a) = (b) + ROTL(a, s);        " + BS + "\n"
          "    }\n"
          "#define S11 7 // the first rotation\n"
          "#define NEW(p) ((p)[1])\n")


def test_macros_continuations():
    m = A.macros(THEIRS)
    assert m["FF"][0] == ["a", "b", "c", "d", "x", "t", "s"]
    assert m["FF"][1].startswith("{ (a) += F(b, c, d)"), m["FF"]
    assert m["S11"] == (None, "7")


def test_statement_form_flagged():
    rows = A.macro_rows(OURS, THEIRS)
    ff = [r for r in rows if r.startswith("FF:")]
    assert ff and "pokebw2's is a statement, the file's an expression" in ff[0], rows
    assert not [r for r in rows if r.startswith("ROTL:")]
    assert any("lack: NEW" in r for r in rows) and any("no longer has: OLD" in r for r in rows), rows
    assert not [r for r in A.macro_rows(OURS, THEIRS, {"NEW"}, {"OLD"}) if "NEW" in r or "OLD" in r]


def test_same_macro_renames():
    a = (["x"], "(gTable[(x)] + Black_Get(x))")
    assert A.same_macro(a, (["y"], "(sTable[(y)] + B2_Get(y))"))  # free names renamed, the parameter by position
    assert not A.same_macro(a, (["y"], "(sTable[(y)] + sTable(y))"))  # two names of ours for one of pokebw2's
    assert not A.same_macro((["a", "b"], "((a) - (b))"), (["a", "b"], "((b) - (a))"))
    assert not A.same_macro((["a"], "((a) + 1)"), (["a"], "((a) + 2)"))
    assert not A.same_macro((["a"], "(int)(a)"), (["a"], "(u8)(a)"))  # a type is no free name


def entry(name, addr, own=None, anonymous=False, there=None):
    return (name, "ov009", addr, own, anonymous, there)


def test_compare_sets_md5():
    # the bad version: a pool word names the padding table where the original's names the word index
    ours = {0x21a1de8: entry("data_ov009_021a1de8", 0x21a1de8, own=17), 0x21a1e28: entry("data_ov009_021a1e28", 0x21a1e28)}
    theirs = {0x21a1da8: entry("data_ov009_021a1da8", 0x21a1da8), 0x21a1e28: entry("data_ov009_021a1e28", 0x21a1e28)}
    mine, placed, orig, shifted, same = A.compare((ours, {0x21a1de8: 64}, set()), (theirs, set()))
    assert (mine, placed, orig, shifted, same) == (["data_ov009_021a1de8"], [], ["data_ov009_021a1da8"], [], [])
    # the same bytes at the original's: one table under two names, no row of its own
    mine, _, orig, _, same = A.compare((ours, {}, set()), (theirs, set()), lambda a, b: True)
    assert not mine and not orig and same == [("data_ov009_021a1de8", "data_ov009_021a1da8")]
    # an address the original's pool holds as a plain word is no difference
    mine, _, orig, _, _ = A.compare((ours, {}, set()), (theirs, {0x21a1de8}))
    assert not mine and orig == ["data_ov009_021a1da8"]


def test_compare_slots_and_offsets():
    ours = {0x100: entry("sA", 0x100, own=1, there=0x200), 0x300: entry("sB", 0x304, own=2)}
    theirs = {0x200: entry("data_200", 0x200), 0x300: entry("data_300", 0x304), 0x400: entry("data_400", 0x400)}
    # word by word: our sA lands at 0x200 (misnamed), slot 8 reads sB where the original reads data_400
    mine, placed, orig, shifted, same = A.compare((ours, {}, set(), {0: 0x100, 4: 0x300, 8: 0x300}),
                                                  (theirs, set(), {0: 0x200, 4: 0x300, 8: 0x400}))
    assert same == [("sA", "data_200")] and mine == ["sB"] and orig == ["data_400"], (mine, orig, same)
    # another offset of one of our tables (the original's target lies inside it)
    _, _, orig, shifted, _ = A.compare(({0x300: entry("sB", 0x300, own=2)}, {0x300: 0x20}, set()),
                                       ({0x310: entry("data_310", 0x310)}, set()))
    assert shifted == [("sB", "data_310")] and not orig
    # a target no name tells (pokebw2's name of a static) is said apart and doesn't count as another table
    mine, placed, orig, _, _ = A.compare(({0x500: entry("gControl", 0x500, own=3, anonymous=True)}, {}, set()),
                                         ({0x600: entry("data_600", 0x600)}, set()))
    assert not mine and placed == ["gControl"] and orig == ["data_600"]


def test_open_rows_and_header():
    with tempfile.NamedTemporaryFile("w", suffix=".md", delete=False) as f:
        f.write("| `func_ov009_02162ff0` | `0x02162ff0` | `lib/nitrowifi/src/ssl/md5.c` | 3 lines | x |\n")
    try:
        assert A.open_rows(f.name) == {("lib/nitrowifi/src/ssl/md5.c", "func_ov009_02162ff0")}
    finally:
        os.unlink(f.name)
    head = ("// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/ssl/md5.c, "
            "ported by tools/port_file.py.\n")
    assert A.HEADER.match(head)[1] == "lib/nitrowifi/src/ssl/md5.c"


def test_alias_label_is_the_function():
    # the C's pool names the label _u32_div_f, the original's __aeabi_uidivmod at the same address (union_subdisp.c)
    root = tempfile.mkdtemp(prefix="port_audit_")
    os.makedirs(os.path.join(root, "arm9"))
    with open(os.path.join(root, "arm9", "symbols.txt"), "w") as f:
        f.write("__aeabi_uidivmod kind:function(arm,size=0x1e4) addr:0x0209c2b0\n"
                "_u32_div_f kind:label(arm) addr:0x0209c2b0\n"
                "data_0209c4a0 kind:data(any) addr:0x0209c4a0\n")
    names = A.Names(root)
    assert names.locate("_u32_div_f", "main") == names.locate("__aeabi_uidivmod", "main") == ("main", 0x0209c2b0)
    assert names.datum("main", 0x0209c2b0) == ("__aeabi_uidivmod", 0)  # a label starts no datum
    assert names.datum("main", 0x0209c4a4) == ("data_0209c4a0", 4)
    assert names.locate("nothing", "main") is None


def test_header_macros_outside_comments():
    root = tempfile.mkdtemp(prefix="port_audit_")
    with open(os.path.join(root, "a.h"), "w") as f:
        f.write("#define REAL(x) (x)\n/*\n#define GONE(x) (x)\n*/\n// #define ALSO(x) (x)\n#define URL \"http://x\"\n")
    assert A.header_macros([root]) == {"REAL", "URL"}, A.header_macros([root])


def main():
    failed = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_") and callable(fn):
            try:
                fn()
                print(f"ok {name}")
            except Exception as e:  # noqa: BLE001
                failed += 1
                print(f"FAIL {name}: {type(e).__name__}: {e}")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
