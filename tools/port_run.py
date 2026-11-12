#!/usr/bin/env python3
"""Build and run the PC exe (Linux) headless and say how far it got (docs/port-build.md "Running").

    python3 tools/port_run.py                         # build, run 30 s, a screenshot every 5 s, the verdict
    python3 tools/port_run.py -t 120 --shots 2 --keys 'start@f600 a@15 a*0.5@17 down@f1500'

One run: `ninja -C build/port/linux bwport` (first meson setup when there is no build dir, `meson subprojects update
--reset` + --reconfigure when a wrap or packagefiles patch changed, --reconfigure when port/src or port/exclude.txt
did), an Xvfb display of its own, the exe under gdb in the run dir (default build/port/run, kept between runs: the
extracted ROM, save.bin, sim_config.ini) with BWPORT_ROM and SDL_AUDIODRIVER=dummy, for -t seconds or until it
stops (at the end gdb stops it with SIGINT). Into --out (default <run dir>/out, emptied first): log.txt (the exe's
output, the keys sent, gdb's stop, `bt` and every thread's `bt`), shots/<seconds>s_f<frame>.png (the window: both
DS screens as libntr draws them), final.png (the window when it stopped), verdict.txt. The last line printed is the
verdict:

    reached-frame N in T s (F fps)   still running at the end, frames advancing
    hang in F (file:line) ...        running, but no frame for --hang seconds: the main thread's innermost frame
                                     with source
    stub X called from F, frame N    an undefined function's abort stub (build/port/undefined.txt)
    crash SIGSEGV in F <- G <- H     a signal: the innermost three frames with source (game, port/src, libntr)
    exit N at frame M                the exe ended by itself
    build failed                     ninja's or meson's error (the log's tail is printed)

A hang's and a crash's place end with `... G (file:line)`, the innermost game frame (src/, lib/), when it isn't one
of those (whose call it was).

The frame is the V-blank count (port/src/run_frames.c gives its address; read through /proc/<pid>/mem).
--keys presses DS buttons with xdotool: KEY[*HOLD]@WHEN, WHEN in seconds after the start or fN for frame N (better:
it doesn't depend on how fast the start was); KEY a b x y l r start select up down left right, mapped through the
run dir's sim_config.ini [PAD] (main_start.c's defaults: A X, B Z, X S, Y A, L Q, R W, Start Enter, Select right
Shift). HOLD in seconds (default 0.1: the game reads the keys once a frame).
--skip N draws one frame in N (frame skip: tools/port_run.py --fast --skip 4 is the quickest way to run the game's logic;
screen checks need the default).
--save NAME starts from /root/saves/NAME.bin (the in-game saves library, /root/saves/README); --store NAME keeps the run's
save.bin there afterwards. Both use the save where the running game reads it: <run dir>/<game folder>/save.bin once the
launcher made that folder ("Pokemon Black"), else <run dir>/save.bin; a run that loaded another save is an error. A run
from --save skips the title and menu (BWPORT_SKIPMENU=1, as tools/mac_run.sh: the field is up at ~f220, key frames are
counted from boot; BWPORT_SKIPMENU=0 or BWPORT_CONTINUE_KEYS="start@f600 start@f900 start@f1300 a@f1700" press the title,
as does any key script of yours with a `start` key: old scripts keep working).
UNCAPPED by default (FrameRate=unlimited, keys held 0.03 s; --realtime: 60 fps, for timing and battle speed).
--play NAME runs tools/plays/*/NAME.txt (GAME/NAME when both games have one; tools/play_script.py: header, keys, walk lines) at maximum speed: the save the
script starts from, uncapped, cfgscript ff=1, frame skip 4 (--skip N; 1 = none), the exe presses the keys itself and ends
the run 30 frames after the last and leaves the last picture as final.png (-t is only the cap). Functional runs only: a timing run uses --realtime.
 --fresh starts without save.bin, sim_config.ini, imgui.ini (the extracted ROM is kept). Exit code 0 when the exe was
still running at the end (or exited 0), else 1.
"""
import argparse
import glob
import os
import re
import shutil
import signal
import struct
import subprocess
import sys
import time
import zlib
import hashlib
import port_wraps
import play_script

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD = os.path.join(REPO, os.environ.get('BWPORT_BUILD', 'build/port/linux'))
EXE = os.path.join(BUILD, 'bwport')
TRACE = os.environ.get('ROUTE_TRACE') == '1'


def trace(what):
    """ROUTE_TRACE=1: a phase's monotonic time on stdout (tools/route.py prints a leg's phases)"""
    if TRACE:
        print(f'trace: {time.monotonic():.3f} {what}', flush=True)

# sim_config.ini's [PAD] names for the DS buttons, and SDL keycodes -> X keysyms (those not their own character)
PAD_NAMES = {'a': 'AKey', 'b': 'BKey', 'x': 'XKey', 'y': 'YKey', 'l': 'LKey', 'r': 'RKey', 'start': 'StartKey',
             'select': 'SelectKey', 'up': 'UpKey', 'down': 'DownKey', 'left': 'LeftKey', 'right': 'RightKey'}
PAD_DEFAULTS = {'AKey': 'x', 'BKey': 'z', 'XKey': 's', 'YKey': 'a', 'LKey': 'q', 'RKey': 'w', 'StartKey': 'Return',
                'SelectKey': 'Shift_R', 'UpKey': 'Up', 'DownKey': 'Down', 'LeftKey': 'Left', 'RightKey': 'Right'}
SDL_KEYSYMS = {8: 'BackSpace', 9: 'Tab', 13: 'Return', 27: 'Escape', 32: 'space', 96: 'grave',
               0x4000004F: 'Right', 0x40000050: 'Left', 0x40000051: 'Down', 0x40000052: 'Up',
               0x400000E0: 'Control_L', 0x400000E1: 'Shift_L', 0x400000E2: 'Alt_L',
               0x400000E4: 'Control_R', 0x400000E5: 'Shift_R', 0x400000E6: 'Alt_R'}


def sh(cmd, **kw):
    return subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, **kw)


SLICE = shutil.which('systemd-run') and os.path.isdir('/run/systemd/system')


def slice_cmd(cmd):
    """cmd in the worker slice (CPU work), when systemd runs"""
    return ['systemd-run', '--scope', '-q', '--slice=bwworkers.slice'] + cmd if SLICE else cmd


def setup_args():
    """meson setup's native files: linux.ini, and linux-ccache.ini on top when ccache is installed (only a NEW build dir
    takes them: /etc/ccache.conf's base_dir=/root/bw-port lets every worktree share the objects)"""
    if os.environ.get('BWPORT_NATIVE'):
        return ['--native-file', os.environ['BWPORT_NATIVE']]
    args = ['--native-file', 'port/machine/linux.ini']
    if shutil.which('ccache'):
        args += ['--native-file', 'port/machine/linux-ccache.ini']
    return args


def build():
    """ninja the exe after what meson doesn't see by itself; the error output, or None"""
    err = port_wraps.refresh(BUILD, setup_args())
    if err:
        return err
    r = sh(slice_cmd(['ninja', '-C', BUILD, 'bwport']), cwd=REPO)
    return r.stdout if r.returncode != 0 else None


def find_rom():
    if os.environ.get('BWPORT_ROM'):
        return os.environ['BWPORT_ROM']
    common = sh(['git', 'rev-parse', '--path-format=absolute', '--git-common-dir'], cwd=REPO).stdout.strip()
    for root in (REPO, os.path.dirname(common)):
        path = os.path.join(root, 'orig/baserom_b_us.nds')
        if os.path.exists(path):
            return path
    sys.exit('port_run: no ROM: set BWPORT_ROM')


def parse_keys(script, hold=0.1):
    """'start@f600 a*0.5@12' -> [(seconds or None, frame or None, hold, sim_config.ini's name)]"""
    events = []
    for item in re.split(r'[\s,]+', script.strip()) if script.strip() else []:
        m = re.fullmatch(r'(\w+)(?:\*([\d.]+))?@(f?)([\d.]+)', item)
        if not m or (m.group(1).lower() not in PAD_NAMES and not m.group(1).startswith('x_')):
            sys.exit(f'port_run: bad key {item!r} (KEY[*HOLD]@SECONDS or KEY[*HOLD]@fFRAME; x_NAME is the X key NAME, x_F1; keys {" ".join(PAD_NAMES)})')
        when = float(m.group(4))
        events.append((None if m.group(3) else when, int(when) if m.group(3) else None, float(m.group(2) or hold),
                       PAD_NAMES.get(m.group(1).lower()) or '=' + m.group(1)[2:]))
    return events


def read_pad(run_dir):
    pad = dict(PAD_DEFAULTS)
    try:
        text = open(os.path.join(run_dir, 'sim_config.ini')).read()
    except OSError:
        return pad
    section = re.search(r'^\[PAD\]\n(.*?)(?=^\[|\Z)', text, re.M | re.S)
    for name, code in re.findall(r'^(\w+)=(\d+)', section.group(1) if section else '', re.M):
        code = int(code)
        if name in pad and (code in SDL_KEYSYMS or 33 <= code < 127):
            pad[name] = SDL_KEYSYMS[code] if code in SDL_KEYSYMS else chr(code)
    return pad


def xwd_to_png(xwd, png):
    """xwd's ZPixmap (32 bits a pixel, TrueColor: Xvfb's depth 24) as an RGB PNG"""
    data = open(xwd, 'rb').read()
    if len(data) < 100:
        return False                           # the window was gone
    f = struct.unpack('>25I', data[:100])
    hsize, w, h, order, bpp, stride, ncolors = f[0], f[4], f[5], f[7], f[11], f[12], f[19]
    pix = data[hsize + 12 * ncolors:][:stride * h]
    if bpp != 32 or len(pix) < stride * h:
        return False
    r, g, b = (2, 1, 0) if order == 0 else (1, 2, 3)
    rows = []
    for y in range(h):
        row = pix[y * stride:y * stride + 4 * w]
        rgb = bytearray(3 * w)
        rgb[0::3], rgb[1::3], rgb[2::3] = row[r::4], row[g::4], row[b::4]
        rows.append(b'\0' + bytes(rgb))

    def chunk(kind, body):
        return struct.pack('>I', len(body)) + kind + body + struct.pack('>I', zlib.crc32(kind + body))
    with open(png, 'wb') as out:
        out.write(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0)) +
                  chunk(b'IDAT', zlib.compress(b''.join(rows), 6)) + chunk(b'IEND', b''))
    return True


class Exe:
    """the running exe: found among gdb's descendants; its V-blank count read through /proc/<pid>/mem"""

    def __init__(self, root_pid):
        self.root_pid, self.pid, self.count_addr = root_pid, None, None
        out = sh(['nm', '--defined-only', EXE]).stdout
        m = re.search(r'^([0-9a-f]+) \w bwport_vblank_count$', out, re.M)
        self.sym = int(m.group(1), 16) if m else None
        self.exe = os.path.realpath(EXE)

    def find(self):
        todo = [self.root_pid]
        while todo and self.pid is None:
            pid = todo.pop()
            try:
                if os.readlink(f'/proc/{pid}/exe') == self.exe:
                    self.pid = pid
                for task in os.listdir(f'/proc/{pid}/task'):
                    todo += [int(c) for c in open(f'/proc/{pid}/task/{task}/children').read().split()]
            except OSError:
                pass
        return self.pid

    def frame(self):
        if self.pid is None or self.sym is None:
            return None
        try:
            with open(f'/proc/{self.pid}/mem', 'rb') as mem:
                if self.count_addr is None:
                    for line in open(f'/proc/{self.pid}/maps'):
                        parts = line.split()
                        if len(parts) >= 6 and parts[5] == self.exe and int(parts[2], 16) == 0:
                            mem.seek(int(parts[0].split('-')[0], 16) + self.sym)
                            self.count_addr = struct.unpack('<Q', mem.read(8))[0]
                            break
                if not self.count_addr:
                    self.count_addr = None
                    return None
                mem.seek(self.count_addr)
                return struct.unpack('<I', mem.read(4))[0]
        except (OSError, struct.error, ValueError):
            self.count_addr = None             # read before the loader relocated the pointer: read it again
            return None


def x_listening(n):
    """display n's abstract socket listens (Xvfb's only one here). A client never connects to a display before that:
    with no socket libX11 falls back to TCP, and WSL's localhost drops the SYN instead of refusing it, so xdotool
    hangs for the kernel's ~130 s of SYN retries (tools/route.py legs that took 140 s instead of 6)"""
    try:
        with open('/proc/net/unix') as f:
            return any(line.rstrip().endswith(f' @/tmp/.X11-unix/X{n}') for line in f)
    except OSError:
        return False


def start_xvfb():
    """Xvfb on the first free display from :300 (-displayfd can't be used: WSL's /tmp/.X11-unix makes every display's
    socket fail, and it would then take :0's abstract socket, WSLg's)"""
    for n in range(300, 400):
        if os.path.exists(f'/tmp/.X{n}-lock'):
            continue
        xvfb = subprocess.Popen(['Xvfb', f':{n}', '-screen', '0', '1280x1024x24', '-nolisten', 'tcp'],
                                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        env = dict(os.environ, DISPLAY=f':{n}', SDL_VIDEODRIVER='x11', WAYLAND_DISPLAY='')
        for _ in range(200):
            if xvfb.poll() is not None:
                break
            if x_listening(n) and sh(['xdotool', 'getdisplaygeometry'], env=env).returncode == 0:
                try:                           # ours, not another run's that took the number first
                    if int(open(f'/tmp/.X{n}-lock').read().strip()) == xvfb.pid:
                        return xvfb, f':{n}'
                except (OSError, ValueError):
                    pass
                break
            time.sleep(0.05)
        xvfb.kill()
    sys.exit('port_run: Xvfb did not start')


def find_window(env):
    out = sh(['xdotool', 'search', '--onlyvisible', '--name', '.'], env=env).stdout.split()
    best = None
    for win in out:
        geo = sh(['xdotool', 'getwindowgeometry', win], env=env).stdout
        m = re.search(r'Geometry: (\d+)x(\d+)', geo)
        if m and int(m.group(1)) >= 64 and (best is None or int(m.group(1)) * int(m.group(2)) > best[1]):
            best = (win, int(m.group(1)) * int(m.group(2)))
    return best[0] if best else None


FRAME_RE = re.compile(r'^#(\d+)\s+(?:0x[0-9a-f]+ in )?(\S+) \(.*?\)(?: at (\S+):(\d+))?(?: from (\S+))?$')


def frames_of(text):
    """gdb bt lines -> [(function, file, line, ours)]: ours when the file is the repo's (game, port/src, libntr)"""
    out = []
    for line in text.splitlines():
        m = FRAME_RE.match(line.strip())
        if m:
            path, mine = m.group(3), False
            if path:
                full = os.path.normpath(os.path.join(BUILD, path))
                if full.startswith(REPO + '/') and os.path.exists(full):
                    path, mine = os.path.relpath(full, REPO), True
            out.append((m.group(2), path, m.group(4), mine))
    return out


def ours(fr):
    return fr[3]


def where(fr):
    return f'{fr[0]} ({os.path.basename(fr[1])}:{fr[2]})' if fr[1] else fr[0]


def place(bt, n):
    """the innermost n frames with source, and then the innermost game frame (src/, lib/) when it isn't one of them:
    whose call it was when the place is in libntr or port/src"""
    mine = [fr for fr in bt if ours(fr)]
    game = [fr for fr in mine if fr[1].startswith(('src/', 'lib/'))][:1]
    text = ' <- '.join(where(fr) for fr in mine[:n] or bt[:n]) or '?'
    return text + (f' ... {where(game[0])}' if game and game[0] not in mine[:n] else '')


def verdict(log, timed_out, frame, stall, hang_secs, fps, last_t):
    stop = log.split('\n== port_run: stop\n', 1)
    gdb_text = stop[1] if len(stop) > 1 else ''
    bt = frames_of(gdb_text.split('== port_run: threads')[0])
    at = f'frame {frame}' if frame is not None else 'no frame count'
    stub = re.search(r'bwport: undefined function (\S+) called', log)
    sig = re.search(r'(?:Program|Thread \d+ "[^"]*") received signal (\w+)|Program terminated with signal (\w+)',
                    stop[0])
    exited = re.search(r'\[Inferior \d+ \(process \d+\) exited (?:normally|with code (\d+))\]', stop[0])
    if stub and not timed_out:
        callers = [fr for i, fr in enumerate(bt) if i > 0 and bt[i - 1][0] == stub.group(1) and ours(fr)]
        return f'stub {stub.group(1)} called' + (f' from {where(callers[0])}' if callers else '') + f', {at}'
    if exited:
        return f'exit {int(exited.group(1) or 0)} at {at}'
    if sig and not timed_out:
        return f'crash {sig.group(1) or sig.group(2)} in {place(bt, 3)}, {at}'
    if not timed_out:
        return f'stopped ({(sig.group(1) or sig.group(2)) if sig else "?"}), {at}'
    if stall >= hang_secs or not frame:
        main = re.search(r'^Thread 1 .*?\n(.*?)(?=^Thread |\Z)', gdb_text, re.M | re.S)
        where_ = place(frames_of(main.group(1) if main else ''), 1)
        return f'hang in {where_}, {at} (no frame for {stall:.0f} s)' if frame else             f'hang in {where_} before the first frame'
    return f'reached-frame {frame} in {last_t:.0f} s ({fps:.0f} fps' + (f'; none for the last {stall:.0f} s)' if
                                                                         stall >= 1 else ')')


SAVES = '/root/saves'   # the in-game saves library: --save NAME, --store NAME
PLAYS = os.path.join(REPO, 'tools/plays')   # the play scripts: --play NAME


def load_play(name):
    # GAME/NAME picks one game's script (black/boot-room and white/boot-room share a name)
    found = glob.glob(os.path.join(PLAYS, name + '.txt') if '/' in name else os.path.join(PLAYS, '*', name + '.txt'))
    if len(found) != 1:
        have = ' '.join(sorted(os.path.basename(p)[:-4] for p in glob.glob(os.path.join(PLAYS, '*', '*.txt'))))
        sys.exit(f'port_run: --play {name}: {"no" if not found else "several"} script in {PLAYS}/*/ ({have})')
    try:
        return play_script.parse(open(found[0]).read(), found[0])
    except ValueError as e:
        sys.exit(f'port_run: {e}')


def game_of(save_name):
    """the game a library save belongs to, by the README's naming: b2-*, w2-*, white-*, else Black"""
    name = os.path.basename(save_name)
    return 'b2' if name.startswith('b2-') else 'w2' if name.startswith('w2-') else 'white' if name.startswith('white-') else 'black'


def save_paths(run_dir, game):
    """where a save can be: the game's launcher folder, or the run dir's root (a flat layout the launcher migrates)"""
    return [os.path.join(run_dir, play_script.GAMES[game], 'save.bin'), os.path.join(run_dir, 'save.bin')]


def save_path(run_dir, game):
    """where the running game reads its save: the game's folder once it exists (a save in the root is ignored then)"""
    folder, root = save_paths(run_dir, game)
    return folder if os.path.isdir(os.path.dirname(folder)) else root


ROMS = {'white': 'baserom_w_us.nds', 'b2': 'baserom_b2_us.nds', 'w2': 'baserom_w2_us.nds'}
CODES = {'black': 'IRBO', 'white': 'IRAO', 'b2': 'IREO', 'w2': 'IRDO'}   # BWPORT_GAME: the game the exe starts


def import_game(run_dir, game):
    """a launcher root's game folder (the launcher's own importrom step, no window): the ROM is orig/baserom_*_us.nds"""
    rom = os.path.join(os.path.dirname(find_rom()), ROMS[game])
    if not os.path.exists(rom):
        sys.exit(f'port_run: no {rom}')
    env = dict(os.environ, BWPORT_LCTEST=f'importrom={rom}', SDL_VIDEODRIVER='dummy', SDL_AUDIODRIVER='dummy')
    r = sh([EXE], cwd=run_dir, env=env, timeout=300)
    if 'lctest: 0 failed' not in r.stdout:
        sys.exit(f'port_run: {game} import failed:' + chr(10) + chr(10).join(r.stdout.splitlines()[-8:]))


def sha1(path):
    return hashlib.sha1(open(path, 'rb').read()).hexdigest()


def check_save(log, run_dir, game, want_sha):
    """None, or what is wrong: the game ran from another save than --save's. The launcher logs the folder the game runs in
    ('launcher: Pokemon Black, in DIR') and its start's backup of the save it found ('launcher: backup DIR/backups/save-T.bin')"""
    m = re.search(r'bwport: launcher: (Pokemon [\w ]+?), in (.+)', log)
    if not m:
        return 'the exe never said which game folder it runs in (no "launcher: ..., in DIR" line)'
    folder = os.path.join(run_dir, play_script.GAMES[game])
    if os.path.realpath(m.group(2).strip()) != os.path.realpath(folder):
        return f'the exe ran in {m.group(2).strip()}, the save went to {folder}'
    b = re.search(r'bwport: launcher: backup (.+\.bin)', log)
    if b and os.path.exists(b.group(1)) and sha1(b.group(1)) != want_sha:
        return f'the exe loaded a different save ({b.group(1)} is not the --save one)'
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('-t', '--time', type=float, default=30, help='seconds to run (default 30)')
    ap.add_argument('--shots', type=float, default=5, help='a screenshot every SHOTS seconds (default 5; 0: none)')
    ap.add_argument('--mouse', default='', help="mouse clicks: 'X,Y@SECONDS[*HOLD] ...', X,Y in window pixels "
                    "(the touch screen is where the mouse is: a touch on the lower screen)")
    ap.add_argument('--keys', default='', help="key script: 'start@f600 a@12 a*0.5@14' (KEY[*HOLD]@SECONDS|fFRAME)")
    ap.add_argument('--dir', default=os.path.join(REPO, 'build/port/run'), help='run dir (default build/port/run)')
    ap.add_argument('--out', help='output dir (default <run dir>/out, emptied first)')
    ap.add_argument('--fresh', action='store_true',
                    help='remove save.bin, sim_config.ini (kept with --fast), imgui.ini first')
    ap.add_argument('--save', metavar='NAME',
                    help='start from the saves library: /root/saves/NAME.bin (or a path) copied to save.bin first '
                         '(/root/saves/README lists them)')
    ap.add_argument('--store', metavar='NAME',
                    help="after the run, copy save.bin to /root/saves/NAME.bin (a milestone others can start from; "
                         "add a line to /root/saves/README)")
    ap.add_argument('--skip', type=int, metavar='N', default=0,
                    help="frame skip: the game runs every frame, the picture is drawn one frame in N (BWPORT_FRAMESKIP=N; "
                         "capture frames always; a screenshot or the end of the run draws the frames before it). Faster "
                         "with --fast; screen checks need N=1")
    ap.add_argument('--realtime', action='store_true', help='the 60 fps cap (default: uncapped; timing runs, battle speed)')
    ap.add_argument('--play', metavar='NAME',
                    help='run the play script tools/plays/<game>/NAME.txt at maximum speed (tools/play_script.py); '
                         '--store NAME keeps the save at its end')
    ap.add_argument('--game', help="black|white|b2|w2 (default: the play script's, else the save name's: white-*)")
    ap.add_argument('--hang', type=float, default=5, help='no frame for this many seconds at the end: a hang')
    ap.add_argument('--hang-stop', action='store_true',
                    help='end the run as soon as no frame came for --hang seconds (verdict: hang in F), not at -t')
    ap.add_argument('--cheat', action=argparse.BooleanOptionalAction, default=None,
                    help='BWPORT_TESTCHEAT=win: the player moves first, one-shots, takes no damage (docs/port-runtime.md); '
                         'on by default with --play, --no-cheat turns it off')
    ap.add_argument('--no-build', action='store_true', help="run the exe as it is (no ninja)")
    ap.add_argument('--fast', action='store_true', help="the default now (uncapped); kept for old command lines")
    ap.add_argument('--gdb', action='append', default=[], metavar='CMD',
                    help="a gdb command before `run` (repeatable): 'break exit', 'source FILE' (dprintf, breakpoints"
                         " with commands ... continue); its output goes to log.txt")
    args = ap.parse_args()
    cheat_asked = args.cheat
    if args.cheat is None:
        args.cheat = bool(args.play)   # play scripts run with the cheat by default
    if args.cheat:
        os.environ['BWPORT_TESTCHEAT'] = 'win'
    elif cheat_asked is False:
        os.environ.pop('BWPORT_TESTCHEAT', None)
    args.fast = not args.realtime
    play = None
    if args.play:
        play = load_play(args.play)
        args.game = args.game or play['game']
        if args.save is None and play['from'] != 'boot':
            args.save = play['from']
        if play['from'] == 'boot':
            args.fresh = True
        args.skip = args.skip or 4             # functional runs: logic every frame, the picture 1 in 4
        if args.time == 30:
            args.time = 900
        if args.shots == 5:
            args.shots = 0
        os.environ['BWPORT_KEYS'] = ' '.join(play['keys'])
        if not os.environ.get('BWPORT_CFGSCRIPT'):
            os.environ['BWPORT_CFGSCRIPT'] = "f1:ff=1"
    keys = parse_keys(args.keys, 0.03 if args.fast else 0.1)
    clicks = []
    for item in args.mouse.split():
        m = re.fullmatch(r'(\d+),(\d+)@([\d.]+)(?:\*([\d.]+))?', item)
        if not m:
            sys.exit(f'port_run: bad mouse click {item!r} (X,Y@SECONDS[*HOLD])')
        clicks.append((float(m.group(3)), int(m.group(1)), int(m.group(2)), float(m.group(4) or 0.3)))

    if not args.no_build:
        err = build()
        if err:
            print('\n'.join(err.splitlines()[-30:]))
            print('build failed')
            return 1
    game = args.game or game_of(args.save or args.store or '')
    if game not in play_script.GAMES:
        sys.exit(f'port_run: --game {game}: one of {" ".join(play_script.GAMES)}')
    if game != 'black' and args.dir == ap.get_default('dir'):
        args.dir += '-' + game                 # a launcher root of its own: Black's flat run dir would migrate into Black's folder
    run_dir = os.path.abspath(args.dir)
    out = os.path.abspath(args.out or os.path.join(run_dir, 'out'))
    os.makedirs(run_dir, exist_ok=True)
    if game != 'black' and not os.path.isdir(os.path.join(run_dir, play_script.GAMES[game])):
        import_game(run_dir, game)
    if args.fresh:
        for name in ('imgui.ini',) + (() if args.fast else ('sim_config.ini',)):
            if os.path.exists(os.path.join(run_dir, name)):
                os.remove(os.path.join(run_dir, name))
        for path in save_paths(run_dir, game):
            if os.path.exists(path):
                os.remove(path)
    want_sha = None
    if args.save:
        src = args.save if os.path.sep in args.save else os.path.join(SAVES, args.save + '.bin')
        if not os.path.exists(src):
            sys.exit(f'port_run: no save {src} (see {SAVES}/README)')
        want_sha = sha1(src)
        dst = save_path(run_dir, game)
        for other in save_paths(run_dir, game):
            if other != dst and os.path.exists(other):
                os.remove(other)               # the save a launcher migration or an earlier run left in the other place
        shutil.copyfile(src, dst)
        if os.environ.get('BWPORT_CONTINUE_KEYS'):
            os.environ['BWPORT_SKIPMENU'] = os.environ.get('BWPORT_SKIPMENU', '0')
            args.keys = os.environ['BWPORT_CONTINUE_KEYS'] + ' ' + args.keys
            keys = parse_keys(args.keys, 0.03 if args.fast else 0.1)
        elif 'BWPORT_SKIPMENU' not in os.environ:
            # port/src/skip_menu.c: straight into CONTINUE; unless the keys press the title themselves (an old script's
            # start@f600 ...: that START in the field would open the menu)
            presses_title = re.search(r'start(?:\*[\w.]+)?@f?[\d.]+', args.keys + ' ' + os.environ.get('BWPORT_KEYS', ''))
            os.environ['BWPORT_SKIPMENU'] = '0' if presses_title else '1'
    ini = os.path.join(run_dir, 'sim_config.ini')
    if os.path.exists(ini):                    # main_start.c writes it when there is none (capped): --fast from the 2nd run
        text = open(ini).read()
        # libntr-rn80.patch: FrameRate=60|120|monitor|unlimited (the old CapFrameRate=true|false still loads in an old file)
        if re.search(r'^FrameRate=', text, flags=re.M):
            if args.fast or re.search(r'^FrameRate=unlimited', text, flags=re.M):
                new = re.sub(r'^FrameRate=\w+', 'FrameRate=unlimited' if args.fast else 'FrameRate=60', text, flags=re.M)
            else:
                new = text
        else:
            new = re.sub(r'^CapFrameRate=\w+', f'CapFrameRate={"false" if args.fast else "true"}', text, flags=re.M)
        if new != text:
            open(ini, 'w').write(new)
    shutil.rmtree(out, ignore_errors=True)
    os.makedirs(os.path.join(out, 'shots'))

    trace('port_run set up')
    xvfb, display = start_xvfb()
    trace('Xvfb up')
    # the game only ever sees the Xvfb display: SDL's default Wayland socket (wayland-0) and DISPLAY :0 are WSLg's,
    # on the desktop of whoever uses this PC
    env = dict(os.environ, DISPLAY=display, SDL_VIDEODRIVER='x11', WAYLAND_DISPLAY='', SDL_AUDIODRIVER='dummy',
               BWPORT_ROM=find_rom())
    if game != 'black':
        env.setdefault('BWPORT_GAME', CODES[game])
    if args.skip > 1:
        env['BWPORT_FRAMESKIP'] = str(args.skip)
    win_file = os.path.join(out, '.window')
    final = (f'shell [ -s {win_file} ] && xwd -silent -id $(cat {win_file}) -out {out}/final.xwd')
    gdb = ['gdb', '-q', '-nx', '-batch', '-ex', 'set pagination off', '-ex', 'set confirm off',
           '-ex', 'set print thread-events off', '-ex', 'set startup-with-shell off',
           '-ex', 'handle SIGPIPE SIGUSR1 SIGUSR2 nostop noprint pass',
           '-ex', 'set breakpoint pending on'] + [a for c in args.gdb for a in ('-ex', c)] + ['-ex', 'run',
           '-ex', 'echo \\n== port_run: stop\\n', '-ex', final, '-ex', 'bt 40',
           '-ex', 'echo \\n== port_run: threads\\n', '-ex', 'thread apply all bt 40', '-ex', 'kill', '--args', EXE]
    log_path = os.path.join(out, 'log.txt')
    log = open(log_path, 'w')
    proc = subprocess.Popen(slice_cmd(gdb), cwd=run_dir, env=env, stdin=subprocess.DEVNULL, stdout=log,
                            stderr=subprocess.STDOUT, start_new_session=True)
    trace('gdb started')
    exe = Exe(proc.pid)
    launched, t0, window, pad, next_shot = time.monotonic(), None, None, None, 0.0
    samples, timed_out, scanned = [], False, 0          # (seconds, frame); the log read up to scanned
    last_frame, frame_t = None, 0.0                     # --hang-stop: the last frame count and when it changed
    try:
        while proc.poll() is None:
            now = time.monotonic()
            if t0 is None:
                if exe.find():
                    t0 = now
                    trace('exe found')
                elif now - launched > 60:
                    break
                time.sleep(0.05)
                continue
            t = now - t0
            frame = exe.frame()
            if frame is not None:
                if TRACE and frame and not any(f for _, f in samples):
                    trace(f'first frame {frame}')
                if frame != last_frame:
                    last_frame, frame_t = frame, t
                samples.append((t, frame))
            if args.hang_stop and frame and t - frame_t >= args.hang:
                timed_out = True               # no game frame for --hang seconds: stop now (gdb's backtrace: hang in F)
                break
            with open(log_path, 'rb') as f:
                f.seek(max(0, scanned - 32))
                new = f.read()
                scanned = max(0, scanned - 32) + len(new)
                for mark in (b'bwport: field: up', b'pilot: script done', b'pilot: end token') if TRACE else ():
                    if mark in new:
                        trace(f'game log: {mark.decode()} (frame {frame})')
                if b'\n== port_run: stop' in new:
                    trace(f'gdb stop seen (frame {frame})')
                    break                      # stopped by itself: gdb has it
            if window is None:
                window = find_window(env)
                if window:
                    trace('window found')
                    open(win_file, 'w').write(window)
                    sh(['xdotool', 'windowfocus', window], env=env)
                    pad = read_pad(run_dir)    # main_start.c has written sim_config.ini by now
            for ev in list(keys if window else []):
                if (ev[0] is not None and t >= ev[0]) or (ev[1] is not None and frame is not None and frame >= ev[1]):
                    keys.remove(ev)
                    sym = ev[3][1:] if ev[3].startswith('=') else pad[ev[3]]   # x_F1: that X key itself
                    subprocess.Popen(['xdotool', 'keydown', sym, 'sleep', str(ev[2]), 'keyup', sym], env=env)
                    print(f'port_run: {t:.1f} s, frame {frame}: {ev[3]} ({sym}) for {ev[2]} s', file=log, flush=True)
            for ck in list(clicks if window else []):
                if t >= ck[0]:
                    clicks.remove(ck)
                    subprocess.Popen(['xdotool', 'mousemove', '--window', window, str(ck[1]), str(ck[2]), 'sleep', '0.2',
                                      'mousedown', '1', 'sleep', str(ck[3]), 'mouseup', '1'], env=env)
                    print(f'port_run: {t:.1f} s: click at {ck[1]},{ck[2]} for {ck[3]} s', file=log, flush=True)
            if window and args.shots and t >= next_shot:
                next_shot += args.shots
                xwd = os.path.join(out, 'shot.xwd')
                # the exe draws the next frames (SIGUSR2), then the shot; only once the game has drawn a frame: libntr
                # installs the handler at its first SIM_Render, and an unhandled SIGUSR2 ends the process
                if args.skip > 1 and exe.pid and frame:
                    os.kill(exe.pid, signal.SIGUSR2)
                    time.sleep(0.4)
                if sh(['xwd', '-silent', '-id', window, '-out', xwd], env=env).returncode == 0:
                    xwd_to_png(xwd, os.path.join(out, 'shots', f'{t:04.0f}s_f{frame}.png'))
            if t >= args.time or (play and frame is not None and frame >= play['end_frame']):
                timed_out = True               # a play script is over (the window's last picture is final.png)
                break
            time.sleep(0.1)
        if timed_out and args.skip > 1 and exe.pid and frame:   # final.png: the frames before the stop are drawn
            try:
                os.kill(exe.pid, signal.SIGUSR2)
                time.sleep(0.4)
            except OSError:
                pass
        for sig in (signal.SIGINT, signal.SIGSTOP) if timed_out else ():   # gdb stops it: the backtraces
            os.kill(exe.pid, sig)
            try:
                proc.wait(timeout=5 if sig == signal.SIGINT else 30)
                break
            except subprocess.TimeoutExpired:
                if b'\n== port_run: stop' in open(log_path, 'rb').read():
                    proc.wait(timeout=30)
                    break
        if not timed_out:
            proc.wait(timeout=30)
    except subprocess.TimeoutExpired:
        pass
    finally:
        for p in (exe.pid, None):
            try:
                os.killpg(proc.pid, signal.SIGKILL) if p is None else os.kill(p, signal.SIGKILL)
            except (OSError, TypeError):
                pass
        trace('exe and gdb gone')
        xvfb.terminate()
        log.close()
    if os.path.exists(os.path.join(out, 'final.xwd')):
        xwd_to_png(os.path.join(out, 'final.xwd'), os.path.join(out, 'final.png'))
    for name in ('final.xwd', 'shot.xwd', '.window'):
        if os.path.exists(os.path.join(out, name)):
            os.remove(os.path.join(out, name))

    trace('port_run done')
    frames = [s for s in samples if s[1] is not None]
    frame = frames[-1][1] if frames else None
    last_t = samples[-1][0] if samples else 0
    stall = last_t - min((t for t, f in frames if f == frame), default=last_t)
    moving = [s for s in frames if s[1]]
    fps = (frame - moving[0][1]) / (last_t - moving[0][0]) if len(moving) > 1 and last_t > moving[0][0] else 0
    text = open(log_path, errors='replace').read()
    line = verdict(text, timed_out, frame, stall, args.hang, fps, last_t)
    bad_save = check_save(text, run_dir, game, want_sha) if want_sha else None
    if bad_save:
        print(f'port_run: WRONG SAVE: {bad_save}')
        line = f'wrong-save: {bad_save}; ' + line
    open(os.path.join(out, 'verdict.txt'), 'w').write(line + '\n')
    nshots = len(os.listdir(os.path.join(out, 'shots')))
    print(f'port_run: {log_path}, {nshots} shots in {out}/shots' + (', final.png' if
          os.path.exists(os.path.join(out, 'final.png')) else ''))
    print(line)
    if args.store:
        os.makedirs(SAVES, exist_ok=True)
        shutil.copyfile(save_path(run_dir, game), os.path.join(SAVES, args.store + '.bin'))
        print(f'port_run: save.bin stored as {SAVES}/{args.store}.bin: add its line to {SAVES}/README')
    return 0 if line.startswith(('reached-frame', 'exit 0')) else 1


if __name__ == '__main__':
    sys.exit(main())
