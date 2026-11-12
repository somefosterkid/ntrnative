#!/usr/bin/env python3
"""Tests for tools/rom_text_check.py: the policy on synthetic message banks of Gen 4 and Gen 5 (no ROM needed), a
synthetic NDS image through the ROM loader, and one test that takes real text out of every ROM present at run time
(never written to the repo; skipped without a ROM):

    python3 tools/rom_text_check_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import contextlib
import io
import mmap
import os
import struct
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rom_text_check as T  # noqa: E402

CODE = {c: k for k, c in T.CHARMAP.items() if c != "'"}
CODE.update({' ': 0x1DE, "'": 0x1B3, '.': 0x1AE, ',': 0x1AD, '\n': 0xE000})


def encode(s):
    out = []
    for ch in s:
        if ch == '@':   # a variable: FFFE, command, 1 param, the param
            out += [0xFFFE, 0x0100, 1, 0]
        else:
            out.append(CODE[ch])
    return out + [0xFFFF]


def bank(msgs, key=0x1234):
    """a Gen 4 message bank holding msgs, encrypted the way the game's are."""
    codes = [encode(m) for m in msgs]
    off = 4 + 8 * len(msgs)
    table, body = b'', b''
    for i, c in enumerate(codes, 1):
        k = (765 * i * key) & 0xFFFF
        k |= k << 16
        table += struct.pack('<II', (off + len(body)) ^ k, len(c) ^ k)
        ck, enc = (i * 596947) & 0xFFFF, []
        for x in c:
            enc.append(x ^ ck)
            ck = (ck + 18749) & 0xFFFF
        body += struct.pack('<%dH' % len(enc), *enc)
    return struct.pack('<HH', len(msgs), key) + table + body


def encode5(s):
    """a Gen 5 string as UTF-16 code units: @ is a variable (0xF000, code, 1 parameter, the parameter), \n is 0xFFFE."""
    out = []
    for ch in s:
        if ch == '@':
            out += [0xF000, 0x0100, 1, 0]
        elif ch == '\n':
            out.append(0xFFFE)
        else:
            out.append(ord(ch))
    return out + [0xFFFF]


def bank5(msgs, sections=1, keybase=0x1234):
    """a Gen 5 message file holding msgs in each of `sections` sections, encrypted the way the game's are (each string
    from its last character backwards, the last being 0xFFFF). A msg starting with '#' is stored as a 0xF100 string
    (compressed: not decoded, so what follows the marker is filler)."""
    nent = len(msgs)
    head = 12 + 4 * sections
    secs, body = [], b''
    for s in range(sections):
        table, data = b'', b''
        start = 4 + 8 * nent
        for i, m in enumerate(msgs):
            codes = [0xF100, 1, 2, 3, 0xFFFF] if m.startswith('#') else encode5(m)
            key = (keybase + 77 * i + s) & 0xFFFF
            enc = [0] * len(codes)
            for j in range(len(codes) - 1, -1, -1):
                enc[j] = codes[j] ^ key
                key = ((key >> 3) | (key << 13)) & 0xFFFF
            table += struct.pack('<IHH', start + len(data), len(codes), 0)
            data += struct.pack('<%dH' % len(enc), *enc)
        sec = struct.pack('<I', 4 + len(table) + len(data)) + table + data
        secs.append(head + len(body))
        body += sec
    return struct.pack('<HHII', sections, nent, len(body), 0) + struct.pack('<%dI' % sections, *secs) + body


def narc(members):
    n = len(members)
    ents, blob = b'', b''
    for m in members:
        ents += struct.pack('<II', len(blob), len(blob) + len(m))
        blob += m + b'\0' * (-len(m) % 4)
    btaf = b'BTAF' + struct.pack('<II', 12 + 8 * n, n) + ents
    btnf = b'BTNF' + struct.pack('<I', 16) + struct.pack('<IHH', 4, 0, 1)
    gmif = b'GMIF' + struct.pack('<I', 8 + len(blob)) + blob
    return b'NARC' + struct.pack('<HHIHH', 0xFFFE, 0x100, 16 + len(btaf) + len(btnf) + len(gmif), 16, 3) \
        + btaf + btnf + gmif


def nds(files):
    """a minimal NDS image (header fields 0x40.., FNT, FAT, file data) holding files: path -> bytes."""
    dirs = ['']
    for p in files:
        parts = p.split('/')[:-1]
        for k in range(1, len(parts) + 1):
            if '/'.join(parts[:k]) not in dirs:
                dirs.append('/'.join(parts[:k]))
    order = [p for d in dirs for p in sorted(files) if p.rpartition('/')[0] == d]    # file ids: dir by dir
    first, n = {}, 0
    for d in dirs:
        first[d] = n
        n += sum(1 for p in files if p.rpartition('/')[0] == d)
    subs = b''
    main = b''
    base = 8 * len(dirs)
    for d in dirs:
        main += struct.pack('<IHH', base + len(subs), first[d], len(dirs) if d == '' else dirs.index(d.rpartition('/')[0]))
        for p in sorted(files):
            if p.rpartition('/')[0] == d:
                nm = p.rpartition('/')[2].encode()
                subs += bytes([len(nm)]) + nm
        for k, sub in enumerate(dirs):
            if sub and sub.rpartition('/')[0] == d:
                nm = sub.rpartition('/')[2].encode()
                subs += bytes([0x80 | len(nm)]) + nm + struct.pack('<H', 0xF000 + k)
        subs += b'\0'
    fnt = main + subs
    fnt += b'\0' * (-len(fnt) % 4)
    fnt_off, fat_off = 0x200, 0x200 + len(fnt)
    data_off = fat_off + 8 * len(order)
    fat, blob = b'', b''
    for p in order:
        fat += struct.pack('<II', data_off + len(blob), data_off + len(blob) + len(files[p]))
        blob += files[p] + b'\0' * (-len(files[p]) % 4)
    head = bytearray(0x200)
    struct.pack_into('<IIII', head, 0x40, fnt_off, len(fnt), fat_off, len(fat))
    return bytes(head) + fnt + fat + blob


def index5(msgs):
    idx = set()
    for codes in T.messages5(bank5(msgs)):
        for r in T.message_words(codes, 5):
            idx.update(T.h(r[i:i + T.RUN]) for i in range(len(r) - T.RUN + 1))
    return idx


def index(msgs):
    idx = set()
    for codes in T.messages(bank(msgs)):
        for r in T.message_words(codes):
            idx.update(T.h(r[i:i + T.RUN]) for i in range(len(r) - T.RUN + 1))
    return idx


def spans(found):
    return [f[:3] for f in found]


def test_decode_words():
    ms = list(T.messages(bank(['Hello there, you\nare Pokémon.', 'A b@c d e f'])))
    assert T.message_words(ms[0]) == [['hello', 'there', 'you', 'are', 'pokemon']], T.message_words(ms[0])
    assert T.message_words(ms[1]) == [['a', 'b'], ['c', 'd', 'e', 'f']], T.message_words(ms[1])


def test_gen5_decode_words():
    ms = list(T.messages5(bank5(['Hello there, you\nare Pok\u00e9mon.', 'A b@c d e f', '#', 'It\u2019s the one'], sections=2)))
    assert len(ms) == 6, len(ms)   # two sections of 4 strings, the compressed one skipped in each
    assert ms[0][-1] == 0xFFFF
    assert T.message_words(ms[0], 5) == [['hello', 'there', 'you', 'are', 'pokemon']], T.message_words(ms[0], 5)
    assert T.message_words(ms[1], 5) == [['a', 'b'], ['c', 'd', 'e', 'f']], T.message_words(ms[1], 5)
    assert T.message_words(ms[2], 5) == [['its', 'the', 'one']], T.message_words(ms[2], 5)
    assert T.message_words(ms[3], 5) == T.message_words(ms[0], 5)
    assert list(T.messages5(b'')) == [] and list(T.messages5(b'\x01\x00\x05\x00' + b'\xff' * 20)) == []   # not a message file
    assert list(T.messages5(bank5(['x'])[:20])) == []                                                          # truncated


def test_gen5_scan_finds_comment():
    idx = index5(["Hey, don't touch the red switch at all."])
    f = T.scan_text('x = 1;\n// hey dont touch the red\nint y;\n', idx)
    assert spans(f) == [(2, 2, 5)], f
    assert T.scan_text('// touch the red @ switch\n', index5(['touch the red@switch'])) == []   # a variable breaks it


def run_main(args):
    with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
        return T.main(args)


def test_gen5_rom_and_main():
    """a synthetic NDS with a/0/0/2 and a/0/0/3 through rom_index and main (exit codes, several ROMs)."""
    gen5 = nds({'a/0/0/2': narc([bank5(['Please wait until the other player decides.']), b'junk']),
                'a/0/0/3': narc([bank5(['Nothing here but seven words in a row.'])]), 'a/0/1/0': b'x' * 7})
    gen4 = nds({'msgdata/msg': narc([bank(['The quick brown fox jumps over.'])])})
    with tempfile.TemporaryDirectory() as d:
        r5, r4, f = (os.path.join(d, x) for x in ('r5.nds', 'r4.nds', 'f.c'))
        Path(r5).write_bytes(gen5)
        Path(r4).write_bytes(gen4)
        idx, n, gen = T.rom_index(r5)
        assert (n, gen) == (2, 5) and len(idx) == 4 + 5, (n, gen, len(idx))
        idx4, n4, g4 = T.rom_index(r4, set(idx))
        assert (n4, g4) == (1, 4) and len(idx4) == len(idx) + 3
        Path(f).write_text('// please wait until the other player decides\n')
        assert run_main(['--rom', r5, f]) == 1
        assert run_main(['--rom', r4, f]) == 0
        assert run_main(['--rom', r4, '--rom', r5, f]) == 1
        assert run_main(['--rom', os.path.join(d, 'none.nds'), f]) == 2
        Path(f).write_text('// the quick brown fox jumps over\n')
        assert run_main(['--rom', r5, '--rom', r4, f]) == 1


def test_scan_finds_comment():
    idx = index(["Hey, don't touch the red switch at all."])
    f = T.scan_text('x = 1;\n// hey dont touch the red\nint y;\n', idx)
    assert spans(f) == [(2, 2, 5)], f
    assert T.scan_text('// hey dont touch\n', idx) == []                         # 3 words: below RUN
    assert T.scan_text('// touch the red @ switch\n', index(['touch the red@switch'])) == []   # a variable breaks it


def test_names():
    idx = index(["Hey, don't touch the red switch at all."])
    # 5 words inside one name: pret's vocabulary, not text; 6 or more in one name count
    assert T.scan_text('#define DontTouchTheRed_Switch 3\n', idx) == []
    assert spans(T.scan_text('#define Hey_DontTouchTheRedSwitch 3\n', idx)) == [(1, 1, 6)]
    assert spans(T.scan_text('int hey_dont_touch_the_red_switch_at;\n', idx)) == [(1, 1, 7)]


def test_common_words_ignored():
    idx = index(['You will have to go there now.'])
    assert T.scan_text('// you will have to go there now\n', idx) == []


def test_short_stretch_needs_cover():
    idx = index(['Please wait until the other player decides.'])
    assert spans(T.scan_text('// please wait until the other\n', idx)) == [(1, 1, 5)]                  # the whole comment
    assert T.scan_text('// we must please wait until the other side answers and then go on\n', idx) == []   # a fragment
    assert spans(T.scan_text('foo(a, b, c, d, e, f); // please wait until the other\n', idx)) == [(1, 1, 5)]   # code isn't prose
    # 6 or more words always count, wherever they are
    f = T.scan_text('// we must please wait until the other player decides and then go on\n', idx)
    assert spans(f) == [(1, 1, 7)], f
    # a comment over two lines is one stretch
    assert spans(T.scan_text('/*\n * please wait until\n * the other player\n */\n', idx)) == [(2, 3, 6)]


def test_allow():
    idx = index(['one of the best things'])
    allow = {T.h(['one', 'of', 'the', 'best'])}
    assert T.scan_text('// one of the best\n', idx, allow) == []
    assert spans(T.scan_text('// one of the best things\n', idx, allow)) == [(1, 1, 5)]
    f = T.scan_text('// one of the best things\n', idx)
    assert len(f) == 1 and len(f[0][3]) == 12
    assert T.scan_text('// one of the best things\n', idx, {f[0][3]}) == []          # by id: no text needed
    with tempfile.TemporaryDirectory() as d:
        p = os.path.join(d, 'allow.txt')
        Path(p).write_text('# comment line\n@%s # generic wording\nof the best things # generic\n' % f[0][3])
        a = T.load_allow(p)
        assert f[0][3] in a and T.h(['of', 'the', 'best', 'things']) in a
        assert T.scan_text('// one of the best things\n', idx, a) == []


def first_long_run(data):
    """the first 10 words of the first run of the ROM's messages that has them, all letters, 5 not common."""
    gen, banks = T.rom_banks(data)
    for bk in banks:
        for codes in (T.messages(bk) if gen == 4 else T.messages5(bk)):
            for r in T.message_words(codes, gen):
                r = r[:10]
                if len(r) == 10 and all(w.isascii() and w.isalpha() for w in r) \
                        and sum(1 for w in r if w not in T.COMMON) >= 5:
                    return r
    return None


def test_real_game_text():
    """text taken from each ROM now (a message, as a comment, over two lines, as names) must be found."""
    roms = [r for r in (T.find_rom(os.path.join(T.ROOT, r)) for r in T.ROMS) if os.path.exists(r)]
    if not roms:
        print('skip test_real_game_text: no ROM')
    for rom in roms:
        real_game_text(rom)


def real_game_text(rom):
    with open(rom, 'rb') as f, mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ) as data:
        run = first_long_run(data)
    assert run is not None, rom
    idx, _, _ = T.rom_index(rom)
    cases = {
        'comment': '// %s\n' % ' '.join(run),
        'id and comment': 'MSG_X_0001, // %s\n' % ' '.join(run),
        'two lines': '/*\n * %s\n * %s\n */\n' % (' '.join(run[:5]), ' '.join(run[5:])),
        'camel name': 'int %s;\n' % ''.join(w.capitalize() for w in run[:8]),
        'snake name': 'int %s;\n' % '_'.join(run[:8]),
        'quoted': 'f("%s");\n' % ' '.join(run),
    }
    for name, text in cases.items():
        with tempfile.TemporaryDirectory() as d:   # through a file, the way the tool reads them
            p = os.path.join(d, 'case.c')
            Path(p).write_text(text)
            found = T.scan_text(open(p, encoding='utf-8').read(), idx, T.load_allow())
        assert found and max(f[2] for f in found) >= 6, '%s %s: not found (%r)' % (os.path.basename(rom), name, spans(found))


def main():
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith('test_'):
            try:
                fn()
                print('ok', name)
            except AssertionError as e:
                bad += 1
                print('FAIL', name, e)
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
