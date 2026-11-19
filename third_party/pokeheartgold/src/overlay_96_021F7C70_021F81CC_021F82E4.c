#include "global.h"

#include "error_handling.h"
#include "overlay_96_unk_struct_ov96_021_f7934.h"

void ov96_021F82E4(UnkStruct_ov96_021F8094 *arg0, void *arg1, s32 arg2);

void ov96_021F82E4(UnkStruct_ov96_021F8094 *arg0, void *arg1, s32 arg2) {
    UnkStruct_ov96_021F8128 *temp_r4;

    temp_r4 = &arg0->slots[arg0->head];
    if (temp_r4->active != 0) {
        GF_AssertFail();
    }
    temp_r4->active = 1;
    temp_r4->sprite = arg1;
    temp_r4->delay = arg2;
    arg0->head = (u8)((arg0->head + 1) % 4);
    arg0->shown = (u8)(arg0->shown + 1);
}
