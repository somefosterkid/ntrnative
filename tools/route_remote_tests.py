#!/usr/bin/env python3
"""Tests for tools/route_remote.py (route.py --remote), no game and no other machine needed: python3 tools/route_remote_tests.py
(prints "ok" per test, exits 1 on a failure). The round trip test runs the slot script in a scratch clone with a stand-in route.py."""
import io
import os
import subprocess
import sys
import tarfile
import tempfile
from pathlib import Path

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import route_remote as R  # noqa: E402

FAKE_ROUTE = '''import os, sys
from pathlib import Path
WORK = Path(os.environ["FAKE_WORK"])
SAVES = Path(os.environ["FAKE_SAVES"])
if __name__ == "__main__":
    print("args " + " ".join(sys.argv[1:]))
    print("dump: " + str(WORK / "b-lookleg-map" / "mapdump-1.txt") + " root " + str(Path.cwd()) + " save " + str(SAVES / "new.bin"))
    (WORK / "b-lookleg-map").mkdir(parents=True, exist_ok=True)
    (WORK / "b-lookleg-map" / "mapdump-1.txt").write_text("pos: zone 3 4,5\\n")
    (SAVES / "new.bin").write_bytes(b"\\x01\\x02")
    with open(SAVES / "README", "a") as f:
        f.write("new  a new save\\n")
    with open("tools/routes/b.txt", "a") as f:
        f.write("leg2 | new | M | ? | -\\n")
    Path("made.txt").write_text("made\\n")
    sys.exit(0)
'''


def sh(cwd, *a, **kw):
    return subprocess.run(list(a), cwd=cwd, check=True, capture_output=True, text=True, **kw).stdout


def repo(d):
    d = Path(d)
    (d / "tools/routes").mkdir(parents=True)
    (d / "tools/route.py").write_text(FAKE_ROUTE)
    (d / "tools/routes/b.txt").write_text("leg1 | start | M | zone 1 1,1 | -\n")
    (d / ".gitignore").write_text("build/\n")
    sh(d, "git", "init", "-q")
    sh(d, "git", "add", "-A")
    sh(d, "git", "-c", "user.name=t", "-c", "user.email=t@t", "commit", "-q", "-m", "base")
    return d


def test_strip_remote():
    assert R.strip_remote(["hg", "look", "s", "T", "--remote", "auto"]) == ("auto", ["hg", "look", "s", "T"])
    assert R.strip_remote(["b", "--remote", "laptop", "leg"]) == ("laptop", ["b", "leg"])
    assert R.strip_remote(["b", "leg", "--remote"]) == ("auto", ["b", "leg"])
    assert R.strip_remote(["b", "--remote", "--store", "x"]) == ("auto", ["b", "--store", "x"])
    assert R.strip_remote(["b", "--remote=bd,laptop"]) == ("bd,laptop", ["b"])
    assert R.strip_remote(["b", "leg"]) == (None, ["b", "leg"])
    try:
        R.strip_remote(["b", "--remote=mars"])
        raise AssertionError("accepted mars")
    except SystemExit:
        pass


def test_slot_args():
    assert R.slot_args("hg", "auto", "abc", 900) == ["--wait", "900", "--commit", "abc", "--hg", "--family", "hgss", "--no-port"]
    assert R.slot_args("pt", "bd", "abc", 0)[:2] == ["--on", "bd"] and "plat" in R.slot_args("pt", "bd", "abc", 0)
    assert R.slot_args("b2", "laptop", "abc", 5)[-2:] == ["--family", "gen5"]


def test_route_args():
    assert R.route_args(["b", "add", "x"]) == (["b", "add", "x"], False)
    assert R.route_args(["b", "leg1", "--repeat", "2"])[1] is True
    assert R.route_args(["b", "leg1", "-j", "3"])[1] is False and R.route_args(["b", "-j3"])[1] is False
    s = R.remote_script(["hg", "look", "sv", "Md C"])
    assert "tools/route.py hg look sv 'Md C' >" in s and "NTR_SLOT_JOBS" not in s.split("tools/route.py")[1].split("\n")[0]


def test_run_of():
    busy = "lp_route: 1118-1-a: commit a851d0ee5 (x)\nlp_route: busy: all slots are taken\nchain_slot: laptop: every slot is taken\n"
    assert R.run_of(busy) == ("", "")
    got = busy + "lp_route: bd-1118-2-a: commit a851d0ee5 (x)\nlp_route: slot hg1 (a dir), family hgss\nchain_slot: got a slot on bd\n"
    assert R.run_of(got) == ("bd-1118-2-a", "bd")


def test_transport_failed():
    assert R.transport_failed(2, "lp_route: slot 1 (d), family plat\nlp_route: bundle copy failed\n") == "bundle copy failed"
    assert R.transport_failed(2, "lp_route: 10:00:00 phase rom\nlp_route: 10:01:00 finished rc=2\n") == ""
    assert R.transport_failed(1, "lp_route: bundle copy failed\n") == ""


def test_rewrite():
    t = "dump: /r/slot1/build/w/x.txt save /r/saves/a.bin src /r/slot1/tools"
    out = R.rewrite(t, [("/r/slot1/build/w", "/me/w"), ("/r/saves", "/lib"), ("/r/slot1", "/me")])
    assert out == "dump: /me/w/x.txt save /lib/a.bin src /me/tools", out


def test_snapshot():
    with tempfile.TemporaryDirectory() as td:
        d = repo(td)
        head = sh(d, "git", "rev-parse", "HEAD").strip()
        assert R.snapshot(d) == (head, [])
        (d / "tools/routes/b.txt").write_text("leg1 | start | M2 | zone 1 1,1 | -\n")
        (d / "new.txt").write_text("n\n")
        (d / "build").mkdir()
        (d / "build/junk").write_text("ignored\n")
        sha, changed = R.snapshot(d)
        assert sha != head and sorted(changed) == ["new.txt", "tools/routes/b.txt"], changed
        assert sh(d, "git", "show", f"{sha}:tools/routes/b.txt") == "leg1 | start | M2 | zone 1 1,1 | -\n"
        assert sh(d, "git", "rev-parse", f"{sha}^").strip() == head
        assert R.snapshot(d)[0] == sha   # the same changes again: the same commit
        # the worktree's own index is untouched: the change is still unstaged, the new file untracked
        assert sh(d, "git", "status", "--porcelain").splitlines() == [" M tools/routes/b.txt", "?? new.txt"]
        try:
            R.snapshot(d, cap=1)
            raise AssertionError("cap not applied")
        except SystemExit as e:
            assert "new.txt" in str(e)


def test_round_trip():
    """the slot script in one clone (the slot), its archive unpacked into another (the worktree)"""
    with tempfile.TemporaryDirectory() as td:
        td = Path(td)
        src = repo(td / "src")
        slot = td / "slot"
        sh(td, "git", "clone", "-q", str(src), str(slot))
        rd, rwork, rsaves = td / "rd", td / "rwork", td / "rsaves"
        for p in (rd, rwork, rsaves):
            p.mkdir()
        (rsaves / "README").write_text("old  an old save\n")
        env = dict(os.environ, LP_RD=str(rd), NTR_RUN_ID="t-1", FAKE_WORK=str(rwork), FAKE_SAVES=str(rsaves))
        r = subprocess.run(["bash", "-c", R.remote_script(["b", "look", "sv", "Md C"])], cwd=slot, env=env,
                           capture_output=True, text=True)
        assert r.returncode == 0, r.stdout + r.stderr
        ret = rd / f"t-1.{R.RET}"
        assert ret.exists() and "args b look sv Md C" in r.stdout
        lwork, lsaves = td / "lwork", td / "lsaves"
        lsaves.mkdir()
        (lsaves / "README").write_text("old  an old save\n")
        rc, out, rep, meta = R.unpack(ret, src, lwork, lsaves)
        assert rc == 0 and meta["root"] == str(slot)
        assert f"dump: {lwork}/b-lookleg-map/mapdump-1.txt root {src} save {lsaves}/new.bin" in out, out
        assert (lwork / "b-lookleg-map/mapdump-1.txt").read_text() == "pos: zone 3 4,5\n"
        assert (lsaves / "new.bin").read_bytes() == b"\x01\x02"
        assert (lsaves / "README").read_text() == "old  an old save\nnew  a new save\n"
        assert (src / "tools/routes/b.txt").read_text().endswith("leg2 | new | M | ? | -\n")
        assert (src / "made.txt").read_text() == "made\n"
        assert any("applied" in l for l in rep) and any("new.bin" in l for l in rep), rep
        # again: the README line is not doubled; a save that differs is left alone unless it is --replace's
        (lsaves / "new.bin").write_bytes(b"\x09")
        sh(src, "git", "checkout", "-q", "--", "tools/routes/b.txt")
        rc, out, rep, meta = R.unpack(ret, src, lwork, lsaves)
        assert (lsaves / "README").read_text().count("new  a new save") == 1
        assert (lsaves / "new.bin").read_bytes() == b"\x09" and any("CONFLICT" in l for l in rep), rep
        sh(src, "git", "checkout", "-q", "--", "tools/routes/b.txt")
        R.unpack(ret, src, lwork, lsaves, replace={"new"})
        assert (lsaves / "new.bin").read_bytes() == b"\x01\x02"


def test_unsafe_members():
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w") as tf:
        for n in ("ok.txt", "../evil", "/abs"):
            i = tarfile.TarInfo(n)
            i.size = 1
            tf.addfile(i, io.BytesIO(b"x"))
    buf.seek(0)
    with tarfile.open(fileobj=buf) as tf:
        assert [m.name for m in R.safe_members(tf)] == ["ok.txt"]


if __name__ == "__main__":
    bad = 0
    for name, f in list(globals().items()):
        if name.startswith("test_") and callable(f):
            try:
                f()
                print("ok", name)
            except Exception as e:  # noqa: BLE001
                bad += 1
                print("FAIL", name, type(e).__name__, e)
    sys.exit(1 if bad else 0)
