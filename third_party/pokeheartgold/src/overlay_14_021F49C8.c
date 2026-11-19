#include "global.h"
#include "overlay_14_box_funcs.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

s32 ov14_021F4958(UnkStruct_ov14_boxapp *, u32);
void ov14_021F49C8(UnkStruct_ov14_boxapp *arg0);

void ov14_021F49C8(UnkStruct_ov14_boxapp *arg0) {
    u32 v0;

    v0 = 0;
    do {
        ov14_021F4958(arg0, v0);
        v0 += 1;
    } while (v0 < 0x12U);
}
