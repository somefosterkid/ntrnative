#!/usr/bin/env python3
"""touch_hold_test.py (tp73): the stylus stays down while the left button is held. The exe's BWPORT_TOUCHLOG gets, besides the
layout line, "ev down|up VBLANK X Y" (the button events on the touch screen) and "tp VBLANK touch T X Y" (every touch sample the
GAME took, at the V-blank count it took it: the touched ones and the first untouched one after). The cases:
  hold   the button held still for N frames: every frame from the one after the press to the one the release came in is touched,
         at the pressed position, and the next frame is not (the release is seen at once)
  move   held and moved in steps: every frame touched, the positions are the path's, in order, ending at the last one
  leave  held, moved off the touch screen and back: lifted while off, down again on return, no frame touched while off
  quick  press and release in one command (no time between): each press gives the game at least one touched frame
  quick2 the same with the move before it, 40 presses
Each case runs for the 60 fps cap and uncapped, FrameSkip 0 and 3.
usage: touch_hold_test.py [--only substr,...] [--jobs N]     (run from the worktree, build/port/linux/bwport built)"""
import os, sys, subprocess, time, shutil, multiprocessing

REPO = os.getcwd()
sys.path.insert(0, REPO + '/tools')
import port_run
EXE = REPO + '/build/port/linux/bwport'
SRC = REPO + '/build/port/run'
OUT = '/root/mt/tp73/hold'
os.makedirs(OUT, exist_ok=True)


def sh(c, env=None, **k):
    return subprocess.run(c, env=env, capture_output=True, text=True, **k)


def make_ini(c):
    return f"""[General]
WindowWidth=0
WindowHeight=0
FullScreen=false
IntegerScale=false
MasterVolume=0
ScreenLayout={c.get('layout', 'vertical')}
SwapScreens=false
VsyncInterval=0
FrameRate={'60' if c['cap'] else 'unlimited'}
ShowInputPanel=false
ShowMetrics=false
ShowMenuBar=false
Renderer=vcore
"""


class Rig:
    def __init__(self, c, idx):
        self.c = c
        self.idx = idx
        self.rd = f'{OUT}/rd{idx}'
        self.tl = f'{OUT}/tl{idx}.txt'
        self.fails = []

    def start(self):
        c = self.c
        shutil.rmtree(self.rd, ignore_errors=True)
        os.makedirs(self.rd)
        for f in os.listdir(SRC):
            if f not in ('out', 'sim_config.ini', 'save.bin', 'DbgLog.txt'):
                os.symlink(SRC + '/' + f, self.rd + '/' + f)
        open(self.rd + '/sim_config.ini', 'w').write(make_ini(c))
        if os.path.exists(self.tl):
            os.remove(self.tl)
        n = 540 + self.idx
        lock = f'/tmp/.X{n}-lock'
        if os.path.exists(lock):
            try:
                pid = int(open(lock).read().strip())
                if open(f'/proc/{pid}/comm').read().startswith('Xvfb'):   # a stale lock's pid may be reused: only an Xvfb
                    os.kill(pid, 9)
            except Exception:
                pass
            try:
                os.remove(lock)
            except OSError:
                pass
        self.xvfb = subprocess.Popen(['Xvfb', f':{n}', '-screen', '0', '1600x1200x24', '-nolisten', 'tcp'],
                                     stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        self.env = dict(os.environ, DISPLAY=f':{n}', SDL_VIDEODRIVER='x11', WAYLAND_DISPLAY='', SDL_AUDIODRIVER='dummy', BWPORT_ROM=port_run.find_rom(),
                        BWPORT_TOUCHLOG=self.tl, BWPORT_FRAMESKIP=str(c.get('skip', 0)))
        for _ in range(100):
            if port_run.x_listening(n) and sh(['xdotool', 'getdisplaygeometry'], env=self.env).returncode == 0:
                break
            time.sleep(0.05)
        self.proc = subprocess.Popen([EXE], cwd=self.rd, env=self.env, stdout=open(self.rd + '/log', 'w'),
                                     stderr=subprocess.STDOUT, start_new_session=True)
        t0 = time.time()
        self.win = None
        while time.time() - t0 < 60:
            r = sh(['xdotool', 'search', '--onlyvisible', '--pid', str(self.proc.pid)], env=self.env)
            ids = [i for i in r.stdout.split() if i]
            if ids and self.lines('layout'):
                self.win = ids[-1]
                break
            time.sleep(0.3)
        if not self.win:
            return False
        # the game samples the touch screen from its first frames; wait until it does (a "tp" line needs a touch, so look at
        # the V-blank count through a click)
        time.sleep(2.0)
        self.touch = self.rect()
        return True

    def stop(self):
        try:
            os.killpg(self.proc.pid, 9)
        except OSError:
            pass
        self.proc.wait()
        self.xvfb.kill()
        self.xvfb.wait()

    def lines(self, prefix):
        try:
            return [l for l in open(self.tl).read().splitlines() if l.startswith(prefix)]
        except OSError:
            return []

    def rect(self):
        t = self.lines('layout')[-1].split()
        i = t.index('touch')
        return tuple(float(v) for v in t[i + 1:i + 5])

    def at(self, fx, fy):
        """window pixel for the fraction (fx, fy) of the touch screen"""
        x0, y0, x1, y1 = self.touch
        return int(x0 + (x1 - x0) * fx), int(y0 + (y1 - y0) * fy)

    def expect(self, px, py):
        x0, y0, x1, y1 = self.touch
        return (int((px + 0.5 - x0) * 256 / (x1 - x0)), int((py + 0.5 - y0) * 192 / (y1 - y0)))

    def mouse(self, x, y):
        sh(['xdotool', 'mousemove', '--window', self.win, str(int(x)), str(int(y))], env=self.env)

    def fail(self, tag, msg):
        self.fails.append(f'{tag}: {msg}')
        print(f'FAIL [{self.c["name"]}] {tag}: {msg}', flush=True)

    def mark(self):
        return len(self.lines('tp')), len(self.lines('ev'))

    def new(self, m):
        tp = [l.split() for l in self.lines('tp')[m[0]:]]
        ev = [l.split() for l in self.lines('ev')[m[1]:]]
        return ([(int(a[1]), int(a[3]), int(a[4]), int(a[5])) for a in tp],
                [(a[1], int(a[2]), int(a[3]), int(a[4])) for a in ev])


def segments(tp):
    """tp: [(vblank, touch, x, y)] -> runs of touched samples [(first, last, [(x, y)...])]"""
    segs = []
    for v, t, x, y in tp:
        if t:
            if segs and segs[-1][1] == v - 1:
                segs[-1][1] = v
                segs[-1][2].append((x, y))
            else:
                segs.append([v, v, [(x, y)]])
    return segs


def case_hold(r):
    px, py = r.at(0.4, 0.5)
    ex = r.expect(px, py)
    r.mouse(px, py)
    time.sleep(0.2)
    m = r.mark()
    sh(['xdotool', 'mousedown', '1'], env=r.env)
    time.sleep(1.5)
    sh(['xdotool', 'mouseup', '1'], env=r.env)
    time.sleep(0.4)
    tp, ev = r.new(m)
    if [e[0] for e in ev] != ['down', 'up']:
        return r.fail('hold', f'events {ev}')
    cd, cu = ev[0][1], ev[1][1]
    touched = [v for v, t, x, y in tp if t]
    want = list(range(cd + 1, cu + 1))
    if cu - cd < 20:
        return r.fail('hold', f'held only {cu - cd} frames')
    if touched != want:
        miss = sorted(set(want) - set(touched))
        extra = sorted(set(touched) - set(want))
        return r.fail('hold', f'touched frames {len(touched)} of {len(want)} (V-blank {cd}..{cu}); missing {miss[:8]} extra {extra[:8]}')
    bad = [(v, x, y) for v, t, x, y in tp if t and (x, y) != ex]
    if bad:
        return r.fail('hold', f'position {bad[:3]} want {ex}')
    after = [t for v, t, x, y in tp if v == cu + 1]
    if after != [0]:
        return r.fail('hold', f'frame after the release: {after}')
    print(f'ok   [{r.c["name"]}] hold: {len(want)} frames touched at {ex}, lifted at {cu + 1}', flush=True)


def case_move(r):
    pts = [r.at(0.2 + 0.05 * i, 0.3 + 0.04 * i) for i in range(12)]
    r.mouse(*pts[0])
    time.sleep(0.2)
    m = r.mark()
    sh(['xdotool', 'mousedown', '1'], env=r.env)
    time.sleep(0.25)
    for p in pts[1:]:
        r.mouse(*p)
        time.sleep(0.12)
    time.sleep(0.2)
    sh(['xdotool', 'mouseup', '1'], env=r.env)
    time.sleep(0.4)
    tp, ev = r.new(m)
    if [e[0] for e in ev] != ['down', 'up']:
        return r.fail('move', f'events {ev}')
    cd, cu = ev[0][1], ev[1][1]
    touched = [v for v, t, x, y in tp if t]
    if touched != list(range(cd + 1, cu + 1)):
        return r.fail('move', f'touched frames {len(touched)} of {cu - cd}')
    path = [r.expect(*p) for p in pts]
    seen = []
    for v, t, x, y in tp:
        if t and (not seen or seen[-1] != (x, y)):
            seen.append((x, y))
    if seen != path:
        return r.fail('move', f'positions seen {seen} want {path}')
    print(f'ok   [{r.c["name"]}] move: {len(touched)} frames touched, {len(seen)} positions in order', flush=True)


def case_leave(r):
    inp = r.at(0.5, 0.5)
    out = (int(r.touch[0]) - 40, inp[1]) if r.touch[0] > 60 else (inp[0], int(r.touch[1]) - 40)
    ex = r.expect(*inp)
    r.mouse(*inp)
    time.sleep(0.2)
    m = r.mark()
    sh(['xdotool', 'mousedown', '1'], env=r.env)
    time.sleep(0.4)
    r.mouse(*out)
    time.sleep(0.5)
    r.mouse(*inp)
    time.sleep(0.4)
    sh(['xdotool', 'mouseup', '1'], env=r.env)
    time.sleep(0.4)
    tp, ev = r.new(m)
    segs = segments(tp)
    if len(segs) != 2:
        return r.fail('leave', f'{len(segs)} touched runs {[(s[0], s[1]) for s in segs]}, want 2')
    gap = segs[1][0] - segs[0][1] - 1
    if gap < 10:
        return r.fail('leave', f'lifted for only {gap} frames')
    if any(set(s[2]) != {ex} for s in segs):
        return r.fail('leave', f'positions {set(segs[0][2])} {set(segs[1][2])} want {ex}')
    cd = [e for e in ev if e[0] == 'down'][0][1]
    cu = [e for e in ev if e[0] == 'up'][0][1]
    if segs[0][0] != cd + 1 or segs[1][1] != cu:
        return r.fail('leave', f'runs {segs[0][0]}..{segs[0][1]}, {segs[1][0]}..{segs[1][1]}; button {cd}..{cu}')
    print(f'ok   [{r.c["name"]}] leave: 2 runs, lifted {gap} frames while off the screen', flush=True)


def case_quick(r, n=40, tag='quick'):
    px, py = r.at(0.6, 0.6)
    ex = r.expect(px, py)
    r.mouse(px, py)
    time.sleep(0.2)
    m = r.mark()
    for i in range(n):
        sh(['xdotool', 'mousedown', '1', 'mouseup', '1'], env=r.env)
        time.sleep(0.09 + 0.002 * (i % 7))
    time.sleep(0.4)
    tp, ev = r.new(m)
    downs = [e for e in ev if e[0] == 'down']
    segs = segments(tp)
    if len(downs) != n:
        return r.fail(tag, f'{len(downs)} presses reached the touch code, want {n}')
    if len(segs) != n:
        return r.fail(tag, f'{len(segs)} of {n} presses gave the game a touched frame')
    if any(set(s[2]) != {ex} for s in segs):
        return r.fail(tag, f'positions {[s[2][0] for s in segs if set(s[2]) != {ex}][:3]} want {ex}')
    lens = [s[1] - s[0] + 1 for s in segs]
    print(f'ok   [{r.c["name"]}] {tag}: {n} presses, {len(segs)} touched runs (frames per run min {min(lens)} max {max(lens)})', flush=True)


CASES = {'hold': case_hold, 'move': case_move, 'leave': case_leave, 'quick': case_quick}


def run_config(a):
    c, idx, only = a
    r = Rig(c, idx)
    ok = r.start()
    if not ok:  # the window search races the exe's first frame under load: once more
        r.stop()
        r = Rig(c, idx)
        ok = r.start()
    if not ok:
        print(f'FAIL [{c["name"]}] start', flush=True)
        r.stop()
        return [f'{c["name"]}: start']
    try:
        for name, fn in CASES.items():
            if only and not any(o in name for o in only):
                continue
            fn(r)
    finally:
        r.stop()
    return r.fails


def main():
    only = []
    jobs = int(os.environ.get('BW_JOBS', '4'))
    i = 1
    while i < len(sys.argv):
        if sys.argv[i] == '--only':
            only = sys.argv[i + 1].split(',')
            i += 1
        elif sys.argv[i] == '--jobs':
            jobs = int(sys.argv[i + 1])
            i += 1
        i += 1
    cfgs = []
    for cap in (True, False):
        for skip in (0, 3):
            cfgs.append({'name': f'{"cap" if cap else "fast"}-skip{skip}', 'cap': cap, 'skip': skip})
    cfgs.append({'name': 'cap-horizontal', 'cap': True, 'skip': 0, 'layout': 'horizontal'})
    ctx = multiprocessing.get_context('fork')
    with ctx.Pool(min(jobs, len(cfgs), 4)) as p:
        res = p.map(run_config, [(c, k, only) for k, c in enumerate(cfgs)])
    fails = [f for r in res for f in r]
    print('touch_hold_test:', 'FAIL %d' % len(fails) if fails else 'PASS', flush=True)
    return 1 if fails else 0


if __name__ == '__main__':
    sys.exit(main())
