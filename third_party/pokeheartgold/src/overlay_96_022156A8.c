#include "global.h"
#include "bg_window.h"
#include "error_handling.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02009D48.h"

#include "overlay_96_ov96_rd_work.h"

s32 ov96_022156A8(s32 arg0, s32 arg1, u8 *arg2);

u32 ov96_022156E8(VecFx32 *param_1, s32 param_2, VecFx32 *param_3, s32 param_4);
void ov96_02215710(SysTask *arg0, void *arg1);
u32 ov96_02215864(Ov96RdScroll *scroll);
void ov96_0221587C(Ov96RdScroll *scroll, BgConfig *bgConfig, SpriteSystem *spriteSystem);
void ov96_02215884(Ov96RdScroll *scroll, s32 param_2, s32 param_3);
s32 ov96_022158D4(s32 arg0, s32 arg1);
u32 ov96_022158EC(VecFx32 *param_1, VecFx32 *param_2, s32 param_3);

s32 ov96_022156A8(s32 arg0, s32 arg1, u8 *arg2) {
    s32 v0;
    s32 v1;
    s32 v2;
    u8 v3;

    v3 = 0;
loop_1:
    v0 = v3 * 4;
    v1 = arg0 - *(u16 *)(arg2 + v0);
    v2 = arg1 - *(u16 *)((u8 *)(arg2 + v0) + 2);
    if ((v1 >= -8) && (v1 <= 8) && (v2 >= -8) && (v2 <= 8)) {
        return 1;
    }
    v3 += 1;
    if ((u32)v3 >= 3U) {
        return 0;
    }
    goto loop_1;
}

u32 ov96_022156E8(VecFx32 *param_1, s32 param_2, VecFx32 *param_3, s32 param_4) {
    fx32 fVar1;
    VecFx32 VStack_18;

    VEC_Subtract(param_1, param_3, &VStack_18);
    fVar1 = VEC_Mag(&VStack_18);
    if (fVar1 < param_2 + param_4) {
        return 1;
    }
    return 0;
}

void ov96_02215710(SysTask *task, void *data) {
    Ov96RdScroll *scroll = data;
    s32 v0;
    s32 v1;
    s32 v2;
    u32 value;

    if (scroll->dir == 0) {
        v1 = 1;
    } else {
        v1 = -1;
    }
    value = (scroll->total >> 0xC) * v1;
    v2 = (s32)(value + (value >> 0x1F)) >> 1;
    v0 = scroll->count % 3;
    switch (v0) {
    case 2:
        break;
    case 0:
        v2 *= -1;
        break;
    case 1:
        v2 = 0;
        break;
    default:
        GF_AssertFail();
        break;
    }
    ScheduleSetBgPosText(scroll->bgConfig, 0U, BG_POS_OP_SET_ROT, v2);
    ScheduleSetBgPosText(scroll->bgConfig, 1U, BG_POS_OP_SET_ROT, v2);
    ScheduleSetBgPosText(scroll->bgConfig, 2U, BG_POS_OP_SET_ROT, v2);
    ScheduleSetBgPosText(scroll->bgConfig, 3U, BG_POS_OP_SET_ROT, v2);
    ScheduleSetBgPosText(scroll->bgConfig, 0U, BG_POS_OP_SET_XSCALE, (s32)value);
    ScheduleSetBgPosText(scroll->bgConfig, 1U, BG_POS_OP_SET_XSCALE, (s32)value);
    ScheduleSetBgPosText(scroll->bgConfig, 2U, BG_POS_OP_SET_XSCALE, (s32)value);
    ScheduleSetBgPosText(scroll->bgConfig, 3U, BG_POS_OP_SET_XSCALE, (s32)value);
    G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(scroll->spriteSystem), v2 << 0xC, value << 0xC);
    scroll->total = scroll->total - scroll->step;
    scroll->dir = 1 ^ scroll->dir;
    scroll->count = scroll->count + 1;
    if ((s32)scroll->count >= (s32)scroll->rate) {
        ScheduleSetBgPosText(scroll->bgConfig, 0U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(scroll->bgConfig, 1U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(scroll->bgConfig, 2U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(scroll->bgConfig, 3U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(scroll->bgConfig, 0U, BG_POS_OP_SET_XSCALE, 0);
        ScheduleSetBgPosText(scroll->bgConfig, 1U, BG_POS_OP_SET_XSCALE, 0);
        ScheduleSetBgPosText(scroll->bgConfig, 2U, BG_POS_OP_SET_XSCALE, 0);
        ScheduleSetBgPosText(scroll->bgConfig, 3U, BG_POS_OP_SET_XSCALE, 0);
        G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(scroll->spriteSystem), 0, 0);
        scroll->task = NULL;
        SysTask_Destroy(task);
    }
}

u32 ov96_02215864(Ov96RdScroll *scroll) {
    if (scroll == NULL) {
        GF_AssertFail();
    }
    if (scroll->task == NULL) {
        return 1;
    }
    return 0;
}

void ov96_0221587C(Ov96RdScroll *scroll, BgConfig *bgConfig, SpriteSystem *spriteSystem) {
    scroll->bgConfig = bgConfig;
    scroll->spriteSystem = spriteSystem;
    return;
}

void ov96_02215884(Ov96RdScroll *scroll, s32 param_2, s32 param_3) {
    fx32 fVar1;
    SysTask *pSVar2;

    if (scroll->bgConfig == NULL) {
        GF_AssertFail();
    }
    if (scroll->spriteSystem == NULL) {
        GF_AssertFail();
    }
    if (scroll->task != NULL) {
        SysTask_Destroy(scroll->task);
    }
    scroll->dir = 0;
    scroll->count = 0;
    scroll->rate = param_3;
    scroll->total = param_2 << 0xc;
    scroll->unk_16 = param_2;
    fVar1 = FX_Div(param_2 << 0xc, param_3 << 0xc);
    scroll->step = fVar1;
    pSVar2 = SysTask_CreateOnMainQueue(ov96_02215710, scroll, 2);
    scroll->task = pSVar2;
    return;
}

s32 ov96_022158D4(s32 arg0, s32 arg1) {
    if ((arg0 >= 0x68) && (arg0 < 0xA0) && (arg1 >= 0) && (arg1 < 0x20)) {
        return 1;
    }
    return 0;
}

u32 ov96_022158EC(VecFx32 *param_1, VecFx32 *param_2, s32 param_3) {
    fx32 fVar1;
    VecFx32 VStack_18;

    VEC_Subtract(param_2, param_1, &VStack_18);
    fVar1 = VEC_Mag(&VStack_18);
    if (fVar1 <= param_3) {
        return 1;
    }
    return 0;
}
