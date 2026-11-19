#include "global.h"

#include "sprite_system.h"
#include "touchscreen.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E79AC(s32, s32, s32 *);

s32 ov14_021E65C4(void *);
s32 ov14_021E70B0(void *, s32);
s32 ov14_021E7148(void *, void *);
s32 ov14_021E765C(void *);
s32 ov14_021E7ED0(void *);
s32 ov14_021E7EE0(void *);
s32 ov14_021E7F4C(void *);
s32 ov14_021E80A8(void *);
s32 ov14_021E8434(void *);
s32 ov14_021E8514(void *);
s32 ov14_021E8824(UnkStruct_ov14_boxsys *);
s32 ov14_021F2F88(u32, void *, s32 *, u32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021F69F0(void *, s32);
s32 ov14_021E9C88(struct UnkStruct_ov14_boxapp *arg0);

s32 sub_02019978(void *, s32);

extern s32 ov14_021F7C08;

s32 ov14_021E9C88(UnkStruct_ov14_boxapp *arg0) {
    u32 spC;
    u32 sp8;
    u8 sp4[4];
    s32 sp0;
    s16 temp_r2;
    s16 temp_r3;
    s32 temp_r6;
    s32 temp_r7;
    s32 var_r2;
    u16 temp_r0;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r1;
    void *temp_r1_2;
    void *temp_r5;

    temp_r5 = *((void * *)&arg0->sys);
    sp0 = ov14_021E8514(*((void **)&((UnkStruct_ov14_boxsys *)temp_r5)->unk_2F0));
    temp_r6 = ov14_021E80A8(arg0);
    temp_r7 = sub_02019978(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0), 0xA);
    temp_r0 = ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10;
    switch ((u32)temp_r0) {
    case 0:
        ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 1U;
    case 1:
        temp_r0_2 = *((void * *)&arg0->sys);
        if ((*(u8 *)((u8 *)temp_r0_2 + 0x44A) == 1) && (temp_r6 == 0)) {
            *(u8 *)((u8 *)temp_r0_2 + 0x44A) = 2U;
            ov14_021F69F0(arg0, 0x28);
        }
        if (System_GetTouchHeldCoords(&spC, &sp8) == 0) {
            var_r2 = 0xFF;
            temp_r1 = *((void * *)&arg0->sys);
            if (*(u8 *)((u8 *)temp_r1 + 0x44A) == 2) {
                var_r2 = ov14_021E79AC((s16) * (u32 *)((u8 *)temp_r1 + 0x40B8), (s16) * (u32 *)((u8 *)temp_r1 + 0x40BC), &ov14_021F7C08);
            }
            ov14_021E7034(arg0, arg0->unk21, var_r2);
            ov14_021F40E8(arg0, 0);
            arg0->unk21 = ov14_021E70B0(arg0, arg0->unk21);
            if ((u32) arg0->unk21 >= 0x1EU) {
                ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 5U;
            } else {
                temp_r1_2 = *((void * *)&arg0->sys);
                if (*(u8 *)((u8 *)temp_r1_2 + 0x44A) != 0) {
                    ov14_021E7F4C(*((void **)&((UnkStruct_ov14_boxsys *)temp_r1_2)->unk_2F0));
                }
                ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 2U;
            }
        } else {
            if ((*((u8 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_448[2]) == 0) && (sp0 == 0) && (temp_r7 == 0) && ((ov14_021F2F88(arg0->unk21, &sp4[2], (s32 *)sp4, arg0->unk22), temp_r3 = *(s16 *)((u8 *)((s32 *)sp4) + 2), (spC < (u32)(temp_r3 - 0x10))) || (spC >= (u32)(temp_r3 + 0x10)) || (temp_r2 = *(s16 *)((s32 *)sp4), (sp8 < (u32)(temp_r2 - 0x10))) || (sp8 >= (u32)(temp_r2 + 0x10)))) {
                *((u8 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_448[2]) = 1U;
                ov14_021E7ED0(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
                ov14_021E7EE0(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
            }
            temp_r0_3 = *((void * *)&arg0->sys);
            ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)(temp_r0_3 + (*(u8 *)((u8 *)(temp_r0_3 + arg0->unk21) + 0x4094) * 4)) + 0x2FC), (s16)spC, (s16)((s16)sp8 - 8));
            ov14_021F4174(arg0);
            *((u32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40B8) = spC;
            *((u32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40BC) = sp8;
        }
        break;
    case 2:
        if (temp_r6 == 0) {
            ov14_021E8434(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
            ov14_021E8824(*((void * *)&arg0->sys));
            ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 3U;
        }
    case 3:
    case 4:
        if ((ov14_021E65C4(arg0) == 0) && (((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 == 4) && (sp0 == 0)) {
            ov14_021E7148(arg0, *(void **)((u8 *)temp_r5 + 0xC));
            ov14_021F4174(arg0);
            ov14_021F40E8(arg0, *((s32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40C4));
            ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 8U;
        } else if (((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 == 3) {
            ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 4U;
        }
        break;
    case 5:
        if (ov14_021E65C4(arg0) == 0) {
            ov14_021E7148(arg0, *(void **)((u8 *)temp_r5 + 0xC));
            arg0->unk21 = 0xFFU;
            ov14_021E7F4C(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
            ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = (u16)(((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 + 1);
        }
        break;
    case 6:
        if (temp_r6 == 0) {
            ov14_021E765C(arg0);
            ov14_021E8328(((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0);
            ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = (u16)(((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 + 1);
        }
        break;
    case 7:
        if (temp_r7 == 0) {
            ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 8U;
        }
        break;
    case 8:
        ov14_021F3488(arg0, 1, 0);
        ov14_021F3488(arg0, 2, 0);
        *((u8 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_448[2]) = 0U;
        ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 0U;
        return 0;
    }
    return 1;
}
