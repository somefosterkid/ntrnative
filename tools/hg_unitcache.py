#!/usr/bin/env python3
"""tools/hg_unitcache.py: a per-translation-unit result cache for the HG/SS re-pin's two analysis passes (hgss_repin.py's
casts compile, hg_declscan.py's fact scan). Both recompiled every C file a changed header "reached" (10212 of 10212 in
a typical re-pin, ~20 of its 30 minutes); most of those files are byte-for-byte what an earlier run already analysed.

The key of a unit is everything its result can depend on: the analysis kind and its version, the compiler's version, the
command line, and the content of every file the preprocessor reads for it (`clang -M`: ~0.12 s against the ~0.5 s of
the analysis compile; raw bytes, so a column shift or a comment edit misses too, an unchanged file under another
checkout hits). Checkout roots are replaced by @R@ in the paths, so worktrees on one machine share entries. A header
that changed misses exactly the units that include it; one that no unit reads costs nothing.

Entries live in $BWPORT_UNITCACHE (default ~/.cache/bwport/unitcache; "off" disables), one small JSON file each, written
atomically (concurrent workers and worktrees are safe), touched when used; prune() drops what was not used for TTL_DAYS
days and then the oldest until the directory is under the cap ($BWPORT_UNITCACHE_MAX_MB, default 3072).
"""
import hashlib, json, os, subprocess, time

TTL_DAYS = 7
ROOTTAG = '@R@'
_FILEH = {}
_CLANGV = {}


REACH_MAX = 0.4        # a re-pin whose changed files reach more of the units than this does not use the cache (see cache_wanted)


def cache_wanted(reach, total, env=None):
    """Whether the cache pays for a run whose changed files reach `reach` of `total` units. A unit's key costs a clang -M
    (+30% CPU cold on the casts compile, +52% on the declscan) and a miss saves nothing; a real re-pin changes widely
    included headers (rp12/rp14: 10212 of 10212 units reached) and the cache would then only add that cost. Above
    $HGSS_REPIN_CACHE_REACH (default REACH_MAX) of the units: no (old path, no keys). Reach unknown (total 0): no."""
    env = os.environ if env is None else env
    try:
        limit = float(env.get('HGSS_REPIN_CACHE_REACH', REACH_MAX))
    except ValueError:
        limit = REACH_MAX
    return total > 0 and reach <= limit * total


def skipped(src, root):
    """src is a unit whose own .c changed ($BWPORT_UNITCACHE_SKIP: a file of tree-relative paths): its key would miss
    for certain, so it is neither computed nor stored."""
    p = os.environ.get('BWPORT_UNITCACHE_SKIP')
    if not p:
        return False
    if p not in _SKIP:
        try:
            _SKIP[p] = set(open(p).read().split())
        except OSError:
            _SKIP[p] = set()
    parts = norm(src, root).split('/')
    return any('/'.join(parts[i:]) in _SKIP[p] for i in range(len(parts)))


_SKIP = {}


def cache_dir():
    v = os.environ.get('BWPORT_UNITCACHE')
    if v is not None and v.lower() in ('off', '0', 'none', 'no'):
        return None
    return v or os.path.join(os.path.expanduser('~'), '.cache', 'bwport', 'unitcache')


def norm(s, root):
    return s.replace(root, ROOTTAG) if root else s


def file_hash(p):
    """sha1 of a file's bytes, kept per process while its (mtime, size) stand."""
    try:
        st = os.stat(p)
    except OSError:
        return 'missing'
    sig = (st.st_mtime_ns, st.st_size)
    h = _FILEH.get(p)
    if h and h[0] == sig:
        return h[1]
    with open(p, 'rb') as f:
        d = hashlib.sha1(f.read()).hexdigest()
    _FILEH[p] = (sig, d)
    return d


def clang_version(clang='clang'):
    if clang not in _CLANGV:
        r = subprocess.run([clang, '--version'], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, text=True)
        _CLANGV[clang] = r.stdout.splitlines()[0] if r.stdout else 'none'
    return _CLANGV[clang]


def parse_make_deps(text):
    """The prerequisites of `clang -M`'s one rule (backslash-newline continuations, backslash-space in a name)."""
    text = text.replace('\\\r\n', ' ').replace('\\\n', ' ')
    toks, cur, i = [], '', 0
    while i < len(text):
        c = text[i]
        if c == '\\' and i + 1 < len(text) and text[i + 1] in ' #\\':
            cur += text[i + 1]
            i += 2
            continue
        if c.isspace():
            if cur:
                toks.append(cur)
            cur = ''
        else:
            cur += c
        i += 1
    if cur:
        toks.append(cur)
    i = next((n for n, t in enumerate(toks) if t.endswith(':')), -1)
    return toks[i + 1:]


def deps(args, src, cwd, clang='clang'):
    """The files the preprocessor reads for src under args, or None (it failed: the unit is not cached)."""
    a = [x for x in args if x not in ('-fsyntax-only', '-c')]
    r = subprocess.run([clang] + a + [src, '-M'], cwd=cwd, stdout=subprocess.PIPE,
                       stderr=subprocess.DEVNULL, text=True, errors='replace')
    if r.returncode:
        return None
    return parse_make_deps(r.stdout)


def unit_key(kind, version, args, src, cwd, root, clang='clang'):
    """The cache key of one unit, or None (cache off, or the preprocessor failed)."""
    if cache_dir() is None or skipped(src, root):
        return None
    d = deps(args, src, cwd, clang)
    if d is None:
        return None
    h = hashlib.sha256()
    for part in (kind, version, clang_version(clang), norm(' '.join(args), root)):
        h.update(part.encode() + b'\0')
    names = {norm(os.path.normpath(os.path.join(cwd, x)), root) for x in d}
    names.add(norm(os.path.normpath(os.path.join(cwd, src)), root))
    for p in sorted(names):
        real = p.replace(ROOTTAG, root) if root else p
        h.update(p.encode() + b'=' + file_hash(real).encode() + b'\0')
    return h.hexdigest()


def _path(kind, key):
    return os.path.join(cache_dir(), kind, key[:2], key)


def get(kind, key):
    """The stored object, or None. A hit counts as use (the file's mtime)."""
    if not key or cache_dir() is None:
        return None
    p = _path(kind, key)
    try:
        with open(p) as f:
            obj = json.load(f)
        os.utime(p)
        return obj
    except (OSError, ValueError):
        return None


def has(kind, key):
    return bool(key) and cache_dir() is not None and os.path.exists(_path(kind, key))


def put(kind, key, obj):
    if not key or cache_dir() is None:
        return
    p = _path(kind, key)
    try:
        os.makedirs(os.path.dirname(p), exist_ok=True)
        tmp = '%s.%d.tmp' % (p, os.getpid())
        with open(tmp, 'w') as f:
            json.dump(obj, f, separators=(',', ':'))
        os.replace(tmp, p)
    except OSError:
        pass


def prune(max_mb=None, ttl_days=TTL_DAYS, now=None):
    """Drop entries unused for ttl_days, then the least recently used until under the cap: (kept, dropped)."""
    top = cache_dir()
    if top is None or not os.path.isdir(top):
        return 0, 0
    cap = (int(os.environ.get('BWPORT_UNITCACHE_MAX_MB', '3072')) if max_mb is None else max_mb) * 1048576
    now = time.time() if now is None else now
    files = []
    for dp, _, fn in os.walk(top):
        for f in fn:
            p = os.path.join(dp, f)
            try:
                st = os.stat(p)
            except OSError:
                continue
            files.append((st.st_mtime, st.st_size, p))
    files.sort()
    total, dropped = sum(s for _, s, _ in files), 0
    for m, s, p in files:
        if now - m > ttl_days * 86400 or total > cap or (p.endswith('.tmp') and now - m > 3600):
            try:
                os.remove(p)
                total -= s
                dropped += 1
            except OSError:
                pass
    return len(files) - dropped, dropped


# ------------------------------------------------------------------------------------------------- casts compile
def casts_unit(work, args, out, root, src):
    """pokegen4.one for the analysis compile, from the cache when the unit's key is there: (rel, rc, output, hit)."""
    rel = os.path.relpath(src, work)
    log = os.path.join(out, 'log', rel + '.log')
    os.makedirs(os.path.dirname(log), exist_ok=True)
    key = unit_key('casts', 'casts1', args, src, work, root)
    hit = get('casts', key)
    if hit is not None:
        txt, rc, was = hit['out'].replace(ROOTTAG, root), hit['rc'], True
    else:
        r = subprocess.run(['clang'] + args + [src, '-fsyntax-only'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                           text=True, errors='replace', cwd=work)
        txt, rc, was = r.stdout, r.returncode, False
        if rc in (0, 1):
            put('casts', key, {'rc': rc, 'out': norm(txt, root)})
    open(log, 'w').write(txt)
    return rel, rc, txt, was


def compile_all(work, args, files, out, root):
    """pokegen4.compile_all with the cache: (res [(rel, rc, output)], hits)."""
    import functools, multiprocessing
    import pokegen4 as G4
    os.makedirs(out, exist_ok=True)
    with multiprocessing.get_context('fork').Pool(int(os.environ.get('BW_JOBS', '8'))) as p:
        res = p.map(functools.partial(casts_unit, work, args, out, root), files, chunksize=4)
    open(os.path.join(out, 'summary.txt'), 'w').write(G4.summarize([r[:3] for r in res], work, os.path.basename(work)))
    return [r[:3] for r in res], sum(1 for r in res if r[3])
