#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021F1100(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);
struct UnkStruct_ov14_boxapp;

s32 ov14_021E9434(UnkStruct_ov14_boxapp *app);

void ov14_021F1100(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    *((s8 *)&arg0->unk21) = (float)0xFF;
    ov14_021E84A4(arg0->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021E9434, arg1);
}
