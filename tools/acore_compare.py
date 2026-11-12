#!/usr/bin/env python3
"""acore_compare: compare sound-core output (pure Python 3, wave + array; no numpy).

  acore_compare.py compare REF.wav OUT.wav [--lag MS] [--from S] [--to S]
  acore_compare.py regs REF.txt OUT.txt [--tol CYCLES] [-n N]
  acore_compare.py corpus --acore BIN --oracle BIN [--corpus DIR] [--out DIR] [--lag MS] [--no-regs]

docs/acore.md "Compare tool".  A WAV is stereo s16 (one sample per 1024 ARM7 cycles).
compare: per channel (L, R): sample-exact match %, max error, RMS error relative to the reference's RMS,
  and two tolerant scores in 0..1: spectral (correlation of the magnitude spectra of 2048-sample windows,
  averaged over windows) and envelope (RMS similarity per 256-sample window).  The best lag within +-MS
  (default 10 ms) is searched (positive: OUT is later than REF) and the scores are given at lag 0 and at the
  best lag.  Exit 0 only when the sample-exact match is 100% at lag 0.
regs: two register traces ("T ADDR SIZE VALUE": T cycles in decimal, ADDR and VALUE hex), aligned by order:
  the first difference, counts, the time offsets, and per register the first divergence of its own value
  sequence.  Exit 0 when the (addr, size, value) sequences are equal (and times within --tol if given).
corpus: every DIR/*.acs through `acore_render F.acs OUT.wav --regs R.txt` and `aoracle F.acs OUT.wav --regs R.txt`,
  a table (per clip: L% R% exact, spectral, envelope at the best lag, register verdict, features from index.txt).
"""
import sys, os, wave, array, math, subprocess, argparse, glob, cmath

RATE = 32728.5
SPEC_WIN = 2048
ENV_WIN = 256
MAX_SPEC_WINDOWS = 160

# ---------------------------------------------------------------- WAV
def read_wav(path):
    """-> (rate, [chan0 list, chan1 list...]) of ints (s16)."""
    with wave.open(path, 'rb') as w:
        nch, sw, rate, n = w.getnchannels(), w.getsampwidth(), w.getframerate(), w.getnframes()
        raw = w.readframes(n)
    if sw == 2:
        a = array.array('h'); a.frombytes(raw)
        if sys.byteorder == 'big': a.byteswap()
        vals = list(a)
    elif sw == 1:
        vals = [(b - 128) << 8 for b in raw]
    else:
        raise ValueError('%s: unsupported sample width %d' % (path, sw))
    return rate, [vals[c::nch] for c in range(nch)]

def write_wav(path, chans, rate=32728):
    nch = len(chans); n = min(len(c) for c in chans) if chans else 0
    a = array.array('h', [0]) * (n * nch)
    for c in range(nch): a[c::nch] = array.array('h', chans[c][:n])
    if sys.byteorder == 'big': a.byteswap()
    with wave.open(path, 'wb') as w:
        w.setnchannels(nch); w.setsampwidth(2); w.setframerate(rate); w.writeframes(a.tobytes())

# ---------------------------------------------------------------- FFT / scores
def fft(x):
    """Iterative radix-2 FFT of a list of complex/real (len a power of 2)."""
    n = len(x); a = [complex(v) for v in x]
    j = 0
    for i in range(1, n):
        bit = n >> 1
        while j & bit: j ^= bit; bit >>= 1
        j ^= bit
        if i < j: a[i], a[j] = a[j], a[i]
    ln = 2
    while ln <= n:
        w = cmath.exp(-2j * math.pi / ln); half = ln >> 1
        tw = [1 + 0j] * half
        for k in range(1, half): tw[k] = tw[k - 1] * w
        for s in range(0, n, ln):
            for k in range(half):
                u = a[s + k]; v = a[s + k + half] * tw[k]
                a[s + k] = u + v; a[s + k + half] = u - v
        ln <<= 1
    return a

_HANN = {}

def mag_spectrum(seg):
    n = len(seg)
    if n not in _HANN: _HANN[n] = [0.5 - 0.5 * math.cos(2 * math.pi * i / n) for i in range(n)]
    h = _HANN[n]
    f = fft([seg[i] * h[i] for i in range(n)])
    return [abs(f[k]) for k in range(n // 2)]

def _corr(a, b):
    n = len(a); ma = sum(a) / n; mb = sum(b) / n
    sa = sum((v - ma) ** 2 for v in a); sb = sum((v - mb) ** 2 for v in b)
    if sa <= 0 or sb <= 0: return 1.0 if sa <= 0 and sb <= 0 else 0.0
    return sum((a[i] - ma) * (b[i] - mb) for i in range(n)) / math.sqrt(sa * sb)

def spectral_score(ref, out, win=SPEC_WIN, max_windows=MAX_SPEC_WINDOWS):
    """Mean over windows of the correlation of the magnitude spectra; a window silent in both counts 1,
    silent in one only 0.  None when the signals are too short."""
    n = min(len(ref), len(out))
    if n < win:
        if n < 64: return None
        win = 1 << (n.bit_length() - 1)
    nwin = n // win
    idx = list(range(nwin))
    if nwin > max_windows: idx = [i * nwin // max_windows for i in range(max_windows)]
    tot = 0.0
    for i in idx:
        a = ref[i * win:(i + 1) * win]; b = out[i * win:(i + 1) * win]
        sa = any(a); sb = any(b)
        if not sa and not sb: tot += 1.0
        elif sa != sb: tot += 0.0
        else: tot += max(0.0, _corr(mag_spectrum(a), mag_spectrum(b)))
    return tot / len(idx)

def envelope_score(ref, out, win=ENV_WIN):
    """Mean over windows of 1 - |rmsA - rmsB| / max(rmsA, rmsB) (windows silent in both: 1)."""
    n = min(len(ref), len(out)) // win
    if n == 0: return None
    tot = 0.0
    for i in range(n):
        a = ref[i * win:(i + 1) * win]; b = out[i * win:(i + 1) * win]
        ra = math.sqrt(sum(v * v for v in a) / win); rb = math.sqrt(sum(v * v for v in b) / win)
        m = max(ra, rb)
        tot += 1.0 if m == 0 else 1.0 - abs(ra - rb) / m
    return tot / n

def shifted(ref, out, lag):
    """Align so that out[i + lag] pairs with ref[i]: returns the two overlapping lists."""
    if lag >= 0: o = out[lag:]; r = ref[:len(o)]
    else: r = ref[-lag:]; o = out[:len(r)]
    n = min(len(r), len(o))
    return r[:n], o[:n]

def exact_stats(ref, out):
    n = min(len(ref), len(out))
    if n == 0: return dict(n=0, match=100.0, maxerr=0, rms_rel=0.0)
    same = 0; mx = 0; se = 0; sr = 0
    for i in range(n):
        r = ref[i]; d = out[i] - r
        if d == 0: same += 1
        else:
            if d < 0: d = -d
            if d > mx: mx = d
            se += d * d
        sr += r * r
    rms_ref = math.sqrt(sr / n); rms_err = math.sqrt(se / n)
    rel = rms_err / rms_ref if rms_ref > 0 else (0.0 if rms_err == 0 else float('inf'))
    return dict(n=n, match=100.0 * same / n, maxerr=mx, rms_rel=rel)

def best_lag(ref, out, max_lag, seg=2048):
    """Lag in samples (|lag| <= max_lag) minimising the summed abs difference over one segment of the
    reference taken at its first nonzero sample; 0 if there is no signal.  Ties go to the smaller |lag|."""
    if max_lag <= 0: return 0
    n = len(ref)
    start = None
    for i in range(n):
        if ref[i] != 0: start = i; break
    if start is None: return 0
    start = max(start, max_lag)
    end = min(n, start + seg, len(out) - max_lag)
    if end - start < 16: return 0
    r = ref[start:end]; best = None; bl = 0
    for lag in sorted(range(-max_lag, max_lag + 1), key=abs):
        o = out[start + lag:end + lag]
        s = 0
        for a, b in zip(r, o):
            s += abs(a - b)
            if best is not None and s >= best: break
        else:
            if best is None or s < best: best = s; bl = lag
    return bl

def compare_wavs(ref_chans, out_chans, max_lag_samples, t0=0, t1=None):
    res = []
    for c in range(min(len(ref_chans), len(out_chans))):
        ref = ref_chans[c][t0:t1]; out = out_chans[c][t0:t1]
        r = dict(ch=c, ref_n=len(ref), out_n=len(out))
        r['exact'] = exact_stats(ref, out)
        r['spec0'] = spectral_score(ref, out); r['env0'] = envelope_score(ref, out)
        lag = best_lag(ref, out, max_lag_samples)
        r['lag'] = lag
        if lag:
            a, b = shifted(ref, out, lag)
            r['exact_lag'] = exact_stats(a, b)
            r['spec'] = spectral_score(a, b); r['env'] = envelope_score(a, b)
        else:
            r['exact_lag'] = r['exact']; r['spec'] = r['spec0']; r['env'] = r['env0']
        res.append(r)
    return res

def _f(v, fmt='%.4f'): return '   -  ' if v is None else fmt % v

def cmd_compare(args):
    rr, ref = read_wav(args.a); ro, out = read_wav(args.b)
    if len(ref) != len(out): print('channel count differs: %d vs %d' % (len(ref), len(out)))
    if rr != ro: print('note: sample rates %d vs %d' % (rr, ro))
    t0 = int(args.t_from * RATE) if args.t_from else 0
    t1 = int(args.t_to * RATE) if args.t_to else None
    lagn = int(args.lag * 0.001 * RATE)
    res = compare_wavs(ref, out, lagn, t0, t1)
    names = 'LR' if len(res) == 2 else [str(i) for i in range(len(res))]
    if res and res[0]['ref_n'] != res[0]['out_n']:
        print('length: ref %d samples, out %d samples (compared over the shorter)' % (res[0]['ref_n'], res[0]['out_n']))
    ok = True
    for r in res:
        e = r['exact']
        print('%s  match %8.4f%%  maxerr %5d  rms %.4f of ref  spectral %s  envelope %s' %
              (names[r['ch']], e['match'], e['maxerr'], e['rms_rel'], _f(r['spec0']), _f(r['env0'])))
        if r['lag']:
            e = r['exact_lag']
            print('%s  best lag %+d samples (%+.2f ms): match %8.4f%%  maxerr %5d  rms %.4f  spectral %s  envelope %s' %
                  (names[r['ch']], r['lag'], r['lag'] * 1000.0 / RATE, e['match'], e['maxerr'], e['rms_rel'], _f(r['spec']), _f(r['env'])))
        if r['exact']['match'] < 100.0 or r['ref_n'] != r['out_n']: ok = False
    return 0 if ok else 1

# ---------------------------------------------------------------- register traces
def read_trace(path):
    """-> list of (t, addr, size, value)."""
    out = []
    with open(path) as f:
        for ln, line in enumerate(f, 1):
            line = line.strip()
            if not line or line.startswith('#'): continue
            p = line.split()
            if len(p) < 4: raise ValueError('%s:%d: bad trace line %r' % (path, ln, line))
            t = int(p[0], 0) if p[0][:2].lower() == '0x' else int(p[0])
            out.append((t, int(p[1], 16), int(p[2]), int(p[3], 16)))
    return out

def compare_traces(a, b, tol=None):
    """Result dict: n_a, n_b, equal (count of equal writes from the start), first (index, a_write, b_write,
    kind) or None, dt (min, max, mean of out - ref time over the equal writes), per_reg
    {addr: (na, nb, first own divergence or None)}."""
    res = dict(n_a=len(a), n_b=len(b))
    first = None; eq = 0; dts = []
    for i in range(min(len(a), len(b))):
        x, y = a[i], b[i]
        if x[1:] != y[1:]: first = (i, x, y, 'write'); break
        if tol is not None and abs(x[0] - y[0]) > tol: first = (i, x, y, 'time'); break
        eq += 1; dts.append(y[0] - x[0])
    if first is None and len(a) != len(b):
        i = min(len(a), len(b))
        first = (i, a[i] if i < len(a) else None, b[i] if i < len(b) else None, 'length')
    res['first'] = first; res['equal'] = eq
    res['dt'] = (min(dts), max(dts), sum(dts) / len(dts)) if dts else None
    sa = {}; sb = {}; per = {}
    for t, ad, sz, v in a: sa.setdefault(ad, []).append((sz, v))
    for t, ad, sz, v in b: sb.setdefault(ad, []).append((sz, v))
    for ad in sorted(set(sa) | set(sb)):
        la = sa.get(ad, []); lb = sb.get(ad, []); div = None
        for k in range(min(len(la), len(lb))):
            if la[k] != lb[k]: div = k; break
        if div is None and len(la) != len(lb): div = min(len(la), len(lb))
        per[ad] = (len(la), len(lb), div)
    res['per_reg'] = per
    return res

def _fl(x):
    return 'none' if x is None else 't=%d %08X %d %08X' % x

def cmd_regs(args):
    a = read_trace(args.a); b = read_trace(args.b)
    r = compare_traces(a, b, args.tol)
    print('writes: ref %d, out %d; equal in order from the start: %d' % (r['n_a'], r['n_b'], r['equal']))
    if r['dt']: print('time offset (out - ref) over the equal writes: min %d max %d mean %.1f cycles' % r['dt'])
    f = r['first']
    if f:
        i, x, y, kind = f
        print('first difference (%s) at write #%d:\n  ref %s\n  out %s' % (kind, i, _fl(x), _fl(y)))
    else:
        print('traces identical')
    bad = [(ad, v) for ad, v in r['per_reg'].items() if v[2] is not None]
    print('registers: %d, with a divergent own sequence: %d' % (len(r['per_reg']), len(bad)))
    for ad, (na, nb, div) in bad[:args.n]:
        print('  %08X  ref %d writes, out %d writes, first own divergence at #%d' % (ad, na, nb, div))
    if len(bad) > args.n: print('  ... %d more' % (len(bad) - args.n))
    return 0 if f is None else 1

# ---------------------------------------------------------------- corpus
def load_index(d):
    feats = {}
    for fn in ('index.txt', 'INDEX.txt', 'index.tsv'):
        p = os.path.join(d, fn)
        if os.path.exists(p):
            for line in open(p):
                line = line.strip()
                if not line or line.startswith('#'): continue
                if '|' in line:
                    # the capture's index (docs/acore.md "The capture"): `clip | frames | counts | what is heard |
                    # features`, the features `NAME COUNT, ...`: the names
                    cols = [c.strip() for c in line.split('|')]
                    names = [w for w in cols[-1].replace(',', ' ').split() if not w.isdigit()]
                    feats[os.path.splitext(os.path.basename(cols[0]))[0]] = names
                    continue
                parts = line.replace(',', ' ').replace(':', ' ').split()
                feats[os.path.splitext(os.path.basename(parts[0]))[0]] = parts[1:]
            break
    return feats

def run_tool(binpath, acs, wav, regs):
    for p in (wav, regs):
        if p and os.path.exists(p): os.remove(p)
    cmd = [binpath, acs, wav] + (['--regs', regs] if regs else [])
    try:
        r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=600)
    except (subprocess.TimeoutExpired, OSError) as e:
        return str(e)
    if r.returncode != 0 or not os.path.exists(wav):
        return 'rc=%d %s' % (r.returncode, r.stdout.decode('latin1').strip()[-80:])
    return None

def cmd_corpus(args):
    os.makedirs(args.out, exist_ok=True)
    files = sorted(glob.glob(os.path.join(args.corpus, '*.acs')))
    if not files:
        print('no .acs files in', args.corpus); return 1
    index = load_index(args.corpus); lagn = int(args.lag * 0.001 * RATE)
    rows = []
    for f in files:
        name = os.path.splitext(os.path.basename(f))[0]
        w = {k: os.path.join(args.out, '%s.%s.wav' % (name, k)) for k in ('acore', 'oracle')}
        t = {k: None if args.no_regs else os.path.join(args.out, '%s.%s.regs.txt' % (name, k)) for k in w}
        row = dict(name=name, feats=index.get(name, []), note='', res=None, reg=None)
        e = run_tool(args.oracle, f, w['oracle'], t['oracle'])
        if e: row['note'] = 'oracle failed: ' + e; rows.append(row); continue
        e = run_tool(args.acore, f, w['acore'], t['acore'])
        if e: row['note'] = 'acore failed: ' + e; rows.append(row); continue
        try:
            _, ref = read_wav(w['oracle']); _, out = read_wav(w['acore'])
            row['res'] = compare_wavs(ref, out, lagn)
            row['lens'] = (len(ref[0]) if ref else 0, len(out[0]) if out else 0)
        except Exception as ex:
            row['note'] = 'wav: %s' % ex; rows.append(row); continue
        if not args.no_regs:
            try:
                r = compare_traces(read_trace(t['oracle']), read_trace(t['acore']))
                row['reg'] = ('same %d' % r['n_a']) if r['first'] is None else \
                    'diff@%d of %d/%d' % (r['first'][0], r['n_a'], r['n_b'])
            except Exception:
                row['reg'] = 'err'
        rows.append(row)
    print('%-26s %8s %8s %6s %6s %6s %6s %5s  %-16s %s' %
          ('clip', 'L%', 'R%', 'specL', 'specR', 'envL', 'envR', 'lag', 'regs', 'features'))
    agg = {}; nfail = 0; nperfect = 0; tot = [0.0, 0]
    for row in rows:
        if row['res'] is None:
            print('%-26s %s' % (row['name'], row['note'])); nfail += 1; continue
        rs = row['res']
        def g(i, k): return rs[i][k] if i < len(rs) else None
        m0 = [rs[i]['exact']['match'] if i < len(rs) else None for i in (0, 1)]
        lag = rs[0]['lag'] if rs else 0
        print('%-26s %8s %8s %6s %6s %6s %6s %5d  %-16s %s' % (
            row['name'], _f(m0[0], '%.3f'), _f(m0[1], '%.3f'), _f(g(0, 'spec'), '%.3f'), _f(g(1, 'spec'), '%.3f'),
            _f(g(0, 'env'), '%.3f'), _f(g(1, 'env'), '%.3f'), lag, row['reg'] or '-', ' '.join(row['feats'])))
        have = [x for x in m0 if x is not None]
        avg = sum(have) / len(have) if have else 100.0
        nperfect += all(x == 100.0 for x in have) and row['lens'][0] == row['lens'][1]
        tot[0] += avg; tot[1] += 1
        for ft in row['feats'] or ['(none)']:
            a = agg.setdefault(ft, [0.0, 0]); a[0] += avg; a[1] += 1
    print('-- per feature (mean exact match %)')
    for ft in sorted(agg): print('%-26s %8.3f%%  clips %d' % (ft, agg[ft][0] / agg[ft][1], agg[ft][1]))
    if tot[1]: print('TOTAL: mean exact %.3f%% over %d clips (%d identical, %d failed)' % (tot[0] / tot[1], tot[1], nperfect, nfail))
    else: print('TOTAL: nothing compared (%d failed)' % nfail)
    return 0

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    c = sub.add_parser('compare'); c.add_argument('a'); c.add_argument('b')
    c.add_argument('--lag', type=float, default=10.0); c.add_argument('--from', dest='t_from', type=float)
    c.add_argument('--to', dest='t_to', type=float)
    g = sub.add_parser('regs'); g.add_argument('a'); g.add_argument('b')
    g.add_argument('--tol', type=int, default=None); g.add_argument('-n', type=int, default=20)
    k = sub.add_parser('corpus')
    k.add_argument('--acore', required=True); k.add_argument('--oracle', required=True)
    k.add_argument('--corpus', default='/root/ref/acorpus'); k.add_argument('--out', default='/root/mt/acore_cmp')
    k.add_argument('--lag', type=float, default=10.0); k.add_argument('--no-regs', action='store_true')
    args = ap.parse_args(argv)
    return {'compare': cmd_compare, 'regs': cmd_regs, 'corpus': cmd_corpus}[args.cmd](args)

if __name__ == '__main__':
    sys.exit(main())
