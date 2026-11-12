#!/usr/bin/env python3
"""The prebuilt library's own .bss objects (names, sizes, types: tools/prebuilt_dwarf.py) as the statics of a file whose C
reaches its .bss through byte-offset casts over externs (tools/bss_statics.py uses this).

A library built into Black as one translation unit (libdwcnd's 13 members are Black's nd.c) lays out its .bss as MWCC does
with -ipa file: every object of the members sorted by size, smallest first. Black's code reaches the small ones from one
base register (ldr r4, =0x021d3b9c; ldr r1, [r4, #0x14]), where the C has ((void (**)())&data_x)[5]. The prebuilt
objects say what is there, so:

  objects(dest)   the .bss objects of the members whose functions Black's file holds: [Obj] in the archive's order
  fit(...)        which objects sit at which of Black's symbol addresses (sorted by size; an object Black's version of the
                  library lacks is skipped, a struct may be larger): [(Obj, address, size)] for the longest run that fits
  rewrite(...)    the C with the uses of one extern block rewritten onto the statics: ((T *)&x)[k], *(T *)(&x + n),
                  *(T *)((u8 *)((void *)&x) + n), &x, x -> the static at that address (None if a use doesn't fit)

Names are the prebuilt's where the objects of one size can be told apart by the code Black's functions have and the
prebuilt's paired functions have (vote()); the others get the address as their name, and the report lists the candidates.
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from collections import namedtuple  # noqa: E402

Obj = namedtuple("Obj", "name size align decl kind member glob tag")
# decl: the declaration text with %s for the name ("s8 %s[9]", "void (*%s)(void *)"); kind: 'int', 'ptr', 'fn', 'array',
# 'struct'; tag: the struct tag or ""


# --- the prebuilt's objects ----------------------------------------------------------------------------------------

_KNOWN = []


def known_types(root):
    """(typedef names, struct tags) the tree's headers define: a DWARF struct is spelled as the tree's typedef of it when
    it has one."""
    if _KNOWN:
        return _KNOWN[0]
    import glob
    tds, tags = set(), set()
    heads = glob.glob(f"{root}/include/**/*.h", recursive=True) + glob.glob(f"{root}/lib/*/include/**/*.h", recursive=True)
    for h in heads:
        try:
            t = re.sub(r"//[^\n]*|/\*.*?\*/", " ", open(h, errors="replace").read(), flags=re.S)
        except OSError:
            continue
        tds.update(re.findall(r"\btypedef\b[^;{}]*?\b(\w+)\s*;", t))
        tds.update(re.findall(r"\}\s*(\w+)\s*;", t))
        tags.update(re.findall(r"\bstruct\s+(\w+)\s*\{", t))
    _KNOWN.append((tds, tags))
    return _KNOWN[0]


def enum_free(text):
    """The spelled type with its enums as s32 (Black's headers have none of the prebuilt's anonymous ones)."""
    return re.sub(r"\benum\s+@enum\b|\benum\s+\w+\b", "s32", text)


def spell_object(unit, t, root, size, align):
    """(declaration with %s for the name, kind, struct tag) of an object of DWARF type t."""
    import prebuilt_dwarf as pdw
    tds, tags = known_types(root)
    st = pdw.strip(t)
    base = st
    while base[0] == "arr":
        base = pdw.strip(base[1])
    if base[0] == "tag" and base[1] in ("struct", "union"):
        tag = base[2] or ""
        alias = next((n for n, td in unit.typedefs.items() if td[2][0] == "tag" and td[2][3] == base[3] and n in tds), None)
        if alias:
            text = pdw.spell(t, "%s", known={alias}, alias={base[3]: alias})
        elif tag in tags:
            text = pdw.spell(t, "%s")
        else:  # a struct this tree doesn't define: its bytes
            word = "u32" if align >= 4 and size % 4 == 0 else "u8"
            return f"{word} %s[{size // 4 if word == 'u32' else size}]", "struct", tag
        return enum_free(text), "struct", tag
    text = enum_free(pdw.spell(t, "%s"))
    kind = "array" if st[0] == "arr" else "fn" if st[0] == "ptr" and pdw.strip(st[1])[0] == "fn" else \
        "ptr" if st[0] == "ptr" else "int"
    return text, kind, ""


def objects(dest, root):
    """[Obj] of the .bss objects of the prebuilt members that have functions paired with dest: archive order, then the
    object's symbol order."""
    import prebuilt_dwarf as pdw
    import prebuilt_map as pm
    from port_file import parse_object
    wanted = {(p.archive, p.member) for p in pdw.pairing().values() if p.tree_file == dest}
    units = pdw.units()
    out = []
    for archive in sorted({a for a, _ in wanted}):
        data = open(os.path.join(pdw.REF, archive), "rb").read()
        for member, body in pm.members(data):
            if (archive, member) not in wanted or (archive, member) not in units:
                continue
            unit = units[(archive, member)]
            _, secs, syms, _ = parse_object(body)
            globs = {g.name: g for g in unit.globals}
            for name, ndx, value, size, glob, func in syms:
                if func or not size or not 0 < ndx < len(secs) or not secs[ndx][0].startswith(".bss") or \
                        name.startswith(".") or secs[ndx][1] != 8:
                    continue
                base = re.sub(r"[$]\d+$", "", name)
                g = globs.get(base)
                align = secs[ndx][4] or 1
                if g is not None:
                    decl, kind, tag = spell_object(unit, g.type, root, size, align)
                elif size == 4:
                    decl, kind, tag = "u32 %s", "int", ""
                else:
                    decl, kind, tag = f"u8 %s[{size}]", "array", ""
                out.append(Obj(base, size, align, decl, kind, member.rstrip(".o"), glob, tag))
    return out


def sort_objects(objs):
    """MWCC's order: smallest first; equal sizes in the archive's order for now (the definitions' order that makes MWCC
    lay them out as Black has them is found by compiling: tools/bss_statics.py lay_out)."""
    return sorted(objs, key=lambda o: o.size)


# --- which objects are at which addresses ------------------------------------------------------------------------

def align_up(x, a):
    return -(-x // a) * a


def fit(starts, lo, hi, objs, skip=3, grow=8):
    """Lays the sorted objects out over [lo, hi) so that Black's symbol addresses (starts) are the starts of objects.
    Returns ([(Obj, address, size)], end address): the run of Black's range that accounts for the most symbols, from the
    run's first symbol on (an object that Black's version of the library lacks may be skipped, up to skip of them; a
    struct or array may be up to grow bytes larger than the prebuilt's, to the next symbol; a symbol inside a struct or
    array is one of its fields, not a miss). ([], 0) when no run covers two symbols."""
    syms = sorted({s for s in starts if lo <= s < hi})
    last = syms[-1] if syms else 0
    best = [(-1, 0), [], 0]
    sys.setrecursionlimit(10000)

    def go(j, c, placed, skipped, base):
        n = sum(1 for s in syms if base <= s < c)
        score = (n, -skipped)
        if len(placed) >= 2 and n >= 2 and score > tuple(best[0]):
            best[:] = [score, list(placed), c]
        if j >= len(objs) or c > last:
            return
        o = objs[j]
        a = align_up(c, o.align)
        if not any(c <= s < a for s in syms):  # no symbol in the padding
            sizes = [o.size] + ([s - a for s in syms if a + o.size < s <= a + o.size + grow]
                                if o.kind == "struct" else [])
            for sz in sizes:
                if a + sz > hi:
                    continue
                if any(a < s < a + sz for s in syms) and o.kind != "struct":
                    continue
                placed.append((o, a, sz))
                go(j + 1, a + sz, placed, skipped, base)
                placed.pop()
        if skipped < skip:
            go(j + 1, c, placed, skipped + 1, base)

    for first in syms:
        go(0, first, [], 0, first)
    return best[1], best[2]


# --- rewriting the uses of an extern block onto statics -----------------------------------------------------------

SIZES = {"u8": 1, "s8": 1, "char": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4, "int": 4, "uint": 4, "BOOL": 4, "f32": 4,
         "long": 4, "unsigned": 4, "u64": 8, "s64": 8, "void": 1, "OSMutex": 24}
TYPE_WORDS = {"unsigned", "signed", "short", "long", "const", "struct", "union", "enum"} | set(SIZES)
BYTE_TYPES = {"u8", "s8", "char", "void"}
LO, HI = "", ""
TOK = LO + r"(\d+)" + HI
NUM = r"(?:0x[0-9a-fA-F]+|\d+)"
CAST = r"\(\s*((?:const\s+)?[A-Za-z_]\w*(?:\s+[A-Za-z_]\w*)*(?:\s*\*)*(?:\s*\(\s*\*+\s*\)\s*\([^()]*\))?)\s*\)"
LVALUE_AFTER = re.compile(r"\s*(?:\+\+|--|(?:<<|>>|[-+*/%&|^])?=(?!=))")


def norm(t):
    return re.sub(r"\s+", "", re.sub(r"\bconst\b", "", t))


def type_size(t):
    t = re.sub(r"\bconst\b", "", t).strip()
    if "*" in t:
        return 4
    return SIZES.get(" ".join(t.split()))


def pointee(t):
    """(pointee type text, its size) of a pointer type text; (None, 0) for a pointer to a function; None if t is no pointer."""
    t = " ".join(t.split())
    m = re.fullmatch(r"(.*?)\(\s*(\*+)\s*\)\s*(\(.*\))", t)
    if m:
        if len(m[2]) == 1:
            return None, 0
        return m[1].strip() + " (*" + "*" * (len(m[2]) - 2) + ")" + m[3], 4
    m = re.fullmatch(r"(.*?)\s*\*", t)
    if not m:
        return None
    elem = m[1].strip()
    return elem, type_size(elem)


def pointer_to(t):
    """The type text of a pointer to type t."""
    return t.replace("(*", "(**", 1) if "(*" in t else t + " *"


def unary_before(s, i):
    """Whether the operator at s[i] is unary: what precedes it is not an operand (a word, ), ] or a node)."""
    k = i - 1
    while k >= 0 and s[k] in " \t\n":
        k -= 1
    if k < 0:
        return True
    if s[k] == ")":  # a cast before the operator: (T *)&x, (u32)*p
        depth, j = 0, k
        while j >= 0:
            depth += {")": 1, "(": -1}.get(s[j], 0)
            if depth == 0:
                break
            j -= 1
        inner = s[j + 1:k].strip()
        word = re.search(r"(\w+)\s*$", s[:j])
        if j < 0 or word and word[1] not in ("return", "case", "sizeof"):
            return False
        words = re.findall(r"\w+", inner)
        return bool(inner) and ("*" in inner or all(w in TYPE_WORDS or w[0].isupper() for w in words))
    if s[k] in "]" + HI:
        return False
    if s[k].isalnum() or s[k] == "_":
        word = re.search(r"(\w+)$", s[:k + 1])
        return word[1] in ("return", "case")
    return True


class Piece:
    """What rewrite() needs of a static: address, size, name, the (element) type text, [n] for an array, kind."""

    def __init__(self, addr, size, name, ctype, suffix="", kind="int"):
        self.addr, self.size, self.name, self.ctype, self.suffix, self.kind = addr, size, name, ctype, suffix, kind


def piece_at(pieces, a, n=1):
    return next((p for p in pieces if p.addr <= a and a + n <= p.addr + p.size), None)


def elem_of(p):
    """(element type text, element size) of a piece: an array's element, else the whole."""
    if p.suffix:
        return p.ctype, type_size(p.ctype) or p.size // int(re.match(r"\[(\d+)\]", p.suffix)[1])
    return p.ctype, p.size


def emit_acc(p, a, elem, esz, lvalue):
    """The expression for the esz-byte object of type elem at address a, inside piece p."""
    po = a - p.addr
    et, es = elem_of(p)
    if p.suffix:
        if es == esz and po % es == 0 and norm(et) == norm(elem):
            return f"{p.name}[{po // es}]"
        return f"(*({pointer_to(elem)})((u8 *){p.name} + {po}))"
    if po == 0 and esz == p.size:
        if norm(elem) == norm(p.ctype):
            return p.name
        if lvalue or p.kind == "struct":
            return f"(*({pointer_to(elem)})&{p.name})"
        return f"(({elem}){p.name})"
    if not po:
        return f"(*({pointer_to(elem)})&{p.name})"
    return f"(*({pointer_to(elem)})((u8 *)&{p.name} + {po}))"


def emit_addr(p, a, ptype, after):
    po = a - p.addr
    base = p.name if p.suffix else f"&{p.name}"
    body = f"((u8 *){base} + {po})" if po else base
    text = f"({ptype})" + body if ptype else "(u8 *)" + body
    return "(" + text + ")" if re.match(r"\s*(?:->|\.|\[|\()", after) else text


class Rewriter:
    """One extern block's uses -> statics. Text is reduced to nodes: ('addr', offset, pointer type or None) for &x and the
    casts and sums over it, ('acc', offset, type, size) for an object read or written at an offset, ('arr',) for a table
    by its name; the nodes are then written as the statics at those addresses."""

    def __init__(self, name, base, pieces, declared):
        self.name, self.base, self.pieces = name, base, pieces
        self.ctype, self.dims = declared
        self.nodes = []

    def tok(self, n):
        self.nodes.append(n)
        return f"{LO}{len(self.nodes) - 1}{HI}"

    def reduce(self, s):
        name = self.name

        def amp(m):
            return self.tok(("addr", 0, None)) if unary_before(s, m.start()) else m[0]
        s = re.sub(rf"&\s*\(\s*{name}\s*\)|&\s*{name}\b(?!\s*\[)", amp, s)
        if self.dims:
            s = re.sub(rf"(?<![\w.{LO}]){name}\b", lambda m: self.tok(("arr",)), s)
        else:
            s = re.sub(rf"(?<![\w.{LO}]){name}\b", lambda m: self.tok(("acc", 0, self.ctype, type_size(self.ctype))), s)
        for _ in range(30):
            before = s
            s = re.sub(rf"\(\s*({TOK})\s*\)", lambda m, s=s: m[1] if unary_before(s, m.start()) else m[0], s)
            s = re.sub(CAST + rf"\s*{TOK}", lambda m, s=s: self.cast(m, s), s)
            s = re.sub(rf"{TOK}\s*([+-])\s*(?:\(\s*({NUM})\s*\)|({NUM}))", lambda m, s=s: self.plus(m, s), s)
            s = re.sub(rf"\*\s*{TOK}", lambda m, s=s: self.deref(m, s), s)
            s = re.sub(rf"{TOK}\s*\[\s*({NUM})\s*\]", self.index, s)
            if s == before:
                break
        return s

    def cast(self, m, s):
        n = self.nodes[int(m[2])]
        if n[0] != "addr" or "*" not in m[1] or not unary_before(s, m.start()) or re.match(r"\s*(\[|->|\.)", s[m.end():]):
            return m[0]
        return self.tok(("addr", n[1], " ".join(m[1].split())))

    def plus(self, m, s):
        n = self.nodes[int(m[1])]
        if n[0] != "addr" or re.match(r"\s*[*/%]", s[m.end():]):
            return m[0]
        if n[2] is None:
            unit = 1 if not self.dims and self.ctype in BYTE_TYPES else None
        else:
            pt = pointee(n[2])
            unit = pt[1] if pt else None
        if not unit:
            return m[0]
        k = int(m[3] or m[4], 0) * (1 if m[2] == "+" else -1)
        return self.tok(("addr", n[1] + k * unit, n[2]))

    def deref(self, m, s):
        n = self.nodes[int(m[1])]
        if n[0] != "addr" or not n[2] or not unary_before(s, m.start()) or re.match(r"\s*(\[|->|\.)", s[m.end():]):
            return m[0]
        pt = pointee(n[2])
        if not pt or not pt[0] or not pt[1]:
            return m[0]
        return self.tok(("acc", n[1], pt[0], pt[1]))

    def index(self, m):
        n = self.nodes[int(m[1])]
        if n[0] != "addr" or not n[2]:
            return m[0]
        pt = pointee(n[2])
        if not pt or not pt[0] or not pt[1]:
            return m[0]
        return self.tok(("acc", n[1] + int(m[2], 0) * pt[1], pt[0], pt[1]))

    def write(self, s):
        def final(m):
            n = self.nodes[int(m[1])]
            before, after = s[:m.start()], s[m.end():]
            if n[0] == "arr":
                p = piece_at(self.pieces, self.base)
                if p is None or not p.suffix or elem_of(p)[1] != (type_size(self.ctype) or 0):
                    raise ValueError(f"{self.name}: no table of the same element at {self.base:#x}")
                return p.name if norm(elem_of(p)[0]) == norm(self.ctype) else f"(({self.ctype} *){p.name})"
            a = self.base + n[1]
            if n[0] == "addr":
                p = piece_at(self.pieces, a)
                if p is None:
                    raise ValueError(f"{self.name}: no static at {a:#x}")
                return emit_addr(p, a, n[2], after)
            _, _, elem, esz = n
            if not elem or not esz:
                raise ValueError(f"{self.name}: an access of unknown size")
            p = piece_at(self.pieces, a, esz)
            if p is None:
                raise ValueError(f"{self.name}: no static holds the {esz} bytes at {a:#x}")
            lvalue = bool(LVALUE_AFTER.match(after)) or bool(re.search(r"(?:\+\+|--)\s*$", before)) or \
                bool(re.search(r"&\s*$", before) and unary_before(before, len(before.rstrip()) - 1))
            return emit_acc(p, a, elem, esz, lvalue)
        return re.sub(TOK, final, s)


def rewrite(text, name, base, pieces, declared, skip=()):
    """text with the uses of the extern/static block `name` (at address base; declared: (ctype, dims) as the C spells it)
    rewritten onto the pieces (statics at absolute addresses). skip: (start, end) spans of its declarations, left as they
    are. Returns (new text, None) or (None, why it can't)."""
    ctype, dims = declared
    if dims and not re.fullmatch(r"\[\s*(?:0x[0-9a-fA-F]+|\d*)\s*\]", dims):
        return None, f"{name} is a table of more than one dimension"
    if not dims and not type_size(ctype):  # a struct: its static takes the name (x.f stays x.f)
        p = piece_at(pieces, base)
        if p is None or p.addr != base or p.kind != "struct":
            return None, f"{name} is of a type of unknown size ({ctype}) and no struct static is at {base:#x}"
        out, last = [], 0
        for a, e in sorted(skip) + [(len(text), len(text))]:
            out.append(re.sub(rf"(?<![\w.]){name}\b", p.name, text[last:a]))
            out.append(text[a:e])
            last = e
        return "".join(out), None
    rw = Rewriter(name, base, pieces, declared)
    out, last = [], 0
    try:
        for a, e in sorted(skip) + [(len(text), len(text))]:
            out.append(rw.write(rw.reduce(text[last:a])))
            out.append(text[a:e])
            last = e
    except ValueError as e:
        return None, str(e)
    new = "".join(out)
    kept = "".join(text[a:e] for a, e in sorted(skip))
    if LO in new or len(re.findall(rf"(?<![\w.]){name}\b", new)) != len(re.findall(rf"(?<![\w.]){name}\b", kept)):
        return None, f"{name}: a use is not one of the forms tools/bss_dwarf.py rewrites"
    return new, None


# --- names: which of the equal-sized objects is at which address -------------------------------------------------

def prebuilt_uses(dest):
    """({(archive, member, function): [the .bss objects it touches, in the order of first touch]}, {that key: Black's
    function it is paired with}) for the functions of the members paired with dest. The
    function's ARM code is read for ldr/str with an immediate offset from a register loaded from a pool word that
    relocates to .bss (the section's base plus the offset, as Black's code has it) or to an object's own symbol."""
    import struct
    import prebuilt_dwarf as pdw
    import prebuilt_map as pm
    from port_file import parse_object
    from collections import namedtuple
    pairs = [p for p in pdw.pairing().values() if p.tree_file == dest]
    paired = {(p.archive, p.member, p.func): p.black for p in pairs}
    out, bodies = {}, {}
    todo = []
    for p in pairs:
        key = (p.archive, p.member)
        if key not in bodies:
            data = open(os.path.join(pdw.REF, p.archive), "rb").read()
            bodies[key] = next((parse_object(b) for n, b in pm.members(data) if n == p.member), None)
            if bodies[key] is not None:
                todo += [(key, s) for s in bodies[key][2] if s[5] and s[3]]
    for key, fn in todo:
        d, secs, syms, relocs = bodies[key]
        if fn[1] not in relocs:
            continue
        sec = fn[1]
        base_off, size = secs[sec][2], secs[sec][3]
        pool = {}
        for off, typ, symi, add in relocs[sec]:
            tsym = syms[symi] if symi < len(syms) else None
            if typ == 2 and tsym and 0 < tsym[1] < len(secs) and secs[tsym[1]][1] == 8:
                pool[off] = (tsym, add or struct.unpack_from("<i", d, base_off + off)[0])

        def object_at(tsym, off):
            if tsym[3]:
                return re.sub(r"[$]\d+$", "", tsym[0])
            hit = next((s for s in syms if s[1] == tsym[1] and s[3] and not s[5] and s[2] <= off < s[2] + s[3]), None)
            return re.sub(r"[$]\d+$", "", hit[0]) if hit else None

        reg, pending, seq = {}, set(), []

        def touch(r, off):
            tsym, add = reg[r]
            nm = object_at(tsym, add + off if not tsym[3] else off)
            if tsym[3]:  # the object's own symbol
                nm = re.sub(r"[$]\d+$", "", tsym[0])
            if nm and nm not in seq:
                seq.append(nm)
        for pc in range(0, size, 4):
            w = struct.unpack_from("<I", d, base_off + pc)[0]
            cond, kind = w >> 28, (w >> 26) & 3
            if cond == 15:
                continue
            rn, rd = (w >> 16) & 15, (w >> 12) & 15
            if kind == 1 and not (w >> 25) & 1:  # ldr/str with an immediate offset
                off = (w & 0xFFF) * (1 if (w >> 23) & 1 else -1)
                if rn == 15:
                    pw = pc + 8 + off
                    if (w >> 20) & 1 and pw in pool:
                        reg[rd] = pool[pw]
                        pending.add(rd)
                    else:
                        reg.pop(rd, None)
                elif rn in reg:
                    pending.discard(rn)
                    touch(rn, off)
                    if (w >> 20) & 1:
                        reg.pop(rd, None)
                elif (w >> 20) & 1:
                    reg.pop(rd, None)
            elif kind == 0 and (w >> 25) & 1 and (w >> 21) & 15 == 4 and rn in reg:  # add rd, rn, #imm
                imm = w & 0xFF
                rot = ((w >> 8) & 15) * 2
                imm = ((imm >> rot) | (imm << (32 - rot))) & 0xFFFFFFFF if rot else imm
                tsym, add = reg[rn]
                reg[rd] = (tsym, add + imm)
                pending.discard(rn)
                pending.add(rd)
            elif kind == 0 and not (w >> 25) & 1 and (w >> 21) & 15 == 13 and (w & 0xFFF) < 16 and (w & 15) in reg:
                reg[rd] = reg[w & 15]  # mov rd, rm
                if (w & 15) in pending:
                    pending.add(rd)
            elif (w >> 24) & 15 == 11:  # bl: an address in r0-r3 is an argument
                for r_ in range(4):
                    if r_ in reg and r_ in pending:
                        touch(r_, 0)
                    reg.pop(r_, None)
                    pending.discard(r_)
            elif kind == 0 and not (w >> 20) & 1 + 0 and rd in reg:
                reg.pop(rd, None)
        if seq:
            out[(key[0], key[1], fn[0])] = seq
    return out, paired


def votes(black_seq, slots, uses, objs):
    """{slot address: Counter(object name)}. A Black function whose sequence of distinct slots is as long as that of the
    prebuilt function it is paired with (same sizes, pairwise) votes with it; one that is not (the pairing is by
    similarity) votes with the prebuilt function that is the only one of that size pattern, if it is the only Black
    function of the pattern too (and has at least 3 objects)."""
    from collections import Counter, defaultdict
    size = {o.name: o.size for o in objs}
    use, paired = uses
    out = defaultdict(Counter)
    pre = {k: [n for n in s if n in size] for k, s in use.items()}
    pattern = lambda sizes: tuple(sizes)  # noqa: E731
    by_pattern = defaultdict(list)
    for k, s in pre.items():
        by_pattern[pattern([size[n] for n in s])].append(k)
    black_by_pattern = defaultdict(list)
    for fn, seq in black_seq.items():
        if seq:
            black_by_pattern[pattern([slots[a] for a in seq])].append(fn)
    mine = {v: k for k, v in paired.items()}
    for fn, seq in black_seq.items():
        if not seq:
            continue
        k = mine.get(fn)
        s = pre.get(k)
        if s is not None and len(s) == len(seq) and all(size[n] == slots[a] for n, a in zip(s, seq)):
            pass
        else:
            pat = pattern([slots[a] for a in seq])
            cands = by_pattern.get(pat, [])
            if len(seq) < 3 or len(cands) != 1 or len(black_by_pattern[pat]) != 1:
                continue
            s = pre[cands[0]]
        for n, a in zip(s, seq):
            out[a][n] += 1
    return out


def assign_names(slots, objs, vote):
    """{slot address: Obj or None} for slots = {address: size}: an object goes to a slot by its votes, then a slot that is the
    only one left of its size gets the only candidate left (when there are as many)."""
    result, taken = {}, set()
    cand = {}
    for o in objs:
        cand.setdefault(o.size, []).append(o)
    by_name = {o.name: o for o in objs}
    ranked = sorted(((n, a, c) for a, cs in vote.items() for n, c in cs.items()), key=lambda x: -x[2])
    for n, a, c in ranked:
        if a in slots and a not in result and n not in taken and by_name[n].size == slots[a]:
            # a slot with a rival of equal votes is not decided
            rivals = [x for x in ranked if x[1] == a and x[0] != n and x[2] >= c]
            others = [x for x in ranked if x[0] == n and x[1] != a and x[2] >= c]
            if rivals or others:
                continue
            result[a] = by_name[n]
            taken.add(n)
    for size, cs in cand.items():
        free = [a for a, s in slots.items() if s == size and a not in result]
        left = [o for o in cs if o.name not in taken]
        if len(free) == 1 and len(left) == 1:
            result[free[0]] = left[0]
            taken.add(left[0].name)
        elif len(set(o.decl for o in left)) == 1 and left and free and len(cs) == len(slots_of_size(slots, size)):
            pass
    return result


def slots_of_size(slots, size):
    return [a for a, s in slots.items() if s == size]
