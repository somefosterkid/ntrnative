#!/usr/bin/env python3
"""Bring a port build dir up to date with the meson wraps (port_run.py and port_win64.sh both call this).

    tools/port_wraps.py BUILD_DIR [meson setup args...]     e.g. --native-file port/machine/win64.ini -Dbuild_target=win64

- no BUILD_DIR/build.ninja: meson setup (which fetches and patches the subprojects that are missing)
- a .wrap or a packagefiles/ file newer than build.ninja: `meson subprojects update --reset` (the checkouts again,
  with every diff_files patch applied) and meson setup --reconfigure
- port/src, port/exclude.txt, port/extern (any of its dirs), port/gpu/src, port/gpu/shaders or port/gpu/msl newer than build.ninja:
  meson setup --reconfigure (a file added or taken out; port/meson.build lists the shaders it embeds at configure time)
- the checkouts were patched from other wraps than these (the hash of every .wrap and packagefiles/ file, kept in
  STAMP after each reset or setup): the reset as above, before a setup too. A new build dir's build.ninja is newer than
  any wrap, so the mtime test alone left a stale shared checkout in place (a fresh macOS dir built against a libntr
  without the newest patch and aborted at start on a missing symbol).
The checkouts in port/subprojects/ are shared by the build dirs: a reset for one dir makes the others reset too
(harmless; their next build recompiles what the reset touched).
Prints the error output and exits 1 when a step fails.
"""
import glob
import hashlib
import os
import shutil
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SLICE = shutil.which('systemd-run') and os.path.isdir('/run/systemd/system')
STAMP = os.path.join(REPO, 'port/subprojects/libntr/.bwport-wraps')   # inside the (ignored) shared checkout


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


def refresh(build, setup_args=(), log=None):
    """meson setup / subprojects reset / reconfigure as the state needs; the error output, or None"""
    ninja = os.path.join(build, 'build.ninja')
    mtime = lambda paths: max(os.path.getmtime(os.path.join(REPO, p)) for p in paths)
    # port/extern's stand-ins for game files (tools/port_sources.py), in subdirectories: a file added there changes its own dir
    extern_dirs = [os.path.relpath(d, REPO) for d, _, _ in os.walk(os.path.join(REPO, 'port/extern'))]
    wraps = glob.glob('port/subprojects/*.wrap', root_dir=REPO) + glob.glob('port/subprojects/packagefiles/*',
                                                                             root_dir=REPO)
    say = log or (lambda msg: None)
    h = hashlib.sha1()
    for p in sorted(wraps):
        h.update(p.encode())
        with open(os.path.join(REPO, p), 'rb') as f:
            h.update(f.read())
    want = h.hexdigest()
    have = open(STAMP).read().strip() if os.path.exists(STAMP) else ''
    stale = os.path.isdir(os.path.dirname(STAMP)) and have != want   # the checkout exists but was patched from other wraps
    r = None
    if stale:
        say('checkouts patched from other wraps: meson subprojects update --reset')
        r = sh(['meson', 'subprojects', 'update', '--reset'], cwd=os.path.join(REPO, 'port'))
        if r.returncode:
            return r.stdout
        r = None
    if not os.path.exists(ninja):
        say('meson setup')
        r = sh(slice_cmd(['meson', 'setup', build, 'port'] + list(setup_args)), cwd=REPO)
    elif stale or mtime(wraps) > os.path.getmtime(ninja):     # a wrap's patches changed: the checkouts again
        say('wraps changed: meson subprojects update --reset, reconfigure')
        if not stale:
            r = sh(['meson', 'subprojects', 'update', '--reset'], cwd=os.path.join(REPO, 'port'))
        r = r if r is not None and r.returncode else sh(slice_cmd(['meson', 'setup', '--reconfigure', build, 'port']), cwd=REPO)
    elif mtime(['port/src', 'port/exclude.txt', 'port/gpu/src', 'port/gpu/shaders', 'port/gpu/msl'] + extern_dirs) > os.path.getmtime(ninja):   # a file added or taken out
        say('sources changed: reconfigure')
        r = sh(slice_cmd(['meson', 'setup', '--reconfigure', build, 'port']), cwd=REPO)
    if r is not None and r.returncode != 0:
        return r.stdout
    if os.path.isdir(os.path.dirname(STAMP)):
        with open(STAMP, 'w') as f:
            f.write(want + '\n')
    return None


if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    err = refresh(os.path.abspath(sys.argv[1]), sys.argv[2:], lambda m: print('port_wraps:', m, flush=True))
    if err:
        print(err)
        sys.exit(1)
