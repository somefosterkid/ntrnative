#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"
#include "gf_gfx_loader.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "constants/species.h"
#include "constants/sndseq.h"
#include "filesystem_files_def.h"
#include "list_menu_items.h"

void ov74_0222DC60(OverlayManager *appMan, int *state, StateTransitionFuncPtr onCancelStateTransition);
void ov74_0222DC60(OverlayManager *appMan, int *state, StateTransitionFuncPtr onCancelStateTransition) {
    enum WonderCardsAppState nextState;
    WonderCardsAppData *appData = OverlayManager_GetData(appMan);
    extern StateTransitionFuncPtr ov74_0223D0BC;

    u32 input = ListMenu_ProcessInput(appData->listMenu);

    switch (input) {
    case LIST_NOTHING_CHOSEN:
        break;
    case LIST_CANCEL:
        PlaySE(SEQ_SE_DP_SELECT);

        if (onCancelStateTransition) {
            nextState = onCancelStateTransition(appMan);

            if (nextState != -1) {
                *state = nextState;
            }
        }
        break;
    default:
        PlaySE(SEQ_SE_DP_SELECT);

        if (input) {
            if (input < NUM_WC_APP_STATES) {
                *state = input;
            } else {
                ov74_0223D0BC = (StateTransitionFuncPtr)input;
                nextState = ov74_0223D0BC(appMan);

                if (nextState != -1) {
                    *state = nextState;
                }
            }
        }
        break;
    }
}
void ov74_0223563C(void);
BOOL ov74_0223567C(void);
void ov74_02235690(void);
void ov74_02235728(NarcId narcID, int tilesID, int plttID, int cellID, int animID, enum GFScreen screen);
Sprite *ov74_02235930(enum GFScreen screen, Sprite *sprite, int x, int y, int animationIdx);
void ov74_0222D024(WonderCardsAppData *appData);
void ov74_0222DCD4(WonderCardsAppData *appData);
void ov74_0222DCD4(WonderCardsAppData *appData) {
    int spriteX, i;

    if (appData->selectedWcSprites[0] == NULL && appData->selectedWcSprites[1] == NULL && appData->selectedWcSprites[2] == NULL) {
        if (ov74_0223567C() == 1) {
            ov74_0222D024(appData);
        }

        ov74_0223563C();
        ov74_02235690();
        ov74_02235728(NARC_a_1_1_3, 30, 27, 29, 28, SCREEN_MAIN);

        GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_3_OFFSET, 0, appData->heapID);
    }

    u16 species = SPECIES_BULBASAUR;
    spriteX = 178;

    for (i = 0; i < 3; i++, spriteX += 25) {
        species = appData->wonderCards[appData->selectedWondercardSlot]->monIcon[i];

        if (species == SPECIES_NONE) {
            if (appData->selectedWcSprites[i]) {
                Sprite_SetDrawFlag(appData->selectedWcSprites[i], 0);
            }

            continue;
        }

        NNSG2dCharacterData *charData;

        appData->selectedWcSprites[i] = ov74_02235930(SCREEN_MAIN, appData->selectedWcSprites[i], spriteX, 16, 10 + i);
        u8 *ncgrBuffer = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(species, 0, HEAP_ID_DEFAULT), 0, &charData, appData->heapID);

        DC_FlushRange(charData->pRawData, (4 * 4 * (8 * (8 / 2))));
        GX_LoadOBJ(charData->pRawData, (0x64 + i * 4 * 4) * (8 * (8 / 2)), (4 * 4 * (8 * (8 / 2))));

        Sprite_SetPaletteOverride(appData->selectedWcSprites[i], GetMonIconPaletteEx(species, 0, 0) + 3);
        Heap_Free(ncgrBuffer);
    }
}
