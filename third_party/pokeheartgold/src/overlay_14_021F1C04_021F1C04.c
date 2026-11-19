#include "global.h"

#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021E7588(UnkStruct_ov14_boxapp *param_1, u32 param_2);
void ov14_021E765C(UnkStruct_ov14_boxapp *param_1);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021F1F24(void *param_1);
s32 ov14_021EAC24(UnkStruct_ov14_boxapp *app);
void ov14_021F1C04(UnkStruct_ov14_boxapp *param_1);

void ov14_021F1C04(UnkStruct_ov14_boxapp *param_1) {
    u32 uVar1;
    s32 iVar2;

    uVar1 = GridInputHandler_GetNextInput(param_1->sys->unk_2C);
    if (uVar1 < 0x24) {
        iVar2 = ov14_021E7588(param_1, uVar1);
        if (iVar2 == 1) {
            ov14_021E8620(param_1->sys->unk_2F0);
        }
    } else {
        ov14_021E765C(param_1);
    }
    ov14_021F1F24(param_1);
    ov14_021F0234(param_1, ov14_021EAC24, 0x86);
    return;
}
