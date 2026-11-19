#!/usr/bin/env python3
"""Tests of tools/sweep_wall.py: the frame-based verdicts of a run: slow progress is not a finding, a frame stall is a hang, a
frozen screen is stuck-screen, a player on one tile is stuck-walk. No game run."""
import argparse
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import port_run  # noqa: E402
import sweep_wall  # noqa: E402

LOG = 'bwport: boot\ntest: wild battle 151\ntest: move slot 0 move 33\n'


class Tests(unittest.TestCase):
    def test_slow_progress_is_not_a_finding(self):
        # 17 fps on a loaded host, the frame cap reached after 700 s of wall time: the verdict is the same as at 130 fps
        slow = port_run.verdict('bwport: exit-frame 12000\n', False, 12000, 0, 30, 17, 700, 0, 12000)
        fast = port_run.verdict('bwport: exit-frame 12000\n', False, 12000, 0, 30, 130, 90, 0, 12000)
        self.assertTrue(slow.startswith('exit-frame 12000 in 700 s'), slow)
        self.assertTrue(fast.startswith('exit-frame 12000 in 90 s'), fast)
        # the frames move 1 s ago, no wall watchdog (-t 0): port_run has no way to call it a hang
        self.assertFalse(port_run.verdict(LOG, False, 6800, 1, 30, 17, 400, None, 12000).startswith('hang'))

    def test_open_file_leak_stops_under_the_checker(self):
        log = 'openfiles: leak: 300 host files open at frame 4096 (peak 300, bound 256): a game opens files and never closes them\n'
        old = os.environ.pop('BWPORT_HEAPCHECK', None)
        try:
            self.assertFalse(port_run.verdict(log, False, 4096, 0, 30, 130, 90, None, 12000).startswith('openfiles'))   # said, not stopped
            os.environ['BWPORT_HEAPCHECK'] = '1'
            self.assertEqual(port_run.verdict(log, False, 4096, 0, 30, 130, 90, -6, 12000),
                             'openfiles: 300 host files open at frame 4096 (peak 300, bound 256)')
        finally:
            os.environ.pop('BWPORT_HEAPCHECK', None)
            if old is not None:
                os.environ['BWPORT_HEAPCHECK'] = old

    def test_frame_stall_is_a_hang(self):
        line = port_run.verdict(LOG, True, 700, 45, 30, 17, 120, None, 12000)    # no V-blank for 45 s
        self.assertTrue(line.startswith('hang in '), line)
        self.assertTrue(port_run.verdict(LOG, True, 0, 0, 30, 0, 60, None, 12000).startswith('hang'))   # no frame ever

    def test_t0_is_no_wall_cap(self):
        ap = argparse.ArgumentParser()
        sweep_wall.ignore_t(ap)
        self.assertIsNone(ap.parse_args([]).timeout)
        self.assertEqual(ap.parse_args(['-t', '400']).timeout, 400)     # old command lines still parse
        self.assertEqual(sweep_wall.NO_WALL_CAP, '0')

    def test_stuck_walk(self):
        warp = 'warp 1656 120\nzone 342 123 855 1748\npos 1750 10 20 in\npos 1790 11 20 in\npos 1900 12 20 in\nend 4800\n'
        self.assertEqual(sweep_wall.stuck_walk(warp, 4800, 1800), (12, 20, 1900))       # on (12,20) from 1900 to the end
        self.assertIsNone(sweep_wall.stuck_walk(warp + 'pos 4000 13 20 in\n', 4800, 1800))   # moved 800 V-blanks before the end
        self.assertEqual(sweep_wall.stuck_walk('pos 1750 10 20 in\n', 4800, 1800), (10, 20, 1800))   # never moved in the walk
        self.assertIsNone(sweep_wall.stuck_walk('', 4800, 1800))
        self.assertIsNone(sweep_wall.stuck_walk(warp, 2500, 1800))      # a walk shorter than STUCK_FRAMES cannot be stuck

    def test_stuck_screen(self):
        moving = [(f, f // 7) for f in range(0, 3000)]                  # a new picture every 7 V-blanks
        self.assertIsNone(sweep_wall.stuck_screen(moving, range(0, 3000, 20)))
        frozen = moving[:1000] + [(f, 'same') for f in range(1000, 2600)] + [(f, f) for f in range(2600, 3000)]
        self.assertEqual(sweep_wall.stuck_screen(frozen, range(0, 3000, 20)), (1000, 2599))   # 1600 V-blanks, keys pressed
        self.assertIsNone(sweep_wall.stuck_screen(frozen, [10, 20, 30]))      # frozen but nobody pressed a key: not stuck
        short = moving[:1000] + [(f, 'same') for f in range(1000, 1500)] + [(f, f) for f in range(1500, 3000)]
        self.assertIsNone(sweep_wall.stuck_screen(short, range(0, 3000, 20)))   # 500 V-blanks: a text box waiting
        tail = moving[:1000] + [(f, 'same') for f in range(1000, 2400)]       # frozen to the end of the run
        self.assertEqual(sweep_wall.stuck_screen(tail, range(0, 2400, 20)), (1000, 2399))


BT = ('x\n== port_run: stop\nProgram received signal SIGINT\n#0  0x00007f in pthread_cond_wait () from /lib/libc.so.6\n#1  0x1 in SIM_Wait () at /w/lib/ntr/sim.c:9\n'
      '\n== port_run: threads\n\nThread 2 (Thread 0x2):\n#0  0x1 in poll () from /lib/libc.so.6\n#1  0x2 in SoundThread () at /w/port/src/snd.c:4\n\n'
      'Thread 1 (Thread 0x1 "bwport"):\n#0  0x1 in __futex_wait () from /lib/libc.so.6\n#1  0x2 in OS_WaitVBlankIntr () at /w/lib/NitroSDK/os.c:77\n'
      '#2  0x3 in ov05_02200ABC () at /w/src/field/x.c:123\n#3  0x4 in main () at /w/src/main.c:5\n\n')
BANNER = 'undefined data: 2 (see build/port/linux-hgss-cov/undefined.txt)'


class FakeRun:
    def __init__(self, stdout='', stderr='', returncode=0):
        self.stdout, self.stderr, self.returncode = stdout, stderr, returncode


class WhereTests(unittest.TestCase):
    def test_frozen_row_names_the_main_threads_functions(self):
        top, text, bid = sweep_wall.frozen_top(BT)
        self.assertEqual(top, 'frozen OS_WaitVBlankIntr <- ov05_02200ABC')   # not the SIGINT-stopped thread's SIM_Wait
        self.assertIn('__futex_wait <- OS_WaitVBlankIntr (os.c:77) <- ov05_02200ABC (x.c:123)', text)
        self.assertEqual(len(bid), 8)
        self.assertEqual(sweep_wall.frozen_top('no gdb stop here\n'), ('frozen ?', '', ''))

    def test_stuck_row_names_the_script_or_the_task(self):
        log = 'bwport: x\nbwport: state: frame=12000 ovl=1,5,12 field=up task=+0x1a2b script=3150 mode=2 native=+0x77f0\nbwport: exit-frame 12000\n'
        self.assertEqual(sweep_wall.stuck_top(log)[0], 'stuck script 3150 wait +0x77f0')
        self.assertIn('ovl=1,5,12', sweep_wall.stuck_top(log)[1])
        self.assertEqual(sweep_wall.stuck_top('bwport: state: frame=9 ovl=1 task=+0x1 script=7 mode=1 native=+0x0\n')[0], 'stuck script 7 bytecode')
        self.assertEqual(sweep_wall.stuck_top('bwport: state: frame=9 ovl=1,5 field=up task=+0x1a2b\n')[0], 'stuck task +0x1a2b')
        self.assertEqual(sweep_wall.stuck_top('bwport: state: frame=9 ovl=1,5,12\n')[0], 'stuck ovl 12')
        self.assertEqual(sweep_wall.stuck_top('bwport: exit-frame 9\n'), ('stuck ?', '', ''))   # an exe without the hook

    def test_no_verdict_is_a_harness_row_not_the_banner(self):
        import tempfile
        with tempfile.TemporaryDirectory() as d:
            tb = FakeRun(BANNER + '\n', 'Traceback (most recent call last):\n  File "x"\nOSError: [Errno 61] No data available\n', 1)
            line = sweep_wall.verdict_line(tb, d)
            self.assertTrue(line.startswith('harness: no verdict from port_run (rc 1): OSError: [Errno 61]'), line)
            self.assertNotIn('undefined data', line)
            self.assertTrue(sweep_wall.verdict_line(FakeRun(BANNER + '\n', '', 0), d).startswith('harness:'))
            open(os.path.join(d, 'verdict.txt'), 'w').write('hang in F, frame 9\n')
            self.assertEqual(sweep_wall.verdict_line(tb, d), 'hang in F, frame 9')

    def test_port_run_survives_a_failed_log_read(self):
        import tempfile
        with tempfile.TemporaryDirectory() as d:
            p = os.path.join(d, 'log.txt')
            open(p, 'w').write('abcdef')
            self.assertEqual(port_run.read_log(p, 2), b'cdef')
            self.assertEqual(port_run.read_log(p + '.gone', 0, 1), b'')   # the drive's ENODATA is an OSError too: no traceback


if __name__ == '__main__':
    unittest.main()
