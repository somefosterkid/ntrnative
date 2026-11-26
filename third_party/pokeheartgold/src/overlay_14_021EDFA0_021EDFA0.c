#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E7588(void *, s32);
s32 ov14_021E765C(void *);
s32 ov14_021E884C(UnkStruct_ov14_boxsys *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F0244(void *, s32);
s32 ov14_021F0660(void *, s32);
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov14_021F0794(void *, u8);
#else
s32 ov14_021F0794(void *, s32);
#endif
s32 ov14_021F2270(void *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021F5EB4(void *, s32);
u32 ov14_021F6A24(void *);
s32 ov14_021F7AC4(UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F6654(void *, int);

s32 ov14_021EA130(UnkStruct_ov14_boxapp *app);

extern s32 ov14_021F7D1C;

s32 ov14_021EDFA0(UnkStruct_ov14_boxapp *arg0) {
    s32 temp_r0;
    u32 temp_r0_3;
    u32 temp_r0_4;
    u8 temp_r5;
    UnkStruct_ov14_boxsys *temp_r2;

    temp_r0 = ov14_021F6A24(arg0);
    if (temp_r0 != -1) {
        if (ov14_021E6070(arg0, temp_r0 + 0x1E, 0xAC, 0) != 0) {
            temp_r2 = arg0->sys;
            System_GetTouchNewCoords(&temp_r2->unk_40B8, &temp_r2->unk_40BC);
            if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
                ov14_021F5EE4(arg0, &ov14_021F7D1C, 4);
            }
            PlaySE(0x5EBU);
            ov14_021E7588(arg0, temp_r0 + 0x1E);
            ov14_021F2A18(arg0->sys, 9, 0);
            return ov14_021F0660(arg0, temp_r0 + 0x1E);
        }
        if (ov14_021E8544(arg0->sys->unk_2F0) == 1) {
            temp_r5 = arg0->unk21 - 0x1E;
            ov14_021F7AC4(arg0->sys, temp_r5, GridInputHandler_GetNextInput(arg0->sys->unk_2C));
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)temp_r5);
            ov14_021F5EB4(arg0, 0);
            ov14_021E84A4(arg0->sys->unk_2F0);
            ov14_021E8328(arg0->sys->unk_2F0);
            ov14_021E884C(arg0->sys);
            ov14_021F40E8(arg0, 0);
            return ov14_021F0234(arg0, ov14_021EA130, 0x70);
        }
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, (u8)temp_r0);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        ov14_021E765C(arg0);
        return 0x5B;
    }
    temp_r0 = ov14_021F6BC0(arg0);
    switch ((u32)temp_r0) {
    case 7:
        PlaySE(0x5DDU);
        return ov14_021F2270(arg0, 4, 0xA9);
    case 8:
        PlaySE(0x5DDU);
        arg0->unk2C = 8;
        return ov14_021F2270(arg0, 5, 0x97);
    case 9:
        PlaySE(0x5DDU);
        ov14_021F6654(arg0->sys, 39);
        return ov14_021F2270(arg0, 6, 0x99);
    case 10:
        PlaySE(0x5DDU);
        return ov14_021F2270(arg0, 7, 0x9B);
    case 11:
        temp_r5 = arg0->unk21 - 0x1E;
        ov14_021F7AC4(arg0->sys, temp_r5, GridInputHandler_GetNextInput(arg0->sys->unk_2C));
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)temp_r5);
        PlaySE(0x5DCU);
        return ov14_021F2270(arg0, 11, 0xAA);
    case -1:
        break;
    case -4:
        temp_r0_3 = (u32)GridInputHandler_GetNextInput(arg0->sys->unk_2C);
        if (temp_r0_3 <= 6U) {
            ov14_021E7588(arg0, temp_r0_3 + 0x1E);
        }
        PlaySE(0x5DCU);
        return 0x5B;
    case -3:
        temp_r0_4 = (u32)GridInputHandler_GetNextInput(arg0->sys->unk_2C);
        if (temp_r0_4 <= 6U) {
            ov14_021E7588(arg0, temp_r0_4 + 0x1E);
        }
        PlaySE(0x5DCU);
        return ov14_021F0244(arg0, 0x6F);
    case 6:
        PlaySE(0x5DDU);
        ov14_021E765C(arg0);
        return ov14_021F2270(arg0, 10, 0x93);
    case -2:
        PlaySE(0x5DDU);
        return ov14_021F2270(arg0, 0xA, 0x94);
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    default:
        if (ov14_021E6070(arg0, temp_r0 + 0x1E, 0xAC, 0) != 0) {
            PlaySE(0x5DDU);
            ov14_021F5EE4(arg0, &ov14_021F7D1C, 4);
            ov14_021E7588(arg0, temp_r0 + 0x1E);
            ov14_021F7AC4(arg0->sys, 7U, GridInputHandler_GetNextInput(arg0->sys->unk_2C));
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, 7);
            return ov14_021F0794(arg0, temp_r0 + 0x1E);
        }
        break;
    }
    return 0x5B;
}
