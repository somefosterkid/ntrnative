#!/usr/bin/env python3
"""Give a Black file proposed by tools/twin_files.py its full extent: every section, as add_source_file.py arguments.

.text: twin_files.py's range runs from the first twin to the last, and over X's functions that pass its file name to
an assert or the heap (tools/name_files.py), which never twin: their line numbers changed. Walking out from it to the
next function twinned to another pokebw2 file Y (or a file already in Black's delinks.txt, or the module's edge), the
untwinned functions in between are X's remaining B2 functions plus Y's, by count. A twin under 0x10 bytes is no
evidence (any getter), nor is one whose file Y has more functions on X's side than the gap holds: that is X's own
function with the same code as Y's. When the counts don't add up (Black lacks some of Y's functions), the nearest
untwinned functions with the sizes of X's remaining B2 functions, in order, are X's if they add up to BY_SIZE bytes;
else the range takes X's first if nothing of Y is left there, else none, and says "check" with the sizes of the
functions taken and of the B2 file's.

.rodata, .data, .bss: the symbols X's functions load (relocs.txt), the data that points to X's functions and to no
other code of the module (a table of callbacks only other modules use), and what that data points to in .rodata and
.data, but not a table of other files' functions. A symbol that code outside X loads too counts only next to X's own
ones, with no symbol only others load in between. Each range ends at the next symbol, and grows toward the size of the
B2 file's section (pokebw2's delinks.txt) over symbols no other code of the module loads: a table's tail,
unreferenced data. A section that only other files' code loads is where the twins of that code load the B2 file's
(same offsets in functions of the same size). A size that still differs from B2's is printed.

    python3 tools/file_ranges.py src/gfl/strbuf.c src/ov294/intro_mcss.c   # rows of build/twin_files.tsv
    python3 tools/file_ranges.py --check     # every file in Black's delinks.txt: does it reproduce their ranges?
"""
import argparse
import bisect
import functools
import glob
import os
import re
from collections import defaultdict

import bwcache
from callee_names import read_relocs
from twin_files import module_of, read_symbols

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA = (".rodata", ".data", ".bss")
TINY = 0x10  # bytes: a twin smaller than this is no evidence of its file
BY_SIZE = 0x40  # bytes: untwinned functions with the sizes of the B2 file's, in order, are its own from this many
# The sections a file's code can be in: TwlSDK's LTD files (twl/ltdmain_begin.h's `#pragma section LTDMAIN`) have
# theirs in the LTD autoload's .ltdmain, and are judged, ported and checked as a .text file is (t34d, t34e)
CODE = (".text", ".ltdmain")


def code_section(secs):
    """The section of a file's code among its delinks.txt ranges {section: (start, end)} (or a module header's):
    .text, else .ltdmain, else None (a data-only file)."""
    return next((s for s in CODE if s in secs), None)


def read_delinks(root):
    """Returns {module: (header {section: (start, end)}, {source: {section: (start, end)}})}. Read from
    build/cache/delinks_ranges while no delinks.txt of the tree changes (size and mtime): brief.py asks for it often."""
    paths = glob.glob(f"{root}/**/delinks.txt", recursive=True)
    token = bwcache.files_token(sorted(paths), __file__)
    out = bwcache.get("delinks_ranges", root, token)
    if out is None:
        out = _read_delinks(paths)
        bwcache.put("delinks_ranges", root, token, out)
    return out


def _read_delinks(paths):
    out = {}
    for path in paths:
        header, files, src = {}, {}, None
        for line in open(path):
            if line.strip() and not line[0].isspace():
                src = line.strip().rstrip(":")
                files[src] = {}
            m = re.match(r"\s+(\.\w+)\s+start:(0x\w+) end:(0x\w+)", line)
            if m:
                (files[src] if src else header)[m[1]] = (int(m[2], 16), int(m[3], 16))
        out[module_of(path)] = (header, files)
    return out


def module_dir(mod):
    return "." if mod == "main" else f"overlays/{mod}" if re.match(r"ov\d+$", mod) else mod


@functools.lru_cache(maxsize=None)
def _lib_path(bw2_source):
    lib = "/".join(bw2_source.split("/")[:3])
    if not os.path.isfile(f"{ROOT}/{bw2_source}") and os.path.isdir(f"{ROOT}/{lib}"):
        same = glob.glob(f"{ROOT}/{lib}/**/{os.path.basename(bw2_source)}", recursive=True)
        if len(same) == 1:
            return os.path.relpath(same[0], ROOT)
    return bw2_source


def black_path(bw2_source, mod):
    """Where a pokebw2 source goes in this repo: game code under src/<module>/, pokebw2's src/gfl/ in lib/gfl/src/."""
    if bw2_source.startswith("src/gfl/"):
        return "lib/gfl/src/" + bw2_source[len("src/gfl/"):]
    if bw2_source.startswith("lib/"):
        # Black keeps GameSpy under gs/ (lib/dwc/src/gs/serverbrowsing/sb_serverlist.c for pokebw2's
        # lib/dwc/src/serverbrowsing/sb_serverlist.c): the one file of Black's library with the name, if the path is not
        return _lib_path(bw2_source)
    if mod == "main":
        return bw2_source
    return f"src/{mod}/{os.path.basename(bw2_source)}"


class Ranges:
    def __init__(self, bw2, config, twins_tsv):
        self.bw2, self._b2_data, self._anchors, self._empties = bw2, None, {}, {}
        self.funcs = read_symbols(config)
        self.b2_funcs = read_symbols(f"{bw2}/config/b2_us/arm9")
        self.delinks = read_delinks(config)
        self.b2_delinks = read_delinks(f"{bw2}/config/b2_us/arm9")
        self.relocs = read_relocs(config)
        self.syms = defaultdict(set)  # module -> {address of every symbol}
        for path in glob.glob(f"{config}/**/symbols.txt", recursive=True):
            for line in open(path):
                m = re.search(r"addr:(0x\w+)", line)
                if m:
                    self.syms[module_of(path)].add(int(m[1], 16))
        self.syms = {mod: sorted(a) for mod, a in self.syms.items()}
        self.twins = defaultdict(set)  # (module, Black address) -> {(pokebw2 source, B2 function)}
        for line in list(open(twins_tsv))[1:]:
            _, mod, addr, _, src, fn, _ = line.rstrip("\n").split("\t")
            self.twins[mod, int(addr, 16)].add((src, fn))
        self.b2_files = {src: (mod, secs) for mod, (_, files) in self.b2_delinks.items() for src, secs in files.items()}
        # Who loads each data address: {module: {address: {(module, from)}}}, and the addresses in order
        self.loads = defaultdict(lambda: defaultdict(set))
        for mod, rows in self.relocs.items():
            for fr, to, add, tmod, kind in rows:
                if kind == "load" and tmod:
                    self.loads[tmod][to].add((mod, fr))
        self.targets = {mod: sorted(t) for mod, t in self.loads.items()}

    def image(self, mod):
        """The module's bytes in the ROM and its base address, or (b"", 0)."""
        path = f"{ROOT}/extract/b_us/arm9/" + ("arm9.bin" if mod == "main" else f"../arm9_overlays/{mod}.bin")
        header = self.delinks[mod][0]
        if not os.path.exists(path) or ".text" not in header:
            return b"", 0
        return open(path, "rb").read(), min(s for sec, (s, _) in header.items() if sec != ".bss")

    def anchors(self, mod):
        """{file name: [address of each function that loads it]}: the names a module embeds (an assert's or
        GFL_HeapAllocate's argument), which only that file's functions pass."""
        if mod not in self._anchors:
            header, out = self.delinks[mod][0], defaultdict(set)
            image, base = self.image(mod)
            names = {base + m.start(): m[1].decode() for m in re.finditer(rb"(?<![\w.])(\w+\.c)\x00", image)}
            addrs = [a for a, _, _ in self.funcs.get(mod, [])]
            for fr, to, _, tmod, _ in self.relocs.get(mod, []):
                if tmod == mod and to in names and header[".text"][0] <= fr < header[".text"][1]:
                    out[names[to]].add(addrs[bisect.bisect_right(addrs, fr) - 1])
            self._anchors[mod] = {n: sorted(a) for n, a in out.items()}
        return self._anchors[mod]

    def empty_strings(self, mod):
        """{address: [address of each function that loads it]} of the module's "" objects: 4 zero bytes in .data that
        code loads, GFL_ASSERT's. MWCC pools one per file, so two that different functions use are two files (20
        complete files hold one each, none two)."""
        if mod not in self._empties:
            header, out = self.delinks[mod][0], defaultdict(set)
            image, base = self.image(mod)
            ds, de = header.get(".data", (0, 0))
            syms = set(self.syms.get(mod, ()))
            addrs = [a for a, _, _ in self.funcs.get(mod, [])]
            for fr, to, _, tmod, kind in self.relocs.get(mod, []) if image else ():
                if tmod == mod and kind == "load" and ds <= to < de and to in syms and self.code(mod, fr) and \
                        self.end_of(mod, to, de) - to == 4 and image[to - base:to - base + 4] == bytes(4):
                    out[to].add(addrs[bisect.bisect_right(addrs, fr) - 1])
            # an assert's: one of its users calls GFL_DebugAssertFail (a table's zero words that dsd split aren't)
            fails = {a for a, _, n in self.funcs["main"] if n.startswith("GFL_DebugAssertFail")}
            asserts = {addrs[bisect.bisect_right(addrs, fr) - 1] for fr, to, _, tmod, kind in self.relocs.get(mod, [])
                       if tmod == "main" and to & ~1 in fails and "call" in kind and self.code(mod, fr)}
            self._empties[mod] = {a: sorted(f) for a, f in out.items() if f & asserts}
        return self._empties[mod]

    def b2_order(self, src):
        """pokebw2's functions of a file, in order: [(name, size)]."""
        mod, secs = self.b2_files[src]
        start, end = secs[code_section(secs)]
        return [(n, s) for a, s, n in self.b2_funcs.get(mod, []) if start <= a < end]

    def text(self, src, mod, lo, hi, own=None):
        """Widens [lo, hi) to the file's whole .text. Returns (start, end, notes)."""
        b2_sizes = self.b2_order(src)
        order = [n for n, _ in b2_sizes]
        index = {n: i for i, n in enumerate(order)}
        funcs = self.funcs[mod]
        addrs = [a for a, _, _ in funcs]
        known = [secs[".text"] for s, secs in self.delinks[mod][1].items() if ".text" in secs and s != own]
        module_text = self.delinks[mod][0].get(".text", (0, 1 << 32))
        inside = [self.twins[mod, a] for a in addrs[bisect.bisect_left(addrs, lo):bisect.bisect_left(addrs, hi)]]
        # a twin under a name pokebw2's symbols.txt doesn't give the function (a static the C names, which dsd has
        # as a placeholder: shortcut_menu.c's FreeMenu) is X's, its place in the order unknown
        b2_inside = [index[fn] for t in inside for s, fn in t if s == src and fn in index]
        notes = []
        # X's functions that pass its name to an assert or the heap, which never twin (their line numbers changed),
        # unless a file in delinks.txt lies between
        i, j = bisect.bisect_left(addrs, lo), bisect.bisect_left(addrs, hi)
        for a in self.anchors(mod).get(os.path.basename(src), []):
            between = (a, lo) if a < lo else (hi, a + 1)
            if not any(s < between[1] and between[0] < e for s, e in known):
                k = bisect.bisect_right(addrs, a) - 1
                i, j = min(i, k), max(j, k + 1)
        extra = (bisect.bisect_left(addrs, lo) - i, j - bisect.bisect_left(addrs, hi))  # X's functions on each side
        lo, hi = addrs[i], addrs[j - 1] + 1

        def walk(i, step, first, last):
            """Walks out from funcs[i]. Returns the index of the outermost function that is X's."""
            gap, taken = [], i - step  # functions outside with no clear twin; the outermost one surely X's
            own_left = first if step < 0 else len(order) - 1 - last  # X's B2 functions not yet seen on this side
            while True:
                if not 0 <= i < len(funcs) or not module_text[0] <= funcs[i][0] < module_text[1]:
                    other, why = 0, "the module's edge"
                    break
                if any(s <= funcs[i][0] < e for s, e in known):
                    other, why = 0, "a file in delinks.txt"
                    break
                twins = self.twins[mod, funcs[i][0]]
                files = {s for s, _ in twins}
                if files == {src}:  # one more of X's twins
                    k = index.get(next(fn for _, fn in twins))
                    if k is not None:
                        k = k if step < 0 else len(order) - 1 - k
                        own_left = k if k < own_left else max(own_left - 1, 0)  # out of order: a copy, but X's
                    gap, taken = [], i
                    i += step
                    continue
                if len(files) == 1 and funcs[i][1] >= TINY:  # a tiny twin can be any getter: no evidence
                    y, fn = next(iter(twins))
                    y_order = [n for n, _ in self.b2_order(y)] if y in self.b2_files else [fn]
                    k = y_order.index(fn)
                    other = len(y_order) - 1 - k if step < 0 else k  # Y's B2 functions between it and X
                    why = f"{y} ({fn})"
                    # Y's twin is in place only if Y's functions between it and X fit in the gap. Otherwise it is
                    # one of X's own, with the same code as Y's (twins.py gives each Black function one twin).
                    if other <= len(gap):
                        break
                gap.append(i)
                if len(gap) > own_left + 16:
                    other, why = None, "nothing within 16 functions"
                    break
                i += step
            exact = other is not None and len(gap) == own_left + other
            n = own_left if exact else min(own_left, len(gap)) if other == 0 else 0  # X first when Y has none there
            b2 = b2_sizes[:own_left] if step < 0 else b2_sizes[len(order) - own_left:]
            # Counts that don't add up (Black lacks some of Y's functions): X's are the nearest ones with the sizes of
            # its remaining B2 functions, in order, if they add up to BY_SIZE bytes
            near = b2[::-1] if step < 0 else b2
            k = next((k for k, (g, (_, size)) in enumerate(zip(gap, near)) if funcs[g][1] != size),
                     min(len(gap), len(near)))
            by_size = not exact and not n and sum(size for _, size in near[:k]) >= BY_SIZE
            n = k if by_size else n
            if n or not exact:
                side = "start" if step < 0 else "end"
                sizes = lambda fs: ", ".join(f"{s:#x}" for s in fs) or "none"
                notes.append(("" if exact or by_size else "check the ") + f"{side}: {n} of {len(gap)} untwinned "
                             f"functions up to {why} taken" + (" by their sizes" if by_size else "") +
                             f", of {sizes(funcs[g][1] for g in gap[:n])} bytes; the B2 file's {own_left} are of "
                             f"{sizes(size for _, size in b2)}" + ("" if exact else f", {other} more of {why}"))
            return gap[n - 1] if n else taken

        first, last = (min(b2_inside), max(b2_inside)) if b2_inside else (0, len(order) - 1)
        first, last = max(first - extra[0], 0), min(last + extra[1], len(order) - 1)
        start = funcs[walk(bisect.bisect_left(addrs, lo) - 1, -1, first, last)][0]
        j = walk(bisect.bisect_left(addrs, hi), 1, first, last)
        end = funcs[j][0] + funcs[j][1]
        end = min([(end + 3) & ~3] + [a for a in addrs if a >= end])
        return start, end, notes

    def data(self, mod, start, end, own=None, b2=None):
        """The data sections the functions in [start, end) own, given the B2 file (module, {section: (start, end)}).
        Returns ({section: (start, end)}, notes)."""
        b2_mod, b2 = b2 or (None, {})
        header, files = self.delinks[mod]
        known = [r for s, secs in files.items() if s != own for sec, r in secs.items() if sec in DATA]
        syms = self.syms[mod]
        t = self.targets.get(mod, [])

        def symbol(addr):
            return syms[bisect.bisect_right(syms, addr) - 1]

        def others(sym, here=False):
            """Does code outside X (in this module, with here) load the symbol at sym?"""
            e = self.end_of(mod, sym, 1 << 32)
            return any(self.code(m, fr) and (m == mod and not start <= fr < end or m != mod and not here)
                       for a in t[bisect.bisect_left(t, sym):bisect.bisect_left(t, e)] for m, fr in self.loads[mod][a])

        def section(addr):
            return next(((s0, s1) for s0, s1 in (header[s] for s in DATA if s in header) if s0 <= addr < s1), None)

        # What X's code loads, and the data that points to X's functions and to no other code of the module (a table
        # of callbacks that only other modules use), then what that data points to, in any section
        text0, text1 = header.get(".text", (0, 0))
        to_code = defaultdict(set)  # data symbol -> does it point to code outside X?
        for fr, to, _, tmod, _ in self.relocs[mod]:
            if tmod == mod and text0 <= to < text1 and section(fr):
                to_code[symbol(fr)].add(not start <= to & ~1 < end)
        todo = [(to, True) for fr, to, _, tmod, kind in self.relocs[mod]
                if kind == "load" and tmod == mod and start <= fr < end and section(to)]
        todo += [(sym, True) for sym, outside in to_code.items() if outside == {False}]
        bss = header.get(".bss", (0, 0))
        refs = {}  # symbol -> loaded by other code of this module too
        while todo:
            addr, direct = todo.pop()
            sym = symbol(addr)
            # a section's first datum without a symbol has the symbol before it, in another section
            if sym in refs or not section(sym) or any(a <= sym < b for a, b in known):
                continue
            # Not through a pointer: a table of other files' functions (theirs), or .bss (dsd reads some byte tables
            # as pointers)
            if not direct and (True in to_code.get(sym, ()) or bss[0] <= sym < bss[1]):
                continue
            refs[sym] = others(sym)
            e = self.end_of(mod, sym, section(sym)[1])
            todo += [(x, False) for f, x, _, tmod, _ in self.relocs[mod] if sym <= f < e and tmod == mod and section(x)]
        all_refs = refs

        out, notes = {}, []
        for sec in DATA:
            if sec not in header:
                continue
            # the files' order: X's data lies after that of the files whose code comes first, before the others'. What
            # X loads outside is another file's (fieldmap.c loads a table after field_camera.c's data)
            s0 = max([secs[sec][1] for s, secs in files.items() if s != own and sec in secs and code_section(secs)
                      and secs[code_section(secs)][0] < start], default=header[sec][0])
            s1 = min([secs[sec][0] for s, secs in files.items() if s != own and sec in secs and code_section(secs)
                      and secs[code_section(secs)][0] >= end], default=header[sec][1])
            size = b2[sec][1] - b2[sec][0] if sec in b2 else 0
            refs = {a: shared for a, shared in all_refs.items() if header[sec][0] <= a < header[sec][1]}
            theirs = sorted(a for a in refs if not s0 <= a < s1)
            if theirs and len(theirs) < len(refs):
                notes.append(f"{sec}: {', '.join(map(hex, theirs[:4]))} out of the files' order, another file's")
            refs = {a: shared for a, shared in refs.items() if s0 <= a < s1} if len(theirs) < len(refs) else refs
            if not refs:
                # data only other files' code loads: where the twins of that code put the B2 file's
                at = {to - (a - b2[sec][0]) for (m, a), (tm, to) in self.b2_data().items()
                      if m == b2_mod and sec in b2 and b2[sec][0] <= a < b2[sec][1] and tm == mod}
                if len(at) == 1 and s0 <= min(at) and min(at) + size <= s1 and not any(
                        x < min(at) + size and min(at) < y for x, y in known):
                    out[sec] = (min(at), min(at) + size)
                continue
            excl = [a for a, shared in refs.items() if not shared]
            if excl:
                lo, hi = min(excl), max(excl)
                # a symbol X shares with others is X's if no symbol only others load lies between it and X's own
                theirs = {symbol(a) for a in t[bisect.bisect_left(t, s0):bisect.bisect_left(t, s1)]
                          if not any(m == mod and start <= fr < end for m, fr in self.loads[mod][a]) and others(a)}
                for a in sorted(refs, key=lambda a: min(abs(a - lo), abs(a - hi))):
                    if not any(min(a, lo) < x < max(a, hi) for x in theirs):
                        lo, hi = min(lo, a), max(hi, a)
            elif size and self.end_of(mod, max(refs), s1) - min(refs) <= size:
                lo, hi = min(refs), max(refs)
            else:
                notes.append(f"{sec}: only symbols that other code loads too: {', '.join(map(hex, sorted(refs)))}")
                continue
            e = self.end_of(mod, hi, s1)
            # grow toward the B2 file's size over symbols no other code loads: tails of tables, unreferenced data
            free = lambda a: not others(a, here=True) and not any(x <= a < y for x, y in known)
            while e - lo < size and e < s1 and free(e) and self.end_of(mod, e, s1) - lo <= size:
                e = self.end_of(mod, e, s1)
            # not to a symbol before the section: nhttp_rap.c's .rodata grew from the section's first symbol to the
            # last function of .text, and dsd refused it
            while e - lo < size and lo > s0 and symbol(lo - 1) >= s0 and free(symbol(lo - 1)) and \
                    e - symbol(lo - 1) <= size:
                lo = symbol(lo - 1)
            # A section starts 4-aligned, so a symbol at an unaligned address is the rest of the object before it (a
            # string's tail): a file ending just before it left a gap the link moved, and the module's checksum
            # failed (draw_system.c's .rodata ended at data_ov172_021c6079, 2026-10-13)
            # never over another file's range, nor before the section (pms_input_data.c's .rodata from 0x021c1852 went
            # to a function at 0x021c1848, and dsd refused it: the section starts at 0x021c184c)
            taken = lambda a, b: any(a < max(y, x + 1) and x < b for x, y in known)
            while e % 4 and e < s1 and not taken(lo, self.end_of(mod, e, s1)):
                e = self.end_of(mod, e, s1)
            while lo % 4 and lo > s0 and not taken(max(symbol(lo - 1), s0), e):
                lo = max(symbol(lo - 1), s0)
            out[sec] = (lo, e)
        # Each section holds the files' data in the files' order: data before that of a file whose code comes first, or
        # after that of one whose code comes later, is another file's that X loads (a link-order cycle in dsd otherwise)
        for sec, (lo, e) in list(out.items()):
            for s, secs in files.items():
                code = secs.get(code_section(secs))
                if s != own and code and sec in secs and (
                        code[0] < start and secs[sec][0] > lo or code[0] >= end and secs[sec][0] < lo):
                    del out[sec]
                    notes.append(f"{sec}: {lo:#x}-{e:#x} would be out of order with {s}'s, so another file's")
                    break
        # one "" per file: code that uses two is two files, split between their users
        used = sorted((f, a) for a, fs in self.empty_strings(mod).items() for f in fs if start <= f < end)
        if len({a for _, a in used}) > 1:
            runs = [(a, [f for f, b in used if b == a]) for a in dict.fromkeys(a for _, a in used)]
            notes.append(f"its code uses {len(runs)} \"\" objects, one per file: " + ", ".join(
                f"{a:#x} by {fs[0]:#x}-{fs[-1]:#x}" for a, fs in runs) + "; split it between their users")
        return out, notes

    def b2_data(self):
        """{B2 (module, address): Black (module, address)} of the data that twins of the same size load at the same
        offsets, where they all agree."""
        if self._b2_data is None:
            b2_relocs = read_relocs(f"{self.bw2}/config/b2_us/arm9")
            froms = {id(rows): {m: [row[0] for row in rs] for m, rs in rows.items()} for rows in (b2_relocs, self.relocs)}
            at = lambda rows, m, a, n: rows[m][bisect.bisect_left(froms[id(rows)][m], a):
                                               bisect.bisect_left(froms[id(rows)][m], a + n)]
            b2_fn = {(m, n): (a, sz) for m, fs in self.b2_funcs.items() for a, sz, n in fs}
            size = {(m, a): sz for m, fs in self.funcs.items() for a, sz, _ in fs}
            votes = defaultdict(set)
            for (m, a), fns in self.twins.items():
                for src, fn in fns:
                    b2m = self.b2_files.get(src, (None,))[0]
                    b2a, sz = b2_fn.get((b2m, fn), (None, None))
                    if sz is None or sz != size.get((m, a)) or b2m not in b2_relocs or m not in self.relocs:
                        continue
                    theirs = {f - b2a: (tm, t) for f, t, _, tm, k in at(b2_relocs, b2m, b2a, sz) if k == "load" and tm}
                    for f, t, _, tm, k in at(self.relocs, m, a, sz):
                        if k == "load" and tm and f - a in theirs:
                            votes[theirs[f - a]].add((tm, t))
            self._b2_data = {k: v.pop() for k, v in votes.items() if len(v) == 1}
        return self._b2_data

    def code(self, mod, addr):
        start, end = self.delinks[mod][0].get(".text", (0, 0))
        return start <= addr < end

    def end_of(self, mod, sym, limit):
        syms = self.syms[mod]
        i = bisect.bisect_right(syms, sym)
        return min(syms[i], limit) if i < len(syms) else limit

    def compare_b2(self, src, ranges):
        """Notes where Black's sections differ in size from the B2 file's."""
        notes = []
        b2 = self.b2_files[src][1]
        for sec in DATA:
            ours = ranges[sec][1] - ranges[sec][0] if sec in ranges else 0
            theirs = b2[sec][1] - b2[sec][0] if sec in b2 else 0
            if ours != theirs:
                notes.append(f"{sec}: {ours:#x} bytes here, {theirs:#x} in B2")
        return notes


def read_rows(path):
    """build/twin_files.tsv: {pokebw2 source: (module, start, end, Black files it overlaps)}."""
    rows = {}
    for line in list(open(path))[1:]:
        f = line.rstrip("\n").split("\t")
        if f[5]:
            rows[f[1]] = (f[4], int(f[5], 16), int(f[6], 16), f[11])
    return rows


def propose(r, rows, src):
    """Returns the module, {section: (start, end)} and notes of the Black file twinned to a pokebw2 file."""
    mod, lo, hi, overlaps = rows[src]
    own = black_path(src, mod)  # in delinks.txt already, its own entry is no other file's
    start, end, notes = r.text(src, mod, lo, hi, own=own)
    ranges, more = r.data(mod, start, end, own=own, b2=r.b2_files[src])
    ranges = {".text": (start, end), **ranges}
    return mod, ranges, notes + more + r.compare_b2(src, ranges) + ([f"overlaps {overlaps}"] if overlaps else [])


def fmt(ranges):
    return " ".join(f"{sec}:{s:#010x}-{e:#010x}" for sec, (s, e) in ranges.items())


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("sources", nargs="*", help="pokebw2 sources, as in build/twin_files.tsv")
    ap.add_argument("--bw2", default=os.path.expanduser("~/ref/pokebw2"), help="pokebw2 checkout (read only)")
    ap.add_argument("--config", default=f"{ROOT}/config/b_us/arm9")
    ap.add_argument("--twins", default=f"{ROOT}/build/twins_b_us.tsv", help="written by tools/twins.py")
    ap.add_argument("--files", default=f"{ROOT}/build/twin_files.tsv", help="written by tools/twin_files.py")
    ap.add_argument("--check", action="store_true", help="compare with the files already in Black's delinks.txt")
    args = ap.parse_args()

    r = Ranges(args.bw2, args.config, args.twins)
    rows = read_rows(args.files)

    if args.check:
        twin_of = {ov: src for src, (_, _, _, ov) in rows.items() if ov and "," not in ov}
        same = total = 0
        for mod, (_, files) in sorted(r.delinks.items()):
            for src, secs in files.items():
                if ".text" not in secs:
                    continue
                b2 = r.b2_files[twin_of[src]] if src in twin_of else None
                got, _ = r.data(mod, *secs[".text"], own=src, b2=b2)
                want = {s: v for s, v in secs.items() if s in DATA}
                total += 1
                same += got == want
                if got != want:
                    print(f"data {src}: delinks {fmt(want) or '-'}, found {fmt(got) or '-'}")
        print(f"data: {same}/{total} files reproduce their delinks.txt ranges")
        same = total = 0
        for src, (mod, lo, hi, overlaps) in rows.items():
            if overlaps and "," not in overlaps and overlaps in r.delinks[mod][1]:
                start, end, _ = r.text(src, mod, lo, hi, own=overlaps)
                got = {".text": (start, end), **r.data(mod, start, end, own=overlaps, b2=r.b2_files[src])[0]}
                want = r.delinks[mod][1][overlaps]
                total += 1
                same += got == want
                if got != want:
                    print(f"all {overlaps}: delinks {fmt(want)}, found {fmt(got)}")
        print(f"all sections from the twins alone: {same}/{total} twinned files reproduce their delinks.txt ranges")
        return

    for src in args.sources:
        if src not in rows:
            print(f"{src}: no twins in build/twin_files.tsv")
            continue
        mod, ranges, notes = propose(r, rows, src)
        print(f"python3 tools/add_source_file.py {black_path(src, mod)} {module_dir(mod)} {fmt(ranges)}")
        for n in notes:
            print(f"    {n}")

if __name__ == "__main__":
    main()
