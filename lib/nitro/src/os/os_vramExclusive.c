// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_vramExclusive.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/os/ARM9/vramExclusive.h>
#include <nitro/os/common/interrupt.h>

static u32 OSi_vramExclusive;
static u16 OSi_vramLockId[OS_VRAM_BANK_KINDS];

#include <nitro/code32.h>

#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else): clz, 32 for 0
static u32 OsCountZeroBits(u32 bitmap)
{
    u32 n = 0;

    if (bitmap == 0) {
        return 32;
    }
    while (!(bitmap & 0x80000000)) {
        bitmap <<= 1;
        n++;
    }
    return n;
}
#else
static asm u32 OsCountZeroBits (u32 bitmap)
{
    clz r0, r0
    bx lr
}
#endif

#include <nitro/codereset.h>

void OSi_InitVramExclusive (void)
{
    s32 i;

    OSi_vramExclusive = 0x0000;

    for(i = 0 ; i < OS_VRAM_BANK_KINDS ; i++) {
        OSi_vramLockId[i] = 0;
    }
}

void OSi_UnlockVram (u16 bank, u16 lockId)
{
    u32 workMap;
    s32 zeroBits;
    OSIntrMode enabled = OS_DisableInterrupts();

    workMap = (u32)(bank & OSi_vramExclusive & OS_VRAM_BANK_ID_ALL);
    while (TRUE) {
        zeroBits = (s32)(31 - OsCountZeroBits((u32)workMap));
        if (zeroBits < 0) {
            break;
        }
        workMap &= ~(0x00000001 << zeroBits);
        if (OSi_vramLockId[zeroBits] == lockId) {
            OSi_vramLockId[zeroBits] = 0;
            OSi_vramExclusive &= ~(0x00000001 << zeroBits);
        }
    }

    (void)OS_RestoreInterrupts(enabled);
}
