import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import mkacs as m
from p11_streams import make, L, notes

F = m.FRAME
out = sys.argv[1]
os.makedirs(out, exist_ok=True)
BASE = 0x02200000
# lock/unlock flags and the driver info
a = m.Acs().info(source='exp', name='x_lockinfo', mode='driver', frames=40)
steps = [
    [('LOCK_CHANNEL', 0x30, 0)],
    [('LOCK_CHANNEL', 0x300, 1)],
    [('UNLOCK_CHANNEL', 0x10, 0)],
    [('UNLOCK_CHANNEL', 0x100, 0)],
    [('UNLOCK_CHANNEL', 0x300, 1)],
    [('LOCK_CHANNEL', 0xF000, 2)],
    [('UNLOCK_CHANNEL', 0x1000, 0)],
    [('LOCK_CHANNEL', 0x1000, 0)],
    [('UNLOCK_CHANNEL', 0x1000, 3)],
]
for k, st in enumerate(steps):
    t = (2 + k * 2) * F
    a.cmds(t, 2 + k * 2, st + [('READ_DRIVER_INFO', BASE + k * 0x2000)])
    a.wake(t)
a.end(40 * F)
a.write(os.path.join(out, 'x_lockinfo.acs'))

# stop unlocked
s = make('x_stopun', [('note', 60, L), ('rest', 100)], frames=120)
s.chunks.insert(len(s.chunks) - 1, (b'CMDS', struct.pack('<QII', 20 * F, 20, 1) + struct.pack('<5I', 28, 0x10, 0, 0, 0)))
s.chunks.insert(len(s.chunks) - 1, (b'WAKE', struct.pack('<Q', 20 * F)))
s.chunks.insert(len(s.chunks) - 1, (b'CMDS', struct.pack('<QII', 30 * F, 30, 1) + struct.pack('<5I', 28, 0x20, 0, 0, 0)))
s.chunks.insert(len(s.chunks) - 1, (b'WAKE', struct.pack('<Q', 30 * F)))
s.write(os.path.join(out, 'x_stopun.acs'))

s = make('x_lockact', [('note', 60, L), ('rest', 100)], frames=120)
for fr, cid, mk in ((20, 26, 0x10), (30, 27, 0x10), (40, 28, 0x10)):
    s.chunks.insert(len(s.chunks) - 1, (b'CMDS', struct.pack('<QII', fr * F, fr, 1) + struct.pack('<5I', cid, mk, 0, 0, 0)))
    s.chunks.insert(len(s.chunks) - 1, (b'WAKE', struct.pack('<Q', fr * F)))
s.write(os.path.join(out, 'x_lockact.acs'))

# steal by envelope level / release state
def prog(p, n=1):
    return [('prog', p)]
L2 = 4000
ops = [('prog', 4), ('note', 60, L2), ('rest', 12), ('prog', 0)]
for i in range(15): ops += [('note', 60, L2), ('rest', 12)]
ops += [('note', 60, L2), ('rest', 100)]
make('x_lvl', ops, frames=200).write(os.path.join(out, 'x_lvl.acs'))
# a short released note first (slow release), then 15 long, then a steal
ops = [('prog', 3), ('note', 60, 30), ('rest', 12), ('prog', 0)]
for i in range(15): ops += [('note', 60, L2), ('rest', 12)]
ops += [('note', 60, L2), ('rest', 100)]
make('x_rel1', ops, frames=200).write(os.path.join(out, 'x_rel1.acs'))
# the released note has a higher track prio than the rest
ops = [('prog', 0)]
for i in range(4): ops += [('note', 60, L2), ('rest', 12)]
ops += [('prio', 100), ('prog', 3), ('note', 60, 30), ('rest', 12), ('prio', 64), ('prog', 0)]
for i in range(11): ops += [("note", 60, L2), ("rest", 12)]
ops += [('note', 60, L2), ('rest', 100)]
make('x_rel2', ops, frames=200).write(os.path.join(out, 'x_rel2.acs'))

for p in (0, 1, 2, 3):
    ops = [('prog', 3), ('note', 60, 30), ('rest', 12), ('prog', 0)]
    for i in range(15): ops += [('note', 60, L2), ('rest', 12)]
    ops += [('prio', p), ('note', 60, L2), ('rest', 100)]
    make('x_relp%d' % p, ops, frames=200).write(os.path.join(out, 'x_relp%d.acs' % p))

# allocation vs lock flags: lock channel 4 (and 5) with flags f, then 4 notes
for f in (0, 1, 2, 3):
    s = make('x_lockf%d' % f, notes(4), frames=120)
    s.chunks.insert(len(s.chunks) - 2, (b'CMDS', struct.pack('<QII', 0, 0, 1) + struct.pack('<5I', 26, 0x30, f, 0, 0)))
    s.write(os.path.join(out, 'x_lockf%d.acs' % f))
