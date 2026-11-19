#include "party.h"
#include "party.h"
#include "pokemon_types_def.h"
s32 ov14_021E6070(void *, u32, s32, s32);

s32 ov14_021E690C(void *, u32, u32);
s32 ov14_021E6B48(void *, void *);

s32 ov14_021E6CC8(void *, u32, u32);


void ov14_021E6CF8(void *arg0, u32 arg1, u32 arg2) {
    void *sp14;
    u32 sp10;
    u32 spC;
    u32 sp8;
    u32 sp4;
    u32 sp0;
    s32 temp_r1_3;
    s32 var_r7;
    u32 temp_r0;
    u32 temp_r0_3;
    u32 temp_r0_2;
    u32 var_r1;
    void *var_r4;
    void *temp_r1;
    void *temp_r1_2;
    void *temp_r1_4;
    void *temp_r1_5;
    void *temp_r2;
    void *var_r2;
    u32 var_r5;
    void *var_r6;

    var_r6 = arg0;
    sp4 = arg2;
    temp_r2 = *(void **)((u8 *)var_r6 + 0x34);
    sp0 = arg1;
    sp14 = *(void **)((u8 *)temp_r2 + 0xC);
    *(s32 *)((u8 *)temp_r2 + 0x40C4) = 1;
    var_r1 = 0;
    var_r2 = sp14;
    do {
        *(s32 *)((u8 *)var_r2 + 4) = 0;
        *(s32 *)((u8 *)var_r2 + 8) = 0;
        *(s32 *)((u8 *)var_r2 + 0xC) = 0;
        var_r1 += 1;
        var_r2 += 0x20;
    } while (var_r1 < 7U);
    *(u32 *)((u8 *)sp14 + 0xEC) = sp4;
    if ((sp4 == 0xFF) || (sp0 == sp4) || (ov14_021E690C(var_r6, sp0, sp4) == 0)) {
        *(u32 *)((u8 *)sp14 + 0xE4) = 0xFFU;
        *(u32 *)((u8 *)sp14 + 0xE8) = 0xFFU;
        *(u32 *)((u8 *)sp14 + 4) = sp0;
        *(u32 *)((u8 *)sp14 + 8) = sp0;
        *(s32 *)((u8 *)sp14 + 0xC) = 1;
        ov14_021E6B48(var_r6, sp14);
        return;
    }
    *(u32 *)((u8 *)sp14 + 0xE4) = sp0;
    *(u32 *)((u8 *)sp14 + 0xE8) = sp4;
    var_r7 = Party_GetCount(*(Party **)((u8 *)var_r6 + 8));
    if (sp4 & 0x80) {
        if (sp0 < 0x1EU) {
            *(u32 *)((u8 *)sp14 + 4) = sp0;
            *(u32 *)((u8 *)sp14 + 8) = sp4;
            *(s32 *)((u8 *)sp14 + 0xC) = 1;
            ov14_021E6B48(var_r6, sp14);
            *(s32 *)((u8 *)(*(void **)((u8 *)var_r6 + 0x34)) + 0x40C4) = 0;
            return;
        }
        sp10 = sp0;
        temp_r0 = sp0 - 0x1E;
        var_r5 = temp_r0 + 1;
        sp10 = temp_r0;
        if (var_r5 < 6U) {
            var_r7 = 1;
            do {
                var_r4 = (u8 *)sp14 + (var_r5 << 5);
                *(s32 *)((u8 *)var_r4 + 4) = (s32) (var_r5 + 0x1E);
                *(s32 *)((u8 *)var_r4 + 8) = (s32) (var_r5 + 0x1D);
                *(s32 *)((u8 *)var_r4 + 0xC) = 1;
                ov14_021E6B48(var_r6, var_r4);
                var_r5 += 1;
            } while (var_r5 < 6U);
        }
        temp_r1 = (u8 *)sp14 + (sp10 << 5);
        *(u32 *)((u8 *)temp_r1 + 4) = sp0;
        *(u32 *)((u8 *)temp_r1 + 8) = sp4;
        *(s32 *)((u8 *)temp_r1 + 0xC) = 1;
        ov14_021E6B48(var_r6, temp_r1);
        *(s32 *)((u8 *)(*(void **)((u8 *)var_r6 + 0x34)) + 0x40C4) = 0;
        return;
    }
    if (sp0 < 0x1EU) {
        if (sp4 < 0x1EU) {
            ov14_021E6CC8(var_r6, sp0, sp4);
            return;
        }
        if ((u32) (sp4 - 0x1E) < (u32) var_r7) {
            ov14_021E6CC8(var_r6, sp0, sp4);
            return;
        }
        var_r7 += 0x1E;
        ov14_021E6CC8(var_r6, sp0, (u32) var_r7);
        return;
    }
    if (sp4 < 0x1EU) {
        if (ov14_021E6070(var_r6, sp4, 0xAC, 0) != 0) {
            ov14_021E6CC8(var_r6, sp0, sp4);
            return;
        }
        sp8 = sp0;
        temp_r0_2 = sp0 - 0x1E;
        var_r5 = temp_r0_2 + 1;
        sp8 = temp_r0_2;
        if (var_r5 < (u32) var_r7) {
            do {
                var_r4 = (u8 *)sp14 + (var_r5 << 5);
                *(s32 *)((u8 *)var_r4 + 4) = (s32) (var_r5 + 0x1E);
                *(s32 *)((u8 *)var_r4 + 8) = (s32) (var_r5 + 0x1D);
                *(s32 *)((u8 *)var_r4 + 0xC) = 1;
                ov14_021E6B48(var_r6, var_r4);
                var_r5 += 1;
            } while (var_r5 < (u32) var_r7);
        }
        temp_r1_2 = (u8 *)sp14 + (sp8 << 5);
        *(u32 *)((u8 *)temp_r1_2 + 4) = sp0;
        *(u32 *)((u8 *)temp_r1_2 + 8) = sp4;
        *(s32 *)((u8 *)temp_r1_2 + 0xC) = 1;
        ov14_021E6B48(var_r6, temp_r1_2);
        temp_r1_3 = var_r7 << 5;
        var_r7 += 0x1D;
        temp_r1_4 = (u8 *)sp14 + temp_r1_3;
        *(u32 *)((u8 *)temp_r1_4 + 4) = sp4;
        *(s32 *)((u8 *)temp_r1_4 + 8) = var_r7;
        *(s32 *)((u8 *)temp_r1_4 + 0xC) = 1;
        ov14_021E6B48(var_r6, temp_r1_4);
        return;
    }
    if ((u32) (sp4 - 0x1E) < (u32) var_r7) {
        ov14_021E6CC8(var_r6, sp0, sp4);
        return;
    }
    spC = sp0;
    temp_r0_3 = sp0 - 0x1E;
    var_r5 = temp_r0_3 + 1;
    spC = temp_r0_3;
    if (var_r5 < (u32) var_r7) {
        do {
            var_r4 = (u8 *)sp14 + (var_r5 << 5);
            *(s32 *)((u8 *)var_r4 + 4) = (s32) (var_r5 + 0x1E);
            *(s32 *)((u8 *)var_r4 + 8) = (s32) (var_r5 + 0x1D);
            *(s32 *)((u8 *)var_r4 + 0xC) = 1;
            ov14_021E6B48(var_r6, var_r4);
            var_r5 += 1;
        } while (var_r5 < (u32) var_r7);
    }
    var_r7 += 0x1D;
    temp_r1_5 = (u8 *)sp14 + (spC << 5);
    *(u32 *)((u8 *)temp_r1_5 + 4) = sp0;
    *(s32 *)((u8 *)temp_r1_5 + 8) = var_r7;
    *(s32 *)((u8 *)temp_r1_5 + 0xC) = 1;
    ov14_021E6B48(var_r6, temp_r1_5);
}
