#!/usr/bin/python3
"""wav_stats.py A.wav [B.wav ...] [--from S --to S]: peak, samples at full scale, RMS, DC, energy above 10 kHz and 16 kHz
(share of the total, dB), the spectral flatness above 12 kHz. Reads 16-bit stereo WAVs (the BWPORT_AUDIO_WAV form: the
header says 32728 Hz whatever the rate; --rate overrides)."""
import sys, struct, numpy as np
def load(p):
    b = open(p, 'rb').read(); n = (len(b) - 44) // 4
    a = np.frombuffer(b[44:44 + n * 4], dtype='<i2').reshape(-1, 2).astype(np.float64)
    return a, struct.unpack('<I', b[24:28])[0]
def main():
    args = sys.argv[1:]; t0 = t1 = None; rate = None; files = []
    while args:
        x = args.pop(0)
        if x == '--from': t0 = float(args.pop(0))
        elif x == '--to': t1 = float(args.pop(0))
        elif x == '--rate': rate = float(args.pop(0))
        else: files.append(x)
    for p in files:
        a, r = load(p); r = rate or r
        a = a[int((t0 or 0) * r):int(t1 * r) if t1 else None]
        m = a.mean(axis=1)
        full = int(((np.abs(a) >= 32767).sum()))
        N = 4096; w = np.hanning(N); acc = np.zeros(N // 2 + 1); k = 0
        for i in range(0, len(m) - N, N // 2):
            acc += np.abs(np.fft.rfft(m[i:i + N] * w)) ** 2; k += 1
        f = np.fft.rfftfreq(N, 1 / r); tot = acc.sum() + 1e-9
        def share(lo): return 10 * np.log10(acc[f >= lo].sum() / tot + 1e-15)
        hf = acc[f >= 12000]; flat = np.exp(np.log(hf + 1e-9).mean()) / (hf.mean() + 1e-9)
        print(f"{p.split('/')[-1]}: {len(a)/r:.1f}s peak {int(np.abs(a).max())} fullscale {full} rms {np.sqrt((a**2).mean()):.0f} "
              f"dc {a.mean():.1f} >10k {share(10000):.1f} dB >16k {share(16000):.1f} dB flat12k {flat:.3f} "
              f"(rate {r:.0f})")
        edges = [0, 2000, 5000, 8000, 10000, 12000, 14000, 16000]
        pw = acc / k / (N * (w ** 2).sum() / N) / N * 2  # mean-square per bin, in sample units^2
        bands = []
        for lo, hi in zip(edges, edges[1:]):
            sel = (f >= lo) & (f < hi)
            bands.append(10 * np.log10(pw[sel].sum() + 1e-9))
        print('   band dB (0-2-5-8-10-12-14-16 kHz): ' + ' '.join(f'{b:.1f}' for b in bands))
main()
