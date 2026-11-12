#!/usr/bin/env python3
"""Which compiler and flags make a library file's functions match: a per-file search over lib/, for library.toml.

Nintendo built its libraries apart from the game, and a file of one may have been built with other flags than its
library's (o294: nnsys's g2d_Renderer.c with -ipa file, which pools the SDK's file statics under one base where a fake
struct of them never gave the loads); a game file under src/ shares the game's. For each incomplete file of lib/
(docs/nonmatching.md has rows of it, or the files given) this compiles it under:
  - each compiler of tools/mwccarm (dsi/*, 2.0/*, 1.2/*) with the file's flags;
  - the file's compiler with each -ipa (file, function), each -inline (on,noauto, auto, off) and their combinations,
    and each other -O level;
  - those combinations again under the three compilers that did best;
  - pokebw2's flags for the file (its library.toml, without the Thumb flag), if it has an entry;
and judges every function of the file (tools/brief.py's judge: 0 matches, else the lines of diff) against the file as it
is. An option is kept when no matching function stops matching and the file gains: more functions match, or the
unmatched are closer in total. Prints the library.toml lines for it; changes nothing.

    BW_JOBS=4 python3 tools/flag_search.py                          # every incomplete lib/ file: build/flag_search.tsv
    python3 tools/flag_search.py lib/nnsys/src/g2d/g2d_Renderer.c   # one file, with the options that lose or don't help
    python3 tools/flag_search.py --lib dwc --lib nitro              # the incomplete files of some libraries
    python3 tools/flag_search.py --triggers                         # files whose flags alone may not be enough

A compiler of its own goes in library.toml as [file_compiler] "g2d/g2d_Renderer.c" = "2.0/sp2p3" (configure.py's
compiler_of reads it). A library-wide option is changed only after tools/judge_all.py <regex of its headers> --save
before and after shows no loss: this judges files one at a time.

--all also searches the complete files, so that a library-wide option shows what it loses (the summary at the end lists
the compilers and flags that gain somewhere and lose nowhere in the library).

--triggers lists the functions with the mark of file statics that -ipa file reached from one base (docs/lessons.md,
data_02147b88): a function of an incomplete file whose literal pool differs from the original's, where a pool word of
the original is used as a base with offsets that land in other objects of Black's .data/.bss (symbols.txt). The flags
alone don't give those loads: the file has to define the statics, which needs its .data/.bss in delinks.txt (the list
says whether it has them). Written to build/flag_triggers.tsv; a heuristic that lists a big struct's fields too.
"""
import argparse
import concurrent.futures
import itertools
import json
import multiprocessing
import os
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import brief as b  # noqa: E402
import jobs as bwjobs  # noqa: E402
import compiler_probe as cp  # noqa: E402
import local_driver as ld  # noqa: E402
from configure import compiler_of  # noqa: E402

ROOT = ld.ROOT
IPA = ["", "-ipa file", "-ipa function"]
INLINE = ["", "-inline on,noauto", "-inline auto", "-inline off"]
LEVELS = ["-O4,s", "-O3,p", "-O3,s", "-O2,p"]
GRID = [" ".join(x for x in c if x) for c in itertools.product(IPA, INLINE)][1:]  # the 11 combinations
THUMB = {"-thumb", "-nothumb"}
POKEBW2 = Path.home() / "ref" / "pokebw2"
MISSING = 1000  # lines a function that isn't emitted counts for in a total


def compilers():
    root = cp.TOOLS / "mwccarm"
    return sorted(f"{d.parent.name}/{d.name}" for d in root.glob("*/*") if (d / "mwccarm.exe").exists())


def incomplete():
    """The lib/ C files docs/nonmatching.md has rows of."""
    out = set()
    for line in (ROOT / "docs" / "nonmatching.md").read_text().splitlines():
        for cell in line.split("|"):
            f = cell.strip().strip("`")
            if f.startswith("lib/") and f.endswith(".c") and (ROOT / f).is_file():
                out.add(f)
    return sorted(out)


def scores(path, names, compiler, flags):
    """{function: lines of diff, 0 for a match, None for one not emitted} of the file under a compiler and extra
    flags, or None if it doesn't compile."""
    try:
        results, lines, failed = cp.probe_source(path, [compiler], flags, cwd=ROOT)
    except Exception:  # noqa: BLE001 - a compiler that cannot take the file
        return None
    if failed:
        return None
    diffs, cur = {}, None
    for l in lines:
        m = re.match(r"--- (\w+) \(", l)
        if m:
            cur = m[1]
            diffs[cur] = 0
        elif cur:
            diffs[cur] += 1
    out = {}
    for n in names:
        st = next(iter(results.get(n, {}).values()), None)
        out[n] = None if st in (None, "not emitted", "unknown") else 0 if st == "MATCH" else max(1, diffs.get(n, 0))
    return out


def total(sc):
    return sum(MISSING if v is None else v for v in sc.values())


def matched(sc):
    return {n for n, v in sc.items() if v == 0}


def verdict(base, sc):
    """(matches gained, matching functions lost, total lines gained: negative is closer) of a result against the file's."""
    return len(matched(sc) - matched(base)), sorted(matched(base) - matched(sc)), total(sc) - total(base)


def pokebw2_flags(dest):
    """pokebw2's compiler and flags for the same file of its library, without the Thumb flag, or None."""
    import tomllib
    parts = Path(dest).parts
    toml = POKEBW2 / "lib" / parts[1] / "library.toml"
    if not toml.exists() or not (POKEBW2 / dest).exists():
        return None
    lib = tomllib.loads(toml.read_text())
    over = [f for f in lib.get("file_flags", {}).get("/".join(parts[3:]), []) if f not in THUMB]
    return lib["compiler"], " ".join(f for f in over if f.split()[0] in ("-ipa", "-inline", "-O4,p", "-O2,p"))


def search(dest, every=False):
    """{file, own compiler, base: {function: score}, rows: [(compiler, flags, score map)]} of one file, every variant."""
    print(f"start {dest}", file=sys.stderr, flush=True)
    path, _, _, funcs = b.locate(dest)
    names = [n for _, _, n, _ in funcs]
    own, _ = compiler_of(Path(dest))
    base = scores(path, names, own, "")
    if base is None:
        return {"file": dest, "error": "does not compile"}
    seen = {(own, "")}
    rows = []

    def run(c, f):
        if (c, f) in seen:
            return
        seen.add((c, f))
        sc = scores(path, names, c, f)
        if sc is not None:
            rows.append((c, f, sc))

    if matched(base) == set(names) and not every:
        return {"file": dest, "own": own, "base": base, "rows": [], "complete": True}
    for c in compilers():
        run(c, "")
    for f in GRID + LEVELS:
        run(own, f)
    p = pokebw2_flags(dest)
    if p:
        run(p[0], p[1])
    best = sorted((r for r in rows if r[0] != own), key=lambda r: (-len(matched(r[2])), total(r[2])))
    top = []
    for r in best:
        if r[0] not in top and len(top) < 3:
            top.append(r[0])
    for c in top:
        for f in GRID:
            run(c, f)
    return {"file": dest, "own": own, "base": base, "rows": rows}


def rank(res):
    """The variants that keep every match and gain, best first: [(gained, closer lines, compiler, flags, score map)]."""
    base = res["base"]
    out = []
    for c, f, sc in res["rows"]:
        gained, lost, lines = verdict(base, sc)
        if not lost and (gained or lines < 0):
            out.append((gained, lines, c, f, sc))
    own = res["own"]
    out.sort(key=lambda r: (-r[0], r[1], r[2] != own, len(r[3])))
    return out


def splits(res):
    """[(functions gained, functions lost, compiler, flags)] best first: the variants under which some functions match
    that don't in the file, but others of it stop matching, so that no rank() keeps them. The file can be split: the
    gained functions go to a file of their own built with the variant, the rest stay (s347: `exit` of
    lib/msl/src/abort_exit_arm_eabi.c matches under another compiler, abort and the others don't)."""
    base = res["base"]
    out = []
    for c, f, sc in res.get("rows", ()):
        gained = sorted(matched(sc) - matched(base))
        lost = sorted(matched(base) - matched(sc))
        if gained and lost and len(base) > 1:
            out.append((gained, lost, c, f))
    out.sort(key=lambda r: (-len(r[0]), len(r[1]), r[2] != res["own"], len(r[3])))
    seen, best = set(), []
    for r in out:  # a variant that gains only what a better one gained is left out
        if not set(r[0]) <= seen:
            best.append(r)
            seen |= set(r[0])
    return best


def toml_lines(dest, own, c, f):
    rel = "/".join(Path(dest).parts[3:])
    out = []
    flags = [x.strip() for x in re.split(r"(?= -)", " " + f) if x.strip()] if f else []
    if flags:
        out.append("[file_flags] " + json.dumps(rel) + " += " + json.dumps(flags))
    if c != own:
        out.append("[file_compiler] " + json.dumps(rel) + " = " + json.dumps(c))
    return out


def report(res, verbose):
    dest = res["file"]
    if "error" in res:
        return [dest + ": " + res["error"]], None
    base = res["base"]
    n, m = len(matched(base)), len(base)
    if res.get("complete"):
        return [f"{dest}: all {m} match"], None
    kept = rank(res)
    lines = [f"{dest}: {n}/{m} match, {total(base)} lines; {len(res['rows'])} variants, {len(kept)} gain"]
    for gained, closer, c, f, sc in kept[: (5 if verbose else 1)]:
        lines.append(f"    +{gained} match, {closer:+d} lines: {c} {f}".rstrip() + "  -> " + "; ".join(
            toml_lines(dest, res["own"], c, f)))
    for gained, lost, c, f in splits(res)[:3]:
        lines.append(f"    split the file: {', '.join(gained[:4])}{' and more' if len(gained) > 4 else ''} match"
                     f"{'es' if len(gained) == 1 else ''} under {c} {f}".rstrip() + f", which loses {len(lost)} "
                     f"({', '.join(lost[:3])}): move {'it' if len(gained) == 1 else 'them'} to a file of their own "
                     "(tools/add_source_file.py, its .text range) built with " + ("; ".join(toml_lines(dest, res["own"], c, f)) or "that compiler"))
    if verbose:
        for c, f, sc in res["rows"]:
            g, lost, lines_ = verdict(base, sc)
            if lost:
                lines.append(f"    loses {len(lost)} ({', '.join(lost[:3])}), +{g}: {c} {f}".rstrip())
    return lines, kept[0] if kept else None


def summary(results):
    """Per library, the compiler and flags tried in all its files that no file loses by and some gain by: candidates
    for library.toml's own, once tools/judge_all.py over the library (complete files too) shows no loss."""
    libs = {}
    for res in results:
        if res.get("rows"):
            libs.setdefault(res["file"].split("/")[1], []).append(res)
    for lib, items in sorted(libs.items()):
        keys = set.intersection(*({(c, f) for c, f, _ in r["rows"]} for r in items))
        for c, f in sorted(keys):
            gain = loss = 0
            for r in items:
                sc = next(s for cc, ff, s in r["rows"] if (cc, ff) == (c, f))
                g, lost, lines = verdict(r["base"], sc)
                loss += bool(lost)
                gain += bool(not lost and (g or lines < 0))
            if gain and not loss:
                print(f"library-wide {lib}: {c} {f} gains in {gain} of {len(items)} incomplete files, loses in none")


def placed(names):
    """Every lib/ C file of the libraries that has a .text in a delinks.txt."""
    from file_ranges import read_delinks
    out = set()
    for _, files in read_delinks(str(ld.CONFIG)).values():
        out |= {f for f, secs in files.items() if ".text" in secs and f.startswith("lib/") and f.endswith(".c")
                and (ROOT / f).is_file()}
    return sorted(f for f in out if f.split("/")[1] in names)


def libraries(names, every=False):
    """The incomplete files of the libraries, DS Protect's too (its functions are encrypted in the ROM and reading their
    decrypted bytes out of the delinked object is slow, but dsprot_cipher.c matched only under 2.0/sp2p2: wave 17)."""
    wanted = set(names) or set(os.listdir(ROOT / "lib"))
    return placed(wanted) if every else [f for f in incomplete() if f.split("/")[1] in wanted]


# --- the trigger: a base literal with offsets in other objects

def data_symbols(directory):
    """Sorted [(address, name)] of the .data/.bss symbols of a module and of ARM9 main."""
    out = []
    for d in {directory, str(ROOT / "config" / cp.VERSION / "arm9")}:
        text = (Path(d) / "symbols.txt").read_text()
        out += [(int(m[2], 16), m[1]) for m in re.finditer(r"^(\S+) kind:(?:data|bss)\S* addr:(0x[0-9a-f]+)", text, re.M)]
    return sorted(set(out))


def sections_of(dest):
    """{section: (start, end)} of the file in delinks.txt (the module that places its .text)."""
    from file_ranges import read_delinks
    for _, (_, files) in read_delinks(str(ld.CONFIG)).items():
        if dest in files:
            return files[dest]
    return {}


NUM = r"-?(?:0x[0-9a-f]+|\d+)"
LOAD = re.compile(r"(\w+), \[pc, #(" + NUM + r")\]$")
MEM = re.compile(r"(\w+), \[(\w+)(?:, #(" + NUM + r"))?\]!?$")
ADD = re.compile(r"(\w+), (\w+), #(" + NUM + r")$")


def trigger(name, syms):
    """[(literal address, its symbol, [(offset, symbol at literal+offset)])] of the pool words that the original
    function `name` uses as a base with offsets that land in other symbols than the literal's."""
    import bisect
    import capstone
    found = cp.find_function(name, cp.load_modules())
    if not found:
        return []
    data, addr, thumb = found
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if thumb else capstone.CS_MODE_ARM)
    addrs = [a for a, _ in syms]

    def at(a):
        i = bisect.bisect_right(addrs, a) - 1
        return syms[i][1] if i >= 0 else None

    regs = {}  # register -> (literal, delta)
    uses = {}
    d0 = next((o for o, k in cp.pool_marks(data, addr, thumb) if k == "$d"), len(data))
    for x in md.disasm(data[:d0], addr):
        op, mn = x.op_str, x.mnemonic
        m = LOAD.match(op)
        if mn.startswith("ldr") and m:
            base = (x.address + 4) & ~3 if thumb else x.address + 8
            o = base + int(m[2], 0) - addr
            val = int.from_bytes(data[o : o + 4], "little") if 0 <= o <= len(data) - 4 else 0
            regs.pop(m[1], None)
            if syms[0][0] <= val < 0x0e000000 and at(val):
                regs[m[1]] = (val, 0)
            continue
        mm = MEM.match(op)
        if re.match(r"(ldr|str)(sb|sh|b|h|d)?(eq|ne|cs|cc|mi|pl|vs|vc|hi|ls|ge|lt|gt|le)?$", mn) and mm:
            rd, rb, off = mm[1], mm[2], int(mm[3] or "0", 0)
            if rb in regs:
                lit, delta = regs[rb]
                uses.setdefault(lit, set()).add(delta + off)
            if mn.startswith("ldr"):
                regs.pop(rd, None)
            continue
        ad = ADD.match(op)
        if re.match(r"(add|sub)s?$", mn) and ad and ad[2] in regs:
            lit, delta = regs[ad[2]]
            regs[ad[1]] = (lit, delta + int(ad[3], 0) * (-1 if mn.startswith("sub") else 1))
            continue
        if mn.startswith("bl"):
            for r in ("r0", "r1", "r2", "r3", "ip"):
                regs.pop(r, None)
            continue
        first = op.split(",")[0]
        if first in regs and not mn.startswith(("cmp", "cmn", "tst", "teq", "str", "b")):
            regs.pop(first, None)
    out = []
    for lit, offs in uses.items():
        own = at(lit)
        other = sorted((o, at(lit + o)) for o in offs if at(lit + o) != own)
        if other:
            out.append((lit, own, other))
    return out


def triggers(files):
    rows = []
    for dest in files:
        try:
            path, _, mod, funcs = b.locate(dest)
        except SystemExit:
            continue
        status, diffs, errors = ld.probe(path)
        if errors:
            continue
        secs = sections_of(dest)
        for _, _, n, _ in funcs:
            if status.get(n) == "MATCH":
                continue
            entry = cp.symbol_index().get(n)
            if not entry:
                continue
            for lit, own, other in trigger(n, data_symbols(entry[0])):
                rows.append((dest, n, lit, own, other, sorted(s for s in secs if s in (".data", ".bss")),
                             ".word" in diffs.get(n, "")))
    return rows


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="*", help="lib/ C files (default: the incomplete ones)")
    ap.add_argument("--lib", action="append", default=[], help="only this library's incomplete files")
    ap.add_argument("--all", action="store_true", help="the complete files too (with --lib: a library-wide option)")
    ap.add_argument("--triggers", action="store_true", help="the files with a base literal used across objects")
    args = ap.parse_args()
    files = [str(Path(f).resolve().relative_to(ROOT)) for f in args.files] or libraries(args.lib, args.all)
    out = ROOT / "build"
    out.mkdir(exist_ok=True)
    if args.triggers:
        per = {}
        for dest, n, lit, own, other, secs, pool in triggers(files):
            per.setdefault(dest, []).append((n, lit, own, other, secs, pool))
        with open(out / "flag_triggers.tsv", "w") as t:
            t.write("file\tfunction\tliteral\tits symbol\tits pool differs\toffsets in other symbols\tsections in delinks\n")
            for dest, items in sorted(per.items()):
                for n, lit, own, other, secs, pool in items:
                    t.write(f"{dest}\t{n}\t{lit:#x}\t{own}\t{'yes' if pool else 'no'}\t"
                            f"{', '.join(f'+{o:#x} {s}' for o, s in other[:4])}\t{','.join(secs) or 'none'}\n")
        shown = 0
        for dest, items in sorted(per.items()):
            # the mark of the trigger: the function's literal pool differs from the original's, whose base is reached
            # with offsets in other objects (a fake struct in our C for the SDK's separate statics)
            fake = [i for i in items if i[5] and any(o >= 4 for o, _ in i[3])]
            if not fake:
                continue
            shown += 1
            bases = sorted({i[2] for i in fake})
            print(f"{dest}: {len(fake)} of {len(items)} functions ({', '.join(sorted({i[0] for i in fake})[:3])}); "
                  f"bases {', '.join(bases[:3])}; delinks has {','.join(fake[0][4]) or 'no .data/.bss'}")
        print(f"{shown} of {len(per)} files with a base used across objects: {out / 'flag_triggers.tsv'}")
        return
    jobs = bwjobs.jobs()
    results = []
    with concurrent.futures.ProcessPoolExecutor(jobs, mp_context=multiprocessing.get_context("fork")) as pool, \
            open(out / "flag_search.tsv", "w") as t:
        t.write("file\tmatching\tfunctions\tlines\tgained\tlines gained\tcompiler\tflags\tlibrary.toml\n")
        for fut in concurrent.futures.as_completed([pool.submit(search, f, args.all) for f in files]):
            res = fut.result()
            results.append(res)
            text, best = report(res, verbose=len(files) == 1)
            print("\n".join(text), flush=True)
            if best and "base" in res:
                base = res["base"]
                t.write(f"{res['file']}\t{len(matched(base))}\t{len(base)}\t{total(base)}\t{best[0]}\t{best[1]}\t"
                        f"{best[2]}\t{best[3]}\t{'; '.join(toml_lines(res['file'], res['own'], best[2], best[3]))}\n")
                t.flush()
    json.dump(results, open(out / "flag_search.json", "w"))
    if len(results) >= 10:
        summary(results)
    gain = [r for r in results if "base" in r and rank(r)]
    print(f"{len(gain)} of {len(results)} files gain under another option: {out / 'flag_search.tsv'}")
    cut = [r["file"] for r in results if "base" in r and splits(r)]
    if cut:
        print(f"{len(cut)} files to split (a function matches under another compiler, others don't): {', '.join(cut[:8])}")


if __name__ == "__main__":
    main()
