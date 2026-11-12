// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/mi/src/mi_memory.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/types.h>
#include <nitro/mi/memory.h>

#define HALFW_CONDAL  0xe0000000
#define HALFW_CONDNE  0x10000000
#define HALFW_CONDEQ  0x00000000

#define HALFW_OFF_PL  0x00800000
#define HALFW_OFF_MI  0x00000000
#define HALFW_LOAD    0x00100000
#define HALFW_STORE   0x00000000
#define HALFW_RN(n)   ((n) << 16)
#define HALFW_RD(n)   ((n) << 12)

#define HALFW_DEF1    0x004000B0
#define HALFW_DEF2    0x014000B0

#define HALFW_IMM(n)   (((n) & 0xf) | (((n) & 0xf0) << 4))

#define HALFW_DCD(cond, d, n, offset, sign, ldst, def) \
    dcd (def) | (cond) | (sign) | (ldst) | HALFW_RN(n) | HALFW_RD(d) | HALFW_IMM(offset)

#define LDRH_AD1(cond, d, n, offset) \
    HALFW_DCD(cond, d, n, offset, HALFW_OFF_PL, HALFW_LOAD, HALFW_DEF1)

#define LDRH_AD2(cond, d, n, offset) \
    HALFW_DCD(cond, d, n, offset, HALFW_OFF_PL, HALFW_LOAD, HALFW_DEF2)

#define LDRH_AD3(cond, d, n, offset) \
    HALFW_DCD(cond, d, n, offset, HALFW_OFF_MI, HALFW_LOAD, HALFW_DEF1)

#define LDRH_AD4(cond, d, n, offset) \
    HALFW_DCD(cond, d, n, offset, HALFW_OFF_MI, HALFW_LOAD, HALFW_DEF2)

#define STRH_AD1(cond, d, n, offset) \
    HALFW_DCD(cond, d, n, offset, HALFW_OFF_PL, HALFW_STORE, HALFW_DEF1)

#define STRH_AD2(cond, d, n, offset) \
    HALFW_DCD(cond, d, n, offset, HALFW_OFF_PL, HALFW_STORE, HALFW_DEF2)

#define STRH_AD3(cond, d, n, offset) \
    HALFW_DCD(cond, d, n, offset, HALFW_OFF_MI, HALFW_STORE, HALFW_DEF1)

#define STRH_AD4(cond, d, n, offset) \
    HALFW_DCD(cond, d, n, offset, HALFW_OFF_MI, HALFW_STORE, HALFW_DEF2)

#include <nitro/code32.h>

#ifndef __MWERKS__
// C for a PC build: MWCC, which defines __MWERKS__, compiles the asm (the SDK's) under #else; each function of
// this file that is asm has its C twin so beside it
void MIi_CpuClear16(u16 data, void *destp, u32 size)
{
    u32 i;

    for (i = 0; i < size; i += 2) {
        *(u16 *)((u8 *)destp + i) = data;
    }
}
#else
asm void MIi_CpuClear16 (register u16 data, register void * destp, register u32 size)
{
    mov r3, #0
@00:
    cmp r3, r2
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    blt @strlth1
    b @strlth2
@strlth1:
    strh  r0, [r1, r3]
@strlth2:
    blt @addlt1
    b @addlt2
@addlt1:
    add r3, r3, #2
@addlt2:
#else
    strlth r0, [r1, r3]
    addlt r3, r3, #2
#endif
    blt @00
    bx lr
}
#endif

#ifndef __MWERKS__
void MIi_CpuCopy16(const void *srcp, void *destp, u32 size)
{
    u32 i;

    for (i = 0; i < size; i += 2) {
        *(u16 *)((u8 *)destp + i) = *(const u16 *)((const u8 *)srcp + i);
    }
}
#else
asm void MIi_CpuCopy16 (register const void * srcp, register void * destp, register u32 size)
{
    mov r12, #0
@10:
    cmp r12, r2
#ifndef CW_BUG_FOR_LDRH_AND_STRH
    #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
        blt @ldrlth1
        b @ldrlth2
@ldrlth1:
        ldrh r3, [r0, r12]
@ldrlth2:
    #else
        ldrlth r3, [r0, r12]
    #endif
#else
    dcd 0xb19030bc
#endif
#ifndef CW_BUG_FOR_LDRH_AND_STRH
    #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
        blt @strlth1
        b @strlth2
@strlth1:
        strh r3, [r1, r12]
@strlth2:
    #else
        strlth r3, [r1, r12]
    #endif
#else
    dcd 0xb18130bc
#endif
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    blt @addlt1
    b @addlt2
@addlt1:
    add r12, r12, #2
@addlt2:
#else
    addlt r12, r12, #2
#endif
    blt @10
    bx lr
}
#endif

#ifndef __MWERKS__
void MIi_CpuClear32(u32 data, void *destp, u32 size)
{
    u32 *dest = (u32 *)destp;
    u32 *end = (u32 *)((u8 *)destp + size);

    while (dest < end) {
        *dest++ = data;
    }
}
#else
asm void MIi_CpuClear32 (register u32 data, register void * destp, register u32 size)
{
    add r12, r1, r2
@20:
    cmp r1, r12
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    blt @stmltia1
    b @stmltia2
@stmltia1:
    stmia r1!, {r0}
@stmltia2:
#else
    stmltia r1 !, {r0}
#endif
    blt @20
    bx lr
}
#endif

#ifndef __MWERKS__
void MIi_CpuCopy32(const void *srcp, void *destp, u32 size)
{
    const u32 *src = (const u32 *)srcp;
    u32 *dest = (u32 *)destp;
    u32 *end = (u32 *)((u8 *)destp + size);

    while (dest < end) {
        *dest++ = *src++;
    }
}
#else
asm void MIi_CpuCopy32 (register const void * srcp, register void * destp, register u32 size)
{
    add r12, r1, r2
@30:
    cmp r1, r12
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    blt @ldmltia1
    b @ldmltia2
@ldmltia1:
    ldmia r0!, {r2}
@ldmltia2:
    blt @stmltia1
    b @stmltia2
@stmltia1:
    stmia r1!, {r2}
@stmltia2:
#else
    ldmltia r0 !, {r2}
    stmltia r1 !, {r2}
#endif
    blt @30
    bx lr
}
#endif

#ifndef __MWERKS__
// every word to the one address (a FIFO port)
void MIi_CpuSend32(const void *srcp, volatile void *destp, u32 size)
{
    const u32 *src = (const u32 *)srcp;
    const u32 *end = (const u32 *)((const u8 *)srcp + size);

    while (src < end) {
        *(vu32 *)destp = *src++;
    }
}
#else
asm void MIi_CpuSend32 (register const void * srcp, volatile void * destp, u32 size)
{
    add r12, r0, r2
@31:
    cmp r0, r12
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    blt @ldmltia1
    b @ldmltia2
@ldmltia1:
    ldmia r0!, {r2}
@ldmltia2:
    blt @strlt1
    b @strlt2
@strlt1:
    str r2, [r1]
@strlt2:
#else
    ldmltia r0 !, {r2}
    strlt r2, [r1]
#endif
    blt @31
    bx lr
}
#endif

#ifndef __MWERKS__
// the asm stores 32 bytes at a time, then the words left
void MIi_CpuClearFast(u32 data, void *destp, u32 size)
{
    u32 *dest = (u32 *)destp;
    u32 *end = (u32 *)((u8 *)destp + size);

    while (dest < end) {
        *dest++ = data;
    }
}
#else
asm void MIi_CpuClearFast (register u32 data, register void * destp, register u32 size)
{
    stmfd sp !, {r4 - r9}
    add r9, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
    mov r2, r0
    mov r3, r2
    mov r4, r2
    mov r5, r2
    mov r6, r2
    mov r7, r2
    mov r8, r2
@40:
    cmp r1, r12
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    blt @stmltia1
    b @stmltia2
@stmltia1:
    stmia r1!, {r0, r2-r8}
@stmltia2:
#else
    stmltia r1 !, {r0, r2 - r8}
#endif
    blt @40
@41:
    cmp r1, r9
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    blt @stmltia3
    b @stmltia4
@stmltia3:
    stmia r1!, {r0}
@stmltia4:
#else
    stmltia r1 !, {r0}
#endif
    blt @41
    ldmfd sp !, {r4 - r9}
    bx lr
}
#endif

#ifndef __MWERKS__
void MIi_CpuCopyFast(const void *srcp, void *destp, u32 size)
{
    const u32 *src = (const u32 *)srcp;
    u32 *dest = (u32 *)destp;
    u32 *end = (u32 *)((u8 *)destp + size);

    while (dest < end) {
        *dest++ = *src++;
    }
}
#else
asm void MIi_CpuCopyFast (register const void * srcp, register void * destp, register u32 size)
{
    stmfd sp !, {r4 - r10}
    add r10, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
@50:
    cmp r1, r12
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    blt @ldmltia1
    b @ldmltia2
@ldmltia1:
    ldmia r0!, {r2-r9}
@ldmltia2:
    blt @stmltia1
    b @stmltia2
@stmltia1:
    stmia r1!, {r2-r9}
@stmltia2:
#else
    ldmltia r0 !, {r2 - r9}
    stmltia r1 !, {r2 - r9}
#endif
    blt @50
@51:
    cmp r1, r10
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    blt @ldmltia3
    b @ldmltia4
@ldmltia3:
    ldmia r0!, {r2}
@ldmltia4:
    blt @stmltia3
    b @stmltia4
@stmltia3:
    stmia r1!, {r2}
@stmltia4:
#else
    ldmltia r0 !, {r2}
    stmltia r1 !, {r2}
#endif
    blt @51
    ldmfd sp !, {r4 - r10}
    bx lr
}
#endif

#ifndef __MWERKS__
void MI_Copy32B(const void *pSrc, void *pDest)
{
    const u32 *src = (const u32 *)pSrc;
    u32 *dest = (u32 *)pDest;
    int i;

    for (i = 0; i < 32 / 4; i++) {
        dest[i] = src[i];
    }
}
#else
asm void MI_Copy32B (register const void * pSrc, register void * pDest)
{
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0 !, {r2, r3}
    stmia r1 !, {r2, r3}
    bx lr
}
#endif

#ifndef __MWERKS__
void MI_Copy36B(const void *pSrc, void *pDest)
{
    const u32 *src = (const u32 *)pSrc;
    u32 *dest = (u32 *)pDest;
    int i;

    for (i = 0; i < 36 / 4; i++) {
        dest[i] = src[i];
    }
}
#else
asm void MI_Copy36B (register const void * pSrc, register void * pDest)
{
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    bx lr
}
#endif

#ifndef __MWERKS__
void MI_Copy48B(const void *pSrc, void *pDest)
{
    const u32 *src = (const u32 *)pSrc;
    u32 *dest = (u32 *)pDest;
    int i;

    for (i = 0; i < 48 / 4; i++) {
        dest[i] = src[i];
    }
}
#else
asm void MI_Copy48B (register const void * pSrc, register void * pDest)
{
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    bx lr
}
#endif

#ifndef __MWERKS__
void MI_Copy64B(const void *pSrc, void *pDest)
{
    const u32 *src = (const u32 *)pSrc;
    u32 *dest = (u32 *)pDest;
    int i;

    for (i = 0; i < 64 / 4; i++) {
        dest[i] = src[i];
    }
}
#else
asm void MI_Copy64B (register const void * pSrc, register void * pDest)
{
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0 !, {r2, r3, r12}
    stmia r1 !, {r2, r3, r12}
    ldmia r0, {r0, r2, r3, r12}
    stmia r1 !, {r0, r2, r3, r12}
    bx lr
}
#endif

#pragma thumb off
#pragma thumb off
#ifndef __MWERKS__
// a byte at a time (the asm writes halfwords and words only, for VRAM, which takes no byte writes)
void MI_CpuFill8(void *dstp, u8 data, u32 size)
{
    u8 *dest = (u8 *)dstp;
    u32 i;

    for (i = 0; i < size; i++) {
        dest[i] = data;
    }
}
#else
asm void MI_CpuFill8 (register void * dstp, register u8 data, register u32 size)
{
    cmp r2, #0
    bxeq lr
    tst r0, #1
    beq @_1
    ldrh r12, [r0, #- 1]
    and r12, r12, #0x00FF
    orr r3, r12, r1, lsl #8
    strh r3, [r0, #- 1]
    add r0, r0, #1
    subs r2, r2, #1
    bxeq lr
@_1:
    cmp r2, #2
    bcc @_6
    orr r1, r1, r1, lsl #8
    tst r0, #2
    beq @_8
    strh r1, [r0], #2
    subs r2, r2, #2
    bxeq lr
@_8:
    orr r1, r1, r1, lsl #16
    bics r3, r2, #3
    beq @_10
    sub r2, r2, r3
    add r12, r3, r0
@_9:
    str r1, [r0], #4
    cmp r0, r12
    bcc @_9
@_10:
    tst r2, #2
    strneh r1, [r0], #2
@_6:
    tst r2, #1
    bxeq lr
    ldrh r3, [r0]
    and r3, r3, #0xFF00
    and r1, r1, #0x00FF
    orr r1, r1, r3
    strh r1, [r0]
    bx lr
}
#endif

#pragma thumb off
#ifndef __MWERKS__
// memset: data in each of size bytes
void MI_CpuFill(void *dest, u8 data, u32 size)
{
    u8 *d = (u8 *)dest;
    u32 i;

    for (i = 0; i < size; i++) {
        d[i] = data;
    }
}
#else
asm void MI_CpuFill(void *dest, u8 data, u32 size) {
    cmp r2, #0
    bxeq lr
    cmp r2, #8
    bgt @large
    rsb r3, r2, #8
    add pc, pc, r3, lsl #2
    mov r0, r0
    strb r1, [r0], #1
    strb r1, [r0], #1
    strb r1, [r0], #1
    strb r1, [r0], #1
    strb r1, [r0], #1
    strb r1, [r0], #1
    strb r1, [r0], #1
    strb r1, [r0], #1
    bx lr
@large:
    orr r1, r1, r1, lsl #8
    orr r1, r1, r1, lsl #16
    tst r0, #1
    subne r2, r2, #1
    strneb r1, [r0], #1
    tst r0, #2
    subne r2, r2, #2
    strneh r1, [r0], #2
    tst r0, #4
    subne r2, r2, #4
    strne r1, [r0], #4
    cmp r2, #0x20
    blt @words
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10}
    mov r4, r1
    mov r5, r1
    mov r6, r1
    mov r7, r1
    mov r8, r1
    mov r9, r1
    mov r10, r1
    subs r2, r2, #0x20
@block_loop:
    stmgeia r0!, {r1, r4, r5, r6, r7, r8, r9, r10}
    subges r2, r2, #0x20
    bge @block_loop
    add r2, r2, #0x20
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10}
@words:
    cmp r2, #4
    blt @bytes
    subs r2, r2, #4
@word_loop:
    strge r1, [r0], #4
    subs r2, r2, #4
    bge @word_loop
    add r2, r2, #4
@bytes:
    subs r2, r2, #1
    strgeb r1, [r0], #1
    subges r2, r2, #1
    strgeb r1, [r0], #1
    subges r2, r2, #1
    strgeb r1, [r0], #1
    bx lr
}
#endif

#ifndef __MWERKS__
// a byte at a time, forwards (the asm reads and writes halfwords and words only, for VRAM)
void MI_CpuCopy8(const void *srcp, void *dstp, u32 size)
{
    const u8 *src = (const u8 *)srcp;
    u8 *dest = (u8 *)dstp;
    u32 i;

    for (i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}
#else
    asm void MI_CpuCopy8 (register const void * srcp, register void * dstp, register u32 size)
    {
        cmp r2, #0
    #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
        beq @bxeq1
        b @bxeq2
@bxeq1:
        bx lr
@bxeq2:
    #else
        bxeq lr
    #endif
        tst r1, #1
        beq @_1
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        ldrh r12, [r1, #- 1]
    #else
        LDRH_AD4(HALFW_CONDAL, 12, 1, 1)
    #endif
        and r12, r12, #0x00FF
        tst r0, #1
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
            bne @ldrneh1
            b @ldrneh2
@ldrneh1:
            ldrh r3, [r0, #-1]
@ldrneh2:
        #else
            ldrneh r3, [r0, #- 1]
        #endif
    #else
        LDRH_AD4(HALFW_CONDNE, 3, 0, 1)
    #endif
    #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
        bne @movne1
        b @movne2
@movne1:
        mov r3, r3, lsr #8
@movne2:
    #else
        movne r3, r3, lsr #8
    #endif
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
            beq @ldreqh1
            b @ldreqh2
@ldreqh1:
            ldrh r3, [r0]
@ldreqh2:
        #else
            ldreqh r3, [r0]
        #endif
    #else
        LDRH_AD2(HALFW_CONDEQ, 3, 0, 0)
    #endif
        orr r3, r12, r3, lsl #8
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        strh r3, [r1, #- 1]
    #else
        STRH_AD4(HALFW_CONDAL, 3, 1, 1)
    #endif
        add r0, r0, #1
        add r1, r1, #1
        subs r2, r2, #1
    #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
        beq @bxeq3
        b @bxeq4
@bxeq3:
        bx lr
@bxeq4:
    #else
        bxeq lr
    #endif
    @_1:
        eor r12, r1, r0
        tst r12, #1
        beq @_2
        bic r0, r0, #1
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        ldrh r12, [r0], #2
    #else
        LDRH_AD1(HALFW_CONDAL, 12, 0, 2)
    #endif
        mov r3, r12, lsr #8
        subs r2, r2, #2
        bcc @_3
    @_4:
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        ldrh r12, [r0], #2
    #else
        LDRH_AD1(HALFW_CONDAL, 12, 0, 2)
    #endif
        orr r12, r3, r12, lsl #8
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        strh r12, [r1], #2
    #else
        STRH_AD1(HALFW_CONDAL, 12, 1, 2)
    #endif
        mov r3, r12, lsr #16
        subs r2, r2, #2
        bcs @_4
    @_3:
        tst r2, #1
    #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
        beq @bxeq5
        b @bxeq6
@bxeq5:
        bx lr
@bxeq6:
    #else
        bxeq lr
    #endif
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        ldrh r12, [r1]
    #else
        LDRH_AD2(HALFW_CONDAL, 12, 1, 0)
    #endif
        and r12, r12, #0xFF00
        orr r12, r12, r3
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        strh r12, [r1]
    #else
        STRH_AD2(HALFW_CONDAL, 12, 1, 0)
    #endif
        bx lr
    @_2:
        tst r12, #2
        beq @_5
        bics r3, r2, #1
        beq @_6
        sub r2, r2, r3
        add r12, r3, r1
    @_7:
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        ldrh r3, [r0], #2
    #else
        LDRH_AD1(HALFW_CONDAL, 3, 0, 2)
    #endif
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        strh r3, [r1], #2
    #else
        STRH_AD1(HALFW_CONDAL, 3, 1, 2)
    #endif
        cmp r1, r12
        bcc @_7
        b @_6
    @_5:
        cmp r2, #2
        bcc @_6
        tst r1, #2
        beq @_8
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        ldrh r3, [r0], #2
    #else
        LDRH_AD1(HALFW_CONDAL, 3, 0, 2)
    #endif
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        strh r3, [r1], #2
    #else
        STRH_AD1(HALFW_CONDAL, 3, 1, 2)
    #endif
        subs r2, r2, #2
    #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
        beq @bxeq7
        b @bxeq8
@bxeq7:
        bx lr
@bxeq8:
    #else
        bxeq lr
    #endif
    @_8:
        bics r3, r2, #3
        beq @_10
        sub r2, r2, r3
        add r12, r3, r1
    @_9:
        ldr r3, [r0], #4
        str r3, [r1], #4
        cmp r1, r12
        bcc @_9
    @_10:
        tst r2, #2
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
            bne @ldrneh3
            b @ldrneh4
@ldrneh3:
            ldrh r3, [r0], #2
@ldrneh4:
            bne @strneh1
            b @strneh2
@strneh1:
            strh r3, [r1], #2
@strneh2:
        #else
            ldrneh r3, [r0], #2
            strneh r3, [r1], #2
        #endif
    #else
        LDRH_AD1(HALFW_CONDNE, 3, 0, 2)
        STRH_AD1(HALFW_CONDNE, 3, 1, 2)
    #endif
    @_6:
        tst r2, #1
    #ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
        beq @bxeq9
        b @bxeq10
@bxeq9:
        bx lr
@bxeq10:
    #else
        bxeq lr
    #endif
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        ldrh r2, [r1]
        ldrh r0, [r0]
    #else
        LDRH_AD2(HALFW_CONDAL, 2, 1, 0)
        LDRH_AD2(HALFW_CONDAL, 0, 0, 0)
    #endif
        and r2, r2, #0xFF00
        and r0, r0, #0x00FF
        orr r0, r2, r0
    #ifndef CW_BUG_FOR_LDRH_AND_STRH
        strh r0, [r1]
    #else
        STRH_AD2(HALFW_CONDAL, 0, 1, 0)
    #endif
        bx lr
    }
#endif

#ifndef __MWERKS__
// memcmp: the first differing bytes' difference, 0 when the size bytes are the same
int sys_memcmp(const void *a, const void *b, u32 size)
{
    const u8 *p = (const u8 *)a;
    const u8 *q = (const u8 *)b;
    u32 i;

    for (i = 0; i < size; i++) {
        if (p[i] != q[i]) {
            return p[i] - q[i];
        }
    }
    return 0;
}
#else
asm int sys_memcmp(const void *a, const void *b, u32 size) {
    add r12, r0, r2
    cmp r0, r12
    bhs @equal
@loop:
    ldrb r3, [r0], #1
    ldrb r2, [r1], #1
    subs r2, r3, r2
    movne r0, r2
    bxne lr
    cmp r0, r12
    blo @loop
@equal:
    mov r0, #0
    bx lr
}
#endif

#ifndef __MWERKS__
// memcpy with the source first: forwards
void sys_memcpy_fast(const void *src, void *dest, u32 size)
{
    const u8 *s = (const u8 *)src;
    u8 *d = (u8 *)dest;
    u32 i;

    for (i = 0; i < size; i++) {
        d[i] = s[i];
    }
}
#else
asm void sys_memcpy_fast(const void *src, void *dest, u32 size) {
    cmp r2, #8
    bgt @large
    rsb r3, r2, #8
    add pc, pc, r3, lsl #3
    mov r0, r0
    ldrb r3, [r0], #1
    strb r3, [r1], #1
    ldrb r3, [r0], #1
    strb r3, [r1], #1
    ldrb r3, [r0], #1
    strb r3, [r1], #1
    ldrb r3, [r0], #1
    strb r3, [r1], #1
    ldrb r3, [r0], #1
    strb r3, [r1], #1
    ldrb r3, [r0], #1
    strb r3, [r1], #1
    ldrb r3, [r0], #1
    strb r3, [r1], #1
    ldrb r3, [r0], #1
    strb r3, [r1], #1
    bx lr
@large:
    tst r0, #1
    subne r2, r2, #1
    ldrneb r3, [r0], #1
    strneb r3, [r1], #1
    tst r0, #2
    subne r2, r2, #2
    ldrneb r3, [r0], #1
    strneb r3, [r1], #1
    ldrneb r3, [r0], #1
    strneb r3, [r1], #1
    and r3, r1, #3
    bic r1, r1, #3
    cmp r3, #0
    beq @dest_offset_0
    cmp r3, #1
    beq @dest_offset_1
    cmp r3, #2
    beq @dest_offset_2
    b @dest_offset_3
@dest_offset_0:
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #0x20
@dest_offset_0_block_loop:
    ldmgeia r0!, {r4, r5, r6, r7, r8, r9, r10, lr}
    stmgeia r1!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subges r2, r2, #0x20
    bge @dest_offset_0_block_loop
    add r2, r2, #0x20
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #4
@dest_offset_0_word_loop:
    ldrge r3, [r0], #4
    strge r3, [r1], #4
    subges r2, r2, #4
    bge @dest_offset_0_word_loop
    add r2, r2, #4
    b @tail
@dest_offset_1:
    ldr r12, [r1]
    mov r12, r12, lsl #0x18
    mov r12, r12, lsr #0x18
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #0x20
@dest_offset_1_block_loop:
    ldmgeia r0!, {r4, r5, r6, r7, r8, r9, r10, lr}
    movge r3, r4, lsr #0x18
    orrge r4, r12, r4, lsl #8
    movge r12, r5, lsr #0x18
    orrge r5, r3, r5, lsl #8
    movge r3, r6, lsr #0x18
    orrge r6, r12, r6, lsl #8
    movge r12, r7, lsr #0x18
    orrge r7, r3, r7, lsl #8
    movge r3, r8, lsr #0x18
    orrge r8, r12, r8, lsl #8
    movge r12, r9, lsr #0x18
    orrge r9, r3, r9, lsl #8
    movge r3, r10, lsr #0x18
    orrge r10, r12, r10, lsl #8
    movge r12, lr, lsr #0x18
    orrge lr, r3, lr, lsl #8
    stmgeia r1!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subges r2, r2, #0x20
    bge @dest_offset_1_block_loop
    add r2, r2, #0x20
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #4
@dest_offset_1_word_loop:
    ldrge r3, [r0], #4
    orrge r12, r12, r3, lsl #8
    strge r12, [r1], #4
    movge r12, r3, lsr #0x18
    subges r2, r2, #4
    bge @dest_offset_1_word_loop
    add r2, r2, #4
    sub r0, r0, #1
    add r2, r2, #1
    b @tail
@dest_offset_2:
    ldr r12, [r1]
    mov r12, r12, lsl #0x10
    mov r12, r12, lsr #0x10
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #0x20
@dest_offset_2_block_loop:
    ldmgeia r0!, {r4, r5, r6, r7, r8, r9, r10, lr}
    movge r3, r4, lsr #0x10
    orrge r4, r12, r4, lsl #16
    movge r12, r5, lsr #0x10
    orrge r5, r3, r5, lsl #16
    movge r3, r6, lsr #0x10
    orrge r6, r12, r6, lsl #16
    movge r12, r7, lsr #0x10
    orrge r7, r3, r7, lsl #16
    movge r3, r8, lsr #0x10
    orrge r8, r12, r8, lsl #16
    movge r12, r9, lsr #0x10
    orrge r9, r3, r9, lsl #16
    movge r3, r10, lsr #0x10
    orrge r10, r12, r10, lsl #16
    movge r12, lr, lsr #0x10
    orrge lr, r3, lr, lsl #16
    stmgeia r1!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subges r2, r2, #0x20
    bge @dest_offset_2_block_loop
    add r2, r2, #0x20
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #4
@dest_offset_2_word_loop:
    ldrge r3, [r0], #4
    orrge r12, r12, r3, lsl #16
    strge r12, [r1], #4
    movge r12, r3, lsr #0x10
    subges r2, r2, #4
    bge @dest_offset_2_word_loop
    add r2, r2, #4
    sub r0, r0, #2
    add r2, r2, #2
    b @tail
@dest_offset_3:
    ldr r12, [r1]
    mov r12, r12, lsl #8
    mov r12, r12, lsr #8
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #0x20
@dest_offset_3_block_loop:
    ldmgeia r0!, {r4, r5, r6, r7, r8, r9, r10, lr}
    movge r3, r4, lsr #8
    orrge r4, r12, r4, lsl #24
    movge r12, r5, lsr #8
    orrge r5, r3, r5, lsl #24
    movge r3, r6, lsr #8
    orrge r6, r12, r6, lsl #24
    movge r12, r7, lsr #8
    orrge r7, r3, r7, lsl #24
    movge r3, r8, lsr #8
    orrge r8, r12, r8, lsl #24
    movge r12, r9, lsr #8
    orrge r9, r3, r9, lsl #24
    movge r3, r10, lsr #8
    orrge r10, r12, r10, lsl #24
    movge r12, lr, lsr #8
    orrge lr, r3, lr, lsl #24
    stmgeia r1!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subges r2, r2, #0x20
    bge @dest_offset_3_block_loop
    add r2, r2, #0x20
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #4
@dest_offset_3_word_loop:
    ldrge r3, [r0], #4
    orrge r12, r12, r3, lsl #24
    strge r12, [r1], #4
    movge r12, r3, lsr #8
    subges r2, r2, #4
    bge @dest_offset_3_word_loop
    add r2, r2, #4
    sub r0, r0, #3
    add r2, r2, #3
    b @tail
@tail:
    tst r2, #4
    ldrneb r3, [r0], #1
    strneb r3, [r1], #1
    ldrneb r3, [r0], #1
    strneb r3, [r1], #1
    ldrneb r3, [r0], #1
    strneb r3, [r1], #1
    ldrneb r3, [r0], #1
    strneb r3, [r1], #1
    tst r2, #2
    ldrneb r3, [r0], #1
    strneb r3, [r1], #1
    ldrneb r3, [r0], #1
    strneb r3, [r1], #1
    tst r2, #1
    ldrneb r3, [r0], #1
    strneb r3, [r1], #1
    bx lr
}
#endif

#ifndef __MWERKS__
// memmove with the source first: forwards (sys_memcpy_fast) when the source is above the destination, else from the end
void sys_memcpy_ex(const void *src, void *dest, u32 size)
{
    const u8 *s = (const u8 *)src;
    u8 *d = (u8 *)dest;

    if (size == 0 || s == d) {
        return;
    }
    if (s > d) {
        sys_memcpy_fast(src, dest, size);
        return;
    }
    while (size != 0) {
        size--;
        d[size] = s[size];
    }
}
#else
asm void sys_memcpy_ex(const void *src, void *dest, u32 size) {
    cmp r2, #0
    subnes r3, r0, r1
    bxeq lr
    bgt sys_memcpy_fast ; EXTERNAL
    add r1, r1, r2
    add r0, r0, r2
    cmp r2, #8
    bgt @large
    rsb r3, r2, #8
    add pc, pc, r3, lsl #3
    mov r0, r0
    ldrb r3, [r0, #-1]!
    strb r3, [r1, #-1]!
    ldrb r3, [r0, #-1]!
    strb r3, [r1, #-1]!
    ldrb r3, [r0, #-1]!
    strb r3, [r1, #-1]!
    ldrb r3, [r0, #-1]!
    strb r3, [r1, #-1]!
    ldrb r3, [r0, #-1]!
    strb r3, [r1, #-1]!
    ldrb r3, [r0, #-1]!
    strb r3, [r1, #-1]!
    ldrb r3, [r0, #-1]!
    strb r3, [r1, #-1]!
    ldrb r3, [r0, #-1]!
    strb r3, [r1, #-1]!
    bx lr
@large:
    tst r0, #2
    subne r2, r2, #2
    ldrneb r3, [r0, #-1]!
    strneb r3, [r1, #-1]!
    ldrneb r3, [r0, #-1]!
    strneb r3, [r1, #-1]!
    tst r0, #1
    subne r2, r2, #1
    ldrneb r3, [r0, #-1]!
    strneb r3, [r1, #-1]!
    and r3, r1, #3
    bic r1, r1, #3
    cmp r3, #0
    beq @end_offset_0
    cmp r3, #1
    beq @end_offset_1
    cmp r3, #2
    beq @end_offset_2
    b @end_offset_3
@end_offset_0:
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #0x20
@end_offset_0_block_loop:
    ldmgedb r0!, {r4, r5, r6, r7, r8, r9, r10, lr}
    stmgedb r1!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subges r2, r2, #0x20
    bge @end_offset_0_block_loop
    add r2, r2, #0x20
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #4
@end_offset_0_word_loop:
    ldrge r3, [r0, #-4]!
    strge r3, [r1, #-4]!
    subges r2, r2, #4
    bge @end_offset_0_word_loop
    add r2, r2, #4
    b @tail
@end_offset_1:
    ldr r12, [r1]
    mov r12, r12, lsr #8
    mov r12, r12, lsl #8
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #0x20
@end_offset_1_block_loop:
    ldmgedb r0!, {r4, r5, r6, r7, r8, r9, r10, lr}
    movge r3, lr, lsl #8
    orrge lr, r12, lr, lsr #24
    movge r12, r10, lsl #8
    orrge r10, r3, r10, lsr #24
    movge r3, r9, lsl #8
    orrge r9, r12, r9, lsr #24
    movge r12, r8, lsl #8
    orrge r8, r3, r8, lsr #24
    movge r3, r7, lsl #8
    orrge r7, r12, r7, lsr #24
    movge r12, r6, lsl #8
    orrge r6, r3, r6, lsr #24
    movge r3, r5, lsl #8
    orrge r5, r12, r5, lsr #24
    movge r12, r4, lsl #8
    orrge r4, r3, r4, lsr #24
    stmgeda r1!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subges r2, r2, #0x20
    bge @end_offset_1_block_loop
    add r2, r2, #0x20
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #4
@end_offset_1_word_loop:
    ldrge r3, [r0, #-4]!
    orrge r12, r12, r3, lsr #24
    strge r12, [r1], #-4
    movge r12, r3, lsl #8
    subges r2, r2, #4
    bge @end_offset_1_word_loop
    add r2, r2, #4
    add r1, r1, #4
    add r0, r0, #3
    add r2, r2, #3
    b @tail
@end_offset_2:
    ldr r12, [r1]
    mov r12, r12, lsr #0x10
    mov r12, r12, lsl #0x10
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #0x20
@end_offset_2_block_loop:
    ldmgedb r0!, {r4, r5, r6, r7, r8, r9, r10, lr}
    movge r3, lr, lsl #0x10
    orrge lr, r12, lr, lsr #16
    movge r12, r10, lsl #0x10
    orrge r10, r3, r10, lsr #16
    movge r3, r9, lsl #0x10
    orrge r9, r12, r9, lsr #16
    movge r12, r8, lsl #0x10
    orrge r8, r3, r8, lsr #16
    movge r3, r7, lsl #0x10
    orrge r7, r12, r7, lsr #16
    movge r12, r6, lsl #0x10
    orrge r6, r3, r6, lsr #16
    movge r3, r5, lsl #0x10
    orrge r5, r12, r5, lsr #16
    movge r12, r4, lsl #0x10
    orrge r4, r3, r4, lsr #16
    stmgeda r1!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subges r2, r2, #0x20
    bge @end_offset_2_block_loop
    add r2, r2, #0x20
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #4
@end_offset_2_word_loop:
    ldrge r3, [r0, #-4]!
    orrge r12, r12, r3, lsr #16
    strge r12, [r1], #-4
    movge r12, r3, lsl #0x10
    subges r2, r2, #4
    bge @end_offset_2_word_loop
    add r2, r2, #4
    add r1, r1, #4
    add r0, r0, #2
    add r2, r2, #2
    b @tail
@end_offset_3:
    ldr r12, [r1]
    mov r12, r12, lsr #0x18
    mov r12, r12, lsl #0x18
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #0x20
@end_offset_3_block_loop:
    ldmgedb r0!, {r4, r5, r6, r7, r8, r9, r10, lr}
    movge r3, lr, lsl #0x18
    orrge lr, r12, lr, lsr #8
    movge r12, r10, lsl #0x18
    orrge r10, r3, r10, lsr #8
    movge r3, r9, lsl #0x18
    orrge r9, r12, r9, lsr #8
    movge r12, r8, lsl #0x18
    orrge r8, r3, r8, lsr #8
    movge r3, r7, lsl #0x18
    orrge r7, r12, r7, lsr #8
    movge r12, r6, lsl #0x18
    orrge r6, r3, r6, lsr #8
    movge r3, r5, lsl #0x18
    orrge r5, r12, r5, lsr #8
    movge r12, r4, lsl #0x18
    orrge r4, r3, r4, lsr #8
    stmgeda r1!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subges r2, r2, #0x20
    bge @end_offset_3_block_loop
    add r2, r2, #0x20
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    subs r2, r2, #4
@end_offset_3_word_loop:
    ldrge r3, [r0, #-4]!
    orrge r12, r12, r3, lsr #8
    strge r12, [r1], #-4
    movge r12, r3, lsl #0x18
    subges r2, r2, #4
    bge @end_offset_3_word_loop
    add r2, r2, #4
    add r1, r1, #4
    add r0, r0, #1
    add r2, r2, #1
    b @tail
@tail:
    tst r2, #4
    ldrneb r3, [r0, #-1]!
    strneb r3, [r1, #-1]!
    ldrneb r3, [r0, #-1]!
    strneb r3, [r1, #-1]!
    ldrneb r3, [r0, #-1]!
    strneb r3, [r1, #-1]!
    ldrneb r3, [r0, #-1]!
    strneb r3, [r1, #-1]!
    tst r2, #2
    ldrneb r3, [r0, #-1]!
    strneb r3, [r1, #-1]!
    ldrneb r3, [r0, #-1]!
    strneb r3, [r1, #-1]!
    tst r2, #1
    ldrneb r3, [r0, #-1]!
    strneb r3, [r1, #-1]!
    bx lr

}
#endif




#pragma thumb on
#ifndef __MWERKS__
// NitroSDK's MI_Zero36B: 36 bytes of zero (nnsys's g3d clears a 3x3 rotation with it)
void MI_Zero36B(void *dest)
{
    u32 *d = (u32 *)dest;
    int i;

    for (i = 0; i < 36 / 4; i++) {
        d[i] = 0;
    }
}
#else
asm void MI_Zero36B(void *dest) {
    mov r1, #0
    mov r2, #0
    mov r3, #0
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2, r3}
    bx lr
}
#endif

#pragma thumb reset
#pragma thumb reset

#pragma thumb reset
#pragma thumb reset



#include <nitro/codereset.h>
#include <nitro/code16.h>

#include <nitro/codereset.h>
