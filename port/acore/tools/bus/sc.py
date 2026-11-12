"""sc.py REF.wav@mark TEST.wav@mark S0 'TICK:SPEED,...' [SECS]: live speed changes. The music alone (SOLO=music) of a run whose
speed changes at game ticks, against the 1x music from the same start: per 0.5 s window the best lag (+-3000 samples) and the
correlation after it. A skipped or doubled stretch of music shows as a lag step, a gap or level jump as a corr dip at the
transition. Also: the longest run of near-silence and the biggest sample-to-sample jump of the test (vs the ref)."""
import os, sys, wave
import numpy as np

def load(p):
    off = 0
    if '@' in p:
        p, o = p.split('@'); off = int(o)
    w = wave.open(p)
    a = np.frombuffer(w.readframes(w.getnframes()), dtype='<i2').reshape(-1, 2).astype(np.float64).mean(1)
    return a[off:]

SR = 32728
ref = load(sys.argv[1]); tst = load(sys.argv[2])
S0 = int(sys.argv[3])
tr = [(int(t), int(s)) for t, s in (x.split(':') for x in sys.argv[4].split(','))] if len(sys.argv) > 4 and sys.argv[4] else []
secs = float(sys.argv[5]) if len(sys.argv) > 5 else 28.0
TICK = 560190 / 1024.0   # DS frames per game tick

def onset(a):
    h = 655
    for i in range(0, len(a) - h, h):
        if np.sqrt((a[i:i + h] ** 2).mean()) > 300: return i
    return 0
o1, o2 = onset(ref), onset(tst)
# test-timeline position (frames since its mark) of each transition: the game ticks since the mark / speed. The mark is the BGM start tick
# (given by the marks as WAV positions, so just use total ticks: pos(T) = sum TICK / S) minus the mark's own position
def pos_of_tick(T):
    p, t, s = 0.0, 0, S0
    for (tt, ss) in tr:
        if T <= tt: break
        p += (tt - t) * TICK / s; t = tt; s = ss
    return p + (T - t) * TICK / s
print('onsets', o1, o2, ' transitions at WAV frames', [(t, s, int(pos_of_tick(t))) for t, s in tr])
mark = int(sys.argv[2].split('@')[1]) if '@' in sys.argv[2] else 0
W = SR // 2
n = int(min(len(ref) - o1, len(tst) - o2, secs * SR)) // W
rows = []
for k in range(1, n - 1):
    a = ref[o1 + k * W: o1 + (k + 1) * W]
    seg = tst[o2 + k * W - 3000: o2 + (k + 1) * W + 3000]
    L = 1 << 16
    c = np.fft.irfft(np.fft.rfft(seg, L) * np.conj(np.fft.rfft(a, L)), L)[:6001]
    j = int(np.argmax(c)); lag = j - 3000
    b = tst[o2 + k * W + lag: o2 + (k + 1) * W + lag]
    corr = float((a * b).sum() / np.sqrt((a * a).sum() * (b * b).sum() + 1e-9))
    rows.append((k * 0.5, lag, corr))
tp = [mark + o2 + int(0) for _ in tr]
print('window(s) lag(samples) corr')
trt = [(pos_of_tick(t) - mark) / SR for t, s in tr]
for (t, lag, corr) in rows:
    near = any(abs(t - x + (o2 / SR)) < 1.0 for x in trt)
    print('%5.1f %+5d %.3f%s' % (t, lag, corr, '  <- transition' if near else ''))
lags = np.array([r[1] for r in rows]); cs = np.array([r[2] for r in rows])
print('lag range %+d..%+d samples, corr min %.3f median %.3f' % (lags.min(), lags.max(), cs.min(), np.median(cs)))
def silence_run(a):
    q = np.abs(a) < 20
    best = cur = 0
    for v in q:
        cur = cur + 1 if v else 0
        best = max(best, cur)
    return best
t = tst[o2: o2 + int(secs * SR)]; r = ref[o1: o1 + int(secs * SR)]
print('longest near-silence run: ref %.1f ms, test %.1f ms; max |sample step|: ref %.0f test %.0f' % (silence_run(r) * 1000.0 / SR, silence_run(t) * 1000.0 / SR, np.abs(np.diff(r)).max(), np.abs(np.diff(t)).max()))
