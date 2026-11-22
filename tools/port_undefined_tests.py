#!/usr/bin/env python3
"""Tests for tools/port_undefined.py's nm cache, no game needed: python3 tools/port_undefined_tests.py   (prints "ok" per
test, exits 1 on a failure)"""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import port_undefined as U  # noqa: E402


def objects(d):
    for name, src in (("a", "int f_a(void){return 1;}\nint g_a = 3;\n"), ("sub/b", "int f_b(void){return 2;}\nint g_b[8];\n")):
        p = Path(d, name + ".c")
        p.parent.mkdir(exist_ok=True)
        p.write_text(src)
        subprocess.run(["cc", "-c", "-o", str(p.with_suffix(".o")), str(p)], check=True)


class CountNm:
    """subprocess.run in port_undefined, counting the objects nm is asked about."""

    def __init__(self):
        self.asked = []
        self.run = subprocess.run

    def __enter__(self):
        def run(cmd, *a, **k):
            if cmd[0] == "nm":
                self.asked += [Path(c).name for c in cmd if c.endswith(".o")]
            return self.run(cmd, *a, **k)
        U.subprocess.run = run
        return self

    def __exit__(self, *e):
        U.subprocess.run = self.run


def test_cache_matches_nm_and_skips_unchanged_objects():
    with tempfile.TemporaryDirectory() as d:
        rom, cache = Path(d, "rom"), Path(d, "cache.json")
        rom.mkdir()
        objects(rom)
        with CountNm() as n:
            cold = U.rom_definitions(rom, cache)
            assert sorted(n.asked) == ["a.o", "b.o"]
        assert cold[0]["f_a"] == ["a.o"] and cold[0]["g_b"] == ["sub/b.o"]
        assert cold[1]["f_b"][0] == "T" and cold[1]["g_b"] == ("B", 32)
        with CountNm() as n:
            warm = U.rom_definitions(rom, cache)
            assert n.asked == []
        assert (dict(warm[0]), warm[1]) == (dict(cold[0]), cold[1])
        # one object rewritten (a new symbol, a new size): only it is read again, its old symbols are gone
        Path(rom, "a.c").write_text("int f_a2(void){return 1;}\nint g_a = 3;\nint more[100];\n")
        subprocess.run(["cc", "-c", "-o", str(rom / "a.o"), str(rom / "a.c")], check=True)
        with CountNm() as n:
            defs, kinds = U.rom_definitions(rom, cache)
            assert n.asked == ["a.o"]
        assert "f_a" not in defs and defs["f_a2"] == ["a.o"] and kinds["more"] == ("B", 400)


def test_frozen_cache_needs_no_rom_dir():
    with tempfile.TemporaryDirectory() as d:
        rom, cache = Path(d, "rom"), Path(d, "cache.json")
        rom.mkdir()
        objects(rom)
        cold = U.rom_definitions(rom, cache)
        gone = Path(d, "no_rom_here")   # a slot job: no build/b_us at all
        os.environ["BWPORT_ROM_DEFS_FROZEN"] = "1"
        try:
            with CountNm() as n:
                frozen = U.rom_definitions(gone, cache)
                assert n.asked == []
            assert (dict(frozen[0]), frozen[1]) == (dict(cold[0]), cold[1])
            cache.write_text("{not json")
            try:
                U.rom_definitions(gone, cache)
                assert False
            except SystemExit:
                pass
        finally:
            del os.environ["BWPORT_ROM_DEFS_FROZEN"]


def test_cache_of_another_rom_dir_or_garbage_is_ignored():
    with tempfile.TemporaryDirectory() as d:
        rom, cache = Path(d, "rom"), Path(d, "cache.json")
        rom.mkdir()
        objects(rom)
        cache.write_text("{not json")
        with CountNm() as n:
            U.rom_definitions(rom, cache)
            assert len(n.asked) == 2
        other = Path(d, "other")
        other.mkdir()
        objects(other)
        with CountNm() as n:
            U.rom_definitions(other, cache)
            assert len(n.asked) == 2   # the cache was for rom/


USES_C = """extern void called_fn(void);
extern int callback_fn(int);
extern const unsigned char tbl[];
int (*volatile fp)(int) = callback_fn;
int use(int i) { called_fn(); return tbl[i]; }
"""


def test_reloc_uses_calls_are_functions_and_other_references_addresses():
    with tempfile.TemporaryDirectory() as d:
        src, obj = Path(d, "uses.c"), Path(d, "uses.o")
        src.write_text(USES_C)
        subprocess.run(["cc", "-c", "-fPIC", "-O0", "-o", str(obj), str(src)], check=True)
        regular, thin, junk = Path(d, "regular.a"), Path(d, "thin.a"), Path(d, "junk.o")
        subprocess.run(["ar", "rcs", str(regular), str(obj)], check=True, cwd=d)
        subprocess.run(["ar", "rcsT", str(thin), "uses.o"], check=True, cwd=d)
        junk.write_bytes(b"not an object file at all")
        want = {"called_fn", "callback_fn", "tbl", "use"}
        for files in ([obj], [regular], [thin], [junk, obj, regular]):
            called, addressed = U.reloc_uses(files, want)
            assert (called, addressed) == ({"called_fn"}, {"callback_fn", "tbl"}), (files, called, addressed)
        assert U.reloc_uses([junk], want) == (set(), set())


def test_reloc_uses_aarch64_calls():
    try:
        with tempfile.TemporaryDirectory() as d:
            src, obj = Path(d, "uses.c"), Path(d, "uses.o")
            src.write_text(USES_C)
            subprocess.run(["clang", "--target=aarch64-linux-gnu", "-c", "-fPIC", "-O0", "-o", str(obj), str(src)],
                           check=True, capture_output=True)
            called, addressed = U.reloc_uses([obj], {"called_fn", "callback_fn", "tbl"})
    except (OSError, subprocess.CalledProcessError):
        print("skip (no clang for aarch64)", end=" ")
        return
    assert (called, addressed) == ({"called_fn"}, {"callback_fn", "tbl"})


def test_is_data_prefers_a_call_then_the_rom_kind_then_the_name_then_an_address():
    kinds = {"rom_data": ("D", 8), "rom_fn": ("T", 4), "called_but_data": ("D", 8)}
    called, addressed = {"called_but_data", "plain_called"}, {"rom_fn", "rom_data", "plain_addr", "sTable"}
    expect = {"called_but_data": False, "plain_called": False,   # a call: a function, whatever the ROM says
              "rom_fn": False, "rom_data": True,                 # the ROM's kind beats an address
              "sGeonetMenuTemplate": True, "gLookup": True,      # the name (Platinum's sFoo/gFoo style, hg's tables)
              "ov69_021E764C": False, "plain_addr": True,        # only an address taken: a datum
              "nothing_seen": False}                             # no relocation seen: a function, as before
    for sym, want in expect.items():
        assert U.is_data(sym, kinds, called, addressed) == want, sym


def test_write_stubs_makes_data_a_zeroed_block_and_functions_traps():
    with tempfile.TemporaryDirectory() as d:
        out = Path(d, "stubs.c")
        U.write_stubs(out, {"tbl", "called_fn", "rom_data"}, {"rom_data": ("D", 0x400)}, {"called_fn"}, {"tbl"})
        text = out.read_text()
        assert "char tbl[0x100]" in text and "char rom_data[0x400]" in text
        assert 'long called_fn(void) { static int said; return bwport_undefined("called_fn"' in text


if __name__ == "__main__":
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            fn()
            print("ok", name)
