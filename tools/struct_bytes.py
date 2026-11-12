"""tools/struct_detect.py's detectors over the C files' constant-offset accesses (tools/struct_scan.py): bytes (a
scalar pointer read as a struct) and typed (a struct pointer read by byte offsets: fields to name, the struct too
small)."""
import re
from collections import Counter, defaultdict

import struct_scan as ss

SCALAR = re.compile(r"(?:const\s+|volatile\s+)*(?:u8|s8|u16|s16|u32|s32|u64|s64|char|short|int|long|void|BOOL|fx32|"
                    r"fx16|f32|unsigned(?:\s+\w+)?|signed\s+\w+|float|uintptr_t|size_t|HeapID|\w+\s*\*)")


def scalar_pointee(written):
    """The pointee name of a pointer or array type of C text when it is a number or void, else None."""
    p = ss.pointee(written)
    return p if p is not None and SCALAR.fullmatch(p.strip()) else None


class Graph:
    """The scanned functions with their pointers linked: a pointer passed to a call (at an offset) is the callee's
    parameter at that offset; nodes are ("p", function, index) for parameters, ("l", function, id) for locals and
    ("g", name) for globals."""

    def __init__(self, scans, matched):
        self.fn = {}  # {name: (file, record)}
        self.dups = set()
        for f, fns in scans.items():
            for name, rec in fns.items():
                if name in self.fn:
                    self.dups.add(name)
                self.fn[name] = (f, rec)
        for name in self.dups:
            self.fn.pop(name, None)
        self.matched = matched
        self.up = {}
        self.acc = defaultdict(list)  # {node: [(offset, width, signed, form, function, range)]}
        self.typed = defaultdict(Counter)  # {node: Counter of (callee struct type name, offset)}
        self.decl = {}  # {node: (written type, name)}
        for name, (f, rec) in self.fn.items():
            pidx = {p[3]: i for i, p in enumerate(rec["params"])}
            for a in rec["acc"]:
                n = self.node(name, rec, a[0], pidx)
                self.acc[n].append((a[1], a[2], a[3], a[4], name, (a[5], a[6], a[7] if len(a) > 7 else "")))
            for callee, i, d, off in rec["calls"]:
                n = self.node(name, rec, d, pidx)
                if callee in self.fn:
                    crec = self.fn[callee][1]
                    if i < len(crec["params"]):
                        c = ("p", callee, i)
                        self.decl.setdefault(c, (crec["params"][i][2], crec["params"][i][0]))
                        want = crec["params"][i][2]
                        if scalar_pointee(want) is None and ss.pointee(want) is not None:
                            self.typed[n][ss.pointee(want).replace("struct ", ""), off] += 1
                        same = self.fn[callee][0] == f and scalar_pointee(want) == scalar_pointee(rec["types"].get(d, ""))
                        pt = scalar_pointee(want) or ""
                        if (off or same) and not pt.endswith("char") and pt != "void":
                            self.link(c, n, off)

    def node(self, fn, rec, d, pidx):
        name = rec["names"].get(d, "")
        n = ("g", name) if name.startswith("::") else ("p", fn, pidx[d]) if d in pidx else ("l", fn, d)
        self.decl.setdefault(n, (rec["types"].get(d, ""), name))
        return n

    def find(self, n):
        """(root, offset of n from the root's start)."""
        off = 0
        while n in self.up:
            n, d = self.up[n]
            off += d
        return n, off

    def link(self, child, parent, delta):
        """child's start is parent's start plus delta."""
        (ra, oa), (rb, ob) = self.find(child), self.find(parent)
        if ra == rb:
            return
        # ra + oa is child; rb + ob is parent; child = parent + delta -> ra = rb + ob + delta - oa
        self.up[ra] = (rb, ob + delta - oa)

    def components(self):
        out = defaultdict(list)
        nodes = set(self.acc) | set(self.up) | {v[0] for v in self.up.values()}
        for n in nodes:
            r, o = self.find(n)
            out[r].append((n, o))
        return out


def fields_of(accesses):
    """[(offset, width, signed, count, others)] the accesses make: at each offset the widest width that ends before
    the next offset read (others: the widths dropped for overlapping the next field)."""
    by = defaultdict(Counter)
    sign = defaultdict(Counter)
    for o, w, sg, *_ in accesses:
        if w:
            by[o][w] += 1
            sign[o][sg] += 1
    offs = sorted(by)
    out = []
    for i, o in enumerate(offs):
        limit = offs[i + 1] - o if i + 1 < len(offs) else 8
        fit = [w for w in by[o] if w <= limit]
        w = max(fit) if fit else min(by[o])
        out.append((o, w, sign[o][True] > sign[o][False], sum(by[o].values()), sorted(set(by[o]) - {w})))
    return out


def rows_text(rows):
    return "; ".join(f"{o:#x} {'s' if sg else 'u'}{w * 8}" + (f" x{n}" if n > 1 else "") for o, w, sg, n, _ in rows)


def bytes_candidates(ctx, scans):
    """[dict] of the scalar pointers (parameters, locals, globals, linked through the calls that pass them on at an
    offset) the C files read at constant offsets like a struct: at three or more offsets, widths mixed or offsets
    unaligned (or four words of u32/s32), not a plain array."""
    g = Graph(scans, ctx.matched)
    out = []
    for root, members in g.components().items():
        accs, nodes = [], []
        for n, base in members:
            typ = g.decl.get(n, ("", ""))[0]
            if n in g.acc and scalar_pointee(typ) is not None:
                nodes.append((n, base, typ))
                accs += [(o + base, w, sg, form, fn, rg, n) for o, w, sg, form, fn, rg in g.acc[n]]
        offs = {a[0] for a in accs if a[1]}
        if len(offs) < 3:
            continue
        lo = min(offs)
        widths = {a[1] for a in accs if a[1]}
        unaligned = any(a[0] % a[1] for a in accs if a[1])
        step = max(widths)
        array = len(widths) == 1 and not unaligned and sorted(offs) == list(range(min(offs), min(offs) + step * len(offs), step))
        words = widths == {4} and len(offs) >= 4
        if array or not (len(widths) > 1 or unaligned or words):
            continue
        fns = {a[4] for a in accs}
        fns |= {n[1] for n, _, _ in nodes if n[0] in "pl"}
        un = fns - ctx.matched
        typed = Counter()
        for n, base, _ in nodes:
            for (name, off), c in g.typed[n].items():
                typed[name, base + off - lo] += c
        fields = fields_of([(a[0] - lo, a[1], a[2]) for a in accs])
        names = ", ".join(sorted({f"{g.decl[n][1]}" + (f"@{base:#x}" if base else "") for n, base, _ in nodes})[:4])
        where = sorted({g.fn[f][0] for f in fns if f in g.fn})
        if where and all(w.startswith("lib/dwc") or w.startswith("lib/nitro/src/card") for w in where):
            continue  # another job's: the DWC and GameSpy structs, the card structs
        out.append({"kind": "bytes", "unmatched": len(un), "fns": sorted(fns), "unmatched_fns": sorted(un),
                    "files": where, "fields": fields, "nodes": nodes, "base": lo, "typed": typed,
                    "mixed": len(widths) > 1 or unaligned,
                    "text": f"{names} in {', '.join(sorted(un or fns)[:3])}{' ...' if len(un or fns) > 3 else ''}: "
                            f"{len(offs)} offsets {'mixed ' if len(widths) > 1 else ''}widths "
                            f"{'/'.join(map(str, sorted(widths)))}; {len(fns)} functions, {len(un)} unmatched"
                            + (f"; passed to functions taking {', '.join(f'{n} at +{o:#x}' for (n, o), _ in typed.most_common(2))}"
                               if typed else "") + f"\n      {rows_text(fields)}"})
    out.sort(key=lambda c: (-c["unmatched"], not c["mixed"], -len(c["fields"])))
    return out


def field_path(f, off):
    """The member expression of the leaf row f for a scalar at off (name, name.sub or name[i]), or None."""
    elem_count = ss_count(f[4])
    if elem_count > 1:
        elem = f[1] // elem_count
        if (off - f[0]) % elem:
            return None
        return f"{f[3]}[{(off - f[0]) // elem}]"
    return f[3] if off == f[0] else None


def ss_count(kind):
    n = 1
    for d in re.findall(r"\[(\d+)\]", kind):
        n *= int(d)
    return n


def typed_candidates(ctx, scans, leaf, elem_of):
    """[dict] of the struct types the C files read at constant byte offsets through a pointer of that type (a cast
    to u8 *, p[n] on a u32 *'s cast): each access against the type's layout: a field of that offset and width (the
    access can name it), past the end, in a blob nothing names, another width than the field's."""
    g = Graph(scans, ctx.matched)
    by = defaultdict(list)
    for node, accs in g.acc.items():
        typ, name = g.decl.get(node, ("", ""))
        if scalar_pointee(typ) is not None or ss.pointee(typ) is None:
            continue
        sname = re.sub(r"\b(const|volatile|struct|union)\b", "", ss.pointee(typ)).strip()
        for a in accs:
            f = g.fn[a[4]][0]
            key = ctx.types.key(sname, f)
            if key:
                by[key].append((a, name, f, node))
    out = []
    for key, accs in sorted(by.items()):
        lay = ctx.layout(key)
        if not lay:
            continue
        kinds = defaultdict(list)
        for a, name, f, node in accs:
            (o, w, sg, form, fn, rg), size = a, lay[0]
            row = leaf(lay, o)
            if not w:
                continue
            if o + w > size:
                kind = "past"
            elif row is None:
                kind = "padding"
            else:
                path = field_path(row, o)
                e = elem_of(row)
                if path and e and e[0] == w:
                    kind = "unk" if re.fullmatch(r"unk\w*", row[3]) else "named"
                elif e is None:
                    kind = "other"
                elif re.fullmatch(r"(unk|pad|dummy|reserved)\w*", row[3]):
                    kind = "blob"
                else:
                    kind = "width"
            kinds[kind].append((a, name, f, node, row))
        if not kinds.get("past") and not kinds.get("named") and not kinds.get("blob") and not kinds.get("unk"):
            continue
        fns = {a[4] for a, *_ in accs}
        un = fns - ctx.matched
        stat = ", ".join(f"{k} {len(v)}" for k, v in sorted(kinds.items()))
        out.append({"kind": "typed", "key": key, "unmatched": len(un), "fns": sorted(fns), "unmatched_fns": sorted(un),
                    "kinds": kinds, "size": lay[0],
                    "text": f"{key.split(':')[-1]} ({lay[0]:#x} bytes): {len(accs)} byte-offset accesses in "
                            f"{len(fns)} functions, {len(un)} unmatched: {stat} ({', '.join(sorted(un or fns)[:3])})"})
    out.sort(key=lambda c: (-c["unmatched"], -len(c["kinds"].get("past", [])), c["text"]))
    return out


def rewrites(cands, ctx, keep_matched=False):
    """{C file: [(begin, end, text, function, old)]} of the byte-offset accesses of unmatched functions (or all) that
    name a field of the struct their pointer points to: `*(u16 *)((u8 *)p + 0x14)` -> `p->unk_14`. The offsets are
    the file's bytes; only a parameter or a local pointer of the struct type, a field of the same size and signedness."""
    out = defaultdict(list)
    for c in cands:
        for a, name, f, node, row in c["kinds"].get("named", []) + c["kinds"].get("unk", []):
            o, w, sg, form, fn, rg = a
            if (not keep_matched and fn in ctx.matched) or rg[0] is None or node[0] == "g" or not name:
                continue
            path = field_path(row, o)
            kind = row[4].split("[")[0].strip()
            want = re.sub(r"\s+", "", rg[2] if len(rg) > 2 else "")
            if want.endswith("*") != kind.endswith("*") or want.endswith("*") and want != re.sub(r"\s+", "", kind):
                continue  # another type of pointer, or a number where the field is a pointer (or the other way)
            fsigned = True if re.fullmatch(r"s(8|16|32)|int|short", kind) else                 False if re.fullmatch(r"u(8|16|32)|unsigned.*", kind) else None
            if path is None or fsigned is not None and fsigned != bool(sg):
                continue
            path = f"{name}->{path}"
            out[f].append((rg[0], rg[1], path, fn, ""))
    return out


def apply_rewrites(rw):
    """Writes the rewrites into the files, innermost (latest) first; returns how many were written per file."""
    from pathlib import Path
    done = {}
    for f, items in rw.items():
        p = Path(ss.cl.ROOT) / f
        data = p.read_bytes()
        last = len(data) + 1
        n = 0
        for b, e, text, fn, _ in sorted(set(items), reverse=True):
            if e > last:
                continue  # overlaps one already written
            old = data[b:e].decode(errors="replace")
            if "\n" in old and old.count("\n") > 1:
                continue
            data = data[:b] + text.encode() + data[e:]
            last, n = b, n + 1
        p.write_bytes(data)
        done[f] = n
    return done
