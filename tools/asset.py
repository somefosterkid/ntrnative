#!/usr/bin/env python3
"""The game's assets (graphics, palettes, fonts, data tables) stay out of the C: the build takes their bytes from the base ROM.

A C file keeps the array and includes its values, which the build writes from the extracted ROM:

    static const u8 data_0209e8cc[] = {
    #include "asset/bmp_cursor_pixels.inc"
    };

config/<version>/assets.tsv lists each asset: its name, module, start and end address, and its type. The type of a plain
array is the type of its elements (u8, u16, u32, s8, s16, s32, little-endian as in the ROM); a table of records (structs,
arrays of arrays, names mixed in) gives the fields of a record instead (tools/asset_table.py: `u16,u8`, `u8[255=NAME]*4`,
`ptr`). configure.py adds a build step per row that writes build/asset/<name>.inc with this tool (another version's to
build/<version>/asset, from its own ROM at its own address), and the compiler finds it there (build/ is the last include
directory).

    python3 tools/asset.py --move src/system/bmp_cursor.c data_0209e8cc bmp_cursor_pixels   # from C values to a row
    python3 tools/asset.py main 0x0209e8cc 0x0209e8ec u8 build/asset/bmp_cursor_pixels.inc   # what ninja runs

--move takes an array or table whose values are in the C: it checks that they are the ROM's at the symbol's address
(symbols.txt of the module whose delinks.txt lists the file), replaces them with the #include and adds the row.
Rerun configure.py after it (ninja does when assets.tsv changes).

Black 2 and White 2 (--version b2_us or w2_us; docs/black2-white2.md): the C is third_party/pokebw2's, its delinks.txt
and symbols.txt that tree's config/, and one C file serves both games, so a move adds a row to config/b2_us/assets.tsv
and one to config/w2_us/assets.tsv, each at its version's address of the symbol (none for a version whose symbols lack
it). Both ROMs' bytes must give the same text, else nothing is changed. The port's build writes build/<version>/asset
from the player's ROM (tools/rom_modules.py --assets orig --version b2_us).

    python3 tools/asset.py --version b2_us --move third_party/pokebw2/src/system/poke_icon.c sPokeIconPalettes \
        poke_icon_palettes

Two fallbacks of a table move: a table without a symbol (a static local) is found by its values, packed as u32, u16 or
u8, at the one place of each version's module that holds them (several places: not moved); and a file-scope static
const table without pointers whose layout the inference can not fit (bitfields, packed structs, records of different
lengths, floats, expressions without a value) becomes a union of its type with the ROM's bytes up to the next symbol:

    static const union { u8 raw[52]; T t[4]; } sX_raw = { {
    #include "asset/foo_sX.inc"
    } };
    _Static_assert(sizeof(sX_raw.t) <= sizeof(sX_raw.raw), "sX: the ROM's bytes are its layout");
    #define sX (sX_raw.t)

(the port's compilers lay out a struct of fixed-width integers as MWCC does; the row is type u8).
"""
import argparse
import re
import signal
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asset_table  # noqa: E402
import c_consts  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
TYPES = {"u8": "B", "u16": "H", "u32": "I", "s8": "b", "s16": "h", "s32": "i"}
ALIAS = {"fx16": "s16", "fx32": "s32"}  # the same elements under their C names


def module_dir(version, mod):
    """The module's config directory: main, an overlay (ovNNN) or an autoload (autoload_N, ltd_autoload_N)."""
    return asset_table.arm9_dir(version) / ("" if mod == "main" else f"overlays/{mod}" if mod.startswith("ov") else mod)


def module_image(version, mod):
    """The module's extracted binary."""
    if mod == "main":
        return ROOT / "extract" / version / "arm9/arm9.bin"
    if mod.startswith("ov"):
        return ROOT / "extract" / version / f"arm9_overlays/{mod}.bin"
    return next(p for p in (ROOT / "extract" / version).glob(f"*/{mod}.bin"))


def rom_bytes(version, mod, start, end):
    """The ROM's bytes in [start, end) of a module."""
    base = rom_base(version, mod)
    return module_image(version, mod).read_bytes()[start - base:end - base]


def rom_base(version, mod):
    """The address of a module image's first byte."""
    header = (module_dir(version, mod) / "delinks.txt").read_text().split("\n\n")[0]
    return min(int(m[2], 16) for m in re.finditer(r"(\.\w+)\s+start:(0x\w+)", header) if m[1] != ".bss")


def values(version, mod, start, end, kind):
    """The ROM's elements of type kind in [start, end) of a module."""
    data = rom_bytes(version, mod, start, end)
    fmt = TYPES[kind]
    if (end - start) % struct.calcsize(fmt):
        sys.exit(f"{end - start:#x} bytes is not a whole number of {kind}")
    return list(struct.unpack(f"<{(end - start) // struct.calcsize(fmt)}{fmt}", data))


def inc_text(vals, kind):
    per_line = 16 if kind[1:] == "8" else 8
    hexa = kind.startswith("u")
    return "".join("    " + " ".join((f"{v:#04x}," if hexa else f"{v},") for v in vals[i:i + per_line]) + "\n"
                   for i in range(0, len(vals), per_line))


def asset_text(version, mod, start, end, kind):
    """One assets.tsv row's .inc text: plain elements of a type, or the records of a table."""
    if kind in TYPES:
        return inc_text(values(version, mod, start, end, kind), kind)
    return asset_table.render(rom_bytes(version, mod, start, end), kind, asset_table.symbol_names(version, mod))


def write_asset(version, mod, start, end, kind, out):
    """Writes one assets.tsv row's .inc."""
    try:
        text = asset_text(version, mod, start, end, kind)
    except ValueError as e:
        sys.exit(f"{out.name} ({version} {mod} {start:#010x}-{end:#010x}): {e}")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(text)


def module_of(version, c_file):
    """The module whose delinks.txt lists a C file."""
    c_file = c_file.removeprefix(asset_table.VENDORED + "/") if version in TWIN else c_file   # (its delinks' name)
    mods = ["main" if p.parent.name == "arm9" else p.parent.name
            for p in asset_table.arm9_dir(version).rglob("delinks.txt") if f"\n{c_file}:\n" in p.read_text()]
    if len(mods) != 1 or not re.match(r"main$|ov\d+$", mods[0]):
        sys.exit(f"{c_file} is in the delinks.txt of {mods or 'no module'}: one, main or an overlay, is read")
    return mods[0]


def symbol_addr(version, mod, symbol, c_file=None):
    """The symbol's address; of a name two files' statics share (ov142's sItemNameOrder), the one in c_file's sections."""
    text = (module_dir(version, mod) / "symbols.txt").read_text()
    addrs = [int(a, 16) for a in re.findall(rf"^{symbol} kind:\S+ addr:(0x\w+)", text, re.M)]
    if len(addrs) > 1 and c_file:
        name = c_file.removeprefix(asset_table.VENDORED + "/") if version in TWIN else c_file
        block = (module_dir(version, mod) / "delinks.txt").read_text().partition(f"\n{name}:\n")[2].partition("\n\n")[0]
        spans = [(int(s, 16), int(e, 16)) for s, e in re.findall(r"start:(0x\w+) end:(0x\w+)", block)]
        addrs = [a for a in addrs if any(s <= a < e for s, e in spans)] or addrs
    return addrs[0] if addrs else None


def section_end(version, mod, c_file, addr):
    """The end of c_file's section (delinks.txt) that holds addr; None when none does."""
    name = c_file.removeprefix(asset_table.VENDORED + "/") if version in TWIN else c_file
    block = (module_dir(version, mod) / "delinks.txt").read_text().partition(f"\n{name}:\n")[2].partition("\n\n")[0]
    return next((int(e, 16) for s, e in re.findall(r"start:(0x\w+) end:(0x\w+)", block) if int(s, 16) <= addr < int(e, 16)), None)


def locate(version, c_file, symbol):
    """(module, address) of a symbol of a C file: the module whose delinks.txt lists the file."""
    mod = module_of(version, c_file)
    addr = symbol_addr(version, mod, symbol, c_file)
    if addr is None:
        sys.exit(f"{symbol} is not in {mod}'s symbols.txt")
    return mod, addr


def find_by_bytes(version, mod, recs, value):
    """(address, size) of the one place in a module that holds a table's values packed as u32, u16 or u8 (aligned): the
    place of a table without a symbol (a static local). Exits when no width or more than one place fits."""
    vals = []
    for t in (t for r in recs for t in r):
        v = int(t.rstrip("uUlL"), 0) if asset_table.NUM.fullmatch(t) else value(t)
        if v is None:
            sys.exit(f"not in {mod}'s symbols.txt, and {t!r} has no value to search its bytes by")
        vals.append(v)
    image = module_image(version, mod).read_bytes()
    base = rom_base(version, mod)
    hits = []
    for w, f in ((4, "I"), (2, "H"), (1, "B")):
        data = struct.pack(f"<{len(vals)}{f}", *[v & ((1 << 8 * w) - 1) for v in vals])
        i = image.find(data)
        while i >= 0:
            hits += [(base + i, len(data))] if (base + i) % w == 0 else []
            i = image.find(data, i + 1)
    if len(hits) != 1 or len(vals) < 4:
        sys.exit(f"not in {mod}'s symbols.txt, and its {len(vals)} values are at {len(hits)} places of {version}'s {mod} "
                 "(one is needed)")
    return hits[0]


TWIN = {"b2_us": "w2_us", "w2_us": "b2_us"}   # Black 2 and White 2: one C, a row in each version's assets.tsv


def rows(version, c_file, symbol, mod, start, end, kind, find=None):
    """[(version, mod, start, end)] of a moved table: its own, and the twin version's when that has the symbol (or
    find(twin, module) finds it) and its ROM's bytes there give the same text."""
    out = [(version, mod, start, end)]
    twin = TWIN.get(version)
    if twin and find:
        tmod = module_of(twin, c_file)
        tstart = find(twin, tmod)[0]
    elif not twin or not re.search(rf"^{symbol} kind:", "".join(p.read_text() for p in
                                                              asset_table.arm9_dir(twin).rglob("symbols.txt")), re.M):
        return out
    else:
        tmod, tstart = locate(twin, c_file, symbol)
    if asset_text(twin, tmod, tstart, tstart + end - start, kind) != asset_text(version, mod, start, end, kind):
        sys.exit(f"{symbol}: {twin}'s bytes at {tstart:#010x} are not {version}'s: not moved (an #if in the table?)")
    return out + [(twin, tmod, tstart, tstart + end - start)]


def add_row(version, name, mod, start, end, kind):
    table = ROOT / "config" / version / "assets.tsv"
    rows = table.read_text() if table.exists() else "name\tmodule\tstart\tend\ttype\n"
    table.parent.mkdir(parents=True, exist_ok=True)
    table.write_text(rows + f"{name}\t{mod}\t{start:#010x}\t{end:#010x}\t{kind}\n")


def body_span(text, symbol):
    """(start, end) of the text between the braces of symbol's initialiser (the first one not moved yet: static locals
    of one name), and the line it is declared on."""
    first = None
    for m in re.finditer(rf"^(?:\}}\s*)?[^\n;{{}}()]*\b{symbol}\s*(?:\[[^\]]*\]\s*)*(?:ATTRIBUTE_ALIGN\(\d+\)\s*)?=\s*\{{", text, re.M):
        depth, i = 1, m.end()
        while depth:
            c = text[i]
            depth += (c == "{") - (c == "}")
            i += 1
        first = first or (m.end(), i - 1)
        if '#include "asset/' not in text[m.end():i]:
            return m.end(), i - 1
    return first


RAW_DECL = r"^(static\s+const\s+)([^=;{{}}()]*?)\b{sym}\s*((?:\[[^\]]*\]\s*)+)(ATTRIBUTE_ALIGN\(\d+\)\s*)?=\s*\{{"


def move_raw(version, c_file, symbol, name, text, span, mod, start, syms, why):
    """A table whose layout the inference can not fit: its type in a union with the ROM's bytes up to the next symbol
    (the module docstring). Only a file-scope static const array that holds no pointer."""
    m = next((m for m in re.finditer(RAW_DECL.format(sym=re.escape(symbol)), text, re.M) if m.end() == span[0]), None)
    if not m:
        sys.exit(f"{why}; not a file-scope static const array: by hand")
    body = text[span[0]:span[1]]
    if re.search(r"^[ \t]*#", body, re.M):
        sys.exit(f"{why}; #if in the table: split it by hand")
    names = {x for v in syms.values() for x in v}
    if "*" in m[2] or "&" in body or "*" in asset_table.strip_comments(body) or (names | {"NULL"}) & set(re.findall(r"[A-Za-z_]\w*", asset_table.strip_comments(body))):
        sys.exit(f"{why}; it may hold pointers: by hand")
    decls = list(re.finditer(rf"^[ \t]*(?:static\s+|extern\s+)?(?:const\s+)?(?!return\b)\w+(?:\s+const)?[\s*]+{symbol}\s*"
                             rf"(?:\[[^\]]*\]\s*)+(?:ATTRIBUTE_ALIGN\(\d+\)\s*)?[=;]", text, re.M))
    fwd = [d for d in decls if d[0].endswith(";")]   # a declaration before the definition: the union goes there
    if re.search(rf"(?:\.|->)\s*{symbol}\b", text) or len(decls) - len(fwd) != 1 or len(fwd) > 1 or \
            fwd and fwd[0].start() > m.start():
        sys.exit(f"{why}; {symbol} is also a field's name or defined twice (an #if?): by hand")
    if fwd and version not in TWIN:   # MWCC lays out data in the order of the definitions: the union can't move up
        sys.exit(f"{why}; declared before its definition, and MWCC keeps the definitions' order: by hand")
    after = sorted([a for a in syms if a > start] + [e for e in [section_end(version, mod, c_file, start)] if e])
    if not after or after[0] - start > 0x4000:
        sys.exit(f"{why}; no next symbol within 16 KB to end its bytes: by hand")
    end = after[0]
    data = rom_bytes(version, mod, start, end)
    if any(0x02000000 <= struct.unpack_from("<I", data, i)[0] < 0x02800000 for i in range(-start % 4, len(data) - 3, 4)):
        sys.exit(f"{why}; its bytes hold values that look like pointers: by hand")
    dims = re.findall(r"\[([^\]]*)\]", m[3])
    first = dims[0].strip() or str(c_consts.consts(c_file, version).nelems(symbol))
    todo = rows(version, c_file, symbol, mod, start, end, "u8")
    close = text.index(";", span[1])
    fits = (f"_Static_assert(sizeof({symbol}_raw.t) <= sizeof({symbol}_raw.raw), \"{symbol}: the ROM's bytes are its layout\");"
            if version in TWIN else   # (MWCC has no _Static_assert: an array of -1 elements fails the same way)
            f"typedef char {symbol}_fits[sizeof({symbol}_raw.t) <= sizeof({symbol}_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout")
    decl = (f"static const union {{ u8 raw[{end - start}]; {m[2].strip()} t[{first}]{''.join(f'[{d}]' for d in dims[1:])}; }} "
            f"{symbol}_raw {m[4] or ''}= {{ {{\n#include \"asset/{name}.inc\"\n}} }};\n"
            f"{fits}\n"
            f"#define {symbol} ({symbol}_raw.t)")
    f = fwd[0] if fwd else None
    (ROOT / c_file).write_text(text[:m.start()] + decl + text[close + 1:] if not f else
                               text[:f.start()] + decl + text[f.end():m.start()] + text[close + 1:].removeprefix("\n"))
    for v, md, s, e in todo:
        add_row(v, name, md, s, e, "u8")
    print(f"{c_file}: {symbol} now a union with asset/{name}.inc ({mod} {start:#010x}-{end:#010x}, u8; {why})")


def move_table(version, c_file, symbol, name):
    """Replaces a table's values (records, struct initialisers, names mixed in) with the #include of a new asset."""
    text = (ROOT / c_file).read_text()
    span = body_span(text, symbol)
    if not span:
        sys.exit(f"no initialised {symbol} in {c_file}")
    mod = module_of(version, c_file)
    addr = symbol_addr(version, mod, symbol, c_file)
    syms = asset_table.symbol_names(version, mod)
    value = c_consts.consts(c_file, version).value
    find = None
    try:
        recs = asset_table.parse_init("{" + text[span[0]:span[1]] + "}", value)
        if addr is None:   # a static local: the one place that holds its values, in each version
            start, size = find_by_bytes(version, mod, recs, value)
            find = lambda v, md: find_by_bytes(v, md, recs, value)
            spec, stride = asset_table.infer(recs, rom_bytes(version, mod, start, start + size), syms, size, value)
        start = addr if addr is not None else start
        for named_only in (False, True) if addr is not None else ():   # the next symbol; else the next with a real name (data_0208f91e splits inside a table)
            after = sorted(a for a, ns in syms.items() if a > start and not (named_only and all(AUTO_NAME.match(n) for n in ns)))
            ends = after[:1] + [e for e in [section_end(version, mod, c_file, start)] if e]   # (the file's section ends it too)
            region = (min(ends) if ends else start + 0x10000) - start
            try:
                spec, stride = asset_table.infer(recs, rom_bytes(version, mod, start, start + region), syms, region, value)
                break
            except asset_table.Bail:
                if named_only:
                    raise
    except asset_table.Bail as e:
        if addr is None:
            sys.exit(f"{symbol} in {c_file}: not moved ({e})")
        return move_raw(version, c_file, symbol, name, text, span, mod, addr, syms, f"not moved ({e})")
    end = start + len(recs) * stride
    got = asset_table.render(rom_bytes(version, mod, start, end), spec, syms)
    if len(got.splitlines()) != len(recs):
        sys.exit(f"{symbol}: the layout {spec} does not give {len(recs)} records")
    todo = rows(version, c_file, symbol, mod, start, end, spec, find)
    (ROOT / c_file).write_text(text[:span[0]] + f"\n#include \"asset/{name}.inc\"\n" + text[span[1]:])
    for v, m, s, e in todo:
        add_row(v, name, m, s, e, spec)
    print(f"{c_file}: {symbol} now includes asset/{name}.inc ({mod} {start:#010x}-{end:#010x}, {len(recs)} x {spec})")


def move(version, c_file, symbol, name):
    """Replaces a C array's values with the #include of a new asset, if they are the ROM's."""
    text = (ROOT / c_file).read_text()
    m = re.search(rf"^(.*\b(u8|u16|u32|s8|s16|s32|fx16|fx32)\s+{symbol}(?:\s*\[[^\]]*\])+\s*=\s*\{{)(.*?)(^\}};|\}};$)", text, re.M | re.S)
    if m:
        body = m[3]
        flat = re.sub(r"//[^\n]*|/\*.*?\*/", "", body, flags=re.S)
        rest = re.sub(r"(?<![\w.])-?(?:0[xX][0-9a-fA-F]+|\d+)[uUlL]*(?![\w.])", "", flat)
        if re.sub(r"[\s,{}]", "", rest) or body.count("{"):
            m = None
    if not m or symbol_addr(version, module_of(version, c_file), symbol, c_file) is None:
        return move_table(version, c_file, symbol, name)
    kind = ALIAS.get(m[2], m[2])
    ours = [int(v, 0) for v in re.findall(r"(?<![\w.])-?(?:0[xX][0-9a-fA-F]+|\d+)(?![\w.])", re.sub(r"//[^\n]*|/\*.*?\*/", "", m[3], flags=re.S))]
    mod, start = locate(version, c_file, symbol)
    end = start + len(ours) * struct.calcsize(TYPES[kind])
    rom = values(version, mod, start, end, kind)
    if [v & ((1 << 8 * struct.calcsize(TYPES[kind])) - 1) for v in ours] != \
            [v & ((1 << 8 * struct.calcsize(TYPES[kind])) - 1) for v in rom]:
        sys.exit(f"{symbol}'s {len(ours)} values in {c_file} are not the ROM's at {start:#010x}")
    todo = rows(version, c_file, symbol, mod, start, end, kind)
    (ROOT / c_file).write_text(text[:m.start(3)] + f"\n#include \"asset/{name}.inc\"\n" + text[m.start(4):])
    for v, md, s, e in todo:
        add_row(v, name, md, s, e, kind)
    print(f"{c_file}: {symbol} now includes asset/{name}.inc ({mod} {start:#010x}-{end:#010x}, {kind})")


TABLE_SECONDS = 60
AUTO_NAME = re.compile(r"(?:data|func|sub)_(?:ov\d+_)?[0-9a-f]{6,8}$")   # dsd's names for what has none



def _alarm(*_):
    raise TimeoutError


signal.signal(signal.SIGALRM, _alarm)


def move_all(version, c_file, min_values=12, only=None):
    """--move for every initialised array of a file that holds at least min_values numbers (or constant names); one line
    per table it leaves: `skipped <table>: <reason>`. Never raises. Returns ({table: reason} of the skipped, moved count);
    only = the tables to try (whatever their size)."""
    text = (ROOT / c_file).read_text()
    stem = Path(c_file).stem
    done, skipped = 0, {}
    for m in re.finditer(r"^(?:\}\s*)?[^\n;{}()]*?\b(\w+)\s*(?:\[[^\]]*\]\s*)*(?:ATTRIBUTE_ALIGN\(\d+\)\s*)?=\s*\{",
                         text, re.M):   # (a struct type's closing brace before the name)
        sym = m[1]
        if only is not None and sym not in only:
            continue
        span = body_span(text, sym)
        if not span or "#include \"asset/" in text[span[0]:span[1]]:
            continue
        if only is None and len(re.findall(r"(?<![\w.])(?:0[xX][0-9a-fA-F]+|\d+)(?![\w.])", text[span[0]:span[1]])) < min_values:
            continue
        signal.alarm(TABLE_SECONDS)
        reason = None
        try:
            table = ROOT / "config" / version / "assets.tsv"
            rows_text = table.read_text() if table.exists() else ""
            name, n = f"{stem}_{sym}", 2
            while f"\n{name}\t" in rows_text:   # static locals of one name
                name, n = f"{stem}_{sym}_{n}", n + 1
            move(version, c_file, sym, name)
            done += 1
        except SystemExit as e:
            reason = str(e.code)
        except TimeoutError:
            reason = f"took over {TABLE_SECONDS} s"
        except Exception as e:   # a table the tools do not understand is skipped, whatever it is
            reason = f"{type(e).__name__}: {e}"
        finally:
            signal.alarm(0)
        if reason is not None:
            reason = re.sub(rf"^{sym}(?: in \S+)?: ", "", " ".join(reason.split()))
            skipped[sym] = reason
            print(f"skipped {sym}: {reason}")
    return skipped, done


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("args", nargs="+", help="module start end type out, or with --move: file symbol name")
    ap.add_argument("--move", action="store_true", help="a C array's or table's values to a new asset")
    ap.add_argument("--move-all", action="store_true", help="every table of a file (args: file)")
    ap.add_argument("--version", default="b_us")
    a = ap.parse_args()
    if a.move_all:
        move_all(a.version, *a.args)
    elif a.move:
        move(a.version, *a.args)
    else:
        mod, start, end, kind, out = a.args
        write_asset(a.version, mod, int(start, 16), int(end, 16), kind, Path(out))


if __name__ == "__main__":
    main()
