#!/usr/bin/env python3
"""tests for tools/hg_sweep.py (no game run): the walk's keys, the backtrace parse, the map table"""
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import hg_sweep  # noqa: E402

LOG = """...
== port_run: stop
#0  0x0000555556f0e262 in MapObject_SetFlagsBits (object=0x1000, bits=1) at ../../pokeheartgold/src/map_object.c:1005
#1  0x0000555556f0ed57 in MapObject_SetVisible (object=0x1000, set=1) at ../../pokeheartgold/src/map_object.c:1475
#2  0x00007ffff7a4 in start_thread () from /lib/libc.so.6

== port_run: threads
#0  0x1 in other () at x.c:1
"""


class Tests(unittest.TestCase):
    def test_walk_deterministic(self):
        a, b = hg_sweep.walk_keys(5, 3000), hg_sweep.walk_keys(5, 3000)
        self.assertEqual(a, b)
        self.assertNotEqual(a, hg_sweep.walk_keys(6, 3000))
        first = a.split()[0]
        self.assertTrue(first.endswith(f'@f{hg_sweep.WALK_FROM}'), first)

    def test_crash_info(self):
        top, text, bid = hg_sweep.crash_info(LOG)
        self.assertEqual(top, 'MapObject_SetFlagsBits')
        self.assertIn('MapObject_SetVisible (map_object.c:1475)', text)
        self.assertNotIn('other', text)
        self.assertEqual(len(bid), 8)

    def test_parse_maps(self):
        maps = {i: ('MAP_X', '', None) for i in range(10)}
        self.assertEqual(hg_sweep.parse_maps('2-4,7,apps', maps), [2, 3, 4, 7, -1, -2, -3, -4, -5, -6, -7])

    def test_apps_keys(self):
        keys, frames = hg_sweep.apps_keys(100)
        self.assertEqual(keys.split().count('x@f' + str(hg_sweep.WALK_FROM)), 1)
        one, _ = hg_sweep.apps_keys(100, [2])
        self.assertEqual(one.split().count('down@f' + str(hg_sweep.WALK_FROM + 60)), 1)
        self.assertEqual(sum(k.startswith('x@') for k in one.split()), 1)
        self.assertGreater(frames, 100)

    @unittest.skipUnless(os.path.exists(os.path.join(hg_sweep.TREE, 'include/constants/maps.h')) and os.path.exists(
        os.path.join(hg_sweep.REPO, 'orig/baserom_hg_us.nds')), 'no generated tree / HeartGold ROM')
    def test_map_table(self):
        maps = hg_sweep.read_maps()
        self.assertEqual(maps[60][0], 'MAP_NEW_BARK')
        self.assertEqual(hg_sweep.spawn(maps[60][2]), (684, 393))


if __name__ == '__main__':
    unittest.main()
