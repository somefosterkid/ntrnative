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
Prints the error output and exits 1 when a step fails.
"""
import json
import os
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
    mtime = lambda paths: max(os.path.getmtime(os.path.join(REPO, p)) for p in paths)
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
    elif mtime(['port/src', 'port/exclude.txt', 'port/gpu/src', 'port/gpu/shaders', 'port/gpu/msl'] + extern_dirs) > os.path.getmtime(ninja):   # a file added or taken out
        say('sources changed: reconfigure')
        r = sh(slice_cmd(['meson', 'setup', '--reconfigure', build, 'port']), cwd=REPO)
    elif white_pending(build):
        say("White's assets are there now: reconfigure")
        r = sh(slice_cmd(['meson', 'setup', '--reconfigure', build, 'port']), cwd=REPO)
    if r is not None and r.returncode != 0:
        return r.stdout
    return None


if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    err = refresh(os.path.abspath(sys.argv[1]), sys.argv[2:], lambda m: print('port_wraps:', m, flush=True))
    if err:
        print(err)
        sys.exit(1)
