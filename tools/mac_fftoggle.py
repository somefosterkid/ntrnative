#!/usr/bin/env python3
"""Fast-forward toggle check: pacing after N fast-forward toggles must match pacing before them.

tools/mac_fftoggle.py RUNDIR [--toggles 20] [--renderer gpu] [--res 3] [--panel 120] [--save town] [--smooth 1]

One tools/mac_run.sh run (no frame skip) from a save: Frame rate Monitor with a pretended 120 Hz display link
(BWPORT_PANELHZ: the iOS present path, slots locked to the link and the panel model's drops, on the desktop), two `bench:`
windows, then N toggles of the fast-forward button's path (Game speed 4x for 480 frames, 1x for 120), then two more bench
windows. Compares the means of before and after: speed reached, presents/s, drawn/s, in-between pictures/s and present
drops/s, and exits 1 when one got worse by more than its tolerance (--check-only: compare an earlier run's log). The machine should be quiet (uptime); a loaded one adds noise.
"""
import argparse, os, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ap = argparse.ArgumentParser()
ap.add_argument("rundir")
ap.add_argument("--toggles", type=int, default=20)
ap.add_argument("--renderer", default="gpu")
ap.add_argument("--res", type=int, default=3)
ap.add_argument("--panel", type=int, default=120)
ap.add_argument("--save", default="town")
ap.add_argument("--smooth", type=int, default=1)
ap.add_argument("--check-only", action="store_true", help="only compare the bench rows of RUNDIR/log.txt")
a = ap.parse_args()

steps = [f"f1200:renderer={a.renderer},resolution={a.res},smooth={a.smooth},framerate=monitor",
         "p300:bench=before1", "p1:bench=before2"]
for _ in range(a.toggles):
    steps += ["p1:speed=4", "p480:speed=1", "p120:speed=1"]
steps += ["p60:bench=after1", "p1:bench=after2", "p5:quit=1"]
env = dict(os.environ, BWPORT_CFGSCRIPT=";".join(steps), BWPORT_PANELHZ=str(a.panel))
secs = 60 + a.toggles * 8
if not a.check_only:
    subprocess.run([os.path.join(ROOT, "tools/mac_run.sh"), "--noskip", "--save", a.save, a.rundir, str(secs)], env=env,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

NUM = ["speed x", "pres/s", "drawn/s", "mid/s", "upMB/s", "swap ms/s", "gap ms", "cores", "CPU ms/tick", "gpu lag ms", "max",
       "in flight", "shown ms", "drops", "behind/s", "gpu/s", "late/s", "none/s", "held/s", "mdrop/s", "glate/s"]  # the bench row's last numbers (sim_main.cpp BenchDone)
rows = {}
for line in open(os.path.join(a.rundir, "log.txt"), errors="replace"):
    f = line.split()
    if len(f) >= 2 + len(NUM) and f[0] == "bench:" and f[1] != "step":
        rows[f[1]] = dict(zip(NUM, f[-len(NUM):]))
need = ["before1", "before2", "after1", "after2"]
if any(n not in rows for n in need):
    sys.exit(f"fftoggle: bench rows missing ({sorted(rows)}); see {a.rundir}/log.txt")

def mean(pre, col, per_s=False):
    v = [float(rows[pre + i][col]) for i in "12"]
    return sum(v) / 2 / (4.0 if per_s else 1.0)  # (drops are a 4 s window's count)

# column, tolerance (absolute), drops are per second
checks = [("speed x", 0.03, False), ("pres/s", 3.0, False), ("drawn/s", 2.0, False), ("mid/s", 6.0, False), ("drops", 1.0, True)]
bad = 0
for col, tol, per_s in checks:
    b, f = mean("before", col, per_s), mean("after", col, per_s)
    worse = (f - b > tol) if col == "drops" else (b - f > tol)  # (worse only: fewer, or more drops)
    bad += worse
    print(f"fftoggle: {col + ('/s' if per_s else ''):10s} before {b:7.2f}  after {f:7.2f}  {'FAIL' if worse else 'ok'}")
print(f"fftoggle: {a.toggles} toggles, {'FAIL' if bad else 'PASS'}")
sys.exit(1 if bad else 0)
