#!/usr/bin/env python3
"""Instructions and cycles per drawn frame on Linux: a number that does not depend on the CPU's clock or the machine's load.

    python3 tools/perf_frame.py platinum town-walk                 # window 600 frames, default start per play
    python3 tools/perf_frame.py black route1-battle --start 400 --window 300 --env BWPORT_RENDERER=vcore BWPORT_RESOLUTION=3
    python3 tools/perf_frame.py platinum town-walk --json out.json --ledger "item=..."   # also append a perf_ledger row

The game runs headless through tools/port_run.py (--no-gdb, --skip 1: every frame drawn, uncapped, the pinned clock and seed
of a play) twice: to V-blank START and to START+WINDOW (BWPORT_EXIT_FRAME: the exe ends itself at the game's own frame).
The CPU counters (perf_event_open through ctypes, user space only, no perf binary) count each whole run from the exec on; the
difference of the two runs over WINDOW is the cost of a drawn frame, boot and loading cancelled. The counters count retired
instructions and core cycles, so a slower or busier CPU moves the cycles' wall time but not the instructions.
Per thread: a poll every 2 ms opens a counter on each new thread (by tid) and names it from /proc comm; the process total
is the exact one (an inherited counter, enabled at the exec). Threads that live shorter than a poll (vcore's per-frame
workers) are only in the total: 'other' = total - the named threads.

--wrap (internal, BWPORT_RUN_WRAP in tools/port_run.py): the program in front of the exe; forks it, counts it, writes the
numbers to $BWPORT_PERF_OUT as JSON when it ends. Needs /proc/sys/kernel/perf_event_paranoid <= 2 and a CPU PMU
(/sys/bus/event_source/devices/cpu); a VM without counters (hardware events not supported) is reported, not guessed."""
import argparse
import ctypes
import json
import os
import re
import struct
import subprocess
import sys
import time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LIBC = ctypes.CDLL(None, use_errno=True)
SYS_PERF_EVENT_OPEN = 298   # x86-64
# (name, perf type, config): hardware instructions / cpu cycles, software task clock
EVENTS = (('instructions', 0, 1), ('cycles', 0, 0), ('task_ns', 1, 1))
ENABLE_ON_EXEC = 1 << 12
INHERIT = 1 << 1
EXCLUDE_KERNEL = 1 << 5
EXCLUDE_HV = 1 << 6
DISABLED = 1 << 0


def perf_open(typ, cfg, pid, inherit=False, on_exec=False):
    """a counter fd (user space only) on thread/process pid, any CPU; -errno when it fails"""
    flags = EXCLUDE_KERNEL | EXCLUDE_HV
    if inherit:
        flags |= INHERIT
    if on_exec:
        flags |= DISABLED | ENABLE_ON_EXEC
    attr = b'\0' * 0
    attr = struct.pack('IIQQQQQ', typ, 128, cfg, 0, 0, 0, flags).ljust(128, b'\0')
    fd = LIBC.syscall(SYS_PERF_EVENT_OPEN, ctypes.c_char_p(attr), pid, -1, -1, 0)
    return fd if fd >= 0 else -ctypes.get_errno()


def read_fd(fd):
    try:
        return struct.unpack('Q', os.read(fd, 8))[0]
    except OSError:
        return 0


class Sampler:
    """diagnosis (BWPORT_PERF_SAMPLE=PERIOD, from V-blank BWPORT_PERF_SAMPLE_FROM): where a thread's instructions go. One
    sample (IP and the user call chain) every PERIOD user-space instructions of the thread (a perf_event ring, drained by the 2 ms
    poll); at the end each sample's function in the exe (addr2line), or its library and the exe function that called into it
    ('libX <- Fn': the chain through frame pointers). The kernel caps the samples a second (perf_event_max_sample_rate, 2500 in
    WSL): a period of ~1 M instructions keeps under it (100 k lost two thirds, and not evenly)"""
    PAGES = 256

    def __init__(self, pid, tid, period):
        self.pid, self.n, self.lost, self.ips = pid, 0, 0, {}
        attr = struct.pack('IIQQQQQ', 0, 128, 1, period, 1 | 32, 0,   # sample_type IP | CALLCHAIN, user chains only
                           EXCLUDE_KERNEL | EXCLUDE_HV | 1 << 21).ljust(128, b'\0')
        self.fd = LIBC.syscall(SYS_PERF_EVENT_OPEN, ctypes.c_char_p(attr), tid, -1, -1, 0)
        if self.fd < 0:
            raise OSError(ctypes.get_errno(), 'perf_event_open (sampling)')
        import mmap
        self.buf = mmap.mmap(self.fd, (1 + self.PAGES) * 4096, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
        self.maps = []
        for line in open(f'/proc/{pid}/maps'):
            p = line.split()
            if len(p) >= 6:
                lo, hi = (int(x, 16) for x in p[0].split('-'))
                self.maps.append((lo, hi, int(p[2], 16), p[5]))
        exe = os.path.realpath(f'/proc/{pid}/exe')
        self.exe = [(lo, hi) for lo, hi, _, path in self.maps if path == exe]
        self.exe_path, self.exe_base = exe, min((lo - off for lo, _, off, path in self.maps if path == exe), default=0)

    def in_exe(self, a):
        return any(lo <= a < hi for lo, hi in self.exe)

    def drain(self):
        size = self.PAGES * 4096
        head = struct.unpack_from('Q', self.buf, 1024)[0]
        tail = struct.unpack_from('Q', self.buf, 1032)[0]
        while tail < head:
            off = tail % size
            typ, _, sz = struct.unpack('IHH', (bytes(self.buf[4096 + off:4096 + min(off + 8, size)]) + bytes(self.buf[4096:4104]))[:8])
            rec = (bytes(self.buf[4096 + off:4096 + min(off + sz, size)]) + bytes(self.buf[4096:4096 + sz]))[:sz]   # (across the end)
            if typ == 9 and sz >= 24:    # PERF_RECORD_SAMPLE: ip, nr, ips[nr]
                ip, nr = struct.unpack_from('QQ', rec, 8)
                caller = 0
                if not self.in_exe(ip):
                    chain = struct.unpack_from(f'{min(nr, (sz - 24) // 8)}Q', rec, 24)
                    caller = next((a for a in chain[1:] if a < 0xfffffffffffff000 and self.in_exe(a)), 0)
                self.ips[(ip, caller)] = self.ips.get((ip, caller), 0) + 1
                self.n += 1
            elif typ == 2:               # PERF_RECORD_LOST
                self.lost += 1
            tail += max(sz, 8)
        struct.pack_into('Q', self.buf, 1032, tail)

    def profile(self, top=40):
        """[(share %, where)] by function in the exe, by library and the calling exe function elsewhere"""
        addrs = sorted({ip - self.exe_base for ip, _ in self.ips if self.in_exe(ip)}
                       | {c - 1 - self.exe_base for _, c in self.ips if c})
        out = subprocess.run(['addr2line', '-f', '-e', self.exe_path] + [hex(a) for a in addrs], stdout=subprocess.PIPE,
                             text=True).stdout.split('\n') if addrs else []
        fn = {a: (out[2 * i] if 2 * i < len(out) else '?') for i, a in enumerate(addrs)}
        where = {}
        for (ip, c), n in self.ips.items():
            if self.in_exe(ip):
                k = fn[ip - self.exe_base]
            else:
                m = next((m for m in self.maps if m[0] <= ip < m[1]), None)
                k = (os.path.basename(m[3]) if m else '?') + ' <- ' + (fn[c - 1 - self.exe_base] if c else '?')
            where[k] = where.get(k, 0) + n
        tot = max(1, sum(where.values()))
        return [(round(100.0 * c / tot, 2), k) for k, c in sorted(where.items(), key=lambda x: -x[1])[:top]]


def counters_available():
    """'' when the hardware counters work, else why not"""
    if not os.path.isdir('/sys/bus/event_source/devices/cpu'):
        return 'no cpu PMU in /sys/bus/event_source/devices (a VM without a virtual PMU)'
    fd = perf_open(0, 1, 0)
    if fd < 0:
        return f'perf_event_open failed: {os.strerror(-fd)} (perf_event_paranoid {open("/proc/sys/kernel/perf_event_paranoid").read().strip()})'
    os.close(fd)
    return ''


def wrap(argv):
    """fork the exe, count it and its threads, write $BWPORT_PERF_OUT; the exit code is the exe's"""
    r, w = os.pipe()
    pid = os.fork()
    if pid == 0:
        os.close(w)
        os.read(r, 1)           # until the parent's counters are open
        os.execv(argv[0], argv)
    os.close(r)
    total = {n: perf_open(t, c, pid, inherit=True, on_exec=True) for n, t, c in EVENTS}
    bad = [n for n, fd in total.items() if fd < 0]
    os.write(w, b'x')
    os.close(w)
    series = []
    ex = None
    if os.environ.get('BWPORT_PERF_SERIES'):   # diagnosis: (time, V-blank, game thread's instructions) every poll
        sys.path.insert(0, os.path.join(REPO, 'tools'))
        import port_run
        ex = port_run.Exe(pid)
        ex.pid = pid
    sample, sampler = int(os.environ.get('BWPORT_PERF_SAMPLE', '0')), None
    sample_from = int(os.environ.get('BWPORT_PERF_SAMPLE_FROM', '0'))
    if sample and ex is None:
        sys.path.insert(0, os.path.join(REPO, 'tools'))
        import port_run
        ex = port_run.Exe(pid)
        ex.pid = pid
    threads = {}                # tid -> (name, {event: fd})
    done = False
    while not done:
        try:
            tids = [int(t) for t in os.listdir(f'/proc/{pid}/task')]
        except OSError:
            tids = []
        for tid in tids:
            try:
                name = open(f'/proc/{pid}/task/{tid}/comm').read().strip()   # read again: the forked child is renamed by the exec
            except OSError:
                continue
            if tid in threads:
                threads[tid] = (name, threads[tid][1])
                continue
            threads[tid] = (name, {n: perf_open(t, c, tid) for n, t, c in EVENTS})
        if ex is not None and pid in threads and os.environ.get('BWPORT_PERF_SERIES'):
            series.append((time.time(), ex.frame(), read_fd(threads[pid][1]['instructions'])))
        if sample and sampler is None and (ex.frame() or 0) >= max(sample_from, 1):
            sampler = Sampler(pid, pid, sample)
            sampler.instr0 = read_fd(threads[pid][1]['instructions']) if pid in threads else 0
        if sampler is not None:
            sampler.drain()
        try:
            p, status = os.waitpid(pid, os.WNOHANG)
        except ChildProcessError:
            p, status = pid, 0
        done = p == pid
        if not done:
            time.sleep(0.002)
    rc = os.waitstatus_to_exitcode(status)
    out = {'exit': rc, 'total': {n: (read_fd(fd) if fd >= 0 else None) for n, fd in total.items()}, 'threads': {}, 'errors': bad,
           'tids': {}}
    for tid, (name, fds) in threads.items():
        if tid == pid:
            name += ' (main)'   # the game thread, apart from the other threads named like the exe (the renderer's workers)
        d = out['threads'].setdefault(name, {n: 0 for n, _, _ in EVENTS} | {'count': 0})
        d['count'] += 1
        one = {n: (read_fd(fd) if fd >= 0 else 0) for n, fd in fds.items()}
        for n, v in one.items():
            d[n] += v
        out['tids'][tid] = dict(one, name=name, main=tid == pid)   # each thread alone (the main thread is the game's)
    if series:
        out['series'] = series
    if sampler is not None:
        sampler.drain()
        out['profile'] = {'samples': sampler.n, 'lost': sampler.lost, 'period': sample, 'from': sample_from, 'instr0': sampler.instr0,
                          'where': sampler.profile()}
    with open(os.environ['BWPORT_PERF_OUT'], 'w') as f:
        json.dump(out, f)
    return rc


# the exe's measurement settings (--env's K=V go over them):
# - every tick drawn and presented, whatever the wall clock (pf2 11-19): a play's fast forward (ff=1) draws and presents at most
#   60 ticks a WALL second (libntr-ios-pace.patch, SpPaceBegin), so the renderer's share followed the run's speed (the "spin"
#   pf1 saw: ~2 G instructions a second whatever the frames); Unlimited (ff=0) draws every tick, and PRESENTDROP=0 keeps the
#   presents a busy GPU would drop (LagSubmit: two presents waiting)
# - the sound by game frames (pf3 11-20): in fast forward (Unlimited too) the music bus (libntr-mu83.patch) makes the music as
#   the device plays it, on the game thread, so its share followed the wall clock (route1-battle 400..700, hq_output on the
#   main thread: 0.58 M instructions a frame at 246 fps, 1.91 M at 19 fps). Off, the music is in the frame's sound (SoundAdvance
#   by V-blank) at its normal tempo (Speed 1): what a player at 60 fps pays a frame
# - the sys line's GPU timer burst 14 of every 60 FRAMES, not of every wall second (BWPORT_SYSBURST=frames, port/src/sysmetrics.c)
# - no side panel (cfgscript side=0, pf4 11-20): its frame graph and perf text follow the frame times, so its GL drawing
#   (ImGui_ImplOpenGL3_RenderDrawData, llvmpipe) grew the main thread ~10 % on a loaded host
BENCH_ENV = {'BWPORT_CFGSCRIPT': 'f1:ff=0,side=0', 'BWPORT_PRESENTDROP': '0', 'BWPORT_MUSICBUS': 'off', 'BWPORT_SYSBURST': 'frames'}


def bench_env(env_extra):
    """the exe's measurement settings, --env's K=V over them"""
    return dict(BENCH_ENV, **env_extra)


def run_to(game, play, frames, env_extra, tag, extra, out):
    """one run to V-blank `frames`, its files in `out`; the counters' JSON"""
    os.makedirs(out, exist_ok=True)
    pf = os.path.join(out, f'{tag}.json')
    if os.path.exists(pf):
        os.remove(pf)
    env = dict(os.environ, BWPORT_RUN_WRAP=f'{sys.executable} {os.path.abspath(__file__)} --wrap --',
               BWPORT_PERF_OUT=pf, BWPORT_SYSPERIOD='1')
    # Mesa's GL program cache of its own (pf4), filled by the warm-up runs, not by the machine's other runs
    env.setdefault('MESA_SHADER_CACHE_DIR', os.path.join(out, 'mesa_cache'))
    env.update(bench_env(env_extra))
    cmd = [sys.executable, os.path.join(REPO, 'tools', 'port_run.py'), '--game', game, '--play', play, '--skip', '1',
           '--frames', str(frames), '--no-gdb', '--shots', '0', '--out', os.path.join(out, tag)]
    cmd += extra if '--dir' in extra else ['--dir', os.path.join(out, 'run')] + extra
    t0 = time.time()
    r = subprocess.run(cmd, cwd=REPO, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    verdict = r.stdout.strip().splitlines()[-1] if r.stdout.strip() else ''
    if not os.path.exists(pf):
        sys.exit(f'perf_frame: no counters from the run ({verdict}); log {out}/{tag}/log.txt')
    d = json.load(open(pf))
    d['wall_s'] = time.time() - t0
    d['verdict'] = verdict
    d['log'] = os.path.join(out, tag, 'log.txt')
    if not verdict.startswith('exit-frame'):
        sys.exit(f'perf_frame: the run did not end at frame {frames}: {verdict}')
    return d


def gpu_ms(log):
    """the mean GPU ms a frame from the exe's sys: lines, None when there is no timer readout"""
    v = []
    for line in open(log, errors='replace'):
        m = re.search(r'gpu (?:<=)?([\d.]+) ms', line)
        if line.startswith('sys:') and m:
            v.append(float(m.group(1)))
    return sum(v) / len(v) if v else None


PORT = ('bwport (main)', 'bwport')   # the port's own code: the game thread and the renderer's workers (vcore, the bridges)


def diff(a, b, frames):
    """per drawn frame (millions of instructions / cycles, ms of task clock) of each thread class and the total"""
    def per(x, y):
        return {k: (y[k] - x[k]) / frames for k in ('instructions', 'cycles', 'task_ns')}
    zero = {'instructions': 0, 'cycles': 0, 'task_ns': 0}
    res = {'total': per(a['total'], b['total'])}
    for name in sorted(set(a['threads']) | set(b['threads'])):
        res[name] = per(a['threads'].get(name, zero), b['threads'].get(name, zero))
    named = {k: sum(v[k] for n, v in res.items() if n != 'total') for k in zero}
    res['(other: short threads)'] = {k: res['total'][k] - named[k] for k in zero}
    # the stable part: the port's threads (exact to ~0.1 % run to run); llvmpipe (the software GL here) moves ~1 % with
    # the scheduling, the sound callback and the WM thread follow the wall clock
    res['port (main + workers)'] = {k: sum(res.get(n, zero)[k] for n in PORT) for k in zero}
    return {n: {'M_instr': v['instructions'] / 1e6, 'M_cycles': v['cycles'] / 1e6, 'cpu_ms': v['task_ns'] / 1e6}
            for n, v in res.items()}


def main_instr(d):
    """the game thread's instructions in one run's counters"""
    return next(v['instructions'] for v in d['tids'].values() if v.get('main'))


def warm_up(run, most=4, tol=0.005):
    """warm-up runs (to the window's end, as run b) until two in a row agree on the main thread; the runs made.
    A new run dir's first start migrates and hashes the ROM on the main thread (launcher_core.c LC_Migrate, ~60 G instructions;
    pf3: it landed in run a only, a negative main thread). Renderer gpu's GL programs (Mesa's cache) take several runs to settle
    (pf4, Platinum town-walk gpu 1x, main thread of runs to 7900: 131.5, 106.7, 100.3, 100.3 G; one warm-up run to frame 2 gave
    a0..b0 -39 M a frame, one to 7900 6.9 M, the settled runs 21.3)"""
    prev = None
    for i in range(most):
        m = main_instr(run(i))
        if prev is not None and abs(m / prev - 1) <= tol:
            return i + 1
        prev = m
    return most


def main():
    if sys.argv[1:2] == ['--wrap']:
        return wrap(sys.argv[3:] if sys.argv[2:3] == ['--'] else sys.argv[2:])
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('game')
    ap.add_argument('play')
    ap.add_argument('--start', type=int, help='V-blank where the window starts (default: the play script\'s walk start + 100)')
    ap.add_argument('--window', type=int, default=600, help='frames in the window (default 600)')
    ap.add_argument('--env', nargs='*', default=[], metavar='K=V', help='the exe\'s environment (BWPORT_RENDERER=gpu ...)')
    ap.add_argument('--json', help='write the result here')
    ap.add_argument('--repeat', type=int, default=1, help='runs (the table shows the minimum of each figure and the spread)')
    ap.add_argument('--work', default=os.path.join(REPO, 'build', 'perf_frame'),
                    help='the runs\' files and run dir (default build/perf_frame; one each for runs side by side)')
    a, extra = ap.parse_known_args()
    a.work = os.path.abspath(a.work)   # the wrapper writes its JSON from the exe's run dir
    why = counters_available()
    if why:
        sys.exit('perf_frame: the CPU counters are not available here: ' + why)
    env = dict(e.split('=', 1) for e in a.env)
    start = a.start or DEFAULT_START.get((a.game, a.play), 0)
    if not start:
        sys.exit('perf_frame: no default --start for this play: give --start FRAME')
    warm_up(lambda i: run_to(a.game, a.play, start + a.window, env, f'warm{i}', extra, a.work))
    runs = []
    for i in range(a.repeat):
        ra = run_to(a.game, a.play, start, env, f'a{i}', extra, a.work)
        rb = run_to(a.game, a.play, start + a.window, env, f'b{i}', extra, a.work)
        runs.append({'threads': diff(ra, rb, a.window), 'gpu_ms': gpu_ms(rb['log']), 'wall_s': ra['wall_s'] + rb['wall_s'],
                     'load1': os.getloadavg()[0]})
    # the minimum over the runs of each figure (as perf_ledger's rows), and how far the runs were apart
    res = {'game': a.game, 'play': a.play, 'start': start, 'window': a.window, 'env': env, 'runs': len(runs),
           'threads': {n: {k: min(r['threads'][n][k] for r in runs) for k in runs[0]['threads'][n]} for n in runs[0]['threads']
                       if all(n in r['threads'] for r in runs)},
           'spread_pct': {k: 100.0 * (max(r['threads']['total'][k] for r in runs) / min(r['threads']['total'][k] for r in runs) - 1)
                          for k in ('M_instr', 'M_cycles', 'cpu_ms')},
           # each row's instructions: how far the runs were apart (max / min - 1)
           'spread_instr_pct': {n: 100.0 * (max(r['threads'][n]['M_instr'] for r in runs) / min(r['threads'][n]['M_instr'] for r in runs) - 1)
                                for n in runs[0]['threads'] if all(n in r['threads'] for r in runs)
                                and min(r['threads'][n]['M_instr'] for r in runs) > 0.05},
           'all_total': [r['threads']['total'] for r in runs], 'all_threads': [r['threads'] for r in runs], 'gpu_ms': runs[0]['gpu_ms'], 'wall_s': [r['wall_s'] for r in runs],
           'load1': [r['load1'] for r in runs]}
    if a.json:
        json.dump(res, open(a.json, 'w'), indent=1)
    print(f'{a.game} {a.play} frames {start}..{start + a.window} {" ".join(a.env)}')
    print(f'{"thread":<26}{"M instr":>10}{"M cycles":>10}{"CPU ms":>9}{"IPC":>6}{"spread":>9}')
    for n, v in res['threads'].items():
        if abs(v['M_instr']) >= 0.005 or n == 'total':
            sp = res['spread_instr_pct'].get(n)
            sp = f'{sp:>8.2f}%' if sp is not None and len(runs) > 1 else ''
            print(f'{n:<26}{v["M_instr"]:>10.3f}{v["M_cycles"]:>10.3f}{v["cpu_ms"]:>9.3f}{v["M_instr"] / max(v["M_cycles"], 1e-9):>6.2f}{sp}')
    print(f'gpu ms/frame: {res["gpu_ms"]}')
    if len(runs) > 1:
        print('spread of the total over %d runs: instructions %.2f %%, cycles %.2f %%, CPU ms %.2f %% (load average %s)' % (
            len(runs), *res['spread_pct'].values(), ' '.join('%.1f' % x for x in res['load1'])))
    return 0


# the play's walk starts at this V-blank (tools/plays/*/NAME.txt); the window opens 100 frames in
DEFAULT_START = {('platinum', 'town-walk'): 7600, ('black', 'route1-battle'): 400}

if __name__ == '__main__':
    sys.exit(main())
