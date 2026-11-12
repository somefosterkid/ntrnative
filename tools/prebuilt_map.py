#!/usr/bin/env python3
"""Map the reference libraries' prebuilt objects (every .a under ~/ref) onto Black's code, and say where this tree's
files differ from them: the real files, their names, their compilers and their functions' names.

A library that ships only as a prebuilt archive (NitroDWC's libdwc*.a, NitroWiFi's libssl.a, the SDK's libcht.a,
libe3s.a and libdgt.a, the FP library FP_fullI_v4t_LE.a, HeartGold's libdsprot.a) still tells a lot: each member is
one file of the original build, its functions are in their link order with their real names, its .comment section
names its compiler, and its DWARF line table names the C file. Black links the same objects, and where Black's build
used the same compiler, its code is the prebuilt's byte for byte.

1. Hash: each function of each member, and each of Black's (build/b_us/delinks: the files' and the gaps' delinked
   objects), by its mode and code with the relocated fields and every BL/BLX's word zeroed (relocations are
   wildcards, and a call one side resolved locally is one too). Functions of TINY bytes or more anchor.
2. Place: a member's anchors in a module are the longest run of its functions whose twins follow its order there
   (one module per placement; a member can be placed in two). Members are placed by their anchored bytes, the most
   first; a Black function one placement took is no other's. Between two anchors, a member's functions pair with
   Black's in order when both sides count the same ("order"), else by alignment on the code's similarity ("aligned",
   SIM or more), and the same at the run's two ends, up to the next placement's function ("edge"). An anchor is near
   the one before it (Black's code between them is at most 1.5 times the member's), and a twin that other functions
   of the module share weighs less (libdwcutil's scene files have the same template functions). Then the members
   between two placed ones of an archive, where the module has them in the archive's order, pair with the code
   between ("archive", ORDER_SIM or more, or by order when the counts agree). Each pair is checked by what the two
   functions call and load by name (Func.check: the share of the member's that Black's reaches too).
3. Report, per library and placement: the tree's files over the placement's range, and what differs:
   - name:     the tree's file has another name than the member's C file (or another library's directory);
   - glued:    one tree file holds two or more members (or a member and code of no member);
   - split:    one member over two or more tree files;
   - compiler: the tree file's compiler is not the one of the member's .comment, while most of its functions are
               the prebuilt's byte for byte (when few are, Black's build compiled the file again: tgc's dwcutil
               finding, its ARM files under dsi/1.1p1);
   - func_:    Black's placeholder names where the member has real ones (and other names that differ);
   - edge:     the tree's file has functions past the member's pairs that no member took, no more than the member's
               unpaired ones (most likely its own: a recompile's code that doesn't pair).
   A compiler finding is a hint: check it with tools/compiler_probe.py --compilers <tree's>,<member's>. The
   prebuilts are older builds than Black's (NitroDWC's against Black's TWL-DWC 5.3), so a member that pairs by order
   only can differ from Black's file in its functions, and a compiler that matches the prebuilt can fail on Black's.

    python3 tools/prebuilt_map.py                      # the report (build/prebuilt_map.tsv: one row a function)
    python3 tools/prebuilt_map.py --library libssl     # one archive (a substring of its path)
    python3 tools/prebuilt_map.py --member ssl.o -v    # one member, function by function
    python3 tools/prebuilt_map.py --renames            # the func_ renames it would make (exact and order pairs)
    python3 tools/prebuilt_map.py --renames --apply    # make them (tools/rename.py's files); then ninja
    python3 tools/prebuilt_map.py --protos             # the reference headers' prototypes against the tree's
    python3 tools/prebuilt_map.py --kinds glued,split  # only the placements with these findings

Needs ninja to have run (the delinked objects). Reads objects only: no compile. Black's side is cached in
build/cache (tools/bwcache.py) while its objects don't change.
"""
import argparse
import difflib
import glob
import hashlib
import os
import re
import struct
import subprocess
import sys
from collections import Counter, defaultdict

import bwcache
from file_ranges import ROOT
from port_file import parse_object, read_object, uleb
from twin_files import read_delinks
from twins import SHORT_RELOCS

REF = os.path.expanduser("~/ref")
CONFIG = f"{ROOT}/config/b_us/arm9"
DELINKS = f"{ROOT}/build/b_us/delinks"
TSV = f"{ROOT}/build/prebuilt_map.tsv"
TINY = 0x10  # bytes: a getter or a stub twins anything
SIM = 0.6  # the least similarity of an aligned pair
ORDER_SIM = 0.45  # of a pair between two placed members of an archive, in its order
# ~/ref's checkouts that hold no library of Black's, or a copy of one (pokebw2's build trees)
SKIP = re.compile(r"^(pokebw2.*|decomp-permuter|m2c)$")
# A prefered checkout first when two archives are the same bytes (libntr's copies of the SDK's)
PREFER = ("ntrtwl-", "libntr", "pokeheartgold")
# MWCC's version in a .comment (Metrowerks C/C++ for ARM <version>) to tools/mwccarm's compiler (mwccarm -version)
COMPILERS = {"2.0.0.56": "1.2/b56", "2.0.0.72": "1.2/base", "2.0.0.79": "1.2/sp2", "2.0.0.82": "1.2/sp2p3",
             "2.0.0.84": "1.2/sp3", "2.0.0.87": "1.2/sp4", "3.0.0.114": "2.0/base", "3.0.0.123": "2.0/sp1",
             "3.0.0.126": "2.0/sp1p2", "3.0.0.131": "2.0/sp1p5", "3.0.0.133": "2.0/sp1p6", "3.0.0.134": "2.0/sp1p7",
             "3.0.0.136": "2.0/sp2", "3.0.0.137": "2.0/sp2p2", "3.0.0.138": "2.0/sp2p3", "3.0.0.139": "2.0/sp2p4",
             "4.0.0.1018": "dsi/1.1", "4.0.0.1024": "dsi/1.1p1", "4.0.0.1026": "dsi/1.2", "4.0.0.1027": "dsi/1.2p1",
             "4.0.0.1028": "dsi/1.2p2", "4.0.0.1034": "dsi/1.3", "4.0.0.1036": "dsi/1.3p1",
             "4.0.0.1051": "dsi/1.6sp2"}
# An archive's directory in this tree: its library's
LIBDIRS = {"libdwcutil": "lib/dwcutil/", "libdwcac": "lib/dwc/src/ac/", "libdwcauth": "lib/dwc/src/auth/",
           "libdwcnd": "lib/dwc/src/nd/", "libdwcnhttp": "lib/dwc/src/nhttp/", "libdwcranking": "lib/dwc/src/ranking/",
           "libdwcgspinger": "lib/dwc/src/gs/", "libdwcgspt": "lib/dwc/src/gs/", "libssl": "lib/nitrowifi/src/ssl/",
           "libcht": "lib/nitro/src/cht/", "libdgt": "lib/nitro/src/dgt/", "libe3s": "lib/nitro/src/e3s/",
           "FP_fullI_v4t_LE": "lib/msl/", "libdsprot": "lib/dsprot/"}
NL = bytes([10])
ELF = bytes([0x7F]) + b"ELF"


# --- the archives ---------------------------------------------------------------------------------------------------

def archives():
    """[(path, bytes)] of every ARM archive (and loose relocatable ARM object) under ~/ref, one of identical ones."""
    found = []
    for top in sorted(os.listdir(REF)):
        if SKIP.match(top) or not os.path.isdir(f"{REF}/{top}"):
            continue
        for dirpath, dirs, files in os.walk(f"{REF}/{top}"):
            dirs[:] = [d for d in dirs if d not in (".git", "build", "node_modules")]
            found += [f"{dirpath}/{f}" for f in files if f.endswith((".a", ".o"))]
    seen, out = {}, []
    for path in sorted(found, key=lambda p: (not p[len(REF) + 1:].startswith(PREFER), p)):
        data = open(path, "rb").read()
        if not (data[:8] == b"!<arch>" + NL or is_arm_object(data)):
            continue
        digest = hashlib.sha1(data).digest()
        if digest not in seen:
            seen[digest] = path
            out.append((path, data))
    return out


def is_arm_object(data):
    return data[:4] == ELF and len(data) > 20 and struct.unpack_from("<HH", data, 16) == (1, 40)


def members(data):
    """[(name, bytes)] of an ar archive's ELF members, in the archive's order (a loose object is its own member)."""
    if is_arm_object(data):
        return [("", data)]
    i, longnames, out = 8, b"", []
    while i + 60 <= len(data):
        name, size = data[i:i + 16].decode(errors="replace").rstrip(), int(data[i + 48:i + 58])
        body = data[i + 60:i + 60 + size]
        i += 60 + size + (size & 1)
        if name == "//":
            longnames = body
            continue
        if name in ("/", "/SYM64/") or name.startswith("__.SYMDEF"):
            continue
        if name.startswith("#1/"):  # BSD: the name leads the body
            n = int(name[3:])
            name, body = body[:n].rstrip(bytes(1)).decode(errors="replace"), body[n:]
        elif re.fullmatch(r"/[0-9]+", name):  # GNU: an offset in the long names
            o = int(name[1:])
            name = longnames[o:longnames.index(NL, o)].decode(errors="replace")
        name = name.rstrip("/")
        if is_arm_object(body):
            out.append((name, body))
    return out


# --- the code -------------------------------------------------------------------------------------------------------

def mask(code, mode, refs):
    """The code with the relocated fields (refs: [(offset, width)]) and every BL/BLX's word zeroed."""
    code = bytearray(code)
    for at, width in refs:
        code[at:at + width] = bytes(width)
    if mode == "t":
        k = 0
        while k + 4 <= len(code):
            h1, h2 = struct.unpack_from("<HH", code, k)
            if h1 & 0xF800 == 0xF000 and h2 & 0xE800 == 0xE800:
                code[k:k + 4] = bytes(4)
                k += 4
            else:
                k += 2
    else:
        for k in range(0, len(code) - 3, 4):
            w, = struct.unpack_from("<I", code, k)
            if w & 0x0F000000 == 0x0B000000 and w >> 28 != 15 or w >> 25 == 0x7D:  # BL, BLX <label>
                code[k:k + 4] = bytes(4)
    return bytes(code)


def object_functions(obj):
    """[(name, section, value, size, mode, masked code, global, names)] of a parsed object's functions, in its order
    (names: of the symbols its relocations reach, but local labels, sections and literals)."""
    d, secs, syms, relocs = obj
    modes = {(s[1], s[2]): s[0][1] for s in syms if s[0] in ("$a", "$t")}
    out = []
    for name, ndx, value, size, global_, is_func in syms:
        if not is_func or not size or name.startswith("$") or ndx >= len(secs) or secs[ndx][1] != 1:
            continue
        off = secs[ndx][2]
        refs = [(at - value, SHORT_RELOCS.get(kind, 4)) for at, kind, _, _ in relocs.get(ndx, [])
                if value <= at < value + size]
        mode = modes.get((ndx, value), "a")
        names = frozenset(syms[sym][0] for at, _, sym, _ in relocs.get(ndx, []) if value <= at < value + size
                          and syms[sym][0] and syms[sym][0][0] not in ".$@" and syms[sym][0] != name)
        out.append((name, ndx, value, size, mode, mask(d[off + value:off + value + size], mode, refs), global_,
                    names))
    return sorted(out, key=lambda f: (f[1], f[2]))


def sleb(b, i):
    v = shift = 0
    while True:
        byte = b[i]
        v, shift, i = v | (byte & 0x7F) << shift, shift + 7, i + 1
        if byte < 0x80:
            return (v - (1 << shift) if byte & 0x40 else v), i


def cstr(d, q):
    """(the string at q, the offset past its NUL)."""
    e = d.index(bytes(1), q)
    return d[q:e].decode(errors="replace"), e + 1


def line_rows(obj, lines=False):
    """{section: [(offset, file)]}: the DWARF 2 line table's rows, the C file of each address of the object's code
    (lines: [(offset, file, line)], with the source line too: tools/prebuilt_diff.py's statement order)."""
    d, secs, syms, relocs = obj
    rows = defaultdict(list)
    for idx, (name, _, off, size, _) in enumerate(secs):
        if name != ".debug_line" or not size:
            continue
        rel = {o: (sym, add) for o, _, sym, add in relocs.get(idx, [])}
        p = off
        while p + 15 <= off + size:
            unit, = struct.unpack_from("<I", d, p)
            end = p + 4 + unit
            hlen, = struct.unpack_from("<I", d, p + 6)
            min_inst, _, line_base, line_range, opbase = struct.unpack_from("<BBbBB", d, p + 10)
            lens, q = d[p + 15:p + 14 + opbase], p + 14 + opbase
            while d[q]:  # the include directories
                _, q = cstr(d, q)
            q, files = q + 1, [None]
            while d[q]:
                fname, q = cstr(d, q)
                files.append(fname)
                for _ in range(3):
                    _, q = uleb(d, q)
            q, sec, addr, f, line = p + 10 + hlen, None, 0, 1, 1

            def emit():
                if sec is not None:
                    fl = files[f] if f < len(files) else None
                    rows[sec].append((addr, fl, line) if lines else (addr, fl))
            while q < end:
                op, q = d[q], q + 1
                if op == 0:
                    n, q = uleb(d, q)
                    if d[q] == 1:
                        f, line = 1, 1
                    elif d[q] == 2:
                        raw, = struct.unpack_from("<I", d, q + 1)
                        sym, add = rel.get(q + 1 - off, (None, 0))
                        sec, addr = (syms[sym][1], syms[sym][2] + add + raw) if sym is not None else (None, 0)
                    elif d[q] == 3:
                        files.append(cstr(d, q + 1)[0])
                    q += n
                elif op >= opbase:
                    addr += (op - opbase) // line_range * min_inst
                    line += line_base + (op - opbase) % line_range
                    emit()
                elif op == 1:
                    emit()
                elif op == 2:
                    v, q = uleb(d, q)
                    addr += v * min_inst
                elif op == 3:
                    v, q = sleb(d, q)
                    line += v
                elif op == 4:
                    f, q = uleb(d, q)
                elif op == 5:
                    _, q = uleb(d, q)
                elif op == 8:
                    addr += (255 - opbase) // line_range * min_inst
                elif op == 9:
                    addr += struct.unpack_from("<H", d, q)[0]
                    q += 2
                elif op not in (6, 7):
                    for _ in range(lens[op - 1]):
                        _, q = uleb(d, q)
            p = end
    return rows


def comment(obj):
    """The compiler's version in the object's .comment (3.0.0.137), or None."""
    d, secs, _, _ = obj
    for name, _, off, size, _ in secs:
        if name == ".comment":
            m = re.search(r"for ARM ([0-9.]+)", d[off:off + size].decode(errors="replace"))
            if m:
                return m[1]
    return None


class Func:
    """A function of a member or of Black. Black's have a module, address, tree file and match state; a member's,
    once placed, the Black function it pairs with (pair) and how (how: exact, order, aligned, edge)."""
    __slots__ = ("name", "size", "mode", "code", "global_", "file", "mod", "addr", "src", "done", "member", "pair",
                 "how", "sim", "index", "refs", "check")

    def __init__(self, name, size, mode, code, global_=True, file=None, mod=None, addr=None, src=None, done=False):
        self.name, self.size, self.mode, self.code, self.global_, self.file = name, size, mode, code, global_, file
        self.mod, self.addr, self.src, self.done = mod, addr, src, done
        self.member = self.pair = self.how = self.sim = self.index = self.check = None
        self.refs = frozenset()

    def key(self):
        return self.mode, self.code


class Member:
    """One prebuilt object: its archive, name, C file, compiler and functions."""

    def __init__(self, archive, name, data):
        obj = parse_object(data)
        self.archive, self.name, self.version = archive, name, comment(obj)
        self.compiler = COMPILERS.get(self.version or "")
        self.lib = os.path.splitext(os.path.basename(archive))[0]
        rows = line_rows(obj)
        self.funcs = []
        for fname, sec, value, size, mode, code, global_, names in object_functions(obj):
            at = [fl for a, fl in rows.get(sec, []) if value <= a < value + size and fl]
            src = Counter(fl for fl in at if not fl.endswith(".h")).most_common(1) or Counter(at).most_common(1)
            func = Func(fname, size, mode, code, global_, src[0][0] if src else None)
            func.refs = names
            func.member = self
            self.funcs.append(func)
        # an assembly member (the FP library's) has no function sizes: its .text sections are blocks, searched for
        # in Black's code, and its global symbols name the functions they start
        self.blocks = []
        if not self.funcs:
            d, secs, syms, relocs = obj
            modes = {(sy[1], sy[2]): sy[0][1] for sy in syms if sy[0] in ("$a", "$t")}
            for ndx, (sname, kind, off, size, _) in enumerate(secs):
                if kind != 1 or not size or not sname.startswith(".text"):
                    continue
                refs = [(at, SHORT_RELOCS.get(k, 4)) for at, k, _, _ in relocs.get(ndx, [])]
                mode = modes.get((ndx, 0), "a")
                names = sorted((sy[2], sy[0]) for sy in syms if sy[1] == ndx and sy[4] and not sy[0].startswith("$"))
                self.blocks.append((mask(d[off:off + size], mode, refs), mode, names))
        files = Counter(base(f.file) for f in self.funcs if f.file and not f.file.endswith(".h"))
        # the C file: the member's name (a member is one compile, which can include other C files: aoss.o holds
        # aoss_ds.c, aossrc4.c and more), or for a name ar cut at 15 characters, the line table's file it starts
        cut = name[:-2] if name.endswith(".o") else name.rstrip(".")
        whole = [f for f in files if stem(f).lower().startswith(cut.lower())]
        same = [f for f in files if stem(f).lower() == cut.lower()]
        self.file = (same[0] if same else cut + ".c" if name.endswith(".o") else whole[0] if whole else
                     files.most_common(1)[0][0] if files else cut + ".c")
        self.files = files  # a prelinked member (libdwcranking's) holds several
        self.offsets, at = [], 0
        for f in self.funcs:
            self.offsets.append(at)
            at += -(-f.size // 4) * 4

    def __repr__(self):
        return f"{self.lib}:{self.name}"


def base(path):
    return re.split(r"[/:]", path.replace(chr(92), "/"))[-1]


def prebuilt(only=None):
    """[Member] of every archive (whose path under ~/ref has `only` in it)."""
    out = []
    for path, data in archives():
        rel = path[len(REF) + 1:]
        if only and only not in rel:
            continue
        out += [Member(rel, name, body) for name, body in members(data)]
    return out


# --- Black ----------------------------------------------------------------------------------------------------------

def delinked_functions(path):
    """object_functions of a delinked object, cached while it doesn't change."""
    token = (bwcache.stamp(path), bwcache.code_hash(__file__, parse_object.__code__.co_filename))
    out = bwcache.get("prebuilt_map", path, token)
    if out is None:
        out = [(n, s, m, c, r) for n, _, _, s, m, c, _, r in object_functions(read_object(path))]
        bwcache.put("prebuilt_map", path, token, out)
    return out


def black():
    """{module: [Func]} of Black's functions in address order, with their tree file and whether they match."""
    import clones
    addr = clones.symbol_addrs()
    matched = {(r[0], r[1]): r[5] for r in clones.black()}
    files = {os.path.splitext(src)[0]: (src, mod, done) for mod, src, done, _, _ in read_delinks(CONFIG)}
    out, seen = defaultdict(list), set()
    for path in sorted(glob.glob(f"{DELINKS}/**/*.o", recursive=True), key=lambda p: ("_dsd_gap@" in p, p)):
        rel = path[len(DELINKS) + 1:-2]
        gap = re.match(r"_dsd_gap@(.*)_[0-9]+$", rel)
        if gap:
            src, mod, done = None, gap[1], False
        elif rel in files:
            src, mod, done = files[rel]
        else:
            continue
        for name, size, mode, code, refs in delinked_functions(path):
            if (mod, name) in seen or addr.get((mod, name)) is None:
                continue
            seen.add((mod, name))
            f = Func(name, size, mode, code, mod=mod, addr=addr[(mod, name)], src=src,
                     done=bool(matched.get((mod, name))))
            f.refs = refs
            out[mod].append(f)
    for funcs in out.values():
        funcs.sort(key=lambda f: f.addr)
    return out


# --- placing --------------------------------------------------------------------------------------------------------

def similarity(a, b):
    """How much of two functions' masked code is the same (0 to 1): their instructions' longest common runs."""
    if a.mode != b.mode or not a.size or not b.size:
        return 0.0
    w = 2 if a.mode == "t" else 4
    x = [a.code[k:k + w] for k in range(0, len(a.code), w)]
    y = [b.code[k:k + w] for k in range(0, len(b.code), w)]
    m = difflib.SequenceMatcher(None, x, y, autojunk=False)
    if m.real_quick_ratio() < SIM or m.quick_ratio() < SIM:
        return m.quick_ratio() * 0.9
    return m.ratio()


def chain(pairs, weight, near=lambda a, b: True):
    """The longest (by weight) run of pairs (i, j) increasing in both, each near (near(pair, next pair)) the one
    before it."""
    pairs = sorted(pairs)
    best = []  # best[k]: (total weight, chain) ending at pairs[k]
    for k, (i, j) in enumerate(pairs):
        top = (0, None)
        for q in range(k):
            pi, pj = pairs[q]
            if pi < i and pj < j and best[q][0] > top[0] and near(pairs[q], (i, j)):
                top = (best[q][0], q)
        best.append((top[0] + weight(i, j), top[1]))
    if not best:
        return []
    k = max(range(len(best)), key=lambda q: best[q][0])
    out = []
    while k is not None:
        out.append(pairs[k])
        k = best[k][1]
    return out[::-1]


def align(ps, bs, least=SIM):
    """[(p, b, similarity)]: the pairs of an order-keeping alignment of two lists of functions, `least` or more
    each."""
    if not ps or not bs:
        return []
    sims = [[similarity(p, b) for b in bs] for p in ps]
    n, m = len(ps), len(bs)
    score = [[0.0] * (m + 1) for _ in range(n + 1)]
    for i in range(n - 1, -1, -1):
        for j in range(m - 1, -1, -1):
            take = sims[i][j] + score[i + 1][j + 1] if sims[i][j] >= least else -1
            score[i][j] = max(take, score[i + 1][j], score[i][j + 1])
    out, i, j = [], 0, 0
    while i < n and j < m:
        if sims[i][j] >= least and score[i][j] == sims[i][j] + score[i + 1][j + 1]:
            out.append((ps[i], bs[j], sims[i][j]))
            i, j = i + 1, j + 1
        elif score[i][j] == score[i + 1][j]:
            i += 1
        else:
            j += 1
    return out


class Placement:
    """A member placed in one module: its functions' pairs with Black's."""

    def __init__(self, member, mod, anchors):
        self.member, self.mod, self.anchors = member, mod, anchors  # [(member index, Black index)]
        self.pairs = {}  # member index: (Black Func, how, similarity)

    def black(self):
        return sorted((b for b, _, _ in self.pairs.values()), key=lambda f: f.addr)

    def span(self):
        fs = self.black()
        return fs[0].addr, fs[-1].addr + fs[-1].size

    def __repr__(self):
        return f"{self.member}@{self.mod}"


def place(members, blk):
    """[Placement] of the members in Black's code (blk: black()), each Black function in one at most."""
    for funcs in blk.values():
        for j, f in enumerate(funcs):
            f.index = j
    keyed = defaultdict(list)
    for funcs in blk.values():
        for f in funcs:
            if f.size >= TINY:
                keyed[f.key()].append(f)
    claimed = {}  # id(Black Func): Placement
    cands = []
    for m in members:
        per_mod = defaultdict(list)
        for i, f in enumerate(m.funcs):
            if f.size >= TINY:
                for b in keyed.get(f.key(), ()):
                    per_mod[b.mod].append((i, b.index))
        cands += [(m, mod, pairs) for mod, pairs in per_mod.items()]

    def best(m, mod, pairs):
        pairs = [(i, j) for i, j in pairs if id(blk[mod][j]) not in claimed]
        funcs = blk[mod]

        def near(a, b):  # Black's code between two anchors is no more than the member's (a recompile's sizes vary)
            return funcs[b[1]].addr - funcs[a[1]].addr <= (m.offsets[b[0]] - m.offsets[a[0]]) * 3 // 2 + 0x100
        # a twin that other functions of the module have too weighs less: the template functions every scene file
        # of libdwcutil has would otherwise place one scene's member in another's file
        c = chain(pairs, lambda i, j: m.funcs[i].size / len(keyed[m.funcs[i].key()]), near)
        return sum(m.funcs[i].size / len(keyed[m.funcs[i].key()]) for i, _ in c), c

    out, copies = [], {}
    scored = [(best(*c), c) for c in cands]
    while scored:
        scored.sort(key=lambda s: -s[0][0])
        (weight, c), cand = scored.pop(0)
        again = best(*cand)
        if again[0] != weight:  # a placement before took some: rescore
            if again[0]:
                scored.append((again, cand))
            continue
        m, mod, _ = cand
        if weight < 48 or len(c) < 2 and weight < 64:
            continue
        if m in copies and weight < copies[m] / 2:  # a second copy is most of the member again, not a few twins
            continue
        copies[m] = max(copies.get(m, 0), weight)
        scored.append((best(*cand), cand))  # what is left of its twins there can be a second copy
        p = Placement(m, mod, c)
        for i, j in c:
            b = blk[mod][j]
            claimed[id(b)] = p
            p.pairs[i] = (b, "exact", 1.0)
        out.append(p)
    out += place_blocks(members, blk, claimed)
    for p in out:
        if p.anchors:
            fill(p, blk[p.mod], claimed)
    out += place_by_order(members, out, blk, claimed)
    check(out, blk)
    return out, claimed


def check(placements, blk):
    """Each pair's check (Func.check of the member's function): of what the member's function calls and loads by
    name that Black has (a name Black's symbols have, or a member's function placed), the share Black's function
    reaches too (its callees mapped through the pairs). None when there is nothing to compare."""
    names = {f.name for funcs in blk.values() for f in funcs}
    mapped = {}
    for p in placements:
        for i, (b, _, _) in p.pairs.items():
            mapped[(p.mod, b.name)] = p.member.funcs[i].name
    known = names | set(mapped.values())
    for p in placements:
        for i, (b, _, _) in p.pairs.items():
            f = p.member.funcs[i]
            want = {n for n in f.refs if n in known}
            have = {mapped.get((p.mod, n), n) for n in b.refs}
            f.check = len(want & have) / len(want) if want else None


def place_by_order(members, placements, blk, claimed):
    """Placements of the members between two placed ones of their archive, where the module has the archive's
    members in its order (Black's link of a library: libdwcutil's): the unclaimed functions between the two are
    the members' between them in the archive, with the two's own unpaired ends, by order where the counts agree,
    else by alignment (ORDER_SIM or more: the order is the evidence here)."""
    out = []
    index = {id(m): k for k, m in enumerate(members)}
    by = defaultdict(list)
    for p in placements:
        if p.anchors:
            by[(p.member.archive, p.mod)].append(p)
    for (archive, mod), ps in by.items():
        ps.sort(key=lambda p: index[id(p.member)])
        keep = chain([(index[id(p.member)], p.span()[0]) for p in ps], lambda i, j: 1)
        ps = [p for p in ps if (index[id(p.member)], p.span()[0]) in set(keep)]
        placed = {id(p.member) for p in placements}
        funcs = blk[mod]
        for p1, p2 in zip(ps, ps[1:]):
            k1, k2 = index[id(p1.member)], index[id(p2.member)]
            between = [m for m in members[k1 + 1:k2] if id(m) not in placed]
            lo, hi = p1.span()[1], p2.span()[0]
            if lo > hi:
                continue
            last = max(p1.pairs)
            first = min(p2.pairs)
            fs = [(p1, f) for f in p1.member.funcs[last + 1:]]
            fs += [(None, f) for m in between for f in m.funcs]
            fs += [(p2, f) for f in p2.member.funcs[:first]]
            bs = [b for b in funcs if lo <= b.addr < hi and id(b) not in claimed]
            if not fs or not bs:
                continue
            if len(fs) == len(bs):
                pairs = [(f, b, similarity(f, b)) for (_, f), b in zip(fs, bs)]
            else:
                pairs = align([f for _, f in fs], bs, ORDER_SIM)
            owner = {id(f): q for q, f in fs}
            new = {}
            for f, b, sim in pairs:
                q = owner[id(f)] or new.get(id(f.member))
                if q is None:
                    q = new[id(f.member)] = Placement(f.member, mod, [])
                    out.append(q)
                q.pairs[f.member.funcs.index(f)] = (b, "exact" if f.key() == b.key() else "archive", sim)
                claimed[id(b)] = q
    return out


def fill(p, funcs, claimed):
    """Pairs the member's functions between and around its anchors with Black's: by order where the counts agree,
    else by alignment."""
    mfuncs, a = p.member.funcs, p.anchors

    def take(pairs, how):
        for f, b, sim in pairs:
            p.pairs[mfuncs.index(f)] = (b, how if f.key() != b.key() else "exact", sim)
            claimed[id(b)] = p

    for (i1, j1), (i2, j2) in zip(a, a[1:]):
        ps, bs = mfuncs[i1 + 1:i2], funcs[j1 + 1:j2]
        if any(id(b) in claimed for b in bs):  # another member's code inside: pair only what aligns
            take(align(ps, [b for b in bs if id(b) not in claimed]), "aligned")
        elif len(ps) == len(bs):
            take([(f, b, similarity(f, b)) for f, b in zip(ps, bs)], "order")
        else:
            take(align(ps, bs), "aligned")
    # the ends: the nearest functions first, up to another placement's
    (i0, j0), (i9, j9) = a[0], a[-1]
    before = []
    for j in range(j0 - 1, max(-1, j0 - 1 - i0 - 4), -1):
        if id(funcs[j]) in claimed:
            break
        before.append(funcs[j])
    take(align(mfuncs[:i0][::-1], before), "edge")
    after = []
    for j in range(j9 + 1, min(len(funcs), j9 + 1 + len(mfuncs) - i9 + 4)):
        if id(funcs[j]) in claimed:
            break
        after.append(funcs[j])
    take(align(mfuncs[i9 + 1:], after), "edge")


def place_blocks(members, blk, claimed):
    """Placements of the assembly members: each block found in a module's code (zero bytes on either side are
    wildcards: the relocated fields), its global symbols paired with the Black functions at their offsets."""
    images = {}
    for mod, funcs in blk.items():
        lo = funcs[0].addr
        img = bytearray(funcs[-1].addr + funcs[-1].size - lo)
        for f in funcs:
            img[f.addr - lo:f.addr - lo + f.size] = f.code
        images[mod] = (lo, bytes(img), {f.addr: f for f in funcs})
    out = {}
    for m in members:
        for code, mode, names in m.blocks:
            key = next((code[k:k + 16] for k in range(0, len(code) - 15, 4) if all(code[k:k + 16])), None)
            if key is None or len(code) < 32:
                continue
            k0 = code.index(key)
            for mod, (lo, img, at) in images.items():
                hit = img.find(key)
                while hit >= 0:
                    start = hit - k0
                    if start >= 0 and (lo + start) % (2 if mode == "t" else 4) == 0 and all(
                            x == y or not x or not y for x, y in zip(code, img[start:start + len(code)])):
                        p = out.get((m, mod)) or Placement(m, mod, [])
                        for off, name in names:
                            b = at.get(lo + start + off)
                            if b is not None and id(b) not in claimed:
                                f = Func(name, b.size, mode, b.code)
                                f.member = m
                                m.funcs.append(f)
                                p.pairs[len(m.funcs) - 1] = (b, "asm", 1.0)
                                claimed[id(b)] = p
                        if p.pairs:
                            out[(m, mod)] = p
                    hit = img.find(key, hit + 1)
    return list(out.values())


# --- the tree -------------------------------------------------------------------------------------------------------

def stem(path):
    return os.path.splitext(os.path.basename(path or ""))[0]


def same_name(tree, real):
    """Whether a tree file's name is the member's C file's: the same stem, or one with the library's prefix (dsd takes
    a file name once: dwcutil_main.c for libdwcutil's main.c)."""
    a, b = stem(tree).lower(), stem(real).lower()
    return a == b or a.endswith("_" + b)


def compiler_of(src):
    sys.path.insert(0, ROOT)
    from configure import compiler_of as of
    from pathlib import Path
    try:
        return of(Path(src))
    except Exception:
        return None, []


def findings(placements, blk, claimed):
    """{Placement: [(kind, text)]}: what differs between the tree and the member (the module docstring's kinds)."""
    by_src = defaultdict(list)
    for funcs in blk.values():
        for f in funcs:
            if f.src:
                by_src[f.src].append(f)
    out = {}
    for p in placements:
        m, notes = p.member, []
        lo, hi = p.span()
        inside = [f for f in blk[p.mod] if lo <= f.addr < hi]
        srcs = Counter(f.src for f in inside)
        tree = srcs.most_common(1)[0][0]
        others = [s for s in srcs if s != tree]
        if None in srcs:
            notes.append(("gap", f"{srcs[None]} of its functions in a gap (no file)"))
        if len([s for s in srcs if s]) > 1:
            notes.append(("split", "over " + ", ".join(f"{s} ({n})" for s, n in srcs.most_common() if s)))
        for src in [s for s in srcs if s]:
            outside = [f for f in by_src[src] if not lo <= f.addr < hi]
            if outside:
                who = Counter(str(claimed[id(f)].member) if id(f) in claimed else "no member" for f in outside)
                # the file's functions no member took are the member's own unpaired ones when it has as many
                if set(who) == {"no member"} and who["no member"] <= len(m.funcs) - len(p.pairs):
                    notes.append(("edge", f"{src} holds {len(outside)} more, no member's: the member has "
                                          f"{len(m.funcs) - len(p.pairs)} unpaired"))
                else:
                    notes.append(("glued", f"{src} holds {len(outside)} more: " +
                                  ", ".join(f"{w} ({n})" for w, n in who.most_common(6))))
        if tree and not same_name(tree, m.file):
            notes.append(("name", f"{tree} is {m.file}"))
        want = LIBDIRS.get(m.lib)
        if tree and want and not tree.startswith(want):
            notes.append(("library", f"{tree} is not under {want}"))
        exact = sum(1 for _, how, _ in p.pairs.values() if how in ("exact", "asm"))
        if tree and m.compiler:
            comp, flags = compiler_of(tree)
            if comp and comp != m.compiler:
                share = exact / len(p.pairs)
                notes.append(("compiler" if share >= 0.8 else "rebuilt",
                              f"{tree} is {comp}, the member {m.compiler} ({m.version}): "
                              f"{exact} of {len(p.pairs)} functions the prebuilt's"))
        named = [(f, b) for i, f in enumerate(m.funcs) if i in p.pairs for b in [p.pairs[i][0]] if b.name != f.name]
        unnamed = [(f, b) for f, b in named if b.name.startswith("func_")]
        if unnamed:
            notes.append(("func_", f"{len(unnamed)} func_ names: " +
                          ", ".join(f"{b.name}={f.name}" for f, b in unnamed[:4]) + (" ..." if len(unnamed) > 4 else "")))
        if len(named) > len(unnamed):
            other = [(f, b) for f, b in named if not b.name.startswith("func_")]
            notes.append(("names", f"{len(other)} other names: " +
                          ", ".join(f"{b.name}={f.name}" for f, b in other[:4]) + (" ..." if len(other) > 4 else "")))
        out[p] = (tree, inside, notes)
    return out


def write_tsv(placements):
    with open(TSV, "w") as fh:
        fh.write("archive\tmember\tfile\tcompiler\tfunction\tmodule\taddress\tsize\tblack_name\thow\tsimilarity"
                 "\ttree_file\tmatches\n")
        for p in placements:
            m = p.member
            for i, (b, how, sim) in sorted(p.pairs.items(), key=lambda kv: kv[1][0].addr):
                fh.write(f"{m.archive}\t{m.name}\t{m.file}\t{m.compiler or m.version or ''}\t{m.funcs[i].name}\t"
                         f"{p.mod}\t{b.addr:#010x}\t{b.size:#x}\t{b.name}\t{how}\t{sim:.2f}\t{b.src or ''}\t"
                         f"{int(b.done)}\n")


def sure(f, how, sim):
    """Whether a pair is sure enough to give Black's function the member's name: the same code, or what it calls and
    loads by name agreeing (Func.check), or, with nothing to compare, a pair by order between two anchors (or a very
    similar one)."""
    if how in ("exact", "asm"):
        return True
    if f.check is not None:
        return f.check >= 0.5
    return how == "order" and sim >= 0.3 or sim >= 0.8


BS, LF = chr(92), chr(10)  # (a backslash in this file is hard to keep through the tools that write it)
# a // or /* */ comment, a string or character literal, or an identifier (group 1)
CODE_WORDS = re.compile("//[^" + LF + "]*|/[*].*?[*]/|" + '"(?:[^"' + BS * 2 + LF + "]|" + BS * 2 + '.)*"|'
                        + "'(?:[^'" + BS * 2 + LF + "]|" + BS * 2 + ".)*'|([A-Za-z_][A-Za-z0-9_]*)", re.S)


def taken_names():
    """The names a new symbol can't take: every symbol of the symbols.txt files, and every identifier of the tree's
    C, headers and assembly outside comments and strings (a comment that names pokebw2's guess is no conflict)."""
    import rename
    words = set()
    for path in rename.FILES:
        if not path.is_file() or path.name == "nonmatching.md":
            continue
        text = path.read_text(errors="surrogateescape")
        if path.suffix == ".txt":
            words.update(line.split()[0] for line in text.splitlines() if path.name == "symbols.txt" and line.strip())
        else:
            words.update(m[1] for m in CODE_WORDS.finditer(text) if m[1])
    return words


def apply_renames(pairs):
    """Renames whole words in tools/rename.py's files (as it does, without its refusal of a name a comment has)."""
    import rename
    word = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
    changed = 0
    for path in rename.FILES:
        if not path.is_file():
            continue
        text = path.read_text(errors="surrogateescape")
        if "func_" not in text:
            continue
        new = word.sub(lambda m: pairs.get(m[0], m[0]), text)
        if new != text:
            path.write_text(new, errors="surrogateescape")
            changed += 1
    return changed


def renames(placements):
    """[(old, new, placement)]: Black's func_ names the members name, for the sure() pairs. A name that is taken
    (taken_names()) or that two functions would take gets the member's file stem (name_<stem>), but for the one
    global function of that name."""
    words = taken_names()
    rows = []
    for p in placements:
        for i, (b, how, sim) in p.pairs.items():
            f = p.member.funcs[i]
            if b.name.startswith("func_") and not f.name.startswith("func_") and sure(f, how, sim):
                rows.append((b, f, p))
    by_name = defaultdict(list)
    for b, f, p in rows:
        by_name[f.name].append((b, f, p))
    out = []
    for name, rs in by_name.items():
        globals_ = [r for r in rs if r[1].global_]
        plain = globals_[0] if len(globals_) == 1 else rs[0] if len(rs) == 1 else None
        for r in rs:
            b, f, p = r
            new = name
            if new in words or r is not plain:
                new = f"{name}_{re.sub(r'[^A-Za-z0-9_]', '_', stem(p.member.file))}"
            if new in words or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", new):
                continue
            words.add(new)
            out.append((b.name, new, p))
    return sorted(out)


def declarations(paths, names):
    """{name: (path, declaration)} of the first prototype or definition of each name in the files."""
    out = {}
    head = re.compile(r"^[A-Za-z_][^;{}()#=]*?[*\s]([A-Za-z_][A-Za-z0-9_]*)\s*[(]([^;{}()]*(?:[(][^;{}()]*[)][^;{}()]*)*)[)]\s*[;{]",
                      re.M)
    for path in paths:
        try:
            text = open(path, errors="replace").read()
        except OSError:
            continue
        for m in head.finditer(text):
            if m[1] in names and m[1] not in out:
                out[m[1]] = (path, " ".join(m[0].rstrip("{;").split()))
    return out


def arity(decl):
    inside = decl[decl.index("(") + 1:decl.rindex(")")].strip()
    if inside in ("", "void"):
        return 0
    depth, n = 0, 1
    for ch in inside:
        depth += ch == "("
        depth -= ch == ")"
        n += ch == "," and depth == 0
    return n


def prototypes(placements):
    """[(name, tree declaration, reference declaration)] of the members' functions Black has by their names, where
    the reference headers (the member's checkout under ~/ref, its include/ and libraries/) declare them."""
    names = {p.member.funcs[i].name for p in placements for i, (b, _, _) in p.pairs.items()
             if b.name == p.member.funcs[i].name}
    roots = {p.member.archive.split("/")[0] for p in placements}
    refs = [f for r in sorted(roots) for f in glob.glob(f"{REF}/{r}/**/*.h", recursive=True)]
    tree = [f for d in ("include", "lib", "src") for f in glob.glob(f"{ROOT}/{d}/**/*.[ch]", recursive=True)]
    theirs, ours = declarations(refs, names), declarations(tree, names)
    return [(n, ours.get(n), theirs[n]) for n in sorted(theirs)]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--library", help="only the archives whose path under ~/ref has this in it")
    ap.add_argument("--member", help="only this member (its name or C file)")
    ap.add_argument("-v", "--verbose", action="store_true", help="each function's pair")
    ap.add_argument("--kinds", help="only placements with these findings (comma-separated: name,glued,split,"
                                    "compiler,rebuilt,func_,names,gap,library,edge)")
    ap.add_argument("--renames", action="store_true", help="list the func_ renames (with --apply: make them)")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--protos", action="store_true", help="the reference headers' prototypes of the functions Black "
                                                          "has by their names, against the tree's")
    args = ap.parse_args()
    members = prebuilt(args.library)
    blk = black()
    placements, claimed = place(members, blk)
    write_tsv(placements)
    if args.member:
        placements = [p for p in placements if args.member in (p.member.name, p.member.file)]
        members = [m for m in members if args.member in (m.name, m.file)]
    if args.renames:
        rs = renames(placements)
        for old, new, p in rs:
            print(f"{old} {new}  # {p.member}")
        print(f"{len(rs)} renames")
        if args.apply and rs:
            print(f"{len(rs)} renamed in {apply_renames({old: new for old, new, _ in rs})} files: run ninja")
        return
    if args.protos:
        for name, ours, theirs in prototypes(placements):
            mark = "" if ours and arity(ours[1]) == arity(theirs[1]) else "  <- arguments differ" if ours else ""
            print(f"{name}{mark}")
            print(f"    ref:  {theirs[1]}  ({os.path.relpath(theirs[0], REF)})")
            print(f"    tree: {ours[1] if ours else '-'}" + (f"  ({os.path.relpath(ours[0], ROOT)})" if ours else ""))
        return
    notes = findings(placements, blk, claimed)
    kinds = set(args.kinds.split(",")) if args.kinds else None
    by_lib = defaultdict(list)
    for m in members:
        by_lib[(m.lib, m.archive)].append(m)
    placed = defaultdict(list)
    for p in placements:
        placed[p.member].append(p)
    for (lib, archive), ms in by_lib.items():
        ps = [p for m in ms for p in placed[m]]
        n = sum(len(p.pairs) for p in ps)
        exact = sum(1 for p in ps for _, how, _ in p.pairs.values() if how in ("exact", "asm"))
        lines = [f"{lib} (~/ref/{archive}): {len(ms)} members, {len(ps)} placements, {n} of "
                 f"{sum(len(m.funcs) for m in ms)} functions paired, {exact} the prebuilt's byte for byte"]
        for m in ms:
            for p in placed[m]:
                tree, inside, ns = notes[p]
                if kinds and not kinds & {k for k, _ in ns}:
                    continue
                lo, hi = p.span()
                ex = sum(1 for _, how, _ in p.pairs.values() if how in ("exact", "asm"))
                lines.append(f"  {m.name or m.file:<22} {m.file:<22} {m.compiler or m.version or '?':<10} {p.mod} "
                             f"{lo:#010x}-{hi:#010x}  {tree}  {len(p.pairs)}/{len(m.funcs)} paired, {ex} exact, "
                             f"{len(inside) - len(p.pairs)} unpaired inside")
                lines += [f"      {kind}: {text}" for kind, text in ns]
                if args.verbose:
                    for i, (b, how, sim) in sorted(p.pairs.items(), key=lambda kv: kv[1][0].addr):
                        f = m.funcs[i]
                        ck = "" if f.check is None else f"calls {f.check:.2f}"
                        lines.append(f"        {b.addr:#010x} {b.size:#6x} {b.name:<36} {f.name:<36} {how:<7} "
                                     f"{sim:.2f} {ck:<10} {'matches' if b.done else ''}")
        unplaced = [m for m in ms if not placed[m]]
        if unplaced and not kinds:
            lines.append(f"  not placed ({len(unplaced)}): " + ", ".join(m.name or m.file for m in unplaced))
        if len(lines) > 1 or not kinds:
            print(chr(10).join(lines))
    print(f"(one row a function: {os.path.relpath(TSV, ROOT)})")


if __name__ == "__main__":
    main()
