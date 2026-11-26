#include "global.h"

#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "sprite_system.h"
#include "pokemon_summary_app.h"

void *sub_0208A520(PokemonSummary *summaryScreen);

void sub_0208BE00(PokemonSummary *, void *, s32);

void sub_0208BE70(PokemonSummary *arg0);

void sub_0208BE70(PokemonSummary *arg0) {
    sub_0208BE00(arg0, sub_0208A520(arg0), 0x13);
    thunk_Sprite_SetPaletteOverride(arg0->sprites[19], GetMonIconPaletteEx((u32) arg0->mon.species, (u32) arg0->mon.form, arg0->mon.isEgg) + 0xC);
    Sprite_SetFlipMode(arg0->sprites[19], 1 ^ GetMonBaseStat_HandleAlternateForm((s32) arg0->mon.species, (s32) arg0->mon.form, 0x1C));
}
