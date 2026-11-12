// DS Protect 1.27's dsprot_main.c (taxicat1/dsprot, https://github.com/taxicat1/dsprot), with the decryption wrappers
// the reference's elfcoder writes for its six functions (dsprot_main_decrypter.s, which the game calls: the
// reference's __DSProt_DetectFlashcart and the rest) and their decoder (dsprot_main_decrypter_decoder.s), which runs
// from .ctor. Black's ROM has the six functions encrypted with key 0x946f. Each runs the decryption wrappers of two
// checks (one for the dummy ones) and calls the callback by what they return. The ROM doesn't name them.
#include "dsprot/dsprot_internal.h"

// What the obfuscated function pointers of a queue are offset by, on top of 0x1600
#define DSPROT_MAIN_OBFS_OFFSET 0x320

typedef u32 (*DSProtTaskFunc)(void);
typedef u32 (*DSProtMainCallback)(void *);

// The reference's dsprotMain: runs the queue's checks, adds up what they return and calls callback(param) when the
// sum says they passed (expected_result 1: divisible by the true prime) or failed (0: not divisible by the false one).
// The reference: likely not an inline originally, but it matches as one
static inline u32 DSProt_RunQueue(u32 *func_queue_ptr, int expected_result, void *callback, void *param) {
    u32 func_ret_total;

    func_ret_total = DSPROT_PRIME_MAIN * DSPROT_PRIME_TRUE * DSPROT_PRIME_FALSE;
    do {
        func_ret_total += ((DSProtTaskFunc)(*func_queue_ptr - 0x1600 - DSPROT_MAIN_OBFS_OFFSET))();
        func_queue_ptr++;
    } while (*func_queue_ptr != 0);

    if (expected_result == 1) {
        if (!(func_ret_total % DSPROT_PRIME_TRUE)) {
            return ((DSProtMainCallback)callback)(param);
        }
    } else {
        if (func_ret_total % DSPROT_PRIME_FALSE) {
            return ((DSProtMainCallback)callback)(param);
        }
    }

    return func_ret_total;
}

// The reference's DetectFlashcart: calls callback(param) when the ROM test or its wrapper's integrity check fails
u32 func_ov230_02187f00(void *callback, void *param, u32 unused) {
#pragma unused(unused)
    u32 func_queue[32];

    func_queue[2] = 0;
    func_queue[0] = (u32)(func_ov230_02189340 + 0x1600) + DSPROT_MAIN_OBFS_OFFSET;
    func_queue[1] = (u32)(func_ov230_02188938 + 0x1600) + DSPROT_MAIN_OBFS_OFFSET;

    return DSProt_RunQueue(&func_queue[0], 0, callback, param);
}

// The reference's DetectNotFlashcart: calls callback(param) when the ROM test and its wrapper's integrity check pass
u32 func_ov230_02187fa0(void *callback, void *param, u32 unused) {
#pragma unused(unused)
    u32 func_queue[32];

    func_queue[2] = 0;
    func_queue[0] = (u32)(func_ov230_0218937c + 0x1600) + DSPROT_MAIN_OBFS_OFFSET;
    func_queue[1] = (u32)(func_ov230_02188974 + 0x1600) + DSPROT_MAIN_OBFS_OFFSET;

    return DSProt_RunQueue(&func_queue[0], 1, callback, param);
}

// The reference's DetectEmulator: calls callback(param) when the MAC address test or its wrapper's integrity check
// fails
u32 func_ov230_02188040(void *callback, void *param, u32 unused) {
#pragma unused(unused)
    u32 func_queue[32];

    func_queue[2] = 0;
    func_queue[0] = (u32)(func_ov230_02188e3c + 0x1600) + DSPROT_MAIN_OBFS_OFFSET;
    func_queue[1] = (u32)(func_ov230_021888c0 + 0x1600) + DSPROT_MAIN_OBFS_OFFSET;

    return DSProt_RunQueue(&func_queue[0], 0, callback, param);
}

// The reference's DetectNotEmulator: calls callback(param) when the MAC address test and its wrapper's integrity
// check pass
u32 func_ov230_021880e0(void *callback, void *param, u32 unused) {
#pragma unused(unused)
    u32 func_queue[32];

    func_queue[2] = 0;
    func_queue[0] = (u32)(func_ov230_02188e78 + 0x1600) + DSPROT_MAIN_OBFS_OFFSET;
    func_queue[1] = (u32)(func_ov230_021888fc + 0x1600) + DSPROT_MAIN_OBFS_OFFSET;

    return DSProt_RunQueue(&func_queue[0], 1, callback, param);
}

// The reference's DetectDummy: calls callback(param) when the dummy test fails (it never does). No integrity check
// on the dummy tests
u32 func_ov230_02188180(void *callback, void *param, u32 unused) {
#pragma unused(unused)
    u32 func_queue[32];

    func_queue[0] = (u32)(func_ov230_021893d0 + 0x1600) + DSPROT_MAIN_OBFS_OFFSET;
    func_queue[1] = 0;

    return DSProt_RunQueue(&func_queue[0], 0, callback, param);
}

// The reference's DetectNotDummy: calls callback(param) when the dummy test passes (it always does)
u32 func_ov230_02188210(void *callback, void *param, u32 unused) {
#pragma unused(unused)
    u32 func_queue[32];

    func_queue[0] = (u32)(func_ov230_0218940c + 0x1600) + DSPROT_MAIN_OBFS_OFFSET;
    func_queue[1] = 0;

    return DSProt_RunQueue(&func_queue[0], 1, callback, param);
}

// The reference's __DSProt_DetectFlashcart: decrypts func_ov230_02187f00, runs it and encrypts it again
asm u32 func_ov230_021882a0(void *callback, void *param, u32 unused) {
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
    dcd DSProt_BSS_ov230 + 0x946f + 0x1600
    dcd func_ov230_02187f00 + 0x1600
    dcd DSProt_BSS_ov230 + 0x8c + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's __DSProt_DetectNotFlashcart: decrypts func_ov230_02187fa0, runs it and encrypts it again
asm u32 func_ov230_021882dc(void *callback, void *param, u32 unused) {
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
    dcd DSProt_BSS_ov230 + 0x946f + 0x1600
    dcd func_ov230_02187fa0 + 0x1600
    dcd DSProt_BSS_ov230 + 0x8c + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's __DSProt_DetectEmulator: decrypts func_ov230_02188040, runs it and encrypts it again
asm u32 func_ov230_02188318(void *callback, void *param, u32 unused) {
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
    dcd DSProt_BSS_ov230 + 0x946f + 0x1600
    dcd func_ov230_02188040 + 0x1600
    dcd DSProt_BSS_ov230 + 0x8c + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's __DSProt_DetectNotEmulator: decrypts func_ov230_021880e0, runs it and encrypts it again
asm u32 func_ov230_02188354(void *callback, void *param, u32 unused) {
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
    dcd DSProt_BSS_ov230 + 0x946f + 0x1600
    dcd func_ov230_021880e0 + 0x1600
    dcd DSProt_BSS_ov230 + 0x8c + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's __DSProt_DetectDummy: decrypts func_ov230_02188180, runs it and encrypts it again
asm u32 func_ov230_02188390(void *callback, void *param, u32 unused) {
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
    dcd DSProt_BSS_ov230 + 0x946f + 0x1600
    dcd func_ov230_02188180 + 0x1600
    dcd DSProt_BSS_ov230 + 0x80 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The reference's __DSProt_DetectNotDummy: decrypts func_ov230_02188210, runs it and encrypts it again
asm u32 func_ov230_021883cc(void *callback, void *param, u32 unused) {
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
    dcd DSProt_BSS_ov230 + 0x946f + 0x1600
    dcd func_ov230_02188210 + 0x1600
    dcd DSProt_BSS_ov230 + 0x80 + 0x1600
    dcd 0
    dcd func_ov230_02188c60 + 0x1600
}

// The main module's decoder (the reference's NitroStaticInit of dsprot_main_decrypter_decoder.s): has
// func_ov230_02188a48 decode the six wrappers of the table of {function, size} pairs after its code. The word after
// the table points at extra.c's garbage words
// clang-format off
asm void __sinit_ov230_02188408(void) {
    orr r0, pc, #0x0
    adds r0, r0, #0x4
    bne func_ov230_02188a48
    dcd func_ov230_021882a0 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_021882dc + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_02188318 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_02188354 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_02188390 + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd func_ov230_021883cc + 0x1600
    dcd DSProt_BSS_ov230 + 0x1624
    dcd 0
    dcd 0
    dcd data_ov230_02189954 + 0x1600
}
// clang-format on

#pragma define_section ctor ".ctor" ".ctor" ".ctor" abs32 RW
__declspec(ctor) static void (*const sDecodeMain)(void) = __sinit_ov230_02188408;
