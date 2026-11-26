#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8314(void *);
s32 ov14_021E8434(void *);
s32 ov14_021E8824(UnkStruct_ov14_boxsys *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F3190(void *, u32, s32);
s32 ov14_021F3F6C(void *);
s32 ov14_021F685C(void *, u32, s32, s32);
void ov14_021F0794(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);
void ov14_021F07F0(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);
struct UnkStruct_ov14_boxapp;

s32 ov14_021EA068(UnkStruct_ov14_boxapp *app);

void ov14_021F0794(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    arg0->unk21 = arg1;
    ov14_021F3190(arg0->sys, arg0->unk21, 1);
    ov14_021F3F6C(arg0);
    ov14_021F685C(arg0, arg0->unk21, 1, 0x27);
    ov14_021E8434(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8314(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8824(arg0->sys);
    ov14_021F0234(arg0, ov14_021EA068, 0x6F);
}

s32 ov14_021EA0B8(UnkStruct_ov14_boxapp *app);

void ov14_021F07F0(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    arg0->unk21 = arg1;
    ov14_021F3190(arg0->sys, arg0->unk21, 1);
    ov14_021F3F6C(arg0);
    ov14_021E8434(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8314(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8824(arg0->sys);
    ov14_021F0234(arg0, ov14_021EA0B8, 0x4C);
}
