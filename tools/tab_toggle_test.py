#!/usr/bin/env python3
"""tab_toggle_test.py (rn80): the Tab key (Frame rate Unlimited and back). For each Frame rate in 60, 120, monitor the exe
starts at that Frame rate (BWPORT_FRAMERATE), a key script presses Tab (BWPORT_KEYS 'tab@S', port/src/keys_script.c posts
the SDL key event) at 9 s and again at 18 s, and BWPORT_PRESENTLOG says what the window did: 'P' rows are paced presents
(Frame rate 60/120/monitor), 'U' rows uncapped frames (Unlimited). Checks, per rate R: before the first Tab P rows at R
(60: 55..65 a second, 120: 100..130, monitor: any, the same before and after); between the Tabs U rows and no P rows; after
the second Tab P rows again at the rate of before; the saved sim_config.ini ends FrameRate=R FrameRateRestore=R. A second
run per rate presses Tab once: the ini ends FrameRate=unlimited FrameRateRestore=R (what a restart would load).
  python3 tools/tab_toggle_test.py [60 120 monitor]      exit 0 = pass
Run it alone: it uses build/port/run (port_run.py rebuilds the binary)."""
import os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RUN = os.path.join(ROOT, 'build/port/run')
PL = '/root/mt/rn80/tab.pl'


def run(rate, keys, secs):
    env = dict(os.environ, BWPORT_FRAMERATE=rate, BWPORT_KEYS=keys, BWPORT_PRESENTLOG=PL)
    for k in ('BWPORT_GAMESPEED', 'BWPORT_SPEED', 'BWPORT_DISPLAYCAP', 'BWPORT_SMOOTH', 'BWPORT_INTERP'):
        env.pop(k, None)
    os.makedirs(os.path.dirname(PL), exist_ok=True)
    if os.path.exists(PL):
        os.remove(PL)
    ini = os.path.join(RUN, 'sim_config.ini')
    if os.path.exists(ini):
        os.remove(ini)           # a fresh run dir ini: main_start.c writes FrameRate=60
    subprocess.run([sys.executable, os.path.join(ROOT, 'tools/port_run.py'), '-t', str(secs)], cwd=ROOT, env=env,
                   stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    rows = []
    for l in open(PL):
        a = l.split()
        if len(a) >= 2 and a[0] in ('P', 'U') and (a[0] == 'U' or len(a) >= 6):   # (the last line of a killed run can be cut)
            rows.append((a[0], int(a[1])))
    # an uncapped frame logs a 'U' row and also a 'P' row: drop the P rows within 20 ms of a U row
    import bisect
    ut = sorted(t for k, t in rows if k == 'U')
    def near(t):
        i = bisect.bisect_left(ut, t)
        return any(0 <= j < len(ut) and abs(ut[j] - t) < 20000 for j in (i - 1, i))
    rows = [(k, t) for k, t in rows if k == 'U' or not near(t)]
    cfg = {}
    for l in open(ini):
        if '=' in l:
            k, v = l.strip().split('=', 1)
            cfg[k] = v
    return rows, cfg


def phases(rows):
    """[(kind, t_first_us, t_last_us, n)] of the runs of equal row kinds"""
    out = []
    for k, t in rows:
        if out and out[-1][0] == k:
            out[-1][2] = t
            out[-1][3] += 1
        else:
            out.append([k, t, t, 1])
    return out


def rate(p):
    return p[3] / max(1e-6, (p[2] - p[1]) / 1e6)


def main():
    rates = sys.argv[1:] or ['60', '120', 'monitor']
    bad = 0
    for R in rates:
        rows, cfg = run(R, 'tab@9 tab@18', 28)
        ph = [p for p in phases(rows) if p[3] >= 20]    # (a few stray rows at a switch are not a phase)
        errs = []
        kinds = ''.join(p[0] for p in ph)
        if kinds != 'PUP':
            errs.append('phases %s (want PUP): %s' % (kinds, [(p[0], p[3], round(rate(p))) for p in ph]))
        else:
            r1, r3 = rate(ph[0]), rate(ph[2])
            lo, hi = {'60': (55, 65), '120': (100, 130)}.get(R, (20, 1000))
            if not (lo <= r1 <= hi):
                errs.append('rate before %.0f not in %d..%d' % (r1, lo, hi))
            if abs(r3 - r1) > 0.1 * r1:
                errs.append('rate after %.0f differs from before %.0f' % (r3, r1))
            if rate(ph[1]) < 1.5 * r1 and R != 'monitor':
                errs.append('Unlimited only %.0f a second (before %.0f)' % (rate(ph[1]), r1))
        if cfg.get('FrameRate') != R or cfg.get('FrameRateRestore') != R:
            errs.append('ini FrameRate=%s FrameRateRestore=%s want %s' % (cfg.get('FrameRate'), cfg.get('FrameRateRestore'), R))
        print('%-4s %s -> Tab -> Unlimited -> Tab -> %s   %s' % ('FAIL' if errs else 'ok', R, R, '; '.join(errs) or
              ' '.join('%s%d@%.0f/s' % (p[0], p[3], rate(p)) for p in ph)))
        bad += bool(errs)
        rows, cfg = run(R, 'tab@9', 18)
        ph = [p for p in phases(rows) if p[3] >= 20]
        errs = []
        if ''.join(p[0] for p in ph) != 'PU':
            errs.append('phases %s (want PU)' % ''.join(p[0] for p in ph))
        if cfg.get('FrameRate') != 'unlimited' or cfg.get('FrameRateRestore') != R:
            errs.append('ini FrameRate=%s FrameRateRestore=%s want unlimited / %s' % (cfg.get('FrameRate'), cfg.get('FrameRateRestore'), R))
        print('%-4s %s -> Tab (ends Unlimited): ini FrameRate=%s FrameRateRestore=%s %s' % ('FAIL' if errs else 'ok', R, cfg.get('FrameRate'),
              cfg.get('FrameRateRestore'), '; '.join(errs)))
        bad += bool(errs)
    print('tab-toggle: %s' % ('FAIL' if bad else 'PASS'))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
