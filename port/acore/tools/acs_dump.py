#!/usr/bin/env python3
"""acs_dump: list an .acs stream (docs/acore.md "The stream: .acs").  Pure Python 3.

  acs_dump.py FILE.acs [--cmds] [--mem] [--regs] [--json]

Always: the INFO text, then one line per chunk (index, tag, time in cycles and frames, a summary), then a summary:
chunk counts, the WAKE times, the main-memory regions the MEM chunks write (merged ranges), the stream's length.
--cmds   every command of a CMDS chunk by name with its four arguments (and the decoded fields of the common ones)
--mem    for a MEM chunk: the file magic and size when the bytes are an SDAT-family file, else the first bytes
--regs   every register write of a REGS chunk with the register's name (SOUNDxCNT/SAD/TMR/PNT/LEN, SOUNDCNT, ...)
--json   the parsed chunks as JSON instead
Exit 1 on a malformed stream (the error names the byte offset).
"""
import sys, os, struct, json, argparse

FRAME = 560190
CMD_NAMES = ('START_SEQ STOP_SEQ PREPARE_SEQ START_PREPARED_SEQ PAUSE_SEQ SKIP_SEQ PLAYER_PARAM TRACK_PARAM MUTE_TRACK '
             'ALLOCATABLE_CHANNEL PLAYER_LOCAL_VAR PLAYER_GLOBAL_VAR START_TIMER STOP_TIMER SETUP_CHANNEL_PCM '
             'SETUP_CHANNEL_PSG SETUP_CHANNEL_NOISE SETUP_CAPTURE SETUP_ALARM CHANNEL_TIMER CHANNEL_VOLUME CHANNEL_PAN '
             'SURROUND_DECAY MASTER_VOLUME MASTER_PAN OUTPUT_SELECTOR LOCK_CHANNEL UNLOCK_CHANNEL STOP_UNLOCKED_CHANNEL '
             'SHARED_WORK INVALIDATE_SEQ INVALIDATE_BANK INVALIDATE_WAVE READ_DRIVER_INFO').split()
FORMATS = ('PCM8', 'PCM16', 'ADPCM', 'PSG/NOISE')
MAGICS = (b'SSEQ', b'SSAR', b'SBNK', b'SWAR', b'SWAV', b'STRM', b'SDAT')

class AcsError(Exception):
    pass

def parse_acs(data):
    """-> dict(version, chunks=[dict(tag, offset, ...)]).  Per tag: INFO text/kv; MEM time addr data; CMDS time frame
    cmds=[(id, [a0..a3])]; WAKE time; REGS time writes=[(addr, value, size)]; END time."""
    if len(data) < 8 or data[:4] != b'ACST':
        raise AcsError('not an ACST stream (bad magic)')
    ver, = struct.unpack_from('<I', data, 4)
    if ver != 1:
        raise AcsError('unsupported version %d' % ver)
    pos = 8; chunks = []
    while pos < len(data):
        if pos + 8 > len(data):
            raise AcsError('truncated chunk header at offset %d' % pos)
        tag = data[pos:pos + 4].decode('latin1').strip(); size, = struct.unpack_from('<I', data, pos + 4)
        body = data[pos + 8:pos + 8 + size]
        if len(body) < size:
            raise AcsError('chunk %s at offset %d: size %d runs past the end' % (tag, pos, size))
        c = dict(tag=tag.strip(), offset=pos, size=size)
        try:
            if tag == 'INFO':
                c['text'] = body.decode('latin1')
                c['kv'] = dict(l.split('=', 1) for l in c['text'].splitlines() if '=' in l)
            elif tag == 'MEM':
                c['time'], c['addr'] = struct.unpack_from('<QI', body); c['data'] = body[12:]
            elif tag == 'CMDS':
                c['time'], c['frame'], n = struct.unpack_from('<QII', body)
                if 16 + 20 * n > size: raise struct.error('n=%d commands do not fit' % n)
                c['cmds'] = [(struct.unpack_from('<I', body, 16 + 20 * i)[0], list(struct.unpack_from('<4I', body, 20 + 20 * i)))
                             for i in range(n)]
            elif tag == 'WAKE':
                c['time'], = struct.unpack_from('<Q', body)
            elif tag == 'REGS':
                c['time'], n = struct.unpack_from('<QI', body)
                if 12 + 12 * n > size: raise struct.error('n=%d writes do not fit' % n)
                c['writes'] = [struct.unpack_from('<3I', body, 12 + 12 * i) for i in range(n)]
            elif tag == 'END':
                c['time'], = struct.unpack_from('<Q', body)
            else:
                c['unknown'] = True
        except struct.error as e:
            raise AcsError('chunk %s at offset %d: %s' % (tag, pos, e))
        chunks.append(c)
        pos += 8 + size + (-size & 3)
    return dict(version=ver, chunks=chunks)

def reg_name(addr):
    """Name of a sound register (GBATEK 57-60), or hex."""
    o = addr - 0x04000400
    if 0 <= o < 0x100:
        ch, f = divmod(o, 16)
        names = {0: 'CNT', 1: 'CNT+1', 2: 'CNT+2', 3: 'CNT+3', 4: 'SAD', 8: 'TMR', 10: 'PNT', 12: 'LEN'}
        return 'SOUND%d%s' % (ch, names.get(f, '+%X' % f))
    return {0x04000500: 'SOUNDCNT', 0x04000504: 'SOUNDBIAS', 0x04000508: 'SNDCAP0CNT', 0x04000509: 'SNDCAP1CNT',
            0x04000510: 'SNDCAP0DAD', 0x04000514: 'SNDCAP0LEN', 0x04000518: 'SNDCAP1DAD', 0x0400051C: 'SNDCAP1LEN'}.get(addr, '%08X' % addr)

def decode_cnt(v):
    return ('vol %d shift %d hold %d pan %d duty %d repeat %d fmt %s start %d' %
            (v & 127, v >> 8 & 3, v >> 15 & 1, v >> 16 & 127, v >> 24 & 7, v >> 27 & 3, FORMATS[v >> 29 & 3], v >> 31 & 1))

def cmd_name(cid):
    return CMD_NAMES[cid] if cid < len(CMD_NAMES) else 'CMD%d' % cid

def decode_cmd(cid, a):
    n = cmd_name(cid)
    if n in ('START_SEQ', 'PREPARE_SEQ'):
        return 'player %d base %08X offset %X bank %08X' % tuple(a)
    if n == 'SETUP_CHANNEL_PCM':
        return ('ch %d timer %04X data %08X vol %d shift %d len %d | loop %d fmt %s pan %d loopstart %d' %
                (a[0] & 0xFFFF, a[0] >> 16, a[1], a[2] >> 24, a[2] >> 22 & 3, a[2] & 0x3FFFFF,
                 a[3] >> 26 & 3, FORMATS[a[3] >> 24 & 3], a[3] >> 16 & 0xFF, a[3] & 0xFFFF))
    if n in ('START_TIMER', 'STOP_TIMER'):
        return 'ch %04X cap %X alarm %X flags %X' % tuple(a)
    if n in ('LOCK_CHANNEL', 'UNLOCK_CHANNEL', 'STOP_UNLOCKED_CHANNEL'):
        return 'ch %08X flags %X' % (a[0], a[1])
    if n in ('MASTER_VOLUME', 'MASTER_PAN'):
        return 'value %d' % a[0]
    return None

def magic_of(data):
    if data[:4] in MAGICS and len(data) >= 12:
        return '%s size %d' % (data[:4].decode(), struct.unpack_from('<I', data, 8)[0])
    return None

def regions(chunks):
    """Merged [start, end) ranges the MEM chunks write."""
    rs = sorted((c['addr'], c['addr'] + len(c['data'])) for c in chunks if c['tag'] == 'MEM' and c['data'])
    out = []
    for s, e in rs:
        if out and s <= out[-1][1]: out[-1][1] = max(out[-1][1], e)
        else: out.append([s, e])
    return out

def dump(acs, out, cmds=False, mem=False, regs=False):
    P = lambda s='': print(s, file=out)
    ch = acs['chunks']
    for c in ch:
        if c['tag'] == 'INFO':
            P('INFO'); [P('  ' + l) for l in c['text'].splitlines()]
    P('%-4s %-5s %16s %7s  %s' % ('#', 'chunk', 'time(cycles)', 'frame', 'summary'))
    counts = {}; wakes = []
    for i, c in enumerate(ch):
        t = c['tag']; counts[t] = counts.get(t, 0) + 1
        if t == 'INFO': P('%-4d INFO  %16s %7s  %d bytes' % (i, '', '', c['size'])); continue
        if t == 'WAKE': wakes.append(c['time'])
        time = c.get('time')
        ts = '%16d %7.2f' % (time, time / FRAME) if time is not None else '%16s %7s' % ('', '')
        if t == 'MEM':
            s = '%08X..%08X (%d bytes)' % (c['addr'], c['addr'] + len(c['data']), len(c['data']))
            m = magic_of(c['data'])
            if m: s += '  ' + m
            P('%-4d MEM   %s  %s' % (i, ts, s))
            if mem and not m: P('       first bytes: ' + c['data'][:16].hex(' '))
        elif t == 'CMDS':
            P('%-4d CMDS  %s  frame %d, %d commands' % (i, ts, c['frame'], len(c['cmds'])))
            if cmds:
                for cid, a in c['cmds']:
                    d = decode_cmd(cid, a)
                    P('       %-22s %s%s' % (cmd_name(cid), ' '.join('%08X' % x for x in a), ('   ; ' + d) if d else ''))
        elif t == 'WAKE':
            P('%-4d WAKE  %s' % (i, ts))
        elif t == 'REGS':
            P('%-4d REGS  %s  %d writes' % (i, ts, len(c['writes'])))
            if regs:
                for a, v, sz in c['writes']:
                    nm = reg_name(a)
                    extra = '   ; ' + decode_cnt(v) if nm.endswith('CNT') and nm.startswith('SOUND') and nm != 'SOUNDCNT' and sz == 4 else ''
                    P('       %-12s %d bytes = %0*X%s' % (nm, sz, sz * 2, v & ((1 << (8 * sz)) - 1 if sz < 4 else 0xFFFFFFFF), extra))
        elif t == 'END':
            P('%-4d END   %s' % (i, ts))
        else:
            P('%-4d %-5s %s  unknown chunk, %d bytes' % (i, t, ts, c['size']))
    P()
    P('chunks: ' + ', '.join('%s %d' % (k.strip(), v) for k, v in sorted(counts.items())))
    P('wakeups: %d%s' % (len(wakes), (' at ' + ' '.join(str(w) for w in wakes[:8]) + (' ...' if len(wakes) > 8 else '')) if wakes else ''))
    rs = regions(ch)
    P('memory regions: ' + ('none' if not rs else ', '.join('%08X-%08X' % (s, e) for s, e in rs)))
    ends = [c['time'] for c in ch if c['tag'] == 'END']
    P('length: %s' % ('%d cycles (%.2f frames, %.3f s)' % (ends[-1], ends[-1] / FRAME, ends[-1] / 33513982.0) if ends else 'no END chunk'))

def to_json(acs):
    def conv(c):
        d = {k: v for k, v in c.items() if k != 'data'}
        if 'data' in c: d['data_len'] = len(c['data']); d['data_head'] = c['data'][:16].hex()
        return d
    return json.dumps(dict(version=acs['version'], chunks=[conv(c) for c in acs['chunks']]), indent=1)

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('file'); ap.add_argument('--cmds', action='store_true'); ap.add_argument('--mem', action='store_true')
    ap.add_argument('--regs', action='store_true'); ap.add_argument('--json', action='store_true')
    args = ap.parse_args(argv)
    try:
        acs = parse_acs(open(args.file, 'rb').read())
    except (AcsError, OSError) as e:
        print('acs_dump: %s: %s' % (args.file, e), file=sys.stderr); return 1
    if args.json: print(to_json(acs))
    else: dump(acs, sys.stdout, args.cmds, args.mem, args.regs)
    return 0

if __name__ == '__main__':
    sys.exit(main())
