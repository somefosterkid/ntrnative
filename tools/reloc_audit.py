#!/usr/bin/env python3
"""Relocations of our objects that name a symbol of the wrong overlay or the wrong kind.

    python3 tools/reloc_audit.py [src/ovNNN/ ...]

Overlays share addresses: dsd's relocs.txt lists, for a target address, the overlays it may be in (module:overlays(a,b)).
For each relocation in a compiled object of overlay M that names func_/data_ovN_ADDR (Black's name) this checks that
N is among those overlays, that the symbol's kind in N's symbols.txt (function / data) agrees with how the C declares
it (an `extern T name;` of a function symbol, a prototype of a data symbol) and with the relocation (a call to a
datum), and lists the ambiguous addresses where the name differs from the original object's (fix_undef.py's
choices) with what the other overlays there are. tools/fix_undef.py picks by kind now; this finds what it picked
before."""
import re, sys, pathlib, collections
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import fix_undef as fu

ROOT = fu.ROOT
NAME = re.compile(r"(func|data)_ov(\d+)_([0-9a-f]{8})$")

def modules(ov, cache={}):
    """{address (8 hex digits, thumb bit cleared): set of overlays} from overlay ov's relocs.txt."""
    if ov not in cache:
        d = collections.defaultdict(set)
        f = ROOT / f"config/b_us/arm9/overlays/ov{ov:03d}/relocs.txt"
        if f.exists():
            for l in f.read_text().splitlines():
                m = re.search(r"to:0x([0-9a-f]{8}) module:(.*)$", l)
                if m:
                    a = f"{int(m[1], 16) & ~1:08x}"
                    d[a] |= {int(k) for k in re.findall(r"\d+", m[2])} if not m[2].startswith("main") else set()
        cache[ov] = d
    return cache[ov]

def file_mods(ov, src, cache={}):
    """{target address (thumb bit cleared): set of overlays} over the relocations of overlay ov's relocs.txt whose
    site lies in one of SRC's delinks ranges. (A file that doesn't match yet has other offsets than the original's,
    so a site is not paired by offset.)"""
    if (ov, src) not in cache:
        rg = []
        f = ROOT / f"config/b_us/arm9/overlays/ov{ov:03d}/delinks.txt"
        cur = None
        for l in f.read_text().splitlines():
            m = re.match(r"(\S+\.\w+):$", l)
            if m:
                cur = m[1]
                continue
            m = re.match(r"\s+\.\w+\s+start:0x([0-9a-f]+) end:0x([0-9a-f]+)", l)
            if m and cur == src:
                rg.append((int(m[1], 16), int(m[2], 16)))
        d = collections.defaultdict(set)
        f = ROOT / f"config/b_us/arm9/overlays/ov{ov:03d}/relocs.txt"
        if rg and f.exists():
            for l in f.read_text().splitlines():
                m = re.match(r"from:0x([0-9a-f]{8}) kind:\w+ to:0x([0-9a-f]{8}) module:(.*)$", l)
                if m and any(a <= int(m[1], 16) < b for a, b in rg):
                    d[int(m[2], 16) & ~1] |= {int(k) for k in re.findall(r"\d+", m[3])} if not m[3].startswith("main") else set()
        cache[(ov, src)] = d
    return cache[(ov, src)]

def decl_kind(name, cache={}):
    """'func' / 'data' by the declarations of NAME in the headers and C."""
    if name not in cache:
        cache[name] = fu.want_kind(name, set())
    return cache[name]

def main():
    pre = sys.argv[1:] or ["src/ov"]
    objs = sorted((ROOT / "build/b_us/src").rglob("*.o"))
    n = 0
    for o in objs:
        rel = str(o.relative_to(ROOT / "build/b_us")).replace("\\", "/")[:-2] + ".c"
        m = re.match(r"src/ov(\d+)/", rel)
        if not m or not any(rel.startswith(p) for p in pre):
            continue
        M = int(m[1])
        orig = ROOT / "build/b_us/delinks" / rel[:-2]
        orig = orig.with_suffix(".o")
        theirs = {(s, off): sym for s, off, t, sym, a in fu.relocs(orig)} if orig.exists() else {}
        seen = set()
        for s, off, t, sym, a in fu.relocs(o):
            mm = NAME.match(sym)
            if not mm:
                continue
            N, A = int(mm[2]), mm[3]
            mods = modules(M).get(A)
            call = bool(re.search(r"CALL|JUMP|PC24|THM_PC22", t))
            why = []
            fm = file_mods(M, rel).get(int(A, 16))
            if fm and N != M and N not in fm:
                why.append(f"WRONG OVERLAY: relocs.txt for this file lists {sorted(fm)}")
            k = fu.sym_kind(N, A)
            dk = decl_kind(sym)
            if k and dk and k != dk:
                why.append(f"WRONG KIND: symbols.txt says {k}, the C declares {dk}")
            if k == "data" and call:
                why.append("WRONG KIND: called, but a datum")
            if mods and len(mods) > 1 and not why:
                orig_sym = theirs.get((s, off))
                if orig_sym and orig_sym != sym:
                    alt = [(x, fu.sym_kind(x, A)) for x in sorted(mods)]
                    why.append(f"AMBIGUOUS, differs from the original object's {orig_sym}; overlays {alt}")
            if why and (sym, tuple(why)) not in seen:
                seen.add((sym, tuple(why)))
                print(f"{rel}: {sym} ({t} at {s}+{off:#x}, {dk or '?'} decl): " + "; ".join(why))
                n += 1
    print(n, "findings")

if __name__ == "__main__":
    main()
