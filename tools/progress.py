#!/usr/bin/env python3
"""Print how much of Black's code is in complete source files: code bytes and functions in the .text ranges of every
file marked complete in config/b_us/arm9/**/delinks.txt, against all functions in symbols.txt.

    python3 tools/progress.py
"""
import re
from pathlib import Path

CONFIG = Path(__file__).resolve().parents[1] / "config" / "b_us" / "arm9"
FUNCTION_RE = re.compile(r"kind:function\((?:arm|thumb),size=(0x[0-9a-f]+)[^)]*\) addr:(0x[0-9a-f]+)")


def main():
    total_bytes = total_functions = done_bytes = done_functions = files = 0
    for delinks in CONFIG.rglob("delinks.txt"):
        symbols = (delinks.parent / "symbols.txt").read_text()
        functions = [(int(m[2], 16), int(m[1], 16)) for m in FUNCTION_RE.finditer(symbols)]
        total_bytes += sum(size for _, size in functions)
        total_functions += len(functions)
        for entry in re.split(r"\n(?=\S)", delinks.read_text()):
            text = re.search(r"\.text\s+start:(0x[0-9a-f]+) end:(0x[0-9a-f]+)", entry)
            if not entry.startswith(("src/", "lib/")) or not re.search(r"^\s+complete$", entry, re.M) or not text:
                continue
            start, end = int(text[1], 16), int(text[2], 16)
            inside = [size for addr, size in functions if start <= addr < end]
            done_bytes += sum(inside)
            done_functions += len(inside)
            files += 1
    print(f"{files} files complete: {done_bytes:,} of {total_bytes:,} code bytes ({100 * done_bytes / total_bytes:.2f}%), "
          f"{done_functions:,} of {total_functions:,} functions ({100 * done_functions / total_functions:.2f}%)")


if __name__ == "__main__":
    main()
