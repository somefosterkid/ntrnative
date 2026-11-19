#!/usr/bin/env python3
"""What the sweep tools (pt_sweep, pt_battle_sweep, hg_sweep, hg_battle_sweep, hg_app_sweep, hg_int_sweep) do about time.

There is NO total wall-clock watchdog: a run that is merely slow (a loaded host, 17 fps) is never killed and never a finding.
A run ends by the game's own frames: the exe exits itself at its END, or port_run's `--frames CAP` ends it at V-blank CAP
(`exit-frame`, judged as before: ok / cap / exit). The one wall-clock check is port_run's stall rule, `--hang 30 --hang-stop`:
no new V-blank for 30 s (60 s is fine under gdb) = verdict `hang`, with gdb's backtrace.

Frame-based no-progress verdicts, findings with a location (unlike a slow run):
  stuck-walk    hg_sweep: the player stood on one tile for STUCK_FRAMES V-blanks of the walk (warp.txt's `pos F X Z` lines are
                written on each tile change): the verdict's top column is `MAP X,Z`;
  stuck-screen  the screen's picture was unchanged for STUCK_FRAMES V-blanks while keys were pressed, stuck_screen() on the
                records `F HASH` of a per-V-blank hash of the screens' source state (the game-side hook that writes them,
                BWPORT_SCREENLOG=FILE, is not built yet: the function and its tests are, tools/sweep_wall_tests.py)."""
import hashlib
import os
import re

STUCK_FRAMES = 1200   # V-blanks (20 game seconds) without a new tile / a new picture
END_SLACK = 6000      # V-blanks past a sweep's END the frame cap waits for a battle or a script still running at END
NO_WALL_CAP = '0'     # port_run's -t: 0 = no wall watchdog, only the --hang stall rule ends a stuck run


# WHERE a run that did not finish was (every sweep's hang / stuck rows group by it, like crashes):
#   frozen  no frame for --hang s: port_run's gdb stop left `thread apply all bt`; the row's top is `frozen F <- G` (the main
#           thread's innermost game functions: those with a source file; the first two frames when none has one)
#   stuck   frames advance but nothing happens (frame cap in a battle, stuck-walk ...): the exe prints one `bwport: state: k=v ...`
#           line when it ends itself at the cap (port/src/run_frames.c, game part: port/game/route_gen4.c PortRoute_StateLine:
#           frame, overlays loaded, the field task, the running script's id and the native function it waits in: image offsets,
#           `addr2line -f -e EXE 0xOFF` names them); the top is `stuck script N wait +0xOFF` / `stuck task +0xOFF` / `stuck ovl N`
#   harness port_run itself died (no verdict.txt: a traceback on stderr): `harness: ...`, never a game finding
FRAME_RE = re.compile(r'^#(\d+)\s+(?:0x[0-9a-f]+ in )?(\S+) \(.*?\)(?: at (\S+):(\d+))?(?: from (\S+))?$')
STATE_RE = re.compile(r'^bwport: state: (.*)$', re.M)


def verdict_line(r, out_dir):
    """port_run's verdict line of a finished subprocess r (CompletedProcess, text): verdict.txt; without it port_run itself died,
    and the row says so with the traceback's last line (it used to be stdout's last line, the build's undefined-data banner)"""
    try:
        line = open(os.path.join(out_dir, 'verdict.txt')).read().strip()
    except OSError:
        line = ''
    if line:
        return line
    last = ([l.strip() for l in r.stderr.splitlines() if l.strip()] or [l.strip() for l in r.stdout.splitlines() if l.strip()] or ['?'])[-1]
    return f'harness: no verdict from port_run (rc {r.returncode}): {last[:100]}'


def main_bt(log):
    """[(function, file, line)] of the main thread (gdb's Thread 1) in the stop a hang leaves, [] when the log has none"""
    stop = log.split('\n== port_run: stop\n', 1)
    if len(stop) < 2:
        return []
    m = re.search(r'^Thread 1 .*?\n(.*?)(?=^Thread |\Z)', stop[1], re.M | re.S)
    out = []
    for line in (m.group(1) if m else '').splitlines():
        f = FRAME_RE.match(line.strip())
        if f:
            out.append((f.group(2), f.group(3) or '', f.group(4) or ''))
    return out


def frozen_top(log):
    """(top, text, id) of a frozen run: top `frozen F <- G`, text the main thread's first 6 frames"""
    bt = main_bt(log)
    if not bt:
        return 'frozen ?', '', ''
    src = [fr for fr in bt if fr[1]] or bt
    top = 'frozen ' + ' <- '.join(fr[0] for fr in src[:2])
    text = ' <- '.join(f'{fr[0]} ({os.path.basename(fr[1])}:{fr[2]})' if fr[1] else fr[0] for fr in bt[:6])
    return top, text, hashlib.sha1(' '.join(fr[0] for fr in bt[:6]).encode()).hexdigest()[:8]


def state_of(log):
    """{key: value} of the last `bwport: state: k=v k=v ...` line (the exe's game state at its end), {} when none"""
    lines = STATE_RE.findall(log)
    return dict(t.split('=', 1) for t in lines[-1].split() if '=' in t) if lines else {}


def stuck_top(log):
    """(top, text, id) of a run that ended with frames advancing and no result: top `stuck script N wait +0xOFF` (bytecode
    when it waits in no native function) / `stuck task +0xOFF` / `stuck ovl N` from the exe's state line, text the whole line;
    `stuck ?` when the exe printed none"""
    st = state_of(log)
    if not st:
        return 'stuck ?', '', ''
    if st.get('script'):
        native = st.get('native', '+0x0')
        top = f'stuck script {st["script"]} ' + (f'wait {native}' if native != '+0x0' else 'bytecode')
    elif st.get('task', '+0x0') != '+0x0':   # +0x0: no field task runs (an idle field)
        top = f'stuck task {st["task"]}'
    else:
        top = 'stuck ovl ' + st.get('ovl', '?').split(',')[-1]
    return top, ' '.join(f'{k}={v}' for k, v in st.items()), hashlib.sha1(top.encode()).hexdigest()[:8]


def ignore_t(ap):
    """the retired -t option stays accepted (old command lines, lp_route.sh --cmd strings) and does nothing"""
    ap.add_argument('-t', '--timeout', type=float, default=None, help='ignored: no total wall watchdog (sweep_wall.py); a run ends '
                    'by its frames, only a stall (no frame for 30 s) is a hang')


def stuck_walk(warp, end, start, n=STUCK_FRAMES):
    """(X, Z, since) when the walk (V-blanks start..end) ended with the player on one tile for >= n V-blanks, else None.
    warp: the hook's warp.txt text (`pos F X Z in|off` on each tile change)."""
    pos = [(int(f), int(x), int(z)) for f, x, z in re.findall(r'^pos (\d+) (-?\d+) (-?\d+) (?:in|off)$', warp, re.M)]
    if not pos:
        return None
    f, x, z = pos[-1]
    since = max(f, start)
    return (x, z, since) if end - since >= n else None


def stuck_screen(records, key_frames, n=STUCK_FRAMES):
    """(since, last) V-blanks of the first stretch where the picture's hash stayed the same for >= n V-blanks while at
    least one key frame fell in it, else None. records: [(frame, hash)] in frame order (one per V-blank, or per change);
    key_frames: V-blanks at which a key was pressed or held."""
    keys = sorted(key_frames)
    i = 0
    while i < len(records):
        j = i
        while j + 1 < len(records) and records[j + 1][1] == records[i][1]:
            j += 1
        since, last = records[i][0], records[j][0]
        if j + 1 < len(records):
            last = records[j + 1][0] - 1       # the next record's frame is the first one with a new picture
        if last - since + 1 >= n and any(since <= k <= last for k in keys):
            return since, last
        i = j + 1
    return None
