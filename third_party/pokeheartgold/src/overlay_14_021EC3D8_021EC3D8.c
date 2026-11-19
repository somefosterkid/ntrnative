#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2330(void *, s32, s32);
s32 ov14_021F2490(void *, s32, s32);

s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E7588(void *, s32);
s32 ov14_021E765C(void *);
s32 ov14_021E884C(UnkStruct_ov14_boxsys *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F0244(void *, s32);
s32 ov14_021F0594(void *, s32);
s32 ov14_021F07F0(void *, s32);
s32 ov14_021F2270(void *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021F6A24(void *);
s32 ov14_021F7AC4(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021EA180(UnkStruct_ov14_boxapp *app);

extern s32 ov14_021F7D3C;

s32 ov14_021EC3D8(void *arg0) {
    s32 temp_r0_2;
    u32 temp_r0_3;
    u32 temp_r0_4;
    u8 temp_r5;
    void *temp_r2;

    temp_r0_2 = ov14_021F6A24(arg0);
    if (temp_r0_2 != -1U) {
        if (ov14_021E6070(arg0, temp_r0_2 + 0x1E, 0xAC, 0) != 0) {
            PlaySE(0x5EBU);
            temp_r2 = ((void *)((UnkStruct_ov14_boxapp *)arg0)->sys);
            System_GetTouchNewCoords(temp_r2 + 0x40B8, temp_r2 + 0x40BC);
            if (ov14_021E8544(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0) == 0) {
                ov14_021F5EE4(arg0, &ov14_021F7D3C, 5);
            }
            ov14_021E7588(arg0, temp_r0_2 + 0x1E);
            ov14_021F2A18(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 0);
            return ov14_021F0594(arg0, temp_r0_2 + 0x1E);
        }
        if (ov14_021E8544(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0) == 1) {
            temp_r5 = ((UnkStruct_ov14_boxapp *)arg0)->unk21 - 0x1E;
            ov14_021F7AC4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), temp_r5, GridInputHandler_GetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C));
            GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (s32)temp_r5);
            ov14_021E84A4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
            ov14_021E8328(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
            ov14_021E884C(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
            ov14_021F40E8(arg0, 0);
            return ov14_021F0234(arg0, ov14_021EA180, 0x4C);
        }
        GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (u8)temp_r0_2);
        GridInputHandler_SetButtonInputMode(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 1);
        ov14_021E765C(arg0);
        return 0x24;
    }
    temp_r0_2 = ov14_021F7388(arg0);
    switch ((u32)temp_r0_2) {
    case 0xFFFFFFFFU:
        break;
    case 6:
        PlaySE(0x5DDU);
        *((u8 *)arg0 + 0x2A) = 0;
        *((u8 *)arg0 + 0x2B) = 0;
        return ov14_021F2490(arg0, 0, 0x9D);
    case 8:
        PlaySE(0x5DDU);
        if (System_GetTouchNew() == 0) {
            *((u8 *)arg0 + 0x2A) = 2;
        }
        *((u8 *)arg0 + 0x2B) = *((u8 *)arg0 + 0x21);
        return ov14_021F2330(arg0, 3, 0x9D);
    case 9:
        PlaySE(0x5DDU);
        ((UnkStruct_ov14_boxapp *)arg0)->unk2C = 9;
        return ov14_021F2270(arg0, 4, 0x97);
    case 10:
        PlaySE(0x5DDU);
        return ov14_021F2270(arg0, 5, 0x98);
    case 11:
        PlaySE(0x5DDU);
        return ov14_021F2270(arg0, 6, 0x99);
    case 12:
        PlaySE(0x5DDU);
        return ov14_021F2270(arg0, 7, 0x9B);
    case 0xFFFFFFFDU:
        temp_r0_4 = (u32)GridInputHandler_GetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C);
        if (temp_r0_4 <= 5U) {
            ov14_021E7588(arg0, temp_r0_4 + 0x1E);
        } else if ((temp_r0_4 != 8) && (temp_r0_4 != 9) && (temp_r0_4 != 0xA) && (temp_r0_4 != 0xB) && (temp_r0_4 != 0xC) && (temp_r0_4 != 0xD)) {
            ov14_021E765C(arg0);
        }
        PlaySE(0x5DCU);
        return ov14_021F0244(arg0, 0x4C);
    case 0xFFFFFFFEU:
    case 7:
        PlaySE(0x633U);
        return ov14_021F2270(arg0, 0xA, 0x9F);
    case 13:
        PlaySE(0x5DCU);
        temp_r5 = *((u8 *)arg0 + 0x21) - 0x1E;
        ov14_021F7AC4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), temp_r5, GridInputHandler_GetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C));
        GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (s32)temp_r5);
        return ov14_021F2270(arg0, 0xB, 0x9E);
    case 0xFFFFFFFCU:
        temp_r0_3 = (u32)GridInputHandler_GetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C);
        if (temp_r0_3 <= 5U) {
            ov14_021E7588(arg0, temp_r0_3 + 0x1E);
        } else {
            if ((temp_r0_3 != 8) && (temp_r0_3 != 9) && (temp_r0_3 != 0xA) && (temp_r0_3 != 0xB) && (temp_r0_3 != 0xC) && (temp_r0_3 != 0xD)) {
                ov14_021E765C(arg0);
            }
        }
        PlaySE(0x5DC);
        break;
    default:
        if (ov14_021E6070(arg0, temp_r0_2 + 0x1E, 0xAC, 0) != 0) {
            PlaySE(0x5DDU);
            ov14_021F5EE4(arg0, &ov14_021F7D3C, 5);
            ov14_021E7588(arg0, temp_r0_2 + 0x1E);
            ov14_021F7AC4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 8U, GridInputHandler_GetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C));
            GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 8);
            return ov14_021F07F0(arg0, temp_r0_2 + 0x1E);
        }
        break;
    }
    return 0x24;
}
