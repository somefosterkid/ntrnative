import struct, sys, collections
d = open(sys.argv[1] + '/probes.bin', 'rb').read()
n = struct.unpack_from('<i', d, 0)[0]
o = 4
rows = {}
nonuni = 0
tot = 0
for i in range(n):
    pid, nv, x0, y0, w, h, _ = struct.unpack_from('<7i', d, o)
    o += 28
    vs = [struct.unpack_from('<7i', d, o + 28 * k) for k in range(3)]
    o += 84
    # apex = vertex with smallest sy
    ap = min(vs, key=lambda v: v[1])
    bt = max(vs, key=lambda v: v[1])
    dy = bt[1] - ap[1]
    for ch in range(3):
        c0 = ap[4 + ch]; c1 = bt[4 + ch]
        seq = []
        for y in range(h):
            vals = set()
            for x in range(w):
                v = struct.unpack_from('<I', d, o + (y * w + x) * 4)[0]
                if v:
                    vals.add((v >> (8 * ch)) & 63)
            tot += 1
            if len(vals) > 1:
                nonuni += 1
            seq.append(sorted(vals))
        rows[(pid, ch)] = (dy, c0, c1, ap[1] - 0, y0, seq)
    o += w * h * 4
print('rows', tot, 'non-uniform rows', nonuni)
want = sys.argv[2:]
k = 0
for (pid, ch), (dy, c0, c1, ay, y0, seq) in rows.items():
    if want and str(pid) not in want:
        continue
    print(pid, ch, 'dy', dy, 'c5', c0, c1, 'y0-ay', y0 - ay, [s[0] if len(s) == 1 else s for s in seq][:60])
    k += 1
    if k > 12 and not want:
        break
