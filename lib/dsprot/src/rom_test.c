// DS Protect 1.27's rom_test.c (taxicat1/dsprot, https://github.com/taxicat1/dsprot), with the decryption wrappers
// the reference's elfcoder writes for its two functions (rom_test_decrypter.s). Black's ROM has the two functions
// encrypted with key 0xc20d. The ROM doesn't name them.
#include "dsprot/dsprot_internal.h"
#include "nitro/card.h"
#include "nitro/mi.h"
#include "nitro/os.h"

// fail_ret when the card's ROM reads the same by hand at 0x1000-0x1600 as CARD_ReadRom reads it at 0x8000-0x8600
// (which a flash card's patched CARD_ReadRom does), or when the hand reads disagree; pass_ret otherwise
static inline u32 DSProt_TestROM(u32 pass_ret, u32 fail_ret) {
    // The reference: an extra CRC entry is required to match
    u32 crcs[7];
    u8 rom_buf[CARD_ROM_PAGE_SIZE];
    int i;
    u32 rom_addr;
    u16 lock_id;
    u32 ret;

    rom_addr = 0x1000;

    lock_id = OS_GetLockID();
    CARD_LockRom(lock_id);

    for (i = 0; i < 3; i++) {
        func_ov230_02189098(&rom_buf[0], rom_addr, CARD_ROM_PAGE_SIZE);
        crcs[i] = func_ov230_021890d4(&rom_buf[0], CARD_ROM_PAGE_SIZE);

        CARD_ReadRom(MI_DMA_NOT_USE, (void *)(rom_addr + 0x7000), &rom_buf[0], CARD_ROM_PAGE_SIZE);
        crcs[i + 3] = func_ov230_021890d4(&rom_buf[0], CARD_ROM_PAGE_SIZE);

        rom_addr += CARD_ROM_PAGE_SIZE;
    }

    CARD_UnlockRom(lock_id);
    OS_ReleaseLockID(lock_id);

    for (i = 0; i < 3; i++) {
        if (crcs[i] != crcs[3]) {
            ret = fail_ret;
            goto EXIT;
        }
    }

    if (crcs[3] == crcs[4] && crcs[3] == crcs[5]) {
        ret = fail_ret;
    } else {
        ret = pass_ret;
    }

EXIT:
    // Clears the read buffer
    for (i = 0; i < CARD_ROM_PAGE_SIZE / 4; i++) {
        ((u32 *)&rom_buf[0])[i] = i;
    }

    return ret;
}

// The reference's ROMTest_IsBad
u32 func_ov230_02189110(void) {
    return DSProt_TestROM(DSPROT_PRIME_FALSE, DSPROT_PRIME_TRUE) * DSPROT_PRIME_ROM_TEST_1;
}

// The reference's ROMTest_IsGood
u32 func_ov230_02189228(void) {
    return DSProt_TestROM(DSPROT_PRIME_TRUE, DSPROT_PRIME_FALSE) * DSPROT_PRIME_ROM_TEST_2;
}

// The reference's RunEncrypted_ROMTest_IsBad: decrypts func_ov230_02189110, runs it and encrypts it again
asm u32 func_ov230_02189340(void) {
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
    dcd func_ov230_02189110 + 0x1600
    dcd DSProt_BSS_ov230 + 0x118 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's RunEncrypted_ROMTest_IsGood
asm u32 func_ov230_0218937c(void) {
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
    dcd func_ov230_02189228 + 0x1600
    dcd DSProt_BSS_ov230 + 0x118 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}
