#include "global.h"

#include "list_menu_items.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02241DB4(void *, s32);

s16 ov70_02243750(UnkStruct_ov70_02242E58 *arg0);

s16 ov70_02243750(UnkStruct_ov70_02242E58 *arg0) {
    ov70_02241DB4(arg0, 0);
    ListMenuItems_Delete(arg0->unk_34);
    return arg0->unk_40;
}
