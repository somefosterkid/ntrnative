import sys, wave, numpy as np

SR = 48000

def load(p):
    w = wave.open(p, 'rb')
    global SR
    SR = w.getframerate()
    n = w.getnframes()
    a = np.frombuffer(w.readframes(n), dtype='<i2').reshape(-1, w.getnchannels()).astype(np.float32)
    return a

def mono(a):
    return a.mean(1)

def stft_mag(x, win=1024, hop=256):
    n = (len(x) - win) // hop
    if n <= 0:
        return np.zeros((0, win // 2 + 1))
    idx = np.arange(win)[None, :] + hop * np.arange(n)[:, None]
    fr = x[idx] * np.hanning(win)[None, :]
    return np.abs(np.fft.rfft(fr, axis=1))

def onset_env(x, hop=256):
    m = stft_mag(x, 1024, hop)
    k = int(5000 / (SR / 1024))
    m = np.log1p(m[:, :k] / 50.0)
    d = np.maximum(m[1:] - m[:-1], 0).sum(1)
    d = d - np.convolve(d, np.ones(40) / 40, mode='same')   # remove the slow level
    d = np.maximum(d, 0)
    return d, SR / hop

def seg(a, t0, t1):
    return a[int(t0 * SR):int(t1 * SR)]

def best_ratio(ref, test, fps, rlo, rhi, rstep, maxlag=12.0):
    """test plays r times faster than ref: ref(t) ~ test(t / r). Returns (r, lag_s, corr) of the best match."""
    ref = (ref - ref.mean()) / (ref.std() + 1e-9)
    best = (0, 0, -1)
    curve = []
    ml = int(maxlag * fps)
    for r in np.arange(rlo, rhi + rstep / 2, rstep):
        # resample test to the ref's time axis: sample times t/r
        tt = np.arange(len(test)) * r              # test index i happens at ref time i*r
        n = int(tt[-1]) + 1
        te = np.interp(np.arange(n), tt, test)
        te = (te - te.mean()) / (te.std() + 1e-9)
        # normalized cross-correlation by FFT over all lags
        L = 1 << int(np.ceil(np.log2(len(ref) + len(te))))
        c = np.fft.irfft(np.fft.rfft(ref, L) * np.conj(np.fft.rfft(te, L)), L)
        # lag k: ref[k + j] ~ te[j]
        pos = c[:min(ml, len(ref))]
        neg = c[L - min(ml, len(te)):]
        cc = np.concatenate([neg, pos])
        lags = np.concatenate([np.arange(-len(neg), 0), np.arange(len(pos))])
        # normalize by the overlap length
        ov = np.minimum(len(ref) - np.maximum(lags, 0), len(te) + np.minimum(lags, 0))
        ok = ov > 0.5 * min(len(ref), len(te))
        v = np.where(ok, cc / np.maximum(ov, 1), -1)
        j = int(np.argmax(v))
        curve.append((r, v[j]))
        if v[j] > best[2]:
            best = (r, lags[j] / fps, float(v[j]))
    return best, curve

def refine_ratio(ref, test, fps, r0, span, step):
    return best_ratio(ref, test, fps, r0 - span, r0 + span, step)

def avg_spectrum(x, win=8192):
    m = stft_mag(x, win, win // 2)
    return np.sqrt((m ** 2).mean(0)), SR / win

def peaks(sp, df, n=8, lo=80, hi=4000):
    k0, k1 = int(lo / df), int(hi / df)
    s = sp[k0:k1]
    idx = [i for i in range(1, len(s) - 1) if s[i] > s[i - 1] and s[i] >= s[i + 1]]
    idx.sort(key=lambda i: -s[i])
    sel = []
    for i in idx:
        if all(abs(i - j) > 6 for j in sel):
            sel.append(i)
        if len(sel) >= n:
            break
    return sorted(round((i + k0) * df, 1) for i in sel)

def freq_scale(sp1, sp2, df, lo=100, hi=3500):
    """f such that sp2(f * x) ~ sp1(x): the pitch ratio of 2 relative to 1"""
    k0, k1 = int(lo / df), int(hi / df)
    a = np.log1p(sp1 / 20.0)
    b = np.log1p(sp2 / 20.0)
    best = (0, -2)
    for f in np.arange(0.95, 1.0501, 0.001):
        xs = np.arange(k0, k1)
        bb = np.interp(xs * f, np.arange(len(b)), b[:len(b)])
        aa = a[k0:k1]
        c = np.corrcoef(aa, bb)[0, 1]
        if c > best[1]:
            best = (f, c)
    return best

def lsd(x1, x2, t1, t2, dur, off=0.0):
    """log-spectral distance of two (time aligned) mono segments"""
    a = seg(x1, t1, t1 + dur)
    b = seg(x2, t2 + off, t2 + off + dur)
    n = min(len(a), len(b))
    ma = np.log10(stft_mag(a[:n], 2048, 1024) + 10)[:, 5:300]
    mb = np.log10(stft_mag(b[:n], 2048, 1024) + 10)[:, 5:300]
    return float(np.sqrt(((20 * (ma - mb)) ** 2).mean()))

def mod_spectrum(x, fb=(4, 12, 12, 40)):
    """share of the 10 ms-RMS envelope's variance in 4-12 Hz and 12-40 Hz (a warble shows up there)"""
    hop = int(SR * 0.01)
    n = len(x) // hop
    e = np.sqrt((x[:n * hop].reshape(n, hop) ** 2).mean(1)) + 1e-3
    e = np.log(e)
    e = e - e.mean()
    sp = np.abs(np.fft.rfft(e * np.hanning(len(e)))) ** 2
    fr = np.fft.rfftfreq(len(e), 0.01)
    tot = sp[(fr > 0.5) & (fr < 50)].sum()
    r = [sp[(fr >= lo) & (fr < hi)].sum() / tot for lo, hi in ((4, 12), (12, 40))]
    return r

def clicks(x, win=480):
    """largest |second difference| against the local RMS (a click is a jump a smooth signal doesn't make)"""
    d2 = np.abs(np.diff(x, 2))
    n = len(d2) // win
    loc = np.sqrt((x[:n * win].reshape(n, win) ** 2).mean(1)) + 30
    m = d2[:n * win].reshape(n, win).max(1) / loc
    return float(m.max()), float(np.percentile(m, 99.9))

def zero_gaps(x, minms=20, around=2.0):
    z = np.abs(x) < 1
    idx = np.flatnonzero(np.diff(np.concatenate(([0], z.astype(np.int8), [0]))))
    runs = []
    for st, en in zip(idx[::2], idx[1::2]):
        if (en - st) > minms * 1e-3 * SR and st > around * SR:
            hop = 480
            pre = np.sqrt((x[max(0, st - SR // 2):st] ** 2).mean()) if st > 0 else 0
            post = np.sqrt((x[en:en + SR // 2] ** 2).mean()) if en < len(x) else 0
            if pre > 50 and post > 50:
                runs.append((st / SR, (en - st) / SR))
    return runs
