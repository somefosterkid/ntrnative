import sys, wave, numpy as np
w = wave.open(sys.argv[1]); sr = w.getframerate()
a = np.frombuffer(w.readframes(w.getnframes()), dtype='<i2').reshape(-1, 2).astype(np.float32).mean(1)
step = float(sys.argv[2]) if len(sys.argv) > 2 else 1.0
h = int(step * sr)
out = []
for i in range(0, len(a) - h + 1, h):
    out.append('%d' % np.sqrt((a[i:i+h]**2).mean()))
print('sr', sr, 'secs', len(a)/sr, 'rms/step:', ' '.join(out))
