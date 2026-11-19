#!/usr/bin/env python3
"""tools/pokegen4.py: what the Gen 4 working-copy tools share (tools/pokeplat_gen.py + pokeplat_cc.py for Platinum,
tools/pokehg_gen.py + pokehg_cc.py for HeartGold/SoulSilver; docs/platinum.md, docs/hgss.md).

- pinned / stamp / checkout: a working copy in build/<game> = a local clone of the game's submodule at the commit this
  tree pins, the port's patches (port/subprojects/<game>-*.patch) applied in name order; the submodule is only read.
- meson: pret's meson (the version their meson.build asks for), cloned once into the working copy.
- closure: the generated headers the C includes (pret's build makes them), as paths relative to the build dir.
- base_args: the port's clang line for one Black game file (libntr's and lib/nnsys's headers and defines).
- compile_all: every file with that line, logs, and DIR/summary.txt (files ok / failed, errors by class and message).

Not run on its own.
"""
import collections, functools, hashlib, json, multiprocessing, os, re, shlex, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def hgss_build():
    """The build dir the HG/SS tools read compile_commands.json from: $HGSS_BUILD, else build/port/linux-hgss (the older module
    build) when it has one, else build/port/linux (the default build has every game)."""
    d = os.environ.get('HGSS_BUILD')
    if not d:
        d = 'build/port/linux-hgss' if os.path.exists(os.path.join(ROOT, 'build/port/linux-hgss/compile_commands.json')) \
            else 'build/port/linux'
    return d


INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)


def pinned(sub):
    """The submodule's commit as this tree pins it (the index's gitlink)."""
    return subprocess.check_output(['git', '-C', ROOT, 'ls-files', '-s', sub], text=True).split()[1]


def stamp(commit, patches):
    h = hashlib.sha1(commit.encode())
    for p in patches:
        h.update(os.path.basename(p).encode() + b'\0' + open(p, 'rb').read())
    return commit + ' ' + h.hexdigest()


def head(work):
    """work's checked-out commit (None when it is no clone yet)."""
    r = subprocess.run(['git', '-C', work, 'rev-parse', 'HEAD'], capture_output=True, text=True)
    return r.stdout.strip() if not r.returncode else None


def checkout(work, sub, commit, patches):
    """work: a clone of sub at commit (sharing sub's objects), patches applied in order. sub is only read."""
    if not os.path.isdir(os.path.join(work, '.git')):
        subprocess.check_call(['git', 'clone', '-q', '--shared', '--no-checkout', sub, work])
    if subprocess.call(['git', '-C', work, 'cat-file', '-e', commit + '^{commit}'], stderr=subprocess.DEVNULL):
        subprocess.check_call(['git', '-C', work, 'fetch', '-q', sub, commit])
    subprocess.check_call(['git', '-C', work, 'checkout', '-q', '-f', '--detach', commit])
    # the files a patch added last time (they would make it fail), not the build's own (gen/, hbuild/, the stamp)
    subprocess.check_call(['git', '-C', work, 'clean', '-fdq', '-e', '/gen', '-e', '/hbuild', '-e', '/.bwport-stamp'])
    for p in patches:
        subprocess.check_call(['git', '-C', work, 'apply', '--whitespace=nowarn', p])


SNAP_EXT = ('.c', '.h', '.inc', '.s', '.cpp', '.naix')  # what the port's compiles read from a working copy


def snapshot(root):
    """{path relative to root: [sha1, mtime_ns]} of root's C sources and headers (not .git/, not hbuild/)."""
    s = {}
    for dp, dn, fn in os.walk(root):
        if dp == root:
            dn[:] = [d for d in dn if d not in ('.git', 'hbuild')]
        for f in fn:
            p = os.path.join(dp, f)
            if f.endswith(SNAP_EXT) and os.path.isfile(p) and not os.path.islink(p):
                s[os.path.relpath(p, root)] = [hashlib.sha1(open(p, 'rb').read()).hexdigest(), os.stat(p).st_mtime_ns]
    return s


def keep_mtimes(root, snap):
    """snap's mtime back on root's files whose content is still snap's (a checkout and patch rewrite every patched file:
    without this ninja rebuilds them all); the count of files set back. No build may run between snapshot and this."""
    n = 0
    for rel, (h, t) in snap.items():
        p = os.path.join(root, rel)
        try:
            st = os.stat(p)
        except FileNotFoundError:
            continue
        if st.st_mtime_ns != t and hashlib.sha1(open(p, 'rb').read()).hexdigest() == h:
            os.utime(p, ns=(st.st_atime_ns, t))
            n += 1
    return n


def submodule(sub):
    if not os.path.exists(os.path.join(ROOT, sub, '.git')):
        subprocess.check_call(['git', '-C', ROOT, 'submodule', 'update', '--init', sub])
    return os.path.join(ROOT, sub)


def meson(co, version='1.12.0'):
    """[python, meson.py]: pret's meson version, cloned into co/subprojects once."""
    m = os.path.join(co, 'subprojects/meson-' + version)
    if not os.path.isdir(m):
        subprocess.check_call(['git', 'clone', '-q', '--depth=1', '-b', version, 'https://github.com/mesonbuild/meson', m])
    return [sys.executable, os.path.join(m, 'meson.py')]


def header_targets(b):
    """The build dir's .h and .naix targets (not the subprojects' or the per-target private dirs')."""
    tg = subprocess.check_output(['ninja', '-C', b, '-t', 'targets', 'all'], text=True)
    return sorted({m.group(1) for m in re.finditer(r'^([^:]*\.(?:h|naix)): ', tg, re.M)
                   if not m.group(1).startswith('subprojects/') and '.p/' not in m.group(1)})


def naix_dirs(b, top):
    """basename -> path (relative to b) of every .naix under b/top: some C includes one by its bare name."""
    out = {}
    for dp, dn, fn in os.walk(os.path.join(b, top)):
        dn[:] = [d for d in dn if not d.endswith('.p')]
        for f in sorted(fn):
            if f.endswith('.naix'):
                out.setdefault(f, os.path.relpath(os.path.join(dp, f), b))
    return out


def closure(co, b, tops, resolve, exts=('.c', '.h', '.cpp')):
    """The generated headers (paths relative to b) that the C under co/tops includes, transitively (exts: the files
    looked in). resolve(h, including_file) -> path relative to b, or None when the C tree has it."""
    todo, seen = [], set()
    for top in tops:
        for dp, dn, fn in os.walk(os.path.join(co, top)):
            todo += [os.path.join(dp, f) for f in fn if f.endswith(exts)]
    while todo:
        f = todo.pop()
        for h in INCLUDE.findall(open(f, errors='replace').read()):
            p = resolve(h, f)
            if p and p not in seen:
                seen.add(p)
                todo.append(os.path.join(b, p))
    return sorted(seen)


def base_args():
    """The port's clang line for one Black game file, minus Black's own include dirs, prefix and sources."""
    cc = json.load(open(os.path.join(ROOT, 'build/port/linux/compile_commands.json')))
    ent = next(e for e in cc if e['file'].endswith('src/system/main.c') and 'pokebw2' not in e['file'])
    a = shlex.split(ent['command'])
    out = []
    i = 1
    while i < len(a):
        x = a[i]
        if x in ('-o', '-MF', '-MQ', '-c'):
            i += 2
            continue
        if x in ('-include', '-idirafter', '-isystem', '-I'):
            out.append((x, a[i + 1]))
            i += 2
            continue
        out.append((x,))
        i += 1
    keep = []
    d = ent['directory']
    for t in out:
        x = t[0]
        if x == '-include':
            continue                       # Black's prefix (port_prefix.h ...): each game has its own
        if x.startswith('-I') or x in ('-idirafter', '-isystem', '-I'):
            p = t[1] if len(t) > 1 else x[2:]
            p = os.path.normpath(os.path.join(d, p))
            main = d.split('/build/')[0]       # compile_commands names the main checkout: a worktree's own lib/nnsys
            if (p.startswith(main + '/lib/') or p.startswith(main + '/port/include_nnsys')) and 'subprojects' not in p:
                p = os.path.join(ROOT, p[len(main) + 1:])
            # libntr's and lib/nnsys's headers stay; Black's include/, lib/*/include (but nnsys), port/include go
            if ('subprojects' in p or '/lib/nnsys' in p or 'include_nnsys' in p) and 'pokebw2' not in p:
                keep += [x if len(t) > 1 else '-I', p] if len(t) > 1 else ['-I' + p]
            continue
        if x.startswith(('-D', '-U', '-f', '-m', '-std', '-W', '-O', '-g', '--target')):
            if x.startswith(('-Werror', '-fdiagnostics-color', '-DBWPORT_')):
                continue
            keep.append(x)
    return keep


# the diagnostics a survey asks for on top of the port's line: pointer size, implicit declarations
DIAG = ['-ferror-limit=0', '-fno-diagnostics-color', '-Wno-everything', '-Wint-to-pointer-cast', '-Wpointer-to-int-cast',
        '-Wshorten-64-to-32', '-Wincompatible-pointer-types', '-Wimplicit-function-declaration', '-Wint-conversion',
        '-Wno-error']

CATS = [
    ('pointer size', r'(int-to-pointer|pointer-to-int|cast to smaller integer|incompatible integer to pointer|'
                     r'incompatible pointer to integer|static_assert|static assertion)'),
    ('inline asm / ARM-only', r'(\basm\b|__asm|invalid instruction|unknown register|register variable)'),
    ('generated header / res', r"'(res|generated|files|msgdata)/[^']*' file not found|\.naix' file not found"),
    ('missing header', r"file not found"),
    ('SDK/NNS undeclared name', r'(undeclared identifier|implicit declaration|unknown type name|undeclared function)'),
    ('SDK/NNS signature/struct', r'(too (few|many) arguments|no member named|incompatible pointer types|'
                                 r'conflicting types|redefinition|passing .* to parameter|incomplete)'),
]


def cat(msg):
    for n, p in CATS:
        if re.search(p, msg):
            return n
    return 'other'


def norm(m):
    m = re.sub(r"'[^']*'", "'X'", m)
    return re.sub(r'\d+', 'N', m)


DIAG_LINE = re.compile(r'(.*?):(\d+):\d+: (?:fatal )?(error|warning): (.*)')


def one(work, args, out, obj, src):
    rel = os.path.relpath(src, work)
    log = os.path.join(out, 'log', rel + '.log')
    os.makedirs(os.path.dirname(log), exist_ok=True)
    cmd = ['clang'] + args + [src]
    if obj:
        o = os.path.join(out, 'obj', rel + '.o')
        os.makedirs(os.path.dirname(o), exist_ok=True)
        cmd += ['-c', '-o', o]
    else:
        cmd += ['-fsyntax-only']
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace', cwd=work)
    open(log, 'w').write(r.stdout)
    return rel, r.returncode, r.stdout


def summarize(res, work, own):
    """summary.txt's text for [(rel, returncode, output)]. Warnings count only in files under own (the game's tree)."""
    ok = [r for r in res if r[1] == 0]
    bad = [r for r in res if r[1] != 0]
    bycat, bycat_files, bymsg, first = collections.Counter(), collections.defaultdict(set), collections.Counter(), {}
    for rel, rc, txt in res:
        for line in txt.splitlines():
            m = DIAG_LINE.match(line)
            if not m:
                continue
            path = os.path.abspath(os.path.join(work, m.group(1)))
            if m.group(3) == 'warning' and own not in path:
                continue
            msg = ('W ' if m.group(3) == 'warning' else '') + m.group(4)
            c = cat(msg)
            bycat[c] += 1
            bycat_files[c].add(rel)
            k = norm(msg)
            bymsg[(c, k)] += 1
            first.setdefault((c, k), '%s:%s: %s' % (os.path.relpath(path, work), m.group(2), msg))
    s = ['files %d ok %d failed %d\n' % (len(res), len(ok), len(bad))]
    for c, n in bycat.most_common():
        s.append('%-28s %6d errors in %4d files' % (c, n, len(bycat_files[c])))
    s.append('')
    for (c, k), n in bymsg.most_common(150):
        s.append('%6d  [%s] %s\n        e.g. %s' % (n, c, k[:160], first[(c, k)][:200]))
    s.append('\nFAILED FILES\n' + '\n'.join(r[0] for r in bad))
    return '\n'.join(s) + '\n'


def compile_all(work, args, files, out, obj=False, own=None):
    """Compile files (in work) with args; logs in out/log, summary in out/summary.txt; returns [(rel, rc, output)]."""
    os.makedirs(out, exist_ok=True)
    with multiprocessing.get_context('fork').Pool(int(os.environ.get('BW_JOBS', '8'))) as p:
        res = p.map(functools.partial(one, work, args, out, obj), files, chunksize=4)
    open(os.path.join(out, 'summary.txt'), 'w').write(summarize(res, work, own or os.path.basename(work)))
    return res
