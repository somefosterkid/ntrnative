#!/usr/bin/env python3
"""Tests for tools/plat_lint.py's parsing and classification (canned clang output, scratch files; no clang run, no ROM):

    python3 tools/plat_lint_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import plat_lint as L  # noqa: E402

CLANG = """\
/w/src/a.c:10:5: warning: cast from 'u8 *' (aka 'unsigned char *') to 'Big *' increases required alignment from 1 to 8 [-Wcast-align]
/w/src/a.c:11:5: warning: cast from 'u32 (*)[52]' to 'Cell *' increases required alignment from 4 to 8 [-Wcast-align]
/w/src/a.c:12:5: warning: cast from 'u8 *' (aka 'unsigned char *') to 'Small *' increases required alignment from 1 to 4 [-Wcast-align]
/w/src/a.c:13:5: warning: cast from 'u8 *' (aka 'unsigned char *') to 'SysTask **' increases required alignment from 1 to 8 [-Wcast-align]
/w/src/a.c:20:9: warning: incompatible pointer to integer conversion passing 'void *' to parameter of type 'int' [-Wint-conversion]
/usr/lib/clang/18/include/__stddef_null.h:26:14: note: expanded from macro 'NULL'
/w/src/a.c:21:9: warning: incompatible pointer to integer conversion passing 'void *' to parameter of type 'int' [-Wint-conversion]
/w/src/a.c:30:9: warning: cast to smaller integer type 'u32' (aka 'unsigned int') from 'u32 *' (aka 'unsigned int *') [-Wpointer-to-int-cast]
/w/src/a.c:31:9: warning: cast to 'u8 *' (aka 'unsigned char *') from smaller integer type 'u32' (aka 'unsigned int') [-Wint-to-pointer-cast]
/w/src/a.c:32:9: warning: cast to 'void *' from smaller integer type 'u16' (aka 'unsigned short') [-Wint-to-void-pointer-cast]
/w/src/a.c:40:9: warning: unused variable 'x' [-Wunused-variable]
"""


def checks():
    return [(d['line'], L.classify(d)) for d in L.parse_diags(CLANG)]


def test_diagnostics_become_checks():
    got = dict(checks())
    assert got[10] == 'C1' and got[11] == 'C1', got        # u8 blob / u32 array to a struct of alignment 8
    assert 12 not in got or got[12] is None, got           # alignment 4 is the same on the DS and the host
    assert got[13] == 'C3', got                            # a pointer array: `T **`
    assert got[21] == 'C4', got                            # a real pointer passed as an int
    assert got[30] == got[31] == got[32] == 'C4', got      # pointer <-> narrower integer casts
    assert got[40] is None, got


def test_sizeof_of_a_pointer_is_c5():
    out = L.parse_diags("/w/src/b.c:7:30: warning: 'sizeof (outFlags)' will return the size of the pointer, not the array itself "
                        "[-Wsizeof-pointer-div]\n"
                        "/w/src/b.c:8:30: warning: sizeof on array function parameter will return size of 'u8 *' instead of 'u8[4]' "
                        "[-Wsizeof-array-argument]\n")
    assert [L.classify(d) for d in out] == ['C5', 'C5'], out


def test_null_as_zero_is_not_a_hit():
    got = dict(checks())
    assert got[20] is None, got                            # the note says the expression is the NULL macro


def test_array_that_covers_the_host_struct_is_silent():
    d = L.parse_diags(CLANG)[1]
    assert L.array_fits(d, {'Cell': 200}) is True          # 52 * 4 = 208 >= 200
    assert L.array_fits(d, {'Cell': 216}) is False         # the host struct is bigger than the DS-sized array: real
    assert L.array_fits(d, {}) is None                     # size unknown: reported
    assert L.array_fits(L.parse_diags(CLANG)[0], {'Big': 8}) is None   # a u8 * is no array


def test_text_scans_find_the_hgss_patterns():
    assert L.text_hits('    p = *(Foo **)((u8 *)x + 0x14);')       # offset 4 mod 8
    assert not L.text_hits('    p = *(Foo **)((u8 *)x + 0x10);')   # 8-aligned slot: fine
    assert L.text_hits('    p = *(Foo **)(base + i * 4);')         # 4-byte stride
    assert not L.text_hits('    p = table[i];')


def test_size_expressions():
    assert L.eval_size(['0x1C']) == 28
    assert L.eval_size(['0x20', '*', '4']) == 128
    assert L.eval_size(['32']) == 32
    assert L.eval_size(['sizeof', '(', 'Foo', ')']) is None   # a sizeof is the host's own: not a literal
    assert L.eval_size(['SIZE_MACRO']) is None


def test_allow_list_keys_ignore_line_numbers_and_spacing():
    with tempfile.TemporaryDirectory() as tmp:
        f = os.path.join(tmp, 'allow.txt')
        open(f, 'w').write('# why\nC1\tsrc/a.c\tx  =  (Big *)buf;\tbuffer is heap-sized for the host struct\nbad line\n')
        rows = L.load_allow(f)
        assert rows == {('C1', 'src/a.c', 'x = (Big *)buf;'): 'buffer is heap-sized for the host struct'}, rows


if __name__ == '__main__':
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith('test_'):
            try:
                fn()
                print('ok', name)
            except Exception as e:  # noqa: BLE001
                bad = 1
                print('FAIL', name, repr(e))
    sys.exit(bad)
