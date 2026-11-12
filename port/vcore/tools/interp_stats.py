#!/usr/bin/env python3
"""an.py FILE [FILE...] [--range A B]: per-scene frame-pair table from BWPORT_INTERP_STATS logs (docs/interp.md)."""
import re, sys, collections

def parse(path, lo=None, hi=None):
    rows = []
    for ln in open(path):
        m = re.match(r'f=(\d+) on=(\d)/(\d) nl=(\d+)/(\d+) nv=(\d+)/(\d+) npoly=(\d+) str=(\d+) loose=(\d+) rem=(\d+) add=(\d+) '
                     r'jump=(\d+) behind=(\d+) vtp=(\d+) vtr=(\d+) vta=(\d+) chA=(\d+) chAt=(\d+) chT=(\d+) chP=(\d+) chV=(\d+) '
                     r'same=(\d) dmean=([\d.]+) dmax=([\d.]+)', ln)
        if not m:
            continue
        g = m.groups()
        r = dict(f=int(g[0]), on_a=int(g[1]), on_b=int(g[2]), nl_a=int(g[3]), nl_b=int(g[4]), nv_a=int(g[5]), nv_b=int(g[6]),
                 npoly=int(g[7]), strict=int(g[8]), loose=int(g[9]), rem=int(g[10]), add=int(g[11]), jump=int(g[12]),
                 behind=int(g[13]), vtp=int(g[14]), vtr=int(g[15]), vta=int(g[16]), chA=int(g[17]), chAt=int(g[18]),
                 chT=int(g[19]), chP=int(g[20]), chV=int(g[21]), same=int(g[22]), dmean=float(g[23]), dmax=float(g[24]))
        # 2D parts
        for e in (0, 1):
            m2 = re.search(r'\| e%d scr=(\w+) sd=(\d+) aff=(\w+) reg=(\d+) obj=(\d+) mv=(\d+) mvmax=(\d+) chg=(\d+) app=(\d+) dis=(\d+) pal=(\d+)' % e, ln)
            if m2:
                k = m2.groups()
                r['e%d' % e] = dict(scr=int(k[0], 16), sd=int(k[1]), aff=int(k[2], 16), reg=int(k[3]), obj=int(k[4]), mv=int(k[5]),
                                    mvmax=int(k[6]), chg=int(k[7]), app=int(k[8]), dis=int(k[9]), pal=int(k[10]))
        rows.append(r)
    return rows

def classify(r):
    if not r['on_a'] and not r['on_b']:
        return 'NO3D'
    if r['on_a'] != r['on_b']:
        return 'TOGGLE'
    nv = max(r['nv_a'], r['nv_b'], 1)
    use = r['vtp']
    if r['nl_a'] == r['nl_b'] and r['rem'] == 0 and r['add'] == 0 and r['jump'] == 0 and r['behind'] == 0:
        if r['dmean'] < 0.05:
            return 'STILL'
        return 'MATCH'
    if use >= 0.9 * nv:
        return 'MOSTLY'      # >= 90 % of the vertices pair and move sanely
    if use >= 0.5 * nv:
        return 'PARTIAL'
    return 'CUT'

def table(name, rows):
    n = len(rows)
    c = collections.Counter(classify(r) for r in rows)
    print('== %s: %d frame pairs' % (name, n))
    order = ['STILL', 'MATCH', 'MOSTLY', 'PARTIAL', 'CUT', 'TOGGLE', 'NO3D']
    print('   ' + '  '.join('%s %d (%.1f%%)' % (k, c[k], 100.0 * c[k] / max(n, 1)) for k in order if c[k]))
    # the reasons of non-STILL/MATCH pairs
    bad = [r for r in rows if classify(r) in ('MOSTLY', 'PARTIAL', 'CUT')]
    if bad:
        rs = collections.Counter()
        for r in bad:
            if r['rem']: rs['lists removed'] += 1
            if r['add']: rs['lists added'] += 1
            if r['jump']: rs['moved > 48 px'] += 1
            if r['behind']: rs['behind camera'] += 1
            if r['chAt']: rs['attr changed'] += 1
            if r['chA']: rs['alpha changed'] += 1
            if r['chT']: rs['texparam changed'] += 1
            if r['chP']: rs['palette changed'] += 1
            if r['chV']: rs['viewport changed'] += 1
        print('   non-trivial pairs %d, reasons (a pair can have several): %s' % (len(bad), ', '.join('%s %d' % kv for kv in rs.most_common())))
    # blendable vertex share, over the 3D-on pairs
    on = [r for r in rows if r['on_a'] and r['on_b']]
    if on:
        tv = sum(max(r['nv_a'], r['nv_b']) for r in on)
        uv = sum(r['vtp'] for r in on)
        print('   vertices usable for a blend: %.1f %% of %d (pairs with 3D on both: %d); mean lists %.0f, vertices %.0f, polygons %.0f'
              % (100.0 * uv / max(tv, 1), tv, len(on), sum(r['nl_b'] for r in on) / len(on), sum(r['nv_b'] for r in on) / len(on),
                 sum(r['npoly'] for r in on) / len(on)))
    # 3D update rate: runs of STILL between non-STILL
    cls = [classify(r) for r in rows]
    mv = [i for i, k in enumerate(cls) if k not in ('STILL', 'NO3D')]
    if len(mv) > 10:
        gaps = collections.Counter(b - a for a, b in zip(mv, mv[1:]))
        tot = sum(gaps.values())
        print('   gap between frames that changed the 3D (frames): ' + ', '.join('%d:%.0f%%' % (k, 100.0 * v / tot) for k, v in sorted(gaps.items())[:8]))
    dm = [r["dmean"] for r in rows if classify(r) in ("MATCH", "MOSTLY") and r["dmean"] > 0.05]
    if dm:
        dm.sort()
        print('   motion of changed pairs (mean px, 256 wide): median %.2f, p90 %.2f, max %.2f' % (dm[len(dm) // 2], dm[int(len(dm) * .9)], dm[-1]))

def twod(name, rows):
    rows = [r for r in rows if "e0" in r and "e1" in r]
    if not rows:
        return
    n = len(rows)
    print('-- 2D, %s (%d pairs)' % (name, n))
    for e in (0, 1):
        k = 'e%d' % e
        scr = sum(1 for r in rows if r[k]['scr'])
        aff = sum(1 for r in rows if r[k]['aff'])
        reg = sum(1 for r in rows if r[k]['reg'])
        mv = sum(1 for r in rows if r[k]['mv'])
        chg = sum(1 for r in rows if r[k]['chg'])
        app = sum(1 for r in rows if r[k]['app'] or r[k]['dis'])
        pal = sum(1 for r in rows if r[k]['pal'])
        mx = max(r[k]['mvmax'] for r in rows)
        objs = sum(r[k]['obj'] for r in rows) / n
        sdm = max(r[k]['sd'] for r in rows)
        print('   engine %s: BG scroll changed %.1f%% (max sum|d| %d), affine %.1f%%, other regs %.1f%%, OBJ moved %.1f%% (max %d px), '
              'OBJ other attr %.1f%%, OBJ appear/disappear %.1f%%, palette %.1f%%, mean visible OBJs %.1f'
              % ('A' if e == 0 else 'B', 100.0 * scr / n, sdm, 100.0 * aff / n, 100.0 * reg / n, 100.0 * mv / n, mx, 100.0 * chg / n,
                 100.0 * app / n, 100.0 * pal / n, objs))

if __name__ == '__main__':
    args = sys.argv[1:]
    lo = hi = None
    if '--range' in args:
        i = args.index('--range')
        lo, hi = int(args[i + 1]), int(args[i + 2])
        del args[i:i + 3]
    for p in args:
        rows = parse(p)
        if lo is not None:
            rows = [r for r in rows if lo <= r['f'] < hi]
        table(p.split('/')[-1], rows)
        twod(p.split('/')[-1], rows)
