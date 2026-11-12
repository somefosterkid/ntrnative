#!/usr/bin/env python3
"""metric.py CHARLOG [-v]: the jt81 metric over BWPORT_INTERP_CHARLOG.
Per picture that is an in-between (P line blend=1, pairing valid), for every small list (n == 4, 12..40 px: the sprites: the player,
NPCs, shadows, trees) that is paired (m=1) the line holds the shown box and the old and key keyframes' boxes:
  step   = the list's texcoords differ between the keyframes by >= 8 texels (a walk-animation cell change)
  mix    = step and the shown texel rectangle is neither the old keyframe's nor the key's (a strip of two cells)
  off    = the shown centre is outside [old centre, key centre] by > 0.6 px on an axis (not between its two keyframes)
  fb     = small lists of an in-between that are NOT paired (m=0): drawn as in the newest keyframe, the others blended
Prints totals; -v prints the first offenders."""
import sys

fn = sys.argv[1]
verbose = '-v' in sys.argv
pics = blendpics = 0
lists = steps = mixes = offs = fbs = 0
fbpics = set()
shown = []
cur = None
TOL = 0.07


def near(a, b):
    return all(abs(x - y) <= TOL for x, y in zip(a, b))


for l in open(fn):
    t = l.split()
    if not t:
        continue
    if t[0] == 'P':
        if len(t) < 8:
            continue
        pics += 1
        cur = t
        isbl = t[6] == '1' and t[7] == '1'
        if isbl:
            blendpics += 1
        continue
    if cur is None or not (cur[6] == '1' and cur[7] == '1') or len(t) < 15:
        continue
    n = int(t[2])
    if n != 4:
        continue
    m = int(t[6])
    v = list(map(float, t[7:]))
    sh = v[0:8]
    if not (12 <= sh[2] - sh[0] <= 40 and 12 <= sh[3] - sh[1] <= 40):
        continue
    if m == 0:
        fbs += 1
        fbpics.add(cur[1])
        continue
    if len(v) < 24:
        continue
    ob, kb = v[8:16], v[16:24]
    lists += 1
    stepped = max(abs(ob[4] - kb[4]), abs(ob[5] - kb[5]), abs(ob[6] - kb[6]), abs(ob[7] - kb[7])) >= 8
    if stepped:
        steps += 1
        if not (near(sh[4:8], ob[4:8]) or near(sh[4:8], kb[4:8])):
            mixes += 1
            if verbose and len(shown) < 8:
                shown.append('mix pic %s f=%s list %s tex shown %s old %s key %s' % (cur[1], cur[5], t[1], sh[4:8], ob[4:8], kb[4:8]))
    cx, cy = (sh[0] + sh[2]) / 2, (sh[1] + sh[3]) / 2
    ox, oy = (ob[0] + ob[2]) / 2, (ob[1] + ob[3]) / 2
    kx, ky = (kb[0] + kb[2]) / 2, (kb[1] + kb[3]) / 2
    if cx < min(ox, kx) - 0.6 or cx > max(ox, kx) + 0.6 or cy < min(oy, ky) - 0.6 or cy > max(oy, ky) + 0.6:
        offs += 1
        if verbose and len(shown) < 16:
            shown.append('off pic %s f=%s list %s c=(%.1f,%.1f) old (%.1f,%.1f) key (%.1f,%.1f)' % (cur[1], cur[5], t[1], cx, cy, ox, oy, kx, ky))
print('pictures %d, in-betweens %d; sprite lists in in-betweens: paired %d, cell steps %d, MIX (strip of two cells) %d, off-path %d, unpaired (fallback) %d in %d pictures'
      % (pics, blendpics, lists, steps, mixes, offs, fbs, len(fbpics)))
for s in shown:
    print('  ', s)
