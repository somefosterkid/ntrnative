#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2330(void *, s32, s32);

s32 ov14_021F0244(void *, u32);
s32 ov14_021F7B7C(UnkStruct_ov14_boxapp *);

s32 ov14_021F1F44(UnkStruct_ov14_boxapp *param_1) {
    s32 input;

    if (ov14_021F7B7C(param_1) == 1) {
        PlaySE(0x5dd);
        *((u8 *)&param_1->unk26[0]) = 1;
        *((u8 *)&param_1->unk26[1]) = 1;
        *((u8 *)&param_1->unk26[2]) = GridInputHandler_GetNextInput(param_1->sys->unk_2C);
        return ov14_021F2330(param_1, 0xf, 0x97);
    }
    input = ov14_021F7340(param_1);
    if (GridInputHandler_IsButtonInputMode(param_1->sys->unk_2C) == 0) {
        input = -2;
    }
    switch ((u32)input) {
    case -3:
        PlaySE(0x5dc);
        return ov14_021F0244(param_1, 0x74);
    case -2:
    case 36:
        return ov14_021F15C8(param_1, 0xff);
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    default:
        return ov14_021F15C8(param_1, input);
    case -4:
    case -1:
        break;
    }
    return 0x73;
}
