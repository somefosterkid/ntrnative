import sys
from fractions import Fraction as Fr
def c6(c): return c*2 + (1 if c > 0 else 0)
rows = [tuple(map(int, l.split())) for l in open(sys.argv[1])]
off = Fr(14, 16)
miss = []
tot = 0
for dy, a, b, t, v in rows:
    if t == 0 or a == b: continue
    A, B = c6(a), c6(b)
    ex = A + Fr((B - A) * t, dy)
    got = int((ex + off) // 1)
    tot += 1
    if got != v:
        miss.append((dy, a, b, t, v, got, float(ex), float(ex + off)))
print(tot, len(miss))
import collections
c = collections.Counter((m[4] - m[5]) for m in miss)
print(c)
# by fractional part of exact
fr = collections.Counter(int((m[6] % 1) * 32) for m in miss)
print(sorted(fr.items()))
by = collections.Counter(m[0] for m in miss)
print(sorted(by.items()))
for m in miss[:int(sys.argv[2])]:
    print(m)
