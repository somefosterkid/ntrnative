# piece 11 oracle experiments (driver streams: allocation order, steals, release, lock flags; see p11_alloc.h).
# usage: python3 p11_streams.py DIR (allocation) and p11_streams2.py DIR (locks, release, stop-unlocked); then aoracle --regs.
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import mkacs as m

SW = m.SWAR_ADDR

def seq(ops, tempo=240):
    b = bytearray(b'\xE1' + struct.pack('<H', tempo) + bytes((0xC7, 0)))
    for o in ops:
        k = o[0]
        if k == 'prog': b += b'\x81' + m.varlen(o[1])
        elif k == 'prio': b += bytes((0xC6, o[1]))
        elif k == 'note': b += bytes((o[1], o[2] if len(o) > 3 else 100)) + m.varlen(o[-1])
        elif k == 'rest': b += b'\x80' + m.varlen(o[1])
        elif k == 'raw': b += bytes(o[1])
        elif k == 'rel': b += bytes((0xD3, o[1]))
    b += b'\xFF'
    return bytes(b)

def make(name, ops, frames=300, tempo=240, swars=None):
    swar = m.build_swar([(1, True, 32000, 0, 32, m.sine16(64))])
    insts = [(1, 0, 0, 60, 255, 0, 127, 127, 64),
             (2, 4, 0, 60, 255, 0, 127, 127, 64),
             (3, 0, 0, 60, 255, 0, 127, 127, 64),
             (1, 0, 0, 60, 255, 0, 127, 20, 64),
             (1, 0, 0, 60, 255, 60, 40, 127, 64)]
    sbnk = m.build_sbnk(insts, [SW])
    sseq = m.build_sseq(seq(ops, tempo))
    a = m.Acs().info(source='exp', name=name, mode='driver', frames=frames)
    a.mem(0, m.SSEQ_ADDR, sseq).mem(0, m.SBNK_ADDR, sbnk).mem(0, SW, swar)
    a.cmds(0, 0, [('MASTER_VOLUME', 127), ('START_SEQ', 0, m.SSEQ_ADDR, 0x1C, m.SBNK_ADDR)])
    a.wake(0)
    a.end(frames * m.FRAME)
    return a

L = 4000
def notes(n, prog=0, prio=None, rest=12, key=60):
    o = [('prog', prog)]
    for i in range(n):
        if prio is not None: o.append(('prio', prio(i)))
        o += [('note', key, L), ('rest', rest)]
    return o

if __name__ == '__main__':
    out = sys.argv[1]
    os.makedirs(out, exist_ok=True)
    E = {}
    E['x_pcm17'] = notes(17)
    E['x_psg8'] = notes(8, 1)
    E['x_noise3'] = notes(3, 2)
    E['x_prio_dec'] = notes(17, prio=lambda i: 100 - i * 2) + [('prio', 90), ('note', 60, L)]
    E['x_prio_inc'] = notes(17, prio=lambda i: 40 + i * 2)
    E['x_prio_low'] = notes(16, prio=lambda i: 80) + [('prio', 10), ('note', 60, L)]
    E['x_t1'] = [('note', 60, 100), ('rest', 12), ('note', 60, 100)]
    E['x_t2'] = [('note', 60, 1000), ('rest', 12), ('note', 60, 1000)]
    E['x_k72'] = [('note', 72, 100), ('rest', 12)]
    E['x_none'] = []
    for k, v in E.items():
        make(k, v).write(os.path.join(out, k + '.acs'))
