#!/usr/bin/env python3
"""A struct's field map from all of Black's assembly, written in C here or not, against the layout here.

The functions: those whose prototypes here (headers and C files) take a pointer to the struct, from the start, and the
callers of those that return one, from the call. Each one's assembly (build/b_us/asm, which `ninja asm` writes) is
read in order, following the registers that hold the pointer plus an offset: moves, adds and subtracts of constants
(mov, lsl, a pool word), stack slots. A load or store through one is a field at that offset, with its width and
signedness; through the pointer plus a scaled register, an array with that stride; the pointer plus an offset passed to
a call, a field's address (a struct or array the callee takes). Branches are read in order: at a label the registers
keep only what every branch to it seen so far and the fall-through (unless the code before ends in a branch or a
return) agree on, so a register one path moves doesn't misread the next path's accesses; a label reached only from a
later branch keeps what was there. After khdays-decomp's tools/accsweep.py and structviews.py (CC0).

Each offset is checked against clang's layout here: past the end, in padding, across two fields, inside a field that
isn't an array, or of another width than the field's are Black's code disagreeing with it.

    python3 tools/struct_map.py CGearSw          # each offset Black's code reaches, the field here, what disagrees
    python3 tools/struct_map.py CGearSw --odd    # only what disagrees
"""
import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import alloc_sizes  # noqa: E402  (re-runs this in the venv)
import check_layouts as cl  # noqa: E402
import near_twins  # noqa: E402
import port_decls as pd  # noqa: E402

ROOT = cl.ROOT
COND = r"(?:eq|ne|cs|cc|hs|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al)?"
MEMOP = re.compile(rf"(ldr|str)(sb|sh|b|h|d)?{COND}$")
MULTI = re.compile(rf"(ldm|stm)(ia|ib|da|db|fd|ed|fa|ea)?{COND}$")
ARITH = re.compile(rf"(mov|add|sub|lsl|mul){COND}s?{COND}$")
WIDTH = {None: 4, "b": 1, "h": 2, "sb": 1, "sh": 2, "d": 8}
SCRATCH = ("r0", "r1", "r2", "r3", "ip")
ALLOCS = {"GFL_HeapAllocate": "r1", "GFL_ProcInitSubsystem": "r1", "GFL_HeapAllocateCore": "r1"}  # size


def read_asm():
    """({function: [instructions]}, {label: number} of the literal pool words, {function: its callers}) of
    build/b_us/asm. The overlays share addresses, so each file's labels get its number (.L_0219... is .L3_0219...).
    A call dsd can't tell the overlay of (`bl A ; B`: A or B, the overlays at one address) is `bl ?A`."""
    funcs, pool, callers = {}, {}, defaultdict(set)
    for n, s in enumerate(sorted((ROOT / "build/b_us/asm").rglob("*.s"))):
        cur = None
        for line in s.read_text(errors="replace").replace(".L_", f".L{n}_").splitlines():
            m = re.match(r"(\w+): ; 0x", line)
            if m:
                name, cur = m[1], funcs.setdefault(m[1], [])
                continue
            m = re.match(r"(\.L\w+): \.word (0x[0-9a-fA-F]+|\d+|(?:data|bss)_\w+)\s*$", line)
            if m:
                pool[m[1]] = int(m[2], 0) if m[2][0].isdigit() else m[2]  # a number, or a datum's address
            elif re.match(r"\s+(thumb|arm)_func_end", line):
                cur = None
            elif cur is not None and re.match(r"\.L\w+:", line):  # a label: walk() merges the paths to it
                cur.append(line.split(":")[0] + ":")
            elif cur is not None and re.match(r"\s+\.short (\.L\w+) - ", line):  # a jump table's case
                cur.append("case " + line.split()[1])
            elif cur is not None and line.startswith("    ") and not line.strip().startswith("."):
                cur.append(line.split(";")[0].strip())
                if re.match(r"\s+blx?\s+\w+\s*;\s*\w", line):  # dsd's guess, the other overlays' functions after it
                    cur[-1] = re.sub(r"(blx?\s+)", r"\1?", cur[-1])
                m = re.match(r"blx?\s+(\w+)$", cur[-1])
                if m:
                    callers[m[1]].add(name)
    return funcs, pool, callers


def operands(text):
    """The operands of an instruction, split at the commas outside brackets and braces."""
    out, depth, cur = [], 0, ""
    for c in text:
        if c in "[{":
            depth += 1
        elif c in "]}":
            depth -= 1
        if c == "," and not depth:
            out.append(cur.strip())
            cur = ""
        else:
            cur += c
    return out + [cur.strip()] if cur.strip() else out


def imm(text):
    m = re.fullmatch(r"#(-?(?:0x[0-9a-fA-F]+|\d+))", text.strip())
    return int(m[1], 0) if m else None


def trace(lines, entry, pool, ret_of, field_of, events, fn, global_of=None):
    """Reads a function's assembly in order (the module docstring's reading) with each register's pointer typed:
    ("p", offset, stride, type), type a struct key or an unknown's symbol (tools/struct_infer.py's pointer flow).
    entry: {register: type}; ret_of(callee): the type its return points to, or None; field_of(type, offset): the type
    of the pointer a word loaded from there holds, or None; global_of(datum): the type its address points to, or
    None. events: lists "acc" [(type, offset, width, what, stride)], "call" [(callee, register, type, offset)],
    "store" [(type, offset, stored type)], "ret" [type], "new" [(symbol, size)] (a block allocated, its size).
    Also (tools/struct_detect.py): "span" [(type, origin, offset, width, what)] the accesses through a pointer made by
    adding a constant to a struct's pointer (origin: the offset it was made at: a nested struct's or array's start);
    "bits" [(type, offset, width, signed, position, bits, how)] a loaded field cut down to some of its bits by a
    shift pair or a shift and a mask."""
    st = {r: ("p", 0, None, t) for r, t in entry.items()}
    slots = {}
    spans, bits = events.setdefault("span", []), events.setdefault("bits", [])
    loaded = {}  # {register: (type, offset, width, signed)} a field just loaded, or (..., shift left, shift right)
    branched, dead = defaultdict(list), False
    acc, calls, stores, rets, new = (events[k] for k in ("acc", "call", "store", "ret", "new"))
    allocated = 0

    def merge(states):
        return {r: v for r, v in states[0].items() if v is not None and all(x.get(r) == v for x in states[1:])}

    def val(x):
        x = x.strip()
        if x.startswith("#"):
            return ("c", imm(x))
        m = re.fullmatch(r"(\w+),\s*lsl\s+#(\w+)", x)
        if m:
            v = st.get(m[1])
            return ("c", v[1] << int(m[2], 0)) if v and v[0] == "c" else ("s", 1 << int(m[2], 0))
        return st.get(x)

    def add(a, b, sign=1, derive=False):
        """a + b; the pointer keeps (or, with derive, gets) an origin: the offset it was first moved to from the start."""
        if not a or not b:
            return None
        if b[0] == "p" and sign > 0:
            a, b = b, a
        if a[0] == "c" and b[0] == "c":
            return ("c", a[1] + sign * b[1])
        if a[0] == "p" and b[0] == "c":
            new = a[1] + sign * b[1]
            org = a[4] if len(a) > 4 else new if derive and new != a[1] and not a[1] else None
            return ("p", new, a[2], a[3]) + ((org,) if org is not None else ())
        if a[0] == "p" and b[0] == "s" and sign > 0:
            return ("p", a[1], b[1], a[3]) + (a[4:5] if len(a) > 4 else ())
        return None

    def ret():
        v = st.get("r0")
        if v and v[0] == "p" and not v[1] and v[2] is None:
            rets.append(v[3])

    for line in lines:
        if line.endswith(":"):
            paths = branched.pop(line[:-1], []) + ([] if dead else [st])
            if paths:
                st = merge(paths)
            dead = False
            loaded.clear()
            continue
        op, _, rest = line.partition(" ")
        op, ops = op.lower(), operands(rest)
        was = dict(loaded)
        if op in ("bl", "blx") or MULTI.fullmatch(op) or op == "pop":
            loaded.clear()
        elif ops and not NOWRITE.match(op) and re.fullmatch(r"r\d+|ip", ops[0]):
            loaded.pop(ops[0], None)
        bit_op(op, ops, st, was, loaded, bits)
        if op == "case" or re.fullmatch(rf"b{COND}", op) and ops and ops[0].startswith(".L"):
            branched[ops[0]].append(dict(st))
            dead = op in ("b", "bal")
            continue
        if op == "bx" and ops == ["lr"] or op == "pop" and "pc" in rest:
            ret()
        if op == "bx" or op == "pop" and "pc" in rest or ops and ops[0] == "pc" and not op.startswith(("cmp", "str")):
            dead = True
        m = MEMOP.fullmatch(op)
        if m and len(ops) >= 2:
            if ops[1].startswith(".L") or ops[1].startswith("="):
                v = pool.get(ops[1])
                t = global_of(v) if isinstance(v, str) and global_of else None
                st[ops[0]] = ("c", v) if isinstance(v, int) else ("p", 0, None, t) if t else None
                continue
            inner = re.match(r"\[([^\]]*)\]", ops[1])
            if not inner:
                st[ops[0]] = None
                continue
            parts = [x.strip() for x in inner[1].split(",", 1)]
            base, off = st.get(parts[0]), val(parts[1]) if len(parts) > 1 else ("c", 0)
            if len(ops) > 2:
                off = ("c", 0)
            at = add(base, off) if base else None
            if parts[0] == "sp" and off and off[0] == "c":
                if m[1] == "str":
                    slots[off[1]] = st.get(ops[0])
                else:
                    st[ops[0]] = slots.get(off[1])
                continue
            hit = at and at[0] == "p" and 0 <= at[1] < 0x100000
            if hit:
                what = "st" if m[1] == "str" else "lds" if m[2] in ("sb", "sh") else "ld"
                acc.append((at[3], at[1], WIDTH[m[2]], what, at[2]))
                if len(at) > 4 and at[2] is None:
                    spans.append((at[3], at[4], at[1], WIDTH[m[2]], what))
                if m[1] == "ldr" and at[2] is None and len(ops) == 2:
                    loaded[ops[0]] = (at[3], at[1], WIDTH[m[2]], m[2] in ("sb", "sh"))
                v = st.get(ops[0])
                if m[1] == "str" and m[2] is None and v and v[0] == "p" and not v[1] and v[2] is None:
                    stores.append((at[3], at[1], v[3]))
            if m[1] == "ldr":
                t = field_of(at[3], at[1]) if hit and m[2] is None and at[2] is None else None
                st[ops[0]] = ("p", 0, None, t) if t else None
                if m[2] == "d":
                    st[f"r{int(ops[0][1:]) + 1}"] = None
            continue
        m = MULTI.fullmatch(op)
        if m or op in ("push", "pop"):
            regs = re.findall(r"\b(r\d+|ip|lr|pc)\b", ops[-1])
            base = st.get(ops[0].rstrip("!")) if m else None
            if base and base[0] == "p":
                for i, r in enumerate(regs):
                    acc.append((base[3], base[1] + 4 * i, 4, "ld" if m[1] == "ldm" else "st", None))
            if op.startswith(("ldm", "pop")):
                for r in regs:
                    st[r] = None
            continue
        if op in ("bl", "blx") and ops:
            callee = ops[0]
            for r in ("r0", "r1", "r2", "r3"):
                v = st.get(r)
                if v and v[0] == "p" and v[2] is None:
                    calls.append((callee, r, v[3], v[1]))
            size = st.get(ALLOCS.get(callee, ""))
            for r in SCRATCH:
                st[r] = None
            t = ret_of(callee)
            if size and size[0] == "c" and 0 < size[1] < 0x100000:
                allocated += 1
                t = f"new:{fn}:{allocated}"
                new.append((t, size[1]))
            if t:
                st["r0"] = ("p", 0, None, t)
            continue
        m = ARITH.fullmatch(op)
        if m and ops:
            d, src = ops[0], ops[1:]
            if len(src) > 1 and src[-1].startswith("lsl"):
                src = src[:-2] + [f"{src[-2]}, {src[-1]}"]
            if m[1] == "mov":
                st[d] = val(src[0]) if src else None
            elif m[1] == "lsl":
                v = val(src[0]) if len(src) == 2 else st.get(d)
                n = imm(src[-1]) if src else None
                st[d] = None if n is None else ("c", v[1] << n) if v and v[0] == "c" else ("s", 1 << n)
            elif m[1] == "mul":
                a, b = (st.get(d), val(src[0])) if len(src) == 1 else (val(src[0]), val(src[1]))
                c = [x[1] for x in (a, b) if x and x[0] == "c"]
                st[d] = ("c", c[0] * c[1]) if len(c) == 2 else ("s", c[0]) if c else None
            else:
                a, b = (st.get(d), val(src[0])) if len(src) == 1 else (val(src[0]), val(src[1]))
                st[d] = add(a, b, 1 if m[1] == "add" else -1, derive=True)
            continue
        if ops and not re.fullmatch(rf"(cmp|cmn|tst|teq|b|bx){COND}", op) and re.fullmatch(r"r\d+|ip", ops[0]):
            st[ops[0]] = None
    return events


NOWRITE = re.compile(rf"(cmp|cmn|tst|teq|str|push|stm)|bx?{COND}$|blx?{COND}$")


def bit_op(op, ops, st, was, loaded, bits):
    """Notes a field cut down to some of its bits (trace()): lsl then lsr/asr of one loaded field, or lsr then and, or
    and with a contiguous mask smaller than the field; `loaded` keeps the shifted value for the next step. was:
    `loaded` before the instruction cleared its destination."""
    m = re.fullmatch(rf"(lsl|lsr|asr|and|ands){COND}s?", op)
    if not m or len(ops) < 2:
        return
    d, kind = ops[0], m[1]
    if kind in ("and", "ands"):
        src = ops[1] if len(ops) == 3 and ops[1] in was else d if d in was else None
        other = ops[2] if len(ops) == 3 else ops[1]
        mask = imm(other) if other.startswith("#") else (st.get(other) or (None, None))[1] if (
            st.get(other) or ("",))[0] == "c" else None
    else:
        src = ops[1] if len(ops) == 3 else d
        mask = imm(ops[-1])
    cur = was.get(src)
    if not cur or mask is None or mask < 0:
        return
    typ, off, width, signed = cur[:4]
    sh = cur[4:]
    unit = width * 8
    if kind == "lsl" and not sh:
        loaded[d] = (typ, off, width, signed, "l", mask)
    elif kind in ("lsr", "asr") and sh[:1] == ("l",):
        pos, size = mask - sh[1], 32 - mask
        if size > 0 and pos >= 0 and not (pos == 0 and size >= unit):
            bits.append((typ, off, width, signed or kind == "asr", pos, size, "lsl+" + kind))
    elif kind in ("lsr", "asr") and not sh and 0 < mask < unit:
        loaded[d] = (typ, off, width, signed, "r", mask)
    elif kind in ("and", "ands") and mask and mask & (mask + 1) == 0:  # 2^k - 1
        pos, size = (sh[1] if sh[:1] == ("r",) else 0), mask.bit_length()
        if pos + size < unit or sh[:1] == ("r",) and pos:
            bits.append((typ, off, width, signed, pos, size, "lsr+and" if sh else "and"))


def walk(lines, entry, returns, pool):
    """[(offset, width, what, stride)] of a function's accesses through the pointer: what is "ld", "lds" (a signed
    load), "st", or "&callee" for its address passed to a call. entry: {register: offset} of the pointer at the start;
    returns: the functions that return one."""
    ev = {"acc": [], "call": [], "store": [], "ret": [], "new": []}
    trace(lines, {r: "it" for r in entry}, pool, lambda c: "it" if c in returns else None, lambda t, o: None, ev, "")
    return [(o, w, what, stride) for t, o, w, what, stride in ev["acc"] if t == "it"] + \
        [(o, 0, f"&{c}", None) for c, r, t, o in ev["call"] if t == "it" and o]


PROTO = re.compile(r"^([\w \t\*]+?)\b(\w+)\s*\(([^()]*)\)\s*(?:;|\{)", re.M)
POINTER = r"(?:const\s+)?(?:(?:struct|union)\s+)?(\w+)\s*\*(?!\s*\*)"


def prototypes():
    """{type: ({function: {register: 0}}, {functions}) of the functions whose prototypes here (headers and C files)
    take a pointer to the type in a register, and those that return one."""
    out = defaultdict(lambda: (defaultdict(dict), set()))
    paths = pd.header_files(ROOT) + sorted(str(p.relative_to(ROOT)) for p in (ROOT / "src").rglob("*.[hc]"))
    for path in paths:
        for m in PROTO.finditer(pd.strip_comments((ROOT / path).read_text(errors="replace"))):
            r = re.fullmatch(rf"(?:(?:static|inline|extern)\s+)*{POINTER}\s*", m[1].strip() + " ")
            if r:
                out[r[1]][1].add(m[2])
            for i, p in enumerate(m[3].split(",")[:4]):
                q = re.match(rf"\s*{POINTER}\s*\w*\s*$", p)
                if q:
                    out[q[1]][0][m[2]][f"r{i}"] = 0
    return out


def field_map(names, protos, asm, pool, callers):
    """({offset: {"widths", "what", "fns", "strides"}}, how many functions) of the accesses through a pointer to the
    struct (names: its tag's and typedefs') in Black's functions that take one or call one that returns one."""
    takes, returns = defaultdict(dict), set()
    for n in names:
        if n in protos:
            for fn, regs in protos[n][0].items():
                takes[fn].update(regs)
            returns |= protos[n][1]
    seen = defaultdict(lambda: {"widths": set(), "what": set(), "fns": set(), "strides": set()})
    todo = {fn for fn in set(takes) | {c for r in returns for c in callers.get(r, ())} if fn in asm}
    for fn in sorted(todo):
        for o, w, what, stride in walk(asm[fn], takes.get(fn, {}), returns, pool):
            s = seen[o]
            s["widths"].add(w)
            s["what"].add(what if not what.startswith("&") else "&")
            s["fns"].add(fn if not what.startswith("&") else f"{fn}>{what[1:]}")
            if stride:
                s["strides"].add(stride)
    return seen, len(todo)


def layout(name):
    """(record, (size, rows, {path: the element's layout} of its arrays of structs)) of the struct as clang lays it
    out here (near_twins.record_layouts)."""
    path, key = alloc_sizes.defined_structs()[name]
    if path.endswith(".h"):
        # a src/ header isn't under an include directory: by its path (src/ov135/worldtrade_local.h, sx8)
        name = str(ROOT / path) if path.startswith("src/") else pd.include_name(path)
        (ROOT / "build" / "struct_map.c").write_text(f'#include "{name}"\n')
        path = "build/struct_map.c"
    text = (ROOT / path).read_text() + f"\ntypedef char __lay0[sizeof({key})];\n"
    out = cl.clang(path, "-Xclang", "-fdump-record-layouts", text=text)
    size, rows = near_twins.record_layouts(out, {key})[key]
    arrays = struct_arrays(out, key)
    elems = near_twins.record_layouts(out, {n for t in arrays.values() for n in (t, f"struct {t}")})
    elems = {path: elems.get(t) or elems.get(f"struct {t}") for path, t in arrays.items()}
    return key, (size, rows, {path: (*e, {}) for path, e in elems.items() if e})


def struct_arrays(out, key):
    """{path: element type} of the record's arrays whose elements are structs, from clang's layout dump, which lays
    out no array's elements."""
    for block in out.split("*** Dumping AST Record Layout")[1:]:
        lines = [x for x in block.strip("\n").splitlines() if x.strip()]
        if not lines or lines[0].split("|", 1)[-1].strip() != key:
            continue
        found, names = {}, {}
        for x in lines[1:]:
            if "sizeof=" in x:
                break
            off, _, field = x.partition("|")
            if not off.strip().split(":")[0].isdigit():
                continue
            depth = (len(field) - len(field.lstrip())) // 2
            names[depth] = field.split()[-1]
            kind = field.strip().rsplit(" ", 1)[0]
            base = re.sub(r"\[\d+\]|\b(const|volatile)\b", "", kind).strip()
            if "[" in kind and not base.endswith("*") and base not in near_twins.SCALAR:
                found[".".join(names[d] for d in range(1, depth + 1))] = base
        return found
    return {}


def verdict(o, w, lay):
    """("field here", what disagrees or "") of an access of w bytes at o (w 0: an address)."""
    size, rows, structs = lay if len(lay) > 2 else (*lay, {})
    if o >= size:
        return "", f"past the end ({size:#x})"
    if o < 0:
        return "", "before the start"
    leaves = [r for i, r in enumerate(rows) if r[0] <= o < r[0] + r[1] and not any(
        x[2] > r[2] and r[0] <= x[0] < r[0] + r[1] for x in rows[i + 1:])]
    if not leaves:
        outer = [r for r in rows if r[0] <= o < r[0] + r[1]]
        return (outer[-1][3] if outer else ""), "padding"
    start, n, _, path, elem, count = leaves[-1]
    at = path + (f"+{o - start:#x}" if o != start else "")
    if count > 1 and path in structs:  # an array of structs: the access against the element's layout
        inner, bad = verdict((o - start) % elem, w, structs[path])
        return f"{path}[{(o - start) // elem}]" + (f".{inner}" if inner else ""), bad
    if count > 1 and elem > 8:  # an array of structs whose type clang doesn't name: their fields aren't laid out here
        return at, ""
    if not w:
        return at, "" if (o - start) % (elem or n) == 0 else "inside a field"
    if count == 1 and elem and n < elem and w <= elem and o % elem + w <= elem:  # a bitfield's unit, whole
        return at, ""
    if o + w > start + n:
        return at, "across two fields"
    if count > 1:
        return at, "" if (o - start) % elem == 0 and w <= elem else f"elements of {elem}"
    if elem == 8 and w == 4 and o - start in (0, 4):  # a 64-bit field, loaded in halves
        return at, ""
    if o != start:
        return at, "inside a field"
    return at, "" if elem is None or w == elem else f"{elem} bytes here"


def past_end(seen, size):
    """{offset: functions} of the loads and stores past the struct's end here, not further than its size again (a
    misread constant)."""
    return {o: {fn for fn in s["fns"] if ">" not in fn} for o, s in seen.items()
            if size <= o < 2 * size and s["what"] - {"&"}}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("struct")
    ap.add_argument("--odd", action="store_true", help="only the offsets that disagree with the layout here")
    args = ap.parse_args()
    name = args.struct.split()[-1]
    defined = alloc_sizes.defined_structs()
    if name not in defined:
        sys.exit(f"{name}: no struct of that name in the headers or src/")
    key, lay = layout(name)
    asm, pool, callers = read_asm()
    seen, walked = field_map({name, key.split()[-1]} | {n for n, (_, k) in defined.items() if k == key},
                             prototypes(), asm, pool, callers)
    odd = 0
    print(f"{key}: {lay[0]:#x} bytes here; {walked} of Black's functions have a pointer to it, "
          f"{len(seen)} offsets reached, the last {max(seen, default=0):#x}")
    for o in sorted(seen):
        s = seen[o]
        w = max(s["widths"])
        here, bad = verdict(o, w, lay)
        odd += bool(bad)
        if args.odd and not bad:
            continue
        widths = "/".join(str(x) if x else "&" for x in sorted(s["widths"]))
        kind = ",".join(sorted(s["what"]))
        strides = f" stride {'/'.join(f'{x:#x}' for x in sorted(s['strides']))}" if s["strides"] else ""
        fns = sorted(s["fns"])
        print(f"  {o:#06x} {widths:5} {kind:9} {len(fns):3} fns  {here:32} {bad}{strides}  "
              f"{', '.join(fns[:2])}{' ...' if fns[2:] else ''}")
    print(f"{odd} of {len(seen)} offsets disagree with the layout here")


if __name__ == "__main__":
    main()
