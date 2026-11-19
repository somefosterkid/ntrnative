#!/usr/bin/env python3
"""Tests of the time handling in hg_sweep, hg_battle_sweep, hg_app_sweep and hg_int_sweep (tools/sweep_wall.py): each run function
with a fake port_run (no game run): no wall cap (-t 0) and a frame cap are passed, a run that was merely slow is judged like a
fast one, a frame stall is a hang, a player on one tile is stuck-walk (hg_sweep), stuck-walk is a finding (crash_groups)."""
import argparse
import os
import sys
import tempfile
import types
import unittest
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import hg_app_sweep  # noqa: E402
import hg_battle_sweep  # noqa: E402
import hg_int_sweep  # noqa: E402
import hg_sweep  # noqa: E402
import port_run  # noqa: E402

LOG = 'test: wild battle 242\ntest: move slot 0 move 33\n'
HANG = port_run.verdict(LOG, True, 700, 45, 30, 17, 120, None, 12000)    # no V-blank for 45 s
CAPPED = 'exit-frame 9000 in 900 s (10 fps)'                               # slow, and the frame cap ended it
WALK_OK = 'warp 1656 120\nzone 7 5 5 1700\npos 1750 5 6 in\npos 4300 6 6 in\nend 4800\n'
WALK_STUCK = 'warp 1656 120\nzone 7 5 5 1700\npos 1750 5 6 in\npos 1900 6 6 in\nend 4800\n'   # on (6,6) from 1900 to 4800


def fake_run(line, calls, log=LOG, warp=''):
    """stands for `nice -n 10 python3 tools/port_run.py ... --out DIR`: writes DIR/verdict.txt and DIR/log.txt (and the hook's
    warp.txt, where BWPORT_HG_WARP_LOG says)"""
    def run(cmd, env=None, **kw):
        out = cmd[cmd.index('--out') + 1]
        os.makedirs(out, exist_ok=True)
        open(os.path.join(out, 'verdict.txt'), 'w').write(line + '\n')
        open(os.path.join(out, 'log.txt'), 'w').write(log)
        if warp and env and env.get('BWPORT_HG_WARP_LOG'):
            open(env['BWPORT_HG_WARP_LOG'], 'w').write(warp)
        calls.append(cmd)
        return types.SimpleNamespace(stdout=line + '\n', stderr='')
    return run


def opt(cmd, name):
    return cmd[cmd.index(name) + 1]


class Tests(unittest.TestCase):
    def map_run(self, line, warp='', mid=7, frames=3000):
        calls = []
        with tempfile.TemporaryDirectory() as t:
            a = argparse.Namespace(out=t, build='b', reload='', game='heartgold', save='s', frames=frames, seed=0, skip=4,
                                   heapcheck=None)
            with mock.patch('subprocess.run', fake_run(line, calls, warp=warp)):
                row, _ = hg_sweep.run_map(a, mid, 'MAP', (1, 1), 0, None)
        return row, calls[0]

    def test_hg_sweep(self):
        row, cmd = self.map_run('exit 0 at frame 4800', WALK_OK)
        self.assertEqual(row[2], 'ok')
        self.assertEqual(opt(cmd, '-t'), '0')                                              # no wall cap
        self.assertEqual(opt(cmd, '--frames'), str(hg_sweep.WALK_FROM + 3000 + 6000))      # the frame cap: END + 6000
        slow = self.map_run('exit 0 at frame 4800 (7 fps, 900 s)', WALK_OK)[0]
        self.assertEqual(slow[2], 'ok')                                                    # a slow host: the same verdict
        self.assertEqual(self.map_run(HANG, WALK_OK)[0][2], 'hang')
        self.assertEqual(self.map_run(CAPPED, WALK_OK.replace('end 4800\n', ''))[0][2], 'exit')   # the cap, a script at END

    def test_stuck_walk(self):
        row, _ = self.map_run('exit 0 at frame 4800', WALK_STUCK, frames=3000)
        self.assertEqual((row[2], row[4]), ('stuck-walk', 'MAP 6,6'))                      # a finding with its place
        self.assertEqual(self.map_run('exit 0 at frame 4800', WALK_OK)[0][2], 'ok')
        self.assertEqual(self.map_run('exit 0 at frame 4800', WALK_STUCK, mid=266)[0][2], 'entered-wrong')   # a harness artefact

    def battle_run(self, line, log=LOG):
        calls = []
        with tempfile.TemporaryDirectory() as t:
            a = argparse.Namespace(out=t, game='heartgold', build='b', save='s', frames=12000, timeout=None, skip=4, heapcheck=None)
            with mock.patch('subprocess.run', fake_run(line, calls, log)):
                row, _ = hg_battle_sweep.run_one(a, 'B', 75, 'spec', {}, 0)
        return row, calls[0]

    def test_hg_battle_sweep(self):
        row, cmd = self.battle_run('exit-frame 12000 in 700 s (17 fps)')
        self.assertEqual(row[3], 'cap')
        self.assertEqual((opt(cmd, '-t'), opt(cmd, '--frames')), ('0', '12000'))
        ok = LOG + 'test: moves done at V-blank 5100\n'
        self.assertEqual(self.battle_run('exit-frame 5100 in 700 s (7 fps)', ok)[0][3:5], ['ok', '5100'])
        self.assertEqual(self.battle_run(HANG)[0][3], 'hang')

    def test_hg_app_sweep(self):
        p = hg_app_sweep.Play('x', 'X', 'a+60', warp=(1, 2, 3))
        for line, want in ((HANG, 'hang'), (CAPPED, 'timeout')):
            calls = []
            with tempfile.TemporaryDirectory() as t:
                a = argparse.Namespace(out=t, build='b', reload='', shots=0, skip=4, heapcheck=None)
                with mock.patch('subprocess.run', fake_run(line, calls, warp='warp 1700 5\n')):
                    row, _ = hg_app_sweep.run_play(a, p, 0, None)
            self.assertEqual(row[2], want)
            self.assertEqual((opt(calls[0], '-t'), opt(calls[0], '--frames')), ('0', str(p.end + 6000)))

    def test_hg_int_sweep(self):
        a = argparse.Namespace(out='', build='b', reload='', game='heartgold', save='s', budget=1500, maxn=40, skip=4,
                               no_force=False, no_show=False, heapcheck=None)
        calls = []
        with tempfile.TemporaryDirectory() as t:
            a.out = t
            with mock.patch('subprocess.run', fake_run(CAPPED, calls)):
                hg_int_sweep.run_segment(a, 7, (1, 1), 0, 0)
        self.assertEqual((opt(calls[0], '-t'), opt(calls[0], '--frames')), ('0', str(40 * 1500 + 6000)))

    def test_summary_groups(self):
        with tempfile.TemporaryDirectory() as t:
            rows = [[1, 'A', 'ok', '4800', '', '', '1', '1', '40', ''], [3, 'B', 'stuck-walk', '', 'B 6,6', 'x', '3', '3', '40', ''],
                    [9, 'C', 'stuck-walk', '', 'C 1,2', 'x', '9', '9', '40', ''], [10, 'D', 'exit', '', '', '', '', '', '40', '']]
            for r in rows[1:3]:
                os.makedirs(os.path.join(t, 'maps', str(r[0])))
                open(os.path.join(t, 'maps', str(r[0]), 'bt.txt'), 'w').write(f'line\non one tile {r[0]}\n')
            open(os.path.join(t, 'sweep.tsv'), 'w').write('\t'.join(hg_sweep.HEAD) + '\n' +
                                                          ''.join('\t'.join(map(str, r)) + '\n' for r in rows))
            hg_sweep.summarize(argparse.Namespace(out=t, save='s', frames=3000), {i: (f'MAP_{i}', None) for i in range(12)})
            groups = [l.split('\t') for l in open(os.path.join(t, 'crash_groups.tsv')).read().splitlines()[1:]]
            self.assertEqual(sorted(g[0] for g in groups), ['B 6,6', 'C 1,2'])             # stuck-walk is a finding, exit is not


if __name__ == '__main__':
    unittest.main()
