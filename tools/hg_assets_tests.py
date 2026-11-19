#!/usr/bin/env python3
"""Tests for tools/hg_assets.py (no ROM, no compile): an asm label's DS bytes and host segments (a pointer between
numbers), the blanking of asm and C numbers (a designator's index kept), a function's tables moved to file scope (an
automatic one copied back, a static one renamed in the body), `const` dropped from a table's other declarations, the
ELF reader and the ROM search with a mask.

    python3 tools/hg_assets_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import hg_assets as H  # noqa: E402


def check(name, cond):
    print(("ok   " if cond else "FAIL ") + name)
    return cond


ASM = """\t.rodata

\t.global ov01_02200000
ov01_02200000: ; 0x02200000
\t.byte 0x01, 0x02, 0x03, 0x04 // a comment
\t.word sub_02000000
\t.short 0x0005, POCKET_ITEMS
\t.size ov01_02200000,.-ov01_02200000

_02200010:
\t.word 0
\t.ascii "ab"
"""

C = """static const u8 sTable[] = { 1, 2, 3 };
extern const u8 sOther[4];
const u8 sOther[4] = { [2] = 7, 9 };

void f(int x) {
    static const u16 sLocal[2] = { 300, 400 };
    const u8 autoT[3] = { 5, 6, 7 };
    g(sLocal[x], autoT[x]);
}
"""


class FakeRom(H.Rom):
    def __init__(self, mods):
        self.mods = mods


def tests():
    ok = True
    objs = H.asm_objects(ASM)
    ok &= check("asm: two labels, the comment and a constant's name not numbers",
                [o['name'] for o in objs] == ['ov01_02200000', '_02200010'] and objs[0]['ok'] and
                [v for _, v, _ in objs[0]['items']] == [1, 2, 3, 4, None, 5, None])
    data, mask, segs = H.asm_layout(objs[0]['items'])
    ok &= check("asm: DS bytes with the pointer and the name unknown, host segments around the 8-byte pointer",
                data == bytes([1, 2, 3, 4, 0, 0, 0, 0, 5, 0, 0, 0]) and mask == b'\1' * 4 + b'\0' * 4 + b'\1\1\0\0' and
                segs == [(0, 0, 4), (8, 16, 4)])
    blanked = H.blank_spans(ASM, [sp for _, v, sp in objs[0]['items'] if v and sp])
    ok &= check("asm: the numbers blanked, the rest kept",
                '.byte 0, 0, 0, 0 // a comment' in blanked and '.short 0, POCKET_ITEMS' in blanked and
                'sub_02000000' in blanked)
    ok &= check("asm: .ascii bytes known (text is not blanked)", [v for _, v, _ in objs[1]['items']] == [0, 97, 98])

    big = 'u16 t[] = {' + ', '.join(str(i) for i in range(450)) + '};\nint x = 5;'
    sp = H.static_spans(big.encode())
    ok &= check("a table --static would flag is a span (a ramp the full check calls trivial)",
                sp == [(big.index('0,'), big.index('449'))] and not H.static_spans(b'u8 t[] = {1, 2, 3};'))
    ok &= check("C: a designator's index kept", H.c_blank('{ [2] = 7, 9, 0x1F, -1, 1.5f }') == '{ [2] = 0, 0, 0, -0, 0 }')
    cobjs = H.c_objects(C)
    by = {o['name']: o for o in cobjs}
    ok &= check("C: file scope and function tables found",
                set(by) == {'sTable', 'sOther', 'sLocal', 'autoT'} and by['sTable']['func'] is None and
                by['sLocal']['static'] and not by['autoT']['static'] and by['sLocal']['func'] == C.index('void f'))
    moved, sym = H.c_rewrite(C, [by['sLocal'], by['autoT']])
    ok &= check("C: the function's tables moved above it, renamed; the automatic one copied back",
                sym == ['hgasset_f_sLocal', 'hgasset_f_autoT'] and
                moved.index('static u16 hgasset_f_sLocal[2] = { 300, 400 };') < moved.index('void f') and
                'g(hgasset_f_sLocal[x], autoT[x])' in moved and
                '__builtin_memcpy((void *)&autoT, &hgasset_f_autoT, sizeof(autoT));' in moved)
    ok &= check("C: const dropped from a table's other declarations",
                'extern u8 sOther[4];' in H.unconst_decls(C, 'sOther') and
                'const u8 sOther[4] = {' in H.unconst_decls(C, 'sOther'))
    reg = H.c_register('src/x.c', ['sTable'], 'hgasset_register_')
    ok &= check("C: the constructor registers each table for both games, its name unique",
                'hgasset_register_src_x_c_1(void)' in reg and reg.count('PORT_AssetAdd(') == 3)

    # a minimal ELF64: one .data section of 8 bytes, one object `t` of 4 bytes at 4, a relocation at 0 (another object)
    strtab = b'\0t\0u\0'
    data = bytes(range(1, 9))
    sym = bytes(24) + struct.pack('<IBBHQQ', 1, 1, 0, 1, 4, 4) + struct.pack('<IBBHQQ', 3, 1, 0, 1, 0, 4)
    rela = struct.pack('<QQq', 0, 0, 0)
    body = data + strtab + sym + rela
    offs = [64, 64 + len(data), 64 + len(data) + len(strtab), 64 + len(data) + len(strtab) + len(sym)]
    shoff = 64 + len(body)
    secs = [bytes(64),
            struct.pack('<IIQQQQIIQQ', 0, 1, 3, 0, offs[0], 8, 0, 0, 1, 0),
            struct.pack('<IIQQQQIIQQ', 0, 3, 0, 0, offs[1], len(strtab), 0, 0, 1, 0),
            struct.pack('<IIQQQQIIQQ', 0, 2, 0, 0, offs[2], len(sym), 2, 1, 8, 24),
            struct.pack('<IIQQQQIIQQ', 0, 4, 0, 0, offs[3], 24, 3, 1, 8, 24)]
    head = bytearray(64)
    struct.pack_into('<Q', head, 0x28, shoff)
    struct.pack_into('<HH', head, 0x3A, 64, len(secs))
    elf = H.elf_objects(bytes(head) + body + b''.join(secs))
    ok &= check("ELF: an object's bytes, a relocation inside another marks it", elf == {'t': (bytes([5, 6, 7, 8]), False),
                                                                                     'u': (bytes([1, 2, 3, 4]), True)})

    rom = FakeRom({-1: (0x02000000, b'\0' * 16 + b'\x09\x08\x07\x06\xAA\xBB\x05'), 3: (0x02200000, b'\x09\x08\x07\x06\xCC\xDD\x05')})
    ok &= check("ROM search: a mask's unknown bytes match anything, the hint module first",
                rom.find(b'\x09\x08\x07\x06\0\0\x05', b'\1\1\1\1\0\0\1', 3) == [(3, 0x02200000), (-1, 0x02000010)])
    ok &= check("a name's address and module", H.name_address('ov12_0226D680') == (12, 0x0226D680) and
                H.path_module('asm/overlay_18.s') == 18)
    return ok


if __name__ == "__main__":
    sys.exit(0 if tests() else 1)
