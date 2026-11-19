#include "global.h"

#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "heap.h"

extern s32 ov51_021E7DD8;

extern s32 ov51_021E7DF0;

extern s32 ov51_021E7E08;

extern s32 ov51_021E7E20;

typedef struct Words6 {
    u32 w[6];
} Words6;

void ov51_021E60F4(u32 arg0, s32 arg1, s32 arg2);

void ov51_021E60F4(u32 arg0, s32 arg1, s32 arg2) {
    Words6 sp4C;
    Words6 sp34;
    Words6 sp1C;
    Words6 sp4;
    NNSG2dPaletteData *sp0;
    s32 var_r1;
    void *temp_r4_5;
    void *temp_r5_5;

    if ((u8)(arg2 + 0xF9) <= 1) {
        if (arg1 != 0) {
            sp4C = *(Words6 *)&ov51_021E7E08;
            if (arg0 > 5) {
                GF_AssertFail();
                var_r1 = sp4C.w[0];
            } else {
                var_r1 = sp4C.w[arg0];
            }
        } else {
            var_r1 = 6;
        }
    } else if (arg2 == 10) {
        sp34 = *(Words6 *)&ov51_021E7DF0;
        if (arg0 > 5) {
            GF_AssertFail();
            var_r1 = sp34.w[0];
        } else {
            var_r1 = sp34.w[arg0];
        }
    } else if (arg2 == 11) {
        sp1C = *(Words6 *)&ov51_021E7E20;
        if (arg0 > 5) {
            GF_AssertFail();
            var_r1 = sp1C.w[0];
        } else {
            var_r1 = sp1C.w[arg0];
        }
    } else if (arg2 == 12) {
        sp4 = *(Words6 *)&ov51_021E7DD8;
        if (arg0 > 5) {
            GF_AssertFail();
            var_r1 = sp4.w[0];
        } else {
            var_r1 = sp4.w[arg0];
        }
    } else {
        GF_AssertFail();
        var_r1 = 0;
    }
    temp_r4_5 = GfGfxLoader_GetPlttData(NARC_a_0_4_9, var_r1, &sp0, HEAP_ID_25);
    DC_FlushRange(sp0->pRawData, 0x200);
    temp_r5_5 = sp0->pRawData;
    GXS_LoadBGPltt(temp_r5_5 + 0x20, 0x20, 0x100);
    GXS_LoadBGPltt(temp_r5_5 + 0x1E0, 0x1E0, 0x20);
    Heap_Free(temp_r4_5);
}
