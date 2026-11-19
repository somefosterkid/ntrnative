#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

void ov96_021E8228(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, s32 param_4, u32 param_5);

void ov96_0220D744(UnkStruct_ov96_0220E3F0_Unit *param_1, PokeathlonCourseData *param_2);

u32 ov96_0220D8C4(UnkStruct_ov96_0220E3F0_Unit *param_1, s32 param_2, s32 param_3);

void ov96_0220D910(UnkStruct_ov96_0220E3F0_Unit *param_1);

void ov96_0220D9A4(UnkStruct_ov96_0220E3F0_Unit *param_1, PokeathlonCourseData *arg1);

void ov96_0220D9A4(UnkStruct_ov96_0220E3F0_Unit *param_1, PokeathlonCourseData *arg1) {
    s32 iVar1;
    u32 uVar2;

    switch (param_1->state) {
    case 4:
        param_1->state = 2;
        return;
    case 3:
        param_1->unk_44 = param_1->unk_44 + -1;
        if (param_1->unk_44 == 0) {
            ov96_0220D910(param_1);
            param_1->state = 4;
            return;
        }
        break;
    case 2:
        ov96_0220D744(param_1, arg1);
        iVar1 = ov96_0220D8C4(param_1, param_1->unk_1C.x, param_1->unk_1C.y);
        if (iVar1 != 0) {
            ov96_021E8228(param_1->course, param_1->unk_45, param_1->unk_46, 1, 1);
            ov96_021E8228(param_1->course, param_1->unk_45, param_1->unk_46, 5, 1);
            if ((u32)(param_1->unk_40.raw << 2) >> 31) {
                ov96_021E8228(param_1->course, param_1->unk_45, param_1->unk_46, 8, 1);
            }
            param_1->unk_40.raw = param_1->unk_40.raw & 0xdfffffff;
            param_1->unk_44 = 0x5a;
            param_1->state = 3;
            return;
        }
        break;
    case 1:
        uVar2 = (u8)((u8)param_1->unk_40.raw - 1);
        param_1->unk_40.raw = param_1->unk_40.raw & 0xffffff00 | uVar2;
        if ((u8)param_1->unk_40.raw == 0) {
            param_1->state = 2;
        }
        ov96_0220D744(param_1, arg1);
        return;
    default:
        GF_AssertFail();
        break;
    }
    return;
}
