// DS Protect 1.27's dummy.c (taxicat1/dsprot, https://github.com/taxicat1/dsprot), with the decryption wrappers the
// reference's elfcoder writes for its two functions (dummy_decrypter.s) and the decoder of the core tests' wrappers
// (coretests_decrypter_decoder.s), which runs from .ctor. Black's ROM has the two functions encrypted with key 0xc20d.
// The ROM doesn't name them.
#include "dsprot/dsprot_internal.h"

// The reference's Dummy_IsBad
u32 func_ov230_021893b8(void) {
    return DSPROT_PRIME_DUMMY_1 * DSPROT_PRIME_FALSE;
}

// The reference's Dummy_IsGood
u32 func_ov230_021893c4(void) {
    return DSPROT_PRIME_DUMMY_2 * DSPROT_PRIME_TRUE;
}

// The reference's RunEncrypted_Dummy_IsBad: decrypts func_ov230_021893b8, runs it and encrypts it again
asm u32 func_ov230_021893d0(void) {
    orr ip, pc, pc
    ands ip, ip, ip
    moveq ip, #0x0
    addne ip, ip, #0x1c
    ldr ip, [ip, #0x14]
    sub ip, ip, #0x1600
    stmdb sp!, {ip}
    orr ip, pc, pc
    ldmia sp!, {pc}
    dcd DSProt_BSS_ov230 + 1
    dcd DSProt_BSS_ov230 + 0xc20d + 0x1600
    dcd func_ov230_021893b8 + 0x1600
    dcd DSProt_BSS_ov230 + 0x8 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's RunEncrypted_Dummy_IsGood
asm u32 func_ov230_0218940c(void) {
    orr ip, pc, pc
    ands ip, ip, ip
    moveq ip, #0x0
    addne ip, ip, #0x1c
    ldr ip, [ip, #0x14]
    sub ip, ip, #0x1600
    stmdb sp!, {ip}
    orr ip, pc, pc
    ldmia sp!, {pc}
    dcd DSProt_BSS_ov230 + 1
    dcd DSProt_BSS_ov230 + 0xc20d + 0x1600
    dcd func_ov230_021893c4 + 0x1600
    dcd DSProt_BSS_ov230 + 0x8 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The core tests' decoder (the reference's NitroStaticInit of coretests_decrypter_decoder.s): has
// func_ov230_02188a48 decode the eight wrappers of the table of {function, size} pairs after its code
// clang-format off
asm void __sinit_ov230_02189448(void) {
    orr r0, pc, #0x0
    adds r0, r0, #0x4
    bne func_ov230_02188a48
    dcd func_ov230_02189340 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_0218937c + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_02188e3c + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_02188e78 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_02189098 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_021890d4 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_021893d0 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_0218940c + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd 0
    dcd 0
}
// clang-format on

#pragma define_section ctor ".ctor" ".ctor" ".ctor" abs32 RW
__declspec(ctor) static void (*const sDecodeCoreTests)(void) = __sinit_ov230_02189448;
