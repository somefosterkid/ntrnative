#include "global.h"

#include "party.h"
#include "pokemon_types_def.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

s32 ov14_021E6A3C(void *, s32, u32);

s32 ov14_021E6B48(void *, void *);

s32 ov14_021E6C0C();

void ov14_021E6F3C(void *arg0, s32 arg1, u32 arg2);

void ov14_021E6F3C(void *arg0, s32 arg1, u32 arg2) {
    void *spC;
    s32 sp8;
    u32 sp4;
    s32 sp0;
    s32 temp_r0;
    s32 temp_r1_2;
    s32 var_r7;
    u32 var_r1;
    void *temp_r1;
    void *temp_r1_3;
    UnkStruct_ov14_boxsys *temp_r2;
    void *var_r2;
    void *var_r4;
    void *var_r6;
    u32 var_r5;
    var_r6 = arg0;
    sp4 = arg2;
    temp_r2 = ((UnkStruct_ov14_boxapp *)var_r6)->sys;
    sp0 = arg1;
    spC = temp_r2->unk_C;
    temp_r2->unk_40C4 = 1;
    var_r1 = 0;
    var_r2 = spC;
    do {
        *((s32 *)(((u8 *)var_r2) + 4)) = 0;
        *((s32 *)(((u8 *)var_r2) + 8)) = 0;
        *((s32 *)(((u8 *)var_r2) + 0xC)) = 0;
        var_r1 += 1;
        var_r2 += 0x20;
    } while (var_r1 < 7U);
    if ((sp4 == 0xFF) || (ov14_021E6A3C(var_r6, sp0, sp4) == 0)) {
        *((s32 *)(((u8 *)spC) + 0xE4)) = 0xFF;
        *((u32 *)(((u8 *)spC) + 0xE8)) = 0xFFU;
        *((s32 *)(((u8 *)spC) + 4)) = sp0;
        *((s32 *)(((u8 *)spC) + 8)) = sp0;
        *((s32 *)(((u8 *)spC) + 0xC)) = 1;
        ov14_021E6B48(var_r6, spC);
        return;
    }
    *((s32 *)(((u8 *)spC) + 0xE4)) = sp0;
    *((u32 *)(((u8 *)spC) + 0xE8)) = sp4;
    var_r7 = Party_GetCount(((UnkStruct_ov14_boxapp *)var_r6)->party);
    if (sp4 < 0x1EU) {
        sp8 = sp0;
        temp_r0 = sp0 - 0x1E;
        var_r5 = temp_r0 + 1;
        sp8 = temp_r0;
        if (var_r5 < ((u32)var_r7)) {
            var_r4 = spC + (var_r5 << 5);
            do {
                *((s32 *)(((u8 *)var_r4) + 4)) = (s32)(var_r5 + 0x1E);
                *((s32 *)(((u8 *)var_r4) + 8)) = (s32)(var_r5 + 0x1D);
                *((s32 *)(((u8 *)var_r4) + 0xC)) = 1;
                ov14_021E6C0C(var_r6, var_r4);
                var_r5 += 1;
                var_r4 += 0x20;
            } while (var_r5 < ((u32)var_r7));
        }
        temp_r1 = spC + (sp8 << 5);
        *((s32 *)(((u8 *)temp_r1) + 4)) = sp0;
        *((u32 *)(((u8 *)temp_r1) + 8)) = sp4;
        *((s32 *)(((u8 *)temp_r1) + 0xC)) = 1;
        ov14_021E6C0C(var_r6);
        temp_r1_2 = var_r7 << 5;
        var_r7 += 0x1D;
        temp_r1_3 = spC + temp_r1_2;
        *((u32 *)(((u8 *)temp_r1_3) + 4)) = sp4;
        *((s32 *)(((u8 *)temp_r1_3) + 8)) = var_r7;
        *((s32 *)(((u8 *)temp_r1_3) + 0xC)) = 1;
        ov14_021E6C0C(var_r6);
        ((UnkStruct_ov14_boxapp *)var_r6)->sys->unk_40C4 = 0;
    }
}
