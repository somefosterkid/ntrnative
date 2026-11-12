#!/usr/bin/env python3
"""mkacs: write synthetic .acs streams (docs/acore.md "The stream: .acs").  Pure Python 3.

  mkacs.py --all DIR            hw_pcm16 hw_pcm8 hw_adpcm hw_psg hw_noise hw_mix drv_note drv_cry drv_psg drv_noise
  mkacs.py NAME [NAME...] --out DIR
  mkacs.py --list

The hw_* streams (mode=hw) are register writes only: sample data MEM'd at 0x02100000, a SOUNDxCNT start, one to
sixteen channels.  The drv_* streams (mode=driver) are what the ARM9 sends: a hand-made SSEQ + SBNK + SWAR (a
one-note sequence on a looping PCM16 sine) started with START_SEQ; the wave-out cry (LOCK_CHANNEL,
SETUP_CHANNEL_PCM, START_TIMER); PSG and noise channels set up directly.

Library use: Acs() builds a stream (info / mem / cmds / wake / regs / end, .bytes()); build_sseq, build_swar,
build_sbnk make the three DS files; the SBNK is in its DS memory layout (the wave archive link filled in at 0x18).
Written from GBATEK (56-72) and the public SDAT format document (SSEQ / SBNK / SWAR / SWAV).
"""
import sys, os, struct, math, argparse

CPU_HZ = 33513982
FRAME = 560190                 # ARM7 cycles per frame
DATA_BASE = 0x02100000         # the address plan: data 0x02100000-0x0237FFFF
WAVEOUT_BASE = 0x02230000      # wave-out sample ring
IO_BASE = 0x04000000

# SNDCommandID (lib/nitro/sdk/nitro/snd/common/command.h)
CMD = {name: i for i, name in enumerate((
    'START_SEQ STOP_SEQ PREPARE_SEQ START_PREPARED_SEQ PAUSE_SEQ SKIP_SEQ PLAYER_PARAM TRACK_PARAM MUTE_TRACK '
    'ALLOCATABLE_CHANNEL PLAYER_LOCAL_VAR PLAYER_GLOBAL_VAR START_TIMER STOP_TIMER SETUP_CHANNEL_PCM '
    'SETUP_CHANNEL_PSG SETUP_CHANNEL_NOISE SETUP_CAPTURE SETUP_ALARM CHANNEL_TIMER CHANNEL_VOLUME CHANNEL_PAN '
    'SURROUND_DECAY MASTER_VOLUME MASTER_PAN OUTPUT_SELECTOR LOCK_CHANNEL UNLOCK_CHANNEL STOP_UNLOCKED_CHANNEL '
    'SHARED_WORK INVALIDATE_SEQ INVALIDATE_BANK INVALIDATE_WAVE READ_DRIVER_INFO').split())}

# ---------------------------------------------------------------- the stream
class Acs:
    """Chunks in the order added (events at one time run in file order)."""
    def __init__(self):
        self.chunks = []

    def _add(self, tag, body):
        assert len(tag) == 4
        self.chunks.append((tag, bytes(body)))
        return self

    def info(self, **kv):
        return self._add(b'INFO', ''.join('%s=%s\n' % (k, v) for k, v in kv.items()).encode())

    def mem(self, time, addr, data):
        return self._add(b'MEM ', struct.pack('<QI', time, addr) + bytes(data))

    def cmds(self, time, frame, cmds):
        """cmds: [(id_or_name, a0, a1, a2, a3)] (missing args 0)."""
        body = struct.pack('<QII', time, frame, len(cmds))
        for c in cmds:
            cid = CMD[c[0]] if isinstance(c[0], str) else c[0]
            a = list(c[1:]) + [0] * (4 - len(c[1:]))
            body += struct.pack('<5I', cid, *[x & 0xFFFFFFFF for x in a])
        return self._add(b'CMDS', body)

    def wake(self, time):
        return self._add(b'WAKE', struct.pack('<Q', time))

    def regs(self, time, writes):
        """writes: [(addr, value, size)]; size 1, 2 or 4 (a bare (addr, value) is 4 bytes)."""
        body = struct.pack('<QI', time, len(writes))
        for w in writes:
            body += struct.pack('<3I', w[0], w[1], w[2] if len(w) > 2 else 4)
        return self._add(b'REGS', body)

    def end(self, time):
        return self._add(b'END ', struct.pack('<Q', time))

    def bytes(self):
        out = bytearray(b'ACST' + struct.pack('<I', 1))
        for tag, body in self.chunks:
            out += tag + struct.pack('<I', len(body)) + body + b'\0' * (-len(body) & 3)
        return bytes(out)

    def write(self, path):
        with open(path, 'wb') as f:
            f.write(self.bytes())

# ---------------------------------------------------------------- the DS files
def _fhdr(magic, size, nblocks=1):
    return magic + struct.pack('<IIHH', 0x0100FEFF, size, 16, nblocks)

def varlen(v):
    """SSEQ variable-length number (7-bit groups, high bit = more)."""
    out = [v & 0x7F]
    v >>= 7
    while v:
        out.append(0x80 | (v & 0x7F)); v >>= 7
    return bytes(reversed(out))

def build_sseq(events):
    """events: the track's bytes (one track, from offset 0x1C).  -> the SSEQ file."""
    data = bytes(events)
    size = 0x1C + len(data)
    return _fhdr(b'SSEQ', size) + b'DATA' + struct.pack('<II', size - 16, 0x1C) + data

def seq_note_track(prg=0, key=60, vel=int(__import__("os").environ.get("VEL","100")), dur=48, tempo=120, vol=127, pan=64, tail_rest=48):
    """A one-track sequence: tempo, program, volume, pan, one note, a rest, end of track."""
    return (b'\xE1' + struct.pack('<H', tempo) + b'\x81' + varlen(prg) + bytes((0xC1, vol, 0xC0, pan))
            + bytes((key, vel)) + varlen(dur) + b'\x80' + varlen(tail_rest) + b'\xFF')

def pack_swav_info(wtype, loop, rate, loop_start_words, length_words):
    """The 12-byte wave header (type, loop flag, rate, time, loop offset in words, non-loop length in words)."""
    return struct.pack('<BBHHHI', wtype, 1 if loop else 0, rate, (CPU_HZ // 2) // rate, loop_start_words, length_words)

def build_swar(waves):
    """waves: [(wtype, loop, rate, loop_start_words, length_words, data_bytes)] -> the SWAR file (offsets from the
    file start; the data padded to words)."""
    n = len(waves)
    head = 0x38 + 4 + 4 * n
    blobs = []; offs = []; pos = head
    for w in waves:
        d = bytes(w[5]) + b'\0' * (-len(w[5]) & 3)
        blob = pack_swav_info(*w[:5]) + d
        offs.append(pos); blobs.append(blob); pos += len(blob)
    body = b''.join(blobs)
    hdr = _fhdr(b'SWAR', pos) + b'DATA' + struct.pack('<I', pos - 16) + b'\0' * 32 + struct.pack('<I', n)
    return hdr + b''.join(struct.pack('<I', o) for o in offs) + body

def build_sbnk(insts, swar_addrs=()):
    """insts: a list indexed by program number: None (empty) or a tuple
         (1, swav, swar, key, a, d, s, r, pan)          PCM          (types 4 and 5 the same fields)
         (2, duty, 0, key, a, d, s, r, pan)             PSG;  (3, ...) noise
         (16, lo, hi, [(swav, swar, key, a, d, s, r, pan), ...])    a drum set
         (17, [region upper notes], [(swav, swar, key, a, d, s, r, pan), ...])    a key split
    swar_addrs: the DS addresses of the wave archives (links 0-3) to write at 0x18 in the DS memory layout
    ({SNDWaveArc *waveArc, next} x 4).  -> the SBNK bytes as the driver sees them in memory."""
    n = len(insts)
    base = 0x38 + 4 + 4 * n
    recs = []; ents = []; pos = base

    def sub(p, t):  # a 12-byte instrument record: u16 type, wave[2], key, A, D, S, R, pan
        return struct.pack('<HHHBBBBBB', t, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7])
    for it in insts:
        if it is None:
            ents.append(struct.pack('<BHB', 0, 0, 0)); continue
        t = it[0]
        if t in (1, 2, 3, 4, 5):
            rec = struct.pack('<HHBBBBBB', it[1], it[2], it[3], it[4], it[5], it[6], it[7], it[8])
        elif t == 16:
            rec = bytes((it[1], it[2])) + b''.join(sub(p, 1) for p in it[3])
        elif t == 17:
            regs = (list(it[1]) + [0] * 8)[:8]
            rec = bytes(regs) + b''.join(sub(p, 1) for p in it[2])
        else:
            raise ValueError('instrument type %d' % t)
        ents.append(struct.pack('<BHB', t, pos, 0)); recs.append(rec); pos += len(rec)
    body = b''.join(recs)
    size = pos + (-pos & 3)
    links = b''.join(struct.pack('<II', swar_addrs[i] if i < len(swar_addrs) else 0, 0) for i in range(4))
    hdr = _fhdr(b'SBNK', size) + b'DATA' + struct.pack('<I', size - 16) + links + struct.pack('<I', n)
    return hdr + b''.join(ents) + body + b'\0' * (size - pos)

def sine16(n, amp=0x6000, cycles=1):
    return b''.join(struct.pack('<h', int(amp * math.sin(2 * math.pi * cycles * i / n))) for i in range(n))

# ---------------------------------------------------------------- hardware registers (GBATEK 57-61)
def ch_reg(ch, ofs): return IO_BASE + 0x400 + 0x10 * ch + ofs

def cnt_word(vol=127, shift=0, hold=0, pan=64, duty=0, repeat=1, fmt=1, start=1):
    """SOUNDxCNT: volume 0-6, shift 8-9, hold 15, pan 16-22, duty 24-26, repeat 27-28 (0 manual, 1 loop, 2 one-shot),
    format 29-30, start/busy 31."""
    return (vol | shift << 8 | hold << 15 | pan << 16 | duty << 24 | repeat << 27 | fmt << 29 | start << 31)

SOUNDCNT = IO_BASE + 0x500      # u16: master volume 0-6, outputs 8-11, ch1/ch3 no-mix 12/13, enable 15
SOUNDBIAS = IO_BASE + 0x504

def hw_channel_writes(ch, sad, tmr, pnt, length, cnt):
    """The order the SDK's driver writes a channel in: SAD, TMR, PNT, LEN, then CNT with the start bit."""
    return [(ch_reg(ch, 4), sad, 4), (ch_reg(ch, 8), tmr, 2), (ch_reg(ch, 10), pnt, 2),
            (ch_reg(ch, 12), length, 4), (ch_reg(ch, 0), cnt, 4)]

def hw_master(vol=127):
    return [(SOUNDCNT, 0x8000 | vol, 2)]

HW_FRAMES = 6

def _hw(name, setup):
    a = Acs().info(source='mkacs', name=name, mode='hw', frames=HW_FRAMES)
    setup(a)
    return a.end(HW_FRAMES * FRAME)

def make_hw_pcm16():
    def s(a):
        a.mem(0, DATA_BASE, sine16(64))
        a.regs(0, hw_master() + hw_channel_writes(0, DATA_BASE, 0xFE00, 0, 32, cnt_word(fmt=1, repeat=1, pan=64)))
    return _hw('hw_pcm16', s)

def make_hw_pcm8():
    def s(a):
        data = bytes((i * 4 - 128) & 0xFF for i in range(64))
        a.mem(0, DATA_BASE, data)
        a.regs(0, hw_master() + hw_channel_writes(1, DATA_BASE, 0xFF00, 0, 16, cnt_word(fmt=0, repeat=1, pan=32)))
    return _hw('hw_pcm8', s)

def make_hw_adpcm():
    def s(a):
        # header: PCM16 sample 0 and step index 0; then 3 words of nibbles (rising then falling codes)
        nib = [1, 2, 3, 4, 5, 6, 7, 7, 9, 10, 11, 12, 13, 14, 15, 15] * 3
        words = b''.join(struct.pack('<I', sum(nib[w * 8 + i] << (4 * i) for i in range(8))) for w in range(6))
        a.mem(0, DATA_BASE, struct.pack('<I', 0) + words)
        a.regs(0, hw_master() + hw_channel_writes(2, DATA_BASE, 0xFE00, 1, 6, cnt_word(fmt=2, repeat=1)))
    return _hw('hw_adpcm', s)

def make_hw_psg():
    def s(a):
        a.regs(0, hw_master() + [(ch_reg(8, 8), 0xF000, 2), (ch_reg(8, 0), cnt_word(fmt=3, duty=3, repeat=0), 4)])
    return _hw('hw_psg', s)

def make_hw_noise():
    def s(a):
        a.regs(0, hw_master() + [(ch_reg(14, 8), 0xFC00, 2), (ch_reg(14, 0), cnt_word(fmt=3, repeat=0), 4)])
    return _hw('hw_noise', s)

def make_hw_mix():
    """Channels 0 (PCM16 left), 1 (PCM8 right), 2 (ADPCM centre), 8 (PSG), 14 (noise) at once, with dividers, a
    master volume of 100 and a mid-stream pan change."""
    def s(a):
        a.mem(0, DATA_BASE, sine16(64))
        a.mem(0, DATA_BASE + 0x1000, bytes((i * 8 - 128) & 0xFF for i in range(32)))
        w = hw_master(100)
        w += hw_channel_writes(0, DATA_BASE, 0xFE00, 0, 32, cnt_word(fmt=1, pan=0, vol=100))
        w += hw_channel_writes(1, DATA_BASE + 0x1000, 0xFF00, 0, 8, cnt_word(fmt=0, pan=127, shift=1))
        w += [(ch_reg(8, 8), 0xF800, 2), (ch_reg(8, 0), cnt_word(fmt=3, duty=5, repeat=0, shift=2, vol=64), 4)]
        w += [(ch_reg(14, 8), 0xFC00, 2), (ch_reg(14, 0), cnt_word(fmt=3, repeat=0, shift=3, vol=40, pan=100), 4)]
        a.regs(0, w)
        a.regs(2 * FRAME, [(ch_reg(0, 2), 64, 1)])      # CNT byte 2 = pan
    return _hw('hw_mix', s)

# ---------------------------------------------------------------- driver streams
SSEQ_ADDR = DATA_BASE
SBNK_ADDR = DATA_BASE + 0x1000
SWAR_ADDR = DATA_BASE + 0x2000
FRAMES_DRV = 60

def make_drv_note():
    """A one-note sequence on a hand-made bank: program 0 = a looping PCM16 sine at key 60, START_SEQ on player 0."""
    swar = build_swar([(1, True, 32000, 0, 32, sine16(64))])
    sbnk = build_sbnk([(1, 0, 0, 60, 100, 0, 127, 127, 64)], [SWAR_ADDR])
    sseq = build_sseq(seq_note_track())
    a = Acs().info(source='mkacs', name='drv_note', mode='driver', frames=FRAMES_DRV)
    a.mem(0, SSEQ_ADDR, sseq).mem(0, SBNK_ADDR, sbnk).mem(0, SWAR_ADDR, swar)
    a.cmds(0, 0, [('MASTER_VOLUME', 127), ('START_SEQ', 0, SSEQ_ADDR + 0x1C, 0, SBNK_ADDR)])
    a.wake(0)
    return a.end(FRAMES_DRV * FRAME)

def pcm_cmd(ch, fmt, addr, loop, loop_start, loop_len, vol, shift, timer, pan):
    """SND_SetupChannelPcm's packing (snd_interface.c)."""
    return ('SETUP_CHANNEL_PCM', timer << 16 | ch, addr, vol << 24 | shift << 22 | loop_len,
            loop << 26 | fmt << 24 | pan << 16 | loop_start)

def make_drv_cry():
    """A wave-out channel the way pm_voice.c drives it: LOCK_CHANNEL, SETUP_CHANNEL_PCM, START_TIMER, then
    CHANNEL_TIMER / CHANNEL_VOLUME changes a few frames later."""
    a = Acs().info(source='mkacs', name='drv_cry', mode='driver', frames=FRAMES_DRV)
    a.mem(0, WAVEOUT_BASE, sine16(256))
    ch = 4
    a.cmds(0, 0, [('MASTER_VOLUME', 127), ('LOCK_CHANNEL', 1 << ch, 0),
                  pcm_cmd(ch, 1, WAVEOUT_BASE, 2, 0, 128, 127, 0, 0xFA00, 64), ('START_TIMER', 1 << ch, 0, 0, 0)])
    a.wake(0)
    a.cmds(6 * FRAME, 6, [('CHANNEL_TIMER', 1 << ch, 0xF800), ('CHANNEL_VOLUME', 1 << ch, 80, 0)])
    a.wake(6 * FRAME)
    return a.end(FRAMES_DRV * FRAME)

def make_drv_psg():
    """Direct PSG channel (the SDK's SND_SetupChannelPsg packing from the public NitroSDK headers; check against
    the oracle: piece 15's open question)."""
    a = Acs().info(source='mkacs', name='drv_psg', mode='driver', frames=FRAMES_DRV)
    a.cmds(0, 0, [('MASTER_VOLUME', 127), ('LOCK_CHANNEL', 1 << 8, 0),
                  ('SETUP_CHANNEL_PSG', 8, 0 << 8 | 100, 0xF000 << 8 | 64, 3), ('START_TIMER', 1 << 8, 0, 0, 0)])
    a.wake(0)
    return a.end(FRAMES_DRV * FRAME)

def make_drv_noise():
    a = Acs().info(source='mkacs', name='drv_noise', mode='driver', frames=FRAMES_DRV)
    a.cmds(0, 0, [('MASTER_VOLUME', 127), ('LOCK_CHANNEL', 1 << 14, 0),
                  ('SETUP_CHANNEL_NOISE', 14, 0 << 8 | 100, 0xFC00 << 8 | 64, 0), ('START_TIMER', 1 << 14, 0, 0, 0)])
    a.wake(0)
    return a.end(FRAMES_DRV * FRAME)

STREAMS = {n[5:]: f for n, f in sorted(globals().items()) if n.startswith('make_') and callable(f)}

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('names', nargs='*'); ap.add_argument('--all', metavar='DIR'); ap.add_argument('--out')
    ap.add_argument('--list', action='store_true')
    args = ap.parse_args(argv)
    if args.list:
        print('\n'.join(STREAMS)); return 0
    out = args.all or args.out
    names = list(STREAMS) if args.all else args.names
    if not out or not names:
        ap.print_usage(); return 2
    for n in names:
        if n not in STREAMS:
            print('mkacs: unknown stream %r (have: %s)' % (n, ' '.join(STREAMS))); return 2
    os.makedirs(out, exist_ok=True)
    for n in names:
        p = os.path.join(out, n + '.acs')
        STREAMS[n]().write(p)
        print(p)
    return 0

if __name__ == '__main__':
    sys.exit(main())
