#!/usr/bin/env python3
"""Tests for tools/port_wraps.py's stale_launcher (a build dir made before the ccache-nopch launcher existed):

    python3 tools/port_wraps_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import port_wraps as W  # noqa: E402

INI = ("# comment naming ccache-nopch\n[binaries]\n"
       "c = ['@GLOBAL_SOURCE_ROOT@/machine/ccache-nopch', 'clang']\n")


def case(ini, ninja, args=None):
    with tempfile.TemporaryDirectory() as d:
        Path(d, 'n.ini').write_text(ini)
        if ninja is not None:
            Path(d, 'build.ninja').write_text(ninja)
        return W.stale_launcher(d, args or ['--native-file', os.path.join(d, 'n.ini')])


def test_old_dir_is_stale():
    assert case(INI, 'rule c_COMPILER\n command = ccache clang $ARGS\n')


def test_new_dir_is_not():
    assert not case(INI, 'rule c_COMPILER\n command = /x/port/machine/ccache-nopch clang $ARGS\n')


def test_comment_only_mention_is_not():
    assert not case("# ccache-nopch\n[binaries]\nc = ['ccache', 'clang']\n", 'command = ccache clang\n')


def test_equals_form_and_no_ninja():
    with tempfile.TemporaryDirectory() as d:
        Path(d, 'n.ini').write_text(INI)
        assert not W.stale_launcher(d, ['--native-file=' + os.path.join(d, 'n.ini')])   # no build.ninja: meson setup instead
    assert not case(INI, 'ccache clang\n', ['-Ddev_fast=true'])


SYSROOT_INI = "[binaries]\nc = ['ccache', 'clang', '--config=/sr/clang.cfg']\n"
OLD_NINJA = 'rule c_COMPILER\n command = ccache clang $ARGS\n'


def multi(files, ninja):
    """files: [(name, text)] in --native-file order -> stale_launcher"""
    with tempfile.TemporaryDirectory() as d:
        args = []
        for n, t in files:
            Path(d, n).write_text(t)
            args += ['--native-file', os.path.join(d, n)]
        Path(d, 'build.ninja').write_text(ninja)
        return W.stale_launcher(d, args)


def test_sysroot_native_file_wins():   # pch1: BWPORT_SYSROOT + ccache: the sysroot's `c` (plain ccache clang) comes last, no dir ever has the launcher
    assert not multi([('linux-ccache.ini', INI), ('native-ccache.ini', SYSROOT_INI)], OLD_NINJA)
    assert multi([('native-ccache.ini', SYSROOT_INI), ('linux-ccache.ini', INI)], OLD_NINJA)   # the launcher file last: it is the one that counts
    assert multi([('linux.ini', '[built-in options]\nc_args = []\n'), ('linux-ccache.ini', INI)], OLD_NINJA)   # a file without `c` is skipped


def test_refresh_wipes_once():   # a dir that stays stale after the wipe (its recorded native files) is not wiped again, it is warned about
    calls, said = [], []
    real_sh = W.sh
    W.sh = lambda cmd, **kw: (calls.append(cmd), type('R', (), {'returncode': 0, 'stdout': ''})())[1]
    try:
        with tempfile.TemporaryDirectory() as d:
            Path(d, 'linux-ccache.ini').write_text(INI)
            Path(d, 'build').mkdir()
            Path(d, 'build', 'build.ninja').write_text(OLD_NINJA)
            args = ['--native-file', os.path.join(d, 'linux-ccache.ini')]
            assert W.refresh(str(Path(d, 'build')), args, said.append) is None
            wipes = [c for c in calls if '--wipe' in c]
            assert len(wipes) == 1 and os.path.join(d, 'linux-ccache.ini') in wipes[0]   # the new native file goes with the wipe
            assert os.path.exists(Path(d, 'build', W.STALE_MARK))
            calls.clear(); said.clear()
            assert W.refresh(str(Path(d, 'build')), args, said.append) is None
            assert not [c for c in calls if '--wipe' in c]
            assert any('still compiled without ccache-nopch' in m for m in said), said
    finally:
        W.sh = real_sh


def pch_case(ninja, sum_exists=False, listing='cc other\n/x/port/machine/ccache-nopch clang -o a.p/pch/a.h.pch -c a.h\n'):
    """-> the commands refresh()'s pch_sums ran, as (argv, CCACHE_NOPCH_SUMONLY)"""
    calls = []
    real_sh = W.sh

    def fake(cmd, **kw):
        calls.append((cmd, (kw.get('env') or {}).get('CCACHE_NOPCH_SUMONLY')))
        return type('R', (), {'returncode': 0, 'stdout': listing})()
    W.sh = fake
    try:
        with tempfile.TemporaryDirectory() as d:
            Path(d, 'build.ninja').write_text(ninja)
            Path(d, 'a.p/pch').mkdir(parents=True)
            Path(d, 'a.p/pch/a.h.pch').write_text('p')
            if sum_exists:
                Path(d, 'a.p/pch/a.h.pch.sum').write_text('s')
            said = []
            W.pch_sums(d, said.append)
            return calls, said
    finally:
        W.sh = real_sh


def test_pch_sums_runs_the_last_command_sumonly():   # ccx2: an existing dir gets its .sum without recompiling anything
    calls, said = pch_case('command = /x/port/machine/ccache-nopch clang\n')
    assert [c[0][:3] for c in calls] == [['ninja', '-C', calls[0][0][2]], ['sh', '-c', '/x/port/machine/ccache-nopch clang -o a.p/pch/a.h.pch -c a.h']], calls
    assert calls[0][0][-2:] == ['commands', os.path.join('a.p', 'pch', 'a.h.pch')] and calls[1][1] == '1' and said


def test_pch_sums_leaves_good_and_old_dirs_alone():
    assert pch_case('command = /x/port/machine/ccache-nopch clang\n', sum_exists=True)[0] == []   # has its sum
    assert pch_case('command = ccache clang\n')[0] == []                                          # not a launcher dir (stale_launcher's case)
    assert [c[0][0] for c in pch_case('command = ccache-nopch\n', listing='plain clang -c a.h\n')[0]] == ['ninja']   # last line is not the launcher: no run


if __name__ == '__main__':
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith('test_'):
            try:
                fn()
                print('ok', name)
            except AssertionError:
                print('FAIL', name)
                bad = 1
    sys.exit(bad)
