#!/usr/bin/env python3
"""The PC build's compile errors by file and by cause, from a `ninja -k 0` log of the port build (docs/port-build.md).

    ninja -C build/port/linux -k 0 > log 2>&1; python3 tools/port_errors.py log            # files, then causes
    python3 tools/port_errors.py log --files                                              # file<TAB>first error
    python3 tools/port_errors.py log --file src/system/main.c                             # one file's errors
"""
import argparse
import collections
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FAILED = re.compile(r"^FAILED: \[code=\d+\] (\S+)")
STEP = re.compile(r"^\[\d+/\d+\] ")
DIAG = re.compile(r"^(.+?):(\d+):\d+: (?:fatal )?error: (.*)$")


def parse(log):
    """{source (repo-relative, or the object for a subproject's): [(path, line, msg)]}"""
    out, cur = {}, None
    for line in Path(log).read_text(errors="replace").splitlines():
        line = re.sub(r"\x1b\[[0-9;]*m", "", line)
        m = FAILED.match(line)
        if m:
            obj = m[1]
            src = re.sub(r"^.*?bwport\.p/", "", obj)
            src = re.sub(r"\.c\.o$", ".c", src)
            prefix = str(ROOT).replace("/", "_")
            cur = real_path(src[len(prefix) + 1:]) if src.startswith(prefix) else obj
            out[cur] = []
            continue
        if STEP.match(line) or line.startswith("ninja:"):
            cur = None
            continue
        m = DIAG.match(line)
        if m and cur is not None:
            p = m[1]
            try:
                p = str(Path(p).resolve().relative_to(ROOT))
            except ValueError:
                pass
            out[cur].append((p, int(m[2]), m[3]))
    return out


def real_path(mangled):
    """Undo meson's '/' -> '_' in an object name (src_system_main.c): the repo file it stands for."""
    def walk(base, rest):
        if not rest:
            return base if base.is_file() else None
        for i in range(1, len(rest) + 1):
            name = "_".join(rest[:i])
            if (base / name).exists():
                r = walk(base / name, rest[i:])
                if r:
                    return r
        return None
    r = walk(ROOT, mangled.split("_"))
    return str(r.relative_to(ROOT)) if r else mangled


def cause(msg):
    ids = re.findall(r"'([^']*)'", msg)
    if msg.startswith("no member named"):
        return f"field {ids[1]}.{ids[0]}" if len(ids) > 1 else "field"
    for pre, name in (("call to undeclared function", "undeclared function"), ("use of undeclared identifier",
                      "undeclared identifier"), ("unknown type name", "unknown type"), ("conflicting types for",
                      "conflicting types"), ("redefinition of", "redefinition"), ("typedef redefinition",
                      "typedef redefinition"), ("static declaration", "static after non-static")):
        if msg.startswith(pre):
            return f"{name} {ids[0] if ids else ''}"
    return re.sub(r"'[^']*'", "'_'", msg)[:90]


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("log")
    ap.add_argument("--files", action="store_true")
    ap.add_argument("--file")
    ap.add_argument("--top", type=int, default=60)
    a = ap.parse_args()
    res = parse(a.log)
    if a.file:
        for p, ln, m in res.get(a.file, []):
            print(f"{p}:{ln}: {m}")
        return
    if a.files:
        for f, errs in sorted(res.items()):
            print(f"{f}\t{len(errs)}\t" + (f"{errs[0][0]}:{errs[0][1]}: {errs[0][2]}" if errs else "?"))
        return
    print(f"{len(res)} files fail")
    by = collections.Counter()
    files = collections.defaultdict(set)
    for f, errs in res.items():
        for p, ln, m in errs:
            c = cause(m)
            by[c] += 1
            files[c].add(f)
    for c, n in sorted(files.items(), key=lambda kv: -len(kv[1]))[:a.top]:
        print(f"{len(n):4} files {by[c]:5} errors  {c}")


if __name__ == "__main__":
    main()
