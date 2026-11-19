#include "global.h"

#include "bg_window.h"

typedef struct UnkStruct_ov96_02203CE4 {
    u8 unk_0[0x8];
    BgConfig *unk_8;
    u8 unk_C[0x64];
    u8 unk_70;
    u8 unk_71;
} UnkStruct_ov96_02203CE4;

void ov96_02203CE4(UnkStruct_ov96_02203CE4 *arg0);

void ov96_02203CE4(UnkStruct_ov96_02203CE4 *arg0) {
    s32 var_r4;
    u8 temp_r0;

    if (arg0->unk_70 != 0) {
        arg0->unk_71 = (u8)(arg0->unk_71 + 1);
        temp_r0 = arg0->unk_71;
        var_r4 = 0 - ((temp_r0 % 2) * 8);
        if ((u32)temp_r0 >= 0xAU) {
            var_r4 = 0;
            arg0->unk_70 = 0U;
            arg0->unk_71 = 0U;
        }
        ScheduleSetBgPosText(arg0->unk_8, 4U, BG_POS_OP_SET_XSCALE, var_r4);
        ScheduleSetBgPosText(arg0->unk_8, 5U, BG_POS_OP_SET_XSCALE, var_r4);
        ScheduleSetBgPosText(arg0->unk_8, 6U, BG_POS_OP_SET_XSCALE, var_r4);
        return;
    }
    ScheduleSetBgPosText(arg0->unk_8, 4U, BG_POS_OP_SET_XSCALE, 0);
    ScheduleSetBgPosText(arg0->unk_8, 5U, BG_POS_OP_SET_XSCALE, 0);
    ScheduleSetBgPosText(arg0->unk_8, 6U, BG_POS_OP_SET_XSCALE, 0);
}
