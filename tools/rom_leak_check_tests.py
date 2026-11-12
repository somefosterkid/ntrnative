#!/usr/bin/env python3
"""Tests for rom_leak_check.py on a made-up ROM (random bytes, no game data):

    python3 tools/rom_leak_check_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os
import random
import struct
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rom_leak_check as R  # noqa: E402

rnd = random.Random(7)
ROM = bytes(rnd.randrange(256) for _ in range(20000))
OTHER = bytes(rnd.randrange(256) for _ in range(5000))


def lz10(data):
    """Literal-only LZ77 (type 0x10) stream: enough to test the decompressor and the cache."""
    out = bytearray([0x10]) + len(data).to_bytes(3, "little")
    for i in range(0, len(data), 8):
        out.append(0)
        out += data[i:i + 8]
    return bytes(out)


def setup(tmp):
    ext = os.path.join(tmp, "ext")
    os.makedirs(os.path.join(ext, "files"))
    os.makedirs(os.path.join(ext, "arm9"))
    open(os.path.join(ext, "arm9", "arm9.bin"), "wb").write(ROM)
    hidden = bytes(rnd.randrange(256) for _ in range(3000))  # only inside an LZ77 file
    open(os.path.join(ext, "files", "a.bin"), "wb").write(lz10(hidden))
    rom = os.path.join(tmp, "rom.nds")
    open(rom, "wb").write(ROM)
    roms = [("t", rom, ext)]
    cache = os.path.join(tmp, "cache")
    R.build_cache(roms, cache)
    R.build_index(roms, cache)
    return hidden


def scan(data):
    return {k: n for _, k, _, n, *_ in R.scan_data("x", data)}


def test_all():
    with tempfile.TemporaryDirectory() as tmp:
        hidden = setup(tmp)
        assert R.lz_decompress(lz10(hidden)) == hidden
        # a raw copy of 100 ROM bytes, anywhere in a file
        assert scan(b"junk" * 50 + ROM[1234:1334] + b"tail").get("raw", 0) >= 100
        # 40 bytes is under the limit
        assert not scan(OTHER + ROM[500:540] + OTHER)
        # the LZ77-compressed file's content, found through the cache
        assert scan(hidden[100:300]).get("raw", 0) >= 200
        # a C array of u16 values written as text, and as hex u8
        vals = struct.unpack("<60H", ROM[2000:2120])
        c16 = "static const u16 t[] = {\n" + ",\n".join(str(v) for v in vals) + "};\n"
        assert scan(c16.encode()).get("u16", 0) >= 100
        c8 = "u8 t[] = {" + ", ".join(f"0x{b:02X}" for b in ROM[3000:3080]) + "};"
        assert scan(c8.encode()).get("u8", 0) >= 70
        c32 = "u32 t[] = {" + ", ".join(str(v) for v in struct.unpack("<20I", ROM[4000:4080])) + "};"
        assert scan(c32.encode()).get("u32", 0) >= 70
        # trivial: one byte repeated, a counter
        assert not scan(bytes([ROM[0]]) * 200)
        assert not scan(("int t[] = {" + ", ".join(str(i) for i in range(300)) + "};").encode())
    print("ok scan")


def test_modules():
    """A ROM without a dsd extract: its overlays' decompressed images (tools/rom_modules.py) are searched too."""
    import rom_modules
    packed = bytes(rnd.randrange(256) for _ in range(2000))  # only in a "compressed" overlay
    old = rom_modules.arm9, rom_modules.overlays
    rom_modules.arm9, rom_modules.overlays = (lambda rom: rom[:100]), (lambda rom: iter([(7, packed)]))
    try:
        with tempfile.TemporaryDirectory() as tmp:
            rom = os.path.join(tmp, "rom.nds")
            open(rom, "wb").write(ROM)
            cache = os.path.join(tmp, "cache")
            R.build_cache([("t", rom, None)], cache)
            R.build_index([("t", rom, None)], cache)
            found = R.scan_data("x", b"junk" * 20 + packed[300:500])
            assert found and found[0][3] >= 200 and "t:ov007" in found[0][5], found
    finally:
        rom_modules.arm9, rom_modules.overlays = old
    print("ok modules")


def test_static():
    with tempfile.TemporaryDirectory() as tmp:
        old = R.REPO
        R.REPO = tmp
        try:
            open(os.path.join(tmp, "a.png"), "wb").write(b"\x89PNG\0\0")
            open(os.path.join(tmp, "b.c"), "w").write("u8 t[] = {" + ", ".join(str(i % 7) for i in range(500)) + "};")
            open(os.path.join(tmp, "c.c"), "w").write("int main(void) { return 0; }\n" * 30)
            got = {r[0] for r in R.static_check(["a.png", "b.c", "c.c"])}
            assert got == {"a.png", "b.c"}, got
        finally:
            R.REPO = old
    print("ok static")


def test_result_cache():
    """scan_cached: the cached run gives the findings of an uncached one, a changed file is rescanned, the rest reused."""
    with tempfile.TemporaryDirectory() as tmp:
        hidden = setup(tmp)
        roms = [("t", os.path.join(tmp, "rom.nds"), os.path.join(tmp, "ext"))]
        cache = os.path.join(tmp, "cache")
        old = R.REPO
        R.REPO = os.path.join(tmp, "repo")
        os.makedirs(R.REPO)
        try:
            def put(n, d):
                open(os.path.join(R.REPO, n), "wb").write(d)
            put("a.bin", ROM[1234:1434])
            put("b.bin", OTHER)
            put("c.bin", hidden[10:300])
            paths = ["a.bin", "b.bin", "c.bin", "gone.bin"]
            key = sorted
            full = R.scan_cached(paths, roms, cache, 2, use_cache=False)
            assert {r[0] for r in full} == {"a.bin", "c.bin"}, full
            assert os.path.exists(os.path.join(cache, "scan_results.pkl"))
            assert key(R.scan_cached(paths, roms, cache, 2)) == key(full)
            put("b.bin", ROM[500:700])  # now a leak
            got = R.scan_cached(paths, roms, cache, 2)
            assert {r[0] for r in got} == {"a.bin", "b.bin", "c.bin"}, got
            assert key(got) == key(R.scan_cached(paths, roms, cache, 2, use_cache=False))
            R.scan_cached(paths[:1], roms, cache, 2, full=False)  # a partial run keeps the others
            assert key(R.scan_cached(paths, roms, cache, 2)) == key(got)
        finally:
            R.REPO = old
    print("ok result cache")


def test_where():
    """--where: a run of the number stream maps back to the lines (and the table) its literals came from."""
    with tempfile.TemporaryDirectory() as tmp:
        setup(tmp)
        vals = struct.unpack("<60H", ROM[2000:2120])
        src = ("#define N 3" + chr(10) + "static const u16 sSmall[] = { 1, 2, 3 };" + chr(10) + "static const u16 sBig[] = {" + chr(10)
               + chr(10).join(f"    {v}," for v in vals) + chr(10) + "};" + chr(10) + "int f(void) { return 5; }" + chr(10))
        found = R.where_data(src.encode())
        kinds = {k: runs for k, runs in found}
        a, b, n, tables = kinds["u16"][0]
        assert (a, b, n, tables) == (4, 63, 120, ["sBig"]), kinds["u16"]
        assert R.where_lines("f.c", found)[0].startswith("WHERE f.c [u16] 120B: L4-63 sBig 120B")
        # raw bytes: the line of the first and last byte
        raw = R.where_data(b"x" + chr(10).encode() * 3 + ROM[1234:1334] + chr(10).encode() + b"tail")
        assert raw and raw[0][0] == "raw" and raw[0][1][0][0] == 4, raw
        # the digits of a string literal are text, not numbers (gfl_use's archive paths "a/0/0/1")
        assert [v for _, v in R.numbers(b'x = 3; p = "a/0/1/2", "\\"7"; y = 4; q = "b"; z = 5;')] == [3, R.STR_BREAK, 4, 5]
        # an aligned table is a table, not code (worldtrade_deposit's countries)
        assert [n for _, _, n in R.initializers("static const u16 sA[] ATTRIBUTE_ALIGN(4) = { 1, 2 };")] == ["sA"]
    print("ok where")


def test_file_args():
    """--files: a folder is its tracked files; a path that names nothing is reported, not silently scanned as empty."""
    files, bad = R.file_args(["tools/rom_leak_check.py", "tools/", "src/no_such_file.c"])
    assert "tools/rom_leak_check.py" in files and "tools/rom_leak_check_tests.py" in files and files.count("tools/rom_leak_check.py") == 1
    assert bad == ["src/no_such_file.c"], bad
    old = sys.argv
    sys.argv = ["rom_leak_check", "--static", "--files", "src/no_such_file.c"]
    try:
        assert R.main() == 2
    finally:
        sys.argv = old
    print("ok file_args")


if __name__ == "__main__":
    test_file_args()
    test_all()
    test_static()
    test_modules()
    test_result_cache()
    test_where()
