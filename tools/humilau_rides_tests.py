#!/usr/bin/env python3
"""Check that G:ride's Humilau Gym table (kHumilau in port/include/port_route_map_game.h) is pokebw2's sRides (x, z, dir in order;
the lane lengths stay the game's): python3 tools/humilau_rides_tests.py   (prints "ok" per test, exits 1 on a failure)"""
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DIRS = {"DIR_UP": 0, "DIR_DOWN": 1, "DIR_LEFT": 2, "DIR_RIGHT": 3}


def decomp_rides():
    src = open(os.path.join(ROOT, "third_party/pokebw2/src/ov101/gym_humilau.c")).read()
    body = re.search(r"sRides\[RIDE_COUNT\] = \{(.*?)\n\};", src, re.S).group(1)
    return [(int(x), int(z), DIRS[d]) for x, z, d, _ in re.findall(r"\{\s*(\d+),\s*(\d+),\s*(DIR_\w+),\s*(\d+)\s*\}", body)]


def adapter_rides():
    hdr = open(os.path.join(ROOT, "port/include/port_route_map_game.h")).read()
    body = re.search(r"kHumilau\[37\]\[3\] = \{(.*?)\n    \};", hdr, re.S).group(1)
    return [tuple(map(int, t)) for t in re.findall(r"\{(\d+), (\d+), (\d+)\}", body)]


def test_table_is_srides():
    a, b = adapter_rides(), decomp_rides()
    assert len(a) == len(b) == 37, (len(a), len(b))
    assert a == b


if __name__ == "__main__":
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            fn()
            print("ok", name)
