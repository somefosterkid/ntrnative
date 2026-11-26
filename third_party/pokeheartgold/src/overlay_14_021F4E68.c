#include "global.h"

#include "obj_char_transfer.h"
#include "unk_02013534.h"
#include "overlay_14_box_funcs.h"

void ov14_021F4E68(UnkStruct_ov14_boxsys *param_1);

void ov14_021F4E68(UnkStruct_ov14_boxsys *param_1) {
    u32 i;

    for (i = 0; i < 2; i++) {
        sub_02021B5C(&param_1->textObjs[i].charTransfer);
        FontOAM_Delete(param_1->textObjs[i].textObj);
    }
    sub_020135AC(param_1->unk_410);
    return;
}

void ov14_021F4EA0(UnkStruct_ov14_boxsys *arg0, Window *window, s32 arg2) {
    UnkStruct_02013910 *v0;

    v0 = sub_02013910(window, HEAP_ID_10);
    TextOBJ_CopyFromBGWindow(arg0->textObjs[arg2].textObj, v0, window, HEAP_ID_10);
    sub_02013938(v0);
}
