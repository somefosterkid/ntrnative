#include "global.h"
struct UnkStruct_ov14_boxapp;
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F10DC(void *, s32);
s32 ov14_021F29E4(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F3F6C(void *);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021F6AC0(void *, s32, s32);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F4CA0(UnkStruct_ov14_boxapp *arg0);
void ov14_021EE4E4(void *arg0);
struct UnkStruct_ov14_boxsys;

void ov14_021EE4D8(void *arg0) {
    ov14_021F10DC(arg0, 0x64);
}

s32 ov14_021E98AC(UnkStruct_ov14_boxapp *app);

void ov14_021EE4E4(void *arg0) {
    ov14_021F4CA0(arg0);
    ov14_021F0234(arg0, ov14_021E98AC, 0x65);
}

s32 ov14_021EE4FC(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F6AC0(arg0, 0, 7);
    ov14_021F29E4(arg0->sys, 9, 8);
    ov14_021F2A18(arg0->sys, 9, 0);
    ov14_021F3F6C(arg0);
    ov14_021F40E8(arg0, 1);
    ov14_021F3488(arg0, 2, 0);
    return 0xE;
}
