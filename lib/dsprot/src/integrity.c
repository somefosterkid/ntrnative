// DS Protect 1.27's integrity.c (taxicat1/dsprot, https://github.com/taxicat1/dsprot), with the decryption wrappers
// the reference's elfcoder writes for its four functions (integrity_decrypter.s) and their decoder
// (integrity_decrypter_decoder.s), which runs from .ctor. Black's ROM has the four functions encrypted with key 0x998e.
// They check that the MAC address and ROM tests' decryption wrappers still begin as run_encrypted_func writes them. The
// ROM doesn't name them.
#include "dsprot/dsprot_internal.h"

// match_ret when the code at addr + 0x1600 is the start of a decryption wrapper (orr ip, pc, pc; ands ip, ip, ip;
// moveq ip, #0; addne ip, ip, #0x1c), mismatch_ret otherwise. The reference: likely not an inline originally, but it
// matches as one
static inline u32 DSProt_CheckDecryptionWrapper(u8 *addr, u32 match_ret, u32 mismatch_ret) {
    u32 offset;

    addr += 0x1600 & ~0xfff;
    offset = 0x1600 - (0x1600 & ~0xfff);

    if (addr[offset++] != 0x0f) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0xc0) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0x8f) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0xe1) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0x0c) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0xc0) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0x1c) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0xe0) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0x00) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0xc0) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0xa0) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0x03) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0x1c) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0xc0) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0x8c) {
        return mismatch_ret;
    }
    if (addr[offset++] != 0x12) {
        return mismatch_ret;
    }

    return match_ret;
}

// The reference's Integrity_MACOwner_IsBad
u32 func_ov230_02188450(void) {
    u8 *addr;

    addr = (u8 *)(func_ov230_02188e3c + 0x1600) - 0x1600 * 2;

    return DSProt_CheckDecryptionWrapper(addr, DSPROT_PRIME_INTEGRITY * DSPROT_PRIME_FALSE, DSPROT_PRIME_INTEGRITY * DSPROT_PRIME_TRUE);
}

// The reference's Integrity_MACOwner_IsGood
u32 func_ov230_0218856c(void) {
    u8 *addr;

    addr = (u8 *)(func_ov230_02188e78 + 0x1600) - 0x1600 * 2;

    return DSProt_CheckDecryptionWrapper(addr, DSPROT_PRIME_INTEGRITY * DSPROT_PRIME_TRUE, DSPROT_PRIME_INTEGRITY * DSPROT_PRIME_FALSE);
}

// The reference's Integrity_ROMTest_IsBad
u32 func_ov230_02188688(void) {
    u8 *addr;

    addr = (u8 *)(func_ov230_02189340 + 0x1600) - 0x1600 * 2;

    return DSProt_CheckDecryptionWrapper(addr, DSPROT_PRIME_INTEGRITY * DSPROT_PRIME_FALSE, DSPROT_PRIME_INTEGRITY * DSPROT_PRIME_TRUE);
}

// The reference's Integrity_ROMTest_IsGood
u32 func_ov230_021887a4(void) {
    u8 *addr;

    addr = (u8 *)(func_ov230_0218937c + 0x1600) - 0x1600 * 2;

    return DSProt_CheckDecryptionWrapper(addr, DSPROT_PRIME_INTEGRITY * DSPROT_PRIME_TRUE, DSPROT_PRIME_INTEGRITY * DSPROT_PRIME_FALSE);
}

// The reference's RunEncrypted_Integrity_MACOwner_IsBad: decrypts func_ov230_02188450, runs it and encrypts it again
asm u32 func_ov230_021888c0(void) {
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
    dcd DSProt_BSS_ov230 + 0x998e + 0x1600
    dcd func_ov230_02188450 + 0x1600
    dcd DSProt_BSS_ov230 + 0x110 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's RunEncrypted_Integrity_MACOwner_IsGood: decrypts func_ov230_0218856c, runs it and encrypts it again
asm u32 func_ov230_021888fc(void) {
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
    dcd DSProt_BSS_ov230 + 0x998e + 0x1600
    dcd func_ov230_0218856c + 0x1600
    dcd DSProt_BSS_ov230 + 0x110 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's RunEncrypted_Integrity_ROMTest_IsBad: decrypts func_ov230_02188688, runs it and encrypts it again
asm u32 func_ov230_02188938(void) {
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
    dcd DSProt_BSS_ov230 + 0x998e + 0x1600
    dcd func_ov230_02188688 + 0x1600
    dcd DSProt_BSS_ov230 + 0x110 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's RunEncrypted_Integrity_ROMTest_IsGood: decrypts func_ov230_021887a4, runs it and encrypts it again
asm u32 func_ov230_02188974(void) {
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
    dcd DSProt_BSS_ov230 + 0x998e + 0x1600
    dcd func_ov230_021887a4 + 0x1600
    dcd DSProt_BSS_ov230 + 0x110 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The integrity checks' decoder (the reference's NitroStaticInit of integrity_decrypter_decoder.s): has
// func_ov230_02188a48 decode the four wrappers of the table of {function, size} pairs after its code
// clang-format off
asm void __sinit_ov230_021889b0(void) {
    orr r0, pc, #0x0
    adds r0, r0, #0x4
    bne func_ov230_02188a48
    dcd func_ov230_021888c0 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_021888fc + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_02188938 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_02188974 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd 0
    dcd 0
}
// clang-format on

#pragma define_section ctor ".ctor" ".ctor" ".ctor" abs32 RW
__declspec(ctor) static void (*const sDecodeIntegrity)(void) = __sinit_ov230_021889b0;
