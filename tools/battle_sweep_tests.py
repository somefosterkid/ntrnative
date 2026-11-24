#!/usr/bin/env python3
"""tools/battle_sweep_tests.py: unit tests of the sweep core (no game): the dynamic queue claims each unit once, --pairs parsing,
case lists, the watchdog ending a process whose V-blank counter stands still, the report."""
import argparse
import base64
import gzip
import os
import subprocess
import sys
import tempfile
import threading
import time
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import battle_sweep as bs  # noqa: E402
import battle_sweep_hg as hg  # noqa: E402


class FakeExe:
    def __init__(self, pid, frames):
        self.pid, self.frames, self.i = pid, frames, 0

    def find(self):
        return self.pid

    def frame(self):
        v = self.frames[min(self.i, len(self.frames) - 1)]
        self.i += 1
        return v


class Cases(unittest.TestCase):
    def test_partner_covers_every_move_once_per_side(self):
        ps = [bs.foe_partner(m, 467) for m in range(1, 468)]
        self.assertEqual(sorted(ps), list(range(1, 468)))

    def test_pairs(self):
        c = bs.make_pairs('464:150@161/5,86:150')
        self.assertEqual(c[0], ('p464-150@161L5', 'moves', 464, 150, '161/5'))
        self.assertEqual(c[1][4], '')
        with self.assertRaises(SystemExit):
            bs.make_pairs('x')

    def test_pair_env_sets_the_foe_species(self):
        a = argparse.Namespace()
        env = hg.instance_env('moves', [('p464-150@161L5', 'moves', 464, 150, '161/5')], a)
        self.assertIn(':161/5:', env['BWPORT_HG_TESTBATTLE'])
        self.assertEqual(env['BWPORT_HG_TESTLIST'], '464:150')

    def test_decode_rows_survives_a_cut_line(self):
        good = base64.b64encode(gzip.compress(b'a\tb\nc\td\n')).decode()
        self.assertEqual(bs.decode_rows('BSROWS ' + good), ['a\tb', 'c\td'])
        self.assertEqual(bs.decode_rows('BSROWS ' + good[:-9]), [])
        self.assertEqual(bs.decode_rows('BSROWS ' + good[:7] + '\x00\x00' + good[7:]), ['a\tb', 'c\td'])
        self.assertEqual(bs.decode_rows('BSROWS'), [])

    def test_drop_off_machines(self):
        sh = [['bd', '3'], ['laptop', '1']]
        self.assertEqual(bs.drop_off(sh, ''), sh)
        self.assertEqual(bs.drop_off(sh, 'bd'), [['laptop', '1']])
        self.assertEqual(bs.drop_off(sh, 'bd,laptop'), [])

    def test_take_batch(self):
        cs = [('m1', 'moves', 1, 2), ('m2', 'moves', 2, 3), ('m3', 'moves', 3, 4)]
        self.assertEqual(len(bs.take_batch(cs, 2)), 2)
        self.assertEqual(len(bs.take_batch([('p1', 'moves', 1, 2, '161/5')] + cs, 5)), 1)

    def test_shard_slices_cover(self):
        cs = list(range(467))
        parts = [bs.shard_slice(cs, f'{i}/3', '3,3,1') for i in (1, 2, 3)]
        self.assertEqual(sum(parts, []), cs)
        self.assertGreater(len(parts[0]), len(parts[2]) * 2)


class Queue(unittest.TestCase):
    def test_claim_once_across_threads(self):
        with tempfile.TemporaryDirectory() as d:
            won = []
            lock = threading.Lock()

            def go():
                for k in range(200):
                    if bs.claim(d, f'm{k}-{k}'):
                        with lock:
                            won.append(k)
            ts = [threading.Thread(target=go) for _ in range(8)]
            [t.start() for t in ts]
            [t.join() for t in ts]
            self.assertEqual(sorted(won), list(range(200)))


class Watchdog(unittest.TestCase):
    def run_fake(self, frames, secs, **kw):
        a = argparse.Namespace(watchdog=secs, max_log=300)
        with tempfile.TemporaryDirectory() as d:
            log = os.path.join(d, 'log.txt')
            open(log, 'w').close()
            p = subprocess.Popen(['sleep', '120'], start_new_session=True)
            real = bs._frame_reader
            bs._frame_reader = lambda pid: FakeExe(pid, frames)
            try:
                t0 = time.time()
                why = bs.watch_run(a, p, log)
                return why, time.time() - t0, p.poll()
            finally:
                bs._frame_reader = real
                try:
                    os.killpg(p.pid, 9)
                except OSError:
                    pass

    def test_frozen_counter_is_killed(self):
        why, wall, rc = self.run_fake([5, 6, 7, 8, 8], 3)
        self.assertIn('no V-blank for', why)
        self.assertIsNotNone(rc)
        self.assertLess(wall, 40)

    def test_boot_gets_three_times_the_limit(self):
        # the counter never starts (0): 3x the limit before it is a hang
        why, wall, rc = self.run_fake([0], 2)
        self.assertIn('no V-blank for', why)
        self.assertGreaterEqual(wall, 5)

    def test_spam_log_is_killed(self):
        a = argparse.Namespace(watchdog=1000, max_log=1)
        with tempfile.TemporaryDirectory() as d:
            log = os.path.join(d, 'log.txt')
            with open(log, 'wb') as f:
                f.write(b'x' * (2 * 1048576))
            p = subprocess.Popen(['sleep', '120'], start_new_session=True)
            real = bs._frame_reader
            bs._frame_reader = lambda pid: FakeExe(pid, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20])
            try:
                why = bs.watch_run(a, p, log)
            finally:
                bs._frame_reader = real
                try:
                    os.killpg(p.pid, 9)
                except OSError:
                    pass
            self.assertIn('log over', why)


class Report(unittest.TestCase):
    def test_labels(self):
        self.assertIn('our move 86', bs.case_label(hg, 'm86-150'))
        self.assertIn('THUNDER_WAVE', bs.case_label(hg, 'm86-150')) if hg.names()[0] else None
        self.assertIn('foe species 16', bs.case_label(hg, 'f16'))


if __name__ == '__main__':
    unittest.main()
