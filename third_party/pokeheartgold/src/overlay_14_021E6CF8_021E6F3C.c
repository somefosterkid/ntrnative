#include "global.h"

#include "party.h"
#include "pokemon_types_def.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

s32 ov14_021E6A3C(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2);

void ov14_021E6B48(UnkStruct_ov14_boxapp *, MoveEntry *);

void ov14_021E6C0C(UnkStruct_ov14_boxapp *arg0, MoveEntry *arg1);

void ov14_021E6F3C(UnkStruct_ov14_boxapp *arg0, s32 arg1, u32 arg2);

void ov14_021E6F3C(UnkStruct_ov14_boxapp *arg0, s32 arg1, u32 arg2) {
    MoveSet *spC;
    s32 sp8;
    u32 sp4;
    s32 sp0;
    s32 temp_r0;
    s32 temp_r1_2;
    s32 var_r7;
    u32 var_r1;
    MoveEntry *temp_r1;
    MoveEntry *temp_r1_3;
    UnkStruct_ov14_boxsys *temp_r2;
    MoveEntry *var_r2;
    MoveEntry *var_r4;
    UnkStruct_ov14_boxapp *var_r6;
    u32 var_r5;
    var_r6 = arg0;
    sp4 = arg2;
    temp_r2 = var_r6->sys;
    sp0 = arg1;
    spC = temp_r2->unk_C;
    temp_r2->unk_40C4 = 1;
    var_r1 = 0;
    var_r2 = spC->e;
    do {
        var_r2->idx = 0;
        var_r2->arg = 0;
        var_r2->active = 0;
        var_r1 += 1;
        var_r2++;
    } while (var_r1 < 7U);
    if ((sp4 == 0xFF) || (ov14_021E6A3C(var_r6, sp0, sp4) == 0)) {
        spC->from = 0xFF;
        spC->to = 0xFFU;
        spC->e[0].idx = sp0;
        spC->e[0].arg = sp0;
        spC->e[0].active = 1;
        ov14_021E6B48(var_r6, spC->e);
        return;
    }
    spC->from = sp0;
    spC->to = sp4;
    var_r7 = Party_GetCount(var_r6->party);
    if (sp4 < 0x1EU) {
        sp8 = sp0;
        temp_r0 = sp0 - 0x1E;
        var_r5 = temp_r0 + 1;
        sp8 = temp_r0;
        if (var_r5 < ((u32)var_r7)) {
            var_r4 = &spC->e[var_r5];
            do {
                var_r4->idx = (s32)(var_r5 + 0x1E);
                var_r4->arg = (s32)(var_r5 + 0x1D);
                var_r4->active = 1;
                ov14_021E6C0C(var_r6, var_r4);
                var_r5 += 1;
                var_r4++;
            } while (var_r5 < ((u32)var_r7));
        }
        temp_r1 = &spC->e[sp8];
        temp_r1->idx = sp0;
        temp_r1->arg = sp4;
        temp_r1->active = 1;
        ov14_021E6C0C(var_r6, temp_r1);
        temp_r1_2 = var_r7 << 5;
        var_r7 += 0x1D;
        temp_r1_3 = (MoveEntry *)((u8 *)spC + temp_r1_2);
        temp_r1_3->idx = sp4;
        temp_r1_3->arg = var_r7;
        temp_r1_3->active = 1;
        ov14_021E6C0C(var_r6, temp_r1_3);
        var_r6->sys->unk_40C4 = 0;
    }
}
