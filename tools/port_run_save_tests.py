#!/usr/bin/env python3
"""Tests for port_run.py's save placement (the 2026-11-07 trap: a game folder's save.bin hides the run dir's):
python3 tools/port_run_save_tests.py   (prints "ok" per test, exits 1 on a failure)"""
import os
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import port_run as R  # noqa: E402


def test_placement():
    with tempfile.TemporaryDirectory() as d:
        assert R.save_path(d, 'black') == os.path.join(d, 'save.bin')          # flat: the launcher migrates it
        os.mkdir(os.path.join(d, 'Pokemon Black'))
        assert R.save_path(d, 'black') == os.path.join(d, 'Pokemon Black', 'save.bin')
        assert R.save_path(d, 'white') == os.path.join(d, 'save.bin')          # White's folder is not there yet
        assert R.save_paths(d, 'b2')[0].endswith('Pokemon Black 2/save.bin')


def test_game_of():
    assert [R.game_of(n) for n in ('room', 'white-room', 'b2-pc', 'w2-room', '/x/b2-a.bin')] == ['black', 'white', 'b2', 'w2', 'b2']


def test_check_save():
    with tempfile.TemporaryDirectory() as d:
        os.makedirs(os.path.join(d, 'Pokemon Black', 'backups'))
        mine, other = os.path.join(d, 'mine.bin'), os.path.join(d, 'Pokemon Black', 'backups', 'save-1.bin')
        open(mine, 'wb').write(b'a')
        open(other, 'wb').write(b'b')
        want = R.sha1(mine)
        folder = os.path.join(d, 'Pokemon Black')
        ok = f'bwport: launcher: Pokemon Black, in {folder}\n'
        assert R.check_save(ok, d, 'black', want) is None
        assert 'different save' in R.check_save(ok + f'bwport: launcher: backup {other}\n', d, 'black', want)
        assert 'ran in' in R.check_save(f'bwport: launcher: Pokemon Black, in {d}/x\n', d, 'black', want)
        assert 'never said' in R.check_save('nothing', d, 'black', want)


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
