#!/usr/bin/env python3
"""tools/hgss_repin.py: re-pin third_party/pokeheartgold (HeartGold, docs/hgss.md "Re-pin") to a new hgss-port commit in
one command, fast when little changed. Run by tools/hgss_repin.sh HASH; the steps, their times and what they found go
to build/hgss_repin/summary.txt. The decomp's commits come from a clone of it: $HGSS_DECOMP (default build/hgss-decomp,
fetched from its origin when HASH is not there); third_party/pokeheartgold is the vendored tree (tools/hg_vendor.py, its
PIN the old commit), made again from the clone at HASH by the commit step. Exit code:
    0  pinned, built, the boot run as good as before: committed (the vendored tree + patches)
    2  a zhostfix hunk no longer applies (files + hunks listed; fix the patch, run again)
    3  build errors (listed), or the typedef check found some
    4  the boot run got worse (it ends at an earlier frame; the first frame its trace differs at), or the test battle
       (BWPORT_HG_TESTBATTLE to the first move menu) passed before and does not now
    1  the script itself failed (a command's output is printed), or a boot run hit its --secs watchdog (no result: the host
       was too slow or the run is stuck; run again with a bigger --secs)

1. The boot run's baseline: the exe as it is (HEAD's pin), unless build/hgss_repin/base/ has it for that pin.
1b. Handovers (hg39): hand-patch blocks tagged `Handover-ID: X` whose X landed upstream (OLD..HASH's `Handover: X`
   trailers, HANDOVER_TSV's backfill rows) are dropped whole; partly landed ones go hunk by hunk where they fail.
2. The submodule to HASH. The three generated patches (widepointers, widepointers2, wideprotos: pokehg_widen.py --tsv,
   --casts, pokehg_protos.py) moved onto HASH as they are (HEAD's): a file whose hunks no longer apply is dropped from all
   three (taken as pret has it) and made again by step 3. zhostfix (by hand) is left aside meanwhile.
3. The generators again on that tree; they leave what they did already, so they add only what is new or dropped:
   widen --tsv; clang's analysis compile (pokehg_cc's args) of only the C a changed file reaches (the last build's
   `ninja -t deps`; all of it without one) and widen --casts on those logs; meson reconfigure; protos --write --chains.
   What they change goes into wideprotos.
4. zhostfix per file on the result (git apply --check). Then zzabi made again (pokehg_abi.py) with the patches that sort
   after it aside (post_abi()); those are made again on the stack through zzabi (zzzdecl: hg_declscan.py --emit-fixes)
   or put back as they are.
5. pokehg_gen.py with the mtimes of before step 2 kept on the files whose content is the same (ninja rebuilds only what
   changed; ccache has the rest); the typedef check; ninja.
6. The boot run (tools/port_run.py, tools/hgss_repin_keys.txt, BWPORT_HGTRACE) against the baseline, the test battle
   (battle()), then the commit.

The boot runs end on a GAME FRAME (port_run --frames N: the exe ends itself at V-blank N), never on a wall time: a loaded host
reached fewer frames in the old `-t` seconds and the run read as worse. The baseline and the new run both go to --frames
(default FRAMES; the new one to the baseline's end frame when that is later), or end earlier where the game crashes or hangs;
they are compared at those frames (the end frame, and the traces up to it). --secs is only the watchdog: a run that hit it
is no result (exit 1), not a worse one.
"""
import argparse, glob, json, os, re, shutil, signal, subprocess, sys, time, traceback

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pokegen4 as G4  # noqa: E402
import hg_unitcache as UC  # noqa: E402

ROOT = G4.ROOT
SUB = 'third_party/pokeheartgold'
W = os.path.join(ROOT, 'build/pokeheartgold')
BUILD = G4.hgss_build()   # build/port/linux-hgss, or the all-games default build/port/linux (HGSS_BUILD names another)
OUT = os.path.join(ROOT, 'build/hgss_repin')
GENERATED = ['widepointers', 'widepointers2', 'wideprotos']
DECOMP = os.path.abspath(os.environ.get('HGSS_DECOMP', os.path.join(ROOT, 'build/hgss-decomp')))
HAND = 'zhostfix'
FSVIEW = 'zfsview'      # tools/pokehg_fsview.py's diff on the tree with every other patch (hg23)
LATE = [HAND, FSVIEW]   # aside while the stack moves and the generators run (with late_names()'s hand patches)
ABI = 'zzabi'           # tools/pokehg_abi.py's patch; the ones that sort after it are made against it (post_abi())
REGEN = {'zzzdecl': 'declscan'}   # post-abi patches made again by a tool (Repin.<method>), the others are re-applied
KEYS = os.path.join(ROOT, 'tools/hgss_repin_keys.txt')
JOBS = os.environ.get('BW_JOBS', '8')
FAILED_FILE = re.compile(r'^error: (\S+): ', re.M)      # git apply's "error: PATH: patch does not apply" and the like
FRAME = re.compile(r'(?:reached-frame|exit-frame|frame) (\d+)')
RAW_MAIN = re.compile(r'\b0x02[0-9A-Fa-f]{6}\b')   # a raw DS main-RAM address (what dsaddr.patch names)
FRAMES = 60000          # the boot run's end V-blank (the room is drawn from ~f16450, the key script ends at f19950)
BATTLE = '155:5'        # the test battle (port/game/route_gen4.c Pr4_TestBattle): our Cyndaquil L5 against a wild one
BATTLE_FRAMES = 24000   # its frame cap (the battle starts at the room's first free field frame)
BATTLE_PULSE = 16000    # A every 20 V-blanks from here on (the battle's messages, FIGHT)


class Killed(Exception):
    """SIGTERM / SIGHUP (a watchdog's timeout, a closed ssh): run()'s finally puts the aside patches back"""


def killed(sig, _frame):
    raise Killed('killed by signal %d' % sig)


def missing_patches(root):
    """the committed port/subprojects files the work tree lacks (an earlier run killed while they were aside)"""
    return subprocess.run(['git', '-C', root, 'ls-files', '--deleted', '--', 'port/subprojects'], stdout=subprocess.PIPE,
                          text=True, check=True).stdout.split()


def late_names():
    """LATE and every other patch that sorts after the generated ones (zheapalign, zversion): aside while the generators
    run, as fold() diffs W from wideprotos' base to now and would take their changes into wideprotos (hg32: zheapalign's
    heap.c and zversion's files were folded in, then zheapalign no longer applied after them)."""
    names = sorted(os.path.basename(f)[len('pokeheartgold-'):-len('.patch')] for f in glob.glob(patch_path('*')))
    return [n for n in names if n in LATE or n > max(GENERATED)]


def post_abi():
    """The patches that sort after zzabi (made against the pin + every patch before them, zzabi included): aside while
    abi() makes zzabi again, then made again (REGEN) or put back, and the tree generated with them (rp3: zzzdecl stayed in
    place, pokehg_abi.py's checkout applied it without zzabi and 150 of its 156 files failed)."""
    names = {os.path.basename(f)[len('pokeheartgold-'):-len('.patch')] for f in glob.glob(patch_path('*'))}
    return sorted(n for n in names | set(REGEN) if n > ABI)


def patch_path(n):
    return os.path.join(ROOT, 'port/subprojects', 'pokeheartgold-%s.patch' % n)


def split_patch(text):
    """(the header before the first file, {path: that file's diff}) of a git patch: split_blocks() with the pieces of
    one path joined (in order)."""
    head, blocks = split_blocks(text)
    files = {}
    for p, b in blocks:
        files[p] = files.get(p, '') + b
    return head, files


def split_blocks(text):
    """(the header before the first file, [(path, block)] in patch order) of a git patch. A block starts at its
    'diff --git' line, or at a bare '--- a/PATH' + '+++ ' pair with no 'diff --git' before it (hand-appended pieces:
    zhostfix's hgr2/hg35 crash fixes, each after a prose line); the prose since the last hunk line goes with it.
    Before hg36 (11-15) such pieces counted as part of the file above them: --zdrop of their own path did nothing, and
    zdropping the file above dropped them too. Two pieces of one path stay two blocks, each with its own `Handover-ID:`
    lines (right above its 'diff --git' or bare '---': hg39, block_ids()); rpz1 11-15: joined, a landed or --zdrop'd
    first piece took the second one with it (ov12_0225B960's hgr4-0001 hunk: every battle crashed at the send-out)."""
    lines = text.splitlines(True)
    starts, last_hunk, in_hdr, body_end = [], -1, False, -1
    for i, l in enumerate(lines):
        if l.startswith('diff --git '):
            starts.append((tag_start(lines, i, starts, body_end), re.match(r'diff --git a/(\S+) b/', l).group(1)))
            in_hdr, body_end = True, i
            continue
        if l.startswith('--- ') and i + 1 < len(lines) and lines[i + 1].startswith('+++ '):
            if not in_hdr:
                path = re.match(r'--- (?:a/)?(\S+)', l).group(1)
                if path == '/dev/null':
                    path = re.match(r'\+\+\+ (?:b/)?(\S+)', lines[i + 1]).group(1)
                starts.append((last_hunk + 1 if 0 <= last_hunk and (not starts or last_hunk >= starts[-1][0])
                               else tag_start(lines, i, starts), path))
            in_hdr = False
            continue
        if l.startswith('@@'):
            in_hdr = False
        if not in_hdr and (l[:1] in (' ', '+', '-', '\\', '@') or l == '\n'):
            last_hunk = i
        if in_hdr or last_hunk == i:
            body_end = i
    if not starts:
        return text, []
    blocks = [(p, ''.join(lines[s:starts[k + 1][0] if k + 1 < len(starts) else len(lines)]))
              for k, (s, p) in enumerate(starts)]
    return ''.join(lines[:starts[0][0]]), blocks


def block_hunks(block):
    """(a block's text before its first hunk, [its hunks])"""
    m = re.search(r'(?m)^@@', block)
    if not m:
        return block, []
    return block[:m.start()], [h for h in re.split(r'(?m)^(?=@@)', block[m.start():]) if h.startswith('@@')]


def hunk_line(h):
    return int(re.match(r'@@ -(\d+)', h).group(1))


def zdrop(text, specs):
    """A hand patch without what --zdrop names (rpz1, 11-15: per hunk or per handover, not per file). SPEC: PATH:LINE =
    the hunk of PATH whose old side starts at LINE (what a failing zcheck prints as ZDROP, the watcher's retry), a
    Handover-ID = every block tagged with it, PATH = every block of PATH (by hand only: it takes the still-applying
    blocks of PATH too). (the new text, ['PATH @@-LINE (IDS)' or 'PATH block (IDS)' for each thing dropped], [the
    specs that matched nothing])."""
    head, blocks = split_blocks(text)
    default, specs, out, gone, used = block_ids(head), set(specs), [], [], set()
    for p, b in blocks:
        ids = block_ids(b, default) or []
        tag = ' (%s)' % (','.join(ids) if ids else 'no Handover-ID')
        whole = {s for s in specs if s == p or s in ids}
        if whole:
            used |= whole
            gone.append('%s block%s' % (p, tag))
            continue
        pre, hs = block_hunks(b)
        keep = [h for h in hs if '%s:%d' % (p, hunk_line(h)) not in specs]
        for h in hs:
            if h not in keep:
                used.add('%s:%d' % (p, hunk_line(h)))
                gone.append('%s @@-%d%s' % (p, hunk_line(h), tag))
        if keep or not hs:
            out.append(pre + ''.join(keep))
    return head + ''.join(out), gone, sorted(specs - used)


def tag_start(lines, i, starts, body_end=None):
    """i, moved up over the `Handover-ID:` lines right above it (not into the file before). Given body_end (a 'diff --git'
    block: the last header or hunk line of the block before it), over the block's whole paragraph instead: the non-blank
    lines right above it, back to body_end or a blank line (r2fx 11-18: a block a re-pin, a landing or --zdrop drops takes
    its paragraph with it; before, the paragraph was the tail of the block above and stayed: rev2 found zhostfix's hg36,
    hbs2 and cut1 paragraphs in front of other blocks). Above the first block only when a blank line parts it from the
    patch head (a head written straight into its first block keeps its prose, as before)."""
    lo = starts[-1][0] + 1 if starts else 0
    if body_end is not None:
        k, top = i, body_end + 1 if starts else 0
        while k > top and lines[k - 1].strip():
            k -= 1
        if starts or k > 0:
            return k
    while i > lo and lines[i - 1].startswith('Handover-ID:'):
        i -= 1
    return i


# Handover IDs (hg39, 11-15): every hand-patch block says which handover to admin-79 it is (`Handover-ID: hg36-0001`, a
# comma list, or `Handover-ID: none (port-only)`); upstream marks the commit that lands one with a trailer `Handover: ID`
# (`Handover: ID partial`), and the older ones are in admin-79's backfill list (HANDOVER_TSV). A block whose IDs all
# landed whole is dropped whole before the rebase; one with a partial landing is tried hunk by hunk (prune()).
HANDOVER_TAG = re.compile(r'(?m)^Handover-ID:[ \t]*(.*)$')
HANDOVER_ID = re.compile(r'^[a-z]+\d+-[\w.-]+$')
TRAILER = re.compile(r'(?mi)^Handover:[ \t]*(\S+)(?:[ \t]+(partial))?')
HANDOVER_TSV = 'port/hgss_handover_landed.tsv'


def block_ids(body, default=None):
    """The handover IDs of a block (its `Handover-ID:` lines before its first hunk); `default` without any such line
    (the patch head's IDs: a patch that is one handover says it once); [] for `none`."""
    m = re.search(r'(?m)^@@', body)
    tags = HANDOVER_TAG.findall(body[:m.start()] if m else body)
    if not tags:
        return default
    return [x for t in tags for x in re.split(r'[,\s]+', t.strip()) if HANDOVER_ID.match(x)]


def parse_trailers(log):
    """{id: 'full' | 'partial'} of `git log --format=%B`'s `Handover:` trailers (a full landing wins)."""
    out = {}
    for hid, part in TRAILER.findall(log):
        hid = hid[:-6] if hid.endswith('.patch') else hid
        if out.get(hid) != 'full':
            out[hid] = 'partial' if part else 'full'
    return out


def parse_tsv(text):
    """[(id, status, coverage, landing commit)] of admin-79's backfill list (tab-separated, '#' comments)."""
    rows = []
    for line in text.splitlines():
        f = line.rstrip('\r').split('\t')
        if len(f) >= 4 and not f[0].startswith('#'):
            rows.append((f[0][:-6] if f[0].endswith('.patch') else f[0], f[1].strip(), f[2].strip(), f[3].strip()))
    return rows


def landing(ids, landed):
    """'full' when every ID of a block landed whole, 'partial' when some landed (or partly), None when none did. A landed
    key matches a tag when it is the tag or the tag plus '-...' (the backfill names hg37's by file: 'hg37-0001-ov01_...')."""
    st = []
    for t in ids or []:
        hit = [v for k, v in landed.items() if k == t or k.startswith(t + '-')]
        st.append('full' if 'full' in hit else 'partial' if hit else None)
    if not st or all(x is None for x in st):
        return None
    return 'full' if all(x == 'full' for x in st) else 'partial'


def drop_landed(text, landed):
    """(text without the blocks whose handovers all landed whole, or None when nothing is left; {path: 'full' | 'partial'},
    'partial' when any block of the path landed partly). Block by block (rpz1): two pieces of one path, two handovers."""
    head, blocks = split_blocks(text)
    default = block_ids(head)
    keep, got = [], {}
    for f, body in blocks:
        st = landing(block_ids(body, default), landed)
        if st and got.get(f) != 'partial':
            got[f] = st
        if st != 'full':
            keep.append(body)
    if not keep:
        return None, got
    return head + ''.join(keep), got


def parse_deps(text, builddir, work):
    """{source: [headers]} (paths relative to work, the ones outside it left out) of `ninja -t deps`'s output."""
    out, cur, rel = {}, None, {}
    for line in text.splitlines():
        if not line.strip():
            cur = None
        elif not line[0].isspace():
            cur = []
        elif cur is not None:
            d = line.strip()
            p = rel.get(d) or rel.setdefault(d, os.path.relpath(os.path.normpath(os.path.join(builddir, d)), work))
            if not cur:
                out[p] = cur
            cur.append(p)
    return {s: [d for d in ds[1:] if not d.startswith('..')] for s, ds in out.items() if not s.startswith('..')}


def affected(changed, deps, sources):
    """The sources to compile again: the changed ones, the ones a changed header reaches, the ones deps lacks."""
    return sorted(s for s in sources if s in changed or s not in deps or any(d in changed for d in deps[s]))


def end_frame(verdict):
    m = FRAME.findall(verdict)
    return int(m[-1]) if m else 0


def slow(verdict):
    """The boot run hit the watchdog (port_run says `watchdog: frame F of N`): no result, a slow host or a stuck run."""
    return verdict.startswith('watchdog')


def upto(trace, frame):
    """The trace lines of V-blank FRAME or before (a line starts with the host's V-blank count)."""
    out = []
    for line in trace:
        w = line.split(None, 1)
        if w and w[0].isdigit() and int(w[0]) > frame:
            break
        out.append(line)
    return out


def first_diff(a, b):
    """The first line of trace a that b differs at (None: one is the other's start)."""
    for x, y in zip(a, b):
        if x != y:
            return x
    return None


def compare(base_v, new_v, base_tr, new_tr):
    """(worse, one line) for the boot run's verdict and trace lines against the baseline's."""
    b, n = end_frame(base_v), end_frame(new_v)
    d = first_diff(upto(base_tr, min(b, n)), upto(new_tr, min(b, n)))   # the traces to the frame both runs reached
    div = 'traces the same to frame %d' % min(b, n) if d is None else 'traces differ from V-blank %s' % d.split()[0]
    if n < b:
        return True, 'WORSE: ends at frame %d, before %d (%s): %s' % (n, b, div, new_v)
    same = re.sub(r'\d+ s|\([\d.]+ fps\)', '', base_v) == re.sub(r'\d+ s|\([\d.]+ fps\)', '', new_v)
    return False, ('same end as before' if same else 'ends later or elsewhere (a NEW end, look at it)') + \
        ' at frame %d (before: %d; %s): %s' % (n, b, div, new_v)


class Repin:
    def __init__(self, new, secs, worker, zdrop=(), frames=FRAMES, battle_frames=BATTLE_FRAMES):
        self.secs, self.worker, self.zdrop, self.lines, self.t0 = secs, worker, set(zdrop), [], time.time()
        self.frames, self.battle_frames = frames, battle_frames
        self.hdrop, self.partial, self.tsv_path = {}, {}, os.path.join(ROOT, HANDOVER_TSV)
        os.makedirs(OUT, exist_ok=True)
        self.old = open(os.path.join(ROOT, SUB, 'PIN')).read().strip()
        sub = DECOMP
        if not os.path.isdir(os.path.join(sub, '.git')):
            raise SystemExit('hgss_repin: no decomp clone at %s (clone it there, or set HGSS_DECOMP)' % sub)
        if subprocess.call(['git', '-C', sub, 'cat-file', '-e', new + '^{commit}'], stderr=subprocess.DEVNULL):
            self.sh(['git', '-C', sub, 'fetch', '-q', 'origin'])
        self.new = self.git(sub, 'rev-parse', new + '^{commit}').strip()

    # helpers
    def sh(self, cmd, log=None, check=True, env=None, cwd=ROOT):
        r = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL,
                           text=True, errors='replace', env=env)
        if log:
            open(os.path.join(OUT, log), 'w').write(r.stdout)
        if check and r.returncode:
            raise RuntimeError('exit %d: %s\n%s' % (r.returncode, ' '.join(cmd)[:300], r.stdout[-3000:]))
        return r

    def git(self, where, *a, check=True):
        return self.sh(['git', '-C', where] + list(a), check=check).stdout

    def say(self, s):
        self.lines.append(s)
        print(s, flush=True)

    def step(self, name, f, *a):
        t = time.time()
        r = f(*a)
        self.say('%-10s %5.0f s  %s' % (name, time.time() - t, r if isinstance(r, str) else ''))
        return r

    def diff(self, a, b):
        return self.git(W, 'diff', '-U2', a, b, '--', '*.c', '*.h')

    def write_patch(self, name, body):
        p = patch_path(name)
        text = self.heads[name] + body
        if open(p).read() != text:
            open(p, 'w').write(text)

    def state(self):
        self.git(W, 'add', '-u')
        return self.git(W, 'write-tree').strip()

    def mark(self):
        """The stamp pokehg_gen.py would write for the patches as they are now (W is them applied), so that meson's
        run of it leaves W alone."""
        patches = sorted(glob.glob(patch_path('*')))
        open(os.path.join(W, '.bwport-stamp'), 'w').write(G4.stamp(self.new, patches))

    def fold(self):
        """W's changes since widepointers2 -> wideprotos; the files they touched."""
        t = self.state()
        self.write_patch('wideprotos', self.diff(self.trees['wideprotos'][0], t))
        self.mark()
        names = set(self.git(W, 'diff', '--name-only', self.cur, t).split())
        self.cur = t
        return names

    # steps
    def boot(self, d, frames):
        """The boot run to V-blank `frames` (port_run --frames), --secs the watchdog; the verdict line."""
        d = os.path.join(OUT, d)
        # not rmtree(d): the test battle runs at the same time and keeps its own files in d/battle (rp15: its makedirs came
        # between this rmtree and the makedirs below: FileExistsError, or the battle's run dir deleted under it)
        shutil.rmtree(os.path.join(d, 'run'), ignore_errors=True)
        for f in ('trace.txt', 'run.txt', 'verdict.txt'):
            if os.path.exists(os.path.join(d, f)):
                os.remove(os.path.join(d, f))
        os.makedirs(d, exist_ok=True)
        keys = open(KEYS).read().replace('\r', '').replace('\n', '')
        env = dict(os.environ, BWPORT_HGTRACE=os.path.join(d, 'trace.txt'))
        r = self.sh([sys.executable, 'tools/port_run.py', '--game', 'heartgold', '--no-build', '--fresh', '--skip', '4',
                     '--frames', str(frames), '-t', str(self.secs), '--out', os.path.join(d, 'run'), '--keys', keys],
                    check=False, env=env)
        open(os.path.join(d, 'run.txt'), 'w').write(r.stdout)
        v = ([x for x in r.stdout.splitlines() if x.strip()] or ['no output'])[-1]
        open(os.path.join(d, 'verdict.txt'), 'w').write(v + '\n')
        return v

    def battle(self, d):
        """The test battle (rpz1, 11-15: a lost zhostfix battle hunk crashed every send-out, which the boot run never
        reaches): the boot's key script and A every 20 V-blanks from BATTLE_PULSE, BWPORT_HG_TESTBATTLE at the first free
        field frame, the end at the first move menu (BWPORT_HG_TESTSLOT=stop + TESTEXIT), --battle-frames the cap.
        'pass ...' or 'FAIL ...' (also in d/battle.txt); '' with --battle-frames 0."""
        if not self.battle_frames:
            return ''
        d = os.path.join(OUT, d)
        os.makedirs(d, exist_ok=True)
        keys = open(KEYS).read().replace('\r', '').replace('\n', '') + ' ' + ' '.join(
            'a@f%d' % f for f in range(BATTLE_PULSE, self.battle_frames, 20))
        env = dict(os.environ, BWPORT_HG_TESTBATTLE=BATTLE, BWPORT_HG_TESTSLOT='stop', BWPORT_HG_TESTEXIT='1')
        for k in ('BWPORT_PILOT', 'BWPORT_TESTCHEAT', 'BWPORT_HGTRACE'):
            env.pop(k, None)
        out = os.path.join(d, 'battle')
        # a run dir of its own: the boot run (concurrent with this one) and this one each remove and write save.bin there
        r = self.sh([sys.executable, 'tools/port_run.py', '--game', 'heartgold', '--no-build', '--fresh', '--skip', '4',
                     '--shots', '0', '--frames', str(self.battle_frames), '-t', str(self.secs), '--out', out,
                     '--dir', os.path.join(ROOT, 'build/port/run-heartgold-battle'), '--keys', keys], check=False, env=env)
        v = ([x for x in r.stdout.splitlines() if x.strip()] or ['no output'])[-1]
        log = open(os.path.join(out, 'log.txt'), errors='replace').read() if os.path.exists(
            os.path.join(out, 'log.txt')) else ''
        done = re.search(r'^test: moves done.* at V-blank (\d+)', log, re.M)
        start = re.search(r'^test: wild battle.* at V-blank (\d+)', log, re.M)
        if done:
            res = 'pass: the battle from V-blank %s, its first move menu at %s' % (start.group(1) if start else '?',
                                                                                  done.group(1))
        elif slow(v):
            raise RuntimeError('the battle run hit the --secs watchdog (%s): no result' % v)
        else:
            res = 'FAIL: %s: %s' % ('in the battle (from V-blank %s)' % start.group(1) if start else 'no battle', v)
        open(os.path.join(d, 'battle.txt'), 'w').write(res + '\n')
        return res

    def baseline(self):
        b = os.path.join(OUT, 'base')
        pin = os.path.join(b, 'pin.txt')
        exe = os.path.exists(os.path.join(ROOT, BUILD, 'bwport'))
        if os.path.exists(pin) and open(pin).read().strip() == self.old:
            v = 'kept: ' + open(os.path.join(b, 'verdict.txt')).read().strip()
            bt = os.path.join(b, 'battle.txt')
            if os.path.exists(bt):
                return v + '; battle ' + open(bt).read().strip()
            return v + ('; battle ' + self.battle('base') if exe and self.battle_frames else '')
        if not exe:
            return 'no exe: no baseline'
        v = self.boot('base', self.frames)
        if slow(v):
            raise RuntimeError('the baseline boot run hit the --secs watchdog (%s): no baseline; run again with a bigger '
                               '--secs' % v)
        bt = self.battle('base')
        open(os.path.join(b, 'pin.txt'), 'w').write(self.old + '\n')
        return v + ('; battle ' + bt if bt else '')

    def rebase(self):
        sub = DECOMP
        # from here pokehg_gen.py (this script's runs and meson's) makes W from the clone at the new pin, not from the
        # vendored tree (the old pin's until commit())
        os.environ.update(HGSS_SRC=DECOMP, HGSS_REV=self.new)
        # W stops being what its stamp says from here: without the stamp, a failed run (exit 2 on a hand patch) leaves a
        # tree pokehg_gen.py makes again, not one it takes for the old pin's (lp_route's next build in this slot, 11-15)
        st = os.path.join(W, '.bwport-stamp')
        if os.path.exists(st):
            os.remove(st)
        if subprocess.call(['git', '-C', W, 'cat-file', '-e', self.new + '^{commit}'], stderr=subprocess.DEVNULL):
            self.git(W, 'fetch', '-q', sub, self.new)
        # every patch in name order, as pokehg_gen.py applies them (zhostfix is aside): the generated ones HEAD's, the
        # hand ones (compile, dsaddr, widepointers3) as they are in the worktree
        order = sorted(os.path.basename(f)[len('pokeheartgold-'):-len('.patch')] for f in glob.glob(patch_path('*')))
        self.heads, ins = {}, {}
        for n in order:
            ins[n] = patch_path(n)
            if n in GENERATED:
                text = self.git(ROOT, 'show', 'HEAD:' + os.path.relpath(patch_path(n), ROOT))
                self.heads[n] = split_patch(text)[0]
                ins[n] = os.path.join(OUT, n + '.head.patch')
                open(ins[n], 'w').write(text)
        self.drop, self.hbad, self.hrep = set(), {}, {}
        for _ in range(6):
            self.git(W, 'checkout', '-q', '-f', '--detach', self.new)
            self.git(W, 'clean', '-fdq', '-e', '/gen', '-e', '/hbuild', '-e', '/.bwport-stamp')
            trees, bad = {}, set()
            for n in order:
                ex = []
                if not os.path.exists(ins[n]):     # a hand patch hand_upstream() emptied
                    continue
                while True:
                    if n in GENERATED:
                        ex = ['--exclude=' + f for f in sorted(self.drop | bad)]
                    r = self.sh(['git', '-C', W, 'apply', '--check', '--index', '--whitespace=nowarn'] + ex + [ins[n]],
                                check=False)
                    if not r.returncode:
                        break
                    if n not in GENERATED and self.hand_upstream(n, r.stdout):
                        if not os.path.exists(ins[n]):
                            break
                        continue
                    if n not in GENERATED:          # by hand: a person (or agent) moves it
                        self.hbad[n] = [x for x in r.stdout.splitlines() if x.startswith('error')]
                        self.hrep[n] = self.hand_report(n)
                        return 'hand patch %s does not apply' % n
                    more = set(FAILED_FILE.findall(r.stdout)) - bad - self.drop
                    if not more:
                        raise RuntimeError('git apply %s: %s' % (n, r.stdout[-2000:]))
                    bad |= more
                if not os.path.exists(ins[n]):
                    continue
                before = self.git(W, 'write-tree').strip()
                self.sh(['git', '-C', W, 'apply', '--index', '--whitespace=nowarn'] + ex + [ins[n]])
                trees[n] = (before, self.git(W, 'write-tree').strip())
            if not bad - self.drop:
                break
            self.drop |= bad
        self.trees = trees
        for n in GENERATED:
            self.write_patch(n, self.diff(*trees[n]))
        open(os.path.join(OUT, 'dropped.txt'), 'w').write(''.join(f + '\n' for f in sorted(self.drop)))
        return '%d files dropped from the generated patches (dropped.txt): %s' % (
            len(self.drop), ' '.join(sorted(self.drop)[:6]) + (' ...' if len(self.drop) > 6 else ''))

    def handovers(self):
        """Drops the hand-patch blocks whose handovers landed upstream between the pins (the `Handover:` trailers of
        OLD..NEW, and the backfill list's landed rows whose commit NEW contains) whole; remembers the partly landed ones
        for prune(). Before the rebase, so no hunk-by-hunk guessing for them (hg39: hk488 took widepointers3 whole, and
        its hunks that still applied would have doubled the particle area a second time)."""
        sub = DECOMP
        landed = parse_trailers(self.git(sub, 'log', '--format=%B', '%s..%s' % (self.old, self.new)))
        n_tr = len(landed)
        if os.path.exists(self.tsv_path):
            for hid, status, cov, commit in parse_tsv(open(self.tsv_path).read()):
                if status != 'landed' or not re.match(r'^[0-9a-f]{7,40}$', commit):
                    continue
                if subprocess.call(['git', '-C', sub, 'merge-base', '--is-ancestor', commit, self.new],
                                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL):
                    continue        # not in NEW (or not known here)
                if landed.get(hid) != 'full':
                    landed[hid] = 'partial' if cov == 'partial' else 'full'
        self.landed = landed
        gone = []
        for p in sorted(glob.glob(patch_path('*'))):
            n = os.path.basename(p)[len('pokeheartgold-'):-len('.patch')]
            if n in GENERATED or n in REGEN:    # REGEN: made again after abi() from the new tree
                continue
            text = open(p).read()
            new, got = drop_landed(text, landed)
            for f, st in got.items():
                if st == 'partial':
                    self.partial[f] = n
            if new is None:
                os.remove(p)
                self.hdrop[n] = 'deleted'
            elif new != text:
                open(p, 'w').write(new)
                self.hdrop[n] = 'kept'
            for f in sorted(f for f, st in got.items() if st == 'full'):
                gone.append('%s:%s' % (n, f))
                self.say('%s: dropped %s (its handover landed)%s' % (n, f, '' if new else '; the patch is empty: deleted'))
        part = ' '.join('%s:%s' % (n, f) for f, n in sorted(self.partial.items()))
        return '%d landed IDs (%d by trailer), %d blocks dropped%s' % (
            len(landed), n_tr, len(gone), '; partly landed (hunk by hunk): ' + part if part else '')

    def hunks(self, body):
        """[('PATH:LINE' (zdrop()'s spec), the block's Handover-IDs, hunk, 'applies' | 'upstream has' | 'FAILS')] of one
        file's diff (each of its blocks with its own header) on W as it is."""
        hdr_line = re.compile(r'^(diff --git |index |--- |\+\+\+ |new file|deleted file|old mode|new mode|similarity|rename )')
        tmp = os.path.join(OUT, 'hand-hunk.patch')
        out = []
        for p, b in split_blocks(body)[1]:
            pre, hs = block_hunks(b)
            hdr = ''.join(x for x in pre.splitlines(True) if hdr_line.match(x))
            for h in hs:
                open(tmp, 'w').write(hdr + h)
                chk = lambda *a: not self.sh(['git', '-C', W, 'apply', '--check', '--whitespace=nowarn'] + list(a) + [tmp],
                                             check=False).returncode
                out.append(('%s:%d' % (p, hunk_line(h)), block_ids(b) or [], h,
                            'applies' if chk() else 'upstream has' if chk('-R') else 'FAILS'))
        return out

    def prune(self, body):
        """A partly landed block on W: (the block without its hunks upstream has, or '' when none is left; None when a
        hunk FAILS: a person reads it)."""
        hs = self.hunks(body)
        if any(v == 'FAILS' for *_, v in hs):
            return None
        return zdrop(body, [s for s, _, _, v in hs if v != 'applies'])[0]

    def hand_report(self, n):
        """Hand patch n, hunk by hunk, on W as it is now (11-15, hg36: hk404 rewrote half of widepointers3's context and
        the run said one FAIL): '  FILE: @@-LINE applies | upstream has | FAILS ...' for each file not wholly applying,
        so a remake redoes only the failing hunks (each still needed, or upstream has it in another form: read it)."""
        _, files = split_patch(open(patch_path(n)).read())
        return [r for f, body in files.items() for r in self.file_report(f, body)]

    def file_report(self, f, body):
        """['FILE: @@LINE verdict, ...'] when a hunk of it does not apply; DROP-WHOLE-FILE-SUSPECT when some apply and
        some upstream has (hg39, 11-15: hk488 took widepointers3's heap.c/particle doubling whole, its other hunks still
        applied and would have doubled twice): upstream most likely took the whole change in its own form."""
        hs = self.hunks(body)
        res = ['@@%s %s' % (s.rsplit(':', 1)[1], v) for s, _, _, v in hs]
        if all(v == 'applies' for *_, v in hs):
            return []
        vs = {v for *_, v in hs}
        flag = (' DROP-WHOLE-FILE-SUSPECT (some hunks upstream has, some still apply: did a handover land it whole? '
                'tag the block Handover-ID / --zdrop it)') if {'applies', 'upstream has'} <= vs else ''
        return ['%s: %s%s' % (f, ', '.join(res), flag)]

    def hand_upstream(self, n, out):
        """A hand patch that does not apply on the new pin: drops the files of it upstream now has (11-15, hg35: hk381 named
        dsaddr.patch's raw addresses itself) and says so; True when every failing file went (the patch is rewritten, or
        deleted when nothing is left). A file goes when the new pin already has the patch's change (it reverse-applies), when
        --zdrop names it, or for dsaddr when the new file has none of the raw main-RAM literals the hunk replaced."""
        path = patch_path(n)
        head, files = split_patch(open(path).read())
        bad = set(FAILED_FILE.findall(out))
        if not bad or not bad <= set(files):
            return False
        why = {}
        for f in sorted(bad):
            tmp = os.path.join(OUT, 'hand-check.patch')
            open(tmp, 'w').write(files[f])
            if not subprocess.call(['git', '-C', W, 'apply', '--check', '-R', '--whitespace=nowarn', tmp],
                                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL):
                why[f] = 'upstream has the change'
            elif f in self.zdrop:
                why[f] = '--zdrop'
            elif self.partial.get(f) == n and self.prune(files[f]) is not None:
                pr = self.prune(files[f])
                if pr:      # the hunks upstream lacks stay
                    files[f] = pr
                    open(path, 'w').write(head + ''.join(files.values()))
                    self.hdrop[n] = 'kept'
                    self.say('%s: %s: partly landed handover: the hunks upstream has dropped' % (n, f))
                    return True
                why[f] = 'partly landed handover: upstream has every hunk'
            elif n == 'dsaddr':
                raw = {m for x in files[f].splitlines() if x.startswith('-') and not x.startswith('---')
                       for m in RAW_MAIN.findall(x)}
                cur = open(os.path.join(W, f), errors='replace').read() if os.path.exists(os.path.join(W, f)) else ''
                if raw and not any(re.search(r'\b%s\b' % r, cur, re.I) for r in raw):
                    why[f] = 'upstream names the raw addresses (%s) itself' % ' '.join(sorted(raw))
            if f not in why:
                return False
        left = {f: b for f, b in files.items() if f not in why}
        if left:
            open(path, 'w').write(head + ''.join(left.values()))
        else:
            os.remove(path)
        self.hdrop[n] = 'kept' if left else 'deleted'
        for f in sorted(why):
            self.say('%s: dropped %s (%s)%s' % (n, f, why[f], '' if left else '; the patch is empty: deleted'))
        return True

    def gen(self):
        self.sh([sys.executable, 'tools/pokehg_gen.py'], log='gen1.log')
        self.cur = self.state()
        return 'stamp ' + open(os.path.join(W, '.bwport-stamp')).read().split()[-1][:12]

    def widen_tsv(self):
        self.sh([sys.executable, 'tools/pokehg_widen.py', '--tsv'], log='widen_tsv.log')
        self.tsv = self.fold()
        return '%d files changed' % len(self.tsv)

    def casts(self):
        import pokehg_cc as C
        changed = set(self.git(DECOMP, 'diff', '--name-only', self.old, self.new).split())
        changed |= self.drop | self.tsv
        snap = json.load(open(os.path.join(OUT, 'mtimes.json')))      # the generated headers (message IDs, .naix)
        changed |= {'gen/' + p for p, (h, t) in G4.snapshot(os.path.join(W, 'gen')).items()
                    if snap.get('gen/' + p, [None])[0] != h}
        t = time.time()
        r = self.sh(['ninja', '-C', BUILD, '-t', 'deps'], check=False)
        deps = parse_deps(r.stdout, os.path.join(ROOT, BUILD), W) if not r.returncode else {}
        srcs = [os.path.relpath(s, W) for s in C.sources()]
        todo = affected(changed, deps, srcs) if deps else srcs
        # the cache only when few units are reached (hg_unitcache.cache_wanted): otherwise the old path, no -M keys
        use_cache = bool(deps) and UC.cache_wanted(len(todo), len(srcs))
        open(os.path.join(OUT, 'unitcache.txt'), 'w').write('%d\n' % use_cache)
        skip = os.path.join(OUT, 'unitcache_skip.txt')
        open(skip, 'w').write('\n'.join(sorted(c for c in changed if c.endswith('.c'))) + '\n')
        cc = os.path.join(OUT, 'cc')
        reuse = os.environ.get('HGSS_REPIN_REUSE_CC')   # a re-run at the same pin and base after exit 2 (rp2): the first run's analysis logs
        if reuse and os.path.exists(os.path.join(reuse, 'summary.txt')) and not os.path.exists(os.path.join(cc, 'summary.txt')):
            shutil.rmtree(cc, ignore_errors=True)
            shutil.copytree(reuse, cc)
            # the logs name the analysed files by the checkout they ran in (<slot checkout>/build/pokeheartgold/...):
            # pokehg_widen.py --casts keeps only paths under this checkout's W, so another slot's logs widened nothing
            # (rp6: "widened 0 files" where the first run widened 3). Their checkout root becomes ours.
            src_root = None
            for dp, _, fn in os.walk(os.path.join(cc, 'log')):
                for f in fn:
                    m = re.search(r'(/\S*?)/build/pokeheartgold/', open(os.path.join(dp, f), errors='replace').read())
                    if m:
                        src_root = m.group(1)
                        break
                if src_root:
                    break
            if src_root and src_root != ROOT:
                for dp, _, fn in os.walk(os.path.join(cc, 'log')):
                    for f in fn:
                        q = os.path.join(dp, f)
                        t = open(q, errors='surrogateescape').read()
                        if src_root + '/' in t:
                            open(q, 'w', errors='surrogateescape').write(t.replace(src_root + '/', ROOT + '/'))
                self.say('casts: HGSS_REPIN_REUSE_CC logs moved from %s to %s' % (src_root, ROOT))
        t1 = time.time()
        hits = 0
        if os.path.exists(os.path.join(cc, 'summary.txt')) and reuse:
            res = [(s, 0, '') for s in todo]
            m = re.search(r'failed (\d+)', open(os.path.join(cc, 'summary.txt')).read())
            res = [(s, 1 if i < int(m.group(1) if m else 0) else 0, '') for i, s in enumerate(todo)]
        else:
            shutil.rmtree(cc, ignore_errors=True)
            if use_cache:
                os.environ['BWPORT_UNITCACHE_SKIP'] = skip
                res, hits = UC.compile_all(W, C.hg_args(), [os.path.join(W, s) for s in todo], cc, ROOT)
                UC.prune()
            else:
                res = G4.compile_all(W, C.hg_args(), [os.path.join(W, s) for s in todo], cc)
        failed = sum(1 for x in res if x[1])
        if todo and failed == len(todo):
            raise RuntimeError('the analysis compile failed for all %d files: build/hgss_repin/cc/summary.txt' % failed)
        t2 = time.time()
        self.sh([sys.executable, 'tools/pokehg_widen.py', '--casts', cc], log='widen_casts.log')
        n = self.fold()
        return '%d changed files reach %d of %d C files (%.0f s); compiled in %.0f s (%d failed; unit cache %s, %d hits), widened %d files' % (
            len(changed), len(todo), len(srcs), t1 - t, t2 - t1, failed, 'on' if use_cache else 'off: reach too wide', hits, len(n))

    def meson(self):
        self.sh(['meson', 'setup', '--reconfigure', BUILD, 'port'], log='meson.log')
        return ''

    def protos(self):
        r = self.sh([sys.executable, 'tools/pokehg_protos.py', '--write', '--chains', BUILD], log='protos.log')
        n = self.fold()
        s = [x for x in r.stdout.splitlines() if re.match(r'(slots in conflict|chains:)', x)]
        return '%d files changed; %s' % (len(n), '; '.join(s)[:200])

    def zcheck(self):
        """zhostfix's files one by one on W; the failures {path: git apply's errors} (their hunks in z/)."""
        head, files = split_patch(open(patch_path(HAND)).read()) if os.path.exists(patch_path(HAND)) else ('', {})
        z = os.path.join(OUT, 'z')
        shutil.rmtree(z, ignore_errors=True)
        os.makedirs(z)
        bad, pruned = {}, False
        for i, (f, body) in enumerate(list(files.items())):
            p = os.path.join(z, '%02d.patch' % i)
            open(p, 'w').write(body)
            r = self.sh(['git', '-C', W, 'apply', '--check', p], check=False)
            if r.returncode and self.partial.get(f) == HAND:
                pr = self.prune(body)
                if pr is not None:
                    self.say('zhostfix: %s: partly landed handover: %s' % (
                        f, 'the hunks upstream has dropped' if pr else 'upstream has every hunk: dropped'))
                    if pr:
                        files[f] = pr
                    else:
                        del files[f]
                    pruned = True
                    os.remove(p)
                    continue
            if r.returncode:
                bad[f] = [x for x in r.stdout.splitlines() if x.startswith('error')] + [
                    'hunks ' + x for x in self.file_report(f, body)] + [
                    'ZDROP %s (%s, %s)' % (s, ','.join(ids) or 'no Handover-ID', v)
                    for s, ids, _, v in self.hunks(body) if v != 'applies']
            else:
                os.remove(p)
        if pruned:
            open(patch_path(HAND), 'w').write(head + ''.join(files.values()))
            self.hdrop[HAND] = 'kept'
        self.zbad = bad
        return '%d of %d files apply' % (len(files) - len(bad), len(files))

    def fsview(self):
        """zfsview made again: pokehg_fsview.py's diff on the tree with zhostfix applied too."""
        self.sh(['git', '-C', W, 'apply', '--index', '--whitespace=nowarn', patch_path(HAND)])
        before = self.state()
        self.sh([sys.executable, 'tools/pokehg_fsview.py', '--build', os.path.join(ROOT, BUILD)], log='fsview.log')
        body = self.diff(before, self.state())
        self.heads[FSVIEW] = split_patch(open(patch_path(FSVIEW)).read())[0]
        old = split_patch(open(patch_path(FSVIEW)).read())[1]
        self.write_patch(FSVIEW, body)
        new = split_patch(body)[1]
        return '%d files (before: %d; %d differ)' % (len(new), len(old), sum(1 for f in set(old) | set(new)
                                                                              if old.get(f) != new.get(f)))

    def abi(self):
        """pokeheartgold-zzabi.patch made again: tools/pokehg_abi.py on a checkout of the new pin (HGSS_SRC) with every
        other patch (the declarations narrower than their definitions); gen+check's pokehg_gen.py gates on it"""
        r = self.sh([sys.executable, 'tools/pokehg_abi.py'], log='abi.log', check=False)
        if r.returncode:
            raise RuntimeError('pokehg_abi.py could not rewrite every declaration (build/hgss_repin/abi.log: its NOT '
                               'REWRITTEN lines): fix tools/pokehg_abi.py or list them in tools/hg_abi_exceptions.txt')
        return [x for x in r.stdout.splitlines() if 'declared as defined' in x][-1]

    def declscan(self, n, old):
        """pokeheartgold-zzzdecl.patch made again on W (the pin + every patch through zzabi): tools/hg_declscan.py's parse of
        the build's compile_commands.json (--facts), then --from-facts --emit-fixes (each edit verified by a re-parse);
        DIR/port.patch under the old patch's header comment (less the Handover-ID lines whose handover landed). No fix
        left: the patch is deleted."""
        d = os.path.join(OUT, 'declscan')
        shutil.rmtree(d, ignore_errors=True)
        os.makedirs(os.path.join(d, 'tmp'))
        cc = os.path.join(ROOT, BUILD, 'compile_commands.json')
        env = dict(os.environ, TMPDIR=os.path.join(d, 'tmp'))     # its PCH of the prefix: never one of an older tree
        try:        # the casts step's verdict (reach of the changed files); unknown: the old path
            use_cache = open(os.path.join(OUT, 'unitcache.txt')).read().strip() == '1'
        except OSError:
            use_cache = False
        if use_cache:
            env['BWPORT_UNITCACHE_SKIP'] = os.path.join(OUT, 'unitcache_skip.txt')
        else:
            env['BWPORT_UNITCACHE'] = 'off'
        facts = os.path.join(d, 'facts.jsonl')
        tool = [sys.executable, 'tools/hg_declscan.py', '--cc', cc, '-j', JOBS]
        self.sh(tool + ['--facts', facts, '--out', os.path.join(d, 'scan.txt')], log='declscan.log', env=env)
        r = self.sh(tool + ['--from-facts', facts, '--emit-fixes', os.path.join(d, 'fix'), '--out',
                            os.path.join(d, 'rank.txt')], log='declscan_fix.log', env=env)
        shutil.rmtree(os.path.join(d, 'tmp'), ignore_errors=True)
        body = open(os.path.join(d, 'fix', 'port.patch')).read()
        landed = getattr(self, 'landed', {})
        head = old[:old.find('diff --git ')] if 'diff --git ' in old else old
        head = ''.join(x for x in head.splitlines(True)
                       if not (re.match(r'Handover-ID:\s*(\S+)', x) and re.match(r'Handover-ID:\s*(\S+)', x).group(1) in landed))
        before, after = split_patch(old)[1] if old else {}, split_patch(body)[1] if body else {}
        if body.strip():
            open(patch_path(n), 'w').write(head + body)
            self.hdrop[n] = 'kept'
        else:
            if os.path.exists(patch_path(n)):
                os.remove(patch_path(n))
            self.hdrop[n] = 'deleted'
        fixed = [x for x in r.stdout.splitlines() if x.startswith('files edited')]
        return '%d files (before: %d; %d the same)%s; %s' % (
            len(after), len(before), sum(1 for f in after if before.get(f) == after[f]),
            '' if after else ': no fix left, the patch is deleted', (fixed or [''])[-1][:160])

    def final(self):
        self.sh([sys.executable, 'tools/pokehg_gen.py', '--mtimes', os.path.join(OUT, 'mtimes.json')], log='gen2.log')
        r = self.sh(['grep', '-rnE', r'^typedef (s32|u32) [a-z_]*_t;', 'src', 'include'], cwd=W, check=False)
        self.typedefs = r.stdout.splitlines()
        return 'typedef check: %d' % len(self.typedefs)

    def ninja(self):
        r = self.sh(['nice', '-n', '10', 'ninja', '-k', '0', '-j', JOBS, '-C', BUILD, 'bwport'], log='ninja.log',
                    check=False)
        self.errors = [x[:250] for x in r.stdout.splitlines() if x.startswith('FAILED') or ': error:' in x][:40]
        steps = re.findall(r'^\[(\d+)/(\d+)\]', r.stdout, re.M)
        return 'exit %d, %s steps' % (r.returncode, steps[-1][1] if steps else 0) if r.returncode or self.errors \
            else 'ok, %s steps' % (steps[-1][1] if steps else 0)

    def commit(self, run):
        subj = self.git(DECOMP, 'log', '-1', '--format=%s', self.new).strip()
        import hg_vendor
        hg_vendor.vendor(DECOMP, self.new)   # the new pin's tree (its own working copy: W stays as built)
        msg = os.path.join(OUT, 'commit.txt')
        open(msg, 'w').write('HGSS: re-pin to hgss-port %s (%s)\n\ntools/hgss_repin.py:\n%s\nWorker: %s\n' % (
            self.new[:9], subj[:120], '\n'.join('  ' + x for x in self.lines), self.worker))
        self.git(ROOT, 'add', '-A', SUB, *[os.path.relpath(patch_path(n), ROOT) for n in GENERATED + late_names()
                                     if os.path.exists(patch_path(n))])
        for n in getattr(self, 'hdrop', {}):    # hand patches hand_upstream() rewrote or deleted
            self.git(ROOT, 'add', '-A', '--', os.path.relpath(patch_path(n), ROOT))
        self.git(ROOT, 'commit', '-q', '-F', msg)
        return self.git(ROOT, 'rev-parse', '--short', 'HEAD').strip()

    def run(self):
        self.say('re-pin %s -> %s' % (self.old[:9], self.new[:9]))
        gone = missing_patches(ROOT)
        if gone:    # rpz1 11-15: a run killed while they were aside; the next one said "0 of 0 files apply"
            raise RuntimeError('%s missing: a re-pin killed halfway left it aside (build/hgss_repin/*.aside.patch); '
                               '`git checkout -- %s`, then run again' % (' '.join(gone), ' '.join(gone)))
        self.step('baseline', self.baseline)
        self.step('handover', self.handovers)
        if self.zdrop and os.path.exists(patch_path(HAND)):
            text, dropped, unused = zdrop(open(patch_path(HAND)).read(), self.zdrop)
            open(patch_path(HAND), 'w').write(text)
            self.hdrop[HAND] = 'kept'
            for x in dropped:
                self.say('zhostfix: --zdrop dropped %s' % x)
            if unused:
                self.say('zhostfix: --zdrop matched nothing: %s' % ' '.join(unused))
        json.dump(G4.snapshot(W), open(os.path.join(OUT, 'mtimes.json'), 'w'))
        late, aside = late_names(), []
        try:
            for n in late:
                shutil.copyfile(patch_path(n), os.path.join(OUT, n + '.aside.patch'))
                aside.append(n)
                os.remove(patch_path(n))
            self.step('rebase', self.rebase)
            if self.hbad:
                for f, e in self.hbad.items():
                    self.say('  FAIL %s: %s' % (f, ' | '.join(e)[:300]))
                    for x in self.hrep.get(f, []):
                        self.say('    hunks %s' % x)
                self.say('fix that patch (port/subprojects/pokeheartgold-NAME.patch) so it applies on %s, then run '
                         'again' % self.new[:9])
                return 2
            self.step('gen', self.gen)
            self.step('widen-tsv', self.widen_tsv)
            self.step('casts', self.casts)
            self.step('meson', self.meson)
            self.step('protos', self.protos)
        finally:
            for n in aside:
                shutil.copyfile(os.path.join(OUT, n + '.aside.patch'), patch_path(n))
        self.step('zhostfix', self.zcheck)
        if self.zbad:
            for f, e in self.zbad.items():
                self.say('  FAIL %s: %s' % (f, ' | '.join(x for x in e if not x.startswith(('hunks ', 'ZDROP ')))[:300]))
                for x in e:
                    if x.startswith('hunks '):
                        self.say('    ' + x)
                for x in e:
                    if x.startswith('ZDROP '):
                        self.say('  ' + x)
            self.say('fix port/subprojects/pokeheartgold-zhostfix.patch so that each file applies on build/pokeheartgold '
                     '(its hunks: build/hgss_repin/z/; `git -C build/pokeheartgold apply --3way` on one shows the conflict), or '
                     'drop the hunks upstream now fixes its own way (--zdrop PATH:LINE for each ZDROP line, or --zdrop '
                     'HANDOVER-ID), then run again')
            return 2
        if FSVIEW in late:
            self.step('fsview', self.fsview)
        post, kept, put = post_abi(), {}, set()
        for n in post:      # aside: zzabi is made without them, the tree for REGEN's tools is the stack through zzabi
            if os.path.exists(patch_path(n)):
                kept[n] = open(patch_path(n)).read()
                open(os.path.join(OUT, n + '.aside.patch'), 'w').write(kept[n])
                os.remove(patch_path(n))
        try:
            self.step('abi', self.abi)
            if post:
                self.step('gen+check', self.final)
            for n in post:
                if n in REGEN:
                    self.step(n, getattr(self, REGEN[n]), n, kept.get(n, ''))
                elif n in kept:
                    open(patch_path(n), 'w').write(kept[n])
                put.add(n)
        finally:
            for n in set(kept) - put:
                open(patch_path(n), 'w').write(kept[n])
        self.step('gen+check', self.final)
        if self.typedefs:
            self.say('\n'.join('  ' + x for x in self.typedefs[:20]))
            return 3
        self.step('ninja', self.ninja)
        if self.errors:
            self.say('\n'.join('  ' + x for x in self.errors))
            return 3
        base = os.path.join(OUT, 'base')
        rd = lambda d, f: open(os.path.join(d, f)).read().splitlines() if os.path.exists(os.path.join(d, f)) else []
        have = os.path.exists(os.path.join(base, 'verdict.txt'))
        # the same game frame as the baseline's end, or the default if that is later: the baseline may have ended
        # at a crash (or a wall cap, an old one) before it
        # the battle run does not depend on the boot run (and has its own run dir): both at once, ~1 min off the wall time
        import concurrent.futures as cf
        pool = cf.ThreadPoolExecutor(1)
        btf = pool.submit(self.step, 'battle', self.battle, 'new')     # port_run's Xvfb display pick is atomic (flock)
        try:
            v = self.step('boot', self.boot, 'new', max(self.frames, end_frame(rd(base, 'verdict.txt')[0]) if have else 0))
        except BaseException:
            btf.cancel()
            pool.shutdown(wait=False)
            raise
        if slow(v):
            raise RuntimeError('the boot run hit the --secs watchdog (%s): no result (a slow host or a stuck run); run '
                               'again with a bigger --secs' % v)
        if not have:
            worse, why = False, 'no baseline: ' + v
        else:
            worse, why = compare(rd(base, 'verdict.txt')[0], v, rd(base, 'trace.txt'),
                                 rd(os.path.join(OUT, 'new'), 'trace.txt'))
        self.say('boot run: ' + why)
        bt = btf.result()
        pool.shutdown()
        base_bt = (rd(base, 'battle.txt') or [''])[0]
        if bt:
            self.say('battle run: %s (before: %s)' % (bt, base_bt or 'none'))
        if worse:
            return 4
        if base_bt.startswith('pass') and not bt.startswith('pass'):
            return 4
        h = self.commit('boot run: ' + why)
        shutil.rmtree(base, ignore_errors=True)
        os.rename(os.path.join(OUT, 'new'), base)
        open(os.path.join(base, 'pin.txt'), 'w').write(self.new + '\n')
        self.say('committed %s' % h)
        return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('hash', help="hgss-port's commit to pin (fetched from the submodule's origin if needed)")
    ap.add_argument('--secs', type=int, default=900, help="the boot run's watchdog (port_run -t): a stuck run is killed "
                    "after this; hitting it is no result, not a worse run")
    ap.add_argument('--frames', type=int, default=FRAMES, help="the boot run's end V-blank (port_run --frames; default "
                    "%(default)s)")
    ap.add_argument('--worker', default='hgss_repin', help="the name in the commit's last trailer line")
    ap.add_argument('--battle-frames', type=int, default=BATTLE_FRAMES, help="the test battle's frame cap (0: no battle "
                    "check; default %(default)s)")
    ap.add_argument('--zdrop', action='append', default=[], metavar='SPEC',
                    help="take a zhostfix hunk out first (after an exit 2: PATH:LINE as its ZDROP line says; HANDOVER-ID: "
                    "its blocks; PATH: all of PATH's blocks, the ones that still apply too)")
    ap.add_argument('--handover-tsv', metavar='FILE', help="admin-79's backfill list of landed handovers (default "
                    "%s; the `Handover:` trailers of the new commits are read as well)" % HANDOVER_TSV)
    o = ap.parse_args()
    os.chdir(ROOT)
    for s in (signal.SIGTERM, signal.SIGHUP):
        signal.signal(s, killed)
    rc, r = 1, None
    try:
        r = Repin(o.hash, o.secs, o.worker, o.zdrop, o.frames, o.battle_frames)
        if o.handover_tsv:
            r.tsv_path = os.path.abspath(o.handover_tsv)
        rc = r.run()
    except Exception as e:  # noqa: BLE001 - any failure: the summary says it, exit 1
        traceback.print_exc()
        if r:
            r.say('FAILED: %s' % ((str(e) or type(e).__name__).splitlines() or [''])[0])
    if r:
        r.say('exit %d after %.0f s' % (rc, time.time() - r.t0))
        open(os.path.join(OUT, 'summary.txt'), 'w').write('\n'.join(r.lines) + '\n')
    sys.exit(rc)


if __name__ == '__main__':
    main()
