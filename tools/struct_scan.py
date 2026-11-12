#!/usr/bin/env python3
"""What the C files' functions read and write through pointers at constant offsets, from clang's AST
(tools/struct_detect.py's bytes and past-the-end detectors).

    python3 tools/struct_scan.py src/ov010/script_work.c    # the accesses of each function of the file

scan(path) gives {function: scan_function()}: the accesses (subscripts and dereferences of a constant sum on a
parameter, a local or a global pointer or array; a local set once from a pointer expression is that pointer's alias),
and the pointers the function passes to calls.
"""
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_layouts as cl  # noqa: E402  (re-runs this in the venv)

SIZES = {"char": 1, "signed char": 1, "unsigned char": 1, "_Bool": 1, "short": 2, "unsigned short": 2, "int": 4,
         "unsigned int": 4, "long": 4, "unsigned long": 4, "float": 4, "void": 1}


def qual(node):
    t = node.get("type", {})
    return t.get("desugaredQualType") or t.get("qualType") or ""


def pointee(t):
    """The type a pointer or array type of clang's text points to, or None."""
    t = t.strip()
    if t.endswith("*"):
        return t[:-1].strip()
    m = re.fullmatch(r"(.*?)\s*\[\d*\]", t)
    return m[1].strip() if m else None


TDEFS = {}  # {typedef name: the type it names} of the file being scanned


def scalar_size(t):
    t = re.sub(r"\b(const|volatile)\b", "", t).strip()
    if t.endswith("*"):
        return 4
    if t in SIZES:
        return SIZES[t]
    if t in TDEFS and TDEFS[t] != t:
        return scalar_size(TDEFS[t])
    return None


def signed_of(t):
    return not (t.startswith("unsigned") or t == "_Bool" or t.endswith("*") or t in ("float", "void"))


def int_value(n):
    """The integer a constant expression of literals, casts, parentheses and + - * << is, or None."""
    k = n.get("kind")
    if k == "IntegerLiteral":
        return int(n["value"])
    if k in ("ImplicitCastExpr", "CStyleCastExpr", "ParenExpr") and n.get("inner"):
        return int_value(n["inner"][0])
    if k == "UnaryOperator" and n.get("opcode") == "-" and n.get("inner"):
        v = int_value(n["inner"][0])
        return None if v is None else -v
    if k == "BinaryOperator" and n.get("opcode") in ("+", "-", "*", "<<") and len(n.get("inner", [])) == 2:
        a, b = (int_value(x) for x in n["inner"])
        if a is None or b is None:
            return None
        return {"+": a + b, "-": a - b, "*": a * b, "<<": a << b}[n["opcode"]]
    return None


def unparen(n):
    while n.get("kind") in ("ParenExpr", "ImplicitCastExpr", "CStyleCastExpr") and n.get("inner"):
        n = n["inner"][0]
    return n


def scan(path):
    """{function: scan_function()} of the functions a C file defines (clang's AST)."""
    out = cl.clang(path, "-Xclang", "-ast-dump=json")
    if not out.strip():
        return {}
    ast = json.loads(out)
    result = {}
    TDEFS.clear()
    for d in ast["inner"]:
        if d.get("kind") == "TypedefDecl" and d.get("name"):
            t = d.get("type", {})
            TDEFS[d["name"]] = t.get("desugaredQualType") or t.get("qualType", "")
    for d in ast["inner"]:
        if d.get("kind") != "FunctionDecl" or not d.get("name") or d["name"] in result:
            continue
        loc = d.get("loc", {})
        if "includedFrom" in loc.get("expansionLoc", loc):
            continue
        body = next((c for c in d.get("inner", []) if c.get("kind") == "CompoundStmt"), None)
        if body is None:
            continue
        params = [c for c in d["inner"] if c.get("kind") == "ParmVarDecl"]
        result[d["name"]] = scan_function(params, body)
    return result


def scan_function(params, body):
    """{"params": [[name, type, written type, id]], "acc": [[declaration id, byte offset, width, signed, form, range
    begin, range end]], "calls": [[callee, argument index, declaration id, byte offset]], "names", "types" (written
    types) and "alias" of the declarations}; a global's name starts with "::"; form: "ix" (p[n]), "add"
    (*(T *)(p + n)), "star" (*p)."""
    names = {p["id"]: p["name"] for p in params if p.get("name")}
    types = {p["id"]: qual(p) for p in params}
    written = {p["id"]: p.get("type", {}).get("qualType", "") for p in params}
    alias = {}
    sets = defaultdict(list)  # {declaration id: the expressions assigned to it}
    nodes = []

    def collect(n):
        nodes.append(n)
        for c in n.get("inner", ()):
            collect(c)
    collect(body)
    for n in nodes:
        k, op = n.get("kind"), n.get("opcode")
        if k == "VarDecl" and n.get("id"):
            names[n["id"]] = n.get("name", "")
            types[n["id"]] = qual(n)
            written[n["id"]] = n.get("type", {}).get("qualType", "")
            if n.get("inner"):
                sets[n["id"]].append(n["inner"][-1])
        elif k == "BinaryOperator" and op in ("=", "+=", "-=") and len(n.get("inner", [])) == 2:
            lhs = unparen(n["inner"][0])
            if lhs.get("kind") == "DeclRefExpr":
                sets[lhs["referencedDecl"]["id"]].extend([n["inner"][1]] if op == "=" else [{}, {}])
        elif k == "UnaryOperator" and op in ("++", "--") and n.get("inner"):
            ref = unparen(n["inner"][0])
            if ref.get("kind") == "DeclRefExpr":
                sets[ref["referencedDecl"]["id"]].extend([{}, {}])  # moved: no alias

    def ptr(n):
        """(declaration id, byte offset) of a pointer expression made of a declaration, casts and constant sums."""
        k = n.get("kind")
        if k in ("ImplicitCastExpr", "CStyleCastExpr", "ParenExpr") and n.get("inner"):
            return ptr(n["inner"][0])
        if k == "DeclRefExpr":
            r = n["referencedDecl"]
            if r["kind"] not in ("ParmVarDecl", "VarDecl") or pointee(qual(r)) is None:
                return None
            did = r["id"]
            if r["kind"] == "VarDecl" and did not in types:  # a global
                names[did], types[did] = "::" + r["name"], qual(r)
                written[did] = r.get("type", {}).get("qualType", "")
                return did, 0
            return alias.get(did, (did, 0))
        if k == "BinaryOperator" and n.get("opcode") in ("+", "-") and len(n.get("inner", [])) == 2:
            a, b = n["inner"]
            for p, q in ((a, b), (b, a) if n["opcode"] == "+" else (None, None)):
                if p is None:
                    continue
                pt = pointee(qual(p))
                v = int_value(q)
                base = ptr(p) if pt is not None and v is not None else None
                size = scalar_size(pt) if base else None
                if base and size:
                    return base[0], base[1] + (v if n["opcode"] == "+" else -v) * size
            return None
        if k == "UnaryOperator" and n.get("opcode") == "&" and n.get("inner"):
            lv = lvalue(n["inner"][0])
            return (lv[0], lv[1]) if lv else None
        return None

    def lvalue(n):
        """(declaration id, offset, width, signed, form) of a dereference or subscript of a constant sum."""
        while n.get("kind") == "ParenExpr":
            n = n["inner"][0]
        k, t = n.get("kind"), qual(n)
        if k == "ArraySubscriptExpr" and len(n.get("inner", [])) == 2:
            v = int_value(n["inner"][1])
            pt = pointee(qual(n["inner"][0]))
            base = ptr(n["inner"][0])
            size = scalar_size(pt) if pt is not None else None
            if v is None or base is None or not size:
                return None
            return base[0], base[1] + v * size, scalar_size(t) or 0, signed_of(t), "ix"
        if k == "UnaryOperator" and n.get("opcode") == "*" and n.get("inner"):
            base = ptr(n["inner"][0])
            if base is None:
                return None
            form = "add" if unparen(n["inner"][0]).get("kind") == "BinaryOperator" else "star"
            return base[0], base[1], scalar_size(t) or 0, signed_of(t), form
        return None

    for _ in range(3):  # a local set once to a pointer expression is that pointer
        for did, exprs in sets.items():
            if len(exprs) == 1 and exprs[0] and did not in alias and did in types and pointee(types[did]) is not None:
                p = ptr(exprs[0])
                if p and p[0] != did:
                    alias[did] = p
    acc, calls = [], []

    def rng(n):
        r = n.get("range", {})
        b, e = r.get("begin", {}), r.get("end", {})
        if "offset" in b and "offset" in e and "includedFrom" not in b and "includedFrom" not in e:
            return b["offset"], e["offset"] + e.get("tokLen", 0)
        return None, None

    for n in nodes:
        k = n.get("kind")
        if k == "CallExpr" and n.get("inner"):
            callee = unparen(n["inner"][0])
            cn = callee.get("referencedDecl", {}).get("name") if callee.get("kind") == "DeclRefExpr" else None
            for i, a in enumerate(n["inner"][1:]):
                p = ptr(a)
                if cn and p:
                    calls.append([cn, i, p[0], p[1]])
        elif k == "ArraySubscriptExpr" or k == "UnaryOperator" and n.get("opcode") == "*":
            lv = lvalue(n)
            if lv:
                b, e = rng(n)
                acc.append([lv[0], lv[1], lv[2], lv[3], lv[4], b, e, n.get("type", {}).get("qualType", "")])
    ids = {a[0] for a in acc} | {c[2] for c in calls}
    ptrs = sorted({w for i, w in written.items() if w.rstrip().endswith("*") and i in names})
    return {"params": [[p.get("name", ""), qual(p), p.get("type", {}).get("qualType", ""), p["id"]] for p in params],
            "ptypes": ptrs,
            "acc": acc, "calls": calls, "names": {i: names.get(i, "") for i in ids},
            "types": {i: written.get(i, "") for i in ids}, "alias": {i: list(a) for i, a in alias.items()}}


if __name__ == "__main__":
    for fn, r in scan(sys.argv[1]).items():
        if r["acc"] or r["calls"]:
            print(fn, [p[:3] for p in r["params"]])
            for a in r["acc"]:
                print("  ", r["names"][a[0]], r["types"][a[0]], *a[1:])
            for c in r["calls"]:
                print("   call", c[0], c[1], r["names"][c[2]], c[3])


def _key(path):
    import hashlib
    return cl.file_key(path) + hashlib.sha1(Path(__file__).read_bytes()).hexdigest()[:8]


def _scan_one(path):
    try:
        return path, _key(path), scan(path)
    except Exception as e:  # a file that doesn't compile or parse: left out
        print(f"{path}: {e}", file=sys.stderr)
        return path, None, {}


def scan_all(jobs=None, cache=None, verbose=True):
    """{C file: {function: scan_function()}} of every C file of the build, cached in build/struct_scan.json by the
    file's and its headers' contents; the files that changed are scanned `jobs` at a time (BW_JOBS, at most 4)."""
    import os
    from concurrent.futures import ProcessPoolExecutor
    cache = cache or cl.ROOT / "build" / "struct_scan.json"
    have = json.loads(cache.read_text()) if cache.exists() else {}
    files = sorted(cl.c_files())
    keys = {f: _key(f) for f in files}
    todo = [f for f in files if have.get(f, {}).get("key") != keys[f]]
    import jobs as bwjobs
    jobs = min(jobs or bwjobs.jobs(), 4)
    if todo:
        if verbose:
            print(f"scanning {len(todo)} of {len(files)} C files, {jobs} at a time", file=sys.stderr)
        with ProcessPoolExecutor(jobs) as pool:
            for i, (path, key, result) in enumerate(pool.map(_scan_one, todo, chunksize=4)):
                if key:
                    have[path] = {"key": key, "scan": result}
                if verbose and i % 100 == 99:
                    print(f"  {i + 1} of {len(todo)}", file=sys.stderr, flush=True)
        cache.write_text(json.dumps(have))
    return {f: have[f]["scan"] for f in files if f in have}
