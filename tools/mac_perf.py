#!/usr/bin/env python3
"""macOS performance runs (docs/perf.md, "M2 macOS, 2026-11-01"), the fast way: ONE hidden bwport run per scene that steps through the
renderer x Resolution configs live (BWPORT_CFGSCRIPT, libntr-ios-cfgscript.patch: the same fields the Config window sets), and in each
config records a 60 fps capped window and an unlimited window (BWPORT_PERFLOG machinery, perf_NAME.txt, 5 s each, after a warm-up).
The scene comes from a save (tools/mac_run.sh's ~/bw-saves) or the boot key script; run folders are clones of the
extracted template (tools/mac_runbase.sh).  Usage:
  tools/mac_perf.py matrix RUNDIR OUT.jsonl [--scenes title,dialog,room] [--configs vcore:1,gpu:1,...] [--quiet LOAD] [--maxwait S]
Prints and appends one JSON row per config, scene and mode (capped / unlimited); tools/mac_perf_table.py makes the table.
Quiet check: before each scene run it waits (at most --maxwait, default 300 s) until the 1-minute load average is below --quiet
(default 6); each row logs the load average at the window's start and end (load0, load1) and the other processes' CPU in cores over
the window (ext); a window is DISTURBED when ext >= 1.0 cores or the load rose by 2 or more during it, and a scene's disturbed
configs are run again (up to 2 more scene runs).  The disturbed flag stays in the row when the last attempt was disturbed too.
"""
import json, os, re, shutil, statistics, subprocess, sys, time, signal

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SAVES = os.path.expanduser(os.environ.get("BWPORT_SAVES", "~/bw-saves"))
CONTINUE = "start@f600 start@f750 start@f900 a@f1000"      # tools/mac_run.sh's
# scene: (save or None, the keys, the frame the first config starts at)
BOOT = "start@f600 start@f900 start@f1300 " + " ".join("a*4@f%d" % f for f in range(1700, 5781, 30))
SCENES = {"title": (None, "start@f600", 900), "dialog": (None, BOOT, 5900), "room": ("room", CONTINUE, 1100)}
CONFIGS = [(r, n) for n in (1, 2, 4, 8) for r in ("vcore", "gpu")]
WARM = 150      # frames after a config switch before its window
EXT_BUSY, LOAD_RISE = 1.0, 2.0


def sh(*a):
    return subprocess.run(list(a), capture_output=True, text=True).stdout


def cpu_all():
    """pid -> CPU seconds of every process (ps time)."""
    d = {}
    for l in sh("ps", "-axo", "pid=,time=").splitlines():
        p = l.split()
        m = re.match(r"(?:(\d+):)?(\d+):(\d+\.\d+)$", p[1]) if len(p) == 2 else None
        if m:
            h, mi, s = m.groups()
            d[int(p[0])] = int(h or 0) * 3600 + int(mi) * 60 + float(s)
    return d


def wait_quiet(thr, maxwait):
    t0 = time.time()
    while os.getloadavg()[0] >= thr and time.time() - t0 < maxwait:
        time.sleep(5)
    return os.getloadavg()[0], time.time() - t0


def prepare(rundir, save):
    if not os.path.isdir(rundir):
        base = sh(os.path.join(ROOT, "tools/mac_runbase.sh")).strip()
        subprocess.run(["cp", "-cR", base, rundir], check=True)
    if save:
        shutil.copyfile(os.path.join(SAVES, save + ".bin"), os.path.join(rundir, "save.bin"))


def script(configs, start):
    """the CFGSCRIPT: per config a 60 fps window, then an unlimited one (each: warm-up, perf log 5 s)."""
    st = []
    for i, (r, n) in enumerate(configs):
        st.append(("f%d" % start if i == 0 else "p60") + ":renderer=%s,resolution=%d,framerate=60" % (r, n))
        st += ["p%d:perflog=cap%d" % (WARM, i), "p60:framerate=unlimited", "p%d:perflog=unl%d" % (WARM, i)]
    st.append("p60:quit=1")
    return ";".join(st)


def scene_run(rundir, scene, configs, maxsec=1500):
    save, keys, start = SCENES[scene]
    prepare(rundir, save)
    for f in os.listdir(rundir):
        if f.startswith("perf_") and f.endswith(".txt"):
            os.remove(os.path.join(rundir, f))
    rom = os.environ.get("BWPORT_ROM") or sh(os.path.join(ROOT, "tools/mac_rom.sh")).strip()
    env = dict(os.environ, BWPORT_DIR=rundir, SDL_AUDIODRIVER="dummy", BWPORT_ROM=rom, BWPORT_HIDDEN="1", BWPORT_KEYS=keys,
               BWPORT_FRAMERATE="unlimited", BWPORT_FRAMESKIP="0", BWPORT_CFGSCRIPT=script(configs, start), BWPORT_BREAKDOWN="1")
    for k in ("BWPORT_RENDERER", "BWPORT_RESOLUTION"):
        env.pop(k, None)
    p = subprocess.Popen([os.path.join(ROOT, "build/port/macos/bwport")], cwd=rundir, env=env,
                         stdout=open(os.path.join(rundir, "log.txt"), "w"), stderr=subprocess.STDOUT)
    me, ext, last, samples = os.getpid(), 0.0, cpu_all(), []
    t0 = time.time()
    while p.poll() is None and time.time() - t0 < maxsec:
        time.sleep(1.0)
        cur = cpu_all()
        ext += sum(max(0.0, c - last.get(pid, c)) for pid, c in cur.items() if pid not in (p.pid, me))
        last = cur
        samples.append((time.time(), cur.get(p.pid, 0.0), ext, os.getloadavg()[0]))
    if p.poll() is None:
        p.send_signal(signal.SIGKILL)   # SIGTERM is ignored by the game
    p.wait()
    wall = time.time() - t0

    def at(t):
        return min(samples, key=lambda s: abs(s[0] - t))
    rows = {}
    for i, (r, n) in enumerate(configs):
        for mode, tag, nmax in (("capped", "cap", 300), ("unlimited", "unl", 600)):
            log = os.path.join(rundir, "perf_%s%d.txt" % (tag, i))
            if not os.path.exists(log) or not samples:
                continue
            w1 = os.path.getmtime(log)
            a, b = at(w1 - 5.0), at(w1)
            dt = max(b[0] - a[0], 1e-6)
            d = summarize(log, nmax)
            q = lambda v: [round(statistics.median(v), 3), round(pct(v, 0.99), 3)] if v else None
            row = dict(renderer=r, res=n, scene=scene, mode=mode, n=d["n"], fps=round(d["fps"], 1), period=q(d["period"]),
                       work=q(d["work"]), game=q(d["game"]), render=q(d["render"]), present=q(d["present"]),
                       worker=q(d["worker"]), gpu=q(d["gpu"]), cores=round((b[1] - a[1]) / dt, 2), ext=round((b[2] - a[2]) / dt, 2),
                       load0=round(a[3], 1), load1=round(b[3], 1))
            row["disturbed"] = row["ext"] >= EXT_BUSY or row["load1"] - row["load0"] >= LOAD_RISE
            rows[(r, n, mode)] = row
    return rows, wall


def pct(v, q):
    v = sorted(v)
    return v[min(len(v) - 1, int(q * len(v)))]


def summarize(log, nmax):
    """median / p99 of the frame's period, its work (period minus the frame-rate wait) split game / render / present, the
    renderer's worker (vcore's draw, gpu's stage 1) and the GPU's busy time; fps from the mean period. Rows past nmax are left out."""
    rows = [l.split() for l in open(log) if l[:1].isdigit()]
    names = [l.split()[2:] for l in open(log) if l.startswith("frame period")][0]
    ix = {n: i + 1 for i, n in enumerate(["period"] + names)}
    col = lambda n: [float(r[ix[n]]) for r in rows[30:nmax + 30] if n in ix]
    def sm(*ns):
        cs = [col(n) for n in ns if n in ix]
        return [sum(t) for t in zip(*cs)] if cs else []
    d = {}
    d["n"] = len(col("period"))
    d["fps"] = 1000.0 * d["n"] / sum(col("period"))
    d["period"] = col("period")
    d["game"] = sm("frame:game_logic", "frame:game_V-blank", "frame:sound_step")
    d["render"] = sm("frame:renderer", "frame:picture_upload_+_quad")
    d["present"] = sm("frame:events_+_UI_build", "frame:UI_draw", "frame:swap")
    d["work"] = [a + b + c for a, b, c in zip(d["game"], d["render"], d["present"])]
    d["worker"] = sm("worker:draw_(whole)") or sm("worker:stage_1_(whole)")
    d["gpu"] = sm(*[n for n in ix if n.startswith("gpu:")])
    return d


def matrix(rundir, out, scenes, configs, quiet, maxwait):
    t00 = time.time()
    for scene in scenes:
        todo, done = list(configs), {}
        for attempt in range(3):
            load, waited = wait_quiet(quiet, maxwait)
            print("%s attempt %d: %d configs, load %.1f after waiting %.0f s" % (scene, attempt + 1, len(todo), load, waited), flush=True)
            rows, wall = scene_run(rundir, scene, todo)
            print("  scene run %.0f s, %d windows" % (wall, len(rows)), flush=True)
            for k, row in rows.items():
                old = done.get(k)
                if old is None or old["disturbed"]:
                    done[k] = row
            todo = [c for c in todo if any(done.get((c[0], c[1], m)) is None or done[(c[0], c[1], m)]["disturbed"]
                                           for m in ("capped", "unlimited"))]
            if not todo:
                break
        for c in configs:
            for m in ("capped", "unlimited"):
                r = done.get((c[0], c[1], m))
                if r:
                    open(out, "a").write(json.dumps(r) + "\n")
                    print(r, flush=True)
    print("matrix wall %.0f s" % (time.time() - t00))


if __name__ == "__main__":
    a = sys.argv[1:]
    opt = lambda k, d: a[a.index(k) + 1] if k in a else d
    if a and a[0] == "matrix":
        cfgs = [(c.split(":")[0], int(c.split(":")[1])) for c in opt("--configs", "").split(",") if c] or CONFIGS
        matrix(a[1], a[2], opt("--scenes", "title,dialog,room").split(","), cfgs, float(opt("--quiet", 6)), float(opt("--maxwait", 300)))
    else:
        sys.exit(__doc__)
