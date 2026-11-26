#!/usr/bin/env python3
"""HeartGold/SoulSilver's data still in assembly (docs/hgss.md, "asm data") as one relocatable object for the PC build.

Our decompilation keeps 4,395 of the .s objects main.lsf.in links as data only (the 133 with functions have C in
port/nonmatching). They are mwasm syntax for the DS, 32-bit; the host assembles them the way Platinum's battle AI script is
(port/meson.build: the build's C compiler, preprocessor first), after host(): the per-file .include lists inlined (a missing
one, a list pret's build writes, dropped), `;` comments cut, .public as .global, .extern dropped, .rodata/.text/.sinit
as .data.rel.ro (a pointer in it is relocated at load, as clang does for a const table of pointers), .align as .p2align (the
ARM's power of two), and each .word item 4 bytes (.long), or 8 (.quad) when it names a symbol: a pointer. A table that
mixes numbers and pointers then has the host's pointer size but not C's padding: such a table is the HGSS side's to
turn into C (the run finds it). A .space in .bss is twice its DS size: the C stores host pointers there (a .space 0x40
of 16 pointers needs 0x80; hg10: the particle systems' table overran the next symbol). Files that don't assemble are
listed and left out: their symbols are the link's undefined.

    python3 tools/pokehg_asm.py --cc 'clang --target=x86_64-linux-gnu' -o build/port/linux-hgss/hg_data.o
    python3 tools/pokehg_asm.py --list            # the data .s files, in main.lsf.in's order
"""
import argparse, os, re, shlex, subprocess, sys, tempfile
from concurrent.futures import ThreadPoolExecutor

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pokehg_cc  # noqa: E402

HG = pokehg_cc.HG
FUNC = re.compile(r"\b(thumb_func_start|arm_func_start|glabel|non_word_aligned_thumb_func_start)\b")
INC = re.compile(r'^\s*\.include\s+"([^"]+)"', re.M)
SYM = re.compile(r"[A-Za-z_.$][A-Za-z0-9_.$]*")


def files(root=HG):
    """main.lsf.in's .s objects that hold no function."""
    lsf = open(os.path.join(root, "main.lsf.in")).read()
    out = []
    for f in re.findall(r"Object\s+(\S+\.s)\b", lsf):
        p = os.path.join(root, f)
        if os.path.exists(p) and not FUNC.search(open(p, errors="replace").read()):
            out.append(f)
    return out


def cut_comment(line):
    """The line before a `;` that is not in a string (mwasm's comment; GNU as for x86 takes `;` as a separator)."""
    q = False
    for i, c in enumerate(line):
        if c == '"' and (i == 0 or line[i - 1] != "\\"):
            q = not q
        elif c == ";" and not q:
            return line[:i].rstrip()
    return line


def items(args):
    """A directive's operands, split at the commas outside parentheses."""
    out, depth, cur = [], 0, ""
    for c in args:
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        if c == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += c
    return out + [cur.strip()] if cur.strip() else out


def word(item):
    """.quad for an operand that names a symbol (a pointer; a macro argument stays 4 bytes), else .long."""
    bare = re.sub(r"\b0[xX][0-9a-fA-F]+\b|\b\d+\b", "", item)
    return ".quad" if "\\" not in item and SYM.search(bare) else ".long"


# Data the port defines in C, in the host's layout (a hunk of port/subprojects/pokeheartgold-zhostfix.patch each): a DS
# record holding a pointer can't be read through the C's struct on the host (an 8-byte pointer, other offsets). Its
# label here becomes local (NAME__ds) and its .global is dropped, so the C's definition is the one the link takes; the
# patch never carries the ROM's bytes (rom_leak_check). The decomp's port/asmdata/LABELS.txt (hk73, cc952ee13 on): the
# labels whose data port/asmdata/*.c (PC-only typed copies, port/meson.build compiles them) defines in host layout, and
# HAND_C_DATA: ours (zhostfix C), and labels upstream's C defines under SDK_PORT || NONMATCHING without a LABELS.txt
# line (hgss 19502d33a, hk284: five .bss words; the asm's .bss label would be a duplicate symbol at the link).
# (hg11's window fades' block is C upstream since 28a2a022f: no asm label left.)
HAND_C_DATA = {"_021D10A0", "_021D10A8", "_021D2208", "_021D4134", "_021D4140"}


def c_data(root):
    p = os.path.join(root, "port/asmdata/LABELS.txt")
    return HAND_C_DATA | (set(open(p).read().split()) if os.path.exists(p) else set())


C_DATA = c_data(HG)


def asmdata_refs(root):
    """Labels port/asmdata/*.c (upstream's PC-only typed copies) declare extern: some are local in their .s (a table's
    strings, crash 43 hg24: ov01_02206450's five path strings had no .public, the link took undefined_stubs' function).
    host() makes them global. Upstream's generator lists exactly those (hgss-port d65712bbb on: port/asmdata/GLOBALS.txt,
    170 labels, hk151): that list when it is there, else every extern a port/asmdata/*.c declares (hg24: 2186)."""
    d = os.path.join(root, "port/asmdata")
    g = os.path.join(d, "GLOBALS.txt")
    if os.path.isfile(g):
        return set(open(g).read().split()) - C_DATA
    refs = set()
    for f in (os.listdir(d) if os.path.isdir(d) else []):
        if f.endswith(".c"):
            refs |= set(re.findall(r"^extern [^;(]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*\s*;", open(os.path.join(d, f), errors="replace").read(), re.M))
    return refs - C_DATA


ASMDATA_REFS = asmdata_refs(HG)


def host(text, dirs, seen=None):
    """The DS's data assembly as the host's (the docstring's rules); dirs: where .include looks."""
    seen = set() if seen is None else seen
    out = []
    bss = False
    for line in text.splitlines():
        line = cut_comment(line)
        lab = line.strip()
        if lab.endswith(":") and lab[:-1] in C_DATA:
            out.append(lab[:-1] + "__ds:")
            continue
        if lab.endswith(":") and lab[:-1] in ASMDATA_REFS:
            out.append("\t.global " + lab[:-1])
        if line.split()[:1] in ([".global"], [".public"]) and line.split()[1:] and line.split()[1] in C_DATA:
            continue
        m = INC.match(line)
        if m:
            p = next((os.path.join(d, m[1]) for d in dirs if os.path.exists(os.path.join(d, m[1]))), None)
            if p and p not in seen:
                seen.add(p)
                out.append(host(open(p, errors="replace").read(), dirs, seen))
            continue
        w = line.split(None, 1)
        d = w[0] if w else ""
        rest = w[1] if len(w) > 1 else ""
        if d in (".bss", ".data", ".rodata", ".text", ".section"):
            bss = d == ".bss" or (d == ".section" and rest.split(",")[0].strip().startswith(".bss"))
        if bss and d in (".space", ".skip"):
            out.append(f"\t{d} ({rest.strip()})*2")
            continue
        if d == ".public":
            out.append("\t.global " + rest)
        elif d in (".extern", ".thumb", ".arm") or d.startswith(".code"):
            continue
        elif COFF and d in (".type", ".size"):
            # ELF's symbol type and size: COFF has no .size, and its .type wants a number (win64: hwin1 found two
            # files left out, the move-category and type-icon tables then zeroed blocks of the link)
            continue
        elif d in (".rodata", ".text") or line.split(",")[0].split() == [".section", ".sinit"]:
            # (.sinit: overlay 12's static initialiser table; kept as data, nothing runs it yet)
            out.append('\t.section .data.rel.ro,"aw"')
        elif d == ".align":
            out.append("\t.p2align " + rest)
        elif d in (".word", ".4byte"):
            out += [f"\t{word(x)} {x}" for x in items(rest)]
        elif d in (".byte", ".short", ".hword"):
            out.append(line.rstrip().rstrip(","))   # (mwasm takes a trailing comma)
        else:
            out.append(line)
    return "\n".join(out)


COFF = False   # main(): the target's objects are PE/COFF (win64)
MACHO = False   # main(): Mach-O (macOS, iOS): macho() below
LABEL = re.compile(r"^\s*([A-Za-z_.$][A-Za-z0-9_.$]*):\s*$")
SECT = (".data", ".section", ".bss", ".text", ".rodata")


def natural(text):
    """C's alignment for the host's pointers (hg13): each .quad 8-aligned, and a label whose data holds one starts
    8-aligned. A DS record {u32; pointer} (`.word 3` then `.word sub_X`) then has its pointer at 8, where C's struct
    reads it (map_object.c's UnkLMOCallbackStruct records, crash 14: unk4 read half of one pointer and half of the
    next). Left wrong: a record whose numbers come after a pointer and before the next record's pointer
    ({u32; ptr; u32}: C pads the end) -- `--mixed` lists those labels (column `ambiguous`).
    A label holding only pointers and `.word 0` is a pointer table with NULLs (or a terminator): its zeros are 8 bytes.
    A C_DATA label's local copy (NAME__ds: the C defines NAME) is left as host() wrote it: nothing reads it."""
    lines = text.split("\n")
    out = []
    zero = False
    ds = False
    for i, line in enumerate(lines):
        w = line.split()
        if LABEL.match(line) or w[:1] and w[0] in SECT:
            zero = False
            ds = bool(LABEL.match(line)) and LABEL.match(line)[1].endswith("__ds")
            if not LABEL.match(line) or ds:
                out.append(line)
                continue
            q = z = other = 0
            for n in lines[i + 1:]:
                s = n.split()
                if not s:
                    continue
                if LABEL.match(n) or s[0] in SECT:
                    break
                if s[0] == ".quad":
                    q += 1
                elif s[:2] in ([".long", "0"], [".long", "0x0"], [".long", "0x00000000"]):
                    z += 1
                elif s[0] in SIZES or s[0] in (".space", ".skip", ".ascii", ".asciz", ".incbin"):
                    other += 1
            if q:
                out.append("\t.p2align 3")
            zero = q >= 2 and z and not other
        elif ds:
            pass
        elif w[:1] == [".quad"] or zero and w[:1] == [".long"]:
            out.append("\t.p2align 3")
            if zero and w[0] == ".long":
                line = "\t.quad 0"
        out.append(line)
    return "\n".join(out)


SIZES = {".long": 4, ".quad": 8, ".short": 2, ".hword": 2, ".2byte": 2, ".byte": 1}


def layout(text):
    """Per label of a host() text: (label, DS bytes, pad bytes natural() adds, ambiguous) for the labels whose data
    mixes pointers (.quad) and numbers."""
    res, cur = [], None

    def close():
        if cur and cur["q"] and cur["n"]:
            res.append((cur["name"], cur["ds"], cur["pad"], cur["amb"]))
    for line in text.split("\n"):
        m = LABEL.match(line)
        s = line.split(None, 1)
        if m or (s and s[0] in SECT):
            close()
            cur = {"name": m[1], "off": 0, "ds": 0, "pad": 0, "q": 0, "n": 0, "amb": False, "run": None, "lead": 0} if m else None
            continue
        if not cur or not s or s[0] not in SIZES:
            if cur and s and s[0] in (".space", ".skip", ".ascii", ".asciz", ".incbin"):
                cur["n"] += 1   # (sizes not followed: the offsets after it are approximate)
            continue
        k = len(items(s[1])) if len(s) > 1 else 1
        if s[0] == ".quad":
            p = -cur["off"] % 8
            if cur["run"] is None:
                cur["lead"] = cur["off"]   # the numbers before the first pointer: a record starting with them
            elif cur["lead"] and cur["run"] and (cur["run"] - cur["lead"]) % 8:
                cur["amb"] = True   # {u32; ptr; u32}[]: C pads after the trailing u32, natural() does not
            cur["pad"] += p
            cur["off"] += p + 8 * k
            cur["ds"] += 4 * k
            cur["q"] += k
            cur["run"] = 0
        else:
            cur["off"] += SIZES[s[0]] * k
            cur["ds"] += SIZES[s[0]] * k
            cur["n"] += k
            if cur["run"] is not None:
                cur["run"] += SIZES[s[0]] * k
    close()
    return res


def mixed(cc, cpp_args, fl, out, root=HG):
    """--mixed: every asm data label mixing pointers and numbers, with the C declarations that read it (src/,
    port/nonmatching): the candidates for a DS-layout record read through a struct on the host."""
    rows = []
    for f in fl:
        src = os.path.join(root, f)
        r = subprocess.run(cc + ["-E", "-P", "-x", "assembler-with-cpp"] + cpp_args + [src], capture_output=True, text=True)
        if r.returncode:
            continue
        dirs = [os.path.dirname(src)] + [os.path.join(root, d) for d in ("asm", "asm/include", "include", ".")]
        rows += [(f,) + x for x in layout(natural(host(r.stdout, dirs)))]
    names = {r[1] for r in rows}
    decl = {}
    for d in ("src", "port/nonmatching", "include"):
        for dp, _, fs in os.walk(os.path.join(root, d)):
            for fn in fs:
                if fn.endswith((".c", ".h")):
                    p = os.path.join(dp, fn)
                    for ln in open(p, errors="replace"):
                        if "extern" in ln:
                            for nm in SYM.findall(ln):
                                if nm in names:
                                    decl.setdefault(nm, set()).add(" ".join(ln.split())[:120])
    with open(out, "w") as o:
        o.write("file\tlabel\tds_bytes\tpad_added\tambiguous\tC declarations\n")
        for f, nm, ds, pad, amb in rows:
            o.write(f"{f}\t{nm}\t{ds}\t{pad}\t{int(amb)}\t{' | '.join(sorted(decl.get(nm, ())))}\n")
    n_amb = sum(1 for r in rows if r[4])
    n_c = sum(1 for r in rows if r[1] in decl)
    print(f"pokehg_asm.py --mixed: {len(rows)} labels mix pointers and numbers ({sum(1 for r in rows if r[3])} padded by "
          f"natural(), {n_amb} ambiguous, {n_c} declared in C): {out}")


SET_NAME = re.compile(r"^\s*\.(?:set|equ)\s+([\w.$]+)\s*,")


def coff_globals(text):
    """PE/COFF: a `.global X` the file does not define is dropped. The .inc files declare the C's functions `.public`
    (overlay_12_battle_command.inc: battle_command.c's static InitBattleMsgData); ELF keeps such a name as an undefined symbol
    no relocation uses and its linker says nothing, PE's linker fails on it (win64, hwin1). A use still makes it an extern.
    A `.set X, Y` defines X too (tools/hg_assets.py's alias of a blanked label: dropping its .global left it undefined)."""
    lines = text.split("\n")
    defined = {m[1] for m in map(LABEL.match, lines) if m}
    defined |= {m[1] for m in map(SET_NAME.match, lines) if m}
    return "\n".join(l for l in lines
                     if not (l.split()[:1] == [".global"] and len(l.split()) == 2 and l.split()[1] not in defined))


MACHO_ID = re.compile(r"(?<![\w.$\\'\"])([A-Za-z_][\w$]*)(?![\w$])")
MACHO_LABEL = re.compile(r"^(\s*)([A-Za-z_$][\w$]*):")


def macho(text):
    """Mach-O (ld64): a C name X is the symbol _X, so every name the data defines or uses gets the underscore (labels,
    .global, .set, the operands of data directives and of macro calls; a macro's own \\arg is left alone, the strings too);
    ELF's .type/.size dropped; the sections as Mach-O names (.data.rel.ro: __DATA,__const; .bss: zerofill); every .quad
    8-aligned (a pointer fixup must be)."""
    out = []
    for line in text.split("\n"):
        m = MACHO_LABEL.match(line)
        if m:
            out.append(f"{m[1]}_{m[2]}:")
            line = line[m.end():]
        w = line.split(None, 1)
        d = w[0] if w else ""
        rest = w[1] if len(w) > 1 else ""
        if d in (".type", ".size"):
            continue
        if d == ".bss" or d == ".section" and rest.startswith(".bss"):
            out.append("\t.section __DATA,__bss,zerofill")
        elif d == ".section" and rest.startswith(".data.rel.ro"):
            out.append("\t.section __DATA,__const")
        elif d == ".section" and rest.startswith("."):
            out.append("\t.section __DATA,__" + re.split(r"[\s,]", rest[1:])[0][:14])
        elif d in (".macro", ".endm", ".include", ".incbin") or '"' in line:
            out.append(line)
        elif d:
            if d == ".quad":   # (Mach-O's fixups are 8-aligned: natural() leaves a C_DATA label's unread copy as it was)
                out.append("\t.p2align 3")
            out.append(line[:len(line) - len(line.lstrip())] + d + " " + MACHO_ID.sub(r"_\1", rest))
    return "\n".join(out)


def assemble(cc, cpp_args, f, obj, root=HG):
    """One .s to an object: (file, None) or (file, the first error line)."""
    src = os.path.join(root, f)
    r = subprocess.run(cc + ["-E", "-P", "-x", "assembler-with-cpp", "-MD", "-MF", obj + ".d"] + cpp_args + [src],
                       capture_output=True, text=True)
    if r.returncode:
        return f, (r.stderr.strip().splitlines() or ["cpp failed"])[0]
    dirs = [os.path.dirname(src)] + [os.path.join(root, d) for d in ("asm", "asm/include", "include", ".")]
    seen = set()   # (its .include files: --depfile)
    text = natural(host(r.stdout, dirs, seen))
    with open(obj + ".d", "a") as d:
        d.write("\n" + " ".join(os.path.abspath(x) for x in sorted(seen)) + "\n")
    if COFF:
        text = coff_globals(text)
    if MACHO:
        text = macho(text)
    r = subprocess.run(cc + ["-c", "-x", "assembler", "-", "-o", obj], input=text + "\n", capture_output=True, text=True)
    errs = [l for l in r.stderr.splitlines() if "error" in l]
    return f, (errs[0] if errs else "assembler failed") if r.returncode else None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--cc", default="cc", help="the build's C compiler with its target flags")
    ap.add_argument("-o", "--output", help="the relocatable object")
    ap.add_argument("--depfile", help="write its inputs (each data .s, what it #includes and .includes, main.lsf.in, "
                    "LABELS.txt) as a Makefile depfile: ninja rebuilds the object when a table changes (rp6: a re-pin "
                    "kept the vendored tree's blanked tables after the tree became the decomp's, with no fill)")
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--root", default=HG)
    ap.add_argument("--soulsilver", action="store_true")
    ap.add_argument("--mixed", metavar="OUT.tsv", help="list the labels mixing pointers and numbers (natural()'s doc)")
    a = ap.parse_args()
    global C_DATA
    C_DATA = c_data(a.root)
    fl = files(a.root)
    if a.mixed:
        own = pokehg_cc.own_args("SOULSILVER" if a.soulsilver else "HEARTGOLD")
        mixed(shlex.split(a.cc), [x for x in own if x.startswith(("-I", "-D"))] + ["-DPM_ASM"], fl, a.mixed, a.root)
        return
    if a.list or not a.output:
        print("\n".join(fl))
        return
    cc = shlex.split(a.cc)
    global COFF
    COFF = any("mingw" in x or "windows" in x for x in cc)   # win64's objects (PE/COFF), not ELF
    global MACHO
    MACHO = sys.platform == "darwin"   # (macOS and iOS: the build's compiler targets Apple there)
    own = pokehg_cc.own_args("SOULSILVER" if a.soulsilver else "HEARTGOLD")
    cpp_args = [x for x in own if x.startswith(("-I", "-D"))] + ["-DPM_ASM"]   # (pret's headers hide their C from it)
    with tempfile.TemporaryDirectory() as t:
        jobs = [(f, os.path.join(t, f"{i:05d}.o")) for i, f in enumerate(fl)]
        with ThreadPoolExecutor(int(os.environ.get("BW_JOBS", os.cpu_count() or 4))) as ex:
            res = list(ex.map(lambda j: assemble(cc, cpp_args, j[0], j[1], a.root), jobs))
        bad = [(f, e) for f, e in res if e]
        if a.depfile:
            deps = {os.path.abspath(os.path.join(a.root, p)) for p in ["main.lsf.in", "port/asmdata/LABELS.txt"] + fl
                    if os.path.exists(os.path.join(a.root, p))}
            for _, o in jobs:
                if os.path.exists(o + ".d"):
                    words = open(o + ".d").read().replace("\\\n", " ").split()
                    deps.update(w for w in words if not w.endswith(":") and os.path.isfile(w))
            esc = lambda x: x.replace("\\", "/").replace(" ", "\\ ")
            with open(a.depfile, "w") as d:
                d.write(esc(a.output) + ": " + " ".join(esc(x) for x in sorted(deps)) + "\n")
        with open(a.output + ".log", "w") as lg:
            lg.write("".join(f"{f}\t{e}\n" for f, e in bad))
        objs = [o for (f, o), (_, e) in zip(jobs, res) if not e]
        # the linker's overlay IDs (makelcf's SDK_OVERLAY_<name>_ID, the number as the symbol's address): data tables and
        # C bodies take their address (`(u32)&SDK_OVERLAY_OVY_98_ID`); absolute symbols here
        ids = os.path.join(t, "ids.o")
        names = re.findall(r"(?m)^Overlay\s+(\S+)", open(os.path.join(a.root, "main.lsf.in")).read())
        u = "_" if MACHO else ""
        src = "".join(f"\t.globl {u}SDK_OVERLAY_{n}_ID\n\t.set {u}SDK_OVERLAY_{n}_ID, {i}\n" for i, n in enumerate(names))
        if subprocess.run(cc + ["-c", "-x", "assembler", "-", "-o", ids], input=src, text=True).returncode:
            sys.exit("pokehg_asm.py: the overlay IDs did not assemble")
        objs.append(ids)
        lst = os.path.join(t, "objs.txt")
        open(lst, "w").write("\n".join(objs) + "\n")
        if subprocess.call(cc + ["-r", "-nostdlib", "-o", a.output, "@" + lst]):
            sys.exit("pokehg_asm.py: the relocatable link failed")
    print(f"pokehg_asm.py: {len(objs)} of {len(fl)} data files assembled" +
          (f"; the {len(bad)} left out: {a.output}.log" if bad else ""))


if __name__ == "__main__":
    main()
