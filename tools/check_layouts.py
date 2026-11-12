#!/usr/bin/env python3
"""Check the layouts of the game's structs that came from pokebw2 as Black 2's against Black's code, and fix what it
pins down.

tools/port_decls.py notes a game struct "Black 2's, not checked against Black", or "Black 2's; twinned code confirms
the offsets of ..." when twins confirm some of its fields; tools/port_file.py the structs a C file defines itself. Those
of an incomplete file it ported before it noted them count as noted: they are Black 2's too, unless a worker rebuilt
them. Black's own C here shows more:
- a function that matches (its file complete, or compiler_probe.py's MATCH) was compiled with the struct, so the
  fields its code accesses are at Black's offsets, and a struct whose size it takes or that it indexes has Black's size;
- a parked function (docs/nonmatching.md) whose diff has ours `[r4, #0x10]` where the original has `[r4, #0x14]` (or a
  pool word, or an immediate), for a field of a struct it accesses at 0x10, votes that the field is 4 bytes further in
  Black; the same shift in several functions says a field before it is missing or smaller in Black 2's layout;
- a near twin (tools/near_twins.py: Black's function is pokebw2's but for some numbers) whose load or store of a field
  has another offset votes the same way, written in C here or not (not one of a complete file's).
clang's AST of each C file says which fields of which structs each function accesses (not those of a static variable,
which a relocation may hold), its record layouts their offsets. Both, and compiler_probe.py's results, are cached in
build/check_layouts.json by the text of the file and of the headers it includes.

--fix tries, for a shift the votes show from some field on, each edit of the struct's own fields that makes it: a field
removed, an array shrunk or grown (its length's #define if it has one), a field narrowed, padding inserted, between the
last field whose offset Black's code confirms and the first that shifts, never moving a confirmed one. Each is judged on
every file that uses the struct or one that holds it: no function that matches may stop matching, and the parked
functions' diffs must lose lines. The best is kept only if it is pinned down: no other edit does as well, and the fields
where the shift may start allow no other (fields that add up to it, an array, a field to narrow, a place for padding).
Otherwise the run lists those, for a look at Black's code: a loop's bound or a static table's size gives an array's
length (ZknListWork's windows: 3, not Black 2's 6), an access the offset of a field no C here reaches yet. Edit the
header by hand then: --judge measures the uncommitted changes against HEAD on the files that use the struct, and --apply
drops the rows of the functions that match now. Then ninja must pass.
--notes rewrites the notes above the noted structs that matching code (twinned, or Black's C here) confirms more of,
in port_decls.py's style; a struct it confirms whole loses its note.

--protos checks the prototypes port_decls.py notes as untwinned against Black's functions: one that reads r2 before
it writes it takes three arguments at least (read in the order of its code, up to its first call). The port gave some
of Black 2's functions to the wrong address, and those prototypes take too few (trainer_game_info_savedata.c's
accessors). Black's callers decide the types.

--offsets lists the offsets that differ the same way in several parked functions of a file, attributed or not
(zukan_detail_touchbar.c's work: ours [r4, #0x2c], the original's [r4, #0x30], in 13 functions). Then the immediates
that three or more differ in alike, mostly Black 2's constants: gimmick_roller_coaster.c's gimmick state, 48 against
Black's 2 in 9 functions. --constants names them: the
#define a parked function's C uses whose value is ours where an immediate differs, when it is the only one of that
value the function names and the diffs agree on Black's value. Constants of include/constants/ are
tools/black_constants.py's: Black's sound and zone names can be other numbers than Black 2's.

    .venv/bin/python tools/check_layouts.py                 # each noted struct: parked and matching functions using it
    .venv/bin/python tools/check_layouts.py --shifts X      # struct X's fields: shifts the parked functions' diffs show
    .venv/bin/python tools/check_layouts.py --offsets       # offsets that differ alike in a file's parked functions
    .venv/bin/python tools/check_layouts.py --protos        # untwinned prototypes with fewer arguments than Black's
    .venv/bin/python tools/check_layouts.py --constants     # #defines whose values the parked functions' diffs give
    .venv/bin/python tools/check_layouts.py --fix X Y       # fix X's and Y's layouts where Black's code pins them down
    .venv/bin/python tools/check_layouts.py --judge X --apply   # judge the headers' changes, drop matched rows
    .venv/bin/python tools/check_layouts.py --notes         # rewrite the notes from the evidence
"""
import argparse
import hashlib
import json
import re
import subprocess
import sys
import textwrap
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT))
try:  # compiler_probe.py's disassembler is in the repo's venv
    import capstone  # noqa: F401
except ImportError:
    _venv = ROOT / ".venv" / "bin" / "python"
    if _venv.exists() and Path(sys.executable).resolve() != _venv.resolve():
        import os
        os.execv(str(_venv), [str(_venv), *sys.argv])
import near_twins  # noqa: E402
import port_decls as pd  # noqa: E402
from configure import INCLUDE_DIRS  # noqa: E402

CACHE = ROOT / "build" / "check_layouts.json"
NONMATCHING = ROOT / "docs" / "nonmatching.md"
ROW = re.compile(r"^\| `(\w+)`[^|]*\| `0x[0-9a-f]+` \| `([^`]+)` \| (.*) \| (.*) \|$", re.M)
PARTIAL = ("// Black 2's; twinned code confirms the ", "// Black 2's; matching code confirms the ")
CONFIRMS = re.compile(r"// Black 2's; (?:twinned|matching) code confirms the offsets of (.*?)(, and its size)? "
                      r"in Black", re.S)
JOBS = 2  # (a number the caller set, else BW_JOBS or the cores free in the worker slice: jobs_now())


def jobs_now():
    import jobs as bwjobs
    return bwjobs.jobs() if JOBS == 2 else JOBS
IMM = re.compile(r"#(-?(?:0x[0-9a-f]+|\d+))")
SCALARS = {1: "u8", 2: "u16", 4: "u32"}
NEAREST = 6  # positions tried before the first shifted field


# MWCC lays a struct's 64-bit integers and doubles out 4-aligned (the old ARM ABI's way: DWCMatchState's u64 at 0x3c);
# clang's ARM EABI aligns them to 8 and moves every field after one. -fpack-struct=4 caps a field's alignment at 4 and
# leaves the rest as the EABI has it, bit-fields too (-mabi=apcs-gnu aligns the 64-bit types to 4 but no bit-field to
# its type: `struct { u32 a : 4; }` is 1 byte there, 4 in MWCC). Every clang run that lays out a struct takes it.
MWCC_LAYOUT = "-fpack-struct=4"


def clang(c_file, *extra, text=None):
    """clang's output for a C file of this repo (or text in its place)."""
    path = ROOT / c_file
    if text is not None:
        path = path.with_name(f".layout_{path.name}")
        path.write_text(text)
    incs = [f"-I{ROOT / d}" for d in INCLUDE_DIRS] + [f"-I{(ROOT / c_file).parent}",
                                                      f"-I{ROOT / 'build/include/generated'}"]
    try:
        return subprocess.run(["nice", "-n", "19", "clang", "--target=armv5te-none-eabi", MWCC_LAYOUT, "-ffreestanding",
                               "-std=c99", "-fsyntax-only", "-ferror-limit=0", "-w", *incs, *extra, str(path)],
                              capture_output=True, text=True, cwd=ROOT).stdout
    finally:
        if text is not None:
            path.unlink(missing_ok=True)


def headers_of(c_file):
    """The headers of this repo a C file includes, directly or not."""
    dirs = [ROOT / d for d in INCLUDE_DIRS] + [ROOT / "build/include/generated"]
    seen, todo = set(), [ROOT / c_file]
    while todo:
        p = todo.pop()
        for name in re.findall(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', p.read_text(errors="replace"), re.M):
            for d in [p.parent, *dirs]:
                h = d / name
                if h.is_file():
                    if h not in seen:
                        seen.add(h)
                        todo.append(h)
                    break
    return sorted(seen)


def file_key(c_file):
    h = hashlib.sha1((ROOT / c_file).read_bytes())
    for p in headers_of(c_file):
        h.update(p.read_bytes())
    return h.hexdigest()


def noted(files):
    """{record: (header or C file, Item)} of the game's structs and unions noted as Black 2's, whole or in part, and
    those an incomplete C file port_file.py ported defines without a note. files: c_files()."""
    out = {}
    paths = [p for p in pd.header_files(ROOT) if not pd.is_library(p) and p != pd.STRUCT_DECLS]
    for path in paths + sorted(f for f in files if f.startswith("src/")):
        text = (ROOT / path).read_text(errors="replace")
        ported = path.endswith(".c") and not files[path] and text.startswith("// From pokebw2")
        if "Black 2's" not in text and not ported:
            continue
        for item in pd.parse(ROOT / path):
            if item.struct and not item.fwd and "{" in item.code() and (
                    pd.GAME_TYPE_NOTE in item.comment or any(x in item.comment for x in PARTIAL) or
                    ported and "Black 2's" not in item.comment):
                out[item.tag or item.names[-1]] = (path, item)
    return out


def twin_confirmed(item):
    """(fields, size) its note says twinned code confirms."""
    m = CONFIRMS.search(re.sub(r"\n//\s*", " ", item.comment))
    if not m:
        return set(), False
    return set(re.split(r",? and |, ", m[1])), bool(m[2])


def uses(c_file):
    """{function: [[fields as "record.field"], [records whose size it takes or indexes]]} of the functions a C file
    defines, from clang's AST."""
    out = clang(c_file, "-Xclang", "-ast-dump=json")
    if not out.strip():
        return {}
    ast = json.loads(out)
    owner, _, _ = pd.ast_records(ast)
    mine = {d["name"] for d in ast["inner"] if d.get("kind") == "FunctionDecl" and d.get("name")
            and any(c.get("kind") == "CompoundStmt" for c in d.get("inner", []))
            and "includedFrom" not in d.get("loc", {}).get("expansionLoc", d.get("loc", {}))}
    found = pd.ast_functions(ast, owner, mine)
    result = {}
    for d in ast["inner"]:
        if d.get("kind") != "FunctionDecl" or d.get("name") not in found or d["name"] in result:
            continue
        fields, _, sizes = found[d["name"]]
        # an array of structs indexed: its element's size
        sizes = set(sizes) | {pd.record_of(n.get("type", {})) for n in pd.nodes(d)
                              if n.get("kind") == "ArraySubscriptExpr"}
        result[d["name"]] = [sorted(f"{r}.{f}" for r, f in fields), sorted(sizes - {None})]
    return result


def probe(c_file):
    """compiler_probe.py on a file: ({function: status}, {function: diff}), or None if it doesn't compile."""
    import compiler_probe
    results, lines, failed = compiler_probe.probe_source(Path(c_file), cwd=ROOT)
    if failed:
        return None
    status = {n: next(iter(row.values())) for n, row in results.items()}
    diffs, cur = {}, None
    for line in lines:
        m = re.match(r"--- (\w+) \(", line)
        if m:
            cur = m[1]
            diffs[cur] = ""
        elif cur:
            diffs[cur] += line + "\n"
    return status, diffs


def evidence(files, probed):
    """{file: {"uses": uses(), "status": {function: status}, "diffs": {function: diff}}}: probed only for the files in
    probed (in the others everything matches). Cached by file_key()."""
    cache = json.loads(CACHE.read_text()) if CACHE.exists() else {}
    keys = {f: file_key(f) for f in files}
    todo = [f for f in files if cache.get(f, {}).get("key") != keys[f] or f in probed and "status" not in cache[f]]
    if todo:
        print(f"clang and compiler_probe.py: {len(todo)} files", file=sys.stderr)

        def one(f):
            entry = {"key": keys[f], "uses": uses(f)}
            if f in probed:
                entry["status"], entry["diffs"] = probe(f) or ({}, {})
            return entry

        with ThreadPoolExecutor(jobs_now()) as pool:
            for f, entry in zip(todo, pool.map(one, todo)):
                cache[f] = entry
        CACHE.parent.mkdir(exist_ok=True)
        CACHE.write_text(json.dumps(cache))
    return {f: cache[f] for f in files}


def layouts(c_file, records):
    """{record: (size, [(offset, size, depth, path)])} of the records, laid out by clang as the C file sees them."""
    force = "".join(f"typedef char __lay{i}[sizeof({r})];\n" for i, r in enumerate(sorted(records)))
    out = clang(c_file, "-Xclang", "-fdump-record-layouts", text=(ROOT / c_file).read_text() + "\n" + force)
    result = {}
    for block in out.split("*** Dumping AST Record Layout")[1:]:
        lines = [x for x in block.strip("\n").splitlines() if x.strip()]
        rec = lines[0].split("|", 1)[-1].strip() if lines else ""
        if rec not in records or rec in result:
            continue
        rows, size, names = [], 0, {}
        for x in lines[1:]:
            if "sizeof=" in x:
                size = int(re.search(r"sizeof=(\d+)", x)[1])
                break
            off, _, field = x.partition("|")
            byte = off.strip().split(":")[0]
            if not byte.isdigit():
                continue
            depth = (len(field) - len(field.lstrip())) // 2
            names[depth] = field.split()[-1]
            rows.append((int(byte), depth, ".".join(names[d] for d in range(1, depth + 1))))
        sized = []
        for i, (o, d, path) in enumerate(rows):
            nxt = next((r[0] for r in rows[i + 1:] if r[1] <= d), size)
            sized.append((o, nxt - o, d, path))
        result[rec] = (size, sized)
    return result


def pairs(diff):
    """[(kind, ours, original)] of the numbers that differ between the two sides of a diff's lines that are otherwise
    the same: an offset in brackets (mem), a pool word (word) or another immediate (imm); not the stack's."""
    out = []
    for line in diff.splitlines():
        if line.startswith("@@") or " | " not in line:
            continue
        a, b = (s.strip() for s in line.split(" | ", 1))
        if not a or not b:
            continue
        if a.startswith(".word") and b.startswith(".word"):
            try:
                x, y = int(a.split()[1], 0), int(b.split()[1], 0)
            except ValueError:
                continue
            if x != y and x < 0x10000 and y < 0x10000:
                out.append(("word", x, y))
            continue
        if IMM.sub("#N", a) != IMM.sub("#N", b) or re.search(r"\bsp\b", a):  # the stack's offsets: locals
            continue
        for m, n in zip(IMM.finditer(a), IMM.finditer(b)):
            x, y = int(m[1], 0), int(n[1], 0)
            if x != y:
                out.append(("mem" if a.rfind("[", 0, m.start()) > a.rfind("]", 0, m.start()) else "imm", x, y))
    return out


def lines_of(diff):
    return sum(1 for line in diff.splitlines() if not line.startswith("@@"))


def parked(written=True):
    """{file: {function: difference}} of docs/nonmatching.md's nonmatching C, of the functions written in C (or all)."""
    out = defaultdict(dict)
    for m in ROW.finditer(NONMATCHING.read_text()):
        if not written or "not written" not in m[3]:
            out[m[2]][m[1]] = m[3]
    return out


def c_files():
    """{C file: complete} of the files the delinks.txt list."""
    out = {}
    for path in (ROOT / "config/b_us/arm9").rglob("delinks.txt"):
        src = None
        for line in open(path):
            if line.strip() and not line[0].isspace():
                src = line.strip().rstrip(":")
                out[src] = False
            elif src and line.strip() == "complete":
                out[src] = True
    return {f: c for f, c in out.items() if f.endswith(".c") and (ROOT / f).exists()}


class Evidence:
    """What the C here shows of the noted structs. A record is keyed as clang names it, "struct X" (a tagged struct, as
    `struct X { ... };` and `typedef struct X { ... } X;` both define it) or the typedef of an untagged one; record()
    takes the plain name too."""

    def __init__(self):
        self.files, self.park = c_files(), parked()
        self.structs = noted(self.files)
        self.ev = evidence(sorted(self.files), {f for f, c in self.files.items() if not c})
        self.info = defaultdict(lambda: {"parked": {}, "matching": {}, "fields": set(), "sized": False,
                                         "files": set(), "near": []})
        # a C file's own struct: only that file's code is evidence of it
        local = {r: p for r, (p, _) in self.structs.items() if p.endswith(".c")}
        for f, e in self.ev.items():
            for fn, (fields, sizes) in e["uses"].items():
                match = self.files[f] or e.get("status", {}).get(fn) == "MATCH"
                for r in {x.split(".", 1)[0] for x in fields} | set(sizes):
                    if local.get(r, f) != f:
                        continue
                    i = self.info[r]
                    i["files"].add(f)
                    if fn in self.park.get(f, {}) and not match:
                        i["parked"][fn] = f
                    elif match:
                        i["matching"][fn] = f
                        i["fields"] |= {x.split(".", 1)[1] for x in fields if x.split(".", 1)[0] == r}
                        i["sized"] |= r in sizes
        for r, (_, item) in self.structs.items():
            fields, sized = twin_confirmed(item)
            self.info[r]["fields"] |= fields
            self.info[r]["sized"] |= sized
        for t in near_twins.near_twins():
            if self.files.get(t["file"]):  # a complete file's C is Black's: it is its own evidence
                continue
            for record, path, o, a, b, _ in t.get("fields", []):
                if local.get(record, t["file"]) == t["file"]:
                    self.info[record]["near"].append((t["black"], path, o, a, b))

    def record(self, name):
        """The key of a noted struct from the name a user gives: UnionSystem, struct UnionSystem or union UnionSystem."""
        if name in self.structs:
            return name
        short = name.split()[-1]
        hits = [r for r in self.structs if r.split()[-1] == short]
        if len(hits) != 1:
            like = [r for r in self.structs if short.lower() in r.lower()]
            sys.exit(f"{name}: " + (f"ambiguous: {', '.join(hits)}" if hits else
                                    f"not a noted struct" + (f"; like it: {', '.join(like[:8])}" if like else "")))
        return hits[0]

    def holders(self, record):
        """The records of the headers that hold this one by value, at any depth."""
        if not hasattr(self, "defs"):
            self.defs = [(it.tag or it.names[-1], it.code()) for path in pd.header_files(ROOT) + sorted(self.files)
                         for it in pd.parse(ROOT / path) if it.struct and not it.fwd and "{" in it.code()]
        out, todo = set(), [record]
        while todo:
            short = todo.pop().split()[-1]
            member = re.compile(rf"[;{{}}]\s*(?:const\s+)?(?:struct\s+|union\s+)?{short}\s+\w+\s*(?:\[[^\]]*\]\s*)*;")
            for key, code in self.defs:
                if key != record and key not in out and member.search(code[code.index("{"):]):
                    out.add(key)
                    todo.append(key)
        return out

    def users(self, record):
        """The C files that use the record or one that holds it."""
        recs = {record} | self.holders(record)
        return sorted({f for r in recs for f in self.info.get(r, {}).get("files", ())})


def shift_votes(record, info, ev, layout):
    """{offset: {shift: functions}} of the record's fields the parked functions' diffs and the near twins show shifted,
    and the size's."""
    size, rows = layout
    votes = defaultdict(lambda: defaultdict(set))
    for fn, f in info["parked"].items():
        fields, sizes = ev[f]["uses"].get(fn, ([], []))
        tops = {x.split(".", 1)[1] for x in fields if x.split(".", 1)[0] == record}
        for kind, a, b in pairs(ev[f].get("diffs", {}).get(fn, "")):
            hit = [r for r in rows if r[3].split(".")[0] in tops and (r[0] == a if kind == "imm" else
                                                                       r[0] <= a < r[0] + r[1])]
            if hit:
                votes[hit[-1][0]][b - a].add(fn)
            elif record in sizes and a == size and kind != "mem":
                votes[size][b - a].add(fn)
    # a near twin's field at pokebw2's offset o, accessed at a there and at b in Black: where is it here?
    for fn, path, o, a, b in info.get("near", ()):
        start = next((r[0] for r in rows if r[3] == path), None)
        if start is None or fn in info["parked"] or fn in info["matching"]:
            continue
        ours = start + a - o
        hit = [r for r in rows if r[3].split(".")[0] == path.split(".")[0] and r[0] <= ours < r[0] + r[1]]
        if hit and b != ours:
            votes[hit[-1][0]][b - ours].add(fn)
    return votes


def print_shifts(record, info, ev, layout, brief=False):
    """Prints the record's fields, those that matching code confirms (ok), and the shifts the votes show. brief: only
    a record with a shift at a field not confirmed, and only those fields and the confirmed ones next to them."""
    votes = shift_votes(record, info, ev, layout)
    size, rows = layout
    rows = [r for r in rows if r[2] <= 2] + [(size, 0, 1, "(size)")]
    sure = [r[3] in info["fields"] or r[3] == "(size)" and info["sized"] for r in rows]
    voted = [r[2] == 1 and any(s for s in votes.get(r[0], {})) and not ok for r, ok in zip(rows, sure)]
    if brief and not any(voted):
        return
    print(f"{record}: size {size:#x}{' (confirmed)' if info['sized'] else ''}; {len(info['parked'])} parked, "
          f"{len(info['matching'])} matching functions use it; {len({x[0] for x in info['near']})} near twins")
    for i, ((o, n, d, path), ok) in enumerate(zip(rows, sure)):
        near = any(voted[j] for j in range(max(0, i - 1), min(len(rows), i + 2)))
        if brief and not (voted[i] or ok and near):
            continue
        v = votes.get(o, {}) if d == 1 else {}
        shown = ", ".join(f"{s:+#x} x{len(fs)}" for s, fs in sorted(v.items(), key=lambda x: -len(x[1])))
        print(f"  {o:#06x} {n:#6x} {'ok' if ok else '  '} {'  ' * (d - 1)}{path:40s} {shown}")


# The struct's text: its fields as statements

def statements(text):
    """[(start, end, name, dims)] of the top-level declarations in a struct's definition text: their span (with the
    comment lines above), the field's name and array dimensions ("" if none); name None for one that declares several
    fields or a nested record."""
    body_start = text.index("{") + 1
    out, depth, start, i = [], 0, body_start, body_start
    comment_start = None
    while i < len(text):
        c = text[i]
        if text.startswith("//", i):
            j = text.find("\n", i)
            i = len(text) if j < 0 else j
            continue
        if text.startswith("/*", i):
            i = text.index("*/", i) + 2
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            if depth == 0:
                break
            depth -= 1
        elif c == ";" and depth == 0:
            stmt = text[start:i + 1]
            code = re.sub(r"//[^\n]*|/\*.*?\*/", "", stmt, flags=re.S).strip()
            m = re.match(r"(?s)(?:const\s+)?[\w\s\*]+?\b(\w+)\s*((?:\[[^\]]*\]\s*)*)(:\s*\w+\s*)?;$", code)
            name = m[1] if m and "," not in code and "{" not in code and not m[3] else None
            # the span starts at the line's beginning, so the comment lines above go with it
            line_start = text.rfind("\n", 0, start + len(stmt) - len(stmt.lstrip())) + 1
            out.append((line_start, text.find("\n", i) + 1 or len(text), name, (m[2] or "").strip() if m else ""))
            start = i + 1
        i += 1
    return out


def unk_name(text, offset):
    if re.search(r"\bunk_[0-9A-Fa-f]+\b", text):
        return f"unk_{offset:X}"
    return f"unk{offset:02X}"


def define_of(name, first):
    """(file, its text, the #define's span, value) of a numeric #define in the file first or the headers, or None."""
    for path in [first] + pd.header_files(ROOT):
        text = (ROOT / path).read_text(errors="replace")
        m = re.search(rf"^#define {name} (0x[0-9a-fA-F]+|\d+)\b", text, re.M)
        if m:
            return path, text, m.span(1), int(m[1], 0)
    return None


def candidates(record, info, layout, votes, header, usage):
    """[(description, {header: new text}, position, preference, gap)] of the edits that would make the shifts the votes
    show, each for the first field it shows; gap: the fields where the shift may start, between the last confirmed
    one and the first shifted one."""
    size, rows = layout
    head = (ROOT / header).read_text()
    text = find_item(header, record).text
    tops = [(o, n, path) for o, n, d, path in rows if d == 1]
    sure = {path for _, _, path in tops if path in info["fields"]}
    stmts = {s[2]: s for s in statements(text) if s[2]}
    out = []

    def edit(new):
        return {header: head.replace(text, new)}

    # a vote inside a confirmed field is another record's, or a number that only looks like an offset
    shifted = sorted((o, s, len(fs)) for o, v in votes.items() for s, fs in v.items() if s and not any(
        t[0] <= o < t[0] + t[1] for t in tops if t[2] in sure))
    tried = set()
    for o_min, shift, _ in shifted:
        if shift in tried or abs(shift) > 0x400:
            continue
        tried.add(shift)
        # the fields that may change: after the last confirmed field before o_min (or with another vote there)
        lower = max([o + n for o, n, path in tops if path in sure and o < o_min] + [0])
        for o, v in votes.items():
            if o < o_min and shift not in v and any(v):
                lower = max(lower, o + 1)
        before = [t for t in tops if lower <= t[0] + t[1] <= o_min or t[0] <= lower < t[0] + t[1]]
        before = [t for t in before if t[0] < o_min][-NEAREST:]
        at = [t for t in tops if t[0] >= o_min][:1]
        gap = ways(shift, [t for t in tops if t[0] < o_min and t[0] + t[1] > lower], stmts)
        for o, n, path in before + at:
            st = stmts.get(path)
            if not st:
                continue
            a, b, name, dims = st
            line = text[a:b]
            decl = re.sub(r"//[^\n]*", "", line).strip()
            if shift > 0 and o >= lower:  # padding before the field: Black has bytes there that Black 2 lacks
                indent = re.match(r"[ \t]*", line.splitlines()[-1])[0] or "    "
                t = SCALARS.get(shift) if o % shift == 0 else None
                pad = f"{indent}{t} {unk_name(text, o)};\n" if t else f"{indent}u8 {unk_name(text, o)}[{shift}];\n"
                out.append((f"insert {shift:#x} bytes before {path}", edit(text[:a] + pad + text[a:]), o, 0, gap))
            if o >= o_min:
                continue
            dim = re.fullmatch(r"\[(\w+)\]", dims)
            define = define_of(dim[1], header) if dim and not dim[1].isdigit() else None
            count = int(dim[1]) if dim and dim[1].isdigit() else define[3] if define else 0
            elem = n // count if count else 0
            if elem and shift % elem == 0 and 0 < count + shift // elem:  # an array: elements go, or more come
                more = count + shift // elem
                if define:  # its length's constant is Black 2's
                    dh, dtext, (s, e), _ = define
                    changed = {dh: dtext[:s] + str(more) + dtext[e:]}
                    if dh == header:
                        changed = {header: changed[header]}
                    out.append((f"{dim[1]} {count} -> {more}", changed, o, 2, gap))
                else:
                    new = line.replace(f"[{count}]", f"[{more}]", 1)
                    out.append((f"{name}[{count}] -> [{more}]", edit(text[:a] + new + text[b:]), o, 2, gap))
            if shift < 0 and path not in sure and o >= lower:
                if n == -shift:
                    out.append((f"remove {path}", edit(text[:a] + text[b:]), o, 1 + (path not in usage), gap))
                m = re.match(r"(\s*)(?:const\s+)?(u32|s32|u16|s16|int|BOOL|fx32)\s+(\w+)\s*;", decl)
                if m and n + shift in (1, 2) and not dims:
                    signed = m[2] in ("s32", "s16", "int", "fx32")
                    t = {1: "s8" if signed else "u8", 2: "s16" if signed else "u16"}[n + shift]
                    new = re.sub(rf"\b{m[2]}\b", t, line, count=1)
                    out.append((f"{path}: {m[2]} -> {t}", edit(text[:a] + new + text[b:]), o, 1, gap))
    return out


def ways(shift, gap, stmts):
    """The edits of the fields in the gap that could make the shift: a run of fields that adds up to it removed, an
    array that loses or gains the bytes, a field narrowed; padding before any field, or before the shifted one."""
    out = []
    for i, (o, n, path) in enumerate(gap):
        dims = (stmts.get(path) or (0, 0, None, ""))[3]
        if dims:
            out.append(f"{path}[]")
        elif shift < 0 and n + shift in (1, 2):
            out.append(f"{path} narrowed")
        if shift > 0:
            out.append(f"before {path}")
        total = 0
        for j in range(i, len(gap)):
            total += gap[j][1]
            if shift < 0 and total == -shift:
                out.append(" and ".join(g[2] for g in gap[i:j + 1]))
    return tuple(out) + (("before the shifted field",) if shift > 0 else ())


def find_item(header, record):
    return next(it for it in pd.parse(ROOT / header) if it.struct and not it.fwd and "{" in it.code()
                and (it.tag or it.names[-1]) == record)


def judge(files, edits):
    """{file: (status, diffs)} of the files with the headers' texts replaced (None for one that doesn't compile)."""
    old = {h: (ROOT / h).read_text() for h in edits}
    for h, text in edits.items():
        (ROOT / h).write_text(text)
    try:
        with ThreadPoolExecutor(jobs_now()) as pool:
            return dict(zip(files, pool.map(probe, files)))
    finally:
        for h, text in old.items():
            (ROOT / h).write_text(text)


def score(results, park):
    """(matching functions, lines of the parked functions' diffs) or None if a file doesn't compile."""
    if any(r is None for r in results.values()):
        return None
    match, lines = set(), 0
    for f, (status, diffs) in results.items():
        match |= {(f, fn) for fn, s in status.items() if s == "MATCH"}
        lines += sum(lines_of(d) for fn, d in diffs.items() if fn in park.get(f, {}))
    return match, lines


def fix(record, e, apply=True):
    """Finds the edits of a record's layout that Black's code pins down; with apply, keeps them in the headers: one
    that helps when no other does as well and the shift can start at one field only. Returns ([descriptions of the
    edits kept], judge()'s results with them)."""
    header, _ = e.structs[record]
    info = e.info[record]
    files = e.users(record)
    saved = {}
    usage = {x.split(".", 1)[1] for f in files for fn, (fields, _) in e.ev[f]["uses"].items() for x in fields
             if x.split(".", 1)[0] == record}
    now = judge(files, {})
    best = score(now, e.park)
    if best is None:
        print(f"{record}: a file that uses it doesn't compile as it is")
        return [], {}
    kept = []
    print(f"{record}: {len(files)} files use it; {len(best[0])} functions match, {best[1]} lines of parked diffs")
    for _ in range(8):
        ev = {f: {"uses": e.ev[f]["uses"], "diffs": r[1]} for f, r in now.items()}
        info_now = dict(info, parked={fn: f for f, r in now.items() for fn in r[1] if fn in e.park.get(f, {})})
        lay = None
        for f in sorted(set(info_now["parked"].values())) or files:
            lay = layouts(f, {record}).get(record)
            if lay:
                break
        if not lay:
            break
        votes = shift_votes(record, info_now, ev, lay)
        found = []
        for desc, edits, pos, pref, gap in candidates(record, info, lay, votes, header, usage):
            res = judge(files, edits)
            sc = score(res, e.park)
            lost = sc is None or not best[0] <= sc[0]
            print(f"  {desc}: " + ("doesn't compile" if sc is None else
                                   f"{len(sc[0]) - len(best[0]):+} matching, {sc[1] - best[1]:+} lines"
                                   + (" (loses a match)" if lost else "")))
            if not lost and (len(sc[0]) > len(best[0]) or sc[1] < best[1]):
                found.append(((len(sc[0]), -sc[1], pos, pref), desc, edits, res, sc, gap))
        if not found:
            break
        found.sort(key=lambda x: x[0], reverse=True)
        top = found[0]
        ties = [d for k, d, *_ in found[1:] if k[:2] == top[0][:2]]
        if ties or len(top[5]) > 1:
            could = ", ".join(top[5][:6]) + (f" and {len(top[5]) - 6} more" if top[5][6:] else "")
            print(f"  not pinned down: {top[1]}" + (f", as good: {'; '.join(ties)}" if ties else "") +
                  f"; it could be {could}: Black's code decides (a loop's bound, a table's size, an access to one "
                  "of them)")
            break
        print(f"  kept: {top[1]}")
        kept.append(top[1])
        for h, text in top[2].items():
            saved.setdefault(h, (ROOT / h).read_text())
            (ROOT / h).write_text(text)
        now, best = top[3], top[4]
    if not apply:
        for h, text in saved.items():
            (ROOT / h).write_text(text)
    return kept, now


def matched_rows(e, results):
    """Removes from docs/nonmatching.md the rows of the functions that match in the results (written since, too);
    returns them."""
    rows = parked(written=False)
    gone = [(fn, f) for f, (status, _) in results.items() for fn in rows.get(f, {}) if status.get(fn) == "MATCH"]
    if gone:
        names = {fn for fn, _ in gone}
        text = NONMATCHING.read_text()
        out = [line for line in text.splitlines(keepends=True)
               if not (ROW.match(line) and ROW.match(line)[1] in names)]
        NONMATCHING.write_text("".join(out))
    return gone


def offsets(e):
    """Prints, for each file, the offsets that differ the same way in two of its parked functions or more, whatever
    struct they belong to (the file's own structs, and those of headers without a note, show only here), and the other
    immediates that differ alike in three or more: a constant of Black 2's (an archive's ID, a gimmick's)."""
    rows = []
    for f, entry in e.ev.items():
        seen = defaultdict(set)
        for fn, diff in entry.get("diffs", {}).items():
            if fn in e.park.get(f, {}):
                for kind, a, b in pairs(diff):
                    seen[(kind == "imm", a, b)].add(fn)
        rows += [(len(fns), f, imm, a, b, sorted(fns)) for (imm, a, b), fns in seen.items() if len(fns) >= 2 + imm]
    for n, f, imm, a, b, fns in sorted(rows, key=lambda r: (r[2], -r[0], r[1])):
        print(f"{n:3} {f}: ours {'#' if imm else ''}{a:#x}, Black's {b:#x} ({b - a:+#x}): {', '.join(fns[:4])}"
              f"{' ...' if fns[4:] else ''}")


DEFINE = re.compile(r"^#define ([A-Z][A-Z0-9_]+) (0x[0-9a-fA-F]+|\d+)\s*(?://.*)?$", re.M)


def constants(e):
    """Prints the #defines that the parked functions' diffs give Black's values: a constant the function's C names
    whose value is ours in an immediate or pool word that differs, the only one of that value it names, and with one
    value of Black's for it in all the functions. Their file, name, value, Black's and the functions."""
    found = defaultdict(set)
    for f, entry in e.ev.items():
        if f not in e.park:
            continue
        text = (ROOT / f).read_text(errors="replace")
        heads = [ROOT / f] + headers_of(f)
        defs = [(h, m[1], m[2]) for h in heads for m in DEFINE.finditer(h.read_text(errors="replace"))]
        for fn, diff in entry.get("diffs", {}).items():
            if fn not in e.park[f]:
                continue
            m = re.search(rf"^[^\n;]*\b{fn}\([^;{{]*\)\s*\{{", text, re.M)
            end = text.find("\n}", m.end()) if m else -1
            names = set(re.findall(r"\b[A-Z][A-Z0-9_]{2,}\b", text[m.start():end])) if m else set()
            for a, b in {(a, b) for k, a, b in pairs(diff) if k != "mem" and a != b and max(a, b) < 0x10000}:
                hits = [(h, n, v) for h, n, v in defs if n in names and int(v, 0) == a]
                if len(hits) == 1:
                    h, n, v = hits[0]
                    found[(str(h.relative_to(ROOT)), n, v)].add((hex(b) if v.startswith("0x") else str(b), fn))
    for (path, name, value), seen in sorted(found.items()):
        if len({b for b, _ in seen}) == 1:
            print(f"{path}: {name} {value} -> {next(iter(seen))[0]}: {', '.join(sorted(fn for _, fn in seen))}")


def judge_changes(e, records):
    """Judges the uncommitted changes of headers and C files against HEAD on the files that use the records (or, with
    none, on those that include a changed header): prints the functions that match now or no longer, and the lines of
    the parked functions' diffs. Returns (the results now, whether no match was lost)."""
    changed = subprocess.run(["git", "diff", "--name-only", "HEAD", "--", "include", "lib", "src"], cwd=ROOT,
                             capture_output=True, text=True).stdout.split()
    if records:
        files = sorted({f for r in records for f in e.users(r)} | {f for f in changed if f in e.files})
    else:
        heads = {ROOT / h for h in changed if h.endswith(".h")}
        files = sorted(f for f in e.files if f in changed or heads & set(headers_of(f)))
    head = {h: subprocess.run(["git", "show", f"HEAD:{h}"], cwd=ROOT, capture_output=True, text=True).stdout
            for h in changed}
    before, now = judge(files, head), judge(files, {})
    ok = True
    for f in files:
        if now[f] is None or before[f] is None:
            print(f"{f}: {'does not compile now' if now[f] is None else 'did not compile before'}")
            ok = ok and now[f] is not None
            continue
        gained = sorted(fn for fn, s in now[f][0].items() if s == "MATCH" and before[f][0].get(fn) != "MATCH")
        lost = sorted(fn for fn, s in before[f][0].items() if s == "MATCH" and now[f][0].get(fn) != "MATCH")
        a, b = (sum(lines_of(d) for fn, d in r[1].items() if fn in e.park.get(f, {})) for r in (before[f], now[f]))
        if gained or lost or a != b:
            print(f"{f}: {len(gained)} match now{': ' + ', '.join(gained) if gained else ''}"
                  f"{'; LOST: ' + ', '.join(lost) if lost else ''}; parked diffs {a} -> {b} lines")
        ok = ok and not lost
    return {f: r for f, r in now.items() if r is not None}, ok


def field_names(text):
    """The names of a struct's fields as clang's AST names them: those of an untagged member without a name too."""
    names = []
    for a, b, name, _ in statements(text):
        code = re.sub(r"//[^\n]*|/\*.*?\*/", "", text[a:b], flags=re.S).strip()
        if name:
            names.append(name)
        elif "{" in code:
            m = re.search(r"\}\s*(\w+)\s*(?:\[[^\]]*\]\s*)*;$", code)
            names += [m[1]] if m else field_names("{" + code[code.index("{") + 1:code.rindex("}")] + "}")
        elif re.search(r"\(\s*\*\s*(\w+)\s*\)", code):
            names.append(re.search(r"\(\s*\*\s*(\w+)\s*\)", code)[1])
        else:
            names += re.findall(r"(\w+)\s*(?:\[[^\]]*\]\s*)*(?::\s*\w+\s*)?[,;]", code)
    return names


def note_text(record, e, item):
    """The note a noted struct gets from the evidence: None when all its fields are confirmed."""
    info = e.info[record]
    every = field_names(item.text)
    sure = [n for n in every if n in info["fields"]]
    if every and len(sure) == len(every):
        return None
    if not sure:
        return pd.GAME_TYPE_NOTE
    text = "Black 2's; matching code confirms the offsets of " + (", ".join(sure[:-1]) + " and " if sure[1:] else "") \
        + sure[-1] + (", and its size" if info["sized"] else "") + " in Black"
    return "\n".join("// " + line for line in textwrap.wrap(text, 117))


def notes(e):
    """Rewrites the notes above the noted structs; returns how many changed and how many went."""
    changed = gone = 0
    for record, (header, item) in sorted(e.structs.items()):
        lines = item.comment.split("\n")
        first = next((i for i, x in enumerate(lines) if x.startswith(("// Black 2's,", "// Black 2's;"))), None)
        if first is None:  # a struct of a C file ported before port_file.py noted them: no note to rewrite
            continue
        last = first
        while not lines[last].endswith((" in Black", "// Black", "against Black")) and last + 1 < len(lines):
            last += 1
        new = note_text(record, e, item)
        old = "\n".join(lines[first:last + 1])
        fields, sized = twin_confirmed(item)
        sure = {n for n in field_names(item.text) if n in e.info[record]["fields"]}
        if new == old or new and fields and (sure, e.info[record]["sized"]) == (fields, sized):
            continue  # the same news in other words
        comment = "\n".join(lines[:first] + ([new] if new else []) + lines[last + 1:])
        path = ROOT / header
        text = path.read_text()
        before = item.comment + "\n" + item.text
        if text.count(before) != 1:
            print(f"{record}: its note isn't in {header} as parsed")
            continue
        path.write_text(text.replace(before, (comment + "\n" if comment else "") + item.text))
        changed += 1
        gone += new is None
    return changed, gone


REG = re.compile(r"\b(r[0-9]|r1[0-2])\b")


def assembly():
    """{function: [its instructions]} of Black's disassembly (build/b_us/asm, which `ninja asm` writes)."""
    out = {}
    for s in sorted((ROOT / "build/b_us/asm").rglob("*.s")):
        cur = None
        for line in s.read_text(errors="replace").splitlines():
            m = re.match(r"^(\w+): ; 0x", line)
            if m:
                cur = out.setdefault(m[1], [])
            elif re.match(r"\s+(thumb|arm)_func_end", line):
                cur = None
            elif cur is not None and line.startswith("    ") and not line.strip().startswith("."):
                cur.append(line.strip())
    return out


def arguments_read(instructions):
    """How many of r0-r3 a function reads before it writes them, in the order of its code, up to its first call: the
    arguments it takes at least (the order of the code stands for the order it runs in)."""
    state = {}
    for line in instructions:
        op, _, rest = line.split(";")[0].strip().partition(" ")
        op = op.lower()
        if not op or op.endswith(":") or op == "push" or op == "stmdb" and "sp" in rest:
            continue
        if op.startswith("bl"):
            break
        if op.startswith("b") and not op.startswith("bic"):
            continue
        regs = REG.findall(rest)
        toks = [x.strip() for x in rest.split(",")]
        reads, writes = regs, []
        if op.startswith(("ldm", "pop")):
            reads = REG.findall(toks[0]) if "{" not in toks[0] else []
            writes = [r for r in regs if r not in reads]
        elif op.startswith("ldr"):
            reads, writes = regs[1:], regs[:1]
        elif op.startswith(("mov", "mvn", "neg", "rsb", "lsl", "lsr", "asr", "ror", "add", "sub", "and", "orr", "eor",
                            "bic", "adc", "sbc", "mul")):
            writes, srcs = REG.findall(toks[0]), [r for x in toks[1:] for r in REG.findall(x)]
            # Thumb's two operands (add r0, #1; add r0, r1) read the destination too, a move doesn't
            reads = srcs if len(toks) >= 3 or op.startswith(("mov", "mvn", "neg")) else writes + srcs
        for r in reads:
            state.setdefault(r, "read")
        for r in writes:
            state.setdefault(r, "write")
    used = [int(r[1]) for r, s in state.items() if s == "read" and r in ("r0", "r1", "r2", "r3")]
    return max(used) + 1 if used else 0


def check_protos():
    """Prints the prototypes under port_decls.py's note of untwinned ones that take fewer arguments than Black's
    function reads."""
    asm = assembly()
    found = 0
    for path in pd.header_files(ROOT):
        if pd.UNTWINNED_NOTE not in (ROOT / path).read_text(errors="replace"):
            continue
        after = False
        for item in pd.parse(ROOT / path):
            after = after or pd.UNTWINNED_NOTE in item.comment
            code = " ".join(pd.strip_comments(item.text).split())
            m = re.search(r"\((.*)\)\s*;$", code)
            if not after or item.kind != "proto" or not m or "..." in m[1] or item.names[0] not in asm:
                continue
            args = [] if m[1].strip() in ("", "void") else re.split(r",(?![^()]*\))", m[1])
            words = sum(2 if re.search(r"\b(u64|s64|fx64)\b", a) and "*" not in a else 1 for a in args)
            need = arguments_read(asm[item.names[0]])
            if need > words:
                found += 1
                print(f"{path}: {item.names[0]} takes {words}, Black's reads {need}: {code}")
    print(f"{found} prototypes take fewer arguments than Black's functions read")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--shifts", nargs="*", metavar="STRUCT")
    ap.add_argument("--offsets", action="store_true")
    ap.add_argument("--protos", action="store_true")
    ap.add_argument("--constants", action="store_true")
    ap.add_argument("--fix", nargs="+", metavar="STRUCT")
    ap.add_argument("--dry-run", action="store_true", help="with --fix: change nothing")
    ap.add_argument("--notes", action="store_true")
    ap.add_argument("--judge", nargs="*", metavar="STRUCT")
    ap.add_argument("--apply", action="store_true", help="with --judge: drop the rows of the functions that match")
    args = ap.parse_args()
    e = Evidence()
    if args.judge is not None:
        now, ok = judge_changes(e, [e.record(x) for x in args.judge])
        if args.apply and ok:
            gone = matched_rows(e, now)
            print(f"{len(gone)} rows of docs/nonmatching.md dropped: {', '.join(fn for fn, _ in gone)}")
        return
    if args.notes:
        changed, gone = notes(e)
        print(f"{changed} notes changed, {gone} of them dropped: matching code confirms those structs whole")
        return
    if args.offsets:
        offsets(e)
        return
    if args.protos:
        check_protos()
        return
    if args.constants:
        constants(e)
        return
    if args.shifts is not None:
        for r in [e.record(x) for x in args.shifts] or sorted(
                (r for r in e.structs if e.info[r]["parked"] or e.info[r]["near"]),
                key=lambda r: -len(e.info[r]["parked"]) - len({x[0] for x in e.info[r]["near"]})):
            files = sorted(set(e.info[r]["parked"].values())) or e.users(r)
            lay = next((x for f in files for x in [layouts(f, {r}).get(r)] if x), None)
            if lay:
                print_shifts(r, e.info[r], e.ev, lay, brief=not args.shifts)
        return
    if args.fix:
        for r in [e.record(x) for x in args.fix]:
            kept, now = fix(r, e, apply=not args.dry_run)
            if kept and not args.dry_run:
                gone = matched_rows(e, now)
                print(f"{r}: {'; '.join(kept)}; {len(gone)} parked functions match: "
                      + ", ".join(fn for fn, _ in gone))
        return
    print(f"{len(e.structs)} noted structs; {sum(1 for r in e.structs if e.info[r]['files'])} used by C here")
    for r in sorted(e.structs, key=lambda r: (-len(e.info[r]["parked"]), -len(e.info[r]["matching"]))):
        i = e.info[r]
        if i["files"]:
            print(f"{len(i['parked']):4} parked {len(i['matching']):4} matching {len(i['fields']):3} fields "
                  f"{'sized' if i['sized'] else '     '}  {r}  {e.structs[r][0]}")


if __name__ == "__main__":
    main()
