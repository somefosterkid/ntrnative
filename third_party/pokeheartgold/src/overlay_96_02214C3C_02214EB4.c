#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_app_02210C98.h"

void ov96_021E8228(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, s32 param_4, u32 param_5);

s32 ov96_02215058(Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Mover *mover, u32 action, s32 player);

u32 ov96_02215614(s32 param_1, s32 param_2, s32 param_3, s32 param_4);

u32 ov96_0221567C(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6);

u32 ov96_02214EB4(PokeathlonCourseData *param_1, u8 param_2, u8 param_3, Ov96App_02210C98_Unit *param_4, Ov96App_02210C98_Mover *param_5);

u32 ov96_02214EB4(PokeathlonCourseData *param_1, u8 param_2, u8 param_3, Ov96App_02210C98_Unit *param_4, Ov96App_02210C98_Mover *param_5) {
    u32 uVar1;
    u32 uVar2;
    fx32 fVar3;
    fx32 fVar4;
    u32 uVar5;
    s32 iVar6;
    Ov96App_02210C98_Mover *mover;
    u8 abStack_28[4];
    VecFx32 VStack_24;

    if (param_4->unk_78 != 0) {
        return 0;
    }
    if (param_4->unk_48 != 0) {
        return 0;
    }
    uVar1 = ov96_02215614(0x80, 0x60, ((param_4->unk_30.x) / 0x1000), ((param_4->unk_30.y) / 0x1000));
    uVar5 = 0;
    do {
        mover = &param_5[uVar5];
        if ((mover->unk_39 == 0) && (mover->unk_38 != 0)) {
            uVar2 = ov96_02215614(0x80, 0x60, ((mover->unk_8.x) / 0x1000), ((mover->unk_8.y) / 0x1000));
        } else {
            uVar2 = 4;
        }
        if (uVar2 != 4) {
            if (param_2 == uVar2) {
                iVar6 = ov96_0221567C(((mover->unk_8.x) / 0x1000),
                    ((mover->unk_8.y) / 0x1000),
                    ((param_4->unk_30.x) / 0x1000),
                    ((param_4->unk_30.y) / 0x1000),
                    uVar1,
                    param_2);
                switch (iVar6) {
                default:
                    abStack_28[uVar5] = 7;
                    break;
                case 0:
                    abStack_28[uVar5] = 6;
                    break;
                }
            } else {
                abStack_28[uVar5] = 6;
            }
        } else {
            abStack_28[uVar5] = 5;
        }
        uVar5 = uVar5 + 1 & 0xff;
    } while (uVar5 < 2);
    uVar5 = (u32)abStack_28[1];
    if ((abStack_28[0] == 7) == (uVar5 == 7)) {
        VEC_Subtract(&param_4->unk_30, &param_5[0].unk_8, &VStack_24);
        fVar3 = VEC_Mag(&VStack_24);
        VEC_Subtract(&param_4->unk_30, &param_5[1].unk_8, &VStack_24);
        fVar4 = VEC_Mag(&VStack_24);
        if (fVar3 >= fVar4) {
            uVar1 = ov96_02215058(param_4, param_5, (u32)abStack_28[0], param_2);
        } else {
            uVar1 = ov96_02215058(param_4, param_5 + 1, uVar5, param_2);
        }
    } else if (abStack_28[0] >= uVar5) {
        uVar1 = ov96_02215058(param_4, param_5, (u32)abStack_28[0], param_2);
    } else {
        uVar1 = ov96_02215058(param_4, param_5 + 1, uVar5, param_2);
    }
    if (param_4->unk_78 == 1) {
        ov96_021E8228(param_1, param_2, param_3, 6, 1);
    }
    return uVar1;
}
