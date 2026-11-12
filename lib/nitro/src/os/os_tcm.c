// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_tcm.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/os.h>
#include <nitro/code32.h>

#define OSi_TCM_REGION_SIZE_MASK  HW_C9_TCMR_SIZE_MASK
#define OSi_TCM_REGION_BASE_MASK  HW_C9_TCMR_BASE_MASK

asm u32 OS_GetDTCMAddress (void)
{
    mrc p15, 0, r0, c9, c1, 0
    ldr r1, = OSi_TCM_REGION_BASE_MASK
    and r0, r0, r1
    bx lr
}
#include <nitro/codereset.h>
