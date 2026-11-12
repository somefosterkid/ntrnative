#!/usr/bin/env python3
"""walk.py START STEP MOVES...: print a --keys list of taps, one per tile, from frame START, STEP frames apart.
A move is DIR[N] (up3 = three tiles up; a direction change adds one tap that only turns), a KEY (a, b, x, start: one
press), or wN (wait N frames). Example: walk.py 3000 40 up3 right5 a w300 b"""
import re
import sys

start, step = int(sys.argv[1]), int(sys.argv[2])
import os
HOLD = os.environ.get('HOLD', 'f6')      # a direction tap's hold (BWPORT_KEYS: fN frames)
f, facing, out = start, None, []
for m in sys.argv[3:]:
    g = re.fullmatch(r'(up|down|left|right)(\d*)(:f\d+)?', m)
    if g:
        # DIR[N][:fH]: N tiles; :fH holds each tap H frames (a stair or door warp wants the key held)
        d, n = g.group(1), int(g.group(2) or 1)
        hold = g.group(3)[1:] if g.group(3) else HOLD
        if d != facing:
            out.append(f'{d}*{HOLD}@f{f}')
            f += step
            facing = d
        for _ in range(n):
            out.append(f'{d}*{hold}@f{f}')
            f += step
    elif re.fullmatch(r'[a-z]+\*\d+(/\d+)?', m):
        # KEY*N[/GAP]: N presses GAP frames apart (default 90): a*40 for the texts
        k, rest = m.split('*')
        n, gap = (rest.split('/') + ['90'])[:2]
        for _ in range(int(n)):
            out.append(f'{k}@f{f}')
            f += int(gap)
    elif m.startswith('w') and m[1:].isdigit():
        f += int(m[1:])
    else:
        out.append(f'{m}@f{f}')
        f += step * 3
print(' '.join(out))
print(f'last frame {f}', file=sys.stderr)
