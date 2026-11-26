#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

#include "unk_02019BA4.h"
#include "field_system.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);
s32 ov14_021F6F94(UnkStruct_ov14_boxapp *arg0);
void ov14_021E5A50(UnkStruct_ov14_boxsys *sys, UnkFuncPtr_ov14_boxtask task);
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F7AC4(UnkStruct_ov14_boxsys *arg0, s32 target, s32 arg2);
#else
void ov14_021F7AC4(UnkStruct_ov14_boxsys *arg0, s32 target, s32 arg2);
#endif
void ov14_021F708C(UnkStruct_ov14_boxapp *arg0, s32 target, s32 arg2);

s32 ov14_021E9F20(UnkStruct_ov14_boxapp *app);

s32 ov14_021F6F94(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    s32 nextInput;

    nextInput = GridInputHandler_GetNextInput(arg0->sys->unk_2C);
    v0 = GridInputHandler_HandleInput_AllowHold(arg0->sys->unk_2C);
    if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
        if (((u32)v0 >= 0x24U) && ((u32)v0 <= 0x28U)) {
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, nextInput);
            ov14_021F6B28(arg0, nextInput);
            return -1;
        }
    } else {
        if ((u32)(v0 - 0x21) <= 1U) {
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, nextInput);
            ov14_021F6B28(arg0, nextInput);
            return -1;
        }
        if ((v0 == 0x23) || (v0 == -2)) {
            return 0x29;
        }
    }
    return v0;
}

void ov14_021F7010(UnkStruct_ov14_boxapp *arg0, u8 arg1, s32 arg2) {
    u32 v0;
    u8 v1;
    v1 = arg1;
    if (arg2 != (-1)) {
        if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
            v0 = v1 - 0x1E;
            switch (v0) {
            case 0:

            case 3:

            case 4:

            case 5:
                break;

            case 1:

            case 2:
                v1 = 0x1E;
                break;

            default:
                v1 = arg0->unk21;
                if (((u32)v1) >= 0x1EU) {
                    v1 = 0;
                }
                break;
            }

        } else if (((((v1 != 0x24) || (v1 != 0x25)) || (v1 != 0x26)) || (v1 != 0x27)) || (v1 != 0x28)) {
            v1 = 0x24;
        }
    }
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(((v1 & 0xFFu) & 0xFFu) & 0xFFu));
    ov14_021F6B28(arg0, v1);
}

void ov14_021F708C(UnkStruct_ov14_boxapp *arg0, s32 target, s32 arg2) {
    ov14_021F7AC4(arg0->sys, target, arg2);
    ov14_021E5A50(arg0->sys, ov14_021E9F20);
}
