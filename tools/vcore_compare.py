#!/usr/bin/env python3
"""vcore_compare: compare video-core output PNGs (pure Python 3 + zlib).

  vcore_compare.py compare A.png B.png [--diff OUT.png]
  vcore_compare.py corpus --vcore BIN --oracle BIN [--libntr] [--corpus DIR] [--out DIR]

A picture is 256x384 (top screen over bottom screen). Pixels compare equal
after reducing each channel to 6 bits (the DS's colour depth).
Tools are run as `BIN FILE.vcs OUT.png`. libntr's own picture is the LSHT
chunk (256x384 RGBA) of the .vcs.
Corpus index (from g51a): DIR/index.json ({name: [features]} or a list of
{"name"/"file", "features"}) or DIR/index.txt (one line per frame: name, then
features separated by spaces/commas; '#' lines ignored). No index: features
are empty.
"""
import sys, os, zlib, struct, subprocess, argparse, json, glob

# ---------------------------------------------------------------- PNG
class Image:
    def __init__(self, w, h, px):  # px: bytearray RGB, 3 bytes per pixel
        self.w, self.h, self.px = w, h, px

def _paeth(a, b, c):
    p = a + b - c
    pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
    if pa <= pb and pa <= pc: return a
    return b if pb <= pc else c

def read_png(path_or_bytes):
    data = path_or_bytes if isinstance(path_or_bytes, (bytes, bytearray)) else open(path_or_bytes, 'rb').read()
    if data[:8] != b'\x89PNG\r\n\x1a\n':
        raise ValueError('not a PNG')
    pos = 8; idat = []; plte = None
    while pos < len(data):
        ln, = struct.unpack('>I', data[pos:pos+4]); tag = data[pos+4:pos+8]
        body = data[pos+8:pos+8+ln]; pos += 12 + ln
        if tag == b'IHDR':
            w, h, depth, ctype, comp, filt, inter = struct.unpack('>IIBBBBB', body)
        elif tag == b'PLTE': plte = body
        elif tag == b'IDAT': idat.append(body)
        elif tag == b'IEND': break
    if inter: raise ValueError('interlaced PNG unsupported')
    chans = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}[ctype]
    if depth not in (8, 16) and ctype not in (0, 3):
        raise ValueError('unsupported bit depth')
    bits = depth * chans
    stride = (w * bits + 7) // 8
    bpp = max(1, bits // 8)
    raw = zlib.decompress(b''.join(idat))
    rows = []; prev = bytearray(stride); p = 0
    for y in range(h):
        f = raw[p]; line = bytearray(raw[p+1:p+1+stride]); p += 1 + stride
        if f == 1:
            for i in range(bpp, stride): line[i] = (line[i] + line[i-bpp]) & 255
        elif f == 2:
            for i in range(stride): line[i] = (line[i] + prev[i]) & 255
        elif f == 3:
            for i in range(stride):
                l = line[i-bpp] if i >= bpp else 0
                line[i] = (line[i] + ((l + prev[i]) >> 1)) & 255
        elif f == 4:
            for i in range(stride):
                l = line[i-bpp] if i >= bpp else 0
                u = prev[i]; ul = prev[i-bpp] if i >= bpp else 0
                line[i] = (line[i] + _paeth(l, u, ul)) & 255
        elif f != 0: raise ValueError('bad filter')
        rows.append(line); prev = line
    px = bytearray(w * h * 3); o = 0
    for line in rows:
        if ctype == 2 and depth == 8:
            px[o:o+w*3] = line
        elif ctype == 6 and depth == 8:
            px[o:o+w*3:3] = line[0::4]; px[o+1:o+w*3:3] = line[1::4]; px[o+2:o+w*3:3] = line[2::4]
        elif ctype == 2:  # 16-bit: high bytes
            px[o:o+w*3] = line[0::2]
        elif ctype == 6:
            for x in range(w):
                px[o+x*3:o+x*3+3] = bytes((line[x*8], line[x*8+2], line[x*8+4]))
        else:
            for x in range(w):
                if ctype == 3 or depth < 8:
                    if depth == 8: v = line[x]
                    else:
                        per = 8 // depth
                        v = (line[x // per] >> (8 - depth * (x % per + 1))) & ((1 << depth) - 1)
                    if ctype == 3: c = plte[v*3:v*3+3]
                    else:
                        g = v * 255 // ((1 << depth) - 1); c = bytes((g, g, g))
                elif depth == 8:
                    g = line[x*chans]; c = bytes((g, g, g))
                else:
                    g = line[x*chans*2]; c = bytes((g, g, g))
                px[o+x*3:o+x*3+3] = c
        o += w * 3
    return Image(w, h, px)

def _chunk(tag, body):
    c = struct.pack('>I', len(body)) + tag + body
    return c + struct.pack('>I', zlib.crc32(tag + body) & 0xffffffff)

def png_bytes(img):
    raw = bytearray(); st = img.w * 3
    for y in range(img.h):
        raw.append(0); raw += img.px[y*st:(y+1)*st]
    return (b'\x89PNG\r\n\x1a\n' + _chunk(b'IHDR', struct.pack('>IIBBBBB', img.w, img.h, 8, 2, 0, 0, 0))
            + _chunk(b'IDAT', zlib.compress(bytes(raw), 6)) + _chunk(b'IEND', b''))

def write_png(path, img):
    with open(path, 'wb') as f: f.write(png_bytes(img))

def rgba_to_image(w, h, rgba):
    px = bytearray(w * h * 3)
    px[0::3] = rgba[0::4]; px[1::3] = rgba[1::4]; px[2::3] = rgba[2::4]
    return Image(w, h, px)

# ---------------------------------------------------------------- compare
class ScreenResult:
    def __init__(self, total, diff, maxerr):
        self.total, self.diff, self.maxerr = total, diff, maxerr
        self.match = 100.0 * (total - diff) / total if total else 100.0

def compare_images(a, b, diff_img=None):
    """Return [top, bottom] ScreenResult (w x 2h image: two stacked screens).
    maxerr is the largest 6-bit channel difference. If diff_img is given it
    is filled: differing pixels red, the rest a dimmed copy of A."""
    if (a.w, a.h) != (b.w, b.h):
        raise ValueError('size mismatch %dx%d vs %dx%d' % (a.w, a.h, b.w, b.h))
    if a.h % 2: raise ValueError('height must be even (two screens)')
    half = a.h // 2; res = []
    if diff_img is not None:
        diff_img.w, diff_img.h = a.w, a.h
        diff_img.px = bytearray(len(a.px))
    for s in range(2):
        lo, hi = s * half * a.w * 3, (s + 1) * half * a.w * 3
        pa, pb = a.px[lo:hi], b.px[lo:hi]
        diff = 0; maxerr = 0
        if pa == pb:
            if diff_img is not None:
                diff_img.px[lo:hi] = bytes(v >> 2 for v in pa)
            res.append(ScreenResult(a.w * half, 0, 0)); continue
        for i in range(0, hi - lo, 3):
            r0, g0, b0 = pa[i], pa[i+1], pa[i+2]
            r1, g1, b1 = pb[i], pb[i+1], pb[i+2]
            m = max(abs((r0>>2)-(r1>>2)), abs((g0>>2)-(g1>>2)), abs((b0>>2)-(b1>>2)))
            if m:
                diff += 1
                if m > maxerr: maxerr = m
                if diff_img is not None:
                    diff_img.px[lo+i:lo+i+3] = b'\xff\x00\x00'
            elif diff_img is not None:
                diff_img.px[lo+i:lo+i+3] = bytes((r0>>2, g0>>2, b0>>2))
        res.append(ScreenResult(a.w * half, diff, maxerr))
    return res

def cmd_compare(args):
    a, b = read_png(args.a), read_png(args.b)
    d = Image(a.w, a.h, bytearray()) if args.diff else None
    res = compare_images(a, b, d)
    for name, r in zip(('top', 'bottom'), res):
        print('%-6s match %7.3f%%  maxerr %2d/63  differing %d/%d' % (name, r.match, r.maxerr, r.diff, r.total))
    if d: write_png(args.diff, d)
    return 0 if all(r.diff == 0 for r in res) else 1

# ---------------------------------------------------------------- vcs / corpus
def vcs_lsht(path):
    """The LSHT chunk (libntr's own picture) of a .vcs as an Image, or None."""
    data = open(path, 'rb').read()
    if data[:4] != b'VCST': raise ValueError('not a VCST file')
    pos = 8
    while pos + 8 <= len(data):
        tag = data[pos:pos+4]; size, = struct.unpack('<I', data[pos+4:pos+8])
        body = data[pos+8:pos+8+size]; pos += 8 + ((size + 3) & ~3)
        if tag == b'LSHT':
            if size < 256 * 384 * 4: return None
            return rgba_to_image(256, 384, body[:256*384*4])
    return None

def load_index(d):
    feats = {}
    def norm(n): return os.path.splitext(os.path.basename(n))[0]
    def fl(v):
        if isinstance(v, list): return v
        if isinstance(v, dict): return fl(v.get('features', []))
        return str(v).replace(',', ' ').split()
    p = os.path.join(d, 'index.json')
    if os.path.exists(p):
        j = json.load(open(p))
        if isinstance(j, dict) and 'frames' in j: j = j['frames']
        if isinstance(j, dict):
            for k, v in j.items(): feats[norm(k)] = fl(v)
        else:
            for e in j: feats[norm(e.get('name') or e.get('file'))] = fl(e.get('features', []))
        return feats
    for fn in ('index.txt', 'INDEX.txt', 'index.tsv'):
        p = os.path.join(d, fn)
        if os.path.exists(p):
            for line in open(p):
                line = line.strip()
                if not line or line.startswith('#'): continue
                parts = line.replace(',', ' ').replace(':', ' ').split()
                feats[norm(parts[0])] = parts[1:]
            return feats
    return feats

def run_tool(binpath, vcs, out):
    if os.path.exists(out): os.remove(out)
    try:
        r = subprocess.run([binpath, vcs, out], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=120)
    except (subprocess.TimeoutExpired, OSError) as e:
        return str(e)
    if r.returncode != 0 or not os.path.exists(out):
        return 'rc=%d %s' % (r.returncode, r.stdout.decode('latin1').strip()[-80:])
    return None

def cmd_corpus(args):
    os.makedirs(args.out, exist_ok=True)
    files = sorted(glob.glob(os.path.join(args.corpus, '*.vcs')))
    index = load_index(args.corpus)
    if not files:
        print('no .vcs files in', args.corpus); return 1
    rows = []  # (name, label, [ScreenResult] or None, note)
    for f in files:
        name = os.path.splitext(os.path.basename(f))[0]
        o_png = os.path.join(args.out, name + '.oracle.png')
        v_png = os.path.join(args.out, name + '.vcore.png')
        labels = ['vcore'] + (['libntr'] if args.libntr else [])
        err = run_tool(args.oracle, f, o_png)
        if err:
            for l in labels: rows.append((name, l, None, 'oracle failed: ' + err))
            continue
        pairs = [('vcore', v_png, run_tool(args.vcore, f, v_png))]
        if args.libntr:
            l_png = os.path.join(args.out, name + '.libntr.png')
            try:
                im = vcs_lsht(f)
                if im is None: pairs.append(('libntr', l_png, 'no LSHT'))
                else: write_png(l_png, im); pairs.append(('libntr', l_png, None))
            except Exception as e: pairs.append(('libntr', l_png, str(e)))
        try: oimg = read_png(o_png)
        except Exception as e:
            for l, _, _ in pairs: rows.append((name, l, None, 'oracle png: %s' % e))
            continue
        for label, png, err in pairs:
            if err: rows.append((name, label, None, 'failed: ' + err)); continue
            try:
                d = Image(oimg.w, oimg.h, bytearray())
                res = compare_images(read_png(png), oimg, d)
                write_png(os.path.join(args.out, '%s.%s.diff.png' % (name, label)), d)
                rows.append((name, label, res, ''))
            except Exception as e:
                rows.append((name, label, None, str(e)))
    for label in ['vcore'] + (['libntr'] if args.libntr else []):
        print('\n== %s vs oracle: per frame' % label)
        print('%-28s %9s %9s %5s %5s  %s' % ('frame', 'top%', 'bottom%', 'maxT', 'maxB', 'features'))
        agg = {}; tot = [0, 0, 0]; nfail = 0; nperfect = 0
        for name, l, res, note in rows:
            if l != label: continue
            fe = index.get(name, [])
            if res is None:
                print('%-28s %s' % (name, note)); nfail += 1; continue
            print('%-28s %9.3f %9.3f %5d %5d  %s' % (name, res[0].match, res[1].match, res[0].maxerr, res[1].maxerr, ' '.join(fe)))
            d = sum(r.diff for r in res); t = sum(r.total for r in res)
            tot[0] += d; tot[1] += t; tot[2] += 1
            if d == 0: nperfect += 1
            for ft in fe or ['(none)']:
                a = agg.setdefault(ft, [0, 0, 0]); a[0] += d; a[1] += t; a[2] += 1
        print('-- per feature')
        for ft in sorted(agg):
            a = agg[ft]; print('%-28s %9.3f%%  frames %d' % (ft, 100.0 * (a[1] - a[0]) / a[1], a[2]))
        if tot[1]:
            print('TOTAL %s: %.3f%% over %d frames (%d identical, %d failed)' % (label, 100.0 * (tot[1] - tot[0]) / tot[1], tot[2], nperfect, nfail))
        else:
            print('TOTAL %s: nothing compared (%d failed)' % (label, nfail))
    return 0

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    c = sub.add_parser('compare'); c.add_argument('a'); c.add_argument('b'); c.add_argument('--diff')
    k = sub.add_parser('corpus')
    k.add_argument('--vcore', required=True); k.add_argument('--oracle', required=True)
    k.add_argument('--libntr', action='store_true')
    k.add_argument('--corpus', default='/root/ref/corpus'); k.add_argument('--out', default='/root/mt/vcore_cmp')
    args = ap.parse_args(argv)
    return cmd_compare(args) if args.cmd == 'compare' else cmd_corpus(args)

if __name__ == '__main__':
    sys.exit(main())
