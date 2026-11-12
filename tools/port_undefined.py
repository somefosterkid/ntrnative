#!/usr/bin/env python3
"""The PC build's undefined symbols (docs/port-build.md), grouped by where a definition would come from. It links the
exe again with the undefined references reported as warnings (the build itself ignores them), then looks each symbol
up in the ROM build's objects (build/b_us, `ninja` writes them) to say who defines it on the DS.

    python3 tools/port_undefined.py build/port/linux                # writes build/port/undefined.txt, prints counts
    python3 tools/port_undefined.py build/port/win64 -o build/port/undefined-win64.txt
    ninja -C build/port/linux undefined                             # the same, as a meson run target

Groups: sdk (lib/nitro, lib/twl: an SDK function or global libntr lacks), nnsys (lib/nnsys: libntrsystem lacks),
dsi (the DSi-only files port/exclude.txt leaves out, lib/twl's camera/DSP/SSP/mic), online (lib/dwc*, lib/nitrowifi,
lib/dpw, and the files exclude.txt calls online), dsprot (lib/dsprot), msl (lib/msl: MSL's own libc names), excluded
(game files exclude.txt leaves out as sdk or todo), game (a game file the PC build compiles, whose ROM object
defines it and the PC object doesn't: an incomplete file's assembly, or C still missing), gap (rom gaps: data or code
no C file owns, the _dsd_gap objects), linker (in no ROM object: the linker script's symbols, SDK_OVERLAY_* ...), libntr
(only libntr's own code references it: a gap in libntr).

It exits 1 (after writing its files, so the build stops) when an undefined function is defined in the game's own C: a
plain C99 `inline` definition, which emits no symbol (game_definitions; the fix is a port/extern stand-in).
"""
import argparse
import collections
import re
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import port_sources  # noqa: E402

UNDEF = re.compile(r"undefined reference to [`']([^']+)'")
INFN = re.compile(r"^(.*?): in function [`']([^']+)'")
# lld (the Android NDK's linker): 'ld.lld: warning: undefined symbol: X', then '>>> referenced by' lines naming the objects
LLD_UNDEF = re.compile(r"^ld\.lld: (?:warning|error): undefined symbol: (\S+)$")
LLD_REF = re.compile(r"^>>>\s+(?:referenced by\s+)?(\S+?\.o(?:bj)?):\(")
GROUPS = ["sdk", "nnsys", "dsi", "online", "dsprot", "msl", "excluded", "game", "gap", "linker", "libntr"]
DSI_LIB = re.compile(r"^lib/twl/src/(camera|dsp|ssp|mic|snd|jpeg|aes|photo)", re.I)


def link_command(bdir, edge):
    """The link command of build.ninja's edge (an executable): (linker, its arguments, the output to delete)."""
    text = (bdir / "build.ninja").read_text()
    m = re.search(rf"^build {re.escape(edge)}(?:\.exe)?: (\w+) (.*?)(?: \| .*)?\n((?: [^\n]*\n)*)", text, re.M)
    if not m:
        sys.exit(f"no {edge} link edge in build.ninja")
    rule, ins, block = m[1], m[2], m[3]
    vars_ = dict(re.findall(r"^ (\w+) = (.*)$", block, re.M))
    r = re.search(rf"^rule {rule}\n((?: [^\n]*\n)*)", text, re.M)
    rvars = dict(re.findall(r"^ (\w+) = (.*)$", r[1], re.M))
    out = "bwport.undef.tmp"

    def sub(s):
        return (s.replace("$ARGS", vars_.get("ARGS", "")).replace("$LINK_ARGS", vars_.get("LINK_ARGS", ""))
                .replace("$in", ins).replace("$out", out))
    cmd = sub(rvars["command"])
    n = 4 if cmd.startswith("xcrun ") else 1   # the iOS native files: xcrun --sdk SDK clang++ ARGS
    args = sub(rvars["rspfile_content"]) if "rspfile_content" in rvars else cmd.split(None, n)[n]
    args = args.replace("--unresolved-symbols=ignore-all", "--unresolved-symbols=report-all")
    # macOS: ld64 has no such flag; its failed link lists every undefined symbol
    if "-android" in cmd.split()[0]:   # the NDK's clang (port/machine/android-*.ini) links with lld
        args += " -Wl,--error-limit=0"
    return " ".join(cmd.split()[:n]), args + ("" if sys.platform == "darwin" else " -Wl,--warn-unresolved-symbols"), out


def undefined(bdir, edge):
    """{symbol: {referencing source}} from a link with the undefined references as warnings."""
    linker, args, out = link_command(bdir, edge)
    with tempfile.NamedTemporaryFile("w", suffix=".rsp", dir=bdir, delete=False) as f:
        f.write(args)
    r = subprocess.run(f"{linker} @{f.name}", shell=True, cwd=bdir, capture_output=True, text=True, errors="replace")
    (bdir / out).unlink(missing_ok=True)
    Path(f.name).unlink()
    refs = collections.defaultdict(set)
    where = "?"
    sym = None
    for line in r.stderr.splitlines():
        if sys.platform == "darwin":  # '  "_sym", referenced from:' then '      _func in lib.a[n](obj.o)'
            m = re.match(r'^  "_(.+)", referenced from:', line)
            if m:
                sym = m[1]
                continue
            m = re.match(r"^      \S+ in (?:\S+\[\d+\]\()?([^()\s]+?)\)?$", line)
            if m and sym:
                refs[sym].add(source_of(m[1] if ".p/" in m[1] else "x.p/" + m[1]))
            continue
        m = LLD_UNDEF.match(line)
        if m:
            sym = m[1]
            continue
        m = LLD_REF.match(line)
        if m and sym:
            refs[sym].add(source_of(m[1]))
            continue
        m = INFN.match(line)
        if m:
            where = m[1]
            continue
        m = UNDEF.search(line)
        if m:
            src = line.split(":(")[0].split(": ")[-1] if ":(" in line else where
            refs[m[1]].add(source_of(src))
    if not refs and r.returncode:
        sys.exit(r.stderr[-3000:])
    return refs


def source_of(path):
    """A link message's file (a source path with a line, or an object) as a repo path where it can be one."""
    path = re.sub(r":\d+$", "", path.strip())
    if re.search(r"\.p/_", path):
        mangled = re.sub(r"^.*?\.p/", "", path)
        mangled = re.sub(r"\.c\.o(bj)?$", ".c", mangled)
        mangled = mangled[len(str(ROOT).replace("/", "_")) + 1:]
        return real_path(mangled)
    try:
        return str(Path(path).resolve().relative_to(ROOT))
    except ValueError:
        return path


def real_path(mangled):
    def walk(base, rest):
        if not rest:
            return base if base.is_file() else None
        for i in range(1, len(rest) + 1):
            name = "_".join(rest[:i])
            if (base / name).exists():
                r = walk(base / name, rest[i:])
                if r:
                    return r
        return None
    r = walk(ROOT, mangled.split("_"))
    return str(r.relative_to(ROOT)) if r else mangled


def rom_definitions(rom):
    """{symbol: [repo-style path of the defining ROM object]}: build/b_us/<dir>/x.o -> <dir>/x.c, delinks too; and
    {symbol: (nm type letter, size)}."""
    objs = sorted(str(p) for p in rom.rglob("*.o"))
    defs, kinds = collections.defaultdict(list), {}
    for i in range(0, len(objs), 400):
        r = subprocess.run(["nm", "-A", "-S", "--defined-only", *objs[i:i + 400]], capture_output=True, text=True,
                           errors="replace")
        for line in r.stdout.splitlines():
            m = re.match(r"^(.*?\.o):\s*[0-9a-fA-F]*(?:\s+([0-9a-fA-F]+))?\s+([A-Za-z])\s+(\S+)$", line)
            if m and m[3] not in "aNn" and m[3].isupper():
                defs[m[4]].append(Path(m[1]).relative_to(rom).as_posix())
                if m[4] not in kinds or (m[2] and kinds[m[4]][1] == 0):
                    kinds[m[4]] = (m[3], int(m[2], 16) if m[2] else 0)
    return defs, kinds


DATA_NAME = re.compile(r"^(data_|Data|s_|g_)|(List|Table|Tbl|Data|Entries|Buf|Work)$")


def write_stubs(path, syms, kinds):
    """A C file that defines every undefined symbol: a trap for a function, a zeroed block for data (the ROM's size,
    at least 256 bytes). No game header: the definitions only have to link."""
    out = ["// Written by tools/port_undefined.py --stubs: the symbols the PC link lacks. Not game code: each function",
           "// prints its name and aborts, each datum is a zeroed block. Define the real thing in port/src or the C.",
           "#include <stdio.h>", "#include <stdlib.h>", "",
           "static void bwport_undefined(const char *name) {",
           '    fprintf(stderr, "bwport: undefined function %s called\\n", name);', "    fflush(stderr);",
           "    abort();", "}", ""]
    for sym in sorted(syms):
        kind, size = kinds.get(sym, ("?", 0))
        if kind in "DBRGSC" or (kind == "?" and DATA_NAME.search(sym)):
            out.append(f"char {sym}[{max(size, 0x100):#x}] __attribute__((aligned(32)));")
        else:
            out.append(f'void {sym}(void) {{ bwport_undefined("{sym}"); }}')
    Path(path).write_text("\n".join(out) + "\n")


def group(sym, defs, excluded):
    objs = defs.get(sym, [])
    if not objs:
        return "linker", ""
    if any("_dsd_gap@" in o for o in objs) and all("_dsd_gap@" in o or o.startswith("keep/") for o in objs):
        return "gap", objs[0]
    paths = [re.sub(r"^(delinks/|keep/build/b_us/delinks/)", "", o) for o in objs if "_dsd_gap@" not in o]
    src = re.sub(r"\.o$", ".c", sorted(paths, key=len)[0])
    lib = src.split("/")[1] if src.startswith("lib/") else ""
    if src in excluded:
        cat = excluded[src]
        return ("dsi" if cat == "dsi" else "online" if cat == "online" else "excluded"), src
    if lib in ("nitro", "twl"):
        return ("dsi" if DSI_LIB.match(src) else "sdk"), src
    if lib == "nnsys":
        return "nnsys", src
    if lib in ("dwc", "dwcutil", "nitrowifi", "dpw"):
        return "online", src
    if lib in ("dsprot", "msl"):
        return lib, src
    return "game", src


def game_definitions(syms):
    """{symbol: 'path:line'} of the undefined symbols the game's own C defines: a function defined in a file the PC
    build compiles (or in a game header) is undefined only when its definition emits no symbol, as a C99 plain
    `inline` one doesn't (gnu99), and then every call the compiler doesn't inline (all of them at -O0) aborts. The
    fix: a stand-in for that file under port/extern with an extern declaration (tools/port_sources.py)."""
    if not syms:
        return {}
    files = [ROOT / f.removeprefix(port_sources.EXTERN) for f in subprocess.run([sys.executable, ROOT / "tools/port_sources.py", "--relative"],
             capture_output=True, text=True, check=True).stdout.split()]
    files += [p for d in ("include",) + port_sources.GAME_DIRS for p in (ROOT / d).rglob("*.h")]
    # a definition: the name after a return type at the start of a line, its parameters, then '{' with no ';' between
    names = "|".join(map(re.escape, sorted(syms)))
    defn = re.compile(r"^[ \t]*[A-Za-z_][\w \t*]*?\b(" + names + r")\s*\(([^;{]*)\{", re.M)
    found = {}
    for f in files:
        text = f.read_text(errors="replace")
        for m in defn.finditer(text):
            if m[2].rstrip().endswith(")"):
                line = text.count("\n", 0, m.start()) + 1
                found.setdefault(m[1], f"{f.relative_to(ROOT).as_posix()}:{line}")
    return found


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("builddir", type=Path)
    ap.add_argument("-o", "--out", type=Path)
    ap.add_argument("--rom", type=Path, default=ROOT / "build/b_us")
    ap.add_argument("--edge", default="bwport-nostubs", help="the executable whose link to run (build.ninja)")
    ap.add_argument("--stubs", type=Path, help="write a C file that defines every undefined symbol there")
    a = ap.parse_args()
    bdir = a.builddir.resolve()
    out = a.out or ROOT / "build/port/undefined.txt"
    refs = undefined(bdir, a.edge)
    defs, kinds = rom_definitions(a.rom)
    # Black 2's symbols' kinds from pokebw2's config (tools/pokebw2_fetch.sh's checkout, the maintainers' notes): its C's undefined
    # references are told function from data by its ROM's symbols, not by the name (DWC_IsValidFriendData is code)
    for f in sorted((ROOT / "third_party/pokebw2/config").glob("*/arm9/**/symbols.txt")):
        for line in f.read_text().splitlines():
            m = re.match(r"^(\S+) kind:(function|data|bss)(?:\(.*?size=(0x[0-9a-fA-F]+).*?\))?", line)
            if m and m[1] not in kinds:
                kinds[m[1]] = ("T" if m[2] == "function" else "D", int(m[3], 16) if m[3] else 0)
    if a.stubs:
        write_stubs(a.stubs, refs, kinds)
    excluded = {f: cat for f, cat, _ in (l.split("\t") for l in subprocess.run(
        [sys.executable, ROOT / "tools/port_sources.py", "--excluded"], capture_output=True, text=True,
        check=True).stdout.splitlines())}
    by = collections.defaultdict(list)
    for sym in sorted(refs):
        g, src = group(sym, defs, excluded)
        if all(u.startswith("port/subprojects/") for u in refs[sym]):
            g = "libntr"  # only libntr's own code uses it: a libntr gap
        by[g].append((sym, src, sorted(refs[sym])))
    lines = [f"# {sum(len(v) for v in by.values())} undefined symbols of {bdir.relative_to(ROOT) if bdir.is_relative_to(ROOT) else bdir}"
             f" (tools/port_undefined.py): group, symbol, the ROM object that defines it, the PC files that use it"]
    for g in GROUPS:
        lines.append(f"\n## {g}: {len(by[g])}")
        for sym, src, users in by[g]:
            lines.append(f"{g}\t{sym}\t{src or '-'}\t{' '.join(users[:6])}{' ...' if len(users) > 6 else ''}")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(lines) + "\n")
    print(f"{sum(len(v) for v in by.values())} undefined symbols -> {out}")
    for g in GROUPS:
        print(f"  {g:9} {len(by[g])}")
    bad = game_definitions(refs)
    for sym, where in sorted(bad.items()):
        print(f"error: {sym} is undefined, but {where} defines it: a plain `inline` definition emits no symbol under C99;"
              " give that file a stand-in under port/extern with an extern declaration (tools/port_sources.py)",
              file=sys.stderr)
    if bad:
        sys.exit(1)


if __name__ == "__main__":
    main()
