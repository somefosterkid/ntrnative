"""ev.py REF.wav@mark TEST.wav@mark [S] : sound-effect events (SOLO=rest recordings): bursts of sound separated by >= 40 ms of
silence; prints the REF events (start s, length s) next to the TEST events and the length ratio REF/TEST for events that
pair up in order (same count in the window)."""
import sys, wave
import numpy as np

def load(p):
    off = 0
    if '@' in p:
        p, o = p.split('@'); off = int(o)
    w = wave.open(p)
    sr = w.getframerate()
    a = np.frombuffer(w.readframes(w.getnframes()), dtype='<i2').reshape(-1, 2).astype(np.float64).mean(1)
    return a[off:], sr

def events(a, sr, thr=120.0, gap=0.040, tmax=60.0):
    h = int(0.005 * sr)
    n = min(len(a), int(tmax * sr)) // h
    r = np.sqrt((a[:n * h].reshape(n, h) ** 2).mean(1))
    on = r > thr
    ev = []
    i = 0
    while i < n:
        if on[i]:
            j = i
            last = i
            while j < n and j - last <= int(gap / 0.005):
                if on[j]: last = j
                j += 1
            ev.append((i * 0.005, (last - i + 1) * 0.005, float(r[i:last + 1].max())))
            i = last + 1
        else:
            i += 1
    return ev

ref, sr = load(sys.argv[1]); tst, _ = load(sys.argv[2])
S = float(sys.argv[3]) if len(sys.argv) > 3 else 0
tmax = float(sys.argv[4]) if len(sys.argv) > 4 else 60.0
er = events(ref, sr, tmax=tmax); et = events(tst, sr, tmax=tmax / max(S, 1) + 1)
print('REF %d events, TEST %d events' % (len(er), len(et)))
big = [e for e in er if e[1] >= 0.25]
print('REF events >= 0.25 s:', ' '.join('%.2f@%.2f' % (e[1], e[0]) for e in big[:12]))
bt = [e for e in et if e[1] >= 0.25 / max(S, 1) * 0.8]
print('TEST events:', ' '.join('%.2f@%.2f' % (e[1], e[0]) for e in bt[:16]))
if len(er) == len(et):
    rat = [a[1] / b[1] for a, b in zip(er, et) if a[1] >= 0.2]
    print('paired %d events, REF/TEST length: median %.2f (want %.1f) min %.2f max %.2f' % (len(rat), np.median(rat), S, min(rat), max(rat)))

# pairing by time: a REF event at t pairs with the TEST event that starts closest to t / S (within 60 ms)
if S:
    rows = []
    for a in er:
        if a[1] < 0.25: continue
        b = min(et, key=lambda e: abs(e[0] - a[0] / S))
        if abs(b[0] - a[0] / S) < 0.06:
            rows.append((a[0], a[1], b[1], a[1] / b[1]))
    if rows:
        rs = np.array([r[3] for r in rows])
        print('time-paired %d events >= 0.25 s: REF/TEST length median %.2f mean %.2f min %.2f max %.2f (want %.1f)' % (len(rs), np.median(rs), rs.mean(), rs.min(), rs.max(), S))
        print('  ' + ' '.join('%.2f->%.2f' % (r[1], r[2]) for r in rows[:14]))
