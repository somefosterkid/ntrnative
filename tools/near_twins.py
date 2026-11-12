#!/usr/bin/env python3
"""Black's functions that are pokebw2's but for some numbers ("near twins"), and what they show of Black's struct
layouts and constants.

A Thumb function of Black's of at least 0x30 bytes without a twin (tools/twins.py) whose code equals exactly one
pokebw2 function's once the offsets of the loads and stores (imm5), the 8-bit immediates (movs, cmp, adds, subs) and
the literal pool are masked is that function compiled with other numbers: a field at another offset, or another
constant. Smaller ones, and skeletons several pokebw2 functions share, are mostly getters and wrappers alike by chance.
pokebw2's objects are tools/twins.py's (build/twins_work, of the main checkout in a worktree).

A load or store whose offset differs says where a field pokebw2's function accesses is in Black: clang's AST of
pokebw2's source says which fields of which structs it accesses, the source's record layouts which of them the offset
falls in. tools/check_layouts.py counts these as votes beside the parked functions' diffs. The near twins that differ
in immediates only differ in constants: --constants lists them with the #defines pokebw2's function names that have
the immediate's value, for tools/black_constants.py. Cached in build/near_twins.json until an object changes.

    python3 tools/near_twins.py                # counts: near twins, by what differs, in complete files or not
    python3 tools/near_twins.py --fields       # the fields near twins show at other offsets in Black, by struct
    python3 tools/near_twins.py --fields X     # struct X's only
    python3 tools/near_twins.py --constants    # near twins that differ in immediates only, and pokebw2's #defines
"""
import argparse
import glob
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from twins import SHORT_RELOCS  # noqa: E402

BW2 = Path.home() / "ref" / "pokebw2"
CACHE = ROOT / "build" / "near_twins.json"
MIN_SIZE = 0x30
JOBS = 2
LOADSTORE = {0x0c: 4, 0x0d: 4, 0x0e: 1, 0x0f: 1, 0x10: 2, 0x11: 2}  # Thumb str/ldr/strb/ldrb/strh/ldrh imm5: width
IMM8 = (0x04, 0x05, 0x06, 0x07)  # movs, cmp, adds, subs #imm8


def work_dir():
    """tools/twins.py's build/twins_work: this checkout's, else the main checkout's (a worktree has none)."""
    here = ROOT / "build" / "twins_work"
    if here.exists():
        return here
    common = subprocess.run(["git", "rev-parse", "--git-common-dir"], cwd=ROOT, capture_output=True, text=True)
    return (ROOT / common.stdout.strip()).resolve().parent / "build" / "twins_work"


def functions(path):
    """[(name, mode, code)] of the functions of a relocatable ARM ELF, the relocated bytes zeroed."""
    d = open(path, "rb").read()
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", d, 0x2E)
    sh = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    syms, modes, relocs = [], {}, defaultdict(list)
    for s in sh:
        if s[1] == 2:
            strtab = sh[s[6]][4]
            for i in range(s[5] // 16):
                name, value, size, info, _, ndx = struct.unpack_from("<IIIBBH", d, s[4] + 16 * i)
                name = d[strtab + name:d.index(b"\0", strtab + name)].decode()
                if name in ("$a", "$t"):
                    modes[ndx, value] = name[1]
                elif info & 15 == 2 and size and not name.startswith("$"):
                    syms.append((name, ndx, value, size))
        elif s[1] in (4, 9):
            for o in range(s[4], s[4] + s[5], 12 if s[1] == 4 else 8):
                off, info = struct.unpack_from("<II", d, o)
                relocs[s[7]].append((off, SHORT_RELOCS.get(info & 255, 4)))
    out = []
    for name, ndx, value, size in syms:
        if ndx >= len(sh) or not sh[ndx][2] & 4:
            continue
        code = bytearray(d[sh[ndx][4] + value:sh[ndx][4] + value + size])
        for off, w in relocs[ndx]:
            if value <= off < value + size:
                code[off - value:off - value + w] = bytes(w)
        out.append((name, modes.get((ndx, value), "a"), bytes(code)))
    return out


def skeleton(code):
    """(digest of the code with the masked fields zeroed, its halfwords, the index of the last return): the literal
    pool follows the last return (pop {..., pc} or bx lr) and is masked whole."""
    hw = list(struct.unpack_from(f"<{len(code) // 2}H", code))
    last = max([i for i, h in enumerate(hw) if h & 0xff00 == 0xbd00 or h == 0x4770] + [len(hw) - 1])
    out = []
    for i, h in enumerate(hw):
        top = h >> 11
        out.append(0 if i > last else h & ~0x07c0 if top in LOADSTORE else h & ~0xff if top in IMM8 else h)
    return hashlib.sha1(struct.pack(f"<{len(out)}H", *out)).digest(), hw, last


def differences(ours, theirs, last):
    """[[kind, width, pokebw2's number, Black's number]] where the halfwords differ: a load's or store's offset
    ("mem", its width), an 8-bit immediate ("imm") or a literal pool word ("pool")."""
    out = []
    for i, (b, a) in enumerate(zip(ours, theirs)):
        if a == b or i > last:
            continue
        top = a >> 11
        if top in LOADSTORE:
            w = LOADSTORE[top]
            out.append(["mem", w, (a >> 6 & 0x1f) * w, (b >> 6 & 0x1f) * w])
        else:
            out.append(["imm", 0, a & 0xff, b & 0xff])
    start = (last + 1) * 2 + 2 & ~3
    words = lambda hw: [hw[i] | hw[i + 1] << 16 for i in range(start // 2, len(hw) - 1, 2)]  # noqa: E731
    out += [["pool", 4, a, b] for a, b in zip(words(theirs), words(ours)) if a != b]
    return out


def scan(work):
    """[{"black", "file", "bw2", "source", "size", "diffs"}] of the near twins: Black's function, the C file its
    delinked object stands for (None for a gap), pokebw2's function and source, the size and differences()."""
    bw2, exact = defaultdict(list), set()
    for p in glob.glob(f"{work}/obj/dsi/1.1p1/src/**/*.o", recursive=True):
        src = os.path.relpath(p, f"{work}/obj/dsi/1.1p1")[:-2] + ".c"
        for name, mode, code in functions(p):
            if mode == "t" and len(code) >= 0x10:
                exact.add(code)
                bw2[skeleton(code)[0]].append((name, src, code))
    out = []
    delinks = ROOT / "build" / "b_us" / "delinks"
    for p in sorted(delinks.rglob("*.o")):
        rel = str(p.relative_to(delinks))[:-2]
        file = None if rel.startswith("_dsd_gap@") else rel + ".c"
        for name, mode, code in functions(p):
            if mode != "t" or len(code) < MIN_SIZE or code in exact:
                continue
            h, hw, last = skeleton(code)
            if h not in bw2 or len({n for n, _, _ in bw2[h]}) != 1:
                continue
            b2, src, theirs = bw2[h][0]
            out.append({"black": name, "file": file, "bw2": b2, "source": src, "size": len(code),
                        "diffs": differences(hw, skeleton(theirs)[1], last)})
    return out


def clang(src, *extra, text=None):
    """clang's output for a pokebw2 source, with pokebw2's compile_flags.txt; text: in the source's place."""
    flags = (BW2 / "compile_flags.txt").read_text().split() + ["-fpack-struct=4"]  # MWCC's u64 (check_layouts.MWCC_LAYOUT)
    args = ["nice", "-n", "19", "clang", "-fsyntax-only", "-ferror-limit=0", "-w", *flags, f"-I{(BW2 / src).parent}",
            *extra]
    run = subprocess.run(args + (["-x", "c", "-"] if text is not None else [src]), cwd=BW2, input=text,
                         capture_output=True, text=True)
    return run.stdout


SCALAR = {"u8": 1, "s8": 1, "char": 1, "unsigned char": 1, "signed char": 1, "_Bool": 1, "u16": 2, "s16": 2, "fx16": 2,
          "short": 2, "unsigned short": 2, "u32": 4, "s32": 4, "int": 4, "unsigned int": 4, "BOOL": 4, "fx32": 4,
          "f32": 4, "long": 4, "unsigned long": 4, "u64": 8, "s64": 8, "fx64": 8}


def record_layouts(out, records):
    """{record: (size, [(offset, size, depth, path, element size, count)])} of clang's -fdump-record-layouts output: a
    field's size runs to the next field's offset, padding included; its element's (an array's, or its own) is its
    scalar type's or a pointer's, else an array's size / count, else None."""
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
            kind = field.strip().rsplit(" ", 1)[0]
            count = 1
            for n in re.findall(r"\[(\d+)\]", kind):
                count *= int(n)
            base = re.sub(r"\[\d+\]|\b(const|volatile)\b", "", kind).strip()
            elem = 4 if base.endswith("*") else SCALAR.get(base)
            rows.append((int(byte), depth, ".".join(names[d] for d in range(1, depth + 1)), count, elem))
        sized = []
        for i, (o, d, path, count, elem) in enumerate(rows):
            nxt = next((r[0] for r in rows[i + 1:] if r[1] <= d), size)
            sized.append((o, nxt - o, d, path, elem or ((nxt - o) // count if count > 1 else None), count))
        result[rec] = (size, sized)
    return result


def source_fields(job):
    """For one pokebw2 source: ({function: [[record, top field]]} its functions in the job access, {record: layout})."""
    import port_decls as pd
    src, funcs = job
    out = clang(src, "-Xclang", "-ast-dump=json")
    if not out.strip():
        return {}, {}
    ast = json.loads(out)
    owner, _, _ = pd.ast_records(ast)
    found = {fn: sorted(fields) for fn, (fields, _, _) in pd.ast_functions(ast, owner, set(funcs)).items()}
    records = sorted({r for fs in found.values() for r, _ in fs})
    force = "".join(f"typedef char __lay{i}[sizeof({r})];\n" for i, r in enumerate(records))
    lay = record_layouts(clang(src, "-Xclang", "-fdump-record-layouts", text=(BW2 / src).read_text() + "\n" + force),
                         set(records))
    return found, lay


def attribute(twins):
    """Adds to each near twin with a differing offset "fields": [[record, path, field offset, pokebw2's offset, Black's,
    width]] for each such difference that falls in a field its pokebw2 C accesses (several records' when it can't
    tell), from source_fields()."""
    todo = defaultdict(set)
    for t in twins:
        if any(k == "mem" for k, *_ in t["diffs"]):
            todo[t["source"]].add(t["bw2"])
    jobs = sorted((s, sorted(f)) for s, f in todo.items())
    print(f"clang: {len(jobs)} pokebw2 sources", file=sys.stderr)
    import jobs as bwjobs
    with ThreadPoolExecutor(bwjobs.jobs() if JOBS == 2 else JOBS) as pool:
        got = dict(zip((s for s, _ in jobs), pool.map(source_fields, jobs)))
    for t in twins:
        if t["source"] not in got:
            continue
        found, lay = got[t["source"]]
        accessed = found.get(t["bw2"], [])
        hits = []
        for kind, w, a, b in t["diffs"]:
            if kind != "mem":
                continue
            for record, top in accessed:
                if record not in lay:
                    continue
                rows = [r for r in lay[record][1] if r[3].split(".")[0] == top and r[0] <= a < r[0] + r[1]]
                if rows:
                    hits.append([record, rows[-1][3], rows[-1][0], a, b, w])
        t["fields"] = hits
    return twins


def near_twins():
    """The near twins, with their fields: cached in build/near_twins.json until an object of Black's or pokebw2's, or
    this file, changes."""
    work = work_dir()
    objs = glob.glob(f"{work}/obj/dsi/1.1p1/src/**/*.o", recursive=True) + \
        glob.glob(str(ROOT / "build/b_us/delinks/**/*.o"), recursive=True)
    key = hashlib.sha1(Path(__file__).read_bytes() + b"".join(
        f"{p}:{os.path.getmtime(p)}".encode() for p in sorted(objs))).hexdigest()
    if CACHE.exists():
        cached = json.loads(CACHE.read_text())
        if cached.get("key") == key:
            return cached["twins"]
    twins = attribute(scan(work))
    CACHE.parent.mkdir(exist_ok=True)
    CACHE.write_text(json.dumps({"key": key, "twins": twins}))
    return twins


def complete_files():
    """The C files the delinks.txt mark complete."""
    out, src = set(), None
    for path in (ROOT / "config/b_us/arm9").rglob("delinks.txt"):
        for line in open(path):
            if line.strip() and not line[0].isspace():
                src = line.strip().rstrip(":")
            elif src and line.strip() == "complete":
                out.add(src)
    return out


def kinds(t):
    return tuple(sorted({k for k, *_ in t["diffs"]})) or ("none",)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--fields", nargs="*", metavar="STRUCT")
    ap.add_argument("--constants", action="store_true")
    args = ap.parse_args()
    twins = near_twins()
    complete = complete_files()
    if args.fields is not None:
        want = {w.split()[-1] for w in args.fields}
        by = defaultdict(lambda: defaultdict(set))
        for t in twins:
            for record, path, o, a, b, w in t.get("fields", []):
                if not want or record.split()[-1] in want:
                    by[record][(path, o, a, b, w)].add(t["black"])
        for record in sorted(by, key=lambda r: -sum(len(v) for v in by[r].values())):
            print(record)
            for (path, o, a, b, w), fns in sorted(by[record].items(), key=lambda x: (x[0][2], x[0][3])):
                print(f"  {path:32} pokebw2 {a:#06x}, Black {b:#06x} ({b - a:+#x}, {w} bytes): "
                      f"{', '.join(sorted(fns)[:3])}{' ...' if len(fns) > 3 else ''}")
        return
    if args.constants:
        defines = re.compile(r"^#define ([A-Z][A-Z0-9_]+) (0x[0-9a-fA-F]+|\d+)\b", re.M)

        def values(root):
            out = {}
            for h in glob.glob(f"{root}/include/**/*.h", recursive=True) + \
                    glob.glob(f"{root}/build/include/generated/**/*.h", recursive=True):
                for m in defines.finditer(open(h, errors="replace").read()):
                    out.setdefault(m[1], int(m[2], 0))
            return out

        theirs, ours = values(BW2), values(ROOT)
        of = defaultdict(set)
        for n, v in theirs.items():
            of[v].add(n)
        stale = defaultdict(set)
        for t in sorted(twins, key=lambda t: t["black"]):
            if kinds(t) != ("imm",):
                continue
            text = (BW2 / t["source"]).read_text(errors="replace")
            m = re.search(rf"^[^\n;]*\b{t['bw2']}\([^;{{]*\)\s*\{{", text, re.M)
            body = text[m.start():text.find("\n}", m.end())] if m else ""
            named = set(re.findall(r"\b[A-Z][A-Z0-9_]{2,}\b", body))
            shown = []
            for _, _, a, b in t["diffs"]:
                names = sorted(of.get(a, set()) & named)
                shown.append(f"{a:#x}->{b:#x}" + (f" ({', '.join(names)})" if names else ""))
                for n in names:
                    if ours.get(n) == a:
                        stale[n].add((b, t["black"]))
            print(f"{t['black']}\t{t['file'] or '-'}\t{t['bw2']}\t{'; '.join(shown)}")
        print("\nThe constants the headers here still give pokebw2's value, and the near twins' value of Black's:")
        for n, seen in sorted(stale.items()):
            print(f"  {n} {ours[n]:#x}: Black's {', '.join(sorted({f'{b:#x}' for b, _ in seen}))} "
                  f"({', '.join(sorted(fn for _, fn in seen)[:3])})")
        return
    count = Counter()
    for t in twins:
        count[(kinds(t), t["file"] in complete)] += 1
    print(f"{len(twins)} near twins of at least {MIN_SIZE:#x} bytes; "
          f"{sum(1 for t in twins if t.get('fields'))} with differing offsets in fields their pokebw2 C accesses")
    for (k, comp), n in sorted(count.items(), key=lambda x: -x[1]):
        print(f"{n:5} {'+'.join(k):14} {'in complete files' if comp else ''}")


if __name__ == "__main__":
    main()
