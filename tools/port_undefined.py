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

Function or datum (the stub is a trap or a zeroed block, is_data): a symbol some object CALLS (an ELF relocation
R_X86_64_PLT32, R_AARCH64_CALL26/JUMP26: reloc_uses) is a function; else the ROM objects' kind of it (nm; pokebw2's
symbols.txt); else its name (DATA_NAME); else, an object that only takes its address (a GOT load, a pointer in a table:
a table, or a function whose address is taken) makes it a datum; no relocation seen (COFF, Mach-O: only ELF is read), a
function. Every undefined datum is a real bug (the game reads zeros where a table is missing: hg36's battle hang read a
trap's code bytes as battler types): undefined.txt lists them in "UNDEFINED DATA", the run prints one loud line, and
`--fail-on-data --allow-data FILE` exits 1 for one that FILE (the known ones, one per line) does not name.
"""
import argparse
import collections
import json
import mmap
import os
import re
import shlex
import struct
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
    if os.path.basename(cmd.split()[0]).startswith("ccache"):   # port/machine/linux-ccache.ini: the compiler is `ccache-nopch g++`
        n += 1
    args = sub(rvars["rspfile_content"]) if "rspfile_content" in rvars else cmd.split(None, n)[n]
    args = args.replace("--unresolved-symbols=ignore-all", "--unresolved-symbols=report-all")
    # macOS: ld64 has no such flag; its failed link lists every undefined symbol
    if "-android" in cmd.split()[0]:   # the NDK's clang (port/machine/android-*.ini) links with lld
        args += " -Wl,--error-limit=0"
    return " ".join(cmd.split()[:n]), args + ("" if sys.platform == "darwin" else " -Wl,--warn-unresolved-symbols"), out


def undefined(bdir, edge):
    """({symbol: {referencing source}}, the object and archive files the link reads from the build or the repo) from a
    link with the undefined references as warnings."""
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
    return refs, link_inputs(bdir, args)


def link_inputs(bdir, args):
    """The object and archive files of the link's arguments that are in the build or the repo, in order."""
    inputs = []
    for t in args.split():
        p = (bdir / t).resolve()
        if t.endswith((".o", ".a")) and p.is_file() and (p.is_relative_to(bdir) or p.is_relative_to(ROOT)) and p not in inputs:
            inputs.append(p)
    return inputs


def elf_iface(buf, base, out):
    """Adds (name, undefined, binding) of each global symbol of the ELF64 object buf[base:] to `out`; False for anything
    else (COFF, Mach-O, 32 bit): such a file's interface is its whole content."""
    if bytes(buf[base:base + 6]) != b"\x7fELF\x02\x01" or len(buf) < base + 0x40:
        return False
    shoff, = struct.unpack_from("<Q", buf, base + 0x28)
    shentsize, shnum = struct.unpack_from("<HH", buf, base + 0x3A)
    secs = [struct.unpack_from("<IIQQQQII", buf, base + shoff + i * shentsize) for i in range(shnum)]
    for sec in secs:
        if sec[1] != 2:   # SHT_SYMTAB
            continue
        strtab = base + secs[sec[6]][4]
        syms = memoryview(buf)[base + sec[4]:base + sec[4] + sec[5] // 24 * 24]
        for n, info, _, shndx, _, _ in struct.iter_unpack("<IBBHQQ", syms):
            if n and info >> 4:   # not local
                out.append((bytes(buf[strtab + n:buf.find(b"\0", strtab + n)]), shndx == 0, info >> 4))
    return True


def iface_hash(path):
    """A hash of what a link sees of an object or archive: its global symbols' names and whether each is defined
    (not their addresses or sizes), so a function body edit leaves it as it was; any other file: its content."""
    import hashlib
    with open(path, "rb") as f:
        if os.fstat(f.fileno()).st_size < 8:
            return "empty"
        buf = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
        out, ok = [], True
        if bytes(buf[:8]) in (b"!<arch>\n", b"!<thin>\n"):
            for b, base in archive_files(buf, path):
                out.append(None)
                ok = elf_iface(b, base, out) and ok
        else:
            ok = elf_iface(buf, 0, out)
        if not ok:
            return hashlib.sha1(buf).hexdigest()
        return hashlib.sha1(repr(out).encode()).hexdigest()


def stamp_of(p):
    st = p.stat()
    return [st.st_mtime_ns, st.st_size]


def file_info(p, want):
    """[interface hash, the symbols of `want` the file calls, those it takes the address of] (what the link and the
    relocation scan of this file give, for the cache)"""
    called, addressed = reloc_uses([p], want)
    return [iface_hash(p), sorted(called), sorted(addressed)]


def cached_undefined(bdir, edge):
    """undefined() and reloc_uses() as `--cache` wants them: (refs, inputs, called, addressed). The undefined set is the
    last link's while every changed input file has the interface (global symbol names, defined or not) it had then: the
    link is skipped, and only the changed files' relocations are read again. Any other change (a new symbol, a file
    added) links as before. The cache is bdir/undefined_cache.json."""
    import hashlib
    _, args, _ = link_command(bdir, edge)
    inputs = link_inputs(bdir, args)
    key = hashlib.sha1(args.encode()).hexdigest()
    stamps = {str(p): stamp_of(p) for p in inputs}   # (before the link: a file written meanwhile reads as changed next time)
    cpath = bdir / "undefined_cache.json"
    try:
        cache = json.loads(cpath.read_text())
        ok = cache["args"] == key and sorted(cache["files"]) == sorted(stamps)
    except (OSError, ValueError, KeyError, TypeError):
        cache, ok = None, False
    files = {}
    if ok:
        refs = {s: set(v) for s, v in cache["refs"].items()}
        want = set(refs)
        for p in inputs:
            old = cache["files"][str(p)]
            if old[:2] == stamps[str(p)]:
                files[str(p)] = old
                continue
            info = file_info(p, want)
            if info[0] != old[2]:
                ok = False
                break
            files[str(p)] = stamps[str(p)] + info
    if not ok:
        refs, _ = undefined(bdir, edge)
        want = set(refs)
        files = {str(p): stamps[str(p)] + file_info(p, want) for p in inputs}
    tmp = cpath.with_suffix(f".{os.getpid()}.tmp")
    tmp.write_text(json.dumps({"args": key, "refs": {s: sorted(v) for s, v in refs.items()}, "files": files}, separators=(",", ":")))
    tmp.replace(cpath)
    called = {s for v in files.values() for s in v[3]}
    addressed = {s for v in files.values() for s in v[4]} - called
    print(f"port_undefined: {'cache hit (no link)' if ok else 'cache miss (linked)'}", file=sys.stderr)
    return refs, inputs, called, addressed


def source_of(path):
    """A link message's file (a source path with a line, or an object) as a repo path where it can be one."""
    path = re.sub(r":\d+$", "", path.strip())
    if path.startswith("subprojects/"):   # lld names objects (not source lines), relative to the build dir: libntr's
        return "port/" + path
    m = re.search(r"\.p/(?:_|([0-9a-f]{40})_)", path)   # meson's object name: the path with / as _, or a hash and the path
    if m:
        mangled = re.sub(r"^.*?\.p/", "", path)
        mangled = re.sub(r"\.c\.o(bj)?$", ".c", mangled)
        mangled = mangled[41:] if m[1] else mangled[len(str(ROOT).replace("/", "_")) + 1:]
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


NM_CACHE = ROOT / "build/port/rom_definitions.json"


def rom_definitions(rom, cache=NM_CACHE):
    """{symbol: [repo-style path of the defining ROM object]}: build/b_us/<dir>/x.o -> <dir>/x.c, delinks too; and
    {symbol: (nm type letter, size)}. nm over the ~3200 objects took 17 s of every relink, so each object's symbols are
    kept in `cache` (a JSON file) under its mtime and size: only the objects `ninja` rewrote since are read again."""
    frozen = os.environ.get("BWPORT_ROM_DEFS_FROZEN") == "1"   # slot jobs: no ROM build; `cache` was made for this decomp tree
    try:
        kept = json.loads(cache.read_text())
        kept = kept["objs"] if (frozen or kept["rom"] == str(rom.resolve())) else {}
    except (OSError, ValueError, KeyError, TypeError):
        if frozen:
            raise SystemExit(f"port_undefined: BWPORT_ROM_DEFS_FROZEN=1 but {cache} is unreadable")
        kept = {}
    objs = [] if frozen else sorted(rom.rglob("*.o"))
    stamp = {}
    for p in objs:
        st = p.stat()
        stamp[p.relative_to(rom).as_posix()] = [st.st_mtime_ns, st.st_size]
    fresh = dict(kept) if frozen else {rel: kept[rel] for rel in stamp if rel in kept and kept[rel][:2] == stamp[rel]}
    todo = [rel for rel in stamp if rel not in fresh]
    failed = set()
    for i in range(0, len(todo), 400):
        batch = todo[i:i + 400]
        r = subprocess.run(["nm", "-A", "-S", "--defined-only", *(str(rom / rel) for rel in batch)],
                           capture_output=True, text=True, errors="replace")
        found = {rel: [] for rel in batch}
        for line in r.stdout.splitlines():
            m = re.match(r"^(.*?\.o):\s*[0-9a-fA-F]*(?:\s+([0-9a-fA-F]+))?\s+([A-Za-z])\s+(\S+)$", line)
            if m and m[3] not in "aNn" and m[3].isupper():
                found[Path(m[1]).relative_to(rom).as_posix()].append([m[4], m[3], int(m[2], 16) if m[2] else 0])
        for rel in batch:
            fresh[rel] = stamp[rel] + [found[rel]]
            if r.returncode:   # a failed nm run is used this time, not kept
                failed.add(rel)
    if todo:
        keep = {rel: v for rel, v in fresh.items() if rel not in failed}
        try:
            cache.parent.mkdir(parents=True, exist_ok=True)
            tmp = cache.with_suffix(f".{os.getpid()}.tmp")
            tmp.write_text(json.dumps({"rom": str(rom.resolve()), "objs": keep}, separators=(",", ":")))
            tmp.replace(cache)
        except OSError:
            pass
    defs, kinds = collections.defaultdict(list), {}
    for rel in sorted(fresh):
        for sym, letter, size in fresh[rel][2]:
            defs[sym].append(rel)
            if sym not in kinds or (size and kinds[sym][1] == 0):
                kinds[sym] = (letter, size)
    return defs, kinds


# the relocation types that are calls, by ELF e_machine: EM_X86_64 R_X86_64_PLT32 (clang and gcc: a call, a tail jmp),
# EM_AARCH64 R_AARCH64_JUMP26, CALL26. Every other relocation against a symbol is an address (a GOT load, a pointer
# in a table), whether of a datum or of a function whose address is taken
CALL_RELOCS = {62: {4}, 183: {282, 283}}


def elf_uses(buf, base, want, called, addressed):
    """Adds the symbols of `want` that the ELF64 object buf[base:] calls to `called`, those it references any other way
    to `addressed`. Anything else (COFF, Mach-O, 32 bit, another machine) is not read."""
    if bytes(buf[base:base + 6]) != b"\x7fELF\x02\x01" or len(buf) < base + 0x40:
        return
    calls = CALL_RELOCS.get(struct.unpack_from("<H", buf, base + 0x12)[0])
    if not calls:
        return
    shoff, = struct.unpack_from("<Q", buf, base + 0x28)
    shentsize, shnum = struct.unpack_from("<HH", buf, base + 0x3A)
    secs = [struct.unpack_from("<IIQQQQII", buf, base + shoff + i * shentsize) for i in range(shnum)]
    # a section: (name, type, flags, addr, offset, size, link, info)
    symtabs = {}   # symbol table's section index -> {symbol index: name} of its undefined symbols that are in `want`
    for i, sec in enumerate(secs):
        if sec[1] != 2:   # SHT_SYMTAB
            continue
        strtab = base + secs[sec[6]][4]
        found = symtabs[i] = {}
        syms = memoryview(buf)[base + sec[4]:base + sec[4] + sec[5] // 24 * 24]
        for j, (n, _, _, shndx, _, _) in enumerate(struct.iter_unpack("<IBBHQQ", syms)):
            if shndx == 0 and n:
                name = bytes(buf[strtab + n:buf.find(b"\0", strtab + n)]).decode(errors="replace")
                if name in want:
                    found[j] = name
    for sec in secs:
        wanted = symtabs.get(sec[6]) if sec[1] == 4 else None   # SHT_RELA: its symbol table
        if not wanted:
            continue
        # (offset, info, addend) as 64 bit words: info = symbol index << 32 | type
        for info in memoryview(buf)[base + sec[4]:base + sec[4] + sec[5] // 24 * 24].cast("Q")[1::3]:
            name = wanted.get(info >> 32)
            if name:
                (called if info & 0xffffffff in calls else addressed).add(name)


def archive_files(buf, path):
    """(buf, offset) of each object of the ar archive in buf: its members, or for a thin archive the files it names"""
    thin = bytes(buf[:8]) == b"!<thin>\n"
    pos, names = 8, b""
    while pos + 60 <= len(buf):
        head = bytes(buf[pos:pos + 60])
        name, size = head[:16].decode(errors="replace").strip(), int(head[48:58])
        pos += 60
        special = name in ("/", "//")
        if name == "//":
            names = bytes(buf[pos:pos + size])
        elif not special:
            if name[1:].isdigit() and name[0] == "/":   # a long name: an offset into the "//" member's table
                name = names[int(name[1:]):].split(b"\n")[0].decode(errors="replace")
            name = name.removesuffix("/")
            if not thin:
                yield buf, pos
            else:
                for p in (path.parent / name, path.parent.parent / name):   # beside the archive, or relative to the build dir
                    if p.is_file():
                        with open(p, "rb") as f:
                            yield f.read(), 0
                        break
        if special or not thin:
            pos += size + (size & 1)


def reloc_uses(files, syms):
    """(called, addressed): the symbols of `syms` that the ELF objects and archives `files` call, and those they
    reference any other way (see CALL_RELOCS). A symbol in neither is not referenced by a relocation we can read."""
    called, addressed, want = set(), set(), set(syms)
    for path in files:
        try:
            with open(path, "rb") as f:
                if os.fstat(f.fileno()).st_size < 8:
                    continue
                buf = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
                if bytes(buf[:8]) in (b"!<arch>\n", b"!<thin>\n"):
                    for b, base in archive_files(buf, path):
                        elf_uses(b, base, want, called, addressed)
                else:
                    elf_uses(buf, 0, want, called, addressed)
        except (OSError, ValueError, IndexError, struct.error) as e:   # a file we can't read: its symbols fall back to kind and name
            print(f"port_undefined: {path}: not read for relocations ({e})", file=sys.stderr)
    return called, addressed - called


DATA_NAME = re.compile(r"^(data_|Data|s_|g_|[sg][A-Z])|(List|Table|Tbl|Data|Entries|Buf|Work)$")


def is_data(sym, kinds, called=(), addressed=()):
    """True for a datum, False for a function (see the docstring): called, else the ROM's kind, else the name, else
    only its address taken."""
    if sym in called:
        return False
    kind = kinds.get(sym, ("?", 0))[0]
    if kind != "?":
        return kind in "DBRGSC"
    return bool(DATA_NAME.search(sym)) or sym in addressed


def write_stubs(path, syms, kinds, called=(), addressed=()):
    """A C file that defines every undefined symbol: a trap for a function, a zeroed block for data (the ROM's size,
    at least 256 bytes). No game header: the definitions only have to link."""
    out = ["// Written by tools/port_undefined.py --stubs: the symbols the PC link lacks. Not game code: each function",
           "// prints its name and aborts, each datum is a zeroed block. Define the real thing in port/src or the C.",
           "// BWPORT_UNDEFINED=return0: a function says its name once and returns 0 instead (a game brought up past",
           "// the names other jobs own; any return in a register, void or not, gets 0)",
           "#include <stdio.h>", "#include <stdlib.h>", "#include <string.h>", "",
           "static long bwport_undefined(const char *name, int *said) {",
           '    const char *how = getenv("BWPORT_UNDEFINED");', "",
           '    if (how != NULL && strcmp(how, "return0") == 0) {',
           "        if (!*said) {",
           '            fprintf(stderr, "bwport: undefined function %s called: returns 0 (BWPORT_UNDEFINED)\\n", name);',
           "            *said = 1;", "        }", "        return 0;", "    }",
           '    fprintf(stderr, "bwport: undefined function %s called\\n", name);', "    fflush(stderr);",
           "    abort();", "}", ""]
    for sym in sorted(syms):
        size = kinds.get(sym, ("?", 0))[1]
        if is_data(sym, kinds, called, addressed):
            out.append(f"char {sym}[{max(size, 0x100):#x}] __attribute__((aligned(32)));")
        else:
            out.append(f'long {sym}(void) {{ static int said; return bwport_undefined("{sym}", &said); }}')
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
    ap.add_argument("--allow-data", type=Path, help="a file of the undefined data symbols that are known (one per line, # comments)")
    ap.add_argument("--fail-on-data", action="store_true", help="exit 1 for an undefined datum that --allow-data does not name")
    ap.add_argument("--cache", action="store_true", help="skip the link while the changed input files' global symbols are as at the last run (dev builds)")
    a = ap.parse_args()
    bdir = a.builddir.resolve()
    out = a.out or ROOT / "build/port/undefined.txt"
    if a.cache:
        refs, inputs, called, addressed = cached_undefined(bdir, a.edge)
    else:
        refs, inputs = undefined(bdir, a.edge)
        called, addressed = reloc_uses(inputs, refs)
    defs, kinds = rom_definitions(a.rom)
    # Black 2's symbols' kinds from pokebw2's config (tools/pokebw2_fetch.sh's checkout, the maintainers' notes): its C's undefined
    # references are told function from data by its ROM's symbols, not by the name (DWC_IsValidFriendData is code)
    for f in sorted((ROOT / "third_party/pokebw2/config").glob("*/arm9/**/symbols.txt")):
        for line in f.read_text().splitlines():
            m = re.match(r"^(\S+) kind:(function|data|bss)(?:\(.*?size=(0x[0-9a-fA-F]+).*?\))?", line)
            if m and m[1] not in kinds:
                kinds[m[1]] = ("T" if m[2] == "function" else "D", int(m[3], 16) if m[3] else 0)
    if a.stubs:
        write_stubs(a.stubs, refs, kinds, called, addressed)
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
    allowed = set()
    if a.allow_data:
        allowed = {w for l in a.allow_data.read_text().splitlines() for w in l.split("#")[0].split()}
    data = [(g, sym, src, users) for g in GROUPS for sym, src, users in by[g] if is_data(sym, kinds, called, addressed)]
    new = [sym for _, sym, _, _ in data if sym not in allowed]
    lines.append(f"\n## UNDEFINED DATA: {len(data)} ({len(new)} not in the allow file)")
    lines.append("# each is a zeroed block the game reads (a table or a struct nothing defines): a bug to fix, not a gap to wait out")
    for g, sym, src, users in data:
        lines.append(f"data\t{sym}\t{'known' if sym in allowed else 'NEW'}\t{g}:{src or '-'}\t{' '.join(users[:6])}{' ...' if len(users) > 6 else ''}")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(lines) + "\n")
    print(f"{sum(len(v) for v in by.values())} undefined symbols -> {out}")
    for g in GROUPS:
        print(f"  {g:9} {len(by[g])}")
    if data:
        names = " ".join(sym for _, sym, _, _ in data)
        print(f"UNDEFINED DATA: {len(data)} symbols are zeroed blocks, the game reads zeros from them ({len(new)} new): "
              f"{names[:300]}{' ...' if len(names) > 300 else ''} (undefined.txt)")
        if new and a.fail_on_data:
            print(f"error: undefined data not in {a.allow_data}: {' '.join(new)}: nothing defines them, so the game reads "
                  "zeros (define each, or name it in the allow file when it is known)", file=sys.stderr)
    bad = game_definitions(refs)
    for sym, where in sorted(bad.items()):
        print(f"error: {sym} is undefined, but {where} defines it: a plain `inline` definition emits no symbol under C99;"
              " give that file a stand-in under port/extern with an extern declaration (tools/port_sources.py)",
              file=sys.stderr)
    if bad or (new and a.fail_on_data):
        sys.exit(1)


if __name__ == "__main__":
    main()
