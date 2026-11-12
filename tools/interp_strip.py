#!/usr/bin/env python3
"""strip.py OUT.png DIR x0 y0 w h scale pic... : crop (x0,y0,w,h) from DIR/NNNNNN.png of each pic, scale, concat horizontally with a 2px gap"""
import sys, zlib, struct
def readpng(p):
    d = open(p, 'rb').read()
    pos = 8; idat = b''
    while pos < len(d):
        n, = struct.unpack('>I', d[pos:pos+4]); t = d[pos+4:pos+8]; c = d[pos+8:pos+8+n]; pos += 12 + n
        if t == b'IHDR': w, h, bd, ct = struct.unpack('>IIBB', c[:10])
        elif t == b'IDAT': idat += c
    bpp = 4 if ct == 6 else 3
    raw = zlib.decompress(idat); stride = w * bpp
    rows = []; prev = bytearray(stride); o = 0
    for y in range(h):
        f = raw[o]; o += 1
        line = bytearray(raw[o:o+stride]); o += stride
        for i in range(stride):
            a = line[i-bpp] if i >= bpp else 0; b = prev[i]; c2 = prev[i-bpp] if i >= bpp else 0
            if f == 1: line[i] = (line[i] + a) & 255
            elif f == 2: line[i] = (line[i] + b) & 255
            elif f == 3: line[i] = (line[i] + (a + b)//2) & 255
            elif f == 4:
                pa = abs(b - c2); pb = abs(a - c2); pc = abs(a + b - 2*c2)
                pr = a if pa <= pb and pa <= pc else (b if pb <= pc else c2)
                line[i] = (line[i] + pr) & 255
        rows.append(line); prev = line
    return w, h, bpp, rows
def writepng(p, w, h, rows):
    raw = b''.join(b'\x00' + bytes(r) for r in rows)
    def ch(t, c): return struct.pack('>I', len(c)) + t + c + struct.pack('>I', zlib.crc32(t + c) & 0xffffffff)
    open(p, 'wb').write(b'\x89PNG\r\n\x1a\n' + ch(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0)) + ch(b'IDAT', zlib.compress(raw)) + ch(b'IEND', b''))
out, dr = sys.argv[1], sys.argv[2]
x0, y0, cw, ch_, sc = map(int, sys.argv[3:8])
pics = sys.argv[8:]
tiles = []
for pn in pics:
    w, h, bpp, rows = readpng('%s/%06d.png' % (dr, int(pn)))
    t = []
    for y in range(y0, y0 + ch_):
        r = bytearray()
        for x in range(x0, x0 + cw):
            px = rows[y][x*bpp:x*bpp+3]
            r += px * sc
        for _ in range(sc): t.append(r)
    tiles.append(t)
rows = []
gap = bytes([255, 0, 255]) * 2
for y in range(ch_ * sc):
    r = bytearray()
    for t in tiles: r += t[y] + gap
    rows.append(r)
writepng(out, len(tiles) * (cw * sc + 2), ch_ * sc, rows)
