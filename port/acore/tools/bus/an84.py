#!/usr/bin/env python3
"""an84.py REF.wav TEST.wav [SECS] : compare a music-alone recording at speed S (TEST) with the one at 1x (REF).
Aligns on the music onset, then the best lag; prints
  corr    : normalized waveform correlation at the best lag over the whole window (1.0 = same waveform)
  corr_w  : median / min of the per-1s-window correlations (each window re-lagged +-30 ms)
  spec    : median / min per-window (1 s) cosine similarity of log-spectra (0..1)
  lsd     : median log-spectral distance in dB (0 = same)
  env     : correlation of the 10 ms RMS envelopes
  tempo   : onset-envelope tempo ratio TEST/REF (an77.best_ratio, 1.000 = same tempo)
  decay   : median time from an envelope peak to -10 dB, TEST over REF
  vib     : dominant 3-10 Hz modulation of the strongest partial's pitch/level, REF and TEST
"""
import sys, wave
import numpy as np

SR = 48000


def load(p):
    global SR
    off = 0
    if '@' in p:
        p, o = p.split('@')
        off = int(o)
    w = wave.open(p, 'rb')
    SR = w.getframerate()
    n = w.getnframes()
    a = np.frombuffer(w.readframes(n), dtype='<i2').reshape(-1, w.getnchannels()).astype(np.float32)
    return a.mean(1)[off:]


def onset(x, thr=300.0):
    # first sample where the 20 ms RMS exceeds thr
    h = int(0.02 * SR)
    n = len(x) // h
    r = np.sqrt((x[:n * h].reshape(n, h) ** 2).mean(1))
    i = np.nonzero(r > thr)[0]
    return int(i[0]) * h if len(i) else 0


def xcorr_lag(a, b, maxlag):
    """lag k maximizing sum a[k+j] b[j] over |k| <= maxlag; returns k, normalized corr"""
    n = min(len(a), len(b))
    a = a[:n]
    b = b[:n]
    L = 1 << int(np.ceil(np.log2(2 * n)))
    c = np.fft.irfft(np.fft.rfft(a, L) * np.conj(np.fft.rfft(b, L)), L)
    pos = c[:maxlag + 1]
    neg = c[L - maxlag:]
    cc = np.concatenate([neg, pos])
    lags = np.arange(-maxlag, maxlag + 1)
    j = int(np.argmax(cc))
    return int(lags[j])


def ncorr(a, b):
    n = min(len(a), len(b))
    a = a[:n] - a[:n].mean()
    b = b[:n] - b[:n].mean()
    d = np.sqrt((a * a).sum() * (b * b).sum()) + 1e-9
    return float((a * b).sum() / d)


def stft_mag(x, win=2048, hop=1024):
    n = (len(x) - win) // hop
    if n <= 0:
        return np.zeros((0, win // 2 + 1))
    idx = np.arange(win)[None, :] + hop * np.arange(n)[:, None]
    return np.abs(np.fft.rfft(x[idx] * np.hanning(win)[None, :], axis=1))


def rms_env(x, ms=10):
    h = int(ms * SR / 1000)
    n = len(x) // h
    return np.sqrt((x[:n * h].reshape(n, h) ** 2).mean(1)), 1000.0 / ms


def decay_time(x):
    e, fps = rms_env(x, 10)
    e = e + 1e-3
    db = 20 * np.log10(e)
    res = []
    for i in range(3, len(e) - 40):
        if e[i] > e[i - 1] * 1.5 and e[i] >= e[i + 1] and e[i] > 300:
            thr = db[i] - 10
            for k in range(1, 40):
                if db[i + k] < thr:
                    res.append(k / fps)
                    break
            else:
                res.append(40 / fps)
    return float(np.median(res)) if res else float('nan'), len(res)


def vib(x):
    # strongest partial in 150-1500 Hz of the whole window; its level modulation spectrum (3-10 Hz) over 0.5 s hops
    win, hop = 4096, 480
    m = stft_mag(x, win, hop)
    if len(m) < 64:
        return float('nan')
    df = SR / win
    k0, k1 = int(150 / df), int(1500 / df)
    sp = m[:, k0:k1].mean(0)
    k = k0 + int(np.argmax(sp))
    # frequency track of that partial by parabolic interpolation of the magnitude peak near k
    tr = []
    for f in m:
        j = k - 3 + int(np.argmax(f[k - 3:k + 4]))
        a, b, c = f[j - 1], f[j], f[j + 1]
        d = (a - c) / (2 * (a - 2 * b + c) + 1e-9)
        tr.append((j + d) * df)
    tr = np.array(tr)
    fps = SR / hop
    out = []
    for sig in (tr, m[:, k]):
        s = sig - np.convolve(sig, np.ones(int(fps)) / int(fps), mode='same')
        s = s[int(fps):-int(fps)] * np.hanning(len(s) - 2 * int(fps))
        S = np.abs(np.fft.rfft(s, 1 << 14))
        fr = np.fft.rfftfreq(1 << 14, 1 / fps)
        sel = (fr >= 3) & (fr <= 10)
        out.append(float(fr[sel][np.argmax(S[sel])]))
    return (round(float(k * df), 1), out[0], out[1])


def tempo(ref, test):
    import os; sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import an77
    an77.SR = SR
    r, fps = an77.onset_env(ref)
    t, _ = an77.onset_env(test)
    (rr, lag, c), _ = an77.best_ratio(r, t, fps, 0.9, 1.1, 0.002, maxlag=2.0)
    return rr, c


def main():
    ref = load(sys.argv[1])
    test = load(sys.argv[2])
    secs = float(sys.argv[3]) if len(sys.argv) > 3 else 20.0
    o1, o2 = onset(ref), onset(test)
    n = int(secs * SR)
    r = ref[o1:o1 + n]
    t = test[o2:o2 + n]
    n = min(len(r), len(t))
    # coarse onset alignment is within a game frame or so; find the best lag on the first 10 s
    lag = xcorr_lag(r[:min(n, 10 * SR)], t[:min(n, 10 * SR)], int(0.15 * SR))
    # lag k: r[k + j] ~ t[j]
    if lag >= 0:
        r2, t2 = r[lag:], t
    else:
        r2, t2 = r, t[-lag:]
    n = min(len(r2), len(t2))
    r2, t2 = r2[:n], t2[:n]
    print(f'onsets {o1/SR:.3f}s {o2/SR:.3f}s lag {lag*1000.0/SR:+.1f}ms  compared {n/SR:.1f}s  rms ref {np.sqrt((r2**2).mean()):.0f} test {np.sqrt((t2**2).mean()):.0f}')
    print(f'corr   {ncorr(r2, t2):.4f}')
    import os
    W = int(SR * float(os.environ.get("WIN", "1")))
    cw = []
    for i in range(0, n - W, W):
        a, b = r2[i:i + W], t2[i:i + W]
        # per-window re-lag +-30 ms
        k = xcorr_lag(a, b, int(0.03 * SR))
        if k >= 0:
            cw.append(ncorr(a[k:], b))
        else:
            cw.append(ncorr(a, b[-k:]))
    cw = np.array(cw)
    print(f'corr_w median {np.median(cw):.4f} min {cw.min():.4f} p10 {np.percentile(cw,10):.4f}  ({len(cw)} windows)')
    mr, mt = stft_mag(r2), stft_mag(t2)
    m = min(len(mr), len(mt))
    lr, lt = np.log1p(mr[:m] / 30.0), np.log1p(mt[:m] / 30.0)
    cos = (lr * lt).sum(1) / (np.sqrt((lr ** 2).sum(1) * (lt ** 2).sum(1)) + 1e-9)
    print(f'spec   median {np.median(cos):.4f} min {cos.min():.4f} p10 {np.percentile(cos,10):.4f}')
    ref_db = 20 * np.log10(mr[:m] + 1.0)
    tst_db = 20 * np.log10(mt[:m] + 1.0)
    sel = slice(int(100 / (SR / 2048)), int(8000 / (SR / 2048)))
    lsd = np.sqrt(((ref_db[:, sel] - tst_db[:, sel]) ** 2).mean(1))
    print(f'lsd    median {np.median(lsd):.2f} dB max {lsd.max():.2f} dB')
    er, _ = rms_env(r2)
    et, _ = rms_env(t2)
    k = min(len(er), len(et))
    print(f'env    corr {ncorr(er[:k], et[:k]):.4f}')
    try:
        tr, tc = tempo(r2, t2)
        print(f'tempo  ratio {tr:.4f} (onset corr {tc:.3f})')
    except Exception as ex:
        print('tempo  n/a', ex)
    dr, nr = decay_time(r2)
    dt, nt = decay_time(t2)
    print(f'decay  ref {dr*1000:.0f} ms ({nr} peaks) test {dt*1000:.0f} ms ({nt})  ratio {dt/dr if dr else float("nan"):.3f}')
    print(f'vib    ref {vib(r2)}  test {vib(t2)}')


main()
