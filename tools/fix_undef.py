#!/usr/bin/env python3
"""Rename the pokebw2 names a file's object leaves undefined to Black's own (data_/func_ names).

    python3 tools/fix_undef.py <file.c> [--apply] [NAME ...]

The link says `Undefined : "X"` when the C uses a pokebw2 name X that Black names differently. This finds where the
file's object (build/b_us/<file>.o) references X (its relocations), looks at the same place in the original's
delinked object (build/b_us/delinks/<file>.o, which carries Black's symbol names) and prints the symbol there.
With --apply it renames X to that symbol in the file and in the headers that declare X (a declaration of X is
deleted when the headers already declare the Black name). With no NAME it takes the symbols the object leaves
undefined that the last link complained about (runs ninja).
Reports instead of renaming when the original's target has no symbol, differs in addend, or X maps to two names.
Run after the file's functions match (a reloc site is paired by its offset in the .text/.rodata/.data it is in)."""
import re, subprocess, sys, pathlib, collections

ROOT = pathlib.Path(__file__).resolve().parent.parent

def sh(*a):
    return subprocess.run(a, cwd=ROOT, capture_output=True, text=True).stdout

def sections(obj):
    secs = {}
    for l in sh("readelf", "-SW", obj).splitlines():
        m = re.match(r"\s*\[\s*(\d+)\]\s+(\S*)\s+(\w+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+[0-9a-f]+\s+(\w*)\s+(\d+)\s+(\d+)\s+(\d+)", l)
        if m:
            secs[int(m[1])] = dict(name=m[2], type=m[3], off=int(m[5], 16), size=int(m[6], 16),
                                   info=int(m[9]), al=int(m[10]) or 1)
    return secs

def relocs(obj):
    """[(section name, global offset, type, symbol, addend)] for the alloc sections (.text, .rodata, .data ...)."""
    secs = sections(obj)
    base, end = {}, collections.defaultdict(int)
    for i in sorted(secs):                       # concatenate the same-named sections as the linker does
        s = secs[i]
        if s["type"] == "PROGBITS" and "debug" not in s["name"]:
            a = s["al"]
            o = (end[s["name"]] + a - 1) // a * a
            base[i] = o
            end[s["name"]] = o + s["size"]
    byoff = {s["off"]: i for i, s in secs.items() if s["type"] == "RELA"}
    out, cur = [], None
    for l in sh("readelf", "-rW", obj).splitlines():
        m = re.match(r"Relocation section '.*' at offset 0x([0-9a-f]+)", l)
        if m:
            tgt = secs[byoff[int(m[1], 16)]]["info"]
            cur = tgt if tgt in base else None
            continue
        m = re.match(r"([0-9a-f]{8})\s+[0-9a-f]+\s+(R_\w+)\s+[0-9a-f]+\s+(\S+) ([+-]) (\w+)$", l)
        if m and cur is not None:
            add = int(m[5], 16) * (1 if m[4] == "+" else -1)
            out.append((secs[cur]["name"], base[cur] + int(m[1], 16), m[2], m[3], add))
    return out

def undefined_all(obj):
    r = []
    for l in sh("readelf", "-sW", obj).splitlines():
        p = l.split()
        if len(p) >= 8 and p[6] == "UND" and p[7]:
            r.append(p[7])
    return r

def link_undefined():
    out = subprocess.run(["ninja"], cwd=ROOT, capture_output=True, text=True)
    return set(re.findall(r'Undefined : "([^"]+)"', out.stdout + out.stderr))

def defined(name, path=""):
    for l in sh("grep", "-rnw", name, "src", "lib").splitlines():
        if path and not l.startswith(path):
            continue
        body = l.split(":", 2)[-1]
        if body[:1] not in " 	/}#" and "extern" not in body and                 (body.rstrip().endswith("{") or re.search(r"[^=!<>]=[^=]", body)):
            return True
    return False

def uses(name):
    return len(sh("grep", "-rnw", name, "src", "lib", "include").splitlines())

PREFER = collections.Counter()

def shared(y, src):
    m = re.match(r"(data|func)_ov(\d+)_([0-9a-f]{8})$", y)
    n = 0
    for l in (module_dir(src) / "relocs.txt").read_text().splitlines():
        if f"to:0x{m[3]} " in l:
            n += len(set(re.findall(r"\d+", l.split("module:")[-1])))
    return n > 1

SYMKIND = {}

def sym_kind(ov, addr):
    """'func' or 'data': what overlay ov's symbols.txt calls the symbol at addr (None when it has none)."""
    if ov not in SYMKIND:
        d = {}
        f = ROOT / f"config/b_us/arm9/overlays/ov{ov:03d}/symbols.txt"
        if f.exists():
            for l in f.read_text().splitlines():
                mm = re.match(r"\S+ kind:(\w+)\S* addr:0x([0-9a-f]{8})", l)
                if mm:
                    d[mm[2]] = "func" if mm[1] == "function" else "data"
        SYMKIND[ov] = d
    return SYMKIND[ov].get(addr)

def owner_file(ov, addr):
    """(source file, section name, section start) whose delinks ranges hold addr in overlay ov, or None."""
    f = ROOT / f"config/b_us/arm9/overlays/ov{ov:03d}/delinks.txt"
    if not f.exists():
        return None
    a, cur = int(addr, 16), None
    for l in f.read_text().splitlines():
        mm = re.match(r"(\S+\.\w+):$", l)
        if mm:
            cur = mm[1]
            continue
        mm = re.match(r"\s+(\.\w+)\s+start:0x([0-9a-f]+) end:0x([0-9a-f]+)\s*$", l)
        if mm and cur and int(mm[2], 16) <= a < int(mm[3], 16) and (ROOT / cur).exists():
            return (cur, mm[1], int(mm[2], 16))
    return None

def defined_name(ov, addr):
    """The global symbol our compiled object of the file that owns addr defines there, under whatever name its C
    gives it (mailsys.c's MAILSYS_PROC_FUNCTIONS for Black's data_ov136_021c7768), or None."""
    o = owner_file(ov, addr)
    if not o:
        return None
    obj = ROOT / "build/b_us" / (o[0][:-2] + ".o")
    if not obj.exists():
        return None
    secs = sections(obj)
    off = (int(addr, 16) - o[2]) & ~1
    for l in sh("readelf", "-sW", obj).splitlines():
        p = l.split()
        if len(p) >= 8 and p[4] == "GLOBAL" and p[6].isdigit() and secs.get(int(p[6]), {}).get("name") == o[1]:
            if (int(p[1], 16) & ~1) == off and not p[7].startswith("."):
                return p[7]
    return None

DECLS = None

def decl_votes():
    """{identifier: Counter(func / data)} from the top-level declarations (lines starting in column 0) of the headers
    and C files: an identifier followed by ( is a function, by ; = [ , a datum. Read once."""
    global DECLS
    if DECLS is None:
        DECLS = collections.defaultdict(collections.Counter)
        tok = re.compile(r"\b([A-Za-z_]\w*)\s*([(;=\[,])")
        for top in ("include", "src", "lib"):
            for p in (ROOT / top).rglob("*"):
                if p.suffix not in (".h", ".c") or not p.is_file():
                    continue
                for l in p.read_text(errors="replace").splitlines():
                    if not l or l[0] in " \t/}#*{" or not (l[0].isalpha() or l[0] == "_"):
                        continue
                    for mm in tok.finditer(l):
                        DECLS[mm[1]]["func" if mm[2] == "(" else "data"] += 1
    return DECLS

def want_kind(x, rtypes):
    """'func' when X is called/branched to or declared as a function, 'data' when declared as an object, else None."""
    if any(re.search(r"CALL|JUMP|PC24|XPC25|THM_PC22", r) for r in rtypes):
        return "func"
    v = decl_votes().get(x)
    if v and len(v) == 1:
        return next(iter(v))
    return None

def pick_overlay(y, m, src, want=None):
    """Overlays share addresses and the delinked object names the first: relocs.txt lists them, the one the
    code defines (or the only one) is the name to use. A data reference never takes a function (nor a call a
    datum) of another overlay: the candidates are filtered by the kind symbols.txt gives them (WANT, from the
    relocation type or X's declaration), then by who defines the name, then by the source file whose delinks
    ranges own the address."""
    mods = set()
    for rl in [module_dir(src) / "relocs.txt"]:
        for l in rl.read_text().splitlines():
            if f"to:0x{m[3]} " in l:
                mods.add(l.split("module:")[-1])
    ks = {int(k) for mm in mods for k in re.findall(r"\d+", mm)}
    ovs = sorted(ks)
    if len(ovs) <= 1:
        return y
    if want:
        ok = [k for k in ovs if sym_kind(k, m[3]) in (want, None)]
        if ok and len(ok) < len(ovs):
            print(f"  (kind {want}: not {[k for k in ovs if k not in ok]}, the other overlays at that address name it differently)")
            ovs = ok
    # two overlays at one address are never loaded together: the file's own overlay, when it is one of them (code
    # that reaches into its own range means its own code), else the one its module reaches elsewhere without doubt
    # (its relocs.txt's module:overlay(N) lines): wifi_p2pmatch.c (ov099) had TouchBar_SetIconKey given ov098's
    # func_ov098_021b6578, the overlay our code defines a name at, where ov099 reaches ov067 and never ov098 (s542)
    sym_of = lambda k: defined_name(k, m[3]) or f"{'func' if sym_kind(k, m[3]) == 'func' else 'data' if sym_kind(k, m[3]) == 'data' else m[1]}_ov{k:03d}_{m[3]}"  # noqa: E731
    own = re.match(r"ov(\d+)$", module_dir(src).name)
    if own and int(own[1]) in ovs:
        n = sym_of(int(own[1]))
        print(f"  ({y}: the address is in this file's own overlay {int(own[1])} too, and {n} is its symbol there)")
        return n
    loads = co_loaded(src)
    seen = sorted((k for k in ovs if loads[k]), key=lambda k: -loads[k])
    if seen and (len(seen) == 1 or loads[seen[0]] > loads[seen[1]]):
        n = sym_of(seen[0])
        print(f"  ({y}: of the overlays at that address ({ovs}), this module reaches ov{seen[0]:03d} elsewhere "
              f"{loads[seen[0]]} times" + (f", the others {[loads[k] for k in seen[1:]]}" if seen[1:] else ", the others never") +
              f": {n})")
        return n
    if own:
        # no relocation of the module tells: the overlay it is laid out on (ancestors()), when one of those at the
        # address is: wifi_p2pmatch.c (ov099, laid out on ov067) calls ov067's TouchBar function, not ov098's at the
        # same address, which our code defines too. (Second to the relocations: an overlay laid out after a group
        # starts where the longest of it ends, and ov207's code reaches ov206, not ov093 below it)
        under = ancestors(int(own[1]))
        reach = [k for k in ovs if k in under]
        if len(reach) == 1:
            n = sym_of(reach[0])
            print(f"  ({y}: of the overlays at that address ({ovs}), ov{reach[0]:03d} is one this module's is laid "
                  f"out on (ov{int(own[1]):03d} on {' on '.join(f'ov{k:03d}' for k in under)}): {n})")
            return n
    cands = [f"{'func' if sym_kind(k, m[3]) == 'func' else 'data' if sym_kind(k, m[3]) == 'data' else m[1]}_ov{k:03d}_{m[3]}" for k in ovs]
    if len(cands) <= 1:
        return cands[0] if cands else y
    names = {}
    for k in ovs:
        c = cands[ovs.index(k)] if len(cands) == len(ovs) else None
        n = defined_name(k, m[3]) or (c if c and defined(c, f"src/ov{k:03d}/") else None)
        if n:
            names[k] = n
    if len(names) == 1:
        n = next(iter(names.values()))
        print(f"  ({y} is the first of the overlays at that address; {n} is the one our code defines)")
        return n
    own = [k for k in (names or ovs) if owner_file(k, m[3])]
    pool = names or {k: cands[ovs.index(k)] for k in ovs}
    best = sorted(pool, key=lambda k: (-PREFER[k], -uses(pool[k])))
    print(f"  (SHARED address {cands}; defined by our code: {names}; owned by a source file in {own}; took {pool[best[0]]} by use, check)")
    return pool[best[0]]

def module_dir(src):
    for d in list((ROOT / "config/b_us/arm9/overlays").glob("*/delinks.txt")) + [ROOT / "config/b_us/arm9/delinks.txt"]:
        if re.search(rf"^{re.escape(src)}:", d.read_text(), re.M):
            return d.parent
    return ROOT / "config/b_us/arm9"

def known(name):
    """Black's own name (config symbols) or defined by a C file here."""
    if re.match(r"OVERLAY_\d+_ID$", name):      # the linker script defines these
        return True
    if sh("grep", "-rlw", "--include=symbols.txt", name, "config/b_us").strip():
        return True
    return defined(name)

def local_target(orig, sec, off, src):
    """The word the delinked object holds at a site with no relocation: an address, and Black's symbol there."""
    word = None
    for l in sh("readelf", "-x", sec, str(orig)).splitlines():
        m = re.match(r"\s+0x([0-9a-f]{8}) ((?:[0-9a-f]+ ?){1,4})", l)
        if m:
            base, b = int(m[1], 16), bytes.fromhex(m[2].replace(" ", ""))
            if base <= off and off + 4 <= base + len(b):
                word = int.from_bytes(b[off - base:off - base + 4], "little")
            elif base <= off < base + len(b):          # a word straddling two rows
                word = -1
    if word is None or word < 0:
        return (word, ())
    names = []
    for d in list((ROOT / "config/b_us/arm9/overlays").glob("*/symbols.txt")) + [ROOT / "config/b_us/arm9/symbols.txt"]:
        for l in d.read_text().splitlines():
            m = re.match(r"(\S+) kind:\S+ addr:0x%08x\b" % (word & ~1), l)
            if m:
                names.append(m[1])
    return (f"{word:#010x}", tuple(names))

SPANS = {}

def overlay_spans():
    """{overlay: (start, end)} of every overlay: the lowest start and highest end of its delinks.txt's section lines."""
    if not SPANS:
        for d in (ROOT / "config/b_us/arm9/overlays").glob("ov*/delinks.txt"):
            lo = hi = None
            for l in d.read_text().splitlines():
                if l and not l[0].isspace():
                    break                       # the files' ranges start: the module's own sections came first
                m = re.search(r"start:0x([0-9a-f]+) end:0x([0-9a-f]+)", l)
                if m:
                    a, b = int(m[1], 16), int(m[2], 16)
                    lo, hi = min(a, lo if lo is not None else a), max(b, hi if hi is not None else b)
            if lo is not None:
                SPANS[int(d.parent.name[2:])] = (lo, hi)
    return SPANS

def ancestors(ov):
    """The overlays an overlay is laid out on, nearest first: the linker puts an overlay where the one it is loaded
    with ends (its end rounded up to 32: ov099 starts at ov067's end, ov067 at ov013's), and that one on the next.
    227 of Black's 237 overlays start where exactly one other ends; of the 610 shared addresses in relocs.txt where
    both this and the module's own module:overlay(N) relocations name one overlay, they agree on 605."""
    spans, out = overlay_spans(), []
    while ov in spans and len(out) < len(spans):
        below = [k for k, (lo, hi) in spans.items() if k != ov and k not in out and (hi + 31) & ~31 == spans[ov][0]]
        if len(below) != 1:
            break
        ov = below[0]
        out.append(ov)
    return out

def co_loaded(src):
    """Counter {overlay: the relocations of the file's module that reach it without doubt (module:overlay(N))}: the
    overlays loaded with it."""
    out = collections.Counter()
    rl = module_dir(src) / "relocs.txt"
    if rl.exists():
        out.update(int(n) for n in re.findall(r"module:overlay\((\d+)\)", rl.read_text()))
    own = re.match(r"ov(\d+)$", module_dir(src).name)
    if own:
        out.pop(int(own[1]), None)
    return out

def linker_symbol(name):
    """Whether the linker script defines name (SDK_IRQ_STACKSIZE, SDK_SECTION_ARENA_*, OVERLAY_n_ID, ...)."""
    if re.match(r"OVERLAY_\d+_ID$|SDK_[A-Z0-9_]+$", name):
        return True
    try:
        sys.path.insert(0, str(ROOT / "tools"))
        import dead_strip
        return name in getattr(dead_strip, "LINKER_SYMBOLS", {})
    except Exception:  # noqa: BLE001
        return False

def src_dir(src):
    m = re.match(r"src/(ov\d+)/", src)
    return m[1] if m else "/nothing/"

def main():
    args = sys.argv[1:]
    apply = "--apply" in args
    args = [a for a in args if a != "--apply"]
    src = args.pop(0)
    names = set(args)
    ours = ROOT / "build/b_us" / (src[:-2] + ".o")
    orig = ROOT / "build/b_us/delinks" / (src[:-2] + ".o")
    if not names:
        names = link_undefined() & set(undefined_all(ours))
        if not names:     # the file isn't linked yet: what it needs that the original's object doesn't
            names = {n for n in set(undefined_all(ours)) - set(undefined_all(orig)) if not known(n)}
    if not names:
        print("nothing undefined in", src); return 0
    theirs = {(s, o): (sym, a) for s, o, t, sym, a in relocs(orig)}
    found = collections.defaultdict(set)
    rtypes = collections.defaultdict(set)
    for s, o, t, sym, a in relocs(ours):
        if sym in names:
            rtypes[sym].add(t)
            tsym, ta = theirs.get((s, o), (None, 0))
            found[sym].add((tsym, ta, a, f"{s}+{o:#x}"))
    result = {}
    for x in sorted(names):
        for y, ta, a, w in found.get(x, ()):
            mm = re.match(r"(?:data|func)_ov(\d+)_", y or "")
            if mm and not shared(y, src):
                PREFER[int(mm[1])] += 1
    for x in sorted(names):
        sites = found.get(x)
        if linker_symbol(x):
            # (os_thread.c: SDK_IRQ_STACKSIZE was proposed as OSi_IdleThreadProc, the function whose address the
            # original's literal at the paired site holds, s572)
            print(f"{x}: a linker-script symbol (tools/dead_strip.py LINKER_SYMBOLS, the .lcf): the linker defines it, "
                  "not renamed")
            continue
        if not sites:
            print(f"{x}: no relocation in {ours.name} (used only in a declaration?)"); continue
        ys = {y for y, ta, a, w in sites}
        bad = [w for y, ta, a, w in sites if y is None or y.startswith(".") or ta != a]
        if len(ys) == 1 and None in ys:
            got = {local_target(orig, s, o, src) for y, ta, a, w in sites for s, o in [(w.split("+")[0], int(w.split("+")[1], 16))]}
            cands = {c for word, cs in got for c in cs}
            want = want_kind(x, rtypes[x])
            if want:
                cands = {c for c in cands if not (mm := re.match(r"(data|func)_ov\d+_", c)) or mm[1] == want}
            d = [c for c in sorted(cands) if (mm := re.match(r"(?:data|func)_ov(\d+)_", c)) and defined(c, f"src/ov{int(mm[1]):03d}/")]
            if len(d) == 1:
                print(f"{x} -> {d[0]}   (no relocation in the original: the literal's value {sorted(w for w, cs in got)} names it; other overlays at that address: {sorted(cands)})")
                result[x] = d[0]
            else:
                print(f"{x}: NO RELOCATION in the original (value/symbols {sorted(got)}): the target is in this file's ranges or in no set-up overlay; define it here or name it by hand")
            continue
        if len(ys) != 1 or bad:
            print(f"{x}: UNRESOLVED {sorted(sites, key=str)}"); continue
        y = next(iter(ys))
        m = re.match(r"(data|func)_ov(\d+)_([0-9a-f]{8})$", y)
        if m:
            y = pick_overlay(y, m, src, want_kind(x, rtypes[x]))
        print(f"{x} -> {y}   ({len(sites)} site{'s' * (len(sites) > 1)})")
        result[x] = y
    if apply:
        for x, y in result.items():
            rename(x, y, src)
    return 0

def sub_code(pat, y, text):
    """Substitute outside // comments (the 'pokebw2's X' notes keep the old name)."""
    out = []
    for l in text.split(chr(10)):
        i = l.find("//")
        out.append(pat.sub(y, l) if i < 0 else pat.sub(y, l[:i]) + l[i:])
    return chr(10).join(out)

def declaration_of(name, files):
    """The first one-line `extern ... name ...;` declaration of name in these files, or None."""
    pat = re.compile(r"^[ \t]*extern\s[^;{}()]*\b" + re.escape(name) + r"\b[^;{}()]*;[ \t]*$", re.M)
    for f in files:
        m = pat.search((ROOT / f).read_text(errors="replace"))
        if m:
            return m.group(0).strip()
    return None

def compile_errors(src):
    """MWCC's output when the file's object is built, '' when it compiles."""
    out = subprocess.run(["ninja", "build/b_us/" + src[:-2] + ".o"], cwd=ROOT, capture_output=True, text=True)
    return "" if out.returncode == 0 else out.stdout + out.stderr

def ensure_declared(src, y, xdecl, files, x=None):
    """After x became y in src: when the file can't see a declaration of y (a libntr file compiles with
    -i lib/nitro/sdk and doesn't include include/nitro/os.h, which declares Black's y), declare it in the file, as the
    header that declared x did (x's declaration with y), else as the first header that declares y."""
    errors = compile_errors(src)
    flat = errors.replace(chr(10), " ")
    if f"undefined identifier '{y}'" not in flat and f"'{y}' undeclared" not in flat:
        return False
    decl = xdecl or declaration_of(y, sh("grep", "-rlw", y, "include", "lib", "--include=*.h").split())
    if not decl:
        print(f"  NO DECLARATION of {y} visible in {src} and none to copy: declare it by hand")
        return False
    p = ROOT / src
    text = p.read_text()
    lines = text.split(chr(10))
    last = max((i for i, l in enumerate(lines) if l.startswith("#include")), default=-1)
    if x:
        decl = f"{decl} // Black's name for {x}"
    lines[last + 1:last + 1] = ["", decl] if last >= 0 else [decl]
    p.write_text(chr(10).join(lines))
    if compile_errors(src):
        p.write_text(text)
        print(f"  added declaration of {y} to {src} did not compile: {decl}")
        return False
    print(f"  declared {y} in {src}: {decl}")
    return True

def rename(x, y, src):
    pat = re.compile(rf"\b{re.escape(x)}\b")
    ydecl = bool(sh("grep", "-rlw", y, "include", "lib").strip())
    files = sh("grep", "-rlw", x, "include", "src", "lib", "--include=*.h", "--include=*.c").split()
    xdecl = declaration_of(x, [f for f in files if f.endswith(".h")] + ([src] if (ROOT / src).exists() else []))
    if xdecl:
        xdecl = re.sub(chr(92) + "b" + re.escape(x) + chr(92) + "b", y, xdecl)
    if src not in files and (ROOT / src).exists() and pat.search((ROOT / src).read_text(errors="replace")):
        files.append(src)
    for f in files:
        if f.endswith(".c") and f != src:
            print(f"  also uses {x}: {f} (not touched: rename there when that file is done)")
            continue
        p = ROOT / f
        t = p.read_text(errors="replace")
        if f.endswith(".h") and ydecl:
            continue            # Y is declared already; X's declaration stays for the files that still use X
        else:
            t2 = sub_code(pat, y, t)
        if t2 != t:
            p.write_text(t2)
            print(f"  renamed {x} -> {y} in {f}")
    if (ROOT / src).exists():
        ensure_declared(src, y, xdecl, files, x)

if __name__ == "__main__":
    sys.exit(main())
