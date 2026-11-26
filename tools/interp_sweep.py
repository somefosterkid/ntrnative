#!/usr/bin/env python3
"""interp_sweep.py: the Smooth motion (in-between) artifact sweep over every-frame captures (docs/interp-sweep.md).

    python3 tools/interp_sweep.py run --play cherry-door-east --spec 10990-11400/1 --out DIR [--strips 3] [--drop] [--caps DIR]
    python3 tools/interp_sweep.py run --set tools/interp_sweep_set.txt [--shard I/N] --out DIR ...   (the standard set: one
                                      `PLAY SPEC` per line; shard I of N takes every N-th line)
    python3 tools/interp_sweep.py report --out DIR [--md FILE] [--top 8]

run: records (or reuses) the play's captures (tools/caps.py; --caps DIR = a private cache, --drop deletes the captures after
scoring), scores them with build/vcore/vcore_ipsweep (the live pairing / texcoord hold / gap code; classes in its header) into
DIR/<game>-<play>.txt, and with --strips N renders the N worst events of each class as 3x crops OLD | first 120 Hz in-between |
KEY (build/vcore/vcore_interp) into DIR/strips/. report: ranks the events of every DIR/*.txt per class, counts per game.
Several `run`s (one per play) can go to different slots; `report` merges their DIR/*.txt.
"""
import argparse
import collections
import glob
import os
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CLASSES = ['A_MISPAIR', 'A_OFFMOTION', 'B_TEXSTEP', 'B_TEXSCROLL', 'B_HELD', 'B_HELDRUN', 'C_MISSED', 'C_APPEAR', 'C_VANISH', 'C_NEW', 'C_BLINK',
           'C_KEPT', 'E_SPEED', 'E_CUT', 'O_DISAGREE', 'O_MISSED', 'V_NONE', 'V_TWICE', 'V_STALE', 'P_NONE', 'P_TWICE', 'P_CROSS']  # O_ / V_ / P_: --oracle
STRIP_CLASSES = ['A_MISPAIR', 'A_OFFMOTION', 'B_TEXSTEP', 'B_HELDRUN', 'C_MISSED', 'C_APPEAR', 'C_VANISH', 'O_DISAGREE', 'V_NONE', 'V_TWICE']
Ev = collections.namedtuple('Ev', 'cls frame old list key n cx cy size err v120 v60 info')


def parse(text):
    """-> (header dict, [Ev], {cls: (n, v1, v3, maxv, frame, list)}, stat dict)"""
    head, evs, sums, stat = {}, [], {}, {}
    for line in text.splitlines():
        w = line.split()
        if not w:
            continue
        if w[0] == 'PLAY' and len(w) >= 4:
            head = {'game': w[1], 'play': w[2], 'spec': w[3]}
        elif w[0] == 'EV' and len(w) >= 13:
            evs.append(Ev(w[1], int(w[2]), int(w[3]), int(w[4]), w[5], int(w[6]), float(w[7]), float(w[8]), float(w[9]),
                          float(w[10]), float(w[11]), float(w[12]), ' '.join(w[13:])))
        elif w[0] == 'SUM' and len(w) >= 8:
            sums[w[1]] = (int(w[2]), int(w[3]), int(w[4]), float(w[5]), int(w[6]), int(w[7]))
        elif w[0] == 'STAT':
            it = iter(w[1:])
            stat = {k: v for k, v in zip(it, it)}
    return head, evs, sums, stat


# ---- PNG (8-bit RGB/RGBA, any filter) ----
def readpng(path):
    d = open(path, 'rb').read()
    pos, idat = 8, b''
    while pos < len(d):
        n, = struct.unpack('>I', d[pos:pos + 4])
        t, c = d[pos + 4:pos + 8], d[pos + 8:pos + 8 + n]
        pos += 12 + n
        if t == b'IHDR':
            w, h, _, ct = struct.unpack('>IIBB', c[:10])
        elif t == b'IDAT':
            idat += c
    bpp = 4 if ct == 6 else 3
    raw, stride = zlib.decompress(idat), w * bpp
    rows, prev, o = [], bytearray(stride), 0
    for _ in range(h):
        f = raw[o]
        line = bytearray(raw[o + 1:o + 1 + stride])
        o += 1 + stride
        if f:
            for i in range(stride):
                a = line[i - bpp] if i >= bpp else 0
                b = prev[i]
                c2 = prev[i - bpp] if i >= bpp else 0
                if f == 1:
                    line[i] = (line[i] + a) & 255
                elif f == 2:
                    line[i] = (line[i] + b) & 255
                elif f == 3:
                    line[i] = (line[i] + (a + b) // 2) & 255
                else:
                    pa, pb, pc = abs(b - c2), abs(a - c2), abs(a + b - 2 * c2)
                    line[i] = (line[i] + (a if pa <= pb and pa <= pc else b if pb <= pc else c2)) & 255
        prev = line
        rows.append(line if bpp == 3 else bytearray(b for i, b in enumerate(line) if i % 4 != 3))
    return w, h, rows


def writepng(path, w, h, rows):
    raw = b''.join(b'\x00' + bytes(r) for r in rows)

    def ch(t, c):
        return struct.pack('>I', len(c)) + t + c + struct.pack('>I', zlib.crc32(t + c) & 0xffffffff)
    open(path, 'wb').write(b'\x89PNG\r\n\x1a\n' + ch(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0)) +
                           ch(b'IDAT', zlib.compress(raw)) + ch(b'IEND', b''))


def crop_box(cx, cy, size, scale, w, h):
    """the crop (x0, y0, cw, ch) in a scale-x top screen around a list's box: the box plus 24 px, at least 64 x 48, clipped"""
    half_w, half_h = max(32, size / 2 + 24), max(24, size / 2 + 24)
    x0, x1 = max(0, int((cx - half_w) * scale)), min(w, int((cx + half_w) * scale))
    y0, y1 = max(0, int((cy - half_h) * scale)), min(min(h, 192 * scale), int((cy + half_h) * scale))
    return x0, y0, max(1, x1 - x0), max(1, y1 - y0)


def strip(pngs, crop, out):
    """crop the same rectangle of each picture, side by side with a 4 px magenta gap"""
    x0, y0, cw, chh = crop
    tiles = []
    for p in pngs:
        _, _, rows = readpng(p)
        tiles.append([bytes(rows[y][x0 * 3:(x0 + cw) * 3]) for y in range(y0, y0 + chh)])
    gap = bytes([255, 0, 255]) * 4
    writepng(out, len(tiles) * (cw + 4), chh, [b''.join(t[y] + gap for t in tiles) for y in range(chh)])


def worst(evs, cls, n):
    """the n worst events of a class, one per (frame, list key) neighbourhood so a long run of one artifact takes one slot"""
    out, seen = [], set()
    for e in sorted((e for e in evs if e.cls == cls), key=lambda e: -e.v120):
        k = (e.key, e.frame // 30)
        if k in seen:
            continue
        seen.add(k)
        out.append(e)
        if len(out) == n:
            break
    return out


def read_set(path, shard):
    rows = []
    for no, ln in enumerate(open(path), 1):
        if not ln.strip() or ln.startswith('#'):
            continue
        t = ln.split()
        if len(t) < 2:
            sys.exit('interp_sweep: %s:%d: a row is `PLAY SPEC`, got %r' % (path, no, ln.strip()))
        rows.append(t[:2])
    try:
        i, n = (int(x) for x in shard.split('/')) if shard else (1, 1)
    except ValueError:
        sys.exit('interp_sweep: --shard is I/N (two integers), got %r' % shard)
    if n < 1 or not 1 <= i <= n:
        sys.exit('interp_sweep: --shard I/N needs N >= 1 and 1 <= I <= N, got %s' % shard)
    out = [r for k, r in enumerate(rows) if k % n == i - 1]
    if not out:
        sys.exit('interp_sweep: %s has no rows for shard %s (%d rows)' % (path, shard or '1/1', len(rows)))
    return out


def cmd_run(a):
    if a.set:
        rows = read_set(a.set, a.shard)
    elif a.play and a.spec:
        rows = [(a.play, a.spec)]
    else:
        sys.exit('interp_sweep run: --play and --spec, or --set')
    rc = 0
    for play, spec in rows:
        try:
            run_one(a, play, spec)
        except (SystemExit, Exception) as e:  # one play's failure does not stop the set
            print(f'interp_sweep: {play} {spec}: FAILED {e}')
            rc = 1
    return rc


def run_one(a, play, spec):
    sys.path.insert(0, os.path.join(REPO, 'tools'))
    import caps
    if a.caps:
        # a shard's private cache: its cleanup (--drop) must not remove another shard's captures
        caps.CACHE = os.path.join(a.caps, 'shard' + (a.shard or '1/1').replace('/', 'of'))
    d = caps.get(play, spec)
    game = caps.play_file(play)[1]
    files = sorted(glob.glob(os.path.join(d, '*.vcs')))
    tool = os.path.join(REPO, a.build, 'vcore', 'vcore_ipsweep')
    name = f'{game}-{play}'
    # the default run, then each --variants run on the same captures (written to OUT_<variant>)
    for v in [''] + [x for x in (a.variants or '').split(',') if x] + (['vsbuild'] if a.vsbuild else []):
        env = dict(os.environ)
        tl = os.path.join(REPO, a.vsbuild, 'vcore', 'vcore_ipsweep') if v == 'vsbuild' else tool  # another build's scorer: before / after
        if v == 'vsbuild':
            pass
        elif v == 'nohold':
            env['BWPORT_INTERP_TEXHOLD'] = '0'
        elif v:
            sys.exit(f'interp_sweep: unknown variant {v} (nohold)')
        orc = ['--oracle'] if v != 'vsbuild' else []  # (an older build's scorer may not have it)
        r = subprocess.run([tl, '--thr', str(a.thr), '--events'] + orc + files,
                           capture_output=True, text=True, env=env)
        if r.returncode:
            sys.exit(f'interp_sweep: vcore_ipsweep failed ({r.returncode}): {r.stderr[-400:]}')
        od = a.out + ('_' + v if v else '')
        os.makedirs(od, exist_ok=True)
        t = f'PLAY {game} {play} {spec}\n' + r.stdout
        open(os.path.join(od, name + '.txt'), 'w').write(t)
        if not v:
            text = t
    _, evs, sums, stat = parse(text)
    print(f'{name}: {len(files)} captures, ' + ' '.join(f'{k}={v}' for k, v in stat.items()))
    for c in CLASSES:
        if c in sums and sums[c][0]:
            print(f'  {c:12s} {sums[c][0]:6d} vis>=1 {sums[c][1]:5d} vis>=3 {sums[c][2]:5d} max {sums[c][3]:.1f} (f{sums[c][4]})')
    if a.strips:
        sd = os.path.join(a.out, 'strips')
        os.makedirs(sd, exist_ok=True)
        tmp = tempfile.mkdtemp(prefix='ipsw')
        interp = os.path.join(REPO, a.build, 'vcore', 'vcore_interp')
        for c in STRIP_CLASSES:
            for e in worst(evs, c, a.strips):
                gap = min(2, max(1, e.frame - e.old))
                fk, fo = os.path.join(d, 'f%06d.vcs' % e.frame), os.path.join(d, 'f%06d.vcs' % e.old)
                pre = os.path.join(tmp, 'x')
                # vcore_interp blends A (KEY) toward B (OLD) by --f: the first 120 Hz in-between is 1 - 1 / (2 gap) toward OLD
                subprocess.run([interp, fk, fo, pre, '--f', '%.4f' % (1 - 1 / (2 * gap)), '--scale', '3'], capture_output=True)
                pngs = [pre + '_t1.png', pre + '_mid.png', pre + '_t.png']
                if not all(os.path.exists(p) for p in pngs):
                    continue
                w, h, _ = readpng(pngs[0])
                strip(pngs, crop_box(e.cx, e.cy, e.size, 3, w, h), os.path.join(sd, f'{name}-{c}-{e.frame}-{e.list}.png'))
                for p in pngs:
                    os.remove(p)
        shutil.rmtree(tmp, ignore_errors=True)
    if a.drop:
        shutil.rmtree(d, ignore_errors=True)


def load_runs(d):
    runs = []
    for p in sorted(glob.glob(os.path.join(d, '*.txt'))):
        head, evs, sums, stat = parse(open(p).read())
        if head:
            runs.append((head, evs, sums, stat))
    return runs


def class_counts(runs, g, c):
    n = [0, 0, 0]
    for h, _, sums, _ in runs:
        if h['game'] == g and c in sums:
            n = [n[i] + sums[c][i] for i in range(3)]
    return n


def cmd_report(a):
    runs = load_runs(a.out)
    games = sorted({h['game'] for h, *_ in runs})
    L = ['# Smooth motion in-between sweep', '',
         'Generated by `tools/interp_sweep.py report` (docs/interp-sweep.md). err / vis: screen px at 256 x 192 (texels for B_*);',
         'vis120 = what the first 120 Hz in-between shows (err x (1 - f)). Counts: events, events with vis120 >= 1 px / >= 3 px.', '',
         '## Captures', '', '| game | play | frames | keyframes | pairs | cuts / scene | camera moving | camera px | lists paired | gaps 1 / 2 / 3+ |',
         '|---|---|---|---|---|---|---|---|---|---|']
    for h, evs, sums, st in runs:
        L.append(f"| {h['game']} | {h['play']} | {h['spec']} | {st.get('keyframes', '?')} | {st.get('pairs', '?')} | {st.get('cuts', '?')} / {st.get('scene', '?')} | "
                 f"{st.get('moving', '?')} | {st.get('campath', '?')} | "
                 f"{st.get('paired', '?')} | {st.get('gaps1', '?')} / {st.get('gaps2', '?')} / {st.get('gaps3+', '?')} |")
    L += ['', '## Counts per class and game (events / vis>=1 / vis>=3)', '', '| class | ' + ' | '.join(games) + ' |',
          '|---|' + '---|' * len(games)]
    for c in CLASSES:
        L.append(f'| {c} | ' + ' | '.join('%d / %d / %d' % tuple(class_counts(runs, g, c)) for g in games) + ' |')
    if a.vs:
        vs = load_runs(a.vs)
        L += ['', f'## Before / after: {a.vs} -> {a.out} (events, vis>=1 px)', '', '| class | ' + ' | '.join(games) + ' |', '|---|' + '---|' * len(games)]
        for c in CLASSES:
            cells = []
            for g in games:
                b, n = class_counts(vs, g, c), class_counts(runs, g, c)
                cells.append(f'{b[0]} ({b[1]}) -> {n[0]} ({n[1]})' if b[0] or n[0] else '0')
            L.append(f'| {c} | ' + ' | '.join(cells) + ' |')
    sd = os.path.join(a.out, 'strips')
    for c in CLASSES:
        allev = [(h, e) for h, evs, _, _ in runs for e in evs if e.cls == c]
        if not allev:
            continue
        L += ['', f'## {c}: worst {a.top}', '', '| game | play | frame (old) | list | key | n | box cx,cy size | err | vis120 | vis60 | info |',
              '|---|---|---|---|---|---|---|---|---|---|---|']
        per = collections.defaultdict(list)
        for h, e in allev:
            per[(h['game'], h['play'])].append(e)
        top = sorted(((h, e) for (g, p), evs in per.items() for e in worst(evs, c, a.top)
                      for h in [{'game': g, 'play': p}]), key=lambda x: -x[1].v120)[:a.top]
        for h, e in top:
            s = f"{h['game']}-{h['play']}-{c}-{e.frame}-{e.list}.png"
            info = e.info + (f' strip {s}' if os.path.exists(os.path.join(sd, s)) else '')
            L.append(f"| {h['game']} | {h['play']} | {e.frame} ({e.old}) | {e.list} | {e.key} | {e.n} | {e.cx:.0f},{e.cy:.0f} {e.size:.0f} | "
                     f"{e.err:.1f} | {e.v120:.1f} | {e.v60:.1f} | {info} |")
    md = '\n'.join(L) + '\n'
    if a.md:
        open(a.md, 'w').write(md)
    else:
        sys.stdout.write(md)
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    r = sub.add_parser('run')
    r.add_argument('--play')
    r.add_argument('--spec')
    r.add_argument('--set')
    r.add_argument('--shard')
    r.add_argument('--out', required=True)
    r.add_argument('--thr', type=float, default=6.0)
    r.add_argument('--strips', type=int, default=0)
    r.add_argument('--drop', action='store_true')
    r.add_argument('--caps', help='capture cache directory; each shard uses its own subdirectory')
    r.add_argument('--variants', help='also score the same captures as nohold (BWPORT_INTERP_TEXHOLD=0) into OUT_<variant>')
    r.add_argument('--build', default='build')
    r.add_argument('--vsbuild', help='a second build dir (its vcore/vcore_ipsweep, e.g. of an older commit): the same captures scored into OUT_vsbuild')
    p = sub.add_parser('report')
    p.add_argument('--out', required=True)
    p.add_argument('--vs', help='a second run directory: adds a before/after table (its counts -> this one)')
    p.add_argument('--md')
    p.add_argument('--top', type=int, default=8)
    a = ap.parse_args(argv)
    return cmd_run(a) if a.cmd == 'run' else cmd_report(a)


if __name__ == '__main__':
    sys.exit(main())
