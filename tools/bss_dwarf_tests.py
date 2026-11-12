#!/usr/bin/env python3
"""Tests for tools/bss_dwarf.py:

    python3 tools/bss_dwarf_tests.py        # prints "ok" per test, exits 1 on a failure

Synthetic statics and C text; nothing in the tree is read or changed.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import bss_dwarf as bd  # noqa: E402

P = bd.Piece
PIECES = [P(0x100, 4, "cb", "void (*)(void)", kind="fn"), P(0x104, 4, "alloc", "void *(*)(u32, s32)", kind="fn"),
          P(0x108, 4, "opened", "s32"), P(0x10c, 9, "gamecd", "s8", "[9]"), P(0x118, 12, "dlattr", "s8 *", "[3]"),
          P(0x124, 24, "mtx", "OSMutex", kind="struct")]


def rw(src, declared=("u8", "")):
    out, why = bd.rewrite(src, "X", 0x100, PIECES, declared)
    assert why is None, why
    return out


def test_cast_index_and_deref_forms_name_the_static():
    out = rw("a = ((s32 *)&X)[2]; *(s32 *)(&X + 8) = 3; if (*(s32 *)((u8 *)((void *)&X) + 8) == 0) {}")
    assert out.count("opened") == 3 and "X" not in out.replace("u8", ""), out


def test_function_pointer_elements_are_cast_not_indexed():
    out = rw("r = (*((u8 * (**)())&X)[1])(len, 4);")
    assert "alloc" in out and "[1]" not in out, out


def test_address_of_a_static_keeps_its_cast():
    out = rw("OS_LockMutex((OSMutex *)&X);", ("u8", ""))
    assert "(OSMutex *)&cb" in out, out


def test_a_wrong_sized_access_is_cast_through_the_address():
    out = rw("X = v;")
    assert "(*(u8 *)&cb) = v;" in out, out


def test_a_use_that_hits_no_static_is_refused():
    out, why = bd.rewrite("y = ((s32 *)&X)[200];", "X", 0x100, PIECES, ("u8", ""))
    assert out is None and "no static" in why, why


def test_a_table_is_renamed_with_a_cast_when_its_element_differs():
    out, why = bd.rewrite("s32 *a = T; T[i] = 1;", "T", 0x118, PIECES, ("s32", "[8]"))
    assert why is None and "((s32 *)dlattr)[i] = 1" in out, out


def test_declarations_are_left_alone():
    src = "extern u8 X;\nint f(void) { return ((u32 *)&X)[2]; }\n"
    out, why = bd.rewrite(src, "X", 0x100, PIECES, ("u8", ""), skip=[(0, 13)])
    assert why is None and out.startswith("extern u8 X;") and "opened" in out, out


def test_fit_puts_sorted_objects_on_blacks_symbols_and_skips_one_it_lacks():
    objs = [bd.Obj("a", 4, 4, "u32 %s", "int", "", False, ""), bd.Obj("b", 4, 4, "u32 %s", "int", "", False, ""),
            bd.Obj("c", 4, 4, "u32 %s", "int", "", False, ""), bd.Obj("s", 9, 1, "s8 %s[9]", "array", "", False, ""),
            bd.Obj("t", 12, 4, "s8 *%s[3]", "array", "", False, "")]
    placed, end = bd.fit([0x100, 0x104, 0x108, 0x114], 0x100, 0x130, objs)
    assert [o.name for o, a, sz in placed] == ["a", "b", "s", "t"] and end == 0x120, (placed, end)


def main():
    failed = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            try:
                fn()
                print("ok", name)
            except AssertionError as e:
                failed += 1
                print("FAIL", name, e)
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
