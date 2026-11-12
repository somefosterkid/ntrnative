#!/usr/bin/env python3
"""Write a C table's initializer from Black's bytes and relocations: values by the element type, pointers by Black's
symbol names, enumerators where the type is an enum.

    python3 tools/gen_table.py src/ov206/zukan_info.c data_ov206_021bc184           # a table of the C, its own type
    python3 tools/gen_table.py src/ov206/zukan_info.c sLabelMessages --at 0x021bc184   # Black's table at an address
    python3 tools/gen_table.py src/ov211/zukan_open.c --type BGSetup --at 0x021bc8f0 --count 5   # any type of the C

The type is the table's in the file's object (its debug info, ninja builds it), or with --type a type the C file sees
(the file is compiled with `<type> gen_table_[<count>];` added). Black's table is at --at, else at the address in its
name (data_ov206_021bc184), else at the address of the name in symbols.txt; the count is --count, else the C's, else as
many as Black's datum holds (to the next symbol). A word that a relocation of Black's covers is the name of its target
(plus the offset into it), a zero pointer NULL; a value fits its member's type or is printed as a hex constant with a
comment. Floats are hex words with a comment, bit fields their values. Print only: paste it over the old initializer.
"""
import argparse
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from data_order import FLAGS, INCLUDES, module_of  # noqa: E402
from port_file import CONFIG, ROOT, debug_types, read_names, read_object, size_of  # noqa: E402


def compile_type(path, ctype, count):
    """The debug type of `ctype gen_table_[count]` in the C file's context."""
    with tempfile.TemporaryDirectory() as tmp:
        c, o = os.path.join(tmp, os.path.basename(path)), os.path.join(tmp, "t.o")
        open(c, "w").write(open(f"{ROOT}/{path}").read() + f"\n{ctype} gen_table_[{count}];\n")
        r = subprocess.run([f"{ROOT}/tools/wibo", f"{ROOT}/tools/mwccarm/dsi/1.1p1/mwccarm.exe"] + FLAGS + INCLUDES
                           + ["-i", os.path.dirname(f"{ROOT}/{path}"), "-o", o, c], capture_output=True, text=True,
                           cwd=ROOT)
        if r.returncode:
            sys.exit(r.stdout + r.stderr)
        return debug_types(*read_object(o)).get("gen_table_")


def module_config(mod):
    return CONFIG if mod == "main" else f"{CONFIG}/overlays/{mod}"


class Black:
    def __init__(self, mod):
        self.mod = mod
        text = open(f"{module_config(mod)}/delinks.txt").read()
        self.base = int(re.search(r"\.text\s+start:0x(\w+)", text)[1], 16)
        self.rom = open(f"{ROOT}/extract/b_us/" + ("arm9/arm9.bin" if mod == "main" else f"arm9_overlays/{mod}.bin"),
                        "rb").read()
        self.at, self.where = read_names(CONFIG)
        self.relocs = {}  # from -> (to, module or None)
        for line in open(f"{module_config(mod)}/relocs.txt"):
            m = re.match(r"from:(0x\w+) kind:\S+ to:(0x\w+)(?: add:(\S+))? module:(\S+)", line)
            if m:
                # overlays(a,b,...): dsd links to the first, whose symbol the name is (not the table's own module's
                # symbol before it + an offset: `(u8 *)data_... + 0x43831`, t35r)
                tmod = re.match(r"overlays?\((\d+)[,)]", m[4])
                tmod = f"ov{int(tmod[1]):03d}" if tmod else m[4] if m[4] in ("main", "itcm", "dtcm") else mod
                self.relocs[int(m[1], 16)] = (int(m[2], 16) + int(m[3] or "0", 16), tmod)
        self.syms = sorted(a for (m, a) in self.at if m == mod)

    def bytes(self, addr, size):
        return self.rom[addr - self.base:addr - self.base + size]

    def name(self, to, tmod):
        """The symbol at to, or the one before it plus the offset, in its module."""
        if (tmod, to) in self.at or (tmod, to & ~1) in self.at:
            return self.at.get((tmod, to)) or self.at[tmod, to & ~1]
        before = max((a for (m, a) in self.at if m == tmod and a <= to), default=None)
        return f"(u8 *){self.at[tmod, before]} + {to - before:#x}" if before is not None else f"{to:#010x}"

    def datum_end(self, addr):
        later = [a for a in self.syms if a > addr]
        return min(later) if later else addr


def value(w, size, signed, enums):
    if signed and w >= 1 << (size * 8 - 1):
        w -= 1 << (size * 8)
    if w in enums:
        return enums[w]
    return str(w) if -10 < w < 10 or 0 < w < 0x100 and size == 1 else f"-{-w:#x}" if w < 0 else f"{w:#x}" \
        if w >= 0x1000 else str(w)


def render(t, black, addr, depth=0):
    """The initializer of a t at addr, as text."""
    if t is None:
        return "/* ? */"
    kind = t[0]
    if kind == "array":
        n, size = t[2], size_of(t[1])
        items = [render(t[1], black, addr + k * size, depth + 1) for k in range(n)]
        flat = "{ " + ", ".join(items) + " }"
        if t[1] and t[1][0] in ("int", "pointer") and len(flat) + 4 * depth < 100:
            return flat
        if t[1] and t[1][0] in ("int", "pointer"):
            lines, line = [], ""
            for it in items:
                if line and len(line) + len(it) + 2 > 112 - 4 * depth:
                    lines.append(line.rstrip())
                    line = ""
                line += it + ", "
            lines.append(line.rstrip())
            pad = "    " * (depth + 1)
            return "{\n" + "".join(f"{pad}{l}\n" for l in lines) + "    " * depth + "}"
        pad = "    " * (depth + 1)
        return "{\n" + "".join(f"{pad}{it},\n" for it in items) + "    " * depth + "}"
    if kind in ("struct", "union"):
        members = t[2] if kind == "struct" else t[2][:1]
        out = []
        for at, mt, bits, top in members:
            if bits:
                word = int.from_bytes(black.bytes(addr + at, mt[1]), "little")
                low = mt[1] * 8 - top - bits
                out.append(value((word >> low) & ((1 << bits) - 1), 8, False, mt[3] if mt[0] == "int" else {}))
            else:
                out.append(render(mt, black, addr + at, depth + 1))
        return "{ " + ", ".join(out) + " }"
    if kind == "pointer":
        if addr in black.relocs:
            return black.name(*black.relocs[addr])
        w = int.from_bytes(black.bytes(addr, 4), "little")
        return "NULL" if not w else f"{w:#010x} /* no relocation */"
    if kind == "float":
        return f"{int.from_bytes(black.bytes(addr, t[1]), 'little'):#x} /* float's bits */"
    size, signed, enums = t[1], t[2], t[3]
    if addr in black.relocs and size == 4:
        return f"(u32){black.name(*black.relocs[addr])}"
    return value(int.from_bytes(black.bytes(addr, size), "little"), size, signed, enums)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("file", help="the C file (src/...c), for its types")
    ap.add_argument("name", nargs="?", help="a table the C defines, or Black's name of one")
    ap.add_argument("--type", help="the element type, as the C spells it")
    ap.add_argument("--at", help="Black's address of the table")
    ap.add_argument("--count", type=int, help="how many elements")
    a = ap.parse_args()
    mod = module_of(a.file)
    black = Black(mod)
    addr = int(a.at, 16) if a.at else None
    if addr is None and a.name:
        m = re.search(r"_([0-9a-f]{8})$", a.name)
        addr = int(m[1], 16) if m else next((x for m_, x in black.where.get(a.name, []) if m_ == mod), None)
    if addr is None:
        sys.exit("where is Black's table? --at")
    if a.type:
        elem = compile_type(a.file, a.type, 1)[1]
    else:
        types = debug_types(*read_object(f"{ROOT}/build/b_us/{a.file[:-2]}.o"))
        t = types.get(a.name)
        if t is None:
            sys.exit(f"{a.name}: no such variable in the object's debug info (a static nothing uses is dropped): --type")
        elem = t[1] if t[0] == "array" else t
        if t[0] == "array" and not a.count:
            a.count = t[2]
        elif t[0] != "array":
            print(render(t, black, addr))
            return
    size = size_of(elem)
    count = a.count or max(1, (black.datum_end(addr) - addr) // size)
    print(render(("array", elem, count), black, addr))
    end = black.datum_end(addr)
    if end > addr + count * size:
        print(f"// Black's datum runs {end - addr - count * size:#x} bytes further, to {end:#x}")


if __name__ == "__main__":
    main()
