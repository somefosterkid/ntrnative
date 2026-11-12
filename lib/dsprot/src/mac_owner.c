// DS Protect 1.27's mac_owner.c (taxicat1/dsprot, https://github.com/taxicat1/dsprot), with the decryption wrappers
// the reference's elfcoder writes for its two functions (mac_owner_decrypter.s). Black's ROM has the two functions
// encrypted with key 0xc20d: dsd decrypts them to delink and encrypts them again to build. The ROM doesn't name them.
#include "dsprot/dsprot_internal.h"
#include "nitro/os.h"

#define MAC_ADDR_SIZE 6

// The reference's bad_mac_addr, each byte XORed with 0xff: an emulator's MAC address
static const u8 data_ov230_0218996c[MAC_ADDR_SIZE] = {
    0x00 ^ 0xff, 0x09 ^ 0xff, 0xbf ^ 0xff, 0x00 ^ 0xff, 0x00 ^ 0xff, 0x31 ^ 0xff,
};

// fail_ret when the MAC address is the emulator's and the maintainer's birthday is 1/1 with no nickname, or when the MAC
// address is all zeros; pass_ret otherwise
static inline u32 DSProt_TestMACOwner(u32 pass_ret, u32 fail_ret) {
    u8 mac_addr[MAC_ADDR_SIZE];
    OSOwnerInfo owner_info;
    int i;
    u32 ret;

    OS_GetMacAddress(&mac_addr[0]);
    for (i = 0; i < MAC_ADDR_SIZE; i++) {
        if (data_ov230_0218996c[i] != (mac_addr[i] ^ 0xff)) {
            break;
        }
    }

    OS_GetOwnerInfo(&owner_info);
    if (i == MAC_ADDR_SIZE && owner_info.birthday.month == 1 && owner_info.birthday.day == 1
        && owner_info.nickNameLength == 0) {
        ret = fail_ret;
        goto EXIT;
    }

    for (i = 0; i < MAC_ADDR_SIZE; i++) {
        if (mac_addr[i] != 0x00) {
            ret = pass_ret;
            goto EXIT;
        }
    }

    ret = fail_ret;

EXIT:
    return ret;
}

// The reference's MACOwner_IsBad
u32 func_ov230_02188cdc(void) {
    return DSProt_TestMACOwner(DSPROT_PRIME_FALSE, DSPROT_PRIME_TRUE) * DSPROT_PRIME_MAC_OWNER_1;
}

// The reference's MACOwner_IsGood
u32 func_ov230_02188d8c(void) {
    return DSProt_TestMACOwner(DSPROT_PRIME_TRUE, DSPROT_PRIME_FALSE) * DSPROT_PRIME_MAC_OWNER_2;
}

// The reference's RunEncrypted_MACOwner_IsBad: decrypts func_ov230_02188cdc, runs it and encrypts it again
// (run_encrypted_func: the block after the code is {storage, key, function, encrypted size, storage, fragment})
asm u32 func_ov230_02188e3c(void) {
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
    dcd func_ov230_02188cdc + 0x1600
    dcd DSProt_BSS_ov230 + 0xac + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's RunEncrypted_MACOwner_IsGood
asm u32 func_ov230_02188e78(void) {
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
    dcd func_ov230_02188d8c + 0x1600
    dcd DSProt_BSS_ov230 + 0xac + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}
