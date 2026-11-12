#!/usr/bin/env python3
"""Tests for touch_layout.py: every layout setting (layout, swap, integer scale, side panel, bar, many window sizes) through the
model: the touch screen is inside the window, 4:3, a whole multiple under integer scale, on the right side of the layout, and
the click mapping gives (0,0) .. (255,191) at its corners and nothing outside it. Does in a second what the 222 real-game
configs of touch_test.py --full check about the layout (the full run stays the gate's nightly / FULL=1 check)."""
import math
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from touch_layout import touch_rect, click_point, view

SIZES = [(512, 768), (700, 900), (1100, 480), (420, 330), (256, 384), (300, 200), (1280, 1024), (1700, 720), (2600, 1500), (3, 3)]
BARS = [0, 19, 23]


def all_cases():
    for layout in ('vertical', 'horizontal', 'large'):
        for swap in (False, True):
            for integer in (False, True):
                for side in (False, True):
                    for bar in BARS:
                        for (W, H) in SIZES:
                            yield layout, swap, integer, side, bar, W, H


def pix(v):
    return int(math.ceil(v - 0.5))


def t_geometry():
    n = 0
    for layout, swap, integer, side, bar, W, H in all_cases():
        tag = (layout, swap, integer, side, bar, W, H)
        x0, y0, x1, y1 = r = touch_rect(layout, swap, integer, W, H, bar, side)
        vx, top, vw, vh, factor, px, pw = view(layout, integer, W, H, bar, side)
        if W < 8 or H < 8:
            continue
        assert 0 <= x0 < x1 <= W + 1e-3 and bar - 1e-3 <= y0 < y1 <= H + 1e-3, (tag, r)
        assert vx >= 0 and vx + vw + pw <= W + 1, tag
        if W < 8 or H < 8:
            continue
        tw, th = x1 - x0, y1 - y0
        assert abs(tw / th - 256 / 192) < 0.02 + 1.5 / min(tw, th), (tag, r)
        if integer and factor >= 1.0:
            assert abs(factor - round(factor)) < 1e-6 and abs(tw / 256 - round(tw / 256)) < 0.51, (tag, tw)
        if layout == 'vertical':
            assert (y0 < top + vh / 2 - 0.01) == swap or vh < 2, tag
        if layout == 'horizontal':
            assert (x0 < vx + vw / 2 - 0.01) == swap or vw < 2, tag
        if layout == 'large':
            assert (tw > vw / 2) == swap, tag
        n += 1
    assert n > 500, n


def t_mapping():
    for layout, swap, integer, side, bar, W, H in all_cases():
        if W < 200 or H < 200:
            continue
        tag = (layout, swap, integer, side, bar, W, H)
        r = touch_rect(layout, swap, integer, W, H, bar, side)
        if r[2] - r[0] < 200:
            continue
        L, T, R, B = pix(r[0]), pix(r[1]), pix(r[2]) - 1, pix(r[3]) - 1
        c0, c1 = click_point(r, L, T), click_point(r, R, B)
        assert c0[0] <= 1 and c0[1] <= 1, (tag, c0)
        assert c1[0] >= 253 and c1[1] >= 188, (tag, c1)
        cy, cx = (T + B) // 2, (L + R) // 2
        assert click_point(r, L - 1, cy) is None and click_point(r, R + 1, cy) is None, tag
        assert click_point(r, cx, T - 1) is None and click_point(r, cx, B + 1) is None, tag
        prev = -1
        for px in range(L, R + 1):
            tx = click_point(r, px, T)[0]
            assert 0 <= tx - prev <= 2 + 256 // max(R - L, 1), (tag, px, tx, prev)
            prev = tx
        c = click_point(r, cx, cy)
        assert abs(c[0] - 128) <= 2 and abs(c[1] - 96) <= 2, (tag, c)


def t_known():
    # 700x900 vertical, no side, no bar: factor min(700/256, 900/384) = 2.34375 -> view 600x900 at x 50, the touch screen its lower half
    assert touch_rect('vertical', False, False, 700, 900, 0, False) == (50.0, 450.0, 650.0, 900.0)
    # integer: factor 2 -> 512x768 centred (x 94, top 66); swapped: the upper half
    assert touch_rect('vertical', True, True, 700, 900, 0, False) == (94.0, 66.0, 606.0, 450.0)
    # a 20 px bar: the area under it is 880 high
    assert touch_rect('vertical', False, False, 700, 900, 20, False)[3] == 900.0


if __name__ == '__main__':
    bad = 0
    for n, f in sorted(globals().items()):
        if n.startswith('t_'):
            try:
                f()
                print('ok', n)
            except AssertionError as e:
                bad = 1
                print('FAIL', n, repr(e)[:300])
    sys.exit(bad)
