#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6244(UnkStruct_ov14_boxsys *arg0);
void ov14_021F62E4(UnkStruct_ov14_boxsys *arg0);
void ov14_021F62FC(UnkStruct_ov14_boxsys *arg0);
void ov14_021F6314(struct UnkStruct_ov14_boxapp *arg0);
void ov14_021F62CC(UnkStruct_ov14_boxsys *arg0);
struct UnkStruct_ov14_boxapp;

void ov14_021F6244(UnkStruct_ov14_boxsys *arg0) {
    ov14_021F6208(arg0->unk_2F0, 3, &arg0->windows[29]);
    ov14_021F6208(arg0->unk_2F0, 4, &arg0->windows[30]);
    ov14_021F6208(arg0->unk_2F0, 5, &arg0->windows[31]);
    ov14_021F6208(arg0->unk_2F0, 6, &arg0->windows[32]);
    ov14_021F6208(arg0->unk_2F0, 7, &arg0->windows[33]);
    ov14_021F60BC(arg0->unk_2F0, 0xC, &arg0->windows[41]);
    ov14_021F6208(arg0->unk_2F0, 0xE, &arg0->windows[42]);
    ov14_021F6208(arg0->unk_2F0, 0xF, &arg0->windows[43]);
}

void ov14_021F62CC(UnkStruct_ov14_boxsys *arg0) {
    ov14_021F6208(arg0->unk_2F0, 8, &arg0->windows[24]);
}

void ov14_021F62E4(UnkStruct_ov14_boxsys *arg0) {
    ov14_021F6208(arg0->unk_2F0, 9, &arg0->windows[25]);
}

void ov14_021F62FC(UnkStruct_ov14_boxsys *arg0) {
    ov14_021F6208(arg0->unk_2F0, 0xA, &arg0->windows[26]);
}

void ov14_021F6314(UnkStruct_ov14_boxapp *arg0) {
    UnkStruct_ov14_boxsys *v0;

    v0 = ((void *)arg0->sys);
    ov14_021F6208(v0->unk_2F0, 0xB, &v0->windows[28]);
    ov14_021F5ED4(arg0, 0);
}
