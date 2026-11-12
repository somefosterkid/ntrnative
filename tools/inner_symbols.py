#!/usr/bin/env python3
"""Inner and ambiguous dsd symbols: what our C's objects tell relocs.txt and symbols.txt.

(a) An inner symbol: a dsd symbol strictly inside a data object a C file defines (a table, a struct; the object's
extent from the file's compiled object, placed at its delinks ranges as the link places it: tools/port_file.py's
place()) cuts the object in pieces, and once the file is complete nothing defines it (Undefined at the link). It goes
from symbols.txt, and every relocation to an address inside the object, in every module's relocs.txt, becomes
`to:<object> add:N`, as MWCC folds a field's offset into the pool word (gamestats.c's client struct, t32b; the 9 field
symbols of scrcmd_shop.c's 3 tables, t32k). One a C file still names (outside its comments) is kept and printed. A
relocation whose C names a table outside it (sDayOfYear[month - 1]: the table - 4, rtc/convert.c, t34i) becomes
`to:<that table> add:-0x4`, not the object before + its offset.
(b) An ambiguous relocation, one dsd couldn't tell between overlays (`module:overlays(a,b,...)`, linked to the first),
takes one overlay, `module:overlay(N)`, when
  - a call whose C function loads the OVERLAY_<N>_ID of one overlay of the list (GFL_OvlLoad(OVERLAY_37_ID) before
    the call, scrcmd_proc.c, t35l): that one; where the C calls another overlay's function there, a `rename` line
    says so (unless the C's name is a real one, not dsd's default below: a conflict);
  - named: the C of the file it is from names its target there (the object's relocation at the same address names a
    symbol that symbols.txt, or a C object, has at the target in one of the overlays only: data_ov194_021f557c, a
    Black name) - unless the address is inside a C object in that overlay: a conflict, printed, nothing changed.
    dsd's own name in the list's first overlay (func_ov036_021f36c0 of overlays(36,37,...)) is no evidence: dsd
    links to the first, so a C written from its delinked code names that one whatever the code means (t34i: 3 loads
    taken to the wrong overlay so);
  - inside: else the address is inside one of our C's data objects in all overlays of the list but one: that one
    (ov010's pointer to 0x021cf7dd is ov021's function: in ov114 it is inside gamestats.c's client struct);
  - a load of the one function of the overlays at the address (the others have only dsd's `ambiguous` data symbols).
  A call takes an overlay only where it has a function of the call's mode (Thumb for thumb_call). With no one
  overlay, those where the address is inside our object leave the list (overlays(21,93,114) -> overlays(21,93):
  in ov114 0x021d1a75 is inside d_nhttp_common.c's static, t35q). Then the other overlays' symbols at the address
  go when dsd made them for such relocations (`ambiguous`) or they are inner symbols, no relocation reaches them any
  more and no C file or header names them.
A file's data section counts only where it fits its range: the file is complete, or the section's size (4-aligned)
is the range's and its bytes are the ROM's (relocated words aside). Function sections are placed by their names
(symbols.txt), so the relocations of an incomplete file's code count too where relocs.txt agrees on the target. A
complete file's objects are where the link map (build/b_us/arm9.o.xMAP) has them, or at its ranges where the map
doesn't have the object yet (said so: ninja relinks) or is older than them (--placed: tools/fit_data.py's file, whose
ranges it just changed; the map's addresses of its old ones made un_select.c's false 'inside
data_ov218_021bab20+0x38', t37e).

    python3 tools/inner_symbols.py                     # dry run, tree-wide: every case, nothing changed
    python3 tools/inner_symbols.py src/ov021/x.c ...   # only the cases these files' objects or code make
    python3 tools/inner_symbols.py --apply [files]     # write symbols.txt and relocs.txt
    python3 tools/inner_symbols.py --apply --placed src/ov218/un_select.c   # its new ranges, not the map's
Then the gate: ninja, python3 tools/regressions.py HEAD, python3 tools/row_audit.py --changed.
"""
import argparse
import bisect
import glob
import os
import re
import sys
import types
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from port_file import module_bytes, parse_object, place  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CONFIG = "config/b_us/arm9"
DATA = (".rodata", ".data", ".bss")
RELOC = re.compile(r"from:(0x\w+) kind:(\S+) to:(0x\w+)(?: add:(\S+))? module:(\S+)(.*)$")
SYMBOL = re.compile(r"(\S+) kind:(\w+)(\S*) addr:(0x\w+)(.*)$")
R_ARM_ABS32 = 2
# a string or character literal (kept: a // inside one is no comment), a /* */ or a // comment (blanked)
CODE = re.compile(r'"(?:\x5c.|[^"\x5c\n])*"|' + r"'(?:\x5c.|[^'\x5c\n])*'|/\*.*?\*/|//[^\n]*", re.S)
OVERLAY_ID = re.compile(r"OVERLAY_(\d+)_ID$")


def uncommented(text):
    """The C text without its comments: a name in a comment is no use of it."""
    return CODE.sub(lambda m: m[0] if m[0][0] in "\"'" else " ", text)


def dsd_default(name, mods):
    """Whether the name is dsd's own of the first overlay of the list (func_ov036_021f36c0 of overlays(36,37,...)):
    dsd links an ambiguous relocation to the first, so a C written from its delinked code names that one whichever
    overlay the code means - no evidence (t34i: 3 loads put in the wrong overlay that way)."""
    return bool(re.fullmatch(rf"(?:func|data)_{mods[0]}_[0-9a-f]{{8}}", name))


def mods_of(spec):
    """relocs.txt's module field as modules: overlay(21) -> [ov021], overlays(18,19) -> [ov018, ov019],
    ltd_autoload(0) -> [ltd_autoload_0], main -> [main]."""
    m = re.match(r"overlays?\(([\d,]+)\)$", spec)
    if m:
        return [f"ov{int(x):03d}" for x in m[1].split(",")]
    m = re.match(r"(\w+)\((\d+)\)$", spec)
    return [f"{m[1]}_{m[2]}"] if m else [spec]


def spec_of(mods):
    """mods_of() backwards."""
    if len(mods) > 1:
        return "overlays(" + ",".join(str(int(m[2:])) for m in mods) + ")"
    m = re.match(r"ov(\d+)$", mods[0]) or re.match(r"(\w+?)_(\d+)$", mods[0])
    if not m:
        return mods[0]
    return f"overlay({int(m[1])})" if mods[0].startswith("ov") else f"{m[1]}({m[2]})"


def module_of(path):
    mod = os.path.basename(os.path.dirname(path))
    return "main" if mod == "arm9" else mod


def up4(a):
    return -(-a // 4) * 4


class Tree:
    """The config's symbols.txt, relocs.txt and delinks.txt of every module, and our C's data objects."""

    def __init__(self, root=ROOT, placed=()):
        self.root, cfg, self._rom, self._targets = root, f"{root}/{CONFIG}", {}, {}
        self.placed = set(placed)  # complete files placed at their ranges whatever the link map says (newer)
        self.paths = {}  # (module, kind) -> path
        self.sym_lines, self.reloc_lines = {}, {}  # module -> [line]
        self.syms = defaultdict(dict)  # module -> {address: [(line index, name, kind, flags)]}
        self.names = defaultdict(list)  # name -> [(module, address)]
        self.files = {}  # module -> {source: (complete, {section: (start, end)})}
        self.header = {}  # module -> {section: (start, end)}
        for kind in ("symbols", "relocs", "delinks"):
            for path in sorted(glob.glob(f"{cfg}/**/{kind}.txt", recursive=True)):
                self.paths[module_of(path), kind] = path
        for (mod, kind), path in self.paths.items():
            lines = open(path).read().splitlines()
            if kind == "symbols":
                self.sym_lines[mod] = lines
                for k, line in enumerate(lines):
                    m = SYMBOL.match(line)
                    if m:
                        self.syms[mod].setdefault(int(m[4], 16), []).append((k, m[1], m[2], m[3] + m[5]))
                        self.names[m[1]].append((mod, int(m[4], 16)))
            elif kind == "relocs":
                self.reloc_lines[mod] = lines
            else:
                self.files[mod], self.header[mod] = {}, {}
                src = None
                for line in lines:
                    if line.strip() and not line[0].isspace():
                        src = line.strip().rstrip(":")
                        self.files[mod][src] = [False, {}]
                    elif src and line.strip() == "complete":
                        self.files[mod][src][0] = True
                    m = re.match(r"\s+(\.\w+)\s+start:(0x\w+) end:(0x\w+)", line)
                    if m:
                        (self.files[mod][src][1] if src else self.header[mod])[m[1]] = (int(m[2], 16), int(m[3], 16))
        self.sym_addrs = {mod: sorted(s) for mod, s in self.syms.items()}

    def read_object(self, src):
        """read_object()'s tuple of the source's compiled object, or None (tests replace it)."""
        path = f"{self.root}/build/b_us/{os.path.splitext(src)[0]}.o"
        return parse_object(open(path, "rb").read()) if os.path.exists(path) else None

    def rom(self, mod):
        """The module's bytes in the ROM and its base address."""
        if mod not in self._rom:
            self._rom[mod] = module_bytes(types.SimpleNamespace(delinks={mod: (self.header[mod], {})}), mod)
        return self._rom[mod]

    def xmap(self):
        """{(module, object file): {symbol: address}} of the link's map (build/b_us/arm9.o.xMAP), {} without one."""
        path, out, mod = f"{self.root}/build/b_us/arm9.o.xMAP", defaultdict(dict), None
        if not os.path.exists(path):
            return out
        for line in open(path, errors="replace"):
            if line.startswith("# ."):
                mod = line[3:].strip()
                mod = "main" if mod == "arm9" else mod
            m = mod and re.match(r"  ([0-9A-F]{8}) ([0-9A-F]{8}) \.\w+\s+(\S+)\t\((\S+)\)", line)
            if m:
                out[mod, m[4]][m[3]] = int(m[1], 16)
        return out

    def fits(self, mod, ranges, obj, at, funcs):
        """The data sections of an incomplete file's object that sit where their ranges say: their size (4-aligned)
        is the range's, the file's code (each function at Black's address of its name) reaches them where Black's
        relocations do, none elsewhere, and their bytes are the ROM's, relocated words aside (.bss has none: there
        one of the code's relocations at least must agree). The link may strip what nothing reaches, which this
        doesn't model."""
        from fit_data import differs
        d, secs, syms, relocs = obj
        if mod not in self._targets:
            self._targets[mod] = {int(m[1], 16): int(m[3], 16) + int(m[4] or "0", 16)
                                  for m in map(RELOC.match, self.reloc_lines.get(mod, ())) if m}
        black, out = self._targets[mod], {}
        code = {i: funcs[n] for n, i, v, _, _, f in syms if f and v in (0, 1) and n in funcs and i < len(secs)}
        for sec in DATA:
            idx = [i for i in at if secs[i][0] == sec]
            if sec not in ranges or not idx:
                continue
            votes = [(syms[s][1], s, black[code[i] + off] == at[syms[s][1]] + syms[s][2] + add)
                     for i, rows in relocs.items() if i in code for off, kind, s, add in rows
                     if kind == R_ARM_ABS32 and syms[s][1] in idx and code[i] + off in black]
            if not all(v for _, _, v in votes):
                continue
            size = up4(max(at[i] + secs[i][3] for i in idx)) == up4(ranges[sec][1])
            if size and (votes if sec == ".bss" else
                         differs(d, secs, relocs, {i: at[i] for i in idx}, *self.rom(mod)) is None):
                out.update({i: None for i in idx})
            else:  # only the objects the code reaches where Black's relocations do
                for i, s, _ in votes:
                    out.setdefault(i, set()).add(syms[s][0])
        return out

    def objects(self, only=None):
        """Reads our C's objects: self.objs {module: sorted [(address, size, name, global, source)]} of the data
        objects with a size: a complete file's where the link's map has them, an incomplete one's (in only) where
        fits() places them; self.refs {(module, from): (name, addend, source)} of the relocations of their
        functions (placed at Black's address of their name) and data; self.cdefs {name: [(module, address)]} of
        their globals."""
        self.objs, self.refs, self.cdefs = defaultdict(list), {}, defaultdict(list)
        self.statics = {}  # (module, source, name) -> address of a data object of the file, static ones too
        self.spans = defaultdict(list)  # module -> [(start, end, source, {overlays its OVERLAY_<n>_ID words name})]
        self.unmapped, self.missing = [], []  # complete files the link map lacks; asked-for files with no object
        xmap = self.xmap()
        for mod, files in self.files.items():
            funcs = {n: a & ~1 for a, rows in self.syms[mod].items() for _, n, k, _ in rows if k == "function"}
            for src, (complete, ranges) in files.items():
                obj = self.read_object(src) if re.search(r"\.(c|cpp)$", src) else None
                if obj is None:
                    if only and src in only:
                        self.missing.append(src)
                    continue
                _, secs, syms, relocs = obj
                linked = xmap.get((mod, os.path.splitext(os.path.basename(src))[0] + ".o"), {}) \
                    if complete and src not in self.placed else {}
                base, some = {}, {}
                if complete and not linked and any(i < len(secs) and secs[i][0] in DATA and size and not func
                                                   for _, i, _, size, _, func in syms):
                    # the link map doesn't have the object (older than it: a file just made complete): its data
                    # where its ranges say, as the link places it (t35n's fx_sincos.c, "nothing to change")
                    at = place(secs, ranges)
                    base = {i: a for i, a in at.items() if secs[i][0] in DATA}
                    if src not in self.placed:
                        self.unmapped.append(src)
                elif complete:  # each data section where the map has its first symbol
                    for name, i, value, size, glob_, func in syms:
                        if i < len(secs) and secs[i][0] in DATA and name in linked and i not in base:
                            base[i] = linked[name] - value
                elif only and src in only:
                    at = place(secs, ranges)
                    some = self.fits(mod, ranges, obj, at, funcs)
                    base = {i: at[i] for i in some}
                for name, i, value, size, glob_, func in syms:
                    if func and value in (0, 1) and name in funcs and i < len(secs) and secs[i][0] == ".text":
                        base[i] = funcs[name]
                        ids = {f"ov{int(OVERLAY_ID.match(syms[s][0])[1]):03d}" for _, _, s, _ in relocs.get(i, ())
                               if OVERLAY_ID.match(syms[s][0])}
                        self.spans[mod].append((funcs[name], funcs[name] + secs[i][3], src, ids))
                    elif i in base and size and not func and secs[i][0] in DATA and (complete or some[i] is None
                                                                                     or name in some[i]):
                        self.objs[mod].append((base[i] + value, size, name, glob_, src))
                        self.statics[mod, src, name] = base[i] + value
                        if glob_:
                            self.cdefs[name].append((mod, base[i] + value))
                for i, rows in relocs.items():
                    for off, kind, s, add in rows if i in base else ():
                        name = syms[s][0]
                        if name and not name.startswith("."):
                            self.refs[mod, base[i] + off] = (name, add if kind == R_ARM_ABS32 else 0, src)
        for mod in self.objs:
            self.objs[mod].sort()
        for mod in self.spans:
            self.spans[mod].sort()
        self.starts = {mod: [o[0] for o in objs] for mod, objs in self.objs.items()}

    def inside(self, mod, a):
        """Our data object a lies strictly inside in the module: (address, size, name, global, source), or None."""
        k = bisect.bisect_right(self.starts.get(mod, []), a) - 1
        o = self.objs[mod][k] if k >= 0 else None
        return o if o and o[0] < a < o[0] + o[1] else None

    def at(self, mod, a, kinds=None):
        """symbols.txt's symbols at the address: [(line index, name, kind, flags)]."""
        return [s for s in self.syms.get(mod, {}).get(a, []) if kinds is None or s[2] in kinds]

    def pick(self, mod, fr, kind, to, add, mods):
        """The overlay of an ambiguous relocation of the module from fr: (overlay or None, why, {sources}); None
        with a why is a conflict."""
        named = self.c_names(mod, fr, to, add, mods)
        if kind != "load":  # a call: the overlay its function loads (GFL_OvlLoad(OVERLAY_37_ID) before the call)
            span = self.function_at(mod, fr)
            ids = sorted(span[3] & set(mods)) if span else []
            if len(ids) > 1 and named and named[0] in ids:
                ids = [named[0]]
            if len(ids) == 1:
                load = f"its function loads OVERLAY_{int(ids[0][2:])}_ID"
                if named and named[0] != ids[0] and not dsd_default(named[1], mods):
                    return None, f"{named[2]} calls {named[1]} ({named[0]}), but {load}", {span[2]}
                return ids[0], f"{span[2]}: {load}", {span[2]}
        if named and not dsd_default(named[1], mods):
            hit, name, src = named
            o = self.inside(hit, to)
            if o:
                return None, f"the C names {name}, but in {hit} {to:#x} is inside {o[2]} ({o[4]})", {src, o[4]}
            return hit, f"{src} names {name}", {src}
        inner = {m: self.inside(m, to) for m in mods}
        rest = [m for m in mods if not inner[m]]
        if len(rest) == 1 and not self.at(rest[0], to) and not self.at(rest[0], to & ~1, ("function",)):
            return None, f"inside {', '.join(m for m, o in inner.items() if o)}, but {rest[0]} has no symbol there", set()
        if len(rest) == 1:
            why = ", ".join(f"{m}'s {o[2]} ({o[4]})" for m, o in inner.items() if o)
            return rest[0], f"inside {why}", {o[4] for o in inner.values() if o}
        # a pointer to the one function of the overlays at the address (a Thumb one's, odd), where the others have
        # only the data symbols dsd made for such relocations
        fn = [m for m in rest if any(to % 2 == 0 or "thumb" in s[3] for s in self.at(m, to & ~1, ("function",)))]
        if kind == "load" and len(fn) == 1 and all("ambiguous" in s[3] for m in rest if m != fn[0]
                                                    for s in self.at(m, to)):
            return fn[0], f"{self.at(fn[0], to & ~1, ('function',))[0][1]} is the one function there", set()
        return None, "", set()

    def c_names(self, mod, fr, to, add, mods):
        """(overlay, name, source) of the symbol the C names at the relocation where only one overlay of mods has
        it at the target, or None."""
        ref = self.refs.get((mod, fr))
        if not ref:
            return None
        name, radd, src = ref
        hit = sorted({m for m, a in self.names.get(name, []) + self.cdefs.get(name, [])
                      if m in mods and (a + radd == to + add or (a + radd) | 1 == to + add)})
        return (hit[0], name, src) if len(hit) == 1 else None

    def function_at(self, mod, a):
        """(start, end, source, {overlays}) of the C function of the module that holds the address, or None."""
        spans = self.spans.get(mod, [])
        k = bisect.bisect_right(spans, (a, float("inf"))) - 1
        return spans[k] if k >= 0 and spans[k][0] <= a < spans[k][1] else None

    def named_at(self, mod, fr, pick, target):
        """The address in the overlay of the symbol the C names at the relocation (a static of the file too), or
        None."""
        ref = self.refs.get((mod, fr))
        if not ref:
            return None
        own = self.statics.get((pick, ref[2], ref[0]))
        for m, a in self.names.get(ref[0], []) + self.cdefs.get(ref[0], []) + ([(pick, own)] if own else []):
            if m == pick and a + ref[1] in (target, target & ~1):
                return a
        return None

    def source_at(self, mod, a):
        """The file whose ranges hold the address, or None."""
        for src, (_, ranges) in self.files.get(mod, {}).items():
            if any(s <= a < e for s, e in ranges.values()):
                return src
        return None

    def plan(self, only=None):
        """The changes: [(kind, text, {sources}, edit)], kind overlay / inner (a relocs.txt line, edit (module, index,
        new line)), symbol (a symbols.txt line that goes, edit (module, index)), conflict / keep (printed only).
        With only (a set of sources), just the cases one of them makes."""
        self.objects(only)
        wanted = lambda files: only is None or bool(files & only)
        acts, cands, edited = [], {}, {}
        for mod, lines in self.reloc_lines.items():
            for k, line in enumerate(lines):
                m = RELOC.match(line)
                if not m:
                    continue
                fr, to, add, spec = int(m[1], 16), int(m[3], 16), int(m[4] or "0", 16), m[5]
                mods, notes, files, old = mods_of(spec), [], set(), (to, add, spec)
                if len(mods) > 1:
                    pick, why, files = self.pick(mod, fr, m[2], to, add, mods)
                    want = "thumb" if m[2] in ("thumb_call", "arm_call_thumb") else "arm"
                    if pick and m[2] != "load" and not any(re.match(rf"\({want}\b", s[3])
                                                           for s in self.at(pick, to & ~1, ("function",))):
                        pick, why = None, f"{why}, but {pick} has no {want} function at {to:#x}"
                    files = files | {o[4] for o in (self.inside(x, to) for x in mods) if o}
                    named = self.c_names(mod, fr, to, add, mods)
                    if pick and named and named[0] != pick and wanted(files | {named[2]}):
                        there = self.at(pick, to & ~1, ("function",)) or self.at(pick, to)
                        acts.append(("rename", f"{named[2]} names {named[1]} ({named[0]}) at {mod} {fr:#010x}, but "
                                               f"{pick}'s {there[0][1] if there else f'{to:#x}'} is the one ({why})",
                                     {named[2]}, None))
                    rest = [x for x in mods if not self.inside(x, to)]
                    if not pick and not why and 1 < len(rest) < len(mods):
                        # no one overlay, but those where the address is inside our object are none of them
                        amb = [(x, to) for x in mods if x not in rest]
                        notes.append("not " + ", ".join(f"{x} (inside {self.inside(x, to)[2]})" for x, _ in amb))
                        mods = rest
                    if pick:
                        mods = [pick]
                        notes.append(why)
                        # to the symbol the C names, + its offset, where the overlay has no symbol at the target
                        a = self.named_at(mod, fr, pick, to + add)
                        if a is not None and not self.at(pick, to) and not self.at(pick, to & ~1, ("function",)):
                            to, add = a, to + add - a
                        amb = [(o, to) for o in mods_of(spec) if o != pick]
                    elif why and wanted(files | {self.source_at(mod, fr)}):
                        acts.append(("conflict", f"{mod} {fr:#010x} to:{to:#010x} {spec}: {why}", files, None))
                o = self.inside(mods[0], to) if len(mods) == 1 else None
                c = self.named_at(mod, fr, mods[0], to + add) if len(mods) == 1 else None
                ci = bisect.bisect_left(self.starts.get(mods[0], []), c) if c is not None else -1
                cobj = self.objs[mods[0]][ci] if 0 <= ci < len(self.objs[mods[0]]) and \
                    self.objs[mods[0]][ci][0] == c else None
                folded = c is not None and self.at(mods[0], c) and (to + add < c or cobj and to + add >= c + cobj[1])
                if folded:
                    # the C names its table outside it (table[i - 1]: sDayOfYear - 4, rtc/convert.c), no field of
                    # the object before: the table + the offset
                    if (to, add) != (c, to + add - c):
                        notes.append(f"the C names {self.refs[mod, fr][0]}{to + add - c:+#x}")
                    files |= {self.refs[mod, fr][2]} | ({cobj[4]} if cobj else set())
                    to, add, o = c, to + add - c, None
                files = files | ({o[4]} if o else set())
                if (o or folded or spec_of(mods) != spec) and not wanted(files | {self.source_at(mod, fr)}):
                    continue
                if o and self.at(mods[0], o[0]):
                    notes.append(f"inside {o[2]} ({o[4]})")
                    cands.setdefault((mods[0], to), (f"inside {o[2]}+{to - o[0]:#x} ({o[4]})", {o[4]}, False))
                    to, add = o[0], to + add - o[0]
                elif o:
                    acts.append(("keep", f"{mod} {fr:#010x} to:{to:#010x}: inside {o[2]} ({o[4]}), which has no dsd "
                                         f"symbol at {o[0]:#x}", {o[4]}, None))
                if (to, add, spec_of(mods)) == old:
                    continue
                new = f"from:{m[1]} kind:{m[2]} to:{to:#010x}" + (f" add:{hex(add)}" if add else "") + \
                      f" module:{spec_of(mods)}{m[6]}"
                edited[mod, k] = new
                acts.append(("overlay" if spec_of(mods) != spec else "inner",
                             f"{mod} {fr:#010x}: {line[len(m[1]) + 5:].strip()} -> {new[len(m[1]) + 5:].strip()}: "
                             f"{'; '.join(notes)}", files, (mod, k, new)))
                if spec_of(mods) != spec:
                    for c in amb:
                        cands.setdefault(c, (f"{line[len(m[1]) + 5:].strip()} is {spec_of(mods)}", files, True))
        for mod, objs in self.objs.items():
            addrs = self.sym_addrs.get(mod, [])
            for a, size, name, _, src in objs:
                for x in addrs[bisect.bisect_right(addrs, a):bisect.bisect_left(addrs, a + size)]:
                    if wanted({src}):
                        cands.setdefault((mod, x), (f"inside {name}+{x - a:#x} ({src})", {src}, False))
        return acts + self.symbols_going(cands, edited)

    def symbols_going(self, cands, edited):
        """The symbols of cands {(module, address): (why, {sources}, only dsd's ambiguous ones)} that go: no function
        or label, nothing reaches them after the relocs.txt edits ({(module, index): line}), no C file, header or
        assembly file names them."""
        targets = set()
        for mod, lines in self.reloc_lines.items():
            for k, line in enumerate(lines):
                m = RELOC.match(edited.get((mod, k), line))
                if m:
                    targets.update((t, int(m[3], 16)) for t in mods_of(m[5]))
        found = [(c, s) for c, v in sorted(cands.items()) for s in self.at(*c) if s[2] not in ("function", "label")
                 and (not v[2] or "ambiguous" in s[3] or self.inside(*c))]
        named, out = self.named({s[1] for _, s in found}), []
        for (mod, a), (k, name, _, _) in found:
            why, files, _ = cands[mod, a]
            if (mod, a) in targets:
                out.append(("keep", f"{mod} {name}: {why}, but a relocation still reaches it", files, None))
            elif name in named:
                out.append(("keep", f"{mod} {name}: {why}, but {named[name][0]} names it", files, None))
            else:
                out.append(("symbol", f"{mod} {name} at {a:#010x} goes: {why}", files, (mod, k)))
        return out

    def named(self, names):
        """{name: [path]} of the names that a file of src/, lib/ or include/ names (outside its comments)."""
        out = defaultdict(list)
        for d in ("src", "lib", "include"):
            for path in sorted(glob.glob(f"{self.root}/{d}/**/*", recursive=True)):
                if re.search(r"\.(c|cpp|h|inc|s)$", path):
                    for w in set(re.findall(r"\w+", uncommented(open(path, errors="replace").read()))) & names:
                        out[w].append(os.path.relpath(path, self.root))
        return out

    def apply(self, acts):
        """Writes the edits of the plan to relocs.txt and symbols.txt. Returns the paths written."""
        lines = {}
        for kind, _, _, edit in acts:
            if kind in ("overlay", "inner"):
                mod, k, new = edit
                lines.setdefault((mod, "relocs"), list(self.reloc_lines[mod]))[k] = new
            elif kind == "symbol":
                mod, k = edit
                lines.setdefault((mod, "symbols"), list(self.sym_lines[mod]))[k] = None
        for key, rows in lines.items():
            open(self.paths[key], "w").write("".join(r + "\n" for r in rows if r is not None))
        return sorted(self.paths[key] for key in lines)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="*", help="only the cases these sources make (default: every file)")
    ap.add_argument("--apply", action="store_true", help="write symbols.txt and relocs.txt")
    ap.add_argument("-q", "--quiet", action="store_true", help="counts only")
    ap.add_argument("--placed", action="append", default=[], metavar="SOURCE",
                    help="a complete file whose ranges are newer than the link map: placed at them (fit_data.py)")
    ap.add_argument("--root", default=ROOT, help=argparse.SUPPRESS)  # another tree's config (tests)
    args = ap.parse_args()
    tree = Tree(args.root, args.placed)
    acts = tree.plan(set(args.files) or None)
    heads = {"inner": "relocations into one of our objects -> to:<object> add:N",
             "overlay": "ambiguous relocations -> one overlay", "symbol": "symbols that go",
             "keep": "kept (printed only)", "conflict": "conflicts: nothing changed",
             "rename": "C names to change (another overlay's symbol at the address: printed only)"}
    for kind, head in heads.items():
        rows = [text for k, text, _, _ in acts if k == kind]
        if rows:
            print(f"{len(rows)} {head}")
            for text in rows if not args.quiet else ():
                print(f"  {text}")
    if tree.unmapped:
        print(f"{len(tree.unmapped)} complete files the link map (build/b_us/arm9.o.xMAP) doesn't have, placed at "
              f"their ranges (ninja relinks): {', '.join(tree.unmapped[:8])}{' ...' if len(tree.unmapped) > 8 else ''}")
    if tree.missing:
        print(f"no object (ninja it first): {', '.join(tree.missing)}")
    if not acts:
        print("nothing to change")
    if args.apply:
        written = tree.apply(acts)
        print(f"wrote {len(written)} files" + "".join(f"\n  {os.path.relpath(p, ROOT)}" for p in written))
        if written:
            print("now: ninja && python3 tools/regressions.py HEAD && python3 tools/row_audit.py --changed")
    elif any(k in ("inner", "overlay", "symbol") for k, *_ in acts):
        print("(dry run: --apply writes them)")


if __name__ == "__main__":
    main()
