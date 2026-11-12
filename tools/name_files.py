#!/usr/bin/env python3
"""Propose Pokemon Black's source files from the file names its code embeds, where no file or twin places the code.

Many functions pass their file's name to an assert or to GFL_HeapAllocate, so a module holds strings such as
"bmp_oam.c", and the functions that load one are that file's (its anchors, Ranges.anchors in tools/file_ranges.py).
They never twin (their line numbers changed), and the code no file in a delinks.txt and no queue row covers (the local
worker's rows and those tools/prep.py takes, widened by tools/file_ranges.py) is about a third of Black's. Every
original file is one object: its functions are contiguous, and the files' data come in the files' order in each
section. So in that code a name's file runs at least from its first anchor to its last, and its edges are chosen for
all names of a stretch at once, adding up the costs of: each call or function address (direct, or through a table only
that code loads) across an edge (CALL); each data section whose data only this code loads would be out of order
(ORDER); a window's function count and bytes against pokebw2's file of that name (code that changed keeps about its
size; a name without one stays at its anchors, GROW); and code left between two files that are next to each other in
pokebw2 (FREE), unless one of them is a queue row, whose range may be short. Each file has one "" (GFL_ASSERT's,
Ranges.empty_strings): one its anchors' functions use between them is the name's, and a window that holds two or part
of one's users costs EMPTY. Code left over is a row of its own, unnamed, cut between its "" objects, with the pokebw2
files between its neighbors as candidates. A name that is a neighbor's file is that file's own code: a widen row for a
file in a delinks.txt, a note for a queue row. (A cost for edges where the .rodata sizes don't drop, MWCC sorting a
file's small objects by size, found no more edges: 111 right and 30 wrong in --check's hidden runs, against 112 and
31 without it.)

    python3 tools/name_files.py              # build/name_files.tsv, the setup command for each name, and conflicts
    python3 tools/name_files.py --check      # complete files: does it find their .text, and where they meet?
    python3 tools/name_files.py ov021 --profile 0x0219c000 0x0219d000   # each function's edge costs and anchor

build/name_files.tsv has build/twin_files.tsv's columns, then its own, so tools/brief.py shows pokebw2's C for its files
and `tools/file_ranges.py --files build/name_files.tsv <pokebw2 file>` (so tools/local_driver.py's RANGES) reads it.
Class name: the evidence agrees; check: read the notes; widen: a file in a delinks.txt misses its own functions;
unnamed. A name row is set up with tools/port_file.py when pokebw2 has the file, else tools/add_source_file.py
--incomplete, as printed. Files and queue rows whose range holds another file's anchor are printed as conflicts.
"""
import argparse
import bisect
import os
from collections import Counter, defaultdict

from file_ranges import DATA, ROOT, TINY, Ranges, black_path, fmt, module_dir

# build/twin_files.tsv's columns (empty where they say nothing here, overlaps the file a widen row widens), then ours
COLUMNS = ("class bw2_source bw2_complete bw2_functions module start end black_functions twins contiguous same_order "
           "overlaps name ranges notes").split()
FREE = 2  # the cost of code left between two files that are next to each other in pokebw2
CALL, ORDER = 0.1, 0.5  # the cost of a call across a boundary, and of a data section out of order there
GROW = 0.01  # per function a name without a pokebw2 file takes beyond its anchors
EMPTY = 2  # a window that holds two "" objects, or part of one's users


class Module:
    def __init__(self, r, mod):
        self.r, self.mod = r, mod
        self.funcs = r.funcs.get(mod, [])
        self.starts = [a for a, _, _ in self.funcs]
        self.header = r.delinks[mod][0]
        self.text = self.header.get(".text", (0, 0))
        self.anchors = {n: {self.index(a) for a in addrs} for n, addrs in r.anchors(mod).items()}  # name -> {index}
        # the "" objects, one per file: '"" <address>' -> {index of each function that uses it}
        self.empties = {f'"" {a:#x}': {self.index(f) for f in fs} for a, fs in r.empty_strings(mod).items()}
        self.links = []  # (index, index) of code that refers to code
        self.loads = defaultdict(set)  # data symbol -> {function index}, or -1 for data or other code
        to_code = defaultdict(set)  # data symbol -> {function index} it points to
        for fr, to, _, tmod, _ in r.relocs.get(mod, []):
            if tmod != mod:
                continue
            i = self.index(fr) if self.in_text(fr) else None
            if self.in_text(to):
                if i is not None:
                    self.links.append((i, self.index(to & ~1)))
                elif self.section(fr):
                    to_code[self.symbol(fr)].add(self.index(to & ~1))
            elif self.section(to):
                self.loads[self.symbol(to)].add(-1 if i is None else i)
        for sym, users in self.loads.items():  # a table of callbacks ties the code that loads it to them
            self.links += [(i, j) for i in users if i >= 0 for j in to_code.get(sym, ())]

    def index(self, addr):
        return bisect.bisect_right(self.starts, addr) - 1

    def in_text(self, addr):
        return self.text[0] <= addr < self.text[1]

    def section(self, addr):
        return next((sec for sec in DATA if sec in self.header and self.header[sec][0] <= addr < self.header[sec][1]),
                    None)

    def symbol(self, addr):
        syms = self.r.syms[self.mod]
        return syms[bisect.bisect_right(syms, addr) - 1]

    def end(self, j):
        """The .text end of a range whose last function is funcs[j - 1]: the next function's start."""
        return self.starts[j] if j < len(self.funcs) else self.text[1]

    def boundaries(self, lo, hi):
        """{i: (calls, sections)} for a file starting at funcs[i], lo < i < hi, in the code funcs[lo:hi]: the calls and
        function addresses that would cross files, and the data sections whose order it would break."""
        cross = [0] * (hi - lo + 1)
        for a, b in self.links:
            a, b = min(a, b), max(a, b)
            if lo <= a and b < hi and a != b:
                cross[a + 1 - lo] += 1
                cross[b + 1 - lo] -= 1
        own = defaultdict(list)  # section -> [(data symbol only funcs[lo:hi] load, the first and last of them)]
        for sym, users in self.loads.items():
            if users and all(lo <= i < hi for i in users):
                own[self.section(sym)].append((sym, min(users), max(users)))
        out, n = {}, 0
        for i in range(lo + 1, hi):
            n += cross[i - lo]
            order = 0
            for syms in own.values():
                before = [s for s, first, last in syms if last < i]
                after = [s for s, first, last in syms if first >= i]
                order += bool(before and after and max(before) > min(after))
            out[i] = (n, order)
        return out

    def cost(self, at, i):
        """What it costs to start a file at funcs[i], given boundaries() of the stretch."""
        calls, order = at.get(i, (0, 0))
        return CALL * calls + ORDER * order

    def gaps(self, spans):
        """[(lo, hi, the span before, the span after)]: the runs funcs[lo:hi] that no span covers."""
        covered = [None] * len(self.funcs)
        for sp in spans:
            for i in range(bisect.bisect_left(self.starts, sp[0]), bisect.bisect_left(self.starts, sp[1])):
                covered[i] = sp
        out, lo = [], None
        for i, (a, _, _) in enumerate(self.funcs + [(self.text[1], 0, "")]):
            if i < len(self.funcs) and self.in_text(a) and not covered[i]:
                lo = i if lo is None else lo
            elif lo is not None:
                out.append((lo, i, covered[lo - 1] if lo else None, covered[i] if i < len(self.funcs) else None))
                lo = None
        return out


class B2:
    """pokebw2's files: which one a name or a Black file is, and what lies between two of them."""

    def __init__(self, r, files_tsv):
        self.r = r
        self.modmap = Counter()  # (B2 module, Black module): twinned files
        self.of_file = {}  # Black file in a delinks.txt -> the pokebw2 file twin_files.py proposed there
        for line in list(open(files_tsv))[1:]:
            c = line.rstrip("\n").split("\t")
            if c[0] != "none" and c[1] in r.b2_files:
                self.modmap[r.b2_files[c[1]][0], c[4]] += 1
                if c[11] and "," not in c[11]:
                    self.of_file[c[11]] = c[1]
        self.order = defaultdict(list)  # B2 module -> [(start, end, file)]
        for src, (mod, secs) in r.b2_files.items():
            if ".text" in secs:
                self.order[mod].append((*secs[".text"], src))
        for files in self.order.values():
            files.sort()

    def named(self, name, mod):
        """pokebw2's file of that name, in the B2 module whose twins land most in this module."""
        found = [src for src in self.r.b2_files if os.path.basename(src) == name and ".text" in self.r.b2_files[src][1]]
        return max(found, key=lambda src: self.modmap[self.r.b2_files[src][0], mod], default=None)

    def of(self, what, mod):
        """The pokebw2 file of a span: a queue row's own, or a Black file's twin or namesake."""
        if what in self.r.b2_files:
            return what
        return self.of_file.get(what) or self.named(os.path.basename(what), mod)

    def between(self, a, b):
        """The pokebw2 files between files a and b, or None when they aren't in order in one B2 module. A gap in
        pokebw2's own files counts as one unknown file, "?"."""
        if not a or not b or self.r.b2_files[a][0] != self.r.b2_files[b][0]:
            return None
        files = self.order[self.r.b2_files[a][0]]
        _, a_end = self.r.b2_files[a][1][".text"]
        b_start, _ = self.r.b2_files[b][1][".text"]
        if a_end > b_start:
            return None
        out = [src for s, e, src in files if a_end <= s and e <= b_start]
        covered = sum(e - s for s, e, src in files if a_end <= s and e <= b_start)
        return out + (["?"] if covered < b_start - a_end else [])


def known_spans(r, b2, files_tsv):
    """{module: [(start, end, what, its pokebw2 file, in a delinks.txt)]} of the code that files in a delinks.txt and
    queue rows cover: the local worker's rows, then twin_files.py's that tools/prep.py takes, each widened by
    tools/file_ranges.py but not over another."""
    spans = defaultdict(list)
    for mod, (_, fs) in r.delinks.items():
        spans[mod] += [(*secs[".text"], f, b2.of(f, mod), True) for f, secs in fs.items() if ".text" in secs]
    files = {f for _, (_, fs) in r.delinks.items() for f in fs}
    for line in list(open(f"{ROOT}/docs/local-queue.tsv"))[1:] + list(open(files_tsv))[1:]:
        c = line.rstrip("\n").split("\t")
        if c[0] == "none" or not c[5] or int(c[6], 16) - int(c[5], 16) < TINY or black_path(c[1], c[4]) in files:
            continue
        lo, hi = int(c[5], 16), int(c[6], 16)
        taken = [(s, e) for s, e, *_ in spans[c[4]]]
        if any(s < hi and lo < e for s, e in taken):
            continue
        try:
            wide = r.text(c[1], c[4], lo, hi)[:2]
            if not any(s < wide[1] and wide[0] < e for s, e in taken):
                lo, hi = wide
        except (KeyError, ValueError, IndexError):
            pass
        spans[c[4]].append((lo, hi, c[1], c[1] if c[1] in r.b2_files else None, False))
    return spans


def propose(r, b2, m, gap, names):
    """Files for the code of a gap (lo, hi, span before, span after) whose anchors are names {name: {index}}.
    Returns [[class, name or None, pokebw2 file, first index, end index, notes, the file in delinks.txt it widens]]."""
    lo, hi, before, after = gap
    at = m.boundaries(lo, hi)
    size = [0]
    for _, sz, _ in m.funcs[lo:hi]:
        size.append(size[-1] + sz)
    nbytes = lambda s, e: size[e - lo] - size[s - lo]
    blocks = sorted((min(ix), max(ix) + 1, n) for n, ix in names.items())
    span_of = {n: (s0, e0) for s0, e0, n in blocks}
    src = {n: b2.named(n, m.mod) for n in names}
    notes = defaultdict(list)
    for (s0, e0, a), (s1, e1, b) in zip(blocks, blocks[1:]):
        if s1 < e0:
            notes[a].append(f"its code interleaves with {b}'s")
            notes[b].append(f"its code interleaves with {a}'s")
    # a name that is a neighboring queue row's file is that row's code, which the row's range misses
    first = {n: bool(src[n] and before and src[n] == before[3]) for n in names}
    last = {n: bool(src[n] and after and src[n] == after[3]) for n in names}
    extra = {}  # name -> (functions, bytes) of its queue row
    for n in names:
        sp = before if first[n] else after if last[n] else None
        if sp:
            inside = [z for a, z, _ in m.funcs if sp[0] <= a < sp[1]]
            extra[n] = (len(inside), sum(inside))

    def fit(n, s, e):
        """(functions, bytes) of a window with its queue row's, and pokebw2's file's."""
        want = r.b2_order(src[n])
        x = extra.get(n, (0, 0))
        return e - s + x[0], nbytes(s, e) + x[1], len(want), sum(z for _, z in want)

    # one "" per file: a window holds one, all of its users
    empties = gap_empties(m, gap)

    def cost(n, s, e):
        held = [x for x in empties if s <= min(x) and max(x) < e]
        cut = sum(1 for x in empties if not (max(x) < s or e <= min(x)) and x not in held)
        c = EMPTY * (max(len(held) - 1, 0) + cut)
        if not src[n]:
            return c + GROW * (e - s - (span_of[n][1] - span_of[n][0]))
        count, b, count2, b2_size = fit(n, s, e)
        return c + abs(count - count2) / count2 + abs(b - b2_size) / b2_size

    # code left between two files next to each other in pokebw2 costs FREE, unless one is a queue row: its range may
    # be short
    edges = [before[3] if before and before[4] else None] + [src[n] for _, _, n in blocks] + \
        [after[3] if after and after[4] else None]
    free = [FREE if b2.between(a, b) == [] else 0 for a, b in zip(edges, edges[1:])]
    # every name's window at once: dp = {end of the last window: (cost, windows)}
    dp = {lo: (0, [])}
    for k, (s0, e0, n) in enumerate(blocks):
        nxt = blocks[k + 1][0] if k + 1 < len(blocks) else hi
        reach = 2 * len(r.b2_order(src[n])) + 4 if src[n] else 8
        # a queue row's own code runs to the gap's edge, unless another name's code lies between
        starts = [lo, s0] if first[n] else range(max(lo, s0 - reach), s0 + 1)
        ends = [hi, e0] if last[n] else range(e0, max(e0, min(nxt, e0 + reach)) + 1)
        new = {}
        for prev, (c0, w) in dp.items():
            for s in starts:
                if s < min(prev, s0):
                    continue
                c1 = c0 + (m.cost(at, s) + free[k] if s > prev else 0)
                for e in ends:
                    c = c1 + cost(n, s, e) + m.cost(at, e)
                    if e not in new or c < new[e][0]:
                        new[e] = (c, w + [(n, s, e)])
        dp = new
    windows = min(dp.items(), key=lambda x: x[1][0] + (free[-1] if x[0] < hi else 0))[1][1]

    rows, pos, left = [], lo, before[3] if before else None
    for n, s, e in windows + [(None, hi, hi)]:
        if s > pos:
            between = b2.between(left, src[n] if n else after[3] if after else None)
            for a, b in split(m, at, pos, s, empties):
                rows.append(["unnamed", None, None, a, b, [f"pokebw2 has between its neighbors: {', '.join(between)}"]
                             if between else [], None])
        if n:
            own = before if first[n] else after if last[n] else None
            if own and not own[4]:
                notes[n].append(f"{own[2]}'s own code: the queue row's range misses it")
            if src[n]:
                count, b, count2, b2_size = fit(n, s, e)
                if abs(count - count2) > max(1, count2 // 10) or abs(b - b2_size) > b2_size * 0.15:
                    notes[n].append(f"{count} functions, {b:#x} bytes; pokebw2's {count2}, {b2_size:#x}")
            if any(at.get(i, (0, 0))[1] for i in (s, e)):
                notes[n].append("an edge breaks the data's order")
            rows.append(["widen" if own and own[4] else "name", n, src[n], s, e, notes[n], own])
            pos, left = max(pos, e), src[n]
    for k, row in enumerate(rows):
        if row[1] and not row[2] and any(0 <= j < len(rows) and not rows[j][1] for j in (k - 1, k + 1)):
            row[5].append("no pokebw2 file: the free code next to it may be its")
        if row[0] == "name" and row[5]:
            row[0] = "check"
    return rows


def gap_names(m, gap):
    """{name: {index}} of the embedded file names in the gap, each with the "" (one per file) that its functions use
    between their own."""
    names = {n: {i for i in x if gap[0] <= i < gap[1]} for n, x in m.anchors.items()}
    names = {n: x for n, x in names.items() if x}
    for x in gap_empties(m, gap):
        mine = [n for n, ix in names.items() if min(ix) <= max(x) and min(x) <= max(ix)]
        if len(mine) == 1:
            names[mine[0]] |= x
    return names


def gap_empties(m, gap):
    """The "" objects used in the gap: [{index of each function that uses it}]."""
    groups = [{i for i in x if gap[0] <= i < gap[1]} for x in m.empties.values()]
    return [x for x in groups if x]


def split(m, at, s, e, groups):
    """[(start, end)] of the code funcs[s:e], cut between each two of its "" objects where it costs least."""
    groups = sorted((min(x), max(x)) for x in groups if s <= min(x) and max(x) < e)
    cuts = [s] + [min(range(a + 1, b + 1), key=lambda i: m.cost(at, i))
                  for (_, a), (b, _) in zip(groups, groups[1:]) if a < b] + [e]
    return list(zip(cuts, cuts[1:]))


def unqueued(r, files_tsv, spans, mod, start, end):
    """Why the queue leaves out the twin_files.py rows whose twins fall in [start, end)."""
    files = {f for _, (_, fs) in r.delinks.items() for f in fs}
    local = {line.split("\t")[1] for line in list(open(f"{ROOT}/docs/local-queue.tsv"))[1:]}
    out = []
    for line in list(open(files_tsv))[1:]:
        c = line.rstrip("\n").split("\t")
        if c[0] == "none" or c[4] != mod or not c[5] or not (int(c[5], 16) < end and start < int(c[6], 16)):
            continue
        lo, hi = int(c[5], 16), int(c[6], 16)
        why = "it is in a delinks.txt" if black_path(c[1], mod) in files else "the local worker's" if c[1] in local \
            else f"its twins span under {TINY:#x} bytes" if hi - lo < TINY else \
            "overlaps " + ", ".join(sp[2] for sp in spans if sp[0] < hi and lo < sp[1] and sp[2] != c[1])
        out.append(f"{c[1]}'s twin_files.py row is not queued: {why}")
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module", nargs="?", help="for --profile: main or ovNNN")
    ap.add_argument("--profile", nargs=2, metavar=("START", "END"))
    ap.add_argument("--check", action="store_true", help="find the complete files with an embedded name")
    ap.add_argument("--bw2", default=os.path.expanduser("~/ref/pokebw2"), help="pokebw2 checkout (read only)")
    ap.add_argument("--config", default=f"{ROOT}/config/b_us/arm9")
    ap.add_argument("--twins", default=f"{ROOT}/build/twins_b_us.tsv", help="written by tools/twins.py")
    ap.add_argument("--files", default=f"{ROOT}/build/twin_files.tsv", help="written by tools/twin_files.py")
    ap.add_argument("--tsv", default=f"{ROOT}/build/name_files.tsv")
    args = ap.parse_args()
    r = Ranges(args.bw2, args.config, args.twins)

    if args.profile:
        m = Module(r, args.module)
        lo, hi = m.index(int(args.profile[0], 16)), m.index(int(args.profile[1], 16) - 1) + 1
        at = m.boundaries(lo, hi)
        anchor = {i: n for n, ix in m.anchors.items() for i in ix}
        anchor.update({i: n for n, ix in m.empties.items() for i in ix})
        print("address  size   function                                 calls order anchor")
        for i in range(lo, hi):
            a, size, fn = m.funcs[i]
            calls, order = at.get(i, ("", ""))
            print(f"{a:08x} {size:#6x} {fn[:40]:40s} {calls:5} {order:5} {anchor.get(i, '')}")
        return

    b2 = B2(r, args.files)
    spans = known_spans(r, b2, args.files)

    if args.check:
        same = total = 0
        for mod in sorted(r.delinks):
            m = Module(r, mod)
            text = open(f"{args.config}/{module_dir(mod)}/delinks.txt").read()
            for s, e, f, *_ in spans[mod]:
                name = os.path.basename(f)
                if f"\n{f}:\n    complete\n" not in text or name not in m.anchors:
                    continue
                gap = next((g for g in m.gaps([sp for sp in spans[mod] if sp[2] != f]) if g[0] <= m.index(s) < g[1]),
                           None)
                if not gap:
                    print(f"{mod} {f}: another file or queue row covers it")
                    continue
                got = [(m.funcs[row[3]][0], m.end(row[4])) for row in propose(r, b2, m, gap, gap_names(m, gap))
                       if row[1] == name]
                total += 1
                same += got == [(s, e)]
                if got != [(s, e)]:
                    print(f"{mod} {f}: {s:#x}-{e:#x}, found " + ", ".join(f"{a:#x}-{b:#x}" for a, b in got))
        print(f"{same}/{total} complete files with an embedded name found exactly")
        # each run of adjacent complete files, hidden: are the rows' edges where its files meet?
        right = wrong = missed = 0
        for mod in sorted(r.delinks):
            m = Module(r, mod)
            text = open(f"{args.config}/{module_dir(mod)}/delinks.txt").read()
            done = sorted((s, e, f) for s, e, f, *_ in spans[mod] if f"\n{f}:\n    complete\n" in text)
            runs = []
            for s, e, f in done:
                if runs and runs[-1][-1][1] == s:
                    runs[-1].append((s, e, f))
                else:
                    runs.append([(s, e, f)])
            for run in (x for x in runs if len(x) > 1):
                hidden = {f for _, _, f in run}
                lo, hi = m.index(run[0][0]), m.index(run[-1][1] - 1) + 1
                gap = next((g for g in m.gaps([sp for sp in spans[mod] if sp[2] not in hidden]) if g[:2] == (lo, hi)),
                           None)
                if not gap:
                    continue
                names = gap_names(m, gap)
                rows = propose(r, b2, m, gap, names) if names else [
                    [None, None, None, s, e] for s, e in split(m, m.boundaries(lo, hi), lo, hi, gap_empties(m, gap))]
                cuts = {row[3] for row in rows} | {row[4] for row in rows}
                cuts -= {lo, hi}
                true = {m.index(s) for s, _, _ in run[1:]}
                right, wrong, missed = right + len(cuts & true), wrong + len(cuts - true), missed + len(true - cuts)
        print(f"edges in hidden runs of complete files: {right} right, {wrong} wrong, {missed} missed")
        return

    out, counts = [], Counter()
    for mod in sorted(r.delinks):
        m = Module(r, mod)
        for gap in m.gaps(spans[mod]):
            names = gap_names(m, gap)
            rows = propose(r, b2, m, gap, names) if names else [
                ["unnamed", None, None, s, e, [], None]
                for s, e in split(m, m.boundaries(gap[0], gap[1]), gap[0], gap[1], gap_empties(m, gap))]
            for cls, n, src, s, e, notes, own in rows:
                start, end = m.funcs[s][0], m.end(e)
                if not n:
                    notes += unqueued(r, args.files, spans[mod], mod, start, end)
                if cls == "widen":  # the whole file
                    start, end = min(start, own[0]), max(end, own[1])
                ranges, more = r.data(mod, start, end, own=own and own[2], b2=r.b2_files.get(src))
                ranges = fmt({".text": (start, end), **ranges})
                out.append([cls, src or "", "", str(len(r.b2_order(src))) if src else "", mod, f"{start:#010x}",
                            f"{end:#010x}", str(e - s), "", "", "", own[2] if cls == "widen" else "", n or "", ranges,
                            "; ".join(notes + more)])
                counts[cls] += 1
                if n:
                    dest = own[2] if cls == "widen" else black_path(src, mod) if src else \
                        f"src/{'system' if mod == 'main' else mod}/{n}"
                    print(f"{cls:6s} " + (f"python3 tools/port_file.py {src} {mod} {ranges}" if src and cls != "widen"
                                          else f"python3 tools/add_source_file.py {dest} {module_dir(mod)} {ranges} "
                                          "--incomplete") + "".join(f"\n    {x}" for x in notes))
        for s, e, what, *_ in spans[mod]:
            for n, ix in m.anchors.items():
                if n != os.path.basename(what) and any(s <= m.funcs[i][0] < e for i in ix):
                    print(f"conflict: {mod} {what} {s:#x}-{e:#x} holds {n}'s anchor")
    with open(args.tsv, "w") as f:
        f.write("\t".join(COLUMNS) + "\n" + "".join("\t".join(c) + "\n" for c in out))
    print(", ".join(f"{n} {k}" for k, n in counts.items()) + f": {args.tsv}")


if __name__ == "__main__":
    main()
