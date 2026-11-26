#include "global.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E70E0(UnkStruct_ov14_boxapp *arg0, s32 arg1);

void ov14_021E70E0(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    MoveEntry *var_r4;
    s32 temp_r0;
    u32 var_r2;
    u32 var_r5;
    MoveSet *temp_r7;
    MoveEntry *var_r3;
    s32 sp0;
    sp0 = arg1;
    temp_r7 = arg0->sys->unk_C;
    var_r2 = 0;
    sp0 = arg1 - 0x1E;
    var_r3 = temp_r7->e;
    do {
        temp_r0 = var_r2 + 0x1E;
        var_r3->idx = temp_r0;
        var_r3->arg = temp_r0;
        var_r3->active = 0;
        var_r2 += 1;
        var_r3++;
    } while (var_r2 < 6U);
    temp_r7->e[var_r2].active = 0;
    var_r5 = sp0 + 1;
    if (var_r5 < 6U) {
        var_r4 = &temp_r7->e[var_r5];
        do {
            var_r4->arg = (s32)(var_r5 + 0x1D);
            var_r4->active = 1;
            ov14_021E6B48(arg0, var_r4);
            var_r5 += 1;
            var_r4++;
        } while (var_r5 < 6U);
    }
    temp_r7->e[sp0].arg = 0x23;
    temp_r7->e[sp0].active = 1;
    ov14_021E6B48(arg0, &temp_r7->e[sp0]);
}
