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
    python3 tools/port_assets.py incs OUTDIR STAMP             # placeholders: OUTDIR/asset (Black), OUTDIR/w_us/asset
    python3 tools/port_assets.py rows OUT.c                    # the rows table for asset_load.c
    python3 tools/port_assets.py fnv [ROM_DIR]                 # port/asset_rows_fnv.tsv from the ROMs (default orig/)

    python3 tools/port_assets.py ptrs ROM_BUILD                # config/b_us/asset_ptrs.tsv from a ROM build's .inc files

`rows` writes each version's FNV-1a 64 of the bytes its rows read, the rows in order, from port/asset_rows_fnv.tsv (version,
game code, the rows' signature: a sha1 of their names and ranges, and the hash): asset_load.c refuses a ROM whose bytes hash
otherwise (a ROM of another revision with the same game code). `fnv` writes that file from the ROMs; a row added or moved
changes the signature, and `rows` stops until `fnv` is run again with the ROMs.

No ROM byte reaches any output, and no output needs a ROM: the placeholders hold 0s and symbol names (pointer fields
name a function or table of the game, as the ROM build's .inc does). Those names are config/b_us/asset_ptrs.tsv's (row,
record, field, the C text: names only), which `ptrs` writes from a ROM build when tools/asset.py --move adds a row with a
pointer field (White's .inc name the same symbols: `ptrs` checks it when White's ROM build is there).
"""
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import asset_table  # noqa: E402

PLAIN = {"u8": 1, "u16": 2, "u32": 4, "s8": 1, "s16": 2, "s32": 4, "fx16": 2, "fx32": 4}
VERSIONS = {"b_us": "IRBO", "w_us": "IRAO", "b2_us": "IREO", "w2_us": "IRDO"}
# the define each version's C is compiled with (Black's: none), in the order the registration's #if tests them
DEFINES = {"b2_us": "BLACK2", "w2_us": "WHITE2", "w_us": "WHITE", "b_us": None}
SUB = {"b_us": "", "w_us": "w_us", "b2_us": "b2_us", "w2_us": "w2_us"}   # each version's dir under the placeholders' and the ROM build's
PTR_BASE = {"b_us": "b_us", "w_us": "b_us", "b2_us": "b2_us", "w2_us": "b2_us"}   # whose asset_ptrs.tsv names the pointers
B2_ROOT = ROOT / "third_party/pokebw2"
B2_LIBS = ("dpw", "dsprot", "dwc", "dwcutil", "irc", "nitrowifi", "poke_net", "spl")   # port/meson.build's b2_args
AFTER = B2_ROOT / "include"   # (b2_args' -idirafter)
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
# The same by the table's name, when its C type is a plain one whose layout is not the row's (n8: the row's 4-byte value
# in a 1-byte slot, its low byte)
SYM_LAYOUT = {
    "kDriftveilPads": "n8,u8*4",   # port/include/port_route_map_game.h: u8[82][5] of the row's u32,u8*4 records
}


# pokebw2's types where they differ from Black's (its G3DSceneResourceSetup has no union: the DS's layout)
TYPE_LAYOUT_B2W2 = dict(TYPE_LAYOUT, G3DSceneResourceSetup=None)


def c_layout(decl, sym):
    types = TYPE_LAYOUT_B2W2 if B2_ROOT / "generated" in DIRS else TYPE_LAYOUT
    return types.get(ctype(decl)) or SYM_LAYOUT.get(sym)


# HeartGold's and SoulSilver's rows (one module, both games): tools/hg_assets.py writes them into the vendored tree
HG_ROWS = {"hg": "IPKE", "ss": "IPGE"}


def rows(version):
    t = ROOT / "config" / version / "assets.tsv"
    if version in HG_ROWS:
        t = ROOT / "third_party/pokeheartgold/port" / f"assets_{version}.tsv"
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
    cols = asset_table.parse_layout(kind.replace("w32", "ptr").replace("n8", "u8"))
    align = 1
    off = 0
    for k, _ in cols:
        sz = ptr if k in ("ptr", "ref") else asset_table.SIZES[k]
        off = (off + sz - 1) // sz * sz + sz
        align = max(align, sz)
    _, ds_stride = asset_table.offsets(asset_table.parse_layout(kind.replace("w32", "ptr").replace("n8", "u32")))
    return (off + align - 1) // align * align, ds_stride


def host_size(kind, start, end):
    host, ds = host_layout(kind)
    return (end - start) // ds * host


# ---------------------------------------------------------------- the sources
def inc_dirs(top):
    """The include dirs a source is compiled with (port/meson.build's order): Black's, or pokebw2's for its files."""
    if {B2_ROOT, ROOT / "port/extern_b2w2"} & set(Path(top).resolve().parents):   # (its stand-ins too: port_sources.py)
        return ([ROOT / "port/include", ROOT / "port/include_b2w2"] + [B2_ROOT / "lib" / x / "include" for x in B2_LIBS] +
                [B2_ROOT / "generated", B2_ROOT / "include"])
    return [ROOT / "port/include", ROOT / "include"]


DIRS = inc_dirs(ROOT / "src")   # the source being rewritten's (write_src, list)


LOCAL = {}    # local()'s answers: (dir of the includer, name, the include dirs) -> path or None (the same few headers, over and over)
RESOLVED = {}   # Path.resolve()'s answers (a lstat per path component each time)


def resolved(p):
    r = RESOLVED.get(p)
    if r is None:
        r = RESOLVED[p] = Path(p).resolve()
    return r


def local(path, inc):
    """The file `#include "inc"` in path names: beside it, else in the game's include dirs (port/meson.build's order)."""
    parent = Path(path).parent
    key = (parent, inc, tuple(DIRS))
    if key in LOCAL:
        return LOCAL[key]
    found = None
    for d in [parent] + DIRS:   # (os.path on strings: pathlib's is_file costs 10x the stat for the many misses)
        p = os.path.join(d, inc)
        if os.path.isfile(p):
            found = resolved(Path(p))
            break
    LOCAL[key] = found
    return found


HAS = {}   # includes_asset's answers (one DIRS per process): the sources share most headers


def includes_asset(path, seen=None):
    seen = set() if seen is None else seen
    path = resolved(Path(path))
    if path in HAS:
        return HAS[path]
    if path in seen or not path.exists():
        return False
    seen.add(path)
    text = path.read_text(errors="replace")
    HAS[path] = bool(INC.search(text)) or any(includes_asset(local(path, m[2]) or path.parent / m[2], seen)
                                              for m in QUOTE.finditer(text) if not m[2].startswith("asset/"))
    return HAS[path]


def pc_sources(b2w2=False):
    out = subprocess.run([sys.executable, str(ROOT / "tools/port_sources.py")] + (["--b2w2", str(B2_ROOT)] if b2w2 else []),
                         capture_output=True, text=True, check=True)
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


def file_stmt(blank, brace):
    """Where the file-scope declaration whose initialiser opens at brace starts (the index before it): after the last `;`,
    preprocessor line or function's closing `}` (one that ends its line) at depth 0, so a struct or union type's member
    list stays inside the declaration."""
    depth, stmt = 0, -1
    for c in re.finditer(r"[{};]|^[ \t]*#[^\n]*", blank[:brace], re.M):
        depth += {"{": 1, "}": -1}.get(c[0], 0)
        if depth == 0 and (c[0] == ";" or c[0].lstrip().startswith("#") or
                           c[0] == "}" and re.match(r"[ \t]*\n", blank[c.end():])):
            stmt = c.end() - 1
    return stmt


def block_stmt(blank, brace):
    """Where the block-scope declaration whose initialiser opens at brace starts (the index before it): the last `;`, `{`
    or `}` before it at its depth, a struct, union or enum type's member list being inside the declaration."""
    i = brace - 1
    while i >= 0:
        if blank[i] == "}":
            depth, j = 0, i
            while j >= 0:
                depth += {"}": 1, "{": -1}.get(blank[j], 0)
                if depth == 0:
                    break
                j -= 1
            if not re.search(r"\b(?:struct|union|enum)\s*\w*\s*$", blank[:j]):
                return i
            i = j
        elif blank[i] in ";{":
            return i
        i -= 1
    return -1


def ctype(spec):
    """The type name of a declaration's specifiers (static const Foo -> Foo)."""
    words = [w for w in re.findall(r"\w+", spec) if w not in ("static", "const", "volatile", "extern", "struct", "union")]
    return words[-1] if words else ""


OUTDIR = None   # write_src: the directory of the generated source (local headers are named relative to it)


def shown(p):
    """A path as #line gives it: relative to the build dir meson runs the generator in, as its compile lines name sources."""
    return os.path.relpath(p).replace(os.sep, '/')   # (Windows: a backslash in a #line string is an escape)


def rewrite(path, deps, found, top=True):
    """The text of path for the build dir: asset declarations without const, .c includes inlined, local headers absolute."""
    path = resolved(Path(path))
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
        if depth_at(text, brace) == 0:   # file scope: a struct or union type's member list is inside the declaration
            stmt = file_stmt(blank, brace)
        else:
            stmt = max(block_stmt(blank, brace), pre.rfind("\n#"))
            if pre.rfind("\n#") == stmt:
                stmt = pre.find("\n", stmt + 1)
        while blank[stmt + 1].isspace():   # the declaration from its first word (comments before it stay)
            stmt += 1
        tail_at = max(stmt + 1, blank.rfind("}", stmt + 1, brace))   # (comments in a struct type's member list are fine)
        if blank[tail_at:brace] != text[tail_at:brace]:
            sys.exit(f"{path}: asset {m[1]}: a comment inside its declaration")
        decl = text[stmt + 1:brace]
        DECL = r"(\w+)\s*((?:\[[^\]]*\]\s*)*)((?:ATTRIBUTE_ALIGN\(\w+\)\s*)?)=\s*$"   # name, dims, alignment (kept)
        d = re.search(DECL, decl)
        if not d and not blank[stmt + 1:brace].strip() and blank[:stmt + 1].rstrip().endswith("{"):
            # `X_raw = { {#include ...} }`: the table is the first member of a union or struct; the declaration is the outer one
            # (a file-scope one: it starts after the last `;`, preprocessor line or function's `}` at depth 0, so the
            # union's own member list is inside it)
            brace = len(blank[:stmt + 1].rstrip()) - 1
            stmt = file_stmt(blank, brace)
            while blank[stmt + 1].isspace():
                stmt += 1
            decl = text[stmt + 1:brace]
            d = re.search(DECL, decl)
            if d and depth_at(text, stmt + 1) != 0:
                d = None
        if not d:
            sys.exit(f"{path}: asset {m[1]}: no declaration before its #include")
        sym = d[1]
        bare = re.sub(r"\bconst\b", "", decl[:d.start(1)])
        # in .data even as zeros (not .bss: an overlay load zeroes that); Mach-O's name has the segment: BWASSET_DATA below
        attr = d[3] + " __attribute__((section(BWASSET_DATA)))"
        if depth_at(text, stmt + 1) != 0:
            # a static in a function: the table moves before the function under a name of its own (a constructor at the
            # end can't name it there), and the function's uses reach it by a #define until the function ends
            # (an automatic array: the same table, and the declaration becomes a copy of it, made where the original's
            # initialiser ran: a declaration too, so the declarations after it stay before the statements)
            auto = not re.search(r"\bstatic\b", decl)
            close = re.match(r"\s*\};", text[m.end():])
            init, tail = m[0] + "\n", m.end() + close.end() if close else None
            if not close and auto:   # more of the initialiser after the table (a struct's other fields): all of it moves
                depth = 0
                for c in re.finditer(r"[{}]", blank[brace:]):
                    depth += 1 if c[0] == "{" else -1
                    if depth == 0:
                        close = re.compile(r"\}\s*;").match(text, brace + c.start())
                        init, tail = text[brace + 1:brace + c.start()], close.end() if close else None
                        break
            if not close:
                sys.exit(f"{path}: asset {m[1]}: no `}};` after its #include")
            fstart, fend = enclosing(blank, stmt + 1)
            store = f"bwasset_local_{sym}_{stmt + 1}"
            gone = text[stmt + 1:tail]
            edits.append((fstart, fstart, f"static {bare.replace('static', '')}{store} {d[2]}{attr} = {{\n{init}}};\n" +
                          ("" if auto else f"#define {sym} {store}\n") + f"#line {text.count(chr(10), 0, fstart) + 1}\n"))
            copy = (f"__typeof__({store}) {sym}; int bwasset_copy_{stmt + 1} __attribute__((unused)) = "
                    f"(__builtin_memcpy(&{sym}, &{store}, sizeof({sym})), 0);" if auto else "")
            edits.append((stmt + 1, tail, copy + "\n" * gone.count("\n")))
            if not auto:
                edits.append((fend, fend, f"\n#undef {sym}\n#line {text.count(chr(10), 0, fend) + 1}\n"))
            found.append((m[1], store, c_layout(bare, sym)))
            continue
        if re.search(r"\bstatic\b", decl):
            store = sym
            edits.append((stmt + 1, brace, bare + sym + " " + d[2] + attr + " ="))
            # its forward declarations in the file lose const too
            for f in re.finditer(rf"(?:^|(?<=[;}}]))([^;{{}}#=()]*\b{sym}\s*(?:\[[^\]]*\]\s*)*;)", blank, re.M):
                if f.start(1) < stmt + 1 and "const" in f[1] and depth_at(text, f.start(1)) == 0:
                    edits.append((f.start(1), f.end(1), re.sub(r"\bconst\b", "", f[1])))
        else:
            # a global the headers declare const: the storage is a static of another name, the global its alias (Mach-O has
            # no aliases: the storage is the global itself there, by its assembler name; BWASSET_GLOBAL below)
            store = "bwasset_rw_" + sym
            edits.append((stmt + 1, brace, f"BWASSET_STATIC {bare}{store} {d[2]} BWASSET_GLOBAL({sym}){attr} ="))
            close = re.match(r"\s*\};", text[m.end():])
            if not close:
                sys.exit(f"{path}: asset {m[1]}: no `}};` after its #include")
            at = m.end() + close.end()
            q = "const " if re.search(r"\bconst\b", decl[:d.start(1)]) else ""
            edits.append((at, at, f' extern {q}__typeof__({store}) {sym} BWASSET_ALIAS({store});'))
        found.append((m[1], store, c_layout(bare, sym)))
    for m in QUOTE.finditer(text):
        inc = m[2]
        if inc.startswith("asset/"):
            continue
        target = local(path, inc)
        if target is None:
            continue
        line_end = text.find("\n", m.end())
        line_end = len(text) if line_end < 0 else line_end
        if inc.endswith(".c") or includes_asset(target):   # a .c, or a local header with an asset: inlined
            body = rewrite(target, deps, found, top=False)
            nl = text.count("\n", 0, line_end) + 2  # the line after the #include
            edits.append((m.start(), line_end, f'#line 1 "{shown(target)}"\n{body}\n#line {nl} "{shown(path)}"'))
        elif AFTER not in target.parents:   # (a header of an -idirafter dir stays: the system's of that name come first)
            edits.append((m.start(1), m.end(), f'{m[1]}"{os.path.relpath(target, OUTDIR).replace(os.sep, "/")}"'))   # (relative to the generated file: no worktree path in the text, so ccache keys agree across worktrees)
    edits.sort()
    out, pos = [], 0
    for s, e, new in edits:
        if s < pos:
            sys.exit(f"{path}: overlapping rewrites at {s}")
        out += [text[pos:s], new]
        pos = e
    out.append(text[pos:])
    body = "".join(out)
    return (f'#define BWASSET_REWRITTEN 1\n{PREAMBLE}#line 1 "{shown(path)}"\n' + body) if top else body


# The rewritten file's spellings that differ on Mach-O (macOS, iOS): a section name has the segment, and there are no aliases
PREAMBLE = """#ifdef __APPLE__
#define BWASSET_DATA "__DATA,__data"
#define BWASSET_STATIC
#define BWASSET_GLOBAL(sym) __asm__("_" #sym)
#define BWASSET_ALIAS(store)
#else
#define BWASSET_DATA ".data"
#define BWASSET_STATIC static
#define BWASSET_GLOBAL(sym)
#define BWASSET_ALIAS(store) __attribute__((alias(#store)))
#endif
"""


def write_src(src, out, depfile):
    global DIRS, OUTDIR
    DIRS = inc_dirs(src)
    OUTDIR = Path(out).resolve().parent
    deps, found = [], []
    text = rewrite(src, deps, found)
    reg = ["", f'#line 1 "{shown(Path(__file__).resolve())}"',   # (as the other markers: ccache lstat()s a #line file name from the build dir, and gave up on the repo-relative one: "could not read or parse input file", every asset source uncached)
   
           "void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);"]
    # one block per version: the one the compile defines (pokebw2's C BLACK2 or WHITE2, White's WHITE, else Black's).
    # A row of no version of the block's is another game's, in an #if branch this compile skips (port/include's Black 2 /
    # White 2 tables in Black's build); a row of the version missing from its assets.tsv has no placeholder file, so its
    # #include fails
    for i, version in enumerate(DEFINES):
        rng = {name: (kind, start, end) for name, mod, start, end, kind in rows(version)}
        reg.append(f"#{'el' if i else ''}if defined({DEFINES[version]})" if DEFINES[version] else "#else")
        mine = [f for f in found if f[0] in rng]
        for name, sym, layout in mine:
            kind, start, end = rng[name]
            want = host_size(layout or kind, start, end)
            # a plain copy may fill the start of a bigger table (an initialiser shorter than its type: the rest is zeros)
            op = "==" if layout else ">="
            reg.append(f'_Static_assert(sizeof({sym}) {op} {want}, "asset {name}: {sym} is not its row\'s {want} bytes '
                       f'on the host (a pointer in its type? tools/port_assets.py TYPE_LAYOUT)");')
        reg.append("__attribute__((constructor)) static void bwasset_register_(void) {")
        for name, sym, layout in mine:
            lay = f'"{layout}"' if layout else "0"
            reg.append(f'    PORT_AssetAdd("{VERSIONS[version]}", "{name}", (void *)&{sym}, sizeof({sym}), {lay});')
        reg.append("}")
    reg.append("#endif")
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


def ptrs_file(version):
    return ROOT / "config" / PTR_BASE[version] / "asset_ptrs.tsv"


PTRS = ptrs_file("b_us")


def read_ptrs(path=PTRS):
    """{(row name, record, field): the C text} of every pointer field (asset_ptrs.tsv: a line per row and field, the
    records' texts space-separated: a line per record would be a run of record numbers the leak check reads as data)."""
    out = {}
    if path.exists():
        for line in path.read_text().splitlines()[1:]:
            name, col, texts = line.split("\t")
            for rec, text in enumerate(texts.split(" ")):
                out[(name, rec, int(col))] = text
    return out


def write_ptrs(rom_build):
    """Black's and Black 2's asset_ptrs.tsv from the ROM build's .inc, each when there (White's and White 2's, when
    there, must name the same)."""
    rom_build = Path(rom_build)
    for base, other in (("b_us", "w_us"), ("b2_us", "w2_us")):
        if not (rom_build / SUB[base] / "asset").is_dir():
            continue
        lines = ["name\tfield\tc (each record's)"]
        for row in rows(base):
            name, kind = row[0], row[4]
            if kind in PLAIN:
                continue
            cols = asset_table.parse_layout(kind)
            ptrs = [i for i, (k, _) in enumerate(cols) if k in ("ptr", "ref")]
            if not ptrs:
                continue
            recs = rom_inc_fields(rom_build / SUB[base] / "asset" / f"{name}.inc")
            w = rom_build / SUB[other] / "asset" / f"{name}.inc"
            if w.exists() and [[r[i] for i in ptrs] for r in rom_inc_fields(w)] != [[r[i] for i in ptrs] for r in recs]:
                sys.exit(f"{w}: its pointers name other symbols than {base}'s (asset_ptrs.tsv has one list)")
            lines += [f"{name}\t{i}\t" + " ".join(r[i] for r in recs) for i in ptrs]
        ptrs_file(base).write_text("\n".join(lines) + "\n")


def placeholder(version, row, ptr_text):
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
    missing = [(r, i) for r in range(n) for i in ptrs if (name, r, i) not in ptr_text]
    if missing:
        sys.exit(f"asset {name}: {ptrs_file(version).relative_to(ROOT)} lacks its pointer fields (record, field) {missing[:4]}: "
                 "python3 tools/port_assets.py ptrs build")
    lines = []
    for r in range(n):
        f = [ptr_text[(name, r, i)] if i in ptrs else "0" for i in range(len(cols))]
        lines.append("    " + (f[0] + "," if len(f) == 1 else "{ " + ", ".join(f) + " },"))
    return head + "\n".join(lines) + "\n"


def write_incs(outdir, stamp):
    outdir = Path(outdir)
    count = 0
    for version, sub in SUB.items():
        d = outdir / sub / "asset"
        d.mkdir(parents=True, exist_ok=True)
        ptr_text = read_ptrs(ptrs_file(version))
        for row in rows(version):
            text = placeholder(version, row, ptr_text)
            f = d / f"{row[0]}.inc"
            if not f.exists() or f.read_text() != text:
                f.write_text(text)
            count += 1
    Path(stamp).write_text(f"{count} placeholders\n")


# ---------------------------------------------------------------- the rows
FNV_FILE = ROOT / "port/asset_rows_fnv.tsv"
ROM_NAME = {"hg": "baserom_hg_us.nds", "ss": "baserom_ss_us.nds"}   # (the rest: baserom_<version>.nds)


def rows_sig(rs):
    """The rows' names, modules and ranges (what the hash depends on), as a short sha1."""
    import hashlib
    return hashlib.sha1("".join(f"{n} {m} {s:x} {e:x}\n" for n, m, s, e, _ in rs).encode()).hexdigest()[:16]


def read_fnv():
    out = {}
    if FNV_FILE.exists():
        for line in FNV_FILE.read_text().splitlines():
            if line.strip() and not line.startswith("#"):
                version, code, sig, h = line.split("\t")
                out[version] = (code, sig, int(h, 16))
    return out


def rows_fnv(rom, rs):
    """asset_load.c's RowsHash: FNV-1a 64 over each row's bytes in its module (main: the ARM9 decompressed; ovN: overlay
    N decompressed), the rows in order; a row of another module (the ARM9i's ltd_autoload_0: modcrypted, no PC table
    includes it, the loader refuses it) is left out."""
    import struct
    import rom_modules
    mods = {}

    def module(m):
        if m not in mods:
            if m == "main":
                mods[m] = (struct.unpack_from("<I", rom, 0x28)[0], rom_modules.arm9(rom))
            elif re.fullmatch(r"ov\d+", m):   # (rom_modules.overlays' reading, only the one asked for)
                fat, _, ovt, ovt_size = struct.unpack_from("<4I", rom, 0x48)
                mods[m] = (0, b"")
                for e in range(ovt, ovt + ovt_size, 32):
                    ov_id, ram = struct.unpack_from("<II", rom, e)
                    if ov_id == int(m[2:]):
                        file_id, word = struct.unpack_from("<II", rom, e + 24)
                        start, stop = struct.unpack_from("<II", rom, fat + 8 * file_id)
                        data = rom[start:stop]
                        mods[m] = (ram, rom_modules.blz_decompress(data[:word & 0xFFFFFF]) if word >> 24 & 1 else data)
        return mods[m]

    h = 0xCBF29CE484222325
    for name, mod, start, end, _ in rs:
        if mod != "main" and not re.fullmatch(r"ov\d+", mod):
            continue
        base, image = module(mod)
        if start < base or end > base + len(image) or end < start:
            sys.exit(f"asset {name}: the ROM has no {mod} bytes {start:08x}-{end:08x}")
        for b in image[start - base:end - base]:
            h = ((h ^ b) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return h


def write_fnv(rom_dir):
    """port/asset_rows_fnv.tsv: every version with rows whose ROM is in rom_dir (the others keep their line)."""
    old = read_fnv()
    lines = ["# tools/port_assets.py fnv: each version's rows (version, game code, the rows' signature, FNV-1a 64 of their ROM bytes)"]
    for version, code in list(VERSIONS.items()) + list(HG_ROWS.items()):
        rs = rows(version)
        if not rs:
            continue
        rom = Path(rom_dir) / ROM_NAME.get(version, f"baserom_{version}.nds")
        if rom.exists():
            entry = (code, rows_sig(rs), rows_fnv(rom.read_bytes(), rs))
            print(f"{version} {code}: {len(rs)} rows, fnv {entry[2]:016x}")
        elif version in old:
            entry = old[version]
            print(f"{version} {code}: no {rom}: kept")
        else:
            sys.exit(f"{rom}: not there, and {FNV_FILE.relative_to(ROOT)} has no line for {version}")
        lines.append(f"{version}\t{entry[0]}\t{entry[1]}\t{entry[2]:016x}")
    FNV_FILE.write_text("\n".join(lines) + "\n")


def write_rows(out):
    fnv = read_fnv()
    lines = ["// tools/port_assets.py rows: each version's assets.tsv rows, which port/src/asset_load.c reads the ROM by",
             "// (module: -1 the ARM9 static module, N overlay N, -2 another module, which the loader refuses)",
             '#include "asset_rows.h"', ""]
    games = []
    for version, code in list(VERSIONS.items()) + list(HG_ROWS.items()):
        rs = rows(version)
        if not rs:
            continue
        lines.append(f"static const BwAssetRow k_{version}[] = {{")
        for name, mod, start, end, kind in rs:
            m = -1 if mod == "main" else int(mod[2:]) if re.fullmatch(r"ov\d+", mod) else -2
            lines.append(f'    {{"{name}", {m}, 0x{start:08x}, 0x{end:08x}, "{kind}"}},')
        lines.append("};")
        if fnv.get(version, (None, None))[:2] != (code, rows_sig(rs)):
            sys.exit(f"{FNV_FILE.relative_to(ROOT)}: no line for {version}'s rows as they are now (a row added or moved): "
                     "python3 tools/port_assets.py fnv, with the ROMs")
        games.append(f'    {{"{code}", k_{version}, {len(rs)}, 0x{fnv[version][2]:016x}ULL}},')
    lines += ["const BwAssetGame bwasset_games[] = {"] + games + ["    {0, 0, 0, 0},", "};", ""]
    new = "\n".join(lines)
    out = Path(out)
    if not out.exists() or out.read_text() != new:
        out.write_text(new)


def main():
    a = sys.argv[1:]
    if a[:1] == ["list"] and a[1:] in ([], ["b2w2"]):
        global DIRS
        DIRS = inc_dirs(B2_ROOT / "src" if a[1:] else ROOT / "src")
        for s in pc_sources(b2w2=bool(a[1:])):
            if includes_asset(s):
                print(s)
    elif a[:1] == ["src"] and len(a) == 4:
        write_src(*a[1:])
    elif a[:1] == ["incs"] and len(a) == 3:
        write_incs(*a[1:])
    elif a[:1] == ["ptrs"] and len(a) == 2:
        write_ptrs(a[1])
    elif a[:1] == ["rows"] and len(a) == 2:
        write_rows(a[1])
    elif a[:1] == ["fnv"] and len(a) <= 2:
        write_fnv(a[1] if len(a) == 2 else ROOT / "orig")
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
