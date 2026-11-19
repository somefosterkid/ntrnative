#!/usr/bin/env python3
"""tools/pokehg_cc.py: compile HeartGold/SoulSilver's C (the working copy build/pokeheartgold: the submodule
third_party/pokeheartgold plus the port's patches and the generated headers, tools/pokehg_gen.py, made first) as 64-bit
host C with the port's line for a game file (libntr's and lib/nnsys's headers and defines, from build/port/linux's
compile_commands.json), and classify the failures (docs/hgss.md, "The failure table"). The pattern is
tools/pokeplat_cc.py's (shared code: tools/pokegen4.py).

  python3 tools/pokehg_cc.py [--out DIR] [--obj] [--soulsilver] [FILES...]

The files: every .c that main.lsf.in links (src/, lib/) and port/nonmatching/*.c (the C the port uses for the
functions still in asm/). --obj compiles to objects in DIR/obj; the default is -fsyntax-only. Per-file logs in DIR/log/,
the summary in DIR/summary.txt (files OK / failed, errors by category and by normalized message). Default DIR:
build/pokehg_cc.
"""
import argparse, glob, os, re, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pokegen4 as G4  # noqa: E402

ROOT = G4.ROOT
HG = os.path.join(ROOT, 'build/pokeheartgold')   # tools/pokehg_gen.py's WORK


def nonmatching():
    """port/nonmatching's .c files: the C the port uses for the functions still in asm/ (port/meson.build links them)."""
    return sorted(glob.glob(os.path.join(HG, 'port/nonmatching/*.c')))


def asmdata():
    """port/asmdata's .c files (the decomp's PC-only typed copies of asm data with pointers, hk73: port/meson.build links
    them; tools/pokehg_asm.py's C_DATA makes the asm's labels local from port/asmdata/LABELS.txt)."""
    return sorted(glob.glob(os.path.join(HG, 'port/asmdata/*.c')))


def sources():
    """main.lsf.in's C objects (the files pret's build links; a .c it does not list is not part of the ROM) and
    port/nonmatching's functions."""
    lsf = open(os.path.join(HG, 'main.lsf.in')).read()
    s = {os.path.join(HG, f) for f in re.findall(r'Object\s+(\S+\.c)\b', lsf)}
    s |= set(nonmatching())
    return sorted(f for f in s if os.path.exists(f))


def own_args(game='HEARTGOLD'):
    """HGSS's part of the line: pret's include dirs and defines, the port's online headers and port_hgss.h."""
    a = ['-I' + os.path.join(HG, d) for d in ['include', 'asm/include', 'lib/include', 'files', 'src', 'asm', '.',
                                               'lib/spl/include', 'gen', 'gen/files']]
    # a few files include a .naix by its bare name (pret's meson gives each such target its dir)
    dirs = {dp for dp, dn, fn in os.walk(os.path.join(HG, 'gen/files')) if any(f.endswith('.naix') for f in fn)}
    a += ['-idirafter' + d for d in sorted(dirs)]
    a += ['-DPM_KEEP_ASSERTS', '-D' + game, '-fsigned-char',
          '-idirafter', os.path.join(ROOT, 'port/include'),
          # the online side (DWC, NitroWiFi, libvct) is stubbed, as Black's and Platinum's: the port's own headers
          # declare it (port/include_plat's dwc.h / vct.h shims are Gen 4's, shared)
          '-idirafter', os.path.join(ROOT, 'port/include_plat'),
          '-idirafter', os.path.join(ROOT, 'lib/dwc/include'),
          '-idirafter', os.path.join(ROOT, 'lib/dwc/include/dwc'),
          '-idirafter', os.path.join(ROOT, 'lib/nitrowifi/include'),
          '-include', 'port_hgss.h']
    return a


def hg_args(game='HEARTGOLD'):
    return (['-I' + os.path.join(ROOT, 'port/include_hgss'), '-I' + os.path.join(ROOT, 'port/include_gen4')]
            + G4.base_args() + own_args(game) + G4.DIAG)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default='build/pokehg_cc')
    ap.add_argument('--obj', action='store_true')
    ap.add_argument('--soulsilver', action='store_true', help='-DSOULSILVER instead of -DHEARTGOLD')
    ap.add_argument('--print-args', action='store_true')
    ap.add_argument('--meson-args', action='store_true', help="own_args(), one per line (port/meson.build)")
    ap.add_argument('--nonmatching', action='store_true', help="port/nonmatching's .c files (absolute), one per line (port/meson.build)")
    ap.add_argument('--asmdata', action='store_true', help="port/asmdata's .c files (absolute), one per line (port/meson.build)")
    ap.add_argument('files', nargs='*')
    o = ap.parse_args()
    if o.meson_args:
        print('\n'.join(own_args())); return
    if o.nonmatching:
        print('\n'.join(nonmatching())); return
    if o.asmdata:
        print('\n'.join(asmdata())); return
    import pokehg_gen
    pokehg_gen.tree()
    args = hg_args('SOULSILVER' if o.soulsilver else 'HEARTGOLD')
    if o.print_args:
        print(' '.join(args))
        return
    files = [os.path.abspath(f) for f in o.files] or sources()
    G4.compile_all(HG, args, files, o.out, o.obj, own='pokeheartgold')
    print(open(os.path.join(o.out, 'summary.txt')).read()[:3000])


if __name__ == '__main__':
    main()
