// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/mi/src/mi_uncompress.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/types.h>
#include <nitro/mi/uncompress.h>

#include <nitro/code32.h>

#ifndef __MWERKS__
// C for a PC build: MWCC, which defines __MWERKS__, compiles the asm (the SDK's) under #else. LZ77 of type 0x10, and
// 0x11 (a header's low nibble set: lengths up to 0x10110 in one to three nibbles); a byte at a time
void MI_UncompressLZ8(const void *srcp, void *destp)
{
    const u8 *src = (const u8 *)srcp;
    u8 *dest = (u8 *)destp;
    u32 header = *(const u32 *)srcp;
    s32 destCount = (s32)(header >> 8);
    BOOL exFormat = (header & 0x0F) != 0;

    src += 4;
    while (destCount > 0) {
        u32 flags = *src++;
        int i;

        for (i = 0; i < 8 && destCount > 0; i++, flags <<= 1) {
            if (!(flags & 0x80)) {
                *dest++ = *src++;
                destCount--;
            } else {
                u32 b = src[0];
                u32 length;
                u32 offset;

                if (!exFormat) {
                    length = 3;
                } else if (b & 0xE0) {
                    length = 1;
                } else {
                    src++;
                    length = (b & 0xF) << 4;
                    if (b & 0x10) {
                        length <<= 8;
                        length += (u32)*src++ << 4;
                        length += 0x100;
                    }
                    length += 0x11;
                    b = src[0];
                }
                length += b >> 4;
                offset = (((b & 0xF) << 8) | src[1]) + 1;
                src += 2;
                destCount -= (s32)length;
                do {
                    *dest = *(dest - offset);
                    dest++;
                } while (--length > 0);
            }
        }
    }
}
#else
asm void MI_UncompressLZ8 (register const void * srcp, register void * destp)
{
    stmfd sp !, {r4 - r7, lr}
    ldr r5, [r0], #4
    mov r2, r5, lsr #8
    mov r7, #0
    tst r5, #0x0F
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    bne @movne1
    b @movne2
@movne1:
    mov r7, #1
@movne2:
#else
    movne r7, #1
#endif
@21:
    cmp r2, #0
    ble @26
    ldrb r14, [r0], #1
    mov r4, #8
@22:
    subs r4, r4, #1
    blt @21
    tst r14, #0x80
    bne @23
    ldrb r6, [r0], #1
    swpb r6, r6, [r1]
    add r1, r1, #1
    sub r2, r2, #1
    b  @25
@23:
    ldrb r5, [r0, #0]
    cmp r7, #0
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    beq @moveq3
    b @moveq4
@moveq3:
    mov r6, #3
@moveq4:
#else
    moveq r6, #3
#endif
    beq @23_2
    tst r5, #0xE0
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    bne @movne5
    b @movne6
@movne5:
    mov r6, #1
@movne6:
#else
    movne r6, #1
#endif
    bne @23_2
    add r0, r0, #1
    and r6, r5, #0xf
    mov r6, r6, lsl #4
    tst r5, #0x10
    beq @23_1
    mov r6, r6, lsl #8
    ldrb r5, [r0], #1
    add r6, r6, r5, lsl #4
    add r6, r6, #0x100
@23_1:
    add r6, r6, #0x11
    ldrb r5, [r0, #0]
@23_2:
    add r3, r6, r5, asr #4
    add r0, r0, #1
    and r5, r5, #0xf
    mov r12, r5, lsl #8
    ldrb r6, [r0], #1
    orr r5, r6, r12
    add r12, r5, #1
    sub r2, r2, r3
@24:
    ldrb r5, [r1, -r12]
    swpb r5, r5, [r1]
    add r1, r1, #1
    subs r3, r3, #1
    bgt @24
@25:
    cmp r2, #0
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    bgt @movgt7
    b @movgt8
@movgt7:
    mov r14, r14, lsl #1
@movgt8:
#else
    movgt r14, r14, lsl #1
#endif
    bgt @22
    b @21
@26:
    ldmfd sp !, {r4 - r7, lr}
    bx lr
}
#endif

#include <nitro/code16.h>

#include <nitro/code32.h>

#include <nitro/codereset.h>
