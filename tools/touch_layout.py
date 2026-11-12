#!/usr/bin/env python3
"""touch_layout.py: a model of how libntr maps the window and the layout settings to the touch screen's rectangle and a click
to a DS touch point (sim_main.cpp's main loop: the 'Scale the viewport' block and 'The touch screen's rectangle', both in
float arithmetic; the click -> (tx, ty) line). The game does not run it: tools/touch_test.py compares it with the exe's own
BWPORT_TOUCHLOG 'layout' line in its real-game configs, so a libntr change that moves a rectangle fails there, and
tools/touch_layout_tests.py runs every layout setting through it in a second."""
import math
import struct


def f32(x):
    return struct.unpack('f', struct.pack('f', x))[0]


LAYOUT_WH = {'vertical': (256.0, 384.0), 'horizontal': (512.0, 192.0), 'large': (768.0, 384.0)}


def view(layout, integer, W, H, bar_h, side_on):
    """(viewX, viewTop, viewW, viewH, factor, panelPxX, panelPxW): the screens' area (viewW without the side panel)"""
    lw, lh = LAYOUT_WH[layout]
    pw = f32(lh * f32(0.625)) if side_on else 0.0
    total = f32(lw + pw)
    area = max(H - bar_h, 1)
    factor = min(f32(W / total), f32(area / lh))
    if integer and factor >= 1.0:
        factor = float(math.floor(f32(factor + f32(0.0001))))
    vw = min(int(f32(factor * total) + 0.5), W)
    vh = min(int(f32(factor * lh) + 0.5), area)
    vx = (W - vw) // 2
    top = bar_h + (area - vh) // 2
    panel_x, panel_w = vx + vw, 0
    if pw > 0.0:
        scr_w = min(int(f32(factor * lw) + 0.5), vw)
        panel_x = vx + scr_w
        panel_w = vw - scr_w
        vw = scr_w
    return vx, top, vw, vh, factor, panel_x, panel_w


def touch_rect(layout, swap, integer, W, H, bar_h, side_on):
    """(x0, y0, x1, y1) in window coordinates, y down"""
    vx, vy, vw, vh = [float(v) for v in view(layout, integer, W, H, bar_h, side_on)[:4]]
    half_h, half_w = f32(vh * 0.5), f32(vw * 0.5)
    if layout == 'vertical':
        return (vx, vy if swap else f32(vy + half_h), f32(vx + vw), f32(vy + half_h) if swap else f32(vy + vh))
    if layout == 'horizontal':
        return (vx if swap else f32(vx + half_w), vy, f32(vx + half_w) if swap else f32(vx + vw), f32(vy + vh))
    third = f32(vw * f32(2.0 / 3.0))
    if swap:
        return (vx, vy, f32(vx + third), f32(vy + vh))
    return (f32(vx + third), f32(vy + half_h), f32(vx + vw), f32(vy + vh))


def click_point(rect, px, py):
    """the DS touch point (tx, ty) for a click at window pixel (px, py) (its centre), None outside the touch screen"""
    x0, y0, x1, y1 = rect
    mx, my = px + 0.5, py + 0.5
    if not (x0 <= mx < x1 and y0 <= my < y1):
        return None
    return min(255, int((mx - x0) * 256 / (x1 - x0))), min(191, int((my - y0) * 192 / (y1 - y0)))
