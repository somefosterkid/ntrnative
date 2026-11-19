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


def test_saves_dir():
    """--save/--store use $BWPORT_SAVES, else ~/saves (tools/route.py's rule on Linux)"""
    old = os.environ.pop('BWPORT_SAVES', None)
    try:
        assert R.saves_dir() == os.path.join(os.path.expanduser('~'), 'saves')
        os.environ['BWPORT_SAVES'] = '/tmp/x'
        assert R.saves_dir() == '/tmp/x'
        os.environ['BWPORT_SAVES'] = ''                                         # empty = unset
        assert R.saves_dir() == os.path.join(os.path.expanduser('~'), 'saves')
    finally:
        os.environ.pop('BWPORT_SAVES', None)
        if old is not None:
            os.environ['BWPORT_SAVES'] = old


def test_verdict_without_gdb():
    """--no-gdb: the exit status says what gdb's stop text says (the place is `?`: no backtrace)"""
    v = lambda timed_out=False, rc=None, log='', frame=9: R.verdict(log, timed_out, frame, 0, 5, 60, 3, rc)  # noqa: E731
    assert v(rc=-11) == 'crash SIGSEGV in ?, frame 9'
    assert v(rc=-6) == 'crash SIGABRT in ?, frame 9'
    assert v(rc=0) == 'exit 0 at frame 9' and v(rc=3) == 'exit 3 at frame 9'
    assert v(rc=-6, log='bwport: undefined function foo called\n') == 'stub foo called, frame 9'
    assert v(timed_out=True).startswith('reached-frame 9')                  # port_run killed it at the end
    assert v(timed_out=True, frame=0).startswith('hang in ? before the first frame')
    assert R.verdict('', False, 9, 0, 5, 60, 3) == 'stopped (?), frame 9'    # under gdb, with no stop text: as before


def test_verdict_frames():
    """--frames N: the exe's own end at V-blank N is the result; the wall cap is only the watchdog"""
    ended = 'x\nbwport: exit-frame 3000\n'
    assert R.verdict(ended, False, 2990, 0, 5, 60, 3, 0, 3000) == 'exit-frame 3000 in 3 s (60 fps)'
    gdb = ended + '[Inferior 1 (process 7) exited normally]\n\n== port_run: stop\n'
    assert R.verdict(gdb, False, 2990, 0, 5, 60, 3, None, 3000) == 'exit-frame 3000 in 3 s (60 fps)'
    assert R.verdict(ended, False, 9, 0, 5, 60, 3, 3, 3000) == 'exit 3 at frame 9'   # a failure status is no pass
    assert R.verdict('', True, 900, 0, 5, 60, 3, None, 3000) == 'watchdog: frame 900 of 3000 in 3 s (60 fps)'
    assert R.verdict('', True, 900, 0, 5, 60, 3).startswith('reached-frame 900')     # no --frames: the wall cap as before
    assert R.verdict('', True, 900, 9, 5, 60, 3, None, 3000).startswith('hang in ')   # a stuck run is still a hang


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
