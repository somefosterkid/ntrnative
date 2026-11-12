#!/usr/bin/env python3
"""The data the PC build (port/, docs/port-build.md) needs that the ROM build takes from the base ROM.

The ROM build links some bytes no C file owns: the assets the C includes (config/<version>/assets.tsv), the gaps
between the files' ranges (`_dsd_gap@*` objects) and the incomplete files (delinks.txt without `complete`: their
delinked objects, not their C). The PC build compiles the C only. This tool says what of that the PC build's files
reach, and writes what it can from the extracted base ROM (extract/<version>/), never into the repo.

    python3 tools/port_data.py assets                 # the assets the PC files include, each with its assets.tsv row
    python3 tools/port_data.py gaps                   # ROM-only data the PC files use (exit 1 if any; ninja's objects)
    python3 tools/port_data.py gap-c OUT.c            # that data as C, from the base ROM (none today: not built)
    python3 tools/port_data.py incomplete [FILE...]   # what the incomplete PC files' C lacks against their ROM bytes

The PC files are tools/port_sources.py's (the PC build's sources less port/src) if that tool is there, else every C
file under src/ and lib/ but lib/nitro, lib/twl, lib/nnsys, lib/dwc*, lib/nitrowifi, lib/dsprot and lib/msl (the
libraries libntr, libntrsystem or the port's stubs stand in for). `gaps` and `incomplete` read the ROM build's
objects (build/<version>/: the compiled C and build/<version>/delinks/ for the ROM's), so run `ninja` first.

`incomplete` compares, for each incomplete PC file, the symbols its delinked object (the ROM's bytes, symbols.txt's
names) defines with those of its compiled C: functions the C lacks, data the C lacks, data of another size, and data
whose bytes differ from the ROM's (relocated words aside, in both). Data the C names otherwise (MWCC's @123 labels for
string literals and float constants) is not compared. A missing symbol some PC file references is one the PC link
lacks (`undefined`); the rest are only bytes the PC build doesn't have.
"""
import argparse
import re
import struct
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import asset  # noqa: E402

NOT_COMPILED = ("lib/nitro/", "lib/twl/", "lib/nnsys/", "lib/dwc", "lib/nitrowifi/", "lib/dsprot/", "lib/msl/")
LINKER_MADE = re.compile(r"^SDK_[A-Z0-9_]+$")  # the linker script's (port/src/linker_syms.c), not data


# ---- objects ----------------------------------------------------------------------------------------------------------

class Obj:
    """An ELF32 object's sections, symbols and relocations."""

    def __init__(self, path):
        data = Path(path).read_bytes()
        if data[:5] != b"\x7fELF\x01":
            raise ValueError(f"{path}: not an ELF32 object")
        shoff, = struct.unpack_from("<I", data, 0x20)
        size, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2E)
        self.secs = [struct.unpack_from("<10I", data, shoff + i * size) for i in range(shnum)]
        self.data = data

        def string(tab, off):
            o = self.secs[tab][4] + off
            return data[o:data.index(b"\0", o)].decode(errors="replace")

        self.names = [string(shstrndx, s[0]) for s in self.secs]
        st = next(i for i, s in enumerate(self.secs) if s[1] == 2)
        self.syms = []
        for k in range(self.secs[st][5] // 16):
            nm, val, sz, info, _, shndx = struct.unpack_from("<IIIBBH", data, self.secs[st][4] + k * 16)
            self.syms.append((string(self.secs[st][6], nm) if nm else "", val, sz, info >> 4, info & 15, shndx))
        self.relocs = defaultdict(dict)  # section index: {offset: symbol name}
        for s in self.secs:
            if s[1] not in (4, 9) or not 0 < s[7] < shnum:
                continue
            ent = 12 if s[1] == 4 else 8
            for k in range(s[5] // ent):
                off, info = struct.unpack_from("<II", data, s[4] + k * ent)
                self.relocs[s[7]][off] = self.syms[info >> 8][0] if info >> 8 < len(self.syms) else ""

    def section_bytes(self, i):
        s = self.secs[i]
        return bytes(s[5]) if s[1] == 8 else self.data[s[4]:s[4] + s[5]]

    def defined(self):
        """{name: (kind, section name, size, masked bytes or None for .bss, sorted relocated offsets)} of the named
        functions and data (not $a/$t/$d, .L_ or section symbols). A size of 0 (dsd's) runs to the next symbol."""
        by_sec = defaultdict(list)
        for name, val, sz, bind, typ, shndx in self.syms:
            if shndx and shndx < 0xFF00 and name and typ in (0, 1, 2) and not name.startswith(("$", ".L")):
                by_sec[shndx].append((val, sz, name, typ, bind))
        out = {}
        for shndx, syms in by_sec.items():
            sec = self.secs[shndx]
            if not sec[2] & 2:  # not SHF_ALLOC
                continue
            code = bool(sec[2] & 4)
            raw = None if code else self.section_bytes(shndx)
            starts = sorted({v for v, *_ in syms})
            for val, sz, name, typ, bind in syms:
                if not sz:
                    nxt = next((s for s in starts if s > val), sec[5])
                    sz = nxt - val
                kind = "function" if code or typ == 2 else "data"
                masked, rel = None, []
                if raw is not None and sec[1] != 8:
                    b = bytearray(raw[val:val + sz])
                    for off in self.relocs.get(shndx, {}):
                        if val <= off < val + sz:
                            rel.append(off - val)
                            b[off - val:off - val + 4] = bytes(min(4, val + sz - off))
                    masked = bytes(b)
                out.setdefault(name, (kind, self.names[shndx], sz, masked, sorted(rel)))
        return out

    def undefined(self):
        return {n for n, _, _, bind, _, shndx in self.syms if n and not shndx and bind == 1}


# ---- the tree ---------------------------------------------------------------------------------------------------------

def build_dir(version):
    return ROOT / "build" / version


def pc_files():
    """The game's C files the PC build compiles."""
    lister = ROOT / "tools" / "port_sources.py"
    if lister.exists():
        try:
            out = subprocess.run([sys.executable, str(lister), "--relative"], capture_output=True, text=True,
                                 check=True, cwd=ROOT).stdout.split()
            if out:
                return sorted(p for p in out if p.endswith(".c") and not p.startswith("port/"))
        except subprocess.CalledProcessError:
            pass
    files = sorted(p.relative_to(ROOT).as_posix() for d in ("src", "lib") for p in (ROOT / d).rglob("*.c"))
    return [f for f in files if not f.startswith(NOT_COMPILED)]


def incomplete_files():
    """{file: module} of the files the delinks.txt list without `complete`."""
    out = {}
    for p in sorted((ROOT / "config").rglob("delinks.txt")):
        mod = "main" if p.parent.name == "arm9" else p.parent.name
        for m in re.finditer(r"^(\S[^\n]*):\n((?:[ \t]+[^\n]*(?:\n|$))*)", p.read_text(), re.M):
            if not re.search(r"^\s*complete\s*$", m[2], re.M):
                out[m[1].strip()] = mod
    return out


def rom_objects(version):
    """[(path, origin)] of the link's objects that are the ROM's bytes: "gap" (no file's) or the incomplete file."""
    out = []
    for p in re.findall(r'"([^"]+)"|(\S+)', (build_dir(version) / "arm9.o.objects").read_text()):
        p = p[0] or p[1]
        orig = p.split("/keep/", 1)[1] if "/keep/" in p else p
        if "/delinks/" in orig:
            rel = orig.split("/delinks/", 1)[1]
            out.append((ROOT / p, "gap" if rel.startswith("_dsd_gap@") else str(Path(rel).with_suffix(".c"))))
    return out


def module_addrs(version):
    """{module: {symbol: address}} from each module's symbols.txt."""
    out = {}
    for p in (ROOT / "config" / version / "arm9").rglob("symbols.txt"):
        mod = "main" if p.parent.name == "arm9" else p.parent.name
        out[mod] = {m[1]: int(m[2], 16) for m in re.finditer(r"^(\S+) kind:\S+ addr:(0x[0-9a-f]+)", p.read_text(), re.M)}
    return out


# ---- assets -----------------------------------------------------------------------------------------------------------

def asset_rows(version):
    table = ROOT / "config" / version / "assets.tsv"
    rows = {}
    if table.exists():
        for ln in table.read_text().splitlines()[1:]:
            if ln.strip():
                name, mod, start, end, kind = ln.split("\t")
                rows[name] = (mod, int(start, 16), int(end, 16), kind)
    return rows


def cmd_assets(a):
    rows = asset_rows(a.version)
    used = defaultdict(list)
    for f in pc_files():
        for m in re.finditer(r'^\s*#\s*include\s+"asset/([^"]+)\.inc"', (ROOT / f).read_text(errors="replace"), re.M):
            used[m[1]].append(f)
    bad = 0
    for name, files in sorted(used.items()):
        row = rows.get(name)
        print(f"{name}\t{','.join(files)}\t" + (f"{row[0]} {row[1]:#010x}-{row[2]:#010x} {row[3]}" if row else "NO ROW"))
        bad += row is None
        if row and a.out:
            asset.write_asset(a.version, row[0], row[1], row[2], row[3], Path(a.out) / "asset" / f"{name}.inc")
    print(f"{len(used)} assets in {sum(len(v) for v in used.values())} PC files; {len(rows)} rows in assets.tsv")
    return 1 if bad else 0


# ---- gaps -------------------------------------------------------------------------------------------------------------

def pc_references(version, files):
    """{symbol: {file}} of the symbols the PC files' ROM-build objects use and none of them defines."""
    defs, undef = set(), defaultdict(set)
    for f in files:
        o = build_dir(version) / Path(f).with_suffix(".o")
        if not o.exists():
            continue
        obj = Obj(o)
        defs |= {n for n, _, _, bind, _, shndx in obj.syms if n and shndx and bind in (1, 2, 14)}
        for n in obj.undefined():
            undef[n].add(f)
    return {n: fs for n, fs in undef.items() if n not in defs}


def needed(version):
    """[(name, module, start, end, align, origin, users, pointers)] of the data that only the ROM's objects define
    and that the PC files use (origin "gap", or the incomplete file whose ROM bytes define it; pointers: the words
    Black's relocations cover), and [(name, origin, kind, users)] of the functions, linker-script symbols and .bss
    among them (other agents' stubs and port/src/linker_syms.c)."""
    files = pc_files()
    refs = pc_references(version, files)
    incomplete = incomplete_files()
    addrs = module_addrs(version)
    data, other = [], []
    for path, origin in rom_objects(version):
        for name, (kind, sec, size, masked, rel) in sorted(Obj(path).defined().items()):
            if name not in refs:
                continue
            users = sorted(refs[name])
            if kind != "data" or LINKER_MADE.match(name) or sec.endswith("bss"):
                other.append((name, origin, "linker" if LINKER_MADE.match(name) else
                              "bss" if kind == "data" else kind, users))
                continue
            mod = incomplete.get(origin) if origin != "gap" else Path(path).stem.split("@")[1].rsplit("_", 1)[0]
            start = addrs.get(mod, {}).get(name)
            if start is None:
                other.append((name, origin, "no address", users))
                continue
            align = 4 if start % 4 == 0 else 2 if start % 2 == 0 else 1
            data.append((name, mod, start, start + size, align, origin, users, len(rel)))
    return data, other


def cmd_gaps(a):
    data, other = needed(a.version)
    for name, mod, start, end, align, origin, users, ptrs in data:
        print(f"data\t{name}\t{mod} {start:#010x}-{end:#010x}" + (f" ({ptrs} pointers: C only)" if ptrs else "") +
              f"\t{origin}\t{','.join(users)}")
    for name, origin, kind, users in other:
        print(f"{kind}\t{name}\t{origin}\t{','.join(users)}")
    print(f"{len(data)} data symbols ({sum(r[3] - r[2] for r in data)} bytes) the PC files use that only the ROM's "
          f"objects define; {len(other)} other symbols (functions, linker script, .bss: stubs and linker_syms.c)")
    return 1 if data else 0


def cmd_gap_c(a):
    """The data needed() finds, as byte arrays from the base ROM; not the data that holds pointers (Black's
    addresses mean nothing on PC: that needs a C definition), which stays undefined in the PC link."""
    data, _ = needed(a.version)
    out = ["/* Written by tools/port_data.py gap-c from the base ROM (extract/): the data the PC build's C uses that",
           " * only the ROM's bytes define (a gap's, an incomplete file's). Generated into the build dir; never commit",
           " * it. Better: give the data a C definition (docs/port-build.md, Data). */",
           "#include <stdint.h>", ""]
    written = 0
    for name, mod, start, end, align, origin, users, ptrs in data:
        out.append(f"/* {mod} {start:#010x}-{end:#010x}, {origin}'s; used by {', '.join(users)} */")
        if ptrs:
            out.append(f"/* {name}: {ptrs} pointers, left out (the ROM's addresses): it needs C */")
            print(f"{name} ({origin}): {ptrs} pointers, left out: it needs C", file=sys.stderr)
            continue
        vals = asset.values(a.version, mod, start, end, "u8")
        out.append(f"_Alignas({align}) const uint8_t {name}[{end - start}] = {{")
        out += ["    " + " ".join(f"{v:#04x}," for v in vals[i:i + 16]) for i in range(0, len(vals), 16)]
        out.append("};")
        written += 1
    if not written:
        out.append("typedef int gap_data_none; /* none written */")
    Path(a.out).parent.mkdir(parents=True, exist_ok=True)
    Path(a.out).write_text("\n".join(out) + "\n")
    print(f"{a.out}: {written} of {len(data)} data symbols")
    return 1 if written < len(data) else 0


# ---- incomplete files -------------------------------------------------------------------------------------------------

def compare(version, f):
    """What f's compiled C lacks against its ROM bytes: {kind: [(name, detail)]}."""
    rom = build_dir(version) / "delinks" / Path(f).with_suffix(".o")
    ours = build_dir(version) / Path(f).with_suffix(".o")
    if not rom.exists():
        return {"error": [("", f"no {rom.relative_to(ROOT)}")]}
    if not ours.exists():
        return {"error": [("", f"no {ours.relative_to(ROOT)}: the C doesn't compile?")]}
    robj = Obj(rom)
    r, o = robj.defined(), Obj(ours).defined()
    # the ROM's data by section, in address order: a C datum longer than dsd's symbol covers the symbols after it
    order = defaultdict(list)
    for name, (kind, sec, size, masked, rel) in r.items():
        if kind == "data":
            order[sec].append((robj_offset(robj, name), name))
    for sec in order:
        order[sec].sort()
    covered, out = set(), defaultdict(list)
    for name, (kind, sec, size, masked, rel) in sorted(r.items()):
        if name not in o or kind != "data":
            continue
        okind, osec, osize, omasked, orel = o[name]
        at = robj_offset(robj, name)
        span = [n for off, n in order[sec] if at < off < at + osize]
        covered |= set(span)
        if masked is None or omasked is None:  # .bss: its size
            if osize < size:
                out["data size"].append((name, f"ROM {size:#x}, C {osize:#x}"))
            continue
        romb, romrel = robj.span(name, max(size, osize))
        m1, m2 = bytearray(romb[:osize]), bytearray(omasked)
        if len(m1) < osize:
            out["data size"].append((name, f"ROM {size:#x}, C {osize:#x}: past the ROM section's end"))
            continue
        for off in set(romrel) | set(orel):  # a word one side relocates is not compared
            m1[off:off + 4] = m2[off:off + 4] = bytes(len(m1[off:off + 4]))
        tail = romb[osize:size]
        if m1 != m2:
            first = next(i for i in range(len(m1)) if m1[i] != m2[i])
            out["data bytes"].append((name, f"differ at +{first:#x} of {osize:#x}"))
        elif any(tail):
            out["data size"].append((name, f"ROM {size:#x}, C {osize:#x}: the C lacks the last {size - osize:#x} bytes"))
        elif sorted(x for x in romrel if x < osize) != sorted(orel):
            out["data relocations"].append((name, f"ROM {len(romrel)} words, C {len(orel)}"))
    unclaimed_bss = defaultdict(list)
    for name, (kind, sec, size, masked, rel) in o.items():
        if kind == "data" and name not in r and masked is None:
            unclaimed_bss[size].append(name)
    for name, (kind, sec, size, masked, rel) in sorted(r.items()):
        if name in o or name in covered:
            continue
        if kind == "function":
            out["function missing"].append((name, f"{sec} {size:#x}"))
            continue
        if masked is None:
            maybe = unclaimed_bss.get(size)
            out["bss elsewhere" if maybe else "bss missing"].append(
                (name, f"{size:#x}" + (f", maybe the C's {maybe.pop(0)}" if maybe else "")))
            continue
        pat = bytes(masked).rstrip(b"\0")  # dsd's datum runs to the next symbol: its padding too
        if sum(1 for b in pat if b) < 2:
            out["data zeros"].append((name, f"{sec} {size:#x}: all zeros, not looked for"))
            continue
        hits = [n for n, (k, s, sz, m, _) in o.items() if k == "data" and m is not None and pat in m]
        named = [n for n in hits if not n.startswith("@")]
        if named:
            out["data elsewhere"].append((name, f"{sec} {size:#x}, in the C's {named[0]}"))
        elif hits:
            out["data literal"].append((name, f"{sec} {size:#x}, a literal of the C"))
        else:
            out["data missing"].append((name, f"{sec} {size:#x}"))
    return out


def robj_offset(obj, name):
    return next(v for n, v, *_ in obj.syms if n == name)


def _span(self, name, n):
    """(n bytes of the symbol's section from the symbol on, relocated offsets in them from the symbol)."""
    sym = next(s for s in self.syms if s[0] == name)
    raw = self.section_bytes(sym[5])
    rel = [off - sym[1] for off in self.relocs.get(sym[5], {}) if sym[1] <= off < sym[1] + n]
    return raw[sym[1]:sym[1] + n], rel


Obj.span = _span


def cmd_incomplete(a):
    files = pc_files()
    targets = a.files or [f for f in incomplete_files() if f in files]
    refs = pc_references(a.version, files)
    found = ("data literal", "data elsewhere", "bss elsewhere")
    for f in targets:
        res = compare(a.version, f)
        quiet = sum(len(v) for k, v in res.items() if k in found)
        print(f"== {f}" + (f" ({quiet} of the ROM's data under other names in the C: string literals, statics)"
                           if quiet and not a.all else ""))
        if not any(k not in found for k in res):
            print("   nothing lacking: every named function and datum is in the C, the data with the ROM's bytes")
        for kind, items in res.items():
            if kind in found and not a.all:
                continue
            for name, detail in items:
                users = sorted(refs.get(name, ()))
                print(f"   {kind}\t{name}\t{detail}" + (f"\tundefined in the PC link: {','.join(users)}" if users else ""))
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", default="b_us")
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("assets")
    p.add_argument("--out", help="write each asset to OUT/asset/<name>.inc")
    p = sub.add_parser("gaps")
    p = sub.add_parser("gap-c")
    p.add_argument("out")
    p = sub.add_parser("incomplete")
    p.add_argument("files", nargs="*")
    p.add_argument("--all", action="store_true", help="also the data found under other names")
    a = ap.parse_args()
    return {"assets": cmd_assets, "gaps": cmd_gaps, "gap-c": cmd_gap_c, "incomplete": cmd_incomplete}[a.cmd](a)


if __name__ == "__main__":
    sys.exit(main())
