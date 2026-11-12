// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/src/photo_sign.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "photo_sign.h"
#include "types.h"
#include "nitro/math.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "photo_sign_private.h"
#include "gfl/std.h"

// The DSi's photo signature, which the SSP JPEG encoder and decoder of overlay 257 call: a signature is a random nonce
// and the MAC that the ARM7 computes over the photo with it (photo_sign_client.c). The nonces are drawn from random
// numbers that the ARM7 supplies, after it is seeded once from the console's entropy. ARM code in the DSi-only LTD
// autoload. The ROM names none of it; the file's name and those of its functions and data are descriptive guesses.
// Its .bss is reached from its own base, so it was a file of its own, built with -ipa file like the library's others.

static void func_02707558(u32 result, void *arg);
static void func_02707570(void);
static int func_0270768c(u8 *dst, u32 size);

// The block of random bytes from the ARM7, which writes it, so it has a cache line of its own: aligned to it and as
// large as it (32 bytes, of which a block uses 16), which puts photo_sign_util.c's .bss 16 bytes further on
// pokebw2's sRandomBuffer
static u8 data_02735b00[32] ATTRIBUTE_ALIGN(32);
// pokebw2's sRandomInitialized
static BOOL data_02735ae0;
static BOOL sRandomUnk04;
// How many bytes of the block are left, at its end
static u32 sRandomRemaining;
// pokebw2's sMac
static u8 data_02735aec[PHOTO_SIGN_MAC_SIZE];

// The MAC's callback: wakes the signing thread with the result
// pokebw2's PhotoSign_SignalEvent
static void func_02707558(u32 result, void *arg) {
    OS_SignalEvent(arg, result);
}

// Seeds the ARM7's random numbers once with the digest of the console's entropy, then overwrites the entropy and the
// digest with random numbers of their own
// pokebw2's PhotoSign_InitRandom
static void func_02707570(void) {
    MATHRandContext32 rand;
    u32 entropy[8];
    u32 digest[MATH_SHA1_DIGEST_SIZE / sizeof(u32)];
    u32 i;

    OS_InitTick();
    sRandomRemaining = 0;
    data_02735ae0 = TRUE;
    MATH_InitRand32(&rand, ((u64)(u32)&rand << 32) | (u32)&rand);
    OS_GetLowEntropyData(entropy);
    MATH_CalcSHA1((u8 *)digest, entropy, sizeof(entropy));
    for (i = 0; i < NELEMS(entropy); i++) {
        entropy[i] = MATH_Rand32(&rand, 0);
    }
    func_0270741c(digest);
    for (i = 0; i < NELEMS(digest); i++) {
        digest[i] = MATH_Rand32(&rand, 0);
    }
}

// Fills dst with random bytes, using up the rest of the last block from the ARM7 first
// pokebw2's PhotoSign_GetRandom
static int func_0270768c(u8 *dst, u32 size) {
    u32 count;
    int result;

    if (!data_02735ae0) {
        func_02707570();
    }
    if (sRandomRemaining != 0) {
        count = sRandomRemaining;
        if (count > size) {
            count = size;
        }
        sRandomRemaining -= count;
        MI_CpuCopy8(data_02735b00 + sRandomRemaining, dst, count);
        size -= count;
        dst += count;
    }
    while (size >= PHOTO_SIGN_RANDOM_BLOCK_SIZE) {
        result = func_02707458(data_02735b00);
        if (result != PHOTO_SIGN_RESULT_SUCCESS) {
            return result;
        }
        MI_CpuCopy8(data_02735b00, dst, PHOTO_SIGN_RANDOM_BLOCK_SIZE);
        dst += PHOTO_SIGN_RANDOM_BLOCK_SIZE;
        size -= PHOTO_SIGN_RANDOM_BLOCK_SIZE;
    }
    if (size != 0) {
        result = func_02707458(data_02735b00);
        if (result != PHOTO_SIGN_RESULT_SUCCESS) {
            return result;
        }
        sRandomRemaining = PHOTO_SIGN_RANDOM_BLOCK_SIZE - size;
        MI_CpuCopy8(data_02735b00 + sRandomRemaining, dst, size);
    }
    return PHOTO_SIGN_RESULT_SUCCESS;
}

// pokebw2's PhotoSign_Sign
int func_02707774(u8 *sig, const u8 *data, u32 size) {
    OSEvent event;
    int result;

    func_02707b68();
    if (!data_02735ae0) {
        func_02707570();
    }
    OS_InitEvent(&event);
    result = func_0270768c(sig, PHOTO_SIGN_NONCE_SIZE);
    if (result != PHOTO_SIGN_RESULT_SUCCESS) {
        // BUG: The lock stays taken, so other threads wait for it forever
#ifdef BUGFIX
        func_02707bdc();
#endif
        return result;
    }
    do {
        if (!sRandomUnk04) {
            func_02707994(PHOTO_SIGN_FIFO_TAG, PHOTO_SIGN_COMMAND_UNK_8, 1);
        }
        result = func_027074b0(sig, data, size, data_02735aec, func_02707558, &event);
        if (result == PHOTO_SIGN_RESULT_SUCCESS) {
            result = OS_WaitEventEx(&event, 0xffffffff, OS_EVENT_MODE_OR, 0xffffffff);
            if (result == PHOTO_SIGN_RESULT_SUCCESS) {
                MI_CpuCopy8(data_02735aec, sig + PHOTO_SIGN_NONCE_SIZE, PHOTO_SIGN_MAC_SIZE);
            }
        }
    } while (result == PHOTO_SIGN_RESULT_BUSY);
    if (!sRandomUnk04) {
        func_02707994(PHOTO_SIGN_FIFO_TAG, PHOTO_SIGN_COMMAND_UNK_8, 0);
    }
    func_02707bdc();
    return result;
}

// pokebw2's PhotoSign_Verify
int func_0270788c(const u8 *sig, const u8 *data, u32 size) {
    OSEvent event;
    int result;

    func_02707b68();
    if (!data_02735ae0) {
        func_02707570();
    }
    OS_InitEvent(&event);
    do {
        if (!sRandomUnk04) {
            func_02707994(PHOTO_SIGN_FIFO_TAG, PHOTO_SIGN_COMMAND_UNK_8, 1);
        }
        result = func_027074b0(sig, data, size, data_02735aec, func_02707558, &event);
        if (result == PHOTO_SIGN_RESULT_SUCCESS) {
            result = OS_WaitEventEx(&event, 0xffffffff, OS_EVENT_MODE_OR, 0xffffffff);
            if (result == PHOTO_SIGN_RESULT_SUCCESS &&
                sys_memcmp(data_02735aec, sig + PHOTO_SIGN_NONCE_SIZE, PHOTO_SIGN_MAC_SIZE) != 0) {
                result = PHOTO_SIGN_RESULT_MISMATCH;
            }
        }
    } while (result == PHOTO_SIGN_RESULT_BUSY);
    if (!sRandomUnk04) {
        func_02707994(PHOTO_SIGN_FIFO_TAG, PHOTO_SIGN_COMMAND_UNK_8, 0);
    }
    func_02707bdc();
    return result;
}
