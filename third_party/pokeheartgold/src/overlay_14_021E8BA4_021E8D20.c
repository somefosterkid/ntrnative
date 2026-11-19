#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7960(s16, s16);
s32 ov14_021E79AC(s32, s32, s32 *);

u32 ov14_021E8514(void *arg0);
void ov14_021E8824(UnkStruct_ov14_boxsys *arg0);
s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E66F4(void *);
s32 ov14_021E6CF8(void *, u8, s32);
s32 ov14_021E70B0(void *, s32);
s32 ov14_021E7148(void *, void *);
s32 ov14_021E7588(void *, s32);
s32 ov14_021E7EE0(void *);
s32 ov14_021E7FB8(void *arg0);
s32 ov14_021E7FEC(void *);
s32 ov14_021E80A8(void *);
s32 ov14_021E8434(void *);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021F69F0(void *, s32);
s32 ov14_021E8D20(UnkStruct_ov14_boxapp *arg0);

extern s32 ov14_021F7BF0;

s32 ov14_021E8D20(UnkStruct_ov14_boxapp *arg0) {
    u32 sp8;
    u32 sp4;
    s16 sp0[2];
    s32 temp_r6;
    s32 var_r0;
    u16 temp_r0_2;
    u32 temp_r7;
    u8 temp_r1_2;
    void *temp_r0;
    void *temp_r0_3;
    void *temp_r0_4;
    void *temp_r0_5;
    void *temp_r0_6;
    void *temp_r1;
    void *temp_r5;

    temp_r5 = *((void * *)&arg0->sys);
    temp_r7 = ov14_021E8514(*((void **)&((UnkStruct_ov14_boxsys *)temp_r5)->unk_2F0));
    if (((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 == 0) {
        temp_r0 = *((void * *)&arg0->sys);
        ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)(temp_r0 + (*(u8 *)((u8 *)(temp_r0 + arg0->unk21) + 0x4094) * 4)) + 0x2FC), &sp0[1], &sp0[0]);
    }
    temp_r6 = ov14_021E80A8(arg0);
    temp_r0_2 = ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10;
    switch ((u32)temp_r0_2) {
    case 0:
        temp_r0_3 = *((void * *)&arg0->sys);
        if ((*(u8 *)((u8 *)temp_r0_3 + 0x44A) == 1) && (temp_r6 == 0)) {
            *(u8 *)((u8 *)temp_r0_3 + 0x44A) = 2U;
            ov14_021F69F0(arg0, 0x28);
            ov14_021F3488(arg0, 1, 0);
        }
        if (System_GetTouchHeldCoords(&sp8, &sp4) == 0) {
            temp_r0_4 = *((void * *)&arg0->sys);
            ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)(temp_r0_4 + (*(u8 *)((u8 *)(temp_r0_4 + arg0->unk21) + 0x4094) * 4)) + 0x2FC), sp0[1], sp0[0]);
            temp_r1 = *((void * *)&arg0->sys);
            if (*(u8 *)((u8 *)temp_r1 + 0x44A) == 2) {
                var_r0 = ov14_021E7960((s16) * (u32 *)((u8 *)temp_r1 + 0x40B8), (s16) * (u32 *)((u8 *)temp_r1 + 0x40BC));
            } else {
                var_r0 = ov14_021E79AC((s16) * (u32 *)((u8 *)temp_r1 + 0x40B8), (s16) * (u32 *)((u8 *)temp_r1 + 0x40BC), &ov14_021F7BF0);
            }
            ov14_021E6CF8(arg0, arg0->unk21, var_r0);
            ov14_021F40E8(arg0, 0);
            arg0->unk21 = ov14_021E70B0(arg0, arg0->unk21);
            if (arg0->unk21 < 0x1E) {
                *((s32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40C4) = 0;
            } else {
                *((s32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40C4) = 1;
            }
            temp_r1_2 = arg0->unk21;
            if ((u32)temp_r1_2 < 0x1EU) {
                if (ov14_021E6070(arg0, temp_r1_2, 0xAC, 0) == 0) {
                    ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 3U;
                } else {
                    ov14_021E7EE0(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
                    ov14_021E7588(arg0, 0xFF);
                    ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 1U;
                }
            } else {
                ov14_021E7FEC(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
                ov14_021E8434(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
                ov14_021E8824(*((void * *)&arg0->sys));
                ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 1U;
            }
        } else {
            if (temp_r7 == 0) {
                temp_r0_5 = *((void * *)&arg0->sys);
                if ((*(u8 *)((u8 *)temp_r0_5 + 0x44A) == 0) && ((sp8 < 0x10U) || (sp4 < 0x30U) || (sp8 >= 0x68U))) {
                    *(u8 *)((u8 *)temp_r0_5 + 0x44A) = 1U;
                    ov14_021E7FB8(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
                }
            }
            temp_r0_6 = *((void * *)&arg0->sys);
            ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)(temp_r0_6 + (*(u8 *)((u8 *)(temp_r0_6 + arg0->unk21) + 0x4094) * 4)) + 0x2FC), (s16)sp8, (s16)((s16)sp4 - 8));
            ov14_021F4174(arg0);
            *((u32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40B8) = sp8;
            *((u32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40BC) = sp4;
        }
        break;
    case 1:
    case 2:
        if ((ov14_021E6814(arg0) == 0) && (temp_r7 == 0) && (temp_r6 == 0)) {
            ov14_021E7148(arg0, *(void **)((u8 *)temp_r5 + 0xC));
            if ((u32) arg0->unk21 < 0x1EU) {
                arg0->unk21 = 0xFFU;
            }
            ov14_021F4174(arg0);
            ov14_021F40E8(arg0, *((s32 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_40C4));
            ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 5U;
        }
        break;
    case 3:
        if (ov14_021E66F4(arg0) == 0) {
            ov14_021E7148(arg0, *(void **)((u8 *)temp_r5 + 0xC));
            ov14_021E7EE0(((u8 *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0));
            ov14_021E7588(arg0, 0xFF);
            arg0->unk21 = 0xFFU;
            ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = (u16)(((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 + 1);
        }
        break;
    case 4:
        if (temp_r6 == 0) {
            ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 5U;
        }
        break;
    case 5:
        ov14_021F3488(arg0, 1, 1);
        *((u8 *)&((UnkStruct_ov14_boxsys *)arg0->sys)->unk_448[2]) = 0U;
        ((UnkStruct_ov14_boxsys *)temp_r5)->unk_10 = 0U;
        return 0;
    default:
        break;
    }
    return 1;
}
