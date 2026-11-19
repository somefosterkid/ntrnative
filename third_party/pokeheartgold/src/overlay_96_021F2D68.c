#include "global.h"

#include "sys_task_api.h"

#include "pokeathlon/pokeathlon.h"
#include "sys_task.h"
#include "overlay_96_ov96_arena_work.h"

extern s32 ov96_0221DB70;

void ov96_021F2D68(SysTask *arg0, void *arg1);

void ov96_021F2D98(Ov96ArenaFx *param_1);

void ov96_021F2E4C(SysTask *task, void *data);

void ov96_021F2E2C(Ov96ArenaFx *arg0, s32 arg1);

void ov96_021F2D68(SysTask *task, void *arg1) {
    Ov96ArenaFx *fx = arg1;

    fx->unk_24 = (s16)(fx->unk_24 + *(s32 *)(&ov96_0221DB70 + fx->unk_26));
    fx->unk_26 = (u16)(fx->unk_26 + 1);
    if ((u32)fx->unk_26 >= 0x12U) {
        fx->unk_24 = 0;
        fx->unk_10 = NULL;
        fx->unk_26 = 0U;
        SysTask_Destroy(task);
    }
}

void ov96_021F2D98(Ov96ArenaFx *param_1) {
    u8 *puVar1;

    param_1->unk_03 = 0;
    if (param_1->unk_04 != NULL) {
        puVar1 = PokeathlonCourse_GetDataCopyArea(param_1->unk_30);
        puVar1 = ov96_021E8A20(puVar1);
        puVar1[8] = 1;
        SysTask_Destroy(param_1->unk_04);
        param_1->unk_04 = NULL;
    }
    if (param_1->unk_08 != NULL) {
        SysTask_Destroy(param_1->unk_08);
        param_1->unk_08 = NULL;
    }
    if (param_1->unk_0C != NULL) {
        SysTask_Destroy(param_1->unk_0C);
        param_1->unk_0C = NULL;
    }
    if (param_1->unk_10 != NULL) {
        SysTask_Destroy(param_1->unk_10);
        param_1->unk_10 = NULL;
    }
    if (param_1->unk_14 != NULL) {
        SysTask_Destroy(param_1->unk_14);
        param_1->unk_14 = NULL;
    }
    reg_G2_BG2OFS = 0x800000;
    param_1->unk_00 = 0;
    param_1->unk_01 = 0;
    param_1->unk_02 = 0;
    param_1->unk_18 = 0;
    param_1->unk_1A = 0;
    param_1->unk_26 = 0;
    param_1->unk_2E = 0;
    param_1->unk_24 = 0;
    param_1->unk_2C = 0;
    param_1->unk_28 = 0xc;
    G2_SetBlendAlpha(0, 1, 0xc, 4);
    return;
}

void ov96_021F2E2C(Ov96ArenaFx *arg0, s32 arg1) {
    if (arg0->unk_14 == NULL) {
        arg0->unk_28 = arg1;
        arg0->unk_14 = SysTask_CreateOnMainQueue(ov96_021F2E4C, arg0, 0U);
    }
}
