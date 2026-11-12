#!/usr/bin/env python3
"""Declare Black's named functions in this repo's headers, from the headers pokebw2 declares them in, and with --file
what a file tools/port_file.py ported uses.

Every function Black's symbols.txt names (not a dsd placeholder, not one of pokebw2's own guesses from its
config/names.txt) that no header here declares yet gets pokebw2's prototype from include/ or lib/*/include/, in the
header at the same path here, with the types it needs:
- a struct of the game is forward-declared in struct_decls.h, its layout left out unless a file needs it;
- a type of Game Freak's library (include/gfl/) or an SDK (lib/) is copied whole, as are all the types and constants
  of their headers: their code is twinned, so their layouts are Black's too;
- another type of the game (an enum, a scalar or a function pointer) or a constant is copied, marked as Black 2's
  unless twinned code confirms it (below).
A twin (tools/twins.py's tables) shows that pokebw2's C for a function compiles to Black's code, so its prototype fits.
The game's and GFL's functions without one under the same name, most of them still assembly in pokebw2 too, go last in
their header, under a comment that says their prototypes are unchecked. A function the C here defines as static is left
out, and so is a declaration under a condition other than the header guard (the run lists those).

--file adds, from the pokebw2 file the C file's first line names, only what that C file uses and the headers lack (the
whole-tree pass above is a run without --file; --file once ran it too and added 2,800 lines to 71 headers the file
didn't need, t34h 2026-10-27): the
layouts of the structs its functions use whole (clang's AST of pokebw2's file says which), the types, constants (also
those pokebw2 generates from its data), inline functions and data it names, and pokebw2's prototypes of the functions it
calls or defines that Black has, under the names port_file.py gave them (the words the two files differ by), pokebw2's
in a comment. A function the file defines, not static, needs a prototype it sees (-requireprotos): one that a header
the file doesn't reach declares already (btl_tables.c's IsArceus, in btl_server_flow.h) gets pokebw2's prototype in
pokebw2's header if the file reaches that and the two agree, else the file includes the header that declares it.
The C file gets an #include of each header that declares what it uses and that its #include lines don't
reach, and loses those of headers this repo lacks; include/gfl/overlay.h gets the linker's OVERLAY_<n>_ID of each
overlay its OVERLAY_ID(n) names. Names Black lacks (Black 2's own functions) stay undeclared, and so
does a dsd placeholder pokebw2's file has too, which can be another function in each game, unless a twin pairs them. A
run after which nothing is missing changes nothing.

Twinned code also shows what of a game type or constant Black has: the fields a twin's pokebw2 C accesses are at
Black's offsets, with Black's widths and signedness, and the constants it uses have Black's values (twins of at least
16 bytes, or that Black gives pokebw2's name; not a static variable's fields, which a relocation may hold). From clang's
AST of the twins' sources, cached in build/twin_evidence.json until a twins table is newer. A game struct, enum or
constant twinned code confirms whole has no mark; one it confirms in part says which fields or values, and its size if
twinned code takes it; one it doesn't confirm is marked as Black 2's. Without clang every game type is marked, and
--file adds only the layouts the file's own declarations need.

A constant twinned code doesn't confirm, of a header tools/black_constants.py reads Black's numbers for (sounds, text
banks, names, zones, trainers, archives), gets Black's number from it, under its note, or keeps Black 2's under its
note that Black lacks the name; only one it can't tell is marked as Black 2's. Its reading of Black's files is cached in
build/black_constants.json (2 s to make, 0.02 s to read).

A pokebw2 file's own headers (src/ov207/p_status_local.h beside src/ov207/p_sta_skill.c: the C file includes it with
quotes) are read like its include/ ones: what the C file uses of them goes into the header of that name beside the C file
here (src/ov131/p_status_local.h), a new one if there is none, with the same marks (twinned code confirms, else Black 2's)
and the same includes. The C file keeps its #include of it. A struct or typedef that another C file beside defines in
its own words (p_sta_ribbon.c's PStaScreen, matched code) is that file's: the header takes it from there, with the
macros it uses (an array's length), and the other file loses it.

A struct the C file lays out itself, which a header here defines as well ("redefined"), is settled by who made the
header's layout:
- tools/struct_infer.py (its marker comment: unk_ fields only): the C file's layout, pokebw2's with names, takes its
  place in the header, with the C file's own types and constants it needs (a PiecesManager its field points to), and
  leaves the C file. Unless another C file accesses a field of the inferred layout (clang's AST says): then as below;
- a worker's, which matched code uses (BtlvScd): the header's stays and the C file's definition goes. Its field
  accesses take the header's names where both layouts (clang's) have a field of the same size at the same offset
  (pokebw2's strbuf for Black's strBuf, its unk18 for unk_18); a name another struct of pokebw2's file has too is left,
  and so is a field with no counterpart (Black 2 added fields that Black's offsets lack), which the functions that use
  it will fail to compile on. What it did is printed.

    python3 tools/port_decls.py                                      # prints what it would add
    python3 tools/port_decls.py --apply                              # writes the headers
    python3 tools/port_decls.py --apply --file src/system/season.c   # only what that file uses (after port_file.py)
    python3 tools/port_decls.py --check    # compiles each header alone and all together, lists names declared twice
"""
import argparse
import csv
import difflib
import glob
import json
import os
import re
import shutil
import subprocess
import sys
import textwrap
from collections import defaultdict
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor
from pathlib import Path

import bwcache
import black_constants

ROOT = Path(__file__).resolve().parents[1]
URL = "https://github.com/fuddlesworth/pokebw2"
SWAN_NOTE = "// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)"
GAME_TYPE_NOTE = "// Black 2's, not checked against Black"
UNTWINNED_NOTE = "// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code"
BLACK_NOTES = (black_constants.BLACK_NOTE, black_constants.MISSING_NOTE)
KEYWORDS = set("""auto break case char const continue default do double else enum extern float for goto if inline int
long register restrict return short signed sizeof static struct switch typedef union unsigned void volatile while asm
__attribute__ __inline _Bool defined""".split())
# The libraries Black links that pokebw2 has headers for: twinned code says Black links the same versions
LIBRARY_DIRS = ("include/gfl/", "lib/nitro/", "lib/nnsys/", "lib/twl/", "lib/spl/", "lib/dwc/", "lib/dpw/", "lib/dsprot/",
                "lib/poke_net/", "lib/dwcutil/")
C_LIBRARY = ("include/stddef.h", "include/stdarg.h", "include/stdlib.h", "include/math.h")
# A comment about Black 2's addresses or overlays would mislead here
B2_COMMENT = re.compile(r"0x[0-9a-fA-F]{5,}|\b(func|data|unk)_\w+|\bov\d|\b[Oo]verlays? \d|Black 2|White 2|"
                        r"\bB2\b|\bW2\b")
STRUCT_DECLS = "include/struct_decls.h"
EVIDENCE = ROOT / "build" / "twin_evidence.json"
TINY = 0x10  # bytes: a twin smaller than this can be any getter (tools/file_ranges.py), no evidence


def is_library(path):
    """Game Freak's library, an SDK or the C library: their code is twinned, so their types are Black's too."""
    return path.startswith(LIBRARY_DIRS) or path in C_LIBRARY


def include_name(path):
    """How a header is included: its path below its include directory; a header beside a C file (src/ov131/
    p_status_local.h), by its name."""
    if path.startswith("src/"):
        return os.path.basename(path)
    return path.split("/include/", 1)[1] if path.startswith("lib/") else path[len("include/"):]


def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    return re.sub(r"//[^\n]*", "", text)


def tokens(text):
    text = re.sub(r'"(?:\\.|[^"\\])*"', '""', strip_comments(text))
    return re.findall(r"[A-Za-z_]\w*|\d\w*|\S", text)


def is_name(token):
    return re.match(r"[A-Za-z_]", token) is not None


class Item:
    """A top-level piece of a header: a declaration, a definition or a preprocessor line, with its comment."""

    def __init__(self, text, comment="", cond=(), pos=-1):
        self.text = text.rstrip()  # without the comment
        self.comment = comment  # the comment lines right above it, or ""
        self.cond = list(cond)  # the #if conditions around it, outermost first, the header guard left out
        self.pos = pos  # its index in its header
        self.kind = "other"  # pp, proto, func (inline), typedef, tag (struct/union/enum), var or other
        self.names = []  # what it declares: functions, types, tags ("struct X"), macros, enumerators
        self.fwd = False  # a declaration of a struct it doesn't define (typedef struct X X; or struct X;)
        self.struct = False  # a struct or union
        self.tag = None  # the tag of a typedef'd struct, union or enum
        self.note = None  # a comment to add above it
        classify(self)

    def code(self):
        return strip_comments(self.text)


def classify(item):
    code = " ".join(item.code().split())
    if code.startswith("#"):
        item.kind = "pp"
        m = re.match(r"#\s*define\s+(\w+)", code)
        if m:
            item.names = [m.group(1)]
        return
    body = re.sub(r"\{.*\}", "{}", code)  # the outermost braces, emptied
    if code.startswith("typedef"):
        item.kind = "typedef"
        m = re.search(r"\(\s*\*\s*(\w+)\s*\)\s*\(", body)
        if m:  # a function pointer type
            item.names = [m.group(1)]
            return
        m = re.match(r"typedef\s+(?:const\s+)?(struct|union|enum)\s+(\w+)?\s*(\{\})?\s*(.*?);?$", body)
        if m:
            kind, tag, has_body, rest = m.groups()
            item.names = re.findall(r"(\w+)\s*(?:\[[^\]]*\]\s*)*(?:,|$)", rest)
            item.struct = kind != "enum"
            if tag:
                item.tag = f"{kind} {tag}"
                if has_body:
                    item.names.append(item.tag)
                else:
                    item.fwd = True
            if kind == "enum" and has_body:
                item.names += enumerators(code)
            return
        m = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*;?$", body)
        if m:
            item.names = [m.group(1)]
        return
    m = re.match(r"(struct|union|enum)\s+(\w+)?\s*\{\}\s*;?$", body)
    if m:
        item.kind = "tag"
        item.struct = m.group(1) != "enum"
        if m.group(2):
            item.names = [f"{m.group(1)} {m.group(2)}"]
        if m.group(1) == "enum":
            item.names += enumerators(code)
        return
    m = re.match(r"(struct|union)\s+(\w+)\s*;$", body)
    if m:
        item.kind, item.fwd, item.struct = "tag", True, True
        item.names = [f"{m.group(1)} {m.group(2)}"]
        return
    head = body.split("{")[0]
    m = re.search(r"(\w+)\s*\(", head)
    if m and not head.startswith("extern") and m.group(1) not in ("__attribute__", "asm"):
        item.kind = "func" if "{" in body else "proto"
        item.names = [m.group(1)]
        return
    m = re.match(r"extern\s+.*?(\w+)\s*(?:\[[^\]]*\]\s*)*;$", body)
    if m:
        item.kind = "var"
        item.names = [m.group(1)]


def enumerators(code):
    inner = code[code.index("{") + 1:code.rindex("}")]
    return re.findall(r"(?:^|,)\s*(\w+)", inner)


def parse(path):
    """Returns the Items of a header, in order."""
    # a C++ guard's `extern "C" {` and its `}` (MSL's headers): what they hold is the header's own (memset, strcmp: read
    # as one block, they were missing, and pokebw2's copies went into dwc/ext.h beside MSL's, 2026-10-15)
    text = re.sub(r'#ifdef __cplusplus\s*\n\s*(?:extern "C" \{|\})\s*\n\s*#endif[^\n]*', "", Path(path).read_text())
    lines = text.split("\n")
    items, comment, cond, buf, depth = [], [], [], [], 0
    in_block = False

    def add(text):
        guarded = [c for c in cond if not re.match(r"#ifndef \w+_H$", c)]
        items.append(Item(text, "\n".join(comment), guarded, len(items)))

    i = 0
    while i < len(lines):
        line = lines[i]
        s = line.strip()
        i += 1
        if not buf:
            if in_block or s.startswith(("//", "/*")):
                comment.append(line)
                in_block = (in_block or s.startswith("/*")) and "*/" not in s
                continue
            if not s:
                comment = []
                continue
            if s.startswith("#"):
                text = line
                while text.endswith("\\") and i < len(lines):
                    text += "\n" + lines[i]
                    i += 1
                word = re.match(r"#\s*(\w*)", s).group(1)
                if word in ("if", "ifdef", "ifndef"):
                    cond.append(s)
                elif word in ("else", "elif"):
                    cond[-1] += " / " + s
                elif word == "endif" and cond:
                    cond.pop()
                add(text)
                comment = []
                continue
            m = re.match(r"(\w+)\((\w+)[^;{]*\)$", s)
            if m:  # a macro that defines a function, the first argument
                add(line)
                items[-1].kind, items[-1].names = "func", [m.group(2)]
                comment = []
                continue
        buf.append(line)
        for ch in strip_comments(line):
            depth += (ch == "{") - (ch == "}")
        if depth:
            continue
        joined = strip_comments("\n".join(buf)).strip()
        head = joined.split("{")[0]
        if joined.endswith(";") or joined.endswith("}") and "(" in head and "=" not in head and not re.match(
                r"(typedef\s+)?(struct|union|enum)\b", head):  # a declaration, or an inline function's body
            add("\n".join(buf))
            buf, comment = [], []
    return items


def header_files(root):
    paths = []
    for base in ["include", *sorted(glob.glob("lib/*/include", root_dir=root))]:
        paths += sorted(glob.glob(f"{base}/**/*.h", root_dir=root, recursive=True))
    return paths


def index(root, paths, cache=False):
    """Returns ({name: (path, Item)} of every declaration, {path: [Item]}). With cache, a header that is as it was
    (size and mtime) and was parsed by this code is read from build/cache/headers (every header of the root in one
    file), not parsed again; only the headers that changed are, and the file is written again when any did."""
    names, files = {}, {}
    token = bwcache.code_hash(__file__) if cache else None
    old = (bwcache.get("headers", root, token) or {}) if cache else {}
    kept = {}
    for path in paths:
        full = os.path.join(root, path)
        st = bwcache.stamp(full) if cache else None
        if cache and st is not None and path in old and old[path][0] == st:
            files[path] = old[path][1]
        else:
            files[path] = parse(full)
        if cache:
            kept[path] = (st, files[path])
    if cache and (len(kept) != len(old) or any(kept[p][1] is not old.get(p, (0, 0))[1] for p in kept)):
        bwcache.put("headers", root, token, kept)  # before anything below changes the Items
    for path in paths:
        for item in files[path]:
            if item.kind == "other" or item.kind == "pp" and not re.match(r"\s*#\s*define", item.text):
                continue
            for name in item.names:
                old_item = names.get(name)
                # A definition wins over a forward declaration
                if old_item is None or old_item[1].fwd and not item.fwd and item.kind in ("typedef", "tag"):
                    names[name] = (path, item)
    return names, files


def declarator_names(toks):
    """The names a declaration declares (parameters, fields, its own), which aren't dependencies."""
    out = set()
    for i in range(1, len(toks) - 1):
        prev, t = toks[i - 1], toks[i]
        # not a product: `sizeof(BoxPkm) * BOX_MAX_POS;` kept box_savedata.c's constant from its header
        product = prev == "*" and i >= 2 and (toks[i - 2] in (")", "]") or toks[i - 2][:1].isdigit())
        keyword = prev in ("struct", "union", "enum", "return", "case", "goto", "sizeof", "else", "do")
        if is_name(t) and toks[i + 1] in ",);[:=" and not product and (prev in ("*", "}") or is_name(prev) and
                                                                        not keyword):
            out.add(t)
    return out


def dependencies(item):
    """Returns [(name, whether the definition is needed)] of the types, tags, macros and enumerators the item uses."""
    if item.fwd:
        return []
    toks = tokens(item.text)
    own = set(item.names) | declarator_names(toks)
    m = re.match(r"#\s*define\s+\w+\(([^)]*)\)", item.code().strip())
    if m:
        own |= set(re.findall(r"\w+", m.group(1)))  # a macro's parameters
    out = []
    for i, t in enumerate(toks):
        if not is_name(t) or t in KEYWORDS:
            continue
        nxt = toks[i + 1] if i + 1 < len(toks) else ""
        if i > 0 and toks[i - 1] in ("struct", "union", "enum"):
            if nxt not in ("*", "{") and f"{toks[i - 1]} {t}" not in own:
                out.append((f"{toks[i - 1]} {t}", True))
        elif t not in own:
            out.append((t, nxt not in ("*", ")") or item.kind == "pp"))
    return out


def read_symbols(root, every=False):
    """Returns {name: (module, address)} of Black's named functions; with every, of all its functions and data."""
    out = {}
    for path in sorted(glob.glob(f"{root}/config/b_us/arm9/**/symbols.txt", recursive=True)):
        module = os.path.relpath(os.path.dirname(path), f"{root}/config/b_us/arm9")
        module = "main" if module == "." else module.split("/")[-1]
        for line in open(path):
            parts = line.split()
            if len(parts) >= 3 and (parts[1].startswith("kind:function") and not parts[0].startswith("func_") or
                                    every and parts[1].startswith(("kind:function", "kind:data"))):
                out.setdefault(parts[0], (module, int(parts[2].split(":")[1], 16)))
    return out


def read_twins(paths):
    """Returns {(module, address): {pokebw2 names}} of Black's functions with the same code as pokebw2's."""
    out = defaultdict(set)
    for path in paths:
        for row in csv.DictReader(open(path), delimiter="\t"):
            out[(row["module"], int(row["address"], 16))].add(row["bw2_function"])
    return out


def default_twins():
    """tools/twins.py's tables in build/, here or, from a worktree, in the main checkout."""
    common = subprocess.run(["git", "rev-parse", "--path-format=absolute", "--git-common-dir"], cwd=ROOT,
                            capture_output=True, text=True).stdout.strip()
    for build in (ROOT / "build", Path(common).parent / "build"):
        paths = [build / "twins_b_us.tsv", build / "twins_b_us.thumb.tsv"]
        if all(path.exists() for path in paths):
            return paths
    raise SystemExit("No build/twins_b_us.tsv and build/twins_b_us.thumb.tsv: make them with tools/twins.py, or pass "
                     "--twins")


def static_in_sources(root):
    """Names the C here defines or declares as static."""
    out = set()
    for path in glob.glob(f"{root}/src/**/*.c", recursive=True) + glob.glob(f"{root}/lib/*/src/**/*.c", recursive=True):
        for item in parse(path):
            if item.kind in ("proto", "func") and re.match(r"\s*static\b", item.code()):
                out.update(item.names)
    return out


def private_headers(root):
    """The headers beside the C files (src/ov131/p_status_local.h, lib/nitro/src/card/card_internal.h), which only
    their library's or overlay's files include."""
    return sorted(glob.glob(f"{root}/src/**/*.h", recursive=True) + glob.glob(f"{root}/lib/*/src/**/*.h", recursive=True))


def private_names(root):
    """{name: header} of what the private headers declare: a function they declare is declared here already (a second
    prototype in a public header, in the C file's words, named card_internal.h's CARDAccessLevel in card.h)."""
    out = {}
    for path in private_headers(root):
        for item in parse(path):
            if item.kind in ("proto", "var", "func", "typedef", "tag") or item.kind == "pp" and item.names:
                for n in item.names:
                    out.setdefault(n, os.path.relpath(path, root))
    return out


def types_in_sources(root):
    """The types and macros a C file here defines for itself (a library's own, like g3d's anm.c NNSG3dFuncAnmVis), or
    a header beside it does (card_internal.h's CARDAccessLevel)."""
    out = set()
    for path in glob.glob(f"{root}/src/**/*.c", recursive=True) + glob.glob(f"{root}/lib/*/src/**/*.c", recursive=True) + \
            private_headers(root):
        for item in parse(path):
            if item.kind in ("typedef", "tag", "pp"):
                out.update(n.split()[-1] for n in item.names)
    return out


def clang_ast(bw2, src):
    """clang's JSON AST of a pokebw2 source file, with pokebw2's compile_flags.txt, or None."""
    flags = (Path(bw2) / "compile_flags.txt").read_text().split() + ["-fpack-struct=4"]  # MWCC's u64 (check_layouts.MWCC_LAYOUT)
    run = subprocess.run(["clang", "-fsyntax-only", "-ferror-limit=0", *flags, "-Xclang", "-ast-dump=json", src],
                         cwd=bw2, capture_output=True, text=True)
    return json.loads(run.stdout) if run.stdout.strip() else None


def nodes(node):
    """The nodes of a clang JSON AST from node down, in order."""
    stack = [node]
    while stack:
        n = stack.pop()
        yield n
        stack.extend(reversed(n.get("inner", ())))


def ast_records(ast):
    """Returns ({FieldDecl id: (record, field)}, {record: [fields]}, {records the source file itself defines}) of the
    structs and unions an AST defines. A record is named as index() names it, "struct Tag" or the typedef of an untagged
    one; the field is the record's own, of which a field of an untagged member struct or union is part."""
    typedef_of = {}
    for n in nodes(ast):
        if n.get("kind") == "TypedefDecl" and n.get("inner") and "ownedTagDecl" in n["inner"][0]:
            typedef_of.setdefault(n["inner"][0]["ownedTagDecl"]["id"], n["name"])
    owner, fields, local = {}, {}, set()

    def add(record, key, top):
        member = None
        for c in record.get("inner", []):
            if c.get("kind") == "RecordDecl" and not c.get("name") and c.get("completeDefinition"):
                member = c
            elif c.get("kind") == "FieldDecl":
                name = c.get("name", "")
                owner[c["id"]] = (key, top or name)
                if not top and name:
                    fields[key].append(name)
                if member is not None:
                    add(member, key, top or name)
                    member = None

    for n in nodes(ast):
        if n.get("kind") == "RecordDecl" and n.get("completeDefinition"):
            key = f"{n['tagUsed']} {n['name']}" if n.get("name") else typedef_of.get(n["id"])
            if key and key not in fields:
                fields[key] = []
                add(n, key, "")
                loc = n.get("loc", {})
                if "includedFrom" not in loc.get("expansionLoc", loc):
                    local.add(key)
    return owner, fields, local


def record_of(qual_type):
    """The record a clang type is, by value or as an array, as ast_records() names it; else None."""
    q = qual_type.get("qualType", "")
    full = qual_type.get("desugaredQualType", q)
    if "*" in q or not re.match(r"((const|volatile) )*(struct|union)\b", full):
        return None
    for name in (full, q):
        name = re.sub(r"\[[^]]*\]|\b(const|volatile)\b", "", name).strip()
        if re.fullmatch(r"(struct|union) \w+|\w+", name):
            return name
    return None


def ast_functions(ast, owner, wanted):
    """{function: (fields, records, sizes)} for the functions in wanted that the AST defines: the (record, field) pairs
    whose offsets their code has (not those of a static variable, which a relocation may hold), the records whose
    layout they need, and the records whose size they take."""
    statics = {n["id"] for n in ast["inner"] if n.get("kind") == "VarDecl"}
    statics |= {n["id"] for n in nodes(ast) if n.get("kind") == "VarDecl" and n.get("storageClass") == "static"}

    def in_static(member):
        base = member["inner"][0]
        while base.get("kind") in ("ImplicitCastExpr", "ParenExpr", "ArraySubscriptExpr") or \
                base.get("kind") == "MemberExpr" and not base.get("isArrow"):
            base = base["inner"][0]
        return base.get("kind") == "DeclRefExpr" and base.get("referencedDecl", {}).get("id") in statics

    out = {}
    for d in ast["inner"]:
        body = d.get("kind") == "FunctionDecl" and d.get("name") in wanted and next(
            (c for c in d.get("inner", []) if c.get("kind") == "CompoundStmt"), None)
        if not body:
            continue
        fields, sizes = set(), set()
        records = {record_of(p.get("type", {})) for p in d["inner"] if p.get("kind") == "ParmVarDecl"}
        for n in nodes(body):
            kind = n.get("kind")
            if kind == "MemberExpr" and n.get("referencedMemberDecl") in owner:
                record, field = owner[n["referencedMemberDecl"]]
                records.add(record)
                if field and (n.get("isArrow") or not in_static(n)):  # not an untagged member, a step to its fields
                    fields.add((record, field))
            elif kind == "UnaryExprOrTypeTraitExpr" and n.get("name") == "sizeof":
                sizes.add(record_of(n.get("argType") or n["inner"][0].get("type", {})))
            elif kind in ("VarDecl", "CompoundLiteralExpr", "InitListExpr"):
                records.add(record_of(n.get("type", {})))
        out[d["name"]] = (fields, (records | sizes) - {None}, sizes - {None})
    return out


def source_evidence(job):
    """What a pokebw2 source's twinned functions show: ({record: fields}, {record: all its fields}, [records whose
    size they take], [names])."""
    bw2, src, funcs = job
    ast = clang_ast(bw2, src)
    if ast is None:
        return {}, {}, [], []
    owner, records, local = ast_records(ast)
    fields, sizes = defaultdict(set), set()
    for f, _, s in ast_functions(ast, owner, funcs).values():
        for record, field in f:
            if record not in local:  # a header's, not one of the many the files themselves define, say Work
                fields[record].add(field)
        sizes |= s - local
    names = set()
    for item in parse(Path(bw2) / src):
        if item.kind == "func" and item.names[0] in funcs:
            names.update(t for t in tokens(item.text) if is_name(t))
    return {r: sorted(f) for r, f in fields.items()}, {r: records[r] for r in fields if r in records}, sorted(sizes), \
        sorted(names)


def twin_evidence(bw2, twin_paths):
    """What twinned code shows of the game's types and constants in Black: {"fields": {record: fields the code
    accesses}, "records": {record: all its fields}, "sizes": records whose size it takes, "names": names it uses}. A
    twin of at least TINY bytes (or one Black gives pokebw2's name) compiles to Black's code, so the fields it accesses
    are at the same offsets and of the same width and signedness, and the constants it uses have the same values. From
    clang's AST of the twins' sources, cached in build/twin_evidence.json until a twins table is newer."""
    newest = max(os.path.getmtime(p) for p in twin_paths)
    if EVIDENCE.exists() and EVIDENCE.stat().st_mtime > newest:
        return json.loads(EVIDENCE.read_text())
    if not shutil.which("clang"):
        print("no clang: no twin evidence, so every game type is marked as Black 2's")
        return None
    funcs = defaultdict(set)
    for path in twin_paths:
        for row in csv.DictReader(open(path), delimiter="\t"):
            if row["bw2_source"].startswith("src/") and (int(row["size"]) >= TINY or
                                                         row["black_symbol"] == row["bw2_function"]):
                funcs[row["bw2_source"]].add(row["bw2_function"])
    out = {"fields": defaultdict(set), "records": {}, "sizes": set(), "names": set()}
    with ProcessPoolExecutor() as pool:
        for fields, records, sizes, names in pool.map(source_evidence, [(bw2, s, f) for s, f in sorted(funcs.items())]):
            for record, f in fields.items():
                out["fields"][record].update(f)
            for record, f in records.items():
                out["records"].setdefault(record, f)
            out["sizes"].update(sizes)
            out["names"].update(names)
    out = {"fields": {r: sorted(f) for r, f in sorted(out["fields"].items())}, "records": out["records"],
           "sizes": sorted(out["sizes"]), "names": sorted(out["names"])}
    EVIDENCE.parent.mkdir(exist_ok=True)
    EVIDENCE.write_text(json.dumps(out, indent=0))
    print(f"twin evidence: {len(out['fields'])} records, {len(out['names'])} names, from {len(funcs)} sources")
    return out


def name_map(ours, theirs):
    """{our name: pokebw2's} of the words tools/port_file.py replaced in a file it ported, from the two texts: their
    lines aligned by their shape (the words left out), then word by word."""
    def lines(text):
        return [[t for t in tokens(line) if t != "static"] for line in strip_comments(text).split("\n")
                if line.strip()]

    def shape(line):
        return " ".join("w" if is_name(t) and t not in KEYWORDS else t for t in line)

    a, b = lines(theirs), lines(ours)
    votes = defaultdict(lambda: defaultdict(int))
    matcher = difflib.SequenceMatcher(None, [shape(x) for x in a], [shape(y) for y in b], autojunk=False)
    for op, i1, i2, j1, j2 in matcher.get_opcodes():
        for x, y in zip(a[i1:i2], b[j1:j2]) if op == "equal" else ():
            for t, u in zip(x, y):
                if t != u and is_name(t):
                    votes[u][t] += 1
    # A replaced word is gone from our file, and ours wasn't in pokebw2's, unless it is another replaced word too (a
    # placeholder name can be another function's in the other game)
    theirs_words, our_words = {t for line in a for t in line}, {u for line in b for u in line}
    out = {u: max(count, key=count.get) for u, count in votes.items()}
    out = {u: t for u, t in out.items() if 2 * votes[u][t] > sum(votes[u].values())}
    replaced = set(out.values())
    return {u: t for u, t in out.items() if (t not in our_words or t in out) and
            (u not in theirs_words or u in replaced)}


class Planner:
    """What to add to which header: prototypes, the types and constants they need, and the includes for those."""

    def __init__(self, bw2, ours, evidence=None, black=None):
        self.bw2, self.ours = bw2, ours
        self.black = black  # tools/black_constants.py's read_black()
        evidence = evidence or {"fields": {}, "records": {}, "sizes": [], "names": []}
        self.fields, self.records, self.sizes = evidence["fields"], evidence["records"], set(evidence["sizes"])
        # the names twinned code uses, and those of the macros they expand to
        self.used = set(evidence["names"])
        todo = list(self.used)
        while todo:
            name = todo.pop()
            if name in bw2 and bw2[name][1].kind == "pp":
                new = {t for t in tokens(bw2[name][1].text) if is_name(t)} - self.used
                self.used |= new
                todo += new
        self.types = defaultdict(list)  # our path -> [Item]
        self.protos = defaultdict(list)  # our path -> [(Item, twinned)]
        self.includes = defaultdict(set)  # our path -> {our paths it must include}
        self.placed = {}  # name -> our path, of what this run adds
        # typedef name -> struct tag, of the typedefs here and of this run that only forward-declare their struct
        self.fwd_tags = {n: it.tag for n, (_, it) in ours.items() if it.fwd and it.kind == "typedef" and it.tag}
        self.done = set()
        self.unresolved = defaultdict(set)  # name -> {paths that need it}
        self.skipped = []  # (Item, why)
        self.private = {}  # our path -> pokebw2's private header of a library file to copy there
        self.private_names = {}  # our path of a private header -> {pokebw2's name: the C file's}
        self.beside = {}  # name -> the header beside C files here that declares it (private_names())
        self.local = {}  # our path of a header beside the C file -> pokebw2's path of the one beside its file
        self.replaced = defaultdict(list)  # our path -> [(Item it has, [Items of the C file that take its place])]
        self.adapt = []  # (names of a struct the C file lays out that a header here has too, our path of that header)
        self.b2_fields = {}  # clang's {record: [fields]} of pokebw2's file, when there is clang
        self.declared = defaultdict(set)  # name -> every header here that declares it (ours has one of them)

    def where(self, name):
        if name in self.placed:
            return self.placed[name]
        return self.ours[name][0] if name in self.ours else None

    def provide(self, name, need_def):
        """Declares name here if it is a type, a tag, a macro or an enumerator; returns the header that declares it."""
        here = self.where(name)
        if name not in self.bw2 or self.bw2[name][1].kind not in ("typedef", "tag", "pp"):
            return here  # not pokebw2's, or a function or variable, which a macro may name: not a dependency
        path, item = self.bw2[name]
        tag = self.fwd_tags.get(name)
        if here is not None:
            if need_def and tag:  # the layout of a struct this repo only forward-declares, under its tag
                if self.where(tag) is None and tag in self.bw2 and not self.bw2[tag][1].fwd:
                    self.provide(tag, True)
                elif self.where(tag) is None and not item.fwd:
                    self.place(item, path)
                return self.where(tag) or here
            return here
        if item.fwd:
            if item.tag:
                self.fwd_tags.setdefault(name, item.tag)
            if need_def and item.tag in self.bw2 and not self.bw2[item.tag][1].fwd:
                self.provide(item.tag, True)
            self.place(item, path)
            return self.where(item.tag) if need_def and self.where(item.tag) else path
        one_name = re.search(r"\}\s*\w+\s*;$", item.code().strip())
        td = next((n for n in item.names if " " not in n), None)  # name may be its tag, "struct X"
        if item.kind == "typedef" and item.struct and not is_library(path) and td and (one_name or not need_def) \
                and (self.where(td) is None or td in self.fwd_tags):
            # A game struct: forward-declared, and its layout, when a file needs it, defined under its tag. Runs in
            # parallel then write the same lines whether another run forward-declared the struct first or not.
            if self.where(td) is None:
                kind = "union" if re.match(r"typedef\s+(const\s+)?union\b", item.code()) else "struct"
                self.fwd_tags[td] = item.tag or f"{kind} {td}"
                self.place(Item(f"typedef {self.fwd_tags[td]} {td};"), STRUCT_DECLS)
            if not need_def:
                return self.where(td)
            self.place(item, path)
            return self.where(self.fwd_tags[td]) or path
        self.place(item, path)
        return self.placed.get(name)

    def place(self, item, path):
        if id(item) in self.done:
            return
        self.done.add(id(item))
        if item.cond:
            self.skipped.append((item, f"under {' '.join(item.cond)}"))
            return
        clash = [n for n in item.names if n in self.ours and not (self.ours[n][1].fwd and item.kind == "typedef")]
        clash += [f"{n} (in {self.beside[n]})" for n in item.names if n in self.beside and n not in self.ours]
        if clash:
            self.skipped.append((item, f"{', '.join(clash)} already declared here differently"))
            return
        note = self.note(item) if path != STRUCT_DECLS and not is_library(path) and not item.fwd else None
        if note == GAME_TYPE_NOTE and item.kind == "pp" and self.black:
            item, note = self.black_number(item, path)
        fwd_here = [n for n in item.names if n in self.fwd_tags]
        if item.kind == "typedef" and item.struct and not item.fwd and fwd_here:
            # This repo already typedefs the struct: define it under the tag this repo gives it
            tag = self.fwd_tags[fwd_here[0]]
            if tag in self.ours or tag in self.placed:
                return
            text = re.sub(r"^typedef\s+(struct|union)\s*(\w+\s*)?\{", f"{tag} {{", item.text)
            # the typedef's name goes, a comment after it stays: "} X; // size 0x14" (btlv_effect.h's BtlvMoveEffectParam
            # was left a variable of the struct, "illegal name overloading")
            text = re.sub(r"\}\s*\w+\s*;([ \t]*(?://[^\n]*|/\*.*?\*/))?\s*$", lambda m: "};" + (m[1] or ""), text,
                          flags=re.S)
            item = Item(text, item.comment, (), item.pos)
            self.includes[path].add(STRUCT_DECLS)  # what names it, here or in a header that includes this one
        item.note = note
        for name in item.names:
            self.placed[name] = path
        self.use(item, path)
        self.types[path].append(item)

    def note(self, item):
        """The note above a game type or constant: none when twinned code confirms all of it in Black, else what it
        confirms, else GAME_TYPE_NOTE."""
        code = item.code()
        if item.kind == "pp":
            return None if item.names and item.names[0] in self.used else GAME_TYPE_NOTE
        if item.kind not in ("typedef", "tag") or "{" not in code:
            return GAME_TYPE_NOTE
        size = ""
        if item.struct:
            key = item.tag or item.names[-1]
            everything, sure = self.records.get(key, []), set(self.fields.get(key, ()))
            what, size = "offsets of", ", and its size" if key in self.sizes else ""
        else:
            everything, what = enumerators(code), "values of"
            sure = set(everything) & self.used
        if not sure:
            return GAME_TYPE_NOTE
        if sure >= set(everything):
            return None
        sure = [n for n in everything if n in sure]
        text = f"Black 2's; twinned code confirms the {what} " + (", ".join(sure[:-1]) + " and " if sure[1:] else "") \
            + sure[-1] + size + " in Black"
        return "\n".join("// " + line for line in textwrap.wrap(text, 117))

    def black_number(self, item, path):
        """A constant's define and note: with Black's number under tools/black_constants.py's note when Black's files
        tell it, as it is under its note when Black lacks the name, else as it is under GAME_TYPE_NOTE."""
        header = path.removeprefix("include/constants/")
        define = black_constants.DEFINE.match(item.text)
        if header not in black_constants.HEADERS or not define:
            return item, GAME_TYPE_NOTE
        name, here = define[1], define[2]
        told = black_constants.numbers(self.black, header, {name: int(here, 0)})
        if name not in told:
            return item, GAME_TYPE_NOTE
        if told[name] is None:
            return item, black_constants.MISSING_NOTE
        number = f"#define {name} {black_constants.fmt(told[name], here)}"
        return Item(number, item.comment, item.cond, item.pos), black_constants.BLACK_NOTE

    def missing(self, item):
        """Whether this repo lacks something the item declares, or the layout of a struct it only forward-declares."""
        for name in item.names:
            if name not in self.ours and name not in self.placed:
                return True
            ours = self.ours.get(name, (None, None))[1]
            if ours is not None and ours.fwd and item.kind == "typedef" and item.struct and not item.fwd:
                if ours.tag not in self.ours and ours.tag not in self.placed:
                    return True
        return False

    def use(self, item, path):
        for name, need_def in dependencies(item):
            dep = self.provide(name, need_def)
            if dep is None and item.kind != "pp":
                self.unresolved[name].add(path)
            elif dep is not None and dep != path:
                self.includes[path].add(dep)

    def add_proto(self, item, path, twinned):
        self.done.add(id(item))
        self.use(item, path)
        self.protos[path].append((item, twinned))
        for name in item.names:
            self.placed[name] = path


def renamed(item, old, new):
    """pokebw2's declaration of old under Black's name new, with a comment that gives pokebw2's."""
    if old == new:
        return item
    copy = Item(re.sub(rf"\b{old}\b", new, item.text, count=1), item.comment, item.cond, item.pos)
    copy.note = None if B2_COMMENT.search(old) else f"// pokebw2's {old}"  # not a placeholder with B2's address
    return copy


def local_headers(plan, dest, src, text, bw2_root, includes_of):
    """pokebw2's headers beside its file (src/ov207/p_status_local.h) that the C file includes: what they declare joins
    what pokebw2's include/ headers do (plan.bw2), as the lines of the header beside the C file here, which gets what
    the C file uses (a new one if there is none). What a header beside the C file has already is in plan.ours, and its
    #include lines in includes_of."""
    if not dest.startswith("src/"):
        return
    by_name = {include_name(p): p for p in includes_of if not p.startswith("src/")}
    for name in re.findall(r'^#include\s+"([^"/]+\.h)"', text, re.M):
        theirs = Path(bw2_root) / os.path.dirname(src) / name
        if not theirs.exists():
            continue
        path = os.path.join(os.path.dirname(dest), name)
        plan.local[path] = os.path.relpath(theirs, bw2_root)
        includes_of.setdefault(path, set())
        for item in parse(theirs):
            if item.kind != "other" and not (item.kind == "pp" and not re.match(r"\s*#\s*define", item.text)):
                for n in item.names:
                    plan.bw2.setdefault(n, (path, item))
        if not (ROOT / path).exists():
            continue
        for item in parse(ROOT / path):
            m = re.match(r'\s*#\s*include\s+"([^"]+)"', item.text)
            if m and m[1] in by_name:
                includes_of[path].add(by_name[m[1]])
            if item.kind == "other" or item.kind == "pp" and not re.match(r"\s*#\s*define", item.text):
                continue
            for n in item.names:
                old = plan.ours.get(n)
                if old is None or old[1].fwd and not item.fwd and item.kind in ("typedef", "tag"):
                    plan.ours[n] = (path, item)
                if item.fwd and item.kind == "typedef" and item.tag:
                    plan.fwd_tags[n] = item.tag


def struct_items(items):
    """The structs and unions among items that have a layout."""
    return [it for it in items if it.kind in ("typedef", "tag") and it.struct and not it.fwd and "{" in it.code()
            and not it.cond]


def struct_users(dest, hpath, old, includes_of, fields=None):
    """The C files other than dest that access a field of the struct old (the Item of header hpath), or one of fields,
    through clang's AST, which have them reach it: [(file, [fields])]."""
    import check_layouts as cl
    fields = set(fields or re.findall(r"(\w+)\s*(?:\[[^\]]*\])?\s*(?::\s*\w+\s*)?;", old.code()))
    by_name = {include_name(p): p for p in includes_of}
    found = []
    paths = sorted(glob.glob(f"{ROOT}/src/**/*.c", recursive=True) + glob.glob(f"{ROOT}/lib/*/src/**/*.c",
                                                                               recursive=True))
    for path in paths:
        rel, text = os.path.relpath(path, ROOT), Path(path).read_text(errors="replace")
        if rel == dest or not re.search(r"(?:->|\.)\s*(?:" + "|".join(sorted(fields)) + r")\b", text):
            continue
        direct = [os.path.join(os.path.dirname(rel), m[1]) if os.path.join(os.path.dirname(rel), m[1]) in includes_of
                  else by_name.get(m[1]) for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"', text, re.M)]
        if not any(h and reaches(includes_of, h, hpath) for h in direct):
            continue
        hit = sorted({f for use in cl.uses(rel).values() for r, _, f in (x.rpartition(".") for x in use[0])
                      if r in old.names and f in fields})
        if hit:
            found.append((rel, hit))
    return found


def layout_rows(out, record):
    """[(offset, size, name, bitfield)] of the fields of a record itself (not those of its members), from clang's
    -fdump-record-layouts output; a size runs to the next field's offset. None if the output has no such record."""
    for block in out.split("*** Dumping AST Record Layout")[1:]:
        lines = [x for x in block.strip("\n").splitlines() if x.strip()]
        if not lines or lines[0].split("|", 1)[-1].strip() != record:
            continue
        rows, size = [], 0
        for x in lines[1:]:
            if "sizeof=" in x:
                size = int(re.search(r"sizeof=(\d+)", x)[1])
                break
            off, _, field = x.partition("|")
            byte = off.strip().split(":")[0]
            if byte.isdigit() and (len(field) - len(field.lstrip())) // 2 == 1:
                rows.append((int(byte), field.split()[-1], ":" in off))
        return [(o, (rows[i + 1][0] if i + 1 < len(rows) else size) - o, n, bit) for i, (o, n, bit) in enumerate(rows)]
    return None


def own_structs(plan, dest, items, text, includes_of, bw2_root, src):
    """A struct the C file lays out itself (pokebw2's C does for a struct its headers don't have) that a header here
    defines too is a second definition, "redefined". Which layout stays:
    - a header's from tools/struct_infer.py (its marker comment: unk_ fields only, no names) gives way to the C file's,
      pokebw2's named layout of Black 2's, noted as tools/port_decls.py notes a struct it copies. The C file's
      definition goes to the header, in the place of the inferred one, with the C file's own types and constants it
      needs (a PiecesManager its field points to), and loses its own. Unless another C file accesses a field of the
      inferred layout by its unk_ name: then the header's stays, as for a layout a worker made;
    - any other (a worker's, which matched code uses: BtlvScd) stays, and the C file takes its field names:
      adapt_struct(). Returns the C text without what moved."""
    for item in struct_items(items):
        found = [plan.ours[n] for n in item.names if n in plan.ours and plan.ours[n][1].struct
                 and not plan.ours[n][1].fwd and "{" in plan.ours[n][1].code()]
        if not found:
            continue
        hpath, old = found[0]
        users = struct_users(dest, hpath, old, includes_of) if "tools/struct_infer.py" in old.comment else True
        if users:
            if users is not True:
                print(f"{hpath}: {old.names[0]}'s inferred layout stays: " + "; ".join(
                    f"{f} accesses {', '.join(h)}" for f, h in users))
            text = adapt_struct(plan, dest, items, text, includes_of, bw2_root, src, item, hpath, old)
            continue
        text, block = move_to_header(plan, items, text, item, hpath, old, [])
        print(f"{hpath}: {old.names[0]}'s inferred layout gives way to the C file's, with "
              f"{len(block) - 1} of its types and constants")
    return text


def move_to_header(plan, items, text, item, hpath, old, extra):
    """The C file's struct item replaces old in header hpath (plan.replaced), with the C file's own types and constants
    it needs and those of the Items extra, which go before it: placed (their dependencies go to headers, and the
    includes), and taken out of the C text. Returns (the text, [the Items that took old's place])."""
    defined = {n: it for it in items if it.kind in ("pp", "typedef", "tag") and not it.cond for n in it.names}
    order, seen = [], set()  # what each needs first (a macro an array's length is), then the item, extra first

    def visit(cur):
        if id(cur) not in seen:
            seen.add(id(cur))
            for n, _ in sorted(dependencies(cur), key=lambda d: defined[d[0]].pos if d[0] in defined else 0):
                if n in defined:
                    visit(defined[n])
            order.append(cur)

    for cur in [*extra, item]:
        visit(cur)
    for n in old.names:
        plan.ours.pop(n, None)
    block = []
    for cur in order:
        plan.place(cur, hpath)
        if cur in plan.types[hpath]:
            plan.types[hpath].remove(cur)
            block.append(cur)
            if cur in items:  # not one made here
                chunk = (cur.comment + "\n" if cur.comment else "") + cur.text
                text = re.sub(r"\n*" + re.escape(chunk if chunk in text else cur.text) + r"\n*", "\n\n", text, count=1)
    plan.replaced[hpath].append((old, block))
    return text, block


def adapt_struct(plan, dest, items, text, includes_of, bw2_root, src, item, hpath, old):
    """The header's layout of a struct stays (a worker's, which matched code uses) and the C file's definition goes.
    clang lays out both, pokebw2's with its own flags (-fdump-record-layouts-complete) and the header's alone:
    - a field of pokebw2's at the offset of one of the header's, of the same size, is that one: its accesses take the
      header's name (plan.adapt: rename_members() compiles the C file for them once the headers are written);
    - a field of pokebw2's that lies in one of the header's unk_ arrays of u8, which no other C file accesses, is carved
      out of it, with the C file's own types and constants it needs: Black 2's layout is Black's for as far as its
      twinned functions show (BtlvScd's fields from 0x18 to 0x94), and those functions say which fields they are. It
      goes under a note that says which of them twinned code confirms. A field of a type or a size nothing here has,
      one that is not wholly inside one array, or a bitfield, is left, and the functions that use it do not compile.
    Prints what it did. Returns the C text without the struct and what moved with it."""
    import check_layouts as cl
    tag = next((n for n in item.names if n.startswith(("struct ", "union "))), None)
    if tag is None or tag not in old.names:
        print(f"{dest}: {item.names[0]} is untagged, which the header has too: left to a worker")
        return text
    flags = (Path(bw2_root) / "compile_flags.txt").read_text().split() + ["-fpack-struct=4"]  # MWCC's u64
    theirs = layout_rows(subprocess.run(["clang", "-fsyntax-only", "-ferror-limit=0", *flags, "-Xclang",
                                         "-fdump-record-layouts-complete", src], cwd=bw2_root, capture_output=True,
                                        text=True).stdout, tag)
    mine = layout_rows(cl.clang(dest, "-Xclang", "-fdump-record-layouts-complete",
                                text=f'#include "{include_name(hpath)}"\n'), tag)
    if not theirs or not mine:
        print(f"{dest}: clang gives no layout of {tag} here or in pokebw2: left to a worker")
        return text
    norm = lambda n: n.replace("_", "").lower()
    at = defaultdict(lambda: ([], []))
    for o, s, n, _ in mine:
        at[o][0].append((n, s))
    for o, s, n, _ in theirs:
        at[o][1].append((n, s))
    rename = {}
    for off, (here, there) in at.items():
        if len(here) == len(there) and (len(there) > 1 or here[0][1] == there[0][1]):
            # not onto a header's named field (BtlvScd's core is not pokebw2's answer, which only lies at its offset)
            rename.update({b: a for (a, _), (b, _) in zip(here, there)
                           if a != b and (a.startswith("unk") or norm(a) == norm(b))})
    # the unk_ arrays of u8 with fields of pokebw2's inside them
    defined = {n: it for it in items if it.kind in ("pp", "typedef", "tag") and not it.cond for n in it.names}
    body = item.text.split("\n")
    lines, carved, extra = old.text.split("\n"), [], []
    for o, s, name, _ in mine:
        inside = [t for t in theirs if o <= t[0] < o + s]
        if not name.startswith("unk") or not inside or (len(inside) == 1 and inside[0][:2] == (o, s)):
            continue
        row = next((i for i, l in enumerate(lines) if re.match(rf"\s*u8\s+{name}\s*\[", l)), None)
        decls = {n: next((l for l in body if re.search(rf"\b{n}\b\s*(\[[^\]]*\]\s*)*;", l)), None) for _, _, n, _ in inside}
        deps = {d for l in decls.values() if l for d, _ in dependencies(Item(re.sub(r"//.*", "", l)))}
        if row is None or any(t[0] + t[1] > o + s or t[3] for t in inside) or len({t[0] for t in inside}) < len(inside) \
                or not all(decls.values()) or any(d not in plan.ours and d not in plan.bw2 and d not in plan.placed
                                                  and d not in defined and d not in KEYWORDS for d in deps) \
                or struct_users(dest, hpath, old, includes_of, {name}):
            print(f"{dest}: {tag}'s {name} is left whole")
            continue
        out, cursor = [], o
        for t in sorted(inside):
            if t[0] > cursor:
                out.append(f"    u8 unk_{cursor:02x}[{t[0] - cursor:#x}];")
            out.append(decls[t[2]].rstrip())
            cursor = t[0] + t[1]
        if cursor < o + s:
            out.append(f"    u8 unk_{cursor:02x}[{o + s - cursor:#x}];")
        lines[row] = "\n".join(out)
        carved += [t[2] for t in inside]
        extra += [defined[d] for d in deps if d in defined]
        for t in inside:
            rename.pop(t[2], None)
    if carved:
        new = Item("\n".join(lines), old.comment, (), old.pos)
        text, block = move_to_header(plan, items, text, new, hpath, old, extra)
        sure = [n for n in carved if n in plan.fields.get(tag, ())]
        new.note = "\n".join("// " + line for line in textwrap.wrap(
            f"{', '.join(carved)} carved out of the unk_ arrays, from pokebw2's layout"
            + (f"; twinned code confirms the offsets of {', '.join(sure)} in Black" if sure else ", not checked against"
               " Black"), 117))
        print(f"{hpath}: {tag}: {', '.join(carved)} carved out of the unk_ arrays")
    chunk = (item.comment + "\n" if item.comment else "") + item.text  # the C file's struct goes either way
    text = re.sub(r"\n*" + re.escape(chunk if chunk in text else item.text) + r"\n*", "\n\n", text, count=1)
    plan.adapt.append((tag, rename))
    return text


def rename_members(dest, record, rename):
    """Compiles the C file dest, and at each line MWCC says "'x' is not a member of class 'struct <record>'" for an x
    in rename, x becomes rename[x], for up to 4 rounds (a name another struct has too is not touched elsewhere).
    Returns how many accesses it renamed."""
    obj = f"build/b_us/{os.path.splitext(dest)[0]}.o"
    base, count = record.split()[-1], 0
    for _ in range(4):
        run = subprocess.run(["nice", "-n", "10", "ninja", obj], cwd=ROOT, capture_output=True, text=True)
        hits = {(int(line), name) for line, name, tag in re.findall(
            r"\.c:(\d+): '(\w+)' is not a member of class '(?:struct|union)?\s*(?:\S+:\d+: )?(\w+)'",
            run.stdout + run.stderr) if tag == base and name in rename}
        if not hits:
            break
        lines = (ROOT / dest).read_text().split("\n")
        for line, name in sorted(hits):
            lines[line - 1] = re.sub(rf"(->|\.)(\s*){name}\b", lambda m: m[1] + m[2] + rename[name], lines[line - 1])
            count += 1
        (ROOT / dest).write_text("\n".join(lines))
    return count


def adapt_structs(plan, dest):
    """Once the headers are written: the accesses in the C file of the fields adapt_struct() matched by offset take the
    header's names. Prints what it did."""
    for tag, rename in plan.adapt:
        if rename:
            done = rename_members(dest, tag, rename)
            print(f"{dest}: {tag}: {done} accesses renamed to the header's names ("
                  + ", ".join(f"{a} to {b}" for a, b in list(rename.items())[:8]) + ")")


def own_prototypes(plan, funcs, b2_of, black_has, declare, reached):
    """-requireprotos: a function the C file defines, not static, needs a prototype the file sees. One that a header
    the file doesn't reach declares already (btl_tables.c's IsArceus and data_ov095_0689c8fd, in btl_server_flow.h
    and btl_server_flow_sub.h, which their callers wrote: t34h added them to btl_tables.h by hand, 2026-10-27) gets
    pokebw2's prototype in pokebw2's header when the file reaches that header and the two prototypes agree; else the
    file includes the header that declares it. Returns the headers to include."""
    need = set()
    for item in funcs:
        name = item.names[0]
        here = plan.where(name)
        if re.match(r"\s*static\b", item.code()) or here is None or ({here} | plan.declared[name]) & reached:
            continue
        b2 = b2_of.get(name, name)
        path, theirs = plan.bw2.get(b2, (None, None))
        mine = plan.ours.get(name, (None, None))[1]
        if theirs is not None and theirs.kind == "proto" and not theirs.cond and path in reached and \
                black_has(b2, name) and (mine is None or signature(theirs.code()) == signature(mine.code())):
            declare(theirs, path, b2, name)
        else:
            need.add(here)
    return need


def plan_file(plan, dest, bw2_root, twins, includes_of):
    """Adds to the plan what the C file dest, which tools/port_file.py ported, uses and the headers lack, from the
    pokebw2 file its first line names: the layouts of the structs its functions need whole (from clang's AST of
    pokebw2's file), the types and constants it names, and the prototypes of the functions and data of Black it calls
    or defines, under the names port_file.py gave them. Returns the file's text with an #include of each header that
    declares something it uses and that its #include lines don't reach, and without those of headers this repo lacks."""
    text = (ROOT / dest).read_text()
    m = re.match(r"// From pokebw2 \([^)]*\), (\S+), ported by tools/port_file\.py", text)
    if not m:
        sys.exit(f"{dest}: its first line names no pokebw2 file that tools/port_file.py ported")
    src = m.group(1)
    b2_of = name_map(text, (Path(bw2_root) / src).read_text())  # our name -> pokebw2's
    # A library's private headers beside its file in pokebw2 (DWC's vct_internal.h): copied whole, like its public ones,
    # with the names port_file.py gave the file (TwlSDK's photo_sign_private.h declared PhotoSign_SeedRandom, which
    # photo_sign.c calls as Black's func_0270741c), in a copy made for another file of the library too
    if is_library(dest):
        for name in re.findall(r'^#include\s+"([^"/]+\.h)"', text, re.M):
            theirs = Path(bw2_root) / os.path.dirname(src) / name
            path = os.path.join(os.path.dirname(dest), name)
            if theirs.exists() and not (ROOT / path).exists():
                plan.private[path] = theirs
            if theirs.exists():
                plan.private_names.setdefault(path, {}).update({b2: n for n, b2 in b2_of.items() if n != b2})
    black = read_symbols(str(ROOT), every=True)
    items = parse(ROOT / dest)
    local_headers(plan, dest, src, text, bw2_root, includes_of)
    text = own_structs(plan, dest, items, text, includes_of, bw2_root, src)
    defined, local = set(), set()
    for item in items:
        defined.update(item.names)
        if item.kind != "pp":
            local |= declarator_names(tokens(item.text))  # parameters, variables, fields
    funcs = [item for item in items if item.kind == "func"]

    def black_has(b2, name):
        """Whether Black has the function or data pokebw2 calls b2 under the name the C file gives it: a dsd
        placeholder port_file.py left as it was can name another function in each game, unless a twin pairs them."""
        return name in black and (b2 != name or not re.fullmatch(r"(func|data)_(ov\d+_)?[0-9a-f]{8}", name) or
                                  b2 in twins.get(black[name], ()))

    def declare(item, path, b2, name):
        if name in plan.beside and name not in plan.ours:  # a header beside C files has it, in their words
            return
        public = path.startswith("lib/") or path in C_LIBRARY
        plan.add_proto(renamed(item, b2, name), path, item.kind == "proto" and (
            public or b2 in twins.get(black[name], ())))

    def inline(item, path, seen):
        """Places an inline function of pokebw2's headers with the functions it calls, if Black has them all."""
        body = tokens(item.text)
        calls = {t for t, n in zip(body, body[1:]) if n == "(" and t in plan.bw2 and plan.where(t) is None} - seen
        protos = {c for c in calls if plan.bw2[c][1].kind == "proto"}
        if any(not black_has(c, c) or plan.bw2[c][1].cond for c in protos) or not all(
                inline(plan.bw2[c][1], plan.bw2[c][0], seen | {c}) for c in sorted(calls)
                if plan.bw2[c][1].kind == "func"):
            return False
        for c in sorted(protos):
            declare(plan.bw2[c][1], plan.bw2[c][0], c, c)
        plan.place(item, path)
        return True

    whole = set()
    ast = clang_ast(bw2_root, src) if shutil.which("clang") else None
    if ast:
        owner, plan.b2_fields, own_records = ast_records(ast)
        for _, records, _ in ast_functions(ast, owner, {b2_of.get(f.names[0], f.names[0]) for f in funcs}).values():
            whole |= records - own_records
    toks = tokens(text)
    used = []  # the names it uses, not as a field
    for i, t in enumerate(toks):
        if not is_name(t) or t in KEYWORDS or t in defined or t in local or toks[i - 1] == "." or \
                toks[i - 1] == ">" and toks[i - 2] == "-":
            continue
        used.append(f"{toks[i - 1]} {t}" if toks[i - 1] in ("struct", "union", "enum") else t)
    bare = set(used)
    for record in sorted(whole):
        plan.provide(record, True)
    for item in items:
        if item.kind != "pp":  # what the file's types, data, prototypes and local variables need
            for name, need_def in dependencies(item):
                if name not in defined and (name in bare or " " in name):
                    plan.provide(name, need_def)
                    if need_def:
                        whole.add(name)
    for name in dict.fromkeys(used):
        b2 = b2_of.get(name, name)
        if plan.where(name) is not None or b2 not in plan.bw2 or " " in name and name not in whole:
            continue  # a struct only pointed to needs no declaration
        path, item = plan.bw2[b2]
        if item.kind in ("proto", "var"):
            if black_has(b2, name) and not item.cond:  # not one of Black 2's own
                declare(item, path, b2, name)
        elif item.kind == "func":
            inline(item, path, {name})
        else:
            plan.provide(b2, False)
    for item in funcs:  # the file's own functions, which other files call
        name = item.names[0]
        b2 = b2_of.get(name, name)
        if not re.match(r"\s*static\b", item.code()) and plan.where(name) is None and black_has(b2, name) and \
                b2 in plan.bw2 and plan.bw2[b2][1].kind == "proto" and not plan.bw2[b2][1].cond:
            declare(plan.bw2[b2][1], plan.bw2[b2][0], b2, name)

    # The headers the file must reach, through the #include lines it has or new ones
    changed = set(plan.types) | set(plan.protos)
    by_include = {include_name(p): p for p in set(includes_of) | changed if p not in plan.local}
    by_include.update({include_name(p): p for p in plan.local})  # beside the C file: found first
    graph = defaultdict(set)
    for path in set(includes_of) | changed:
        graph[path] = set(includes_of.get(path, ())) | plan.includes.get(path, set())
    lines = text.split("\n")
    kept, reached = [], set()
    for line in lines:
        m = re.match(r'#include\s+"([^"]+)"', line)
        if m and m.group(1) not in by_include and not (ROOT / os.path.dirname(dest) / m.group(1)).exists() and \
                os.path.join(os.path.dirname(dest), m.group(1)) not in plan.private:
            continue  # a header this repo lacks, of which the file needs nothing
        kept.append(line)
        todo = [by_include[m.group(1)]] if m and m.group(1) in by_include else []
        while todo:
            path = todo.pop()
            if path not in reached:
                reached.add(path)
                todo += graph.get(path, ())
    # a struct used whole needs the header of its layout, which is under its tag when struct_decls.h has its typedef
    need = {plan.where(name) for name in set(used) - whole} | {
        plan.where(plan.fwd_tags[name]) if plan.where(plan.fwd_tags.get(name)) else plan.where(name) for name in whole}
    need = (need | own_prototypes(plan, funcs, b2_of, black_has, declare, reached)) - reached - {None, dest}
    if need:
        last = max(i for i, line in enumerate(kept) if line.startswith("#include") or i == 0)
        kept[last + 1:last + 1] = [f'#include "{include_name(p)}"' for p in sorted(need)]
    return "\n".join(kept)


def local_declarations(root, skip=None):
    """{name: Item} of the functions the C here declares or defines at the top level (not inline) with a prototype of
    their own: a complete file's loose prototype (void *, a u32 for a u16) that MWCC matched Black's code with, kept by
    tools/port_file.py under the name it gave the function. A static declaration the file gives no definition is one
    too: a call into code that was no file's yet (plist_menu.c's forward declaration of plist_sys.c's function).
    None for one that two files declare differently."""
    out, every = {}, defaultdict(list)
    for path in sorted(glob.glob(f"{root}/src/**/*.c", recursive=True) + glob.glob(f"{root}/lib/*/src/**/*.c",
                                                                                   recursive=True)):
        if skip and os.path.abspath(path) == os.path.abspath(skip):
            continue
        if is_libntr(path):
            continue
        items = [it for it in parse(path) if it.kind in ("proto", "func") and not it.cond
                 and not re.match(r"\s*(asm|__asm)\b", it.code())]
        defined = {n for it in items if it.kind == "func" for n in it.names}
        for item in items:
            static = re.match(r"\s*static\b", item.code())
            if not re.match(r"\s*(inline|extern)\b", item.code()) and (
                    not static or item.kind == "proto" and item.names[0] not in defined):
                out.setdefault(item.names[0], item)
                every[item.names[0]].append(item)
    # Files that declare it in their own words but disagree (game_data.c's s32 func_02007168 and
    # save_control_intr.c's u32): any prototype in a header breaks one of them, so it gets none
    for name, items in every.items():
        sigs = {signature(re.sub(r"^\s*static\s+", "", " ".join(it.code().split("{")[0].split()).rstrip(";")))
                for it in items}
        if len(sigs) > 1:
            out[name] = None
    return out


def is_libntr(path):
    """Whether the C file is libntr's, ported as it is (tools/port_lib.py) with libntr's own SDK headers
    (lib/nitro/sdk): its types (fx64c, MATHCRC16Context) and asm functions are not ours in include/ and lib/*/include."""
    with open(path, errors="replace") as f:
        return f.readline().startswith("// From libntr")


C_TYPE_WORDS = {"void", "int", "char", "short", "long", "unsigned", "signed", "float", "double", "const"}


def signature(code):
    """A prototype's return type and parameter types as token lists, the parameter names left out."""
    code = " ".join(strip_comments(code).split("{")[0].split()).rstrip(";")
    m = re.match(r"(.*?)\b\w+\s*\((.*)\)\s*$", code)
    if not m:
        return code
    params, depth, cur = [], 0, ""
    for ch in m[2]:
        depth += ch == "("
        depth -= ch == ")"
        if ch == "," and depth == 0:
            params.append(cur)
            cur = ""
        else:
            cur += ch
    params.append(cur)
    out = []
    for p in params:
        toks = re.findall(r"\w+|\*|\.\.\.|[\[\]()]", p)
        if len(toks) > 1 and is_name(toks[-1]) and toks[-1] not in C_TYPE_WORDS and toks[-2] not in (
                "struct", "union", "enum") and "(" not in toks:
            toks = toks[:-1]
        out.append(tuple(toks))
    return tuple(re.findall(r"\w+|\*", m[1])), tuple(out)


def adopt_local_declarations(plan, root, skip=None):
    """A prototype a header gets for a function that a C file already declares or defines in its own words is the
    C file's: it compiles to Black's code, and a header that disagrees with it (pokebw2's u16 where Black's file
    has a u32, a typed pointer for its void *) fails to compile the file: the build then fails on a file that is
    complete (the setup of src/ov010/fldmmdl.c broke game_data.c and field_actor_animation.c this way). One that C
    files declare differently gets none. Returns the names whose prototype changed."""
    local, changed = local_declarations(root, skip), []
    for path, items in plan.protos.items():
        for item, tw in list(items):
            name = item.names[0] if item.names else None
            if name not in local or item.kind != "proto":
                continue
            if local[name] is None:  # C files that disagree: no header prototype
                items.remove((item, tw))
                plan.placed.pop(name, None)
                changed.append(f"{name} (none: C files declare it differently)")
                continue
            code = " ".join(local[name].code().split("{")[0].split()).rstrip(";").strip()
            code = re.sub(r"^static\s+", "", code)
            if signature(item.code()) != signature(code):
                # the C file's words name its own types (natneg.c's NatNegPeer, card_internal.h's CARDAccessLevel):
                # in a header that can't see them, the prototype is a compile error in every file that includes it
                # (NATify.h, card.h: 15 files of prep's queue skipped, 2026-10-24). It stays where its types are
                deps = [n for n, _ in dependencies(Item(code + ";")) if " " not in n]
                if any(plan.where(n) is None for n in deps):
                    items.remove((item, tw))
                    plan.placed.pop(name, None)
                    changed.append(f"{name} (none: its types are the C's own: "
                                   f"{', '.join(n for n in deps if plan.where(n) is None)})")
                    continue
                item.text = code + ";"
                changed.append(name)
                for n in deps:
                    if plan.where(n) not in (None, path):
                        plan.includes[path].add(plan.where(n))
                # A tag only the C file defines, first named in a header's parameter list, is another type there (of
                # the prototype's scope): btlv_gauge.c's struct BtlvUnkWork. Declared in struct_decls.h
                for tag in re.findall(r"\b((?:struct|union) \w+)\s*\*", code):
                    if plan.where(tag) is None and not any(tag in it.names for it in plan.types[STRUCT_DECLS]):
                        plan.place(Item(f"{tag};"), STRUCT_DECLS)
    return changed


def adopt_local_types(plan, root, includes_of, by_include, skip=None):
    """A struct, union or enum that a C file defines itself, with a layout of its own, goes to the header it includes
    in the C file's words, and the C file loses its definition: pokebw2's layout beside it is a second definition,
    a compile error in a file that is complete (the setup of mb_parent_sys.c broke mb_comm_sys.c, whose
    MBCommParentInfo is a u16 where pokebw2's has three fields). The file that needs pokebw2's layout then lacks the
    fields, and the driver drops the functions that use them. Defined by more than one C file, it is left out of the
    headers. Returns ({C file: [definitions to take out of it]}, the tags left out)."""
    defined = defaultdict(dict)  # tag -> {C file: Item}
    parsed = {}  # C file -> [Item]
    for path in sorted(glob.glob(f"{root}/src/**/*.c", recursive=True)):
        if skip and os.path.abspath(path) == os.path.abspath(skip):
            continue
        parsed[path] = parse(path)
        for item in parsed[path]:
            if item.kind in ("tag", "typedef") and not item.fwd and "{" in item.code() and not item.cond:
                for name in item.names:  # a typedef's name too, for the types of a header beside the C files
                    if re.match(r"(struct|union|enum) \w+$", name) or (
                            plan.local and item.kind == "typedef" and " " not in name):
                        defined[name][path] = item
    # A typedef'd struct a C file defines itself, anonymous (lib/gfl/src/clact.c's ClActSys): a forward declaration
    # of the name in a header is another type to MWCC, "identifier redeclared" in that file. Left out, with the
    # prototypes that name it.
    own_typedefs, dropped = set(), []
    for path in sorted(glob.glob(f"{root}/src/**/*.c", recursive=True) + glob.glob(f"{root}/lib/*/src/**/*.c",
                                                                                   recursive=True)):
        if (skip and os.path.abspath(path) == os.path.abspath(skip)) or is_libntr(path):
            continue
        for item in parse(path):
            if item.kind == "typedef" and item.struct and not item.fwd and not item.tag and "{" in item.code()                     and not item.cond:
                own_typedefs.update(n for n in item.names if " " not in n)
    for hpath, items in plan.types.items():
        for item in list(items):
            if item.kind == "typedef" and item.fwd and any(n in own_typedefs for n in item.names):
                items.remove(item)
                gone = set(item.names)
                for n in item.names:
                    plan.placed.pop(n, None)
                for protos in plan.protos.values():
                    for entry in list(protos):
                        if gone & set(tokens(entry[0].text)):
                            protos.remove(entry)
                            dropped.append(entry[0].names[0])
    moved, reach = defaultdict(list), {}  # reach: C file -> the headers it includes, directly or not
    for hpath, items in plan.types.items():
        for item in list(items):
            tags = [n for n in item.names if n in defined and (" " in n or hpath in plan.local)]
            if not tags and hpath in plan.local:  # a struct typedef'd by a C file, named by its tag here
                tags = [td for td, tag in plan.fwd_tags.items() if tag in item.names and td in defined]
            if not tags or "{" not in item.code():
                continue
            files = {}
            for cfile in defined[tags[0]]:
                if cfile not in reach:
                    beside = os.path.relpath(os.path.dirname(cfile), root)  # found first
                    reach[cfile] = {os.path.join(beside, m[1]) if os.path.join(beside, m[1]) in plan.local
                                    else by_include[m[1]] for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"',
                                                                               Path(cfile).read_text(), re.M)
                                    if m[1] in by_include or os.path.join(beside, m[1]) in plan.local}
                if any(reaches(includes_of, h, hpath) for h in reach[cfile]):
                    files[cfile] = defined[tags[0]][cfile]
            if len(files) == 1:
                cfile, local = next(iter(files.items()))
                item.text, item.comment, item.note = local.text, local.comment, None
                moved[cfile].append(local)
                dropped.append(tags[0])
                # the macros it uses (an array's length) that the C file defines go with it, above it
                for dep, _ in dependencies(local):
                    macro = next((it for it in parsed[cfile] if it.kind == "pp" and dep in it.names and not it.cond
                                  and re.match(r"\s*#\s*define\b", it.text)), None)
                    if macro and plan.where(dep) is None and macro not in moved[cfile]:
                        moved[cfile].append(macro)
                        plan.placed[dep] = hpath
                        items.append(Item(macro.text, macro.comment, (), item.pos - 0.5))
            elif files:
                items.remove(item)
                dropped.append(tags[0])
    return moved, dropped


def render(item):
    lines = [item.note] if item.note else []
    if item.comment and not B2_COMMENT.search(item.comment):
        lines.append(item.comment)
    return "\n".join(lines + [item.text])


def join(rendered, blank_around_long=False):
    """Declarations one per line; with blank_around_long, a blank line around any of more than a line."""
    out, last_long = "", False
    for text in rendered:
        long = blank_around_long and "\n" in text
        if out and (long or last_long):
            out += "\n"
        out += text + "\n"
        last_long = long
    return out


def reaches(includes_of, src, dst, seen=None):
    """Whether header src includes dst, directly or not."""
    seen = seen if seen is not None else set()
    if src == dst:
        return True
    seen.add(src)
    return any(reaches(includes_of, n, dst, seen) for n in includes_of.get(src, ()) if n not in seen)


def render_types(items):
    """render() of each item, but a run of one-line items with the same note gets it once."""
    out, last = [], None
    for it in items:
        if out and it.note and it.note == last.note and "\n" not in it.text + last.text and not last.comment:
            out[-1] += "\n" + render(it).split("\n", it.note.count("\n") + 1)[-1]
        else:
            out.append(render(it))
        last = it
    return out


STD_HEADERS = {"assert.h", "ctype.h", "errno.h", "float.h", "limits.h", "locale.h", "math.h", "setjmp.h", "signal.h",
               "stdarg.h", "stddef.h", "stdio.h", "stdlib.h", "string.h", "time.h", "wchar.h"}


def write_header(path, plan, includes_of, bw2_root, what="the prototypes of Black's named functions and the types "
                 "they need"):
    """Adds the plan's items for path to our header, or makes the header, its first line saying what it has. In a header
    with unchecked prototypes already, the types and the other prototypes go above them and the unchecked ones after."""
    full = ROOT / path
    if Path(path).name in STD_HEADERS and not full.exists():
        return  # a header named like the compiler's hides it from every lib file that includes <stdio.h> (2026-10-15)
    types = sorted(plan.types[path], key=lambda it: it.pos)
    black = [it for it in types if it.note in BLACK_NOTES]  # under its notes, where tools/black_constants.py adds them
    types = [it for it in types if it.note not in BLACK_NOTES]
    decls = join(render_types([it for it in types if it.kind != "func"]), blank_around_long=True)
    twinned = join([render(it) for it, tw in plan.protos[path] if tw])
    unchecked = join([render(it) for it, tw in plan.protos[path] if not tw])
    inline = join(render_types([it for it in types if it.kind == "func"]), blank_around_long=True)  # after its callees
    split = full.exists() and UNTWINNED_NOTE in full.read_text()
    above = "\n".join(p for p in (decls, twinned) if p)
    below = "\n".join(p for p in (unchecked if split else unchecked and UNTWINNED_NOTE + "\n" + unchecked, inline) if p)
    body = "\n".join(p for p in (above, below) if p)
    # types.h first and struct_decls.h last, as pokebw2 has them
    includes = sorted((include_name(p) for p in plan.includes[path] if path != STRUCT_DECLS
                       and not reaches(includes_of, path, p)),
                      key=lambda n: (n != "types.h", n == "struct_decls.h", n))
    if full.exists():
        lines = full.read_text().split("\n")
        if includes:
            last = max((i for i, line in enumerate(lines) if line.startswith("#include")), default=None)
            add = [f'#include "{n}"' for n in includes]
            if last is None:
                guard = next(i for i, line in enumerate(lines) if line.startswith("#define"))
                lines[guard + 1:guard + 1] = [""] + add
            else:
                lines[last + 1:last + 1] = add
        text = "\n".join(lines)
        for old, news in plan.replaced.get(path, ()):  # a layout the C file's takes the place of
            was = (old.comment + "\n" if old.comment else "") + old.text
            text = text.replace(was if was in text else old.text,
                                join(render_types(news), blank_around_long=True).rstrip("\n"), 1)
        if split:
            at = text.index(UNTWINNED_NOTE)
            if unchecked:  # after the unchecked prototypes there, under the same note
                end = text.find("\n\n", at)
                end = end if end >= 0 else text.rindex("\n#endif")
                text = text[:end] + "\n" + unchecked.rstrip("\n") + text[end:]
            if above:
                text = text[:at] + above + "\n" + text[at:]
            body = inline
        if body:
            end = text.rindex("#endif")
            text = text[:end].rstrip("\n") + "\n\n" + body + "\n" + text[end:]
        full.write_text(under_notes(text, black))
        return
    guard = "BWPORT_" + re.sub(r"\W", "_", path[len("src/"):] if path.startswith("src/") else include_name(path)).upper()
    if is_library(path):
        what = "its types and constants, and the prototypes of Black's named functions"
    source = Path(bw2_root) / plan.local.get(path, path)
    if not source.exists():  # one it generates
        source = Path(bw2_root) / "build/include/generated" / include_name(path)
    head = ["// " + line for line in textwrap.wrap(f"From pokebw2 (GPL-3.0, {URL}), "
                                                   f"{source.relative_to(bw2_root)}: {what}.", 117)]
    head += [f"#ifndef {guard}", f"#define {guard}", ""]
    # pokebw2's note on the header, or at least its credit to swan
    m = re.search(r"#define \w+_H\n\n((?://[^\n]*\n)+)", source.read_text())
    if m and "swan" in m.group(1):
        note = m.group(1).rstrip("\n")
        head += [SWAN_NOTE if B2_COMMENT.search(note) else note, ""]
    head += [f'#include "{n}"' for n in ["types.h"] + [n for n in includes if n != "types.h"]] + [""]
    full.parent.mkdir(parents=True, exist_ok=True)
    full.write_text(under_notes("\n".join(head) + "\n" + body + f"\n#endif // {guard}\n", black))


def under_notes(text, items):
    """A header's text with the items under their notes (BLACK_NOTES)."""
    for note in BLACK_NOTES:
        lines = [render(it).removeprefix(note + "\n") for it in items if it.note == note]
        if lines:
            text = black_constants.add_under(text, note, lines)
    return text


def check():
    """Compiles every header alone and all of them together with the game's compiler and flags, which must not warn, and
    lists the names two headers declare. Returns whether all is well."""
    sys.path.insert(0, str(ROOT))
    import configure
    paths = header_files(str(ROOT))
    ok = True
    names = defaultdict(list)
    for path in paths:
        for item in parse(ROOT / path):
            if item.kind not in ("other", "pp") or item.kind == "pp" and item.names:
                for name in item.names:
                    names[(name, item.fwd)].append(path)  # a struct's typedef and its definition are one each
    for (name, _), where in sorted(names.items()):
        if len(where) > 1:
            print(f"declared twice: {name} in {', '.join(sorted(where))}")
            ok = False
    work = ROOT / "build" / "header_check"
    work.mkdir(parents=True, exist_ok=True)
    cc = [str(ROOT / "tools/wibo"), str(ROOT / "tools/mwccarm" / configure.MWCC_VERSION / "mwccarm.exe")]
    flags = " ".join(configure.CC_FLAGS).split() + [a for d in configure.INCLUDE_DIRS for a in ("-i", d)]

    def compile_one(job):
        name, headers = job
        source = work / f"{name}.c"
        source.write_text("".join(f'#include "{include_name(h)}"\n' for h in headers))
        run = subprocess.run(cc + flags + ["-o", str(work / f"{name}.o"), str(source.relative_to(ROOT))], cwd=ROOT,
                             capture_output=True, text=True)
        return name, run.returncode, (run.stdout + run.stderr).strip()

    jobs = [(re.sub(r"\W", "_", path), [path]) for path in paths] + [("all_headers", paths)]
    with ThreadPoolExecutor(os.cpu_count()) as pool:
        for name, code, out in pool.map(compile_one, jobs):
            if code or "warning" in out:
                ok = False
                print(f"{name}: " + "\n".join(out.split("\n")[:6]))
    print("headers check:", "ok" if ok else "FAILED")
    return ok


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--bw2", default=os.path.expanduser("~/ref/pokebw2"), help="pokebw2 checkout (read only)")
    ap.add_argument("--twins", nargs="+", help="tools/twins.py's tables, ARM and Thumb (default: "
                    "build/twins_b_us.tsv and build/twins_b_us.thumb.tsv, here or in the main checkout)")
    ap.add_argument("--apply", action="store_true", help="write the headers")
    ap.add_argument("--file", help="declare only what this C file, which tools/port_file.py ported, uses (not every "
                    "named function's prototype and every library header's types: that is a run without --file)")
    ap.add_argument("--file-only", action="store_true", help="what --file does anyway (tools/local_driver.py passes it)")
    ap.add_argument("--check", action="store_true", help="only compile every header, alone and together")
    ap.add_argument("-v", "--verbose", action="store_true", help="list what is left out")
    args = ap.parse_args()
    if args.check:
        raise SystemExit(0 if check() else 1)

    guesses = {parts[2] for line in open(f"{args.bw2}/config/names.txt")
               if len(parts := line.split()) == 3 and not line.startswith("#")}
    # pokebw2's names.txt also has the SDK's public names, which aren't guesses
    sdk = set()
    for path in glob.glob(f"{args.bw2}/lib/*/include/**/*.h", recursive=True):
        sdk.update(re.findall(r"\b([A-Z][A-Za-z0-9]*_\w+)\s*\(", Path(path).read_text()))
    bw2_paths = header_files(args.bw2)
    bw2, bw2_files = index(args.bw2, bw2_paths)
    # The constants and message IDs pokebw2 generates from its data (`ninja constants_headers` there), Black 2's
    generated = f"{args.bw2}/build/include/generated"
    paths = sorted(glob.glob("**/*.h", root_dir=generated, recursive=True))
    for name, (path, item) in index(generated, paths)[0].items():
        bw2.setdefault(name, (f"include/{path}", item))
    our_paths = header_files(str(ROOT))
    ours, our_files = index(str(ROOT), our_paths)
    by_include = {include_name(p): p for p in our_paths}
    includes_of = {p: {by_include[m.group(1)] for it in items
                       if (m := re.match(r'\s*#\s*include\s+"([^"]+)"', it.text)) and m.group(1) in by_include}
                   for p, items in our_files.items()}
    symbols = read_symbols(str(ROOT))
    twin_paths = args.twins or default_twins()
    twins = read_twins(twin_paths)
    statics = static_in_sources(str(ROOT))
    private = private_names(str(ROOT))

    plan = Planner(bw2, ours, twin_evidence(args.bw2, twin_paths), black_constants.read_black(args.bw2))
    plan.beside = private
    for path, items in our_files.items():
        for item in items:
            if item.kind in ("proto", "var", "func"):
                for name in item.names:
                    plan.declared[name].add(path)
    stats = defaultdict(list)
    for name in sorted(symbols, key=lambda n: (bw2[n][0], bw2[n][1].pos) if n in bw2 else ("", 0)):
        if args.file:
            break
        if name in ours:
            stats["declared here already"].append(name)
        elif name in private:
            stats["declared in a header beside its C here"].append(name)
        elif name in guesses and name not in sdk:
            stats["a pokebw2 guess (config/names.txt)"].append(name)
        elif name in statics:
            stats["static in Black's C"].append(name)
        elif name not in bw2:
            stats["in no pokebw2 header (static there, or not C)"].append(name)
        elif bw2[name][1].kind != "proto":
            stats[f"pokebw2 has it as a {bw2[name][1].kind}"].append(name)
        elif bw2[name][1].cond:
            stats["under a condition in pokebw2"].append(name)
        else:
            path, item = bw2[name]
            # An SDK or C library function keeps its public prototype, twinned or not
            public = path.startswith("lib/") or path in C_LIBRARY
            plan.add_proto(item, path, public or name in twins.get(symbols[name], ()))
            stats["prototypes added"].append(name)
    own = types_in_sources(str(ROOT))  # a library header's copy of one would clash with the C file's (anm.c, 2026-10-15)
    for path in [] if args.file else bw2_paths:
        if is_library(path):
            for item in bw2_files[path]:
                if (item.kind in ("typedef", "tag") or item.kind == "pp" and item.names and not re.match(
                        r"\w+_H$", item.names[0])) and plan.missing(item) and \
                        not {n.split()[-1] for n in item.names} & own:
                    plan.place(item, path)
    before = {id(it) for v in plan.types.values() for it in v} | {id(it) for v in plan.protos.values() for it, _ in v}
    if args.file:
        dest = os.path.relpath(os.path.abspath(args.file), ROOT)
        text = plan_file(plan, dest, args.bw2, twins, includes_of)
    for name in adopt_local_declarations(plan, ROOT, ROOT / dest if args.file else None):
        print(f"kept the C's own prototype of {name}")
    moved, tags = adopt_local_types(plan, ROOT, includes_of, by_include, ROOT / dest if args.file else None)
    for tag in tags:
        print(f"{tag}: a C file defines it itself, which the header takes")
    new_types = {path: [it for it in v if id(it) not in before] for path, v in plan.types.items()}
    new_protos = {path: [it for it, _ in v if id(it) not in before] for path, v in plan.protos.items()}
    for_file = {path for path, v in list(new_types.items()) + list(new_protos.items()) if v}

    for key, names in stats.items():
        print(f"{len(names):5} {key}")
        if args.verbose and key not in ("prototypes added", "declared here already"):
            print("      " + " ".join(names))
    changed = set(plan.types) | set(plan.protos)
    if args.file:
        print(f"{sum(map(len, new_protos.values())):5} prototypes and data, {sum(map(len, new_types.values()))} types "
              f"and constants for {dest}")
    game = [it for path, v in plan.types.items() if path != STRUCT_DECLS and not is_library(path) for it in v]
    print(f"{sum(map(len, plan.types.values())):5} types and constants added, {len(game)} of them the game's, "
          f"{sum(it.note is None for it in game)} of those confirmed by twinned code, "
          f"{sum(bool(it.note) and it.note not in (GAME_TYPE_NOTE, *BLACK_NOTES) for it in game)} in part and "
          f"{sum(it.note in BLACK_NOTES for it in game)} by Black's files (tools/black_constants.py)")
    print(f"{len(changed):5} headers changed, {len(changed - set(our_files))} of them new")
    for item, why in plan.skipped:
        print(f"skipped: {' '.join(item.code().split())[:100]}: {why}")
    if args.verbose:
        for name, paths in sorted(plan.unresolved.items()):
            print(f"unresolved: {name} in {', '.join(sorted(paths))}")
    for path, items in sorted(new_types.items()):
        for it in items:
            if it.struct and "{" in it.code() and not is_library(path):
                print(f"layout: {it.names[-1]} in {path}: " + (" ".join(it.note.replace("// ", "").split())
                                                                if it.note else "confirmed by twinned code"))
    if args.apply:
        for path in sorted(changed):
            if not (ROOT / path).exists() and (ROOT / "lib/msl/include" / include_name(path)).exists():
                # pokebw2's own stdio.h, stddef.h, stdarg.h, math.h: made under include/, they hid MSL's for every file
                # (darray.c, gpiPeer.c, dwc_rap.c's sprintf; prep of 14 files, 2026-10-15). MSL's have the C library.
                print(f"left out: {path}, MSL's lib/msl/include/{include_name(path)} is the C library's")
                continue
            write_header(path, plan, includes_of, args.bw2, *["the lines in use so far"] if path in for_file else [])
        for cfile, locals_ in moved.items():  # their definitions are in the headers now
            ctext = Path(cfile).read_text()
            for local in locals_:
                ctext = ctext.replace((local.comment + "\n" if local.comment else "") + local.text + "\n", "", 1)
            Path(cfile).write_text(ctext)
        for path, theirs in plan.private.items():
            (ROOT / path).write_text(f"// From pokebw2 (GPL-3.0, {URL}), {theirs.relative_to(args.bw2)}.\n"
                                     + theirs.read_text())
            print(f"{path}: pokebw2's private header")
        for path, names in plan.private_names.items():
            if path not in plan.private:  # copied before: not a name the library's other C files have either way
                # (vct_session.c defines vct.c's func_ov013_021b22b4 in its own words, an int for the BOOL)
                others = {w for c in glob.glob(f"{ROOT}/{path.split('/src/')[0]}/src/**/*.c", recursive=True)
                          if os.path.abspath(c) != os.path.abspath(ROOT / dest)
                          for w in re.findall(r"\w+", open(c, errors="replace").read())}
                names = {b2: n for b2, n in names.items() if b2 not in others and n not in others}
            old = (ROOT / path).read_text() if names and (ROOT / path).exists() else ""
            word = re.compile(r"\b(" + "|".join(map(re.escape, names)) + r")\b")
            new = word.sub(lambda m: names[m[0]], old) if old else ""
            if new != old:
                (ROOT / path).write_text(new)
                print(f"{path}: {', '.join(sorted(set(word.findall(old))))} under the C file's names")
        if args.file and text != (ROOT / dest).read_text():
            (ROOT / dest).write_text(text)
            print(f"{dest}: #include lines changed")
        if args.file:
            adapt_structs(plan, dest)
            overlay_ids(text)


def overlay_ids(text):
    """Declares in include/gfl/overlay.h the linker's OVERLAY_<n>_ID of each overlay Black has that the C text's
    OVERLAY_ID(n) names (tools/port_file.py gives them Black's numbers)."""
    header = ROOT / "include" / "gfl" / "overlay.h"
    have = header.read_text()
    overlays = ROOT / "config" / "b_us" / "arm9" / "overlays"
    new = [n for n in sorted({int(n) for n in re.findall(r"\bOVERLAY_ID\((\d+)\)", text)})
           if f"OVERLAY_{n}_ID[]" not in have and (overlays / f"ov{n:03d}").is_dir()]
    if not new:
        return
    last = list(re.finditer(r"^extern u32 OVERLAY_\d+_ID\[\];\n", have, re.M))
    at = last[-1].end() if last else have.rindex("\n#endif") + 1
    header.write_text(have[:at] + "".join(f"extern u32 OVERLAY_{n}_ID[];\n" for n in new) + have[at:])
    print("include/gfl/overlay.h: " + ", ".join(f"OVERLAY_{n}_ID" for n in new) + " declared")


if __name__ == "__main__":
    main()
