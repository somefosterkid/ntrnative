#!/usr/bin/env python3
"""tools/pokeplat_cc.py: compile pret pokeplatinum's C (the working copy build/pokeplatinum: the submodule third_party/pokeplatinum
plus the port's patches and the generated headers, tools/pokeplat_gen.py, made first) as 64-bit host C
with the port's line for a game file (libntr's and lib/nnsys's headers and defines, from build/port/linux's
compile_commands.json), and classify the failures (docs/platinum.md, "The failure table").

  python3 tools/pokeplat_cc.py [--out DIR] [--obj] [FILES...]   # default: every src/**/*.c and lib/{spl,gds,crypto}

--obj compiles to objects in DIR/obj (for a link); the default is -fsyntax-only. The tables' values (asset rows,
config/plat_us/assets.tsv) are written first to build/plat_us/asset from orig/baserom_plat_us.nds (tools/rom_modules.py). Per-file logs in DIR/log/, the
summary in DIR/summary.txt (files OK / failed, errors by category and by normalized message).
"""
import argparse, os, re, subprocess, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pokegen4 as G4  # noqa: E402  (shared with tools/pokehg_cc.py)
from pokegen4 import base_args  # noqa: E402

ROOT = G4.ROOT
PLAT = os.path.join(ROOT, 'build/pokeplatinum')   # tools/pokeplat_gen.py's WORK


def sources():
    # src/*.c that pret's platinum.us/main.lsf.in does not list are not part of its ROM (src/unk_02095CD4.c is a
    # stale file: its header and struct_02095E80_decl.h do not exist): not compiled here either
    lsf = open(os.path.join(PLAT, 'platinum.us/main.lsf.in')).read()
    built = set(re.findall(r'Object\s+(\S+\.c)', lsf))
    s = []
    for top in ['src', 'lib/spl/src', 'lib/gds/src', 'lib/crypto/src']:
        for dp, dn, fn in os.walk(os.path.join(PLAT, top)):
            s += [os.path.join(dp, f) for f in fn if f.endswith('.c')
                  and (top != 'src' or os.path.relpath(os.path.join(dp, f), PLAT) in built)]
    return sorted(s)


def plat_args():
    return (['-I' + os.path.join(ROOT, 'port/include_plat'), '-I' + os.path.join(ROOT, 'port/include_gen4')] + base_args()
            + own_args() + G4.DIAG)


def own_args():
    """Platinum's part of the line: pret's include dirs and defines, the port's online headers and port_plat.h
    (port/meson.build's Platinum module puts them after Black's flags: --meson-args)."""
    a = []
    for d in ['include', 'gen', '.', 'lib/spl/include', 'lib/gds/include', 'lib/crypto/include',
              'lib/ppwlobby/include', 'gen/res/text', 'gen/generated']:
        a.append('-I' + os.path.join(PLAT, d))
    # a few files include a .naix by its bare name (pret's meson gives each such target its dir)
    dirs = set()
    for dp, dn, fn in os.walk(os.path.join(PLAT, 'gen/res')):
        if any(f.endswith('.naix') for f in fn):
            dirs.add(dp)
    a += ['-idirafter' + d for d in sorted(dirs)]
    a += ['-DPOKEPLATINUM_GENERATED_ENUM', '-DPM_KEEP_ASSERTS', '-DGAME_VERSION=VERSION_PLATINUM',
          '-DGAME_LANGUAGE=LANGUAGE_ENGLISH', '-DPOKEPLATINUM_REVISION=1', '-fsigned-char',
          '-idirafter', os.path.join(ROOT, 'port/include'),
          # the tables' values: asset/<name>.inc of config/plat_us/assets.tsv, written from the ROM (main())
          '-idirafter', os.path.join(ROOT, 'build/plat_us'),
          # the online side (DWC, NitroWiFi, libvct) is stubbed, as Black's: the port's own headers declare it
          '-idirafter', os.path.join(ROOT, 'lib/dwc/include'),
          '-idirafter', os.path.join(ROOT, 'lib/dwc/include/dwc'),
          '-idirafter', os.path.join(ROOT, 'lib/nitrowifi/include'),
          # port_plat.h's FS_OVERLAY_ID constants (port_overlays.py plat_id_symbols; written by the linux-plat build)
          '-idirafter', os.path.join(ROOT, 'build/port/linux-plat/ovlgen_pl'),
          '-include', 'port_plat.h']
    return a


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default='build/pokeplat_cc')
    ap.add_argument('--obj', action='store_true')
    ap.add_argument('--print-args', action='store_true')
    ap.add_argument('--meson-args', action='store_true', help="own_args(), one per line (port/meson.build)")
    ap.add_argument('files', nargs='*')
    o = ap.parse_args()
    if o.meson_args:
        print('\n'.join(own_args())); return
    import pokeplat_gen
    pokeplat_gen.tree()
    args = plat_args()
    if o.print_args:
        print(' '.join(args)); return
    files = [os.path.abspath(f) for f in o.files] or sources()
    # the asset rows' .inc from orig/baserom_plat_us.nds (as port/meson.build's step will for a linked Platinum)
    subprocess.run([sys.executable, os.path.join(ROOT, 'tools/rom_modules.py'), '--assets', os.path.join(ROOT, 'orig'),
                    '--version', 'plat_us'], check=True)
    G4.compile_all(PLAT, args, files, o.out, o.obj, own='pokeplatinum')
    print(open(os.path.join(o.out, 'summary.txt')).read()[:3000])


if __name__ == '__main__':
    main()
