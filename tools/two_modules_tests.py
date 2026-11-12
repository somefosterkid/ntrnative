#!/usr/bin/env python3
"""Tests for tools/two_modules.py: an alias's names, and the rewrite of dsd's linker script and object list on a
made-up script (a complete alias's lines name its file's object, its .bss in an LTD autoload the .ltdmain.bss).

    python3 tools/two_modules_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import two_modules as tm  # noqa: E402

LCF = chr(10).join([
    "    .arm9 : {", "        card_rom.o(.text)", "        card_rom.o(.bss)", "    } > ARM9",
    "    .ltd_autoload_0 : {", "        _dsd_gap@ltd_autoload_0_0.o(.ltdmain)", "        card_utility@ltd.o(.ltdmain)",
    "        card_rom@ltd.o(.ltdmain)", "        card_hash.o(.ltdmain)", "        card_rom@ltd.o(.bss)",
    "        card_hash.o(.bss)", "    } > LTD_AUTOLOAD_0", ""])
OBJECTS = ["build/b_us/lib/nitro/src/card/card_utility.o", "build/b_us/lib/nitro/src/card/card_rom.o",
           "build/b_us/delinks/_dsd_gap@ltd_autoload_0_0.o", "build/b_us/lib/nitro/src/card/card_rom@ltd.o",
           "build/b_us/delinks/lib/nitro/src/card/card_utility@ltd.o", "build/b_us/lib/twl/src/card/card_hash.o"]


def test_real_of():
    assert tm.real_of("lib/nitro/src/card/card_rom@ltd.c") == "lib/nitro/src/card/card_rom.c"
    assert tm.real_of("build/b_us/lib/x/a@ltd_autoload_0.o") == "build/b_us/lib/x/a.o"
    assert tm.real_of("lib/nitro/src/card/card_rom.c") is None
    assert tm.real_of("_dsd_gap@main_0") is None and tm.real_of("build/b_us/delinks/_dsd_gap@main_0.o") is None
    assert tm.compiled_alias("build/b_us/lib/nitro/src/card/card_rom@ltd.o")
    assert not tm.compiled_alias("build/b_us/delinks/lib/nitro/src/card/card_rom@ltd.o")  # incomplete: Black's bytes
    assert not tm.compiled_alias("build/b_us/lib/nitro/src/card/card_rom.o")


def test_a_complete_alias_names_its_files_object():
    lcf, objects = tm.rewrite(LCF, OBJECTS)
    lines = [l.strip() for l in lcf.splitlines()]
    assert "card_rom.o(.ltdmain)" in lines and "card_rom.o(.ltdmain.bss)" in lines, lcf
    assert "card_rom@ltd.o(.ltdmain)" not in lines and "card_rom@ltd.o(.bss)" not in lines
    assert lines.count("card_rom.o(.bss)") == 1  # main's, not the LTD block's
    assert "card_utility@ltd.o(.ltdmain)" in lines  # incomplete: its delinked object stays
    assert "_dsd_gap@ltd_autoload_0_0.o(.ltdmain)" in lines and "card_hash.o(.bss)" in lines
    assert objects == [o for o in OBJECTS if o != "build/b_us/lib/nitro/src/card/card_rom@ltd.o"], objects


def test_a_source_of_two_dots_is_named_as_dsd_lists_it():
    # crt0.FLX.c: dsd lcf wrote crt0.FLX.o(.text), its object list crt0.o (to the first dot): the link placed none
    lcf = chr(10).join(["    .arm9 : {", "        crt0.FLX.o(.text)", "        main.o(.text)",
                        "        os_init.FLX.o(.text)", "    } > ARM9", ""])
    objects = ["build/b_us/lib/nitro/src/init/crt0.o", "build/b_us/src/main.o", "build/b_us/lib/os_init.FLX.o"]
    out = tm.dotted(lcf, objects)
    assert "        crt0.o(.text)" in out and "crt0.FLX" not in out, out
    assert "        os_init.FLX.o(.text)" in out, out  # named as the list has it already
    assert tm.dotted(LCF, OBJECTS) == LCF


def test_nothing_to_do_without_complete_aliases():
    objects = [o for o in OBJECTS if o != "build/b_us/lib/nitro/src/card/card_rom@ltd.o"]
    assert tm.rewrite(LCF, objects) == (LCF, objects)


def test_an_alias_complete_before_its_file_stops():
    objects = [o for o in OBJECTS if o != "build/b_us/lib/nitro/src/card/card_rom.o"]
    try:
        tm.rewrite(LCF, objects)
    except SystemExit as e:
        assert "card_rom.o is not linked" in str(e), e
    else:
        raise AssertionError("no stop")


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
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
