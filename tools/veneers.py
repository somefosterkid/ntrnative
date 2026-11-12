#!/usr/bin/env python3
"""Converts a file's linker veneers. The link makes them itself (configure.py's -overlaygroup: right after each caller,
one per call site, in call order), so a converted file's C calls the far function f and the config names no veneer
(the rules; docs/lessons.md, `bx pc`).

A veneer is a Thumb `bx pc; nop` with the ARM `ldr ip, [pc]; bx ip; .word f` after it (a Thumb caller of a Thumb f),
the ARM part alone (an ARM caller), or an ARM `ldr pc, [pc, #-4]; .word f` (a caller of an ARM f): dsd makes a
function of each part, and the calls of the function before it reach it. For each veneer whose caller is the file's:

1. every C file and header that names a veneer names f instead (and a pokebw2 name the C calls at a veneer's call
   site, by the compiled object's offsets); the prototypes the veneers had go where a header the file includes
   declares f, else one stays;
2. the module's symbols.txt loses the veneer's symbols and relocs.txt its relocations and those to it (any module's);
   a .text range that ends before the veneers of its last function ends after them (and a file starting there starts
   after them);
3. docs/nonmatching.md loses the veneers' rows.
Then tools/finish.py judges the file and marks it complete if every function matches and the build is the ROM's.

    python3 tools/veneers.py src/ov095/condition_effects.c     # convert it
    python3 tools/veneers.py --dry-run src/ov095/condition_effects.c
    python3 tools/veneers.py --list      # the files whose config still names veneers, and how many
"""
import argparse
import functools
import re
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import compiler_probe as cp  # noqa: E402
from file_ranges import code_section, module_dir, read_delinks  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config" / "b_us" / "arm9"
NM = ROOT / "docs" / "nonmatching.md"
SYM = re.compile(r"^(\S+) kind:function\((arm|thumb),size=(0x[0-9a-f]+)[^)]*\) addr:(0x[0-9a-f]+)", re.M)
RELOC = re.compile(r"^from:(0x[0-9a-f]+) kind:(\w+) to:(0x[0-9a-f]+) module:(\S+)$")
BX_PC, NOP = b"\x78\x47", b"\xc0\x46"
LDR_IP, BX_IP, LDR_PC = 0xe59fc000, 0xe12fff1c, 0xe51ff004
CALLS = ("thumb_call", "thumb_call_arm", "arm_call", "arm_call_thumb")


def mod_of_dir(d):
    """The module of a config directory: main, ov095, itcm, ltd_autoload_0."""
    return "main" if d == CONFIG else d.name


def image(mod):
    """(bytes, base address) of a module's binary."""
    path, base = cp.load_modules()[str(CONFIG / module_dir(mod))]
    return path.read_bytes(), base


def reloc_mods(s):
    """The modules a relocs.txt module: names, the likeliest first: ['ov093'], ['ov021', 'ov093', 'ov117']."""
    m = re.fullmatch(r"(\w+?)s?\(([\d,]+)\)", s)
    if not m:
        return [s]
    kind, ns = m[1], [int(n) for n in m[2].split(",")]
    return [f"ov{n:03d}" if kind == "overlay" else f"{kind}_{n}" for n in ns]


def word(data, base, addr):
    off = addr - base
    return int.from_bytes(data[off:off + 4], "little") if 0 <= off <= len(data) - 4 else None


class Module:
    def __init__(self, mod):
        self.mod, self.dir = mod, CONFIG / module_dir(mod)
        self.syms_text = (self.dir / "symbols.txt").read_text()
        self.relocs_text = (self.dir / "relocs.txt").read_text()
        # [(addr, size, name, mode)]
        self.funcs = sorted((int(a, 16), int(s, 16), n, m) for n, m, s, a in SYM.findall(self.syms_text))
        self.data, self.base = image(mod)
        self.relocs = [(int(f, 16), k, int(t, 16), m) for f, k, t, m in
                       (RELOC.match(l).groups() for l in self.relocs_text.splitlines() if RELOC.match(l))]
        self.to = {}
        for r in self.relocs:
            self.to.setdefault(r[2], []).append(r)
        self.veneers = self.find()

    def find(self):
        """[(start, end, [symbol names], target address, literal address)] of the module's veneers, by their bytes."""
        out, by_addr = [], {a: (s, n, m) for a, s, n, m in self.funcs}
        for a, s, n, m in self.funcs:
            off = a - self.base
            if m == "thumb" and a % 4 == 0 and self.data[off:off + 2] == BX_PC and s in (2, 4) and \
                    self.data[off + 2:off + 4] == NOP and word(self.data, self.base, a + 4) == LDR_IP and \
                    word(self.data, self.base, a + 8) == BX_IP:
                names = [n] + ([by_addr[a + 4][1]] if a + 4 in by_addr else [])
                out.append((a, a + 16, names, word(self.data, self.base, a + 12), a + 12))
            elif m == "arm" and a % 4 == 0 and word(self.data, self.base, a) == LDR_IP and \
                    word(self.data, self.base, a + 4) == BX_IP and s == 0xc and \
                    not (out and out[-1][1] == a + 12):  # not the ARM part of a pair found already
                out.append((a, a + 12, [n], word(self.data, self.base, a + 8), a + 8))
            elif m == "arm" and a % 4 == 0 and word(self.data, self.base, a) == LDR_PC and s in (4, 8):
                out.append((a, a + 8, [n], word(self.data, self.base, a + 4), a + 4))
        return out

    def caller(self, v):
        """The function whose calls reach veneer v: the one before its run of veneers, if every call to v is from
        it and nothing else (a pointer, a far call) reaches it; else None."""
        start, end = v[0], v[1]
        refs = [r for a in range(start, end, 2) for r in self.to.get(a, []) + self.to.get(a + 1, [])
                if self.mod in reloc_mods(r[3])]
        if not refs or any(r[1] not in CALLS for r in refs):
            return None
        starts = {x[0] for x in self.veneers}
        before = [f for f in self.funcs if f[0] < start and f[0] not in starts and
                  not any(x[0] < f[0] < x[1] for x in self.veneers)]
        if not before:
            return None
        f = before[-1]
        return f if all(f[0] <= r[0] < f[0] + f[1] for r in refs) else None


def target_name(mod, v, literal_relocs):
    """The name of the function a veneer's literal points to, from the relocation at the literal (the module most
    of this module's other relocations name alone, among those it could be), else None."""
    r = literal_relocs.get(v[4])
    if not r:
        # dsd made no relocation (a call into the DSi's LTD autoload from main): the one function at the address
        at = (v[3] or 0) & ~1
        found = [(m[1], mod_of_dir(p.parent)) for p in CONFIG.rglob("symbols.txt")
                 for m in re.finditer(rf"^(\S+) kind:function\(\w+[^)]*\) addr:{at:#010x}$", p.read_text(), re.M)]
        return found[0] if len(found) == 1 else (None, None)
    cands = reloc_mods(r[3])
    if len(cands) > 1:
        alone = Counter(reloc_mods(x[3])[0] for x in mod.relocs if len(reloc_mods(x[3])) == 1)
        cands.sort(key=lambda c: -alone[c])
    for c in cands:
        d = CONFIG / module_dir(c)
        if not (d / "symbols.txt").exists():
            continue
        text = (d / "symbols.txt").read_text()
        m = re.search(rf"^(\S+) kind:function\(\w+[^)]*\) addr:{r[2] & ~1:#010x}$", text, re.M)
        if m:
            return m[1], c
    return None, None


def files_of(mod):
    """[(file, start, end, complete)] of the module's .text ranges (and .ltdmain ones, TwlSDK's LTD code)."""
    out, src = [], None
    text = (CONFIG / module_dir(mod) / "delinks.txt").read_text()
    for line in text.splitlines():
        if line.strip() and not line[0].isspace():
            src = line.strip().rstrip(":")
        m = re.match(r"\s+\.(?:text|ltdmain)\s+start:(0x\w+) end:(0x\w+)", line)
        if m and src:
            out.append([src, int(m[1], 16), int(m[2], 16), f"\n{src}:\n    complete\n" in text])
    return out


def code_range(dtext, src, at, end=None, start=None):
    """The delinks.txt text with the code range of src that starts at `at` (.text, or .ltdmain for an LTD file: the
    rewrite matched only .text, t36c) changed: end=(old, new) moves its end, start=new its start."""
    head = rf"(\n{re.escape(src)}:\n(?:    .*\n)*?\s+\.(?:text|ltdmain)\s+start:)"
    if end is not None:
        return re.sub(head[:-1] + rf"{at:#010x} end:){end[0]:#010x}", rf"\g<1>{end[1]:#010x}", dtext)
    return re.sub(head + rf"{at:#010x}", rf"\g<1>{start:#010x}", dtext)


def owned(mod, file):
    """The veneers of the file's functions: [(veneer, caller, target, target module)] and the problems."""
    rng = next((f for f in files_of(mod.mod) if f[0] == file), None)
    lits = {r[0]: r for r in mod.relocs}
    out, problems = [], []
    for v in mod.veneers:
        c = mod.caller(v)
        inside = rng[1] <= v[0] < rng[2]
        if not c:
            if inside:
                problems.append(f"{v[2][0]}: not only the function before it calls it (a pointer, or no caller)")
            continue
        if not rng[1] <= c[0] < rng[2]:
            if inside:
                problems.append(f"{v[2][0]}: the veneer of {c[2]}, which isn't the file's")
            continue
        t, tm = target_name(mod, v, lits)
        if not t:
            problems.append(f"{v[2][0]}: no function at its literal's target ({word(mod.data, mod.base, v[4]):#x})")
            continue
        out.append((v, c, t, tm))
    return rng, out, problems


C_DIRS = ("src", "include", "lib")


def c_files():
    return [p for d in C_DIRS for p in (ROOT / d).rglob("*") if p.suffix in (".c", ".h") and p.is_file()]


def rewrite_c(renames):
    """Every C file and header naming a veneer names its target; returns {path: new text} changed. A C function of a
    veneer's name (an `asm` stand-in) stops it: none is left, and none is to be written (docs/worker.md)."""
    pat = re.compile(r"\b(" + "|".join(map(re.escape, renames)) + r")\b")
    changed = {}
    for p in c_files():
        text = p.read_text(errors="replace")
        if not pat.search(text):
            continue
        defined = re.search(r"^[^\s#/][^;{}=]*\b(" + "|".join(map(re.escape, renames)) + r")\s*\([^;{}]*\)\s*\{",
                            text, re.M)
        if defined:
            raise Unconvertible(f"{p.relative_to(ROOT)} defines {defined[1]}, a veneer: delete that function first")
        changed[p] = pat.sub(lambda m: renames[m[1]], text)
    return changed


def dedupe(text, targets, declared):
    """The C's prototypes of the targets: none where a header declares it (declared), else the first only; a
    comment above them that speaks of veneers goes if nothing is left under it, else says what they are."""
    # at file scope: from the line's first column (a `return f(x);` in a body is no prototype)
    proto = r"^(?!return\b)(?:extern[ \t]+)?[A-Za-z_][\w \t\*]*?\b(\w+)[ \t]*\([^;{}]*\)[ \t]*;[ \t]*$"
    kept = set()
    lines = []
    for l in text.splitlines(True):  # a prototype over several lines as one
        if lines and lines[-1].count("(") > lines[-1].count(")") and not lines[-1].lstrip().startswith(("//", "#")):
            lines[-1] += l
        else:
            lines.append(l)
    drop = [False] * len(lines)
    for i, l in enumerate(lines):
        m = re.match(proto, l)
        if m and m[1] in targets:
            drop[i] = m[1] in declared or m[1] in kept
            kept.add(m[1])
    # a comment right above a run of prototypes goes with them when they all go; one that spoke of veneers says what
    # the prototypes left are
    i = 0
    while i < len(lines):
        if not lines[i].startswith("//"):
            i += 1
            continue
        s = i
        while i < len(lines) and lines[i].startswith("//"):
            i += 1
        e = i
        while e < len(lines) and re.match(proto, lines[e]):
            e += 1
        if e > i and any(drop[i:e]):
            if all(drop[i:e]):
                drop[s:i] = [True] * (i - s)
            elif any(re.search(r"\b[Vv]eneers?\b", l) for l in lines[s:i]):
                lines[s:i] = ["// Far functions no included header declares (the link makes their veneers)\n"] + \
                    [""] * (i - s - 1)
    # a blank line that a dropped run leaves next to another goes too
    for i in range(1, len(lines)):
        if drop[i - 1] and not drop[i] and not lines[i].strip():
            j = i - 1
            while j >= 0 and drop[j]:
                j -= 1
            if j < 0 or not lines[j].strip():
                drop[i] = True
    return "".join(l for l, d in zip(lines, drop) if not d)


def strip_config(mod, convert, rng, dry):
    """Takes the veneers out of the module's config; returns what it changed, as lines to print."""
    said = []
    names = {n for v in convert for n in v[0][2]}
    spans = [(v[0][0], v[0][1]) for v in convert]
    inside = lambda a: any(s <= a < e for s, e in spans)  # noqa: E731
    sym = [l for l in mod.syms_text.splitlines(True) if not (l.split(" ", 1)[0] in names)]
    said.append(f"{mod.dir.relative_to(ROOT)}/symbols.txt: {len(names)} symbols out")
    keep = []
    gone = 0
    for l in mod.relocs_text.splitlines(True):
        m = RELOC.match(l.rstrip("\n"))
        if m and (inside(int(m[1], 16)) or (inside(int(m[3], 16) & ~1) and mod.mod in reloc_mods(m[4]))):
            gone += 1
            continue
        keep.append(l)
    said.append(f"{mod.dir.relative_to(ROOT)}/relocs.txt: {gone} relocations out")
    others = {}
    for p in CONFIG.rglob("relocs.txt"):
        if p == mod.dir / "relocs.txt":
            continue
        t = p.read_text()
        new = [l for l in t.splitlines(True) if not ((m := RELOC.match(l.rstrip("\n"))) and
                                                    inside(int(m[3], 16) & ~1) and mod.mod in reloc_mods(m[4]))]
        if len(new) != len(t.splitlines(True)):
            others[p] = "".join(new)
            said.append(f"{p.relative_to(ROOT)}: {len(t.splitlines()) - len(new)} relocations to them out")
    # the range's end: past the veneers of its last functions (a .text range, or an LTD file's .ltdmain: files_of()
    # reads both, and the rewrite matched only .text, t36c)
    delinks = mod.dir / "delinks.txt"
    dtext = delinks.read_text()
    end = max([rng[2]] + [e for s, e in spans if s >= rng[2]])
    if end != rng[2]:
        ranges = files_of(mod.mod)
        nxt = [f for f in ranges if f[1] == rng[2]]
        if any(f[1] < end and f[0] != rng[0] and f[1] > rng[2] for f in ranges):
            raise Unconvertible(f"{rng[0]}: its veneers run to {end:#x}, into another file's range")
        dtext = code_range(dtext, rng[0], rng[1], end=(rng[2], end))
        said.append(f"{delinks.relative_to(ROOT)}: {rng[0]}'s .text ends at {end:#x}, after its veneers")
        for f in nxt:
            dtext = code_range(dtext, f[0], f[1], start=end)
            said.append(f"{delinks.relative_to(ROOT)}: {f[0]}'s .text starts at {end:#x}")
    if not dry:
        (mod.dir / "symbols.txt").write_text("".join(sym))
        (mod.dir / "relocs.txt").write_text("".join(keep))
        for p, t in others.items():
            p.write_text(t)
        delinks.write_text(dtext)
    return said


def drop_rows(file, names, dry):
    lines = NM.read_text().splitlines(True)
    out = [l for l in lines if not (f"| `{file}` |" in l and re.match(r"\| `([^`]+)`", l) and
                                    re.match(r"\| `([^`]+)`", l)[1] in names)]
    if not dry:
        NM.write_text("".join(out))
    return len(lines) - len(out)


def declared_in_headers(c_path, targets):
    """The targets a header the C includes (directly or through others) declares."""
    from configure import INCLUDE_DIRS
    dirs = [ROOT / d for d in INCLUDE_DIRS] + [c_path.parent]
    seen, todo, text = set(), [c_path], ""
    while todo:
        p = todo.pop()
        if p in seen:
            continue
        seen.add(p)
        t = p.read_text(errors="replace")
        if p != c_path:
            text += t
        for inc in re.findall(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', t, re.M):
            for d in [p.parent] + dirs:
                if (d / inc).is_file():
                    todo.append((d / inc).resolve())
                    break
    return {t for t in targets if re.search(rf"\b{t}\s*\(", text)}


def compiled(dest):
    """The C file's functions as MWCC compiles it (tools/compiler_probe.py's object_functions), or exits."""
    import subprocess
    import tempfile
    from configure import INCLUDE_DIRS, compiler_of
    compiler, flags = compiler_of(Path(dest))
    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp) / "x.o"
        cmd = [str(ROOT / "tools" / "wibo"), str(ROOT / "tools" / "mwccarm" / compiler / "mwccarm.exe"),
               *" ".join(flags).split(), "-w", "off", *(a for d in INCLUDE_DIRS for a in ("-i", str(ROOT / d))),
               "-o", str(obj), str(ROOT / dest)]
        p = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT)
        if p.returncode:
            raise Unconvertible(f"{dest} doesn't compile:\n" + (p.stdout + p.stderr)[-800:])
        return cp.object_functions(obj)


def call_names(mod, dest, conv):
    """{name the C calls: the veneer's target} for the calls at the veneers' call sites whose C names neither the
    veneer nor its target (a pokebw2 name no symbols.txt has), the object's offsets being Black's; and the problems."""
    by_veneer = {}
    for v, _, t, _ in conv:
        for a in range(v[0], v[1], 2):
            by_veneer[a] = (v, t)
    sites = {r[0]: by_veneer[r[2] & ~1] for r in mod.relocs if r[1] in CALLS and (r[2] & ~1) in by_veneer}
    addr = {n: a for a, _, n, _ in mod.funcs}
    seen, problems = {}, []
    for name, (_, relocs, _) in compiled(dest).items():
        if name not in addr:
            continue
        for o, sym in relocs.items():
            site = sites.get(addr[name] + o)
            if not site or not sym:
                continue
            v, t = site
            sym = sym.split("+")[0]
            if sym in v[2] or sym == t:
                continue
            seen.setdefault(sym, set()).add(t)
    out = {}
    for sym, ts in seen.items():
        at = cp.symbol_address(sym)
        if len(ts) > 1 or at is not None and at != cp.symbol_address(next(iter(ts))):
            problems.append(f"{sym}: the C calls it where Black calls {', '.join(sorted(ts))} (a veneer's target)")
        else:
            out[sym] = next(iter(ts))
    return out, problems


class Unconvertible(Exception):
    pass


def convert(file, dry=False, say=print):
    """Converts the file's veneers (the module docstring), saying what it changes. Returns how many it converted;
    raises Unconvertible, changing nothing, when it can't."""
    dest = str(Path(file).resolve().relative_to(ROOT))
    mod_name = next((m for m, (_, fs) in read_delinks(str(CONFIG)).items() if code_section(fs.get(dest, {}))), None)
    if not mod_name:
        raise Unconvertible(f"{dest} has no .text (or .ltdmain) in a delinks.txt")
    mod = Module(mod_name)
    rng, conv, problems = owned(mod, dest)
    if not conv:
        say(f"{dest}: no veneers to convert" + "".join(f"\nleft: {p}" for p in problems))
        return 0
    renames = {n: t for v, _, t, _ in conv for n in v[2]}
    aliases, more = call_names(mod, dest, conv)
    problems += more
    for p in problems:
        say(f"left: {p}")
    targets = sorted(set(renames.values()))
    say(f"{dest}: {len(conv)} veneers of {len(targets)} functions: " + ", ".join(
        f"{t} ({n})" for t, n in Counter(t for _, _, t, _ in conv).most_common()))
    for a, t in aliases.items():
        say(f"{dest}: calls {t} for {a}, a name no symbols.txt has, at its veneers' call sites")
    changed = rewrite_c(renames)  # before strip_config writes anything: either may refuse
    for line in strip_config(mod, conv, rng, dry):
        say(line)
    if aliases:  # the pokebw2 names, in the file itself only
        p = ROOT / dest
        t = changed.get(p, p.read_text())
        changed[p] = re.sub(r"\b(" + "|".join(map(re.escape, aliases)) + r")\b", lambda m: aliases[m[1]], t)
    # the headers first, so that the C's check of what they declare sees them as they will be
    for p in sorted(changed, key=lambda p: p.suffix == ".c"):
        t = dedupe(changed[p], set(targets) | set(aliases.values()),
                   declared_in_headers(p, set(targets) | set(aliases.values())) if p.suffix == ".c" else set())
        if not dry:
            p.write_text(t)
        say(f"{p.relative_to(ROOT)}: calls the targets")
    say(f"docs/nonmatching.md: {drop_rows(dest, set(renames), dry)} rows out")
    return len(conv)


@functools.cache
def module_veneers(mod_name):
    """{symbol name: the caller's (address, size, name, mode)} of the module's veneers that only the function before
    them calls: both parts of each (the others are real functions of the same bytes, which other code calls)."""
    try:
        mod = Module(mod_name)
    except (KeyError, FileNotFoundError):
        return {}
    return {n: c for v in mod.veneers if (c := mod.caller(v)) for n in v[2]}


def names(mods=None):
    """The names of the linker veneers in the modules' symbols.txt (all modules by default): nothing for a worker to
    write (tools/ladder.py, tools/finish.py and tools/brief.py leave them out)."""
    if mods is None:
        mods = [mod_of_dir(d) for d in [CONFIG] + sorted(p.parent for p in CONFIG.rglob("*/symbols.txt"))]
    return {n for m in mods for n in module_veneers(m)}


def list_all():
    """Each file whose config still names veneers of its functions."""
    by = Counter()
    for d in [CONFIG] + sorted(p.parent for p in CONFIG.rglob("*/symbols.txt")):
        if not (d / "relocs.txt").exists() or not (d / "delinks.txt").exists():
            continue
        try:
            mod = Module(mod_of_dir(d))
        except (KeyError, FileNotFoundError):
            continue
        ranges = files_of(mod.mod)
        for v in mod.veneers:
            c = mod.caller(v)
            if not c:
                continue  # a real function of the same bytes: other code calls it, or a pointer
            f = next((r for r in ranges if r[1] <= c[0] < r[2]), None)
            by[(f[0] if f else f"({mod.mod}: code outside the files)", f[3] if f else False)] += 1
    for (f, done), n in sorted(by.items(), key=lambda x: -x[1]):
        print(f"{n:4} veneers\t{f}" + ("\tcomplete" if done else ""))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("file", nargs="?", help="the C file whose veneers go")
    ap.add_argument("--dry-run", action="store_true", help="say what it would change, change nothing")
    ap.add_argument("--list", action="store_true", help="the files whose config still names veneers")
    args = ap.parse_args()
    if args.list:
        list_all()
    elif args.file:
        try:
            convert(args.file, args.dry_run)
        except Unconvertible as e:
            sys.exit(f"{e}; nothing changed")
    else:
        ap.error("a file, or --list")


if __name__ == "__main__":
    main()
