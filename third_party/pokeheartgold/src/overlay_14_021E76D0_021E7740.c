#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021E7740(void *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4);

void ov14_021E7740(void *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4) {
    NNSG2dScreenData *sp30;
    s16 sp2C;
    void *sp28;
    NNSG2dScreenData *sp24;
    int sp20;
    s32 sp1C;
    s32 sp18;
    u32 sp14;
    void *sp10;
    u32 var_r4;
    u8 temp_r0;
    u8 var_r5;
    u8 var_r7;
    void *var_r6;

    sp10 = arg0;
    sp14 = arg2;
    sp18 = arg3;
    sp28 = GfGfxLoader_GetScrnData(NARC_a_0_1_9, 0xF, 1, &sp30, HEAP_ID_10);
    sp24 = sp30;
    sp24 = (struct NNSG2dScreenData *)(sp30->rawData);
    sp20 = 0;
    sp1C = arg4 << 0xC;
    do {
        var_r6 = (u8 *)((u16 *)sp24 + 0x15 * sp20);
        var_r4 = sp14;
        var_r5 = 0;
        var_r7 = sp20 + 1;
loop_2:
        sp2C = sp18 + (sp1C + (*(u16 *)(var_r6 + (var_r5 * 2)) & 0xFFF));
        LoadRectToBgTilemapRect(((UnkStruct_ov14_boxapp *)sp10)->sys->unk_14, 3U, &sp2C, (u8)var_r4, var_r7, 1U, 1U);
        var_r4 += 1;
        if (var_r4 >= 0x40U) {
            var_r4 = 0;
        }
        var_r5 += 1;
        if ((u32)var_r5 < 0x15U) {
            goto loop_2;
        }
        temp_r0 = sp20 + 1;
        sp20 = (s32)temp_r0;
    } while ((u32)temp_r0 < 0x14U);
    Heap_Free(sp28);
    FillBgTilemapRect(((UnkStruct_ov14_boxapp *)sp10)->sys->unk_14, 3U, 0x1001U, (u8)var_r4, 0U, 2U, 0x14U, 0x11U);
}
