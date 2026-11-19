#include "global.h"

#include "obj_char_transfer.h"
#include "unk_02013534.h"
#include "overlay_14_box_funcs.h"

void ov14_021F4E68(void *param_1);

void ov14_021F4E68(void *param_1) {
    UnkStruct_02021AC8 *a0;
    void *iVar1;
    u32 uVar2;

    uVar2 = 0;
    a0 = (UnkStruct_02021AC8 *)(param_1 + 0x418);
    iVar1 = param_1;
    do {
        sub_02021B5C(a0);
        FontOAM_Delete(*(TextOBJ **)(iVar1 + 0x414));
        uVar2 = uVar2 + 1;
        a0 = (UnkStruct_02021AC8 *)&a0[1].offset;
        iVar1 = iVar1 + 0x10;
    } while (uVar2 < 2);
    sub_020135AC(*(UnkStruct_02013534 **)(param_1 + 0x410));
    return;
}

void ov14_021F4EA0(void *arg0, Window *window, s32 arg2) {
    UnkStruct_02013910 *v0;

    v0 = sub_02013910(window, HEAP_ID_10);
    TextOBJ_CopyFromBGWindow(*(TextOBJ **)((u8 *)(arg0 + (arg2 * 0x10)) + 0x414), v0, window, HEAP_ID_10);
    sub_02013938(v0);
}
