#include <nitro.h>

#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "overlay_96_app_02210C98.h"

s32 ov96_022156A8(s32, s32, void *);

extern s32 ov96_0221D678;

extern s32 ov96_0221D67C;

extern s32 ov96_0221D680;

s32 ov96_02215058(Ov96App_02210C98_Unit *arg0, Ov96App_02210C98_Mover *arg1, u32 arg2, s32 arg3);

s32 ov96_02215058(Ov96App_02210C98_Unit *arg0, Ov96App_02210C98_Mover *arg1, u32 arg2, s32 arg3) {
    s32 sp0;
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r6;
    s32 temp_r0;
    s32 temp_r0_2;
    void *temp_r5;

    switch (arg2) {
    case 5:
    case 8:
        break;
    case 6:
    case 11:
        temp_r6 = (arg3 - 1) * 0xC;
        temp_r1 = arg0->unk_30.x;
        temp_r5 = ((u8 *)&ov96_0221D678) + temp_r6;
        temp_r2 = arg0->unk_30.y;
        if (ov96_022156A8((s32)(temp_r1 + ((u32)(temp_r1 >> 0xB) >> 0x14)) >> 0xC, (temp_r2 / 0x1000), temp_r5) != 0) {
            return 0;
        }
        temp_r0 = LCRandom();
        if ((temp_r0 % 2) == 0) {
            *(u16 *)(&sp0) = (u16) * (u16 *)temp_r5;
            *(u16 *)((u8 *)(&sp0) + 2) = (u16) * (u16 *)((u8 *)temp_r5 + 2);
        } else {
            temp_r0_2 = LCRandom();
            if ((temp_r0_2 % 2) == 0) {
                *(u16 *)(&sp0) = (u16) * (u16 *)(((u8 *)&ov96_0221D67C) + temp_r6);
                *(u16 *)((u8 *)(&sp0) + 2) = (u16) * (u16 *)((u8 *)(((u8 *)&ov96_0221D67C) + temp_r6) + 2);
            } else {
                *(u16 *)(&sp0) = (u16) * (u16 *)(((u8 *)&ov96_0221D680) + temp_r6);
                *(u16 *)((u8 *)(&sp0) + 2) = (u16) * (u16 *)((u8 *)(((u8 *)&ov96_0221D680) + temp_r6) + 2);
            }
        }
        *(u16 *)(&sp0) = (u16)(*(u16 *)(&sp0) + (8 - ((s32)LCRandom() % 17)));
        *(u16 *)((u8 *)(&sp0) + 2) = (u16)(*(u16 *)((u8 *)(&sp0) + 2) + (8 - ((s32)LCRandom() % 17)));
        arg0->unk_24.x = (s32)(*(u16 *)(&sp0) << 0xC);
        arg0->unk_24.y = (s32)(*(u16 *)((u8 *)(&sp0) + 2) << 0xC);
        return 6;
    case 7:
    case 12:
        arg0->unk_24 = arg1->unk_8;
        if (((s32)LCRandom() % 100) < 80) {
            arg0->unk_78 = 1;
        }
        return 10;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
