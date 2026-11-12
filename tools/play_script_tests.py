#!/usr/bin/env python3
"""Tests for play_script.py:  python3 tools/play_script_tests.py   (prints "ok" per test, exits 1 on a failure)"""
import glob
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import play_script as P  # noqa: E402


def test_walk():
    # a direction change adds a turning tap; a key waits 3 steps after
    assert P.walk(100, 10, ['up2', 'right1', 'a']) == (
        ['up*f6@f100', 'up*f6@f110', 'up*f6@f120', 'right*f6@f130', 'right*f6@f140', 'a@f150'], 180)
    assert P.walk(0, 10, ['down:f20', 'w50'])[0] == ['down*f6@f0', 'down*f20@f10']
    assert P.walk(0, 10, ['a*2/5'])[0] == ['a@f0', 'a@f5']


def test_parse():
    m = P.parse('# game: white\n# from: boot\n# reaches: x\nkeys start@f600 a@f700\nwalk 800 10 up\n')
    assert m['game'] == 'white' and m['from'] == 'boot' and m['keys'][:2] == ['start@f600', 'a@f700']
    assert m["last"] == 810 and m["end_frame"] == 840
    assert P.parse('# end: f900\nkeys a@f10')['end_frame'] == 900


def test_bad():
    for text in ('keys a@12', 'bogus', '# game: nes', 'walk x y up', '# end: 5'):
        try:
            P.parse(text)
        except ValueError:
            continue
        raise AssertionError(text)


def test_library():
    """every script in tools/plays parses and has the header lines"""
    for path in glob.glob(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'plays', '*', '*.txt')):
        m = P.parse(open(path).read(), path)
        assert m['reaches'], f'{path}: no "# reaches:" line'
        assert os.path.basename(os.path.dirname(path)) == m['game'], f'{path}: in the wrong game folder'


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
