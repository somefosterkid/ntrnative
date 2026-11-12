import struct, sys
d = open(sys.argv[1] + '/probes.bin', 'rb').read()
n = struct.unpack_from('<i', d, 0)[0]
o = 4
want = int(sys.argv[2])
ch = int(sys.argv[3]) if len(sys.argv) > 3 else -1
for i in range(n):
    pid, nv, x0, y0, w, h, _ = struct.unpack_from('<7i', d, o)
    o += 28
    vs = [struct.unpack_from('<7i', d, o + 28 * k) for k in range(3)]
    o += 84
    if pid == want:
        print(pid, x0, y0, w, h, vs)
        for y in range(h):
            row = []
            for x in range(w):
                v = struct.unpack_from('<I', d, o + (y * w + x) * 4)[0]
                if not v:
                    row.append('  .' if ch >= 0 else '  .  .  .')
                elif ch >= 0:
                    row.append('%3d' % ((v >> (8 * ch)) & 63))
                else:
                    row.append('%2d,%2d,%2d' % tuple((v >> s) & 63 for s in (0, 8, 16)))
            print('%3d' % (y0 + y), ' '.join(row))
    o += w * h * 4
