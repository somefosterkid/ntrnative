#include "global.h"

#include "constants/species.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_74_wonder_cards_app2_data.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"

void ov74_0223563C(void);
BOOL ov74_0223567C(void);
void ov74_02235690(void);
void ov74_02235728(NarcId narcID, int tilesID, int plttID, int cellID, int animID, enum GFScreen screen);
Sprite *ov74_02235930(enum GFScreen screen, Sprite *sprite, int x, int y, int animationIdx);

void ov74_0222EC60(WonderCardsApp2Data *appData);

void ov74_0222F478(WonderCardsApp2Data *appData);

void ov74_0222F478(WonderCardsApp2Data *appData) {
    int spriteX, i;
    u32 species;
    NNSG2dCharacterData *charData;
    u8 *ncgrBuffer;

    if (appData->selectedWcSprites[0] == NULL && appData->selectedWcSprites[1] == NULL && appData->selectedWcSprites[2] == NULL) {
        if (ov74_0223567C() == TRUE) {
            ov74_0222EC60(appData);
        }
        ov74_0223563C();
        ov74_02235690();
        ov74_02235728(NARC_a_1_1_3, 30, 27, 29, 28, SCREEN_MAIN);
        GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_3_OFFSET, 0, appData->heapID);
    }

    species = SPECIES_BULBASAUR;
    spriteX = 178;
    for (i = 0; i < 3; i++, spriteX += 25) {
        species = appData->wonderCard->monIcon[i];
        if (species == SPECIES_NONE) {
            if (appData->selectedWcSprites[i]) {
                Sprite_SetDrawFlag(appData->selectedWcSprites[i], FALSE);
            }
            continue;
        }
        appData->selectedWcSprites[i] = ov74_02235930(SCREEN_MAIN, appData->selectedWcSprites[i], spriteX, 16, 10 + i);
        ncgrBuffer = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(species, FALSE, 0), FALSE, &charData, appData->heapID);
        DC_FlushRange(charData->pRawData, 4 * 4 * 0x20);
        GX_LoadOBJ(charData->pRawData, (0x64 + i * 4 * 4) * 0x20, 4 * 4 * 0x20);
        Sprite_SetPaletteOverride(appData->selectedWcSprites[i], GetMonIconPaletteEx(species, 0, FALSE) + 3);
        Heap_Free(ncgrBuffer);
    }
}
