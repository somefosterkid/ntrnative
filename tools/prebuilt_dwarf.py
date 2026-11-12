#!/usr/bin/env python3
"""The DWARF 2 debug info of the prebuilt objects under ~/ref, read: every function's real signature, its locals, the
struct/union/enum definitions and the globals of each object, and the pairing with Black's functions.

The archives of NitroDWC (libdwcutil, libdwcac, libdwcauth, libdwcnd, libdwcnhttp, libdwcranking, libdwcgspinger,
libdwcgspt), NitroWiFi's libssl and the SDK's libcht and libe3s are compiled with -g: their .debug_info holds, per
function, the parameter names and types (a 1-byte `bool`, the widths and the signedness the C had), the locals (name,
type, declaration line, where the compiler kept it: a register or the stack), and the work data's struct definitions.
MWCC writes no const/volatile types and no lexical blocks (a function's locals are one flat list), and the references
between entries (DW_FORM_ref_addr) are relocations whose target is in the addend. tools/prebuilt_map.py pairs the
prebuilt functions with Black's (build/prebuilt_map.tsv); this reuses that pairing.

    python3 tools/prebuilt_dwarf.py FUNCTION            # the prebuilt's declaration of Black's function: signature, locals, structs
    python3 tools/prebuilt_dwarf.py --object window.o   # every function of the object(s) with this name (--library to narrow)
    python3 tools/prebuilt_dwarf.py --stats             # what the DWARF holds, per archive
    python3 tools/prebuilt_dwarf.py --remap             # write build/prebuilt_map.tsv again (prebuilt_map.py's pairing)

The module: Unit (an object's functions, globals and aggregates), units() (every unit, cached in build/cache),
pairing() ({Black's function: Pair}), spell()/scalar() (a type as C text, or as the integer class Black's headers have).
Types are tuples: ("void",), ("base", name, size, encoding), ("ptr", T), ("arr", T, count), ("td", name, T),
("tag", kind, name, offset) (struct, union or enum, its definition in Unit.aggs[offset]), ("fn", ret, (T, ...), varargs).
"""
import argparse
import os
import sys
from collections import Counter, namedtuple
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import bwcache  # noqa: E402
from port_file import parse_object, uleb  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
REF = os.path.expanduser("~/ref")
TSV = ROOT / "build" / "prebuilt_map.tsv"
NL = chr(10)

# DWARF 2 tags and attributes MWCC writes
T_ARRAY, T_ENUM, T_PARAM, T_MEMBER, T_POINTER, T_CU = 0x01, 0x04, 0x05, 0x0D, 0x0F, 0x11
T_STRUCT, T_SUBR_TYPE, T_TYPEDEF, T_UNION, T_VARARGS, T_SUBRANGE = 0x13, 0x15, 0x16, 0x17, 0x18, 0x21
T_BASE, T_CONST, T_ENUMERATOR, T_SUBPROGRAM, T_VARIABLE, T_VOLATILE = 0x24, 0x26, 0x28, 0x2E, 0x34, 0x35
A_LOC, A_NAME, A_SIZE, A_BIT_OFF, A_BIT_SIZE, A_CONST, A_LOWER, A_UPPER = 0x02, 0x03, 0x0B, 0x0C, 0x0D, 0x1C, 0x22, 0x2F
A_MEMBER_LOC, A_LINE, A_TYPE, A_EXTERNAL, A_ENCODING, A_PRODUCER = 0x38, 0x3B, 0x49, 0x3F, 0x3E, 0x25
FIXED = {0x01: 4, 0x05: 2, 0x06: 4, 0x07: 8, 0x0B: 1, 0x0C: 1, 0x10: 4, 0x11: 1, 0x12: 2, 0x13: 4, 0x14: 8, 0x0E: 4}

DParam = namedtuple("DParam", "name type loc")
DLocal = namedtuple("DLocal", "name type line loc")   # loc: ("reg", n), ("stack", offset), ("static",) or None
DFunc = namedtuple("DFunc", "name ret params locals line external varargs")
DVar = namedtuple("DVar", "name type line external")
Agg = namedtuple("Agg", "kind name size members")   # members: (name, T, offset, bit size, bit offset); an enum's: (name, value)
Unit = namedtuple("Unit", "archive member cu producer funcs globals aggs typedefs")   # funcs: {name: DFunc} in order


class Die:
    __slots__ = ("off", "tag", "attrs", "kids")

    def __init__(self, off, tag, attrs):
        self.off, self.tag, self.attrs, self.kids = off, tag, attrs, []


# ---------------------------------------------------------------------------------------------------------- reading
def section(d, secs, name):
    for s in secs:
        if s[0] == name:
            return d[s[2]:s[2] + s[3]]
    return b""


def abbrevs(ab, at):
    """{code: (tag, has children, [(attribute, form)])} of the table at ab[at:]."""
    out, i = {}, at
    while i < len(ab):
        code, i = uleb(ab, i)
        if not code:
            break
        tag, i = uleb(ab, i)
        children, i = ab[i], i + 1
        spec = []
        while True:
            a, i = uleb(ab, i)
            f, i = uleb(ab, i)
            if not a and not f:
                break
            spec.append((a, f))
        out[code] = tag, children, spec
    return out


def sleb(b, i):
    v = shift = 0
    while True:
        v |= (b[i] & 0x7F) << shift
        shift += 7
        i += 1
        if b[i - 1] < 0x80:
            if b[i - 1] & 0x40:
                v -= 1 << shift
            return v, i


def read_dies(body):
    """({offset: Die}, [compile unit Dies]) of an object's .debug_info; ({}, []) if it has none."""
    d, secs, syms, relocs = parse_object(body)
    index = {s[0]: i for i, s in enumerate(secs)}
    info, ab = section(d, secs, ".debug_info"), section(d, secs, ".debug_abbrev")
    if not info or not ab:
        return {}, []
    refs = {off: syms[s][2] + add for off, _, s, add in relocs.get(index[".debug_info"], [])}
    dies, units, tables, at = {}, [], {}, 0
    while at + 11 <= len(info):
        length = int.from_bytes(info[at:at + 4], "little")
        end, aoff = at + 4 + length, int.from_bytes(info[at + 6:at + 10], "little")
        table = tables.setdefault(aoff, abbrevs(ab, aoff))
        i, stack = at + 11, []
        while i < end:
            off, (code, i) = i, uleb(info, i)
            if not code:
                if stack:
                    stack.pop()
                continue
            tag, children, spec = table[code]
            attrs = {}
            for a, f in spec:
                if f in FIXED:
                    n = FIXED[f]
                    if f == 0x10:   # a reference between entries is a relocation
                        attrs[a] = refs.get(i, 0)
                    elif f in (0x11, 0x12, 0x13, 0x14):   # relative to the unit
                        attrs[a] = int.from_bytes(info[i:i + n], "little") + at
                    else:
                        attrs[a] = int.from_bytes(info[i:i + n], "little")
                    i += n
                elif f == 0x08:
                    j = info.index(bytes(1), i)
                    attrs[a], i = info[i:j].decode(errors="replace"), j + 1
                elif f in (0x03, 0x04, 0x0A, 0x09):
                    if f == 0x09:
                        size, i = uleb(info, i)
                    else:
                        k = {0x03: 2, 0x04: 4, 0x0A: 1}[f]
                        size, i = int.from_bytes(info[i:i + k], "little"), i + k
                    attrs[a], i = bytes(info[i:i + size]), i + size
                elif f == 0x0F:
                    attrs[a], i = uleb(info, i)
                elif f == 0x0D:
                    attrs[a], i = sleb(info, i)
                elif f == 0x15:
                    v, i = uleb(info, i)
                    attrs[a] = v + at
                else:
                    raise ValueError(f"DWARF form {f:#x}")
            die = Die(off, tag, attrs)
            dies[off] = die
            if stack:
                stack[-1].kids.append(die)
            elif tag == T_CU:
                units.append(die)
            if children:
                stack.append(die)
        at = end
    return dies, units


def location(blob):
    """A DW_AT_location block as ("reg", n), ("stack", offset) or ("static",)."""
    if not blob:
        return None
    op = blob[0]
    if 0x50 <= op <= 0x6F:
        return ("reg", op - 0x50)
    if op == 0x91:   # DW_OP_fbreg
        return ("stack", sleb(blob, 1)[0])
    if op == 0x03:
        return ("static",)
    return None


def build_unit(archive, member, body):
    dies, cus = read_dies(body)
    if not cus:
        return None
    cache, aggs, typedefs = {}, {}, {}

    def tp(off):
        """The type at the entry's offset (None is void)."""
        if off is None:
            return ("void",)
        if off in cache:
            return cache[off]
        die = dies.get(off)
        if die is None:
            return ("void",)
        a, tag = die.attrs, die.tag
        if tag == T_BASE:
            t = ("base", a.get(A_NAME, "?"), a.get(A_SIZE, 0), a.get(A_ENCODING, 0))
        elif tag == T_POINTER:
            t = ("ptr", tp(a.get(A_TYPE)))
        elif tag in (T_CONST, T_VOLATILE):
            t = tp(a.get(A_TYPE))
        elif tag == T_TYPEDEF:
            t = ("td", a.get(A_NAME, "?"), tp(a.get(A_TYPE)))
            typedefs.setdefault(a.get(A_NAME, "?"), t)
        elif tag == T_ARRAY:
            t = tp(a.get(A_TYPE))
            ranges = [k for k in die.kids if k.tag == T_SUBRANGE]
            for k in reversed(ranges):
                up = k.attrs.get(A_UPPER)
                t = ("arr", t, None if up is None else up + 1 - k.attrs.get(A_LOWER, 0))
            if not ranges:
                t = ("arr", t, None)
        elif tag in (T_STRUCT, T_UNION, T_ENUM):
            kind = {T_STRUCT: "struct", T_UNION: "union", T_ENUM: "enum"}[tag]
            t = ("tag", kind, a.get(A_NAME), off)
            cache[off] = t    # members can name the aggregate again
            if tag == T_ENUM:
                aggs[off] = Agg(kind, a.get(A_NAME), a.get(A_SIZE, 4),
                                [(k.attrs.get(A_NAME), k.attrs.get(A_CONST, 0)) for k in die.kids if k.tag == T_ENUMERATOR])
            else:
                ms = []
                for k in die.kids:
                    if k.tag != T_MEMBER:
                        continue
                    loc = k.attrs.get(A_MEMBER_LOC, b"")
                    at = uleb(loc, 1)[0] if loc[:1] == bytes([0x23]) else \
                        int.from_bytes(loc[1:5], "little") if loc[:1] == bytes([0x0C]) else 0
                    ms.append((k.attrs.get(A_NAME), tp(k.attrs.get(A_TYPE)), at, k.attrs.get(A_BIT_SIZE),
                               k.attrs.get(A_BIT_OFF)))
                aggs[off] = Agg(kind, a.get(A_NAME), a.get(A_SIZE, 0), ms)
        elif tag == T_SUBR_TYPE:
            ps = tuple(tp(k.attrs.get(A_TYPE)) for k in die.kids if k.tag == T_PARAM)
            t = ("fn", tp(a.get(A_TYPE)), ps, any(k.tag == T_VARARGS for k in die.kids))
        else:
            t = ("void",)
        cache[off] = t
        return t

    funcs, glob, unit_name, producer = {}, [], None, None
    for cu in cus:
        unit_name = unit_name or cu.attrs.get(A_NAME)
        producer = producer or cu.attrs.get(A_PRODUCER)
        for k in cu.kids:
            a = k.attrs
            if k.tag == T_VARIABLE and A_NAME in a:
                glob.append(DVar(a[A_NAME], tp(a.get(A_TYPE)), a.get(A_LINE), bool(a.get(A_EXTERNAL))))
            elif k.tag == T_SUBPROGRAM and A_NAME in a:
                ps = [DParam(c.attrs.get(A_NAME), tp(c.attrs.get(A_TYPE)), location(c.attrs.get(A_LOC)))
                      for c in k.kids if c.tag == T_PARAM]
                # the compiler writes a function's locals last declared first
                ls = [DLocal(c.attrs.get(A_NAME), tp(c.attrs.get(A_TYPE)), c.attrs.get(A_LINE), location(c.attrs.get(A_LOC)))
                      for c in reversed(k.kids) if c.tag == T_VARIABLE and A_NAME in c.attrs]
                ls.sort(key=lambda x: x.line or 0)    # stable: the reversed order stands for equal lines
                funcs[a[A_NAME]] = DFunc(a[A_NAME], tp(a.get(A_TYPE)), ps, ls, a.get(A_LINE), bool(a.get(A_EXTERNAL)),
                                         any(c.tag == T_VARARGS for c in k.kids))
    for off, die in dies.items():   # every aggregate and typedef, also those no function reaches
        if die.tag in (T_STRUCT, T_UNION, T_ENUM, T_TYPEDEF):
            tp(off)
    return Unit(archive, member, unit_name, producer, funcs, glob, aggs, typedefs)


# ------------------------------------------------------------------------------------------------------------ cache
def archives():
    """[(path relative to ~/ref, absolute path)] of the archives that carry debug info, from prebuilt_map's list."""
    import prebuilt_map as pm
    return [(path[len(REF) + 1:], path) for path, data in pm.archives() if b".debug_info" in data]


def units():
    """{(archive relative to ~/ref, member name): Unit} of every object with DWARF, cached per archive."""
    import prebuilt_map as pm
    out = {}
    for rel, path in archives():
        token = (bwcache.stamp(path), bwcache.code_hash(__file__, pm.__file__))
        got = bwcache.get("prebuilt_dwarf", path, token)
        if got is None:
            got = {}
            for name, body in pm.members(open(path, "rb").read()):
                try:
                    u = build_unit(rel, name, body)
                except (ValueError, KeyError, IndexError) as e:
                    sys.stderr.write(f"{rel}:{name}: {type(e).__name__}: {e}{NL}")
                    continue
                if u:
                    got[(rel, name)] = u
            bwcache.put("prebuilt_dwarf", path, token, got)
        out.update(got)
    return out


# ------------------------------------------------------------------------------------------------------------ types
def strip(t):
    """The type without its typedefs."""
    while t[0] == "td":
        t = t[2]
    return t


INT_NAMES = {(1, True): "s8", (1, False): "u8", (2, True): "s16", (2, False): "u16", (4, True): "s32", (4, False): "u32",
             (8, True): "s64", (8, False): "u64"}
ENC_SIGNED = {5: True, 6: True, 7: False, 8: False, 2: False}   # signed, signed char, unsigned, unsigned char, boolean


def scalar(t):
    """A type as ("int", size, signed), ("float", size), ("ptr", pointee), ("enum", size), ("void",) or None (aggregates,
    arrays, functions). A `bool` is an unsigned 1-byte integer, as is a plain `char` the compiler took as unsigned."""
    t = strip(t)
    if t[0] == "base":
        _, _, size, enc = t
        return ("float", size) if enc == 4 else ("int", size, ENC_SIGNED.get(enc, False))
    if t[0] == "ptr":
        return ("ptr", t[1])
    if t[0] == "tag" and t[1] == "enum":
        return ("enum", 4)
    if t[0] == "void":
        return ("void",)
    return None


def int_spelling(sc):
    """Black's name for an integer class ("int", size, signed): u8, s8, ... (None if it has none)."""
    return INT_NAMES.get((sc[1], sc[2])) if sc and sc[0] == "int" else None


def base_name(t, raw, alias=None):
    """The C name of a leaf type (base, void, struct tag). alias: {tag offset: the typedef name that stands for it}."""
    if t[0] == "base":
        if raw:
            return t[1]
        sc = scalar(t)
        return ("f32" if sc[1] == 4 else "f64") if sc[0] == "float" else (int_spelling(sc) or t[1])
    if t[0] == "tag":
        if alias and t[3] in alias:
            return alias[t[3]]
        return f"{t[1]} {t[2]}" if t[2] else f"{t[1]} /* anonymous */"
    return "void"


def aliases(unit, known):
    """{aggregate offset: typedef name} of the unit's typedefs of an aggregate that `known` (the typedef names this tree has)
    includes."""
    return {td[2][3]: n for n, td in unit.typedefs.items() if td[2][0] == "tag" and n in known}


def spell(t, name="", known=None, raw=False, alias=None):
    """C text declaring `name` with type t. known: the typedef names this tree has (others are spelled through);
    raw: the DWARF's own base type names (bool, unsigned long) instead of Black's; alias: see base_name."""
    def named(t):
        return t[0] == "td" and (known is None or t[1] in known)

    def go(t, inner):
        k = t[0]
        if k == "td":
            if named(t):
                return t[1] + (" " + inner if inner else "")
            return go(t[2], inner)
        if k == "ptr":
            target = t[1]
            inner = ("(*" + inner + ")") if strip(target)[0] in ("arr", "fn") and not named(target) else "*" + inner
            return go(target, inner)
        if k == "arr":
            return go(t[1], inner + "[" + ("" if t[2] is None else str(t[2])) + "]")
        if k == "fn":
            ps = ", ".join(spell(p, "", known, raw, alias) for p in t[2]) or "void"
            return go(t[1], inner + "(" + ps + (", ..." if t[3] else "") + ")")
        return base_name(t, raw, alias) + (" " + inner if inner else "")

    return go(t, name).strip()


def agg_text(unit, off, known=None, indent="    "):
    """The C definition of the aggregate at off, its members in order."""
    g = unit.aggs[off]
    head = f"{g.kind} {g.name}" if g.name else g.kind
    if g.kind == "enum":
        return f"{head} {{{NL}" + ("," + NL).join(f"{indent}{n} = {v}" for n, v in g.members) + f"{NL}}};"
    lines = []
    for n, t, at, bits, _ in g.members:
        bit = " : " + str(bits) if bits else ""
        lines.append(indent + spell(t, n or "", known) + bit + f";  // +{at:#x}")
    return f"{head} {{  // {g.size:#x} bytes{NL}" + NL.join(lines) + f"{NL}}};"


def used_aggs(unit, types):
    """The offsets of the aggregates the types name (through typedefs, pointers and members), dependencies first."""
    order, seen = [], set()

    def walk(t):
        while t[0] in ("td", "ptr", "arr"):
            t = t[2] if t[0] == "td" else t[1]
        if t[0] == "fn":
            for p in (t[1],) + tuple(t[2]):
                walk(p)
        elif t[0] == "tag" and t[3] in unit.aggs and t[3] not in seen:
            seen.add(t[3])
            if unit.aggs[t[3]].kind != "enum":
                for m in unit.aggs[t[3]].members:
                    walk(m[1])
            order.append(t[3])

    for t in types:
        walk(t)
    return order


# --------------------------------------------------------------------------------------------------------- pairing
Pair = namedtuple("Pair", "black func archive member how sim tree_file unit dfunc")


def remap():
    """Writes build/prebuilt_map.tsv: prebuilt_map.py's pairing of the members' functions with Black's."""
    import prebuilt_map as pm
    placements, _ = pm.place(pm.prebuilt(), pm.black())
    pm.write_tsv(placements)


def read_tsv():
    """[dict] of the rows of prebuilt_map's table (written when it is not there)."""
    if not TSV.exists():
        remap()
    rows = []
    with open(TSV) as fh:
        head = fh.readline().rstrip(NL).split("\t")
        for line in fh:
            rows.append(dict(zip(head, line.rstrip(NL).split("\t"))))
    return rows


_PAIRS = []


def pairing(rows=None, all_units=None):
    """{Black's function name: Pair} for every function of a unit with DWARF that prebuilt_map pairs."""
    if rows is None and all_units is None and _PAIRS:
        return _PAIRS[0]
    rows = read_tsv() if rows is None else rows
    us = units() if all_units is None else all_units
    out = {}
    for r in rows:
        u = us.get((r["archive"], r["member"]))
        f = u.funcs.get(r["function"]) if u else None
        if f is not None:
            out[r["black_name"]] = Pair(r["black_name"], r["function"], r["archive"], r["member"], r["how"],
                                        float(r["similarity"]), r["tree_file"], u, f)
    if all_units is None and not _PAIRS:
        _PAIRS.append(out)
    return out


def lookup(name, tree_file=None, pairs=None):
    """The Pair of a function of the tree by its Black name or, once renamed, the prebuilt's name."""
    pairs = pairing() if pairs is None else pairs
    if name in pairs:
        return pairs[name]
    found = [p for p in pairs.values() if p.func == name and (tree_file is None or p.tree_file == tree_file)]
    return found[0] if len(found) == 1 else None


# ------------------------------------------------------------------------------------------------------- the report
def signature(f, known=None, raw=False):
    ps = ", ".join(spell(p.type, p.name or "", known, raw) for p in f.params) or "void"
    more = ", ..." if f.varargs else ""
    return spell(f.ret, "", known, raw) + " " + f.name + "(" + ps + more + ")"


def where(loc):
    if not loc:
        return ""
    return "r" + str(loc[1]) if loc[0] == "reg" else "sp+" + str(loc[1]) if loc[0] == "stack" else "static"


def describe(p, known=None, structs=True, raw=False):
    """Text: the prebuilt's declaration of a paired function: signature, locals in declaration order, the structs used."""
    f, u = p.dfunc, p.unit
    out = ["// " + p.archive.split("/")[-1] + ":" + p.member + " " + str(u.cu) + ":" + str(f.line) + "  (" + p.how + ", %.2f)" % p.sim,
           signature(f, known, raw) + " {"]
    for x in f.locals:
        note = "  // line " + str(x.line) + ", " + where(x.loc) if x.line else ""
        out.append("    " + spell(x.type, x.name, known, raw) + ";" + note)
    out.append("}")
    if structs:
        for off in used_aggs(u, [f.ret] + [x.type for x in f.params] + [x.type for x in f.locals]):
            out.append(agg_text(u, off, known))
    if any(x.loc for x in f.params):
        out.append("// parameters: " + ", ".join(str(x.name) + "=" + where(x.loc) for x in f.params))
    return NL.join(out)


def stats():
    us = units()
    per = {}
    for (arc, _), u in us.items():
        row = per.setdefault(arc, Counter())
        row["objects"] += 1
        row["functions"] += len(u.funcs)
        row["params"] += sum(len(f.params) for f in u.funcs.values())
        row["locals"] += sum(len(f.locals) for f in u.funcs.values())
        row["globals"] += len(u.globals)
        row["aggregates"] += len(u.aggs)
        row["bool"] += sum(1 for f in u.funcs.values() for x in [f.ret] + [p.type for p in f.params] + [v.type for v in f.locals]
                           if strip(x)[0] == "base" and strip(x)[1] == "bool")
    for arc, row in sorted(per.items()):
        print(arc + ": " + ", ".join(str(v) + " " + k for k, v in row.items()))
    by = Counter(p.archive for p in pairing(all_units=us).values())
    print("paired with Black's functions: " + ", ".join(a.split("/")[-1] + " " + str(n) for a, n in sorted(by.items())))


def data_lines(u, raw=False):
    """An object's globals and file statics as declarations, each with its line in the source: [lines]."""
    return [("" if g.external else "static ") + spell(g.type, g.name, raw=raw) + ";" +
            (f"  // line {g.line}" if g.line else "") for g in u.globals]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("function", nargs="?", help="a function of the tree (Black's name, or the prebuilt's)")
    ap.add_argument("--object", help="every function of the objects with this name")
    ap.add_argument("--library", help="only archives with this in their path")
    ap.add_argument("--stats", action="store_true")
    ap.add_argument("--remap", action="store_true")
    ap.add_argument("--raw", action="store_true", help="the DWARF's own type names")
    args = ap.parse_args()
    if args.remap:
        remap()
    if args.stats:
        return stats()
    if args.object:
        for (arc, member), u in units().items():
            if member == args.object and (not args.library or args.library in arc):
                print("// " + arc + ":" + member + " " + str(u.cu) + " (" + str(u.producer) + ")")
                for f in u.funcs.values():
                    print(signature(f, raw=args.raw))
                for line in data_lines(u, raw=args.raw):  # its globals and statics too (t35m: only the functions)
                    print(line)
        return
    if not args.function:
        ap.error("a function, --object or --stats")
    p = lookup(args.function)
    if not p:
        sys.exit(args.function + ": no prebuilt function with DWARF is paired with it")
    print(describe(p, raw=args.raw))


if __name__ == "__main__":
    main()
