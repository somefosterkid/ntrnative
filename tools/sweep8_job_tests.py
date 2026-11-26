#!/usr/bin/env python3
"""tools/sweep8_job_tests.py: --maps A-B slicing and the progress line of tools/sweep8_job.sh (SWEEP8_DRY=1 prints the steps, no build)."""
import os
import subprocess
import tempfile
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
SCRIPT = os.path.join(HERE, 'sweep8_job.sh')


def dry(*args):
    with tempfile.TemporaryDirectory() as d:
        env = dict(os.environ, SWEEP8_DRY='1')
        r = subprocess.run(['bash', SCRIPT, *args], cwd=d, env=env, capture_output=True, text=True, timeout=60)
    return [l for l in r.stdout.splitlines() if l.startswith('DRY ')]


class Maps(unittest.TestCase):
    def test_default_sweeps_everything_plus_apps(self):
        lines = dry('n', 'maps_hg', 'maps_ss')
        self.assertIn('--maps all,apps --apps-tap', lines[0])
        self.assertIn('--maps all ', lines[1])

    def test_maps_slice(self):
        lines = dry('--maps', '271-700', 'hgB', 'maps_hg', 'maps_ss')
        self.assertIn('--maps 271-700 ', lines[0])
        self.assertNotIn('apps', lines[0])
        self.assertIn('--maps 271-700 ', lines[1])
        self.assertIn('--game soulsilver', lines[1])

    def test_other_steps_unchanged(self):
        self.assertIn('hg_battle_sweep.py --pass B', dry('--maps', '0-9', 'n', 'battleB_hg')[0])


class Progress(unittest.TestCase):
    def test_progress_line_follows_the_sweep_tsv_and_the_job_ends(self):
        src = open(SCRIPT).read()
        block = src[src.index('progress() {'):src.index('for st in')]
        with tempfile.TemporaryDirectory() as d:
            o = os.path.join(d, 'build', 'sweep8', 'X')
            os.makedirs(o)
            prog = os.path.join(d, 'prog')
            script = (f'O={o}; NAME=X; NSTEPS=2; NDONE=0; t0=$(date +%s); J=1\n' + block +
                      f'run slow bash -c \'mkdir -p {o}/slow; echo head > {o}/slow/sweep.tsv; sleep 1.5; echo r1 >> {o}/slow/sweep.tsv; '
                      f'echo r2 >> {o}/slow/sweep.tsv; sleep 2.5\'\n'
                      'echo NDONE=$NDONE\n')
            env = dict(os.environ, SWEEP8_PROGRESS_S='1', NTR_PROGRESS=prog)
            r = subprocess.run(['bash', '-c', script], cwd=d, env=env, capture_output=True, text=True, timeout=60)
            self.assertIn('NDONE=1', r.stdout, r.stderr)
            self.assertIn('== slow rc=0', r.stdout)
            msg = open(prog).read()
            self.assertIn('sweep8 X: step 1/2 slow: 2 rows done', msg)


if __name__ == '__main__':
    unittest.main()
