#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E805C(UnkStruct_0201956C *arg0);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021F3488(UnkStruct_ov14_boxapp *, u32, u32);
s32 ov14_021E954C(UnkStruct_ov14_boxapp *app);
void ov14_021F0C58(UnkStruct_ov14_boxapp *param_1);

void ov14_021F0C58(UnkStruct_ov14_boxapp *param_1) {
    param_1->unk21 = 0xff;
    ov14_021F3488(param_1, 1, 1);
    ov14_021E805C(param_1->sys->unk_2F0);
    ov14_021F0234(param_1, ov14_021E954C, 0x2e);
    return;
}
