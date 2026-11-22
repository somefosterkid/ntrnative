#!/usr/bin/env python3
"""Tests for elfhash.py: python3 tools/elfhash_tests.py"""
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import elfhash as E


def elf(text=b"\x90" * 16, debug=b"/build/a\0", note=b"id-1", bss=64):
    """a minimal 64-bit LE ELF: .text, .bss, .debug_str, .note.gnu.build-id, .shstrtab"""
    names = [".text", ".bss", ".debug_str", ".note.gnu.build-id", ".shstrtab"]
    strtab, offs = b"\0", []
    for n in names:
        offs.append(len(strtab))
        strtab += n.encode() + b"\0"
    bodies = [text, b"", debug, note, strtab]
    data = bytearray(b"\x7fELF\x02\x01\x01" + b"\0" * 9 + b"\0" * 48)
    secs = [(0, 0, 0, 0, 0, 0)]
    for n, o, b in zip(names, offs, bodies):
        typ = 8 if n == ".bss" else 7 if n.startswith(".note") else 3 if n == ".shstrtab" else 1
        secs.append((o, typ, 6, 0x1000 if n == ".text" else 0, len(data), bss if n == ".bss" else len(b)))
        if n != ".bss":
            data += b
    shoff = len(data)
    for o, typ, fl, addr, off, size in secs:
        data += struct.pack("<IIQQQQIIQQ", o, typ, fl, addr, off, size, 0, 0, 1, 0)
    struct.pack_into("<Q", data, 0x28, shoff)
    struct.pack_into("<HHH", data, 0x3A, 64, len(secs), len(secs) - 1)
    return bytes(data)


def t_debug_and_build_id_do_not_count():
    a = E.elf_hash(elf())
    assert a == E.elf_hash(elf(debug=b"/build/b and a longer path\0", note=b"id-2"))


def t_code_and_data_count():
    a = E.elf_hash(elf())
    assert a != E.elf_hash(elf(text=b"\x90" * 15 + b"\xcc"))
    assert a != E.elf_hash(elf(bss=128))
    assert E.elf_hash(elf()) == a


def t_not_an_elf():
    try:
        E.elf_hash(b"hello")
    except ValueError:
        return
    assert False


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
    print("elfhash_tests:", "FAIL" if bad else "PASS")
    sys.exit(bad)
