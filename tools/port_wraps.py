#!/usr/bin/env python3
"""Bring a port build dir up to date with the meson wraps (port_run.py and port_win64.sh both call this).

    tools/port_wraps.py BUILD_DIR [meson setup args...]     e.g. --native-file port/machine/win64.ini -Dbuild_target=win64

- no BUILD_DIR/build.ninja: meson setup (which fetches and patches the subprojects that are missing)
- the old libntr checkout (port/subprojects/libntr/.git: a wrap's clone with the patch stack applied, before libntr was folded
  into the tree, 11-21) there: removed, then `git checkout -- port/subprojects/libntr` restores the tree's files (a nested .git
  would make git treat the directory as another repository). libntr is a plain directory of the tree now (tools/libntr_fold.py,
  docs/libntr/fold.md): a change to it is a normal commit, and ninja rebuilds what it reaches.
- White's assets (build/w_us/asset) there but the dir configured without White (no libgame_w.a in build.ninja): meson setup
  --reconfigure (port/meson.build looks for the assets at configure time only).
- a dir made before port/machine/linux-ccache.ini named the ccache-nopch launcher (11-21) still compiles with plain `ccache clang`:
  meson keeps a dir's compilers and does not watch the native files, so its .pch went through the shared cache and a second
  checkout got a .pch naming the first one's generated headers ("file ... has been modified since the precompiled header ... was
  built" / "could not find file", 11-25: slot2 and slot3 of the desktop). `meson setup --wipe` once (options and native file
  paths are kept; the objects come back from ccache: ~2 min).
Prints the error output and exits 1 when a step fails.
"""
import json
import os
import re
import shutil
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SLICE = shutil.which('systemd-run') and os.path.isdir('/run/systemd/system')
LIBNTR = os.path.join(REPO, 'port/subprojects/libntr')


def no_cmake_env():
    """macOS: meson's `cmake --version` probe can hang for good in the agents' sandbox (2026-11-04: a reconfigure waited 41
    minutes); the port finds every dependency without cmake, so meson sees a cmake that fails at once"""
    if sys.platform != 'darwin':
        return None
    shim = os.path.join(REPO, 'build', '.nocmake')
    tool = os.path.join(shim, 'cmake')
    if not os.path.exists(tool):
        os.makedirs(shim, exist_ok=True)
        with open(tool, 'w') as f:
            f.write('#!/bin/sh\nexit 1\n')
        os.chmod(tool, 0o755)
    return dict(os.environ, PATH=shim + os.pathsep + os.environ.get('PATH', ''))


def sh(cmd, **kw):
    if 'env' not in kw:
        env = no_cmake_env()
        if env is not None:
            kw['env'] = env
    return subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, **kw)


def slice_cmd(cmd):
    """cmd in the worker slice (CPU work), when systemd runs"""
    return ['systemd-run', '--scope', '-q', '--slice=bwworkers.slice'] + cmd if SLICE else cmd


STALE_MARK = '.stale_launcher_wiped'   # in the build dir: it was wiped once for a missing ccache-nopch (never again: pfx1 11-25)


def native_files(setup_args):
    """the --native-file paths of a meson setup argument list, in order"""
    args = list(setup_args)
    return [a.split('=', 1)[1] if a.startswith('--native-file=') else args[i + 1]
            for i, a in enumerate(args) if a.startswith('--native-file=') or (a == '--native-file' and i + 1 < len(args))]


def stale_launcher(build, setup_args):
    """True when the compiler the native files of setup_args end up with names the ccache-nopch launcher (port/machine/linux-ccache.ini)
    but this build dir was configured without it: its build.ninja runs the compiler without `ccache-nopch`.
    Only the LAST native file that defines [binaries] c counts (meson: the later file wins): with BWPORT_SYSROOT the sysroot's native-ccache.ini
    comes after linux-ccache.ini and names plain `ccache clang`, so no dir ever has the launcher and the old check wiped it on every run (25 min each)."""
    last = None
    try:
        for p in native_files(setup_args):
            with open(os.path.join(REPO, p), errors='replace') as f:
                lines = [ln for ln in f if not ln.lstrip().startswith('#')]
            if any(re.match(r'\s*c\s*=', ln) for ln in lines):
                last = any('ccache-nopch' in ln for ln in lines)
        if last:
            with open(os.path.join(build, 'build.ninja'), errors='replace') as f:
                return 'ccache-nopch' not in f.read(2000000)   # the compiler rules are at the top
    except OSError:
        pass
    return False


def pch_sums(build, say=lambda msg: None):
    """ccx2 11-26: a build dir made before ccache-nopch wrote X.pch.sum has none, so its objects hash the dir's own .pch: a miss in every
    other dir (CCACHE_PCH_EXTSUM, port/machine/ccache-nopch). For each *.pch without a .sum: the last line of `ninja -t commands <pch>`
    is the .pch's own compile; the launcher run with CCACHE_NOPCH_SUMONLY=1 writes the sum and compiles nothing, so ninja sees no change."""
    try:
        with open(os.path.join(build, 'build.ninja'), errors='replace') as f:
            if 'ccache-nopch' not in f.read(2000000):
                return
    except OSError:
        return
    for d, _, files in os.walk(build):
        for n in files:
            if not n.endswith('.pch') or os.path.exists(os.path.join(d, n + '.sum')):
                continue
            rel = os.path.relpath(os.path.join(d, n), build)
            r = sh(['ninja', '-C', build, '-t', 'commands', rel])
            lines = r.stdout.strip().splitlines() if r.returncode == 0 else []
            if not lines or 'ccache-nopch' not in lines[-1]:
                continue
            say('writing ' + rel + '.sum')
            sh(['sh', '-c', lines[-1]], cwd=build, env=dict(os.environ, CCACHE_NOPCH_SUMONLY='1'))


def white_pending(build):
    """True when White's assets exist now but this build dir was configured without them: port/meson.build decides
    build_white once, at configure time (`fs.is_dir(<rom build>/w_us/asset)`), and nothing in meson makes it look again, so a
    dir configured before `ninja` made build/w_us/asset (a worktree that was not at main's commit: setup_worktree.sh copies
    White's build only then) stays White-less: the launcher says "Pokemon White is not playable in this build yet"."""
    if not os.path.isdir(os.path.join(REPO, 'build/w_us/asset')) or sys.platform == 'darwin':
        return False
    try:
        with open(os.path.join(build, 'build.ninja'), errors='replace') as f:
            if 'libgame_w.a' in f.read():
                return False
        with open(os.path.join(build, 'meson-info/intro-buildoptions.json')) as f:
            opts = json.load(f)
    except (OSError, ValueError):
        return False
    return not any(o.get('name') == 'white' and o.get('value') == 'disabled' for o in opts)


def refresh(build, setup_args=(), log=None):
    """meson setup / subprojects reset / reconfigure as the state needs; the error output, or None"""
    ninja = os.path.join(build, 'build.ninja')
    mtime = lambda paths: max(os.path.getmtime(os.path.join(REPO, p)) for p in paths if os.path.exists(os.path.join(REPO, p)))   # (port/gpu/msl is not in the public export)
    # port/extern's stand-ins for game files (tools/port_sources.py), in subdirectories: a file added there changes its own dir
    extern_dirs = [os.path.relpath(d, REPO) for d, _, _ in os.walk(os.path.join(REPO, 'port/extern'))]
    say = log or (lambda msg: None)
    r = None
    if os.path.isdir(os.path.join(LIBNTR, '.git')) and not os.path.exists(os.path.join(REPO, 'port/subprojects/libntr.wrap')):   # the old wrap checkout: not part of the tree (the public repo has the wrap: its checkout is meant to be there)
        say("old libntr wrap checkout removed, the tree's libntr restored")
        shutil.rmtree(LIBNTR)
        r = sh(['git', 'checkout', '--', 'port/subprojects/libntr'], cwd=REPO)
        if r.returncode:
            return r.stdout
        r = None
    if not os.path.exists(ninja):
        say('meson setup')
        r = sh(slice_cmd(['meson', 'setup', build, 'port'] + list(setup_args)), cwd=REPO)
    elif stale_launcher(build, setup_args) and not os.path.exists(os.path.join(build, STALE_MARK)):
        say('compiled without ccache-nopch: meson setup --wipe')
        # the new native files go with the wipe (meson replaces the dir's recorded ones): a dir first set up without linux-ccache.ini stays stale otherwise
        r = sh(slice_cmd(['meson', 'setup', '--wipe', build, 'port'] + [x for f in native_files(setup_args) for x in ('--native-file', f)]), cwd=REPO)
        if r.returncode == 0:
            try:
                with open(os.path.join(build, STALE_MARK), 'w') as f:
                    f.write('wiped once for a missing ccache-nopch launcher'+chr(10))
            except OSError:
                pass
    elif mtime(['port/src', 'port/exclude.txt', 'port/gpu/src', 'port/gpu/shaders', 'port/gpu/msl'] + extern_dirs) > os.path.getmtime(ninja):   # a file added or taken out
        say('sources changed: reconfigure')
        r = sh(slice_cmd(['meson', 'setup', '--reconfigure', build, 'port']), cwd=REPO)
    elif white_pending(build):
        say("White's assets are there now: reconfigure")
        r = sh(slice_cmd(['meson', 'setup', '--reconfigure', build, 'port']), cwd=REPO)
    if r is not None and r.returncode != 0:
        return r.stdout
    if r is None:
        pch_sums(build, say)
    if r is None and os.path.exists(os.path.join(build, STALE_MARK)) and stale_launcher(build, setup_args):
        say('warning: still compiled without ccache-nopch after the one wipe (not wiping again; delete the build dir to rebuild it with the launcher)')
    return None


if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    err = refresh(os.path.abspath(sys.argv[1]), sys.argv[2:], lambda m: print('port_wraps:', m, flush=True))
    if err:
        print(err)
        sys.exit(1)
