#!/usr/bin/env python3
"""tools/battle_sweep_tests.py: unit tests of the sweep core (no game): the dynamic queue claims each unit once, --pairs parsing,
case lists, the watchdog ending a process whose V-blank counter stands still, the report."""
import argparse
import base64
import gzip
import os
import signal
import subprocess
import sys
import tempfile
import threading
import time
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import battle_sweep as bs  # noqa: E402
import battle_sweep_hg as hg  # noqa: E402
import battle_sweep_pt as pt  # noqa: E402
import battle_sweep_bw as bw  # noqa: E402


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

    def test_isolate_reads_pairs_keys(self):
        seen = []
        orig = bs.run_batch
        bs.run_batch = lambda a, ad, kind, cases, slot, rec: (seen.append(cases[0]), rec(dict(outcome='ok')))
        try:
            rows = [dict(case='p433-153@161L5', kind='moves', outcome='hang')]
            a = argparse.Namespace(j=1, pairs='433:153@161/5')
            bs.isolate(a, bw, rows, lambda r: None)
        finally:
            bs.run_batch = orig
        self.assertEqual(rows[0]['culprit'], 'state')
        self.assertEqual([c[2:] for c in seen], [(433, bw.HARMLESS, '161/5'), (bw.HARMLESS, 153, '161/5')])

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


class ShardWorkDir(unittest.TestCase):
    """a --rows shard removes its bs-* work dir when it ends (also on failure); --keep-work and other dirs stay"""

    def go(self, name, rows=True, keep=False, boom=False):
        with tempfile.TemporaryDirectory() as d:
            out = os.path.join(d, name)
            a = argparse.Namespace(out=out, rows=rows, keep_work=keep)

            def core(a, ad):
                os.makedirs(os.path.join(a.out, 'runs'))

                if boom:
                    raise RuntimeError('x')
                return 0
            old, bs.run_core = bs.run_core, core
            old_sig = signal.getsignal(signal.SIGTERM)
            try:
                try:
                    bs.cmd_run(a, None)
                except RuntimeError:
                    pass
            finally:
                bs.run_core = old
                signal.signal(signal.SIGTERM, old_sig)
            return os.path.exists(out)

    def test_removed_when_done(self):
        self.assertFalse(self.go('bs-0101-1a'))

    def test_removed_on_failure(self):
        self.assertFalse(self.go('bs-0101-1a', boom=True))

    def test_keep_work(self):
        self.assertTrue(self.go('bs-0101-1a', keep=True))

    def test_other_dirs_and_non_rows_runs_stay(self):
        self.assertTrue(self.go('results'))
        self.assertTrue(self.go('bs-repro', rows=False))


class Resume(unittest.TestCase):
    def rows(self, d, outcomes):
        with open(os.path.join(d, 'results.tsv'), 'w') as f:
            f.write('\t'.join(bs.HEAD) + '\n')
            for i, o in enumerate(outcomes):
                f.write('\t'.join([f'm{i}', 'moves', o] + [''] * (len(bs.HEAD) - 3)) + '\n')

    def test_harness_rows_are_not_done(self):
        with tempfile.TemporaryDirectory() as d:
            self.rows(d, ['ok', 'harness', 'crash', 'harness'])
            self.assertEqual(bs.drop_harness_rows(d), 2)
            self.assertEqual([r['case'] for r in bs.load_rows(d)], ['m0', 'm2'])
            self.assertEqual(bs.drop_harness_rows(d), 0)   # nothing left to drop: the file is untouched

    def test_all_harness_stale_dir_runs_everything_again(self):
        with tempfile.TemporaryDirectory() as d:
            self.rows(d, ['harness'] * 5)
            bs.drop_harness_rows(d)
            done = {r['case'] for r in bs.load_rows(d)}
            self.assertEqual([c for c in ('m0', 'm1', 'm2', 'm3', 'm4') if c not in done], ['m0', 'm1', 'm2', 'm3', 'm4'])

    def test_no_table_is_fine(self):
        with tempfile.TemporaryDirectory() as d:
            self.assertEqual(bs.drop_harness_rows(d), 0)


class Progress(unittest.TestCase):
    def test_write_progress_only_with_a_path(self):
        with tempfile.TemporaryDirectory() as d:
            p = os.path.join(d, 'progress')
            bs.write_progress('[5s] 3/9 cases', p)
            bs.write_progress('[9s] 6/9 cases', p)
            self.assertEqual(open(p).read(), '[9s] 6/9 cases\n')
            os.environ.pop('NTR_PROGRESS', None)
            bs.write_progress('x')   # no path, no env: nothing happens

    def test_peek_progress_asks_the_runner_for_the_runs_progress_file(self):
        with tempfile.TemporaryDirectory() as d:
            runner = os.path.join(d, 'fake_runner.sh')
            with open(runner, 'w') as f:
                f.write('#!/bin/sh\n[ "$1" = --peek ] && [ "$2" = lc-1-ab ] && [ "$3" = progress ] && echo "[40s] 7/20 cases (worker 1)"\n')
            os.chmod(runner, 0o755)
            log = os.path.join(d, 'shard-1a.log')
            self.assertEqual(bs.peek_progress([runner], log), '')   # no log yet
            open(log, 'w').write('hqjob: hq-9 queued\n')
            self.assertEqual(bs.peek_progress([runner], log), '')   # no run id yet
            open(log, 'a').write('lp_route: lc-1-ab: commit 123456789 (x)\n')
            self.assertEqual(bs.peek_progress([runner], log), '[40s] 7/20 cases (worker 1)')
            self.assertEqual(bs.peek_progress([os.path.join(d, 'missing')], log), '')


class Report(unittest.TestCase):
    def test_labels(self):
        self.assertIn('our move 86', bs.case_label(hg, 'm86-150'))
        self.assertIn('THUNDER_WAVE', bs.case_label(hg, 'm86-150')) if hg.names()[0] else None
        self.assertIn('foe species 16', bs.case_label(hg, 'f16'))


class PtAdapter(unittest.TestCase):
    def test_env(self):
        a = argparse.Namespace(game='heartgold', save=None)
        pt.configure(a)
        self.assertEqual(a.game, 'platinum')
        env = pt.instance_env('moves', [('m1-2', 'moves', 1, 2), ('m3-4', 'moves', 3, 4)], a)
        self.assertEqual(env['BWPORT_PT_TESTLIST'], '1:2,3:4')
        self.assertTrue(env['BWPORT_PT_TESTBATTLE'].endswith(':2,2,2,2'))
        self.assertFalse([k for k in env if 'HG' in k])
        env = pt.instance_env('species-foe', [('f16', 'species-foe', 16)], a)
        self.assertEqual((env['BWPORT_PT_TESTBATTLE'], env['BWPORT_PT_TESTSLOT']), ('151:100:150,33,247,94:16/50', 'stop'))

    def test_logs(self):
        self.assertTrue(pt.species_ok('test: moves done (0) at V-blank 2100\n'))
        self.assertEqual(pt.species_frames('test: battle over at V-blank 2100\n'), '2100')
        self.assertFalse(pt.battle_started('nothing'))

    def test_names(self):
        mv, sp = pt.names()
        if mv:
            self.assertEqual((mv[1], sp[1], sp[493]), ('POUND', 'BULBASAUR', 'ARCEUS'))
        self.assertIn('--family', pt.RUNNER_ARGS)


class BwAdapterTests(unittest.TestCase):
    def test_env_and_names(self):
        a = argparse.Namespace(game='heartgold', save=None, what='moves')
        bw.configure(a)
        self.assertEqual(a.game, 'black')
        env = bw.instance_env('moves', [('m1-2', 'moves', 1, 2), ('m3-4', 'moves', 3, 4)], a)
        self.assertEqual((env['BWPORT_BW_TESTLIST'], env['BWPORT_BW_TESTBATTLE']), ('1:2,3:4', '151:50'))
        env = bw.instance_env('species-foe', [('f16', 'species-foe', 16)], a)
        self.assertEqual((env['BWPORT_BW_TESTBATTLE'], env['BWPORT_BW_TESTSLOT']), ('16:50', 'stop'))
        self.assertFalse([k for k in env if '_PT_' in k or '_HG_' in k])
        mv, sp = bw.names()
        if mv:
            self.assertEqual((mv[1], sp[1], sp[649]), ('POUND', 'BULBASAUR', 'GENESECT'))
        self.assertEqual((bw.MAX_MOVE, bw.MAX_SPECIES), (559, 649))


if __name__ == '__main__':
    unittest.main()
