#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021F0244(void *param_1, u32 param_2);
void ov14_021F1228(u8 *param_1, s32 param_2);
u32 ov14_021F131C(u8 *param_1);
u32 ov14_021F2270(u8 *param_1, s32 param_2, u32 param_3);
void ov14_021F29E4(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);

u32 ov14_021ED820(u8 *param_1) {
    u32 input;

    input = GridInputHandler_HandleInput_AllowHold(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2C);
    switch (input) {
    case 0:
        PlaySE(0x5dd);
        ov14_021F129C(param_1, 0);
        return 0x42;
    case 1:
        PlaySE(0x5dd);
        ov14_021F129C(param_1, 1);
        return 0x42;
    case 2:
        PlaySE(0x5dd);
        ov14_021F129C(param_1, 2);
        return 0x42;
    case 3:
        PlaySE(0x5dd);
        ov14_021F129C(param_1, 3);
        return 0x42;
    case 4:
        PlaySE(0x5dc);
        ov14_021F1228(param_1, -1);
        return 0x42;
    case 5:
        PlaySE(0x5dc);
        ov14_021F1228(param_1, 1);
        return 0x42;
    case 6:
        ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)param_1)->sys), 9, 8);
        return ov14_021F131C(param_1);
    case (u32)-3:
        PlaySE(0x5dc);
        if (((UnkStruct_ov14_boxapp *)param_1)->sys->unk_8 != 0) {
            return ov14_021F0244(param_1, 0x49);
        }
        break;
    case 7:
        ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)param_1)->sys), 9, 8);
    case (u32)-2:
        PlaySE(0x5dc);
        return ov14_021F2270(param_1, 10, 0xa6);
    }
    return 0x42;
}
