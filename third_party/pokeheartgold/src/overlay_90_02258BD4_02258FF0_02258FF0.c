#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "system.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"

void ov90_02258FF0(UnkStruct_ov114_0225CEF0 *param0, GraphicsModes *param1, UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

void ov90_02258FF0(UnkStruct_ov114_0225CEF0 *param0, GraphicsModes *param1, UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID) {
    u32 sp0;
    UnkStruct_ov114_02260284 *var_r4;
    UnkStruct_ov114_02260284 *var_r6;
    u32 var_r7;
    unsigned int new_var;
    var_r6 = param2;
    sp0 = param3;
    SetBothScreensModesAndDisable(param1);
    param0->unk_00 = BgConfig_Alloc(heapID);
    param0->unk_04 = var_r6;
    param0->unk_08 = sp0;
    gSystem.screensFlipped = 0;
    new_var = 0U;
    GfGfx_SwapDisplay();
    var_r7 = 0;
    if (sp0 > new_var) {
        var_r4 = var_r6;
        do {
            InitBgFromTemplate(param0->unk_00, (u8)var_r4->unk_00, &var_r6->unk_04, 0U);
            BG_ClearCharDataRange((u8)var_r4->unk_00, 0x20U, 0U, heapID);
            BgClearTilemapBufferAndCommit(param0->unk_00, (u8)var_r4->unk_00);
            BgSetPosTextAndCommit(param0->unk_00, (u8)var_r4->unk_00, BG_POS_OP_SET_ROT, 0);
            BgSetPosTextAndCommit(param0->unk_00, (u8)var_r4->unk_00, BG_POS_OP_SET_XSCALE, 0);
            var_r7 += 1;
            var_r6 = (void *)(((u8 *)var_r6) + 0x20);
            var_r4 = (void *)(((u8 *)var_r4) + 0x20);
        } while (sp0 > var_r7);
    }
}
