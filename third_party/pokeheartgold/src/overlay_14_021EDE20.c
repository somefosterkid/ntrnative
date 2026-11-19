#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F685C(UnkStruct_ov14_boxapp *, s32, s32, s32);
s32 ov14_021E7ED0(void *);
s32 ov14_021E7EE0(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F3044();
void ov14_021EDE38(struct UnkStruct_ov14_boxapp *arg0);
struct UnkStruct_ov14_boxapp;

s32 ov14_021E9518(UnkStruct_ov14_boxapp *app);

void ov14_021EDE20(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F685C(arg0, 0, 0, 0x27);
    ov14_021EDF90(arg0);
}

void ov14_021EDE38(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F3044();
    ov14_021E7ED0(((u8 *)arg0->sys->unk_2F0));
    ov14_021E7EE0(((u8 *)arg0->sys->unk_2F0));
    *((s8 *)&arg0->unk22) = 2;
    ov14_021F0234(arg0, ov14_021E9518, 0x54);
}
