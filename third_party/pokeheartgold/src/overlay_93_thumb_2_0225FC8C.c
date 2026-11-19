#include "global.h"

#include "bg_window.h"
#include "math_util.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "gf_gfx_planes.h"

void ov93_0225FC8C(BgConfig *param0, UnkStruct_ov117_02263DAC *param1);

void ov93_0225FD8C(s32 param_1, s32 param_2, s32 param_3, s32 *param_4, s32 *param_5);
void ov93_0225FCA4(BgConfig *param_1, s32 *param_2, s32 param_3);

void ov93_0225FC8C(BgConfig *param0, UnkStruct_ov117_02263DAC *param1) {
    BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_3);
    param1->unk_00 = 0;
    param1->unk_2D = 1;
}

void ov93_0225FCA4(BgConfig *param_1, s32 *param_2, s32 param_3) {
    u16 uVar1;
    fx32 value;
    fx32 value_00;
    s32 iVar2;
    s32 iVar3;
    s32 iVar4;
    s32 iStack_18;
    s32 iStack_1c;

    if (*param_2 != 0) {
        ov93_0225FD8C(param_3, param_2[1], param_2[2], &iStack_18, &iStack_1c);
        if (iStack_18 == 0x1000) {
            iVar4 = 0;
        } else if (iStack_18 < 0x1000) {
            iVar4 = 0x80 - ((s32)(iStack_18 * 0x80 + ((u32)(iStack_18 * 0x80 >> 0xb) >> 0x14)) >> 0xc);
        } else {
            iVar4 = (iStack_18 + -0x1000) * 0x80;
            iVar4 = -((s32)(iVar4 + ((u32)(iVar4 >> 0xb) >> 0x14)) >> 0xc);
        }
        if (iStack_1c == 0x1000) {
            iVar3 = 0;
        } else if (iStack_1c < 0x1000) {
            iVar3 = 0x80 - ((s32)(iStack_1c * 0x80 + ((u32)(iStack_1c * 0x80 >> 0xb) >> 0x14)) >> 0xc);
        } else {
            iVar3 = (iStack_1c + -0x1000) * 0x80;
            iVar3 = -((s32)(iVar3 + ((u32)(iVar3 >> 0xb) >> 0x14)) >> 0xc);
        }
        if (param_2[4] == 0) {
            iVar2 = 0;
        } else {
            uVar1 = LCRandom();
            iVar2 = (uVar1 & 7) + 1;
            if ((param_2[10] & 1U) != 0) {
                iVar2 = -iVar2;
            }
            param_2[10] = param_2[10] ^ 1;
        }
        value = FX_Inv(iStack_18);
        value_00 = FX_Inv(iStack_1c);
        ScheduleSetBgAffineScale(param_1, 7, BG_POS_OP_SET_XSCALE, value);
        ScheduleSetBgAffineScale(param_1, 7, BG_POS_OP_SET_YSCALE, value_00);
        ScheduleSetBgPosText(param_1, 7, BG_POS_OP_SET_ROT, iVar2 - iVar4);
        ScheduleSetBgPosText(param_1, 7, BG_POS_OP_SET_XSCALE, 0x27 - iVar3);
    }
    return;
}
