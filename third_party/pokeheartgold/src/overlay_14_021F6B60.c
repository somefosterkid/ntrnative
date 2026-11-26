#include "global.h"

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);
s32 ov14_021F2A18(UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021E5A50(UnkStruct_ov14_boxsys *sys, UnkFuncPtr_ov14_boxtask task);
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F7AC4(UnkStruct_ov14_boxsys *arg0, s32 target, s32 arg2);
#else
void ov14_021F7AC4(UnkStruct_ov14_boxsys *arg0, s32 target, s32 arg2);
#endif
void ov14_021F6B8C(UnkStruct_ov14_boxapp *arg0, s32 target, s32 arg2);

s32 ov14_021E9F20(UnkStruct_ov14_boxapp *app);

void ov14_021F6B60(void *arg0, u8 arg1) {
    GridInputHandler_SetNextInput(*((GridInputHandler **)(((u8 *)(*((void **)(((u8 *)arg0) + (0x34 & 0xFF))))) + 0x2C)), ((((((((((((s32)arg1) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu);
    ov14_021F6B28(arg0, arg1);
}

void ov14_021F6B7C(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(arg0->sys, 9, 0);
}

void ov14_021F6B8C(UnkStruct_ov14_boxapp *arg0, s32 target, s32 arg2) {
    ov14_021F7AC4(arg0->sys, target, arg2);
    ov14_021E5A50(arg0->sys, ov14_021E9F20);
}

void ov14_021F6BA4(void *arg0, u8 arg1) {
    unsigned char new_var;
    new_var = (s32)arg1;
    GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, new_var);
    ov14_021F6B28(arg0, arg1);
}
