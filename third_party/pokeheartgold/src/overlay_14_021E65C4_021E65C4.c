#include "global.h"

#include "sprite_system.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2F88(u32, void *, s32 *, u32);
s32 ov14_021F3190(void *, s32, s32);
s32 ov14_021E65C4(struct UnkStruct_ov14_boxapp *arg0);

s32 ov14_021E65C4(UnkStruct_ov14_boxapp *arg0) {
    u8 sp4[4];
    u32 sp0;
    u32 temp_r0;
    u32 temp_r0_2;
    u32 temp_r1;
    u32 var_r7;
    u8 temp_r4;
    MoveEntry *var_r4;
    MoveSet *var_r6;
    MoveEntry *e;

    var_r6 = arg0->sys->unk_C;
    if (var_r6->frame == 8) {
        var_r7 = 0;
        e = var_r6->e;
        do {
            if (e->active != 0) {
                temp_r1 = e->idx;
                temp_r4 = arg0->sys->unk_4094[temp_r1];
                temp_r0 = e->arg;
                if (!(0x80 & temp_r0)) {
                    ov14_021F2F88(temp_r0, &sp4[2], (s32 *)sp4, arg0->unk22);
                } else {
                    if (temp_r1 >= 0x1EU) {
                        ov14_021F2F88(0x23U, &sp4[2], (s32 *)sp4, arg0->unk22);
                    } else {
                        ov14_021F2F88(temp_r1, &sp4[2], (s32 *)sp4, arg0->unk22);
                    }
                    ov14_021F2A18(arg0->sys, temp_r4, 0);
                }
                ManagedSprite_SetPositionXY(arg0->sys->sprites[temp_r4], *(s16 *)((u8 *)((s32 *)sp4) + 2), *(s16 *)((s32 *)sp4));
            }
            var_r7 += 1;
            e++;
        } while (var_r7 < 7U);
        return 0;
    }
    var_r6->frame += 1;
    var_r4 = var_r6->e;
    sp0 = 0;
    do {
        if (var_r4->active != 0) {
            temp_r4 = arg0->sys->unk_4094[var_r4->idx];
            *(s16 *)((u8 *)((s32 *)sp4) + 2) = (s16)(var_r4->x0 + var_r4->dx * ((u32)(var_r6->frame * var_r4->mulX) >> 0x10));
            *(s16 *)((s32 *)sp4) = (s16)(var_r4->y0 + var_r4->dy * ((u32)(var_r6->frame * var_r4->mulY) >> 0x10));
            ManagedSprite_SetPositionXY(arg0->sys->sprites[temp_r4], *(s16 *)((u8 *)((s32 *)sp4) + 2), *(s16 *)((s32 *)sp4));
            ov14_021F3190(arg0->sys, var_r4->idx, 0);
        }
        var_r4++;
        temp_r0_2 = sp0 + 1;
        sp0 = temp_r0_2;
    } while (temp_r0_2 < 7U);
    return 1;
}
