#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8588(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021EE4AC(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021E9604(UnkStruct_ov14_boxapp *app);

void ov14_021EE4AC(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(arg0->sys, 9, 0);
    ov14_021E8588(((u8 *)arg0->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E9604, 0x63);
}
