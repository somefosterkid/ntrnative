#!/usr/bin/env python3
"""tools/port_run_xvfb_tests.py: port_run.start_xvfb picks its display atomically (a flock claim): runs that start at the
same moment get different displays (the HG/SS re-pin's boot and battle runs used to be staggered by 3 s for this)."""
import os, subprocess, sys, unittest

HERE = os.path.dirname(os.path.abspath(__file__))
CHILD = ('import sys; sys.path.insert(0, %r); import port_run as P, time\n'
         'x, d = P.start_xvfb(); print(d, flush=True); time.sleep(1.5); x.kill()\n' % HERE)


class Xvfb(unittest.TestCase):
    def test_parallel_starts_get_distinct_displays(self):
        import shutil
        if not shutil.which('Xvfb'):
            self.skipTest('no Xvfb')
        ps = [subprocess.Popen([sys.executable, '-c', CHILD], stdout=subprocess.PIPE, text=True) for _ in range(4)]
        ds = [p.communicate()[0].strip() for p in ps]
        self.assertTrue(all(d.startswith(':') for d in ds), ds)
        self.assertEqual(len(set(ds)), 4, ds)


if __name__ == '__main__':
    unittest.main()
