#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_app_02210C98.h"

void ov96_021E8228(PokeathlonCourseData *data, u32 player, u32 slot, s32 arg3, s32 arg4);

u32 ov96_021EAF8C(void *mapObj);

s32 ov96_02214DBC(Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Mover *mover, s32 action, s32 area);

s32 ov96_02215460(Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Mover *mover, s32 action, u8 player, Ov96App_02210C98_Cpu *cpu);

s32 ov96_02215478(Ov96App_02210C98_Unit *unit, Ov96App_02210C98_Cpu *cpu, u8 skip);

u32 ov96_02215614(s32 param_1, s32 param_2, s32 param_3, s32 param_4);

#if defined(SDK_PORT) || defined(NONMATCHING)
u32 ov96_02215650(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u8 param_5, u8 param_6);
#else
u32 ov96_02215650(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6);
#endif

u32 ov96_022156E8(VecFx32 *param_1, s32 param_2, VecFx32 *param_3, s32 param_4);

u32 ov96_02215184(PokeathlonCourseData *param_1, u8 param_2, u8 param_3, Ov96App_02210C98_Unit *param_4, Ov96App_02210C98_Mover *param_5, Ov96App_02210C98_Cpu *param_6);

u32 ov96_02215184(PokeathlonCourseData *param_1, u8 param_2, u8 param_3, Ov96App_02210C98_Unit *param_4, Ov96App_02210C98_Mover *param_5, Ov96App_02210C98_Cpu *param_6) {
    VecFx32 *pos;
    u8 bVar1;
    u32 uVar2;
    s32 iVar4;
    fx32 fVar5;
    fx32 fVar6;
    Ov96App_02210C98_Mover *iVar8;
    u32 uVar9;
    s32 iVar10;
    u32 uVar3;
    VecFx32 vec;
    u8 b[2];
    u8 a[2];

    if (param_4->unk_78 != 0) {
        return 0;
    }
    if (param_4->unk_48 != 0) {
        return 0;
    }
    uVar2 = ov96_02215614(0x80, 0x60, param_4->unk_30.x / 0x1000, param_4->unk_30.y / 0x1000);
    uVar9 = 0;
    do {
        iVar8 = &param_5[uVar9];
        if (iVar8->unk_39 == 0 && iVar8->unk_38 != 0) {
            a[uVar9] = ov96_02215614(0x80, 0x60, iVar8->unk_8.x / 0x1000, iVar8->unk_8.y / 0x1000);
        } else {
            a[uVar9] = 4;
        }
        uVar9 = uVar9 + 1 & 0xff;
    } while (uVar9 < 2);
    uVar9 = 0;
    pos = &param_4->unk_30;
    do {
        iVar8 = &param_5[uVar9];
        b[uVar9] = 8;
        bVar1 = a[uVar9];
        if (bVar1 != 4) {
            iVar10 = 0;
            if (iVar8->unk_38 == 1) {
                iVar10 = 8;
            } else if (iVar8->unk_38 == 2) {
                iVar10 = 0xc;
            } else {
                GF_AssertFail();
            }
            iVar4 = ov96_021EAF8C(param_4->unk_0);
            if (ov96_022156E8(pos, iVar4 << 0xc, &iVar8->unk_8, iVar10 << 0xc) != 0) {
                b[uVar9] = 8;
            } else if (uVar2 == bVar1 && ov96_02215650(iVar8->unk_8.x / 0x1000, iVar8->unk_8.y / 0x1000, param_4->unk_30.x / 0x1000, param_4->unk_30.y / 0x1000, uVar2, param_2) != 0) {
                b[uVar9] = 0xd;
            }
        }
        uVar9 = uVar9 + 1 & 0xff;
    } while (uVar9 < 2);
    if (b[0] == 0xd) {
        return ov96_02214DBC(param_4, param_5, b[0], uVar2);
    }
    if (b[1] == 0xd) {
        param_5 += 1;
        return ov96_02214DBC(param_4, param_5, b[1], uVar2);
    }
    if (param_6->players[param_2].slots[0].unit->unk_71 >= 0x32) {
        return ov96_02215478(param_4, param_6, param_2);
    }
    uVar9 = 0;
    do {
        iVar8 = &param_5[uVar9];
        if (a[uVar9] != 4) {
            if (param_2 == a[uVar9]) {
                if (ov96_02215650(iVar8->unk_8.x / 0x1000, iVar8->unk_8.y / 0x1000, param_4->unk_30.x / 0x1000, param_4->unk_30.y / 0x1000, uVar2, param_2) != 0) {
                    b[uVar9] = 0xc;
                } else {
                    b[uVar9] = 0xb;
                }
            } else {
                b[uVar9] = 0xb;
            }
        } else {
            b[uVar9] = 10;
        }
        uVar9 = uVar9 + 1 & 0xff;
    } while (uVar9 < 2);
    uVar2 = b[0];
    uVar9 = b[1];
    if ((uVar2 == 7) == (uVar9 == 7)) {
        VEC_Subtract(&param_4->unk_30, &param_5[0].unk_8, &vec);
        fVar5 = VEC_Mag(&vec);
        VEC_Subtract(&param_4->unk_30, &param_5[1].unk_8, &vec);
        fVar6 = VEC_Mag(&vec);
        if (fVar5 >= fVar6) {
            uVar3 = ov96_02215460(param_4, param_5, b[0], param_2, param_6);
        } else {
            param_5 += 1;
            uVar3 = ov96_02215460(param_4, param_5, uVar9, param_2, param_6);
        }
    } else if (uVar2 >= uVar9) {
        uVar3 = ov96_02215460(param_4, param_5, uVar2, param_2, param_6);
    } else {
        param_5 += 1;
        uVar3 = ov96_02215460(param_4, param_5, uVar9, param_2, param_6);
    }
    if (param_4->unk_78 == 1) {
        ov96_021E8228(param_1, param_2, param_3, 6, 1);
    }
    return uVar3;
}
