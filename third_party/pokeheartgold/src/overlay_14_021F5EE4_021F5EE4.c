#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "bg_window.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F5C94(void *, s32, int);

void ov14_021F5EE4(UnkStruct_ov14_boxapp *arg0, void *arg1, u32 arg2) {
    u16 sp14[2];
    s32 sp10;
    void *spC;
    s32 temp_r4;
    int new_var2;
    u32 var_r4;
    u32 var_r6;
    void *temp_r2;
    u16 *new_var;
    spC = arg1;
    var_r6 = arg2;
    sub_02019B44(arg0->sys->unk_2F0, 3, &sp14[1], &sp14[0]);
    var_r4 = 0;
    new_var2 = 0U;
    if (var_r6 > new_var2) {
        sp10 = 1;
        sp10 = var_r6 - sp10;
        do {
            temp_r2 = (void *)(spC + ((sp10 - var_r4) * 4));
            if ((*(new_var = (u16 *)(((u8 *)temp_r2) + 2))) == 0) {
                ov14_021F5950(arg0, 0x21 - var_r4, *((u16 *)temp_r2), 0);
                sub_020199F4(arg0->sys->unk_2F0, 7 - var_r4, new_var2, new_var2, (u8)((u8)(sp14[1])), (u8)((u8)(sp14[0])), 0xCU);
            } else {
                ov14_021F5C94(arg0, 0x21 - var_r4, *((u16 *)temp_r2));
                sub_020199F4(arg0->sys->unk_2F0, 7 - var_r4, new_var2, new_var2, (u8)((u8)(sp14[1])), (u8)((s32)((s32)(sp14[0]))), 2U);
            }
            var_r4 += 1;
        } while (var_r4 < var_r6);
    }
    if (var_r6 < 5U) {
        do {
            temp_r4 = 0x21 - var_r6;
            FillWindowPixelBuffer(&arg0->sys->windows[temp_r4], 0U);
            CopyWindowPixelsToVram_TextMode(&arg0->sys->windows[temp_r4]);
            var_r6 += 1;
        } while (var_r6 < 5U);
    }
}
