#!/usr/bin/env python3
"""Tests of tools/int_sweep.py (the Platinum interaction sweep, tools/pt_int_sweep.py): the hook's log parsed, the script ids an event
began, the resumable rows, the map table and the summary files of a made-up out dir (no game run)."""
import argparse
import json
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import int_sweep  # noqa: E402
import pt_sweep  # noqa: E402

LOG = """count 4 2 1 1
ev 0 0 0 10 20 3001 5 0 0
ev 1 0 1 12 20 3002 6 4096 1
ev 2 1 2 14 21 7001 4 0 0
ev 3 2 0 16 22 8001 16384 3 1002
begin 0 1700
script 0 1699 9000
at 0 1750 10 21 0 0
press 0 1790
script 0 1792 3001
script 0 1800 3101
end 0 1900 ok scripts=2 map=411
begin 1 1900
show 1 flag 4096
at 1 1950 12 21 0 0
script 1 1960 3002
battle 1 2100 menu
end 1 2100 battle scripts=1 map=-1
exit 2100 battle
"""


class Tests(unittest.TestCase):
    def test_parse_and_ran(self):
        with tempfile.TemporaryDirectory() as t:
            p = os.path.join(t, 'int_0.txt')
            open(p, 'w').write(LOG)
            r = int_sweep.parse_int_log(p)
            self.assertEqual(r['n'], 4)
            self.assertEqual(r['events'][3], (2, 0, 16, 22, 8001, 16384, 3, 1002))
            self.assertEqual(r['begun'], [0, 1])
            self.assertEqual(r['ends'][0], (1900, 'ok', 'scripts=2', 'map=411'))
            self.assertEqual(r['exit'], (2100, 'battle'))
            # event 0: the arrival script before its press is not its own; event 1 has no press: from its landing
            self.assertEqual(int_sweep.ran_of(r, 0), ['3001', '3101'])
            self.assertEqual(int_sweep.ran_of(r, 1), ['3002'])
            self.assertEqual(int_sweep.parse_int_log(os.path.join(t, 'none.txt'))['n'], None)

    def test_kind_names(self):
        self.assertEqual(int_sweep.kind_name(0, 4), 'npc')
        self.assertEqual(int_sweep.kind_name(1, 2), 'hidden')
        self.assertEqual(int_sweep.kind_name(1, 1), 'sign')
        self.assertEqual(int_sweep.kind_name(2, 0), 'coord')

    def test_rows_resume(self):
        with tempfile.TemporaryDirectory() as t:
            path = os.path.join(t, 'sweep.tsv')
            rows = int_sweep.Rows(path)
            self.assertEqual(rows.map_state(5), (None, set(), False))
            rows.add([5, 'MAP_X', -1, '', '', '', '', 'map', '', '', '', '', '3', 'n=2'])
            rows.add([5, 'MAP_X', 0, 'npc', 1, 2, 3001, 'ok', '3001', 1900, '', '', '3', ''])
            self.assertEqual(rows.map_state(5), (2, {0}, False))
            rows.add([5, 'MAP_X', 1, 'bg', 1, 3, 7001, 'no-script', '', 2000, '', '', '3', ''])
            again = int_sweep.Rows(path)   # read back
            self.assertEqual(again.map_state(5), (2, {0, 1}, True))
            rows.add([6, 'MAP_Y', -1, '', '', '', '', 'map-crash', '', '', 'F', 'id', '3', ''])
            self.assertTrue(rows.map_state(6)[2])
            rows.drop_maps({5})
            self.assertEqual(rows.map_state(5), (None, set(), False))

    def test_game_maps(self):
        with tempfile.TemporaryDirectory() as t:
            os.makedirs(os.path.join(t, 'generated'))
            open(os.path.join(t, 'generated/map_headers.txt'), 'w').write('MAP_HEADER_A\nMAP_HEADER_B\nMAP_HEADER_COUNT\n')
            os.makedirs(os.path.join(t, 'res/field/maps/b'))
            json.dump({'object_events': [{'x': 5, 'z': 7}]}, open(os.path.join(t, 'res/field/maps/b/events.json'), 'w'))
            saved = pt_sweep.TREE
            try:
                pt_sweep.TREE = t
                pt_sweep.read_maps.__defaults__ = (t,)
                g = int_sweep.Game('platinum')
                self.assertEqual(g.maps(), {0: ('MAP_HEADER_A', None), 1: ('MAP_HEADER_B', (5, 8))})
            finally:
                pt_sweep.read_maps.__defaults__ = (saved,)
                pt_sweep.TREE = saved
            self.assertEqual((g.tag, g.save, g.name), ('PT', 'pt-ver69', 'platinum'))
            self.assertIsNone(g.skip(2, 'MAP_HEADER_UNION_ROOM'))

    def test_summary(self):
        with tempfile.TemporaryDirectory() as t:
            rows = int_sweep.Rows(os.path.join(t, 'sweep.tsv'))
            os.makedirs(os.path.join(t, 'maps/7'))
            open(os.path.join(t, 'maps/7/bt_1.txt'), 'w').write('crash\nFoo (a.c:1) <- Bar\n')
            rows.add([7, 'MAP_Z', -1, '', '', '', '', 'map', '', '', '', '', '1', 'n=3'])
            rows.add([7, 'MAP_Z', 0, 'npc', 1, 1, 100, 'ok', '100', 10, '', '', '1', ''])
            rows.add([7, 'MAP_Z', 1, 'sign', 2, 2, 7001, 'crash', '7001', 20, 'Foo', 'abc', '1', ''])
            rows.add([7, 'MAP_Z', 2, 'coord', 3, 3, 8001, 'crash', '', 30, 'Foo', 'abc', '1', ''])
            a = argparse.Namespace(out=t, save='pt-ver69', budget=1500, heapcheck='1')
            int_sweep.summarize(a, 'platinum')
            summary = open(os.path.join(t, 'summary.txt')).read()
            self.assertIn('1 maps, 3 event rows', summary)
            self.assertIn('crash 2', summary)
            groups = open(os.path.join(t, 'crash_groups.tsv')).read().splitlines()
            self.assertEqual(len(groups), 2)
            self.assertTrue(groups[1].startswith('Foo\tcrash\t2\t1\t\t'))
            self.assertIn('Foo (a.c:1)', groups[1])


if __name__ == '__main__':
    unittest.main()
