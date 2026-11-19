#!/usr/bin/env python3
"""Tests of tools/pt_battle_sweep.py's time handling: run_one() with a fake port_run (no game run): no wall cap is passed (-t 0), a
run that was merely slow is judged like a fast one, a frame stall is a hang."""
import argparse
import os
import sys
import tempfile
import types
import unittest
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pt_battle_sweep as S  # noqa: E402
import port_run  # noqa: E402

BATTLE_LOG = 'test: wild battle 242\ntest: move slot 0 move 33\n'


def fake_run(line, log, calls):
    """stands for `nice -n 10 python3 tools/port_run.py ... --out DIR`: writes DIR/verdict.txt and DIR/log.txt"""
    def run(cmd, **kw):
        out = cmd[cmd.index('--out') + 1]
        os.makedirs(out, exist_ok=True)
        open(os.path.join(out, 'verdict.txt'), 'w').write(line + '\n')
        open(os.path.join(out, 'log.txt'), 'w').write(log)
        calls.append(cmd)
        return types.SimpleNamespace(stdout=line + '\n', stderr='')
    return run


class Tests(unittest.TestCase):
    def one(self, line, log=BATTLE_LOG):
        calls = []
        with tempfile.TemporaryDirectory() as t:
            a = argparse.Namespace(out=t, game='platinum', build='b', save='s', frames=12000, timeout=None, skip=4, heapcheck=None)
            with mock.patch.object(S.subprocess, 'run', fake_run(line, log, calls)):
                row, _ = S.run_one(a, 'B', 75, 'spec', {}, 0)
        return row, calls[0]

    def test_no_wall_cap(self):
        row, cmd = self.one('exit-frame 12000 in 90 s (130 fps)')
        self.assertEqual(cmd[cmd.index('-t') + 1], '0')               # port_run: no wall watchdog
        self.assertEqual(cmd[cmd.index('--frames') + 1], '12000')     # the game-frame cap ends the run
        self.assertEqual(cmd[cmd.index('--hang') + 1], '30')          # the one wall-clock check: no frame for 30 s

    def test_slow_progress_is_ok(self):
        ok = BATTLE_LOG + 'test: moves done at V-blank 5100\n'
        fast = self.one('exit-frame 5100 in 30 s (170 fps)', log=ok)[0]
        slow = self.one('exit-frame 5100 in 700 s (7 fps)', log=ok)[0]     # a loaded host: the same verdict
        self.assertEqual((fast[3], fast[4]), ('ok', '5100'))
        self.assertEqual((slow[3], slow[4]), ('ok', '5100'))
        self.assertEqual(self.one('exit-frame 12000 in 700 s (17 fps)')[0][3], 'cap')

    def test_stall_is_a_hang(self):
        line = port_run.verdict(BATTLE_LOG, True, 700, 45, 30, 17, 120, None, 12000)
        self.assertEqual(self.one(line)[0][3], 'hang')


if __name__ == '__main__':
    unittest.main()
