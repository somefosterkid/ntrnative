#!/usr/bin/env python3
"""MWCC data-order solver: Black's order of a file's .rodata against ours, and the declarations to move to get it.

    python3 tools/data_order.py src/ov211/zukan_open.c          # the moves that give Black's order, or why none does
    python3 tools/data_order.py --time-limit 60 FILE            # the search stops after 60 s (default 300, 0: none) and
                                                                # prints the closest order it found
    python3 tools/data_order.py --check src/ov211/zukan_open.c  # the model against our object, object by object
    python3 tools/data_order.py --compile test.c                # compile a C file with the game flags, list its data
    python3 tools/data_order.py --apply src/ov120/gtsnego_proc.c  # do the move, keep it if fit_data.py completes the file
    python3 tools/data_order.py --like HEAD lib/nitrowifi/src/cps/cps.c   # Black's .bss is unknown (an -ipa file reaches
                                                                # its statics from one base): the C at HEAD stands for it
    python3 tools/data_order.py --glued FILE   # does the data fit Black's order only as two objects (tools/glued_files.py)?

--apply moves top-level items of the C: a data definition (a declaration left in its place when code before its new
one uses it; one without an initializer gets one, = NULL, = 0 or = {0}), or a static inline helper with its locals. It
tries every item before every other (after the #includes, outside #if), and pairs of them, in the model, without
compiling; of the ways that give Black's order, the fewest moves, definitions before helpers, the shortest first, it
does the first that compiles, keeps the code (each .text section's bytes and relocations) and gives Black's order in
the model of the new object, then runs tools/fit_data.py, which marks the file complete if ninja passes. Otherwise the
C is put back and it says why. A local of a function that isn't a static inline helper can't move: it says so.

When no moves of definitions give Black's order, --apply tries the other form of the data: a struct const that one
statement of a function copies (`matrix->translation = data_X;`, `T v = data_X;`) written as a local initializer
(`VecFx32 translationInit = { ... }; matrix->translation = translationInit;`), which compiles to the same code and is an
object of a section of its own, laid out after the shared run: ov043_021f36c0.c's two 12-byte VecFx32 came after the
24-byte table, which no order of definitions gives. The search then goes up to five moves (a beam search over the
items), a definition without an initializer that moves stays one (it is listed at the end, in the C's order) before it
gets one, and equal bytes (ours @5946 and @5968) are told apart by the pool words that point to them (Black's
relocations). A library file's module is the one whose delinks.txt has it (lib/nitrowifi's cps.c is in ov009).
.bss: Black's objects are its labels, when each of ours has one; --like REV takes the order the C had at REV.
When only equal sizes are out of order, the model runs backwards (invert()): heapsort compares sizes only, so with
every slot of the list keeping its size it is one fixed permutation of the slots, and "swap" lists the named
definitions to put in the places of the equal-size objects Black has where they lay out, any number of them (box2_obj.c
and box2_ui.c needed a permutation search). When Black's order differs by more than ties, it says no declaration order
gives it and tries the file built with -ipa function (a per-file flag; then MWCC writes the objects above the first
function sorted, every later one where it is defined, and the local initializers as it emits their function), which
gave field_wfbc_data_local.c's order with the code unchanged. The other "impossible" layouts were not the compiler:
a table cut short at other modules' labels (ov021_02197da0.c's 756-byte RAIL table, 92 bytes in the C), constants
the C writes as macros that Black has as objects in the gap before the range (shinka_demo_view.c, pokebw2's twin
defines them), a label over a referenced object and an unused local static (scrcmd_shop.c's u8[3] counts and 4 zero
bytes), two files (ov021_021c8984.c: pokebw2's field_slope_slip.c and field_water_current.c), and .bss statics the C
reaches through an extern base instead of defining them, which leaves them out of the list (sb_server.c).

The model is pokebw2's tools/decomp/rodata_order.py (GPL-3.0, https://github.com/fuddlesworth/pokebw2), checked here
against dsi/1.1p1 with the game flags (each rule by random test files, hundreds of objects, none off):
- MWCC lists each object as it parses it, .rodata, .data AND .bss in ONE list (cg_help.c's .data proc table moves its
  .rodata; gfl_use.c's six 4-byte statics order its 235 path strings): a file-level object at its definition, the
  strings of an initializer just before it (in its order; one used twice is listed the first time), a function's
  local initializers and its local statics (`static` of any section) where each is declared in it (a static inline helper's where the
  helper is defined, even after its caller, so where a helper sits moves its initializers without touching code).
  A definition without an initializer (`static Foo *p;`, a tentative definition, .bss) is listed at the END of the
  file, the last one first; with one (`= NULL`, `= {0}`; still .bss) it is listed where it is. A file-level `static
  const` scalar (`static const u32 x = 0;`, .rodata: no array, no braces; MWCC folds its reads) is listed only when
  code takes its address, after everything else, tentative definitions too: each with the first function whose text
  has `&x` (a static inline helper's own body in its place, not where it is inlined), the functions' groups in
  REVERSE order of their definitions, each group in the order of its first `&x`s; those only data initializers reach
  come last, in the C's order (wifibattlematch_net.c's 4-byte data_ov175_021e2c8c ordered its .data's 4-byte
  strings; random files of such scalars, tentatives and string tables, none off). A local const scalar (`const int
  x = 16;` or `static const u8 x = 1;`, no braces: MWCC emits an object `_ZZ...` even when every read is folded) is
  listed so too, in its function's group where it is declared among the `&x`s (namein.c's eight 4-byte locals of
  three functions, ctvt_talk.c's two static u8; random files, none off). It heapsorts the list by size (max-heap, strict >) from the last object listed. Heapsort isn't stable: equal sizes come out in an order
  set by where every object of the list was declared, so moving one object can reorder others.
- Objects of 64 bytes or more, local initializers and globals nothing in the file refers to (a proc table only another
  overlay loads) get sections of their own; the others share one per section, which sits where the sorted list reaches
  its smallest object (tools/data_order.py's first model, all of them in one sorted run, was wrong there: zukan_open.c's
  proc table is the file's after all). An unreferenced global sits before or after the shared run, never inside it
  (all 5040 orders of 7 objects tried). A reference is anything the parse sees, even what the compile then drops: a
  static function nothing calls, a static inline nothing calls, an unused static's initializer (`static const T *p =
  &table;`), a folded read of a const table's element (`table[1]`, no relocation left); `sizeof` and `if (0)` aren't.
  An unused static is in no list.
- That is dsi/* with -ipa file. The file's compiler and flags (configure.py's compiler_of(): library.toml,
  compiler_flags.toml) change it (MODE): MWCC 2.0 gives a section of its own to each byte array and string (a local's
  initializer too, which otherwise shares one) and none by size (atermset.c's 1024-byte AES tables share one; 2.0/sp2p3
  with -ipa file does both), dsi/1.1p1 without -ipa file none by size. Without -ipa file MWCC sorts only what is listed
  up to the end of the first function that isn't inline (that function's locals too), lays out the later objects as
  they are listed, then the tentative definitions (the last first) and the static const scalars (the reverse of their
  first `&x`s in the C) unsorted (gsPlatformUtil.c: its .bss statics come out the last defined first). 1.2/base sorts
  every object and lists the scalars where they are defined. Random files of each compiler and mode, none off
  (1.2/base: about 1 in 20 has its ties otherwise).
- A function a dead-stripping link drops still put its initializers in the list; its own go with it.
- String literals used by code are pooled into .data, one object per distinct literal, in order of FIRST USE in source
  order (the function definitions' order; a use that is folded away does not reserve a place; an inlined static's
  literal counts where the static is defined). GFL_ASSERT's "" and the file name are such literals. A string literal
  inside an initializer (a `const char *` table) is its own pool entry and FLUSHES the pool: a "" used after it is a
  second "" object. Two "" used by different functions with nothing like that between are two files (colosseum.c and
  union_oneself.c were).

Our list comes from our object: its objects, the locals in @N (parse) order, each named object where the C defines it
among the functions that use the locals. --check says whether that list reproduces our object, as pokebw2's --check
does: a verdict means nothing until it does (an object missing from the list, or a helper defined away from its
caller). Black's order is the file's listing, each object named by ours of its bytes (relocated words aside), else by
size. The moves are the fewest objects to declare elsewhere: a named one's definition, or a local, put in a static
inline helper defined there. Failing that it tries one of ours left out (another file's, a boundary fix) and one
object more that the link drops (a dead function's initializer, which needs a dead-stripping link).
"""
import argparse
import difflib
import glob
import itertools
import math
import os
import re
import subprocess
import sys
import tempfile
import time
from collections import namedtuple

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import functools  # noqa: E402
from file_ranges import module_dir  # noqa: E402
from port_file import read_names as _read_names, module_path, read_object  # noqa: E402

read_names = functools.lru_cache(maxsize=None)(_read_names)  # the module's names: read once (glued() runs over every file)


@functools.lru_cache(maxsize=None)
def slurp(path, binary=False):
    """A module's symbols.txt, relocs.txt or ROM bytes, read once (glued() runs over every file)."""
    with open(path, "rb" if binary else "r") as f:
        return f.read()

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FLAGS = ("-O4,p -proc arm946e -thumb -interworking -enum int -char signed -fp soft -lang=c99 -Cpp_exceptions off "
         "-gccext,on -gccinc -inline on,noauto -ipa file -sym on -requireprotos -nolink -msgstyle gcc").split()
INCLUDES = [a for d in ["include", *sorted(os.path.relpath(p, ROOT) for p in glob.glob(f"{ROOT}/lib/*/include")), "build"]
            for a in ("-i", d)]  # configure.py's INCLUDE_DIRS (lib/msl/include has stdarg.h)
SEPARATE = 64  # bytes: an object this big gets a section of its own (with -ipa file; MODE has the file's rule)
MAX_MOVES = 2
MAX_UNITS = 9  # permute() tries every order of this many definitions at most (9! = 362880 orders, about 15 s)
TIME_LIMIT = 300  # seconds the search for moves runs at most
KEEP = bool(os.environ.get("DATA_ORDER_KEEP"))  # --apply leaves the moved C when fit_data.py then fails (debugging)

# An object of our data: own (a section of its own), its bytes and their relocated offsets, the function or object
# that refers to it, its section, and pooled (a string only code uses, not in the sorted list)
Obj = namedtuple("Obj", "name size own data masked user section pooled")

# The file's compiler's rules (set_mode(), from configure.py's compiler_of: library.toml, compiler_flags.toml).
# "separate": an object this big gets a section of its own: dsi/* with -ipa file (and 2.0/sp2p3's, which gives byte
# arrays one too); MWCC 2.0/sp1p2 gives one to each byte array and string of any size (a local's initializer too)
# and none by size (atermset.c's 1024-byte AES tables share one), dsi/1.1p1 without -ipa file none at all (which
# objects are alone in a section our object says, and moves don't change). "ipa": with "file" MWCC sorts the whole
# list when the file ends; without -ipa file ("function") only the objects defined above the first function that
# isn't inline, the others are laid out as they are listed, and the tentative definitions (listed last, the last
# one first) aren't sorted (gsPlatformUtil.c's .bss statics, laid out the last defined first; random files of each
# compiler and mode, none off), but for the first function's own locals, which are sorted with them. "first": the end
# of the first function, FIRST's place in the C (parse_order()), where a function-mode list is cut. "defer": are
# static const scalars listed at the end (not by 1.2/base, which sorts the whole list; its ties are 95% right)?
MODE = {"separate": SEPARATE, "ipa": "file", "compiler": "dsi/1.1p1", "first": None, "defer": True}
FIRST = Obj("(the first function)", 0, True, b"", set(), None, "", False)  # in the list without -ipa file, at it


GIVEN = {}  # {"compiler": ..., "flags": [...]}: what set_mode() takes instead of the file's (tests, --compile)


def set_mode(path, compiler=None, flags=None):
    """MODE for a source file (or the compiler and flags given, or GIVEN's)."""
    if compiler is None and GIVEN:
        compiler, flags = GIVEN["compiler"], GIVEN["flags"]
    if compiler is None:
        compiler, flags = "dsi/1.1p1", ["-ipa file"]
        try:
            sys.path.insert(0, ROOT)
            from configure import compiler_of
            compiler, flags = compiler_of(path)
        except Exception:
            pass
    flags = " ".join(flags).split()
    ipa = "file" if compiler.startswith("1.2/") else "function"  # (1.2/base sorts the whole list)
    for a, b in zip(flags, flags[1:]):
        if a == "-ipa":
            ipa = "file" if b in ("file", "program") else "function"
    MODE.update(ipa=ipa, compiler=compiler, first=None, defer=not compiler.startswith("1.2/"),
                separate=SEPARATE if ipa == "file" and not compiler.startswith("2.0/sp1") else math.inf)


def heapsort(items, key):
    """MWCC's sort of a section's objects: textbook heap sort, max-heap, strict >."""
    a = list(items)
    n = len(a)

    def sift(i, n):
        while True:
            l, r, m = 2 * i + 1, 2 * i + 2, i
            if l < n and key(a[l]) > key(a[m]):
                m = l
            if r < n and key(a[r]) > key(a[m]):
                m = r
            if m == i:
                return
            a[i], a[m] = a[m], a[i]
            i = m

    for i in range(n // 2 - 1, -1, -1):
        sift(i, n)
    for end in range(n - 1, 0, -1):
        a[0], a[end] = a[end], a[0]
        sift(0, end)
    return a


def emitted(listed):
    """The indices of the list in the order MWCC lays out its objects (each section's shared ones then together):
    with -ipa file the heapsort of the whole list from its last object; without, the heapsort of those listed before
    FIRST (not the tentative definitions nor the deferred scalars), then the others as listed."""
    idx = list(range(len(listed)))
    if MODE["ipa"] == "file":
        return heapsort(idx[::-1], key=lambda i: listed[i].size)
    cut = next((i for i in idx if listed[i].name == FIRST.name), len(listed))
    early = [i for i in idx[:cut] if listed[i].name not in TENTATIVE and listed[i].name not in DEFERRED]
    sort = set(early)
    return heapsort(early[::-1], key=lambda i: listed[i].size) + [i for i in idx if i not in sort and i != cut]


def own(o):
    """Does the object get a section of its own?"""
    return o.own or o.size >= MODE["separate"]


def layout(listed, section=".rodata"):
    """The order MWCC lays out a section's objects from the list in parse order (each with .size, .own and .section):
    the sorted list, each section's shared objects together where its smallest one comes."""
    sections, shared = [], None
    for o in (listed[i] for i in emitted(listed)):
        if o.section != section:
            continue
        if own(o):
            sections.append([o])
        elif shared is None:
            shared = [o]
            sections.append(shared)
        else:
            shared.append(o)
    return [o for s in sections for o in s]


def compile_c(src, incdir=None, flags=None, compiler="dsi/1.1p1"):
    """Compile C text with the game flags: (object bytes, sections, symbols, relocs) as port_file.read_object. incdir:
    the directory of the file it stands for, searched for its local #includes; flags, compiler: others (--compile's
    probes)."""
    with tempfile.TemporaryDirectory() as tmp:
        c, o = os.path.join(tmp, "test.c"), os.path.join(tmp, "test.o")
        open(c, "w").write(src)
        r = subprocess.run([f"{ROOT}/tools/wibo", f"{ROOT}/tools/mwccarm/{compiler}/mwccarm.exe"] + (flags or FLAGS) + INCLUDES
                           + (["-i", incdir] if incdir else []) + ["-o", o, c], capture_output=True, text=True,
                           cwd=ROOT)
        if r.returncode:
            sys.exit(r.stdout + r.stderr)
        return read_object(o)


def compile_flags(path, compiler=None, flags=None):
    """--compile's compiler and flags: those given, else a tree file's own (configure.py's compiler_of(): a library's
    compiler, a file's flags), else the game's."""
    if compiler is None or flags is None:
        own = None
        rel = os.path.relpath(os.path.abspath(path), ROOT)
        if not rel.startswith(".."):
            try:
                sys.path.insert(0, ROOT)
                from configure import compiler_of
                own = compiler_of(rel)
            except Exception:  # noqa: BLE001 (not a file configure.py knows)
                own = None
        compiler = compiler or (own[0] if own else "dsi/1.1p1")
        flags = flags or (" ".join(own[1]).split() if own else list(FLAGS))
    return compiler, flags


def data_objects(obj, section):
    """The object's data of one section name in layout order: [Obj]. A local's user is the function that loads it, an
    initializer's string the object it is in; a string only code uses (pooled) isn't in the sorted list."""
    d, secs, syms, relocs = obj
    defs = [(i, v & ~1, s, n, f) for n, i, v, s, g, f in syms if s and n and not n.startswith(".")]
    users = {}  # (section, offset) -> (the function or object that refers to it, is it a function)
    for i, rows in relocs.items():
        for off, _, s, add in rows:
            for di, start, size, name, f in defs:
                if di == i and start <= off < start + size:
                    users.setdefault((syms[s][1], syms[s][2] + add), (name, f))
                    if f and syms[s][0] and not syms[s][0].startswith("."):
                        SITES.setdefault(syms[s][0], set()).add((name, off - start, add))
    out = []
    for i, (name, kind, off, size, align) in enumerate(secs):
        if name == section and kind in (1, 8):
            here = sorted((s for s in syms if s[1] == i and s[0] and not s[0].startswith(".") and s[3] and not s[5]),
                          key=lambda s: s[2])
            masked = {o + k for o, _, _, _ in relocs.get(i, ()) for k in range(4)}
            for n, _, v, sz, _, _ in here:
                TARGETS[n] = {o - v: (syms[t][0], add) for o, _, t, add in relocs.get(i, ()) if v <= o < v + sz}
                user, code = users.get((i, v), (None, False))
                # a local initializer that holds pointers is in .data too, but it is listed: a string has none
                out.append(Obj(n, sz, len(here) == 1, d[off + v:off + v + sz] if kind == 1 else
                               b"", {k - v for k in masked if v <= k < v + sz}, user, section,
                               section != ".rodata" and n.startswith("@") and code and not TARGETS[n]))
    return out


# and a TwlSDK LTD file's .ltdmain, where twl/ltdmain_begin.h sends its initialized data with its code: MWCC lists those
# objects with the others (mi_sharedWram_twl.c's four tables order its .bss; without them the model was off)
SECTIONS = (".rodata", ".data", ".bss", ".ltdmain")
TENTATIVE = set()  # the names parse_order() found defined without an initializer (listed at the end)
DEFERRED = {}  # the static const scalars parse_order() found: {name: rank}, listed after the tentative ones by rank
INIT = {"on": False}  # --apply's search: do the definitions without an initializer that move get one?
POS = {}  # object: (offset in the C, delta) where parse_order() lists it
TARGETS = {}  # object: {offset in it: (the symbol a relocation there points to, its addend)}, from data_objects()
SITES = {}  # object: {(function, offset in it, addend)} of the code words that point to it, from data_objects()


DROPPED = set()  # our objects the link drops (link_drops()): sorted with the others, gone from Black's layout


def link_drops(path, obj):
    """Our objects the dead-stripping link drops, as tools/dead_strip.py has it link: what nothing kept reaches, in a
    section of its own (a dropped function's local static tables and their strings, its pooled strings: with dsi/1.1p1
    each is a section, so dwc_mov_wh.c's GetWMStateCodeName's 46 strings go with it). Kept are the functions Black's
    symbols.txt has, every global datum, and what they reach. MWCC listed and sorted them all, so they move the others
    as if they stayed; only Black's layout lacks them. Sets DROPPED, returns it."""
    DROPPED.clear()
    d, secs, syms, relocs = obj
    where = read_names(f"{ROOT}/config/b_us")[1]
    mod = module_of(path)
    funcs = {n for n, i, v, sz, g, f in syms if f and n and sz}
    data = {n: (i, g) for n, i, v, sz, g, f in syms if not f and n and sz and not n.startswith(".")
            and secs[i][0] in SECTIONS}
    if not any(mod not in {m for m, _ in where.get(f, ())} for f in funcs):
        return DROPPED
    reach = {}
    for target, users in object_users(obj).items():
        for u in users:
            reach.setdefault(u, set()).add(target)
    sec = {n: i for n, i, v, sz, g, f in syms if n and sz and not n.startswith(".")}
    members = {}
    for n, i in sec.items():
        members.setdefault(i, []).append(n)
    # the link keeps whole sections: what a kept one holds is kept, and what that reaches (a table a dropped
    # function reads that shares its section with a kept one keeps its strings: sakeRequestModify.c's)
    todo = [f for f in funcs if mod in {m for m, _ in where.get(f, ())}] + [n for n, (_, g) in data.items() if g]
    kept = set()
    while todo:
        n = todo.pop()
        if n in kept:
            continue
        kept.add(n)
        todo += [t for t in reach.get(n, ()) if t not in kept] + [m for m in members.get(sec.get(n), ()) if m not in kept]
    DROPPED.update(n for n in data if n not in kept)
    return DROPPED


def linked(listed, section):
    """layout() without what the link drops: Black's layout of the list."""
    return [o for o in layout(listed, section) if o.name not in DROPPED]


def listed_objects(obj):
    """The objects of .rodata, .data and .bss that MWCC sorts in one list (not code's pooled strings)."""
    return [o for sec in SECTIONS for o in data_objects(obj, sec) if not o.pooled]


_MODULES = {}


def module_of(path):
    """The module (ovN or main) a source file is linked in: src/ovN/ files are ovN's, a library file (lib/nitrowifi's
    cps.c is in ov009) the module whose delinks.txt has an entry for it."""
    m = re.search(r"/(ov\d+)/", path)
    if m:
        return m[1]
    if path not in _MODULES:
        _MODULES[path] = "main"
        for d in sorted(glob.glob(f"{ROOT}/config/b_us/arm9/overlays/ov*/delinks.txt")
                        + glob.glob(f"{ROOT}/config/b_us/arm9/*/delinks.txt")):  # and the autoloads' (TwlSDK's LTD)
            if re.search(rf"^{re.escape(path)}:", open(d).read(), re.M):
                _MODULES[path] = os.path.basename(os.path.dirname(d))
                break
    return _MODULES[path]


def fills(size, slot, addr=0):
    """Does an object of size bytes at addr fill a listing label's slot (up to the next object's 4-alignment)?"""
    return size <= slot <= -(-(addr + size) // 4) * 4 - addr


def pointed(relocs):
    """{address: the address its relocation points at} of a relocs.txt: the symbol's plus the addend (`to:<table>
    add:N`, tools/inner_symbols.py's), which atermset.c's lpElementData (&ElementData[2]) needs to be Black's."""
    return {int(fr, 16): int(to, 16) + int(add or "0", 16) for fr, to, add in re.findall(
        r"^from:0x(\w+) kind:\S+ to:0x(\w+)(?: add:(-?0x\w+))?", relocs, re.M)}


# (file, section): (where Black's objects start, the range's start) when black_objects() found ours before the range
EARLY = {}


def black_objects(path, section, ours):
    """Black's objects of the file's section, in address order: [(label, size, {our names it can be})], an object ours
    of its size and bytes (relocated words aside), else any of ours of its size. They are the module's symbols from the
    start of the file's range in delinks.txt, over the range or as many bytes as ours if more (a range that is short
    would leave Black's list incomplete), and before it those of ours in no file's range (EARLY). A label runs to the
    next one; neighbours that add up to one of ours are one object (zukan_open.c's proc table)."""
    mod = module_of(path)
    config = f"{ROOT}/config/b_us/arm9/{module_dir(mod)}"
    text = open(f"{config}/delinks.txt").read()
    m = re.search(re.escape(path) + r":\n(?:    .*\n)*?    " + re.escape(section) + r"\s+start:0x(\w+) end:0x(\w+)", text)
    if not m:
        return []
    start, end = int(m[1], 16), int(m[2], 16)
    short = sum(o.size for o in ours) > end - start  # ours don't fit: the range may start late too
    end, stop = max(end, start + sum(o.size for o in ours)), end
    names = {int(a, 16): n for n, a in re.findall(r"^(\S+) kind:(?:data|bss)\S* addr:(0x\w+)",
                                                  slurp(f"{config}/symbols.txt"), re.M)}
    syms = sorted(names)
    labels = [(names[a], a) for a in syms if start <= a < end]
    rom = slurp(f"{ROOT}/{module_path(mod)}", True)
    base = int(re.search(r"^\s+\.\w+\s+start:0x(\w+)", text, re.M)[1], 16)  # the module's first section
    biggest = max(o.size for o in ours)

    # Black's relocations from the range, and where Black has each name, to tell apart objects of pointers
    black_to = pointed(slurp(f"{config}/relocs.txt"))
    where = read_names(f"{ROOT}/config/b_us")[1]

    def same_bytes(o, addr, size):
        theirs = rom[addr - base:addr - base + size]
        if not (o.data and len(theirs) >= o.size and all(k in o.masked or o.data[k] == theirs[k]
                                                         for k in range(o.size))):
            return False
        for off, (target, add) in TARGETS.get(o.name, {}).items():
            to, places = black_to.get(addr + off), where.get(target, [])
            if to is None:
                return False  # a pointer of ours where Black has none
            if places and all((a + (add if not target.startswith(".") else 0)) & ~1 != to & ~1
                                                 for _, a in places):
                return False
        return True

    def refine(names, addr):
        """Of several of ours that fit a label (equal bytes), those whose code words are where Black's relocations put
        a pointer to it: a function's k-th pool word to this label. The word at the same offset in Black's function
        (ours is that function's size, or it doesn't match) points to another label of a datum of ours: not it. So too
        the pointers of our named data (TARGETS): a table's k-th word, at its Black name's address (btlv_input.c's
        twin s32[7] tables, which a button set's pointer told apart)."""
        if len(names) < 2:
            return names
        score = {}
        for o in names:
            ok = bad = 0
            sites = list(SITES.get(o.name, ())) + [(x, off, add) for x, offs in TARGETS.items() if not x.startswith("@")
                                                   for off, (t, add) in offs.items() if t == o.name]
            for func, rel, add in sites:
                base = [a for m, a in where.get(func, []) if m == mod]
                to = black_to.get(base[0] + rel) if len(base) == 1 else None
                if to is not None:
                    if (to - add) & ~1 == addr & ~1:
                        ok += 1
                    else:
                        bad += 1
            score[o.name] = (bad, -ok)
        best = min(score.values())
        return [o for o in names if score[o.name] == best]

    if section == ".bss":
        # no bytes to tell the objects apart by: each label by the size of its slot and the code words that point to
        # it. Only when every object of ours has a label: an -ipa file reaches its statics from one base (the labels
        # are the few that something else loads) and Black's order of the others is in its code's offsets
        out = []
        for k, (name, addr) in enumerate(labels):
            slot = (labels[k + 1][1] if k + 1 < len(labels) else end) - addr
            fit = refine([o for o in ours if fills(o.size, slot, addr) or k == len(labels) - 1 and o.size <= slot],
                         addr)
            if not fit:
                return []
            out.append((name, min(o.size for o in fit), {o.name for o in fit}))
        return out if len(out) == len(ours) else []

    def cover(a, stop, seen=None):
        """Our objects by their bytes one after another from a up to stop (the next label, or the range's end, which
        zeros may pad), each where the one before it ends or at its 4-alignment: [(address, size)], or None."""
        seen = {} if seen is None else seen
        if a == stop or (stop - a < 4 and stop % 4 == 0 or stop == end) and not any(rom[a - base:stop - base]):
            return []
        if a not in seen:
            seen[a] = None
            for size in sorted({o.size for o in ours if a + o.size <= stop and same_bytes(o, a, o.size)}, reverse=True):
                for nxt in sorted({a + size, -(-(a + size) // 4) * 4}):
                    rest = cover(nxt, stop, seen) if nxt <= stop else None
                    if rest is not None and seen[a] is None:
                        seen[a] = [(a, size)] + rest
        return seen[a]

    # objects in no file's range just before it, when ours don't fit the range: it starts late (t37e:
    # worldtrade_deposit.c's country count, un_select.c's two folded u8 statics showed as another file's). Ours by
    # their bytes, from the earliest label (or where the file before it ends, or the module's section starts) from
    # which they cover the bytes up to the range
    if short:
        spans = [(int(a, 16), int(b, 16)) for a, b in re.findall(
            r"^\s+" + re.escape(section) + r"\s+start:0x(\w+) end:0x(\w+)", text, re.M)]
        lo = max([b for _, b in spans if b <= start] + [min(a for a, _ in spans)])
        c = next((c for c in sorted({lo} | {a for a in syms if lo <= a < start}) if cover(c, start)), None)
        if c is not None:
            EARLY[path, section] = (c, start)
            start, end = c, max(stop, c + sum(o.size for o in ours))
            labels = [(names[a], a) for a in syms if start <= a < end]

    # objects before the range's first label, which nothing relocates (code that folds a read of them, or reads them
    # from a base it computes): ours by their bytes
    first = labels[0][1] if labels else end
    chain = cover(start, first) if first > start else None
    labels[:0] = [(f"{a:#x}", a) for a, _ in chain or ()]

    out, i = [], 0
    while i < len(labels):
        name, addr = labels[i]
        edge = [a for _, a in labels[1:]] + [end]

        # the labels from here that make up one of our objects with its bytes, else of its size, else this one alone
        def matches(j):
            slot = edge[j] - addr
            # the range's last label runs to its end, which can be the module section's padding
            fit = [o for o in ours if fills(o.size, slot, addr) or j == i == len(labels) - 1 and o.size <= slot]
            return fit, [o for o in fit if same_bytes(o, addr, slot)]
        upto = next((j for j in range(i, len(labels)) if edge[j] - addr > biggest), len(labels) - 1)
        j = next((j for j in range(i, upto + 1) if matches(j)[1]), None)
        # several of ours by their bytes, unless one of them fills the label without padding
        chain = cover(addr, edge[i]) if j is None or all(o.size < edge[j] - addr for o in matches(j)[1]) else None
        if chain and len(chain) > 1:
            # a label over several of ours, which no relocation told apart, each by its bytes
            for a, size in chain:
                out.append((name if a == addr else f"{name}+{a - addr:#x}", size,
                            {o.name for o in refine([o for o in ours if o.size == size and same_bytes(o, a, size)], a)}))
            i += 1
            continue
        if j is None:
            j = next((j for j in range(i, upto + 1) if matches(j)[0]), i)
        fit, same = matches(j)
        if not fit:
            # a label over several of ours, which no relocation told apart: the first of them by its bytes, and a
            # label of its own for the rest
            first = [o for o in ours if o.size < edge[i] - addr and same_bytes(o, addr, o.size)]
            if first:
                size = min(o.size for o in first)
                out.append((name, size, {o.name for o in refine([o for o in first if o.size == size], addr)}))
                labels.insert(i + 1, (f"{name}+{-(-size // 4) * 4:#x}", addr + -(-size // 4) * 4))
                i += 1
                continue
        out.append((name, min(o.size for o in same or fit) if fit else edge[j] - addr,
                    {o.name for o in refine(same or fit, addr)}))
        i = j + 1
    return out


LIKE = {}  # {"rev": a git revision}: --like, the layout of the C as it was there stands for Black's where Black has none


def like_targets(path, section, ours):
    """Black's objects of a section as the C at --like REV laid it out (the last build that matched): [(label, size,
    {our names})] in that object's order, an object by its name (a function's static by the variable's), or []."""
    rev = LIKE.get("rev")
    if not rev or not ours:
        return []
    r = subprocess.run(["git", "show", f"{rev}:{path}"], cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        return []
    import data_fix  # (it imports this module): the file's own compile command, for the old text
    ok, out, _, old = data_fix.compile_text(path, r.stdout, want_object=True)
    if not ok:
        return []
    LIKE["obj"] = old
    before = [o for o in data_objects(old, section) if not o.pooled]

    def base(name):
        ls = local_static(name)
        return ls[1] if ls else name
    mine = {base(o.name): o for o in ours if not o.pooled}
    out = [(o.name, mine[base(o.name)].size, {mine[base(o.name)].name}) for o in before if base(o.name) in mine]
    return out if len(out) == len([o for o in ours if not o.pooled]) else []


def black_targets(path, section, ours):
    """Black's objects of a section for the search ([] if unknown): its symbols' (sorted_run), else --like's."""
    return sorted_run(black_objects(path, section, ours), ours) or like_targets(path, section, ours)


def strings_unlike(path, section, ours):
    """The strings code uses (pooled) of ours that Black's module lacks: [(ours, the likeliest of Black's in the file's
    range of the section)]. An assert's text is its expression as written: a name or a space spelt otherwise than in
    Black's C changes it."""
    mod = module_of(path)
    config = f"{ROOT}/config/b_us/arm9/{module_dir(mod)}"
    text = open(f"{config}/delinks.txt").read()
    m = re.search(re.escape(path) + r":\n(?:    .*\n)*?    " + re.escape(section) + r"\s+start:0x(\w+) end:0x(\w+)", text)
    if not m:
        return []
    start, end = int(m[1], 16), int(m[2], 16)
    rom = slurp(f"{ROOT}/{module_path(mod)}", True)
    base = int(re.search(r"^\s+\.\w+\s+start:0x(\w+)", text, re.M)[1], 16)  # the module's first section
    black = [x.decode("latin-1") for x in re.findall(rb"[\x20-\x7e]{2,}", rom[start - base:end - base])]
    out = []
    for o in ours:
        mine = o.data.split(b"\0")[0] if o.pooled and o.data else None
        if mine and mine + b"\0" not in rom:
            mine = mine.decode("latin-1")
            like = difflib.get_close_matches(mine, black, 1, 0.5)
            out.append((mine, like[0] if like else None))
    return out


def object_users(obj):
    """{object name: [the functions and objects whose relocations reach it]} of an object file."""
    d, secs, syms, relocs = obj
    defs = [(i, v & ~1, s, n) for n, i, v, s, g, f in syms if s and n and not n.startswith(".")]
    out = {}
    for i, rows in relocs.items():
        for off, _, s, add in rows:
            target = (syms[s][1], syms[s][2] + add)
            name = next((n for di, start, size, n in defs if (di, start) == target and size), None) or \
                (syms[s][0] if syms[s][0] and not syms[s][0].startswith(".") else None)
            for di, start, size, n in defs:
                if di == i and start <= off < start + size and name and n not in out.get(name, ()):
                    out.setdefault(name, []).append(n)
    return out


@functools.lru_cache(maxsize=8)  # (a loop over a file's functions asks for the same text's again and again)
def blank(text):
    """The C with its comments and the insides of its string and character literals blanked (offsets kept)."""
    out, i, n = list(text), 0, len(text)
    while i < n:
        if text.startswith("//", i) or text.startswith("/*", i):
            j = text.find("\n", i) if text[i + 1] == "/" else text.find("*/", i + 2)
            j = n if j < 0 else j if text[i + 1] == "/" else j + 2
            out[i:j] = [c if c == "\n" else " " for c in text[i:j]]
            i = j
        elif text[i] in "\"'":
            j = i + 1
            while j < n and text[j] != text[i]:
                j += 2 if text[j] == "\\" else 1
            out[i + 1:j] = " " * (min(j, n) - i - 1)
            i = j + 1
        else:
            i += 1
    return "".join(out)


def function_bodies(code):
    """{function: (definition, body start, body end)} of the C's function definitions (offsets)."""
    out = {}
    for m in re.finditer(r"^\w[\w \t\*]*?\b(\w+)\s*\([^;{]*\)\s*\{", code, re.M):
        depth, i = 0, m.end() - 1
        while i < len(code):
            depth += {"{": 1, "}": -1}.get(code[i], 0)
            if depth == 0:
                break
            i += 1
        out.setdefault(m[1], (m.start(), m.end(), i))
    return out


DOLLAR = {}  # {name: (function, variable, offset)} of the C's local statics MWCC 2.0 names <variable>$<N>


def dollar_statics(ours, code, bodies):
    """Fills DOLLAR: the library compilers name a function's static `objs$2448` (N counts in parse order), not
    _ZZ...: each by its variable's declarations in the functions (static, or a local const), the k-th lowest N the
    k-th in the C (nsbma.c's cmask0$2490 and cmask0$2502; g2d_CharCanvas.c's objs and shape were listed first)."""
    DOLLAR.clear()
    by_var = {}
    for o in ours:
        m = re.fullmatch(r"(\w+)\$(\d+)", o.name)
        if m:
            by_var.setdefault(m[1], []).append((int(m[2]), o.name))
        elif not o.name.startswith(("@", "_ZZ")) and not re.search(rf"^(?!extern\b)\w[^;\n(]*\b{re.escape(o.name)}\b",
                                                                    code, re.M):
            by_var.setdefault(o.name, []).append((0, o.name))  # os_thread.c's OSi_ThreadIdCount: its own name
    for var, objs in by_var.items():
        for kw in (r"static", r"static|const"):  # (a static first: a local const of the name elsewhere is a value)
            found = sorted((x.start(), f) for f, (_, b0, b1) in bodies.items()
                           for x in re.finditer(rf"\b(?:{kw})\b[^;{{}}]*\b{re.escape(var)}\b", code[:b1])
                           if x.start() >= b0)
            if len(found) == len(objs):
                for (_, name), (at, f) in zip(sorted(objs), found):
                    DOLLAR[name] = (f, var, at)
                break


def local_static(name):
    """(function, variable) of a function-local static's mangled name (_ZZ<n><function><params>E<n><variable>[_<k>]),
    or of one MWCC 2.0 names <variable>$<N> (DOLLAR), or None."""
    if name in DOLLAR:
        return DOLLAR[name][:2]
    m = re.match(r"_ZZ(\d+)", name)
    if not m:
        return None
    func = name[m.end():m.end() + int(m[1])]
    for e in re.finditer(r"E(\d+)", name):
        var = name[e.end():e.end() + int(e[1])]
        if len(var) == int(e[1]) and re.fullmatch(r"(_\d+)?", name[e.end() + int(e[1]):]):
            return func, var
    return None


def local_initializers(code, body):
    """The offsets of a function body's local initializers that MWCC makes objects of: `= { ... }` not all zeros, and
    a char array's `= "..."`, not static."""
    start, end = body
    out = []
    for m in re.finditer(r"(?<![=!<>+\-*/%&|^])=\s*(\{|\")", code[start:end]):
        stmt = code[start:start + m.start()].rsplit(";", 1)[-1].rsplit("{", 1)[-1].rsplit("}", 1)[-1]
        if re.search(r"\bstatic\b", stmt):
            continue
        if m[1] == "{":
            depth, i = 0, start + m.end() - 1
            while i < end:
                depth += {"{": 1, "}": -1}.get(code[i], 0)
                if depth == 0:
                    break
                i += 1
            if re.fullmatch(r"[\s{},0]*|[\s{},]*(0|NULL|FALSE)([\s{},]*(0|NULL|FALSE))*[\s{},]*", code[start + m.end():i]):
                continue
        out.append(start + m.start())
    return out


def declares(code, m):
    """Is the name a definition's match m found (its group n) the declarator's own, not a parameter of a function
    pointer's declarator? natneg.c's `static void (*natifyCallback)(u32 success, NAT nat);` was nat's definition, which
    listed it with natifyCallback: the model laid out its .bss so (t37a)."""
    prefix = code[m.start():m.start("n")]
    depth = prefix.count("(") - prefix.count(")")
    return depth == 0 or depth == 1 and re.search(r"\(\s*\*\s*(?:const\s+)?$", prefix) is not None


def parse_order(ours, path, users=None, text=None):
    """Our objects in the order the compiler listed them: those named @N (locals, an initializer's strings) by N, each
    named object (a file-level one where it is initialized, a local static where it is declared) before the first of
    them declared after it; then the tentative definitions, the last one first. users ({object: [its users]},
    object_users()) places an initializer's string with the first object that uses it. text is the C (else the
    file's). Each object's place in the C goes in POS. Returns (list, notes)."""
    text = open(f"{ROOT}/{path}").read() if text is None else text
    set_mode(path)
    code = blank(text)
    bodies = function_bodies(code)
    at = {f: b[0] for f, b in bodies.items()}
    MODE["first"] = min((b for b in bodies.values() if not re.search(
        r"\binline\b", code[b[0]:b[1]])), default=(None,) * 3)[2]
    notes, tentative, deferred = [], [], []
    statics = {}  # function: [its local statics' offsets]
    scalars = {}  # function: [(offset, name)] of its local const scalars (listed with the deferred ones)
    dollar_statics(ours, code, bodies)
    for o in ours:
        if o.name.startswith("@"):
            continue
        name = re.escape(o.name)
        ls = local_static(o.name)
        if ls and ls[0] in bodies:
            _, b0, b1 = bodies[ls[0]]
            m = re.compile(rf"\b(static|const)\b[^;{{}}]*\b{re.escape(ls[1])}\b").search(
                code, DOLLAR[o.name][2] if o.name in DOLLAR else b0, b1)
            at[o.name] = m.start() if m else b0
            if m and MODE["defer"] and local_scalar(code, m, b1):
                scalars.setdefault(ls[0], []).append((m.start(), o.name))
                continue
            statics.setdefault(ls[0], []).append(at[o.name])
            continue
        # (a struct defined with it ends a line before: `} name = {`; not a function's closing brace lines above, which
        # made btlv_input.c's data_ov094_0220a3d4 the function's: --apply said it "can't move")
        m = next((x for x in re.finditer(rf"^(?:(?!extern\b)(?!typedef\b)\w|\}}[ \t]*)[^;\n=]*\b(?P<n>{name})\b"
                                         rf"[^;\n=]*=", code, re.M) if declares(code, x)), None)
        if not m:
            # a later declarator of a list, on a line of its own (`static const double bp[] = {...},` then
            # `    dp_h[] = {...},`: e_pow.c's tables), or an indented definition (os_thread.c's `    static int
            # OSi_ThreadIdCount = 0;` in an #ifdef), outside the functions
            m = next((x for x in re.finditer(rf"^[ \t]*(?:\}}[ \t]*,[ \t]*|(?!extern\b)(?!typedef\b)\w[^;\n=]*?\b)?"
                                             rf"(?P<n>{name})\b[^;\n=]*=", code, re.M)
                      if not any(b[1] <= x.start() < b[2] for b in bodies.values()) and declares(code, x)), None)
        if m:
            at[o.name] = m.start()
            if o.section == ".rodata" and MODE["defer"] and scalar_const(code, m, o.name):
                deferred.append(o.name)
            continue
        # without an initializer (a function pointer's declarator has parentheses; a prototype has them right after)
        m = next((x for x in re.finditer(rf"^(?:(?!extern\b)(?!typedef\b)\w|\}}[ \t]*)[^;=\n{{}}]*\b(?P<n>{name})\b"
                                         rf"(?!\s*\()[^;=\n{{}}]*;", code, re.M) if declares(code, x)), None)
        if m:
            at[o.name] = m.start()
            tentative.append(o)
            continue
        at[o.name] = -1
        notes.append(f"{o.name}'s definition isn't in the C: listed first")
    # Where the locals are: when the C's local initializers are as many as our locals (by N, the parse order), each
    # where it is written, which puts a static inline helper's where the helper is; else, for a function with local
    # statics to go between its locals, each of its own
    locals_at = {}
    funcs = set(bodies)
    mine = sorted((o for o in ours if o.name.startswith("@") and o.user in funcs and (o.section == ".rodata" or
                                                                                     TARGETS.get(o.name))),
                  key=lambda o: int(o.name[1:]))
    found = sorted(x for f in bodies for x in local_initializers(code, bodies[f][1:]))
    if len(found) != len(mine):
        # MWCC 2.0 with -ipa file reaches a section's objects from its start, so a local's load names none: when
        # the C has as many local initializers as the object has locals, the N order is theirs
        alone = sorted((o for o in ours if o.name.startswith("@") and o.user is None and (
            o.section == ".rodata" or TARGETS.get(o.name))), key=lambda o: int(o.name[1:]))
        if alone and len(found) == len(mine) + len(alone):
            mine = sorted(mine + alone, key=lambda o: int(o.name[1:]))
    if mine and len(found) == len(mine):
        locals_at.update(zip((o.name for o in mine), found))
    for f, offs in statics.items() if not locals_at else ():
        mine = [o for o in ours if o.name.startswith("@") and o.user == f and o.section == ".rodata"]
        found = local_initializers(code, bodies[f][1:])
        if len(found) == len(mine):
            locals_at.update(zip(sorted((o.name for o in mine), key=lambda n: int(n[1:])), found))
        else:
            notes.append(f"{f}: {len(mine)} local initializer(s), {len(found)} in its C: its local statics listed "
                         "after them")

    by_name = {o.name: o for o in ours}

    def anchor(o, depth=0):
        """(offset in the C, delta): where a local is listed."""
        if o.name in locals_at:
            return locals_at[o.name], 0
        firsts = [at[u] for u in (users or {}).get(o.name, [o.user]) if u in at and u not in bodies] if users else []
        if firsts:
            return min(firsts), -0.5  # an initializer's string: just before the first object that has it
        if o.user in by_name and o.user.startswith("@") and depth < 3:
            return anchor(by_name[o.user], depth + 1)  # a string of a local initializer (of pointers): with it
        return (at[o.user], 0) if o.user in at else None

    POS.clear()
    POS.update({o.name: (at[o.name], 0) for o in ours if not o.name.startswith("@")})
    if MODE["first"] is not None:
        POS[FIRST.name] = (MODE["first"], -1)
    pos = (-1, 0)  # the locals in N order, each where it is, or at the last one's place if before it
    for o in sorted((o for o in ours if o.name.startswith("@")), key=lambda o: int(o.name[1:])):
        a = anchor(o)
        pos = a if a is not None and sum(a) > sum(pos) else pos
        POS[o.name] = pos
    TENTATIVE.clear()
    TENTATIVE.update(o.name for o in tentative)
    DEFERRED.clear()
    groups, seen = [], set()  # each function's scalars, those an earlier function took the address of not again
    for f, (_, b0, b1) in sorted(bodies.items(), key=lambda kv: kv[1][0]):
        uses = sorted((m.start(), n) for n in deferred if n not in seen
                      for m in [re.compile(rf"&\s*(?:\(\s*)*{re.escape(n)}\b").search(code, b0, b1)] if m)
        groups.append([n for _, n in sorted(uses + scalars.get(f, []))])
        seen.update(groups[-1])
    rest = sorted((n for n in deferred if n not in seen), key=lambda n: at[n])
    # (without -ipa file they are laid out unsorted after everything: the reverse of their first `&x`s in the C)
    for n in ([n for g in groups[::-1] for n in g] if MODE["ipa"] == "file" else [n for g in groups for n in g][::-1]
              ) + rest:
        DEFERRED[n] = len(DEFERRED)
    return relist(sorted(ours, key=lambda o: int(o.name[1:]) if o.name.startswith("@") else 0), place), notes


def local_scalar(code, m, end):
    """Is the local m (a match from `static` or `const` to the variable's name, in a body that ends at end) a const
    scalar: `const int left = 0;` or `static const u8 wipeSync = 1;`, the object const (not a pointer to const), no
    array, an initializer that isn't braced? MWCC makes an object of one (`_ZZ...`) even when every read is folded,
    and lists it with the deferred scalars, not where it is declared: each function's after the tentative
    definitions, the functions last first, in the order of the declarations and the deferred scalars' `&x` in it
    (namein.c's eight 4-byte locals of three functions; ctvt_talk.c's, p_sta_sub.c's and plist_sys.c's statics)."""
    head = code[max(code.rfind(c, 0, m.start()) for c in ";{}") + 1:m.end()]
    const = [k.end() for k in re.finditer(r"\bconst\b", head)]
    return bool(const and "*" not in head[const[-1]:] and re.match(r"\s*=(?!\s*[{\x22])", code[m.end():end]))


def scalar_const(code, m, name):
    """Is the definition m (a match from its line's start up to its `=`) a file-level static const scalar: `static`
    and `const` before the name, no array after it, an initializer that isn't braced?"""
    head = code[m.start():m.end()]
    k = re.search(rf"\b{re.escape(name)}\b", head)
    return bool(k and re.search(r"\bstatic\b", head[:k.start()]) and re.search(r"\bconst\b", head[:k.start()])
                and "[" not in head[k.end():] and "(" not in head[:k.start()]
                and not code[m.end():m.end() + 200].lstrip().startswith("{"))


def place(o):
    """An object's place in the C as relist() sorts it: (offset, 1, 0, delta)."""
    at, delta = POS[o.name]
    return at, 1, 0, delta


def relist(ours, key, init=()):
    """The list from each object's place, key(o): the locals (@N, in ours' order where key ties) and the named objects
    in the C's order, a named one before the first local after it; then the definitions without an initializer, the
    last one first (but those of init, which get one)."""
    if MODE["ipa"] != "file" and FIRST.name in POS and all(o.name != FIRST.name for o in ours):
        ours = list(ours) + [FIRST]  # without -ipa file the list is cut at the first function
    tentative = [o for o in ours if o.name in TENTATIVE and o.name not in init]
    late = sorted((o for o in ours if o.name in DEFERRED), key=lambda o: DEFERRED[o.name])
    named = sorted((o for o in ours if not o.name.startswith("@") and o not in tentative and o.name not in DEFERRED),
                   key=key)
    out, k = [], 0
    for o in sorted((o for o in ours if o.name.startswith("@")), key=key):
        while k < len(named) and key(named[k]) < key(o):
            out.append(named[k])
            k += 1
        out.append(o)
    return out + named[k:] + sorted(tentative, key=key, reverse=True) + late


def fits(listed, targets, dropped=None):
    """Does the list lay out each section as Black's objects, {section: [(label, size, {our names})]}, with an object
    the link drops?"""
    for section, target in targets.items():
        lay = [o for o in linked(listed, section) if o is not dropped]
        if len(lay) != len(target) or not all(o.name in names for o, (_, _, names) in zip(lay, target)):
            return False
    return True


def misplaced(listed, targets):
    """How far the list's layout is from Black's: the objects out of place, each section's missing or extra ones
    counted too (0 is Black's order)."""
    bad = 0
    for section, target in targets.items():
        lay = linked(listed, section)
        bad += abs(len(lay) - len(target)) + sum(o.name not in names for o, (_, _, names) in zip(lay, target))
    return bad


class Stopped(Exception):
    """The search ran out of time."""


class Best:
    """The search's clock and the closest order it found: (objects out of place, the move)."""
    def __init__(self, limit):
        self.deadline = time.monotonic() + limit if limit else None
        self.bad, self.way, self.tried = None, None, 0

    def tick(self):
        self.tried += 1
        if self.deadline and self.tried % 256 == 0 and time.monotonic() > self.deadline:
            raise Stopped

    def note(self, bad, way):
        if self.bad is None or bad < self.bad:
            self.bad, self.way = bad, way


def same_run(a, b):
    """Are a and b locals of one function, listed together (a move can't come between them)?"""
    return a.name.startswith("@") and b.name.startswith("@") and a.user and a.user == b.user


def moves(listed, targets, most=MAX_MOVES, named_only=False, best=None):
    """The fewest objects to declare elsewhere for Black's order, each between two top-level definitions (a function's
    locals stay together, in code order; one taken out goes to a helper of its own): [((object, the object listed
    before it then, or None), ...)] for each such way, those that move fewer locals first."""
    n = len(listed)
    movable = [i for i, o in enumerate(listed) if not (named_only and o.name.startswith("@")) and o.name != FIRST.name]
    for k in range(1, most + 1):
        if math.comb(len(movable), k) * (n + 1) ** k * math.factorial(k) > 400000:
            found = near_moves(listed, targets, movable, k, best) if k > 1 else []
            if found:
                return sorted(found, key=lambda way: sum(o.name.startswith("@") for o, _ in way))
            break
        found = []
        for picked in itertools.combinations(movable, k):
            rest = [o for i, o in enumerate(listed) if i not in picked]
            gaps = [p for p in range(len(rest) + 1) if p in (0, len(rest)) or not same_run(rest[p - 1], rest[p])]
            for order in itertools.permutations(picked):
                for places in itertools.product(gaps, repeat=k):
                    new = list(rest)
                    for i, p in sorted(zip(order, places), key=lambda x: x[1], reverse=True):
                        new.insert(p, listed[i])
                    if best:
                        best.tick()
                    bad = misplaced(new, targets) if new != listed else 1
                    if best and new != listed and (best.bad is None or bad < best.bad):
                        best.note(bad, tuple((listed[i], new[new.index(listed[i]) - 1] if new.index(listed[i]) else None)
                                             for i in picked))
                    if new != listed and bad == 0:
                        way = tuple((listed[i], new[new.index(listed[i]) - 1] if new.index(listed[i]) else None)
                                    for i in picked)
                        if way not in found:
                            found.append(way)
        if found:
            return sorted(found, key=lambda way: sum(o.name.startswith("@") for o, _ in way))
    return []


def near_moves(listed, targets, movable, k, best=None, keep=40):
    """moves() for a list too long to try every k moves of: the single moves that bring the list closer, the best
    `keep`, and every k of them together (k = 2 only)."""
    n = len(listed)
    base = misplaced(listed, targets)
    singles = []
    for i in movable:
        rest = listed[:i] + listed[i + 1:]
        for p in range(len(rest) + 1):
            if p == i or 0 < p < len(rest) and same_run(rest[p - 1], rest[p]):
                continue
            new = rest[:p] + [listed[i]] + rest[p:]
            if best:
                best.tick()
            bad = misplaced(new, targets)
            if bad < base:
                singles.append((bad, i, p))
    singles = sorted(singles)[:keep]
    found = []
    for (_, i, p), (_, j, q) in itertools.combinations(singles, 2):
        if i == j:
            continue
        for first, second in (((i, p), (j, q)), ((j, q), (i, p))):
            new = list(listed)
            moved = [listed[first[0]], listed[second[0]]]
            rest = [o for o in new if o not in moved]
            # the two put where each single move put it, in the list without both
            a, b = (listed[first[0]], first[1]), (listed[second[0]], second[1])
            ref = [o for o in listed if o is not a[0]]
            tgt_a = ref[a[1] - 1] if a[1] else None
            ref2 = [o for o in listed if o is not b[0]]
            tgt_b = ref2[b[1] - 1] if b[1] else None
            new = [o for o in listed if o is not a[0] and o is not b[0]]
            for obj, after in ((a[0], tgt_a), (b[0], tgt_b)):
                at = 0 if after is None else new.index(after) + 1 if after in new else None
                if at is None:
                    break
                new.insert(at, obj)
            else:
                if best:
                    best.tick()
                if misplaced(new, targets) == 0:
                    way = tuple((o, new[new.index(o) - 1] if new.index(o) else None) for o in (a[0], b[0]))
                    if way not in found:
                        found.append(way)
    return found


def definitions(listed):
    """The list's top-level definitions, each with its initializer's strings (listed just before it), which move with
    it: [[strings..., object]] in list order. Not the tentative ones and the static const scalars, which are listed at
    the end wherever they are, nor a function's statics."""
    out = []
    for o in listed:
        if (o.name.startswith("@") or o.name in TENTATIVE or o.name in DEFERRED or local_static(o.name)
                or o.name not in POS):
            continue
        at = POS[o.name][0]
        out.append([x for x in listed if x.name.startswith("@") and POS.get(x.name) == (at, -0.5)] + [o])
    return out


def permute(listed, targets, best=None, most=MAX_UNITS):
    """Every declaration order of the file's definitions in the places they have, each with its strings: heapsort's
    ties are set by where every object is listed, so an order of equal sizes that no two moves give can be a whole
    permutation of the definitions (wifibattlematch_net.c's string tables). With more than `most` definitions, those
    that hold an object out of place or one that should be there. Returns (the orders that give Black's, each the
    definitions' last objects in order; (fewest misplaced, its order); the definitions permuted; stopped by the time
    limit). More than `most` of those (it took the first 9 and missed the orders of more) are searched (search_orders()), not tried
    one by one: the orders found are those that give Black's, without the guarantee that none does."""
    units = definitions(listed)
    if len(units) > most:
        wrong = set()
        for section, target in targets.items():
            for o, (_, _, names) in zip(linked(listed, section), target):
                if o.name not in names:
                    wrong |= {o.name} | set(names)
        units = [u for u in units if any(x.name in wrong for x in u)]
    member = {id(x) for u in units for x in u}
    first = {id(u[0]): k for k, u in enumerate(units)}

    def build(perm):
        out = []
        for x in listed:
            if id(x) in first:
                out += units[perm[first[id(x)]]]
            elif id(x) not in member:
                out.append(x)
        return out
    found, closest, stopped = [], (misplaced(listed, targets), list(range(len(units)))), False
    if len(units) > most:
        try:
            perm, bad = search_orders(len(units), lambda p: misplaced(build(p), targets), best)
        except Stopped as e:
            (perm, bad), stopped = e.args[0], True
        if bad == 0:
            found.append([units[k][-1] for k in perm])
        closest = min(closest, (bad, list(perm)))
        return found, (closest[0], [units[k][-1] for k in closest[1]]), [u[-1] for u in units], stopped
    try:
        for perm in itertools.permutations(range(len(units))):
            if best:
                best.tick()
            bad = misplaced(build(perm), targets)
            if bad < closest[0]:
                closest = (bad, list(perm))
            if bad == 0:
                found.append([units[k][-1] for k in perm])
    except Stopped:
        stopped = True
    return found, (closest[0], [units[k][-1] for k in closest[1]]), [u[-1] for u in units], stopped


def search_orders(n, cost, best=None, budget=400000, seed=0):
    """(order, its cost) of the n units nearest cost 0 a simulated annealing finds: runs of 20000 steps, each moving
    one unit to another place or swapping two, a worse order taken by chance as the run cools (heapsort's ties make
    plateaus that a descent sticks on: 10 of 10 random 12-table files in about 4000 orders, 20 tables in 300000), each
    run from the best order so far or the C's; up to `budget` orders, or a Stopped (args: the best so far) at the time
    limit."""
    import random
    rng = random.Random(seed)
    top = (cost(list(range(n))), list(range(n)))
    tried, steps = 1, 20000
    try:
        while top[0] and tried < budget:
            cur = top[1][:] if rng.random() < 0.5 else list(range(n))
            c = cost(cur)
            for step in range(steps):
                heat = 1.5 * (1 - step / steps) + 0.05
                p = cur[:]
                i, j = rng.sample(range(n), 2)
                if rng.random() < 0.5:
                    p.insert(j, p.pop(i))
                else:
                    p[i], p[j] = p[j], p[i]
                if best:
                    best.tick()
                k = cost(p)
                tried += 1
                if k <= c or rng.random() < math.exp((c - k) / heat):
                    cur, c = p, k
                    if c < top[0]:
                        top = (c, cur[:])
                        if not c:
                            break
                if tried >= budget:
                    break
    except Stopped:
        raise Stopped((top[1], top[0]))
    return top[1], top[0]


def order_moves(old, new):
    """The fewest definitions to move to turn the order old into new (those off the longest run new keeps of old):
    [(object, the one it goes before, or None: last)]."""
    rank = [old.index(o) for o in new]
    best = []  # longest increasing subsequence of rank, by indices
    for i in range(len(rank)):
        run = max((r for r in best if rank[r[-1]] < rank[i]), key=len, default=[])
        best.append(run + [i])
    keep = {new[i].name for i in max(best, key=len, default=[])}
    return [(o, new[i + 1] if i + 1 < len(new) else None) for i, o in enumerate(new) if o.name not in keep]


def describe(o, listed):
    if o is None:
        return "the start"
    if o.name == FIRST.name:
        return "the end of the first function (without -ipa file the objects listed up to there are sorted)"
    if o.name.startswith("@") and o.section == ".data":
        return f"{o.user}'s string {o.name} ({o.size} bytes)"
    if o.name.startswith("@"):
        run = [x for x in listed if x.name.startswith("@") and x.user == o.user]
        return f"{o.user}'s local {o.name} ({o.size} bytes, its {run.index(o) + 1} of {len(run)})"
    if o.name in TENTATIVE:
        return f"{o.name} ({o.size} bytes, {o.section}, defined without an initializer: listed at the end)"
    return f"{o.name} ({o.size} bytes{', ' + o.section if o.section != '.rodata' else ''})"


def sorted_run(black, ours):
    """Black's objects of a section that MWCC sorted: those before the strings code uses (which follow them, in the
    order of their first use), as many as ours, each without our names of such strings (a string the code and a table
    both have is two objects, of the same bytes)."""
    pooled = {o.name for o in ours if o.pooled}
    run = [(label, size, names - pooled) for label, size, names in black if not names <= pooled]
    return run[:len(ours) - len(pooled)] if pooled else run


def multiset_orders(items):
    """Each distinct order of items (equal ones aren't told apart), the given order first."""
    if len(items) <= 1:
        yield list(items)
        return
    seen = set()
    for k, x in enumerate(items):
        if x in seen:
            continue
        seen.add(x)
        for rest in multiset_orders(items[:k] + items[k + 1:]):
            yield [x] + rest


def invert(listed, targets, most=20000):
    """The model run backwards: heapsort compares sizes only, so with each slot of the list keeping its size the sort
    is one fixed permutation of the slots, and Black's order is reached by putting each named definition in the slot
    of the equal-size object Black has where it lays out. Only named objects change slots (a local or a string stays),
    a tentative definition only with another. Objects of another section or kind (own section or shared) of the same
    size trade slots too: the layout then groups the slots otherwise (clact.c needed a global in a table's slot and a
    .data table in a .rodata one), so each way to give the slots of a size their objects' kinds is tried, the C's
    first, most of them at most. Returns the new list, or None when some slot needs an object that can't go there
    (the order differs by more than ties: no declaration order gives it)."""
    n = len(listed)
    cut = next((i for i in range(n) if listed[i].name == FIRST.name), n)
    early = lambda o: MODE["ipa"] == "file" or listed.index(o) < cut  # (a slot of the sorted head, without -ipa file)
    order = emitted(listed)  # the slots in sorted order
    free = lambda o: not o.name.startswith("@") and o.name not in DROPPED
    kind = lambda o: (o.section, bool(own(o)))
    # a tentative definition trades places only with another: they are listed last, in the reverse of their
    # definitions' order, so swapping two definitions swaps their slots (nd.c's ten 4-byte .bss words)
    cls = lambda o: (o.size, o.name in TENTATIVE, early(o))
    groups = {}
    for i, o in enumerate(listed):
        if free(o):
            groups.setdefault(cls(o), []).append(i)
    mixed = [(c, slots) for c, slots in groups.items() if len({kind(listed[i]) for i in slots}) > 1]
    tries = itertools.product(*[multiset_orders([kind(listed[i]) for i in slots]) for _, slots in mixed])
    for pattern in itertools.islice(tries, most):
        kinds = {i: kind(o) for i, o in enumerate(listed)}
        for (_, slots), ks in zip(mixed, pattern):
            kinds.update(zip(slots, ks))
        new = assign(listed, targets, order, kinds, free, cls, kind)
        if new is not None:
            return new
    return None


def assign(listed, targets, order, kinds, free, cls, kind):
    """invert() for one way to give the slots kinds ({slot: (section, own)}): each class of interchangeable slots, a
    bipartite matching of the objects of its kind to its slots by Black's names; the slots no target holds keep the
    rest in the C's order. The new list, or None."""
    new, want = list(listed), {}
    for section, target in targets.items():
        # the slots of this section in layout order (layout() groups the sorted list by section, own and shared)
        shared, lay = None, []
        for i in order:
            if kinds[i][0] != section:
                continue
            if kinds[i][1]:
                lay.append([i])
            elif shared is None:
                shared = [i]
                lay.append(shared)
            else:
                shared.append(i)
        lay = [i for g in lay for i in g if listed[i].name not in DROPPED]
        if len(lay) != len(target):
            return None
        for j, i in enumerate(lay):
            names = target[j][2]
            if not free(listed[i]):
                if listed[i].name not in names:
                    return None
                continue
            want.setdefault((cls(listed[i]), kinds[i]), []).append((i, names))
    used = set()
    for c, rows in want.items():
        pool = [o for o in listed if free(o) and (cls(o), kind(o)) == c]
        match = {}

        def augment(r, seen):
            for o in pool:
                if o.name in rows[r][1] and o.name not in seen:
                    seen.add(o.name)
                    if o.name not in match or augment(match[o.name], seen):
                        match[o.name] = r
                        return True
            return False
        for r in range(len(rows)):
            if not augment(r, set()):
                return None
        for o in pool:
            if o.name in match:
                new[rows[match[o.name]][0]] = o
                used.add(o.name)
    # the free slots no target section holds: the objects left of their class and kind, in the C's order
    taken = {i for rows in want.values() for i, _ in rows}
    for i, o in enumerate(listed):
        if free(o) and i not in taken:
            k = (cls(o), kinds[i])
            left = next(x for x in listed if free(x) and x.name not in used and (cls(x), kind(x)) == k)
            new[i] = left
            used.add(left.name)
    return new


def ipa_function(path, obj, targets):
    """Does the file build with -ipa function (a per-file flag) give Black's data order with the same code? MWCC then
    writes the objects defined above the first function sorted by size when it emits that function, every later one
    where it is defined, and each function's local initializers in sections of their own as it emits the function
    (pokebw2's ctvt_draw.c and delivery_beacon.c are such files): an order no -ipa file list gives. Returns a note or
    None."""
    sys.path.insert(0, ROOT)
    try:
        from configure import compiler_of
        compiler, flags = compiler_of(path)
    except Exception:
        return None
    if "-ipa file" not in flags:
        return None
    flags = [f for x in flags for f in ("-ipa function" if x == "-ipa file" else x).split()]
    try:
        obj2 = compile_c(open(f"{ROOT}/{path}").read(), os.path.dirname(f"{ROOT}/{path}"), flags, compiler)
    except SystemExit:
        return None
    if code_of(obj2) != code_of(obj):
        return None
    ours = {o.name: o for sec in SECTIONS for o in data_objects(obj, sec)}
    for sec, target in targets.items():
        lay = [o for o in data_objects(obj2, sec) if not o.pooled and o.name not in DROPPED]
        if len(lay) != len(target) or not all(
                o.size == size and (o.name in names or any(ours[n].data == o.data and o.data for n in names if n in ours))
                for o, (_, size, names) in zip(lay, target)):
            return None
    return (f"with -ipa function it is Black's, the code unchanged: the file was built so (no declaration order "
            f"under -ipa file gives it); a per-file flag for {path} (configure.py's file flags, as pokebw2's "
            f"config/compiler_flags.toml has for src/ov257/ctvt_draw.c)")


def built_object(path):
    """The file's object, built (ninja) when it is older than its C: an object the C has changed since laid out the
    C before the edit, and solve() said MISMATCH. Exits saying so when it can't be built."""
    rel = f"build/b_us/{path[:-2]}.o"
    if not os.path.exists(f"{ROOT}/{rel}") or os.path.getmtime(f"{ROOT}/{rel}") < os.path.getmtime(f"{ROOT}/{path}"):
        p = subprocess.run(NINJA + [rel], cwd=ROOT, capture_output=True, text=True)
        if p.returncode:
            sys.exit(f"the object is stale: {rel} is older than {path}, and ninja fails: "
                     + next((l for l in (p.stdout + p.stderr).splitlines() if "rror" in l), "").strip()[:200])
        print(f"(the object was stale, older than {path}: rebuilt)")
    return f"{ROOT}/{rel}"


def solve(path, section=None, check=False, limit=TIME_LIMIT, ipa_only=False):
    obj = read_object(built_object(path))
    # the first section with objects, unless one is given (not .bss, which has no bytes to tell Black's objects by)
    section = section or next((s for s in SECTIONS[:2] if [o for o in data_objects(obj, s) if not o.pooled]), ".rodata")
    listed, notes = parse_order(listed_objects(obj), path, object_users(obj))
    for n in notes:
        print(n)
    if link_drops(path, obj):
        print(f"the link drops {len(DROPPED)} of ours (nothing Black has reaches them; sorted with the others, then "
              f"gone): " + ", ".join(sorted(DROPPED, key=lambda n: (not n.startswith("@"), n))[:12])
              + (", ..." if len(DROPPED) > 12 else ""))
    print("listed (parse order, .rodata, .data and .bss): " + ", ".join(o.name for o in listed))
    targets = {}  # each section the list reproduces: Black's objects, kept by every move
    for sec in (section,) + tuple(s for s in SECTIONS if s != section):
        whole = data_objects(obj, sec)
        if not whole:
            continue
        ours = [o for o in whole if o.name not in DROPPED]  # (those the link keeps, Black's to compare)
        model = [o.name for o in layout(listed, sec)]
        for mine, like in strings_unlike(path, sec, ours):
            print(f"{sec}: our string {mine!r} isn't Black's" + (f"; Black has {like!r}" if like else ""))
        if sec != section:
            if model == [o.name for o in whole if not o.pooled]:
                targets[sec] = black_targets(path, sec, ours)
                if not targets[sec]:
                    del targets[sec]
            print(f"{sec}: " + ("the list reproduces our object; the moves keep Black's" if sec in targets else
                                "the list reproduces our object, but Black's objects are unknown (its symbols name "
                                "too few of them: --like REV gives the order of the C at REV)"
                                if model == [o.name for o in whole if not o.pooled] else
                                "the list doesn't reproduce our object; not checked"))
            continue
        print(f"ours ({sec}, as laid out): " + ", ".join(f"{o.name}:{o.size}" + ("p" if o.pooled else "" if o.own
                                                            else "s") + ("d" if o.name in DROPPED else "")
                                         for o in whole)
              + "   (s: in the shared section, p: a string code uses, not sorted, d: the link drops it)")
        if model != [o.name for o in whole if not o.pooled]:
            print(f"MISMATCH: the list lays out as {', '.join(model)}, not as our object: an object is missing from "
                  "the list, or a local is listed where its helper isn't (a static inline defined away from its "
                  "caller); the verdicts below mean nothing")
            return
        print("the list reproduces our object")
        pooled = {o.name for o in ours if o.pooled}
        black = black_targets(path, sec, ours)
        print("Black: " + ", ".join(f"{label}:{size}=" + "|".join(sorted(names)) for label, size, names in black))
        if (path, sec) in EARLY:
            print(f"Black's {sec} objects start at {EARLY[path, sec][0]:#x}, before the range's {EARLY[path, sec][1]:#x}"
                  ": ours by their bytes, in no file's range (the range starts late)")
        named = {n for _, _, names in black for n in names}
        lack = [o.name for o in ours if o.name not in named and not o.pooled]
        if len(black) != len(ours) - len(pooled) or lack:
            print(f"Black has {len(black)} objects, ours {len(ours) - len(pooled)}"
                  + (f"; ours Black lacks (by bytes and size): {', '.join(lack)}" if lack else ""))
        targets = {sec: black, **targets}
    if ipa_only:
        print(ipa_function(path, obj, targets) or "with -ipa function it isn't Black's (or the code changes, or the "
              "file isn't built with -ipa file)")
        return
    if section not in targets:
        print(f"no {section} objects in our object")
        return
    if check:
        return
    if fits(listed, targets):
        unknown = [s for s in SECTIONS if s not in targets and [o for o in data_objects(obj, s) if not o.pooled]]
        print("our order is Black's" + (f" in {', '.join(targets)} ({', '.join(unknown)}: not checked, see above)"
                                        if unknown else ""))
        return
    inv = invert(listed, targets)
    if inv is not None and fits(inv, targets):
        moved = [o for o, p in zip(inv, listed) if o is not p]
        print("swap (the model run backwards: each moves to the place of the equal-size object Black has where it "
              "lays out, nothing else moves): " + "; ".join(f"{o.name} to where {p.name} is"
                                                             for o, p in zip(inv, listed) if o is not p))
        if len(moved) > 2 * MAX_MOVES:
            return
    # moving definitions only is the most natural, if it takes one move more
    best = Best(limit)
    try:
        found = moves(listed, targets, best=best)
        found = [w for w in moves(listed, targets, len(found[0]) + 1 if found else MAX_MOVES + 1, True, best)[:3]
                 if w not in found] + found
    except Stopped:
        print(f"stopped after {limit:g} s ({best.tried} orders tried; --time-limit S changes it, 0 is none)")
        base = misplaced(listed, targets)
        if best.way and best.bad < base:
            print(f"the closest order found, with {best.bad} object(s) out of place (ours: {base}): move: "
                  + "; ".join(f"{describe(o, listed)} after {describe(after, listed)}" for o, after in best.way))
        else:
            print(f"no move found that brings it closer than ours ({base} object(s) out of place)")
        return
    for way in found[:6]:
        print("move: " + "; ".join(f"{describe(o, listed)} after {describe(after, listed)}" for o, after in way))
    if found:
        print(f"({len(found)} way(s). A named object moves with its definition; a local goes to a static inline "
              "helper of its own, defined there and called where the local was; a definition without an initializer "
              "listed elsewhere than the end gets one there (= NULL, = {0}), still .bss)")
        return
    print(f"no {MAX_MOVES} moves give Black's order" + ("" if inv is not None else
          " (nor any declaration order of the named objects alone: Black's differs by more than equal sizes)"))
    found, (bad, closest), units, stopped = permute(listed, targets, Best(limit))
    names = ", ".join(o.name for o in units)
    if found:
        way = order_moves(units, found[0])
        how = (f"every order of {names} tried, each with its initializer's strings; {len(found)} give it"
               if len(units) <= MAX_UNITS else f"a local search of the orders of {names}, each with its strings")
        print(f"declaration order {', '.join(o.name for o in found[0])} gives Black's ({how}): move: "
              + "; ".join(f"{describe(o, listed)} before {describe(b, listed) if b else 'the end of them'}"
                          for o, b in way))
        return
    print(f"no declaration order of {names} gives Black's" + (" (stopped by the time limit)" if stopped else
          f" (all {math.factorial(len(units))} tried, each with its initializer's strings)" if len(units) <= MAX_UNITS
          else f" (a local search of the {math.factorial(len(units))} orders found none: not a proof)")
          + (f"; the closest, {bad} out of place: {', '.join(o.name for o in closest)}" if closest != units else ""))
    note = ipa_function(path, obj, targets)
    if note:
        print(note)
    cands = localizable(path, open(f"{ROOT}/{path}").read(), listed)
    for bad, subset in localize_subsets(listed, targets, cands)[:3]:
        print(f"with {', '.join(subset)} as local initializer(s) ({'; '.join(cands[n].lhs + ' = ' + n for n in subset)}) "
              f"{bad} object(s) are out of place: --apply tries it")
    for o in listed:
        if not o.name.startswith("@") and o.own and o.size < MODE["separate"] and o.name != FIRST.name and \
                fits([x._replace(own=False) if x is o else x for x in listed], targets):
            print(f"with {o.name} ({o.size} bytes) in the shared section it is Black's: in Black something in the "
                  "file refers to it (nothing here does, so it has a section of its own); only a reference the "
                  "compile drops would do (a static or inline nothing calls, an unused static's initializer)")
        if not o.name.startswith("@") and o.name != FIRST.name and fits([x for x in listed if x is not o],
                                               {s: [b for b in t if b[2] != {o.name}] for s, t in targets.items()}):
            print(f"without {o.name} ({o.size} bytes) it is Black's: another file's object (a boundary fix: check "
                  "who loads it)")
    for size in range(4, SEPARATE, 4):
        for p in range(len(listed) + 1):
            extra = Obj("dropped", size, True, b"", set(), None, section, False)
            if (p in (0, len(listed)) or not same_run(listed[p - 1], listed[p])) and                     fits(listed[:p] + [extra] + listed[p:], targets, extra):
                print(f"with a {size}-byte initializer listed after {describe(listed[p - 1] if p else None, listed)} "
                      "that the link drops, it is Black's: a function nothing calls, defined there (needs a "
                      "dead-stripping link)")
                return


def fits_split(listed, targets, k):
    """Does the list cut after its k-th object lay out each section as Black's objects when the two halves are two
    objects (two files: the first's sorted objects, then the second's)?"""
    a, b = listed[:k], listed[k:]
    for section, target in targets.items():
        lay = linked(a, section) + linked(b, section)
        if len(lay) != len(target) or not all(o.name in names for o, (_, _, names) in zip(lay, target)):
            return False
    return True


def glued(path):
    """tools/glued_files.py's data signal: a file whose data is Black's order only as two objects, or only without an
    object of another file's. None when its data fits as one object, or can't be told (no object built, the list
    doesn't reproduce ours, Black's objects aren't ours: the C lacks data, the range is wrong), else
    {"cuts": [k, ...], "listed": [Obj], "foreign": [Obj]}: the cut points of the list (parse order) after which a
    second file's objects begin, and the named objects of ours that the file isn't Black's without."""
    objpath = f"{ROOT}/build/b_us/{path[:-2]}.o"
    if not path.endswith(".c") or not os.path.exists(objpath):
        return None
    try:
        obj = read_object(objpath)
        listed, _ = parse_order(listed_objects(obj), path, object_users(obj))
        link_drops(path, obj)
        targets = {}
        for sec in SECTIONS[:2]:
            ours = data_objects(obj, sec)
            if not [o for o in ours if not o.pooled]:
                continue
            if [o.name for o in layout(listed, sec)] != [o.name for o in ours if not o.pooled]:
                return None
            ours = [o for o in ours if o.name not in DROPPED]
            black = sorted_run(black_objects(path, sec, ours), ours)
            if not black or len(black) != len(ours) - sum(o.pooled for o in ours):
                return None
            targets[sec] = black
    except (Exception, SystemExit):
        return None
    if not targets or fits(listed, targets):
        return None
    cuts = [k for k in range(1, len(listed)) if fits_split(listed, targets, k)]
    foreign = [o for o in listed if not o.name.startswith("@") and o.name != FIRST.name and
               fits([x for x in listed if x is not o], {s: [b for b in t if b[2] != {o.name}] for s, t in targets.items()})]
    if not cuts and not foreign:
        return None
    return {"cuts": cuts, "listed": listed, "foreign": foreign, "users": object_users(obj)}


# --apply: the moves done in the C. A move is a top-level item (a data definition, or a static inline helper with its
# locals) cut from its place and put before another (a gap); the model lists the objects from their new places
# (relist()), so every item at every gap is tried without compiling, then the ways that give Black's order are
# compiled, checked and fitted, the most natural first.
NINJA = ["ninja"] + (["-j", os.environ["BW_JOBS"]] if os.environ.get("BW_JOBS") else [])
Item = namedtuple("Item", "start code end kind depth")


def top_items(text, code):
    """The C's top-level items in order: Item(start: its line's start, with the comment lines just above it; code: its
    first character; end: past its last line; kind: "#include", "#if", ..., "function" or "declaration"; depth: the
    #if blocks around it)."""
    items, i, n, depth = [], 0, len(code), 0
    while i < n:
        if code[i].isspace():
            i += 1
            continue
        start = code.rfind("\n", 0, i) + 1
        while start:
            above = text.rfind("\n", 0, start - 1) + 1
            if not text[above:start].strip() or code[above:start].strip():
                break
            start = above
        start = max(start, items[-1].end if items else 0)
        if code[i] == "#":
            e = i
            while True:
                e = code.find("\n", e)
                e = n if e < 0 else e
                if e < n and code[:e].rstrip().endswith("\\"):
                    e += 1
                    continue
                break
            kind = "#" + re.match(r"#\s*(\w*)", code[i:])[1]
            depth -= kind in ("#else", "#elif", "#endif")
            items.append(Item(start, i, min(e + 1, n), kind, depth))
            depth += kind in ("#if", "#ifdef", "#ifndef", "#else", "#elif")
            i = e + 1
            continue
        j, par, brace, func = i, 0, 0, False
        while j < n:
            c = code[j]
            if c in "([":
                par += 1
            elif c in ")]":
                par -= 1
            elif c == "{":
                if par == 0 and brace == 0:
                    head = code[i:j].rstrip()
                    func = head.endswith(")") and "=" not in head
                brace += 1
            elif c == "}":
                brace -= 1
                if brace == 0 and func:
                    break
            elif c == ";" and par == 0 and brace == 0:
                break
            j += 1
        e = code.find("\n", j)
        e = n if e < 0 else e + 1
        if code[j + 1:e].strip():
            e = j + 1
        items.append(Item(start, i, e, "function" if func else "declaration", depth))
        i = e
    return items


def moved_key(spans):
    """relist()'s key with the items of spans [(start, end, gap, k)] cut out and put at their gaps (the k-th there)."""
    def key(o):
        at, delta = POS[o.name]
        for s, e, g, k in spans:
            if s <= at < e:
                return g, 0, k, at - s + delta
        return at, 1, 0, delta
    return key


def analyse(path, obj, text=None):
    """(the list, {section: Black's objects} of each section whose layout the list reproduces, notes)."""
    listed, notes = parse_order(listed_objects(obj), path, object_users(obj), text)
    link_drops(path, obj)
    targets = {}
    for sec in SECTIONS:
        ours = data_objects(obj, sec)
        if not ours:
            continue
        if [o.name for o in layout(listed, sec)] == [o.name for o in ours if not o.pooled]:
            targets[sec] = black_targets(path, sec, [o for o in ours if o.name not in DROPPED])
            if not targets[sec]:
                del targets[sec]
        else:
            notes.append(f"{sec}: the list doesn't reproduce our object (a model gap): not checked")
    return listed, targets, notes


def code_of(obj, ignore=()):
    """The object's code, to tell that a move changed none: each .text section's bytes and relocations (to a data
    object by name, but not its @N or a section's offset, which moves change, nor those of ignore: the names of
    objects made locals). A TwlSDK LTD file's code is the .ltdmain sections that hold its functions."""
    d, secs, syms, relocs = obj
    code = {i for n, i, v, sz, g, f in syms if f}
    return [(d[off:off + size], sorted((o, t) + ((syms[s][0], a) if syms[s][0][:1] not in "@." and syms[s][0] not in
                                                 ignore else ())
                                       for o, t, s, a in relocs.get(i, ())))
            for i, (name, kind, off, size, _) in enumerate(secs) if kind == 1 and (name == ".text" or name == ".ltdmain"
                                                                                   and i in code)]


def element_count(text, code, p):
    """How many elements the initializer at p (after the "=") has, or None (designated, or not simple)."""
    p = len(code) - len(code[p:].lstrip())
    if code[p] == '"':
        q = code.index('"', p + 1)
        if code[q + 1:].lstrip()[:1] != ";":
            return None
        return len(bytes(text[p + 1:q], "latin-1").decode("unicode_escape")) + 1
    if code[p] != "{":
        return None
    depth, count, last, i = 0, 0, "{", p
    while True:
        c = code[i]
        if depth == 1 and not c.isspace() and last in "{," and c in "[.":
            return None
        if c in "{[(":
            depth += 1
        elif c in "}])":
            depth -= 1
            if depth == 0:
                return count + (last not in ",{")
        elif c == "," and depth == 1:
            count += 1
        if not c.isspace():
            last = c if depth >= 1 or c == "}" else last
        i += 1


def declared_in_headers(name):
    r = subprocess.run(["grep", "-rlw", "--include=*.h", name, "include", "src", "lib"], cwd=ROOT,
                       capture_output=True, text=True)
    return bool(r.stdout.strip())


def realize(text, code, items, owner, moves, names):
    """The C with each item of moves [(item index, gap)] cut and put at its gap, a declaration left in its place when
    code between the two uses it, a definition without an initializer given one: (C, [what changed]) or (None, why)."""
    inserts, removed, said = {}, {}, []
    line = lambda p: code.count("\n", 0, p) + 1  # noqa: E731
    for k, g in moves:
        it = items[k]
        body = text[it.start:it.end]
        body += "" if body.endswith("\n") else "\n"
        semi = code.rindex(";", it.code, it.end) if it.kind == "declaration" else None
        late = [o.name for o in owner[k] if o.name in TENTATIVE] if INIT["on"] else []
        if late:
            head = code[it.code:semi]
            nm = late[0]
            if re.search(rf"\*\s*(?:const\s+)?{re.escape(nm)}\s*$", head):
                value = "NULL"
            elif re.search(rf"\b{re.escape(nm)}\s*\[", head) or not re.search(
                    r"\b(u8|u16|u32|u64|s8|s16|s32|s64|int|BOOL|char|short|long|unsigned|signed|fx16|fx32|f32|"
                    r"vu8|vu16|vu32|size_t|OSTick|OSTime|enum)\b", head):
                value = "{0}"
            else:
                value = "0"
            cut = semi - it.start
            body = body[:cut] + f" = {value}" + body[cut:]
            said.append(f"{nm} gets the initializer = {value}")
        inserts.setdefault(g, []).append(body)
        # a declaration where it was, when code between there and its gap uses it
        own = names[k]
        uses = [m.start() for nm in own for m in re.finditer(rf"\b{re.escape(nm)}\b", code)
                if not it.start <= m.start() < it.end]
        ahead = any(x.kind == "declaration" and x.depth == 0 and x.end <= it.start and "=" not in code[x.code:x.end]
                    and re.search(rf"\b{re.escape(nm)}\b", code[x.code:x.end]) for x in items for nm in own)
        decl = ""
        if g > it.start and any(it.end <= u < g for u in uses) and not ahead:
            if it.kind == "function":
                decl = text[it.code:code.index("{", it.code)].rstrip() + ";\n"
            else:
                eq = code.index("=", it.code, semi)
                head = text[it.code:eq].rstrip()
                if re.search(r"\[\s*\]", head):
                    count = element_count(text, code, eq + 1)
                    if count is None:
                        return None, f"{', '.join(own)} would need a declaration ahead, and its size isn't plain"
                    head = re.sub(r"\[\s*\]", f"[{count}]", head, count=1)
                if not re.search(r"\bstatic\b", head):
                    if all(declared_in_headers(nm) for nm in own):
                        head = None
                    else:
                        head = "extern " + head
                decl = head + ";\n" if head else ""
            if decl:
                said.append(f"{', '.join(own)} declared ahead at line {line(it.code)}")
        removed[it.start] = (it, decl)
        said.append(f"{', '.join(own) or 'lines'} (lines {line(it.code)}-{line(it.end - 1)}) moved "
                    + (f"before line {line(g)}" if g < len(text) else "to the end"))
    out, i = [], 0
    for p in sorted(set(inserts) | set(removed)):
        out.append(text[i:p])
        i = max(i, p)
        for body in inserts.get(p, []):
            out.append("\n" + body if p == len(text) else body + "\n")
        if p in removed:
            it, decl = removed[p]
            out.append(decl)
            i = it.end
            if not decl and text.startswith("\n", i):
                i += 1
    out.append(text[i:])
    return "".join(out), said



# --- a constant copied into a variable is Black's local initializer -----------------------------------------------
# `matrix->translation = data_X;` or `VecFx32 v = data_X;` with a struct const data_X that nothing else refers to may
# be a local initializer in Black's C: `VecFx32 v = { ... }; matrix->translation = v;` compiles to the same code (the
# copy from the pool), and a local initializer is an object of its own section, laid out after the shared run
# (ov043_021f36c0.c's two 12-byte VecFx32, scrcmd_shop.c's FieldEvCameraAnimationFlags): an order no moving of
# definitions gives.

Local = namedtuple("Local", "name use stmt lhs kind item head init")


def localizable(path, text, listed):
    """{name: Local} of the named .rodata objects (under SEPARATE bytes) of a plain struct const, defined once at the
    top level of the C with an initializer, that one statement of a function copies (`lhs = X;` or `T v = X;`) and
    nothing else mentions (in this file, headers or other sources)."""
    code = blank(text)
    items = top_items(text, code)
    bodies = function_bodies(code)
    out = {}
    for o in listed:
        if o.name.startswith("@") or o.section != ".rodata" or o.size >= SEPARATE or o.name not in POS:
            continue
        at = POS[o.name][0]
        # (POS may point at the `}` of the function before: parse_order()'s `} name = {` alternative)
        d0 = re.search(rf"\b{re.escape(o.name)}\b\s*(?:\[[^\]]*\]\s*)*=", code[max(at, 0):])
        if d0 is None:
            continue
        at += d0.start()
        it = next((x for x in items if x.start <= at < x.end and x.depth == 0 and x.kind == "declaration"), None)
        if it is None:
            continue
        semi = code.rfind(";", it.code, it.end)
        eq = code.find("=", it.code, semi)
        head = code[it.code:eq] if eq > 0 else ""
        if eq < 0 or "[" in head or "*" in head or "(" in head:
            continue
        m = re.search(rf"\b{re.escape(o.name)}\s*$", head)
        if not m:
            continue
        ctype = " ".join(re.sub(r"\b(static|const|extern|volatile)\b", " ", head[:m.start()]).split())
        if not ctype or not re.fullmatch(r"(struct )?\w+", ctype):
            continue
        mentions = [x.start() for x in re.finditer(rf"\b{re.escape(o.name)}\b", code)]
        uses = [p for p in mentions if not it.start <= p < it.end]
        # a declaration ahead (`extern const T X;` / `static const T X;`) is not a use
        decls = [x for x in items if x.kind == "declaration" and x.depth == 0 and x is not it and any(
            x.start <= p < x.end for p in uses) and "=" not in code[x.code:x.end]]
        uses = [p for p in uses if not any(x.start <= p < x.end for x in decls)]
        if len(uses) != 1 or declared_in_headers(o.name) and False:
            continue
        p = uses[0]
        f = next((n for n, b in bodies.items() if b[1] <= p < b[2]), None)
        if f is None or not re.match(r"\s*;", code[p + len(o.name):]):
            continue
        start = max(code.rfind(c, 0, p) for c in ";{}") + 1
        lhs = code[start:p].rstrip()
        if not lhs.endswith("="):
            continue
        lhs = lhs[:-1].strip()
        if re.search(r"[(){};=]", lhs):
            continue
        d = re.fullmatch(rf"(?:const\s+)?(?:struct\s+)?{re.escape(ctype.replace('struct ', ''))}\s+(\w+)", lhs)
        if not (d or re.fullmatch(r"[\w\.\[\]\s\->\*]+", lhs) and not re.search(r"\w\s+\w", lhs)):
            continue
        # headers and other sources that mention it would break
        r = subprocess.run(["grep", "-rlw", "--include=*.c", "--include=*.h", o.name, "include", "src", "lib"], cwd=ROOT,
                           capture_output=True, text=True).stdout.split()
        if [x for x in r if x != path and not (x.endswith(".h") and False)]:
            continue
        out[o.name] = Local(o.name, p, text.rfind("\n", 0, start + 1) + 1 if False else start, lhs,
                            "decl" if d else "assign", it, ctype, text[code.find("=", it.code, semi) + 1:semi].strip())
    return out


def with_locals(text, locals_):
    """The C with each Local made a local initializer: its definition gone, its use `T v = { ... };` (or the statement
    `lhs = X;` after one). Returns (text, [new local names])."""
    code = blank(text)
    edits, names = [], []
    for loc in locals_:
        it = loc.item
        edits.append((it.start, it.end, ""))
        p = loc.use
        end = code.index(";", p)
        if loc.kind == "decl":
            edits.append((p, p + len(loc.name), loc.init))
        else:
            line = text.rfind("\n", 0, p) + 1
            stmt = code.rfind("\n", 0, p)
            stmt_start = max(code.rfind(c, 0, p) for c in ";{}") + 1
            first = stmt_start
            while first < p and code[first].isspace():
                first += 1
            indent = text[text.rfind("\n", 0, first) + 1:first]
            if indent.strip():
                indent = ""
            ident = re.findall(r"\w+", loc.lhs)[-1]
            tmp = ident + "Init" if ident != loc.lhs else ident + "Init"
            n = 2
            while re.search(rf"\b{tmp}\b", text) or tmp in names:
                tmp = f"{ident}Init{n}"
                n += 1
            names.append(tmp)
            edits.append((first, first, f"{loc.head} {tmp} = {loc.init};\n{indent}"))
            edits.append((p, p + len(loc.name), tmp))
        # a declaration ahead
    # declarations of the name ahead (extern/static prototypes) go too
    for loc in locals_:
        for x in top_items(text, code):
            if x.kind == "declaration" and x.depth == 0 and x is not loc.item and re.search(
                    rf"\b{re.escape(loc.name)}\b", code[x.code:x.end]) and "=" not in code[x.code:x.end]:
                edits.append((x.start, x.end, ""))
    out, last = [], len(text)
    for s, e, new in sorted(edits, key=lambda x: (x[0], x[1]), reverse=True):
        out.append((s, e, new))
    res = text
    for s, e, new in out:
        res = res[:s] + new + res[e:]
    return res, names


def hypothetical(listed, cands, subset):
    """The list with the objects of subset (names of cands) as local initializers where they are copied."""
    saved = {n: POS[n] for n in subset}
    new = []
    for o in listed:
        if o.name in subset:
            POS[o.name] = (cands[o.name].use, 0)
            new.append(o._replace(own=True))
        else:
            new.append(o)
    out = relist(new, place)
    POS.update(saved)
    return out


def localize_subsets(listed, targets, cands, most=3):
    """The subsets of cands (up to most) whose objects as local initializers bring the list closer to Black's, the best
    first: [(objects out of place, subset)]."""
    base = misplaced(listed, targets)
    out = []
    for k in range(1, min(most, len(cands)) + 1):
        for subset in itertools.combinations(sorted(cands, key=lambda n: cands[n].use), k):
            bad = misplaced(hypothetical(listed, cands, subset), targets)
            if bad <= base:
                out.append((bad, subset))
    return sorted(out, key=lambda x: (x[0], len(x[1])))


DEEPEST = 5  # most moves of top-level items --apply tries
BEAM = 60  # the partial ways kept at each depth


def deeper(reps, sim, targets, pairs):
    """Ways of three to DEEPEST moves of top-level items, when no one or two give Black's order: a beam search from the
    closest pairs, each step one more item (not yet moved) at one of its gaps that changed the list (reps), the ways
    with the fewest objects out of place kept. [((item, gap), ...)] that fit."""
    states, seen = {}, set()
    for a, ga, b, gb in pairs[:BEAM * 20]:
        for way in ((a, ga), (b, gb)), ((b, gb), (a, ga)):
            new = sim(list(way))
            key = tuple(o.name for o in new)
            if key not in seen:
                seen.add(key)
                states[way] = misplaced(new, targets)
    beam = sorted(states, key=states.get)[:BEAM]
    for _ in range(3, DEEPEST + 1):
        grown, found = {}, []
        for way in beam:
            used = {k for k, _ in way}
            for k in reps:
                if k in used:
                    continue
                for g in reps[k]:
                    new = way + ((k, g),)
                    lst = sim(list(new))
                    key = tuple(o.name for o in lst)
                    if key in seen:
                        continue
                    seen.add(key)
                    bad = misplaced(lst, targets)
                    if bad == 0:
                        found.append(new)
                    grown[new] = bad
        if found:
            return found
        beam = sorted(grown, key=grown.get)[:BEAM]
    return []


def run_fit(path, obj_path, text, restore=True):
    """tools/fit_data.py on the file as it is: 0 if it marks it complete (prints it), else 1 (the C put back as text
    unless restore is false)."""
    r = subprocess.run([sys.executable, "tools/fit_data.py", path], cwd=ROOT, capture_output=True, text=True)
    out = (r.stdout + r.stderr).strip()
    if "MATCHED" in out:
        print(f"APPLIED {path}: complete, ninja passes; commit it: python3 tools/finish.py {path} "
              "\"data order: tools/data_order.py --apply\"")
        return 0
    if restore and not KEEP:
        open(f"{ROOT}/{path}", "w").write(text)
        subprocess.run(NINJA + [obj_path], cwd=ROOT, capture_output=True)
    print(f"NOT APPLIED {path}: the data is in Black's order then, but fit_data.py fails: "
          + " ".join(out.splitlines()[-1].split())[:300])
    return 1


def apply_localize(path, limit, tries, text, listed, targets, obj_path, before_code):
    """When no moves give Black's order: of the struct consts a function copies once, those made local initializers
    (an object of a section of its own, after the shared run) that bring the list closer, the best first: the C
    rewritten, compiled, its code kept (as the copy from the pool), then the moves again. 0 if one completed the
    file, else 1 (the C as it was)."""
    cands = localizable(path, text, listed)
    if not cands:
        return 1
    tried = []
    for bad, subset in localize_subsets(listed, targets, cands)[:6]:
        new, names = with_locals(text, [cands[n] for n in subset])
        open(f"{ROOT}/{path}", "w").write(new)
        r = subprocess.run(NINJA + [obj_path], cwd=ROOT, capture_output=True, text=True)
        what = ", ".join(subset) + " as local initializers"
        if r.returncode:
            tried.append(f"{what}: doesn't compile")
            continue
        obj2 = read_object(f"{ROOT}/{obj_path}")
        if code_of(obj2, subset) != before_code(subset):
            tried.append(f"{what}: the code changes")
            continue
        listed2, targets2, _ = analyse(path, obj2)
        print(f"{what}: the same code")
        if targets2 and fits(listed2, targets2):
            if run_fit(path, obj_path, text) == 0:
                return 0
        elif apply(path, limit, tries, nested=True) == 0:
            return 0
        open(f"{ROOT}/{path}", "w").write(text)
        subprocess.run(NINJA + [obj_path], cwd=ROOT, capture_output=True)
    for x in tried:
        print("  " + x)
    open(f"{ROOT}/{path}", "w").write(text)
    subprocess.run(NINJA + [obj_path], cwd=ROOT, capture_output=True)
    return 1


def apply(path, limit=TIME_LIMIT, tries=12, nested=False):
    """Finds the moves of top-level items that give Black's order, does the most natural that compiles, keeps the
    code and passes tools/fit_data.py (which marks the file complete, ninja passing). Returns 0 if one did."""
    obj_path = f"build/b_us/{path[:-2]}.o"
    if subprocess.run(NINJA + [obj_path], cwd=ROOT, capture_output=True).returncode:
        print(f"NOT APPLIED {path}: it doesn't compile")
        return 1
    obj = read_object(f"{ROOT}/{obj_path}")
    listed, targets, notes = analyse(path, obj)
    for n in notes:
        print(n)
    if not targets:
        print(f"NOT APPLIED {path}: no section whose layout the model reproduces")
        return 1
    if fits(listed, targets):
        if nested:
            return run_fit(path, obj_path, open(f"{ROOT}/{path}").read(), restore=False)
        print(f"NOT APPLIED {path}: our order is Black's already: nothing to move (tools/fit_data.py --dry-run says "
              "what stops it)")
        return 1
    for sec, black in targets.items():
        ours = [o for o in data_objects(obj, sec) if not o.pooled]
        named = {n for _, _, names in black for n in names}
        lack = [o.name for o in ours if o.name not in named]
        if len(black) != len(ours) or lack:
            print(f"NOT APPLIED {path}: no order gives Black's {sec}: Black has {len(black)} objects, ours {len(ours)}"
                  + (f"; ours Black lacks (by bytes and size): {', '.join(lack)}" if lack else ""))
            return 1
    text = open(f"{ROOT}/{path}").read()
    code = blank(text)
    items = top_items(text, code)
    bodies = function_bodies(code)
    owner, stuck = {}, []  # {item index: its listed objects}, the objects no move can take
    for o in listed:
        at = POS[o.name][0]
        k = next((k for k, it in enumerate(items) if it.start <= at < it.end), None)
        it = items[k] if k is not None else None
        ok = it is not None and it.depth == 0 and (it.kind == "declaration" or it.kind == "function" and re.search(
            r"\bstatic\b", code[it.code:code.index("{", it.code)]) and re.search(
            r"\binline\b", code[it.code:code.index("{", it.code)]))
        if ok:
            owner.setdefault(k, []).append(o)
        else:
            stuck.append(o.name)
    names = {k: [f for f, b in bodies.items() if items[k].code <= b[0] < items[k].end] if items[k].kind == "function"
             else [o.name for o in owner[k] if not o.name.startswith("@")] for k in owner}
    last_include = max((k for k, it in enumerate(items) if it.kind == "#include"), default=-1)
    gaps = [it.start for k, it in enumerate(items) if k > last_include and it.depth == 0 and it.kind not in
            ("#else", "#elif", "#endif")] + [len(text)]
    index = {g: k for k, it in enumerate(items) for g in [it.start]}
    index[len(text)] = len(items)
    best = Best(limit)
    base = [o.name for o in listed]

    def sim(moves):
        best.tick()
        spans = [(items[k].start, items[k].end, g, j) for j, (k, g) in enumerate(moves)]
        init = {o.name for k, _ in moves for o in owner[k] if o.name in TENTATIVE} if INIT["on"] else set()
        return relist(listed, moved_key(spans), init)

    def options(k):
        it = items[k]
        tentative = any(o.name in TENTATIVE for o in owner[k])
        return [g for g in gaps if not it.start < g <= it.end and (g != it.start or tentative)]

    found = []  # ways that fit
    try:
        # a definition without an initializer that moves stays one (it is listed at the end, in the order of the C),
        # else the next round gives it one, = 0, listed where it is
        for mode in (False, True) if any(o.name in TENTATIVE for os_ in owner.values() for o in os_) else (False,):
            INIT["on"] = mode
            reps = {}  # per item, a gap for each list a move of it gives, the nearest first
            for k in owner:
                seen = {}
                for g in sorted(options(k), key=lambda g: abs(index[g] - k)):
                    new = sim([(k, g)])
                    key = tuple(o.name for o in new)
                    if key == tuple(base) or key in seen:
                        continue
                    seen[key] = g
                    if fits(new, targets):
                        found.append(((k, g),))
                reps[k] = sorted(seen.values(), key=lambda g: misplaced(sim([(k, g)]), targets))[:12]
            if not found:
                pairs = sorted(((a, ga, b, gb) for a, b in itertools.combinations(reps, 2) for ga in reps[a]
                                for gb in reps[b]), key=lambda x: reps[x[0]].index(x[1]) + reps[x[2]].index(x[3]))
                for a, ga, b, gb in pairs:
                    for way in ([(a, ga), (b, gb)], [(b, gb), (a, ga)]) if ga == gb else ([(a, ga), (b, gb)],):
                        if fits(sim(way), targets):
                            found.append(tuple(way))
            if not found:
                found.extend(deeper(reps, sim, targets, pairs))
            if found:
                break
    except Stopped:
        print(f"the search stopped after {limit:g} s ({best.tried} moves tried; --time-limit S changes it)")
    if not found and not nested:
        text0 = open(f"{ROOT}/{path}").read()
        if apply_localize(path, limit, tries, text0, listed, targets, obj_path,
                          lambda subset: code_of(obj, subset)) == 0:
            return 0
    if not found:
        why = f"no move of up to {DEEPEST} top-level items (data definitions, static inline helpers) gives Black's order"
        if stuck:
            why += f"; {', '.join(stuck)} can't move (a local of a function that isn't a static inline helper, or " \
                   "inside #if)"
        print(f"NOT APPLIED {path}: {why}")
        return 1

    def cost(way):
        kinds = sum(2 if any(o.name in TENTATIVE for o in owner[k]) else 1 if items[k].kind == "function" else 0
                    for k, _ in way)
        return len(way), kinds, sum(abs(index[g] - k) for k, g in way)
    found.sort(key=cost)
    print(f"{len(found)} way(s) give Black's order in the model")
    # (--like: the code of an -ipa file reaches its statics by offsets from one base, so moving them changes it: it
    # must be the code the C had at REV)
    before = code_of(LIKE["obj"]) if LIKE.get("obj") else code_of(obj)
    tried = []
    for way in found:
        if len(tried) >= tries:
            break
        new, said = realize(text, code, items, owner, way, names)
        if new is None:
            tried.append(said)
            continue
        open(f"{ROOT}/{path}", "w").write(new)
        r = subprocess.run(NINJA + [obj_path], cwd=ROOT, capture_output=True, text=True)
        if r.returncode:
            err = next((l for l in r.stdout.splitlines() if re.search(r"rror|: error", l)), r.stdout[-150:])
            tried.append(f"{'; '.join(said)}: doesn't compile ({' '.join(err.split())[:120]})")
            continue
        obj2 = read_object(f"{ROOT}/{obj_path}")
        if code_of(obj2) != before:
            tried.append(f"{'; '.join(said)}: the code changes")
            continue
        listed2, targets2, _ = analyse(path, obj2)
        if set(targets2) != set(targets) or not fits(listed2, targets2):
            tried.append(f"{'; '.join(said)}: compiled, not Black's order (the model is off here)")
            continue
        print("moved: " + "; ".join(said))
        if LIKE.get("rev"):  # the file is not complete: the layout of the C at --like REV was the target
            print(f"APPLIED {path}: the data lies as it did at {LIKE['rev']}, the code is unchanged")
            return 0
        r = subprocess.run([sys.executable, "tools/fit_data.py", path], cwd=ROOT, capture_output=True, text=True)
        out = (r.stdout + r.stderr).strip()
        if "MATCHED" in out:
            print(f"APPLIED {path}: complete, ninja passes; commit it: python3 tools/finish.py {path} "
                  "\"data order: tools/data_order.py --apply\"")
            return 0
        if not KEEP:
            open(f"{ROOT}/{path}", "w").write(text)
            subprocess.run(NINJA + [obj_path], cwd=ROOT, capture_output=True)
        print(f"NOT APPLIED {path}: the data is in Black's order then, but fit_data.py fails: "
              + " ".join(out.splitlines()[-1].split())[:300])
        return 1
    open(f"{ROOT}/{path}", "w").write(text)
    subprocess.run(NINJA + [obj_path], cwd=ROOT, capture_output=True)
    print(f"NOT APPLIED {path}: none of the ways that fit worked:\n  " + "\n  ".join(tried))
    return 1


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("file", help="a source file (src/...c) or, with --compile, a C file to compile and list")
    ap.add_argument("--section", help="the section to solve (default: .rodata, else .data if it has objects)")
    ap.add_argument("--check", action="store_true", help="only the model against our object, and Black's objects")
    ap.add_argument("--time-limit", type=float, default=TIME_LIMIT, metavar="S",
                    help=f"seconds the search for moves may run, then the closest order found is printed (default "
                    f"{TIME_LIMIT}; 0: no limit)")
    ap.add_argument("--glued", action="store_true", help="only tools/glued_files.py's data signal for the file: "
                    "the cut points where it fits Black's order as two objects, and the objects of another file's")
    ap.add_argument("--compile", action="store_true", help="compile the C file with the game flags and list its data")
    ap.add_argument("--flags", help="with --compile: these flags instead of the game's")
    ap.add_argument("--compiler", help="with --compile: this compiler (tools/mwccarm/...; default: the file's, by "
                    "configure.py's compiler_of(), or dsi/1.1p1)")
    ap.add_argument("--ipa-function", action="store_true", help="only build the file with -ipa function and say whether "
                    "that gives Black's order with the code unchanged")
    ap.add_argument("--apply", action="store_true", help="do the most natural move that gives Black's order, and "
                    "keep it if tools/fit_data.py then marks the file complete")
    ap.add_argument("--like", metavar="REV", help="where Black's symbols give no objects (a .bss its -ipa file reaches "
                    "from one base), the order the C had at REV, the last version that matched, stands for Black's")
    a = ap.parse_args()
    if a.like:
        LIKE["rev"] = a.like
    if a.glued:
        g = glued(a.file)
        if not g:
            print("one object, or can't be told")
            return
        L = g["listed"]
        print(f"two objects, cut before: " + (", ".join(describe(L[k], L) for k in g["cuts"]) or "none"))
        print("another file's: " + (", ".join(describe(o, L) for o in g["foreign"]) or "none"))
        return
    if a.apply:
        sys.exit(apply(a.file, a.time_limit))
    if a.compile:
        compiler, flags = compile_flags(a.file, a.compiler, a.flags.split() if a.flags else None)
        set_mode(a.file, compiler, flags)
        obj = compile_c(open(a.file).read(), os.path.dirname(os.path.abspath(a.file)), flags, compiler)
        for sec in (".rodata", ".data", ".bss"):
            for o in data_objects(obj, sec):
                txt = repr(o.data.split(b"\0")[0].decode(errors="replace")) if o.data and all(
                    c == 0 or 32 <= c < 127 for c in o.data) else ""
                print(f"{sec} {o.name} {o.size}{' own' if o.own else ''}{' pooled' if o.pooled else ''} {txt}")
        return
    solve(a.file, a.section, a.check, a.time_limit, a.ipa_function)


if __name__ == "__main__":
    main()
