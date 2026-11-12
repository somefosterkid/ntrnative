# Piece 8 oracle probe: python3 p08_oracle_probe.py DIR writes pr_a..h.acs (one track, params set in the sequence, READ_DRIVER_INFO at frame 20);
# run aoracle with --mem and read the work with p08_oracle_dump.py (channel user_* fields, the track). Results are in p08_seq_param.c.
import os, sys, struct
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'tools'))
from mkacs import *

def build(name, trk, extra=()):
    swar = build_swar([(1, True, 32000, 0, 32, sine16(64))])
    sbnk = build_sbnk([(1, 0, 0, 60, 255, 0, 127, 127, 64)], [SWAR_ADDR])
    seq = (b'\xE1' + struct.pack('<H', 120) + b'\x81' + varlen(0) + bytes(trk) + bytes((60, 100)) + varlen(480)
           + b'\x80' + varlen(480) + b'\xFF')
    sseq = build_sseq(seq)
    a = Acs().info(source='probe', name=name, mode='driver', frames=40)
    a.mem(0, SSEQ_ADDR, sseq).mem(0, SBNK_ADDR, sbnk).mem(0, SWAR_ADDR, swar)
    a.cmds(0, 0, [('MASTER_VOLUME', 127), ("START_SEQ", 0, SSEQ_ADDR + 0x1C, 0, SBNK_ADDR)])
    a.wake(0)
    ex = [('TRACK_PARAM', 0 | (sz << 24), 1, off, val) for (off, sz, val) in extra]
    if name=="d": ex.append(("PLAYER_PARAM", 0, 6, 0xFFCE, 2))
    if ex:
        a.cmds(10 * FRAME, 10, ex)
        a.wake(10 * FRAME)
    a.cmds(20 * FRAME, 20, [('READ_DRIVER_INFO', 0x0237D000)])
    a.wake(20 * FRAME)
    return a.end(30 * FRAME)

import os
out = sys.argv[1]
os.makedirs(out, exist_ok=True)
P = [
 ('a', [0xC1, 100, 0xD5, 50, 0xC0, 94, 0xC4, 100, 0xC5, 4, 0xC3, 3, 0xC2, 80, 0xCA, 40, 0xCC, 1, 0xE0, 0, 10], []),
 ('b', [0xC1, 0, 0xD5, 127, 0xC0, 0], []),
 ('c', [0xC1, 64, 0xC0, 64, 0xC4, 0x81, 0xC5, 2], [(0x09, 1, 10), (0x0C, 2, 20), (0x0A, 2, 0xFFE2)]),
 ("d", [0xC0, 94, 0xC1, 90], [(0x01, 1, 64), (0x09, 1, 5), (0x0A, 2, 0xFFF6)]),
 ("e", [0xC7, 0], []),
 ("f", [0xC8, 1], []),
 ("g", [0xC3, 5, 0xC9, 60, 0xCF, 9, 0xC6, 33, 0xE3, 0xFE, 0xD0, 1, 0xD1, 2, 0xD2, 3, 0xD3, 4, 0xCD, 3, 0xCB, 7], []),
 ("h", [0xC3, 5, 0xC9, 60, 0xCE, 0], []),
]
for n, t, e in P:
    build(n, t, e).write(os.path.join(out, 'pr_' + n + '.acs'))
