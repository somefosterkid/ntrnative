#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F40E8(void *, s32);
void ov14_021F43F4(void *arg0, s32 arg1);
s32 ov14_021E7EC0(void *);
s32 ov14_021E7EE0(void *);
s32 ov14_021E81A8(void *, s32);
s32 ov14_021F2FDC(void *);
s32 ov14_021F5EC4(void *, s32);
s32 ov14_021F63A8(void *);
s32 ov14_021F63F0(UnkStruct_ov14_boxsys *);
void ov14_021EC354(void *arg0);
s32 ov14_021E7588(void **, s32);
s32 ov14_021F6AC0(void **, s32, s32);

s32 ov14_021EC2EC(void *arg0) {
    ov14_021F67B0(arg0, 5, 0x25);
    ((UnkStruct_ov14_boxapp *)arg0)->unk30 = 0x20;
    return 6;
}

s32 ov14_021E9450(UnkStruct_ov14_boxapp *app);

void ov14_021EC300(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F40E8(arg0, 1);
    if ((u32) arg0->unk21 < 0x1EU) {
        ov14_021F43F4(arg0->sys, 1);
        ov14_021F3488(arg0, 1, 0);
    } else {
        ov14_021F3488(arg0, 2, 0);
    }
    ov14_021F0234(arg0, ov14_021E9450, 0xE);
}

s32 ov14_021EC344(void *arg0) {
    GridInputHandler_SetButtonInputMode(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, 1);
    return 0x5B;
}

s32 ov14_021E9518(UnkStruct_ov14_boxapp *app);

void ov14_021EC354(void *arg0) {
    ov14_021E81A8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0), 1);
    ov14_021E7EC0(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021E7EE0(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F63F0(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021F63A8(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021F5EC4(arg0, 0);
    ov14_021F2FDC(arg0);
    ov14_021F0234(arg0, ov14_021E9518, 0x23);
}

s32 ov14_021EC3A8(void **arg0) {
    ov14_021E7588(arg0, 0x1E);
    if (*(s32 *)((u8 *)(*arg0) + 8) == 3) {
        ov14_021F6AC0(arg0, 7, 0);
        return 0x8B;
    }
    ov14_021F6AC0(arg0, 5, 0);
    return 0x24;
}
