// NitroSDK's RC4 (libcrypto's rc4.c), linked right after the game's own code in ARM9 main. From pokebw2 (GPL-3.0,
// https://github.com/fuddlesworth/pokebw2), lib/nitro/src/crypto/rc4.c. CRYPTO_RC4Encrypt was hand-written assembly
// in the original (an odd register list, stores through the advanced context pointer, loads scheduled across
// iterations), so its C here doesn't match.
#include "nitro/crypto/rc4.h"

// The longest key used: the rest of a longer one is ignored
#define RC4_KEY_LEN_MAX 16

void CRYPTO_RC4Init(CRYPTORC4Context *context, const void *key, u32 key_len) {
    const u8 *keyBytes = key;
    u8 *s;
    int i;
    u8 k;
    u8 j;

    if (key_len > RC4_KEY_LEN_MAX) {
        key_len = RC4_KEY_LEN_MAX;
    }
    context->i = 0;
    context->j = 0;
    s = context->s;
    for (i = 0; i < 256; i++) {
        s[i] = i;
    }
    j = 0;
    k = 0;
    for (i = 0; i < 256; i++) {
        u8 t = s[i];

        j = j + (t + keyBytes[k]);
        k++;
        if (k == (u8)key_len) {
            k = 0;
        }
        s[i] = s[j];
        s[j] = t;
    }
}

#ifndef __MWERKS__
// C for a PC build: MWCC, which defines __MWERKS__, compiles the asm under #else. RC4's output XORed with in, length
// bytes to out; context keeps i and j
void CRYPTO_RC4Encrypt(CRYPTORC4Context *context, const void *in, u32 length, void *out) {
    const u8 *src = (const u8 *)in;
    u8 *dest = (u8 *)out;
    u8 *s = context->s;
    u8 i = context->i;
    u8 j = context->j;

    while (length-- != 0) {
        u8 x;
        u8 y;

        i++;
        x = s[i];
        j += x;
        y = s[j];
        s[i] = y;
        s[j] = x;
        *dest++ = *src++ ^ s[(u8)(x + y)];
    }
    context->i = i;
    context->j = j;
}
#else
asm void CRYPTO_RC4Encrypt(CRYPTORC4Context *context, const void *in, u32 length, void *out) {
    stmdb sp!, {r4, r5, r6, r7, r8, r9, lr}
    ldrb r7, [r0, #0]
    ldrb r6, [r0, #1]
    add r0, r0, #4
    mov lr, #0x1000000
    add r7, lr, r7, lsl #24
    mov r6, r6, lsl #24
    ldrb r9, [r0, r7, lsr #24]
    subs r2, r2, #1
    bmi @end
@loop:
    add r6, r6, r9, lsl #24
    subs r2, r2, #1
    ldrb r8, [r0, r6, lsr #24]
    ldrb r5, [r1], #1
    strb r8, [r0, r7, lsr #24]
    strb r9, [r0, r6, lsr #24]
    add r4, r9, r8
    and r4, r4, #0xff
    add r7, r7, lr
    ldrb r4, [r0, r4]
    ldrb r9, [r0, r7, lsr #24]
    eor r5, r5, r4
    strb r5, [r3], #1
    bge @loop
@end:
    sub r7, r7, lr
    mov r7, r7, lsr #24
    mov r6, r6, lsr #24
    strb r7, [r0, #-4]
    strb r6, [r0, #-3]
    ldmia sp!, {r4, r5, r6, r7, r8, r9, pc}
}
#endif
