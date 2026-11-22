#!/usr/bin/env python3
"""Texture cache quality checks (design texcache.md section 11, tests 4 and 6) on captures, with gpu_render.

    python3 tools/tc_quality.py exact CAPDIR [CAPDIR ...] [--every K] [--scales 1,3]
        --texcache exact and on (filters off) against off: md5 of the N x 3D layer, every K-th capture. Pass: 0 differ.
    python3 tools/tc_quality.py crops CAPDIR [--scales 1,2,3] [--crop NAME ...] [--frames A-B/S]
        per crop and scale: RMSE (six-bit steps) and the share of pixels 8+ steps off against the supersampled reference
        (Exact at R x box-downsampled: R = 8 for 1x/2x, 6 for 3x: gpu_render's scales stop at 8), for off / on / mips / mips + aniso 4 / mips + aniso 16;
        then crawl: the mean abs change (six-bit steps) between consecutive captures inside the crop, per mode and for the
        reference (its change is the camera's own motion: the closer a mode is to it, the less it crawls).
    python3 tools/tc_quality.py gallery CAPDIR OUTDIR [--scale 3] [--zoom 2] [--crop NAME ...] [--strip CAPDIR2]
        "NN name - off.png" / "NN name - mips + aniso16.png": the crops of the N x layer (zoomed by nearest), and with
        --strip a 4-frame strip (off row over mips + aniso16 row) of consecutive captures from CAPDIR2.

--gpu d3d12 runs the real GPU through Mesa's d3d12 driver (WSL: /dev/dxg; the renderer line names the card), --gpu llvmpipe
(the default when absent) the software rasterizer. Pictures go to $TCQ_OUT (default ~/.cache/bwport/tcq). Pure Python (no PIL).
The crops (CROPS) are boxes in 1x pixels of the 3D layer with the captures they belong to (Platinum town-walk)."""
import argparse, hashlib, os, struct, subprocess, sys, zlib
from multiprocessing import get_context

R = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'build', 'gpu', 'gpu_render')
OUT = os.environ.get('TCQ_OUT', os.path.expanduser('~/.cache/bwport/tcq'))

# name: (frames of the town-walk set, box x, y, w, h in 1x pixels)
CROPS = {
    'side wall left': ([7540, 7548, 7556, 7564], (0, 30, 24, 70)),
    'side wall right': ([7700, 7708, 7716], (156, 0, 40, 72)),
    'grass': ([7540, 7700], (200, 96, 56, 40)),
}

MODES = {'off': ['--texcache', 'off'], 'exact': ['--texcache', 'exact'], 'on': ['--texcache', 'on'],
         'mips': ['--texcache', 'on', '--mips'], 'mips4': ['--texcache', 'on', '--mips', '--aniso', '4'],
         'mips16': ['--texcache', 'on', '--mips', '--aniso', '16'], 'aniso16': ['--texcache', 'on', '--aniso', '16']}
GPU = {'llvmpipe': {'GALLIUM_DRIVER': 'llvmpipe'}, 'd3d12': {'GALLIUM_DRIVER': 'd3d12'}}


def png(path):
    d = open(path, 'rb').read()
    i, idat, w, h, ct = 8, b'', 0, 0, 6
    while i < len(d):
        n, t = struct.unpack('>I4s', d[i:i + 8])
        c = d[i + 8:i + 8 + n]
        if t == b'IHDR': w, h, _, ct = struct.unpack('>IIBB', c[:10])
        if t == b'IDAT': idat += c
        i += 12 + n
    bpp = {6: 4, 2: 3}[ct]
    raw, st = zlib.decompress(idat), w * bpp
    out, prev, p = bytearray(), bytearray(st), 0
    for y in range(h):
        f, line = raw[p], bytearray(raw[p + 1:p + 1 + st]); p += 1 + st
        if f:
            for x in range(st):
                a = line[x - bpp] if x >= bpp else 0; b = prev[x]; c = prev[x - bpp] if x >= bpp else 0
                if f == 1: line[x] = (line[x] + a) & 255
                elif f == 2: line[x] = (line[x] + b) & 255
                elif f == 3: line[x] = (line[x] + (a + b) // 2) & 255
                elif f == 4:
                    pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                    line[x] = (line[x] + (a if pa <= pb and pa <= pc else b if pb <= pc else c)) & 255
        if bpp == 3:
            out += b''.join(bytes(line[x:x + 3]) + b'\xff' for x in range(0, st, 3))
        else:
            out += line
        prev = line
    return w, h, out


def write_png(path, w, h, px):
    raw = b''.join(b'\0' + bytes(px[y * w * 4:(y + 1) * w * 4]) for y in range(h))
    def chunk(t, c): return struct.pack('>I', len(c)) + t + c + struct.pack('>I', zlib.crc32(t + c) & 0xffffffff)
    open(path, 'wb').write(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 6, 0, 0, 0)) +
                           chunk(b'IDAT', zlib.compress(raw, 6)) + chunk(b'IEND', b''))


def crop(img, box, scale):
    w, h, px = img
    x0, y0, cw, ch = (v * scale for v in box)
    out = bytearray()
    for y in range(y0, y0 + ch):
        out += px[(y * w + x0) * 4:(y * w + x0 + cw) * 4]
    return cw, ch, out


def zoom(img, z):
    w, h, px = img
    out = bytearray()
    for y in range(h):
        row = b''.join(bytes(px[(y * w + x) * 4:(y * w + x) * 4 + 4]) * z for x in range(w))
        out += row * z
    return w * z, h * z, out


def side(imgs, gap=4):
    """images side by side (same height), white gaps; rows stacked when imgs is a list of lists."""
    rows = imgs if isinstance(imgs[0], list) else [imgs]
    W = max(sum(i[0] for i in r) + gap * (len(r) - 1) for r in rows)
    H = sum(max(i[1] for i in r) for r in rows) + gap * (len(rows) - 1)
    out = bytearray(b'\xff' * (W * H * 4))
    y = 0
    for r in rows:
        x = 0
        for w, h, px in r:
            for yy in range(h):
                o = ((y + yy) * W + x) * 4
                out[o:o + w * 4] = px[yy * w * 4:(yy + 1) * w * 4]
            x += w + gap
        y += max(i[1] for i in r) + gap
    return W, H, out


def render(a):
    cap, mode, scale, ref, gpu = a
    o = f'{OUT}/{gpu}_{os.path.basename(os.path.dirname(cap))[-6:]}_{os.path.basename(cap)[:-4]}_{mode}_{scale}' \
        f'{"_r%d" % ref if ref else ""}.png'
    if not os.path.exists(o):
        e = dict(os.environ); e.update(GPU[gpu])
        subprocess.run([R, cap, '/dev/null', '--hi', o + '.tmp', '-q', '--warm', '2', '--scale', str(scale)] + MODES[mode] +
                       (['--ref-scale', str(ref)] if ref else []), env=e, check=True,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        os.rename(o + '.tmp', o)
    return a, o


def renderer(gpu):
    e = dict(os.environ); e.update(GPU[gpu])
    info = os.path.join(os.path.dirname(R), 'gpu_info')
    r = subprocess.run([info], env=e, capture_output=True, text=True).stdout
    return next((l.split(None, 1)[1] for l in r.splitlines() if l.startswith('renderer')), '?')


def err(a, b, mask=None):
    """RMSE in six-bit steps and the % of pixels 8+ steps off (pixels where both layers are empty are skipped; with mask,
    only the pixels it holds)."""
    se = n = far = 0
    A, B = a[2], b[2]
    for i in range(0, len(A), 4):
        if A[i + 3] == 0 and B[i + 3] == 0: continue
        if mask is not None and i not in mask: continue
        d0, d1, d2 = abs((A[i] >> 2) - (B[i] >> 2)), abs((A[i + 1] >> 2) - (B[i + 1] >> 2)), abs((A[i + 2] >> 2) - (B[i + 2] >> 2))
        se += (d0 * d0 + d1 * d1 + d2 * d2) / 3; n += 1; far += max(d0, d1, d2) >= 8
    return ((se / n) ** 0.5 if n else 0.0), (100.0 * far / n if n else 0.0)


def differs(a, b):
    """the byte offsets of the pixels where two crops differ."""
    A, B = a[2], b[2]
    return {i for i in range(0, len(A), 4) if A[i:i + 3] != B[i:i + 3]}


def change(a, b):
    """mean abs change in six-bit steps per channel between two crops."""
    A, B = a[2], b[2]
    s = sum(abs((A[i] >> 2) - (B[i] >> 2)) for i in range(len(A)) if i % 4 != 3)
    return s / (len(A) * 3 / 4)


def pool():
    return get_context('fork').Pool(int(os.environ.get('BW_JOBS', '8')))


def capfile(d, f):
    return os.path.join(d, f'f{f:06d}.vcs')


def frames_of(d, spec):
    have = sorted(int(x[1:7]) for x in os.listdir(d) if x.endswith('.vcs'))
    if not spec: return have
    ab, _, st = spec.partition('/')
    a, _, b = ab.partition('-')
    return [f for f in have if int(a) <= f <= int(b or a) and (f - int(a)) % int(st or 1) == 0]


def cmd_exact(o):
    caps = [os.path.join(d, f) for d in o.capdir for f in sorted(os.listdir(d)) if f.endswith('.vcs')][::o.every]
    scales = [int(s) for s in o.scales.split(',')]
    jobs = [(c, m, s, 0, o.gpu) for c in caps for m in ('off', 'exact', 'on') for s in scales]
    with pool() as p:
        res = dict(p.map(render, jobs))
        sums = dict(zip(res, p.map(md5, list(res.values()))))
    for m in ('exact', 'on'):
        d = [a for a in sums if a[1] == m and sums[a] != sums[(a[0], 'off', a[2], 0, o.gpu)]]
        print(f'{m} vs off ({o.gpu}): {len(caps) * len(scales)} pictures, {len(d)} differ',
              ' '.join(os.path.basename(x[0]) + f'@{x[2]}' for x in d[:5]))


def md5(f): return hashlib.md5(png(f)[2]).hexdigest()


def crops_of(name, path, scale):
    return crop(png(path), CROPS[name][1], scale)


def cmd_crops(o):
    names = o.crop or [n for n in CROPS if n != 'grass']
    scales = [int(s) for s in o.scales.split(',')]
    modes = o.modes.split(',')
    fr = {n: (frames_of(o.capdir[0], o.frames) if o.frames else CROPS[n][0]) for n in names}
    jobs = set()
    for n in names:
        for f in fr[n]:
            for s in scales:
                jobs.add((capfile(o.capdir[0], f), 'off', s, 6 if s == 3 else 8, o.gpu))
                for m in modes: jobs.add((capfile(o.capdir[0], f), m, s, 0, o.gpu))
    print(f'renderer: {renderer(o.gpu)} ({o.gpu}); captures {o.capdir[0]}')
    with pool() as p:
        res = dict(p.map(render, sorted(jobs)))
        keys = [(n, a) for n in names for a in res if a[0] in [capfile(o.capdir[0], f) for f in fr[n]]]
        cr = dict(zip(keys, p.starmap(crops_of, [(n, res[a], a[2]) for n, a in keys])))
    for n in names:
        print(f'== {n}: box {CROPS[n][1]} (1x), frames {fr[n][0]}-{fr[n][-1]} ({len(fr[n])})')
        for s in scales:
            ref = {f: cr[(n, (capfile(o.capdir[0], f), 'off', s, 6 if s == 3 else 8, o.gpu))] for f in fr[n]}
            get = lambda f, m: cr[(n, (capfile(o.capdir[0], f), m, s, 0, o.gpu))]
            e = {m: [err(get(f, m), ref[f]) for f in fr[n]] for m in modes}
            print(f'  {s}x vs ref {6 if s == 3 else 8}x  RMSE / 8+%: ' + '  '.join(
                f'{m} {sum(r for r, _ in v) / len(v):.2f}/{sum(q for _, q in v) / len(v):.1f}%' for m, v in e.items()))
            # the same on the pixels the cache's filters touch: where any mode differs from off (one set for every mode)
            mask = {f: set().union(*[differs(get(f, m), get(f, 'off')) for m in modes if m != 'off']) for f in fr[n]}
            npx = sum(len(v) for v in mask.values()) / len(fr[n])
            e = {m: [err(get(f, m), ref[f], mask[f]) for f in fr[n]] for m in modes}
            print(f'  {s}x  on the {npx:.0f} px/frame filtering changes ({100 * npx * 4 / len(ref[fr[n][0]][2]):.0f}%): ' +
                  '  '.join(f'{m} {sum(r for r, _ in v) / len(v):.2f}/{sum(q for _, q in v) / len(v):.1f}%'
                            for m, v in e.items()))
            if len(fr[n]) > 1:
                pairs = list(zip(fr[n], fr[n][1:]))
                cref = sum(change(ref[a], ref[b]) for a, b in pairs) / len(pairs)
                print(f'  {s}x crawl (mean abs change, consecutive captures): ref {cref:.3f}  ' + '  '.join(
                    f'{m} {sum(change(get(a, m), get(b, m)) for a, b in pairs) / len(pairs):.3f}' for m in modes))


def cmd_gallery(o):
    names = o.crop or [n for n in CROPS if n != 'grass']
    os.makedirs(o.outdir, exist_ok=True)
    show = {'off': 'off', 'mips16': 'mips + aniso16'}
    pick = {n: CROPS[n][0][len(CROPS[n][0]) // 2] for n in names}
    jobs = [(capfile(o.capdir[0], pick[n]), m, o.scale, 0, o.gpu) for n in names for m in show]
    strip = []
    if o.strip:
        sf = frames_of(o.strip, o.strip_frames)[:4]
        strip = [(capfile(o.strip, f), m, o.scale, 0, o.gpu) for m in show for f in sf]
    print(f'renderer: {renderer(o.gpu)} ({o.gpu})')
    with pool() as p:
        res = dict(p.map(render, jobs + strip))
    for i, n in enumerate(names):
        for m, nm in show.items():
            c = zoom(crop(png(res[(capfile(o.capdir[0], pick[n]), m, o.scale, 0, o.gpu)]), CROPS[n][1], o.scale), o.zoom)
            write_png(os.path.join(o.outdir, f'{o.first + i:02d} {n} - {nm}.png'), *c)
    if strip:
        rows = [[zoom(crop(png(res[a]), CROPS[o.strip_crop][1], o.scale), o.zoom) for a in strip if a[1] == m] for m in show]
        p = os.path.join(o.outdir, f'{o.first + len(names):02d} {o.strip_crop} crawl - 4 frames, off over mips + aniso16.png')
        write_png(p, *side(rows))
    print('gallery:', o.outdir)


def cmd_sheet(o):
    """sheet CAPDIR OUT.png: one crop of the captures --frames (default the crop's own), a row per frame, a column per
    mode of --modes (ref = the supersampled reference), zoomed."""
    n = (o.crop or ['side wall left'])[0]
    fr = frames_of(o.capdir[0], o.frames) if o.frames else CROPS[n][0]
    modes = o.modes.split(',')
    ref = 6 if o.scale == 3 else 8
    jobs = [(capfile(o.capdir[0], f), 'off' if m == 'ref' else m, o.scale, ref if m == 'ref' else 0, o.gpu)
            for f in fr for m in modes]
    with pool() as p:
        res = dict(p.map(render, jobs))
    rows = [[zoom(crop(png(res[a]), CROPS[n][1], o.scale), o.zoom) for a in jobs if a[0] == capfile(o.capdir[0], f)] for f in fr]
    write_png(o.capdir[1], *side(rows))
    print(f'{o.capdir[1]}: {n}, frames {fr}, columns {modes}, {o.scale}x')


def cmd_diff(o):
    """diff A.png B.png OUT.png: |A - B| per channel times 4 (black = equal), and the count of differing pixels."""
    a, b = png(o.capdir[0]), png(o.capdir[1])
    A, B = a[2], b[2]
    out = bytearray(len(A))
    n = 0
    for i in range(0, len(A), 4):
        d = [min(255, 4 * abs(A[i + c] - B[i + c])) for c in range(3)]
        out[i:i + 4] = bytes(d) + b'\xff'
        n += any(d)
    write_png(o.capdir[2], a[0], a[1], out)
    print(f'{n} of {len(A) // 4} pixels differ')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('what', choices=['exact', 'crops', 'gallery', 'diff', 'sheet'])
    ap.add_argument('capdir', nargs='+')
    ap.add_argument('--gpu', choices=list(GPU), default='llvmpipe')
    ap.add_argument('--every', type=int, default=1)
    ap.add_argument('--scales', default='1,2,3')
    ap.add_argument('--modes', default='off,on,mips,mips4,mips16')
    ap.add_argument('--crop', action='append')
    ap.add_argument('--frames', help='A-B/S: these captures of CAPDIR instead of the crop\'s own list')
    ap.add_argument('--scale', type=int, default=3)
    ap.add_argument('--zoom', type=int, default=2)
    ap.add_argument('--first', type=int, default=5, help='number of the first gallery file')
    ap.add_argument('--strip', help='CAPDIR of consecutive captures for the crawl strip')
    ap.add_argument('--strip-frames', default='')
    ap.add_argument('--strip-crop', default='side wall left')
    o = ap.parse_args()
    if o.what == 'gallery':
        o.outdir = o.capdir.pop()
    os.makedirs(OUT, exist_ok=True)
    {'exact': cmd_exact, 'crops': cmd_crops, 'gallery': cmd_gallery, 'diff': cmd_diff, 'sheet': cmd_sheet}[o.what](o)


if __name__ == "__main__":
    main()
