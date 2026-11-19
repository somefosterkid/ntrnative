#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "unk_0201956C.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E85D0(UnkStruct_0201956C *arg0);
void ov14_021E8824(UnkStruct_ov14_boxsys *arg0);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021F08BC(UnkStruct_ov14_boxapp *);
void ov14_021F3190(UnkStruct_ov14_boxsys *, u32, u32);
void ov14_021F3F6C(UnkStruct_ov14_boxapp *);
void ov14_021F6408(UnkStruct_ov14_boxapp *, u32);
s32 ov14_021E8FD4(UnkStruct_ov14_boxapp *app);
void ov14_021F083C(UnkStruct_ov14_boxapp *param_1, u8 param_2);

void ov14_021F083C(UnkStruct_ov14_boxapp *param_1, u8 param_2) {
    s32 sVar1;

    param_1->unk21 = param_2;
    ov14_021F3190(param_1->sys, param_1->unk21, 0);
    ov14_021F3F6C(param_1);
    sVar1 = ov14_021E85E4(param_1->sys->unk_2F0);
    if (sVar1 == 1) {
        ov14_021E85D0(param_1->sys->unk_2F0);
    } else {
        sVar1 = ov14_021E8648(param_1->sys->unk_2F0);
        if (sVar1 == 1) {
            ov14_021E8634(param_1->sys->unk_2F0);
        }
    }
    ov14_021E8824(param_1->sys);
    ov14_021F6408(param_1, 0);
    ov14_021F08BC(param_1);
    param_1->unk22 = 2;
    ov14_021F0234(param_1, ov14_021E8FD4, 0x2a);
    return;
}
