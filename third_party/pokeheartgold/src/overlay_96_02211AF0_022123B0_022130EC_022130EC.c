#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "unk_02020E80.h"

#include "overlay_96_app_02210C98.h"

s32 _s32_div_f(s8, u32);

void ov96_021E8228(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, s32 param_4, u32 param_5);

s32 ov96_022143DC(Ov96App_02210C98_Mover *param_1);

extern Vec2D ov96_0221D474[][2];

void ov96_022130EC(PokeathlonCourseData *param_1);

void ov96_022130EC(PokeathlonCourseData *param_1) {
    u8 cVar1;
    s32 i;
    Ov96App_02210C98 *pvVar3;
    s32 slot;
    Ov96App_02210C98_Mover *elem;
    Ov96App_02210C98_Unit *unit;
    s32 iVar5;
    s32 q;
    s32 rem;
    Vec2D VStack_20;
    pvVar3 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    i = 0;
    elem = &pvVar3->unk_62C[0];
    do {
        if ((elem->unk_38 != 0) && ((elem->unk_39 == 0) || (elem->unk_39 == 2))) {
            VStack_20.x = elem->unk_8.x / 4096;
            VStack_20.y = elem->unk_8.y / 4096;
            slot = ov96_022143DC(elem);
            if (((u8)sub_02020E80(&ov96_0221D474[slot][0], &ov96_0221D474[slot][1], &VStack_20)) != 0) {
                elem->unk_39 = 1;
                elem->unk_3A = 10;
                cVar1 = elem->unk_3B;
                if (cVar1 != 12) {
                    q = cVar1 / 3;
                    rem = cVar1 % 3;
                    unit = &pvVar3->unk_5C[q][rem];
                    if (slot == q) {
                        if ((unit->unk_5E) != 0) {
                            unit->unk_5E -= 1;
                        }
                        if ((pvVar3->unk_6F4[elem->unk_3B / 3]) != 0) {
                            pvVar3->unk_6F4[elem->unk_3B / 3] -= 1;
                        }
                        ov96_021E8228(param_1, (u8)(elem->unk_3B / 3), (u8)(elem->unk_3B % 3), 8, 1);
                        ov96_021E8228(param_1, (u8)(elem->unk_3B / 3), (u8)(elem->unk_3B % 3), 1, 1);
                    } else {
                        iVar5 = 0;
                        if (elem->unk_38 == 1) {
                            iVar5 = 1;
                        } else if (elem->unk_38 == 2) {
                            iVar5 = 2;
                        } else {
                            GF_AssertFail();
                        }
                        if (((unit->unk_5E) + iVar5) <= 99) {
                            unit->unk_5E += iVar5;
                        }
                        if (((pvVar3->unk_6F4[elem->unk_3B / 3]) + iVar5) <= 99) {
                            pvVar3->unk_6F4[elem->unk_3B / 3] += iVar5;
                        }
                        if ((pvVar3->unk_6F4[slot]) != 0) {
                            pvVar3->unk_6F4[slot] -= 1;
                        }
                        ov96_021E8228(param_1, (u8)(elem->unk_3B / 3), (u8)(elem->unk_3B % 3), 3, 1);
                    }
                }
            }
        }
        elem++;
        i++;
    } while (i < 2);
    return;
}
