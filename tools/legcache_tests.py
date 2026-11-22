#!/usr/bin/env python3
"""Tests for legcache.py (a route leg's result cache): python3 tools/legcache_tests.py"""
import io
import json
import lzma
import os
import subprocess
import sys
import tempfile
from contextlib import redirect_stdout
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import legcache as L

ROUTE = "a | NEWGAME | Pa | zone 1 1,1 | s-a\nb | s-a | Pb | zone 2 1,1 | s-b\nc | s-b | Pc | zone 3 1,1\nd | lib-x | Pd | zone 4 1,1 | s-d\n"


def repo():
    d = Path(tempfile.mkdtemp())
    for p, t in (("tools/routes/zz.txt", ROUTE), ("src/game.c", "int x;\n"), ("docs/a.md", "doc\n"), ("port/gpu/shaders/g.glsl", "v\n")):
        (d / p).parent.mkdir(parents=True, exist_ok=True)
        (d / p).write_text(t)
    subprocess.run(["git", "-C", str(d), "init", "-q"], check=True)
    subprocess.run(["git", "-C", str(d), "add", "."], check=True)
    subprocess.run(["git", "-C", str(d), "-c", "user.email=t@t", "-c", "user.name=t", "commit", "-qm", "x"], check=True)
    (d / "saves").mkdir()
    (d / "saves/lib-x.bin").write_bytes(b"LIB")
    cand = d / "cand"
    cand.mkdir()
    (cand / "table.tsv").write_text("leg\tframes\tseconds\tresult\na\t100\t1.0\tok\nb\t200\t2.0\tok\nc\t300\t3.0\tok\nd\t400\t4.0\tok\ntotal\t1000\t10.0\t4/4 ok\n")
    for n in ("a", "b", "d"):
        (cand / f"{n}.bin.xz").write_bytes(lzma.compress(f"SAVE-{n}".encode()))
    return d


def run(d, *a):
    out = io.StringIO()
    with redirect_stdout(out):
        rc = L.main(list(a) + ["--game", "zz", "--root", str(d), "--cache", str(d / "cache"), "--saves", str(d / "saves"), "--klass", "bd"])
    return rc, out.getvalue().strip()


def t_add_then_probe():
    d = repo()
    assert run(d, "probe")[0] == 1
    assert run(d, "add", "--cand", str(d / "cand"))[1] == "added 4/4 legs of zz (class bd)"
    rc, out = run(d, "probe", "--table", str(d / "t.tsv"))
    assert rc == 0 and out == "CACHED 4/4 legs, 1000 frames", out
    assert (d / "t.tsv").read_text().splitlines()[-1] == "total\t1000\t10.0\t4/4 ok"


def t_pictures_and_docs_keep_hits():
    d = repo()
    run(d, "add", "--cand", str(d / "cand"))
    (d / "docs/a.md").write_text("changed\n")
    (d / "port/gpu/shaders/g.glsl").write_text("changed\n")
    assert run(d, "probe")[0] == 0


def t_game_code_misses():
    d = repo()
    run(d, "add", "--cand", str(d / "cand"))
    (d / "src/game.c").write_text("int y;\n")   # (a work-tree edit counts as much as a commit)
    assert run(d, "probe") == (1, "MISS at leg a (0/4)")


def t_leg_line_and_start_save():
    d = repo()
    run(d, "add", "--cand", str(d / "cand"))
    (d / "tools/routes/zz.txt").write_text(ROUTE.replace("Pc", "Pc2"))
    assert run(d, "probe") == (1, "MISS at leg c (2/4)")
    (d / "tools/routes/zz.txt").write_text(ROUTE)
    (d / "saves/lib-x.bin").write_bytes(b"LIB2")   # leg d starts from the library: other bytes, another key
    assert run(d, "probe") == (1, "MISS at leg d (3/4)")
    assert run(d, "probe", "--klass", "laptop")[0] == 1   # (the last --klass wins: another machine class)


def t_fail_never_stored():
    d = repo()
    t = d / "cand/table.tsv"
    t.write_text(t.read_text().replace("b\t200\t2.0\tok", "b\t200\t2.0\toff-route"))
    assert run(d, "add", "--cand", str(d / "cand"))[1].startswith("added 1/4")
    assert run(d, "probe") == (1, "MISS at leg b (1/4)")
    assert not L.put(str(d / "cache"), "zz", "k", "x", "crash", 1, 1.0, None)


def t_a_name_twice():   # Platinum has gym, mine, oreburgh twice: the table's rows by position, the save only for the last
    d = repo()
    (d / "tools/routes/zz.txt").write_text(ROUTE + "c | s-d | Pe | zone 5 1,1\n")
    t = d / "cand/table.tsv"
    t.write_text(t.read_text().replace("total\t1000\t10.0\t4/4", "c\t50\t1.0\tok\ntotal\t1050\t11.0\t5/5"))
    assert run(d, "add", "--cand", str(d / "cand"))[1].startswith("added 5/5"), run(d, "probe")
    assert run(d, "probe") == (0, "CACHED 5/5 legs, 1050 frames")


def t_a_save_making_block_twice():   # Platinum's legs 24-26 again as 27-29: same lines, same start saves, the last saves kept
    d = repo()
    (d / "tools/routes/zz.txt").write_text(ROUTE + "b | s-a | Pb | zone 2 1,1 | s-b\nc | s-b | Pc | zone 3 1,1\n")
    t = d / "cand/table.tsv"
    t.write_text(t.read_text().replace("total\t1000\t10.0\t4/4", "b\t200\t2.0\tok\nc\t300\t3.0\tok\ntotal\t1500\t15.0\t6/6"))
    assert run(d, "add", "--cand", str(d / "cand"))[1].startswith("added 6/6")
    assert run(d, "probe") == (0, "CACHED 6/6 legs, 1500 frames")
    d = repo()   # the second b starts from another save (d's): its key differs; the first b's save bytes are not kept
    (d / "tools/routes/zz.txt").write_text(ROUTE + "b | s-d | Pb | zone 2 1,1 | s-b\n")
    t = d / "cand/table.tsv"
    t.write_text(t.read_text().replace("total\t1000\t10.0\t4/4", "b\t201\t2.0\tok\ntotal\t1201\t12.0\t5/5"))
    assert run(d, "add", "--cand", str(d / "cand"))[1].startswith("added 5/5")
    assert run(d, "probe") == (0, "CACHED 5/5 legs, 1201 frames")
    seen = sorted(json.loads(p.read_text())["save"] for p in (d / "cache/zz").glob("*.json"))
    assert seen.count("?") == 1 and seen.count("") == 1, seen   # (b's first run: "?"; c: no save)


EXE = "a" * 64 + " " + "b" * 64
ROUTE_PY = '"""route."""\nimport helper\n\n\ndef play(x):\n    """doc"""\n    # a comment\n    return x + 1\n'


def with_driver(d):
    (d / "tools").mkdir(exist_ok=True)
    (d / "tools/route.py").write_text(ROUTE_PY)
    (d / "tools/helper.py").write_text("X = 1\n")
    (d / "tools/unrelated.py").write_text("Y = 1\n")
    subprocess.run(["git", "-C", str(d), "add", "."], check=True)
    return d


def t_exe_key_early_cutoff():   # the same artifact: a source change that builds the same exe, or a comment in the driver, keeps the chain cached
    d = with_driver(repo())
    run(d, "add", "--cand", str(d / "cand"), "--exe", EXE)
    assert run(d, "probe") == (0, "CACHED 4/4 legs, 1000 frames")
    (d / "src/game.c").write_text("int y;\n")   # the source key misses ...
    assert run(d, "probe") == (1, "MISS at leg a (0/4)")
    assert run(d, "probe", "--exe", EXE) == (0, "CACHED 4/4 legs, 1000 frames (exe key)")   # ... the same exe hits
    (d / "tools/route.py").write_text(ROUTE_PY.replace("# a comment", "# another comment\n\n").replace('"""doc"""', '"""other doc"""'))
    assert run(d, "probe", "--exe", EXE)[0] == 0
    (d / "docs/a.md").write_text("x\n")
    assert run(d, "probe", "--exe", EXE, "--exe-only")[0] == 0
    (d / "tools/unrelated.py").write_text("Y = 2\n")   # (not imported by route.py)
    assert run(d, "probe", "--exe", EXE, "--exe-only")[0] == 0


def t_exe_key_misses():
    d = with_driver(repo())
    run(d, "add", "--cand", str(d / "cand"), "--exe", EXE)
    (d / "src/game.c").write_text("int y;\n")
    assert run(d, "probe", "--exe", "c" * 64 + " " + "b" * 64) == (1, "MISS at leg a (0/4)")   # another exe
    assert run(d, "probe", "--exe", "a" * 64 + " " + "c" * 64) == (1, "MISS at leg a (0/4)")   # other ROM definitions
    (d / "tools/helper.py").write_text("X = 2\n")   # the driver's code (an imported module)
    assert run(d, "probe", "--exe", EXE)[0] == 1
    (d / "tools/helper.py").write_text("X = 1\n")
    (d / "tools/route.py").write_text(ROUTE_PY.replace("x + 1", "x + 2"))
    assert run(d, "probe", "--exe", EXE)[0] == 1
    (d / "tools/route.py").write_text(ROUTE_PY)
    (d / "tools/routes/zz.txt").write_text(ROUTE.replace("Pc", "Pc2"))   # a leg's line
    assert run(d, "probe", "--exe", EXE, "--exe-only") == (1, "MISS at leg c (2/4)")


if __name__ == "__main__":
    bad = 0
    for n, f in sorted(globals().items()):
        if n.startswith("t_"):
            try:
                f()
                print("ok", n)
            except AssertionError as e:
                bad = 1
                print("FAIL", n, repr(e))
    print("legcache_tests:", "FAIL" if bad else "PASS")
    sys.exit(bad)
