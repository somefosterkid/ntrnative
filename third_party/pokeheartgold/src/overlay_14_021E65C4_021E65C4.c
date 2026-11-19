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
    void *var_r4;
    void *var_r6;

    var_r6 = ((UnkStruct_ov14_boxsys *)arg0->sys)->unk_C;
    if (*(s32 *)((u8 *)var_r6 + 0xE0) == 8) {
        var_r7 = 0;
        do {
            if (*(s32 *)((u8 *)var_r6 + 0xC) != 0) {
                temp_r1 = *(u32 *)((u8 *)var_r6 + 4);
                temp_r4 = *(u8 *)((u8 *)(*((void * *)&arg0->sys) + temp_r1) + 0x4094);
                temp_r0 = *(u32 *)((u8 *)var_r6 + 8);
                if (!(0x80 & temp_r0)) {
                    ov14_021F2F88(temp_r0, &sp4[2], (s32 *)sp4, arg0->unk22);
                } else {
                    if (temp_r1 >= 0x1EU) {
                        ov14_021F2F88(0x23U, &sp4[2], (s32 *)sp4, arg0->unk22);
                    } else {
                        ov14_021F2F88(temp_r1, &sp4[2], (s32 *)sp4, arg0->unk22);
                    }
                    ov14_021F2A18(*((void * *)&arg0->sys), temp_r4, 0);
                }
                ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)(*((void * *)&arg0->sys) + (temp_r4 * 4)) + 0x2FC), *(s16 *)((u8 *)((s32 *)sp4) + 2), *(s16 *)((s32 *)sp4));
            }
            var_r7 += 1;
            var_r6 += 0x20;
        } while (var_r7 < 7U);
        return 0;
    }
    *(s32 *)((u8 *)var_r6 + 0xE0) += 1;
    var_r4 = var_r6;
    sp0 = 0;
    do {
        if (*(s32 *)((u8 *)var_r4 + 0xC) != 0) {
            temp_r4 = *(u8 *)((u8 *)(*((void * *)&arg0->sys) + *(s32 *)((u8 *)var_r4 + 4)) + 0x4094);
            *(s16 *)((u8 *)((s32 *)sp4) + 2) = (s16)(*(s16 *)((u8 *)var_r4 + 0x1C) + *(s16 *)((u8 *)var_r4 + 0x18) * ((u32)(*(s32 *)((u8 *)var_r6 + 0xE0) * *(s32 *)((u8 *)var_r4 + 0x10)) >> 0x10));
            *(s16 *)((s32 *)sp4) = (s16)(*(s16 *)((u8 *)var_r4 + 0x1E) + *(s16 *)((u8 *)var_r4 + 0x1A) * ((u32)(*(s32 *)((u8 *)var_r6 + 0xE0) * *(s32 *)((u8 *)var_r4 + 0x14)) >> 0x10));
            ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)(*((void * *)&arg0->sys) + (temp_r4 * 4)) + 0x2FC), *(s16 *)((u8 *)((s32 *)sp4) + 2), *(s16 *)((s32 *)sp4));
            ov14_021F3190(*((void * *)&arg0->sys), *(s32 *)((u8 *)var_r4 + 4), 0);
        }
        var_r4 += 0x20;
        temp_r0_2 = sp0 + 1;
        sp0 = temp_r0_2;
    } while (temp_r0_2 < 7U);
    return 1;
}
