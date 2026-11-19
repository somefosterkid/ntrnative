#!/usr/bin/env python3
"""tools/port_relink.py: a game's objects as one relocatable object whose only global is its table (Black 2: b2_bwgame; White 2:
w2_bwgame), the GNU way (the PC's counterpart of port/meson.build's `clang -r -Wl,-exported_symbol` on macOS): the linker
(the build's C compiler as the driver: clang, with lld or GNU ld; ELF and PE) joins the archives' members with -r, then objcopy turns
every other global into a local, so no name clashes with Black's and the game's references bind to its own definitions or to what the
games share (libntr, the host).

  port_relink.py --cc 'clang --target=...' --objcopy llvm-objcopy --keep b2_bwgame -o b2.o libb2_game.a libb2_entry.a ...

macOS and iOS (--macho): ld64 does both in its -r link (-exported_symbol keeps the table, every other global becomes local);
the overlay markers are checked from `nm -m` (Mach-O sections: __DATA,__ovldata / __ovlbss, one pair for every game).
"""
import argparse
import os
import shlex
import subprocess
import sys


def split(cmd):
    """a command line meson joined; on Windows its paths have backslashes, which shlex would take as escapes"""
    return shlex.split(cmd.replace("\\", "/") if os.name == "nt" else cmd)


ap = argparse.ArgumentParser()
ap.add_argument("--cc", required=True, help="the compiler driver, with its target flags")
ap.add_argument("--objcopy", help="ELF and PE: objcopy (GNU or LLVM)")
ap.add_argument("--macho", action="store_true", help="macOS and iOS: ld64's -r link (no objcopy)")
ap.add_argument("--keep", required=True, help="the global symbol that stays (a comma-separated list: HeartGold's module keeps hg_bwgame and ss_bwgame)")
ap.add_argument("--nm", help="nm, for --tag and --ovl-tag")
ap.add_argument("--tag", help="PE: the prefix (b2, w2) of what the game defines (see below)")
ap.add_argument("--ovl-tag", help="the game's overlay sections' tag (.<TAG>ovldata): the output's overlay markers are checked"
                " (tools/port_overlays.py check_markers)")
ap.add_argument("-o", "--output", required=True)
ap.add_argument("inputs", nargs="+")
a = ap.parse_args()
keep = a.keep.split(",")
tmp = a.output + ".tmp.o"
if a.macho:
    cmd = split(a.cc) + ["-r", "-nostdlib", "-o", a.output] + ["-Wl,-exported_symbol,_" + k for k in keep] + [
        "-Wl,-all_load"] + a.inputs
    if subprocess.call(cmd) != 0:
        sys.exit("port_relink.py: the relocatable link failed")
cmd = split(a.cc) + ["-r", "-nostdlib", "-o", tmp, "-Wl,--whole-archive"] + a.inputs + ["-Wl,--no-whole-archive"]
extra = ["--remove-section=.llvm_addrsig"]   # a PE image takes it as a section with a virtual address of 0x200000000 and the Windows loader refuses the exe
if a.tag:
    # PE: GNU ld sorts an output section named .pdata (x64 unwind table: begin, end, unwind info) by its bytes, in a -r link
    # too, where the bytes are only addends and the relocations stay where they were: every entry then names another
    # object's function, and the exe's table points a game frame at the wrong unwind info. longjmp on Windows unwinds
    # through the game's frames by that table (the in-process restart, port/src/machine.c), so a restart from the game
    # ended the process with 0xC00000FF (bad function table). So the -r link puts the entries in .bwpdata, which it leaves
    # in order, and objcopy names it .pdata again; the exe's own link sorts the whole table after relocating it.
    with open(tmp + ".ld", "w") as f:
        f.write("SECTIONS { .bwpdata : { *(.pdata) *(.pdata$*) } } INSERT AFTER .xdata;" + chr(10))
    cmd += ["-Wl,-T," + tmp + ".ld"]
    extra += ["--rename-section=.bwpdata=.pdata"]
if not a.macho and subprocess.call(cmd) != 0:
    sys.exit("port_relink.py: the relocatable link failed")
if a.tag:
    os.unlink(tmp + ".ld")
if a.tag:
    # PE: the compiler reaches every extern datum through a COMDAT pointer section `.rdata$.refptr.X` (symbol `.refptr.X`), and the
    # linker keeps one section of each name: Black 2's pointer to ITS X would be dropped for Black's (or White 2's), and its code
    # would read the wrong game's datum. So each name the game defines, and its pointer's section and symbols, get the game's
    # tag (b2_X, .rdata$.refptr.b2_X); the externs it shares with the others (libntr, the host) keep one pointer section.
    out = subprocess.run(split(a.nm) + [tmp], capture_output=True, text=True, check=True).stdout.splitlines()
    syms = [l.split(None, 2) for l in out if len(l.split(None, 2)) == 3]
    defined = {n for t, k, n in ((x[0], x[1], x[2]) for x in syms) if k != "U" and k.isupper() and not n.startswith(".")} - set(keep)
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
if not a.macho:
    if subprocess.call(split(a.objcopy) + extra + ["--keep-global-symbol=" + k for k in keep] + [tmp, a.output]) != 0:
        sys.exit("port_relink.py: objcopy failed")
    os.unlink(tmp)
if a.tag:
    os.unlink(tmp + ".map")
if a.ovl_tag:
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import port_overlays
    if a.macho:   # `nm -m`'s "ADDR (__DATA,__ovldata) ... _NAME" as the sysv lines check_markers reads (.<tag>ovldata)
        nm = ""
        for line in subprocess.run(split(a.nm) + ["-m", a.output], capture_output=True, text=True, check=True).stdout.splitlines():
            f = line.split()
            if len(f) >= 3 and f[1].startswith("(") and f[-1].startswith("_"):
                sect = f[1].strip("()").split(",")[-1]
                sect = "." + a.ovl_tag + sect[len("__"):] if sect in ("__ovldata", "__ovlbss") else sect
                nm += f"{f[-1][1:]}|{f[0]}|s||||{sect}\n"
    else:
        nm = subprocess.run(split(a.nm) + ["-f", "sysv", a.output], capture_output=True, text=True, check=True).stdout
    bad = port_overlays.check_markers(nm, a.ovl_tag)
    if bad:
        os.unlink(a.output)
        sys.exit("port_relink.py: " + a.output + ": the overlay markers are wrong, port/src/ovl.c would reset the wrong bytes:\n  "
                 + "\n  ".join(bad))
