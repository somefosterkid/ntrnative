// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/src/math/sha1_block.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// NitroSDK's libmath SHA-1 compression, MATHi_SHA1ProcessBlock (the SDK's name, as kh-recoded-decomp has it; the file
// split and name are a guess: the code is hand-written ARM that follows math.c's quicksort and that math_sha1.c calls
// through a pointer). MATHi_SHA1ProcessBlock loads the context's 64-byte block as big-endian words, expands them in a
// 16-word ring on the stack, runs the 80 rounds in four loops of 20 and adds the result to the state.
//
// The four round constants are in .text, in the 16 bytes in front of MATHi_SHA1ProcessBlock (0x02081e6c..0x02081e7b,
// where the SDK's assembly file has them), and the rounds reach them by pc-relative loads. MWCC can't place data ahead
// of a function's symbol, so they are an asm function of their own, data_02081e6c, which is never called: the dcd words
// are its whole body (symbols.txt has it as an `unknown` ARM function, so dsd doesn't analyze the words as code), and
// MATHi_SHA1ProcessBlock's loads use pc-relative offsets to them.
#include "nitro/math.h"

#pragma thumb off
// clang-format off
#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): the four round constants, an array
// of the C below
static const u32 data_02081e6c[4] = {0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6};
#else
asm void data_02081e6c(void) {
    dcd 0x5a827999
    dcd 0x6ed9eba1
    dcd 0x8f1bbcdc
    dcd 0xca62c1d6
}
#endif
// clang-format on

// Hand-written assembly in the SDK, so an MWCC asm function here
// pokebw2's sha1_block
#ifndef __MWERKS__
static u32 SHA1Rotl(u32 x, int n) {
    return (x << n) | (x >> (32 - n));
}

// C for a PC build: the block's big-endian words in a 16-word ring, 80 rounds, the result added to the state
void MATHi_SHA1ProcessBlock(MATHSHA1Context *context) {
    u32 w[16];
    u32 a, b, c, d, e, f, t;
    int i;

    for (i = 0; i < 16; i++) {
        const u8 *p = &context->block.bytes[i * 4];

        w[i] = ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
    }
    a = context->state[0];
    b = context->state[1];
    c = context->state[2];
    d = context->state[3];
    e = context->state[4];
    for (i = 0; i < 80; i++) {
        if (i >= 16) {
            t = w[(i - 16) & 15] ^ w[(i - 14) & 15] ^ w[(i - 8) & 15] ^ w[(i - 3) & 15];
            w[i & 15] = SHA1Rotl(t, 1);
        }
        if (i < 20) {
            f = (b & c) | (~b & d);
        } else if (i < 40 || i >= 60) {
            f = b ^ c ^ d;
        } else {
            f = (b & c) | (b & d) | (c & d);
        }
        t = SHA1Rotl(a, 5) + f + e + data_02081e6c[i / 20] + w[i & 15];
        e = d;
        d = c;
        c = SHA1Rotl(b, 30);
        b = a;
        a = t;
    }
    context->state[0] += a;
    context->state[1] += b;
    context->state[2] += c;
    context->state[3] += d;
    context->state[4] += e;
}
#else
asm void MATHi_SHA1ProcessBlock(MATHSHA1Context *context) {
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}
    mov r11, r0
    sub sp, sp, #0x40
    mov ip, sp
    mvn r3, #0xff00
    add r8, r11, #0x14
    mov r1, ip
    ldmia r8!, {r4, r5, r6, r7}
    mov r9, #0x10
byte_swap:
    eor r2, r4, r4, ror #0x10
    and r2, r3, r2, lsr #0x8
    eor r4, r2, r4, ror #0x8
    eor r2, r5, r5, ror #0x10
    and r2, r3, r2, lsr #0x8
    eor r5, r2, r5, ror #0x8
    eor r2, r6, r6, ror #0x10
    and r2, r3, r2, lsr #0x8
    eor r6, r2, r6, ror #0x8
    eor r2, r7, r7, ror #0x10
    and r2, r3, r2, lsr #0x8
    eor r7, r2, r7, ror #0x8
    stmia r1!, {r4, r5, r6, r7}
    subs r9, r9, #0x4
    ldmneia r8!, {r4, r5, r6, r7}
    bne byte_swap
    ldmia r11, {r4, r5, r6, r7, r8}
    ldr ip, [pc, #-0x80]
    mov r9, #0x0
rounds_0_15:
    and r10, r5, r6
    mvn lr, r5
    and lr, lr, r7
    orr r10, r10, lr
    add r10, r10, ip
    and lr, r9, #0xf
    ldr lr, [sp, lr, lsl #0x2]
    add r10, r10, r8
    add r10, r10, lr
    add r10, r10, r4, ror #0x1b
    mov r8, r7
    mov r7, r6
    mov r6, r5, ror #0x2
    mov r5, r4
    mov r4, r10
    add r9, r9, #0x1
    cmp r9, #0x10
    bne rounds_0_15
rounds_16_19:
    and r10, r5, r6
    mvn lr, r5
    and lr, lr, r7
    orr r10, r10, lr
    add r10, r10, ip
    sub r2, r9, #0x10
    and r2, r2, #0xf
    sub lr, r9, #0xe
    and lr, lr, #0xf
    ldr r3, [sp, r2, lsl #0x2]
    ldr r1, [sp, lr, lsl #0x2]
    sub lr, r9, #0x8
    eor r3, r3, r1
    and lr, lr, #0xf
    ldr r1, [sp, lr, lsl #0x2]
    sub r2, r9, #0x3
    eor r3, r3, r1
    and r2, r2, #0xf
    ldr r1, [sp, r2, lsl #0x2]
    and r2, r9, #0xf
    eor r3, r3, r1
    mov r3, r3, ror #0x1f
    str r3, [sp, r2, lsl #0x2]
    and lr, r9, #0xf
    ldr lr, [sp, lr, lsl #0x2]
    add r10, r10, r8
    add r10, r10, lr
    add r10, r10, r4, ror #0x1b
    mov r8, r7
    mov r7, r6
    mov r6, r5, ror #0x2
    mov r5, r4
    mov r4, r10
    add r9, r9, #0x1
    cmp r9, #0x14
    bne rounds_16_19
    ldr ip, [pc, #-0x15c]
rounds_20_39:
    eor r10, r5, r6
    eor r10, r10, r7
    add r10, r10, ip
    sub r2, r9, #0x10
    and r2, r2, #0xf
    sub lr, r9, #0xe
    and lr, lr, #0xf
    ldr r3, [sp, r2, lsl #0x2]
    ldr r1, [sp, lr, lsl #0x2]
    sub lr, r9, #0x8
    eor r3, r3, r1
    and lr, lr, #0xf
    ldr r1, [sp, lr, lsl #0x2]
    sub r2, r9, #0x3
    eor r3, r3, r1
    and r2, r2, #0xf
    ldr r1, [sp, r2, lsl #0x2]
    and r2, r9, #0xf
    eor r3, r3, r1
    mov r3, r3, ror #0x1f
    str r3, [sp, r2, lsl #0x2]
    and lr, r9, #0xf
    ldr lr, [sp, lr, lsl #0x2]
    add r10, r10, r8
    add r10, r10, lr
    add r10, r10, r4, ror #0x1b
    mov r8, r7
    mov r7, r6
    mov r6, r5, ror #0x2
    mov r5, r4
    mov r4, r10
    add r9, r9, #0x1
    cmp r9, #0x28
    bne rounds_20_39
    ldr ip, [pc, #-0x1e4]
rounds_40_59:
    and r10, r5, r6
    and lr, r5, r7
    orr r10, r10, lr
    and lr, r6, r7
    orr r10, r10, lr
    add r10, r10, ip
    sub r2, r9, #0x10
    and r2, r2, #0xf
    sub lr, r9, #0xe
    and lr, lr, #0xf
    ldr r3, [sp, r2, lsl #0x2]
    ldr r1, [sp, lr, lsl #0x2]
    sub lr, r9, #0x8
    eor r3, r3, r1
    and lr, lr, #0xf
    ldr r1, [sp, lr, lsl #0x2]
    sub r2, r9, #0x3
    eor r3, r3, r1
    and r2, r2, #0xf
    ldr r1, [sp, r2, lsl #0x2]
    and r2, r9, #0xf
    eor r3, r3, r1
    mov r3, r3, ror #0x1f
    str r3, [sp, r2, lsl #0x2]
    and lr, r9, #0xf
    ldr lr, [sp, lr, lsl #0x2]
    add r10, r10, r8
    add r10, r10, lr
    add r10, r10, r4, ror #0x1b
    mov r8, r7
    mov r7, r6
    mov r6, r5, ror #0x2
    mov r5, r4
    mov r4, r10
    add r9, r9, #0x1
    cmp r9, #0x3c
    bne rounds_40_59
    ldr ip, [pc, #-0x278]
rounds_60_79:
    eor r10, r5, r6
    eor r10, r10, r7
    add r10, r10, ip
    sub r2, r9, #0x10
    and r2, r2, #0xf
    sub lr, r9, #0xe
    and lr, lr, #0xf
    ldr r3, [sp, r2, lsl #0x2]
    ldr r1, [sp, lr, lsl #0x2]
    sub lr, r9, #0x8
    eor r3, r3, r1
    and lr, lr, #0xf
    ldr r1, [sp, lr, lsl #0x2]
    sub r2, r9, #0x3
    eor r3, r3, r1
    and r2, r2, #0xf
    ldr r1, [sp, r2, lsl #0x2]
    and r2, r9, #0xf
    eor r3, r3, r1
    mov r3, r3, ror #0x1f
    str r3, [sp, r2, lsl #0x2]
    and lr, r9, #0xf
    ldr lr, [sp, lr, lsl #0x2]
    add r10, r10, r8
    add r10, r10, lr
    add r10, r10, r4, ror #0x1b
    mov r8, r7
    mov r7, r6
    mov r6, r5, ror #0x2
    mov r5, r4
    mov r4, r10
    add r9, r9, #0x1
    cmp r9, #0x50
    bne rounds_60_79
    ldmia r11, {r1, r2, r3, r9, r10}
    add r1, r1, r4
    add r2, r2, r5
    add r3, r3, r6
    add r9, r9, r7
    add r10, r10, r8
    stmia r11, {r1, r2, r3, r9, r10}
    add sp, sp, #0x40
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, pc}
}
#endif
#pragma thumb on
