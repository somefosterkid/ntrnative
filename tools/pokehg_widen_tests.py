#!/usr/bin/env python3
"""Tests for tools/pokehg_widen.py's rewrites (scratch files stand in for build/pokeheartgold; no clang):

    python3 tools/pokehg_widen_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pokehg_widen as W  # noqa: E402

C = """#include "global.h"
void ov01_021F1648(s32 param_1, int param_2);
static u32 helper(u32 x);

void ov01_021F1648(s32 param_1, int param_2) {
    s32 iVar1;
    u32 count = 0;
    iVar1 = *(s32 *)(param_1 + 0x20);
    ov01_021F1648(param_1, helper(4));
    return helper((u32)&iVar1);
}
"""
H = "void ov01_021F1648(s32, int);\n"
TSV = ("kind\tfile\tfunction\tline\twhat\texpr_or_root\n"
       "BASE-IN-INT\tsrc/a.c\tov01_021F1648\t8\tparam param_1 is s32, local iVar1, local iVar1 is s32\t*(x)\n"
       "BASE-IN-INT\tsrc/a.c\tov01_021F1648\t8\tparam param_2 is u8\t*(y)\n"
       "BASE-IN-INT\tsrc/a.c\tov01_021F1648\t8\troot is s32\t*(z)\n"
       "NUM-HOLDS-PTR\tsrc/a.c\tov01_021F1648\tNone\tlong loaded pointer used as base\tP:x\n")


def test_declarations_skip_calls():
    got = [C[o:c + 1] for o, c in W.declarations(C, 'ov01_021F1648')]
    assert got == ['(s32 param_1, int param_2)'] * 2, got
    assert len(W.declarations('x = helper(1); return helper(2);', 'helper')) == 0


def test_widen_param_every_prototype_by_position():
    t, n = W.widen_param(C, 'ov01_021F1648', 0)
    assert n == 2 and t.count('intptr_t param_1, int param_2') == 2, t
    t, n = W.widen_param(H, 'ov01_021F1648', 0)
    assert (t, n) == ('void ov01_021F1648(intptr_t, int);\n', 1)
    # a pointer parameter is left as it is
    assert W.widen_param('void f(u32 *p);', 'f', 0) == ('void f(u32 *p);', 0)


def test_widen_local_in_its_function_only():
    t, n = W.widen_local(C, 'ov01_021F1648', 'iVar1')
    assert n == 1 and '    intptr_t iVar1;\n' in t and 'u32 count' in t, t
    t, n = W.widen_local(C, 'ov01_021F1648', 'count')
    assert n == 1 and 'uintptr_t count = 0;' in t


def test_tsv_rules_take_params_and_locals_only():
    import csv, io
    params, locs, skip = W.tsv_rules(list(csv.DictReader(io.StringIO(TSV), delimiter='\t')))
    assert params == {('src/a.c', 'ov01_021F1648', 'param_1')}, params
    assert locs == {('src/a.c', 'ov01_021F1648', 'iVar1')}, locs
    assert skip['param X is u8'] == 1 and skip['root is s32'] == 1 and sum(skip.values()) == 3, skip


def test_apply_tsv_and_casts():
    with tempfile.TemporaryDirectory() as tmp:
        for f, t in [('src/a.c', C), ('include/a.h', H), ('port/UINTPTR.tsv', TSV)]:
            os.makedirs(os.path.dirname(os.path.join(tmp, f)), exist_ok=True)
            open(os.path.join(tmp, f), 'w').write(t)
        W.HG = tmp
        stats, skip = W.apply_tsv(tmp, os.path.join(tmp, 'port/UINTPTR.tsv'))
        assert stats['param: declarations widened'] == 3 and stats['local: widened'] == 1, stats
        assert open(os.path.join(tmp, 'include/a.h')).read() == 'void ov01_021F1648(intptr_t, int);\n'
        # a clang log: the cast at line 10, column 19 (its '(')
        os.makedirs(os.path.join(tmp, 'out/log/src'))
        col = C.split('\n')[9].index('(u32)') + 1
        open(os.path.join(tmp, 'out/log/src/a.c.log'), 'w').write(
            "src/a.c:10:%d: warning: cast to smaller integer type 'u32' (aka 'unsigned int') from 's32 *'\n" % col)
        st = W.apply_casts(tmp, os.path.join(tmp, 'out'))
        assert st['cast: widened'] == 1, st
        assert 'return helper((uintptr_t)&iVar1);' in open(os.path.join(tmp, 'src/a.c')).read()


def test_casts_widen_the_local_holding_them():
    c = ("u32 g;\nvoid *f(void *heap, u32 size) {\n    u32 pos = (u32)heap + size; // { '}'\n    u32 end;\n"
         "    end = (u32)heap;\n    g = (u32)heap;\n    return (void *)pos;\n}\nvoid h(void) { u32 pos; }\n")
    with tempfile.TemporaryDirectory() as tmp:
        os.makedirs(os.path.join(tmp, 'src'))
        os.makedirs(os.path.join(tmp, 'out/log/src'))
        open(os.path.join(tmp, 'src/b.c'), 'w').write(c)
        lines = c.split('\n')
        log = ''.join("src/b.c:%d:%d: warning: cast to smaller integer type 'u32' (aka 'unsigned int') from 'void *'\n"
                      % (ln, lines[ln - 1].index('(u32)') + 1) for ln in (3, 5, 6))
        open(os.path.join(tmp, 'out/log/src/b.c.log'), 'w').write(log)
        st = W.apply_casts(tmp, os.path.join(tmp, 'out'))
        assert st['cast: widened'] == 3 and st['local holding the cast: widened'] == 2, st
        t = open(os.path.join(tmp, 'src/b.c')).read()
        assert 'uintptr_t pos = (uintptr_t)heap + size;' in t and 'uintptr_t end;' in t, t
        assert t.startswith('u32 g;') and 'g = (uintptr_t)heap;' in t and 'void h(void) { u32 pos; }' in t, t


def test_int_to_pointer_casts_widen_what_they_read():
    c = ("void *g(s32 param_1, u32 n, u16 w, s32 *arr) {\n    s32 base;\n    T *a = (T *)(param_1 + 4);\n"
         "    T *b = (T *)base;\n    T *c = (T *)arr[3];\n    T *d = (T *)n;\n    T *e = (T *)w;\n    T *f = (T *)h(base);\n"
         "    return (T *)(base - 1);\n}\nvoid h2(void) { s32 base; }\n")
    h = "void *g(s32, u32, u16, s32 *);\n"
    with tempfile.TemporaryDirectory() as tmp:
        for f, t in [('src/d.c', c), ('include/d.h', h)]:
            os.makedirs(os.path.dirname(os.path.join(tmp, f)), exist_ok=True)
            open(os.path.join(tmp, f), 'w').write(t)
        os.makedirs(os.path.join(tmp, 'out/log/src'))
        W.HG = tmp
        lines = c.split('\n')
        log = ''.join("src/d.c:%d:%d: warning: cast to 'T *' (aka 'struct T *') from smaller integer type 's32' (aka 'int')\n"
                      % (ln, lines[ln - 1].index('(T *)') + 1) for ln in range(3, 10))
        open(os.path.join(tmp, 'out/log/src/d.c.log'), 'w').write(log)
        st = W.apply_casts(tmp, os.path.join(tmp, 'out'))
        t = open(os.path.join(tmp, 'src/d.c')).read()

        assert st['int-to-pointer cast: operand not a plain name (an element, a field, a call)'] == 2, st
        assert st['int-to-pointer cast: local widened'] == 1, st               # base (read twice), once
        assert st['int-to-pointer cast, params (unique)'] == 3, st             # param_1, n, w (u16: not widened)
        assert st['int-to-pointer cast, param: declarations widened'] == 4, st    # param_1 and n: definition + prototype
        assert 'void *g(intptr_t param_1, uintptr_t n, u16 w, s32 *arr)' in t and '    intptr_t base;' in t, t
        assert 'void h2(void) { s32 base; }' in t, t
        assert open(os.path.join(tmp, 'include/d.h')).read() == 'void *g(intptr_t, uintptr_t, u16, s32 *);\n'


def test_params_unsigned_char_and_short_are_not_ints():
    t = "void f(unsigned char a, unsigned short b, unsigned c, unsigned long d, signed char e);\n"
    for i in range(5):
        t, _ = W.widen_param(t, 'f', i)
    assert t == "void f(unsigned char a, unsigned short b, uintptr_t c, uintptr_t d, signed char e);\n", t


def test_cast_operand_and_enclosing_function():
    s = "  obj = (UnkDC7CObj *)(param_1 + 4);"
    assert W.cast_operand(s, s.index('(Unk') + 1) == 'param_1'
    assert W.cast_operand("x = (T *)p;", 5) == 'p' and W.cast_operand("x = (T *)p->q;", 5) is None
    assert W.cast_operand("x = (T *)p[2];", 5) is None and W.cast_operand("x = (T *)f(p);", 5) is None
    assert W.cast_operand("x = (T *)sizeof(p);", 5) is None
    t = "static int a;\nstatic void f(int x) {\n    if (x) {\n        y = 1;\n    }\n}\n"
    assert W.enclosing_function(t, t.index('y = 1')) == 'f' and W.enclosing_function(t, 3) is None


def test_casts_enum_and_typedef_not_u16():
    c = "void f(void *p) {\n    g((T *)((enum Move)(p)), (HeapID)p, (fx32)p, (u16)p);\n}\n"
    s = c.split('\n')[1]
    log = ''.join("src/c.c:2:%d: warning: cast to smaller integer type %s from 'void *' [-Wx]\n" % (s.index(a) + 1, t)
                  for a, t in [('(enum', "'enum Move'"), ('(HeapID', "'HeapID' (aka 'enum HeapID')"),
                               ('(fx32', "'fx32' (aka 'int')"), ('(u16', "'u16' (aka 'unsigned short')")])
    with tempfile.TemporaryDirectory() as tmp:
        os.makedirs(os.path.join(tmp, 'src'))
        os.makedirs(os.path.join(tmp, 'out/log/src'))
        open(os.path.join(tmp, 'src/c.c'), 'w').write(c)
        open(os.path.join(tmp, 'out/log/src/c.c.log'), 'w').write(log)
        st = W.apply_casts(tmp, os.path.join(tmp, 'out'))
        t = open(os.path.join(tmp, 'src/c.c')).read()
        assert 'g((T *)((intptr_t)(p)), (intptr_t)p, (intptr_t)p, (u16)p);' in t, t
        assert st['cast: widened'] == 3, st


if __name__ == '__main__':
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith('test_'):
            try:
                fn()
                print('ok', name)
            except Exception as e:  # noqa: BLE001
                bad += 1
                print('FAIL', name, repr(e))
    sys.exit(1 if bad else 0)
