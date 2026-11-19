#include "global.h"
#include "overlay_68_move_relearner.h"
#include "pokemon.h"
#include "pokepic.h"

void ov68_021E7224(MoveRelearnerMonPic *monPic, Pokemon *mon, enum HeapID heapID);

void ov68_021E7224(MoveRelearnerMonPic *monPic, Pokemon *mon, enum HeapID heapID) {
    PokepicTemplate template;
    u32 species;

    monPic->pokepicManager = PokepicManager_Create(heapID);
    GetPokemonSpriteCharAndPlttNarcIds(&template, mon, 2U);
    monPic->pokepic = PokepicManager_CreatePokepic(monPic->pokepicManager, &template, 0x34, 0x80, 0, 0, NULL, NULL);
    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    Pokepic_SetAttr(monPic->pokepic, 0x23, 1 ^ GetMonBaseStat_HandleAlternateForm((s32)species, (s32)GetMonData(mon, MON_DATA_FORM, NULL), 0x1C));
}
