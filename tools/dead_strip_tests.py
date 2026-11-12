#!/usr/bin/env python3
"""Tests for tools/dead_strip.py's rewrite of dsd's linker script.

    python3 tools/dead_strip_tests.py      # "ok" per test, exit 1 on a failure
"""
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dead_strip as D  # noqa: E402

LCF = chr(10).join([
    "    .ltd_autoload_0 : {",
    "        _dsd_gap@ltd_autoload_0_0.o(.ltdmain)",
    "        pm_twl.o(.ltdmain)",
    "        LTD_AUTOLOAD_0_BSS_START = .;",
    "        _dsd_gap@ltd_autoload_0_0.o(.bss)",
    "        pm_twl.o(.bss)",
    "        camera_api.o(.bss)",
    "    } > LTD_AUTOLOAD_0",
    "",
    "    .ov000 : {",
    "        pm_twl.o(.bss)",
    "    } > OV000",
    ""])


def test_ltdmain_bss_follows_the_bss_of_an_ltd_file():
    # TwlSDK's twl/ltdmain_begin.h sends uninitialised data to .ltdmain.bss, which dsd's script never placed (t34d):
    # an LTD file's goes after its .bss; a .text file's, a gap's and another module's lines stay as they were
    got = D.ltdmain_bss(LCF).splitlines()
    assert got[5:7] == ["        pm_twl.o(.bss)", "        pm_twl.o(.ltdmain.bss)"], got
    assert sum(".ltdmain.bss" in l for l in got) == 1, got


def block(name, region, *labels):
    return [f"    .{name} : {{", "        . = ALIGN(32);", *(f"        {l} = .;" for l in labels), f"    }} > {region}", ""]


def test_autoload_lists_are_the_sdks_lcf_symbols():
    # crt0.c's module parameters hold the SDK's lcf symbols for the autoload lists (t35c): each list follows its
    # module's autoload blocks, an entry of 16 bytes a block; overlays and the static module's own section aren't
    # blocks, and without the ARM9i module's end there are no LTD symbols
    lcf = chr(10).join(["SECTIONS {", *block("arm9", "ARM9", "ARM9_TEXT_START", "ARM9_BSS_START"),
                        *block("itcm", "ITCM", "ITCM_TEXT_START", "ITCM_TEXT_END", "ITCM_BSS_START"),
                        *block("dtcm", "DTCM", "DTCM_DATA_START", "DTCM_BSS_START"),
                        *block("ltd_autoload_0", "LTD_AUTOLOAD_0", "LTD_AUTOLOAD_0_TEXT_START",
                               "LTD_AUTOLOAD_0_LTDMAIN_START", "LTD_AUTOLOAD_0_BSS_START"),
                        *block("ov000", "OV000", "OV000_TEXT_START", "OV000_BSS_START"), "}"])
    got = D.autoload_symbols(lcf, 0x2400004)
    assert got == ["SDK_AUTOLOAD_LIST = ARM9_DATA_END + (ITCM_BSS_START - ITCM_TEXT_START)"
                   " + (DTCM_BSS_START - DTCM_DATA_START);",
                   "SDK_AUTOLOAD_LIST_END = SDK_AUTOLOAD_LIST + 0x20;",
                   "SDK_LTDAUTOLOAD_START = 0x2400004;",
                   "SDK_LTDAUTOLOAD_LIST = SDK_LTDAUTOLOAD_START + (LTD_AUTOLOAD_0_BSS_START"
                   " - LTD_AUTOLOAD_0_TEXT_START);",
                   "SDK_LTDAUTOLOAD_LIST_END = SDK_LTDAUTOLOAD_LIST + 0x10;"], got
    assert [l.split()[0] for l in D.autoload_symbols(lcf)] == ["SDK_AUTOLOAD_LIST", "SDK_AUTOLOAD_LIST_END"]


def test_ltd_blocks_start_after_the_arm9i_module():
    # Black's ARM9i module (0x02400000) is 4 bytes, so its LTD blocks start at 0x02400004 (BuildInfo's LTD twin)
    import tempfile
    here = os.getcwd()
    with tempfile.TemporaryDirectory() as d:
        os.makedirs(os.path.join(d, "extract", "b_us", "dsi"))
        os.makedirs(os.path.join(d, "build", "b_us"))
        open(os.path.join(d, "extract", "b_us", "dsi", "dsi.yaml"), "w").write(
            "arm9i:\n  base_address: 37748736\n  entry: 0\narm7i:\n  base_address: 48758784\n")
        open(os.path.join(d, "extract", "b_us", "dsi", "arm9i.bin"), "wb").write(bytes(4))
        try:
            os.chdir(d)
            assert D.ltd_start("build/b_us/arm9.lcf") == 0x2400004
            assert D.ltd_start("build/b_us2/arm9.lcf") is None
        finally:
            os.chdir(here)


R_SCR, A_SCR = ("data_ov016_021daa7c", 1, 0, 8, True, False), ("DWCi_A_SCR", 2, 0, 8, True, False)
FOUNDATION = dict(ranges={".rodata": (0x21daa7c, 0x21daa84)}, at={0x21daa7c: ["data_ov016_021daa7c"]},
                  black={"data_ov016_021daa7c", "func_ov016_021d5fe8"}, defined={"data_ov016_021daa7c", "DWCi_A_SCR"})
RODATA = [("", 0, 0, 0, 0), (".rodata", 1, 0, 8, 4), (".rodata", 1, 0, 8, 4)]


def test_a_global_black_lacks_is_dropped_after_or_before_blacks():
    # foundation.c's DWCi_A_SCR, which nothing uses (t37e): after DWCi_R_SCR it ends past the file's .rodata, before
    # it it starts where DWCi_R_SCR (Black's name) is; either way the link drops it, which keeps the rest in place
    assert D.foreign(RODATA, [R_SCR, A_SCR], **FOUNDATION) == {"DWCi_A_SCR"}
    assert D.foreign(RODATA, [A_SCR[:1] + (1,) + A_SCR[2:], R_SCR[:1] + (2,) + R_SCR[2:]], **FOUNDATION) == \
        {"DWCi_A_SCR"}


def test_blacks_data_under_our_name_is_kept():
    # a renamed Black global in the range (IRC_RHYTHM_CHECK_UNUSED_400, used or not) is Black's; so are a static and
    # a function, whatever their names
    unused = ("IRC_RHYTHM_CHECK_UNUSED_400", 1, 0, 4, True, False)
    kw = dict(FOUNDATION, defined={"IRC_RHYTHM_CHECK_UNUSED_400"})
    assert D.foreign(RODATA, [unused], **kw) == set()
    assert D.foreign(RODATA[:2], [("sLocal", 1, 0, 12, False, False)], **kw) == set()
    assert D.foreign([("", 0, 0, 0, 0), (".text", 1, 0, 12, 4)], [("f", 1, 0, 12, True, True)], **kw) == set()


def test_ltdmain_bss_is_not_judged():
    # card_rom.c: its .ltdmain.bss (read_object() calls it .bss) goes to the LTD module; main's .bss range holds the
    # rest, which fits only without it
    secs = [("", 0, 0, 0, 0), (".bss", 8, 0, 4, 4), (".bss", 8, 0, 4, 4)]
    syms = [("CARDiHashBufferAddress", 1, 0, 4, True, False), ("cardi_rom_base", 2, 0, 4, True, False)]
    kw = dict(ranges={".bss": (0x2150840, 0x2150844)}, at={}, black=set(), defined=set())
    assert D.foreign(secs, syms, **kw) == {"cardi_rom_base"}
    assert D.foreign(secs, syms, ltd={1}, **kw) == set()


def elf(*names):
    """A relocatable ELF's header and section headers, named names after the null section and .shstrtab."""
    strtab = b"\0.shstrtab\0" + b"".join(n.encode() + b"\0" for n in names)
    offs = [0, 1] + [strtab.index(n.encode() + b"\0", 11) for n in names]
    sh = b"".join(struct.pack("<10I", o, 3 if i == 1 else 8, 0, 0, 0x34 if i == 1 else 0, len(strtab) if i == 1
                              else 4, 0, 0, 4, 0) for i, o in enumerate(offs))
    head = bytearray(0x34)
    struct.pack_into("<I", head, 0x20, 0x34 + len(strtab))
    struct.pack_into("<HHH", head, 0x2E, 40, len(offs), 1)
    return bytes(head) + strtab + sh


def test_ltdmain_bss_sections_by_their_real_names():
    assert D.ltdmain_bss_sections(elf(".bss", ".ltdmain.bss", ".bss", ".ltdmain.bss")) == {3, 5}
    assert D.ltdmain_bss_sections(elf(".bss", ".ltdmain")) == set()


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
