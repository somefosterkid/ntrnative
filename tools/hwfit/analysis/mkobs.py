import struct, sys
d = open(sys.argv[1] + '/probes.bin', 'rb').read()
n = struct.unpack_from('<i', d, 0)[0]
o = 4
obs = {}
conflict = 0
for i in range(n):
    pid, nv, x0, y0, w, h, _ = struct.unpack_from('<7i', d, o)
    o += 28
    vs = [struct.unpack_from('<7i', d, o + 28 * k) for k in range(3)]
    o += 84
    ap = min(vs, key=lambda v: v[1])
    others = [v for v in vs if v is not ap]
    dy = others[0][1] - ap[1]
    steep = all(abs(v[0] - ap[0]) < dy for v in others)
    if steep and others[0][1] == others[1][1]:
        for ch in range(3):
            for y in range(h):
                vals = set()
                for x in range(w):
                    v = struct.unpack_from('<I', d, o + (y * w + x) * 4)[0]
                    if v:
                        vals.add((v >> (8 * ch)) & 63)
                if len(vals) == 1:
                    key = (dy, ap[4 + ch], others[0][4 + ch], y0 + y - ap[1])
                    val = vals.pop()
                    if key in obs and obs[key] != val:
                        conflict += 1
                    obs[key] = val
    o += w * h * 4
with open(sys.argv[2], 'w') as f:
    for k, v in sorted(obs.items()):
        f.write('%d %d %d %d %d\n' % (*k, v))
print(len(obs), 'obs', conflict, 'conflicts')
