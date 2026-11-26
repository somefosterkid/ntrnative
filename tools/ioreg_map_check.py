#!/usr/bin/env python3
"""Check the I/O register map of libntr (include/simulator/ioreg_map.h) against the ioreg_*.h headers.

Every s_reg_* name must sit at the REG_x_OFFSET of each REG_x_ADDR that points to it, be aligned to its size, lie
inside its page, and overlap another name only as a whole part of it (BG0HOFS in BG0OFS); no s_reg_* may be left as
a separate global. Exit 1 with one line per problem."""
import glob, os, re, sys

L = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'port', 'subprojects', 'libntr')
PAGE = {'9': (0, 0x2000), '9X': (0x100000, 0x20), '7': (0, 0x600)}


def main():
    bad = []
    m = {}
    text = open(os.path.join(L, 'include/simulator/ioreg_map.h')).read()
    for x in re.finditer(r'^#define\s+(s_reg_\w+)\s+NTR_IOREG(9X|9|7)\(REGType(\d+)v?, (0x[0-9a-f]+)\)', text, re.M):
        m[x.group(1)] = (x.group(2), int(x.group(4), 16), int(x.group(3)) // 8)
    for name, (pg, ofs, size) in m.items():
        if ofs % size or ofs + size > PAGE[pg][1]:
            bad.append('%s: offset 0x%x, size %d, page %s' % (name, ofs, size, pg))
    for pg in PAGE:
        rows = sorted((o, -s, n) for n, (p, o, s) in m.items() if p == pg)
        for i, (o, s, n) in enumerate(rows):
            for o2, s2, n2 in rows[i + 1:]:
                if o2 >= o - s:
                    break
                if o2 - s2 > o - s:
                    bad.append('%s and %s overlap without one holding the other' % (n, n2))
    nref = 0
    for d in ('include/nitro/hw/X86', 'include/twl/hw/X86'):
        off = {}
        addr = []
        for f in sorted(glob.glob(os.path.join(L, d, 'ioreg_*.h'))):
            t = open(f, errors='replace').read()
            off.update((x.group(1), int(x.group(2), 16)) for x in re.finditer(r'^#define\s+REG_(\w+)_OFFSET\s+(0x[0-9a-fA-F]+)', t, re.M))
            addr += re.findall(r'^#define\s+REG_(\w+)_ADDR\s+\((?:\(u8 \*\))?&(s_reg_\w+)\)', t, re.M)
        for reg, name in addr:
            nref += 1
            if name not in m:
                bad.append('%s: REG_%s_ADDR points to %s, which is not in the map' % (d, reg, name))
            elif m[name][1] + PAGE[m[name][0]][0] != off.get(reg):
                bad.append('%s: %s is at 0x%x in the map, REG_%s_OFFSET is %s' % (d, name, m[name][1], reg, off.get(reg)))
    for root, _, files in os.walk(L):
        for f in files:
            if f.endswith(('.h', '.c', '.cpp', '.inc')):
                t = open(os.path.join(root, f), errors='replace').read()
                for x in re.finditer(r'^(?:extern\s+)?REGType\w+\s+(s_reg_\w+)', t, re.M):
                    bad.append('%s: %s is a separate global' % (f, x.group(1)))
    for b in bad:
        print('ioreg_map_check:', b)
    print('ioreg_map_check: %d names, %d REG_x_ADDR checked, %d problems' % (len(m), nref, len(bad)))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
