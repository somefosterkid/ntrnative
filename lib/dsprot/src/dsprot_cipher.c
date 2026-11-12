// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dsprot/src/dsprot_cipher.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "dsprot/dsprot_internal.h"

// DS Protect 1.27's encryptor.c (taxicat1/dsprot): the encryption of the library's own functions, and the decoder of
// its encoded functions (encryptor_decoder.s, which the reference's elfcoder writes) that runs from .ctor. The ROM
// doesn't name this file; pokebw2 names it for what it does. Black's ov230 has no table of these functions (pokebw2's
// sCipherFunctions is Black 2's): 0x0218996c is mac_owner.c's bad_mac_addr.

// Writes back the data cache and invalidates the instruction cache over a range (the reference's
// clearDataAndInstructionCache: DC_FlushRange and IC_InvalidateRange in one). DS Protect's own assembly
static asm void func_ov230_021889e4(register u32 start, register u32 size) {
    add r1, r1, r0
    mov ip, #0
    bic r0, r0, #0x1f
@1:
    mcr p15, 0, r0, c7, c5, 1
    mcr p15, 0, ip, c7, c10, 4
    mcr p15, 0, r0, c7, c14, 1
    add r0, r0, #0x20
    cmp r0, r1
    blt @1
    bx lr
}

// What a word of code is, by the top byte of the instruction: 1 for a branch with link and exchange, 2 for a branch
// with link, 3 for a branch, 0 for the rest
// pokebw2's DSProt_GetBranchType
u32 func_ov230_02188a0c(u32 instruction) {
    u32 byte = (u8)(instruction >> 24);

    if ((byte & 0xe) == 0xa) {
        if ((byte & 0xf0) == 0xf0) {
            return 1;
        }
        if (byte & 1) {
            return 2;
        }
        return 3;
    }
    return 0;
}

// pokebw2's DSProt_Rc4Init
void func_ov230_02188a48(u32 *functions) {
    u32 *addr;
    u32 size;
    u32 *end_addr;
    u32 xorval;
    u32 *prevmem;

    prevmem = functions - 3;
    prevmem[0] = prevmem[1] = prevmem[2] = 0;

    do {
        xorval = 0xF0618C46;

        addr = (u32 *)functions[0];
        size = functions[1] - (u32)DSProt_BSS_ov230 - 0x1600;

        addr = (u32 *)((u8 *)addr - 0x1600);
        end_addr = addr + (size / 4);

        for (; addr < end_addr; addr++) {
            switch (func_ov230_02188a0c(*addr)) {
            case 1:
            case 3: {
                u32 opcode = (*addr & 0xFF000000) ^ (1 << 24);
                u32 operands = ((*addr & 0x00FFFFFF) - ((0x1600 >> 2) + 2)) & 0x00FFFFFF;

                *addr = opcode | operands;

                xorval ^= *addr >> 24;
                xorval &= 0x00FFFFFF;
            } break;

            case 2:
                *addr ^= (1 << 24);
            default:
                *addr ^= xorval;

                xorval ^= *addr;
                xorval ^= *addr >> 8;
                xorval &= 0x00FFFFFF;
                break;
            }
        }

        func_ov230_021889e4(functions[0] - 0x1600, size);

        functions[0] = functions[1] = 0;

        functions += 2;
    } while (functions[0] != 0);
}

// pokebw2's DSProt_Rc4Next
u32 func_ov230_02188b48(u32 key, u32 func, u32 size) {
    u32 keyWords[4];
    u32 base = (u32)DSProt_BSS_ov230 + 0x1600;
    u32 a = key - base;

    size -= base;
    keyWords[0] = a ^ size;
    keyWords[1] = size ^ ((a << 8) | (a >> 24));
    keyWords[2] = size ^ ((a << 16) | (a >> 16));
    keyWords[3] = size ^ ((a << 24) | (a >> 8));
    func_ov230_021898b0(keyWords, func - 0x1600, func - 0x1600, size);
    func_ov230_021889e4(func - 0x1600, size);
    return func - 0x1600;
}

// pokebw2's DSProt_EncryptCode
u32 func_ov230_02188bd0(u32 key, u32 func, u32 size) {
    u32 keyWords[4];
    u32 base = (u32)DSProt_BSS_ov230 + 0x1600;

    key = key - base + (func >> 20);
    size -= base;
    keyWords[0] = key ^ size;
    keyWords[1] = size ^ ((key << 8) | (key >> 24));
    keyWords[2] = size ^ ((key << 16) | (key >> 16));
    keyWords[3] = size ^ ((key << 24) | (key >> 8));
    func_ov230_02189858(keyWords, func - 0x1600, func - 0x1600, size);
    func_ov230_021889e4(func - 0x1600, size);
    return key + base;
}

// Calls an encrypted function: its caller's ip points to a block {saved lr, key, function, size, saved r10}. It
// decrypts the function, calls it with the caller's r0-r3, encrypts it again with a new key and returns its result.
// DS Protect's own assembly
asm void func_ov230_02188c60(void) {
    stmdb sp!, {r0, r1, r2, r3}
    str r10, [ip, #0x10]
    mov r10, ip
    str lr, [r10, #0x0]
    ldmib r10, {r0, r1, r2}
    bl func_ov230_02188b48
    mov ip, r0
    ldmia sp!, {r0, r1, r2, r3}
    blx ip
    stmdb sp!, {r4}
    mov r4, r0
    ldmib r10, {r0, r1, r2}
    bl func_ov230_02188bd0
    str r0, [r10, #0x4]
    mov r0, r4
    ldmia sp!, {r4}
    ldr lr, [r10, #0x0]
    str sp, [r10, #0x0]
    ldr r10, [r10, #0x10]
    bx lr
}

// The static initializer: decrypts the functions of the table of {function, end} pairs that follows its code, which
// func_ov230_02188a48 then clears with the three instructions before it. DS Protect's own assembly
asm void __sinit_ov230_02188cb0(void) {
    orr r0, pc, #0x0
    adds r0, r0, #0x4
    bne func_ov230_02188a48
    dcd func_ov230_02188bd0 + 0x1600
    dcd DSProt_BSS_ov230 + 0x168c
    dcd func_ov230_02188b48 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1684
    dcd func_ov230_02188c60 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1650
    dcd 0
    dcd 0
}

#pragma define_section ctor ".ctor" ".ctor" ".ctor" abs32 RW
__declspec(ctor) static void (*const sDecryptCipher)(void) = __sinit_ov230_02188cb0;
