// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/mi/src/mi_swap.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py). Black lacks MI_SwapByte: the linker dead-stripped it.
#include <nitro/types.h>
#include <nitro/mi/swap.h>

#include <nitro/code32.h>

#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm below): the swap, without swp's atomicity
u32 MI_SwapWord(u32 setData, vu32 *destp)
{
    u32 old = *destp;

    *destp = setData;
    return old;
}
#else
asm u32 MI_SwapWord (register u32 setData, register vu32 * destp)
{
    swp r0, r0, [r1]
    bx lr
}
#endif

#include <nitro/codereset.h>
