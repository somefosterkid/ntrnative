# png_crop.py OUT.png X Y W H ZOOM IN1.png IN2.png ... : crops the same box from each PNG, zooms (nearest) and tiles them in a row
import sys, zlib, struct
def rd(p):
    d = open(p, 'rb').read(); i = 8; idat = b''
    while i < len(d):
        n, t = struct.unpack('>I4s', d[i:i+8]); c = d[i+8:i+8+n]; i += 12 + n
        if t == b'IHDR': w, h, bd, ct = struct.unpack('>IIBB', c[:10])
        if t == b'IDAT': idat += c
    raw = zlib.decompress(idat); bpp = {2: 3, 6: 4}[ct]; st = w * bpp; rows = []; prev = bytearray(st); o = 0
    for y in range(h):
        f = raw[o]; line = bytearray(raw[o+1:o+1+st]); o += 1 + st
        for x in range(st):
            a = line[x-bpp] if x >= bpp else 0; b = prev[x]; c2 = prev[x-bpp] if x >= bpp else 0
            if f == 1: line[x] = (line[x] + a) & 255
            elif f == 2: line[x] = (line[x] + b) & 255
            elif f == 3: line[x] = (line[x] + (a + b) // 2) & 255
            elif f == 4:
                pa, pb, pc = abs(b - c2), abs(a - c2), abs(a + b - 2 * c2)
                line[x] = (line[x] + (a if pa <= pb and pa <= pc else b if pb <= pc else c2)) & 255
        rows.append(line); prev = line
    return w, h, bpp, rows
out, X, Y, W, H, Z = sys.argv[1], *map(int, sys.argv[2:7]); ins = sys.argv[7:]
tiles = []
for p in ins:
    w, h, bpp, rows = rd(p)
    t = []
    for y in range(Y, Y + H):
        r = bytearray()
        for x in range(X, X + W): r += rows[y][x*bpp:x*bpp+3] * Z
        t += [bytes(r)] * Z
    tiles.append(t)
gap = 4; TW = W * Z; img = []
for y in range(H * Z):
    img.append(b''.join(t[y] + b'\xff\x00\xff' * gap for t in tiles))
raw = b''.join(b'\0' + r for r in img)
def ch(t, d): c = struct.pack('>I', len(d)) + t + d; return c + struct.pack('>I', zlib.crc32(t + d))
open(out, 'wb').write(b'\x89PNG\r\n\x1a\n' + ch(b'IHDR', struct.pack('>IIBBBBB', len(img[0]) // 3, len(img), 8, 2, 0, 0, 0)) + ch(b'IDAT', zlib.compress(raw)) + ch(b'IEND', b''))
