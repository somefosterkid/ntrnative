#!/usr/bin/env python3
"""Set a library file of Black up from another DS decompilation's source: functions, .text range, names, C, delinks.

    python3 tools/port_lib.py ntrtwl-NitroSystem/libraries/fnd/src/expheap.c
    python3 tools/port_lib.py ntrtwl-NitroSDK/libraries/os/src/os_thread.c --dry-run
    python3 tools/port_lib.py ntrtwl-NitroSystem/libraries/fnd/src/expheap.c --name func_02060f98=NNS_FndGetGroupIDForMBlockExpHeap

The source is a file under ~/ref of a checkout tools/lib_twins.py knows (SOURCES). The tool:
1. takes Black's functions of main (or of --module's overlay: ov009 for NitroDWC) that build/lib_twins.tsv (or --tsv)
   pairs with the source's (16 bytes or more), keeps the
   pairs that follow the source's order (a twin the TSV names after a sibling with the same code is paired by order
   instead), and pairs the functions between two pairs by order when both sides count the same. It says which source
   functions Black lacks (the linker dead-stripped them: unreferenced functions of a library object, the debug-only
   ones, the inline ones) and when Black's functions don't follow the source's order (then check them by hand);
2. derives the file's .text range in main: its first function to the start of the function after its last. The
   untwinned functions before the first pair and after the last are split with the neighbouring twinned file by
   count (its source parsed the same way): when the counts don't add up it says "CHECK" and leaves them out;
3. finds its data in relocs.txt: the .rodata, .data and .bss symbols its functions load, each to the next symbol;
4. renames Black's func_ placeholders to the source's names everywhere (tools/port_file.py's rename_everywhere, in
   config/**/symbols.txt, src/, lib/, include/). A static whose name another function of Black already has takes
   pokebw2's name for it (AllocFromHeadForFrmHeap) when pokebw2 has one, else <name>_<file stem>. --name forces one;
5. writes lib/<lib>/library.toml from pokebw2's for that library when it is missing, with the compiler, mode (ARM or
   Thumb) and -ipa of the variant that gives the file its twins; when it exists, says whether it agrees;
6. writes lib/<lib>/src/<library>/<file>.c when it is missing. An MIT or CC0 source with the SDK's names (libntr, not
   RAW's) is ported as it is with credit (a project decision of 2026-10-13): its preprocessor's choices
   for a final ROM made (unifdef(): SDK 4, ARM9, no TWL or debug code), only the functions Black has, its includes
   kept. Its headers are copied as they are into lib/<lib>/sdk/ (vendor(): the ones the file includes when compiled
   as tools/lib_twins.py compiled the source, with the checkout's LICENSE beside them) and the file goes in
   library.toml's [sdk] files, which configure.py builds with those headers ahead of the game's and lib_twins.py's
   defines, so it compiles as its twins did. Any other source gets a header and the list of its functions in Black's
   order, to write natural C from the source into (docs/worker.md: the loop), since its license allows no copy;
7. adds the delinks.txt entry (--incomplete, tools/add_source_file.py) and runs tools/brief.py on the file; with
   --finish, tools/finish.py instead: the file committed complete, or with the functions that don't match parked in
   docs/nonmatching.md for a worker, the source credited in the commit message.

    python3 tools/port_lib.py libraries/gx/src/g3x.c --finish     # libntr's, as it is: 16 of 17 functions match
    python3 tools/port_lib.py ntrtwl-NitroDWC/libraries/gs/qr2/qr2.c --module ov009 --finish   # a skeleton in ov009

A path without its checkout takes the first of PREFERRED that has it, and an ntrtwl-NitroSDK path libntr's same file.
--dry-run prints 1-3 and what 4-7 would do, and changes nothing. The "library" of a source is lib_twins.SOURCES's
regex group (fnd, os, fx); lib/<lib> is LIBS' for the checkout (nnsys, nitro, ...), or --lib.
"""
import argparse
import bisect
import glob
import os
import re
import subprocess
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from file_ranges import ROOT, read_delinks  # noqa: E402
from lib_twins import IPA, RAW, REF, SOURCES, TSV, VARIANTS  # noqa: E402
from port_file import rename_everywhere  # noqa: E402

CONFIG = f"{ROOT}/config/b_us/arm9"
BW2 = os.path.expanduser("~/ref/pokebw2")
TINY = 16  # bytes: a smaller twin is the same code by accident (lib_twins.py)
PLACEHOLDER = re.compile(r"^func_[0-9a-fA-F]{8}$")
LICENSED = ("MIT", "CC0-1.0")  # may be copied with credit
# a source path without its checkout (libraries/gx/src/g3x.c): the first checkout that has it, one that may be ported
# first; an unlicensed checkout's file another has at the same path, under a license: that one
PREFERRED = ("libntr", "ntrtwl-NitroSDK", "ntrtwl-NitroSystem", "ntrtwl-NitroWiFi", "ntrtwl-NitroDWC")
SAME_FILES = {"ntrtwl-NitroSDK": "libntr"}
# the checkout's lib/<lib> here (and in pokebw2); None: LIB_OF_LIBRARY decides from the source's library
LIBS = {"ntrtwl-NitroSystem": "nnsys", "ntrtwl-NitroSDK": "nitro", "libntr": "nitro", "ntrtwl-NitroWiFi": "nitrowifi",
        "ntrtwl-NitroDWC": "dwc", "ntrtwl-libvct": "vct", "dsprot-1.27": "dsprot", "pokeheartgold": "dsprot",
        "pmd-sky": "nitro"}
LIB_OF_LIBRARY = {"nns": "nnsys", "libnns": "nnsys", "NitroSDK": "nitro", "nitro": "nitro", "nds": "nitro",
                  "dsprot": "dsprot", "c": "msl", "cpp": "msl", "runtime": "msl", "MSL_C": "msl", "spl": "spl"}
# the preprocessor's view of a final ROM built for the ARM9 with CodeWarrior (lib_twins.py's SDK defines, and libntr's
# LIBNTR ones for SDK 4)
DEFINED = {"SDK_FINALROM", "NNS_FINALROM", "SDK_ARM9", "SDK_CW", "SDK_TS", "SDK_4M", "SDK_CW_FORCE_EXPORT_SUPPORT",
           "SDK_NITRO", "SDK_BUILD_ARM", "SDK_32BIT", "__MWERKS__"}
UNDEFINED = {"SDK_DEBUG", "NNS_DEBUG", "SDK_TWL", "SDK_TWLLTD", "SDK_TWLHYB", "__cplusplus", "SDK_ARM7",
             "SDK_LINK_ARM7", "SDK_RELEASE", "NNS_RELEASE", "SDK_SMALL_BUILD", "SDK_NO_THREAD", "SDK_PORT",
             "SDK_WIN32", "SDK_ADS", "SDK_GCC", "SDK_TEG", "SDK_NO_MESSAGE", "SDK_TWL_ONLY", "SDK_TWLHYB",
             "SDK_CODE_THUMB", "SDK_BB", "SDK_PROFILE_FUNCTIONCOST", "SDK_PROFILE_CALLTRACE", "SDK_TCM_APPLY",
             "SDK_CONFLICT_CHECK", "SDK_ARM7_PROC", "SDK_FROM_TOOL", "SDK_WFS_DEBUG", "_DEBUG"}
NUMBERS = {"SDK_VERSION_MAJOR": 4}  # libntr's choice between its SDK 4 and 5 code: Black's library is SDK 4's
THUMB = set()  # SDK_CODE_THUMB in DEFINED, SDK_CODE_ARM in UNDEFINED, for a library built as Thumb (main sets it)
# --module: Black's module the file is in (main, or an overlay such as ov009, NitroDWC's), its directory under CONFIG,
# and what relocs.txt calls it
MODULE, MODDIR, RELOC_MOD = "main", ".", "main"


def set_module(mod):
    global MODULE, MODDIR, RELOC_MOD
    MODULE = mod
    MODDIR = "." if mod == "main" else f"overlays/{mod}"
    RELOC_MOD = "main" if mod == "main" else f"overlay({int(mod[2:])})"


# --- the source ------------------------------------------------------------------------------------------------------

def known(name):
    if name in ("SDK_CODE_ARM", "SDK_CODE_THUMB"):
        return (name == "SDK_CODE_THUMB") == bool(THUMB)
    return True if name in DEFINED else False if name in UNDEFINED else None


def number(word):
    """A word of a condition as a number: a literal, a NUMBERS name, or None."""
    m = re.fullmatch(r"(0x[0-9a-fA-F]+|\d+)[uUlL]*", word)
    return int(m[1], 0) if m else NUMBERS.get(word)


def evaluate(cond):
    """True or False for a condition over DEFINED and UNDEFINED names, None when it can't tell."""
    cond = cond.strip()
    while cond.startswith("(") and cond.endswith(")") and balanced(cond[1:-1]):
        cond = cond[1:-1].strip()
    for op, fold in (("||", any), ("&&", all)):
        parts = split_top(cond, op)
        if len(parts) > 1:
            values = [evaluate(p) for p in parts]
            return None if None in values else fold(values)
    m = re.fullmatch(r"!\s*(.*)", cond)
    if m:
        v = evaluate(m[1])
        return None if v is None else not v
    m = re.fullmatch(r"defined\s*\(?\s*(\w+)\s*\)?", cond)
    if m:
        return known(m[1])
    m = re.fullmatch(r"(\w+)\s*(==|!=|>=|<=|>|<)\s*(\w+)", cond)
    if m:
        a, b = number(m[1]), number(m[3])
        if a is None or b is None:
            return None
        return {"==": a == b, "!=": a != b, ">=": a >= b, "<=": a <= b, ">": a > b, "<": a < b}[m[2]]
    m = re.fullmatch(r"(\w+)", cond)
    if m:
        return number(m[1]) != 0 if number(m[1]) is not None else known(m[1])
    return None


def balanced(s):
    depth = 0
    for ch in s:
        depth += (ch == "(") - (ch == ")")
        if depth < 0:
            return False
    return depth == 0


def split_top(s, op):
    parts, depth, start, i = [], 0, 0, 0
    while i < len(s):
        if s[i] == "(":
            depth += 1
        elif s[i] == ")":
            depth -= 1
        elif depth == 0 and s.startswith(op, i):
            parts.append(s[start:i])
            start = i + len(op)
            i += len(op) - 1
        i += 1
    parts.append(s[start:])
    return parts


def strip_comments(text):
    """Comments, string and character literals become spaces, newlines kept."""
    def blank(m):
        return re.sub(r"[^\n]", " ", m[0])
    text = re.sub(r"/\*.*?\*/", blank, text, flags=re.S)
    text = re.sub(r"//[^\n]*", blank, text)
    text = re.sub(r'"(?:\\.|[^"\\\n])*"', blank, text)
    return re.sub(r"'(?:\\.|[^'\\\n])+'", blank, text)


def functions_of(path):
    """The source's function definitions in order: [{name, line, static, inline, active, prefix, params}]. active: a
    final ROM's preprocessor keeps it (not the debug-only ones under #if !defined(NNS_FINALROM)); inline: no code of
    its own."""
    text = strip_comments(open(path, encoding="utf-8", errors="replace").read())
    out, stack = [], []
    states = {True: {"depth": 0, "buf": "", "line": 0}, False: {"depth": 0, "buf": "", "line": 0}}
    header = re.compile(r"^(.*?)\b(\w+)\s*(\((?:[^()]|\([^()]*\))*\))\s*$", re.S)
    lines = text.split("\n")
    i = 0
    while i < len(lines):
        line, n = lines[i], i + 1
        while line.rstrip().endswith("\\") and i + 1 < len(lines):  # a macro continued
            i += 1
            line = line.rstrip()[:-1] + lines[i]
        i += 1
        s = line.strip()
        if s.startswith("#"):
            d = re.match(r"#\s*(\w+)\s*(.*)", s)
            if not d:
                continue
            kind, arg = d[1], d[2]
            parent = all(a for a, _, _ in stack) if stack else True
            if kind in ("if", "ifdef", "ifndef"):
                v = evaluate(arg) if kind == "if" else known(arg.split()[0] if arg.split() else "")
                if kind == "ifndef":
                    v = None if v is None else not v
                v = True if v is None else v
                stack.append((parent and v, v, parent))
            elif kind == "elif" and stack:
                _, taken, parent = stack[-1]
                v = evaluate(arg)
                v = True if v is None else v
                stack[-1] = (parent and not taken and v, taken or v, parent)
            elif kind == "else" and stack:
                _, taken, parent = stack[-1]
                stack[-1] = (parent and not taken, True, parent)
            elif kind == "endif" and stack:
                stack.pop()
            continue
        active = all(a for a, _, _ in stack)
        st = states[active]  # the kept code and the dropped code each count their own braces
        for ch in line:
            if ch == "{":
                if st["depth"] == 0:
                    m = header.match(st["buf"].strip())
                    if m and "=" not in m[1] and not re.search(r"\b(if|while|for|switch|return|sizeof)$",
                                                                m[1].strip()):
                        out.append({"name": m[2], "line": st["line"], "static": bool(re.search(r"\bstatic\b", m[1])),
                                    "inline": bool(re.search(r"\binline\b|_INLINE\b", m[1])), "active": active,
                                    "prefix": " ".join(m[1].split()), "params": " ".join(m[3].split())})
                    st["buf"] = ""
                st["depth"] += 1
            elif ch == "}":
                st["depth"] -= 1
                if st["depth"] == 0:
                    st["buf"] = ""
            elif st["depth"] == 0:
                if ch == ";":
                    st["buf"] = ""
                else:
                    if not st["buf"].strip():
                        st["line"] = n
                    st["buf"] += ch
        if st["depth"] == 0:
            st["buf"] += " "
    out.sort(key=lambda f: f["line"])
    return out


def expected(funcs):
    """The functions a final ROM's object has code of its own for (not inline, not debug-only)."""
    return [f for f in funcs if f["active"] and not f["inline"]]


# --- Black ---------------------------------------------------------------------------------------------------------

def read_symbols():
    """The module's functions sorted, [(addr, size, name, mode)], and every symbol of it {addr: (name, kind)}."""
    text = open(f"{CONFIG}/{MODDIR}/symbols.txt").read()
    funcs = sorted((int(a, 16), int(s, 16), n, m) for n, m, s, a in
                   re.findall(r"^(\S+) kind:function\((\w+),size=(0x\w+)[^)]*\) addr:(0x\w+)", text, re.M))
    syms = {int(a, 16): (n, k) for n, k, a in re.findall(r"^(\S+) kind:(\w+)[^\n]*? addr:(0x\w+)", text, re.M)}
    return funcs, syms


def all_symbol_names():
    names = set()
    for r, _, fs in os.walk(CONFIG):
        if "symbols.txt" in fs:
            names |= set(re.findall(r"^(\S+) kind:", open(os.path.join(r, "symbols.txt")).read(), re.M))
    return names


def read_tsv(tsv=TSV):
    """{addr: [(source, library, file, function, variants)]} of the module's twins of TINY bytes or more."""
    out = defaultdict(list)
    if not os.path.exists(tsv):
        sys.exit(f"{tsv} is missing: run tools/lib_twins.py first")
    for line in open(tsv).read().splitlines()[1:]:
        r = line.split("\t")
        if r[1] == MODULE and int(r[3]) >= TINY:
            out[int(r[2], 16)].append((r[4], r[5], r[6], r[7], r[8]))
    return out


def lis(pairs):
    """The longest subsequence of (black index, source index) pairs increasing in both."""
    pairs = sorted(set(pairs))
    if not pairs:
        return []
    best, prev = [1] * len(pairs), [-1] * len(pairs)
    for i, (b, s) in enumerate(pairs):
        for j in range(i):
            if pairs[j][0] < b and pairs[j][1] < s and best[j] + 1 > best[i]:
                best[i], prev[i] = best[j] + 1, j
    i = max(range(len(pairs)), key=lambda k: (best[k], -k))
    out = []
    while i >= 0:
        out.append(pairs[i])
        i = prev[i]
    return out[::-1]


def pair(black, twins, source, exp, checkout):
    """Pairs Black's functions with the source's by the TSV and by names Black already has, in the source's order:
    ([(black index, source index)], notes)."""
    names = {f["name"]: j for j, f in enumerate(exp)}
    cands = []
    for i, (addr, size, name, _) in enumerate(black):
        if name in names:
            cands.append((i, names[name]))
        for src, _, file, func, _ in twins.get(addr, []):
            if func in names and (src.split("@")[0] == checkout
                                  or os.path.basename(file) == os.path.basename(source)):
                cands.append((i, names[func]))
    anchors, notes = lis(cands), []
    if anchors:  # leave out the twins by accident far from the rest: the same code in the game's own files
        mid = sorted(i for i, _ in anchors)[len(anchors) // 2]
        window = 3 * len(exp) + 16
        near = [(i, j) for i, j in cands if abs(i - mid) <= window]
        far = sorted(set(black[i][2] for i, _ in cands if abs(i - mid) > window))
        if far:
            notes.append(f"twins far from the others left out (the same code by accident): {', '.join(far)}")
        anchors = lis(near)
        chosen = dict(anchors)
        for i, j in sorted(set(near) - set(anchors)):
            if i in chosen:
                notes.append(f"{black[i][2]} has {exp[j]['name']}'s code too (paired as {exp[chosen[i]]['name']})")
            else:
                notes.append(f"{black[i][2]} has {exp[j]['name']}'s code but is out of order: paired by order")
    return anchors, notes


def align(bi, sj, score):
    """The increasing pairing of every Black index of bi with one of sj (the rest of sj dead-stripped) of the best
    total score(i, j): [(i, j)]."""
    n, m, low = len(bi), len(sj), float("-inf")
    best = [[0.0] * (m + 1)] + [[low] * (m + 1) for _ in range(n)]
    for a in range(1, n + 1):
        for b in range(a, m + 1):
            best[a][b] = max(best[a][b - 1], best[a - 1][b - 1] + score(bi[a - 1], sj[b - 1]))
    out, a, b = [], n, m
    while a:
        if best[a][b] == best[a][b - 1] and b > a:
            b -= 1
        else:
            out.append((bi[a - 1], sj[b - 1]))
            a, b = a - 1, b - 1
    return out[::-1]


def fill(black, exp, anchors, sizes=None, twins=None):
    """Pairs the functions between anchors: by order where both sides count the same, else by the code and size of
    the source's compiled functions (sizes: source_sizes()) and the twins' names: ({black index: source index},
    [source names Black lacks], notes). Functions with the same code but for the data they point to
    (GX_ResetBankForTex, ..ForSubBG) are told apart by a second pass: the pair whose words point where the first
    pass's pairs say each symbol is in Black is the right one."""
    sizes = sizes or {}
    names = {f["name"] for f in exp}
    if sizes and anchors and anchors[-1][0] - anchors[0][0] <= anchors[-1][1] - anchors[0][1]:
        # all of it at once: the twins' names alone pick wrong ones among functions with the same code
        anchors = [anchors[0], anchors[-1]]
    bwords = black_words(black) if sizes else {}

    def bases(i, j):
        src = {o: (s, a) for o, s, a in sizes.get(exp[j]["name"], (0, 0, []))[2]}
        return {(src[o][0], t - src[o][1]) for o, t in bwords.get(i, []) if o in src}

    def kind(i, j):  # 3 the same code (a twin), 2 the same size, else -1 (0 without the source's sizes)
        name = exp[j]["name"]
        same = {r[3] for r in (twins or {}).get(black[i][0], []) if r[3] in names}
        size = sizes.get(name, (None,))[0]
        return 3 if name in same else 2 if size == black[i][1] else -1 if same or size is not None else 0

    def run(where):
        def score(i, j):
            k = kind(i, j)
            found = bases(i, j) if where and k >= 2 else set()
            return k + (0 if not found else 1 if all(where.get(s) == b for s, b in found) else -1)

        paired, missing, notes = dict(anchors), [], []
        for (i1, j1), (i2, j2) in zip(anchors, anchors[1:]):
            bi, sj = list(range(i1 + 1, i2)), list(range(j1 + 1, j2))
            if len(bi) == len(sj):
                paired.update(zip(bi, sj))
            elif len(bi) < len(sj):
                if not bi:
                    missing += [exp[j]["name"] for j in sj]
                elif sizes:
                    pairs = align(bi, sj, score)
                    bad = [black[i][2] for i, j in pairs if kind(i, j) < 2]
                    pairs = [(i, j) for i, j in pairs if kind(i, j) >= 2]  # no evidence: no name
                    paired.update(pairs)
                    missing += [exp[j]["name"] for j in sj if j not in {p[1] for p in pairs}]
                    if bad:
                        notes.append(f"CHECK between {black[i1][2]} and {black[i2][2]}: {', '.join(bad)} have no "
                                     "source function of their code or size: left unpaired (Black's version differs, "
                                     "or another file's)")
                else:
                    notes.append(f"between {black[i1][2]} and {black[i2][2]}: Black has {len(bi)} function(s) "
                                 f"({', '.join(black[i][2] for i in bi)}) where the source has {len(sj)} "
                                 f"({', '.join(exp[j]['name'] for j in sj)}): {len(sj) - len(bi)} dead-stripped, "
                                 "unnamed")
                    missing.append(f"{len(sj) - len(bi)} of " + "/".join(exp[j]["name"] for j in sj))
            else:
                notes.append(f"CHECK between {black[i1][2]} and {black[i2][2]}: Black has {len(bi)} functions "
                             f"({', '.join(black[i][2] for i in bi)}) where the source has {len(sj)}: not the "
                             "source's order, or another file's code in between")
        return paired, missing, notes

    paired, missing, notes = run({})
    if not bwords:
        return paired, missing, notes
    votes = defaultdict(Counter)
    for i, j in paired.items():
        for s, b in bases(i, j):
            votes[s][b] += 1
    return run({s: c.most_common(1)[0][0] for s, c in votes.items()})


def neighbour(twins, black, idx, step, own_names, checkout):
    """The next twinned function of another file from Black index idx in direction step: (its Black index, how many
    functions its source has on this side of it or None, 'source/file')."""
    i = idx + step
    while 0 <= i < len(black):
        rows = [r for r in twins.get(black[i][0], []) if r[3] not in own_names and r[0] not in RAW]
        if rows:
            rows.sort(key=lambda r: (r[0].split("@")[0] != checkout, SOURCES[r[0]][0] == "none"))
            src, _, file, func, _ = rows[0]
            path = f"{REF}/{src.split('@')[0]}/{file}"
            label = f"{src}/{file}"
            if not os.path.exists(path):
                return i, None, label
            names = [f["name"] for f in expected(functions_of(path))]
            if func not in names:
                return i, None, label
            k = names.index(func)
            return i, (k if step > 0 else len(names) - 1 - k), label
        i += step
    return max(-1, min(i, len(black))), None, None


def edges(black, exp, paired, twins, checkout, own_names, files, sizes=None):
    """Extends the file over the untwinned functions before its first pair and after its last, splitting them with
    the neighbouring file by count: (first index, last index, notes)."""
    notes = []
    first, last = min(paired), max(paired)
    claimed = set()
    for secs in files.values():
        if ".text" in secs:
            a, b = secs[".text"]
            claimed |= {f[0] for f in black if a <= f[0] < b}
    for step in (-1, 1):
        edge = last if step > 0 else first
        rest = exp[max(paired.values()) + 1:] if step > 0 else exp[:min(paired.values())]
        count = len(rest)
        n_i, k, nfile = neighbour(twins, black, edge, step, own_names, checkout)
        free = [i for i in (range(edge + 1, n_i) if step > 0 else range(n_i + 1, edge)) if black[i][0] not in claimed]
        n, where = len(free), "after" if step > 0 else "before"
        if n == 0:
            if count:
                notes.append(f"the source's {count} function(s) {where} {black[edge][2]} are dead-stripped: "
                             f"{', '.join(f['name'] for f in rest)}")
            continue
        if n == count or (k == 0 and n < count):
            ours = free
        elif k is not None and n == count + k:
            ours = (free[:count] if step > 0 else free[-count:]) if count else []
            if not count:
                continue  # all the next file's
        else:
            ours = []
        if not ours:
            notes.append(f"CHECK {where} {black[edge][2]}: {n} untwinned function(s) "
                         f"({', '.join(black[i][2] for i in free)}) between this file and {nfile or 'nothing twinned'}"
                         f" (this source has {count} there, that one {k}): left out")
            continue
        if n < count:
            notes.append(f"{where} {black[edge][2]}: {n} untwinned function(s) taken as this file's; the source has "
                         f"{count} there ({', '.join(f['name'] for f in rest)}) and the next file ({nfile}) none: "
                         f"{count - n} dead-stripped, so these stay unnamed")
        js = range(len(exp) - count, len(exp)) if step > 0 else range(count)
        off = [black[i][2] for i, j in zip(ours, js) if sizes and len(ours) == count
               and sizes.get(exp[j]["name"], (None,))[0] != black[i][1]]
        if off:
            notes.append(f"CHECK {where} {black[edge][2]}: {', '.join(off)} not the size of the source's functions "
                         f"there ({', '.join(f['name'] for f in rest)}): left out, the file ends at its last twin")
            continue
        if step > 0:
            last = max(ours)
            if len(ours) == count:
                paired.update(zip(ours, range(len(exp) - count, len(exp))))
        else:
            first = min(ours)
            if len(ours) == count:
                paired.update(zip(ours, range(count)))
    return first, last, notes


def data_of(start, end, black, syms, header, files, own):
    """The data sections the functions in [start, end) load: ({section: [(start, end)]}, notes)."""
    out, notes = defaultdict(list), []
    func_addrs = sorted(f[0] for f in black)
    addrs = sorted(syms)
    targets = set()
    for line in open(f"{CONFIG}/{MODDIR}/relocs.txt"):
        m = re.match(r"from:(0x\w+) kind:load to:(0x\w+) module:(\S+)", line)
        if m and m[3] == RELOC_MOD and start <= int(m[1], 16) < end:
            targets.add(int(m[2], 16))
    for t in sorted(targets):
        k = bisect.bisect_right(func_addrs, t) - 1
        if k >= 0 and func_addrs[k] <= t < func_addrs[k] + black[k][1]:
            continue  # a function
        section = next((s for s, (a, b) in header.items() if a <= t < b and s != ".text"), None)
        if not section:
            continue
        k = bisect.bisect_right(addrs, t) - 1
        sym = addrs[k] if k >= 0 else t
        nxt = addrs[k + 1] if k + 1 < len(addrs) else header[section][1]
        owner = next((f for f, secs in files.items() if f != own and section in secs
                      and secs[section][0] <= sym < secs[section][1]), None)
        if owner:
            notes.append(f"{section} {syms.get(sym, ('?',))[0]} ({sym:#010x}) is {owner}'s: left out")
            continue
        out[section].append((sym, min(nxt, header[section][1])))
    merged = {}
    for section, ranges in out.items():
        ranges.sort()
        m = [list(ranges[0])]
        for a, b in ranges[1:]:
            if a <= m[-1][1]:
                m[-1][1] = max(m[-1][1], b)
            else:
                m.append([a, b])
        if len(m) > 1:  # a file has one range a section: the symbols between them too, unless another file's
            others = [secs[section] for f, secs in files.items() if f != own and section in secs]
            if any(m[0][0] < b and a < m[-1][1] for a, b in others):
                notes.append(f"CHECK {section}: {len(m)} separate ranges with another file's data between them, only "
                             "the first taken: " + ", ".join(f"{a:#x}-{b:#x}" for a, b in m))
                m = m[:1]
            else:
                notes.append(f"{section}: {len(m)} ranges, taken as one with the symbols between them (not loaded "
                             "by this file's code): " + ", ".join(f"{a:#x}-{b:#x}" for a, b in m))
                m = [[m[0][0], m[-1][1]]]
        merged[section] = [tuple(r) for r in m]
    return merged, notes


# --- the library and the file -----------------------------------------------------------------------------------------

def variant_of(black, paired, twins, checkout, source):
    """The variant tag that gives the most of the file's twins: (tag, Counter)."""
    c = Counter()
    for i in paired:
        for src, _, file, _, variants in twins.get(black[i][0], []):
            if src.split("@")[0] == checkout and file == source:
                c.update(variants.split(","))
    order = list(VARIANTS)
    tag = max(c, key=lambda t: (c[t], -order.index(t) if t in order else -99)) if c else None
    return tag, c


def toml_from_bw2(lib, tag):
    """pokebw2's library.toml for the library with the variant's compiler, mode and -ipa: (text, error)."""
    src = f"{BW2}/lib/{lib}/library.toml"
    if not os.path.exists(src):
        return None, f"pokebw2 has no lib/{lib}/library.toml: write lib/{lib}/library.toml by hand"
    text = open(src).read()
    compiler, flags = VARIANTS[tag]
    text = re.sub(r'^compiler = "[^"]*"', f'compiler = "{compiler}"', text, flags=re.M)
    thumb = "-thumb" in flags.split()
    text = re.sub(r'^(\s*)"-(?:no)?thumb",.*$',
                  lambda m: f'{m[1]}"{"-thumb" if thumb else "-nothumb"}",{" " * (12 if thumb else 10)}'
                            f'# {"Thumb" if thumb else "ARM"} code', text, flags=re.M)
    has_ipa = bool(re.search(r'^\s*"-ipa file"', text, re.M))
    if "-ipa file" in flags and not has_ipa:
        text = re.sub(r'(^\s*"-sym on",.*$)', r'    "-ipa file",           # Interprocedural analysis within a file\n\1',
                      text, count=1, flags=re.M)
    elif "-ipa file" not in flags and has_ipa:
        text = re.sub(r'^\s*"-ipa file",.*\n', "", text, flags=re.M)
    head = (f"# From pokebw2's lib/{lib}/library.toml (GPL-3.0, https://github.com/fuddlesworth/pokebw2), with the "
            f"compiler, mode and -ipa of the\n# variant that gives Black its twins of this library "
            f"(tools/lib_twins.py: {tag}; tools/port_lib.py wrote it).\n")
    return head + text, None


def skeleton(checkout, rel, lib, library, black, first, last, paired, exp, missing, funcs):
    """The new C file of a source that isn't ported as it is (port_c()): a header and the functions to write."""
    license_, url = SOURCES[checkout][:2]
    lines = []
    inc ={"nnsys": f'#include "nnsys/{library}.h"', "nitro": f'#include "nitro/{library}.h"'}.get(lib, "")
    lines.append(f"// {checkout}'s {rel} ({license_ if license_ != 'none' else 'no license'}: read, not copied; "
                 f"{url}), written as natural C with the SDK's names.")
    if missing:
        lines.append(f"// Black lacks {', '.join(missing)}: the linker dead-stripped them.")
    lines += [inc, "", "// Black's functions in order (Black's size; the source's line), to write below:"]
    for i in range(first, last + 1):
        f = exp[paired[i]] if i in paired else None
        lines.append(f"// {black[i][2]}: {black[i][1]:#x}" + (
            f", {f['prefix']} {f['name']}{f['params']} at line {f['line']}" if f else ", unpaired"))
    inl = [f["name"] for f in funcs if f["inline"] and f["active"]]
    if inl:
        lines.append(f"// Inline in the source (their code is in their callers): {', '.join(inl)}")
    return "\n".join(lines) + "\n"


def unifdef(text):
    """text with the conditionals evaluate() can tell resolved: their directives and the branches not taken go; those
    it can't tell stay as they are, with what is inside them resolved."""
    out, stack = [], []  # stack: [state] with state "take", "skip", "done" (a branch was taken) or "keep"/"keep+"
    lines = text.split("\n")
    i = 0
    while i < len(lines):
        line = lines[i]
        full = line
        while full.rstrip().endswith("\\") and i + 1 < len(lines):  # a directive continued
            i += 1
            full = full.rstrip()[:-1] + " " + lines[i]
            line += "\n" + lines[i]
        i += 1
        emitting = all(s in ("take", "keep", "keep+") for s in stack)
        d = re.match(r"\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)", full)
        if not d:
            if emitting:
                out.append(line)
            continue
        kind, arg = d[1], re.sub(r"/\*.*?\*/|//.*", "", d[2]).strip()
        if kind in ("if", "ifdef", "ifndef"):
            v = evaluate(arg) if kind == "if" else known(arg.split()[0] if arg else "")
            if kind == "ifndef" and v is not None:
                v = not v
            stack.append("keep" if v is None else "take" if v else "skip")
            if v is None and emitting:
                out.append(line)
        elif kind == "elif" and stack:
            s = stack[-1]
            if s in ("keep", "keep+"):
                if all(x in ("take", "keep", "keep+") for x in stack[:-1]):
                    out.append(line)
            elif s in ("take", "done"):
                stack[-1] = "done"
            else:
                v = evaluate(arg)
                if v is None:  # the rest of the chain stays, as an #if of its own
                    stack[-1] = "keep+"
                    if all(x in ("take", "keep", "keep+") for x in stack[:-1]):
                        out.append(re.sub(r"#\s*elif", "#if", line, count=1))
                else:
                    stack[-1] = "take" if v else "skip"
        elif kind == "else" and stack:
            s = stack[-1]
            if s in ("keep", "keep+"):
                if all(x in ("take", "keep", "keep+") for x in stack[:-1]):
                    out.append(line)
            else:
                stack[-1] = "done" if s in ("take", "done") else "take"
        elif kind == "endif" and stack:
            s = stack.pop()
            if s in ("keep", "keep+") and all(x in ("take", "keep", "keep+") for x in stack):
                out.append(line)
    return re.sub(r"\n{3,}", "\n\n", "\n".join(out))


def definitions(text):
    """The top-level function definitions of text (resolved by unifdef): [(name, first line, last line)], 0-based,
    the first line that of the comment right above the definition when it has one."""
    lines = strip_comments(text).split("\n")
    raw = text.split("\n")
    header = re.compile(r"^(.*?)\b(\w+)\s*(\((?:[^()]|\([^()]*\))*\))\s*$", re.S)
    out, depth, buf, start, name = [], 0, "", None, None
    for n, line in enumerate(lines):
        if line.lstrip().startswith("#"):
            continue
        for ch in line:
            if ch == "{":
                if depth == 0:
                    m = header.match(buf.strip())
                    name = m[2] if m and "=" not in m[1] and not re.search(
                        r"\b(if|while|for|switch|return|sizeof)$", m[1].strip()) else None
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    if name:
                        first = start
                        while first > 0:
                            prev = raw[first - 1].strip()
                            if prev.startswith("//"):
                                first -= 1
                                continue
                            k = first - 1
                            while prev.endswith("*/") and k >= 0 and "/*" not in raw[k]:
                                k -= 1
                            if not prev.endswith("*/") or k < 0 or not raw[k].strip().startswith("/*"):
                                break
                            first = k
                        out.append((name, first, n))
                    buf, start, name = "", None, None
            elif depth == 0:
                if ch == ";":
                    buf, start = "", None
                elif not ch.isspace() or buf:
                    if start is None and not ch.isspace():
                        start = n
                    buf += ch
        if depth == 0 and buf:
            buf += " "
    return out


def port_c(checkout, rel, lacks, renames):
    """A licensed source's C for Black: the preprocessor's choices for a final ROM made (unifdef), without the
    functions Black lacks (lacks: their names; the inline ones stay), Black's names for those renamed (renames), its
    includes as they are (the library's sdk/ has the headers: vendor()). Returns the text."""
    text = unifdef(read_text(f"{REF}/{checkout}/{rel}"))
    lines = text.split("\n")
    drop, gone = set(), []
    for name, first, last in definitions(text):
        if name in lacks:
            drop.update(range(first, last + 1))
            gone.append(name)
    for n, line in enumerate(lines):  # the prototypes of the functions that go, at the top level
        if any(re.match(rf"^(?![#\s])[\w\s*]+?\b{g}\s*\([^;{{}}]*\)\s*;\s*$", line) for g in gone):
            drop.add(n)
    text = re.sub(r"\n{3,}", "\n\n", "\n".join(l for n, l in enumerate(lines) if n not in drop)).strip("\n")
    for old, new in renames.items():
        text = re.sub(rf"\b{old}\b", new, text)
    # the library's private headers, which vendor() puts at the top of lib/<lib>/sdk
    text = re.sub(r'#include "\.\./include/', '#include "', text)
    license_, url = SOURCES[checkout][:2]
    return (f"// From {checkout} ({license_}, {url}) at {commit_of(checkout)}, {rel}: ported as it is, with credit\n"
            f"// (a project decision of 2026-10-13), with only the functions Black has and its "
            f"preprocessor's\n// choices for Black's final ROM made (tools/port_lib.py).\n{text}\n")


def compile_as_twin(checkout, path, tag, obj, extra=()):
    """path compiled as tools/lib_twins.py compiles checkout's files as variant tag: (its job, the run)."""
    import lib_twins
    job = next(j for j in lib_twins.jobs(REF, f"{ROOT}/build/lib_twins", [checkout]) if j[3] == tag)
    cmd = job[5][:-3] + [*extra, "-o", obj, path]
    return job, subprocess.run(cmd, cwd=ROOT, env=dict(os.environ, MWCIncludes="."), capture_output=True, text=True)


def source_sizes(checkout, rel, tag):
    """{function: (size, digest, [(offset, symbol, addend)] of its words that point to data)} of the source compiled
    as its twins were, or {} when it doesn't compile."""
    import struct
    import clones
    from port_file import read_object
    obj = f"{ROOT}/build/port_lib_source.o"
    _, r = compile_as_twin(checkout, f"{REF}/{checkout.split('@')[0]}/{rel}", tag, obj)
    if r.returncode or not os.path.exists(obj):
        return {}
    d, secs, syms, relocs = read_object(obj)
    words = defaultdict(list)
    for name, ndx, value, size, _, is_func in syms:
        if is_func and size:
            for at, kind, sym, add in relocs.get(ndx, []):
                if kind == 2 and value <= at < value + size:  # R_ARM_ABS32, its addend in place
                    words[name].append((at - value, syms[sym][0] or secs[syms[sym][1]][0],
                                        struct.unpack_from("<I", d, secs[ndx][2] + at)[0] + add))
    return {fn: (size, h, words[fn]) for fn, size, h, _ in clones.functions(obj)}


def extern_names(obj, black):
    """({Black's name: the C's}, {data section: its start in Black}) from the relocations of the ported C's functions
    Black has (by name and size) and Black's at the same offsets (the addend in place taken off a word's): what they
    call or load from other files under other names (GXi_VRamLockId, Black's data_02150e0e), and where the
    sections of their own data are (sDayOfYear, which Black loads at 0x020a86f0 too, as sDayOfYear[month - 1])."""
    import struct
    from port_file import read_object
    d, secs, syms, relocs = read_object(obj)
    at_addr = {}  # (module, address) -> Black's name
    for path in glob.glob(f"{CONFIG}/**/symbols.txt", recursive=True):
        mod = os.path.relpath(os.path.dirname(path), CONFIG)
        for n, a in re.findall(r"^(\S+) kind:\S+ addr:(0x\w+)", open(path).read(), re.M):
            at_addr.setdefault((mod, int(a, 16)), n)
    mods = {"main": ".", "itcm": "itcm", "dtcm": "dtcm", RELOC_MOD: MODDIR}
    rel = {}
    for line in open(f"{CONFIG}/{MODDIR}/relocs.txt"):
        m = re.match(r"from:(0x\w+) kind:(\w+) to:(0x\w+) module:(\S+)", line)
        if m and m[4] in mods:
            rel[int(m[1], 16)] = (m[2], int(m[3], 16), mods[m[4]])
    where = {n: (a, s) for a, s, n, _ in black}
    taken = {n: k for k, n in at_addr.items()}
    out, starts = {}, defaultdict(Counter)
    for name, ndx, value, size, _, is_func in syms:
        if not is_func or name not in where or where[name][1] != size:
            continue
        for off, kind, sym, add in relocs.get(ndx, []):
            r = rel.get(where[name][0] + off - value)
            if not value <= off < value + size or not syms[sym][0] or not r:
                continue
            target = r[1] - (struct.unpack_from("<i", d, secs[ndx][2] + off)[0] + add if kind == 2 else 0)
            if syms[sym][1] == 0:  # an undefined symbol's: another file's, by Black's name
                theirs = at_addr.get((r[2], target))
                if theirs and theirs != syms[sym][0] and taken.get(syms[sym][0]) != (r[2], target):
                    out[theirs] = syms[sym][0]
            elif r[2] == MODDIR and syms[sym][1] < len(secs) and secs[syms[sym][1]][0] in DATA_SECTIONS:
                # the file's own datum: where its section starts in Black
                starts[secs[syms[sym][1]][0]][target - syms[sym][2]] += 1
    return out, {s: c.most_common(1)[0][0] for s, c in starts.items()}


DATA_SECTIONS = (".rodata", ".data", ".bss")


def black_words(black):
    """{Black index: [(offset, target)]}: the words of each function that point to data (relocs.txt's loads)."""
    starts = [f[0] for f in black]
    out = defaultdict(list)
    for line in open(f"{CONFIG}/{MODDIR}/relocs.txt"):
        m = re.match(r"from:(0x\w+) kind:load to:(0x\w+) module:(\S+)", line)
        if m and m[3] == RELOC_MOD:
            a = int(m[1], 16)
            k = bisect.bisect_right(starts, a) - 1
            if k >= 0 and a < black[k][0] + black[k][1]:
                out[k].append((a - black[k][0], int(m[2], 16)))
    return out


def vendor(dest, lib, checkout, tag):
    """The headers dest includes, copied from the source's checkout into lib/<lib>/sdk/ (each under its path below the
    include directory it was found in, a credit line first): dest is compiled as tools/lib_twins.py compiled the
    source, its dependencies read. Returns (the headers copied, compile errors or "")."""
    job, r = compile_as_twin(checkout, f"{ROOT}/{dest}", tag, f"{ROOT}/build/port_lib.o", ["-gccdep", "-MD"])
    dep = f"{ROOT}/build/port_lib.d"
    if r.returncode or not os.path.exists(dep):
        return [], (r.stdout + r.stderr).strip()[-1500:] or "no dependency file"
    roots = [os.path.normpath(job[5][k + 1]) for k, a in enumerate(job[5]) if a in ("-i", "-ir")]
    copied = []
    for path in re.findall(r"Z:(\S+\.h)\b", open(dep).read().replace("\\", "/")):
        root = max((d for d in roots if path.startswith(d.rstrip("/") + "/")), key=len, default=None)
        if not root:
            continue
        sub = path[len(root.rstrip("/")) + 1:]
        there = f"{ROOT}/lib/{lib}/sdk/{sub}"
        name = os.path.relpath(path, REF).split("/")[0] if path.startswith(REF + "/") else checkout.split("@")[0]
        notice = f"{ROOT}/lib/{lib}/sdk/LICENSE-{name}"
        if not os.path.exists(notice) and os.path.exists(f"{REF}/{name}/LICENSE"):
            os.makedirs(os.path.dirname(notice), exist_ok=True)
            open(notice, "w").write(read_text(f"{REF}/{name}/LICENSE"))
        if os.path.exists(there):
            continue
        origin = os.path.relpath(path, REF) if path.startswith(REF + "/") else None
        if origin is None:  # generated: fx_const.h, from libntr's own generator and table
            gen = f"{REF}/{name}/gen/{os.path.dirname(sub)}"
            script = next(iter(glob.glob(f"{gen}/gen_*.py")), None)
            if not script:
                return copied, f"{sub}: generated by tools/lib_twins.py, and {checkout} has no generator for it"
            os.makedirs(os.path.dirname(there), exist_ok=True)
            subprocess.run([sys.executable, "-I", script, *glob.glob(f"{gen}/*.csv"), there], check=True)
            body = read_text(there)
            origin = f"{os.path.relpath(script, REF)}'s output"
        else:
            body = read_text(path)
        license_, url = SOURCES[name][:2] if name in SOURCES else ("?", "?")
        os.makedirs(os.path.dirname(there), exist_ok=True)
        open(there, "w").write(f"// From {name} ({license_}, {url}) at {commit_of(name)}: {origin.split('/', 1)[1]}, "
                               "as it is (tools/port_lib.py)\n" + body)
        copied.append(sub)
    return copied, ""


def commit_of(checkout):
    return subprocess.run(["git", "-C", f"{REF}/{checkout.split('@')[0]}", "rev-parse", "--short=7", "HEAD"], capture_output=True,
                          text=True).stdout.strip()


def read_text(path):
    raw = open(path, "rb").read().replace(b"\r", b"")
    try:
        return raw.decode()
    except UnicodeDecodeError:
        return raw.decode("cp932", errors="replace")


def run(*cmd):
    print("$", " ".join(cmd), flush=True)
    r = subprocess.run(cmd, cwd=ROOT, text=True, capture_output=True)
    print((r.stdout + r.stderr).rstrip())
    return r.returncode


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("source", help="path under ~/ref: <checkout>/<file>, e.g. ntrtwl-NitroSystem/libraries/fnd/src/list.c")
    ap.add_argument("--lib", help="lib/<lib> here (default LIBS' for the checkout)")
    ap.add_argument("--dest", help="the C file to write (default lib/<lib>/src/<library>/<basename>)")
    ap.add_argument("--name", action="append", default=[], metavar="FUNC=NAME",
                    help="name a Black function by hand (func_02060f98=NNS_FndGetGroupIDForMBlockExpHeap)")
    ap.add_argument("--keep-names", action="store_true", help="rename only func_ placeholders, not other names")
    ap.add_argument("--dry-run", action="store_true", help="print what would be done, change nothing")
    ap.add_argument("--no-brief", action="store_true", help="don't run brief.py at the end")
    ap.add_argument("--module", default="main", help="Black's module of the file: main or an overlay (ov009)")
    ap.add_argument("--tsv", default=TSV, help="tools/lib_twins.py's table (default build/lib_twins.tsv)")
    ap.add_argument("--finish", action="store_true", help="then tools/finish.py: commit the file, complete or with "
                    "the functions that don't match parked in docs/nonmatching.md for a worker")
    args = ap.parse_args()
    set_module(args.module)
    if args.finish:
        import brief  # noqa: F401 - now: local_driver.py starts the repo's venv over this process, before any work

    checkout, _, rel = args.source.strip("/").partition("/")
    if checkout not in SOURCES:  # a path under a checkout, e.g. libraries/gx/src/g3x.c: the first that has it
        rel = args.source.strip("/")
        checkout = next((c for c in PREFERRED if os.path.exists(f"{REF}/{c}/{rel}")), None)
        if not checkout:
            sys.exit(f"no checkout of {', '.join(PREFERRED)} has {rel}: give <checkout>/<file>, the checkout one of "
                     f"{', '.join(k for k in SOURCES if '@' not in k)}")
    elif SOURCES[checkout][0] == "none" and checkout in SAME_FILES and \
            os.path.exists(f"{REF}/{SAME_FILES[checkout]}/{rel}"):
        print(f"{checkout} has no license, {SAME_FILES[checkout]} has {rel} too and may be ported: taken from it")
        checkout = SAME_FILES[checkout]
    path = f"{REF}/{checkout}/{rel}"
    if not os.path.exists(path):
        sys.exit(f"{path} does not exist")
    m = re.search(SOURCES[checkout][5], rel)
    library = next((g for g in m.groups() if g), None) if m else None
    lib = args.lib or LIBS.get(checkout) or LIB_OF_LIBRARY.get(library or "")
    if not lib:
        sys.exit(f"which lib/<lib> takes {checkout}'s {library}? give --lib")
    sub = rel.split("/src/", 1)[1] if "/src/" in rel else os.path.basename(rel)
    if library and not sub.startswith(library + "/") and lib != library:
        sub = f"{library}/{sub}"
    dest = args.dest or f"lib/{lib}/src/{sub}"
    stem = os.path.splitext(os.path.basename(rel))[0]

    funcs = functions_of(path)
    exp = expected(funcs)
    if not exp:
        sys.exit(f"no function definitions found in {rel}")
    print(f"{rel}: {len(funcs)} functions, {len(exp)} with code of their own in a final ROM "
          f"({sum(f['inline'] for f in funcs)} inline, {sum(not f['active'] for f in funcs)} debug-only)")

    black, syms = read_symbols()
    twins = read_tsv(args.tsv)
    anchors, notes = pair(black, twins, rel, exp, checkout)
    if not anchors:
        sys.exit(f"{args.tsv} pairs none of Black's {MODULE} functions with {rel}")
    tag0 = variant_of(black, dict(anchors), twins, checkout, rel)[0]
    sizes = source_sizes(checkout, rel, tag0) if tag0 else {}
    paired, missing, fnotes = fill(black, exp, anchors, sizes, twins)
    notes += fnotes
    header, files = read_delinks(CONFIG)[MODULE]
    own_names = {f["name"] for f in exp}
    first, last, enotes = edges(black, exp, paired, twins, checkout, own_names, files, sizes)
    notes += enotes
    start = black[first][0]
    end = black[last + 1][0] if last + 1 < len(black) else black[last][0] + black[last][1]
    if black[last][0] + black[last][1] != end:
        notes.append(f"{end - black[last][0] - black[last][1]:#x} bytes between the last function's end and the next "
                     "function: taken as padding")
    for f, secs in files.items():
        if f != dest and ".text" in secs and secs[".text"][0] < end and start < secs[".text"][1]:
            sys.exit(f".text {start:#010x}-{end:#010x} overlaps {f}'s {secs['.text'][0]:#x}-{secs['.text'][1]:#x}")
    data, dnotes = data_of(start, end, black, syms, header, files, dest)
    notes += dnotes

    # names
    forced = dict(n.split("=", 1) for n in args.name)
    taken = all_symbol_names()
    b2_symbols = f"{BW2}/config/b2_us/arm9/symbols.txt"
    b2_names = set(re.findall(r"^(\S+) kind:function", open(b2_symbols).read(), re.M)) \
        if os.path.exists(b2_symbols) else set()
    renames, used = {}, set()
    print(f"\n.text {start:#010x}-{end:#010x}: {last - first + 1} functions ({first}..{last} of {MODULE}'s)")
    anchored = {i for i, _ in anchors}
    for i in range(first, last + 1):
        addr, size, name, mode = black[i]
        want = forced.get(name) or (exp[paired[i]]["name"] if i in paired else None)
        tag = "unpaired" if want is None else "twin" if i in anchored else "by order"
        if want and want != name and (PLACEHOLDER.match(name) or not args.keep_names or name in forced):
            new = want
            if (new in taken and new != name) or new in used:
                alt = sorted(n for n in b2_names if n.startswith(new) and n != new and n not in taken and n not in used)
                new = alt[0] if len(alt) == 1 else f"{new}_{stem}"
                tag += f"; {want} is taken, so {new}"
            renames[name] = new
            used.add(new)
        elif want:
            used.add(want)
        print(f"  {addr:#010x} {size:#6x} {mode:5} {name:36} -> {renames.get(name, want or name):36} {tag}")
    if missing:
        print(f"Black lacks (dead-stripped): {', '.join(missing)}")
    for n in notes:
        print(f"note: {n}")
    for section, ranges in data.items():
        for a, b in ranges:
            print(f"{section} {a:#010x}-{b:#010x} ({b - a:#x} bytes): "
                  f"{', '.join(syms[s][0] for s in sorted(syms) if a <= s < b)}")
    vtag, counts = variant_of(black, paired, twins, checkout, rel)
    print(f"variants of the twins: {dict(counts)} -> {vtag} = {VARIANTS.get(vtag) if vtag else None}")

    toml = f"{ROOT}/lib/{lib}/library.toml"
    toml_text = None
    if os.path.exists(toml):
        cur = open(toml).read().split("\n[")[0]  # the library's own flags, not a table's
        compiler = re.search(r'^compiler = "([^"]*)"', cur, re.M)
        thumb = bool(re.search(r'^\s*"-thumb"', cur, re.M))
        ipa = bool(re.search(r'^\s*"-ipa file"', cur, re.M))
        if vtag:
            want_c, want_f = VARIANTS[vtag]
            agree = (compiler and compiler[1] == want_c) and thumb == ("-thumb" in want_f.split()) \
                and ipa == ("-ipa file" in want_f)
            print(f"lib/{lib}/library.toml exists: {compiler[1] if compiler else '?'}, {'thumb' if thumb else 'arm'}"
                  f"{', ipa' if ipa else ''}: {'agrees with' if agree else 'DIFFERS from'} the twins' {vtag}")
    elif vtag:
        toml_text, err = toml_from_bw2(lib, vtag)
        print(f"lib/{lib}/library.toml: {'to write from pokebw2 with ' + vtag if toml_text else err}")
    sections = [f".text:{start:#010x}-{end:#010x}"] + [f"{s}:{a:#010x}-{b:#010x}" for s, rs in data.items()
                                                      for a, b in rs]
    exists = os.path.exists(f"{ROOT}/{dest}")
    how = "ported as it is, with credit" if SOURCES[checkout][0] in LICENSED and checkout not in RAW else \
        "a skeleton to write natural C into"
    print(f"{dest}: {'exists, kept' if exists else 'to write (' + how + ')'}")
    print(f"delinks: {' '.join(sections)}" + (" (already listed)" if dest in files else ""))
    if args.dry_run:
        print("dry run: nothing changed")
        return

    if renames:
        rename_everywhere(renames)
        print(f"renamed: {', '.join(f'{a} -> {b}' for a, b in renames.items())}")
        black = [(a, s, renames.get(n, n), m) for a, s, n, m in black]
    if toml_text:
        os.makedirs(os.path.dirname(toml), exist_ok=True)
        open(toml, "w").write(toml_text)
    ported = SOURCES[checkout][0] in LICENSED and checkout not in RAW
    if not exists and ported:
        # the source's functions under Black's names where those differ (a static renamed <name>_<stem>)
        keep = {exp[paired[i]]["name"] for i in range(first, last + 1) if i in paired}
        own = {exp[paired[i]]["name"]: black[i][2] for i in range(first, last + 1) if i in paired}
        unpaired = [black[i][2] for i in range(first, last + 1) if i not in paired]
        os.makedirs(os.path.dirname(f"{ROOT}/{dest}"), exist_ok=True)
        text = port_c(checkout, rel, {f["name"] for f in exp} - keep, {s: b for s, b in own.items() if s != b})
        open(f"{ROOT}/{dest}", "w").write(text)
        mine = {black[i][2] for i in range(first, last + 1)}
        order = [n for n, _, _ in definitions(text) if n in mine]
        want = [black[i][2] for i in range(first, last + 1) if black[i][2] in order]
        if order != want:
            print(f"note: the C's order isn't Black's: {', '.join(order)} / Black's {', '.join(want)}")
        if unpaired:
            print(f"note: Black's {', '.join(unpaired)} pair with no function of the source: not in the C")
        copied, err = vendor(dest, lib, checkout, vtag)
        print(f"lib/{lib}/sdk/: {len(copied)} headers copied" + (f"; the compile failed: {err}" if err else ""))
        sdk_toml(toml, dest, lib, checkout, vtag)
        ipa = f"{vtag}-ipa" if f"{vtag}-ipa" in VARIANTS else vtag  # as [sdk] builds it
        if not err and compile_as_twin(checkout, f"{ROOT}/{dest}", ipa, f"{ROOT}/build/port_lib.o")[1].returncode == 0:
            # only the data sections the C has: what its code loads may be another file's (GXi_DmaId)
            from port_file import read_object
            names, starts = extern_names(f"{ROOT}/build/port_lib.o", black)
            taken = all_symbol_names()
            clash = {a: b for a, b in names.items() if b in taken}
            names = {a: b for a, b in names.items() if b not in taken and (PLACEHOLDER.match(a) or
                                                                           re.match(r"data_[0-9a-f]{8}$", a)
                                                                           or not args.keep_names)}
            if names:
                rename_everywhere(names)
                print(f"renamed what it calls and loads: {', '.join(f'{a} -> {b}' for a, b in names.items())}")
            if clash:
                print(f"CHECK: Black's {', '.join(f'{a} is the C {b}' for a, b in clash.items())}, a name Black has "
                      "already: not renamed, so the file won't link")
            has = Counter()
            for name, _, _, size, align in read_object(f"{ROOT}/build/port_lib.o")[1]:
                if name in DATA_SECTIONS and size:
                    has[name] += -(-size // max(4, align)) * max(4, align)
            gone = [s for s in sections[1:] if s.split(":")[0] not in has]
            if gone:
                print(f"note: the C has no {', '.join(s.split(':')[0] for s in gone)}: those ranges left out")
            guess = {s.split(":")[0]: s for s in sections[1:] if s not in gone}
            sections = sections[:1] + [f"{n}:{starts[n]:#010x}-{starts[n] + size:#010x}" if n in starts else
                                       guess.get(n, "") for n, size in has.items()]
            sections = [s for s in sections if s]
            print(f"delinks from the object and Black's relocations: {' '.join(sections)}")
    elif not exists:
        os.makedirs(os.path.dirname(f"{ROOT}/{dest}"), exist_ok=True)
        open(f"{ROOT}/{dest}", "w").write(skeleton(checkout, rel, lib, library, black, first, last, paired, exp,
                                                   missing, funcs))
        if sections[1:]:  # a guess a wrong start of breaks the build (an unaligned .bss); tools/fit_data.py fits them
            print(f"note: the data ranges guessed are left for when its functions match: {' '.join(sections[1:])}")
            sections = sections[:1]
    if dest not in files:
        run(sys.executable, "tools/add_source_file.py", dest, MODDIR, *sections, "--incomplete")
    if args.finish:
        finish(dest, checkout, rel, ported)
    elif not args.no_brief:
        run(sys.executable, "tools/brief.py", dest)


def sdk_toml(toml, dest, lib, checkout, tag):
    """dest in lib/<lib>/library.toml's [sdk] files, the table written first if it is missing: the sdk/ headers ahead
    of the game's and the defines tools/lib_twins.py compiles the checkout with."""
    text = open(toml).read()
    sub = dest.split("/src/", 1)[1]
    if "\n[sdk]\n" not in text:
        code = "SDK_CODE_THUMB" if "-thumb" in VARIANTS[tag][1].split() else "SDK_CODE_ARM"
        flags = [f"-i lib/{lib}/sdk"] + [f"-d {d}" for d in SOURCES[checkout][3] + [code]] + \
            re.findall(r"-\S+ \S+", IPA)
        text = text.rstrip("\n") + (
            f"\n\n# Files ported as they are from {checkout} ({SOURCES[checkout][0]}, {SOURCES[checkout][1]}) by "
            "tools/port_lib.py build as\n# tools/lib_twins.py built their twins: with its headers (sdk/) ahead of the "
            "game's, and its defines for a final ROM.\n# -ipa file, as the SDK was built: the code is the same "
            "without it, but the static local of the unused\n# inline GX_SetVCount (gx.h) then stays in each "
            "object's .bss, where Black's link dropped it\n[sdk]\nflags = [\n"
            + "".join(f'    "{f}",\n' for f in flags) + "]\n# the SDK defines some functions without a prototype "
            "before them (OSi_InsertThreadToList)\nwithout = [\"-requireprotos\"]\nfiles = [\n]\n")
    if f'"{sub}"' not in text.split("\n[sdk]\n", 1)[1]:
        head, tail = text.split("\n[sdk]\n", 1)
        k = tail.index("files = [\n") + len("files = [\n")
        k = tail.index("]\n", k)
        tail = tail[:k] + f'    "{sub}",\n' + tail[k:]
        text = head + "\n[sdk]\n" + tail
    open(toml, "w").write(text)


def finish(dest, checkout, rel, ported):
    """tools/finish.py on the file (the functions that match marked, the others parked in docs/nonmatching.md for a
    worker, ninja, the commit), then the source's credit in the commit message."""
    how = f"ported from {checkout}'s {rel} as it is (tools/port_lib.py)" if ported else \
        f"not written: {checkout} has no license, so a worker writes natural C from its {rel} (brief.py shows it)"
    import importlib
    import brief
    import configure
    importlib.reload(configure)  # library.toml's [sdk] files as they are now
    path, _, _, funcs = brief.locate(f"{ROOT}/{dest}")
    judged, errors = brief.judge(path, funcs)
    left = [n for n, (score, _) in judged.items() if score]
    print(f"{dest}: {len(funcs) - len(left)} of {len(funcs)} functions match" +
          (f"; it doesn't compile: {errors.splitlines()[0]}" if errors else ""))
    tried = "; ".join(f"{n}: {how}" for n in left)
    # the callers the renames changed go in the same commit
    others = [f for f in subprocess.run(["git", "diff", "--name-only", "--", "*.c"], cwd=ROOT, capture_output=True,
                                        text=True).stdout.split() if f != dest]
    r = subprocess.run([sys.executable, "tools/finish.py", dest, tried, *(["--with", *others] if others else [])],
                       cwd=ROOT, capture_output=True, text=True)
    print("\n".join(l if not l.startswith("files committed: ") else f"files committed: {l.count(', ') + 1}"
                    for l in (r.stdout + r.stderr).rstrip().splitlines()))
    if r.returncode or "committed:" not in r.stdout:
        return
    msg = subprocess.run(["git", "log", "-1", "--format=%B"], cwd=ROOT, capture_output=True, text=True).stdout.strip()
    license_, url = SOURCES[checkout][:2]
    credit = (f"Ported as it is from {checkout}'s {rel} ({license_}, {url}) at {commit_of(checkout)}, with its "
              f"headers in {dest.split('/src/')[0]}/sdk, by tools/port_lib.py." if ported else
              f"Set up by tools/port_lib.py from {checkout}'s {rel} ({url}): read, not copied.")
    subprocess.run(["git", "commit", "-q", "--amend", "-m", msg + "\n\n" + credit], cwd=ROOT)


if __name__ == "__main__":
    main()
