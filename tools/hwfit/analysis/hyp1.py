import sys
from fractions import Fraction as Fr
rows = [tuple(map(int, l.split())) for l in open(sys.argv[1])]
def V(c, z):
    return Fr(0) if c == 0 and z == 0 else Fr(16 * c + 15, 8) if c else Fr(z, 8)
for z in (0, 1, 7):
    bad = 0
    tot = 0
    badl = []
    for dy, a, b, t, v in rows:
        if t == 0: continue
        A, B = V(a, z), V(b, z)
        ex = A + (B - A) * Fr(t, dy)
        tot += 1
        if int(ex // 1) != v:
            bad += 1
            badl.append((dy, a, b, t, v, float(ex)))
    print('c=0 value %d/8: %d of %d wrong' % (z, bad, tot), badl[:8])
