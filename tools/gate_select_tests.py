#!/usr/bin/env python3
"""Tests for gate_select.py: the change tiers of the merge gate."""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
os.chdir(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.environ['BWPORT_GATE_APK'] = '1'   # the tier tests below include the APK check (off by default since 11-15)
from gate_select import select


def ran(files, full=False):
    why, tests = select(files, full)
    return {c for c, w in why.items() if w}, tests


def t_apk_off_by_default():
    os.environ.pop('BWPORT_GATE_APK')
    try:
        r, _ = ran(['port/src/x.c'])
        assert 'apk' not in r, r
    finally:
        os.environ['BWPORT_GATE_APK'] = '1'


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


def t_plat_only():
    r, _ = ran(['port/game/route_gen4.c', 'port/game/plat_online.c', 'port/subprojects/pokeplatinum-route.patch',
                'port/include_plat/port_plat.h', 'port/include_gen4/nnsys_gen4.h'])
    assert r == set(), r
    r, _ = ran(['port/game/route_gen4.c', 'port/src/audio.c'])   # one shared file and the whole port tier is back
    assert r == {'port', 'cores', 'switch', 'sweep', 'touch', 'apk'}, r
    r, _ = ran(['port/game/game_table.c'])   # Gen 5's table too: not Platinum-only
    assert 'port' in r, r


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
