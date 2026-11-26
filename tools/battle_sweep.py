#!/usr/bin/env python3
"""battle_sweep.py: the game-neutral battle sweep: every move used by BOTH sides and every species on each side, run through
a game's test-battle hooks, many cases per battle, crash-and-continue, one results table (docs/battle-sweep.md).

    python3 tools/battle_sweep.py run --adapter hg --game heartgold --what moves --ids all -j 6 --out DIR
    python3 tools/battle_sweep.py run --adapter hg --what species-foe --ids 1-40,151 --out DIR
    python3 tools/battle_sweep.py submit --adapter hg --shards bd:3,bd:3,laptop:1 --what moves --out DIR --commit REF
    python3 tools/battle_sweep.py summary --out DIR
    python3 tools/battle_sweep.py report --out DIR --md FILE      # ranked markdown: names, backtraces, culprit side, repro

What a case is (the exe's side: the game's own test hook prints the protocol lines on stderr, port/src/run_frames.c):
  moves          one TURN of one battle: our move M against the foe's move partner(M) (every move is used once by each side);
                 `sweep: case K our M foe F start at V-blank N` ... `sweep: case K done at V-blank N (F frames)`
  species-foe    one battle: the wild opponent is species N, the run ends at the first move menu
  species-our    one battle: our Pokemon is species N, the run ends at the first move menu
CRASH-AND-CONTINUE: an instance (one exe run, one battle, up to --batch cases) that crashes, hangs or caps records the case that
was running (the last `start` without `done`) with the gdb backtrace and goes on with the cases after it in a new instance. A case
over the per-case frame cap (--case-frames, game V-blanks, never wall time) is a hang. --isolate re-runs every failed move case
as (M vs a harmless move) and (harmless vs F): which side's move it was.
SHARDING: --shard I/N [--weights w1,..,wN] runs the I-th of N contiguous slices of the case list (weights: a machine 3x faster
takes a 3x longer slice); `submit` starts one shard per `MACHINE:WEIGHT` entry through the job runner (BATTLE_SWEEP_RUNNER, default
chain_slot.sh: --commit REF --on MACHINE --cmd ...), reads their `BSROWS` line back and writes the merged table.
Adapter: tools/battle_sweep_<name>.py, see docs/battle-sweep.md "Adapter interface" (a dozen names; HG/SS is the model).
Out: results.tsv (case, kind, outcome, frames, top, bt_id, backtrace top 3, culprit, prev cases, wall_s), summary.txt, runs/<case>/ (log.txt.gz
of every failed instance, bt.txt).
"""
import argparse
import base64
import collections
import concurrent.futures
import gzip
import hashlib
import importlib
import os
import re
import shlex
import traceback
import shutil
import subprocess
import sys
import threading
import time
import types
import queue
import signal

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'tools'))
import sweep_wall  # noqa: E402  (no wall watchdog: a run is judged on game frames)

HEAD = ['case', 'kind', 'outcome', 'frames', 'top', 'bt_id', 'backtrace', 'culprit', 'prev', 'wall_s']
KINDS = ('moves', 'species-foe', 'species-our')


# ---------------------------------------------------------------- cases

def parse_ids(spec, top):
    """'all' | '1-40,151' -> sorted list within 1..top"""
    if spec == 'all':
        return list(range(1, top + 1))
    out = set()
    for part in spec.split(','):
        lo, _, hi = part.partition('-')
        out.update(range(int(lo), int(hi or lo) + 1))
    return sorted(i for i in out if 1 <= i <= top)


def foe_partner(m, top):
    """the foe's move of case m: half the table away, so every move is also used by the FOE (case m's foe move is case
    partner(m)'s our move: a crash names a pair, --isolate names the side)"""
    return (m - 1 + top // 2) % top + 1


def make_cases(ad, what, ids):
    """[(key, kind, our, foe)]: moves -> 'm<our>-<foe>'; species -> 's<N>' (our = species, foe = 0)"""
    if what == 'moves':
        return [(f'm{m}-{foe_partner(m, ad.MAX_MOVE)}', 'moves', m, foe_partner(m, ad.MAX_MOVE)) for m in ids]
    return [(f'{"f" if what == "species-foe" else "o"}{n}', what, n, 0) for n in ids]


def make_pairs(spec):
    """--pairs OUR:FOE[@SPECIES/LEVEL],..: single turns with a chosen foe species -> (key, 'moves', our, foe, 'SPECIES/LEVEL')"""
    out = []
    for item in spec.split(','):
        m = re.fullmatch(r'(\d+):(\d+)(?:@(\d+/\d+))?', item.strip())
        if not m:
            raise SystemExit(f'--pairs: OUR:FOE[@SPECIES/LEVEL], not {item!r}')
        out.append((f'p{m.group(1)}-{m.group(2)}' + (f'@{m.group(3).replace("/", "L")}' if m.group(3) else ''), 'moves',
                    int(m.group(1)), int(m.group(2)), m.group(3) or ''))
    return out


def shard_slice(cases, shard, weights):
    """the contiguous slice of `cases` for shard 'I/N' (1-based) with the weights (default equal)"""
    if not shard:
        return cases
    i, _, n = shard.partition('/')
    i, n = int(i), int(n)
    w = [float(x) for x in weights.split(',')] if weights else [1.0] * n
    if len(w) != n:
        raise SystemExit(f'--weights needs {n} values')
    tot = sum(w)
    lo = round(len(cases) * sum(w[:i - 1]) / tot)
    hi = round(len(cases) * sum(w[:i]) / tot)
    return cases[lo:hi]


# ---------------------------------------------------------------- one instance (one exe run)

def parse_sweep(log):
    """-> (started [k], done {k: frames}, caps {k}, end why or None, start-vblank {k})"""
    started, done, caps, start_v = [], {}, set(), {}
    for m in re.finditer(r'^sweep: case (\d+) (?:(?!start at).)* ?start at V-blank (\d+)$', log, re.M):
        started.append(int(m.group(1)))
        start_v[int(m.group(1))] = int(m.group(2))
    for m in re.finditer(r'^sweep: case (\d+) done at V-blank \d+ \((\d+) frames\)$', log, re.M):
        done[int(m.group(1))] = int(m.group(2))
    for m in re.finditer(r'^sweep: case (\d+) cap (\d+) frames', log, re.M):
        caps.add(int(m.group(1)))
    end = re.search(r'^sweep: end (\S+) at V-blank', log, re.M)
    return started, done, caps, end.group(1) if end else None, start_v


_EXE_CACHE = threading.local()


def _frame_reader(root_pid):
    """port_run's Exe for the process tree under root_pid (its V-blank counter read through /proc/<pid>/mem); the exe's symbol
    table is read once per thread"""
    import port_run
    ex = getattr(_EXE_CACHE, 'exe', None)
    if ex is None:
        ex = _EXE_CACHE.exe = port_run.Exe(root_pid)
    ex.root_pid, ex.pid, ex.count_addr = root_pid, None, None
    return ex


def watch_run(a, p, log_path):
    """the harness's own watchdog over a port_run subprocess `p` (wall time is allowed for a WATCHDOG only; what a case does is
    judged on V-blanks): the exe's V-blank counter is read every second; no new V-blank for --watchdog s (a game spinning in a
    loop that never reaches the V-blank wait, port_run's own stall rule not firing or its gdb stuck) or a log over --max-log MB (a
    spamming loop) ends the run: SIGINT to the exe (the gdb port_run runs it under prints `thread apply all bt` into the log),
    then SIGKILL of the whole session (started by us: only our own PIDs). Returns a reason string or ''."""
    try:
        ex = _frame_reader(p.pid)
    except Exception:   # no port_run / nm: no watchdog (the process runs to its own end)
        ex = None
    t0 = last_t = time.monotonic()
    last = None
    why = ''
    while p.poll() is None:
        time.sleep(1.0)
        now = time.monotonic()
        if ex is not None:
            if ex.pid is None:
                ex.find()
            f = ex.frame() if ex.pid else None
            if f != last:
                last, last_t = f, now
            limit = a.watchdog if last else a.watchdog * 3   # before the first V-blank: the boot, 3x
            if now - last_t >= limit:
                why = f'watchdog: no V-blank for {now - last_t:.0f} s (counter {last})'
        try:
            if not why and os.path.getsize(log_path) > a.max_log * 1048576:
                why = f'watchdog: log over {a.max_log} MB (the game spams)'
        except OSError:
            pass
        if why:
            break
    if not why:
        return ''
    try:   # the backtrace: gdb under port_run catches the SIGINT and prints it into the log, then ends the process
        if ex is not None and ex.pid:
            os.kill(ex.pid, signal.SIGINT)
        p.wait(timeout=40)
    except (OSError, subprocess.TimeoutExpired):
        pass
    try:
        os.killpg(p.pid, signal.SIGKILL)
    except OSError:
        pass
    p.wait()
    return why


def run_instance(a, ad, kind, batch, slot, tag):
    """one exe run over `batch` ([(key, kind, our, foe)]); returns (log, line, wall)"""
    run_dir = prepare_slot(a, ad, slot)
    out_dir = os.path.join(run_dir, 'out')
    shutil.rmtree(out_dir, ignore_errors=True)
    env = dict(os.environ, PYTHONDONTWRITEBYTECODE='1', BWPORT_SWEEP_CASE_FRAMES=str(a.case_frames))
    for k in [k for k in env if k.startswith('BWPORT_') and k.split('_')[1] in ('PILOT', 'TESTCHEAT')]:
        env.pop(k)
    env.update(ad.instance_env(kind, batch, a))
    pulse_to = a.pulse_to
    pulses = ' '.join(f'a@f{f}' for f in range(ad.PULSE_FROM, pulse_to, ad.PULSE_STEP))
    env['BWPORT_KEYS'] = ad.reload_keys() + ' ' + pulses
    cmd = ['python3', os.path.join(REPO, 'tools/port_run.py'), '--game', ad.PORT_GAME, '--no-build', '--save', a.save or ad.SAVE,
           '--dir', run_dir, '--out', out_dir, '--frames', str(pulse_to), '-t', sweep_wall.NO_WALL_CAP, '--shots', '0',
           '--hang', str(a.port_hang), '--hang-stop', '--skip', '4'] + ad.port_run_args(kind)
    if a.heapcheck:
        cmd += ['--heapcheck', a.heapcheck]
    os.makedirs(out_dir, exist_ok=True)
    so_path = os.path.join(run_dir, 'port_run.out')
    t0 = time.time()
    with open(so_path, 'w') as so:
        p = subprocess.Popen(['nice', '-n', '10'] + cmd, env=env, stdout=so, stderr=subprocess.STDOUT, cwd=REPO,
                             start_new_session=True)
        why = watch_run(a, p, os.path.join(out_dir, 'log.txt')) if a.watchdog > 0 else ''
        p.wait()
    wall = time.time() - t0
    r = types.SimpleNamespace(returncode=p.returncode, stdout=open(so_path, errors='replace').read()[-4000:], stderr='')
    try:
        line = sweep_wall.verdict_line(r, out_dir)
    except OSError:
        line = '?'
    try:
        log = open(os.path.join(out_dir, 'log.txt'), errors='replace').read()
    except OSError:
        log = r.stdout
    if why and not line.startswith(('hang', 'crash', 'heapcheck')):   # port_run's own verdict first; else the watchdog's
        line = 'hang in ? (' + why + ')'
    if why and len(log) > 20 * 1048576:   # a spamming game: keep the head and the tail
        log = log[:4 * 1048576] + '\n[... log cut ...]\n' + log[-4 * 1048576:]
    shutil.rmtree(out_dir, ignore_errors=True)
    return log, line, wall


_SLOT_LOCK = threading.Lock()
KEEP_OUT_OF_LINKS = ('save.bin', 'DbgLog.txt', 'backups')   # what the game or port_run writes in the game folder


def prepare_slot(a, ad, slot):
    """the run dir of worker `slot` (out/run/w<slot>): one ROM import per job (out/run/tmpl, ~250 MB for HG/SS), every worker dir
    links the imported game folder's files (read-only data) and owns only the save and the small files the game writes: a job's
    run dirs are one copy of the data, not one per worker or per batch; cmd_run removes out/run when it ends"""
    run_dir = os.path.join(a.out, 'run', f'w{slot}')
    with _SLOT_LOCK:
        if os.path.exists(os.path.join(run_dir, '.ready')):
            return run_dir
        import port_run
        import play_script
        game = ad.PORT_GAME
        folder = play_script.GAMES[game]
        tmpl = os.path.join(a.out, 'run', 'tmpl')
        if not os.path.isdir(os.path.join(tmpl, folder)):
            os.makedirs(tmpl, exist_ok=True)
            port_run.import_game(tmpl, game)
        shutil.rmtree(run_dir, ignore_errors=True)
        os.makedirs(os.path.join(run_dir, folder))
        for name in os.listdir(os.path.join(tmpl, folder)):
            if name not in KEEP_OUT_OF_LINKS:
                os.symlink(os.path.join(tmpl, folder, name), os.path.join(run_dir, folder, name))
        for name in os.listdir(tmpl):
            if name != folder and os.path.isfile(os.path.join(tmpl, name)):
                shutil.copyfile(os.path.join(tmpl, name), os.path.join(run_dir, name))
        open(os.path.join(run_dir, '.ready'), 'w').close()
    return run_dir


def verdict_of(ad, log, line):
    """(outcome, top, text, btid) of an instance that did not finish by itself; outcome crash|hang|heap|harness|cap|none"""
    if line.startswith('heapcheck:'):
        mo = re.search(r'heap (?:overrun|misuse)( after \S+| \(\w+ [^)]*\))', line)
        return 'heap', 'heap overrun' + (mo.group(1) if mo else ''), line, ''
    if line.startswith('crash'):
        return ('crash',) + tuple(ad.crash_info(log))
    if line.startswith('hang'):
        return ('hang',) + tuple(sweep_wall.frozen_top(log))
    if line.startswith('harness'):
        return 'harness', line[:100], line, ''
    if line.startswith('exit-frame'):
        return ('cap',) + tuple(sweep_wall.stuck_top(log))
    return 'none', line[:80], line, ''


def short_bt(text):
    return ' <- '.join(text.split(' <- ')[:3])


def take_batch(cases, n):
    """the next battle's cases: up to n, a case with its own foe spec (--pairs: a 5th item) alone"""
    return cases[:1] if len(cases[0]) > 4 else cases[:n]


def run_batch(a, ad, kind, cases, slot, record):
    """all of `cases` with crash-and-continue; record(row dict) per case"""
    remaining = list(cases)
    prev = collections.deque(maxlen=3)
    nostart = 0
    while remaining:
        batch = take_batch(remaining, a.batch) if kind == 'moves' else remaining[:1]
        log, line, wall = run_instance(a, ad, kind, batch, slot, f'{batch[0][0]}')
        started, done, caps, end, _ = parse_sweep(log)
        if kind != 'moves':   # one battle per case: ok = the game's own end line
            key = batch[0][0]
            ok = ad.species_ok(log)
            outcome, top, text, btid = ('ok', '', '', '') if ok else verdict_of(ad, log, line)
            if not ok and outcome == 'none':
                outcome = 'no-battle' if not ad.battle_started(log) else 'timeout'
            row = dict(case=key, kind=kind, outcome=outcome, frames=ad.species_frames(log) if ok else '', top=top, bt_id=btid or
                       (hashlib.sha1(top.encode()).hexdigest()[:8] if top else ''), backtrace=short_bt(text), culprit='', prev='',
                       wall_s=f'{wall:.0f}')
            if outcome != 'ok':
                save_failure(a, key, log, line, text)
            record(row)
            remaining = remaining[1:]
            continue
        # moves: the batch's cases are the exe's case numbers 0..len-1
        n_ok = 0
        for k in started:
            if k in done:
                n_ok += 1
        last = started[-1] if started else -1
        finished = end in ('list', 'battle')
        stop_at = None   # index (in batch) of the case that failed
        outcome, top, text, btid = 'ok', '', '', ''
        if end == 'list' and len(done) >= len(batch):
            pass
        elif end == 'battle':   # the battle ended (a blown-away wild Pokemon, a faint): the last case ran and closed
            pass
        elif last in caps:
            outcome, top, text, btid = 'hang', 'frame cap', 'case over %d frames (V-blank wait)' % a.case_frames, ''
            tail = verdict_of(ad, log, line)
            top = 'frame cap ' + (tail[1] if tail[1] else '')
            text = tail[2] or text
            btid = tail[3]
            stop_at = last
        else:
            outcome, top, text, btid = verdict_of(ad, log, line)
            if outcome == 'cap':   # the pulse horizon (the instance's own frame cap), not a case cap
                outcome = 'hang'
            stop_at = last if last >= 0 else 0
            if last < 0:
                outcome = 'no-start' if outcome in ('none', 'hang', 'cap') else outcome
        for k, c in enumerate(batch):
            if stop_at is not None and k > stop_at:
                break
            if k in done or (k == last and end == 'battle'):
                record(dict(case=c[0], kind=kind, outcome='ok' if k in done else 'ok-end', frames=done.get(k, ''), top='', bt_id='',
                            backtrace='', culprit='', prev='', wall_s=''))
            elif k == stop_at:
                bid = btid or (hashlib.sha1(top.encode()).hexdigest()[:8] if top else '')
                record(dict(case=c[0], kind=kind, outcome=outcome, frames='', top=top, bt_id=bid, backtrace=short_bt(text), culprit='',
                            prev=' '.join(prev), wall_s=f'{wall:.0f}'))
                save_failure(a, c[0], log, line, text)
            else:   # started never (the exe ended before the case's menu): re-run it
                break
            prev.append(c[0])
        processed = (stop_at + 1) if stop_at is not None else (last + 1 if end == 'battle' else len(batch))
        if end == 'list':
            processed = len(batch)
        if processed <= 0:   # no progress: the battle never reached its first menu
            nostart += 1
            if nostart >= 2:
                c = batch[0]
                record(dict(case=c[0], kind=kind, outcome='no-start', frames='', top=top or line[:80], bt_id='', backtrace='',
                            culprit='', prev='', wall_s=f'{wall:.0f}'))
                save_failure(a, c[0], log, line, text)
                processed, nostart = 1, 0
            else:
                continue
        else:
            nostart = 0
        remaining = remaining[processed:]


def save_failure(a, key, log, line, text):
    d = os.path.join(a.out, 'runs', key)
    os.makedirs(d, exist_ok=True)
    with gzip.open(os.path.join(d, 'log.txt.gz'), 'wt') as f:
        f.write(log)
    with open(os.path.join(d, 'bt.txt'), 'w') as f:
        f.write(line + '\n' + text + '\n' + '\n'.join(re.findall(r'^(?:sweep|test): .*$', log, re.M)[-30:]) + '\n')


# ---------------------------------------------------------------- run / summary

class Table:
    def __init__(self, out):
        self.path = os.path.join(out, 'results.tsv')
        self.lock = threading.Lock()
        self.rows = []
        if not os.path.exists(self.path):
            with open(self.path, 'w') as f:
                f.write('\t'.join(HEAD) + '\n')

    def add(self, row):
        with self.lock:
            self.rows.append(row)
            with open(self.path, 'a') as f:
                f.write('\t'.join(str(row.get(h, '')).replace('\t', ' ').replace('\n', ' ') for h in HEAD) + '\n')


def load_rows(out):
    path = os.path.join(out, 'results.tsv')
    if not os.path.exists(path):
        return []
    lines = [l.rstrip('\n').split('\t') for l in open(path)]
    return [dict(zip(HEAD, r + [''] * (len(HEAD) - len(r)))) for r in lines[1:]]


def drop_harness_rows(out):
    """resume: a `harness` row is the SWEEP's own error (an exception, a dead exe, no slot), not a verdict on the case: it must not count
    as done (a stale out dir whose earlier run failed everywhere looked like finished results). Removes those rows from results.tsv,
    returns how many; the cases then run again."""
    rows = load_rows(out)
    keep = [r for r in rows if r['outcome'] != 'harness']
    if len(keep) == len(rows):
        return 0
    with open(os.path.join(out, 'results.tsv'), 'w') as f:
        f.write('\t'.join(HEAD) + '\n')
        for r in keep:
            f.write('\t'.join(str(r.get(h, '')).replace('\t', ' ').replace('\n', ' ') for h in HEAD) + '\n')
    return len(rows) - len(keep)


def write_progress(msg, path=None):
    """the job's live progress line for the job runner's progress viewer / `submit --progress` ($NTR_PROGRESS on the slot; none elsewhere)"""
    path = path or os.environ.get('NTR_PROGRESS')
    if path:
        try:
            with open(path, 'w') as f:
                f.write(msg + '\n')
        except OSError:
            pass


def summarize(a, ad):
    rows = load_rows(a.out)
    c = collections.Counter(r['outcome'] for r in rows)
    groups = collections.OrderedDict()
    for r in rows:
        if r['outcome'] in ('ok', 'ok-end'):
            continue
        g = groups.setdefault((r['outcome'], r['top'] or '?', r['bt_id']), {'cases': [], 'bt': r['backtrace'], 'culprit': set()})
        g['cases'].append(r['case'])
        if r['culprit']:
            g['culprit'].add(r['culprit'])
    lines = [f'battle_sweep {getattr(ad, "NAME", "?")}: {len(rows)} cases, ' + ', '.join(f'{k} {v}' for k, v in sorted(c.items())) +
             f' (per-case frame cap {a.case_frames})']
    for (kind, top, bid), g in sorted(groups.items(), key=lambda kv: -len(kv[1]['cases'])):
        lines.append(f'{len(g["cases"]):4d}  {kind:9s} {top}  [{bid}]  e.g. {",".join(g["cases"][:6])}'
                     + (f'  culprit {"/".join(sorted(g["culprit"]))}' if g['culprit'] else ''))
        lines.append(f'        {g["bt"]}')
    fr = sorted(int(r['frames']) for r in rows if r['outcome'] == 'ok' and str(r['frames']).isdigit())
    if fr:
        lines.append(f'ok cases V-blanks: median {fr[len(fr) // 2]}, 90% {fr[int(len(fr) * .9)]}, max {fr[-1]}')
    text = '\n'.join(lines) + '\n'
    open(os.path.join(a.out, 'summary.txt'), 'w').write(text)
    return text


def isolate(a, ad, rows, record_iso):
    """failed move cases -> culprit: run (our vs harmless) and (harmless vs foe) alone, in fresh battles"""
    bad = [r for r in rows if r['kind'] == 'moves' and r['outcome'] in ('crash', 'hang', 'heap', 'cap')]
    sp = ad.HARMLESS
    pool = queue.Queue()
    for i in range(100, 100 + max(1, a.j)):
        pool.put(i)

    specs = {c[0]: c[4] for c in make_pairs(a.pairs)} if a.pairs else {}   # --pairs keys (p433-153@...): their foe species

    def one(r):
        m = re.match(r'[mp](\d+)-(\d+)', r['case'])
        our, foe = int(m.group(1)), int(m.group(2))
        extra = (specs[r['case']],) if specs.get(r['case']) else ()   # the pair's own foe species / level, kept in both halves
        res = {}
        for side, case in (('our', (f'i{our}-{sp}', 'moves', our, sp) + extra), ('foe', (f'i{sp}-{foe}', 'moves', sp, foe) + extra)):
            got = []
            sl = pool.get()
            try:
                run_batch(a, ad, 'moves', [case], sl, got.append)
            finally:
                pool.put(sl)
            res[side] = got[0]['outcome'] if got else '?'
        bits = [s for s in ('our', 'foe') if res[s] not in ('ok', 'ok-end')]
        r['culprit'] = '+'.join(bits) if bits else 'state'   # neither alone: it needs the pair or the battle's earlier state
        record_iso(r)
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, a.j)) as ex:
        list(ex.map(one, bad))


def claim(qdir, key):
    """take a work unit off the (shared) queue: an atomic mkdir, one winner among all the threads and jobs of a host"""
    try:
        os.mkdir(os.path.join(qdir, re.sub(r'[^\w.@-]', '_', key)))
        return True
    except FileExistsError:
        return False


def work_units(a, cases):
    """moves: batches of --batch cases (one battle each); species / pairs: one case each"""
    if a.what != 'moves' or a.pairs:
        return [[c] for c in cases]
    return [cases[i:i + a.batch] for i in range(0, len(cases), a.batch)]


def cmd_run(a, ad):
    """a shard run through the job runner (--rows: the results leave as the BSROWS line) removes its bs-* work dir when it ends,
    on failure and on SIGTERM too, unless --keep-work (0.2 GB per shard were left on the build machines)"""
    if not (a.rows and not a.keep_work and os.path.basename(os.path.abspath(a.out)).startswith('bs-')):
        return run_core(a, ad)
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(143))
    try:
        return run_core(a, ad)
    finally:
        shutil.rmtree(a.out, ignore_errors=True)


def run_core(a, ad):
    os.makedirs(a.out, exist_ok=True)
    top = ad.MAX_MOVE if a.what == 'moves' else ad.MAX_SPECIES
    if a.pairs:
        cases = make_pairs(a.pairs)
    else:
        cases = shard_slice(make_cases(ad, a.what, parse_ids(a.ids, top)), a.shard, a.weights)
    if not a.redo:
        nh = drop_harness_rows(a.out)
        if nh:
            print(f'resume: {nh} harness-error rows of an earlier run in {a.out} do not count as done: they run again', flush=True)
    done = {r['case'] for r in load_rows(a.out)} if not a.redo else set()
    cases = [c for c in cases if c[0] not in done]
    table = Table(a.out)
    units = work_units(a, cases)
    # the dynamic queue: every worker takes the next unit nobody has (--queue DIR shared by the jobs of one host)
    qdir = a.queue or os.path.join(a.out, 'queue')
    if not a.queue:
        shutil.rmtree(qdir, ignore_errors=True)
    os.makedirs(qdir, exist_ok=True)
    t0 = time.time()
    lock = threading.Lock()
    nproc = [0]
    kind = 'moves' if a.what == 'moves' else a.what

    def worker(slot):
        for unit in units:
            if not claim(qdir, unit[0][0]):
                continue
            try:
                run_batch(a, ad, kind, unit, slot, table.add)
            except Exception as e:   # a harness error is a row, never the end of the sweep
                traceback.print_exc()
                for c in unit:
                    table.add(dict(case=c[0], kind=kind, outcome='harness', frames='', top=f'{type(e).__name__}: {e}'[:100], bt_id='',
                                   backtrace='', culprit='', prev='', wall_s=''))
            with lock:
                nproc[0] += len(unit)
                msg = f'[{time.time() - t0:.0f}s] {nproc[0]}/{len(cases)} cases (worker {slot})'
                print(msg, flush=True)
                write_progress(msg)

    try:
        with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, a.j)) as ex:
            for f in [ex.submit(worker, i) for i in range(max(1, a.j))]:
                f.result()
        if a.isolate:
            isolate(a, ad, table.rows, lambda r: None)
            rewrite(a, table.rows, load_rows(a.out))
    finally:
        shutil.rmtree(os.path.join(a.out, 'run'), ignore_errors=True)   # the run dirs (the imported game data): failures keep their logs in runs/
        if not a.queue:
            shutil.rmtree(qdir, ignore_errors=True)
    print(summarize(a, ad))
    print(f'wall {time.time() - t0:.0f} s')
    if a.rows:   # ONE line: the job runner shows only the last lines of a command's output (a line per row lost 427 of 467 rows)
        tsv = '\n'.join('\t'.join(r[h] for h in HEAD) for r in load_rows(a.out))
        print('BSROWS ' + base64.b64encode(gzip.compress(tsv.encode())).decode())
    return 0


def rewrite(a, changed, allrows):
    byc = {r['case']: r for r in changed}
    path = os.path.join(a.out, 'results.tsv')
    with open(path, 'w') as f:
        f.write('\t'.join(HEAD) + '\n')
        for r in allrows:
            r = byc.get(r['case'], r)
            f.write('\t'.join(str(r.get(h, '')).replace('\t', ' ') for h in HEAD) + '\n')


# ---------------------------------------------------------------- submit (shards over machines)

def decode_rows(line):
    """the rows of one `BSROWS <base64 of gzip>` line; a line a flaky ssh/wsl hop cut or mixed with noise gives [] (never an exception)"""
    b = re.sub(r'[^A-Za-z0-9+/=]', '', line.split(' ', 1)[1]) if ' ' in line else ''
    try:
        return [r for r in gzip.decompress(base64.b64decode(b)).decode().splitlines() if r]
    except (ValueError, OSError, EOFError):
        return []



def drop_off(shards, off=None):
    """machines named in BATTLE_SWEEP_OFF (comma list: the ones that are switched off) lose their shards, with a warning; returns the kept shards"""
    off = [m for m in (os.environ.get('BATTLE_SWEEP_OFF', '') if off is None else off).split(',') if m]
    kept = [s for s in shards if s[0] not in off]
    if len(kept) != len(shards):
        print(f'warning: {",".join(off)} is off (BATTLE_SWEEP_OFF): dropped {len(shards) - len(kept)} shard(s)', flush=True)
    return kept


RUN_ID_RE = re.compile(r'^lp_route: (\S+): commit ', re.M)


def peek_progress(runner, log_path):
    """the last progress line of the shard whose runner output is log_path: its run id (the runner's `lp_route: RUN: commit` line) ->
    `RUNNER --peek RUN progress`; '' while it has no run id / no progress file / the runner cannot peek"""
    try:
        m = RUN_ID_RE.search(open(log_path, errors='replace').read())
        if not m:
            return ''
        r = subprocess.run(runner + ['--peek', m.group(1), 'progress'], capture_output=True, text=True, timeout=60)
        lines = [l for l in r.stdout.splitlines() if l.strip()]
        return lines[-1][:120] if r.returncode == 0 and lines else ''
    except (OSError, subprocess.SubprocessError):
        return ''


def cmd_submit(a, ad):
    """jobs through the runner: `--shards bd:3,bd:3,laptop:1` = MACHINE[:WEIGHT] per job. The jobs of one machine share ONE slice of
    the case list (the machine's weights summed) and ONE dynamic queue (each instance takes the next unit: no straggler holds a
    fixed chunk); the slices of different machines are split by weight (a fast machine ~3x a slow one). The merged table = the BSROW lines"""
    os.makedirs(a.out, exist_ok=True)
    runner = shlex.split(os.environ.get('BATTLE_SWEEP_RUNNER', 'chain_slot.sh'))
    shards = drop_off([s.split(':') for s in a.shards.split(',')])
    if not shards:
        print('no shard left (every machine of --shards is in BATTLE_SWEEP_OFF): use e.g. --shards laptop:1')
        return 2
    groups = collections.OrderedDict()
    for s in shards:
        groups.setdefault(s[0], []).append(float(s[1]) if len(s) > 1 else 1.0)
    names = list(groups)
    weights = ','.join(f'{sum(groups[m]):g}' for m in names)
    rid = time.strftime('%m%d%H%M%S') + f'{os.getpid() % 1000:03d}'   # two submits in one second must not share out dirs or queues
    base = a.remote_out or '/tmp'
    procs = []
    t0 = time.time()
    for g, m in enumerate(names, 1):
        for k, w in enumerate(groups[m], 1):
            tag = f'{rid}-{g}{chr(96 + k)}'
            inner = (f'python3 tools/battle_sweep.py run --adapter {a.adapter} --game {a.game} --what {a.what} --ids {a.ids} -j {a.j} '
                     f'--batch {a.batch} --case-frames {a.case_frames} --watchdog {a.watchdog} --shard {g}/{len(names)} --weights {weights} '
                     f'--queue /tmp/bsq-{rid}-{g} --out {base}/bs-{tag} --redo --rows' + (' --isolate' if a.isolate else '')
                     + (f' --pairs {a.pairs}' if a.pairs else '') + (' --keep-work' if a.keep_work else ''))
            cmd = runner + ['--commit', a.commit] + list(getattr(ad, 'RUNNER_ARGS', ['--hg', '--family', 'hgss', '--no-port'])) \
                + ['--on', m, '--wait', str(a.wait), '--cmd', inner]
            log = open(os.path.join(a.out, f'shard-{g}{chr(96 + k)}.log'), 'w')
            procs.append((f'{g}{chr(96 + k)}', m, subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT, cwd=a.cwd or None), log))
            print(f'job {g}{chr(96 + k)} on {m} (slice {g}/{len(names)}, machine weight {sum(groups[m]):g}): started', flush=True)
    table = Table(a.out)
    open(table.path, 'w').write('\t'.join(HEAD) + '\n')
    tp = time.time()
    for tag, m, p, log in procs:
        while a.progress:   # --progress SECS: one line per live shard every SECS (each shard's own `[Ns] n/N cases` line, via the runner's --peek)
            try:
                rc = p.wait(timeout=max(1, tp + a.progress - time.time()))
                break
            except subprocess.TimeoutExpired:
                tp = time.time()
                for t2, m2, p2, _ in procs:
                    if p2.poll() is None:
                        print(f'progress {t2} on {m2}: {peek_progress(runner, os.path.join(a.out, f"shard-{t2}.log")) or "(no progress yet)"}', flush=True)
        else:
            rc = p.wait()
        log.close()
        n = 0
        for line in open(os.path.join(a.out, f'shard-{tag}.log'), errors='replace'):
            if line.startswith('BSROWS '):
                rows = decode_rows(line)
                if not rows:
                    print(f'job {tag} on {m}: a BSROWS line could not be decoded (cut or mixed with noise); see shard-{tag}.log', flush=True)
                for row in rows:
                    open(table.path, 'a').write(row + '\n')
                    n += 1
        print(f'job {tag} on {m}: rc {rc}, {n} rows' + ('   <- NO ROWS: its machine had no free slot or the job died (shard-' + tag + '.log)' if not n else ''), flush=True)
    print(summarize(a, ad))
    print(f'wall {time.time() - t0:.0f} s')
    return 0


# ---------------------------------------------------------------- report (ranked markdown)

def case_label(ad, key):
    """'m71-304' -> 'move 71 (NAME) vs foe move 304 (NAME)'; 'f16' -> 'foe species 16 (NAME)' (constants names from the adapter's names())"""
    mv, sp = ad.names() if hasattr(ad, 'names') else ({}, {})
    m = re.match(r'[mip](\d+)-(\d+)(?:@(\d+)L(\d+))?$', key)
    if m:
        t = f'our move {m.group(1)} {mv.get(int(m.group(1)), "?")} vs foe move {m.group(2)} {mv.get(int(m.group(2)), "?")}'
        return t + (f', foe species {m.group(3)} {sp.get(int(m.group(3)), "?")} L{m.group(4)}' if m.group(3) else '')
    m = re.match(r'([fo])(\d+)$', key)
    if m:
        return f'{"foe" if m.group(1) == "f" else "our"} species {m.group(2)} {sp.get(int(m.group(2)), "?")}'
    return key


def repro_cmd(a, ad, key):
    m = re.match(r'[mip](\d+)-(\d+)(?:@(\d+)L(\d+))?$', key)
    if m:
        pair = f'{m.group(1)}:{m.group(2)}' + (f'@{m.group(3)}/{m.group(4)}' if m.group(3) else '')
        return f'python3 tools/battle_sweep.py run --adapter {a.adapter} --game {a.game} --pairs {pair} -j 1 --out /tmp/bs-repro --redo'
    m = re.match(r'([fo])(\d+)$', key)
    if m:
        return (f'python3 tools/battle_sweep.py run --adapter {a.adapter} --game {a.game} --what species-{"foe" if m.group(1) == "f" else "our"} '
                f'--ids {m.group(2)} -j 1 --out /tmp/bs-repro --redo')
    return ''


def cmd_report(a, ad):
    """ranked markdown of a results table: crashes and hangs first (a group = one backtrace id), cases with names, culprit side,
    the top 3 frames, a repro command; then the frame-cap / other rows and the ok count"""
    rows = load_rows(a.out)
    bad = [r for r in rows if r['outcome'] not in ('ok', 'ok-end')]
    rank = {'crash': 0, 'heap': 1, 'hang': 2, 'cap': 3}
    groups = collections.OrderedDict()
    for r in bad:
        groups.setdefault((r['outcome'], r['bt_id'] or r['top']), []).append(r)
    out = [f'# battle sweep ({getattr(ad, "NAME", "?")}) results', '',
           f'{len(rows)} cases: ' + ', '.join(f'{k} {v}' for k, v in sorted(collections.Counter(r["outcome"] for r in rows).items())), '']
    for (kind, _), rs in sorted(groups.items(), key=lambda kv: (rank.get(kv[0][0], 9), -len(kv[1]))):
        culprits = sorted({r['culprit'] for r in rs if r['culprit']})
        out += [f'## {kind}: {rs[0]["top"] or "?"}  ({len(rs)} cases)', '', f'- backtrace: {rs[0]["backtrace"] or "-"}',
                f'- culprit side: {", ".join(culprits) or "not isolated"}', '- cases:']
        out += [f'  - {r["case"]}: {case_label(ad, r["case"])}' for r in rs[:40]]
        if len(rs) > 40:
            out.append(f'  - ... {len(rs) - 40} more')
        out += [f'- repro: `{repro_cmd(a, ad, rs[0]["case"])}`', '']
    fr = sorted(int(r['frames']) for r in rows if r['outcome'] == 'ok' and str(r['frames']).isdigit())
    if fr:
        out.append(f'ok cases V-blanks: median {fr[len(fr) // 2]}, 90% {fr[int(len(fr) * .9)]}, max {fr[-1]}')
    text = '\n'.join(out) + '\n'
    if a.md:
        open(a.md, 'w').write(text)
    print(text)
    return 0


# ---------------------------------------------------------------- main

def load_adapter(name):
    return importlib.import_module(f'battle_sweep_{name}')


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('mode', choices=['run', 'submit', 'summary', 'report'])
    ap.add_argument('--adapter', default='hg', help='tools/battle_sweep_<name>.py')
    ap.add_argument('--game', default='heartgold', help='the adapter\'s game (hg: heartgold | soulsilver)')
    ap.add_argument('--what', choices=KINDS, default='moves')
    ap.add_argument('--ids', default='all', help='all | 1-40,151 (move or species numbers)')
    ap.add_argument('--out', required=True)
    ap.add_argument('-j', type=int, default=int(os.environ.get('NTR_SLOT_JOBS', '4')), help='instances at once')
    ap.add_argument('--batch', type=int, default=12, help='cases per battle (moves)')
    ap.add_argument('--case-frames', type=int, default=8000, help='V-blanks one case may take before it is a hang')
    ap.add_argument('--pulse-to', type=int, default=0, help='frame cap of an instance (default: from --batch and --case-frames, <= 150000)')
    ap.add_argument('--save')
    ap.add_argument('--shard', help='I/N')
    ap.add_argument('--weights')
    ap.add_argument('--isolate', action='store_true')
    ap.add_argument('--heapcheck')
    ap.add_argument('--redo', action='store_true')
    ap.add_argument('--pairs', help='moves: single turns OUR:FOE[@SPECIES/LEVEL],.. (a chosen foe species), instead of --ids')
    ap.add_argument('--queue', help='the work queue directory shared by several jobs on one host (default: out/queue, this job only)')
    ap.add_argument('--watchdog', type=int, default=90, help='the harness watchdog: wall seconds without a new V-blank = hang (0: off)')
    ap.add_argument('--port-hang', type=int, default=30, help="the stall rule of port_run (--hang): wall seconds without a V-blank")
    ap.add_argument('--max-log', type=int, default=300, help='the watchdog also ends an instance whose log is over this many MB')
    ap.add_argument('--rows', action='store_true', help='print BSROW lines (a shard run through a remote job runner)')
    ap.add_argument('--keep-work', action='store_true', help='a --rows shard keeps its /tmp/bs-* work dir (default: removed when the shard ends)')
    ap.add_argument('--shards', default='local:1', help='submit: MACHINE:WEIGHT,...')
    ap.add_argument('--commit', default='HEAD')
    ap.add_argument('--wait', type=int, default=900)
    ap.add_argument('--cwd')
    ap.add_argument('--md', help='report: write the markdown here')
    ap.add_argument('--progress', type=int, default=0, metavar='SECS', help='submit: print each live shard\'s progress line every SECS')
    ap.add_argument('--remote-out', help="submit: each job keeps its out dir under this path on its machine (default /tmp)")
    a = ap.parse_args()
    ad = load_adapter(a.adapter)
    ad.configure(a)
    if not a.pulse_to:
        a.pulse_to = min(150000, ad.PULSE_FROM + (a.batch if a.what == 'moves' else 1) * a.case_frames + 4000)
    if a.mode == 'report':
        return cmd_report(a, ad)
    if a.mode == 'summary':
        print(summarize(a, ad))
        return 0
    return cmd_run(a, ad) if a.mode == 'run' else cmd_submit(a, ad)


if __name__ == '__main__':
    sys.exit(main())
