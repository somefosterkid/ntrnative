#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"

#include "overlay_14_unk_struct_ov14_boxapp.h"

#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2330(void *, s32, s32);
int ov14_021F6A14(void);

s32 ov14_021F2270(void *, s32, s32);
s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E7588(void *arg0, s32 arg1);
s32 ov14_021E765C(void *);
void ov14_021E76B8(void *);
s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021E884C(UnkStruct_ov14_boxsys *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F0244(void *, s32);
s32 ov14_021F028C(void *, s32);
s32 ov14_021F0314(void *, s32);
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov14_021F039C(void *, u8);
#else
s32 ov14_021F039C(void *, s32);
#endif
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov14_021F0530(void *, u8);
#else
s32 ov14_021F0530(void *, s32);
#endif
s32 ov14_021F1128(void *);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021F6F94(void *);
s32 ov14_021F7AC4(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021EA180(UnkStruct_ov14_boxapp *app);

extern s32 ov14_021F7D3C;

s32 ov14_021EB388(void *arg0) {
    u32 temp_r0_2;
    u32 var_r5;
    UnkStruct_ov14_boxsys *temp_r2;
    UnkStruct_ov14_boxapp *var_r4;

    var_r4 = arg0;
    var_r5 = ov14_021F6A14();
    if (var_r5 != -1U) {
        if (ov14_021E6070(var_r4, var_r5, 0xAC, 0) != 0) {
            PlaySE(0x5EBU);
            temp_r2 = var_r4->sys;
            System_GetTouchNewCoords(&temp_r2->unk_40B8, &temp_r2->unk_40BC);
            if (ov14_021E8544(var_r4->sys->unk_2F0) == 0) {
                ov14_021F5EE4(var_r4, &ov14_021F7D3C, 5);
            }
            ov14_021E7588(var_r4, var_r5);
            ov14_021F2A18(((void *)var_r4->sys), 9, 0);
            return ov14_021F039C(var_r4, var_r5);
        }
        if (ov14_021E8544(var_r4->sys->unk_2F0) == 1) {
            var_r5 = var_r4->unk21;
            ov14_021F7AC4(((void *)var_r4->sys), var_r5, GridInputHandler_GetNextInput(var_r4->sys->unk_2C));
            GridInputHandler_SetNextInput(var_r4->sys->unk_2C, (s32)var_r5);
            ov14_021E84A4(var_r4->sys->unk_2F0);
            ov14_021E8248(*(void **)&var_r4->sys->unk_2F0);
            ov14_021E82A8(*(void **)&var_r4->sys->unk_2F0);
            ov14_021E8328(var_r4->sys->unk_2F0);
            ov14_021E884C(((void *)var_r4->sys));
            ov14_021F40E8(var_r4, 0);
            return ov14_021F0234(var_r4, ov14_021EA180, 0x4A);
        }
        GridInputHandler_SetNextInput(var_r4->sys->unk_2C, (u8)var_r5);
        GridInputHandler_SetButtonInputMode(var_r4->sys->unk_2C, 1);
        ov14_021E765C(var_r4);
        return 0xC;
    }
    var_r5 = ov14_021F6F94(var_r4);
    switch (var_r5) {
    case 0x1E:
        PlaySE(0x5DDU);
        ov14_021E76B8(var_r4);
        return ov14_021F1128(var_r4);
    case 0x1F:
        PlaySE(0x5DCU);
        ov14_021E76B8(var_r4);
        ov14_021F2A18(((void *)var_r4->sys), 9, 0);
        GridInputHandler_SetNextInput(var_r4->sys->unk_2C, 0x1E);
        return ov14_021F028C(var_r4, 0xC);
    case 0x20:
        PlaySE(0x5DCU);
        ov14_021E76B8(var_r4);
        ov14_021F2A18(((void *)var_r4->sys), 9, 0);
        GridInputHandler_SetNextInput(var_r4->sys->unk_2C, 0x1E);
        return ov14_021F0314(var_r4, 0xC);
    case 0x21:
        PlaySE(0x632U);
        return ov14_021F2270(var_r4, 8, 0x95);
    case 0x22:
        PlaySE(0x632U);
        *((s8 *)&var_r4->unk2A) = 0;
        var_r4->unk2B = 0U;
        return ov14_021F2330(var_r4, 9, 0x96);
    case 0x24:
        PlaySE(0x5DDU);
        if (System_GetTouchNew() == 0) {
            *((s8 *)&var_r4->unk2A) = 1;
        }
        var_r4->unk2B = var_r4->unk21;
        return ov14_021F2330(var_r4, 3, 0xB2);
    case 0x25:
        PlaySE(0x5DDU);
        var_r4->unk2C = 0x25;
        return ov14_021F2270(var_r4, 4, 0x97);
    case 0x26:
        PlaySE(0x5DDU);
        return ov14_021F2270(var_r4, 5, 0x98);
    case 0x27:
        PlaySE(0x5DDU);
        return ov14_021F2270(var_r4, 6, 0x99);
    case 0x28:
        PlaySE(0x5DDU);
        return ov14_021F2270(var_r4, 7, 0x9B);
    case 0x29:
        PlaySE(0x5DCU);
        var_r5 = var_r4->unk21;
        ov14_021F7AC4(((void *)var_r4->sys), var_r5, GridInputHandler_GetNextInput(var_r4->sys->unk_2C));
        GridInputHandler_SetNextInput(var_r4->sys->unk_2C, var_r5);
        return ov14_021F2270(var_r4, 0xB, 0x9C);
    case -1U:
        if (GridInputHandler_GetNextInput(var_r4->sys->unk_2C) == 0x1E) {
            if (gSystem.newKeys & 0x20) {
                PlaySE(0x5DCU);
                ov14_021E76B8(var_r4);
                return ov14_021F028C(var_r4, 0xC);
            }
            if (gSystem.newKeys & 0x10) {
                PlaySE(0x5DCU);
                ov14_021E76B8(var_r4);
                return ov14_021F0314(var_r4, 0xC);
            }
        }
        break;
    case -3U:
        temp_r0_2 = GridInputHandler_GetNextInput(var_r4->sys->unk_2C);
        if (temp_r0_2 < 0x1EU) {
            ov14_021E7588(var_r4, temp_r0_2);
        } else if ((temp_r0_2 != 0x24) && (temp_r0_2 != 0x25) && (temp_r0_2 != 0x26) && (temp_r0_2 != 0x27) && (temp_r0_2 != 0x28) && (temp_r0_2 != 0x29)) {
            ov14_021E765C(var_r4);
        }
        PlaySE(0x5DCU);
        return ov14_021F0244(var_r4, 0x4A);
    case 0x23:
        PlaySE(0x5DDU);
        ov14_021E765C(var_r4);
        return ov14_021F2270(var_r4, 0xA, 0x93);
    case -2U:
        PlaySE(0x5DDU);
        return ov14_021F2270(var_r4, 0xA, 0x94);
    case -4U:
        temp_r0_2 = GridInputHandler_GetNextInput(var_r4->sys->unk_2C);
        if (temp_r0_2 < 0x1EU) {
            ov14_021E7588(var_r4, temp_r0_2);
        } else if ((temp_r0_2 != 0x24) && (temp_r0_2 != 0x25) && (temp_r0_2 != 0x26) && (temp_r0_2 != 0x27) && (temp_r0_2 != 0x28) && (temp_r0_2 != 0x29)) {
            ov14_021E765C(var_r4);
        }
        PlaySE(0x5DCU);
        break;
    default:
        if (ov14_021E6070(var_r4, var_r5, 0xAC, 0) != 0) {
            PlaySE(0x5DDU);
            ov14_021F5EE4(var_r4, &ov14_021F7D3C, 5);
            ov14_021E7588(var_r4, var_r5);
            ov14_021F7AC4(((void *)var_r4->sys), 0x24U, GridInputHandler_GetNextInput(var_r4->sys->unk_2C));
            GridInputHandler_SetNextInput(var_r4->sys->unk_2C, 0x24);
            return ov14_021F0530(var_r4, var_r5);
        }
        break;
    }
    return 0xC;
}
