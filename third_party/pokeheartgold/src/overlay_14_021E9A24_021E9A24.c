#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7960(s16, s16);

void ov14_021E765C(void *arg0);
u32 ov14_021E8514(void *arg0);
void ov14_021E8824(UnkStruct_ov14_boxsys *arg0);
s32 ov14_021E65C4(void *);
s32 ov14_021E66F4(void *);
s32 ov14_021E70B0(void *, s32);
s32 ov14_021E7148(void *, void *);
s32 ov14_021E7EE0(void *);
s32 ov14_021E7FB8(void *);
s32 ov14_021E7FEC(void *);
s32 ov14_021E8434(void *);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F40E8(void *, s32);
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F69F0(void *, s32);
#else
s32 ov14_021F69F0(void *, s32);
#endif
s32 ov14_021E9A24(struct UnkStruct_ov14_boxapp *arg0);

s32 sub_02019978(void *, s32);

s32 ov14_021E9A24(UnkStruct_ov14_boxapp *arg0) {
    u32 spC;
    u32 sp8;
    s16 sp4[2];
    s32 sp0;
    s32 temp_r6;
    s32 var_r2;
    u16 temp_r0;
    u32 temp_r7;
    UnkStruct_ov14_boxsys *temp_r0_2;
    UnkStruct_ov14_boxsys *temp_r0_3;
    UnkStruct_ov14_boxsys *temp_r0_4;
    UnkStruct_ov14_boxsys *temp_r1;
    UnkStruct_ov14_boxsys *temp_r5;

    temp_r5 = arg0->sys;
    if ((u32) temp_r5->unk_10 <= 1U) {
        ManagedSprite_GetPositionXY(temp_r5->sprites[temp_r5->unk_4094[arg0->unk21]], &sp4[1], &sp4[0]);
    }
    temp_r7 = ov14_021E8514(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
    temp_r6 = ov14_021E813C(arg0);
    sp0 = sub_02019978(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0), 0xA);
    temp_r0 = temp_r5->unk_10;
    switch ((u32)temp_r0) {
    case 0:
        temp_r5->unk_10 = 1U;
    case 1:
        temp_r0_2 = arg0->sys;
        if ((temp_r0_2->unk_448[2] == 1) && (temp_r6 == 0)) {
            temp_r0_2->unk_448[2] = 2U;
            ov14_021F69F0(arg0, 0x28);
        }
        if (System_GetTouchHeldCoords(&spC, &sp8) == 0) {
            temp_r0_3 = arg0->sys;
            ManagedSprite_SetPositionXY(temp_r0_3->sprites[temp_r0_3->unk_4094[arg0->unk21]], sp4[1], sp4[0]);
            temp_r1 = arg0->sys;
            var_r2 = 0xFF;
            if (temp_r1->unk_448[2] == 2) {
                var_r2 = ov14_021E7960((s16) temp_r1->unk_40B8, (s16) temp_r1->unk_40BC);
            }
            ov14_021E6F3C(arg0, arg0->unk21, var_r2);
            ov14_021F40E8(arg0, 0);
            arg0->unk21 = ov14_021E70B0(arg0, arg0->unk21);
            if ((u32) arg0->unk21 < 0x1EU) {
                temp_r5->unk_10 = 5U;
            } else {
                ov14_021E7FEC(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
                ov14_021E8434(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
                ov14_021E8824(arg0->sys);
                temp_r5->unk_10 = 2U;
            }
        } else {
            temp_r0_3 = arg0->sys;
            if ((temp_r0_3->unk_448[2] == 0) && (temp_r7 == 0) && (sp0 == 0) && ((spC < 0x10U) || (sp8 < 0x30U) || (spC >= 0x68U))) {
                temp_r0_3->unk_448[2] = 1U;
                ov14_021E7FB8(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
            }
            temp_r0_4 = arg0->sys;
            ManagedSprite_SetPositionXY(temp_r0_4->sprites[temp_r0_4->unk_4094[arg0->unk21]], (s16)spC, (s16)((s16)sp8 - 8));
            ov14_021F4174(arg0);
            *((u32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40B8) = spC;
            *((u32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40BC) = sp8;
        }
        break;
    case 2:
    case 3:
    case 4:
        if ((ov14_021E65C4(arg0) == 0) && (temp_r6 == 0) && (temp_r7 == 0)) {
            ov14_021E7148(arg0, temp_r5->unk_C);
            ov14_021F4174(arg0);
            ov14_021F40E8(arg0, *((s32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40C4));
            temp_r5->unk_10 = 7U;
        }
        break;
    case 5:
        if (ov14_021E66F4(arg0) == 0) {
            ov14_021E7148(arg0, temp_r5->unk_C);
            ov14_021E765C(arg0);
            ov14_021E7EE0(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
            arg0->unk21 = 0xFFU;
            temp_r5->unk_10 = 6U;
        }
        break;
    case 6:
        if (temp_r6 == 0) {
            temp_r5->unk_10 = 7U;
        }
        break;
    case 7:
        ov14_021F3488(arg0, 1, 1);
        ov14_021F3488(arg0, 2, 0);
        *((u8 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_448[2]) = 0U;
        temp_r5->unk_10 = 0U;
        return 0;
    }
    return 1;
}
