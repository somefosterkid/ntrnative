#!/usr/bin/env python3
"""Tests for tools/pokehg_cc.py's file list, tools/pokehg_gen.py's header resolver and tools/pokegen4.py's summary
(scratch trees stand in for build/pokeheartgold; no clang, no network):

    python3 tools/pokehg_cc_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pokegen4 as G4  # noqa: E402
import pokehg_cc as C  # noqa: E402
import pokehg_gen as G  # noqa: E402


def write(tmp, files):
    for f, t in files.items():
        p = os.path.join(tmp, f)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, 'w').write(t)


def test_sources_are_the_lsf_c_and_the_nonmatching_functions():
    with tempfile.TemporaryDirectory() as tmp:
        write(tmp, {'src/a.c': '', 'src/stale.c': '', 'lib/spl/src/s.c': '', 'asm/x.s': '',
                    'port/nonmatching/ov00_021E733C.c': '',
                    'main.lsf.in': 'Static main\n{\n    Object src/a.c\n    Object asm/x.s\n    Object lib/spl/src/s.c\n}\n'})
        C.HG = tmp
        got = [os.path.relpath(f, tmp) for f in C.sources()]
        assert got == ['lib/spl/src/s.c', 'port/nonmatching/ov00_021E733C.c', 'src/a.c'], got


def test_meson_lines_are_the_own_args_and_the_nonmatching_files():
    with tempfile.TemporaryDirectory() as tmp:
        write(tmp, {'port/nonmatching/b.c': '', 'port/nonmatching/a.c': '', 'port/nonmatching/readme.txt': '', 'src/c.c': ''})
        C.HG = tmp
        assert [os.path.relpath(f, tmp) for f in C.nonmatching()] == ['port/nonmatching/a.c', 'port/nonmatching/b.c']
    a = C.own_args()
    assert '-DHEARTGOLD' in a and C.own_args('SOULSILVER')[a.index('-DHEARTGOLD')] == '-DSOULSILVER'
    assert a[-2:] == ['-include', 'port_hgss.h'], a[-2:]


def test_resolver_prefers_the_c_tree_then_the_build():
    with tempfile.TemporaryDirectory() as tmp:
        co, b = tmp, os.path.join(tmp, 'hbuild')
        write(tmp, {'include/x.h': '', 'src/local.h': '', 'hbuild/files/msgdata/msg/msg_0001.h': '',
                    'hbuild/files/data/resdat.naix': '', 'hbuild/files/arc/w.naix': ''})
        r = G.resolver(co, b)
        src = os.path.join(co, 'src/a.c')
        assert r('x.h', src) is None and r('local.h', src) is None
        assert r('msgdata/msg/msg_0001.h', src) == 'files/msgdata/msg/msg_0001.h'
        assert r('files/data/resdat.naix', src) == 'files/data/resdat.naix'
        assert r('w.naix', src) == 'files/arc/w.naix'
        assert r('nitro.h', src) is None


def test_summary_counts_errors_and_own_warnings():
    log = ("src/a.c:3:5: error: use of undeclared identifier 'NNS_X'\n"
           "/sdk/include/n.h:1:1: warning: cast to smaller integer type 'u32' from 'void *'\n"
           "src/a.c:4:9: warning: cast to smaller integer type 'u32' (aka 'unsigned int') from 'void *'\n")
    s = G4.summarize([('src/a.c', 1, log), ('src/b.c', 0, '')], '/w/pokeheartgold', 'pokeheartgold')
    assert s.startswith('files 2 ok 1 failed 1\n'), s
    assert 'SDK/NNS undeclared name' in s and s.count('[pointer size]') == 1, s
    assert s.rstrip().endswith('FAILED FILES\nsrc/a.c'), s


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
