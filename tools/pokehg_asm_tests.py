#!/usr/bin/env python3
"""Tests for tools/pokehg_asm.py's rewrite of the DS's data assembly (scratch files; no assembler):

    python3 tools/pokehg_asm_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pokehg_asm as A  # noqa: E402

FAILED = []


def check(name, got, want):
    if got == want:
        print("ok", name)
    else:
        FAILED.append(name)
        print(f"FAIL {name}: {got!r} != {want!r}")


def test_words():
    check("word number", A.word("0x10"), ".long")
    check("word expr", A.word("(1 << 4) + 2"), ".long")
    check("word symbol", A.word("ov01_02209AE0"), ".quad")
    check("word symbol plus", A.word("_020F5710 + 4"), ".quad")
    check("word macro arg", A.word(chr(92) + "what"), ".long")


def test_host():
    with tempfile.TemporaryDirectory() as t:
        Path(t, "x.inc").write_text(".public sub_02000000\n.extern ov01_X\n")
        src = ('\t.include "x.inc"\n\t.include "missing.inc"\n\t.rodata\n\t.align 2\n\t.global T\nT:\n'
               '\t.word 1, T, 0x2 ; a comment\n\t.byte 1, 2,\n\t.ascii "a;b"\n\t.section .sinit,4\n')
        got = A.host(src, [t]).splitlines()
    check("host", got, ["\t.global sub_02000000", '\t.section .data.rel.ro,"aw"', "\t.p2align 2", "\t.global T", "T:",
                        "\t.long 1", "\t.quad T", "\t.long 0x2", "\t.byte 1, 2", '\t.ascii "a;b"',
                        '\t.section .data.rel.ro,"aw"'])


def test_coff_globals():
    src = "T:\n\t.global T\n\t.global A\n\t.set A, T\n\t.global F\n"
    check("coff globals: a .set name kept, an undefined one dropped", A.coff_globals(src).splitlines(),
          ["T:", "\t.global T", "\t.global A", "\t.set A, T"])


def test_files():
    with tempfile.TemporaryDirectory() as t:
        os.makedirs(os.path.join(t, "asm"))
        Path(t, "main.lsf.in").write_text("Static main\n{\n    Object asm/d.s\n    Object asm/f.s\n    Object src/c.c\n}\n")
        Path(t, "asm/d.s").write_text("\t.rodata\nX:\n\t.word 1\n")
        Path(t, "asm/f.s").write_text("\tthumb_func_start F\nF:\n")
        check("files", A.files(t), ["asm/d.s"])


test_words()
test_host()
test_coff_globals()
test_files()
sys.exit(1 if FAILED else 0)
