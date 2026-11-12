#!/usr/bin/env python3
"""ptest2.py (ux69): the mouse-as-touch-screen tests in every layout. The touch screen, the bar, the side and the settings
windows are read from the exe's own BWPORT_TOUCHLOG 'layout' line (libntr-ux69.patch), not found by colour.
usage: ptest2.py [--smoke | --only substr,...] [--jobs N] [--shots]   (--smoke: 11 configs, one per layout family; the layout
model tools/touch_layout.py is checked against the exe in every config; all 222 configs without the flag)   (run from the worktree, with build/port/linux/bwport built)"""
import os, re, subprocess, sys, time, shutil, multiprocessing, math, json

REPO = os.getcwd()
sys.path.insert(0, REPO + '/tools')
import port_run
import touch_layout
EXE = REPO + '/build/port/linux/bwport'
SRC = REPO + '/build/port/run'
# each worktree its own X displays and run dirs (two agents' runs killed each other's Xvfb through the lock files)
import zlib
TAG = 1 + zlib.crc32(REPO.encode()) % 60
BASE = f'/root/mt/ux69/w{TAG}'
OUT = BASE + '/pt'
os.makedirs(OUT, exist_ok=True)
LAYOUTS = ['vertical', 'horizontal', 'large']


def sh(c, env=None, **k):
    return subprocess.run(c, env=env, capture_output=True, text=True, **k)


def make_ini(c):
    side = c['side']
    return f"""[General]
WindowWidth={c.get('ww', 0)}
WindowHeight={c.get('wh', 0)}
FullScreen={'true' if c['fs'] else 'false'}
IntegerScale={'true' if c['int'] else 'false'}
MasterVolume=0
ScreenLayout={c['layout']}
SwapScreens={'true' if c['swap'] else 'false'}
VsyncInterval=0
FrameRate=60
ShowInputPanel={'true' if side in ('both', 'pad') else 'false'}
ShowMetrics={'true' if side in ('both', 'met') else 'false'}
ShowMenuBar={'true' if c['bar'] != 'off' else 'false'}
FullscreenMenuBar={'always' if c['bar'] == 'always' else 'autohide'}
Renderer=vcore
"""


def parse_layout(line):
    t = line.split()
    d = {}
    d['layout'] = int(t[1]); d['swap'] = int(t[3]); d['fs'] = int(t[5]); d['int'] = int(t[7])
    d['W'], d['H'] = int(t[9]), int(t[10])
    d['view'] = tuple(int(x) for x in t[12:16])
    d['touch'] = tuple(float(x) for x in t[17:21])
    i = 21
    assert t[i] == 'bar'
    d['reserve'] = int(t[i + 1]); d['vis'] = int(t[i + 3])
    assert t[i + 4] == 'side'
    d['side'] = tuple(float(x) for x in t[i + 5:i + 9])
    d['wins'] = []
    j = i + 11
    while j < len(t):
        if t[j].startswith('win'):
            d['wins'].append(tuple(float(x) for x in t[j + 1:j + 5]))
            j += 5
        else:
            j += 1
    return d


class Rig:
    def __init__(self, c, idx):
        self.c = c
        self.idx = idx
        self.rd = f'{BASE}/rd{idx}'
        self.tl = f'{BASE}/tl{idx}.txt'
        self.res = []
        self.fails = 0
        self.npts = 0

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
        n = 500 + TAG * 16 + self.idx
        sw, shh = c.get('screen', (2600, 1500))
        for lock in (f'/tmp/.X{n}-lock',):
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
        self.xvfb = subprocess.Popen(['Xvfb', f':{n}', '-screen', '0', f'{sw}x{shh}x24', '-nolisten', 'tcp'],
                                     stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        self.env = dict(os.environ, DISPLAY=f':{n}', SDL_VIDEODRIVER='x11', WAYLAND_DISPLAY='', SDL_AUDIODRIVER='dummy', BWPORT_ROM=port_run.find_rom(),
                        BWPORT_TOUCHLOG=self.tl, BWPORT_UIOPEN=c.get('open', ''), BWPORT_FRAMESKIP='0')
        for _ in range(100):
            if port_run.x_listening(n) and sh(['xdotool', 'getdisplaygeometry'], env=self.env).returncode == 0:
                break
            time.sleep(0.05)
        self.proc = subprocess.Popen([EXE], cwd=self.rd, env=self.env, stdout=open(self.rd + '/log', 'w'),
                                     stderr=subprocess.STDOUT, start_new_session=True)
        t0 = time.time()
        self.win = None
        # readiness = the window and the first 'layout' line of the exe's touch log; no fixed timeout short of a hung start
        # (the exe is dead, or 300 s: a load-dependent 60 s gave 'no window' flakes)
        while time.time() - t0 < 300 and self.proc.poll() is None:
            r = sh(['xdotool', 'search', '--onlyvisible', '--pid', str(self.proc.pid)], env=self.env)
            ids = [i for i in r.stdout.split() if i]
            if ids and self.lines('layout'):
                self.win = ids[-1]
                break
            time.sleep(0.3)
        if self.win and c['fs']:
            # no window manager: SDL's fullscreen flag does nothing to the X window, so make it fill the screen
            sh(['xdotool', 'windowmove', self.win, '0', '0'], env=self.env)
            sh(['xdotool', 'windowsize', self.win, str(sw), str(shh)], env=self.env)
            time.sleep(1.0)
        return self.win is not None

    def why(self):
        try:
            tail = open(self.rd + '/log').read().strip().splitlines()[-2:]
        except OSError:
            tail = []
        st = 'exited %s' % self.proc.poll() if self.proc.poll() is not None else 'alive'
        return 'exe %s, %d layout lines, window %s, log: %s' % (st, len(self.lines('layout')), self.win, ' / '.join(tail)[:200])

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

    def state(self):
        ls = self.lines('layout')
        # the last line that is not the first frame's (windows start 28x47 for a frame)
        return parse_layout(ls[-1])

    def mouse(self, x, y):
        sh(['xdotool', 'mousemove', '--window', self.win, str(int(x)), str(int(y))], env=self.env)

    def down(self):
        sh(['xdotool', 'mousedown', '1'], env=self.env)

    def up(self):
        sh(['xdotool', 'mouseup', '1'], env=self.env)

    def key(self, k):
        sh(['xdotool', 'key', k], env=self.env)

    def click(self, x, y, hold=0.12):
        """a click at window pixel (x, y); returns the new touch lines"""
        n0 = len(self.lines('mouse'))
        self.mouse(x, y)
        time.sleep(0.1)
        self.down()
        time.sleep(hold)
        self.up()
        time.sleep(0.08)
        return self.lines('mouse')[n0:]

    def fail(self, tag, msg):
        self.fails += 1
        self.res.append(f'  FAIL {tag}: {msg}')

    def shot(self, name):
        try:
            x = OUT + f'/{self.idx}.xwd'
            sh(['xwd', '-silent', '-id', self.win, '-out', x], env=self.env)
            port_run.xwd_to_png(x, OUT + '/' + name + '.png')
        except Exception:
            pass


def covered(st, px, py, margin=3):
    """is the pixel under the bar (when drawn) or a settings window: True / False, or None near a window's edge
    (fractional rectangles: not judged)"""
    if py < st['vis']:
        return True
    r = False
    for (x, y, w, h) in st['wins']:
        if x + margin <= px < x + w - margin and y + margin <= py < y + h - margin:
            return True
        if x - margin <= px < x + w + margin and y - margin <= py < y + h + margin:
            r = None
    return r


def expect_point(st, px, py):
    """(tx, ty) expected for a click at pixel (px, py), 'none', or None (not judged)"""
    x0, y0, x1, y1 = st['touch']
    mx, my = px + 0.5, py + 0.5
    if not (x0 <= mx < x1 and y0 <= my < y1):
        return 'none'
    cv = covered(st, px, py)
    if cv is None:
        return None
    if cv:
        return 'none'
    tx = min(255, int((mx - x0) * 256 / (x1 - x0)))
    ty = min(191, int((my - y0) * 192 / (y1 - y0)))
    return tx, ty


def dismiss_popup(rig, st):
    """a click that landed on a settings window may have opened a combo's popup; the next click anywhere else only
    closes it (it is no touch, as it should be). Click a point nothing covers and outside the touch screen first."""
    x0, y0, x1, y1 = st['touch']
    W, H = st['W'], st['H']
    for (px, py) in [(W - 2, H - 2), (1, H - 2), (W - 2, int(st['vis']) + 2), (1, int(st['vis']) + 2), (W // 2, H - 2)]:
        mx, my = px + 0.5, py + 0.5
        if x0 <= mx < x1 and y0 <= my < y1:
            continue
        sx, sy, sw, sh_ = st["side"]
        if sw > 0 and sx <= mx < sx + sw and sy <= my < sy + sh_:
            continue
        if covered(st, px, py) is False:
            rig.click(px, py)
            return


def check_click(rig, tag, st, px, py, name, want_touch=None):
    """click at (px, py) and compare with the expected touch (a None expectation = no touch). want_touch: force an
    expectation (tx, ty) or 'none'"""
    if px < 0 or py < 0 or px >= st['W'] or py >= st['H']:
        return
    ex = expect_point(st, px, py) if want_touch is None else want_touch
    if ex is None:
        return
    if ex == 'none':
        ex = None
    ls = rig.click(px, py)
    rig.npts += 1
    if ex is None and covered(st, px, py) is True and py >= st['vis']:
        dismiss_popup(rig, st)
    m = [re.search(r'touch (\d+) (\d+)', l) for l in ls]
    m = [(int(a.group(1)), int(a.group(2))) for a in m if a]
    if ex is None:
        if m:
            rig.fail(tag, f'{name} ({px},{py}) touched {m[-1]}, expected no touch')
    else:
        if not m:
            rig.fail(tag, f'{name} ({px},{py}) NO TOUCH, expected {ex}')
        else:
            if abs(m[-1][0] - ex[0]) > 1 or abs(m[-1][1] - ex[1]) > 1:
                rig.fail(tag, f'{name} ({px},{py}) touch {m[-1]} expected {ex}')


def pixel_tl(v):
    return int(math.ceil(v - 0.5))


def pixel_last(v):
    return int(math.ceil(v - 0.5)) - 1


def test_state(rig, tag, with_ui=False):
    """the corner / centre / outside clicks on the current layout"""
    st = rig.state()
    x0, y0, x1, y1 = st['touch']
    L, T, R_, B = pixel_tl(x0), pixel_tl(y0), pixel_last(x1), pixel_last(y1)
    cx, cy = (L + R_) // 2, (T + B) // 2
    # a corner under the bar (autohide) or a window is covered: shown in the notes, expected no touch
    check_click(rig, tag, st, L, T, 'tl')
    check_click(rig, tag, st, R_, T, 'tr')
    check_click(rig, tag, st, L, B, 'bl')
    check_click(rig, tag, st, R_, B, 'br')
    check_click(rig, tag, st, cx, cy, 'centre')
    for (px, py, nm) in [(L - 1, cy, 'outL'), (R_ + 1, cy, 'outR'), (cx, T - 1, 'outT'), (cx, B + 1, 'outB')]:
        check_click(rig, tag, st, px, py, nm, 'none')
    return st


def test_stroke(rig, tag, st):
    x0, y0, x1, y1 = st['touch']
    L, T, R_, B = pixel_tl(x0), pixel_tl(y0), pixel_last(x1), pixel_last(y1)
    if st['vis'] and T < st['vis'] + 4:
        T = int(st['vis']) + 4
    a = (L + (R_ - L) // 10, T + (B - T) // 10)
    b = (R_ - (R_ - L) // 10, B - (B - T) // 10)
    n0 = len(rig.lines('mouse'))
    rig.mouse(*a); time.sleep(0.1); rig.down(); time.sleep(0.1)
    steps = 14
    for i in range(1, steps + 1):
        rig.mouse(a[0] + (b[0] - a[0]) * i // steps, a[1] + (b[1] - a[1]) * i // steps)
        time.sleep(0.08)
    time.sleep(0.05)
    rig.up(); time.sleep(0.1)
    rig.npts += 1
    ms = [re.search(r'touch (\d+) (\d+)', l) for l in rig.lines('mouse')[n0:]]
    pts = [(int(m.group(1)), int(m.group(2))) for m in ms if m]
    if len(pts) < steps * 0.7:
        rig.fail(tag, f'stroke: {len(pts)} touch lines for {steps} steps')
    elif pts[-1][0] < 200 or pts[0][0] > 50:
        rig.fail(tag, f'stroke: from {pts[0]} to {pts[-1]}')
    else:
        # continuous: x never goes back
        if any(pts[i + 1][0] < pts[i][0] for i in range(len(pts) - 1)):
            rig.fail(tag, f'stroke goes backwards: {pts}')
        if any(pts[i + 1][0] - pts[i][0] > 60 for i in range(len(pts) - 1)):
            rig.fail(tag, f'stroke jumps: {pts}')
    # a press outside, dragged in: no touch
    n0 = len(rig.lines('mouse'))
    out = (L - 3, cy_of(T, B)) if L - 3 >= 0 else None
    if out is None or covered(st, out[0], out[1]):
        out = (R_ + 3, cy_of(T, B)) if R_ + 3 < st['W'] and not covered(st, R_ + 3, cy_of(T, B)) else None
    if out is None and B + 3 < st['H']:
        out = ((L + R_) // 2, B + 3)
    if out is None and T - 3 > st['vis'] + 3:
        out = ((L + R_) // 2, T - 3)
    if out is not None:
        rig.mouse(*out); time.sleep(0.1); rig.down(); time.sleep(0.1)
        for i in range(1, 9):
            rig.mouse(out[0] + ((L + R_) // 2 - out[0]) * i // 8, out[1] + (cy_of(T, B) - out[1]) * i // 8)
            time.sleep(0.07)
        time.sleep(0.1)
        got = rig.lines('mouse')[n0:]
        rig.up(); time.sleep(0.1)
        rig.npts += 1
        if got:
            rig.fail(tag, f'a press outside the screen dragged in touched: {got[-1]}')
        # a stroke that leaves the screen and comes back
        n0 = len(rig.lines('mouse'))
        cxp, cyp = (L + R_) // 2, cy_of(T, B)
        rig.mouse(cxp, cyp); time.sleep(0.1); rig.down(); time.sleep(0.1)
        rig.mouse(*out); time.sleep(0.15)
        mid = len(rig.lines('mouse'))
        rig.mouse(cxp + 8, cyp); time.sleep(0.15)
        rig.up(); time.sleep(0.1)
        rig.npts += 1
        ls = rig.lines('mouse')[n0:]
        if not ls:
            rig.fail(tag, 'stroke out and back: no touch at all')
        elif len(rig.lines('mouse')) == mid:
            rig.res.append(f'  note {tag}: a stroke that leaves the screen does not resume on coming back')


def cy_of(T, B):
    return (T + B) // 2


def test_ui(rig, tag, st):
    """clicks on the bar, the side, the settings windows never touch; a click that closes a menu does not touch"""
    x0, y0, x1, y1 = st['touch']
    L, T, R_, B = pixel_tl(x0), pixel_tl(y0), pixel_last(x1), pixel_last(y1)
    cx = (L + R_) // 2
    # the bar: bring it out (mouse at the top edge), click it where the touch screen would be
    rig.mouse(cx, 0); time.sleep(0.9)
    st2 = rig.state()
    if st2['vis'] > 0:
        by = st2['vis'] // 2
        rig.npts += 1
        ls = rig.click(cx, by)
        if ls:
            rig.fail(tag, f'a click on the bar at ({cx},{by}) touched {ls[-1]}')
        rig.key('Escape'); time.sleep(0.1)
        ls = rig.click(st2['W'] - 6, by)
        rig.npts += 1
        if ls:
            rig.fail(tag, f'a click on the bar right end touched {ls[-1]}')
        # a menu open (File), then a click on the screen centre: closes the menu, no touch; the next click touches
        rig.mouse(10, by); time.sleep(0.1)
        rig.click(10, by)
        time.sleep(0.3)
        # the lower part of the touch screen: the Game menu (Pause, Speed, Reset, Quit) is long enough to reach the middle of
        # a small window's touch screen, where this click used to hit Quit
        cyp = (max(T, st2['vis'] + 40) + B) // 2
        cyp = (cyp + B) // 2
        ls = rig.click(cx, cyp)
        rig.npts += 1
        if ls:
            rig.fail(tag, f'the click that closes a menu touched {ls[-1]}')
        rig.key('Escape'); time.sleep(0.2)
    else:
        rig.fail(tag, f'the bar did not come out at the top edge (vis {st2["vis"]})')
    rig.mouse(cx, (T + B) // 2); time.sleep(0.5)
    st3 = rig.state()
    if st3['reserve'] == 0 and st3['vis'] != 0:
        rig.fail(tag, f'the bar stayed out with the mouse away (vis {st3["vis"]})')
    # the side
    sx, sy, sw, sh_ = st3['side']
    if sw > 0:
        for (px, py) in [(sx + sw / 2, sy + sh_ * 0.2), (sx + sw / 2, sy + sh_ * 0.8)]:
            ls = rig.click(int(px), int(py)); rig.npts += 1
            if ls:
                rig.fail(tag, f'a click on the side panel ({int(px)},{int(py)}) touched {ls[-1]}')


def test_windows(rig, tag):
    st = rig.state()
    wins = st['wins']
    if not wins:
        rig.fail(tag, 'no settings window in the log')
        return
    x0, y0, x1, y1 = st['touch']
    for (wx, wy, ww, wh) in wins:
        # title bar centre and the inside of the left edge: never a touch (when they overlap the touch screen: 'overlap')
        pts = [(wx + ww / 2, wy + 8, 'title'), (wx + 4, wy + wh * 0.6, 'left edge'), (wx + ww - 4, wy + wh * 0.6, 'right edge')]
        for (px, py, nm) in pts:
            px, py = int(px), int(py)
            if 0 <= px < st['W'] and 0 <= py < st['H']:
                ov = x0 <= px + .5 < x1 and y0 <= py + .5 < y1
                ls = rig.click(px, py); rig.npts += 1
                if ls:
                    rig.fail(tag, f'a click on a window {nm} ({px},{py}) touched {ls[-1]} (overlap {ov})')
        # drag the window by its title bar over the touch screen: never a touch
        n0 = len(rig.lines('mouse'))
        tx_, ty_ = int(wx + ww / 2), int(wy + 8)
        rig.mouse(tx_, ty_); time.sleep(0.1); rig.down(); time.sleep(0.1)
        for i in range(1, 9):
            rig.mouse(tx_ + (int((x0 + x1) / 2) - tx_) * i // 8, ty_ + (int(y1 - 20) - ty_) * i // 8)
            time.sleep(0.07)
        rig.up(); time.sleep(0.2)
        rig.npts += 1
        got = rig.lines('mouse')[n0:]
        if got:
            rig.fail(tag, f'dragging a window over the touch screen touched: {got[-1]}')
        break
    # after the drag the corners still work (where not covered)
    st = rig.state()
    x0, y0, x1, y1 = st['touch']
    L, T, R_, B = pixel_tl(x0), pixel_tl(y0), pixel_last(x1), pixel_last(y1)
    for (px, py, nm) in [(L, T, 'tl'), (R_, B, 'br'), (R_, T, 'tr'), (L, B, 'bl')]:
        check_click(rig, tag, st, px, py, 'afterdrag-' + nm)


def model_check(rig, tag, c, st):
    """tools/touch_layout.py (the layout model the fast tests run) against the exe's own touch rectangle"""
    m = touch_layout.touch_rect(c['layout'], c['swap'], c['int'], st['W'], st['H'], st['reserve'], c['side'] != 'off')
    if any(abs(a - b) > 0.011 for a, b in zip(m, st['touch'])):
        rig.fail(tag, f'the layout model says the touch screen is {tuple(round(v, 2) for v in m)}, the exe {st["touch"]}')


def run_config(args):
    c, idx = args
    rig = Rig(c, idx)
    name = c['name']
    try:
        ok = rig.start()
        if not ok:
            # the start (Xvfb + exe + first layout line) failed: not a test result. Once more on a fresh rig; a second
            # failure is reported with the reason (exe exited / alive, log tail)
            first = rig.why()
            rig.stop()
            time.sleep(2)
            rig = Rig(c, idx)
            ok = rig.start()
            if not ok:
                return name, c, [f'  FAIL {name}: no window twice (first: {first}; then: {rig.why()})'], 1, 0
        if False:
            return name, c, [f'  FAIL {name}: no window ({rig.why()})'], 1, 0
        time.sleep(1.0)
        sizes = c.get('sizes', [None])
        for sz in sizes:
            if sz is not None:
                sh(['xdotool', 'windowsize', rig.win, str(sz[0]), str(sz[1])], env=rig.env)
                time.sleep(1.0)
            st = rig.state()
            tag = f'{name} {st["W"]}x{st["H"]}'
            if c['fs'] and not st['fs']:
                rig.fail(tag, 'fullscreen did not take')
            if c['int']:
                vw = st['view'][2]
                side_on = c['side'] != 'off'
                x0, y0, x1, y1 = st['touch']
                tw = x1 - x0
                if tw >= 256 and abs(tw / 256 - round(tw / 256)) > 0.01 and c['layout'] == 'vertical':
                    rig.fail(tag, f'integer scale: touch screen {tw:.2f} px wide is not a whole multiple of 256')
            model_check(rig, tag, c, st)
            test_state(rig, tag)
            if not c.get('open'):
                test_stroke(rig, tag, rig.state())
                test_ui(rig, tag, rig.state())
            else:
                test_windows(rig, tag)
            if rig.fails and c.get('shots', True):
                rig.shot(re.sub(r'\W+', '_', tag))
    except Exception as e:
        import traceback
        rig.fails += 1
        rig.res.append(f'  EXC {name}: {traceback.format_exc()}')
    finally:
        rig.stop()
    return name, c, rig.res, rig.fails, rig.npts


SMOKE = ['vertical_ns_win_shown_fit_both', 'vertical_sw_win_shown_int_off', 'vertical_ns_fs_always_fit_both_wide',
         'horizontal_ns_win_shown_fit_both', 'horizontal_sw_fs_auto_fit_off', 'horizontal_ns_win_off_int_both',
         'large_ns_win_shown_int_both', 'large_sw_win_shown_fit_both', 'large_ns_fs_off_fit_off',
         'vertical_ns_win_shown_fit_both_winopen', 'large_sw_win_shown_fit_both_winopen']


def configs(only):
    cs = []
    winsizes = [None, (700, 900), (1100, 480), (420, 330)]
    for layout in LAYOUTS:
        for swap in (False, True):
            for fs in (False, True):
                bars = ['shown', 'off'] if not fs else ['always', 'auto', 'off']
                for bar in bars:
                    for integ in (False, True):
                        for side in ('both', 'off'):
                            nm = f'{layout}_{"sw" if swap else "ns"}_{"fs" if fs else "win"}_{bar}_{"int" if integ else "fit"}_{side}'
                            c = dict(name=nm, layout=layout, swap=swap, fs=fs, bar='shown' if bar in ('shown', 'auto') else bar,
                                     int=integ, side=side)
                            if fs:
                                c['bar'] = 'always' if bar == 'always' else ('shown' if bar == 'auto' else 'off')
                                c['sizes'] = [None]
                                c['screen'] = (1280, 1024)
                            else:
                                c['sizes'] = winsizes
                            cs.append(c)
                            if fs:
                                # a second, wide screen
                                c2 = dict(c, name=nm + '_wide', screen=(1700, 720))
                                cs.append(c2)
                            # a settings window open
                            if side == 'both' and not integ:
                                cs.append(dict(c, name=nm + '_winopen', open='view', sizes=[None] if fs else [None, (700, 900)]))
    if only == 'smoke':
        cs = [c for c in cs if c['name'] in SMOKE]
        assert len(cs) == len(SMOKE), sorted(set(SMOKE) - {c['name'] for c in cs})
    elif only:
        cs = [c for c in cs if any(o in c['name'] for o in only)]
    return cs


def main():
    only = None
    jobs = min(int(os.environ.get('BW_JOBS', '8')), 12)
    args = sys.argv[1:]
    while args:
        a = args.pop(0)
        if a == '--only':
            only = args.pop(0).split(',')
        elif a == '--smoke':
            only = 'smoke'
        elif a == '--jobs':
            jobs = int(args.pop(0))
    cs = configs(only)
    print(f'{len(cs)} configs, {jobs} jobs', flush=True)
    ctx = multiprocessing.get_context('fork')
    results = []
    with ctx.Pool(jobs) as pool:
        # the pool's workers need a stable index each: use the process identity
        work = [(c, i % jobs + 1) for i, c in enumerate(cs)]
        # run in waves so that two configs never share a rig index
        for w0 in range(0, len(work), jobs):
            wave = work[w0:w0 + jobs]
            wave = [(c, k + 1) for k, (c, _) in enumerate(wave)]
            for r in pool.imap_unordered(run_config, wave):
                results.append(r)
                name, c, res, fails, npts = r
                print(('FAIL ' if fails else 'ok   ') + f'{name} ({npts} clicks)', flush=True)
                for l in res:
                    print(l, flush=True)
    json.dump([(n, c, res, f, p) for (n, c, res, f, p) in results], open(OUT + '/results.json', 'w'))
    # the table
    tab = {}
    for (n, c, res, f, p) in results:
        mode = ('fs-' + c['bar'] if c['fs'] else 'win-' + c['bar']) + ('-open' if c.get('open') else '')
        key = (c['layout'] + ('-sw' if c['swap'] else ''), mode)
        t = tab.setdefault(key, [0, 0])
        t[0] += 1
        t[1] += 1 if f else 0
    modes = sorted({k[1] for k in tab})
    rows = sorted({k[0] for k in tab})
    print('\ntable (configs failed / run):')
    print('%-14s' % 'layout' + ''.join('%-16s' % m for m in modes))
    for r in rows:
        print('%-14s' % r + ''.join('%-16s' % ('%d/%d' % (tab[(r, m)][1], tab[(r, m)][0]) if (r, m) in tab else '-') for m in modes))
    print('TOTAL FAILS', sum(1 for r in results if r[3]))


if __name__=="__main__": main()
