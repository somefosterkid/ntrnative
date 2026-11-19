#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F4848(u8 *);
s32 ov14_021F48B4(void *);
s32 ov14_021F57B8(void *);
s32 ov14_021E8560(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F60A8(void *arg0);
void ov14_021EE35C(void *arg0);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F6654(void *, s32);
s32 ov14_021F685C(void *, s32, s32, s32);
s32 ov14_021F6AC0(void *, s32, s32);

void ov14_021EE338(void *arg0) {
    ov14_021F4BC0(arg0);
    ov14_021F4848(arg0);
    ov14_021F48B4(arg0);
    ov14_021F57B8(arg0);
    ov14_021F10B4(arg0, 0x5F);
}

s32 ov14_021E95C8(UnkStruct_ov14_boxapp *app);

void ov14_021EE35C(void *arg0) {
    ov14_021F60A8(arg0);
    ov14_021E8560(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E95C8, 0x60);
}

s32 ov14_021EE380(UnkStruct_ov14_boxapp *arg0) {
    arg0->sys->unk_43C = (s32)((s32) arg0->unk25 % 6);
    ov14_021F6AC0(arg0, 1, arg0->sys->unk_43C);
    ov14_021F29E4(arg0->sys, 9, 0xE);
    ov14_021F6654(arg0->sys, 0x25);
    ov14_021F685C(arg0, 0, 3, 0x27);
    return 0x61;
}
