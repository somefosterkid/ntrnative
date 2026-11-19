#!/usr/bin/env python3
"""Tests for tools/port_overlays.py's Platinum part (no ROM, no build): pret's linker spec (main.lsf.in) read, and checked
against a made-up ROM's overlay table; the overlay markers of a relinked game module (check_markers, port_relink.py).

    python3 tools/port_overlays_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import struct
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import port_overlays as P  # noqa: E402

SPEC = """\
Static main
{
\tAddress 0x02000000
\tLibrary libos.a (.version)
\tObject src/main.c
\tLibrary libspl.a
}

Autoload ITCM
{
\tAddress 0x01FF8000
}

Overlay first
{
\tAfter main
\tObject src/a/one.c
\tObject trainer_ai_script.o
}

Overlay second
{
\tAfter first
\tObject src/b/two.c
\tLibrary\tlibgds.a
\tLibrary libdwcauth.a
}

Overlay other
{
\tAfter main
}

Overlay dummy
{
\tAfter ITCM
}
"""


def tree(d):
    for f in ["platinum.us/main.lsf.in", "lib/spl/src/spl_b.c", "lib/spl/src/spl_a.c", "lib/gds/src/g.c"]:
        (d / f).parent.mkdir(parents=True, exist_ok=True)
        (d / f).write_text(SPEC if f.endswith(".in") else "")


def rom(path, table, main_end=0x02100000):
    """A ROM with an ARM9 holding module params (static bss end: main_end) and an overlay table of (id, ram, size, bss)."""
    data = bytearray(0x400)
    image = bytearray(0x40)
    struct.pack_into("<I", image, 0x14 + 0x10, main_end)
    struct.pack_into("<II", image, 0x30, 0xDEC00621, 0x2106C0DE)
    data[0x200:0x240] = image
    struct.pack_into("<4I", data, 0x20, 0x200, 0, 0x02000000, len(image))
    struct.pack_into("<2I", data, 0x50, 0x300, 32 * len(table))
    for i, t in enumerate(table):
        struct.pack_into("<4I", data, 0x300 + 32 * i, *t)
    path.write_bytes(bytes(data))


GOOD = [(0, 0x02100000, 0x100, 0x20), (1, 0x02100120, 0x40, 0), (2, 0x02100000, 0x20, 0), (3, 0x01ff8660, 0x20, 0)]


def test_spec_sections_objects_and_pret_libraries():
    with tempfile.TemporaryDirectory() as d:
        d = Path(d)
        tree(d)
        got = P.lsf(SPEC, d)
    assert [s[:3] for s in got] == [["Static", "main", None], ["Autoload", "ITCM", None], ["Overlay", "first", "main"],
                                    ["Overlay", "second", "first"], ["Overlay", "other", "main"],
                                    ["Overlay", "dummy", "ITCM"]], got
    assert got[0][3] == ["src/main.c", "lib/spl/src/spl_a.c", "lib/spl/src/spl_b.c"], got[0]   # (not the SDK's libos.a)
    assert got[2][3] == ["src/a/one.c"], got[2]   # (no C for the AI script's object)
    assert got[3][3] == ["src/b/two.c", "lib/gds/src/g.c"], got[3]   # (the online library is the port's)


def test_layout_from_the_rom_checked_against_the_spec():
    with tempfile.TemporaryDirectory() as d:
        d = Path(d)
        tree(d)
        rom(d / "rom.nds", GOOD)
        ovs, main = P.plat_overlays(d / "rom.nds", d)
    assert main == ["src/main.c", "lib/spl/src/spl_a.c", "lib/spl/src/spl_b.c"], main
    assert [o[:4] for o in ovs] == GOOD, ovs
    assert ovs[1][4] == ["src/b/two.c", "lib/gds/src/g.c"], ovs


def test_an_overlay_not_where_its_after_puts_it_stops():
    with tempfile.TemporaryDirectory() as d:
        d = Path(d)
        tree(d)
        for bad in [[GOOD[0], (1, 0x02100100, 0x40, 0)] + GOOD[2:],    # second: after first's image, not its bss
                    [(0, 0x02100040, 0x100, 0x20)] + GOOD[1:],         # first: not at main's end
                    GOOD[:3]]:                                         # an overlay fewer than the spec's
            rom(d / "rom.nds", bad)
            try:
                P.plat_overlays(d / "rom.nds", d)
            except SystemExit:
                continue
            raise AssertionError(f"no stop for {bad}")


def sysv(rows):
    """`nm -f sysv` lines: (name, value, section)."""
    return "\n".join(f"{n:<20}|{v:016x}|   D  |              None|                |     |{s}" for n, v, s in rows)


def markers(n, b_sec=".hgovlbss", prefix="hg_"):
    rows = []
    for i in range(n + 1):
        rows += [(f"{prefix}bwovl_d{i:03d}", 0x40 * i, ".hgovldata"), (f"{prefix}bwovl_b{i:03d}", 0x20 * i, b_sec)]
    return rows


def test_markers_in_their_sections_in_order_pass():
    assert P.check_markers(sysv(markers(3)), "hg") == []
    assert P.check_markers(sysv(markers(3, prefix="")), "hg") == []   # (ELF: no tag)
    assert P.check_markers(sysv(markers(3) + [("sIntroTable", 0x48, ".hgovldata"), ("ovlmark001.c", 0, "")]), "hg") == []


def test_bss_markers_among_the_data_fail():
    # clang 21 on COFF: the bss marker went to the data section, right after the data marker
    rows = [(n, v if s == ".hgovldata" else v * 2 + 0x10, ".hgovldata") for n, v, s in markers(3)]
    bad = P.check_markers(sysv(rows), "hg")
    assert len(bad) == 1 and "4 of 4 bwovl_bNNN markers are not in .hgovlbss" in bad[0], bad


def test_markers_out_of_order_missing_or_unpaired_fail():
    rows = markers(3)
    rows[2] = ("hg_bwovl_d001", 0x100, ".hgovldata")   # d001 after d002 and d003
    assert any("not in overlay order" in b for b in P.check_markers(sysv(rows), "hg"))
    assert P.check_markers("", "hg") == ["no bwovl_dNNN markers", "no bwovl_bNNN markers"]
    rows = [r for r in markers(3) if r[0] != "hg_bwovl_b003"]
    assert any("4 data markers but 3 bss markers" in b for b in P.check_markers(sysv(rows), "hg"))
    rows = [r for r in markers(3) if r[0] != "hg_bwovl_d001"]
    assert any("not numbered" in b for b in P.check_markers(sysv(rows), "hg"))


def test_pe_markers_take_only_the_ms_pragmas():
    with tempfile.TemporaryDirectory() as d:
        P.macho_files([(0, 0x02100000, 0x100, 0x20, ["a.c"])], d, "hg")
        text = (Path(d) / "ovlmark001.c").read_text()
        win, elf = text.split("#ifdef _WIN32\n")[1].split("#else\n")
        assert win == '#pragma data_seg(".hgovldata")\n#pragma bss_seg(".hgovlbss")\n', win
        assert elf.startswith('#pragma clang section bss=".hgovlbss" data=".hgovldata"\n#endif\n'), elf
        assert "unsigned char bwovl_b001[16];" in text, text


def main():
    failed = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_") and callable(fn):
            try:
                fn()
                print("ok", name)
            except Exception as e:  # noqa: BLE001
                import traceback
                traceback.print_exc()
                print("FAIL", name, type(e).__name__, e)
                failed += 1
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
