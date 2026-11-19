#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E86E0(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F4720(void *arg0);
s32 ov14_021F4848(void *);
s32 ov14_021F48B4(void *);
s32 ov14_021F57B8(void *);
void ov14_021F1058(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);
s32 ov14_021E8704(void *);
void ov14_021F1090(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);
s32 ov14_021F5EC4(void *, s32);
s32 ov14_021E8314(void *);
void ov14_021F10DC(void *arg0, s32 arg1);
struct UnkStruct_ov14_boxapp;

s32 ov14_021E9554(UnkStruct_ov14_boxapp *app);

void ov14_021F1058(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    ov14_021F4720(arg0);
    ov14_021F4848(arg0);
    ov14_021F48B4(arg0);
    ov14_021F57B8(arg0);
    ov14_021E86E0(((u8 *)arg0->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E9554, arg1);
}

s32 ov14_021E9590(UnkStruct_ov14_boxapp *app);

void ov14_021F1090(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    ov14_021E8704(((u8 *)arg0->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E9590, arg1);
}

s32 ov14_021E95B4(UnkStruct_ov14_boxapp *app);

void ov14_021F10B4(void *arg0, s32 arg1) {
    ov14_021F5EC4(arg0, 0);
    ov14_021E8328(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021E95B4, arg1);
}

void ov14_021F10DC(void *arg0, s32 arg1) {
    ov14_021E8314(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E95B4, arg1);
}
