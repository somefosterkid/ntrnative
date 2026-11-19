#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F63C8(UnkStruct_ov14_boxsys *arg0);

s32 ov14_021F6338(void *, s32, s32, s32);
void ov14_021F638C(void *arg0);
void ov14_021F63A8(void *arg0);
void ov14_021F63B8(void *arg0);
void ov14_021F63F0(UnkStruct_ov14_boxsys *arg0);

void ov14_021F638C(void *arg0) {
    ov14_021F6338(arg0, 0x22, 0x48, 0);
    ov14_021F6338(arg0, 0x23, 0x49, 0);
}

void ov14_021F63A8(void *arg0) {
    ov14_021F6338(arg0, 0x24, 0x4A, 0);
}

void ov14_021F63B8(void *arg0) {
    ov14_021F6338(arg0, 0x24, 0x4B, 0);
}

s32 sub_02019A60(void *, s32, void *);

void ov14_021F63C8(UnkStruct_ov14_boxsys *arg0) {
    sub_02019A60(arg0->unk_2F0, 2, &arg0->windows[34]);
    sub_02019A60(arg0->unk_2F0, 2, &arg0->windows[35]);
}

void ov14_021F63F0(UnkStruct_ov14_boxsys *arg0) {
    sub_02019A60(arg0->unk_2F0, 1, &arg0->windows[36]);
}
