#!/usr/bin/env python3
"""tools/diagrec.py FILE.bwdiag [--sweep EXE] [--keep DIR] [--strides] [--events]: reads a Record diagnostics file (port/bridge/diag_rec.h:
iOS Settings > Developer > Record diagnostics, macOS Debug > Record diagnostics, the cfgscript's diagrec=S) and prints one line an item:
the manifest; the renderer calls (IpGather's outcomes, the game's 3D swaps the pairing saw or missed); the keyframes and their
intervals; the presents (sent, kept, dropped, not presented; late against their slots; repeated pictures, 3D reused); the pairing
distance of every in-between shown (keyframes and game frames between the two it blends); in-betweens per keyframe; then the
recorded keyframes rebuilt as .vcs files and scored by vcore_ipsweep --oracle in the order the app paired them (the oracle's
V_ classes and the per-palette P_ census on the app's own pairs; --strides also every 2nd / 4th of them). --keep DIR leaves the
.vcs files there (else a temporary folder, deleted). --events prints the scorer's EV lines for the V_ and P_ classes."""
import os, re, shutil, struct, subprocess, sys, tempfile
from collections import Counter

a = sys.argv[1:]
def opt(name, default=None):
    if name in a:
        i = a.index(name); v = a[i + 1]; del a[i:i + 2]; return v
    return default
def flag(name):
    if name in a: a.remove(name); return True
    return False
here = os.path.dirname(os.path.abspath(__file__))
sweep = opt('--sweep', os.path.join(here, '..', 'build', 'vcore', 'vcore_ipsweep'))
keep = opt('--keep')
strides, events = flag('--strides'), flag('--events')
if len(a) != 1:
    print(__doc__); sys.exit(2)
path = a[0]
data = open(path, 'rb').read()
if data[:4] != b'BWDG' or struct.unpack_from('<I', data, 4)[0] != 1:
    print(f'{path}: not a bwdiag v1 file'); sys.exit(1)
at, mani, log, keys = 8, {}, '', []
while at + 8 <= len(data):
    tag, n = data[at:at + 4], struct.unpack_from('<I', data, at + 4)[0]
    body = data[at + 8:at + 8 + n]
    at += 8 + ((n + 3) & ~3)
    if tag == b'MANI':
        for l in body.decode('utf-8', 'replace').splitlines():
            if l.startswith('env '): mani.setdefault('env', []).append(l[4:])
            elif l.startswith('ini '): mani.setdefault('ini', []).append(l[4:])
            elif l.startswith('# '): mani.setdefault('settings', []).append(l[2:])
            elif '=' in l: k, v = l.split('=', 1); mani[k] = v
    elif tag == b'LOG ': log = body.decode('utf-8', 'replace')
    elif tag == b'KEY ': keys.append(body)
def mi(k, d=0):
    try: return int(mani.get(k, d))
    except ValueError: return d

def pct(v, p):
    if not v: return 0
    v = sorted(v); return v[min(len(v) - 1, int(p / 100.0 * (len(v) - 1) + 0.5))]
def hist(c, n=8):
    ks = sorted(c, key=lambda k: (isinstance(k, str), k))
    return ' '.join(f'{k}:{c[k]}' for k in ks[:n]) + (' ...' if len(ks) > n else '') if c else '-'

# ---- manifest
st = ' | '.join(s for s in mani.get('settings', []) if s.startswith('Renderer'))
print(f"manifest: build {mani.get('build') or '?'}, {mani.get('device', '?')}, {mani.get('os', '?')}, panel {mani.get('panel_hz', '?')} Hz, "
      f"thermal {mani.get('thermal', '?')}->{mani.get('end_thermal', '?')}, low power {mani.get('lowpower', '?')}->{mani.get('end_lowpower', '?')}, "
      f"{mani.get('seconds', '?')} s from {mani.get('start', '?')}, file {len(data) / 1048576:.1f} MB")
print(f'settings: {st or "?"}; game speed {mani.get("game_speed", "?")}x, walk {mani.get("walk_speed", "?")}x, run {mani.get("run_speed", "?")}x, '
      f'surf {mani.get("surf_speed", "?")}x')
ini = mani.get('ini', [])
if ini: print('ini: ' + ' '.join(ini))
envs = mani.get('env', [])
if envs: print('env: ' + ' '.join(e if len(e) < 48 else e[:45] + '...' for e in envs))
print(f"recorder: {mi('ticks')} calls, {mi('presents')} presents, {mi('keyframes')} keyframes kept ({mi('keyframes_lost')} not kept), "
      f"lost T {mi('ticks_lost')} P {mi('presents_lost')}, keyframe data {mi('key_bytes') / 1048576:.1f} MB, its time {mani.get('rec_ms', '?')} ms "
      f"(longest {mani.get('rec_max_us', '?')} us)")

# ---- the log
T, P = {}, []
for l in log.splitlines():
    f = l.split()
    if not f or f[0] == '#': continue
    if f[0] == 'T':
        v = [int(x) for x in f[1:17]]
        t = dict(zip('id t nominal serial frame swaps swapframe ipN why valid key old nl nv gap work'.split(), v))
        t['rec'], t['bytes'], t['blend'] = int(f[17]), int(f[18]), int(f[19], 16)
        t['f'] = [None] + [float(x) for x in f[20:]]
        T[t['id']] = t
    elif f[0] == 'P':
        v = [int(x) for x in f[1:]]
        P.append(dict(zip('kind t tick r mid why slot call game due early pid pidx pser pr3 pid3d'.split(), v)))
calls = [T[k] for k in sorted(T)]
if not calls:
    print('no renderer calls recorded (the GPU renderer did not run?)'); sys.exit(0)
why = Counter(t['why'] for t in calls)
wn = {-1: 'no slots', 0: 'repeat', 1: 'pair', 2: 'cut', 3: 'no pair'}
print('calls: ' + ', '.join(f'{wn.get(k, k)} {why[k]}' for k in sorted(why)) +
      f"; slots a call {hist(Counter(t['ipN'] for t in calls))}; worker ms median {pct([t['work'] for t in calls], 50) / 1000:.2f} "
      f"p95 {pct([t['work'] for t in calls], 95) / 1000:.2f} max {max(t['work'] for t in calls) / 1000:.2f}")
# the game's 3D swaps against the calls: a swap between two gathers that no gather showed was never seen
sw = [t['swaps'] for t in calls]
miss = sum(max(0, b - a - 1) for a, b in zip(sw, sw[1:]))
newswap = sum(1 for a, b in zip(calls, calls[1:]) if b['swaps'] != a['swaps'])
gnew = [b for a, b in zip(calls, calls[1:]) if b['swaps'] != a['swaps']]
seenrep = sum(1 for t in gnew if t['why'] == 0)
seennone = sum(1 for t in gnew if t['why'] == -1)
at_swap = [i for i, t in enumerate(calls) if i > 0 and t['swaps'] != calls[i - 1]['swaps']]
print(f"3D swaps: {sw[-1] - sw[0]} in the window; gathered {newswap} (as a new keyframe {newswap - seenrep - seennone}, as a repeat "
      f"{seenrep}, in a call with no slots {seennone}); never gathered (two swaps between calls) {miss}; calls between swaps "
      f"{hist(Counter(b - a for a, b in zip(at_swap, at_swap[1:])))}")
kf = {}
for t in calls:
    if t['why'] > 0 and t['key'] >= 0: kf[t['key']] = t
# per keyframe: the 3D swaps between it and the one it pairs with that were gathered as repeats (the same lists: a still scene)
still, pos = {}, {t['id']: i for i, t in enumerate(calls)}
for s, t in kf.items():
    o = kf.get(t['old'])
    if o is None: continue
    i0, i1 = pos[o['id']], pos[t['id']]
    still[s] = sum(1 for i in range(i0 + 1, i1) if calls[i]['swaps'] != calls[i - 1]['swaps'] and calls[i]['why'] == 0)
seqs = sorted(kf)
iv = Counter(kf[b]['swapframe'] - kf[a]['swapframe'] for a, b in zip(seqs, seqs[1:]) if kf[b]['old'] == a)
ivs = [kf[b]['swaps'] - kf[a]['swaps'] for a, b in zip(seqs, seqs[1:]) if kf[b]['old'] == a]
ms = [(kf[b]['t'] - kf[a]['t']) / 1000 for a, b in zip(seqs, seqs[1:]) if kf[b]['old'] == a]
resets = sum(1 for s in seqs if kf[s]['old'] < 0)
print(f"keyframes: {len(seqs)} ({resets} start a new run: no older one); interval in game frames {hist(iv)}; in 3D swaps "
      f"{hist(Counter(ivs))}; ms median {pct(ms, 50):.1f} p95 {pct(ms, 95):.1f} max {max(ms) if ms else 0:.1f}")

# ---- presents
kind = Counter(p['kind'] for p in P)
kn = {0: 'sent', 1: 'kept (skip)', 2: 'dropped', 3: 'not presented (behind)'}
late = [(p['t'] - p['slot']) / 1000 for p in P if p['kind'] in (0, 1) and p['slot']]
pres = [p for p in P if p['kind'] in (0, 1)]
gaps = [(b['t'] - a['t']) / 1000 for a, b in zip(pres, pres[1:])]
print('presents: ' + ', '.join(f'{kn.get(k, k)} {kind[k]}' for k in sorted(kind)) +
      f"; past the slot ms median {pct(late, 50):.2f} p95 {pct(late, 95):.2f} max {max(late) if late else 0:.2f}; "
      f"between presents ms median {pct(gaps, 50):.2f} p95 {pct(gaps, 95):.2f} max {max(gaps) if gaps else 0:.2f}")
wr = Counter(p['why'] for p in P if p['r'] > 0)
wrn = {1: 'behind', 2: 'gpu', 3: 'drawn/none', 4: 'late', 5: 'held'}
print('slots after the first, the in-between rule: ' + ', '.join(f'{wrn.get(k, k)} {wr[k]}' for k in sorted(wr)))
rep = r3 = blend = unbl = own = unknown = skipped = acrossStill = 0
dist_k, dist_f, fbin = Counter(), Counter(), Counter()
mids = Counter()
prev = None
for p in pres:
    if prev is not None and p['pser'] == prev: rep += 1
    prev = p['pser']
    r3 += p['pr3']
    t = T.get(p['pid'])
    if t is None: unknown += 1; continue
    if t['ipN'] == 0: own += 1; continue
    idx = p['pidx']
    if idx < 1 or idx >= len(t['f']) or not (t['blend'] >> idx) & 1: unbl += 1; continue
    blend += 1
    fbin[round(t['f'][idx] * 8) / 8] += 1
    mids[t['key']] += 1
    k, o = kf.get(t['key']), kf.get(t['old'])
    if k is None or o is None: dist_k['?'] += 1; continue
    dist_k[k['swaps'] - o['swaps']] += 1
    dist_f[k['swapframe'] - o['swapframe']] += 1
    lost = k['swaps'] - o['swaps'] - 1 - still.get(t['key'], 0)
    if lost > 0: skipped += 1
    elif k['swaps'] - o['swaps'] > 1: acrossStill += 1
print(f"pictures presented: blended {blend}, a keyframe unblended {unbl}, a call's own (no slots) {own}, unknown {unknown}; the same "
      f"picture as the present before {rep}; 3D layer reused {r3}")
print(f"pairing distance per in-between: 3D swaps apart {hist(dist_k)}; game frames apart {hist(dist_f)}")
print(f"verdict: {skipped} of {blend} in-betweens ({100.0 * skipped / max(1, blend):.1f}%) blend two keyframes with a changed 3D frame "
      f"between them that no pairing saw; {acrossStill} more pair across still frames only (repeats: the same lists)")
print(f"in-between f (eighths): {hist(fbin, 9)}")
mk = Counter(mids.get(s, 0) for s in seqs[1:-1])
print(f"in-betweens shown per keyframe: {hist(mk, 9)}")

# ---- the oracle on the recorded pairs
if not keys:
    print('oracle: no keyframes kept'); sys.exit(0)
if not os.path.exists(sweep):
    print(f'oracle: no scorer at {sweep} (--sweep EXE)'); sys.exit(0)
sz = {k: mi(k) for k in ('io9', 'io9b', 'pltt', 'oam', 'vram', 'page')}
out = keep or tempfile.mkdtemp(prefix='diagrec-')
os.makedirs(out, exist_ok=True)
vram = bytearray(sz['vram'])
segs, cur = [], []
def chunk(tag, b):
    return tag + struct.pack('<I', len(b)) + b + b'\0' * ((4 - len(b) % 4) % 4)
for body in keys:
    seq, cid, frame, swf, swaps, gxw, npg, old = struct.unpack_from('<7Ii', body, 0)
    o = 32
    pages = struct.unpack_from(f'<{npg}H', body, o); o += (npg * 2 + 3) & ~3
    io9 = body[o:o + sz['io9']]; o += sz['io9']
    io9b = body[o:o + sz['io9b']]; o += sz['io9b']
    pltt = body[o:o + sz['pltt']]; o += sz['pltt']
    oam = body[o:o + sz['oam']]; o += sz['oam']
    for pg in pages:
        vram[pg * sz['page']:(pg + 1) * sz['page']] = body[o:o + sz['page']]; o += sz['page']
    gx = body[o:o + gxw * 4]
    if old < 0 or (cur and cur[-1][0] != old):
        if cur: segs.append(cur)
        cur = []
    fr = swf or frame
    d = os.path.join(out, f'seg{len(segs):02d}')
    os.makedirs(d, exist_ok=True)
    fn = os.path.join(d, f'f{fr:06d}.vcs')
    info = f'source=bwport diagrec\nformat=VCS_FORMAT.md v1\nframe={fr}\ndiag_seq={seq}\ndiag_call={cid}\ngather_frame={frame}\n'.encode()
    with open(fn, 'wb') as f:
        f.write(b'VCST' + struct.pack('<I', 1) + chunk(b'INFO', info) + chunk(b'IO9 ', io9) + chunk(b'IO9B', io9b) +
                chunk(b'VRAM', bytes(vram)) + chunk(b'PLTT', pltt) + chunk(b'OAM ', oam) + chunk(b'GX3D', gx))
    cur.append((seq, fn))
if cur: segs.append(cur)
CL = ('V_NONE', 'V_TWICE', 'P_NONE', 'P_TWICE', 'P_CROSS', 'V_STALE')
def score(files):
    p = subprocess.run([sweep, '--events', '--oracle'] + files, capture_output=True, text=True)
    if "--oracle: can't open" in p.stderr:
        print(f'oracle: {sweep} has no --oracle (an older build: ninja -C build/vcore)'); sys.exit(1)
    r = p.stdout
    s = Counter()
    for l in r.splitlines():
        f = l.split()
        if l.startswith('SUM ') and len(f) >= 3: s[f[1]] += int(f[2])
        if l.startswith('ORACLE'):
            for k, v in zip(f[1::2], f[2::2]):
                try: s['ORC_' + k] += int(v)
                except ValueError: pass
    return s, r
for k in (1, 2, 4) if strides else (1,):
    tot, ev, npair = Counter(), [], 0
    for sg in segs:
        for ph in range(k):  # (every phase: each pair k keyframes apart once)
            files = [fn for i, (_, fn) in enumerate(sg) if i % k == ph]
            if len(files) < 2: continue
            npair += len(files) - 1
            s, r = score(files)
            tot += s
            ev += [l for l in r.splitlines() if l.startswith(('EV V_', 'EV P_'))]
    print(f"oracle on the app's pairs{'' if k == 1 else f', keyframes {k} apart (all phases)'}: {npair} pairs in {len(segs)} runs; " +
          ' '.join(f'{c} {tot.get(c, 0)}' for c in CL) +
          f"; oracle disagree {tot.get('ORC_disagree', 0)} missed {tot.get('ORC_missed', 0)}; live A_OFFMOTION {tot.get('A_OFFMOTION', 0)} "
          f"A_MISPAIR {tot.get('A_MISPAIR', 0)} C_VANISH {tot.get('C_VANISH', 0)} C_APPEAR {tot.get('C_APPEAR', 0)}")
    if events and k == 1:
        for l in ev: print('  ' + l)
if not keep: shutil.rmtree(out, ignore_errors=True)
