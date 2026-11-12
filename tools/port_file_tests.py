#!/usr/bin/env python3
"""Tests for tools/port_file.py: definitions that trade places take their comments along (md5.c's tables kept each
other's, s816).

    python3 tools/port_file_tests.py      # "ok" per test, exit 1 on a failure
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import port_file as P  # noqa: E402

MD5 = """#include "ssl_internal.h"

// Which of the 16 message words each of the 64 steps mixes in
// pokebw2's sMD5WordIndex
static u8 data_ov009_021a1da8[64] = {
    0, 1, 2, 3,
};

// The 64 additive constants
static u32 data_ov009_021a1e28[64] = {
    0xd76aa478,
};

// The 64-byte padding CPSi_md5_result appends to a message before its length
// pokebw2's sMD5Padding
static u8 data_ov009_021a1de8[64] = {
    0x80,
};

static void f(void) {
}
"""


def elf(names):
    """A relocatable ELF of empty sections of these names (a section-name table last), for parse_object()."""
    import struct
    nul = bytes(1)
    strtab = nul + b"".join(n.encode() + nul for n in names) + b".shstrtab" + nul
    offs, at = [], 1
    for n in names + [".shstrtab"]:
        offs.append(at)
        at += len(n) + 1
    shoff = 0x34 + len(strtab) + (-len(strtab) % 4)
    head = bytearray(0x34)
    head[:4] = bytes([0x7F]) + b"ELF"
    struct.pack_into("<I", head, 0x20, shoff)
    struct.pack_into("<HHH", head, 0x2E, 40, len(names) + 2, len(names) + 1)
    sh = bytes(40) + b"".join(struct.pack("<10I", o, 8, 0, 0, 0x34, 0, 0, 0, 4, 0) for o in offs[:-1])
    sh += struct.pack("<10I", offs[-1], 3, 0, 0, 0x34, len(strtab), 0, 0, 1, 0)
    return bytes(head) + strtab + bytes(-len(strtab) % 4) + sh


def test_ltdmain_bss_is_the_files_bss():
    # TwlSDK's twl/ltdmain_begin.h sends uninitialised data to .ltdmain.bss: the file's .bss range holds it (t35c)
    _, secs, _, _ = P.parse_object(elf([".ltdmain", ".bss", ".ltdmain.bss"]))
    assert [s[0] for s in secs[1:4]] == [".ltdmain", ".bss", ".bss"], secs


def test_code_section_of_an_ltdmain_file():
    # an LTD file's code is in .ltdmain, judged and ported as a .text file's (t34d, t34e: brief.py and port_file.py
    # refused os_arena_twl.c, KeyError '.text')
    assert P.code_section({".ltdmain": (1, 2), ".bss": (3, 4)}) == ".ltdmain"
    assert P.code_section({".text": (1, 2), ".ltdmain": (3, 4)}) == ".text"
    assert P.code_section({".data": (1, 2)}) is None


def test_module_bytes_of_a_module_without_sections_is_a_key_error():
    # fit_data.holes() reads the ROM if it can and goes on without it on OSError/KeyError: module_bytes' base became
    # min() over the module's sections, and a module without any raised ValueError (complete_check's holes test, t37a)
    import types
    r = types.SimpleNamespace(delinks={"ov009": ({}, {})})
    try:
        P.module_bytes(r, "ov009")
    except KeyError:
        pass
    else:
        raise AssertionError("no KeyError")


def test_swap_takes_comments():
    out = P.swap_definitions(MD5, {"data_ov009_021a1da8": "data_ov009_021a1de8",
                                   "data_ov009_021a1de8": "data_ov009_021a1da8"})
    first, rest = out.split("// The 64 additive constants")
    assert "// The 64-byte padding" in first and "Which of the 16" not in first, first
    assert first.index("// The 64-byte padding") < first.index("// pokebw2's sMD5Padding") < \
        first.index("data_ov009_021a1de8[64]"), first
    assert "Which of the 16 message words" in rest and rest.index("Which of the 16") < \
        rest.index("// pokebw2's sMD5WordIndex") < rest.index("data_ov009_021a1da8[64]"), rest
    assert sorted(out.splitlines()) == sorted(MD5.splitlines())  # nothing lost, nothing doubled


def test_span_stops_at_blank_line_and_code():
    text = "int a = 1;\n// about b\nint b = 2;\n\n// not c's: a blank line\n\nint c = 3;\n"
    s, e = P.definition_span(text, "b")
    assert text[s:e] == "// about b\nint b = 2;", text[s:e]
    s, e = P.definition_span(text, "c")
    assert text[s:e] == "int c = 3;", text[s:e]
    s, e = P.definition_span(text, "a")
    assert text[s:e] == "int a = 1;"


def test_span_block_comment():
    text = "int x;\n/* the table\n   of b */\nint b[2] = {1, 2};\n/* one line */\nint c = 3;\n"
    s, e = P.definition_span(text, "b")
    assert text[s:e].startswith("/* the table"), text[s:e]
    s, e = P.definition_span(text, "c")
    assert text[s:e] == "/* one line */\nint c = 3;", text[s:e]
    assert P.definition_span(text, "x") is None  # no initializer


def test_a_complete_files_own_name_is_no_name_black_lacks():
    # t39n: mailsys.c, complete, defines MAILSYS_PROC_FUNCTIONS where Black's symbols.txt has data_ov136_021c7768;
    # check() took it for a name Black doesn't have and renamed it everywhere (mailsys.h, mailsys.c, the caller)
    import tempfile
    import types
    listing = "src/ov136/mailsys.c:\n    complete\n    .rodata     start:0x021c7750 end:0x021c77c4\n"
    secs = [("", 0, 0, 0, 0), (".rodata", 1, 0, 0x74, 4)]
    syms = [("MAILSYS_PROC_FUNCTIONS", 1, 0x18, 12, True, False), ("sLocal", 1, 0x24, 4, False, False)]
    r = types.SimpleNamespace(delinks={"ov136": ({}, {"src/ov136/mailsys.c": {".rodata": (0x021c7750, 0x021c77c4)}})})
    saved = P.ROOT, P.CONFIG, P.read_object, dict(P.LISTINGS)
    with tempfile.TemporaryDirectory() as d:
        P.ROOT, P.CONFIG, P.read_object = d, f"{d}/config", lambda path: (b"", secs, syms, {})
        P.LISTINGS.clear()
        Path(d, "config/overlays/ov136").mkdir(parents=True)
        Path(d, "config/overlays/ov136/delinks.txt").write_text(listing)
        Path(d, "build/b_us/src/ov136").mkdir(parents=True)
        Path(d, "build/b_us/src/ov136/mailsys.o").write_bytes(b"")
        try:
            assert P.linked_name(r, "ov136", 0x021c7768, "MAILSYS_PROC_FUNCTIONS")
            assert not P.linked_name(r, "ov136", 0x021c776c, "MAILSYS_PROC_FUNCTIONS")  # not where it is
            assert not P.linked_name(r, "ov136", 0x021c7774, "sLocal")  # a static links to nothing outside
            assert not P.linked_name(r, "ov136", 0x021c7768, "MailSys_ProcFunctions")  # another name
            assert not P.linked_name(r, "ov137", 0x021c7768, "MAILSYS_PROC_FUNCTIONS")  # another module
            P.LISTINGS.clear()
            Path(d, "config/overlays/ov136/delinks.txt").write_text(listing.replace("    complete\n", ""))
            assert not P.linked_name(r, "ov136", 0x021c7768, "MAILSYS_PROC_FUNCTIONS")  # linked delinked: Black's name
        finally:
            P.ROOT, P.CONFIG, P.read_object = saved[:3]
            P.LISTINGS.clear()
            P.LISTINGS.update(saved[3])


def test_rename_everywhere_lists_the_files_for_finish():
    # t39n: a rename's other C file (mailsys.c) stayed out of finish.py's commit, which had the header's new name:
    # rename_everywhere lists what it changed in build/renamed_files.txt, as tools/rename.py does
    import tempfile
    saved = P.ROOT, P.CONFIG
    with tempfile.TemporaryDirectory() as d:
        P.ROOT, P.CONFIG = d, f"{d}/config"
        for f, t in (("src/a.c", "int OLD_NAME = 1;\n"), ("include/a.h", "extern int OLD_NAME;\n"),
                     ("src/b.c", "int other;\n"), ("config/symbols.txt", "x kind:data addr:0x0\n")):
            Path(d, f).parent.mkdir(parents=True, exist_ok=True)
            Path(d, f).write_text(t)
        try:
            got = P.rename_everywhere({"OLD_NAME": "data_02000000"})
        finally:
            P.ROOT, P.CONFIG = saved
        assert sorted(got) == ["include/a.h", "src/a.c"], got
        assert Path(d, "src/a.c").read_text() == "int data_02000000 = 1;\n"
        assert sorted(Path(d, "build/renamed_files.txt").read_text().split()) == ["include/a.h", "src/a.c"]


def main():
    failed = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_") and callable(fn):
            try:
                fn()
                print(f"ok {name}")
            except Exception as e:  # noqa: BLE001
                failed += 1
                print(f"FAIL {name}: {type(e).__name__}: {e}")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
