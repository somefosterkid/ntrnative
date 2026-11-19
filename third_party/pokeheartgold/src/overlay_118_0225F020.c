#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "field_system_def.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_01_encounter_effect.h"
#include "sys_task.h"
#include "unk_0200FA24.h"

void ov01_021F0500(NARC *arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, BgConfig *arg6, u32 arg7, u32 arg8);

void ov01_021EFCDC(EncounterEffect *encEffect, SysTask *effectTask);

void ov01_021EFCF8(u32 screen, u32 screenFlashColor, u32 otherScreenFlashColor, BOOL *done, u32 numFlashes);

void ov01_021EFEC8(s32 *, u32, u32, u32, u32);

s32 ov01_021EFF28(s32 *);

extern u8 _0225F270[];

extern u8 ov118_0225F28C[];

void ov118_0225F020(SysTask *param_1, EncounterEffect *param_2);

void ov118_0225F020(SysTask *param_1, EncounterEffect *param_2) {
    void *pvVar1;
    s32 iVar2;
    u32 uVar3;
    s32 *piVar4;

    piVar4 = param_2->param;
    switch (param_2->state) {
    case 0:
        pvVar1 = Heap_Alloc(HEAP_ID_FIELD1, 0x20);
        param_2->param = pvVar1;
        memset(pvVar1, 0, 0x20);
        piVar4 = param_2->param;
        GfGfx_EngineATogglePlanes(2, 1);
        GfGfx_EngineATogglePlanes(8, 1);
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffffe0ff | 0x1500;
        FreeBgTilemapBuffer(param_2->fieldSystem->bgConfig, 1);
        InitBgFromTemplate(param_2->fieldSystem->bgConfig, 1, (BgTemplate *)((u8 *)_0225F270), 0);
        BgClearTilemapBufferAndCommit(param_2->fieldSystem->bgConfig, 1);
        FreeBgTilemapBuffer(param_2->fieldSystem->bgConfig, 3);
        InitBgFromTemplate(param_2->fieldSystem->bgConfig, 3, (BgTemplate *)((u8 *)ov118_0225F28C), 0);
        BgClearTilemapBufferAndCommit(param_2->fieldSystem->bgConfig, 3);
        ov01_021F0500(param_2->narc, 0xa8, 0xa7, 0xa6, 0, 0, param_2->fieldSystem->bgConfig, 1, 3);
        SetBgPriority(2, 0);
        SetBgPriority(8, 0);
        BG_SetMaskColor(1, 0);
        BgSetPosTextAndCommit(param_2->fieldSystem->bgConfig, 1, BG_POS_OP_SET_ROT, 0x80);
        BgSetPosTextAndCommit(param_2->fieldSystem->bgConfig, 3, BG_POS_OP_SET_ROT, 0x100);
        GfGfx_EngineASetPlanes(0x15);
        param_2->state = 1;
        break;
    case 1:
        ov01_021EFCF8(1, 0xfffffff0, 0xfffffff0, &param_2->effectComplete, 2);
        param_2->state = 2;
        break;
    case 2:
        if (param_2->effectComplete != 0) {
            param_2->state = 3;
        }
        break;
    case 3:
        ov01_021EFEC8(piVar4, 0x80000, 0, 0x5000, 0x12);
        GfGfx_EngineATogglePlanes(2, 1);
        GfGfx_EngineATogglePlanes(8, 1);
        piVar4[7] = 1;
        param_2->state = 4;
        break;
    case 4:
        iVar2 = ov01_021EFF28(piVar4);
        if (iVar2 != 0) {
            param_2->state = 5;
        }
        break;
    case 5:
        BgSetPosTextAndCommit(param_2->fieldSystem->bgConfig, 1, BG_POS_OP_SET_ROT, 0);
        BgSetPosTextAndCommit(param_2->fieldSystem->bgConfig, 3, BG_POS_OP_SET_ROT, 0x180);
        piVar4[7] = 0;
        uVar3 = piVar4[6];
        piVar4[6] = uVar3 + 1;
        if (5 < uVar3) {
            GfGfx_EngineATogglePlanes(1, 0);
            piVar4[7] = 1;
            piVar4[6] = 0;
            param_2->state = 6;
        }
        break;
    case 6:
        ov01_021EFEC8(piVar4, 0, 0x80000, 0x5000, 0x12);
        piVar4[7] = 1;
        param_2->state = 7;
        break;
    case 7:
        iVar2 = ov01_021EFF28(piVar4);
        if (iVar2 != 0) {
            param_2->state = 8;
        }
        break;
    case 8:
        piVar4[7] = 0;
        sub_0200FC20(0);
        if (param_2->done != NULL) {
            *param_2->done = TRUE;
        }
        ov01_021EFCDC(param_2, param_1);
        return;
    }
    if (piVar4[7] != 0) {
        ScheduleSetBgPosText(param_2->fieldSystem->bgConfig, 1, BG_POS_OP_SET_ROT, *piVar4 >> 0xc);
        ScheduleSetBgPosText(param_2->fieldSystem->bgConfig, 1, BG_POS_OP_SET_XSCALE, 0);
        ScheduleSetBgPosText(param_2->fieldSystem->bgConfig, 3, BG_POS_OP_SET_ROT, 0x180 - (*piVar4 >> 0xc));
        ScheduleSetBgPosText(param_2->fieldSystem->bgConfig, 3, BG_POS_OP_SET_XSCALE, 0);
    }
    return;
}
