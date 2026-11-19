#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "overlay_96_ov96_arena_work.h"

u32 ov96_021F2A84(Ov96ArenaWork *param_1, s32 param_2);

u32 ov96_021F2A84(Ov96ArenaWork *param_1, s32 param_2) {
    s32 iVar2;

    iVar2 = 0;
    do {
        if (param_1->mon[param_2][iVar2].state != 0) {
            return 0;
        }
        iVar2 = iVar2 + 1;
    } while (iVar2 < 3);
    return 1;
}

void ov96_021F2AA4(Ov96ArenaMon *arg0);

void ov96_021F2AA4(Ov96ArenaMon *arg0) {
    u16 v0;

    if (arg0->unk_44 != 0) {
        arg0->unk_45 = (u8)(arg0->unk_45 - 1);
        if (arg0->unk_45 == 0) {
            arg0->unk_45 = 0U;
            arg0->unk_44 = 0U;
            arg0->unk_8E = arg0->unk_8C;
        }
    } else {
        arg0->unk_8E = (u16)(arg0->unk_8E + 2);
        v0 = arg0->unk_8C;
        if ((u32)arg0->unk_8E > (u32)v0) {
            arg0->unk_8E = v0;
        }
        if (arg0->unk_8E == arg0->unk_8C) {
            arg0->unk_45 = 0U;
            arg0->unk_44 = 0U;
        }
    }
}

void ov96_021F2B24(PokeathlonCourseData *arg0, void *arg1, s32 arg2, s32 arg3, Ov96ArenaMon *arg4);

void ov96_021F2B24(PokeathlonCourseData *arg0, void *arg1, s32 arg2, s32 arg3, Ov96ArenaMon *arg4) {
    PokeathlonMonKinds *v0;

    v0 = ov96_021E60D8(arg0, arg2, arg3);
    arg4->unk_8A = (s16) * (s32 *)(arg1 + (v0->statIdx[0] * 4));
    arg4->unk_8C = (s16) * (s32 *)((u8 *)(arg1 + (v0->statIdx[3] * 4)) + 0x14);
    arg4->unk_8E = (s16) * (s32 *)((u8 *)(arg1 + (v0->statIdx[3] * 4)) + 0x14);
    arg4->unk_89 = (s8) * (s32 *)((u8 *)(arg1 + (v0->statIdx[1] * 4)) + 0x28);
}

static void ov96_021F2BB4(SysTask *arg0, void *arg1);

static void ov96_021F2C04(SysTask *arg0, void *arg1);

void ov96_021F2CD0(SysTask *task, void *data);

void ov96_021F2B68(Ov96ArenaFx *arg0, s16 arg1, s16 arg2, PokeathlonCourseData *arg3);

void ov96_021F2B68(Ov96ArenaFx *arg0, s16 arg1, s16 arg2, PokeathlonCourseData *arg3) {
    arg0->unk_1C = arg1;
    arg0->unk_1E = arg2;
    arg0->unk_00 = 0;
    arg0->unk_04 = SysTask_CreateOnMainQueue(ov96_021F2BB4, arg0, 0U);
    arg0->unk_0C = SysTask_CreateOnMainQueue(ov96_021F2C04, arg0, 1U);
    arg0->unk_30 = arg3;
    reg_G2_BG2OFS = 0x800000;
    arg0->unk_08 = SysTask_CreateOnMainQueue(ov96_021F2CD0, arg0, 2U);
}

void ov96_021F2D68(SysTask *task, void *data);

static void ov96_021F2BB4(SysTask *arg0, void *arg1) {
    Ov96ArenaFx *fx = arg1;
    u8 v0;
    PokeathlonTouchRecord *touch;

    v0 = fx->unk_00;
    switch (v0) {
    case 0:
        if (fx->unk_08 == NULL) {
            fx->unk_10 = SysTask_CreateOnMainQueue(ov96_021F2D68, fx, 3U);
            fx->unk_00 = (u8)(fx->unk_00 + 1);
            return;
        }
        return;
    case 1:
        if (fx->unk_10 == NULL) {
            fx->unk_04 = NULL;
            fx->unk_00 = 0U;
            touch = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(fx->unk_30));
            touch->unk_08 = 1;
            SysTask_Destroy(arg0);
        }
        break;
    }
}

s32 ov96_021F31F0(void *, u8);

s32 ov96_021F3298(void *, s32);

static void ov96_021F2C04(SysTask *arg0, void *arg1) {
    Ov96ArenaFx *fx = arg1;
    u16 v0;
    u16 v2;
    u16 ev1;
    u16 v3;
    u8 v1;

    v1 = fx->unk_01;
    switch (v1) {
    case 0:
        fx->unk_18 = (u16)(fx->unk_18 + 1);
        v0 = fx->unk_18;
        if ((u32)v0 <= 5U) {
            v3 = (u16)((s32)(0xC * (u8)v0) / 5);
            G2_SetBlendAlpha(0, 1, 0xC - v3, v3 + 4);
            return;
        }
        fx->unk_01 = (u8)(fx->unk_01 + 1);
        return;
    case 1:
        ov96_021F31F0(fx->unk_20, (u8)fx->unk_1E);
        ov96_021F3298(fx->unk_20, (u8)fx->unk_1C);
        fx->unk_18 = 0U;
        fx->unk_01 = (u8)(fx->unk_01 + 1);
        PlaySE(0x89FU);
        return;
    case 2:
        fx->unk_18 = (u16)(fx->unk_18 + 1);
        v2 = fx->unk_18;
        if ((u32)v2 <= 5U) {
            ev1 = (u16)((s32)(0xC * (u8)v2) / 5);
            G2_SetBlendAlpha(0, 1, (s32)ev1, 0x10 - ev1);
            return;
        }
        fx->unk_0C = NULL;
        fx->unk_18 = 0U;
        fx->unk_01 = 0U;
        SysTask_Destroy(arg0);
        return;
    }
}
