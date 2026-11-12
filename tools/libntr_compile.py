#!/usr/bin/env python3
"""Compiles the game's C against libntr's headers (docs/libntr/compile.md): clang -fsyntax-only for x86_64-linux of
every C file under src/, lib/gfl and lib/poke_net, with libntr's include dirs and its linux defines (TwlSDK 5, a TWL
hybrid, as Black is) in place of lib/nitro's and lib/twl's, and libntrsystem's in place of lib/nnsys's. No build:
libntr's one generated header, nitro/fx/fx_const.h, is written from its CSV into a temp dir. The C is not changed.

    python3 tools/libntr_compile.py --libntr DIR --libntrsystem DIR      # each stage's count of files that compile
    python3 tools/libntr_compile.py ... --stage pch --causes             # that stage's errors by cause
    python3 tools/libntr_compile.py ... --stage pch FILE...              # the files' errors
    python3 tools/libntr_compile.py ... --sdk 4 | --our-nnsys            # libntr's default NitroSDK 4 (NTR only), or
                                                                         # our lib/nnsys in place of libntrsystem

Stages, each the one before plus a step (clang -w, as libntr builds for linux: errors only, clang's default ones too):
  ours  the control: the build's own include dirs (lib/nitro, lib/twl, lib/nnsys, MSL), the same target and flags
  raw   libntr's include dirs and defines in place of lib/nitro's, lib/twl's and lib/nnsys's
  msl   lib/msl/include after the host's headers (-idirafter): libntr's nitro/os/common/thread.h includes SDL2,
        which includes the host's libc, and MSL's stdarg.h, stdio.h... in its place break it
  shim  pokebw2's SDK header names libntr lacks (nitro/fx.h, nitro/hw.h, twl/ssp.h...), each a header that
        includes <nitro.h> (<nnsys.h> for NitroSystem's)
  pch   -include nitro.h -include nnsys.h, as libntr's include/pch/nitro_pch.h has its users do
A cause (--causes) is that of an error the control lacks (same file, line and message): libntr's, not the target's.
"""
import argparse, collections, json, os, re, subprocess, sys, tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(ROOT / "tools"))
from configure import INCLUDE_DIRS  # noqa: E402
import jobs  # noqa: E402

STAGES = ["ours", "raw", "msl", "shim", "pch"]
SDK = "5"
# libntr's meson.build for build_target=linux, sdk_version_major=5, hardware_type=twl_hyb
DEFS = ("SDK_CW_FORCE_EXPORT_SUPPORT SDK_TS SDK_4M SDK_FINALROM NNS_FINALROM SDK_VERSION_MAJOR=5UL SDK_TWL SDK_TWLHYB "
        "SDK_PORT SDK_X86 SDK_CODE_X86 SDK_BUILD_LINUX SDK_64BIT _NITRO").split()
FLAGS = ["--target=x86_64-linux-gnu", "-std=gnu99", "-fsyntax-only", "-w", "-ferror-limit=0", "-fsigned-char",
         "-fno-color-diagnostics", "-fno-caret-diagnostics"]
DIAG = re.compile(r"^(.+?):(\d+):\d+: (error|fatal error): (.*)$", re.M)
CAUSES = {
    "proto": "a game header or file declares an SDK function with another prototype than libntr's",
    "name": "an SDK macro, constant, type or function our headers have and libntr's lack or spell otherwise",
    "regaddr": "an SDK internal (G2x_*_) called with a REG_*_ADDR integer: libntr's X86 registers are variables",
    "field": "an SDK struct field our headers name and libntr's name otherwise",
    "enum-int": "a prototype returning int where libntr returns an enum (MWCC's -enum int makes them one type)",
    "incl": "a header that relied on our SDK headers' own includes (struct_decls.h, gfl/heap.h)",
    "twl": "a TWL part libntr lacks (MIC's limited sampling, SSP JPEG, camera internals, parental control)",
    "clash": "the file defines a function or type libntr's headers already define (static, inline, typedef)",
    "internal": "an SDK internal libntr has in its sources only, not its headers",
    "other": "other",
}


def c_files():
    return sorted(p.relative_to(ROOT).as_posix() for d in ("src", "lib/gfl", "lib/poke_net")
                  for p in (ROOT / d).rglob("*.c"))


def setup(libntr, nns, tmp):
    """The temp dir's gen/ (fx_const.h from libntr's CSV) and shim/ (the header names libntr lacks)."""
    gen = tmp / "gen/nitro/fx"
    gen.mkdir(parents=True)
    fx = libntr / "gen/nitro/fx"
    subprocess.run([sys.executable, fx / "gen_fx_const.py", fx / "fx_const.csv", gen / "fx_const.h"], check=True)
    for lib, inc, umbrella in (("nitro", "lib/nitro/include", "nitro.h"), ("twl", "lib/twl/include", "nitro.h"),
                               ("nnsys", "lib/nnsys/include", "nnsys.h")):
        have = libntr / "include" if lib != "nnsys" else nns
        for h in (ROOT / inc).rglob("*.h"):
            rel = h.relative_to(ROOT / inc)
            if not (have / rel).exists() and not (tmp / "gen" / rel).exists():  # fx_const.h is generated
                (tmp / "shim" / rel).parent.mkdir(parents=True, exist_ok=True)
                (tmp / "shim" / rel).write_text(f"// a pokebw2 header name libntr lacks\n#include <{umbrella}>\n")


def command(rel, stage, libntr, nns, tmp):
    defs = DEFS if SDK == "5" else [
        "SDK_VERSION_MAJOR=4UL" if d.startswith("SDK_VERSION_MAJOR") else d for d in DEFS if "TWL" not in d]
    if stage == "ours":
        return ["clang", *FLAGS, *(f"-I{d}" for d in INCLUDE_DIRS), f"-I{Path(rel).parent.as_posix()}",
                "-Ibuild/include/generated", rel]
    lv = STAGES.index(stage)
    inc = [f"-I{tmp}/shim"] if lv >= 3 else []
    for d in INCLUDE_DIRS:
        if d == "lib/nitro/include":
            inc += [f"-I{libntr}/include", f"-I{tmp}/gen"]
        elif d == "lib/nnsys/include":
            inc.append(f"-I{nns}")
        elif d != "lib/twl/include" and not (d == "lib/msl/include" and lv >= 2):
            inc.append(f"-I{d}")
    pre = ["-include", "nitro.h", *(["-include", "nnsys.h"] if (nns / "nnsys.h").exists() else [])] if lv >= 4 else []
    return ["clang", *FLAGS, *(f"-D{d}" for d in defs), *pre, *inc, f"-I{Path(rel).parent.as_posix()}",
            "-Ibuild/include/generated", *(["-idirafter", "lib/msl/include"] if lv >= 2 else []), rel]


def compile_one(job):
    rel, stage, libntr, nns, tmp = job
    r = subprocess.run(["nice", "-n", "19", *command(rel, stage, libntr, nns, tmp)], capture_output=True, text=True,
                       cwd=ROOT, errors="replace")
    errs = [(os.path.normpath(m[1]), int(m[2]), m[4]) for m in DIAG.finditer(r.stderr)]
    return errs or ([("?", 0, r.stderr.strip()[-300:])] if r.returncode else [])


def run(files, stage, libntr, nns, tmp):
    return dict(zip(files, jobs.fork_map(compile_one, [(f, stage, libntr, nns, tmp) for f in files])))


def tokens(*dirs, pats=("*.h",)):
    out = set()
    for d in dirs:
        for pat in pats:
            for p in Path(d).rglob(pat):
                out |= set(re.findall(r"\b[A-Za-z_]\w*\b", p.read_text(errors="replace")))
    return out


FOLLOW_ON = re.compile(r"(initializer element is not|expected expression|statement requires expression|"
                       r"initializing '[^']*' with an expression of incompatible)")
UNDECLARED = re.compile(r"(call to undeclared|use of undeclared|unknown type|must use)")


def causes(res, ctl, libntr, nns, tmp):
    """{file: {cause: [(path, line, msg)]}} of the errors the control lacks."""
    ntr, twl = tokens(ROOT / "lib/nitro/include"), tokens(ROOT / "lib/twl/include")
    ours = ntr | twl | tokens(ROOT / "lib/nnsys/include")
    theirs = tokens(libntr / "include", tmp / "gen", nns)
    src = tokens(libntr / "libraries", nns.parent / "libraries", pats=("*.c", "*.h"))
    game = tokens(ROOT / "include")
    out = {}
    for f, errs in res.items():
        seen = {(e[0], e[1], e[2]) for e in ctl.get(f, [])}
        by, at, last = collections.defaultdict(list), {}, "other"
        for path, line, msg in errs:
            if (path, line, msg) in seen:
                continue
            msg = re.sub(r"; ISO C99.*| \[-W[^]]*\]$", "", msg)
            ids = re.findall(r"'([^']*)'", msg)
            x = ids[0] if ids else ""
            undecl = UNDECLARED.match(msg)
            if (path, line) in at and not undecl:
                k = at[(path, line)]
            elif FOLLOW_ON.match(msg) or x == "regs":
                k = last  # a follow-on of the error before it
            elif "passing 'REGType" in msg:
                k = "regaddr"
            elif msg.startswith("no member named"):
                k = "twl" if ids[1].startswith(("struct OSTWL", "OSTWL")) else "field"
            elif msg.startswith("conflicting types"):
                k = ("enum-int" if path == "include/system/rtc.h" else
                     "twl" if x.startswith(("MI_Camera", "MI_FreeWram")) else "proto")
            elif msg.startswith(("static declaration", "redefinition of", "typedef redefinition")):
                k = "clash"
            elif undecl:
                if x == "struct" or x in game and x not in ours:
                    k = "incl"
                elif x in twl and x not in ntr or x.startswith(("SSP_", "CAMERA_", "MIC_StartLimited",
                                                                   "MIC_StopLimited")):
                    k = "twl"
                elif x in src and x not in theirs:
                    k = "internal"
                else:
                    k = "name" if x in ours else "other"
            else:
                k = "other"
            at.setdefault((path, line), k)
            last = k
            by[k].append((path, line, msg))
        out[f] = dict(by)
    return out


def key(msg):
    """What an error names: the member of `no member named`, else its first quoted name, else its start."""
    ids = re.findall(r"'([^']*)'", msg)
    return ids[1] + "." + ids[0] if msg.startswith("no member") else ids[0] if ids else msg[:40]


def report(res, ctl, cs, examples=3):
    ok_ctl = {f for f, v in ctl.items() if not v}
    print(f"{sum(1 for v in res.values() if not v)} of {len(res)} files compile; {len(ok_ctl)} with our headers; "
          f"{sum(1 for v in cs.values() if v)} have an error our headers don't")
    files = collections.defaultdict(set)
    for f, by in cs.items():
        for k in by:
            files[k].add(f)
    for k, fs in sorted(files.items(), key=lambda kv: -len(kv[1])):
        only = sum(1 for f in fs if set(cs[f]) == {k} and f in ok_ctl)
        places = {(p, ln) for f in fs for p, ln, _ in cs[f][k]}
        names = collections.Counter(n for f in fs for n in {key(m) for _, _, m in cs[f][k]})
        print(f"\n{k}: {len(fs)} files ({only} compile once it alone is fixed), {len(places)} places: {CAUSES[k]}")
        print("   ", ", ".join(f"{n} ({c})" for n, c in names.most_common(10)))
        shown = set()
        for f in sorted(fs, key=lambda f: -len(cs[f][k])):
            for p, ln, m in cs[f][k]:
                if (p, ln) not in shown and len(shown) < examples:
                    shown.add((p, ln))
                    print(f"    {p}:{ln}: {m[:150]}")
    fixed, left, steps = set(), set(files), []
    while left:
        best = max(sorted(left), key=lambda k: sum(1 for f in ok_ctl if set(cs[f]) <= fixed | {k}))
        fixed.add(best)
        left.discard(best)
        steps.append(f"{best} {sum(1 for f in ok_ctl if set(cs[f]) <= fixed)}")
    print("\nfiles that compile as the causes are fixed in turn:", ", ".join(steps))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--libntr", type=Path, default=Path.home() / "ref/libntr")
    ap.add_argument("--libntrsystem", type=Path, default=Path.home() / "ref/libntrsystem",
                    help="the clone's root (its include/ is used)")
    ap.add_argument("--sdk", choices=["4", "5"], default="5", help="libntr's sdk_version_major (5: TWL hybrid)")
    ap.add_argument("--our-nnsys", action="store_true", help="lib/nnsys/include in place of libntrsystem's")
    ap.add_argument("--stage", choices=STAGES)
    ap.add_argument("--causes", action="store_true")
    ap.add_argument("--examples", type=int, default=3)
    ap.add_argument("--json", type=Path, help="write {stage: {file: errors}} there")
    ap.add_argument("files", nargs="*")
    a = ap.parse_args()
    global SDK
    SDK = a.sdk  # the forked processes of jobs.fork_map see it
    libntr = a.libntr.resolve()
    nns = ROOT / "lib/nnsys/include" if a.our_nnsys else (a.libntrsystem / "include").resolve()
    files = a.files or c_files()
    with tempfile.TemporaryDirectory() as t:
        tmp = Path(t)
        setup(libntr, nns, tmp)
        stages = [a.stage] if a.stage else STAGES
        if a.causes and "ours" not in stages:
            stages = ["ours", *stages]
        res = {s: run(files, s, libntr, nns, tmp) for s in stages}
        if a.json:
            a.json.write_text(json.dumps(res))
        if a.files and a.stage:
            for f in files:
                for e in res[a.stage][f]:
                    print(f"{e[0]}:{e[1]}: {e[2]}")
        elif a.causes:
            s = a.stage or "pch"
            report(res[s], res["ours"], causes(res[s], res["ours"], libntr, nns, tmp), a.examples)
        else:
            for s in stages:
                print(f"{s:5} {sum(1 for v in res[s].values() if not v):5} of {len(files)} compile")


if __name__ == "__main__":
    main()
