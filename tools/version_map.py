#!/usr/bin/env python3
"""Writes config/<version>/delta.txt, how another version's dsd config differs from Black's, from both ROMs.

White is Black with a few functions changed: every module but main and the title screen's overlay (ov088) has the same
size, and all but about 60 of Black's 35,650 functions are in White with the same bytes, at other addresses only
(main is 32 bytes longer, so the overlays load 32 bytes higher). White's config is therefore made from Black's by
tools/version_config.py, from a short list of changes that this tool finds (docs/white.md):

    hash <module> <hash>                    White's module hash (config.yaml)
    shift <module> <black address> <shift>  from this Black address on, White's address is Black's plus the shift
    size <module> <black address> <size>    White's size of a function that differs
    relocs <module> <black start> <black end>
        <relocation lines>                  White's relocations from that Black range, in White's addresses, in place
                                            of Black's
    incomplete <file>                       a file complete in Black that doesn't match White yet: White links the
                                            code delinked from its ROM

How it finds them: each Black function and data symbol is looked for in White near its expected place, with the bytes
of its relocations masked (they hold addresses, which move). Each of Black's relocations from a symbol found is read in
White at its new place: the address it holds there gives the shift of its target. A symbol's shift is its
relocations' vote, else where its bytes were found, else its neighbor's. A symbol whose bytes differ takes White's size
and relocations from dsd's own analysis of White (`dsd init`, which this runs), and a relocation that holds another
address in White than its mapped target is replaced by one to the address White has.

    python3 tools/version_map.py w_us [--report todo.tsv]

Needs both extracted ROMs (extract/b_us and extract/<version>: `ninja extract/<version>/config.yaml`). Its delta keeps
the `incomplete` lines of the old one for the files still differing, so a file made to match White stays marked
complete. Run it again when Black's config changes in a way the delta can't follow (a symbol added inside a function
that differs): tools/version_config.py fails when the delta no longer applies.
"""
import argparse
import bisect
import collections
import difflib
import re
import shutil
import struct
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BLACK = "b_us"

ADDR = re.compile(r"addr:0x([0-9a-f]+)")
SIZE = re.compile(r"size=0x([0-9a-f]+)")
FROM = re.compile(r"from:0x([0-9a-f]+)")
TO = re.compile(r"to:0x([0-9a-f]+)")
KIND = re.compile(r"kind:(\w+)")
MODULE = re.compile(r"module:(\S+)")


def yaml_int(text, key):
    m = re.search(r"^\s*" + key + r":\s*(\d+)", text, re.M)
    return int(m.group(1)) if m else None


def module_names(config_yaml: Path):
    """The modules of a dsd config, (name, its directory under arm9/), in config.yaml's order."""
    return [(m.group(1), str(Path(m.group(2)).parent)) for m in
            re.finditer(r"^\s*(?:- )?\s*name: (\S+)\n(?:.*\n){0,3}?\s*delinks: (\S+)", config_yaml.read_text(), re.M)]


def module_hashes(config_yaml: Path):
    return dict(re.findall(r"name: (\S+)\n(?:.*\n){0,2}?\s*hash: (\w+)", config_yaml.read_text()))


def extract_module(extract: Path, name: str):
    """A module's bytes, base address and .bss (start, end) in an extracted ROM."""
    if name.startswith("ov"):
        for entry in (extract / "arm9_overlays" / "overlays.yaml").read_text().split("- id:")[1:]:
            if int(entry.split()[0]) == int(name[2:]):
                data = (extract / "arm9_overlays" / f"{name}.bin").read_bytes()
                base = yaml_int(entry, "base_address")
                return data, base, (base + len(data), base + len(data) + yaml_int(entry, "bss_size"))
        raise KeyError(name)
    stem = ("arm9/arm9" if name == "main" else f"dsi/{name}" if name.startswith("ltd_") else
            f"arm9/unk_{name}" if name.startswith("autoload") else f"arm9/{name}")
    data, text = (extract / f"{stem}.bin").read_bytes(), (extract / f"{stem}.yaml").read_text()
    base = yaml_int(text, "base_address")
    if yaml_int(text, "bss_start") is not None:
        return data, base, (yaml_int(text, "bss_start"), yaml_int(text, "bss_end"))
    return data, base, (base + len(data), base + len(data) + yaml_int(text, "bss_size"))


def target_modules(field: str):
    """The module names a relocation's module: field may mean."""
    m = re.fullmatch(r"(\w+)\(([\d,]+)\)", field)
    if not m:
        return [field]
    ids = [int(i) for i in m.group(2).split(",")]
    return [f"ov{i:03d}" for i in ids] if m.group(1) in ("overlay", "overlays") else [f"{m.group(1)}_{i}" for i in ids]


def decode(kind, data, off, addr):
    """The address a relocation's site holds (a branch's target, a word's value), or None."""
    if off < 0 or off + 4 > len(data):
        return None
    if kind == "load":
        return struct.unpack_from("<I", data, off)[0]
    if kind in ("arm_call", "arm_call_thumb"):
        ins = struct.unpack_from("<I", data, off)[0]
        imm = ins & 0xFFFFFF
        imm = (imm - (1 << 24) if imm & 0x800000 else imm) << 2
        return addr + 8 + imm + (((ins >> 24) & 1) << 1 if ins >> 28 == 0xF else 0)
    if kind in ("thumb_call", "thumb_call_arm"):
        h0, h1 = struct.unpack_from("<HH", data, off)
        o = ((h0 & 0x7FF) << 12) | ((h1 & 0x7FF) << 1)
        t = addr + 4 + (o - (1 << 23) if o & 0x400000 else o)
        return t & ~3 if h1 >> 11 == 0x1D else t
    return None


def black_site(mod, f, kind, to):
    """The address Black's site of a relocation holds, or None for a call through a veneer (the linker makes it, next
    to the call, so it says nothing of where the target is)."""
    b = decode(kind, mod.bdata, f - mod.bbase, f)
    if b is not None and kind != "load" and to is not None and (b & ~1) != (to & ~1):
        return None
    return b


class Module:
    """One module of both versions: Black's symbols and relocations, both versions' bytes, dsd init's view of White."""

    def __init__(self, name, sub, bconfig, wconfig, bext, wext):
        self.name, self.sub = name, sub
        self.bdata, self.bbase, bss = extract_module(bext, name)
        self.wdata, self.wbase, wss = extract_module(wext, name)
        self.bss = (bss, wss)  # ((Black start, end), (White start, end))
        self.symbols = []  # (address, kind, size or None, line), by address
        for line in (bconfig / sub / "symbols.txt").read_text().splitlines():
            a, k = ADDR.search(line), KIND.search(line)
            if a and k:
                s = SIZE.search(line)
                self.symbols.append((int(a.group(1), 16), k.group(1), int(s.group(1), 16) if s else None, line))
        self.symbols.sort(key=lambda s: s[0])
        self.starts = sorted({s[0] for s in self.symbols})
        self.relocs = []  # (from, kind, to or None, target modules, line)
        for line in (bconfig / sub / "relocs.txt").read_text().splitlines():
            f, k = FROM.search(line), KIND.search(line)
            if f and k:
                t, m = TO.search(line), MODULE.search(line)
                self.relocs.append((int(f.group(1), 16), k.group(1), int(t.group(1), 16) if t else None,
                                    target_modules(m.group(1)) if m else [], line))
        self.masked = set()
        for f, *_ in self.relocs:
            self.masked.update(range(f, f + 4))
        self.wfuncs = {}  # dsd init's functions in White: address -> size
        for line in (wconfig / sub / "symbols.txt").read_text().splitlines():
            a, s = ADDR.search(line), SIZE.search(line)
            if a and s and "kind:function" in line:
                self.wfuncs[int(a.group(1), 16)] = int(s.group(1), 16)
        self.wrelocs = []  # dsd init's relocations in White: (from, line), by address
        for line in (wconfig / sub / "relocs.txt").read_text().splitlines():
            f = FROM.search(line)
            if f:
                self.wrelocs.append((int(f.group(1), 16), line))
        self.wrelocs.sort()
        self.found = {}  # Black symbol address -> shift where its bytes are in White
        self.shift = {}  # Black symbol address -> its shift, in the end
        self.wsize = {}  # Black function address -> White's size, of a function that differs
        self.same = set()  # Black symbol addresses whose bytes White has at their shift
        self.white_starts = None  # White's addresses of the symbols, once the shifts are known

    def holder(self, a):
        """The start of the symbol holding a Black address (the last one at or before it), or None."""
        i = bisect.bisect_right(self.starts, a) - 1
        return self.starts[i] if i >= 0 else None

    def wmap(self, a):
        """White's address of a Black address: by the shift of the last symbol or section boundary at or before it."""
        i = bisect.bisect_right(self.point_starts, a) - 1
        return a + (self.points[i][1] if i >= 0 else self.wbase - self.bbase)

    def make_points(self):
        """The shifts by address: each symbol's, and the .bss's start and end in both ROMs (the padding before the
        aligned .bss can take up what a function grew by, so the .bss may move less than what is before it)."""
        by_addr = {self.bss[0][1]: self.bss[1][1] - self.bss[0][1]}
        for a in self.starts:
            by_addr[a] = self.shift[a]
        by_addr[self.bss[0][0]] = self.bss[1][0] - self.bss[0][0]
        self.points = sorted(by_addr.items())
        self.point_starts = [p[0] for p in self.points]

    def extent(self, addr, kind, size):
        """A symbol's bytes in Black: a function's size, else up to the next symbol."""
        if kind != "function" or not size:
            i = bisect.bisect_right(self.starts, addr)
            nxt = self.starts[i] if i < len(self.starts) else self.bbase + len(self.bdata)
            size = min(nxt, self.bbase + len(self.bdata)) - addr
        return max(size, 0)

    def bytes_items(self):
        return [s for s in self.symbols if s[1] in ("function", "data") and self.bbase <= s[0] < self.bbase + len(self.bdata)]

    def matches(self, addr, size, shift):
        o, wo = addr - self.bbase, addr + shift - self.wbase
        if wo < 0 or wo + size > len(self.wdata):
            return False
        pat, seg = self.bdata[o:o + size], self.wdata[wo:wo + size]
        return seg == pat or all(seg[k] == pat[k] for k in range(size) if addr + k not in self.masked)


def find_symbols(mod: Module):
    """Looks for each Black function and data symbol in White near its expected place."""
    prev = mod.wbase - mod.bbase
    for addr, kind, size, _ in mod.bytes_items():
        size = mod.extent(addr, kind, size)
        keep = sum(1 for k in range(size) if addr + k not in mod.masked)
        step = 2 if kind == "function" else 4
        candidates = sorted(range(prev - 0x800, prev + 0x801, step), key=lambda d: abs(d - prev))
        if keep >= 8:
            d = next((d for d in candidates if mod.matches(addr, size, d)), None)
        elif mod.matches(addr, size, prev):
            d = prev
        else:
            # one too short to tell from other bytes (a BIOS call among the secure area's filler): only where no
            # other place near has the same bytes
            hits = [d for d in candidates if mod.matches(addr, size, d)] if keep else []
            d = hits[0] if len(hits) == 1 else None
        if d is not None:
            mod.found[addr] = d
            if keep >= 8:
                prev = d


def vote(mods):
    """Each Black relocation from a symbol found, read in White there: the shift of its target, by vote."""
    votes = collections.defaultdict(collections.Counter)
    for mod in mods.values():
        for f, kind, to, targets, _ in mod.relocs:
            s = mod.holder(f)
            if to is None or s not in mod.found or len(targets) != 1 or targets[0] not in mods:
                continue
            b = black_site(mod, f, kind, to)
            w = decode(kind, mod.wdata, f + mod.found[s] - mod.wbase, f + mod.found[s])
            if b is not None and w is not None:
                votes[(targets[0], to)][w - b] += 1
    return votes


def assign(mod: Module, votes):
    """The shift of every Black symbol, and White's size of each function that differs."""
    cur = mod.wbase - mod.bbase
    prev = None  # the function before, (address, Black size, shift)
    for addr, kind, size, _ in mod.symbols:
        v = collections.Counter(votes.get((mod.name, addr), {}))
        if mod.wbase != mod.bbase:
            # a word that holds the same value in a module that moved is a number that looks like an address
            v.pop(0, None)
        top = [d for d, c in v.items() if c == max(v.values())] if v else []
        if top:
            # a tie goes to where the bytes were found, else to the shift so far
            s = top[0] if len(top) == 1 else mod.found.get(addr) if mod.found.get(addr) in top else \
                cur if cur in top else top[0]
        elif addr in mod.found:
            s = mod.found[addr]
        else:
            s = cur
            # right after a function that differs: where White's own size of it ends
            if prev and prev[0] not in mod.same and prev[1] and addr == prev[0] + prev[1]:
                wsize = mod.wfuncs.get(prev[0] + prev[2])
                if wsize is not None:
                    s = prev[2] + wsize - prev[1]
        mod.shift[addr] = cur = s
        if kind in ("function", "data") and mod.bbase <= addr < mod.bbase + len(mod.bdata):
            if mod.matches(addr, mod.extent(addr, kind, size), s):
                mod.same.add(addr)
        if kind == "function":
            prev = (addr, size, s)
    for addr, kind, size, _ in mod.symbols:
        if kind == "function" and addr not in mod.same and size:
            w = addr + mod.shift[addr]
            wsize = mod.wfuncs.get(w)
            if wsize is None:
                # not one dsd found in White: up to where the next symbol starts there
                i = bisect.bisect_right(mod.starts, addr)
                if i < len(mod.starts):
                    wsize = mod.starts[i] + mod.shift[mod.starts[i]] - w
            if wsize is not None and wsize != size:
                mod.wsize[addr] = wsize


def expected(mods, kind, to, targets, b):
    """The addresses White's site may hold for a Black relocation whose site holds b: its target's shift applied."""
    return {b + mods[t].wmap(to) - to for t in targets if t in mods}


def relocations(mods):
    """{module: {(Black start, end): [White's relocation lines]}}: each symbol that differs, and each relocation that
    holds another address in White than Black's target mapped."""
    out = collections.defaultdict(dict)
    retargets = []
    for mod in mods.values():
        differing = [(addr, addr + mod.extent(addr, kind, size), kind) for addr, kind, size, _ in mod.bytes_items()
                     if addr not in mod.same]
        spans = {a: e for a, e, _ in differing}
        for start, end, kind in differing:
            # a data symbol ends where the next symbol starts in White, a function at White's size of it
            ws = mod.wmap(start)
            we = (ws + mod.wsize[start] if start in mod.wsize else mod.wmap(end) if kind == "data"
                  else ws + end - start)
            i = bisect.bisect_left(mod.wrelocs, (ws, ""))
            lines = []
            mine = [r for r in mod.relocs if start <= r[0] < end]
            while i < len(mod.wrelocs) and mod.wrelocs[i][0] < we:
                wf, wline = mod.wrelocs[i]
                # Black's own line where one of the symbol's relocations is of the same kind with the same target
                wk = KIND.search(wline).group(1)
                w = decode(wk, mod.wdata, wf - mod.wbase, wf)
                for f, kind, to, targets, line in mine:
                    b = black_site(mod, f, kind, to)
                    if kind == wk and b is not None and w in expected(mods, kind, to, targets, b):
                        wline = retarget(line, wf, to + (w - b))
                        break
                else:
                    wline = on_symbol(mods, wline)
                if wline:
                    lines.append(wline)
                i += 1
            out[mod.name][(start, end)] = lines
        for f, kind, to, targets, line in mod.relocs:
            s = mod.holder(f)
            if s in spans or to is None:
                continue
            wf = f + mod.shift[s]
            b = black_site(mod, f, kind, to)
            w = decode(kind, mod.wdata, wf - mod.wbase, wf)
            if b is None or w is None or w in expected(mods, kind, to, targets, b):
                continue
            wline = on_symbol(mods, retarget(line, wf, to + (w - b)))
            out[mod.name][(f, f + 4)] = [wline] if wline else []
            retargets.append((mod.name, s, f, line, w - b))
    return out, retargets


def on_symbol(mods, line):
    """A relocation line whose target is a symbol's start in White, or None: dsd's delink wants a symbol there, and a
    word that holds no symbol's address in White (a number that looked like an address in Black, dsd init's guess at
    a .bss address) is best left as the bytes it is, which the delinked object then has."""
    t, m = TO.search(line), MODULE.search(line)
    if not t or not m:
        return line
    name = target_modules(m.group(1))[0]
    if name not in mods:
        return line
    mod = mods[name]
    if mod.white_starts is None:
        mod.white_starts = {mod.wmap(a) for a in mod.starts}
    return line if int(t.group(1), 16) in mod.white_starts else None


def retarget(line, wf, wto):
    line = FROM.sub(f"from:0x{wf:08x}", line)
    return TO.sub(f"to:0x{wto:08x}", line) if TO.search(line) else line


def files_of(bconfig: Path, sub: str):
    """[(file, complete, [(start, end)])] of a module's delinks.txt."""
    out = []
    for line in (bconfig / sub / "delinks.txt").read_text().splitlines():
        if line and not line[0].isspace() and line.endswith(":"):
            out.append([line[:-1], False, []])
        elif out and line.strip() == "complete":
            out[-1][1] = True
        elif out:
            m = re.search(r"start:0x([0-9a-f]+) end:0x([0-9a-f]+)", line)
            if m:
                out[-1][2].append((int(m.group(1), 16), int(m.group(2), 16)))
    return out


def difference(mod, addr, kind, size):
    """How much a symbol differs, for the report."""
    end = mod.extent(addr, kind, size)
    b = mod.bdata[addr - mod.bbase:addr - mod.bbase + end]
    w0 = mod.wmap(addr) - mod.wbase
    wsize = mod.wsize.get(addr, end)
    w = mod.wdata[w0:w0 + wsize]
    hb = [b[k:k + 2] for k in range(0, len(b), 2)]
    hw = [w[k:k + 2] for k in range(0, len(w), 2)]
    ratio = difflib.SequenceMatcher(None, hb, hw, autojunk=False).ratio() if hb and hw else 0
    return f"{100 * (1 - ratio):.0f}% of halfwords differ" + (f", size 0x{end:x} -> 0x{wsize:x}" if wsize != end else "")


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("version")
    parser.add_argument("--report", help="also write a table of what differs, by file (tsv)")
    parser.add_argument("--fresh", action="store_true", help="mark every differing file incomplete, as at the start")
    parser.add_argument("--init",help="dsd init's config (its arm9 directory) of the version, made if not given")
    args = parser.parse_args()
    bconfig = ROOT / "config" / BLACK / "arm9"
    bext, wext = ROOT / "extract" / BLACK, ROOT / "extract" / args.version
    tmp = None
    if args.init:
        wconfig = Path(args.init)
    else:
        tmp = Path(tempfile.mkdtemp(prefix="version_map"))
        print(f"dsd init of {wext}")
        subprocess.run([str(ROOT / "tools" / "dsd"), "init", "--rom-config", str(wext / "config.yaml"),
                        "--output-path", str(tmp / "config"), "--build-path", str(tmp / "build")],
                       check=True, capture_output=True)
        wconfig = tmp / "config" / "arm9"
    mods = {name: Module(name, sub, bconfig, wconfig, bext, wext) for name, sub in module_names(bconfig / "config.yaml")}
    for mod in mods.values():
        find_symbols(mod)
    votes = vote(mods)
    for mod in mods.values():
        assign(mod, votes)
        mod.make_points()
    relocs, retargets = relocations(mods)
    hashes = module_hashes(wconfig / "config.yaml")

    # what differs, by file; a file complete in Black with a difference is incomplete in this version
    delta_path = ROOT / "config" / args.version / "delta.txt"
    old_incomplete = set()
    if delta_path.exists():
        old_incomplete = {l.split()[1] for l in delta_path.read_text().splitlines() if l.startswith("incomplete ")}
    rows, incomplete = [], set()
    retarget_at = collections.defaultdict(list)
    for name, s, f, line, d in retargets:
        retarget_at[(name, s)].append(f)
    for mod in mods.values():
        names = {addr: line.split()[0] for addr, kind, size, line in mod.symbols if kind in ("function", "data")}
        for file, complete, ranges in files_of(bconfig, mod.sub):
            def inside(a):
                return any(s <= a < e for s, e in ranges)
            differs = False
            for addr, kind, size, _ in mod.bytes_items():
                if not inside(addr):
                    continue
                if addr not in mod.same:
                    rows.append((file, mod.name, names[addr], "fn" if kind == "function" else "data",
                                 f"0x{mod.extent(addr, kind, size):x}", difference(mod, addr, kind, size)))
                    differs = True
                elif (mod.name, addr) in retarget_at:
                    rows.append((file, mod.name, names[addr], "fn" if kind == "function" else "data",
                                 f"0x{mod.extent(addr, kind, size):x}",
                                 f"{len(retarget_at[(mod.name, addr)])} address(es) differ, the code is the same"))
                    differs = True
            if differs and complete:
                incomplete.add(file)
    if old_incomplete and not args.fresh:
        # a file made to match this version still differs from Black: it stays complete
        for f in sorted(incomplete - old_incomplete):
            print(f"{f} differs from Black, complete in the old delta: stays complete (--fresh marks it incomplete)")
        incomplete &= old_incomplete

    out = [f"# {args.version}'s config is Black's (config/{BLACK}) with these changes: written by tools/version_map.py "
           "from both ROMs,", "# applied by tools/version_config.py (configure.py runs it). docs/white.md says how.", ""]
    for name, h in hashes.items():
        if name in mods:
            out.append(f"hash {name} {h}")
    out.append("")
    for mod in mods.values():
        last = None
        points = mod.points if mod.points and mod.points[0][0] <= mod.bbase else [(mod.bbase, mod.wbase - mod.bbase),
                                                                                  *mod.points]
        for addr, shift in points:
            if shift != last:
                last = shift
                out.append(f"shift {mod.name} 0x{addr:08x} {last:+#x}")
    out.append("")
    for mod in mods.values():
        for addr, size in sorted(mod.wsize.items()):
            out.append(f"size {mod.name} 0x{addr:08x} 0x{size:x}")
    out.append("")
    for name, ranges in relocs.items():
        for (start, end), lines in sorted(ranges.items()):
            out.append(f"relocs {name} 0x{start:08x} 0x{end:08x}")
            out += [f"    {l}" for l in lines]
    out.append("")
    out += [f"incomplete {f}" for f in sorted(incomplete)]
    delta_path.parent.mkdir(parents=True, exist_ok=True)
    delta_path.write_text("\n".join(out) + "\n")
    print(f"wrote {delta_path.relative_to(ROOT)}: {sum(len(m.symbols) - len(m.same) for m in mods.values())} symbols "
          f"without bytes or differing, {len(rows)} differing, {len(retargets)} relocations retargeted, "
          f"{len(incomplete)} files incomplete")
    if args.report:
        with open(args.report, "w") as fh:
            fh.write("file\tmodule\tsymbol\tkind\tsize\tdifference\n")
            for r in sorted(rows):
                fh.write("\t".join(r) + "\n")
    if tmp:
        shutil.rmtree(tmp)


if __name__ == "__main__":
    main()
