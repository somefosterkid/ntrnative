#include "global.h"

#include "bg_window.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02009D48.h"

typedef struct UnkStruct_ov96_0220D360_arg1 {
    SpriteSystem *unk_0;
    BgConfig *unk_4;
    s32 unk_8;
    s8 unk_C;
    s8 unk_D;
    u8 unk_E[0x2];
    s32 unk_10;
} UnkStruct_ov96_0220D360_arg1;

void ov96_0220D360(SysTask *arg0, void *param);

void ov96_0220D360(SysTask *arg0, void *param) {
    UnkStruct_ov96_0220D360_arg1 *arg1 = param;
    s32 temp_r4;
    s32 var_r1;
    s8 temp_r0;

    if (arg1->unk_10 == 0) {
        var_r1 = 1;
    } else {
        var_r1 = -1;
    }
    temp_r4 = arg1->unk_C * var_r1;
    ScheduleSetBgPosText(arg1->unk_4, 1U, BG_POS_OP_SET_XSCALE, temp_r4 + 8);
    ScheduleSetBgPosText(arg1->unk_4, 0U, BG_POS_OP_SET_XSCALE, temp_r4);
    G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(arg1->unk_0), 0, (0 - temp_r4) << 0xC);
    temp_r0 = arg1->unk_D;
    if ((temp_r0 % 2) == 1) {
        arg1->unk_C = (s8)(arg1->unk_C - 1);
    }
    arg1->unk_10 = (s32)(arg1->unk_10 ^ 1);
    arg1->unk_D = (s8)(arg1->unk_D - 1);
    if ((s32) arg1->unk_D <= 0) {
        ScheduleSetBgPosText(arg1->unk_4, 1U, BG_POS_OP_SET_XSCALE, 8);
        ScheduleSetBgPosText(arg1->unk_4, 0U, BG_POS_OP_SET_XSCALE, 0);
        G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(arg1->unk_0), 0, 0);
        arg1->unk_8 = 0;
        SysTask_Destroy(arg0);
    }
}
