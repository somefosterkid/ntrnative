#!/usr/bin/env python3
"""Compare the boots of BWPORT_MACHINE_CHECK logs (port/src/machine.c): one line per V-blank wait of the game's main loop,
`RESETS VBLANK range:hash ...`. With one log, each boot after a reset is compared with the first (the cold boot); with
two, the first boot of each (two cold boots: how repeatable a boot is at all). Prints, per pair, the frames compared
and the first frame where each range differs.

    tools/mac_reset_check.py RUN/check.txt [--frames N]
    tools/mac_reset_check.py A/check.txt B/check.txt [--frames N]
    tools/mac_reset_check.py --dump RUN EXE V [BOOT]  (a run with BWPORT_MACHINE_DUMP=V,...: the symbols whose bytes differ
                                                       at V-blank V between the cold boot and boot BOOT, default 1)
"""
import argparse
import bisect
import os
import re
import subprocess
import sys


def dump_diff(run, exe, vb, boot):
    starts = {}
    for line in open(os.path.join(run, "log.txt"), errors="replace"):
        m = re.match(r"machine: range (\S+) at (0x[0-9a-f]+), (\d+) bytes", line)
        if m:
            starts[m.group(1)] = (int(m.group(2), 16), int(m.group(3)))
    nm = subprocess.run(["nm", "-n", exe], capture_output=True, text=True).stdout.split("\n")
    syms = []
    for line in nm:
        p = line.split()
        if len(p) == 3 and p[1].lower() in "dbs":
            syms.append((int(p[0], 16), p[2]))
    syms.sort()
    addrs = [a for a, _ in syms]
    link = {}
    if sys.platform == "darwin":
        # the slide: machdata's runtime start against the link's (section$start is not in nm: take otool's section address)
        ot = subprocess.run(["otool", "-l", exe], capture_output=True, text=True).stdout.split(chr(10))
        for i, line in enumerate(ot):
            m = re.match(r"\s+sectname (__\w+)", line)
            if m and i + 2 < len(ot) and "__DATA" in ot[i + 1]:
                link[m.group(1)[2:]] = int(ot[i + 2].split()[1], 16)
    else:
        # Linux, Windows, Android (a linker script, tools/port_overlays.py): the range symbols are in the symbol table
        for line in nm:
            p = line.split()
            if len(p) == 3 and p[2] in ("__bwmach_data0", "__bwmach_bss0", "__bwovl_data", "__bwovl_bss"):
                link[{"__bwmach_data0": "machdata", "__bwmach_bss0": "machbss", "__bwovl_data": "ovldata",
                      "__bwovl_bss": "ovlbss"}[p[2]]] = int(p[0], 16)
    slide = starts["machdata"][0] - link["machdata"]
    for name, (start, size) in sorted(starts.items()):
        a, b = os.path.join(run, f"dump0_{vb}_{name}.bin"), os.path.join(run, f"dump{boot}_{vb}_{name}.bin")
        if not (os.path.exists(a) and os.path.exists(b)):
            continue
        da, db = open(a, "rb").read(), open(b, "rb").read()
        bysym = {}
        for off in range(min(len(da), len(db))):
            if da[off] != db[off]:
                addr = start + off - slide
                k = bisect.bisect_right(addrs, addr) - 1
                sym = syms[k][1] if k >= 0 else "?"
                n = bysym.setdefault(sym, [0, addr - (syms[k][0] if k >= 0 else 0)])
                n[0] += 1
        print(f"{name}: {sum(v[0] for v in bysym.values())} bytes differ in {len(bysym)} symbols")
        for sym, (n, first) in sorted(bysym.items(), key=lambda x: -x[1][0])[:40]:
            print(f"  {sym} {n} bytes (first at +0x{first:x})")


def boots(path):
    out = {}
    with open(path) as f:
        for line in f:
            p = line.split()
            if len(p) < 3 or not all(":" in x for x in p[2:]):
                continue   # (a line cut short at the end of the run)
            boot, vb = int(p[0]), int(p[1])
            out.setdefault(boot, {})
            out[boot].setdefault(vb, dict(x.split(":", 1) for x in p[2:]))   # the first wait of a V-blank
    return out


def compare(name, a, b, frames):
    common = sorted(set(a) & set(b))
    if frames:
        common = [f for f in common if f < frames]
    first = {}
    for f in common:
        for r, h in a[f].items():
            if b[f].get(r) != h and r not in first:
                first[r] = f
    print(f"{name}: {len(common)} frames compared (V-blank {common[0] if common else '-'}..{common[-1] if common else '-'})", end="")
    if not first:
        print(": identical")
        return True
    print(": differ: " + ", ".join(f"{r} from V-blank {f}" for r, f in sorted(first.items(), key=lambda x: x[1])))
    return False


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("logs", nargs="+")
    ap.add_argument("--dump", action="store_true")
    ap.add_argument("--frames", type=int, default=0)
    a = ap.parse_args()
    if a.dump:
        dump_diff(a.logs[0], a.logs[1], int(a.logs[2]), int(a.logs[3]) if len(a.logs) > 3 else 1)
        return
    ok = True
    if len(a.logs) == 1:
        b = boots(a.logs[0])
        for k in sorted(b):
            if k != 0:
                ok &= compare(f"boot {k} against the cold boot", b[0], b[k], a.frames)
    else:
        b0, b1 = boots(a.logs[0]), boots(a.logs[1])
        ok &= compare("cold boot A against cold boot B", b0[0], b1[0], a.frames)
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
