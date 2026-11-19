#include "global.h"
#include "overlay_99_pokeathlon_medals.h"

s32 ov98_0221E6E0(s32, s32 *);

void ov99_021E8198(void *arg0);

void ov99_021E8198(void *arg0) {
    s32 sp10[13] = { 0 };
    s32 spC;
    u32 sp8;
    u32 sp4;
    s32 sp0;
    s32 temp_r7;
    u32 temp_r0;
    u32 var_r4;

    sp8 = 0;
    sp4 = 0;
    sp0 = 0x20;
    do {
        u32 var_r5;
        var_r4 = 0;
        var_r5 = 0x30;
        spC = sp0;
        do {
            temp_r7 = var_r4 + sp4;
            sp10[5] = temp_r7 + 2;
            sp10[6] = 1;
            sp10[7] = 1;
            sp10[8] = 1;
            sp10[4] = 1;
            *(u16 *)(&sp10[0]) = var_r5;
            *(s16 *)((u8 *)(&sp10[0]) + 2) = spC;
            sp10[11] = 1;
            *((s32 *)&((PokeathlonMedals *)arg0)->sprites[(temp_r7 + 0x14)]) = ov98_0221E6E0(*((s32 *)&((PokeathlonMedals *)arg0)->spriteContext), &sp10[0]);
            var_r4 += 1;
            var_r5 += 0x20;
        } while (var_r4 < 6U);
        sp4 += 6;
        sp0 += 0x18;
        temp_r0 = sp8 + 1;
        sp8 = temp_r0;
    } while (temp_r0 < 5U);
}
