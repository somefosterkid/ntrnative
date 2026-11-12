#!/usr/bin/env python3
"""Replays bss_statics.py on the files whose extern Black's source defined as a static of the file.

    python3 tools/bss_statics_replay.py [REV]       # REV: the commit holding the before states (default ea3f17b91)

Each case puts a file's C of that commit (or a synthetic version of it) in the worktree, runs bss_statics.py --no-ranges on
it, prints the declarations of the .bss symbol before and after next to the ones HEAD has (the matched C), and puts the
file back as it was. Nothing is committed. Takes about a minute."""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
D = "lib/dwcutil/src/"
CASES = [  # (file, the .bss symbols)
    ("sound.c", ["data_ov016_021de648"]),
    ("archive.c", ["data_ov016_021de66c"]),
    ("anime.c", ["data_ov016_021de504"]),
    ("screen.c", ["data_ov016_021de630"]),
    ("interrupt.c", ["data_ov016_021de6a0", "data_ov016_021de6a4"]),  # synthetic: the C before has no function
    ("objVram.c", ["data_ov016_021de6ac"]),
    ("cellInfo.c", ["data_ov016_021de674"]),
]


def git(*a):
    return subprocess.run(["git", *a], cwd=ROOT, capture_output=True, text=True).stdout


def decls(text, syms):
    code = re.sub(r"//[^\n]*", "", text)
    return [l.strip() for l in code.splitlines()
            if re.match(r"(static|extern)\b[^(]*\b(" + "|".join(syms) + r")\b[^(]*;", l.strip())]


def synthetic_interrupt(text):
    """HEAD's interrupt.c with the two statics as the one extern an auto port would declare."""
    text = re.sub(r"static OSIrqMask data_ov016_021de6a4;\nstatic OSIrqFunction data_ov016_021de6a0;\n",
                  "extern s32 data_ov016_021de6a0;\n", text)
    text = text.replace("data_ov016_021de6a4", "((OSIrqMask *)&data_ov016_021de6a0)[1]")
    text = text.replace("data_ov016_021de6a0 = OS_GetIrqFunction", "data_ov016_021de6a0 = (s32)OS_GetIrqFunction")
    return text.replace("OS_SetIrqFunction(OS_IE_V_BLANK, data_ov016_021de6a0);",
                        "OS_SetIrqFunction(OS_IE_V_BLANK, (OSIrqFunction)data_ov016_021de6a0);")


def main():
    rev = sys.argv[1] if len(sys.argv) > 1 else "ea3f17b91"
    for name, syms in CASES:
        path = D + name
        full = os.path.join(ROOT, path)
        mine = open(full).read()
        main_text = git("show", f"HEAD:{path}")
        before = synthetic_interrupt(main_text) if name == "interrupt.c" else git("show", f"{rev}:{path}")
        try:
            open(full, "w").write(before)
            r = subprocess.run([sys.executable, "tools/bss_statics.py", "--no-ranges", path], cwd=ROOT,
                               capture_output=True, text=True)
            after = open(full).read()
        finally:
            open(full, "w").write(mine)
        kept = [l.strip() for l in r.stdout.splitlines() if "kept for the tell" in l or "-> (" in l]
        print(f"== {name}")
        print("  before:", decls(before, syms))
        print("  after: ", decls(after, syms), "<- tool")
        print("  main:  ", decls(main_text, syms))
        print("  tool:  ", (kept[0] if kept else (r.stdout.strip().splitlines() or ["no output"])[-2:]))


if __name__ == "__main__":
    main()
