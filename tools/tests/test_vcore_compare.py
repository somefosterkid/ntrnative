import os, sys, struct, tempfile, unittest, zlib
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
import vcore_compare as vc

def solid(r, g, b, w=256, h=384):
    return vc.Image(w, h, bytearray(bytes((r, g, b)) * (w * h)))

def mkpng(w, h, ctype, idat, extra=b''):
    return (b'\x89PNG\r\n\x1a\n' + vc._chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, ctype, 0, 0, 0))
            + extra + vc._chunk(b'IDAT', zlib.compress(idat)) + vc._chunk(b'IEND', b''))

class T(unittest.TestCase):
    def test_png_roundtrip(self):
        im = vc.Image(5, 4, bytearray(range(60)))
        out = vc.read_png(vc.png_bytes(im))
        self.assertEqual((out.w, out.h, bytes(out.px)), (5, 4, bytes(im.px)))

    def test_png_filters(self):
        w, h, bpp = 3, 5, 3
        rows = [bytes((y*37 + x*11) & 255 for x in range(w*3)) for y in range(h)]
        raw = bytearray(); prev = bytes(w*3)
        for y, row in enumerate(rows):
            f = y % 5; raw.append(f)
            for i in range(w*3):
                l = row[i-bpp] if i >= bpp else 0; u = prev[i]; ul = prev[i-bpp] if i >= bpp else 0
                pred = [0, l, u, (l+u) >> 1, vc._paeth(l, u, ul)][f]
                raw.append((row[i] - pred) & 255)
            prev = row
        self.assertEqual(bytes(vc.read_png(mkpng(w, h, 2, bytes(raw))).px), b''.join(rows))

    def test_rgba_and_palette(self):
        png = mkpng(2, 1, 6, b'\x00\x01\x02\x03\xff\x04\x05\x06\x80')
        self.assertEqual(bytes(vc.read_png(png).px), bytes(range(1, 7)))
        png = mkpng(2, 1, 3, b'\x00\x01\x00', vc._chunk(b'PLTE', b'\x10\x20\x30\x40\x50\x60'))
        self.assertEqual(bytes(vc.read_png(png).px), b'\x40\x50\x60\x10\x20\x30')

    def test_identical(self):
        r = vc.compare_images(solid(10, 20, 30), solid(10, 20, 30))
        self.assertEqual([x.diff for x in r], [0, 0]); self.assertEqual(r[0].match, 100.0)

    def test_six_bit_reduction(self):
        r = vc.compare_images(solid(0x40, 0x80, 0xc0), solid(0x43, 0x82, 0xc1))
        self.assertEqual(r[0].diff, 0)
        r = vc.compare_images(solid(0x40, 0x80, 0xc0), solid(0x44, 0x80, 0xc0))
        self.assertEqual((r[0].diff, r[0].maxerr), (49152, 1))

    def test_diff_counts(self):
        a, b = solid(0, 0, 0), solid(0, 0, 0)
        for x in range(10):
            b.px[((192 + 3) * 256 + x) * 3] = 32
        d = vc.Image(256, 384, bytearray())
        r = vc.compare_images(a, b, d)
        self.assertEqual((r[0].diff, r[1].diff, r[1].maxerr), (0, 10, 8))
        self.assertAlmostEqual(r[1].match, 100.0 * (49152 - 10) / 49152)
        o = (195 * 256) * 3
        self.assertEqual(bytes(d.px[o:o+3]), b'\xff\x00\x00')
        self.assertEqual(bytes(d.px[0:3]), b'\x00\x00\x00')

    def test_dimmed(self):
        d = vc.Image(256, 384, bytearray())
        vc.compare_images(solid(200, 100, 40), solid(200, 100, 40), d)
        self.assertEqual(bytes(d.px[:3]), bytes((50, 25, 10)))

    def test_size_mismatch(self):
        with self.assertRaises(ValueError):
            vc.compare_images(solid(0, 0, 0), solid(0, 0, 0, 256, 192))

    def test_cli_and_corpus_mock(self):
        with tempfile.TemporaryDirectory() as t:
            a = os.path.join(t, 'a.png'); b = os.path.join(t, 'b.png')
            vc.write_png(a, solid(1, 2, 3)); vc.write_png(b, solid(200, 2, 3))
            self.assertEqual(vc.main(['compare', a, b, '--diff', os.path.join(t, 'd.png')]), 1)
            self.assertEqual(vc.read_png(os.path.join(t, 'd.png')).px[:3], bytearray(b'\xff\x00\x00'))
            self.assertEqual(vc.main(['compare', a, a]), 0)
            cdir = os.path.join(t, 'corpus'); os.mkdir(cdir)
            with open(os.path.join(cdir, 'f1.vcs'), 'wb') as f:
                f.write(b'VCST' + struct.pack('<I', 1))
                f.write(b'INFO' + struct.pack('<I', 4) + b'abcd')
                f.write(b'LSHT' + struct.pack('<I', 256*384*4) + bytes((9, 9, 9, 255)) * (256*384))
            open(os.path.join(cdir, 'index.txt'), 'w').write('f1 bg0 text\n')
            mock = os.path.join(t, 'mock.sh')
            open(mock, 'w').write('#!/bin/sh\ncp %s "$2"\n' % a); os.chmod(mock, 0o755)
            self.assertEqual(vc.main(['corpus', '--vcore', mock, '--oracle', mock, '--libntr',
                                      '--corpus', cdir, '--out', os.path.join(t, 'o')]), 0)
            self.assertTrue(os.path.exists(os.path.join(t, 'o', 'f1.libntr.diff.png')))
            self.assertEqual(vc.vcs_lsht(os.path.join(cdir, 'f1.vcs')).px[:3], bytearray(b'\t\t\t'))

if __name__ == '__main__':
    unittest.main()
