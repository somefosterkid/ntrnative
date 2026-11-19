#include "global.h"

#include "battle/battler_sprite_context.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "overlay_07.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "unk_02013FDC.h"
#include "wild_encounter.h"

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void ov07_0223494C(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);

void ov07_0223494C(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID) {
    s32 v1;
    s32 gender;
    s32 form;
    int new_var;
    enum HeapID v0;
    PokepicTemplate *pokepicTemplate;
    s32 v2;
    s32 v3;
    s32 whichFacing;
    u32 v4;
    v0 = heapID;
    Pokepic_Pop((Pokepic *)battlerSpriteCtx->sprites[battlerSpriteCtx->targetBattler]);
    pokepicTemplate = Pokepic_GetTemplate((Pokepic *)battlerSpriteCtx->sprites[battlerSpriteCtx->targetBattler]);
    v2 = battlerSpriteCtx->targetBattler;
    v1 = (s32)battlerSpriteCtx->species[v2];
    gender = (s32)battlerSpriteCtx->genders[v2];
    form = (s32)battlerSpriteCtx->forms[v2];
    v4 = battlerSpriteCtx->personalities[v2];
    if (!(1 & battlerSpriteCtx->types[v2])) {
        whichFacing = 0;
    } else {
        whichFacing = 2;
    }
    sub_02014540((enum NarcId)pokepicTemplate->narcID, (s32)pokepicTemplate->charDataID, v0, battlerSpriteCtx->pokemonSpriteData[v2]->tiles, v4, 0, whichFacing, (s32)pokepicTemplate->species);
    battlerSpriteCtx->pokemonSpriteData[battlerSpriteCtx->targetBattler]->narcID = (s32)pokepicTemplate->narcID;
    battlerSpriteCtx->pokemonSpriteData[battlerSpriteCtx->targetBattler]->palette = (s32)pokepicTemplate->palDataID;
    battlerSpriteCtx->pokemonSpriteData[battlerSpriteCtx->targetBattler]->yOffset = (s32)GetMonPicHeightBySpeciesGenderForm((u16)v1, (u8)gender, (u8)whichFacing, (u8)form, v4);
    v3 = battlerSpriteCtx->targetBattler;
    new_var = battlerSpriteCtx->pokemonSpriteData[v3]->yOffset + ov07_02234B5C((s32)battlerSpriteCtx->types[v3], 1);
    Pokepic_SetAttr((Pokepic *)battlerSpriteCtx->sprites[v3], 1, new_var);
}
