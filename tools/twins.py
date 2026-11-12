#!/usr/bin/env python3
"""Find the functions of Pokemon Black that pokebw2 already has as C ("twins").

Compiles pokebw2's C files (the game's once per compiler in --compilers, each library with its own compiler), then
hashes every function of those objects and of Black's delinked objects: ARM/Thumb mode, size and bytes, with the
relocated fields zeroed. Equal hashes make a twin. Prints the counts and writes one TSV row per twin.

    python3 tools/twins.py --bw2 ~/ref/pokebw2 --compilers dsi/1.1p1,dsi/1.1 --tsv build/twins_b_us.tsv

--extra compiles chosen directories once more with another compiler and flags added to the game's. Black builds Game
Freak's library (pokebw2's src/gfl) as ARM where Black 2 builds it as Thumb: `--extra 'dsi/1.2:-nothumb:src/gfl'`
finds its twins (dsi/1.2 to 1.3p1 alike; 1.1p1 misses GFL_RandomMT; -ipa file matters, src/system stays Thumb).

Needs tools/wibo and tools/mwccarm/<version>/ (git-ignored). configure.py only downloads the dsi/ compilers; pokebw2's
libraries also need 1.2/base and 2.0/sp2p2 from the same mwccarm.zip. pokebw2 generates its constant and message ID
headers (since 6f728ff, 2026-10-10): `python3 configure.py b2_us && ninja constants_headers` there first (no ROM
needed, but clang and llvm-objcopy), or a third of its files don't compile. Objects are cached in --work, keyed on
their sources, headers, compiler and flags (compile_all), so a rerun compiles only what changed: a rerun without changes
takes 2 s, all 2,459 objects 2 minutes at -j 2, pokebw2's last 3 PRs of 2026-10-13 (718 objects) 54 s.
tools/resync_pokebw2.sh is the whole re-sync with pokebw2, this file's tables (ARM, and Thumb without --extra) included.

Small functions match by accident (a 4-byte `bx lr`), and wrappers match when only their call targets differ.
"""
import argparse
import ast
import glob
import hashlib
import os
import re
import struct
import subprocess
import sys
import tomllib
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

# Bytes a relocation covers, if not 4: ABS16, THM_ABS5, ABS8, THM_JUMP11, THM_JUMP8
SHORT_RELOCS = {5: 2, 7: 2, 8: 1, 102: 2, 103: 2}


def functions(path):
    """Returns [(name, offset, size, mode, digest)] for the functions of a relocatable ARM ELF, relocations masked."""
    d = open(path, "rb").read()
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", d, 0x2E)
    # Section headers: name, type, flags, addr, offset, size, link, info, align, entsize
    sh = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    syms, modes, relocs = [], {}, defaultdict(list)
    for s in sh:
        if s[1] == 2:  # SYMTAB
            strtab = sh[s[6]][4]
            for i in range(s[5] // 16):
                name, value, size, info, _, ndx = struct.unpack_from("<IIIBBH", d, s[4] + 16 * i)
                name = d[strtab + name:d.index(b"\0", strtab + name)].decode()
                if name in ("$a", "$t"):
                    modes[ndx, value] = name[1]
                elif info & 15 == 2 and size and not name.startswith("$"):  # FUNC
                    syms.append((name, ndx, value, size))
        elif s[1] in (4, 9):  # RELA, REL
            for o in range(s[4], s[4] + s[5], 12 if s[1] == 4 else 8):
                off, info = struct.unpack_from("<II", d, o)
                relocs[s[7]].append((off, SHORT_RELOCS.get(info & 255, 4)))
    out = []
    for name, ndx, value, size in syms:
        if not sh[ndx][2] & 4:  # not executable
            continue
        code = bytearray(d[sh[ndx][4] + value:sh[ndx][4] + value + size])
        for off, width in relocs[ndx]:
            if value <= off < value + size:
                code[off - value:off - value + width] = bytes(width)
        mode = modes.get((ndx, value), "a")
        out.append((name, value, size, mode, hashlib.sha1(mode.encode() + bytes(code)).digest()))
    return out


def parse_extra(extra):
    """'COMPILER:FLAGS:DIRS' (--extra) -> (tag, compiler, flags, directories)"""
    c, flags, dirs = extra.split(":")
    return c + flags.replace(" ", ""), c, flags, dirs.split(",")


# The tags of pokebw2's library C built as Black builds its own libraries (black_lib_jobs), and the flags each adds
BLACK_LIB = {"black-lib": [], "black-lib-ipa": ["-ipa file"]}


def black_path(root, lib, rel):
    """Black's path under lib/<lib>/src/ for pokebw2's lib/<lib>/src/<rel>: the same path, else the one file of Black's
    library with that name (Black keeps GameSpy under gs/: serverbrowsing/sb_serverlist.c is
    gs/serverbrowsing/sb_serverlist.c), else rel itself (the library's flags, no file's)."""
    if os.path.isfile(f"{root}/lib/{lib}/src/{rel}"):
        return rel
    same = glob.glob(f"{root}/lib/{lib}/src/**/{os.path.basename(rel)}", recursive=True)
    return os.path.relpath(same[0], f"{root}/lib/{lib}/src") if len(same) == 1 else rel


def black_lib_jobs(bw2, root, work, command):
    """[(tag, source, object, command, env)] of pokebw2's library C (lib/<name>/src/) built with the compiler and flags
    Black's lib/<name>/library.toml gives the same file (configure.py's compiler_of), once as they are (black-lib) and
    once with -ipa file (black-lib-ipa). Black 2 builds most of DWC and NitroWiFi as Thumb code where Black builds them as
    ARM, so its own build of them (the lib tag) twins none of Black's: built as Black's, ghttpProcess.c, ghttpEncryption.c,
    nhttp_recvbuf.c and sb_serverlist.c give 16 of 18, 17 of 17, 8 of 9 and 55 of 55 twins. Flags that name Black's
    headers or defines (a [sdk] block's -i, -d, -prefix) are left out: the C is pokebw2's, with its headers."""
    sys.path.insert(0, root)
    from configure import compiler_of
    jobs = []
    for toml in sorted(glob.glob(f"{root}/lib/*/library.toml")):
        lib = os.path.basename(os.path.dirname(toml))
        base = f"{bw2}/lib/{lib}/src"
        for src in sorted(glob.glob(f"{base}/**/*.c", recursive=True)):
            rel = src[len(base) + 1:]
            compiler, flags = compiler_of(Path(f"lib/{lib}/src/{black_path(root, lib, rel)}"))
            flags = [f for f in flags if f.split()[0] not in ("-i", "-I", "-ir", "-d", "-D", "-prefix", "-include")]
            for tag, more in BLACK_LIB.items():
                skip = {m.split()[0] for m in more}
                obj = f"{work}/obj/{tag}/{src[len(bw2) + 1:-2]}.o"
                cmd = command(compiler, [f for f in flags if f.split()[0] not in skip] + more, obj, src)
                jobs.append((tag, src, obj, cmd, dict(os.environ, MWCIncludes=".")))
    return jobs


def compile_jobs(bw2, tools, work, compilers, extras, black_libs=None):
    """Returns [(tag, source, object, command, env)]: the game code once per compiler, each library with its own, and
    each extra (parse_extra) once; with black_libs (Black's checkout), the libraries as Black builds them too
    (black_lib_jobs)."""
    cfg = ast.parse(open(f"{bw2}/configure.py").read())
    # CC_FLAGS's items are strings or the names of module-level strings (ENCODING_FLAG = "-enc SJIS", 2026-10)
    strings = {n.targets[0].id: n.value.value for n in cfg.body if isinstance(n, ast.Assign)
               and isinstance(n.targets[0], ast.Name) and isinstance(n.value, ast.Constant) and isinstance(n.value.value, str)}
    game_flags = next([e.value if isinstance(e, ast.Constant) else strings[e.id] for e in n.value.elts] for n in cfg.body
                      if isinstance(n, ast.Assign) and isinstance(n.targets[0], ast.Name) and n.targets[0].id == "CC_FLAGS")
    includes = []
    for p in [f"{bw2}/include", f"{bw2}/build/include/generated", *sorted(glob.glob(f"{bw2}/lib/*/include"))]:
        includes += ["-i", p]

    def command(compiler, flags, obj, src):
        flags = [t for f in flags for t in f.split()]
        exe = f"{tools}/mwccarm/{compiler}/mwccarm.exe"
        return [f"{tools}/wibo", exe, *flags, "-d", "BLACK2", "-gccdep", "-MD", *includes, "-o", obj, src]

    jobs = []
    for src in sorted(glob.glob(f"{bw2}/src/**/*.c", recursive=True)):
        for c in compilers:
            obj = f"{work}/obj/{c}/{src[len(bw2) + 1:-2]}.o"
            jobs.append((c, src, obj, command(c, game_flags, obj, src), None))
    for tag, c, flags, dirs in extras:
        for d in dirs:
            for src in sorted(glob.glob(f"{bw2}/{d}/**/*.c", recursive=True)):
                obj = f"{work}/obj/{tag}/{src[len(bw2) + 1:-2]}.o"
                jobs.append((tag, src, obj, command(c, game_flags + [flags], obj, src), None))
    for toml in sorted(glob.glob(f"{bw2}/lib/*/library.toml")):
        lib = tomllib.load(open(toml, "rb"))
        base = os.path.dirname(toml)
        for src in sorted(glob.glob(f"{base}/src/**/*.c", recursive=True)):
            over = lib.get("file_flags", {}).get(src[len(base) + 5:], [])
            skip = {f.split()[0] for f in over}
            flags = [f for f in lib["flags"] if f.split()[0] not in skip] + over
            obj = f"{work}/obj/lib/{src[len(bw2) + 1:-2]}.o"
            jobs.append(("lib", src, obj, command(lib["compiler"], flags, obj, src), dict(os.environ, MWCIncludes=".")))
    if black_libs:
        jobs += black_lib_jobs(bw2, black_libs, work, command)
    return jobs


def compile_all(bw2, jobs, threads):
    """Compiles the jobs whose object is missing or stale. Returns [(job, error)] of those that failed (their objects
    are deleted) and how many compiled.

    Next to each object, its .key file holds a hash of its command (without the paths; wibo and the compiler hashed
    whole) and of every file its compile read, then those files, from the dependency file MWCC writes (-MD), relative
    to pokebw2. A changed hash recompiles, as a changed header does in ninja; the checkout may move."""
    bw2 = os.path.abspath(bw2)
    digests = {}

    def digest(path):
        if path not in digests:
            digests[path] = hashlib.sha1(open(path, "rb").read()).hexdigest() if os.path.isfile(path) else "-"
        return digests[path]

    def key(cmd, deps):
        parts, args = [digest(cmd[0]), digest(cmd[1])], iter(cmd[2:-1])
        for a in args:
            if a in ("-i", "-o"):
                next(args)
            else:
                parts.append(a)
        parts += [f"{d} {digest(os.path.join(bw2, d))}" for d in deps]
        return hashlib.sha1("\n".join(parts).encode()).hexdigest()

    def cached(job):
        try:
            stored, *deps = open(job[2][:-2] + ".key").read().splitlines()
        except FileNotFoundError:
            return False
        return os.path.exists(job[2]) and stored == key(job[3], deps)

    def build(job):
        tag, src, obj, cmd, env = job
        os.makedirs(os.path.dirname(obj), exist_ok=True)
        r = subprocess.run(cmd, env=env, capture_output=True, text=True)
        if r.returncode:
            for f in (obj, obj[:-2] + ".key"):
                if os.path.exists(f):
                    os.remove(f)
            return job, next((l for l in (r.stdout + r.stderr).splitlines() if "error" in l.lower()), "failed")
        deps = []
        # Windows paths (Z:\root\...), the source's relative to the current directory
        for p in open(obj[:-2] + ".d").read().partition(": ")[2].split():
            if p != "\\":
                p = os.path.abspath(re.sub(r"^[A-Za-z]:", "", p).replace("\\", "/"))
                deps.append(os.path.relpath(p, bw2) if p.startswith(bw2 + "/") else p)
        os.remove(obj[:-2] + ".d")
        with open(obj[:-2] + ".key", "w") as f:
            f.write("\n".join([key(cmd, deps), *deps]) + "\n")
        return job, ""

    todo = [j for j in jobs if not cached(j)]
    with ThreadPoolExecutor(threads) as pool:
        return [(j, e) for j, e in pool.map(build, todo) if e], len(todo)


def black_functions(delinks, symbols):
    """Returns [(module, name, address, size, mode, digest)] for every function of Black's delinked objects."""
    addr = {}
    for path in glob.glob(f"{symbols}/**/symbols.txt", recursive=True):
        mod = os.path.basename(os.path.dirname(path))
        mod = "main" if mod == "arm9" else mod
        for line in open(path):
            m = re.match(r"(\S+) kind:function\(.*\) addr:(0x\w+)", line)
            if m:
                addr[mod, m[1]] = int(m[2], 16)
    # dsd writes a gap's object as _dsd_gap@<module>_<n>.o and a delinks.txt file's under its path: its module is
    # the delinks.txt that lists it
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from file_ranges import read_delinks
    module_of = {os.path.splitext(f)[0]: mod
                 for mod, (_, files) in read_delinks(os.path.dirname(symbols)).items() for f in files}
    out = []
    for path in sorted(glob.glob(f"{delinks}/**/*.o", recursive=True)):
        gap = re.match(r".*@(.*)_\d+\.o$", path)
        mod = gap[1] if gap else module_of.get(os.path.splitext(os.path.relpath(path, delinks))[0])
        if mod is None:
            continue
        out += [(mod, n, addr.get((mod, n)), size, mode, h) for n, _, size, mode, h in functions(path)]
    return out, addr


def main():
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--bw2", default=os.path.expanduser("~/ref/pokebw2"), help="pokebw2 checkout (read only)")
    ap.add_argument("--tools", default=f"{root}/tools", help="directory with wibo and mwccarm/")
    ap.add_argument("--delinks", default=f"{root}/build/b_us/delinks", help="Black's delinked objects")
    ap.add_argument("--symbols", default=f"{root}/config/b_us/arm9", help="dsd's symbols.txt tree")
    ap.add_argument("--work", default=f"{root}/build/twins_work", help="cache of the compiled objects")
    ap.add_argument("--compilers", default="dsi/1.1p1,dsi/1.1", help="compilers for the game code, comma separated")
    ap.add_argument("--extra", action="append", default=[], metavar="COMPILER:FLAGS:DIRS",
                    help="also compile these pokebw2 directories (comma separated) with this compiler and the game "
                         "flags plus FLAGS, where the last of an option wins; repeatable")
    ap.add_argument("--black-libs", action="store_true", help="also build pokebw2's lib/*/src as Black builds its own "
                    "libraries (Black's library.toml: tags black-lib, and black-lib-ipa with -ipa file)")
    ap.add_argument("--tsv", default=f"{root}/build/twins_b_us.tsv")
    ap.add_argument("-j", type=int, default=os.cpu_count())
    args = ap.parse_args()
    compilers = args.compilers.split(",")
    extras = [parse_extra(e) for e in args.extra]
    tags = list(dict.fromkeys(compilers + [e[0] for e in extras] + (list(BLACK_LIB) if args.black_libs else [])))

    jobs = compile_jobs(args.bw2, args.tools, args.work, compilers, extras, root if args.black_libs else None)
    os.makedirs(args.work, exist_ok=True)
    failed, compiled = compile_all(args.bw2, jobs, args.j)
    with open(f"{args.work}/failed.txt", "w") as f:
        f.writelines(f"{j[0]}\t{j[1][len(args.bw2) + 1:]}\t{e}\n" for j, e in failed)
    print(f"{len(jobs)} objects, {compiled} compiled, {len(failed)} failed (see {args.work}/failed.txt)",
          dict(Counter(j[0] for j, _ in failed)))

    # digest -> (source, function) for each tag, from the objects that compiled
    index = defaultdict(lambda: defaultdict(list))
    for tag, src, obj, _, _ in jobs:
        if os.path.exists(obj):
            for name, _, _, _, h in functions(obj):
                index[tag][h].append((src[len(args.bw2) + 1:], name))

    black, listed = black_functions(args.delinks, args.symbols)
    groups = Counter("overlay" if m.startswith("ov") else m for m, _ in listed)
    print(f"Black: {len(listed)} functions in symbols.txt {dict(groups)}, {len(black)} of them with a size")

    twins = {}  # (module, name) -> {compiler: (source, function)}
    for c in tags:
        game = lib = big = new = 0
        for mod, name, addr, size, mode, h in black:
            hit = index[c].get(h)
            if hit:
                game += 1
                new += c not in compilers and (mod, name) not in twins
            elif c in compilers and index["lib"].get(h):
                hit, lib = index["lib"][h], lib + 1
            if hit:
                twins.setdefault((mod, name), {})[c] = hit[0]
                big += size >= 16
        print(f"{c}: {game + lib} twins, {game} game, {lib} lib, {big} of them >= 16 bytes"
              + (f", {new} not found before" if c not in compilers else ""))
    first = {k for k, v in twins.items() if compilers[0] in v}
    print(f"{compilers[0]} twins by module:", dict(Counter("overlay" if m.startswith("ov") else m for m, _ in first)))
    for c in compilers[1:]:
        other = {k for k, v in twins.items() if c in v}
        print(f"only {compilers[0]} vs {c}: {len(first - other)}, e.g. {sorted(first - other)[:2]}; "
              f"only {c}: {len(other - first)}, e.g. {sorted(other - first)[:2]}")

    info = {(m, n): (a, s) for m, n, a, s, _, _ in black}
    with open(args.tsv, "w") as f:
        f.write("black_symbol\tmodule\taddress\tsize\tbw2_source\tbw2_function\tcompilers\n")
        for (mod, name), hits in sorted(twins.items(), key=lambda kv: (kv[0][0], info[kv[0]][0] or 0)):
            src, fn = hits[next(c for c in tags if c in hits)]
            addr, size = info[mod, name]
            f.write(f"{name}\t{mod}\t{addr:#010x}\t{size}\t{src}\t{fn}\t{','.join(hits)}\n")
    print(f"{len(twins)} twins in any build, listed in {args.tsv}")


if __name__ == "__main__":
    main()
