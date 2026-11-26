#include "global.h"

#include "party.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "unk_0201956C.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7960(s16, s16);
s32 ov14_021E79AC(s32, s32, s32 *);

void ov14_021E884C(UnkStruct_ov14_boxsys *arg0);
s32 ov14_021E65C4(void *);
s32 ov14_021E6CF8(void *, u32, s32);
s32 ov14_021E70B0(void *, s32);
s32 ov14_021E7148(void *, void *);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021E8FD4(struct UnkStruct_ov14_boxapp *arg0);

s32 sub_02019978(UnkStruct_0201956C *, s32);

extern s32 ov14_021F7C08;

s32 ov14_021E8FD4(UnkStruct_ov14_boxapp *arg0) {
    u32 sp4;
    u32 sp0;
    s32 temp_r1_4;
    s32 temp_r6;
    s32 temp_r7;
    s32 var_r6;
    u16 temp_r0;
    UnkStruct_ov14_boxsys *temp_r0_2;
    UnkStruct_ov14_boxsys *temp_r1;
    UnkStruct_ov14_boxsys *temp_r1_2;
    UnkStruct_ov14_boxsys *temp_r1_3;
    UnkStruct_ov14_boxsys *temp_r4;
    MoveSet *temp_r6_2;

    temp_r4 = arg0->sys;
    temp_r6 = sub_02019978(temp_r4->unk_2F0, 0xE);
    temp_r7 = sub_02019978(((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0, 0xF);
    temp_r0 = temp_r4->unk_10;
    switch (temp_r0) {
    case 0:
        if (System_GetTouchHeldCoords(&sp4, &sp0) == 0) {
            var_r6 = 0xFF;
            if (*((u8 *)&arg0->unk23[1]) != 0) {
                temp_r1 = arg0->sys;
                var_r6 = ov14_021E79D8((s16) *((u32 *)&temp_r1->unk_40B8), (s16) *((u32 *)&temp_r1->unk_40BC));
            }
            if (var_r6 == 0xFF) {
                temp_r1_2 = arg0->sys;
                var_r6 = ov14_021E7960((s16) *((u32 *)&temp_r1_2->unk_40B8), (s16) *((u32 *)&temp_r1_2->unk_40BC));
            }
            if (var_r6 == 0xFF) {
                temp_r1_3 = arg0->sys;
                var_r6 = ov14_021E79AC((s16) *((u32 *)&temp_r1_3->unk_40B8), (s16) *((u32 *)&temp_r1_3->unk_40BC), &ov14_021F7C08);
            }
            ov14_021E6CF8(arg0, arg0->unk21, var_r6);
            ov14_021F40E8(arg0, 0);
            if (!(0x80 & var_r6)) {
                arg0->unk21 = ov14_021E70B0(arg0, arg0->unk21);
            }
            ov14_021E884C(arg0->sys);
            temp_r4->unk_10 = (u16)(temp_r4->unk_10 + 1);
        } else {
            temp_r0_2 = arg0->sys;
            ManagedSprite_SetPositionXY(temp_r0_2->sprites[temp_r0_2->unk_4094[arg0->unk21]], (s16)sp4, (s16)((s16)sp0 - 8));
            ov14_021F4174(arg0);
            *((u32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40B8) = sp4;
            *((u32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40BC) = sp0;
        }
        break;
    case 1:
        if ((temp_r6 == 0) && (temp_r7 == 0)) {
            temp_r6_2 = temp_r4->unk_C;
            temp_r1_4 = temp_r6_2->to;
            if ((temp_r1_4 != 0xFF) && (0x80 & temp_r1_4)) {
                if (((u32) * (u32 *)((u8 *)temp_r6_2 + 0xE4) >= 0x1EU) && ((u32)(temp_r6_2->from - 0x1E) < (u32)(Party_GetCount(arg0->party) - 1))) {
                    ov14_021E8620(((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0);
                }
            } else {
                ov14_021E8620(((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0);
            }
            temp_r4->unk_10 = (u16)(temp_r4->unk_10 + 1);
        }
    case 2:
        if ((ov14_021E65C4(arg0) == 0) && (temp_r7 == 0) && (temp_r4->unk_10 == 2)) {
            ov14_021E7148(arg0, temp_r4->unk_C);
            ov14_021F4174(arg0);
            ov14_021F40E8(arg0, 0);
            temp_r4->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
