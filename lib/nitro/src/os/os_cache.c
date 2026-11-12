// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_cache.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/os.h>
#include <nitro/code32.h>

#define SDK_AVOID_FLUSH

asm void DC_FlushAll (void)
{
    mov r12, #0
    mov r1, #0
@1:
    mov r0, #0
@2:
    orr r2, r1, r0
    mcr p15, 0, r12, c7, c10, 4
    mcr p15, 0, r2, c7, c14, 2
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, #HW_DCACHE_SIZE / 4
    blt @2
    add r1, r1, #1 << HW_C7_CACHE_SET_NO_SHIFT
    cmp r1, #0
    bne @1
    bx lr
}

asm void DC_InvalidateRange (register void * startAddr, register u32 nBytes)
{
    add r1, r1, r0
    bic r0, r0, #HW_CACHE_LINE_SIZE - 1
@1:
    mcr p15, 0, r0, c7, c6, 1
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, r1
    blt @1
    bx lr
}

asm void DC_StoreRange (register const void * startAddr, register u32 nBytes)
{
    add r1, r1, r0
    bic r0, r0, #HW_CACHE_LINE_SIZE - 1
@1:
    mcr p15, 0, r0, c7, c10, 1
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, r1
    blt @1
    bx lr
}

asm void DC_FlushRange (register const void * startAddr, register u32 nBytes)
{
    mov r12, #0
    add r1, r1, r0
    bic r0, r0, #HW_CACHE_LINE_SIZE - 1
@1:
    mcr p15, 0, r12, c7, c10, 4
    mcr p15, 0, r0, c7, c14, 1
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, r1
    blt @1
    bx lr
}

asm void DC_WaitWriteBufferEmpty (void)
{
    mov r0, #0
    mcr p15, 0, r0, c7, c10, 4
    bx lr
}

asm void IC_InvalidateAll (void)
{
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0
    bx lr
}

asm void IC_InvalidateRange (register void * startAddr, register u32 nBytes)
{
    add r1, r1, r0
    bic r0, r0, #HW_CACHE_LINE_SIZE - 1
@1:
    mcr p15, 0, r0, c7, c5, 1
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, r1
    blt @1
    bx lr
}

#include <nitro/itcm_begin.h>

#include <nitro/itcm_end.h>

#include <nitro/codereset.h>
