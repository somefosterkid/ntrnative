#!/usr/bin/env python3
"""Tests for tools/fit_data.py: its helpers on made-up inputs (the link's value of an absolute word, the size a .bss
range's last dsd symbol gets, the inner_symbols.py hook), then fit() run dry on complete files of the tree, whose
delinks.txt ranges it must give back: a static initializer's .ctor (dsprot), files of data only, OVERLAY_ID(n)
words Black has no relocation for, .rodata/.data that only dropped code reaches, .bss ends.

    python3 tools/fit_data_tests.py        # prints "ok" per test, exits 1 on a failure

The tree's tests need the build (build/b_us); they change nothing.
"""
import os
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fit_data as fd  # noqa: E402

_ranges = []


def ranges():
    if not _ranges:
        _ranges.append(fd.Ranges(os.path.expanduser("~/ref/pokebw2"), fd.CONFIG, f"{fd.ROOT}/build/twins_b_us.tsv"))
    return _ranges[0]


def test_link_value_of_overlay_ids_placed_symbols_and_names():
    where = {"data_ov106_021b9490": [("ov106", 0x021b9490)], "twice": [("ov001", 0x100), ("ov002", 0x200)],
             "own": [("ov001", 0x100), ("ov002", 0x200)]}
    assert fd.link_value("OVERLAY_124_ID", None, 0, where, "ov021") == 124
    assert fd.link_value("OVERLAY_3_ID", None, 4, where, "ov021") == 7
    assert fd.link_value("func_x", 0x021b6dac, 1, where, "ov101") == 0x021b6dad  # a placed Thumb function
    assert fd.link_value("data_ov106_021b9490", None, 0, where, "ov101") == 0x021b9490  # another overlay's, one place
    assert fd.link_value("twice", None, 0, where, "ov021") is None  # two overlays' at two addresses
    assert fd.link_value("own", None, 0, where, "ov002") == 0x200  # the module's own first
    assert fd.link_value("nothing", None, 0, where, "ov021") is None


def test_word_reads_little_endian_and_none_past_the_bytes():
    rom = bytes.fromhex("7c000000 05f6ffff")
    assert fd.word(rom, 0x100, 0x100) == 124
    assert fd.word(rom, 0x100, 0x104) == 0xfffff605
    assert fd.word(rom, 0x100, 0x106) is None and fd.word(rom, 0x100, 0xfc) is None


class FakeRanges:
    end_of = fd.Ranges.end_of

    def __init__(self, syms):
        self.syms = syms


def test_a_bss_ranges_last_symbol_gets_its_size_where_the_range_ends():
    # nwm.c: the object ends 4 bytes before fatfs_api.c's 32-aligned .bss, dsd's data_027341cc runs to it
    real = fd.CONFIG
    with tempfile.TemporaryDirectory() as d:
        os.makedirs(f"{d}/ltd_autoload_0")
        path = f"{d}/ltd_autoload_0/symbols.txt"
        open(path, "w").write("data_027341c8 kind:bss addr:0x027341c8\ndata_027341cc kind:bss addr:0x027341cc\n"
                              "data_027343e0 kind:bss addr:0x027343e0\n"
                              "data_02734400 kind:bss(size=0x8) addr:0x02734400\n")
        fd.CONFIG = d
        try:
            r = FakeRanges({"ltd_autoload_0": [0x027341c8, 0x027341cc, 0x027343e0, 0x02734400, 0x02734410]})
            sizes = fd.sized(r, "ltd_autoload_0", {".bss": (0x027341c8, 0x027343dc)})
            assert sizes == [(0x027341cc, 0x210)], sizes
            before = fd.give_sizes("ltd_autoload_0", sizes)
            assert list(before) == [path]
            text = open(path).read()
            assert "data_027341cc kind:bss(size=0x210) addr:0x027341cc\n" in text, text
            assert "data_027341c8 kind:bss addr:0x027341c8\n" in text
            assert fd.symbol_sizes("ltd_autoload_0") == {0x027341cc: 0x210, 0x02734400: 0x8}
            # sized already, a range up to the next symbol, no .bss: nothing
            assert fd.sized(r, "ltd_autoload_0", {".bss": (0x027341c8, 0x027343dc)}) == []
            assert fd.sized(r, "ltd_autoload_0", {".bss": (0x027341c8, 0x027343e0)}) == []
            assert fd.sized(r, "ltd_autoload_0", {".bss": (0x02734400, 0x02734408)}) == []
            assert fd.sized(r, "ltd_autoload_0", {".data": (0x027341c8, 0x027343dc)}) == []
        finally:
            fd.CONFIG = real


def test_the_inner_symbols_hook_gives_back_what_it_wrote():
    real_config, real_run = fd.CONFIG, fd.run
    with tempfile.TemporaryDirectory() as d:
        os.makedirs(f"{d}/overlays/ov021")
        sym, rel = f"{d}/overlays/ov021/symbols.txt", f"{d}/overlays/ov021/relocs.txt"
        open(sym, "w").write("g_Table kind:data(any) addr:0x021db4fc\n"
                             "data_ov021_021db508 kind:data(any) addr:0x021db508\n")
        open(rel, "w").write("")
        calls = []

        def run(cmd):
            calls.append(cmd)
            open(sym, "w").write("g_Table kind:data(any) addr:0x021db4fc\n")
            return 0, ("1 symbols that go\n  ov021 data_ov021_021db508 at 0x021db508 goes: inside g_Table+0xc\n"
                       "wrote 1 files\n")
        fd.CONFIG, fd.run = d, run
        try:
            before = fd.inner_symbols("src/ov021/enceff_data.c")
        finally:
            fd.CONFIG, fd.run = real_config, real_run
        assert calls and calls[0][1:] == ["tools/inner_symbols.py", "--apply", "--placed", "src/ov021/enceff_data.c",
                                        "src/ov021/enceff_data.c"], calls
        assert before == {sym: "g_Table kind:data(any) addr:0x021db4fc\n"
                                "data_ov021_021db508 kind:data(any) addr:0x021db508\n"}, before


def fitted(dest):
    """fit() of a file of the tree, dry: (its ranges, the new ones), after checking nothing stops it."""
    r = ranges()
    mod = next(m for m, (_, files) in r.delinks.items() if files.get(dest))
    old = r.delinks[mod][1][dest]
    found, problems, renames, drops, fixes = fd.fit(r, mod, dest)
    assert not problems and not drops, (dest, problems, drops)
    assert not fd.holes(r, mod, dest, old, found), (dest, fd.holes(r, mod, dest, old, found))
    return old, found


def test_tree_a_static_initializers_ctor_is_placed():
    # was: "the new ranges leave .ctor 0x02189980-0x02189984 (4 bytes) in no file"
    for dest, ctor in (("lib/dsprot/src/dummy.c", (0x02189980, 0x02189984)),
                       ("lib/dsprot/src/rc4_ov230.c", (0x02189984, 0x02189988))):
        old, found = fitted(dest)
        assert found == old and found[".ctor"] == ctor, (dest, found)


def test_tree_files_of_data_only():
    # was: "no .text in a delinks.txt"; enceff_data.c's table has OVERLAY_ID(n) words Black has no relocation for
    for dest in ("src/ov021/enceff_data.c", "src/ov021/fldmmdl_movedata.c", "lib/dsprot/src/extra.c",
                 "lib/msl/src/ctype.c"):
        old, found = fitted(dest)
        assert found == old and ".text" not in found, (dest, found, old)


def test_tree_words_black_has_no_relocation_for():
    # OVERLAY_ID(n) in proc tables, and irc_compatible.c's pointers to other overlays' procs: "a relocation differs"
    for dest in ("src/ov010/event_field_proclink.c", "src/ov101/irc_compatible.c", "src/ov186/br_core.c",
                 "src/ov111/badge_view.c"):
        old, found = fitted(dest)
        assert found == old, (dest, found, old)


def test_tree_data_only_dropped_code_reaches_goes():
    # mb_wm_base.c's bss_fill of MBi_ScanUnlock (only the dropped MB_StartChild calls it): "no relocation reaches
    # it, and its bytes are at 0 places"; dwc_mov_wh.c's .data of the dropped ChildConnectAuto chain
    for dest in ("lib/nitro/src/mb/mb_wm_base.c", "lib/dwcutil/src/dwc_mov_wh.c", "lib/nnsys/src/snd/sndarc_stream.c"):
        old, found = fitted(dest)
        assert found == old, (dest, found, old)


def test_tree_bss_ends():
    # a sized dsd symbol ends the range (was stretched to the module's end); to Black's last symbol when no other
    # file's .bss follows (vct.c's 0x021b60f5); before fatfs_api.c's .bss, the object's end, not over the padding
    for dest in ("src/ov010/building_enter_effect.c", "src/ov213/comm_btl_demo.c", "lib/dwc/src/vct/vct.c",
                 "src/ov132/sta_acting.c"):
        old, found = fitted(dest)
        assert found == old, (dest, found, old)
    old, found = fitted("lib/twl/src/nwm/nwm.c")
    assert found[".bss"] == (0x027341c8, 0x027343dc), found


def test_tree_ltdmain_files_are_fitted():
    # a TwlSDK LTD file's code is in .ltdmain (t35c's code_section): fitted as a .text file, its .ltdmain.bss as its
    # .bss (card_hash.c); was "its entry has a .ltdmain range, which this doesn't place". A two-module file's LTD
    # alias too (t37c merged os_arena_twl.c into os_arena.c: os_arena@ltd.c; card_rom@ltd.c's .ltdmain.bss)
    for dest in ("lib/nitro/src/os/os_arena@ltd.c", "lib/nitro/src/card/card_rom@ltd.c", "lib/twl/src/card/card_hash.c"):
        old, found = fitted(dest)
        assert found == old, (dest, found, old)


def test_tree_a_section_only_other_files_reach():
    # ghttpCommon.c's .bss is the GameSpy proxy's globals, which only other files' code loads: Black's addresses of
    # their names place it (was ".bss: no relocation of the file's reaches it"; t35q wrote its range by hand)
    old, found = fitted("lib/dwc/src/ghttp/ghttpCommon.c")
    assert found == old and found[".bss"] == (0x021d3b28, 0x021d3b30), (found, old)


def test_tree_unreached_statics_weak_globals_and_the_modules_padding():
    # a function's static the compiler folded away, which only the debug info reaches, the link drops (was "drops
    # ['_ZZ7GX_InitvE15bg_mtx_elem_one']": dropping it from the C); a weak definition is a global (was "STD_TSPrintf
    # ... is the file's static"); the last file's .data runs over the module section's padding (ir_check.c, was
    # .data:0x2180a00-0x2180a04), not over a label code reaches (fs_overlay_digest.c's SDK_OVERLAY_DIGEST_END)
    for dest in ("lib/nitro/src/gx/gx.c", "lib/nnsys/src/g3d/anm/nsbma.c", "lib/twl/src/mi/mi_ndma.c",
                 "lib/nitro/src/std/std_sprintf.c", "src/ov231/ir_check.c", "lib/nitro/src/fs/fs_overlay_digest.c"):
        old, found = fitted(dest)
        assert found == old, (dest, found, old)


def test_tree_a_file_in_two_modules():
    # card_rom.c's main part and its alias card_rom@ltd.c's LTD block are one object (tools/two_modules.py): each
    # module fits its own sections (the .ltdmain.bss went with main's .bss: "the relocations disagree on its start"),
    # and main reaching card_utility.c's LTD static CARDiDmaUsingNew is the file's own reference (was "... which main
    # reaches from 0x2076340, is the file's static CARDiDmaUsingNew")
    for dest in ("lib/nitro/src/card/card_rom.c", "lib/nitro/src/card/card_rom@ltd.c",
                 "lib/nitro/src/card/card_utility.c", "lib/nitro/src/card/card_utility@ltd.c"):
        old, found = fitted(dest)
        assert found == old, (dest, found, old)


def main():
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            try:
                fn()
                print("ok", name)
            except Exception as e:  # noqa: BLE001
                bad += 1
                print("FAIL", name, type(e).__name__, e)
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
