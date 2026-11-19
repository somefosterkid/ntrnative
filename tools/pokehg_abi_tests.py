#!/usr/bin/env python3
"""Tests for tools/pokehg_abi.py (the narrow declarations declared as defined) and tools/hg_abi_scan.py's port view and
--gate (scratch trees stand in for build/pokeheartgold; no clang):

    python3 tools/pokehg_abi_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hg_abi_scan as S  # noqa: E402
import pokehg_abi as A  # noqa: E402

DEF = """#include "sound.h"
static u8 helper(u8 x);
u8 GetLevel(void) {
    return 3;
}
void FadeOut(int volume, int frames) {
}
void SetLayer(enum BgLayer layer, s32 n) {
}
static u8 helper(u8 x) {
    return x;
}
"""
HDR = """#ifndef SOUND_H
#define SOUND_H
void FadeOut(u16 seqno, u16 length); // the fade
#endif
"""
USE = """#include "sound.h"
extern int GetLevel(void);
void SetLayer(u8 layer,
              u8 n);
static u8 helper(u16 x);
"""
FAILS = []


def test(f):
    try:
        f()
        print('ok', f.__name__)
    except AssertionError as e:
        FAILS.append(f.__name__)
        print('FAIL', f.__name__, e)
    return f


def tree():
    d = tempfile.mkdtemp(prefix='pokehg_abi_tests')
    for p, t in (('src/sound.c', DEF), ('include/sound.h', HDR), ('src/use.c', USE)):
        os.makedirs(os.path.join(d, os.path.dirname(p)), exist_ok=True)
        open(os.path.join(d, p), 'w').write(t)
    return d


@test
def ptype_names():
    # rev2: a name starting with u is a name (ov00_021E756C's `u8 unused0`), a u-type word is a type
    for p, t in (('u8 unused0', 'u8'), ('u32 param_1', 'u32'), ('const u8', 'u8'), ('u16', 'u16'), ('unsigned int', 'unsigned int'),
                 ('unsigned int used', 'unsigned int'), ('volatile vu16', 'vu16'), ('const fx32', 'fx32'), ('struct Foo', 'struct Foo'),
                 ('enum BgLayer layer', 'enum BgLayer'), ('GXRgb color', 'GXRgb'), ('s32 unk_4', 's32'), ('u8 *buf', 'ptr')):
        assert S.ptype(p) == t, (p, S.ptype(p))
    assert [S.kind(S.ptype(p)) for p in ('fx16 x', 'GXRgb c', 'vs8 a', 'vs16 b', 'u8 unused0', 'const s32')] == \
        ['narrow'] * 5 + ['wide']


@test
def scan_rows():
    rows = {(r[0], r[2]): S.what(r[1]) for r in S.scan(tree())}
    assert rows == {('FadeOut', 'include/sound.h'): 'param0 u16->int,param1 u16->int',
                    ('GetLevel', 'src/use.c'): 'return u8->int',
                    ('SetLayer', 'src/use.c'): 'param0 u8->enum BgLayer,param1 u8->s32'}, rows   # helper: static, other file


@test
def port_view_skips_ds_branch():
    t = '#if defined(SDK_PORT) || defined(NONMATCHING) // x\nvoid A(int a);\n#else\nvoid A(u8 a);\n#endif\n' \
        '#ifndef SDK_PORT\nvoid B(u8 b);\n#else\nvoid B(int b);\n#endif\n#ifdef OTHER\nvoid C(u8 c);\n#else\nvoid D(u8 d);\n#endif\n'
    v = S.port_view(t)
    assert v.count('\n') == t.count('\n')
    assert 'A(int' in v and 'A(u8' not in v and 'B(int' in v and 'B(u8' not in v and 'C(u8' in v and 'D(u8' in v, v


@test
def write_then_clean():
    d = tree()
    done, bad = A.write(d, {})
    assert len(done) == 3 and not bad, (done, bad)
    assert S.scan(d) == [], S.scan(d)
    h = open(os.path.join(d, 'include/sound.h')).read()
    assert ('#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/sound.c (tools/pokehg_abi.py)\n'
            'void FadeOut(int seqno, int length); // the fade\n#else\nvoid FadeOut(u16 seqno, u16 length); // the fade\n'
            '#endif\n') in h, h
    u = open(os.path.join(d, 'src/use.c')).read()
    assert 'extern u8 GetLevel(void);' in u, u                      # the file uses u8: as defined
    assert 'void SetLayer(int layer,\n              int n);' in u, u   # an enum of another file: int; s32 unnamed: int
    assert A.write(d, {}) == ([], [])                                 # a second run: nothing left


@test
def spelled():
    assert A.spelled('u8', 'u8 x;', False) == 'u8'
    assert A.spelled('u8', 'int x;', False) == 'unsigned char'
    assert A.spelled('enum BgLayer', '', True) == 'enum BgLayer'
    assert A.spelled('enum BgLayer', 'enum BgLayer x;', False) == 'int'
    assert A.spelled('HeapID', '', False) == 'int'
    assert A.spelled('unsigned int', '', False) == 'unsigned int'


@test
def code_after_declaration_not_rewritten():
    t = 'void FadeOut(u16 a, u16 b); int x;\n'
    assert A.rewrite(t, 'FadeOut', 1, [(0, 'u16', 'int')], 'src/sound.c', False) is None
    assert A.rewrite('void FadeOut(u16 a);\n', 'FadeOut', 2, [(0, 'u16', 'int')], 'f', False) is None   # not at that line


@test
def gate_and_exceptions():
    d = tree()
    exc = os.path.join(d, 'exc.txt')
    open(exc, 'w').write('# why\nFadeOut\tthe definition is the bug\nGetLevel\tx\nSetLayer\ty\n')
    assert set(S.exceptions(exc)) == {'FadeOut', 'GetLevel', 'SetLayer'}
    old = S.EXCEPTIONS
    try:
        S.EXCEPTIONS = os.path.join(d, 'none.txt')
        devnull = open(os.devnull, 'w')
        so, se, sys.stdout, sys.stderr = sys.stdout, sys.stderr, devnull, devnull
        try:
            r1 = S.main(['--gate', d])
            S.EXCEPTIONS = exc
            r2 = S.main(['--gate', d])
        finally:
            sys.stdout, sys.stderr = so, se
        assert (r1, r2) == (1, 0), (r1, r2)
        d2 = tree()
        A.write(d2, {'FadeOut': 'x'})
        assert [r[0] for r in S.scan(d2)] == ['FadeOut']               # left as it is
    finally:
        S.EXCEPTIONS = old


if __name__ == '__main__':
    sys.exit(1 if FAILS else 0)
