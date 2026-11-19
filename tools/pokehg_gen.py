#!/usr/bin/env python3
"""tools/pokehg_gen.py: the working copy of Pokemon HeartGold/SoulSilver's C for the port (docs/hgss.md), in
build/pokeheartgold. The pattern is tools/pokeplat_gen.py's (shared code: tools/pokegen4.py).

third_party/pokeheartgold was a git submodule (hgss-decomp, branch hgss-port: pret's pokeheartgold plus our
decompilation and port/, the C for the functions still in asm/ and the 64-bit tables). This clones it (sharing
the submodule's objects; the submodule's checkout is never edited) into build/pokeheartgold (git-ignored), applies the
port's patches (port/subprojects/pokeheartgold-*.patch, in name order: each says what it fixes and why), and writes the
headers pret's build generates into build/pokeheartgold/gen: text bank message IDs (files/msgdata/msg/msg_*.h) and NARC
member indexes (.naix). pret's own generators make them, run by meson with a header-only top-level meson.build (no
NitroSDK, no cross compiler, no ROM build: tools/ and files/ only); then the closure of what the C includes is copied
to gen/. Run again, it does nothing while the pinned commit and the patches are the same (.bwport-stamp).
tools/pokehg_cc.py runs it first. Needs the network once (the submodule, meson, pret's yyjson wrap).

VENDORED (third_party/pokeheartgold/PIN exists: the public tree, tools/hg_vendor.py): third_party/pokeheartgold is plain files,
the paths the port's build reads and gen/ already generated; they are copied into build/pokeheartgold (a git repo of
one commit, so the patches apply and `git diff` makes new ones as before), the patches applied, no generator run.

  python3 tools/pokehg_gen.py [--force]   # prints the working copy's path
  python3 tools/pokehg_gen.py --src DIR [--rev C] --work DIR2   # a decomp clone's commit (generators run) into DIR2
  HGSS_SRC=DIR HGSS_REV=C python3 tools/pokehg_gen.py              # the same (tools/hgss_repin.py sets them for meson)
"""
import argparse, glob, hashlib, json, os, re, shutil, subprocess, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pokegen4 as G4  # noqa: E402

ROOT = G4.ROOT
SUB = 'third_party/pokeheartgold'
WORK = os.path.join(ROOT, 'build/pokeheartgold')
PATCHES = os.path.join(ROOT, 'port/subprojects/pokeheartgold-*.patch')
# the C's include dirs (pret's public_includes, pokehg_cc.py's order) and the build dirs the generated headers sit in
SRC_DIRS = ['include', 'asm/include', 'lib/include', 'files', 'src', 'asm', '.', 'lib/spl/include', 'lib/NitroDWC/include']
GEN_DIRS = ['.', 'files']

# pret's top-level meson.build minus the ROM: its tools/ and files/ subdirs only. The NARCs built from C or asm data
# (json2bin.sh) get arm-none-eabi-gcc (DS_CC, through DS_ASM below for the asm ones) and an empty c_args, not
# mwcc/mwasm: only their .naix is used here
HEADER_MESON = """project('pokeheartgold', ['c', 'cpp'], meson_version: '>=1.12.0',
    default_options: ['buildtype=plain', 'warning_level=0'])
fs = import('fs')
public_includes = include_directories('include', 'asm/include', 'lib/include', 'files', 'src', 'asm')
toplevel_includes = include_directories('.')
relative_source_root = fs.relative_to(meson.project_source_root(), meson.project_build_root())
game_version = get_option('game_version')
c_args = []
subdir('tools')
subdir('files')
"""
# the asm data (zukan_data, zukan_enc, trpoke, safari_enc, headbutt: .s from jsonproc templates) is mwasm syntax: GNU as
# for ARM takes `%object` where mwasm has `@object` (`@` starts a comment) and no bare `.rodata`; and wants the
# preprocessor (#include, #ifdef) run on it. Only the symbols (the .naix member names, in order) matter here
DS_ASM = """#!/usr/bin/env python3
import os, re, subprocess, sys, tempfile
a = sys.argv[1:]
if '-c' in a and a[a.index('-c') + 1].endswith('.s'):
    i = a.index('-c') + 1
    src = open(a[i]).read().replace('@object', '%object').replace('@function', '%function')
    src = re.sub(r'(?m)^(\\s*)\\.rodata\\b', r'\\1.section .rodata', src)
    with tempfile.TemporaryDirectory() as t:
        p = os.path.join(t, 'x.S')
        open(p, 'w').write(src)
        a[i] = p
        sys.exit(subprocess.call(['arm-none-eabi-gcc', '-x', 'assembler-with-cpp'] + a))
sys.exit(subprocess.call(['arm-none-eabi-gcc'] + a))
"""
DS_CC = re.compile(r"' '\.join\(meson\.get_compiler\('(?:c|nasm)', native: false\)\.cmd_array\(\)\)"
                   r"|meson\.get_compiler\('(?:c|nasm)', native: false\)\.cmd_array\(\)\[0\]")
# the event scripts (assembled by the host's arm-none-eabi-gcc for their .naix) include the SDK's types.h only
TYPES_H = """#pragma once
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef unsigned long long u64;
typedef signed char s8; typedef signed short s16; typedef signed int s32; typedef signed long long s64;
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define NULL ((void *)0)
"""


def build(co):
    """pret's generators for the headers only, in co/hbuild. A target that fails (a NARC whose data needs the DS
    compiler) is reported and skipped: ninja -k 0."""
    open(os.path.join(co, 'meson.build'), 'w').write(HEADER_MESON)
    t = os.path.join(co, 'tools/meson.build')
    s = open(t).read()   # makelcf comes from the NitroSDK subproject: unused by the headers
    open(t, 'w').write(s.replace("find_program('makelcf', native: true)", "find_program('true')"))
    cc = os.path.join(co, '.bwport-ds-cc')
    open(cc, 'w').write(DS_ASM)
    os.chmod(cc, 0o755)
    for t in glob.glob(os.path.join(co, 'files/**/meson.build'), recursive=True):
        s = open(t).read()
        if DS_CC.search(s):
            open(t, 'w').write(DS_CC.sub("'%s'" % cc, s))
        s = open(t).read()
        if "'--compiler', 'arm-none-eabi-gcc'" in s:   # zukan_data and its like name the compiler themselves
            open(t, 'w').write(s.replace("'--compiler', 'arm-none-eabi-gcc'", "'--compiler', '%s'" % cc))
    for d in ['subprojects/NitroSDK-4.2.30001/include/nitro', 'subprojects/NitroSystem-071126.1/include']:
        os.makedirs(os.path.join(co, d), exist_ok=True)
    open(os.path.join(co, 'subprojects/NitroSDK-4.2.30001/include/nitro/types.h'), 'w').write(TYPES_H)
    env = dict(os.environ, TMPDIR=os.path.join(co, '.tmp'))
    os.makedirs(env['TMPDIR'], exist_ok=True)
    b = os.path.join(co, 'hbuild')
    if not os.path.exists(os.path.join(b, 'build.ninja')):
        subprocess.check_call(G4.meson(co) + ['setup', '--wrap-mode=nopromote', '--native-file=meson/native.ini',
                                              'hbuild'], cwd=co, env=env, stdout=subprocess.DEVNULL)
    r = subprocess.run(['ninja', '-C', b, '-k', '0', '-j', os.environ.get('BW_JOBS', '8')] + G4.header_targets(b),
                       env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, text=True)
    failed = [l.split(' ', 1)[1] for l in r.stdout.splitlines() if l.startswith('FAILED: ')]
    for f in failed:
        print('pokehg_gen: not generated:', f[:200], file=sys.stderr)
    return b


def resolver(co, b):
    naix = G4.naix_dirs(b, 'files')

    def resolve(h, f):
        """h's path relative to the build dir b, or None when the C tree co (or the SDK) has it."""
        if os.path.exists(os.path.join(os.path.dirname(f), h)) and not f.startswith(b + os.sep):
            return None
        if any(os.path.exists(os.path.join(co, d, h)) for d in SRC_DIRS):
            return None
        for d in GEN_DIRS:
            p = os.path.normpath(os.path.join(d, h))
            if os.path.isfile(os.path.join(b, p)):
                return p
        return naix.get(h) if '/' not in h else None
    return resolve


def overlay_ids(lsf):
    """gen/hg_overlay_ids.h: each overlay's number by its name in main.lsf.in (its order: the ROM's overlay table's). On the
    DS FS_OVERLAY_ID(name) is the address of the linker's SDK_OVERLAY_<name>_ID; port_hgss.h makes it this number, a
    constant a 64-bit host can put in a u32 initializer."""
    names = re.findall(r'(?m)^Overlay\s+(\S+)', lsf)
    return ('// Written by tools/pokehg_gen.py from main.lsf.in. Don\'t edit.\n#pragma once\nenum {\n'
            + ''.join(f'    HG_OVERLAY_{n} = {i},\n' for i, n in enumerate(names)) + '};\n')


def vendored():
    return os.path.isfile(os.path.join(ROOT, SUB, 'PIN'))


def vendored_id():
    """The vendored tree's identity for the stamp: its index entries and any edit not added yet."""
    h = hashlib.sha1(subprocess.check_output(['git', '-C', ROOT, 'ls-files', '-s', '--', SUB]))
    h.update(subprocess.check_output(['git', '-C', ROOT, 'diff', '--', SUB]))
    return 'vendored-' + h.hexdigest()


def vendored_checkout(work, patches):
    """work: a git repo of one commit holding third_party/pokeheartgold's files (gen/ included), patches applied."""
    git = ['git', '-C', work, '-c', 'user.name=pokehg_gen', '-c', 'user.email=pokehg_gen@localhost']
    if not os.path.isdir(os.path.join(work, '.git')):
        os.makedirs(work, exist_ok=True)
        subprocess.check_call(['git', 'init', '-q', work])
    # (an empty index, not `git rm`: pret's build leaves subprojects/meson-*, a nested clone, which `git add -A` below commits
    # as a gitlink with no .gitmodules entry, and the next `git rm` then fails; -ff so that clean removes nested clones too)
    subprocess.check_call(git + ['read-tree', '--empty'])
    subprocess.check_call(git + ['clean', '-ffdxq', '-e', '/hbuild', '-e', '/.bwport-stamp'])
    shutil.copytree(os.path.join(ROOT, SUB), work, dirs_exist_ok=True)
    subprocess.check_call(git + ['add', '-A', '.'])
    env = dict(os.environ, GIT_AUTHOR_DATE='2000-01-01T00:00:00Z', GIT_COMMITTER_DATE='2000-01-01T00:00:00Z')
    subprocess.check_call(git + ['commit', '-qm', 'vendored third_party/pokeheartgold', '--allow-empty'], env=env)
    for p in patches:
        subprocess.check_call(['git', '-C', work, 'apply', '--whitespace=nowarn', p])
    return G4.head(work)


def tree(force=False, mtimes=None, src=None, work=WORK, rev='HEAD'):
    """The working copy's path, made or brought up to date. A file whose content is what it was keeps its mtime (so
    ninja rebuilds only what changed): what it was before this run, or in mtimes (G4.snapshot's, from earlier).
    src: a decomp clone whose commit rev is used (generators run) instead of third_party/pokeheartgold."""
    vend = src is None and vendored()
    patches = sorted(glob.glob(PATCHES))
    if vend:
        commit = vendored_id()
    else:
        sub = os.path.abspath(src) if src else G4.submodule(SUB)
        commit = (subprocess.check_output(['git', '-C', sub, 'rev-parse', rev + '^{commit}'], text=True).strip() if src
                  else G4.pinned(SUB))
    st, sf = G4.stamp(commit, patches), os.path.join(work, '.bwport-stamp')
    # the stamp only says what was applied: a tool that moved the working copy since (hgss_repin.py's rebase checks out
    # the new pin in it; one that failed left it there, 11-15) must not pass for it: its HEAD has to be the pin too
    # (vendored: the commit vendored_checkout made, the stamp's second line)
    old = open(sf).read().split('\n') if os.path.exists(sf) else []
    if not force and old and old[0] == st and G4.head(work) == (old[1] if vend and len(old) > 1 else commit):
        if mtimes is not None:
            G4.keep_mtimes(work, mtimes)
        return work
    snap = mtimes if mtimes is not None else G4.snapshot(work)
    gen = os.path.join(work, 'gen')
    if vend:
        st += '\n' + vendored_checkout(work, patches)
    else:
        G4.checkout(work, sub, commit, patches)
        b = build(work)
        shutil.rmtree(gen, ignore_errors=True)
        # (and the data .s tools/pokehg_asm.py assembles: their #include of the message IDs)
        for p in G4.closure(work, b, ('src', 'include', 'lib', 'port/nonmatching', 'asm'), resolver(work, b),
                            ('.c', '.h', '.cpp', '.s')):
            os.makedirs(os.path.dirname(os.path.join(gen, p)), exist_ok=True)
            shutil.copyfile(os.path.join(b, p), os.path.join(gen, p))
    open(os.path.join(gen, 'hg_overlay_ids.h'), 'w').write(overlay_ids(open(os.path.join(work, 'main.lsf.in')).read()))
    abi_gate(work, patches)
    open(sf, 'w').write(st)
    G4.keep_mtimes(work, snap)
    return work


def abi_gate(work, patches):
    """with pokeheartgold-zzabi.patch applied (not while tools/hgss_repin.py has it aside), no declaration may be left
    narrower than its definition (tools/hg_abi_scan.py --gate: a re-vendor's new rows fail here, before the build)"""
    if not any(p.endswith('pokeheartgold-zzabi.patch') for p in patches):
        return
    import hg_abi_scan
    left = [r for r in hg_abi_scan.scan(work) if r[0] not in hg_abi_scan.exceptions()]
    if left:
        raise SystemExit('pokehg_gen: %d declarations narrower than their definitions (first: %s %s %s:%d): remake '
                         'port/subprojects/pokeheartgold-zzabi.patch (python3 tools/pokehg_abi.py) or list them in '
                         'tools/hg_abi_exceptions.txt' % (len(left), left[0][0], hg_abi_scan.what(left[0][1]),
                                                          left[0][2], left[0][3]))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--force', action='store_true', help='check out, patch and generate again')
    ap.add_argument('--mtimes', metavar='JSON', help="keep the mtimes of this G4.snapshot (tools/hgss_repin.py's) on "
                    "the files whose content is the same")
    ap.add_argument('--src', metavar='DIR', help='a decomp clone: its --rev instead of third_party/pokeheartgold')
    ap.add_argument('--rev', default='HEAD', help="--src's commit (default HEAD)")
    ap.add_argument('--work', metavar='DIR', default=WORK, help='the working copy (default build/pokeheartgold)')
    o = ap.parse_args()
    if not o.src and os.environ.get('HGSS_SRC'):   # tools/hgss_repin.py's rebase onwards (meson's runs too)
        o.src, o.rev = os.environ['HGSS_SRC'], os.environ.get('HGSS_REV', 'HEAD')
    print(tree(o.force, json.load(open(o.mtimes)) if o.mtimes else None, o.src, os.path.abspath(o.work), o.rev))


if __name__ == '__main__':
    main()
