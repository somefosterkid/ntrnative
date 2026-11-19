#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E8234(UnkStruct_0201956C *param_1);
void ov14_021E8294(UnkStruct_0201956C *param_1);
void ov14_021E8314(UnkStruct_0201956C *param_1);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021F2A18(struct UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);
s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);
void ov14_021F0C88(UnkStruct_ov14_boxapp *param_1);

void ov14_021F0C88(UnkStruct_ov14_boxapp *param_1) {
    param_1->unk23[1] = 1;
    param_1->unk26[3] = 0;
    ov14_021F2A18(param_1->sys, 9, 0);
    ov14_021E8234(param_1->sys->unk_2F0);
    ov14_021E8294(param_1->sys->unk_2F0);
    ov14_021E8314(param_1->sys->unk_2F0);
    ov14_021F0234(param_1, ov14_021E94BC, 0x2f);
    return;
}
