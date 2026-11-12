# ring_pitch.py OUT.wav TONE_HZ LOG [skip_s]: the instantaneous frequency of a BWPORT_AUDIO_TONE run's BWPORT_AUDIO_OUT, per
# segment of the log's 'audio: mode' lines (BWPORT_AUDIOLOG=1), in cents: the fast part (vibrato) and the slow part (offsets,
# glides). docs/acore.md "The ring's rate, cuts and fades". Needs numpy (~/bw-cache/sndvenv).
import sys, wave, re
import numpy as np
fn, tone, log = sys.argv[1], float(sys.argv[2]), sys.argv[3]
skip = float(sys.argv[4]) if len(sys.argv) > 4 else 0.5
w = wave.open(fn)
fs = w.getframerate()
x = np.frombuffer(w.readframes(w.getnframes()), dtype=np.int16).reshape(-1, 2)[:, 0].astype(np.float64)
marks = [(0, 'x1')]
for line in open(log, errors='replace'):
    m = re.search(r'audio: mode speed (\d) unpaced (\d) at out frame (\d+)', line)
    if m:
        marks.append((int(m.group(3)), 'x%s%s' % (m.group(1), ' max' if m.group(2) == '1' else '')))
marks.append((len(x), 'end'))
def analytic(v):
    n = len(v); X = np.fft.fft(v); h = np.zeros(n); h[0] = 1
    if n % 2 == 0: h[n // 2] = 1; h[1:n // 2] = 2
    else: h[1:(n + 1) // 2] = 2
    return np.fft.ifft(X * h)
print('file %s rate %d tone %.1f' % (fn, fs, tone))
print('%-8s %6s %6s %8s %8s %8s %8s %8s %6s %6s' % ('segment', 'start', 'len', 'median_c', 'fast_sd', 'fast_p99', 'slow_p99', 'slow_rng', 'gaps', 'clean'))
for i in range(len(marks) - 1):
    a, b = marks[i][0] + int(skip * fs), marks[i + 1][0]
    if b - a < fs // 2:
        continue
    seg = x[a:b]
    z = analytic(seg)
    amp = np.abs(z)
    ph = np.unwrap(np.angle(z))
    f = np.diff(ph) * fs / (2 * np.pi)
    k = int(0.002 * fs)  # 2 ms average
    fsm = np.convolve(f, np.ones(k) / k, mode='valid')
    bad = amp[k // 2:k // 2 + len(fsm)] < 0.7 * 8000
    d = int(0.08 * fs)  # 80 ms around a dropout or splice
    bad = np.convolve(bad.astype(float), np.ones(2 * d + 1), mode='same') > 0
    bad[:fs // 20] = True; bad[-fs // 20:] = True
    gaps = int(np.sum(np.diff((amp < 0.3 * 8000).astype(int)) == 1))
    c = 1200 * np.log2(np.maximum(fsm, 1) / tone)
    # fast part (above ~10 Hz: vibrato, the frame rhythm) and slow part (below ~5 Hz: offsets and glides)
    m = int(0.1 * fs)
    slow = np.convolve(c, np.ones(m) / m, mode='same')
    fast = (c - slow)[~bad]
    sl = slow[~bad]
    if len(fast) < fs // 10:
        print('%-8s no clean signal (gaps %d)' % (marks[i][1], gaps)); continue
    good = ~bad
    print('%-8s %6.1f %6.1f %8.2f %8.2f %8.2f %8.2f %8.2f %6d %5.0f%%' % (marks[i][1], a / fs, (b - a) / fs, np.median(sl), fast.std(),
          np.percentile(fast, 99) - np.percentile(fast, 1), np.percentile(sl, 99) - np.percentile(sl, 1), sl.max() - sl.min(), gaps,
          100.0 * good.mean()))
