import sys
from fractions import Fraction as Fr
rows = [tuple(map(int, l.split())) for l in open(sys.argv[1])]
def c6(c): return c*2 + (1 if c > 0 else 0)
def c6b(c): return c*2 + 1
def c6c(c): return c*2
for name, f in (('2c+(c>0)', c6), ('2c+1', c6b), ('2c', c6c)):
    print(name)
    for cls in ('a0', 'b0', 'a31', 'b31', 'mid_rise', 'mid_fall'):
        lo, hi = Fr(-99), Fr(99)
        cnt = 0
        for dy, a, b, t, v in rows:
            if t == 0 or a == b: continue
            k = ('a0' if a == 0 else 'a31' if a == 31 else None, 'b0' if b == 0 else 'b31' if b == 31 else None)
            if cls in ('a0', 'a31'):
                if k[0] != cls: continue
            elif cls in ('b0', 'b31'):
                if k[1] != cls: continue
            else:
                if k[0] or k[1]: continue
                if (cls == 'mid_rise') != (b > a): continue
            A, B = f(a), f(b)
            ex = A + Fr((B - A) * t, dy)
            lo = max(lo, v - ex)
            hi = min(hi, v + 1 - ex)
            cnt += 1
        print('  %-9s n=%5d offset in [%s, %s) %s' % (cls, cnt, float(lo), float(hi), 'OK' if lo < hi else 'EMPTY'))
