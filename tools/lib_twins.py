#!/usr/bin/env python3
"""Count Black's functions that other DS decompilations have in C: SDK and library code ("library twins").

Compiles each source's library C (SOURCES, checkouts under --ref) once per variant (VARIANTS: a compiler, ARM or Thumb,
with or without -ipa file, the ways pokebw2's lib/*/library.toml builds the same libraries for Black 2), hashes their
functions as tools/clones.py hashes Black's (mode and bytes, relocated fields zeroed), and counts Black's functions
with the same code, per source and library: all, those of 16 bytes or more, those pokebw2's C doesn't have
(build/twins_b_us.tsv) and those this repo doesn't match yet. Writes one TSV row per Black function and source.

    nice -n 10 python3 tools/lib_twins.py -j 4                     # every source, 15 minutes the first time
    python3 tools/lib_twins.py --sources ntrtwl-NitroSDK,dsprot-1.27

Any source's C may be read to write natural C from it; MIT and CC0 code (the table's second column) may be ported as
it is, with credit (a project decision of 2026-10-13). tools/brief.py and tools/local_driver.py show a
function's twin's C from the TSV (twin_c), and, for a function with no twin whose name is a function of Nintendo's
reference sources (NAMED), that function's first 60 lines (name_c). Each source's commit is printed. Objects are
cached under --work; a rerun
only compiles what is missing (or failed: delete the .failed files to retry). Standard headers come from zeldaret/st's
MSL (CC0) when a source has none. Needs every compiler of mwccarm.zip in tools/mwccarm, where configure.py only puts
what the build uses:
    curl -sLo build/mwccarm.zip https://decomp.aetias.com/files/mwccarm.zip && python3 -m zipfile -e build/mwccarm.zip tools/
"""
import argparse
import glob
import json
import os
import re
import subprocess
import sys
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor

import clones
from file_ranges import ROOT

SDK = ["SDK_CW_FORCE_EXPORT_SUPPORT", "SDK_TS", "SDK_4M", "SDK_ARM9", "SDK_CW", "SDK_FINALROM", "NNS_FINALROM"]
NTR = ["ntrtwl-NitroSDK/include", "ntrtwl-NitroSDK/gen"]
NNS = ["ntrtwl-NitroSystem/include", "ntrtwl-NitroSystem/libraries/*/include"]
WIFI = ["ntrtwl-NitroWiFi/include", "ntrtwl-NitroWiFi/libraries/*/include"]
STD = ["zeldaret-st/libs/c/include"]
LIBNTR = ["SDK_CW_FORCE_EXPORT_SUPPORT", "SDK_TS", "SDK_4M", "SDK_FINALROM", "NNS_FINALROM", "SDK_ARM9", "SDK_CW",
          "SDK_BUILD_ARM", "SDK_32BIT"]  # its meson.build for build_target arm
# name (the checkout under --ref, then @ and a configuration's name): (license, URL, include dirs under --ref (a
# trailing /** searches recursively), defines, source globs, the library of a source path: the regex's first group
# that matched)
SOURCES = {
    "ntrtwl-NitroSDK": ("none", "https://github.com/ntrtwl/NitroSDK", NTR + ["ntrtwl-NitroSDK/libraries/*/include"],
                        SDK, ["libraries/*/src/*.c"], r"libraries/([^/]+)/"),
    "ntrtwl-NitroSystem": ("none", "https://github.com/ntrtwl/NitroSystem", NNS + NTR, SDK,
                           ["libraries/*/src/**/*.c"], r"libraries/([^/]+)/"),
    "ntrtwl-NitroWiFi": ("none", "https://github.com/ntrtwl/NitroWiFi", WIFI + NTR, SDK,
                         ["libraries/**/*.c"], r"libraries/([^/]+)/"),
    "ntrtwl-NitroDWC": ("none", "https://github.com/ntrtwl/NitroDWC",
                        ["ntrtwl-NitroDWC/include", "ntrtwl-NitroDWC/include/gs", "ntrtwl-NitroDWC/include/base"]
                        + WIFI + NNS + NTR, SDK + ["_NITRO"], ["libraries/**/*.c"], r"libraries/(gs/[^/.]+|[^/]+)"),
    "ntrtwl-libvct": ("none", "https://github.com/ntrtwl/libvct",
                      ["ntrtwl-libvct/include", "ntrtwl-libvct/src"] + WIFI + NTR, SDK, ["src/*.c"], r"(src)/"),
    "libntr": ("MIT", "https://github.com/cybervisi0n/libntr",
               ["libntr/include", "libntr/gen", "libntr/libraries/*/include"],
               LIBNTR + ["SDK_NITRO", "SDK_VERSION_MAJOR=4UL"], ["libraries/*/src/*.c"], r"libraries/([^/]+)/"),
    "libntr@twl": ("MIT", "https://github.com/cybervisi0n/libntr, sdk_version_major 5, hardware_type twl_hyb",
                   ["libntr/include", "libntr/gen", "libntr/libraries/*/include"],
                   LIBNTR + ["SDK_TWL", "SDK_TWLHYB", "SDK_VERSION_MAJOR=5UL"], ["libraries/*/src/*.c"],
                   r"libraries/([^/]+)/"),
    "pokediamond": ("none", "https://github.com/pret/pokediamond",
                    ["pokediamond/include-mw/**", "pokediamond/arm9/lib/MSL_C/include/**",
                     "pokediamond/arm9/lib/libnns/include/**", "pokediamond/arm9/lib/NitroSDK/include/**",
                     "pokediamond/include"], ["FS_IMPLEMENT", "DIAMOND", "ENGLISH"],
                    ["arm9/lib/*/src/*.c"], r"NitroSDK/src/([A-Za-z0-9]+)_|lib/([^/]+)/"),
    "pokeplatinum": ("none", "https://github.com/pret/pokeplatinum",
                     ["pokeplatinum/lib/*/include", "pokeplatinum/include"] + NNS + WIFI + NTR, SDK,
                     ["lib/*/src/*.c", "lib/ppwlobby/*.c"], r"lib/([^/]+)/"),
    "pokeheartgold": ("none", "https://github.com/pret/pokeheartgold",
                      ["pokeheartgold/lib/dsprot/include"] + STD + ["pokeheartgold/lib/include"] + NTR, SDK,
                      ["lib/dsprot/src/*.c"], r"lib/([^/]+)/"),
    "dsprot-1.27": ("none", "https://github.com/taxicat1/dsprot (branch 1.27)", ["dsprot-1.27/include"], [],
                    ["src/*.c"], r"(src)/"),
    "zeldaret-st": ("CC0-1.0", "https://github.com/zeldaret/st",
                    ["zeldaret-st/libs/c/include", "zeldaret-st/libs/nitro/include", "zeldaret-st/libs/nns/include",
                     "zeldaret-st/libs/dsprot/include", "zeldaret-st/libs/runtime/include"], [],
                    ["libs/c/src/**/*.c", "libs/cpp/src/*.c", "libs/nns/src/**/*.c", "libs/dsprot/src/*.c"],
                    r"libs/([^/]+)/"),
    "zeldaret-ph": ("CC0-1.0", "https://github.com/zeldaret/ph", ["zeldaret-ph/libs/nds/include", "zeldaret-ph/include"],
                    [], ["libs/nds/src/*.c"], r"libs/([^/]+)/"),
    "pmd-sky": ("none", "https://github.com/pret/pmd-sky",
                ["pmd-sky/lib/include", "pmd-sky/lib/*/include", "pmd-sky/include", "pmd-sky/include/library"],
                ["SDK_ARM9", "SDK_FINALROM"],
                ["lib/NitroSDK/src/**/*.c", "lib/MSL_C/src/*.c", "lib/src/*.c"], r"NitroSDK/src/([^/.]+)|lib/([^/]+)/"),
    "khdays-decomp": ("CC0-1.0", "https://github.com/Yokimitsuro/khdays-decomp",
                      ["khdays-decomp/include", "khdays-decomp/include/nitro", "khdays-decomp/include/nnsys"], SDK,
                      ["libs/**/*.c"], r"libs/([^/]+/[^/]+)/"),
    "kh-recoded-decomp": ("MIT; eu/ CC0-1.0", "https://github.com/kh-recoded-decomp/kh-recoded-decomp",
                          ["kh-recoded-decomp/include", "kh-recoded-decomp/include/nitro",
                           "kh-recoded-decomp/include/nnsys", "kh-recoded-decomp/eu/include", "kh-recoded-decomp"], SDK,
                          ["eu/libs/**/*.c", "src/arm9/library_*/*.c"],
                          r"libs/([^/]+/[^/]+)/|library_([a-z]+_[a-z0-9]+)/"),
    "pictochat-decomp": ("MIT", "https://github.com/tangOS-decomps/pictochat-decomp",
                         ["pictochat-decomp/include"] + NTR, SDK, ["src/arm9/*.c", "src/arm9/*.cpp"], r"src/(arm9)/"),
}
# NitroDWC's prefix: the standard C its GameSpy code calls that zeldaret-st's MSL headers lack (a pointer it returns
# breaks the compile when implicitly declared), and ntrtwl's GameSpy asserts off: Black's DWC (overlay 9), a release
# build, has neither their calls nor their strings
GS_PREFIX = """#include <stddef.h>
#define FILENAME_MAX 256
#define EOF (-1)
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
char *strstr(const char *s, const char *t);
char *strcat(char *d, const char *s);
char *strncat(char *d, const char *s, size_t n);
int atoi(const char *s);
long atol(const char *s);
long strtol(const char *s, char **end, int base);
unsigned long strtoul(const char *s, char **end, int base);
int tolower(int c);
void qsort(void *base, size_t n, size_t size, int (*compare)(const void *, const void *));
int rand(void);
void srand(unsigned int seed);
void *malloc(size_t size);
void free(void *p);
void *realloc(void *p, size_t size);
void *calloc(size_t n, size_t size);
#define __msl_assertion_failed(condition, file, func, line) ((void)0)
"""
COMMON = ("-O4,p -proc arm946e -interworking -enum int -char signed -fp soft -lang=c99 -Cpp_exceptions off -gccext,on "
          "-gccinc -nolink -msgstyle gcc -w off")
IPA = "-ipa file -inline on,noauto"
VARIANTS = {
    "dsi1.1p1-arm": ("dsi/1.1p1", "-nothumb"),
    "dsi1.1p1-arm-ipa": ("dsi/1.1p1", f"-nothumb {IPA}"),
    "dsi1.1p1-thumb": ("dsi/1.1p1", "-thumb"),
    "dsi1.1p1-thumb-ipa": ("dsi/1.1p1", f"-thumb {IPA}"),
    "dsi1.2p1-arm": ("dsi/1.2p1", "-nothumb"),  # Black's MSL: its __ieee754_pow matches with dsi/1.2 to 1.3p1 only
    "2.0sp2p2-arm": ("2.0/sp2p2", "-nothumb"),
    "2.0sp2p2-thumb-ipa": ("2.0/sp2p2", f"-thumb {IPA}"),
    "1.2base-arm": ("1.2/base", "-nothumb"),
}
TSV = f"{ROOT}/build/lib_twins.tsv"
REF = os.path.expanduser("~/ref")
TWIN_C = 4000  # characters of a twin's C at most, as local_driver.py's DRAFT
# MIT and CC0, but one function per file with raw offsets or guessed names, not the SDK's: shown after the others
RAW = ("khdays-decomp", "kh-recoded-decomp", "pictochat-decomp")


def read_c(path):
    raw = open(path, "rb").read().replace(b"\r", b"")
    try:
        return raw.decode()
    except UnicodeDecodeError:  # NitroSDK's comments are Shift-JIS
        return raw.decode("cp932", errors="replace")


def find_c(checkout, rel, fn):
    """(fn's C, its file under checkout) from the file rel, or from a .c file it includes with #define aliases for
    its names (kh-recoded-decomp's eu/ files); (None, rel) if neither has it, or one defines it twice (under #if:
    which one was built is unclear)."""
    text = read_c(f"{checkout}/{rel}") if os.path.isfile(f"{checkout}/{rel}") else ""
    alias = dict(re.findall(r"^#define (\w+) (\w+)[ \t]*$", text, re.M))
    texts = [(rel, text)] + [(inc, re.sub(r"\w+", lambda m: alias.get(m[0], m[0]), read_c(f"{checkout}/{inc}")))
                             for inc in re.findall(r'^#include "([^"]+\.c)"', text, re.M)
                             if os.path.isfile(f"{checkout}/{inc}")]
    for where, t in texts:
        found = len(re.findall(rf"^(?![ \t#/])[^;{{}}\n]*\b{fn}\s*\([^;{{}}]*\)\s*\{{", t, re.M))
        if found:
            return (clones.definition(t, fn) if found == 1 else None), where
    return None, rel


def twin_c(mod, addr, dest=None, limit=TWIN_C):
    """The C of a Black function's library twin (TSV), for a worker's or the local model's context: the best source's
    function under a line naming it and its license, and a line naming the other sources; None without one whose C is
    found, or under 16 bytes (the same code by accident). Best: natural C whose license allows porting it, then
    natural C, then RAW's; among those a twin compiled as dest is (configure.py's compiler_of), then the first variant
    of VARIANTS."""
    if not os.path.exists(TSV):
        return None
    rows = [r for r in (l.rstrip("\n").split("\t") for l in open(TSV))
            if r[1] == mod and r[2] == f"{addr:#010x}" and int(r[3]) >= clones.TINY]
    tag = None
    if dest and rows:
        sys.path.insert(0, ROOT)
        from configure import compiler_of
        compiler, flags = compiler_of(dest)
        tag = f"{compiler.replace('/', '')}-{'thumb' if '-thumb' in flags else 'arm'}" + "-ipa" * ("-ipa file" in flags)
    order = list(VARIANTS)
    rows.sort(key=lambda r: (r[4] in RAW, SOURCES[r[4]][0] == "none", tag not in r[8].split(","),
                             min((order.index(t) for t in r[8].split(",") if t in order), default=len(order))))
    for r in rows:
        c, rel = find_c(f"{REF}/{r[4].split('@')[0]}", r[6], r[7])
        if not c:
            continue
        how = "read it, write natural C" if SOURCES[r[4]][0] == "none" or r[4] in RAW else \
            "may be ported as it is, with credit"
        built = "any variant" if len(r[8].split(",")) == len(VARIANTS) else r[8]
        out = (f"// {r[4]}'s {r[7]} in ~/ref/{r[4].split('@')[0]}/{rel}, the same code built as {built} "
               f"({SOURCES[r[4]][0]}{', not the SDK names' * (r[4] in RAW)}: {how})\n")
        others = list(dict.fromkeys(f"{o[4]} ({o[6]})" for o in rows if o[4].split("@")[0] != r[4].split("@")[0]))
        out += f"// also in {', '.join(others[:3])}{', ...' * (len(others) > 3)}\n" if others else ""
        return out + (c if len(c) <= limit else c[:limit] + "\n// ... cut short: the rest is in that file")
    return None


# Nintendo's reference sources, best first: a function of Black's with the same name as one of theirs (name_c)
NAMED = ("ntrtwl-NitroDWC", "ntrtwl-NitroSDK", "ntrtwl-NitroSystem", "ntrtwl-NitroWiFi", "ntrtwl-libvct", "libntr",
         "dsprot-1.27")
NAMED_LINES = 120  # lines of a name match's C at most (NitroWiFi's check_frag is 97)
DEF_HEAD = re.compile(r"^(?![ \t#/}])[^;{}\n(]*?\b([A-Za-z_]\w*)[ \t]*\([^;{}]*\)\s*\{", re.M)
NOT_FUNCTIONS = {"if", "for", "while", "switch", "return", "sizeof", "else", "do", "defined"}


def ref_names(ref=REF):
    """{function name: [(source, path under the checkout)]} of the C files of NAMED under ref (SOURCES' globs): a
    quarter of a second, so not cached."""
    out = defaultdict(list)
    for src in NAMED:
        root = f"{ref}/{src}"
        files = sorted({p for g in SOURCES[src][4] for p in glob.glob(f"{root}/{g}", recursive=True)})
        for p in files:
            try:
                text = read_c(p)
            except OSError:
                continue
            for n in dict.fromkeys(m[1] for m in DEF_HEAD.finditer(text)):
                if n not in NOT_FUNCTIONS:
                    out[n].append((src, os.path.relpath(p, root)))
    return out


# every reference tree under ~/ref but the tools' and pokebw2's copies (pokebw2 is the twin brief.py shows): the
# functions their C defines, by name (ref_index), for name_c
INDEX = f"{ROOT}/build/ref_index.json"
NOT_TREES = ("m2c", "decomp-permuter", "pokebw2")
INDEX_EXT = (".c", ".cpp")
_INDEX = {}


def ref_trees(ref=None):
    ref = ref or REF
    return sorted(d for d in os.listdir(ref) if os.path.isdir(f"{ref}/{d}") and d not in NOT_TREES
                  and not d.startswith("pokebw2-")) if os.path.isdir(ref) else []


def tree_stamp(path):
    """When a checkout last changed: its git index's (a checkout or pull rewrites it), else the directory's."""
    for p in (f"{path}/.git/index", f"{path}/.git/HEAD", path):
        if os.path.exists(p):
            return os.stat(p).st_mtime
    return 0


def tree_rank(tree):
    """The order name_c prefers trees in: Nintendo's sources (NAMED), then the other sources lib_twins compiles
    (MIT and CC0 first), the other trees, the raw ones (RAW) last."""
    if tree in NAMED:
        return (0, NAMED.index(tree))
    if tree in RAW:
        return (3, RAW.index(tree))
    if tree in SOURCES:
        return (1, SOURCES[tree][0] == "none")
    return (2, 0)


def ref_index(ref=None):
    """{function name: [(tree, path under it)]} of the C definitions (DEF_HEAD) in every tree of ref_trees, cached
    per tree in build/ref_index.json and read again when its checkout changed (all of them: two seconds)."""
    ref = ref or REF
    if ref in _INDEX:
        return _INDEX[ref]
    try:
        cache = json.load(open(INDEX))
    except (OSError, ValueError):
        cache = {}
    changed, out = False, defaultdict(list)
    for tree in ref_trees(ref):
        stamp = tree_stamp(f"{ref}/{tree}")
        entry = cache.get(tree)
        if not entry or entry.get("stamp") != stamp:
            names = {}
            for top, dirs, files in os.walk(f"{ref}/{tree}"):
                dirs[:] = sorted(d for d in dirs if not d.startswith("."))
                for f in sorted(files):
                    if not f.endswith(INDEX_EXT):
                        continue
                    p = os.path.join(top, f)
                    try:
                        text = read_c(p)
                    except OSError:
                        continue
                    rel = os.path.relpath(p, f"{ref}/{tree}")
                    for n in dict.fromkeys(m[1] for m in DEF_HEAD.finditer(text)):
                        if n not in NOT_FUNCTIONS:
                            names.setdefault(n, []).append(rel)
            entry = cache[tree] = {"stamp": stamp, "names": names}
            changed = True
        for n, rels in entry["names"].items():
            out[n] += [(tree, r) for r in rels]
    if changed:
        try:
            with open(INDEX + ".tmp", "w") as fh:
                json.dump({t: cache[t] for t in ref_trees(ref) if t in cache}, fh)
            os.replace(INDEX + ".tmp", INDEX)
        except OSError:
            pass
    _INDEX[ref] = out
    return out


def build_files(name):
    """The build/ file of this worktree, else of the main checkout's (build/prebuilt_map.tsv is only there)."""
    out = [f"{ROOT}/build/{name}"]
    try:
        common = subprocess.run(["git", "rev-parse", "--path-format=absolute", "--git-common-dir"], cwd=ROOT,
                                capture_output=True, text=True).stdout.strip()
        if common:
            out.append(f"{os.path.dirname(common)}/build/{name}")
    except OSError:
        pass
    return next((p for p in out if os.path.isfile(p)), None)


_ALIASES = {}


def aliases(mod, addr):
    """[(name, whose)] other names of Black's function at mod/addr that a reference may define it under: pokebw2's
    (build/twins_b_us.tsv) and the prebuilt library member's (build/prebuilt_map.tsv, tools/prebuilt_map.py)."""
    if not _ALIASES:
        _ALIASES[None] = True
        p = build_files("twins_b_us.tsv")
        for line in (open(p).read().splitlines()[1:] if p else []):
            r = line.split("\t")
            if len(r) > 5 and r[5]:
                _ALIASES.setdefault((r[1], int(r[2], 16)), []).append((r[5], "pokebw2's name for it"))
        p = build_files("prebuilt_map.tsv")
        for line in (open(p).read().splitlines()[1:] if p else []):
            r = line.split("\t")
            if len(r) > 8 and r[4]:
                _ALIASES.setdefault((r[5], int(r[6], 16)), []).append(
                    (r[4], f"its name in the prebuilt {os.path.basename(r[0])}'s {r[1]}"))
    out = []
    for n, how in _ALIASES.get((mod, addr), []):
        if n not in (o[0] for o in out):
            out.append((n, how))
    return out


def name_c(name, dest=None, lines=NAMED_LINES, mod=None, addr=None, extra=()):
    """The C of the function called name in a reference tree under ~/ref (ref_index), or called what pokebw2 or the
    prebuilt library names Black's function at mod/addr (aliases: NitroWiFi's network.c writes cps.c's
    func_ov009_0215a450 as check_frag), for a function whose code is no compiled twin of it (twin_c finds those): the
    first `lines` lines under a line naming the tree, the file and the name; None without one. extra: more
    [(name, whose)] (brief.py's pokebw2 version's). Best: Black's own name, then extra's, aliases', then the file
    named as dest's, then tree_rank's order."""
    names = [(name, None)] if name and not name.startswith(("func_", "data_")) else []
    for n, how in list(extra) + (aliases(mod, addr) if mod and addr is not None else []):
        if n not in (m for m, _ in names) and not n.startswith(("func_", "data_")):
            names.append((n, how))
    if not names:
        return None
    index = ref_index()
    base = os.path.splitext(os.path.basename(dest or ""))[0]
    cands = [(src, rel, n, how) for i, (n, how) in enumerate(names) for src, rel in index.get(n, [])]
    cands = sorted(cands, key=lambda c: ([n for n, _ in names].index(c[2]),
                                         os.path.splitext(os.path.basename(c[1]))[0] != base, tree_rank(c[0])))
    for src, rel, n, how in cands:
        c, where = find_c(f"{REF}/{src}", rel, n)
        if not c:
            continue
        body = c.splitlines()
        lic = SOURCES[src][0] if src in SOURCES else "no license given"
        how_c = "may be ported as it is, with credit" if lic.startswith(("MIT", "CC0")) and src not in RAW else \
            "read it, write natural C"
        others = [f"{s} ({p})" for s, p, m, _ in cands if (s, p) != (src, rel) and m == n]
        out = (f"// same name: {src}'s {n} in ~/ref/{src}/{where}, not a code twin of this one ({lic}: "
               f"{how_c})" + (f"; {n} is {how}" if how else "") + "\n")
        out += f"// also in {', '.join(others[:3])}{', ...' * (len(others) > 3)}\n" if others else ""
        return out + "\n".join(body[:lines]) + (f"\n// ... {len(body) - lines} more lines in that file"
                                                if len(body) > lines else "")
    return None


def jobs(ref, work, names):
    """[(source, library, path under the checkout, variant, object, command)]."""
    # NitroSDK's build generates fx_const.h, which the headers of ntrtwl, libntr and pmd-sky include; GameSpy's code
    # includes MSL's extras.h and assert.h, which no checkout has (extras.h only time_t: these compiles allow implicit
    # declarations; assert off, as in a release library)
    gen = f"{work}/gen/nitro/fx/fx_const.h"
    if not os.path.exists(gen) and os.path.exists(f"{ref}/ntrtwl-NitroSDK/gen/nitro/fx/gen_fx_const.py"):
        os.makedirs(os.path.dirname(gen), exist_ok=True)
        subprocess.run([sys.executable, "-I", f"{ref}/ntrtwl-NitroSDK/gen/nitro/fx/gen_fx_const.py",
                        f"{ref}/ntrtwl-NitroSDK/gen/nitro/fx/fx_const.csv", gen], check=True)
        open(f"{work}/gen/extras.h", "w").write("typedef long time_t;\n")
        open(f"{work}/gen/assert.h", "w").write("#define assert(e) ((void)0)\n")
    prefix = f"{work}/gen/gs_prefix.h"
    if not os.path.exists(prefix) or open(prefix).read() != GS_PREFIX:
        os.makedirs(f"{work}/gen", exist_ok=True)
        open(prefix, "w").write(GS_PREFIX)
    out = []
    for name in names:
        _, _, includes, defines, globs, lib_re = SOURCES[name]
        args = ["-prefix", "gs_prefix.h"] if name == "ntrtwl-NitroDWC" else []  # found in gen/, an include dir
        for inc in includes + [os.path.relpath(f"{work}/gen", ref)] + STD:
            flag = "-ir" if inc.endswith("/**") else "-i"
            for d in sorted(glob.glob(f"{ref}/{inc.removesuffix('/**')}")):
                args += [flag, d]
        checkout = f"{ref}/{name.split('@')[0]}/"
        files = sorted({p for g in globs for p in glob.glob(checkout + g, recursive=True)})
        for path in files:
            rel = path[len(checkout):]
            if re.search(r"arm7|/sim7?/|/demo/", rel, re.I):
                continue
            m = re.search(lib_re, rel)
            lib = next((g for g in m.groups() if g), "-") if m else "-"
            for tag, (compiler, flags) in VARIANTS.items():
                obj = f"{work}/obj/{name}/{tag}/{os.path.splitext(rel)[0]}.o"
                code = "SDK_CODE_THUMB" if "-thumb" in flags else "SDK_CODE_ARM"
                cmd = [f"{ROOT}/tools/wibo", f"{ROOT}/tools/mwccarm/{compiler}/mwccarm.exe", *COMMON.split(),
                       *flags.split(), *(a for d in defines + [code] for a in ("-d", d)), *args, "-o", obj, path]
                out.append((name, lib, rel, tag, obj, cmd))
    return out


def compile_one(job):
    obj, cmd = job[4], job[5]
    if os.path.exists(obj) or os.path.exists(obj + ".failed"):
        return job, not os.path.exists(obj)
    os.makedirs(os.path.dirname(obj), exist_ok=True)
    r = subprocess.run(cmd, env=dict(os.environ, MWCIncludes="."), capture_output=True, text=True)
    if r.returncode or not os.path.exists(obj):
        with open(obj + ".failed", "w") as f:  # so that a rerun doesn't try again; delete --work to retry
            f.write((r.stdout + r.stderr)[-2000:])
        return job, True
    return job, False


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--ref", default=os.path.expanduser("~/ref"), help="the sources' checkouts, read only")
    ap.add_argument("--sources", help="comma separated; default: every one checked out under --ref")
    ap.add_argument("--work", default=f"{ROOT}/build/lib_twins")
    ap.add_argument("--twins", default=f"{ROOT}/build/twins_b_us.tsv", help="pokebw2's twins, from tools/twins.py")
    ap.add_argument("--tsv", default=TSV)
    ap.add_argument("-j", type=int, default=0, help="compiles at once (default: BW_JOBS, else the cores free in the worker slice)")
    args = ap.parse_args()
    names = args.sources.split(",") if args.sources else \
        [n for n in SOURCES if os.path.isdir(f"{args.ref}/{n.split('@')[0]}")]

    todo = jobs(args.ref, args.work, names)
    import jobs as bwjobs
    with ThreadPoolExecutor(args.j or bwjobs.jobs()) as pool:
        results = list(pool.map(compile_one, todo))
    index = defaultdict(list)  # digest -> [(source, library, path, function, variant)]
    built = defaultdict(set)  # source -> paths that compiled with some variant
    files = defaultdict(set)
    for (name, lib, rel, tag, obj, _), failed in results:
        files[name].add(rel)
        if not failed:
            built[name].add(rel)
            for fn, size, h, _ in clones.functions(obj):
                index[h].append((name, lib, rel, fn, tag))

    every = clones.black()
    black = {(r[0], r[1]): r[:6] for r in every}  # without the relocations, to be hashable
    twinned = {(l.split("\t")[1], l.split("\t")[0]) for l in list(open(args.twins))[1:]} \
        if os.path.exists(args.twins) else set()
    hits = defaultdict(dict)  # source -> {Black row: [(library, path, function, variant)]}
    for r in every:
        for name, lib, rel, fn, tag in index.get(r[6], []):
            hits[name].setdefault(black[r[0], r[1]], []).append((lib, rel, fn, tag))

    def stats(rows):
        big = [r for r in rows if r[3] >= clones.TINY]
        new = [r for r in big if (r[0], r[1]) not in twinned and not r[5]]
        return (f"{len(rows)} ({sum(r[3] for r in rows)} B), {len(big)} >= 16 B ({sum(r[3] for r in big)} B), "
                f"{len(new)} of those new ({sum(r[3] for r in new)} B)")

    print("source: files compiled; Black's functions with a twin: all, >= 16 bytes, of those neither pokebw2's nor "
          "matched here ('new'); best variants")
    union, free = set(), set()
    for name in names:
        rows = list(hits[name])
        union |= set(rows)
        if SOURCES[name][0] != "none":
            free |= set(rows)
        tags = Counter(t for r in rows if r[3] >= clones.TINY for t in {h[3] for h in hits[name][r]})
        commit = subprocess.run(["git", "-C", f"{args.ref}/{name.split('@')[0]}", "rev-parse", "--short=12", "HEAD"],
                                capture_output=True, text=True).stdout.strip()
        print(f"{name} ({commit}) [{SOURCES[name][0]}]: {len(built[name])}/{len(files[name])} files; {stats(rows)}; "
              + ", ".join(f"{t} {n}" for t, n in tags.most_common(3)))
        libs = defaultdict(list)
        for r in rows:
            libs[hits[name][r][0][0]].append(r)
        for lib, lrows in sorted(libs.items(), key=lambda kv: -sum(r[3] for r in kv[1])):
            print(f"    {lib}: {stats(lrows)}")
    print(f"all sources: {stats(union)}")
    print(f"sources with a license (MIT, CC0): {stats(free)}")

    with open(args.tsv, "w") as f:
        f.write("black_symbol\tmodule\taddress\tsize\tsource\tlibrary\tfile\tfunction\tvariants\tpokebw2_twin\tmatched\n")
        for name in names:
            for r, hs in sorted(hits[name].items(), key=lambda kv: (kv[0][0], kv[0][2] or 0)):
                lib, rel, fn, _ = hs[0]
                tags = ",".join(sorted({h[3] for h in hs if (h[1], h[2]) == (rel, fn)}))
                f.write(f"{r[1]}\t{r[0]}\t{r[2] or 0:#010x}\t{r[3]}\t{name}\t{lib}\t{rel}\t{fn}\t{tags}\t"
                        f"{'yes' if (r[0], r[1]) in twinned else 'no'}\t{'yes' if r[5] else 'no'}\n")
    print(f"written to {args.tsv}")


if __name__ == "__main__":
    sys.exit(main())
