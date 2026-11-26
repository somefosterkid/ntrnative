#!/usr/bin/env python3
"""Where the GPU and CPU time goes, per pass and per thread (docs/perf.md "Where the cost goes (2026-11-25)").
  tools/mac_gpu_profile.py [--rows ROWS] [--start F] [--walk 'right left'] [--hold 180] -- MAC_RUN_ARGS...
  tools/mac_gpu_profile.py --parse LOG...      (the table of any log with bench rows: the phone's too, Share latest log)
Run mode: ONE tools/mac_run.sh run (MAC_RUN_ARGS as it takes them: --root --save NAME RUNDIR SECONDS; env passes through) whose
cfgscript (BWPORT_CFGSCRIPT) fast-forwards to frame --start, turns the GPU passes' times on (gpupasses=1), then per row sets the
row's settings live, settles 1 s and records a 4 s bench row; the keys (BWPORT_KEYS, frames from boot) walk --walk's directions
in turn, --hold frames each, from --start to the end (a back-and-forth walk keeps the player where the save put him). Defaults
for a timing run: BWPORT_FFMAX=0 (real time after the fast forward), BWPORT_PANELHZ=120 (the iPhone's 120 Hz link, pretended),
--noskip. The settings not in a row (Resolution, Present=metal, Frame rate) are the run folder's sim_config.ini's.
ROWS: comma-separated names, each 3d-aa-smooth[-unl][-noside][-xN]: exact|fast, aa1 (off) | aa4 (4 samples) | lite (4 samples
(lite)) | edges (Smooth edges) | smaa, s1 | s0 (Smooth motion on/off); xN: PerfExperiments=N (Exact's pass R cuts' switches, port/gpu
GPU_EXP_*; 0 when not given); unl: Frame rate Unlimited (the GPU flat out, so at its top clock: its ms are the passes' cost, where a paced row's are
at whatever clock the GPU's governor picked for that load); noside: the Mac's side panel not drawn (the present without the UI);
default exact-aa4-s1,exact-aa1-s1,fast-aa4-s1,fast-aa1-s1,exact-aa4-s0,exact-aa1-s0,fast-aa4-s0,fast-aa1-s0.
The table: presents, keyframe and in-between renders a second; the GPU busy share and ms a present (the union of the passes'
timed stages: gpu_mtl.m's timestamps, not the command buffers' spans); ms a render (a keyframe or an in-between) by pass (S span setup, R 3D raster, E edge
map, P post + resolve, AA = SMAA, C compose of both screens, cap = display capture), and pres, ms a present (the final present: the
picture's quad to the drawable plus the UI); the CPU's cores, ms a tick, and the worker's and game thread's ms a drawn frame (threads: line)."""
import os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GROUPS = [('S', ('g02_setup',)), ('R', ('g00_raster', 'g10_fast')), ('E', ('g06_edgemap',)), ('P', ('g06_post',)),
          ('AA', ('g08_smaa',)), ('C', ('g07_compose',)), ('cap', ('g07_capture',)), ('pres', ('present',))]
DEFAULT_ROWS = 'exact-aa4-s1,exact-aa1-s1,fast-aa4-s1,fast-aa1-s1,exact-aa4-s0,exact-aa1-s0,fast-aa4-s0,fast-aa1-s0'


def row_settings(name):
    d3, aa, sm, *more = name.split('-')
    exp = [int(m[1:]) for m in more if m[:1] == 'x' and m[1:].isdigit()]
    return 'renderer3d=%s,antialias=%s,smooth=%s,framerate=%s,side=%d,perfexp=%d' % (
        d3, {'aa1': '1', 'aa4': '2', 'smaa': '3', 'lite': 'lite', 'edges': 'edges'}[aa], sm[1:],
        'unlimited' if 'unl' in more else 'monitor', 'noside' not in more, exp[0] if exp else 0)


def parse(text):
    rows, order = {}, []
    for line in text.splitlines():
        m = re.match(r'bench: (\S+)\s+(.+?)\s+(-?[\d.]+)\s+([\d.]+)\s+([\d.]+)\s+([\d.]+)\s+[\d.]+\s+[\d.]+\s+[\d.]+\s+([\d.]+)\s+([\d.]+)', line)
        if m and m.group(1) not in ('table', 'step'):
            r = rows.setdefault(m.group(1), {})
            if m.group(1) not in order:
                order.append(m.group(1))
            r.update(renderer=m.group(2), pps=float(m.group(4)), cores=float(m.group(7)), tick=float(m.group(8)))
        m = re.match(r'gpu passes: (\S+)\s+([\d.]+) s, ([\d.]+) presents/s, ([\d.]+) keyframe and ([\d.]+) in-between.*?GPU busy ([\d.]+)% = ([\d.]+) ms a present.*?by pass, ms a present \(each, n/s\): (.*); untimed', line)
        if m and m.group(1) != '(10':
            r = rows.setdefault(m.group(1), {})
            pps, key, mid = float(m.group(3)), float(m.group(4)), float(m.group(5))
            r.update(key=key, mid=mid, busy=float(m.group(6)), ms=float(m.group(7)))
            for name, val in re.findall(r'(\S+) ([\d.]+) \([\d.]+ each', m.group(8)):
                for g, pre in GROUPS:
                    if name.startswith(pre):  # a pass's ms a render (keyframe or in-between), the present's a present
                        per = 1.0 if g == 'pres' or key + mid <= 0 else pps / (key + mid)
                        r[g] = r.get(g, 0.0) + float(val) * per
        m = re.match(r'threads: (\S+)\s+CPU ms a drawn frame .*?all ([\d.]+) = (.*?); process', line)
        if m:
            r = rows.setdefault(m.group(1), {})
            th = dict((k.strip(), float(v)) for k, v in re.findall(r'([^,=]+?) ([\d.]+)(?:,|$)', m.group(3)))
            r.update(worker=th.get('bwport-worker', 0.0), game=th.get('game', 0.0))
    return [(n, rows[n]) for n in order]


def table(rows):
    head = ['row', 'renderer', 'pres/s', 'key/s', 'mid/s', 'GPU busy', 'ms/pres'] + [g if g == 'pres' else g + ' ms/render' if g == 'S' else g for g, _ in GROUPS] + ['cores', 'CPU ms/tick', 'worker', 'game']
    out = ['| ' + ' | '.join(head) + ' |', '|' + '---|' * len(head)]
    f = lambda r, k, fmt='%.2f': fmt % r[k] if k in r else '-'
    for n, r in rows:
        out.append('| ' + ' | '.join([n, r.get('renderer', '-'), f(r, 'pps', '%.1f'), f(r, 'key', '%.1f'), f(r, 'mid', '%.1f'),
                                      f(r, 'busy', '%.0f%%'), f(r, 'ms')] + [f(r, g) for g, _ in GROUPS] +
                                     [f(r, 'cores'), f(r, 'tick'), f(r, 'worker'), f(r, 'game')]) + ' |')
    return '\n'.join(out)


def main(argv):
    if argv[:1] == ['--parse']:
        for p in argv[1:]:
            print(table(parse(open(p, errors='replace').read())))
        return 0
    opts, rest = argv, []
    if '--' in argv:
        i = argv.index('--')
        opts, rest = argv[:i], argv[i + 1:]
    get = lambda k, d: opts[opts.index(k) + 1] if k in opts else d
    rows = get('--rows', DEFAULT_ROWS).split(',')
    start, hold, walk = int(get('--start', '10950')), int(get('--hold', '180')), get('--walk', 'right left').split()
    cfg = ['f0:ff=1', 'f%d:ff=0,framerate=monitor,gpupasses=1' % start]
    for n in rows:
        cfg += ['p30:' + row_settings(n), 'p60:bench=' + n]
    cfg.append('p5:quit=1')
    env = dict(os.environ)
    env['BWPORT_CFGSCRIPT'] = ';'.join(cfg)
    env.setdefault('BWPORT_FFMAX', '0')
    env.setdefault('BWPORT_PANELHZ', '120')
    end = start + 60 + len(rows) * 330
    keys = ['%s*f%d@f%d' % (walk[i % len(walk)], hold, f) for i, f in enumerate(range(start + 50, end, hold))]
    env['BWPORT_KEYS'] = (env.get('BWPORT_KEYS', '') + ' ' + ' '.join(keys)).strip()
    cmd = [os.path.join(ROOT, 'tools', 'mac_run.sh'), '--noskip'] + rest
    subprocess.run(cmd, env=env, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    rundir = [a for a in rest if not a.startswith('-') and os.path.isdir(a)]
    log = os.path.join(rundir[0], 'log.txt') if rundir else None
    if log is None or not os.path.exists(log):
        print('no log.txt in the run folder', file=sys.stderr)
        return 1
    print(table(parse(open(log, errors='replace').read())))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
