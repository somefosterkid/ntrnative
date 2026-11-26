#!/usr/bin/env python3
"""Tests of tools/pt_sweep.py: the map table and spawn tiles from a made-up tree, the walk, the verdicts of made-up runs and the
summary files of a made-up out dir (no game run)."""
import argparse
import gzip
import json
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pt_sweep  # noqa: E402

BT = """log line
== port_run: stop
#0  0x0000555556f051e6 in Window_ClearAndCopyToVRAM (window=0x5555) at ../../pokeplatinum/src/bg_window.c:1932
#1  0x000055555706c335 in MapNamePopUp_Hide (mapPopUp=0x5555) at ../../pokeplatinum/src/overlay005/map_name_popup.c:261
#2  0x000055555706c079 in SysTask_MapNamePopUpWindow (task=0x5555, data=0x5555) at ../../pokeplatinum/src/overlay005/map_name_popup.c:169
#3  0x0000555556f0bbd3 in SysTaskManager_ExecuteTasks (sysTaskMgr=0x5555) at ../../pokeplatinum/src/sys_task_manager.c:108

== port_run: threads
#0  0x1 in poll ()
"""


def make_tree(root):
    os.makedirs(os.path.join(root, 'generated'))
    names = ['MAP_HEADER_EVERYWHERE', 'MAP_HEADER_TWINLEAF_TOWN', 'MAP_HEADER_ROUTE_201', 'MAP_HEADER_COUNT',
             'MAP_HEADER_INVALID = MAP_HEADER_COUNT']
    open(os.path.join(root, 'generated/map_headers.txt'), 'w').write('\n'.join(names) + '\n')
    for d, ev in (('twinleaf_town', {'warp_events': [{'x': 10, 'z': 20, 'dest_header_id': 'X'}], 'object_events': []}),
                  ('route_201', {'warp_events': [], 'object_events': [{'x': 5, 'z': 7}], 'bg_events': [{'x': 1, 'z': 2}]})):
        os.makedirs(os.path.join(root, 'res/field/maps', d))
        json.dump(ev, open(os.path.join(root, 'res/field/maps', d, 'events.json'), 'w'))


class Tests(unittest.TestCase):
    def test_maps_and_spawn(self):
        with tempfile.TemporaryDirectory() as t:
            make_tree(t)
            maps = pt_sweep.read_maps(t)
            self.assertEqual(sorted(maps), [0, 1, 2])   # MAP_HEADER_COUNT and what follows are not maps
            self.assertIsNone(maps[0][1])                # no directory: no-spawn
            self.assertEqual(pt_sweep.spawn(maps[1][1]), (10, 20))   # the first warp event's tile
            self.assertEqual(pt_sweep.spawn(maps[2][1]), (5, 8))     # no warps: the object's tile, one below
            self.assertIsNone(pt_sweep.spawn(os.path.join(t, 'nope.json')))
            self.assertEqual(pt_sweep.parse_maps('all', maps), [0, 1, 2])
            self.assertEqual(pt_sweep.parse_maps('1-9,411', maps), [1, 2, 411])

    def test_needs_session(self):
        self.assertIn('Union Room', pt_sweep.needs_session('466', 'crash'))   # the 11-25 sweep: sub_0205C820 <- ScrCmd_142 on map 466
        self.assertIsNone(pt_sweep.needs_session('466', 'ok'))              # an ok run stays ok
        self.assertIsNone(pt_sweep.needs_session('70', 'crash'))            # a real map stays a finding
        with tempfile.TemporaryDirectory() as t:                            # an older 9-column sweep.tsv is reclassified by --summary
            class A: out, save, frames = t, 'pt-x', 100
            rows = ['\t'.join(pt_sweep.HEAD[:9]),
                    '466\tMAP_HEADER_UNION_ROOM\tcrash\t1742\tsub_0205C820\tb8ccddc6\t\t\t39',
                    '70\tMAP_HEADER_X\tcrash\t900\tfoo\t11111111\t\t\t5']
            open(os.path.join(t, 'sweep.tsv'), 'w').write('\n'.join(rows) + '\n')
            os.makedirs(os.path.join(t, 'maps', '70'))
            pt_sweep.summarize(A, {466: ('MAP_HEADER_UNION_ROOM', None), 70: ('MAP_HEADER_X', None)})
            groups = open(os.path.join(t, 'crash_groups.tsv')).read()
            self.assertNotIn('sub_0205C820', groups)
            self.assertIn('foo', groups)
            self.assertIn('needs-session', open(os.path.join(t, 'sweep.tsv')).read())
    def test_walk(self):
        a, b = pt_sweep.walk_keys(342, 3000, 1800), pt_sweep.walk_keys(342, 3000, 1800)
        self.assertEqual(a, b)                                    # seeded by the map: the same every run
        self.assertNotEqual(a, pt_sweep.walk_keys(343, 3000, 1800))
        frames = [int(i.rsplit('@f', 1)[1]) for i in a.split()]
        self.assertTrue(1800 <= min(frames) and max(frames) < 1800 + 3000 + 50 and frames == sorted(frames))

    def test_crash_info(self):
        top, text, bid = pt_sweep.crash_info(BT)
        self.assertEqual(top, 'Window_ClearAndCopyToVRAM')
        self.assertIn('MapNamePopUp_Hide (map_name_popup.c:261) <- SysTask_MapNamePopUpWindow', text)
        self.assertEqual(len(bid), 8)
        self.assertEqual(pt_sweep.crash_info('nothing'), ('?', '', ''))

    def test_classify(self):
        warp = 'warp 1656 120\nzone 342 123 855 1748\nend 4800\n'
        c = pt_sweep.classify
        self.assertEqual(c('exit 0 at frame 4800', '', warp, 342)[:2], ('ok', '4800'))
        self.assertEqual(c('exit 0 at frame 4800', '', 'warp 1656 120\n', 342)[0], 'no-field')   # the map never came up free
        self.assertEqual(c('exit 0 at frame 4800', '', 'zone 5 1 1 9\nend 4800\n', 342)[0], 'no-field')   # another map's
        self.assertEqual(c('timeout', '', warp.replace('end 4800\n', ''), 342)[0], 'timeout')
        # the frame cap with a script still running at END (the exe did not exit by itself): exit, slow or fast host alike
        self.assertEqual(c('exit-frame 10800 in 700 s (15 fps)', '', warp.replace('end 4800\n', ''), 342)[:2], ('exit', '10800'))
        self.assertEqual(c('hang in WaitForIt (no frame for 30 s)', BT, '', 342)[0], 'hang')   # frames stopped: a finding
        v = c('crash SIGSEGV in Window_ClearAndCopyToVRAM (bg_window.c:1932) <- X, frame 6900', BT, warp, 342)
        self.assertEqual((v[0], v[1], v[2]), ('crash', '6900', 'Window_ClearAndCopyToVRAM'))
        v = c('heapcheck: heap overrun after Foo (a.c:3) <- Bar, frame 77 (clean at 76), heap #2', '', warp, 342)
        self.assertEqual((v[0], v[1], v[2]), ('heap', '77', 'heap overrun after Foo'))
        # a hang names the main thread's frames from gdb's stop (sweep_wall.frozen_top), never the verdict line's function
        hbt = BT.replace('== port_run: stop\n', '== port_run: stop\nThread 1 (Thread 0x7f (LWP 9) "bwport"):\n')
        self.assertEqual(c('hang in WaitForIt (no frame for 30 s)', hbt, warp, 342)[:3],
                         ('hang', '', 'frozen Window_ClearAndCopyToVRAM <- MapNamePopUp_Hide'))
        self.assertEqual(c('hang in WaitForIt (no frame for 30 s)', BT, warp, 342)[2], 'frozen ?')   # no Thread 1: unknown
        self.assertEqual(c('stub GetThing called from X, frame 0', '', '', 342)[:3], ('stub', '0', 'GetThing'))

    def test_run_map_no_wall_cap(self):
        import types
        from unittest import mock
        calls = []

        def run(cmd, **kw):
            calls.append(cmd)
            out = cmd[cmd.index('--out') + 1]
            os.makedirs(out, exist_ok=True)
            open(os.path.join(out, 'verdict.txt'), 'w').write('exit-frame 10800 in 900 s (12 fps)\n')
            return types.SimpleNamespace(stdout='', stderr='')
        with tempfile.TemporaryDirectory() as t:
            a = argparse.Namespace(out=t, build='b', save='s', frames=3000, walk_from=1800, seed=0, skip=4, reload='', heapcheck=None)
            with mock.patch.object(pt_sweep.subprocess, 'run', run):
                row, _ = pt_sweep.run_map(a, 342, 'MAP', (1, 1), 0)
        self.assertEqual(calls[0][calls[0].index('-t') + 1], '0')                    # no wall cap
        self.assertEqual(calls[0][calls[0].index('--frames') + 1], '10800')           # END (4800) + 6000
        self.assertEqual(row[2], 'no-field')                                           # no warp.txt: not a verdict of the wall clock

    def test_summary(self):
        with tempfile.TemporaryDirectory() as t:
            for m in (3, 9):
                os.makedirs(os.path.join(t, 'maps', str(m)))
                open(os.path.join(t, 'maps', str(m), 'bt.txt'), 'w').write(f'crash line\nbacktrace {m}\n')
            rows = [[1, 'A', 'ok', '4800', '', '', '1', '1', '40'], [3, 'B', 'crash', '6900', 'Foo', 'abc', '3', '3', '40'],
                    [9, 'C', 'crash', '6900', 'Foo', 'abc', '9', '9', '40'], [10, 'D', 'no-spawn', '', '', '', '', '', '0'],
                    [11, 'E', 'heap', '70', 'heap overrun after Bar', 'x', '11', '11', '40']]
            open(os.path.join(t, 'sweep.tsv'), 'w').write('\t'.join(pt_sweep.HEAD) + '\n' +
                                                          ''.join('\t'.join(map(str, r)) + '\n' for r in rows))
            maps = {i: (f'MAP_{i}', None) for i in range(12)}
            pt_sweep.summarize(argparse.Namespace(out=t, save='pt-x', frames=3000), maps)
            groups = [l.split('\t') for l in open(os.path.join(t, 'crash_groups.tsv')).read().splitlines()]
            self.assertEqual(groups[1][:3], ['Foo', '3 MAP_3', '2'])       # the biggest group first, with its first map
            self.assertEqual(groups[1][3], 'backtrace 3')
            self.assertEqual(groups[2][0], 'heap overrun after Bar')
            summary = open(os.path.join(t, 'summary.txt')).read()
            self.assertIn('5 maps', summary)
            self.assertIn('crash 2', summary)


if __name__ == '__main__':
    unittest.main()
