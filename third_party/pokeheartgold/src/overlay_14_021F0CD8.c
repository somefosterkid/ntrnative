#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021F0D34(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);
s32 ov14_021F0EE8(void *, s32);
s32 ov14_021F1090(void *, s32);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F0CD8(struct UnkStruct_ov14_boxapp *arg0);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
struct UnkStruct_ov14_boxapp;

void ov14_021F0CD8(UnkStruct_ov14_boxapp *arg0) {
    *((s8 *)&arg0->unk23[1]) = 0;
    ov14_021F29E4(arg0->sys, 9, 8);
    ov14_021F2A18(arg0->sys, 9, 0);
    if (ov14_021E85E4(arg0->sys->unk_2F0) == 1) {
        ov14_021F0EE8(arg0, 0x31);
        return;
    }
    if (ov14_021E8648(arg0->sys->unk_2F0) == 1) {
        ov14_021F0D34(arg0, 0x31);
        return;
    }
    ov14_021F1090(arg0, 0x32);
}

s32 ov14_021E9970(UnkStruct_ov14_boxapp *app);

void ov14_021F0D34(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    ov14_021E8634(arg0->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021E9970, arg1);
}
