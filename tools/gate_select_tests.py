#!/usr/bin/env python3
"""Tests for gate_select.py: the change tiers of the merge gate."""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
os.chdir(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from gate_select import select


def ran(files, full=False):
    why, tests = select(files, full)
    return {c for c, w in why.items() if w}, tests


def t_decomp():
    r, _ = ran(['src/field/x.c'])
    assert r == {'rom'}, r
    r, _ = ran(['include/x.h'])
    assert r == {'rom', 'port'}, r


def t_docs():
    r, _ = ran(['docs/a.md', 'CLAUDE.md'])
    assert r == set(), r


def t_tools():
    r, t = ran(['tools/gate_select.py'])
    assert r == set() and t == ['tools/gate_select_tests.py'], (r, t)
    r, _ = ran(['tools/touch_test.py'])
    assert r == {'touch', 'port'}, r
    r, _ = ran(['tools/lockstep/lstep_run.sh'])
    assert r == {'sweep', 'port'}, r


def t_port():
    r, _ = ran(['port/vcore/a.c'])
    assert r == {'port', 'cores', 'switch', 'sweep', 'touch'}, r
    r, _ = ran(['port/subprojects/packagefiles/libntr-x.patch'])
    assert 'apk' in r and 'cores' in r, r
    r, _ = ran(['tools/android/app/a.kt'])
    assert r == {'apk'}, r


def t_full():
    r, _ = ran([], full=True)
    assert len(r) == 7


if __name__ == '__main__':
    bad = 0
    for n, f in sorted(globals().items()):
        if n.startswith('t_'):
            try:
                f()
                print('ok', n)
            except AssertionError as e:
                bad = 1
                print('FAIL', n, repr(e))
    sys.exit(bad)
