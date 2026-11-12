#!/usr/bin/env python3
"""Tests for mkacs.py and acs_dump.py:

    python3 port/acore/tools/acs_tools_tests.py        # "ok" per test, exit 1 on a failure

The last test renders every mkacs stream with build/acore/acore_render when it is built (skipped otherwise).
"""
import sys, os, struct, io, subprocess, tempfile, contextlib
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import mkacs as M  # noqa: E402
import acs_dump as D  # noqa: E402

def dump_text(path, *flags):
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        rc = D.main([str(path), *flags])
    return rc, buf.getvalue()

def test_container():
    a = M.Acs().info(source='t', mode='hw').mem(5, 0x02100000, b'abc').cmds(7, 0, [('START_SEQ', 0, 1, 2, 3), (23, 127)])
    a.wake(9).regs(11, [(0x04000500, 0x807F, 2), (0x04000404, 0x02100000)]).end(1000)
    raw = a.bytes()
    assert raw[:8] == b'ACST\1\0\0\0' and len(raw) % 4 == 0
    p = D.parse_acs(raw)
    t = [c['tag'] for c in p['chunks']]
    assert t == ['INFO', 'MEM', 'CMDS', 'WAKE', 'REGS', 'END']
    c = p['chunks']
    assert c[0]['kv'] == {'source': 't', 'mode': 'hw'}
    assert (c[1]['time'], c[1]['addr'], c[1]['data']) == (5, 0x02100000, b'abc')
    assert c[2]['cmds'] == [(0, [0, 1, 2, 3]), (23, [127, 0, 0, 0])] and c[2]['frame'] == 0
    assert c[3]['time'] == 9
    assert c[4]['writes'] == [(0x04000500, 0x807F, 2), (0x04000404, 0x02100000, 4)]
    assert c[5]['time'] == 1000

def test_bad_streams():
    for bad, msg in ((b'XXXX\1\0\0\0', 'magic'), (b'ACST\2\0\0\0', 'version'), (b'ACST\1\0\0\0WAKE', 'truncated'),
                     (b'ACST\1\0\0\0WAKE\x08\0\0\0\0\0', 'past the end'), (b'ACST\1\0\0\0CMDS\x10\0\0\0' + bytes(8) + struct.pack('<II', 0, 5), 'fit')):
        try:
            D.parse_acs(bad)
        except D.AcsError as e:
            assert msg in str(e), (msg, str(e))
        else:
            raise AssertionError('accepted ' + msg)

def test_swar_layout():
    w = M.build_swar([(1, True, 32000, 0, 8, bytes(range(32))), (0, False, 16000, 0, 2, bytes(8))])
    assert w[:4] == b'SWAR' and struct.unpack_from('<I', w, 8)[0] == len(w)
    assert struct.unpack_from('<I', w, 0x38)[0] == 2
    o0, o1 = struct.unpack_from('<II', w, 0x3C)
    assert o0 == 0x44 and o1 == 0x44 + 12 + 32
    wt, loop, rate, tm, ls, ln = struct.unpack_from('<BBHHHI', w, o0)
    assert (wt, loop, rate, tm, ls, ln) == (1, 1, 32000, 16756991 // 32000, 0, 8)
    assert w[o0 + 12:o0 + 16] == bytes(range(4)) and len(w) == o1 + 12 + 8

def test_sbnk_layout():
    b = M.build_sbnk([(1, 3, 0, 60, 255, 0, 127, 127, 64), None, (16, 10, 11, [(0, 0, 60, 1, 2, 3, 4, 64)] * 2)], [0x02102000])
    assert b[:4] == b'SBNK' and struct.unpack_from('<I', b, 8)[0] == len(b) and len(b) % 4 == 0
    assert struct.unpack_from('<II', b, 0x18) == (0x02102000, 0)          # link 0 (DS layout)
    assert struct.unpack_from('<I', b, 0x38)[0] == 3
    e = [struct.unpack_from('<BHB', b, 0x3C + 4 * i) for i in range(3)]
    assert e[0][0] == 1 and e[1] == (0, 0, 0) and e[2][0] == 16
    assert struct.unpack_from('<HHBBBBBB', b, e[0][1]) == (3, 0, 60, 255, 0, 127, 127, 64)
    assert b[e[2][1]:e[2][1] + 2] == bytes((10, 11))
    assert e[2][1] + 2 + 24 <= len(b)

def test_sseq_layout():
    s = M.build_sseq(M.seq_note_track(key=64, dur=300))
    assert s[:4] == b'SSEQ' and struct.unpack_from('<I', s, 8)[0] == len(s) and s[16:20] == b'DATA'
    assert struct.unpack_from('<I', s, 24)[0] == 0x1C and s[0x1C] == 0xE1
    assert M.varlen(0) == b'\0' and M.varlen(127) == b'\x7f' and M.varlen(128) == b'\x81\x00' and M.varlen(300) == b'\x82\x2c'
    assert b'\x40\x64\x82\x2c' in s and s[-1] == 0xFF

def test_all_streams_parse_and_dump():
    with tempfile.TemporaryDirectory() as d:
        assert M.main(['--all', d]) == 0
        names = sorted(p.stem for p in Path(d).glob('*.acs'))
        assert names == sorted(M.STREAMS) and len(names) == 10, names
        for n in names:
            p = D.parse_acs((Path(d) / (n + '.acs')).read_bytes())
            tags = [c['tag'] for c in p['chunks']]
            assert tags[0] == 'INFO' and tags[-1] == 'END', n
            kv = p['chunks'][0]['kv']
            assert kv['mode'] in ('hw', 'driver') and kv['name'] == n
            rc, out = dump_text(Path(d) / (n + '.acs'), '--cmds', '--mem', '--regs')
            assert rc == 0 and 'length:' in out, n
            assert (kv['mode'] == 'hw') == ('REGS' in tags), n
        rc, out = dump_text(Path(d) / 'drv_note.acs', '--cmds')
        assert 'START_SEQ' in out and 'MASTER_VOLUME' in out and 'SSEQ size' in out and 'SBNK size' in out and 'SWAR size' in out, out
        assert 'player 0 base 02100000 offset 0 bank 02101000' in out and 'wakeups: 1' in out, out
        assert 'memory regions: 02100000-' in out
        rc, out = dump_text(Path(d) / 'drv_cry.acs', '--cmds')
        assert 'SETUP_CHANNEL_PCM' in out and 'ch 4 timer FA00 data 02230000 vol 127 shift 0 len 128 | loop 2 fmt PCM16 pan 64' in out, out
        assert 'LOCK_CHANNEL' in out and 'wakeups: 2' in out
        rc, out = dump_text(Path(d) / 'hw_pcm16.acs', '--regs')
        assert 'SOUND0SAD' in out and 'SOUND0CNT' in out and 'fmt PCM16 start 1' in out and 'SOUNDCNT' in out, out
        rc, out = dump_text(Path(d) / 'hw_mix.acs', '--regs')
        assert 'SOUND14CNT' in out and 'SOUND0CNT+2' in out, out
        rc, out = dump_text(Path(d) / 'hw_pcm16.acs', '--json')
        assert rc == 0 and '"tag": "REGS"' in out

def test_cli_errors():
    assert M.main(['nonesuch', '--out', '/tmp']) == 2
    assert M.main([]) == 2
    with tempfile.TemporaryDirectory() as d:
        (Path(d) / 'x.acs').write_bytes(b'nope')
        assert D.main([str(Path(d) / 'x.acs')]) == 1
        assert D.main([str(Path(d) / 'missing.acs')]) == 1

def test_render_with_acore_when_built():
    exe = HERE.parent.parent.parent / 'build' / 'acore' / 'acore_render'
    if not exe.exists():
        print('  (skipped: %s is not built)' % exe); return
    with tempfile.TemporaryDirectory() as d:
        M.main(['--all', d])
        for n in M.STREAMS:
            r = subprocess.run([str(exe), str(Path(d) / (n + '.acs')), str(Path(d) / (n + '.wav')), '--regs', str(Path(d) / (n + '.txt'))],
                               stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=120)
            assert r.returncode == 0 and (Path(d) / (n + '.wav')).stat().st_size > 44, (n, r.stdout[-200:])

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
