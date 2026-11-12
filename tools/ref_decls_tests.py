#!/usr/bin/env python3
"""Tests for tools/ref_values.py and the constants tools/ref_decls.py writes:

    python3 tools/ref_decls_tests.py        # prints "ok" per test, exits 1 on a failure

The evaluator tests are synthetic; the others read the references' checkouts (~/ref) and are skipped without them.
"""
import os
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ref_decls as rd  # noqa: E402
import ref_values as rv  # noqa: E402

SOURCES = ["ntrtwl-NitroSDK", "ntrtwl-NitroSystem", "ntrtwl-NitroWiFi", "ntrtwl-NitroDWC", "libntr"]
HAVE_REF = os.path.isdir(os.path.join(rd.REF, "ntrtwl-NitroSystem", "include"))


def ev(text, names=None):
    def resolve(n):
        if names and n in names:
            return names[n]
        raise rv.Unresolved(n)
    return rv.Evaluator(text, resolve).value()


def test_hex_literals_are_not_names():
    # the old code replaced "x0000ffff" of 0x0000ffff as a name and read 00
    assert ev("0x0000ffff") == 0xffff
    assert ev("0x80000000") == 0x80000000
    assert ev("0xc0000000u") == 0xc0000000
    assert ev("0x10") == 16 and ev("010") == 8 and ev("12UL") == 12


def test_operators_and_casts():
    assert ev("(1 << 4) | 3") == 0x13
    assert ev("~0xff & 0xffff") == 0xff00
    assert ev("(u32)-1") == 0xffffffff
    assert ev("(s8)0xff") == -1
    assert ev("(unsigned short)0x12345") == 0x2345
    assert ev("-5 / 2") == -2 and ev("-5 % 3") == -2
    assert ev("1 ? 2 : 3") == 2 and ev("0 || 0 ? 2 : 3") == 3
    assert ev("'a' + '" + chr(92) + "n'") == 97 + 10
    assert ev("(A + 1) * B", {"A": 2, "B": 5}) == 15


def test_unresolved_names_and_forms_raise():
    for text in ("UNKNOWN", "UNKNOWN | 1", "sizeof(int)", "F(1)", "(1 +", "1 2", "1 / 0", "$"):
        try:
            ev(text)
        except rv.Unresolved:
            continue
        raise AssertionError(f"{text!r} was given a value")


def test_enum_values_follow_earlier_enumerators_and_say_what_is_unresolved():
    told = []
    got = rv.enum_values("enum { A, B = 4, C, D = B | 8, E = NOPE, F, G = 2, H };", lambda n: (_ for _ in ()).throw(rv.Unresolved(n)),
                         lambda n, w: told.append(n))
    assert got == {"A": 0, "B": 4, "C": 5, "D": 12, "G": 2, "H": 3}, got
    assert told == ["E", "F"], told  # F follows E: not 0 or 1


def test_macros_and_black_values():
    with tempfile.TemporaryDirectory() as t:
        h = os.path.join(t, "x.h")
        open(h, "w").write("#define MASK (0xf0 | 1)" + chr(10) + "enum { ONE = 1, TWO } ;" + chr(10) +
                           "#define F(x) (x)" + chr(10))
        black = rv.Black(files=[h])
        assert black("MASK") == 0xf1 and black("TWO") == 2
        for n in ("F", "NONE"):
            try:
                black(n)
            except rv.Unresolved:
                continue
            raise AssertionError(n)


def test_partial_enum_gives_the_references_values():
    if not HAVE_REF:
        print("skipped (no ~/ref)", end=" ")
        return
    d = rd.lookup("NNSG3dTexSRTElem", SOURCES)
    black_has = {"NNS_G3D_TEXSRTANM_ELEM_FX16", "NNS_G3D_TEXSRTANM_ELEM_CONST", "NNS_G3D_TEXSRTANM_ELEM_STEP_2",
                 "NNS_G3D_TEXSRTANM_ELEM_STEP_MASK", "NNSG3dTexSRTElem"}
    rd._black["black"] = rv.Black(files=[])  # Black's headers know none of them
    new = rd.partial_enum(d, lambda n: n in black_has, SOURCES)
    text = rd.declaration(new)
    assert "NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_MASK = 0x0000ffff" in text or \
        "NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_MASK = 0xffff" in text, text
    assert "NNS_G3D_TEXSRTANM_ELEM_STEP_4 = 0x80000000" in text, text
    assert "NNS_G3D_TEXSRTANM_ELEM_STEP_1 = 0x0" in text, text
    assert "NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_SHIFT = 0" in text, text


def test_partial_enum_leaves_out_what_it_cannot_value():
    rd._black["black"] = rv.Black(files=[])
    d = rd.Def("type", "enum { A = 1, B = NOT_ANYWHERE_X, C, D = A + 4 };", ["A", "B", "C", "D"], "libntr", "x.h")
    d.members = ["A", "B", "C", "D"]
    del rd.UNRESOLVED[:]
    new = rd.partial_enum(d, lambda n: False, SOURCES)
    assert "B =" not in new.text and "C =" not in new.text, new.text
    assert "A = 1" in new.text and "D = 5" in new.text, new.text
    assert [n for n, _, _ in rd.UNRESOLVED] == ["B", "C"], rd.UNRESOLVED


def test_check_file_finds_a_wrong_value():
    if not HAVE_REF:
        print("skipped (no ~/ref)", end=" ")
        return
    with tempfile.TemporaryDirectory() as t:
        f = os.path.join(t, "a.c")
        open(f, "w").write("// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they" +
                           chr(10) + "// belong in the library's header" + chr(10) +
                           "enum {" + chr(10) + "    NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_MASK = 0x0," + chr(10) +
                           "    NNS_G3D_TEXSRTANM_ELEM_STEP_4 = 0x80000000," + chr(10) + "};" + chr(10) + chr(10) + "int x;" + chr(10))
        values = rv.Values(SOURCES, rv.Black(files=[]))
        problems = rd.check_file(f, SOURCES, values, rv)
        assert len(problems) == 1 and "LAST_INTERP_MASK is 0x0" in problems[0] and "0xffff" in problems[0], problems


def test_the_trees_blocks_match_the_references():
    if not HAVE_REF:
        print("skipped (no ~/ref)", end=" ")
        return
    problems = rd.check_tree(SOURCES)
    assert not problems, problems


if __name__ == "__main__":
    failed = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_") and callable(fn):
            try:
                fn()
                print("ok", name)
            except Exception as e:  # noqa: BLE001
                failed += 1
                print("FAIL", name, repr(e))
    sys.exit(1 if failed else 0)
