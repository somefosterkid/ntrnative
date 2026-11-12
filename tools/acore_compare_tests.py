#!/usr/bin/env python3
"""Tests for tools/acore_compare.py with synthetic data (no sound core needed):

    python3 tools/acore_compare_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import sys, os, math, io, stat, tempfile, contextlib, random
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import acore_compare as C  # noqa: E402

def tone(n, freq=440.0, amp=8000, phase=0.0):
    return [int(amp * math.sin(2 * math.pi * freq * (i + phase) / 32728.0)) for i in range(n)]

def run(argv):
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        rc = C.main(argv)
    return rc, buf.getvalue()

def test_exact():
    a = tone(5000); r = C.exact_stats(a, list(a))
    assert r['match'] == 100.0 and r['maxerr'] == 0 and r['rms_rel'] == 0.0

def test_exact_partial():
    a = [100] * 10; b = [100] * 8 + [103, 96]
    r = C.exact_stats(a, b)
    assert r['match'] == 80.0 and r['maxerr'] == 4 and r['n'] == 10
    assert abs(r['rms_rel'] - math.sqrt((9 + 16) / 10) / 100) < 1e-9

def test_fft():
    x = [math.sin(2 * math.pi * 5 * i / 64) for i in range(64)]
    m = [abs(v) for v in C.fft(x)]
    assert m.index(max(m[:32])) == 5 and abs(m[5] - 32) < 1e-9

def test_spectral_same_and_different():
    a = tone(8192, 440); b = tone(8192, 440, amp=4000)
    assert C.spectral_score(a, b) > 0.99               # same sound, other level
    c = tone(8192, 1500)
    assert C.spectral_score(a, c) < 0.3                # other pitch
    assert C.spectral_score([0] * 4096, [0] * 4096) == 1.0
    assert C.spectral_score(a[:4096], [0] * 4096) == 0.0

def test_spectral_tolerates_noise():
    random.seed(1)
    a = tone(8192, 440); b = [v + random.randint(-50, 50) for v in a]
    assert C.spectral_score(a, b) > 0.95
    assert C.exact_stats(a, b)['match'] < 5.0

def test_envelope():
    a = tone(4096, amp=8000); b = tone(4096, amp=4000)
    assert abs(C.envelope_score(a, b) - 0.5) < 0.02
    assert C.envelope_score(a, a) == 1.0

def test_best_lag():
    a = [0] * 200 + tone(3000, 300)
    for lag in (0, 7, -5, 120):
        out = ([0] * lag + a) if lag >= 0 else a[-lag:]
        assert C.best_lag(a, out, 327) == lag, lag
    assert C.best_lag([0] * 100, [0] * 100, 50) == 0

def test_shifted():
    r, o = C.shifted([1, 2, 3, 4], [9, 9, 1, 2, 3, 4], 2)
    assert r == [1, 2, 3, 4] and o == [1, 2, 3, 4]
    r, o = C.shifted([9, 9, 1, 2], [1, 2, 3], -2)
    assert r == [1, 2] and o == [1, 2]

def test_wav_roundtrip_and_compare_cli():
    with tempfile.TemporaryDirectory() as d:
        d = Path(d); L = tone(4096); R = tone(4096, 220)
        C.write_wav(str(d / 'a.wav'), [L, R]); C.write_wav(str(d / 'b.wav'), [L, R])
        rate, ch = C.read_wav(str(d / 'a.wav'))
        assert rate == 32728 and ch == [L, R]
        rc, out = run(['compare', str(d / 'a.wav'), str(d / 'b.wav')])
        assert rc == 0 and 'match 100.0000%' in out and out.count('maxerr     0') == 2, out
        # shifted by 12 samples: not exact, the lag search finds it
        C.write_wav(str(d / 'c.wav'), [[0] * 12 + L, [0] * 12 + R])
        rc, out = run(['compare', str(d / 'a.wav'), str(d / 'c.wav')])
        assert rc == 1 and 'best lag +12' in out and 'length:' in out, out
        # quieter: near-exact fails, the tolerant scores stay high
        C.write_wav(str(d / 'q.wav'), [[v // 2 for v in L], [v // 2 for v in R]])
        rc, out = run(['compare', str(d / 'a.wav'), str(d / 'q.wav')])
        assert rc == 1, out
        res = C.compare_wavs([L, R], [[v // 2 for v in L], [v // 2 for v in R]], 0)
        assert res[0]['spec'] > 0.99 and abs(res[0]['env'] - 0.5) < 0.03

def trace(rows):
    return ''.join('%d %08X %d %08X\n' % r for r in rows)

def test_regs():
    base = [(1000, 0x04000404, 4, 0x02100000), (1000, 0x04000408, 2, 0xFE00), (1000, 0x04000400, 4, 0x8800007F),
            (175000, 0x04000500, 2, 0x807F)]
    with tempfile.TemporaryDirectory() as d:
        d = Path(d)
        (d / 'a.txt').write_text(trace(base)); (d / 'b.txt').write_text(trace(base))
        rc, out = run(['regs', str(d / 'a.txt'), str(d / 'b.txt')])
        assert rc == 0 and 'traces identical' in out and 'equal in order from the start: 4' in out, out
        # same writes, 300 cycles later: identical, with the offset shown; --tol 100 makes it a difference
        late = [(r[0] + 300,) + r[1:] for r in base]
        (d / 'c.txt').write_text(trace(late))
        rc, out = run(['regs', str(d / 'a.txt'), str(d / 'c.txt')])
        assert rc == 0 and 'min 300 max 300' in out, out
        rc, out = run(['regs', str(d / 'a.txt'), str(d / 'c.txt'), '--tol', '100'])
        assert rc == 1 and 'first difference (time) at write #0' in out, out
        # a changed value in the third write
        bad = list(base); bad[2] = (1000, 0x04000400, 4, 0x8800003F)
        (d / 'e.txt').write_text(trace(bad))
        rc, out = run(['regs', str(d / 'a.txt'), str(d / 'e.txt')])
        assert rc == 1 and 'first difference (write) at write #2' in out and '8800007F' in out and '8800003F' in out, out
        assert 'divergent own sequence: 1' in out and '04000400' in out
        # a missing last write
        (d / 'f.txt').write_text(trace(base[:3]))
        rc, out = run(['regs', str(d / 'a.txt'), str(d / 'f.txt')])
        assert rc == 1 and '(length) at write #3' in out, out

def test_regs_interleaving():
    # the same per-register sequences in another order: the order differs, no register diverges
    a = [(1, 0x04000400, 4, 1), (2, 0x04000410, 4, 2)]; b = [(1, 0x04000410, 4, 2), (2, 0x04000400, 4, 1)]
    r = C.compare_traces(a, b)
    assert r['first'][3] == 'write' and r['equal'] == 0
    assert all(v[2] is None for v in r['per_reg'].values())

MOCK = '''#!/usr/bin/env python3
import sys, math, os
sys.path.insert(0, %(dir)r)
import acore_compare as C
mode = %(mode)r
args = sys.argv[1:]
acs, wav = args[0], args[1]
regs = args[args.index('--regs') + 1] if '--regs' in args else None
name = os.path.basename(acs)
if mode == 'fail' or (mode == 'failbad' and name.startswith('bad')):
    sys.stderr.write('mock failure\\n'); sys.exit(3)
n = 6000
L = [int(8000 * math.sin(i * 0.05)) for i in range(n)]
R = [int(6000 * math.sin(i * 0.031)) for i in range(n)]
if mode == 'noisy':
    L = [v + (i %% 7 - 3) for i, v in enumerate(L)]
if mode == 'late':
    L = [0] * 9 + L; R = [0] * 9 + R
C.write_wav(wav, [L, R])
if regs:
    rows = [(1000, 0x04000400, 4, 0x8000007F), (2000, 0x04000500, 2, 0x807F)]
    if mode == 'noisy': rows[1] = (2000, 0x04000500, 2, 0x8070)
    open(regs, 'w').write(''.join('%%d %%08X %%d %%08X\\n' %% r for r in rows))
'''

def mkmock(path, mode):
    Path(path).write_text(MOCK % dict(dir=str(Path(__file__).resolve().parent), mode=mode))
    os.chmod(path, os.stat(path).st_mode | stat.S_IXUSR)

def test_corpus_with_mock_binaries():
    with tempfile.TemporaryDirectory() as d:
        d = Path(d); (d / 'corpus').mkdir()
        for n in ('title', 'cry', 'sfx'): (d / 'corpus' / (n + '.acs')).write_bytes(b'ACST\1\0\0\0')
        (d / 'corpus' / 'index.txt').write_text('# clip features\ntitle: bgm, sseq\ncry: wave-out\nsfx.acs ssar sseq\n')
        mkmock(d / 'oracle', 'exact'); mkmock(d / 'same', 'exact'); mkmock(d / 'noisy', 'noisy'); mkmock(d / 'late', 'late')
        mkmock(d / 'fail', 'fail')
        rc, out = run(['corpus', '--acore', str(d / 'same'), '--oracle', str(d / 'oracle'), '--corpus', str(d / 'corpus'),
                       '--out', str(d / 'o1')])
        assert rc == 0 and 'TOTAL: mean exact 100.000% over 3 clips (3 identical, 0 failed)' in out, out
        assert 'same 2' in out and 'bgm' in out and 'wave-out' in out
        rc, out = run(['corpus', '--acore', str(d / 'noisy'), '--oracle', str(d / 'oracle'), '--corpus', str(d / 'corpus'),
                       '--out', str(d / 'o2')])
        assert '0 identical' in out and 'diff@1' in out, out
        rc, out = run(['corpus', '--acore', str(d / 'late'), '--oracle', str(d / 'oracle'), '--corpus', str(d / 'corpus'),
                       '--out', str(d / 'o3'), '--no-regs'])
        assert '0 identical' in out and '    9  ' in out, out
        rc, out = run(['corpus', '--acore', str(d / 'fail'), '--oracle', str(d / 'oracle'), '--corpus', str(d / 'corpus'),
                       '--out', str(d / 'o4')])
        assert 'acore failed: rc=3' in out and '3 failed' in out, out
        rc, out = run(['corpus', '--acore', str(d / 'same'), '--oracle', str(d / 'fail'), '--corpus', str(d / 'corpus'),
                       '--out', str(d / 'o5')])
        assert 'oracle failed' in out, out
        (d / 'empty').mkdir()
        rc, out = run(['corpus', '--acore', str(d / 'same'), '--oracle', str(d / 'oracle'), '--corpus', str(d / 'empty'),
                       '--out', str(d / 'o6')])
        assert rc == 1 and 'no .acs files' in out

def test_index_pipe_format():
    with tempfile.TemporaryDirectory() as d:
        Path(d, 'index.txt').write_text('# clip | frames | counts | heard | features\n'
                                        'title.acs | 0-1000 | 1032 lists | the music, then START | '
                                        'LOCK_CHANNEL 1, PREPARE_SEQ 2, STOP_SEQ 2\n'
                                        'cry: wave-out\n')
        f = C.load_index(d)
        assert f == {'title': ['LOCK_CHANNEL', 'PREPARE_SEQ', 'STOP_SEQ'], 'cry': ['wave-out']}, f

def main():
    fails = 0
    for name, fn in sorted(globals().items()):
        if name.startswith('test_') and callable(fn):
            try:
                fn(); print('ok', name)
            except Exception as e:  # noqa: BLE001
                import traceback; traceback.print_exc()
                print('FAIL', name, repr(e)[:300]); fails += 1
    return 1 if fails else 0

if __name__ == '__main__':
    sys.exit(main())
