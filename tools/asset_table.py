"""Tables of records (structs, arrays of arrays, names mixed in) as assets: tools/asset.py's second form.

A C table keeps its declaration and includes its values:

    static const ShopItem data_ov021_021d9750[10] = {
    #include "asset/scrcmd_shop_data_ov021_021d9750.inc"
    };

The row of assets.tsv gives, in its type column, the fields of one record (the C's top-level elements) in memory order:

    u16,u8                      two fields; natural alignment as in C, so a u16, a u8 and a padding byte (stride 4)
    u8*8                        eight u8 fields
    u8[255=TOUCH_RECT_END]      a u8 field whose value 255 is written by that name (macro or enum) in the .inc
    ptr                         a 4-byte address of a function or data symbol (written by name, NULL for 0)
    ref                         the same, written &name

The .inc holds one `{ field, field }` line per record (a table of single-field records: the bare values), the ROM's
values of the version being built. Names of ptr/ref fields come from that version's symbols.txt, so a pointer to a
White-only address prints White's symbol; a value that has no name in a name map prints as a number.
"""
import re
import struct
import sys
from functools import lru_cache
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SIZES = {"u8": 1, "u16": 2, "u32": 4, "s8": 1, "s16": 2, "s32": 4, "ptr": 4, "ref": 4}
FMT = {"u8": "B", "u16": "H", "u32": "I", "s8": "b", "s16": "h", "s32": "i", "ptr": "I", "ref": "I"}
FIELD = re.compile(r"(u8|u16|u32|s8|s16|s32|ptr|ref)(?:\[([^\]]*)\])?(?:\*(\d+))?")
NUM = re.compile(r"[-+]?(?:0[xX][0-9a-fA-F]+|\d+)[uUlL]*")


class Bail(Exception):
    """The table can't be an asset (the reason)."""


def parse_layout(spec):
    cols = []
    for item in spec.split(","):
        m = FIELD.fullmatch(item)
        if not m:
            sys.exit(f"bad table layout {item!r} in {spec!r}")
        names = {}
        for kv in (m[2].split("/") if m[2] else []):
            v, n = kv.split("=")
            names[int(v)] = n
        cols += [(m[1], names)] * int(m[3] or 1)
    return cols


def layout_spec(cols):
    out, i = [], 0
    while i < len(cols):
        j = i
        while j < len(cols) and cols[j] == cols[i]:
            j += 1
        kind, names = cols[i]
        maps = "/".join(f"{v}={n}" for v, n in sorted(names.items()))
        out.append(kind + (f"[{maps}]" if maps else "") + (f"*{j - i}" if j - i > 1 else ""))
        i = j
    return ",".join(out)


def offsets(cols):
    """Offset of each field in a record and the record's size (C alignment)."""
    off, offs = 0, []
    for kind, _ in cols:
        w = SIZES[kind]
        off = (off + w - 1) // w * w
        offs.append(off)
        off += w
    a = max(SIZES[k] for k, _ in cols)
    return offs, (off + a - 1) // a * a


VENDORED = "third_party/pokebw2"   # Black 2's and White 2's C and config: the vendored pokebw2 (docs/black2-white2.md)


def arm9_dir(version):
    """A version's config/<version>/arm9 (delinks.txt, symbols.txt): ours, or the vendored pokebw2's for b2_us, w2_us."""
    return ROOT / (VENDORED + "/config" if version in ("b2_us", "w2_us") else "config") / version / "arm9"


@lru_cache(maxsize=None)
def symbol_names(version, mod):
    """{address: [names]} of main, the autoloads and one overlay of a version."""
    root = arm9_dir(version)
    out = {}
    for p in sorted(root.rglob("symbols.txt")):
        m = "main" if p.parent == root else p.parent.name
        if m.startswith("ov") and m != mod:
            continue
        for s in re.finditer(r"^(\S+) kind:\S+ addr:(0x\w+)", p.read_text(), re.M):
            out.setdefault(int(s[2], 16), []).append(s[1])
    return out


def names_at(symbols, v):
    return symbols.get(v) or symbols.get(v & ~1) or []


def field_text(kind, names, v, symbols):
    if kind in ("ptr", "ref"):
        if v == 0:
            return "NULL"
        found = names_at(symbols, v)
        if not found:
            raise ValueError(f"pointer {v:#010x} is not a symbol")
        return ("&" if kind == "ref" else "") + found[0]
    if v in names:
        return names[v]
    return f"{v:#04x}" if kind[0] == "u" else str(v)


def render(data, spec, symbols):
    """The .inc text of a table's bytes."""
    cols = parse_layout(spec)
    offs, stride = offsets(cols)
    if len(data) % stride:
        raise ValueError(f"{len(data):#x} bytes is not a whole number of {stride}-byte records ({spec})")
    lines = []
    for r in range(len(data) // stride):
        f = []
        for (kind, names), o in zip(cols, offs):
            v = struct.unpack_from("<" + FMT[kind], data, r * stride + o)[0]
            f.append(field_text(kind, names, v, symbols))
        lines.append("    " + (f[0] + "," if len(f) == 1 else "{ " + ", ".join(f) + " },"))
    return "\n".join(lines) + "\n"


# ---- moving a C table ------------------------------------------------------------------------------------------------

def strip_comments(s):
    return re.sub(r"//[^\n]*|/\*.*?\*/", "", s, flags=re.S)


LEAF = re.compile(r"[{}]|,|(?:[^{},()]|\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\))+")
DESIG = re.compile(r"\[([^\]]*)\]\s*=\s*")


def parse_init(body, evaluate=None):
    """The records of a C initialiser (text from its { to its }): a list of leaf-expression lists. `[INDEX] = {..}`
    designators place a record (evaluate gives the index; the records between are zeros)."""
    body = strip_comments(body)
    if re.search(r"^\s*#", body, re.M):
        raise Bail("preprocessor lines in the initialiser")
    plain = DESIG.sub("", body)
    if '"' in plain or re.search(r"\.\s*[A-Za-z_]\w*\s*=", plain):
        raise Bail("strings or field designators in the initialiser")
    recs, depth, cur, index = [], 0, None, None
    for t in LEAF.findall(body):
        t = t.strip()
        d = DESIG.match(t) if t not in "{}," else None
        if d:
            v = int(d[1], 0) if re.fullmatch(r"\d+|0[xX][0-9a-fA-F]+", d[1].strip()) else evaluate(d[1]) if evaluate else None
            if v is None:
                raise Bail(f"designator [{d[1]}] has no value")
            index, t = v, t[d.end():].strip()
        if t == "{":
            depth += 1
            if depth == 2:
                cur = []
        elif t == "}":
            if depth == 2:
                if index is not None:
                    recs.extend([["0"] * len(cur)] * (index - len(recs)))
                    index = None
                recs.append(cur)
                cur = None
            depth -= 1
        elif t and t != ",":
            if depth == 1:
                if index is not None:
                    recs.extend([["0"]] * (index - len(recs)))
                    index = None
                recs.append([t])
            elif depth >= 2:
                cur.append(t)
    return recs


def leaf_kind(t):
    if NUM.fullmatch(t):
        return "num"
    if re.fullmatch(r"&\s*[A-Za-z_]\w*", t):
        return "ref"
    if re.fullmatch(r"[A-Za-z_]\w*", t):
        return "ident"
    raise Bail(f"expression {t!r} has no value")


def resolve(recs, symbols, evaluate):
    """The records with each constant name or expression (not a symbol, not & or NULL) replaced by its value."""
    if not evaluate:
        return recs
    allnames = {x for v in symbols.values() for x in v}
    out = []
    for r in recs:
        row = []
        for t in r:
            if not (NUM.fullmatch(t) or t == "NULL" or t.startswith("&") or t in allnames):
                v = evaluate(t)
                t = str(v) if v is not None else t
            row.append(t)
        out.append(row)
    return out


def infer(recs, rom, symbols, region, evaluate=None):
    """Layout spec of the records whose first bytes are rom, or Bail. region = bytes up to the next symbol; evaluate
    gives the value of a constant name or expression (or None)."""
    recs = resolve(recs, symbols, evaluate)
    if not recs or not recs[0]:
        raise Bail("empty initialiser")
    n, k = len(recs), len(recs[0])
    if any(len(r) != k for r in recs):
        raise Bail("records of different lengths")
    if n * k < 8:
        raise Bail("too small")
    allnames = {x for v in symbols.values() for x in v}
    colinfo = []
    for j in range(k):
        leaves = [r[j] for r in recs]
        kinds = [leaf_kind(t) for t in leaves]
        if "ref" in kinds:
            if any(kd == "num" and int(t.rstrip("uUlL"), 0) for kd, t in zip(kinds, leaves)):
                raise Bail("mixed & and numbers")
            colinfo.append("ref")
        elif any(kd == "ident" and t in allnames for kd, t in zip(kinds, leaves)):
            if any(kd == "num" and int(t.rstrip("uUlL"), 0) for kd, t in zip(kinds, leaves)):
                raise Bail("mixed symbols and numbers")
            colinfo.append("ptr")
        else:
            neg = any(t.startswith("-") for t in leaves)
            colinfo.append("s" if neg else "u")
    cols_idents = [[t for t in (r[j] for r in recs) if leaf_kind(t) == "ident" and t != "NULL"] for j in range(k)]

    def column_ok(j, w, kind, o, s, maps):
        mask = (1 << 8 * w) - 1
        seen = {}
        for r in range(n):
            base = r * s + o
            if base + w > len(rom):
                return None
            v = int.from_bytes(rom[base:base + w], "little")
            t = recs[r][j]
            kd = leaf_kind(t)
            if kd == "num":
                x = int(t.rstrip("uUlL"), 0)
                lo, hi = (-(mask >> 1) - 1, mask >> 1) if kind[0] == "s" else (0, mask)
                if (x & mask) != v or not lo <= x <= hi:
                    return None
            elif t == "NULL":
                if v != 0:
                    return None
            elif kind in ("ptr", "ref"):
                if t.lstrip("&").strip() not in names_at(symbols, v):
                    return None
            else:
                if seen.setdefault(t, v) != v:
                    return None
        if len(set(seen.values())) < len(seen):
            return None  # two names of one value: the name written back could be the wrong one, so this is not the column
        return seen

    def solve(s):
        cols = []
        budget = [20000]

        def dfs(j, off):
            budget[0] -= 1
            if budget[0] < 0 or off > s:
                return False
            if j == k:
                return offsets(cols)[1] == s
            ci = colinfo[j]
            options = ([("ptr", 4)] if ci == "ptr" else [("ref", 4)] if ci == "ref"
                       else [((ci + str(8 * w)), w) for w in (1, 2, 4)])
            for kind, w in options:
                o = (off + w - 1) // w * w
                seen = column_ok(j, w, kind, o, s, None)
                if seen is None:
                    continue
                cols.append((kind, dict(sorted((v, t) for t, v in seen.items()))))
                if dfs(j + 1, o + w):
                    return True
                cols.pop()
            return False
        return cols if dfs(0, 0) else None

    cands = [s for s in range(1, 513) if n * s <= region and n * s > region - 16]
    cands += [s for s in range(1, 513) if n * s <= region and s not in cands][::-1]
    for s in cands[:40]:
        cols = solve(s)
        if cols:
            return layout_spec([(kd, nm if kd not in ("ptr", "ref") else {}) for kd, nm in cols]), s
    raise Bail("no layout of the ROM's bytes fits the values (bitfields, a packed struct?)")
