import struct, sys
from fractions import Fraction as Fr
d = open(sys.argv[1] + '/probes.bin', 'rb').read()
n = struct.unpack_from('<i', d, 0)[0]
o = 4
want = int(sys.argv[2]); ch = int(sys.argv[3])
def V(c): return 16 * c + 15 if c else 0
for i in range(n):
    pid, nv, x0, y0, w, h, _ = struct.unpack_from('<7i', d, o)
    o += 28
    vs = [struct.unpack_from('<7i', d, o + 28 * k) for k in range(3)]
    o += 84
    if pid == want:
        vs.sort(key=lambda v: (v[1], v[0]))
        T, M, B = vs
        print('T', T[:2], T[4+ch], 'M', M[:2], M[4+ch], 'B', B[:2], B[4+ch])
        for yy in range(h):
            y = y0 + yy
            px = [struct.unpack_from('<I', d, o + (yy * w + x) * 4)[0] for x in range(w)]
            xs = [x for x in range(w) if px[x]]
            if not xs: continue
            xl, xr = xs[0] + x0, xs[-1] + x0
            def ev(a, b):
                if b[1] == a[1]: return None
                num = (y - a[1])
                val = Fr(V(a[4+ch]) * (b[1] - a[1]) + (V(b[4+ch]) - V(a[4+ch])) * num, b[1] - a[1])
                return val // 1, a[0] + Fr((b[0] - a[0]) * num, b[1] - a[1])
            long_ = ev(T, B)
            short = ev(T, M) if y < M[1] else ev(M, B)
            if long_ is None or short is None: continue
            (la, lx), (sa, sx) = long_, short
            if lx <= sx: L, R = la, sa; xL, xR = lx, sx
            else: L, R = sa, la; xL, xR = sx, lx
            vals = [(px[x] >> (8 * ch)) & 63 for x in xs]
            print('y%3d xl %3d xr %3d xL %.2f xR %.2f  L %6.2f R %6.2f  obs %s' % (y, xl, xr, float(xL), float(xR), L / 8, R / 8, vals))
    o += w * h * 4
