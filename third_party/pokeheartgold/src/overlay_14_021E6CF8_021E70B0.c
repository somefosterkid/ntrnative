#include "global.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E70B0(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);

s32 ov14_021E70B0(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    MoveSet *v0;
    u32 v1;
    MoveEntry *v2;

    v1 = 0;
    v0 = arg0->sys->unk_C;
    v2 = v0->e;
loop_1:
    if ((v2->active == 1) && (arg1 == v2->idx)) {
        return v0->e[v1].arg;
    }
    v1 += 1;
    v2++;
    if (v1 >= 7U) {
        return 0xFF;
    }
    goto loop_1;
}
