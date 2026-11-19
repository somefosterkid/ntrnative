#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F4A20(struct UnkStruct_ov14_boxapp *arg0, u32 arg1);
struct UnkStruct_ov14_boxapp;

void ov14_021F4A20(UnkStruct_ov14_boxapp *arg0, u32 arg1) {
    u8 *v0;

    if (((s32) arg0->unk25 / 6) == (arg1 / 6U)) {
        v0 = *((u8 **)&arg0->sys);
        ov14_021F2C1C(v0, (arg1 % 6U) + 0xF, v0 + 0x40C8 + (arg1 << 0xA), 0x400);
    }
}
