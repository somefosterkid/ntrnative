#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "player_data.h"

s32 ov18_021F684C(PokedexAppData *, s32, s32, s32);

void ov18_021F69C0(PokedexAppData *arg0, s32 arg1);

void ov18_021F6984(PokedexAppData *pokedexApp, int a1, int a2) {
    ov18_021F684C(pokedexApp, a1, a2, 0);
}

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);

void ov18_021F6990(PokedexAppData *pokedexApp) {
    if (pokedexApp->unk_18C7_0 == 0) {
        ov18_021F1294(pokedexApp, 0xD, 0x40, 0x58, 0);
        return;
    }
    ov18_021F1294(pokedexApp, 0xD, 0xC0, 0x58, 0);
}

void ov18_021F69C0(PokedexAppData *pokedexApp, s32 arg1) {
    s32 a2;

    if (PlayerProfile_GetTrainerGender(pokedexApp->args->playerProfile) == 0) {
        a2 = 0;
    } else {
        a2 = 1;
    }
    if (arg1 == 1) {
        a2 += 2;
    }
    ov18_021F118C(pokedexApp, 2, a2);
}
