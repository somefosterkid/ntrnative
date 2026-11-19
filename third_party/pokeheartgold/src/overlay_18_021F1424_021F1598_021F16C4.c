#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

s32 ov18_021F1598(PokedexAppData *, s32, s32);

void ov18_021F16C4(PokedexAppData *pokedexApp, int a1, int a2, u8 a3) {
    s16 sp14[2];
    s32 sp10;
    s32 temp_r0;
    s32 sp4;
    s32 sp0;
    u16 var_r7;
    u16 var_r6;
    PokedexAppData *var_r5;
    s16 *var_r4;
    s32 spC;
    sp0 = a1;
    var_r5 = pokedexApp;
    var_r6 = 0;
    sp4 = a2;
    var_r7 = 0;
    spC = (a3 + 1) * 5;
    var_r4 = &sp14[0];
    do {
        temp_r0 = (sp0 + var_r7) + (0x1E * var_r5->unk_185E);
        sp10 = temp_r0;
        ManagedSprite_GetPositionXY(var_r5->unk_0670[temp_r0], &sp14[1], &sp14[0]);
        if ((sp14[0]) == 0xE0) {
            sp14[0] = -0x10;
            ov18_021F1598(var_r5, var_r6 + ((0xF * var_r5->unk_1859) - spC), sp10);
            var_r6 += 1;
        }
        sp14[0] = (sp14[0]) + sp4;
        ManagedSprite_SetPositionXY(var_r5->unk_0670[temp_r0], sp14[1], sp14[0]);
        var_r7 += 1;
    } while (((u32)var_r7) < 0x1EU);
}

void ov18_021F1760(PokedexAppData *pokedexApp, int a1, int a2, u8 a3) {
    s16 sp14[2];
    s32 sp10;
    s32 temp_r0;
    s32 sp4;
    s32 sp0;
    int new_var;
    u16 var_r7;
    u16 var_r6;
    PokedexAppData *var_r5;
    s16 *var_r4;
    s32 spC;
    sp0 = a1;
    var_r5 = pokedexApp;
    var_r6 = 0;
    sp4 = a2;
    var_r7 = 0;
    spC = (a3 + 5) * 5;
    var_r4 = &sp14[0];
    do {
        temp_r0 = (sp0 + var_r7) + (0x1E * var_r5->unk_185E);
        sp10 = temp_r0;
        ManagedSprite_GetPositionXY(var_r5->unk_0670[temp_r0], &sp14[1], &sp14[0]);
        if ((sp14[0]) == (-0x10)) {
            sp14[0] = 0xE0;
            new_var = 0xF * var_r5->unk_1859;
            ov18_021F1598(var_r5, var_r6 + (new_var + spC), sp10);
            var_r6 += 1;
        }
        sp14[0] = (sp14[0]) + sp4;
        ManagedSprite_SetPositionXY(var_r5->unk_0670[temp_r0], sp14[1], sp14[0]);
        var_r7 += 1;
    } while (((u32)var_r7) < 0x1EU);
}
