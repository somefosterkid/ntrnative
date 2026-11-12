#!/usr/bin/env python3
"""Makes another version's dsd config from Black's (config/b_us) and the version's config/<version>/delta.txt.

White's config is not kept by hand: its 35,000 symbols, relocations and file ranges are Black's at White's addresses,
so they follow every change made to Black's. delta.txt (written by tools/version_map.py from both ROMs) holds only what
differs: each module's shifts, White's sizes and relocations of what White changed, its module hashes, and the files
complete in Black that don't match White yet (docs/white.md). configure.py runs this for each version with a delta
before it reads the configs:

    python3 tools/version_config.py w_us    # writes config/w_us/arm9/** and config/w_us/assets.tsv (not committed)

It fails when the delta no longer fits Black's config (a size for a function that isn't there, a file that's gone):
tools/version_map.py then writes it again. A file is rewritten only when its text changes, so ninja rebuilds nothing
when Black's config hasn't changed.
"""
import bisect
import collections
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BLACK = "b_us"

ADDR = re.compile(r"addr:0x([0-9a-f]+)")
SIZE = re.compile(r"size=0x([0-9a-f]+)")
FROM = re.compile(r"from:0x([0-9a-f]+)")
TO = re.compile(r"to:0x([0-9a-f]+)")
MODULE = re.compile(r"module:(\S+)")
RANGE = re.compile(r"(start|end):0x([0-9a-f]+)")


class Delta:
    def __init__(self, path: Path):
        self.hashes, self.sizes, self.incomplete = {}, {}, set()
        self.shifts = collections.defaultdict(list)  # module -> [(Black address, shift)], by address
        self.relocs = collections.defaultdict(list)  # module -> [(Black start, end, [White lines])]
        block = None
        for n, line in enumerate(path.read_text().splitlines(), 1):
            if not line.strip() or line.startswith("#"):
                continue
            if line.startswith("    "):
                if block is None:
                    sys.exit(f"{path}:{n}: a relocation line outside a relocs block")
                block.append(line.strip())
                continue
            word, *rest = line.split()
            block = None
            if word == "hash":
                self.hashes[rest[0]] = rest[1]
            elif word == "shift":
                self.shifts[rest[0]].append((int(rest[1], 16), int(rest[2], 16)))
            elif word == "size":
                self.sizes[(rest[0], int(rest[1], 16))] = int(rest[2], 16)
            elif word == "relocs":
                block = []
                self.relocs[rest[0]].append((int(rest[1], 16), int(rest[2], 16), block))
            elif word == "incomplete":
                self.incomplete.add(rest[0])
            else:
                sys.exit(f"{path}:{n}: unknown line {line!r}")
        for points in self.shifts.values():
            points.sort()
        self.starts = {m: [p[0] for p in points] for m, points in self.shifts.items()}

    def map(self, module, a):
        """White's address of a Black address in a module."""
        if module not in self.shifts:
            sys.exit(f"delta.txt has no shift for module {module}")
        i = bisect.bisect_right(self.starts[module], a) - 1
        return a + self.shifts[module][max(i, 0)][1]


def target_module(field):
    """The module whose addresses a relocation's module: field names (the first of an ambiguous one's)."""
    m = re.fullmatch(r"(\w+)\(([\d,]+)\)", field)
    if not m:
        return field
    first = int(m.group(2).split(",")[0])
    return f"ov{first:03d}" if m.group(1) in ("overlay", "overlays") else f"{m.group(1)}_{first}"


def module_dirs(config_yaml: Path):
    return [(m.group(1), str(Path(m.group(2)).parent)) for m in
            re.finditer(r"^\s*(?:- )?\s*name: (\S+)\n(?:.*\n){0,3}?\s*delinks: (\S+)", config_yaml.read_text(), re.M)]


def base_of(file):
    """The file an alias in another module stands for (card_rom@ltd.c: card_rom.c), or the file itself."""
    return re.sub(r"@\w+(\.\w+)$", r"\1", file)


def hex8(a):
    return f"0x{a:08x}"


def make(version: str) -> dict[Path, str]:
    """{path: text} of the version's config files."""
    delta_path = ROOT / "config" / version / "delta.txt"
    delta = Delta(delta_path)
    bconfig = ROOT / "config" / BLACK / "arm9"
    wconfig = ROOT / "config" / version / "arm9"
    out = {}
    stale = []
    used_sizes, used_files = set(), set()
    for name, sub in module_dirs(bconfig / "config.yaml"):
        def own(a, name=name):
            return delta.map(name, a)

        lines = []
        for line in (bconfig / sub / "symbols.txt").read_text().splitlines():
            m = ADDR.search(line)
            if m:
                a = int(m.group(1), 16)
                if (name, a) in delta.sizes and "kind:function" in line:
                    line = SIZE.sub(f"size=0x{delta.sizes[(name, a)]:x}", line)
                    used_sizes.add((name, a))
                line = ADDR.sub(f"addr:{hex8(own(a))}", line)
            lines.append(line)
        out[wconfig / sub / "symbols.txt"] = "\n".join(lines) + "\n"

        blocks = delta.relocs.get(name, [])
        starts = [b[0] for b in blocks]
        kept = []
        for line in (bconfig / sub / "relocs.txt").read_text().splitlines():
            m = FROM.search(line)
            if not m:
                kept.append((-1, line))
                continue
            f = int(m.group(1), 16)
            i = bisect.bisect_right(starts, f) - 1
            if i >= 0 and blocks[i][0] <= f < blocks[i][1]:
                continue  # White's own relocations from there come from the delta
            t, mod = TO.search(line), MODULE.search(line)
            line = FROM.sub(f"from:{hex8(own(f))}", line)
            if t and mod:
                line = TO.sub(f"to:{hex8(delta.map(target_module(mod.group(1)), int(t.group(1), 16)))}", line)
            kept.append((own(f), line))
        for _, _, block in blocks:
            kept += [(int(FROM.search(l).group(1), 16), l) for l in block]
        kept.sort(key=lambda r: r[0])
        out[wconfig / sub / "relocs.txt"] = "\n".join(l for _, l in kept) + "\n"

        lines = []
        file = None
        for line in (bconfig / sub / "delinks.txt").read_text().splitlines():
            if line and not line[0].isspace() and line.endswith(":"):
                file = line[:-1]
            elif file and line.strip() == "complete" and (file in delta.incomplete or base_of(file) in delta.incomplete):
                # (a file's alias in a second module, card_rom@ltd.c, links with the file: tools/two_modules.py)
                used_files.add(base_of(file) if base_of(file) in delta.incomplete else file)
                continue
            line = RANGE.sub(lambda m: f"{m.group(1)}:{hex8(own(int(m.group(2), 16)))}", line)
            lines.append(line)
        out[wconfig / sub / "delinks.txt"] = "\n".join(lines) + "\n"

    text = (bconfig / "config.yaml").read_text().replace(f"/{BLACK}", f"/{version}")
    for name, h in delta.hashes.items():
        text, n = re.subn(rf"(name: {re.escape(name)}\n(?:.*\n){{0,2}}?\s*hash: )\w+", rf"\g<1>{h}", text)
        if n != 1:
            stale.append(f"hash of module {name}, not in config.yaml")
    out[wconfig / "config.yaml"] = text

    assets = ROOT / "config" / BLACK / "assets.tsv"
    if assets.exists():
        rows = assets.read_text().splitlines()
        lines = rows[:1]
        for row in rows[1:]:
            asset, mod, start, end, kind = row.split("\t")
            lines.append("\t".join([asset, mod, hex8(delta.map(mod, int(start, 16))),
                                    hex8(delta.map(mod, int(end, 16))), kind]))
        out[ROOT / "config" / version / "assets.tsv"] = "\n".join(lines) + "\n"

    stale += [f"size of {m} {hex8(a)}, not a function" for m, a in sorted(set(delta.sizes) - used_sizes)]
    stale += [f"incomplete {f}, not a complete file" for f in sorted(delta.incomplete - used_files)]
    if stale:
        sys.exit(f"{delta_path.relative_to(ROOT)} doesn't fit config/{BLACK} any more "
                 f"(python3 tools/version_map.py {version} writes it again):\n  " + "\n  ".join(stale))
    return out


def main():
    for version in sys.argv[1:]:
        written = 0
        for path, text in make(version).items():
            if not path.exists() or path.read_text() != text:
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text(text)
                written += 1
        print(f"config/{version}: {written} files written")


if __name__ == "__main__":
    main()
