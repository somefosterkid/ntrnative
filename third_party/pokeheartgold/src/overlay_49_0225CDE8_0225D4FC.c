#include "global.h"

#include "error_handling.h"
#include "gf_gfx_loader.h"

void ov49_0225D4FC(s32 arg0, s32 arg1, enum HeapID arg2);

void ov49_0225D4FC(s32 arg0, s32 arg1, enum HeapID heapID) {
    u32 v0;

    v0 = arg0 + (arg1 * 5);
    if (v0 >= 0x19U) {
        GF_AssertFail();
    }
    GfGfxLoader_LoadFromNarc(NARC_a_2_0_0, v0 + 1, 0, heapID, 1);
}
