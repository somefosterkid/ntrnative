#!/usr/bin/env python3
"""Convert a dependency file that mwccarm wrote under wibo to Unix paths, for ninja.

mwccarm's `-gccdep -MD` writes paths like `Z:\\home\\...\\include\\types.h`. This rewrites the file in place with the
drive letter removed and forward slashes, keeping the backslashes that continue lines, and the checkout's own files
relative to it (ninja runs there): a worktree that starts from a copy of main's build (tools/setup_worktree.sh) then
tracks its own headers.

    fix_depfile.py build/b_us/src/ov198/b_plist_anm.d

From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), tools/scripts/fix_depfile.py.
"""
import os
import re
import sys
from pathlib import Path

HERE = os.getcwd().rstrip("/") + "/"


def unix_path(path: str) -> str:
    path = re.sub(r"^[A-Za-z]:", "", path).replace("\\", "/")
    return path[len(HERE):] if path.startswith(HERE) else path


def main():
    path = Path(sys.argv[1])
    lines = []
    for line in path.read_text().splitlines():
        continued = line.endswith(" \\")
        body = line[:-2] if continued else line
        target, sep, rest = body.partition(": ")
        if sep and not body.startswith((" ", "\t")):
            body = unix_path(target) + ": " + " ".join(unix_path(p) for p in rest.split())
        else:
            body = "\t" + " ".join(unix_path(p) for p in body.split())
        lines.append(body + (" \\" if continued else ""))
    path.write_text("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
