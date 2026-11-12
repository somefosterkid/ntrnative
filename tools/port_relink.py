#!/usr/bin/env python3
"""tools/port_relink.py: a game's objects as one relocatable object whose only global is its table (Black 2: b2_bwgame; White 2:
w2_bwgame), the GNU way (the PC's counterpart of port/meson.build's `clang -r -Wl,-exported_symbol` on macOS): the linker
(the build's C compiler as the driver: clang, with lld or GNU ld; ELF and PE) joins the archives' members with -r, then objcopy turns
every other global into a local, so no name clashes with Black's and the game's references bind to its own definitions or to what the
games share (libntr, the host).

  port_relink.py --cc 'clang --target=...' --objcopy llvm-objcopy --keep b2_bwgame -o b2.o libb2_game.a libb2_entry.a ...
"""
import argparse
import os
import shlex
import subprocess
import sys

ap = argparse.ArgumentParser()
ap.add_argument("--cc", required=True, help="the compiler driver, with its target flags")
ap.add_argument("--objcopy", required=True)
ap.add_argument("--keep", required=True, help="the one global symbol that stays")
ap.add_argument("--nm", help="PE: nm, to rename what the game defines (see below); with --tag")
ap.add_argument("--tag", help="PE: the prefix (b2, w2)")
ap.add_argument("-o", "--output", required=True)
ap.add_argument("inputs", nargs="+")
a = ap.parse_args()
tmp = a.output + ".tmp.o"
cmd = shlex.split(a.cc) + ["-r", "-nostdlib", "-o", tmp, "-Wl,--whole-archive"] + a.inputs + ["-Wl,--no-whole-archive"]
if subprocess.call(cmd) != 0:
    sys.exit("port_relink.py: the relocatable link failed")
extra = ["--remove-section=.llvm_addrsig"]   # a PE image takes it as a section with a virtual address of 0x200000000 and the Windows loader refuses the exe
if a.nm:
    # PE: the compiler reaches every extern datum through a COMDAT pointer section `.rdata$.refptr.X` (symbol `.refptr.X`), and the
    # linker keeps one section of each name: Black 2's pointer to ITS X would be dropped for Black's (or White 2's), and its code
    # would read the wrong game's datum. So each name the game defines, and its pointer's section and symbols, get the game's
    # tag (b2_X, .rdata$.refptr.b2_X); the externs it shares with the others (libntr, the host) keep one pointer section.
    out = subprocess.run(shlex.split(a.nm) + [tmp], capture_output=True, text=True, check=True).stdout.splitlines()
    syms = [l.split(None, 2) for l in out if len(l.split(None, 2)) == 3]
    defined = {n for t, k, n in ((x[0], x[1], x[2]) for x in syms) if k != "U" and k.isupper() and not n.startswith(".")} - {a.keep}
    names = {n for _, _, n in syms}
    mp = []
    for n in sorted(defined):
        mp.append(f"{n} {a.tag}_{n}")
        for pre in (".refptr.", ".rdata$.refptr."):
            if pre + n in names:
                mp.append(f"{pre}{n} {pre}{a.tag}_{n}")
    with open(tmp + ".map", "w") as f:
        f.write(chr(10).join(mp) + chr(10))
    extra += ["--redefine-syms=" + tmp + ".map"]
    # (the pointer of an extern the games share stays a global: the linker finds the one kept section through it)
    for n in sorted(names):
        if n.startswith(".refptr.") and n[len(".refptr."):] not in defined:
            extra.append("--keep-global-symbol=" + n)
    for n in sorted(defined):
        if ".rdata$.refptr." + n in names:
            extra.append(f"--rename-section=.rdata$.refptr.{n}=.rdata$.refptr.{a.tag}_{n}")
if subprocess.call(shlex.split(a.objcopy) + extra + ["--keep-global-symbol=" + a.keep, tmp, a.output]) != 0:
    sys.exit("port_relink.py: objcopy failed")
os.unlink(tmp)
if a.nm:
    os.unlink(tmp + ".map")
