#!/usr/bin/env python3
"""Fit an incomplete file's ranges to its object, for a file whose functions all match but whose data doesn't fit its
delinks.txt entry ("delinks.txt gives it no .rodata", ".rodata is X bytes, delinks.txt gives Y", "... reaches @158
at A, Black's data_X at B"), and mark it complete if the build then matches.

    python3 tools/fit_data.py src/ov225/manual.c src/ov171/dendou_pc.c
    python3 tools/fit_data.py --dry-run src/ov225/manual.c     # what it would do, nothing changed

The file is compiled (ninja) and its object's sections placed as the link places them (tools/port_file.py's place()),
the .text at its range, where each function must be one of Black's of its size, or nothing else can be fitted; the
link's long-branch veneers take their bytes after each function that calls out of a branch's reach (veneers()). Only
functions past the range's end may be off it, when each is at one of Black's of its size and code (relocated words
aside) that no other file's code covers: the range was short (it ended at the last function twinned to pokebw2), so it takes them, and those under a
name Black doesn't have (pokebw2's) get Black's, everywhere (tools/port_file.py's rename_everywhere()).

Each data section of the object (.rodata, .data, .bss) then starts where Black's relocations say: a relocation of the
file's code (or of a section already placed) to a datum of the section is at an address where relocs.txt has Black's,
so the section starts as far before Black's target as the datum is into the section; they must all agree. A section no
relocation reaches is where its bytes are (relocated words aside), if that is one place only; if they are nowhere and
its data are only pokebw2's (names Black lacks, which nothing in the object or in other files uses), the definitions
go, as tools/port_file.py drops data Black lacks. Either way a section's bytes must be the ROM's (relocated words
aside), Black must have each of its relocations, to the same place where the target is placed, and it must lie in
the module's section, overlap no other file's range and keep the files' order: after the
data of the files whose code comes before the file's, before the others' (tools/file_ranges.py's check). A range ends
at the section's end, 4-aligned, and holds the dsd symbol the object ends in, which has no size and runs to the next
one: past the object's end only over zeros up to the module section's end (its padding), else Black's datum is
longer and the C lacks data. New ranges that leave bytes of the old ones in no file's range (alignment padding aside)
stop it: gsPlatformUtil.c's .bss start moved past a dropped function's static. A range the old entry had for a section the object lacks goes, but where the file's code
loads data in the old ranges that the new ones leave out, the C declares data it doesn't define: it stops there.

What code and data outside the file reach in its new ranges (relocs.txt) must be its globals, by Black's names, as
tools/port_file.py checks. A relocation dsd couldn't tell between overlays (module:overlays(182,224)), which it links
to the first, the file's, is another overlay's when it points into the middle of one of the file's objects: its
relocs.txt line gets the other overlays (the_end_graphic.c's 0x021b9bf8 is subway_map.c's proc in ov224). One from
data into the middle of an object is dsd reading a table's bytes as a pointer: its line goes, as tools/port_file.py
drops them (three into union_oneself.c's step table).

A file of data only (no .text range: enceff_data.c, fldmmdl_movedata.c) is placed by Black's names of its globals
or its bytes, between the files' ranges its old one lies between; a static initializer's .ctor pointer (dsprot's
files) is placed as the data are. An absolute word Black has no relocation for is right when the ROM has what the link
writes there: OVERLAY_ID(n)'s number, the address of a placed symbol or of a name Black has at one address. The .rodata
and .data sections only dropped code reaches go with it (mb_wm_base.c's bss_fill, dwc_mov_wh.c's tables). A .bss range
runs past the object only when no other file's .bss follows; else it ends with the object and dsd's symbol it ends
in gets that size (nwm.c's data_027341cc: four bytes of padding before fatfs_api.c's 32-aligned .bss).

The file takes the ranges (tools/add_source_file.py) and is marked complete, then tools/inner_symbols.py --apply
takes the dsd symbols inside its data objects out (those of the relocations it dropped too: snd_sys.c's 104 in
g_GFLSndHeap), and its rows in docs/nonmatching.md go;
`ninja` must pass, or its old entry and C come back and it says why: the build's error, or the first byte of the
module that differs and the symbol before it.
What stops a file otherwise is printed: functions in another order than Black's, data objects in another order or
section than Black's (relocations that disagree on a section's start, or bytes that differ), a neighbour's range out
of order. BW_JOBS caps ninja's processes.
"""
import argparse
import bisect
import glob
import os
import re
import struct
import sys
from collections import Counter

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from callee_names import PLACEHOLDER  # noqa: E402
from file_ranges import CODE, DATA, ROOT, Ranges, code_section, module_dir  # noqa: E402
from two_modules import real_of  # noqa: E402
from port_file import (CONFIG, comment_definitions, definition, module_bytes, place, reached,  # noqa: E402
                       read_names, read_object, run)

# The C++ exception index (MWCC's `-Cpp_exceptions on`: MSL's runtime.c), one 12-byte entry per function with a stack
# frame, which the link gathers in the functions' order before the module's .rodata (dsd's lcf: EXCEPTION): placed
# like a data section that nothing reaches, where its bytes are
EXCEPTIX = (".exceptix",)
# the data sections a file's object places: a static initializer's .ctor pointer too (dsprot's files)
SECTIONS = DATA + (".ctor",) + EXCEPTIX
# a TwlSDK file's twl/ltdmain_begin.h block: the LTD autoload's part of a file in two modules (tools/two_modules.py)
LTD_BLOCK = (".ltdmain", ".ltdmain.bss")
NINJA = ["ninja"] + (["-j", os.environ["BW_JOBS"]] if os.environ.get("BW_JOBS") else [])


# A branch's reach: Thumb's bl and blx, ARM's bl (ARMv5)
REACH = {"thumb": 0x400000, "arm": 0x2000000}
R_ARM_PC24, R_ARM_ABS32, R_ARM_THM_CALL, R_ARM_CALL = 1, 2, 10, 28
OVERLAY_ID = re.compile(r"OVERLAY_(\d+)_ID$")  # the linker script's overlay numbers (gfl/overlay.h's OVERLAY_ID(n))


def word(rom, base, addr):
    """The ROM's 32-bit word at addr (module_bytes()'s bytes from base), or None past them."""
    k = addr - base
    return int.from_bytes(rom[k:k + 4], "little") if 0 <= k and k + 4 <= len(rom) else None


def link_value(name, where, add, black_where, mod):
    """What the link writes for an absolute word to a symbol (R_ARM_ABS32, symbol + addend), or None: the overlay's
    number of the linker script's OVERLAY_n_ID (enceff_data.c's table, where Black has no relocation), the address of
    one of the object's placed symbols (where), or of a name symbols.txt has at one address only (the module's own
    first; irc_compatible.c's table of other overlays' procs, where dsd took none of the words for a pointer)."""
    m = OVERLAY_ID.match(name)
    if m:
        return int(m[1]) + add
    if where is not None:
        return where + add
    places = black_where.get(name, ())
    own = {a for m, a in places if m == mod} or {a for _, a in places}
    return own.pop() + add if len(own) == 1 else None


def function_kinds():
    """{name: [(address, is it ARM)]} of every function in the config's symbols.txt files."""
    out = {}
    for path in glob.glob(f"{CONFIG}/**/symbols.txt", recursive=True):
        for name, mode, addr in re.findall(r"^(\S+) kind:function\((arm|thumb)[^)]*\) addr:(0x\w+)", open(path).read(),
                                           re.M):
            out.setdefault(name, []).append((int(addr, 16), mode == "arm"))
    return out


def veneers(secs, syms, relocs, start, kinds):
    """{.text section index: the bytes of the long-branch veneers the link puts after it}: one per call (each call site,
    -overlaygroup) of a function out of the branch's reach, as Black's machine_use.c has after SystemInit,
    func_020053ec and func_02005440 for their calls to the DSi's power management in the LTD autoload. A Thumb
    caller's veneer is `ldr pc, [pc, #-4]` and the target (8 bytes) to an ARM function, `bx pc; nop` and `ldr ip, [pc];
    bx ip` and the target (16) to a Thumb one; an ARM caller's 8 to ARM, 12 (`ldr ip, [pc]; bx ip`) to Thumb. Where the
    target is, Black's symbols say; one they don't name is taken to be in reach."""
    out = {}
    for i, rows in relocs.items():
        if i >= len(secs) or secs[i][0] not in CODE:
            continue
        for off, kind, s, add in rows:
            if kind not in (R_ARM_PC24, R_ARM_THM_CALL, R_ARM_CALL) or syms[s][1]:
                continue  # not a call, or a call of the object's own function
            thumb = kind == R_ARM_THM_CALL
            for addr, arm in kinds.get(syms[s][0], [])[:1]:
                if abs(addr - (start + off)) >= REACH["thumb" if thumb else "arm"]:
                    out[i] = out.get(i, 0) + (8 if arm else 16 if thumb else 12)
    return out


def place_text(secs, start, extra):
    """place() of the .text sections from start, with extra bytes ({section: bytes}, veneers) after some: a function's
    veneers start at the 4-aligned end of its code."""
    at, out = start, {}
    for i, (name, kind, _, size, align) in enumerate(secs):
        if kind in (1, 8) and name in CODE:
            out[i] = -(-at // max(4, align)) * max(4, align)
            at = out[i] + size if i not in extra else -(-(out[i] + size) // 4) * 4 + extra[i]
    return out


def pieces(secs, sec, start):
    """The object's pieces of a section placed from start: ({section index: address}, the end)."""
    at = place(secs, {sec: (start, start)})
    return at, max((at[i] + secs[i][3] for i in at), default=start)


def differs(d, secs, relocs, at, rom, base):
    """The first address where the placed pieces' bytes aren't the ROM's, relocated words aside, or None."""
    for i, a in sorted(at.items(), key=lambda x: x[1]):
        _, kind, off, size, _ = secs[i]
        if kind != 1:  # NOBITS
            continue
        if a - base + size > len(rom):
            return a
        masked = {o + k for o, _, _, _ in relocs.get(i, ()) for k in range(4)}
        k = next((k for k in range(size) if k not in masked and d[off + k] != rom[a - base + k]), None)
        if k is not None:
            return a + k
    return None


def used_elsewhere(name, dest):
    """Does a file in src/, lib/ or include/ other than dest name it?"""
    for d in ("src", "lib", "include"):
        for p in glob.glob(f"{ROOT}/{d}/**/*", recursive=True):
            if p != f"{ROOT}/{dest}" and re.search(r"\.(c|h|inc)$", p) and re.search(rf"\b{name}\b", open(p).read()):
                return True
    return False


def section_names(d):
    """An object's section names as the ELF has them (read_object() names .ltdmain.bss .bss)."""
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", d, 0x2E)
    sh = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    at = sh[shstrndx][4]
    return [d[at + s[0]:d.index(b"\0", at + s[0])].decode(errors="replace") for s in sh]


def has_alias(r, dest):
    """Whether another module names a part of the file by an alias (tools/two_modules.py: card_rom@ltd.c)."""
    return any(real_of(f) == dest for _, files in r.delinks.values() for f in files)


def one_module(d, secs, alias):
    """secs with what the other module holds of a file in two modules named ".elsewhere": for its alias (alias
    True) all but the twl/ltdmain_begin.h block's sections (.ltdmain and .ltdmain.bss), for the file the block's."""
    raw = section_names(d)
    return [(".elsewhere",) + s[1:] if s[0] in CODE + SECTIONS and (raw[i] in LTD_BLOCK) != alias else s
            for i, s in enumerate(secs)]


def fit(r, mod, dest):
    """For the file's object: ({section: (start, end)}, the problems that stop it, {C name: Black's name} of the
    functions the .text takes past its end, [data definitions to drop]). dest may be an alias of a file in two
    modules (tools/two_modules.py): its part in this module is fitted, the file's object's .ltdmain block."""
    header, files = r.delinks[mod]
    old = files[dest]
    alias = real_of(dest) is not None
    obj = f"build/b_us/{os.path.splitext(real_of(dest) or dest)[0]}.o"
    stop = lambda *why: (None, list(why), {}, [], [])  # noqa: E731
    other = [sec for sec in old if sec not in CODE and sec not in SECTIONS]
    if other:
        return stop(f"its entry has a {other[0]} range, which this doesn't place")
    if run(NINJA + [obj])[0]:
        return stop("it doesn't compile")
    d, secs, syms, relocs = read_object(f"{ROOT}/{obj}")
    if alias or has_alias(r, dest):
        secs = one_module(d, secs, alias)
    rom, base = module_bytes(r, mod)
    black_any = {fr: (to + add, tm) for fr, to, add, tm, _ in r.relocs[mod]}
    black = {fr: to for fr, (to, tm) in black_any.items() if tm == mod}
    black_where = read_names(CONFIG, labels=True)[1]
    sizes = symbol_sizes(mod)
    # a global function Black lacks is one the link drops (tools/dead_strip.py), with its section
    secs = [(".dropped",) + s[1:] if any(f and g and i == k and n not in black_where for n, k, v, sz, g, f in syms)
            else s for i, s in enumerate(secs)]
    # and a static only dropped code calls, which a dead-stripping link drops with it (enc_pokeset.c's createNPkm,
    # which only pokebw2's makeNPokeFromData calls)
    # (also a .bss section of its own, an object of 64 bytes or more, which only dropped code reaches: mb_gameinfo.c's
    # mbrs and bssDescbuf, which only the child's receive code uses; and a .rodata or .data section: mb_wm_base.c's
    # bss_fill of MBi_ScanUnlock, which only the dropped MB_StartChild calls, dwc_mov_wh.c's tables and strings of the
    # dropped ChildConnectAuto chain, each in a .data section of its own; but not one holding a name Black has)
    while True:
        refs = {}
        for i, rows in relocs.items():
            if i < len(secs) and not secs[i][0].startswith(".debug"):
                for _, _, s, _ in rows:
                    if syms[s][1] != i:
                        refs.setdefault(syms[s][1], set()).add(i)
        more = [k for k, s in enumerate(secs) if s[0] in CODE + DATA and refs.get(k) and all(
            secs[j][0] == ".dropped" for j in refs[k]) and not any(
            (f or g) and i == k and n in black_where for n, i, v, sz, g, f in syms)]
        # (and a data section nothing but the debug info reaches, without a global, which tools/dead_strip.py would
        # force in: a function's static the compiler folded away, gx.c's GX_Init bg_mtx_elem_one, nsbma.c's cmask0/1,
        # mi_ndma.c's MIi_CardNDmaRecv32 size; the link drops it, as Black's did)
        more += [k for k, s in enumerate(secs) if s[0] in DATA and s[1] in (1, 8) and s[3] and not refs.get(k)
                 and any(i == k for n, i, *_ in syms) and not any(g and i == k for n, i, v, sz, g, f in syms)]
        if not more:
            break
        secs = [(".dropped",) + s[1:] if k in more else s for k, s in enumerate(secs)]

    # The functions where Black has them; past the range's end, Black's that no other file's code covers
    found, renames, at = {}, {}, {}
    code = code_section(old)  # .text, or a TwlSDK LTD file's .ltdmain
    if code is None:
        # a file of data only (enceff_data.c, fldmmdl_movedata.c, dsprot's extra.c): other files' code reaches it
        start = end = None
        if any(s[0] in CODE and s[3] for s in secs):
            return stop("its object has code, but its entry no .text")
    else:
        start, end = old[code]
        funcs = {a: (s, n) for a, s, n in r.funcs[mod]}
        at = place(secs, {code: old[code]})
        extra = veneers(secs, syms, relocs, start, function_kinds())
        if extra:
            at.update(place_text(secs, start, extra))
        placed = {at[i] + (v & ~1): (n, s) for n, i, v, s, _, f in syms if f and s and i in at}
        others = [x[code_section(x)] for f, x in files.items() if f != dest and code_section(x)]
        past = {a: (n, s) for a, (n, s) in placed.items() if a >= end and funcs.get(a, (0,))[0] == s
                and not any(x <= a < y for x, y in others)}
        moved = [f"{n} at {a:#x}" for a, (n, s) in placed.items() if a not in past and funcs.get(a, (0,))[0] != s]
        moved += [f"none at {a:#x}" for a in funcs if start <= a < end and a not in placed]
        if moved:
            return stop(f"functions not where Black has them, one missing or of another size: {', '.join(moved[:4])}")
        # and Black's code (compiler_probe.py compares only the range's functions)
        other_code = [f"{n} is the size of Black's {funcs[a][1]}, not its code (at {x:#x})"
                      for a, (n, _) in past.items() for i in [next(i for m, i, v, s, _, f in syms if m == n and f)]
                      for x in [differs(d, secs, relocs, {i: at[i]}, rom, base)] if x is not None]
        if other_code:
            return stop(*other_code)
        renames = {n: funcs[a][1] for a, (n, _) in past.items() if n != funcs[a][1]}
        taken = [n for n in renames if n in black_where]
        if taken:
            return stop(f"functions past the .text's end under other Black functions' names: {', '.join(taken)}")
        if past:
            last = max(a + s for a, (_, s) in past.items())
            end = min([-(-last // 4) * 4] + [a for a in funcs if a >= last])
        found[code] = (start, end)

    problems, drops = [], []
    todo = [s for s in SECTIONS if any(n == s and k in (1, 8) and size for n, k, _, size, _ in secs)]
    while todo:
        # The sections placed so far vote for the start of each other one with their relocations into it
        votes = {}
        for sec in todo:
            def vote(lay, base=0):
                return Counter(black[at[i] + off] - (lay[ndx] - base + syms[s][2] + (add if kind == 2 else 0))
                               for i, rows in relocs.items() if i in at for off, kind, s, add in rows
                               for ndx in [syms[s][1]] if ndx in lay and at[i] + off in black)
            votes[sec] = vote(pieces(secs, sec, 0)[0])
            # laid out from 0, a piece aligned to more than 4 lands where it would from a start of that alignment
            # (mb_gameinfo.c's 32-aligned bsendBuff at +0x40, not +0x2c, voted 0x21f6ee0 for 0x21f6ef4): each start
            # within that alignment of a vote, laid out from itself, is taken when all the relocations agree on it
            wide = max([secs[i][4] for i in pieces(secs, sec, 0)[0]] + [4])
            total = sum(votes[sec].values())
            if wide > 4 and votes[sec]:
                tried = sorted({c + k for c in votes[sec] for k in range(-wide + 4, wide, 4)})
                agree = [c for c in tried if vote(pieces(secs, sec, c)[0], c).get(c, 0) == total]
                if len(agree) == 1:
                    votes[sec] = Counter({agree[0]: total})
            if not votes[sec]:
                # a file without code, or a section only other files' code reaches (a library's globals: ghttpCommon.c's
                # GameSpy proxy, t35q): Black's addresses of its globals' names, which other files' code loads; the
                # check of what reaches its ranges below has the link put Black's names there
                lay0 = pieces(secs, sec, 0)[0]
                votes[sec] = Counter(a - lay0[i] - v for n, i, v, size, g, f in syms if i in lay0 and g and size
                                     and not f for m, a in black_where.get(n, ()) if m == mod)
        sec = next((s for s in todo if votes[s]), todo[0])
        todo.remove(sec)
        s0, s1 = header.get(sec, (0, 0))
        known = [x[sec] for f, x in files.items() if f != dest and sec in x]
        # the files' order: after the data of the files whose code comes first, before the others'
        # (a file without code: between the files' ranges its old one lies between; the LTD autoload's files of
        # .text and of .ltdmain have no order by their code: only those with code in the file's section count)
        if start is not None:
            lo, lo_f = max([(x[sec][1], f) for f, x in files.items() if f != dest and sec in x
                            and code_section(x) == code and x[code][0] < start], default=(s0, "the section's start"))
            hi, hi_f = min([(x[sec][0], f) for f, x in files.items() if f != dest and sec in x
                            and code_section(x) == code and x[code][0] >= end], default=(s1, "the section's end"))
        elif sec not in old:
            (lo, lo_f), (hi, hi_f) = (s0, "the section's start"), (s1, "the section's end")
        else:
            o0, o1 = old[sec]
            lo, lo_f = max([(x[sec][1], f) for f, x in files.items() if f != dest and sec in x and x[sec][1] <= o0],
                           default=(s0, "the section's start"))
            hi, hi_f = min([(x[sec][0], f) for f, x in files.items() if f != dest and sec in x and x[sec][0] >= o1],
                           default=(s1, "the section's end"))

        def fits(a):
            """Why the section can't start at a, or None."""
            lay, e = pieces(secs, sec, a)
            if a < s0 or e > s1:
                return f"outside the module's {sec} ({s0:#x}-{s1:#x})"
            if not lo <= a or e > hi:
                return (f"out of the files' order: after {lo_f} ({lo:#x}) and before {hi_f} ({hi:#x}); is one of their "
                        f"ranges wrong?")
            other = next((f"{x:#x}-{y:#x}" for x, y in known if x < e and a < y), None)
            if other:
                return f"over another file's range {other}"
            bad = differs(d, secs, relocs, lay, rom, base) if sec != ".bss" else None
            if bad is not None:
                return f"the bytes differ from the ROM's at {bad:#x}"
            # each relocation where Black has one, to the same place when its target is placed
            for i, x in lay.items():
                for off, kind, s, add in relocs.get(i, ()):
                    ndx, to = syms[s][1], black_any.get(x + off)
                    where = at.get(ndx, lay.get(ndx))
                    if to is None and kind == R_ARM_ABS32 and word(rom, base, x + off) == link_value(
                            syms[s][0], None if where is None else where + syms[s][2], add, black_where, mod):
                        continue  # Black has none where the link writes a number (OVERLAY_ID(n)) or its address
                    if to is None or where is not None and to[1] == mod and \
                            (where + syms[s][2] + (add if kind == 2 else 0)) & ~1 != to[0] & ~1:
                        return f"a relocation differs from Black's at {x + off:#x}"
            return None

        if votes[sec]:
            a = votes[sec].most_common(1)[0][0]
            if len(votes[sec]) > 1:
                # our one "" (a zero byte) where Black has two, each used by other functions: two files
                empties = r.empty_strings(mod)
                twice = sorted({black[at[i] + off] for i, rows in relocs.items() if i in at for off, _, s, _ in rows
                                if at[i] + off in black and black[at[i] + off] in empties and syms[s][3] <= 4
                                and secs[syms[s][1]][0] == ".data"
                                and not any(d[secs[syms[s][1]][2] + syms[s][2]:][:syms[s][3]])})
                problems.append(f"{sec}: the relocations disagree on its start (its objects are in another order or "
                                f"section than Black's): " +
                                ", ".join(f"{x:#x} ({k})" for x, k in votes[sec].most_common(4)) +
                                ("" if len(twice) < 2 else f"; Black has {len(twice)} \"\" where the file has one, used by "
                                 + ", ".join(f"{empties[x][0]:#x}-{empties[x][-1]:#x}" for x in twice) +
                                 ": one per file, so the file is two (split it between them, as union_oneself.c was)"))
                continue
        elif sec == ".bss":
            problems.append(".bss: no relocation of the file's reaches it")
            continue
        else:
            places = [a for a in range(-(-lo // 4) * 4, hi, 4) if not fits(a)]
            if len(places) != 1:
                lay = pieces(secs, sec, 0)[0]
                used = {(syms[s][1], syms[s][2]) for i, rows in relocs.items() if not secs[i][0].startswith(".debug")
                           for _, _, s, _ in rows}  # not the debug info's
                objs = [(n, i, v, g) for n, i, v, size, g, f in syms if i in lay and size and not f]
                # pokebw2's data, which Black lacks: nothing reaches it, here or elsewhere
                b2 = [n for n, i, v, g in objs if (i, v) not in used and n not in black_where
                      and not (g and used_elsewhere(n, dest))]
                if not places and objs and len(b2) == len(objs) and not any((i, 0) in used for i in lay):
                    drops += b2
                    continue
                problems.append(f"{sec}: no relocation reaches it, and its bytes are at {len(places)} places that fit"
                                + (f": {', '.join(map(hex, places[:4]))}" if places else ""))
                continue
            a = places[0]
        why = fits(a)
        if why:
            problems.append(f"{sec} at {a:#x}: {why}")
            continue
        lay, e = pieces(secs, sec, a)
        at.update(lay)
        e4 = -(-e // 4) * 4
        # dsd's data symbols have no size: the one the object ends in runs to the next symbol, and the range must
        # hold it. Only zeros up to the module section's end are padding; in .bss, which has no bytes, anything up to
        # the next symbol that no other file's range holds (sta_acting.c's 4-byte static runs 0x20)
        # .bss only when no other file's range follows in the module's .bss: the link lays the next file's object out
        # at the object's end, not the range's (nwm.c's to fatfs_api.c's 32-aligned .bss, 4 bytes on, and the build
        # failed): the range ends with the object, its last symbol gets a size (sized()) and holes() checks the
        # bytes left: alignment padding, or data the C lacks
        sym = r.syms[mod][bisect.bisect_right(r.syms[mod], e - 1) - 1]
        last = min(r.end_of(mod, sym, s1), sym + sizes.get(sym, s1))  # (a size symbols.txt gives ends it)
        if sec == ".bss" and last > e4 and not any(e <= x < s1 for x, _ in known):
            e4 = last
        elif sec == ".bss":
            pass
        elif last > e4 and last == s1 and not any(rom[e - base:s1 - base]) and not any(e <= x < s1 for x, _ in known):
            e4 = s1
        elif sec != ".bss" and last <= e4 < s1 and padding(e4, s1, (rom, base)) and not any(
                e <= x < s1 for x, _ in known) and not reached(r, mod, {sec: (e4, s1)}):
            # the last file's data: to the module section's end over its alignment padding (ir_check.c's .data, where
            # dsd has an unused data_ov231_02180a04), but not over what code reaches (fs_overlay_digest.c's
            # SDK_OVERLAY_DIGEST_END, the label of its end)
            e4 = s1
        elif last > e4 and sec != ".bss":
            problems.append(f"{sec}: Black's datum at {sym:#x} runs to {last:#x}, {last - e:#x} bytes past the object's "
                            f"end ({rom[e - base:min(last, e + 8) - base].hex(' ')}): data the C lacks")
            continue
        found[sec] = (a, e4)
    # Data in the old ranges that the file's code loads, which the new ones leave out: data the C only declares
    for sec, (s0, s1) in old.items():
        n0, n1 = found.get(sec, (0, 0))
        loads = sorted({to for fr, to, _, tm, _ in r.relocs[mod] if start is not None
                        and start <= fr < end and tm == mod and s0 <= to < s1 and not n0 <= to < n1})
        if loads and not problems:
            problems.append(f"the file's code loads data in its old {sec} {s0:#x}-{s1:#x} that its object lacks (at "
                            f"{', '.join(map(hex, loads[:3]))}): the C declares them without defining them")
    # What code and data outside the file reach in its ranges must be its globals, by Black's names. A relocation
    # dsd couldn't tell between overlays (module:overlays(182,224)), which it links to the first, into the middle of
    # one of the file's objects, or to a static, is another overlay's: it gets the others
    # (a weak definition is a global to the link: std_sprintf.c's SDK_WEAK_SYMBOL STD_TSPrintf, bound 14)
    tab = next((s for s in secs if s[1] == 2), None)  # SYMTAB: the symbols' entries in their order
    weak = {k for k in range(len(syms)) if tab and d[tab[2] + 16 * k + 12] >> 4 in (2, 14)}
    starts = {(at[i] + v) & ~1: (n, g or k in weak) for k, (n, i, v, s, g, f) in enumerate(syms)
              if i in at and (s or f)}
    black_at, theirs = read_names(CONFIG)[0], ambiguous(mod, found)
    in_data = lambda m, a: any(s <= a < e for sec, (s, e) in r.delinks[m][0].items() if sec in (".rodata", ".data"))
    fixes = []
    # (a file in two modules reaches its statics from its part in the other one: main's CARDi_GetDmaInterface takes
    # card_utility.c's LTD CARDiDmaUsingNew, tools/two_modules.py)
    twin = [(m, s, e) for m, (_, fs) in r.delinks.items() for f, x in fs.items() if f != dest
            and (real_of(f) or f) == (real_of(dest) or dest) for s, e in x.values()]
    for a, refs in sorted(reached(r, mod, found).items()) if not problems else ():
        refs = [(m, fr) for m, fr in refs if not any(m == tm and s <= fr < e for tm, s, e in twin)]
        if not refs:
            continue
        name = black_at.get((mod, a)) or black_at.get((mod, a | 1))
        if not name or starts.get(a) == (name, True):
            continue
        # (these two first: a relocation dsd linked to the file's overlay, into the middle of a big global of it, is
        # still another overlay's, and one from data a false pointer; the check of a field pointer below skipped them,
        # the dry run said the ranges fit and the link failed with Undefined)
        # to the middle of one of its objects or to one of its statics, which no other module can reach, from
        # relocations dsd couldn't tell between overlays: they are the other overlays'
        if (a not in starts or not starts[a][1]) and len(theirs.get(a, ())) == len(refs):
            fixes += theirs[a]
            continue
        # data pointing into the middle of an object is dsd reading a byte table as pointers, as port_file.py drops
        if a not in starts and all(in_data(m, fr) for m, fr in refs):
            fixes += [(f"{CONFIG}/{module_dir(m)}/relocs.txt", fr, None) for m, fr in refs]
            continue
        # into the middle of one of the file's global objects (a pointer to a field): code outside reaches it as
        # symbol+offset, which the link check compares with Black's
        if a not in starts and any(at[i] + v < a < at[i] + v + s for n, i, v, s, g, f in syms if i in at and g and s):
            continue
        problems.append(f"{name} at {a:#x}, which {refs[0][0]} reaches from {refs[0][1]:#x}, is " +
                        (f"the file's {'global' if starts[a][1] else 'static'} {starts[a][0]}" if a in starts else
                         "in the middle of one of the file's objects"))
    return found, problems, renames, drops, fixes


def symbol_sizes(mod):
    """{address: size} of the module's symbols that symbols.txt gives a size (kind:bss(size=0x8))."""
    out = {}
    for line in open(f"{CONFIG}/{module_dir(mod)}/symbols.txt"):
        m = re.search(r"kind:\w+\([^)]*size=(0x\w+)[^)]*\) addr:(0x\w+)", line)
        if m:
            out[int(m[2], 16)] = int(m[1], 16)
    return out


def sized(r, mod, found):
    """[(address, size)]: the symbol a .bss range ends in, which has no size in symbols.txt and runs to a next symbol
    past the range's end, gets one up to the end, as the range cuts it there (nwm.c's data_027341cc, 0x210: four
    bytes of padding before fatfs_api.c's 32-aligned .bss)."""
    if ".bss" not in found:
        return []
    a, e = found[".bss"]
    syms = r.syms[mod]
    sym = syms[bisect.bisect_right(syms, e - 1) - 1] if syms else None
    if sym is None or sym < a or sym in symbol_sizes(mod) or r.end_of(mod, sym, 1 << 32) <= e:
        return []
    return [(sym, e - sym)]


def give_sizes(mod, sizes):
    """Writes sized()'s sizes into the module's symbols.txt (kind:bss -> kind:bss(size=N)). Returns {path: text
    before}."""
    path = f"{CONFIG}/{module_dir(mod)}/symbols.txt"
    text = before = open(path).read()
    for addr, size in sizes:
        text = re.sub(rf"^(\S+ kind:bss) (addr:{addr:#010x})\b", lambda m: f"{m[1]}(size={size:#x}) {m[2]}", text,
                      flags=re.M)
    open(path, "w").write(text)
    return {path: before}


def padding(at, end, rom=None):
    """Whether the bytes from at to end are the link's alignment padding before an object at end: fewer than end's
    alignment (a power of two, up to 32), and zeros in the ROM if its bytes are given (module_bytes())."""
    return end - at < min(end & -end, 32) and (rom is None or not any(rom[0][at - rom[1]:end - rom[1]]))


def holes(r, mod, dest, old, found):
    """[(section, start, end)]: the bytes the file's old ranges (old) cover that its new ones (found) leave in no
    file's range, but for alignment padding. A completion must not leave any: fit() moved gsPlatformUtil.c's .bss
    start 44 bytes on (0x021ad138 -> 0x021ad164, a dropped function's static), past bytes no file has, and every
    check passed."""
    files = r.delinks[mod][1]
    try:
        rom = module_bytes(r, mod)
    except (OSError, KeyError):
        rom = None
    out = []
    for sec, (a, b) in old.items():
        new = found.get(sec)
        for x, y in [(a, b)] if new is None else [(a, min(b, new[0])), (max(a, new[1]), b)]:
            if x >= y:
                continue
            covered = sorted((max(x, s), min(y, e)) for f, rs in files.items() if f != dest and sec in rs
                             for s, e in [rs[sec]] if s < y and e > x)
            at = x
            for s, e in covered + [(y, y)]:
                if s > at and not padding(at, s, None if sec == ".bss" else rom):
                    out.append((sec, at, s))
                at = max(at, e)
    return out


def ambiguous(mod, ranges):
    """{address: [(relocs.txt, from, the other overlays)]} of the relocations to the file's ranges that dsd couldn't
    tell between overlays (module:overlays(a,b)), whose first is the file's module."""
    out = {}
    if not re.match(r"ov\d+$", mod):
        return out
    for path in glob.glob(f"{CONFIG}/**/relocs.txt", recursive=True):
        for fr, to, ovs in re.findall(r"^from:(0x\w+) .*to:(0x\w+) module:overlays\(([\d,]+)\)", open(path).read(),
                                      re.M):
            ovs = [int(x) for x in ovs.split(",")]
            if ovs[0] == int(mod[2:]) and any(s <= int(to, 16) < e for s, e in ranges.values()):
                out.setdefault(int(to, 16) & ~1, []).append((path, int(fr, 16), ovs[1:]))
    return out


def disambiguate(fixes):
    """Gives each relocation of fixes its other overlays in its relocs.txt, or drops it (no overlays: a false pointer).
    Returns {path: its text before}."""
    before = {}
    for path, fr, ovs in fixes:
        text = open(path).read()
        before.setdefault(path, text)
        if ovs is None:
            text = re.sub(rf"^from:{fr:#010x} .*\n", "", text, flags=re.M)
        else:
            new = f"module:overlay({ovs[0]})" if len(ovs) == 1 else f"module:overlays({','.join(map(str, ovs))})"
            text = re.sub(rf"^(from:{fr:#010x} .*)module:overlays\([\d,]+\)", lambda m: m[1] + new, text, flags=re.M)
        open(path, "w").write(text)
    return before


def drop(path, names):
    """Removes the definitions of data, with the comment lines above them."""
    text = open(path).read()
    for name in names:
        found = definition(text, name)
        if found:
            start = found[0]
            while start and text[text.rfind("\n", 0, start - 1) + 1:start].startswith("//"):
                start = text.rfind("\n", 0, start - 1) + 1
            text = text[:start] + text[found[2]:].lstrip("\n")
    open(path, "w").write(text)


def rename(renames, dest):
    """Renames the file's functions everywhere in src/, lib/ and include/ (whole words), with pokebw2's name over the
    definition unless a placeholder. Returns {path: its text before}."""
    word, before = re.compile(r"\b(" + "|".join(map(re.escape, renames)) + r")\b"), {}
    for d in ("src", "lib", "include"):
        for path in glob.glob(f"{ROOT}/{d}/**/*", recursive=True):
            if re.search(r"\.(c|h|inc|s)$", path):
                text = open(path).read()
                new = word.sub(lambda m: renames[m[0]], text)
                if new != text:
                    before[path] = text
                    open(path, "w").write(new)
    path = f"{ROOT}/{dest}"
    text = comment_definitions(open(path).read(), {b: n for n, b in renames.items() if not PLACEHOLDER.match(n)})
    open(path, "w").write(text)
    return before


def inner_symbols(dest):
    """Runs tools/inner_symbols.py --apply on the file, now complete, at its new ranges (--placed: the link map has
    its old ones, t37e's un_select.c): the dsd symbols inside its data objects go (Undefined at the link otherwise),
    also those of the relocations disambiguate() dropped (snd_sys.c's 104 in g_GFLSndHeap), and relocations into
    them become to:<object> add:N. Returns {path: text before} of the symbols.txt and relocs.txt files it wrote."""
    paths = glob.glob(f"{CONFIG}/**/symbols.txt", recursive=True) + glob.glob(f"{CONFIG}/**/relocs.txt", recursive=True)
    before = {p: open(p).read() for p in paths}
    code, out = run([sys.executable, "tools/inner_symbols.py", "--apply", "--placed", dest, dest])
    changed = {p: t for p, t in before.items() if open(p).read() != t}
    for line in out.splitlines() if code == 0 and changed else ():
        if line.startswith("  ") and not line.startswith("  config/"):
            print(f"inner_symbols: {line.strip()}")
    return changed


def first_difference(r, mod):
    """Where the built module first differs from the ROM's, and the symbol before it."""
    built, rom = module_bytes(r, mod, built=True)[0], module_bytes(r, mod)[0]
    if built == rom:
        return f"{mod} is the ROM's: another module differs"
    base = module_bytes(r, mod)[1]
    k = next((k for k in range(min(len(built), len(rom))) if built[k] != rom[k]), min(len(built), len(rom)))
    names = sorted((a, n) for (m, a), n in read_names(CONFIG)[0].items() if m == mod and a <= base + k)
    return (f"{mod} differs first at {base + k:#x} ({names[-1][1] if names else '?'}); {len(built):#x} bytes, the "
            f"ROM's {len(rom):#x}")


def one(dest, bw2, dry_run):
    """Fits one file. Returns 0 if it is complete and ninja passes (or, dry, if it fits)."""
    r = Ranges(bw2, CONFIG, f"{ROOT}/build/twins_b_us.tsv")
    mod = next((m for m, (_, files) in r.delinks.items() if files.get(dest)), None)
    if mod is None:
        print(f"FAILED {dest}: no range in a delinks.txt")
        return 1
    old = r.delinks[mod][1][dest]
    path = f"{ROOT}/{real_of(dest) or dest}"  # (an alias's C is its file's, tools/two_modules.py)
    before = {path: open(path).read()}
    found, problems, renames, drops, fixes = fit(r, mod, dest)
    if drops and not problems:
        print(f"data: {', '.join(drops)} dropped, which Black doesn't have")
        if dry_run:
            return 0
        drop(path, drops)
        found, problems, renames, drops, fixes = fit(r, mod, dest)
    if problems or drops:
        open(path, "w").write(before[path])
        print(f"FAILED {dest}: " + "; ".join(problems or [f"data Black lacks: {', '.join(drops)}"]))
        return 1
    left = holes(r, mod, dest, old, found)
    if left:
        open(path, "w").write(before[path])
        print(f"FAILED {dest}: the new ranges leave " + ", ".join(f"{s} {a:#010x}-{b:#010x} ({b - a} bytes)"
                                                                 for s, a, b in left)
              + " in no file: the C lacks data the ROM has there (or they are another file's: give it a range first)")
        return 1
    fmt = lambda rs: " ".join(f"{s}:{a:#010x}-{b:#010x}" for s, (a, b) in rs.items())  # noqa: E731
    print(f"{dest}: {fmt(old)} -> {fmt(found)}" + (f"; renamed {', '.join(f'{a} -> {b}' for a, b in renames.items())}"
                                                  if renames else ""))
    for p, fr, ovs in fixes:
        print(f"relocs: {os.path.relpath(p, ROOT)}'s from:{fr:#010x} " +
              (f"is overlay {', '.join(map(str, ovs))}'s, not {mod}'s" if ovs else "is a table's bytes, dropped") +
              ": it points into the middle of one of the file's objects, or to a static")
    sizes = sized(r, mod, found)
    for addr, size in sizes:
        print(f"symbols: {mod}'s {addr:#010x} gets size={size:#x}, where the .bss range ends")
    if dry_run:
        return 0
    if renames:
        before = {**rename(renames, dest), **before}
    before = {**disambiguate(fixes), **give_sizes(mod, sizes), **before}
    delinks = f"{CONFIG}/{module_dir(mod)}/delinks.txt"
    before[delinks] = open(delinks).read()
    run([sys.executable, "tools/add_source_file.py", dest, module_dir(mod), *fmt(found).split(), "--incomplete"])
    run([sys.executable, "tools/add_source_file.py", dest, module_dir(mod)])
    before = {**inner_symbols(dest), **before}
    run([sys.executable, "configure.py"])
    code, out = run(NINJA)
    if code:
        shown = [l for l in out.splitlines() if not l.startswith(("[", "FAILED:", "   ")) and any(
            w in l for w in ("ndefined", "panicked", "rror", "didn't pass"))]
        why = first_difference(r, mod) if "Checking modules" in out else (shown or out.splitlines()[-1:])[0]
        for p, text in before.items():
            open(p, "w").write(text)
        run([sys.executable, "configure.py"])
        print(f"FAILED {dest}: the ranges fit, but not the build: {why[:200]}")
        return 1
    # its rows in docs/nonmatching.md, which said why it wasn't complete
    nm = f"{ROOT}/docs/nonmatching.md"
    lines = open(nm).read().split("\n")
    open(nm, "w").write("\n".join(l for l in lines if f"| `{dest}` |" not in l))
    print(f"MATCHED {dest}: complete, and ninja passes")
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="+", help="C files in a delinks.txt, e.g. src/ov225/manual.c")
    ap.add_argument("--dry-run", action="store_true", help="print what it would do, change nothing")
    ap.add_argument("--bw2", default=os.path.expanduser("~/ref/pokebw2"), help="pokebw2 checkout (read only)")
    args = ap.parse_args()
    return 1 if sum(one(dest, args.bw2, args.dry_run) for dest in args.files) else 0


if __name__ == "__main__":
    sys.exit(main())
