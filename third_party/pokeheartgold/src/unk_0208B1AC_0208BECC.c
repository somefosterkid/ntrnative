#include "global.h"

#include "party.h"
#include "pokemon_icon_idx.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_0208B1AC.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

Pokemon *sub_0208A520(PokemonSummary *summaryScreen);

void sub_0208BE00(PokemonSummary *, void *, s32);

void sub_0208BECC(PokemonSummary *arg0) {
    s32 v0;
    Pokemon *mon;
    Pokemon *pokemon;
    Pokemon *v7;
    s32 slot;
    u8 v1;
    u8 v2;
    void *v3;
    void *v4;

    v7 = sub_0208A520(arg0);
    v3 = ((void *)arg0->args);
    v1 = ((PokemonSummaryArgs *)v3)->unk11;
    switch (v1) {
    case 0:
        v0 = 1;
        break;
    case 2:
        v0 = 0;
        break;
    case 1:
        v0 = (s32) ((PokemonSummaryArgs *)v3)->partyCount;
        break;
    }
    slot = 0;
    if (v0 > 0) {
        do {
            v4 = ((void *)arg0->args);
            v2 = ((PokemonSummaryArgs *)v4)->unk11;
            switch (v2) {
            case 0:
                mon = sub_0208A520(arg0);
                v7 = mon;
                thunk_Sprite_SetPaletteOverride(arg0->sprites[slot + 73], Pokemon_GetIconPalette(mon) + 0xC);
                break;
            case 2:
                thunk_Sprite_SetPaletteOverride(arg0->sprites[slot + 73], Boxmon_GetIconPalette(&v7->box) + 0xC);
                break;
            case 1:
                pokemon = Party_GetMonByIndex(*(Party **)v4, slot);
                v7 = pokemon;
                thunk_Sprite_SetPaletteOverride(arg0->sprites[slot + 73], Pokemon_GetIconPalette(pokemon) + 0xC);
                break;
            }
            sub_0208BE00(arg0, v7, slot + 0x49);
            slot += 1;
        } while (slot < v0);
    }
    if (slot < 6) {
        do {
            thunk_Sprite_SetDrawFlag(arg0->sprites[slot + 73], 0);
            slot += 1;
        } while (slot < 6);
    }
}

void sub_0208BF9C(PokemonSummary *arg0) {
    if (arg0->args->unk11 == 2) {
        Sprite_SetPositionXY(arg0->sprites[73], 0xD7, 0x50);
        Sprite_SetPositionXY(arg0->sprites[74], 0xD7, 0x50);
    }
}
