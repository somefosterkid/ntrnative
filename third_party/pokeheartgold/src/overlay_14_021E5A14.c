#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

s32 ov14_021F3BC0(UnkStruct_ov14_boxsys *);

s32 sub_02019978(void *, s32);

void ov14_021E5A14(void *arg0);

void ov14_021E5A14(void *arg0) {
    void *v0;

    v0 = ((void *)((UnkStruct_ov14_boxapp *)arg0)->sys);
    if (*(s32 *)((u8 *)(*(void **)arg0) + 8) != 3) {
        sub_02019978(((void *)((UnkStruct_ov14_boxsys *)v0)->unk_2F0), 0x10);
        return;
    }
    sub_02019978(((void *)((UnkStruct_ov14_boxsys *)v0)->unk_2F0), 0x10);
    ov14_021F3BC0(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
}

void ov14_021E5A44(UnkStruct_ov14_boxsys *arg0, UnkFuncPtr_ov14_boxtask arg1);

void ov14_021E5A44(UnkStruct_ov14_boxsys *arg0, UnkFuncPtr_ov14_boxtask arg1) {
    arg0->unk_10 = 0;
    arg0->unk_12 = 0;
    arg0->unk_4 = arg1;
    arg0->unk_8 = NULL;
}



void ov14_021E5A50(UnkStruct_ov14_boxsys *sys, UnkFuncPtr_ov14_boxtask task);

void ov14_021E5A50(UnkStruct_ov14_boxsys *sys, UnkFuncPtr_ov14_boxtask task) {
    sys->unk_8 = task;
}

void ov14_021E5A54(UnkStruct_ov14_boxsys *arg0);

void ov14_021E5A54(UnkStruct_ov14_boxsys *arg0) {
    ov14_021E5A44(arg0, arg0->unk_8);
}
