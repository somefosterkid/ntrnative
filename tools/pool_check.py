#!/usr/bin/env python3
"""Check that no file's .text range starts after the constant pool of its first function.

`dsd dis` logs "Function at X was adjusted to include pre-code constant pool at Y" when a literal pool sits right
before the function at X. If a file's .text range starts at X (Y is before it: the pool is in a gap or the file before),
dis panics (cli/src/cmd/dis.rs:282, index out of bounds) and `ninja asm` fails for everyone: tools/add_source_file.py
(so tools/prep.py, port_lib.py, gap_setup.py and finish.py) runs this after it sets a range, and a start before Y moves
to Y when nothing else covers the pool, else the setup is refused. Fine as it is: a range that contains Y.

    python3 tools/pool_check.py            # every file in the delinks.txt: the ones that would make dis panic
    python3 tools/pool_check.py --fix      # also move each such file's start to Y (where no other file has the pool)
"""
import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from file_ranges import module_dir, read_delinks  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config" / "b_us" / "arm9"
ADJUSTED = re.compile(r"Function at (0x[0-9a-fA-F]+) was adjusted to include pre-code constant pool at (0x[0-9a-fA-F]+)")


def dis_log():
    """What `dsd dis` prints into a scratch directory: (the (function, pool) addresses it adjusted, did it panic)."""
    with tempfile.TemporaryDirectory() as tmp:
        r = subprocess.run([str(ROOT / "tools" / "dsd"), "dis", "--config-path", str(CONFIG / "config.yaml"),
                            "--asm-path", tmp], capture_output=True, text=True, cwd=ROOT)
    text = r.stdout + r.stderr
    return [(int(a, 16), int(b, 16)) for a, b in ADJUSTED.findall(text)], "panicked" in text, text


def functions(mod):
    """The addresses of a module's functions (symbols.txt)."""
    path = CONFIG / module_dir(mod) / "symbols.txt"
    return {int(m[1], 16) for m in re.finditer(r"kind:function\S* addr:(0x[0-9a-f]+)", path.read_text())}


def problems(only=None):
    """[(function, pool, module, source, (start, end), others)] of the files whose .text starts at a function whose
    constant pool lies before it (only: just that source); others: the files whose .text covers the pool."""
    adjusted, _, _ = dis_log()
    delinks = read_delinks(str(CONFIG))
    out = []
    for x, y in adjusted:
        for mod, (_, files) in delinks.items():
            for src, sections in files.items():
                if ".text" not in sections or (only and src != only):
                    continue
                start, end = sections[".text"]
                if start == x and y < start and x in functions(mod):
                    others = [f for f, s in files.items() if f != src and ".text" in s and s[".text"][0] <= y < s[".text"][1]]
                    out.append((x, y, mod, src, (start, end), others))
    return out


def set_start(mod, src, start):
    """Moves the .text start of src in its module's delinks.txt."""
    path = CONFIG / module_dir(mod) / "delinks.txt"
    text = path.read_text()
    new, n = re.subn(r"(^" + re.escape(src) + r":\n(?:    .*\n)*?    \.text\s+start:)0x[0-9a-f]+", lambda m: m[1] + f"{start:#010x}",
                     text, count=1, flags=re.M)
    if n:
        path.write_text(new)
    return bool(n)


def describe(p):
    x, y, mod, src, (start, end), others = p
    return (f"{src} ({mod}): .text starts at {start:#010x}, the function there has its constant pool at {y:#010x} "
            f"before it, so dsd dis panics" + (f"; {', '.join(others)} covers the pool" if others else ""))


def guard(mod, src):
    """After a range of src is set: move its start to the pool if that's a gap (None), else (or if dis still panics)
    the reason the range can't stay."""
    for _ in range(3):
        found = problems(src)
        if not found:
            return None
        p = found[0]
        if p[5]:
            return describe(p) + f": end {p[5][0]}'s .text at {p[1]:#010x} first (or start this one there)"
        if not set_start(p[2], src, p[1]):
            return describe(p)
        print(f"{src}: .text now starts at {p[1]:#010x}, where the constant pool of its first function begins "
              f"(dsd dis panics otherwise)")
    return f"{src}: .text range still makes dsd dis panic after moving its start"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--fix", action="store_true", help="move such a file's start to the pool when no file has it")
    args = ap.parse_args()
    found = problems()
    for p in found:
        print(describe(p))
        if args.fix and not p[5]:
            set_start(p[2], p[3], p[1])
            print(f"  moved its start to {p[1]:#010x}")
    _, panicked, text = dis_log()
    print(f"{len(found)} file(s) with a range that starts after its first function's constant pool"
          + ("; dsd dis panics:\n" + text[-400:] if panicked else ""))
    return 1 if found and not args.fix else 0


if __name__ == "__main__":
    sys.exit(main())
