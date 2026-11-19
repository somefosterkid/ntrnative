#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2490(void *, u32, s32);

s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E7588(void *, s32);
s32 ov14_021E765C(void *);
s32 ov14_021E88F8(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F0244(void *, s32);
s32 ov14_021F1CDC(void *, s32);
s32 ov14_021F1D6C(void *, s32);
s32 ov14_021F2270(void *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
u8 ov14_021F6A24(void);
u32 ov14_021F75C8(void *);
s32 ov14_021F7AC4(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021F40DC(void *);

s32 ov14_021EA674(UnkStruct_ov14_boxapp *app);

s32 ov14_021EFB64(UnkStruct_ov14_boxapp *arg0) {
    s32 temp_r0;
    u32 temp_r0_3;
    u32 temp_r0_4;
    s32 temp_r0_2;
    u8 temp_r5;
    void *temp_r1;
    void *temp_r2;

    temp_r0_2 = ov14_021F6A24();
    if (temp_r0_2 != -1U) {
        if (ov14_021E6070(arg0, temp_r0_2 + 0x1E, 0xAC, 0) != 0) {
            PlaySE(0x5EBU);
            temp_r2 = ((void *)arg0->sys);
            System_GetTouchNewCoords(temp_r2 + 0x40B8, temp_r2 + 0x40BC);
            ov14_021E7588(arg0, temp_r0_2 + 0x1E);
            ov14_021F2A18(((void *)arg0->sys), 9, 0);
            return ov14_021F1D6C(arg0, temp_r0_2 + 0x1E);
        }
        if (ov14_021E8544(arg0->sys->unk_2F0) == 1) {
            temp_r5 = arg0->unk21 - 0x1E;
            ov14_021F7AC4(((void *)arg0->sys), temp_r5, GridInputHandler_GetNextInput(arg0->sys->unk_2C));
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)temp_r5);
            ov14_021E84A4(arg0->sys->unk_2F0);
            ov14_021E8328(arg0->sys->unk_2F0);
            ov14_021F40DC(arg0);
            temp_r1 = ((void *)arg0->sys);
            if (((UnkStruct_ov14_boxsys *)temp_r1)->unk_88C8 != 0) {
                ov14_021E88F8(*((void **)&((UnkStruct_ov14_boxsys *)temp_r1)->unk_2F0));
            }
            return ov14_021F0234(arg0, ov14_021EA674, 0x8C);
        }
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, (u8)temp_r0_2);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        ov14_021E765C(arg0);
        return 0x8B;
    }
    temp_r0 = ov14_021F75C8(arg0);
    switch ((u32)temp_r0) {
    case 6:
        PlaySE(0x5DDU);
        return ov14_021F2490(arg0, 0, 0xB0);
    case 8:
        PlaySE(0x5DDU);
        return ov14_021F2270(arg0, 7, 0xAD);
    case -3:
        temp_r0_3 = (u32)GridInputHandler_GetNextInput(arg0->sys->unk_2C);
        if (temp_r0_3 <= 5U) {
            ov14_021E7588(arg0, temp_r0_3 + 0x1E);
        } else if ((temp_r0_3 != 8) && (temp_r0_3 != 9)) {
            ov14_021E765C(arg0);
        }
        PlaySE(0x5DCU);
        return ov14_021F0244(arg0, 0x8C);
    case 7:
    case -2:
        PlaySE(0x633U);
        return ov14_021F2270(arg0, 0xA, 0x9F);
    case 9:
        PlaySE(0x5DCU);
        temp_r5 = arg0->unk21 - 0x1E;
        ov14_021F7AC4(((void *)arg0->sys), temp_r5, GridInputHandler_GetNextInput(arg0->sys->unk_2C));
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)temp_r5);
        return ov14_021F2270(arg0, 0xB, 0xB1);
    case -4:
        temp_r0_4 = (u32)GridInputHandler_GetNextInput(arg0->sys->unk_2C);
        if (temp_r0_4 <= 5U) {
            ov14_021E7588(arg0, temp_r0_4 + 0x1E);
        } else if ((temp_r0_4 != 8) && (temp_r0_4 != 9)) {
            ov14_021E765C(arg0);
        }
        PlaySE(0x5DCU);
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    default:
        if (ov14_021E6070(arg0, temp_r0 + 0x1E, 0xAC, 0) != 0) {
            PlaySE(0x5DDU);
            ov14_021E7588(arg0, temp_r0 + 0x1E);
            ov14_021F7AC4(((void *)arg0->sys), 8U, GridInputHandler_GetNextInput(arg0->sys->unk_2C));
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, 8);
            return ov14_021F1CDC(arg0, temp_r0 + 0x1E);
        }
        break;
    case -1:
        break;
    }
    return 0x8B;
}
