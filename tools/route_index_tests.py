#!/usr/bin/env python3
"""Tests for tools/route_index.py, no game needed: python3 tools/route_index_tests.py   (prints "ok" per test, exits 1 on a failure)"""
import glob
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import route_index as R  # noqa: E402

SAMPLE = '''// the file
#define X 1
int gShared;   // a global
// ---- the part ----
// does a thing
static int Thing(int a) {
    char c = '{';   /* a brace in a char */
    return a;
}

typedef struct {
    int a;   // a field
} Pair;   // a pair

int Plain(void) {   // trailing
    return 0;
}
'''


def test_items():
    its = R.items("x.c", SAMPLE, False)
    names = {(k, n) for _, k, n, _ in its}
    assert ("var", "gShared") in names and ("static fn", "Thing") in names and ("type", "Pair") in names and ("fn", "Plain") in names
    c = {n: cm for _, _, n, cm in its}
    assert c["gShared"] == "a global" and c["Plain"] == "trailing" and c["Thing"] == "does a thing"
    assert ("sect", "the part") in names


def test_token_table():
    src = ("static void Pilot(void) {\n    if (*sTok == 'W' && (sTok[1] == 'f' || sTok[1] == 't')) {   // waits\n"
           "        WaitStep();\n        return;\n    }\n    switch (*sTok) {\n    case 'K':\n        KeyBits();\n        return;\n    }\n}\n")
    rows, _ = R.token_table(src, {"WaitStep", "KeyBits", "Pilot"})
    assert [(r[1], r[2]) for r in rows] == [("W[ft]", ["WaitStep"]), ("K", ["KeyBits"])]


def run_index(*args):
    return subprocess.run([sys.executable, os.path.join(HERE, "route_index.py"), *args], capture_output=True, text=True, check=True).stdout


def test_repo_index_has_every_route_file():
    """content, not a line count: every route source and header is listed, and every Pilot() token handler is in the table"""
    out = run_index()
    root = os.path.dirname(HERE)
    for pat in ("port/src/route*.c", "port/src/route*.h", "port/include/port_route*.h"):
        files = glob.glob(os.path.join(root, pat))
        assert files, pat
        for f in files:
            rel = os.path.relpath(f, root).replace(os.sep, "/")
            assert f"== {rel} (" in out, rel
    assert "GoStep" in out and "== pilot tokens" in out and "GOAL_WARP" in out
    # an independent look at Pilot(): its handlers' letters (an `if (... *sTok == 'X'` or a `case 'X':` at the function's top level)
    text = open(os.path.join(root, "port/src/route_test.c"), encoding="utf-8", errors="replace").read()
    span = R.function_span(text, "Pilot")
    assert span and span[1] - span[0] > 100, span
    letters = set()
    for s in text.split("\n")[span[0]:span[1]]:
        m = re.match(r"^    (?:if \(.*?\*sTok == '(.)'|case '(.)':)", s)
        if m:
            letters.add(m.group(1) or m.group(2))
    assert len(letters) >= 10, letters
    table = out.split("== pilot tokens", 1)[1].split("\n== G:", 1)[0]
    shown = {m.group(1) for m in re.finditer(r"^\s*\d+ (\S)", table, re.M)}
    assert letters <= shown, sorted(letters - shown)


def test_filters():
    one = run_index("route_slide.c")
    assert "== port/src/route_slide.c" in one and "== port/src/route_map.c" not in one and "== pilot tokens" not in one
    assert "== pilot tokens" in run_index("route_test.c")
    tok = run_index("--token", "W")
    assert "pilot tokens starting with W" in tok and "WaitStep (route_test.c " in tok and "== port/src" not in tok
    assert "G:warp:" in run_index("--token", "G")
    cc = run_index("--token", "C")   # Cc (a scripted battle's catch, before a G:npc) and C:ball are both pilot tokens
    assert "Cc" in cc and "C:" in cc and "CatchStep" in cc, cc
    bad = subprocess.run([sys.executable, os.path.join(HERE, "route_index.py"), "--token", "Q"], capture_output=True, text=True)
    assert bad.returncode == 1 and "no pilot token" in bad.stderr
    bad = subprocess.run([sys.executable, os.path.join(HERE, "route_index.py"), "nosuch.c"], capture_output=True, text=True)
    assert bad.returncode == 1 and "no file nosuch.c" in bad.stderr


if __name__ == "__main__":
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            fn()
            print("ok", name)
