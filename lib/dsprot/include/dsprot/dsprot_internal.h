// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dsprot/include/dsprot/dsprot_internal.h: its
// types and constants, and the prototypes of Black's named functions.
#ifndef BWPORT_DSPROT_DSPROT_INTERNAL_H
#define BWPORT_DSPROT_DSPROT_INTERNAL_H

#include "types.h"
#include "dsprot/dsprot.h"

// dsprot_cipher.c: a stream cipher over the words of code, keyed by 16 bytes. DSProt_DecryptCodeEntry and
// DSProt_EncryptCodeEntry hold the address of DSProt_DecryptCodeWithKey and DSProt_EncryptCodeWithKey plus 0x2200, and
// sCipherFunctions the address of the others in the order below.
typedef struct DSProtCipherState {
    u32 offset;
    u32 i;
    u32 j;
    u8 table[256];
} DSProtCipherState;

typedef struct DSProtCheckArgs {
    DSProtCallback nop;     // called with arg0 and arg1 if all checks pass
    DSProtCallback tamper;  // called with arg0 and arg1 if a check fails
    void *arg0;
    void *arg1;
    void *result;           // what tamper returned
    u32 failed;             // nonzero once tamper has run
} DSProtCheckArgs;

// pokebw2's DSProt_GetBranchType
u32 func_ov230_02188a0c(u32 instruction);
// pokebw2's DSProt_Rc4Init (the Black code decodes a table of {addr, size} function entries in place)
void func_ov230_02188a48(u32 *functions);
extern u8 DSProt_BSS_ov230[];
// pokebw2's DSProt_Rc4Next (the Black code is a wrapper that decrypts a function and calls it)
u32 func_ov230_02188b48(u32 addr, u32 size, u32 key);
void func_ov230_0218949c(DSProtCipherState *state, const u8 *key, u32 keyLength);
u32 func_ov230_021895b8(DSProtCipherState *state, u32 dst, u32 src, u32 size);
u32 func_ov230_021896d0(DSProtCipherState *state, u32 dst, u32 src, u32 size);
u32 func_ov230_021898b0(u32 *keyWords, u32 dst, u32 src, u32 size);
// pokebw2's DSProt_EncryptCode (the Black code is a wrapper that encrypts a function and returns the new key)
u32 func_ov230_02188bd0(u32 key, u32 func, u32 size);
u32 func_ov230_02189858(u32 *keyWords, u32 dst, u32 src, u32 size);
// Black's: an assembly trampoline. Its caller's ip points to a block {saved lr, key, function, size, saved r10}: it
// decrypts the function, calls it with r0-r3, encrypts it again with a new key and returns its result
DSPROT_ASM void func_ov230_02188c60(void);
// Black's: the static initializer, in assembly, that decrypts the functions of the table after its code and clears it
DSPROT_ASM void __sinit_ov230_02188cb0(void);

// One bit of DS Protect's CRC-32, which applies the polynomial to the other half of the values: DSProt_Crc32 and
// DSProt_Crc32Multi write out a byte's eight steps
#define DSPROT_CRC32_STEP(crc)                                                                                         \
    do {                                                                                                               \
        if ((crc) & 1) {                                                                                               \
            (crc) = (crc) >> 1;                                                                                        \
        } else {                                                                                                       \
            (crc) = ((crc) >> 1) ^ 0xedb88320;                                                                         \
        }                                                                                                              \
    } while (0)

// DS Protect 1.27's primes.h: what its checks return, a prime for true or false times a prime of the check
#define DSPROT_PRIME_TRUE 251
#define DSPROT_PRIME_FALSE 241
#define DSPROT_PRIME_MAIN 53
#define DSPROT_PRIME_INTEGRITY 193
#define DSPROT_PRIME_MAC_OWNER_1 173
#define DSPROT_PRIME_MAC_OWNER_2 179
#define DSPROT_PRIME_ROM_TEST_1 191
#define DSPROT_PRIME_ROM_TEST_2 181
#define DSPROT_PRIME_DUMMY_1 193
#define DSPROT_PRIME_DUMMY_2 197

// dsprot_main.c (encrypted, key 0x946f), its decryption wrappers (dsprot.h: the game calls them) and their decoder
u32 func_ov230_02187f00(void *callback, void *param, u32 unused);
u32 func_ov230_02187fa0(void *callback, void *param, u32 unused);
u32 func_ov230_02188040(void *callback, void *param, u32 unused);
u32 func_ov230_021880e0(void *callback, void *param, u32 unused);
u32 func_ov230_02188180(void *callback, void *param, u32 unused);
u32 func_ov230_02188210(void *callback, void *param, u32 unused);
DSPROT_ASM void __sinit_ov230_02188408(void);
// extra.c's garbage words, which dsprot_main.c's decoder points at
extern const u32 data_ov230_02189954[6];
// integrity.c (encrypted, key 0x998e), its decryption wrappers and their decoder
u32 func_ov230_02188450(void);
u32 func_ov230_0218856c(void);
u32 func_ov230_02188688(void);
u32 func_ov230_021887a4(void);
DSPROT_ASM u32 func_ov230_021888c0(void);
DSPROT_ASM u32 func_ov230_021888fc(void);
DSPROT_ASM u32 func_ov230_02188938(void);
DSPROT_ASM u32 func_ov230_02188974(void);
DSPROT_ASM void __sinit_ov230_021889b0(void);
// mac_owner.c (encrypted, key 0xc20d) and its decryption wrappers
u32 func_ov230_02188cdc(void);
u32 func_ov230_02188d8c(void);
DSPROT_ASM u32 func_ov230_02188e3c(void);
DSPROT_ASM u32 func_ov230_02188e78(void);

// rom_util.c and rom_test.c's decryption wrappers (their functions are encrypted, key 0xc20d)
void func_ov230_02188eb4(void *dest, u32 addr, s32 num_bytes);
u32 func_ov230_02189004(void *buf, u32 size);
DSPROT_ASM void func_ov230_02189098(void *dest, u32 addr, s32 num_bytes);
DSPROT_ASM u32 func_ov230_021890d4(void *buf, u32 size);
u32 func_ov230_02189110(void);
u32 func_ov230_02189228(void);
DSPROT_ASM u32 func_ov230_02189340(void);
DSPROT_ASM u32 func_ov230_0218937c(void);
// dummy.c (encrypted, key 0xc20d), its decryption wrappers and the core tests' decoder
u32 func_ov230_021893b8(void);
u32 func_ov230_021893c4(void);
DSPROT_ASM u32 func_ov230_021893d0(void);
DSPROT_ASM u32 func_ov230_0218940c(void);
DSPROT_ASM void __sinit_ov230_02189448(void);

#endif // BWPORT_DSPROT_DSPROT_INTERNAL_H
