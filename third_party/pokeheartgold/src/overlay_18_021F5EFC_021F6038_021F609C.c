#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);

s32 ov18_021F12C8(PokedexAppData *, s32, s16 *, s16 *, s32);

s32 ov18_021F5FFC(PokedexAppData *, u32, s32);

void ov18_021F609C(PokedexAppData *pokedexApp, int a1) {
    s16 sp8[2];
    s32 sp4;
    PokedexAppData *var_r5;
    s16 temp_r1;
    s32 var_r6;
    u32 var_r4;

    var_r4 = 0;
    var_r5 = pokedexApp;
    sp4 = a1;
    var_r6 = 0;
loop_1:
    ov18_021F12C8(var_r5, var_r4 + 0xE, &sp8[1], &sp8[0], 0);
    temp_r1 = *&sp8[0];
    if ((temp_r1 != -0xA) && (temp_r1 != 0x86)) {
        var_r4 += 1;
        if (var_r4 < 6U) {
            goto loop_1;
        }
    }
    if (sp4 < 0) {
        ov18_021F1294(var_r5, var_r4 + 0xE, 0x30, 0x86, 0);
        if ((s32)(var_r5->unk_18C5 + 2) >= (s32)var_r5->numSeenForms) {
            var_r4 += 0xE;
            ov18_021F11C0(var_r5, (s32)var_r4, 0);
            return;
        }
        ov18_021F11C0(var_r5, var_r4 + 0xE, 1);
        var_r4 += 0xE;
        ov18_021F5FFC(var_r5, var_r4, var_r5->unk_18C5 + 2);
        return;
    }
    ov18_021F1294(var_r5, var_r4 + 0xE, 0x30, -0xA, 0);
    if ((s32)(var_r5->unk_18C5 - 2) < 0) {
        var_r4 += 0xE;
        ov18_021F11C0(var_r5, (s32)var_r4, 0);
        return;
    }
    ov18_021F11C0(var_r5, var_r4 + 0xE, 1);
    var_r4 += 0xE;
    ov18_021F5FFC(var_r5, var_r4, var_r5->unk_18C5 - 2);
}
