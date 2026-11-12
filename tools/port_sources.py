#!/usr/bin/env python3
"""The C files of the PC build (docs/port-build.md): the decomp's game C (src/, lib/gfl, lib/poke_net, lib/spl) minus
the files port/exclude.txt leaves out, then every port/src/*.c. A game file with a file of the same path under
port/extern (port/extern/src/ov131/p_sta_info.c) is compiled through that one, which includes it and adds the extern
declarations of its plain `inline` functions; its object name still ends in the game file's (meson's mangled path), so
tools/port_overlays.py keeps it in its overlay. port/meson.build runs it at setup; after adding or
removing a file run `meson setup --reconfigure <builddir>` (meson doesn't watch directories).

    python3 tools/port_sources.py                 # the build's sources, one absolute path a line
    python3 tools/port_sources.py --excluded      # the files left out: path, category, reason
    python3 tools/port_sources.py --check         # exclude.txt's patterns that match no file (stale lines)
    python3 tools/port_sources.py --b2w2 DIR      # Black 2's and White 2's: pokebw2's checkout DIR (docs/b2w2.md):
                                                  # its src/, lib/poke_net, lib/spl minus port/exclude_b2w2.txt, through
                                                  # port/extern_b2w2/; no port/src (Black's build compiles those)
    python3 tools/port_sources.py --nnsys         # our NitroSystem's (port/meson.build's nnsys option, docs/nnsys-port.md):
                                                  # lib/nnsys/src (through port/extern/lib/nnsys/), without g2d/load/
                                                  # once port/src/nnsys_load64.c stands in for its loaders
    python3 tools/port_sources.py --nnsys-port    # the port's side of it: port/src/nnsys_*.c (never in the game's list)

port/exclude.txt: one pattern a line (a path from the repo root, or an fnmatch glob such as src/ov172/*.c), then a
category and the reason. '#' starts a comment. Categories: dsi (DSi-only, unused: the PC runs as an original DS),
hw (drives DS hardware libntr lacks: port/src replaces it), online (online play, stubbed), sdk (SDK code in the
game's dirs that libntr has), todo (doesn't compile yet: fix the C or port/include, then drop the line).
"""
import argparse
import fnmatch
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GAME_DIRS = ("src", "lib/gfl", "lib/poke_net", "lib/spl")
EXCLUDE = ROOT / "port/exclude.txt"
EXTERN = "port/extern/"
CATEGORIES = {"dsi", "hw", "online", "sdk", "todo"}


def excludes():
    """[(pattern, category, reason, line number)] of port/exclude.txt (EXCLUDE)."""
    out = []
    if not EXCLUDE.exists():
        return out
    for n, line in enumerate(EXCLUDE.read_text().splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        parts = line.split(None, 2)
        if len(parts) < 2 or parts[1] not in CATEGORIES:
            sys.exit(f"port/exclude.txt:{n}: want '<path or glob> <{'|'.join(sorted(CATEGORIES))}> <reason>'")
        out.append((parts[0], parts[1], parts[2] if len(parts) > 2 else "", n))
    return out


def game_files(root=ROOT, dirs=GAME_DIRS):
    return sorted(p.relative_to(root).as_posix() for d in dirs for p in (root / d).rglob("*.c"))


def port_files():
    return sorted(p.relative_to(ROOT).as_posix() for p in (ROOT / "port/src").glob("*.c") if not p.name.startswith("nnsys_"))


def nnsys_files():
    """lib/nnsys's C (our NitroSystem), through port/extern/ where it has a stand-in; the 2D loaders (g2d/load/) are
    left out when port/src/nnsys_load64.c has their 64-bit versions"""
    loaders = (ROOT / "port/src/nnsys_load64.c").is_file()
    out = []
    for f in game_files(ROOT, ("lib/nnsys/src",)):
        if loaders and f.startswith("lib/nnsys/src/g2d/load/"):
            continue
        out.append(EXTERN + f if (ROOT / EXTERN / f).is_file() else f)
    return out


def matching(rel, ex):
    for pat, cat, why, _ in ex:
        if rel == pat or fnmatch.fnmatchcase(rel, pat):
            return pat, cat, why
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--excluded", action="store_true")
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--relative", action="store_true", help="paths from the repo root")
    ap.add_argument("--b2w2", metavar="DIR", help="pokebw2's checkout: Black 2's and White 2's files")
    ap.add_argument("--nnsys", action="store_true", help="lib/nnsys's files (our NitroSystem)")
    ap.add_argument("--nnsys-port", action="store_true", help="port/src/nnsys_*.c")
    a = ap.parse_args()
    if a.nnsys or a.nnsys_port:
        out = nnsys_files() if a.nnsys else sorted(p.relative_to(ROOT).as_posix()
                                                   for p in (ROOT / "port/src").glob("nnsys_*.c"))
        print("\n".join(f if a.relative else str(ROOT / f) for f in out))
        return 0
    if a.b2w2:
        global EXCLUDE
        EXCLUDE = ROOT / "port/exclude_b2w2.txt"
        tree, ext = Path(a.b2w2).resolve(), ROOT / "port/extern_b2w2"
        ex = excludes()
        out = [str(ext / f) if (ext / f).is_file() else str(tree / f)
               for f in game_files(tree, ("src", "lib/poke_net", "lib/spl")) if not matching(f, ex)]
        print("\n".join(out))
        return 0
    ex = excludes()
    files = game_files()
    if a.check:
        stale = [e for e in ex if not any(f == e[0] or fnmatch.fnmatchcase(f, e[0]) for f in files)]
        for pat, cat, why, n in stale:
            print(f"port/exclude.txt:{n}: {pat} matches no file")
        return 1 if stale else 0
    if a.excluded:
        for f in files:
            m = matching(f, ex)
            if m:
                print(f"{f}\t{m[1]}\t{m[2]}")
        return 0
    out = [EXTERN + f if (ROOT / EXTERN / f).is_file() else f for f in files if not matching(f, ex)] + port_files()
    print("\n".join(f if a.relative else str(ROOT / f) for f in out))
    return 0


if __name__ == "__main__":
    sys.exit(main())
