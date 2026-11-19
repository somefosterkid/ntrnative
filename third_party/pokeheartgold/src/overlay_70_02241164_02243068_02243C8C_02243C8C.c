#include "global.h"

#include "error_handling.h"
#include "list_menu_items.h"
#include "unk_struct_ov70_02242e58.h"

void ov70_02241DB4(void *, u32);

extern u32 ov70_02245F5C;

u32 ov70_02243C8C(UnkStruct_ov70_02242E58 *param_1);

u32 ov70_02243C8C(UnkStruct_ov70_02242E58 *param_1) {
    u32 uVar1;

    ov70_02241DB4(param_1, 3);
    ListMenuItems_Delete(param_1->unk_34);
    if (8 < param_1->unk_3C) {
        GF_AssertFail();
    }
    if (*(s32 *)(&ov70_02245F5C + param_1->unk_3C * 2) == 1) {
        uVar1 = 0x13;
    } else {
        uVar1 = 0x17;
    }
    param_1->unk_4C = uVar1;
    return 0xffffffff;
}
