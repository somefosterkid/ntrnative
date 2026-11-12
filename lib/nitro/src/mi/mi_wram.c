// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/mi/src/mi_wram.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/mi/wram.h>
#include <nitro/misc.h>

#define MIi_WRAM_ASSERT(x) \
    SDK_ASSERT((x) == MI_WRAM_ARM9_ALL || \
               (x) == MI_WRAM_1616_1 || \
               (x) == MI_WRAM_1616_2 || \
               (x) == MI_WRAM_ARM7_ALL)

    #ifdef SDK_CW_WARNOFF_SAFESTRB
        #include <nitro/code32.h>
    #endif

    void MI_SetWramBank (MIWram cnt)
    {
        MIi_WRAM_ASSERT(cnt);
        reg_GX_VRAMCNT_WRAM = (u8)cnt;
    }

    #ifdef SDK_CW_WARNOFF_SAFESTRB
        #include <nitro/codereset.h>
    #endif
