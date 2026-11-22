#!/usr/bin/env python3
"""tools/pokeplat_gen.py: the working copy of Pokemon Platinum's C for the port (docs/platinum.md), in build/pokeplatinum.

pret's pokeplatinum is not in this repository (it has no licence): third_party/pokeplatinum is a git submodule pinned at
pret's commit. This clones it (a local clone sharing the submodule's objects; the submodule's checkout is never edited)
into build/pokeplatinum (git-ignored), applies the port's patches (port/subprojects/pokeplatinum-*.patch, in name order:
each says what it fixes and why), and writes the headers pret's build generates into build/pokeplatinum/gen.

pret's C includes ~300 headers its build makes from res/: metang enums from generated/*.txt, text bank message IDs, NARC
member indexes (.naix), sprite template indexes, a few compiled-in tables. pret's own generators make them, run by meson
with a header-only top-level meson.build (no NitroSDK, no cross compiler, no ROM build); then the closure of what the C
includes is copied to gen/. Run again, it does nothing while the pinned commit and the patches are the same
(.bwport-stamp). tools/pokeplat_cc.py runs it first. Needs the network once (the submodule, meson).

  python3 tools/pokeplat_gen.py [--force]   # prints the working copy's path
"""
import argparse, glob, os, shutil, subprocess, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pokegen4 as G4  # noqa: E402
from pokegen4 import checkout, stamp  # noqa: E402,F401  (shared with tools/pokehg_gen.py)

ROOT = G4.ROOT
SUB = 'third_party/pokeplatinum'
WORK = os.path.join(ROOT, 'build/pokeplatinum')
PATCHES = os.path.join(ROOT, 'port/subprojects/pokeplatinum-*.patch')
# the C's include dirs (pokeplat_cc.py's order) and the build dirs pret's targets add
SRC_DIRS = ['include', '.', 'src', 'lib/spl/include', 'lib/gds/include', 'lib/crypto/include', 'lib/ppwlobby/include']
GEN_DIRS = ['.', 'res/text', 'generated']

# pret's top-level meson.build minus the ROM: its generated/, tools/ and res/ subdirs only
HEADER_MESON = """project('pokeplatinum', ['c', 'cpp'], meson_version: '>=1.12.0',
    default_options: ['buildtype=plain', 'warning_level=0'])
fs = import('fs')
public_includes = include_directories('include', 'asm')
toplevel_includes = include_directories('.')
subdir('generated')
subdir('tools')
subdir('res')
"""
# res/words/word_bank.c (compiled by the host's arm-none-eabi-gcc for its .naix) includes the SDK's types.h only
TYPES_H = """#pragma once
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef unsigned long long u64;
typedef signed char s8; typedef signed short s16; typedef signed int s32; typedef signed long long s64;
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define NULL ((void *)0)
"""


def build(co):
    """pret's generators for the headers only, in co/hbuild."""
    open(os.path.join(co, 'meson.build'), 'w').write(HEADER_MESON)
    t = os.path.join(co, 'tools/meson.build')
    s = open(t).read()   # makelcf comes from the NitroSDK subproject: unused by the headers
    open(t, 'w').write(s.replace("find_program('makelcf', native: true)", "find_program('true')"))
    os.makedirs(os.path.join(co, 'subprojects/NitroSDK-4.2.30001/include/nitro'), exist_ok=True)
    open(os.path.join(co, 'subprojects/NitroSDK-4.2.30001/include/nitro/types.h'), 'w').write(TYPES_H)
    env = dict(os.environ, TMPDIR=os.path.join(co, '.tmp'))
    os.makedirs(env['TMPDIR'], exist_ok=True)
    b = os.path.join(co, 'hbuild')
    if not os.path.exists(os.path.join(b, 'build.ninja')):
        subprocess.check_call(G4.meson(co) + ['setup', '--wrap-mode=nopromote', '--native-file=meson/native.ini',
                                              'hbuild'], cwd=co, env=env, stdout=subprocess.DEVNULL)
    subprocess.check_call(['ninja', '-C', b, '-j', os.environ.get('BW_JOBS', '8')] + G4.header_targets(b), env=env,
                          stdout=subprocess.DEVNULL, stdin=subprocess.DEVNULL)
    return b


def resolve(h, co, b, naix):
    """h's path relative to the build dir b, or None when the C tree co (or the SDK) has it."""
    if any(os.path.exists(os.path.join(co, d, h)) for d in SRC_DIRS):
        return None
    for d in GEN_DIRS:
        p = os.path.normpath(os.path.join(d, h))
        if os.path.isfile(os.path.join(b, p)):
            return p
    return naix.get(h) if '/' not in h else None


def closure(co, b):
    naix = G4.naix_dirs(b, 'res')
    return G4.closure(co, b, ('src', 'include', 'lib'), lambda h, f: resolve(h, co, b, naix))


def tree(force=False):
    """The working copy's path, made or brought up to date."""
    sub = G4.submodule(SUB)
    commit, patches = G4.pinned(SUB), sorted(glob.glob(PATCHES))
    st, sf = stamp(commit, patches), os.path.join(WORK, '.bwport-stamp')
    if not force and os.path.exists(sf) and open(sf).read() == st:
        return WORK
    snap = G4.snapshot(WORK) if os.path.isdir(WORK) else {}  # (ar6: as pokehg_gen.py: unchanged files keep their mtime)
    checkout(WORK, sub, commit, patches)
    b = build(WORK)
    gen = os.path.join(WORK, 'gen')
    shutil.rmtree(gen, ignore_errors=True)
    for p in closure(WORK, b):
        os.makedirs(os.path.dirname(os.path.join(gen, p)), exist_ok=True)
        shutil.copyfile(os.path.join(b, p), os.path.join(gen, p))
    open(sf, 'w').write(st)
    G4.keep_mtimes(WORK, snap)
    return WORK


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--force', action='store_true', help='check out, patch and generate again')
    print(tree(ap.parse_args().force))


if __name__ == '__main__':
    main()
