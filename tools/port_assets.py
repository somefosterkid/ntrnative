#!/usr/bin/env python3
"""The PC build's game data tables, loaded from the player's ROM at start instead of compiled in (docs/port-build.md,
"Game data from the ROM").

The ROM build compiles each config/<version>/assets.tsv row from the base ROM (tools/asset.py writes build/asset/<name>.inc,
which the C includes). The PC build ships, so it must not hold those bytes: for it this tool

  - writes a placeholder .inc per row (`incs`): the same shape (records, fields, the names of pointer fields) with every
    number 0 (a pointer field the row calls u32 takes it too), so the C compiles to storage of the right size and type, and an #error if a file that is not rewritten
    includes it;
  - rewrites each PC source that includes an asset (`src`, a meson generator): the asset's declaration loses `const` (the
    compiler can then neither fold its values into code nor put it in read-only memory), quote-included .c files are
    inlined and local headers made absolute (the copy lives in the build dir; `#line` keeps the original's names and
    lines), and a constructor at the end registers each table (game code, row name, address, size) with
    port/src/asset_load.c;
  - writes the rows (`rows`): each version's name, module, DS range and record layout, which asset_load.c reads the ROM by.

port/src/asset_load.c fills the running game's tables from its ROM before the game boots (port/src/machine.c), Black's
from Black's ROM and White's from White's, and keeps the values in the overlay and machine snapshots.

    python3 tools/port_assets.py list                          # the PC sources that include an asset (absolute paths)
    python3 tools/port_assets.py src IN OUT DEPFILE            # one rewritten source (the meson generator)
    python3 tools/port_assets.py incs ROM_BUILD OUTDIR STAMP   # placeholders: OUTDIR/asset (Black), OUTDIR/w_us/asset
    python3 tools/port_assets.py rows OUT.c                    # the rows table for asset_load.c

No ROM byte reaches any output: the placeholders hold 0s and symbol names (pointer fields name a function or table
of the game, as the ROM build's .inc does: the ROM build's .inc under ROM_BUILD is read for those names only).
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import asset_table  # noqa: E402

PLAIN = {"u8": 1, "u16": 2, "u32": 4, "s8": 1, "s16": 2, "s32": 4, "fx16": 2, "fx32": 4}
VERSIONS = {"b_us": "IRBO", "w_us": "IRAO"}
INC = re.compile(r'^#include "asset/(\w+)\.inc"[ \t]*$', re.M)
QUOTE = re.compile(r'^([ \t]*#[ \t]*include[ \t]*)"([^"]+)"', re.M)


# A row's layout gives the ROM's bytes, not always the C's fields (tools/asset_table.py infers one that renders the same
# bytes). That is all a plain copy needs, when the host's layout is the DS's. A C type with a pointer (or a union with one)
# is 8 bytes wider there, so its tables are copied field by field by the C's fields, given here by type name: ptr a pointer
# (left as compiled: the ROM's is a DS address, the placeholder keeps the symbol or NULL), w32 a 4-byte value in a
# pointer-sized slot (a union of a u32 and a pointer).
TYPE_LAYOUT = {
    "G3DSceneResourceSetup": "w32,u16*2,u32",          # include/gfl/g3d.h: union { arcId; path }, fileId, unk6, type
    "G3DSceneActorSetup": "u16*4,ptr,u16",             # include/gfl/g3d.h: the animations pointer (NULL in the ROM's)
    # src/ov021/fieldmap.c: FieldG3DMapperConfig (chunkDatIds a pointer, NULL), ctrlVTable, useMapMatrix, heapSize
    "FieldMapConfig": "s32,u32,u16*2,u32*2,u16*2,u32,ptr,u32*6,u8,ptr,u32*2",
}


def rows(version):
    t = ROOT / "config" / version / "assets.tsv"
    if not t.exists():
        return []
    out = []
    for line in t.read_text().splitlines()[1:]:
        if line.strip():
            name, mod, start, end, kind = line.split("\t")
            out.append((name, mod, int(start, 16), int(end, 16), kind))
    return out


def host_layout(kind, ptr=8):
    """(record size, record count factor) on the host: plain arrays and records, pointer fields ptr bytes wide."""
    if kind in PLAIN:
        return PLAIN[kind], PLAIN[kind]
    cols = asset_table.parse_layout(kind.replace("w32", "ptr"))
    align = 1
    off = 0
    for k, _ in cols:
        sz = ptr if k in ("ptr", "ref") else asset_table.SIZES[k]
        off = (off + sz - 1) // sz * sz + sz
        align = max(align, sz)
    _, ds_stride = asset_table.offsets(cols)
    return (off + align - 1) // align * align, ds_stride


def host_size(kind, start, end):
    host, ds = host_layout(kind)
    return (end - start) // ds * host


# ---------------------------------------------------------------- the sources
def includes_asset(path, seen=None):
    seen = set() if seen is None else seen
    path = Path(path).resolve()
    if path in seen or not path.exists():
        return False
    seen.add(path)
    text = path.read_text(errors="replace")
    if INC.search(text):
        return True
    return any(includes_asset(path.parent / m[2], seen) for m in QUOTE.finditer(text) if not m[2].startswith("asset/"))


def pc_sources():
    out = subprocess.run([sys.executable, str(ROOT / "tools/port_sources.py")], capture_output=True, text=True, check=True)
    return out.stdout.split()


def depth_at(text, pos):
    """Brace depth at pos (comments, strings and character constants skipped): 0 = file scope."""
    d = 0
    for m in re.finditer(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|[{}]', text[:pos], re.S):
        if m[0] == "{":
            d += 1
        elif m[0] == "}":
            d -= 1
    return d


def enclosing(blank, pos):
    """(start, end) of the file-scope construct (a function) around pos: after the last file-scope `;`, `}` or preprocessor
    line before it, to just after the `}` that closes it."""
    d, start, end = 0, 0, None
    for m in re.finditer(r"[{};]|^[ \t]*#[^\n]*", blank, re.M):
        if m.start() >= pos and d == 0:
            break
        c = m[0]
        if m.start() < pos:
            if c == "{":
                d += 1
            elif c == "}":
                d -= 1
            if d == 0 and c in ";}" or c.lstrip().startswith("#") and d == 0:
                start = m.end()
        else:
            if c == "{":
                d += 1
            elif c == "}":
                d -= 1
                if d == 0:
                    end = m.end()
                    break
    while blank[start].isspace():
        start += 1
    if end is None:
        sys.exit("enclosing: no end")
    return start, end


def ctype(spec):
    """The type name of a declaration's specifiers (static const Foo -> Foo)."""
    words = [w for w in re.findall(r"\w+", spec) if w not in ("static", "const", "volatile", "extern", "struct", "union")]
    return words[-1] if words else ""


def shown(p):
    """A path as #line gives it: relative to the build dir meson runs the generator in, as its compile lines name sources."""
    import os
    return os.path.relpath(p)


def rewrite(path, deps, found, top=True):
    """The text of path for the build dir: asset declarations without const, .c includes inlined, local headers absolute."""
    path = Path(path).resolve()
    deps.append(path)
    text = path.read_text(errors="replace")
    # the text with comments, strings and character constants blanked (same offsets), for finding declarations
    blank = re.sub(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'',
                   lambda c: re.sub(r"[^\n]", " ", c[0]), text, flags=re.S)
    edits = []  # (start, end, new text)
    for m in INC.finditer(text):
        pre = blank[:m.start()].rstrip()
        if not pre.endswith("{"):
            sys.exit(f"{path}: asset {m[1]}: the #include is not the first thing in its initialiser")
        brace = len(pre) - 1
        stmt = max(pre.rfind(";"), pre.rfind("}"), pre.rfind("\n#"), pre[:brace].rfind("{"))
        if pre.rfind("\n#") == stmt:
            stmt = pre.find("\n", stmt + 1)
        while blank[stmt + 1].isspace():   # the declaration from its first word (comments before it stay)
            stmt += 1
        if blank[stmt + 1:brace] != text[stmt + 1:brace]:
            sys.exit(f"{path}: asset {m[1]}: a comment inside its declaration")
        decl = text[stmt + 1:brace]
        d = re.search(r"(\w+)\s*((?:\[[^\]]*\]\s*)*)=\s*$", decl)
        if not d:
            sys.exit(f"{path}: asset {m[1]}: no declaration before its #include")
        sym = d[1]
        bare = re.sub(r"\bconst\b", "", decl[:d.start(1)])
        attr = " __attribute__((section(\".data\")))"   # in .data even as zeros (not .bss: an overlay load zeroes that)
        if depth_at(text, stmt + 1) != 0:
            # a static in a function: the table moves before the function under a name of its own (a constructor at the
            # end can't name it there), and the function's uses reach it by a #define until the function ends
            if not re.search(r"\bstatic\b", decl):
                sys.exit(f"{path}: asset {m[1]}: {sym} is an automatic array (not static)")
            close = re.match(r"\s*\};", text[m.end():])
            if not close:
                sys.exit(f"{path}: asset {m[1]}: no `}};` after its #include")
            fstart, fend = enclosing(blank, stmt + 1)
            store = f"bwasset_local_{sym}_{stmt + 1}"
            gone = text[stmt + 1:m.end() + close.end()]
            edits.append((fstart, fstart, f"static {bare.replace('static', '')}{store} {d[2]}{attr} = {{\n{m[0]}\n}};\n"
                          f"#define {sym} {store}\n#line {text.count(chr(10), 0, fstart) + 1}\n"))
            edits.append((stmt + 1, m.end() + close.end(), "\n" * gone.count("\n")))
            edits.append((fend, fend, f"\n#undef {sym}\n#line {text.count(chr(10), 0, fend) + 1}\n"))
            found.append((m[1], store, TYPE_LAYOUT.get(ctype(bare))))
            continue
        if re.search(r"\bstatic\b", decl):
            store = sym
            edits.append((stmt + 1, brace, bare + sym + " " + d[2] + attr + " ="))
            # its forward declarations in the file lose const too
            for f in re.finditer(rf"(?:^|(?<=[;}}]))([^;{{}}#=()]*\b{sym}\s*(?:\[[^\]]*\]\s*)*;)", blank, re.M):
                if f.start(1) < stmt + 1 and "const" in f[1] and depth_at(text, f.start(1)) == 0:
                    edits.append((f.start(1), f.end(1), re.sub(r"\bconst\b", "", f[1])))
        else:
            # a global the headers declare const: the storage is a static of another name, the global its alias
            store = "bwasset_rw_" + sym
            edits.append((stmt + 1, brace, "static " + bare + store + " " + d[2] + attr + " ="))
            close = re.match(r"\s*\};", text[m.end():])
            if not close:
                sys.exit(f"{path}: asset {m[1]}: no `}};` after its #include")
            at = m.end() + close.end()
            q = "const " if re.search(r"\bconst\b", decl[:d.start(1)]) else ""
            edits.append((at, at, f' extern {q}__typeof__({store}) {sym} __attribute__((alias("{store}")));'))
        found.append((m[1], store, TYPE_LAYOUT.get(ctype(bare))))
    for m in QUOTE.finditer(text):
        inc = m[2]
        if inc.startswith("asset/"):
            continue
        target = (path.parent / inc).resolve()
        if not target.is_file():
            continue
        line_end = text.find("\n", m.end())
        line_end = len(text) if line_end < 0 else line_end
        if inc.endswith(".c") or includes_asset(target):   # a .c, or a local header with an asset: inlined
            body = rewrite(target, deps, found, top=False)
            nl = text.count("\n", 0, line_end) + 2  # the line after the #include
            edits.append((m.start(), line_end, f'#line 1 "{shown(target)}"\n{body}\n#line {nl} "{shown(path)}"'))
        else:
            edits.append((m.start(1), m.end(), f'{m[1]}"{target}"'))
    edits.sort()
    out, pos = [], 0
    for s, e, new in edits:
        if s < pos:
            sys.exit(f"{path}: overlapping rewrites at {s}")
        out += [text[pos:s], new]
        pos = e
    out.append(text[pos:])
    body = "".join(out)
    return (f'#define BWASSET_REWRITTEN 1\n#line 1 "{shown(path)}"\n' + body) if top else body


def write_src(src, out, depfile):
    deps, found = [], []
    text = rewrite(src, deps, found)
    rng = {}
    for version in VERSIONS:
        for name, mod, start, end, kind in rows(version):
            rng.setdefault(name, (kind, start, end))
    reg = ["", '#line 1 "tools/port_assets.py"',
           "void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);",
           "#ifdef WHITE", '#define BWASSET_GAME_ "IRAO"', "#else", '#define BWASSET_GAME_ "IRBO"', "#endif"]
    for name, sym, layout in found:
        if name not in rng:
            sys.exit(f"{src}: asset {name} is in no assets.tsv")
        kind, start, end = rng[name]
        want = host_size(layout or kind, start, end)
        # a plain copy may fill the start of a bigger table (an initialiser shorter than its type: the rest is zeros)
        op = "==" if layout else ">="
        reg.append(f'_Static_assert(sizeof({sym}) {op} {want}, "asset {name}: {sym} is not its row\'s {want} bytes on '
                   f'the host (a pointer in its type? tools/port_assets.py TYPE_LAYOUT)");')
    reg.append("__attribute__((constructor)) static void bwasset_register_(void) {")
    for name, sym, layout in found:
        lay = f'"{layout}"' if layout else "0"
        reg.append(f'    PORT_AssetAdd(BWASSET_GAME_, "{name}", (void *)&{sym}, sizeof({sym}), {lay});')
    reg.append("}")
    new = text + "\n" + "\n".join(reg) + "\n"
    out = Path(out)
    if not out.exists() or out.read_text() != new:
        out.write_text(new)
    dep_paths = " ".join(str(p).replace(" ", "\\ ") for p in deps + [Path(__file__).resolve()])
    Path(depfile).write_text(f"{out}: {dep_paths}\n")


# ---------------------------------------------------------------- the placeholders
def rom_inc_fields(path):
    """The fields of each record of a ROM build .inc (for the pointer fields' names)."""
    recs = []
    for line in path.read_text().splitlines():
        line = line.strip().rstrip(",")
        if line:
            recs.append([f.strip() for f in line.strip("{} ").split(",")])
    return recs


def placeholder(version, row, rom_dir):
    name, mod, start, end, kind = row
    head = (f"#ifndef BWASSET_REWRITTEN\n#error \"asset {name}: this file must be one tools/port_assets.py rewrites: "
            f"meson setup --reconfigure\"\n#endif\n")
    if kind in PLAIN:
        n = (end - start) // PLAIN[kind]
        return head + "".join("    " + "0, " * min(16, n - i) + "\n" for i in range(0, n, 16))
    cols = asset_table.parse_layout(kind)
    _, stride = asset_table.offsets(cols)
    n = (end - start) // stride
    ptrs = [i for i, (k, _) in enumerate(cols) if k in ("ptr", "ref")]
    rom = rom_inc_fields(rom_dir / f"{name}.inc") if ptrs else None
    if rom is not None and (len(rom) != n or any(len(r) != len(cols) for r in rom)):
        sys.exit(f"{rom_dir / name}.inc: not {n} records of {len(cols)} fields")
    lines = []
    for r in range(n):
        f = [rom[r][i] if i in ptrs else "0" for i in range(len(cols))]
        lines.append("    " + (f[0] + "," if len(f) == 1 else "{ " + ", ".join(f) + " },"))
    return head + "\n".join(lines) + "\n"


def write_incs(rom_build, outdir, stamp):
    rom_build, outdir = Path(rom_build), Path(outdir)
    count = 0
    for version, sub in (("b_us", ""), ("w_us", "w_us")):
        rom_dir = rom_build / sub / "asset"
        if version != "b_us" and not rom_dir.is_dir():
            continue
        d = outdir / sub / "asset"
        d.mkdir(parents=True, exist_ok=True)
        for row in rows(version):
            text = placeholder(version, row, rom_dir)
            f = d / f"{row[0]}.inc"
            if not f.exists() or f.read_text() != text:
                f.write_text(text)
            count += 1
    Path(stamp).write_text(f"{count} placeholders\n")


# ---------------------------------------------------------------- the rows
def write_rows(out):
    lines = ["// tools/port_assets.py rows: each version's assets.tsv rows, which port/src/asset_load.c reads the ROM by",
             "// (module: -1 the ARM9 static module, N overlay N, -2 another module, which the loader refuses)",
             '#include "asset_rows.h"', ""]
    games = []
    for version, code in VERSIONS.items():
        rs = rows(version)
        if not rs:
            continue
        lines.append(f"static const BwAssetRow k_{version}[] = {{")
        for name, mod, start, end, kind in rs:
            m = -1 if mod == "main" else int(mod[2:]) if re.fullmatch(r"ov\d+", mod) else -2
            lines.append(f'    {{"{name}", {m}, 0x{start:08x}, 0x{end:08x}, "{kind}"}},')
        lines.append("};")
        games.append(f'    {{"{code}", k_{version}, {len(rs)}}},')
    lines += ["const BwAssetGame bwasset_games[] = {"] + games + ["    {0, 0, 0},", "};", ""]
    new = "\n".join(lines)
    out = Path(out)
    if not out.exists() or out.read_text() != new:
        out.write_text(new)


def main():
    a = sys.argv[1:]
    if a[:1] == ["list"]:
        for s in pc_sources():
            if includes_asset(s):
                print(s)
    elif a[:1] == ["src"] and len(a) == 4:
        write_src(*a[1:])
    elif a[:1] == ["incs"] and len(a) == 4:
        write_incs(*a[1:])
    elif a[:1] == ["rows"] and len(a) == 2:
        write_rows(a[1])
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
