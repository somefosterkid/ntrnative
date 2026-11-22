#!/usr/bin/env python3
"""legcache.py: a route leg's result cache (lg1 11-19). A leg that passed with these game inputs, this leg line, this start
save, these options on this machine class passes again the same way: its result (frames, seconds, end save) can stand in
for a run. Nothing here runs a leg; route.py --chain and round.sh are meant to consult it (not wired yet).

The key (sha256) of a leg: inputs(ROOT) + the game + the leg's line in tools/routes/GAME.txt + the start save's id
(start_id(): NEWGAME; a library save: sha256 of its bytes; a save an earlier leg of the same run made: "leg:" + that leg's
key, so a chain's keys follow from its first save without the saves between: Platinum's oreburgh, mine and gym run twice
and a round's candidate keeps only the second one's saves) + the run options (opts(): route.py's fast opts, cheat, skip, cap, hang, gpu, the build dir env)
+ the machine class (HG/SS frames differ by compiler, bds1 11-17; saves differed laptop/bd until sv1).
inputs(ROOT): the git blob ids of every tracked file that can change what a leg plays (a file modified in the work tree:
its content's hash). Left out (they change pictures, other platforms or nothing a leg runs; round_select.py's classes):
docs/, *.md, port/gpu/, Android/iOS/Metal host files, tools/routes/ (the leg's own line is in the key), tools/plays/,
tools/*_tests.py. Everything else counts (when unsure: it counts).
The second key (tb4 11-20, arch1 B4: early cutoff): with --exe "ELFHASH RDEFSHA" (what a round's one artifact build prints, `lp_route.sh
--art-hash`: tools/elfhash.py of the exe, which leaves out the debug sections and the build id, and rom_definitions.json's sha256) the
inputs() part is replaced by exe_inputs(): that pair + driver_hash(ROOT) (the Python that plays a leg: tools/route.py and the tools/*.py it
imports, as syntax trees: a comment, a docstring or a blank line there changes nothing). A route.py comment, a docs file, another
platform's .ini give the same artifact and the same key: the chain is cached. A probe tries the source key first (no build needed), then
this one; an add stores under both.
Entries: CACHE/GAME/KEY.json {verdict, frames, secs, save (sha256 of the end save, "" for a leg that makes none, "?" when the
bytes were not kept), leg, made};
end saves CACHE/saves/SHA.bin.xz. Only a passed leg is stored (never a FAIL).

  legcache.py probe --game G [--root R] [--cache DIR] [--saves DIR] [--klass K] [--table OUT] [--exe "ELFHASH RDEFSHA" [--exe-only]]
      the chain (every leg of tools/routes/G.txt in order) from the cache: "CACHED n/n legs, F frames" (rc 0; OUT: route.py's
      chain table) or "MISS at leg L (k/n)" (rc 1)
  legcache.py add --game G --cand DIR --klass K [--root R] [--cache DIR] [--saves DIR] [--made TEXT] [--exe "ELFHASH RDEFSHA"]
      store a chain a round proved (chain_split.py's candidate: legs.txt, table.tsv, LEG.bin.xz)
CACHE default ~/.cache/bwport/legcache (ROUTE_LEGCACHE); SAVES the library (BWPORT_SAVES, ~/saves); K default the host name.
"""
import argparse
import ast
import hashlib
import json
import lzma
import os
import re
import socket
import subprocess
import sys
from pathlib import Path

PASS = ("ok", "look")
SKIP = re.compile(r'^docs/|\.md$|^port/gpu/|^port/src/(android_|ios_|apple_|metal_|imgui_impl_metal)|^port/src/[^/]*\.mm?$'
                  r'|^port/machine/android-|^tools/routes/|^tools/plays/|^tools/[^/]*_tests\.py$')
HG_BUILD = {"BWPORT_BUILD": "build/port/linux-hgss"}   # round_job.sh's HG/SS chains


def sha(b):
    return hashlib.sha256(b).hexdigest()


def inputs(root):
    """one hash of the game-logic inputs of the checkout at ROOT (tracked files, the work tree's edits included)"""
    ls = subprocess.run(["git", "-C", str(root), "ls-files", "-s"], capture_output=True, text=True, check=True).stdout
    dirty = set(subprocess.run(["git", "-C", str(root), "diff", "--name-only", "HEAD"], capture_output=True, text=True, check=True).stdout.split())
    h = hashlib.sha256()
    for line in ls.splitlines():
        meta, path = line.split("\t", 1)
        if SKIP.search(path):
            continue
        blob = meta.split()[1]
        if path in dirty:
            p = Path(root) / path
            blob = sha(p.read_bytes()) if p.is_file() else "deleted"
        h.update(f"{path}\0{blob}\n".encode())
    return h.hexdigest()


def _norm(path):
    """a Python file as its syntax tree without docstrings and positions (a comment or a blank line is not in it); a file that does not parse: its bytes"""
    b = Path(path).read_bytes()
    try:
        t = ast.parse(b)
    except (SyntaxError, ValueError):
        return b
    for n in ast.walk(t):
        if isinstance(n, (ast.Module, ast.ClassDef, ast.FunctionDef, ast.AsyncFunctionDef)) and n.body \
                and isinstance(n.body[0], ast.Expr) and isinstance(getattr(n.body[0], "value", None), ast.Constant) and isinstance(n.body[0].value.value, str):
            n.body = n.body[1:] or [ast.Pass()]
    return ast.dump(t).encode()


def driver_files(root, entry="tools/route.py", skip=("legcache",)):
    """tools/route.py and, transitively, the tools/NAME.py files it imports (anywhere in the file; the cache's own module excepted)"""
    tools, seen, todo = Path(root) / "tools", [], [Path(root) / entry]
    while todo:
        p = todo.pop()
        if p in seen or not p.is_file():
            continue
        seen.append(p)
        try:
            tree = ast.parse(p.read_bytes())
        except (SyntaxError, ValueError):
            continue
        for n in ast.walk(tree):
            names = [a.name for a in n.names] if isinstance(n, ast.Import) else [n.module] if isinstance(n, ast.ImportFrom) and n.module and not n.level else []
            todo += [tools / f"{m.split('.')[0]}.py" for m in names if m.split(".")[0] not in skip]
    return sorted(seen)


def driver_hash(root):
    h = hashlib.sha256()
    for p in driver_files(root):
        h.update(f"{p.relative_to(root).as_posix()}\0".encode() + _norm(p) + b"\n")
    return h.hexdigest()


def exe_inputs(root, exe):
    """the key's inputs part for an artifact: EXE = "ELFHASH RDEFSHA" (the exe's content hash and rom_definitions.json's) + the driver's"""
    e = exe.split()
    if len(e) != 2 or not all(re.fullmatch(r"[0-9a-f]{64}", x) for x in e):
        raise SystemExit(f"legcache: --exe wants 'ELFHASH RDEFSHA' (two sha256 hex): {exe!r}")
    return f"exe:{e[0]}:{e[1]}:{driver_hash(root)}"


def opts(fast_opts=True, cheat=True, skip=8, cap=600, hang=20, gpu=False, env=None):
    e = os.environ if env is None else env
    b = " ".join(f"{k}={e.get(k, '')}" for k in ("BWPORT_BUILD", "BWPORT_PLAT_BUILD", "BWPORT_HG_BUILD"))
    return f"fast={int(fast_opts)} cheat={int(cheat)} skip={skip} cap={cap} hang={hang} gpu={int(gpu)} {b}"


def leg_lines(root, game):
    """[(name, start, save, line)]: route.py's read_legs, the raw line kept (comments and spaces stripped)"""
    out = []
    for line in (Path(root) / "tools/routes" / f"{game}.txt").read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        f = [x.strip() for x in line.split(" | ")]
        f += ["-", ""][len(f) - 4:] if len(f) < 6 else []
        out.append((f[0], f[1], f[4] or "-", " | ".join(f)))
    return out


def start_id(save_bytes=None, leg_key=None):
    """a leg's start save in its key: NEWGAME (neither), the bytes' sha256 (a library save), or the key of the leg that made it"""
    return f"leg:{leg_key}" if leg_key else "NEWGAME" if save_bytes is None else sha(save_bytes)


def key(inp, game, line, sid, opt, klass):
    return sha(f"{inp}\n{game}\n{line}\n{sid}\n{opt}\n{klass}".encode())


def get(cache, game, k):
    p = Path(cache) / game / f"{k}.json"
    return json.loads(p.read_text()) if p.exists() else None


def save_bytes(cache, h):
    p = Path(cache) / "saves" / f"{h}.bin.xz"
    return lzma.decompress(p.read_bytes()) if p.exists() else None


def put(cache, game, k, leg, verdict, frames, secs, end_save, made=""):
    """store a passed leg (a FAIL never: returns False); END_SAVE: bytes, None (no save) or "?" (made, bytes not kept)"""
    if verdict not in PASS:
        return False
    h = end_save if end_save == "?" else ""
    if isinstance(end_save, bytes):
        h = sha(end_save)
        p = Path(cache) / "saves" / f"{h}.bin.xz"
        if not p.exists():
            p.parent.mkdir(parents=True, exist_ok=True)
            p.with_suffix(".tmp").write_bytes(lzma.compress(end_save))
            os.replace(p.with_suffix(".tmp"), p)
    p = Path(cache) / game / f"{k}.json"
    p.parent.mkdir(parents=True, exist_ok=True)
    p.with_suffix(".tmp").write_text(json.dumps({"verdict": verdict, "frames": frames, "secs": secs, "save": h, "leg": leg, "made": made}))
    os.replace(p.with_suffix(".tmp"), p)
    return True


def lib_save(saves, name):
    p = Path(saves) / f"{name}.bin"
    return p.read_bytes() if p.exists() else None


def walk(a, legs, step, inp):
    """the chain's legs in order: STEP(i, name, save, key) -> False to stop; returns the count done. INP: inputs() or exe_inputs()"""
    opt = opts(env=HG_BUILD if a.game in ("hg", "ss") else {})
    produced = {}   # save name -> start_id
    for i, (n, start, sv, line) in enumerate(legs):
        if start == "NEWGAME":
            sid = start_id()
        elif start in produced:
            sid = produced[start]
        else:
            sb = lib_save(a.saves, start)
            if sb is None:
                return i
            sid = start_id(sb)
        k = key(inp, a.game, line, sid, opt, a.klass)
        if step(i, n, sv, k) is False:
            return i
        if sv != "-":
            produced[sv] = start_id(leg_key=k)
    return len(legs)


def probe(a):
    legs = leg_lines(a.root, a.game)
    keys = ([] if a.exe_only and a.exe else [("", inputs(a.root))]) + ([(" (exe key)", exe_inputs(a.root, a.exe))] if a.exe else [])
    for how, inp in keys:
        rows = []

        def step(i, n, sv, k):
            e = get(a.cache, a.game, k)
            if e is None or e["verdict"] not in PASS:
                return False
            rows.append((n, e["frames"], e["secs"], e["verdict"]))

        k = walk(a, legs, step, inp)
        if k == len(legs):
            break
    if k < len(legs):
        print(f"MISS at leg {legs[k][0]} ({k}/{len(legs)})")
        return 1
    if a.table:
        t = ["leg\tframes\tseconds\tresult"] + [f"{n}\t{'-' if v is None else v}\t{s:.1f}\t{r}" for n, v, s, r in rows]
        t.append(f"total\t{sum(v or 0 for _, v, _, _ in rows)}\t{sum(s for _, _, s, _ in rows):.1f}\t{len(rows)}/{len(rows)} ok")
        Path(a.table).write_text("\n".join(t) + "\n")
    print(f"CACHED {len(rows)}/{len(legs)} legs, {sum(r[1] or 0 for r in rows)} frames{how}")
    return 0


def add(a):
    legs, cand = leg_lines(a.root, a.game), Path(a.cand)
    rows = {}
    for l in (cand / "table.tsv").read_text().splitlines()[1:]:
        f = l.split("\t")
        if len(f) >= 4 and f[0] != "total":
            rows.setdefault(f[0], []).append(f)
    names = [n for n, *_ in legs]
    n_added = [0]
    n_each = []

    def step(i, n, sv, k):
        occ, r = names[:i].count(n), rows.get(n, [])
        if len(r) != names.count(n):
            return False
        _, fr, secs, verdict = r[occ][:4]
        end = None
        if sv != "-":   # a name twice (Platinum: oreburgh, mine, gym): the candidate keeps the last one's save only
            p = cand / f"{n}.bin.xz"
            if occ < len(r) - 1:
                end = "?"
            elif not p.exists():
                return False
            else:
                end = lzma.decompress(p.read_bytes())
        if not put(a.cache, a.game, k, n, verdict, None if fr == "-" else int(fr), float(secs), end, a.made):
            return False
        n_added[0] += 1

    for inp in [inputs(a.root)] + ([exe_inputs(a.root, a.exe)] if a.exe else []):
        n_added[0] = 0
        walk(a, legs, step, inp)
        n_each.append(n_added[0])
    print(f"added {n_each[0]}/{len(legs)} legs of {a.game} (class {a.klass}){' + the exe key' if a.exe else ''}")
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=["probe", "add"])
    ap.add_argument("--game", required=True)
    ap.add_argument("--root", default=str(Path(__file__).resolve().parents[1]))
    ap.add_argument("--cache", default=os.environ.get("ROUTE_LEGCACHE", os.path.expanduser("~/.cache/bwport/legcache")))
    ap.add_argument("--saves", default=os.environ.get("BWPORT_SAVES", str(Path.home() / "saves")))
    ap.add_argument("--klass", default=socket.gethostname())
    ap.add_argument("--table", default="")
    ap.add_argument("--exe", default="", help='the artifact\'s "ELFHASH RDEFSHA" (lp_route.sh --art-hash): probe/add under the exe key too')
    ap.add_argument("--exe-only", action="store_true", help="probe: the exe key only (no source key)")
    ap.add_argument("--cand", default="")
    ap.add_argument("--made", default="")
    a = ap.parse_args(argv)
    return probe(a) if a.cmd == "probe" else add(a)


if __name__ == "__main__":
    sys.exit(main())
