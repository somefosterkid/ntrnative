#include "global.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E70B0(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);

s32 ov14_021E70B0(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    void *v0;
    u32 v1;
    void *v2;

    v1 = 0;
    v0 = ((UnkStruct_ov14_boxsys *)arg0->sys)->unk_C;
    v2 = v0;
loop_1:
    if ((*(s32 *)((u8 *)v2 + 0xC) == 1) && (arg1 == *(s32 *)((u8 *)v2 + 4))) {
        return *(s32 *)((u8 *)(v0 + (v1 << 5)) + 8);
    }
    v1 += 1;
    v2 += 0x20;
    if (v1 >= 7U) {
        return 0xFF;
    }
    goto loop_1;
}
