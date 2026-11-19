#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "player_data.h"

extern s32 ov18_021FA5CC;

void ov18_021F504C(PokedexAppData *pokedexApp, PokedexAppData_UnkSub0868_State77_Sub4 *a1) {
    PokedexAppData *var_r5;
    u32 var_r2;
    s32 var_r2_2;
    u8 var_r1;
    u32 temp_r1;
    u32 var_r0;
    u32 temp_r0;
    PokedexAppData_UnkSub0868_State77_Sub4 *var_r4;
    s32 *var_r3;

    var_r5 = pokedexApp;
    var_r4 = a1;
    var_r4->pokedexApp = var_r5;
    var_r4->unk_04 = 0;
    var_r4->state = 0;
    if (PlayerProfile_GetTrainerGender(var_r5->args->playerProfile) == 0) {
        var_r0 = 0x190;
    } else {
        var_r0 = 0x19A;
    }
    temp_r1 = *(u32 *)(var_r5->weights + (var_r5->curSpecies * 4));
    if (temp_r1 >= var_r0) {
        var_r2 = var_r0;
        var_r0 = temp_r1;
        var_r1 = 0;
    } else {
        var_r2 = temp_r1;
        var_r1 = 1;
    }
    var_r4->direction = var_r1;
    temp_r0 = (u32)(var_r0 - var_r2) / 10U;
    var_r3 = &ov18_021FA5CC;
    var_r2_2 = 0;
loop_7:
    if ((temp_r0 >= (u32) * (u16 *)var_r3) && (temp_r0 <= (u32) * (u16 *)((u8 *)var_r3 + 2))) {
        var_r4->animIndex = var_r2_2;
        return;
    }
    var_r2_2 += 1;
    var_r3 += 1;
    if ((u32)var_r2_2 >= 0x11U) {
        return;
    }
    goto loop_7;
}
