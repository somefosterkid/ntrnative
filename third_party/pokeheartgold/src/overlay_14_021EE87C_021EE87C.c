#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2330(void *, s32, s32);
int ov14_021F6A14(void);

s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E7588(void *arg0, s32 arg1);
s32 ov14_021E765C(void *);
s32 ov14_021E76B8(void *);
s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021E88F8(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F0244(void *, s32);
s32 ov14_021F028C(void *, s32);
s32 ov14_021F0314(void *, s32);
s32 ov14_021F1128(void *);
s32 ov14_021F1808(void *, u32);
s32 ov14_021F18B0(void *, s32);
s32 ov14_021F2270(void *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
u32 ov14_021F74B0(void *);
s32 ov14_021F7AC4(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021F40DC(void *);

s32 ov14_021EA674(UnkStruct_ov14_boxapp *app);

s32 ov14_021EE87C(UnkStruct_ov14_boxapp *arg0) {
    u32 temp_r0;
    s32 temp_r0_3;
    u32 temp_r0_2;
    u32 temp_r5;
    u32 temp_r5_2;
    void *temp_r1;
    void *temp_r2;

    temp_r0_2 = ov14_021F6A14();
    if (temp_r0_2 != -1U) {
        if (ov14_021E6070(arg0, temp_r0_2, 0xAC, 0) != 0) {
            PlaySE(0x5EBU);
            temp_r2 = ((void *)arg0->sys);
            System_GetTouchNewCoords(temp_r2 + 0x40B8, temp_r2 + 0x40BC);
            ov14_021E7588(arg0, temp_r0_2);
            ov14_021F2A18(((void *)arg0->sys), 9, 0);
            return ov14_021F18B0(arg0, temp_r0_2);
        }
        if (ov14_021E8544(arg0->sys->unk_2F0) == 1) {
            temp_r5 = arg0->unk21;
            ov14_021F7AC4(((void *)arg0->sys), temp_r5, GridInputHandler_GetNextInput(arg0->sys->unk_2C));
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, temp_r5);
            ov14_021E84A4(arg0->sys->unk_2F0);
            ov14_021E8248(*(void **)&arg0->sys->unk_2F0);
            ov14_021E82A8(*(void **)&arg0->sys->unk_2F0);
            ov14_021E8328(arg0->sys->unk_2F0);
            ov14_021F40DC(arg0);
            temp_r1 = ((void *)arg0->sys);
            if (((UnkStruct_ov14_boxsys *)temp_r1)->unk_88C8 != 0) {
                ov14_021E88F8(*((void **)&((UnkStruct_ov14_boxsys *)temp_r1)->unk_2F0));
            }
            return ov14_021F0234(arg0, ov14_021EA674, 0x76);
        }
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, (u8)temp_r0_2);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        ov14_021E765C(arg0);
        return 0x75;
    }
    temp_r0 = ov14_021F74B0(arg0);
    switch (temp_r0) {
    case 0x1E:
        PlaySE(0x5DDU);
        ov14_021E76B8(arg0);
        ov14_021F2A18(((void *)arg0->sys), 0xB, 0);
        return ov14_021F1128(arg0);
    case 0x1F:
        PlaySE(0x5DCU);
        ov14_021E76B8(arg0);
        ov14_021F2A18(((void *)arg0->sys), 0xB, 0);
        ov14_021F2A18(((void *)arg0->sys), 9, 0);
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, 0x1E);
        return ov14_021F028C(arg0, 0x75);
    case 0x20:
        PlaySE(0x5DCU);
        ov14_021E76B8(arg0);
        ov14_021F2A18(((void *)arg0->sys), 0xB, 0);
        ov14_021F2A18(((void *)arg0->sys), 9, 0);
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, 0x1E);
        return ov14_021F0314(arg0, 0x75);
    case 0x21:
        PlaySE(0x632U);
        return ov14_021F2270(arg0, 8, 0xAB);
    case 0x22:
        PlaySE(0x632U);
        return ov14_021F2330(arg0, 9, 0xAC);
    case 0x24:
        PlaySE(0x5DDU);
        return ov14_021F2270(arg0, 7, 0xAD);
    case 0x25:
        PlaySE(0x5DCU);
        temp_r5_2 = arg0->unk21;
        ov14_021F7AC4(((void *)arg0->sys), temp_r5_2, GridInputHandler_GetNextInput(arg0->sys->unk_2C));
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, temp_r5_2);
        return ov14_021F2270(arg0, 0xB, 0xAE);
    case -1:
        if (GridInputHandler_GetNextInput(arg0->sys->unk_2C) != 0x1E) {
            break;
        }
        if (0x20 & gSystem.newKeys) {
            PlaySE(0x5DCU);
            ov14_021E76B8(arg0);
            return ov14_021F028C(arg0, 0x75);
        }
        if (0x10 & gSystem.newKeys) {
            PlaySE(0x5DCU);
            ov14_021E76B8(arg0);
            return ov14_021F0314(arg0, 0x75);
        }
        break;
    case -3:
        temp_r0_3 = GridInputHandler_GetNextInput(arg0->sys->unk_2C);
        if ((u32)temp_r0_3 < 0x1EU) {
            ov14_021E7588(arg0, temp_r0_3);
        } else if ((temp_r0_3 != 0x24) && (temp_r0_3 != 0x25)) {
            ov14_021E765C(arg0);
        }
        PlaySE(0x5DCU);
        return ov14_021F0244(arg0, 0x76);
    case 0x23:
        PlaySE(0x5DDU);
        ov14_021E765C(arg0);
        return ov14_021F2270(arg0, 0xA, 0x93);
    case -2:
        PlaySE(0x5DDU);
        return ov14_021F2270(arg0, 0xA, 0x94);
    case -4:
        temp_r0_3 = GridInputHandler_GetNextInput(arg0->sys->unk_2C);
        if ((u32)temp_r0_3 < 0x1EU) {
            ov14_021E7588(arg0, temp_r0_3);
        } else if ((temp_r0_3 != 0x24) && (temp_r0_3 != 0x25)) {
            ov14_021E765C(arg0);
        }
        PlaySE(0x5DCU);
        break;
    default:
        if (ov14_021E6070(arg0, temp_r0, 0xAC, 0) != 0) {
            PlaySE(0x5DDU);
            ov14_021E7588(arg0, temp_r0);
            ov14_021F7AC4(((void *)arg0->sys), 0x24U, GridInputHandler_GetNextInput(arg0->sys->unk_2C));
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, 0x24);
            return ov14_021F1808(arg0, temp_r0);
        }
        break;
    }
    return 0x75;
}
