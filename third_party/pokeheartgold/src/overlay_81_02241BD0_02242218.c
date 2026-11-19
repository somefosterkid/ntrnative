#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"

s32 ov81_022409B0(void *, void *, s32, s32, s32, u8, u8, u8, u8, Party *);

void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);

void ov81_02242E08(BattleFactoryAppPanelSprite *panelSprite, int palIndex);

void ov81_02242E14(BattleFactoryAppPanelSprite *panelSprite, void *pSrc, u32 szByte, enum NNS_G2D_VRAM_TYPE type);

void ov81_02242218(BattleFactoryApp *app, Party *party, u32 slot, u32 arg3) {
    NNSG2dCharacterData *ppCharData;
    void *v0;
    Pokemon *pokemon;
    u32 form;
    Pokemon *mon;
    u16 species;
    void *ptr;

    if (slot < arg3) {
        mon = Party_GetMonByIndex(party, (s32)slot);
        pokemon = mon;
        species = (u16)GetMonData(mon, 5, NULL);
        form = (u32)(u16)GetMonData(mon, 0x70, NULL);
        ptr = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, (s32)GetMonIconNaixEx((u32)species, 0, form), 0, &ppCharData, HEAP_ID_100);
        v0 = ptr;
        ov81_02242E14(app->monIconSprites[app->unk_480], ppCharData->pRawData, 0x200, NNS_G2D_VRAM_TYPE_2DSUB);
        Heap_Free(ptr);
        ov81_02242E08(app->monIconSprites[app->unk_480], GetMonIconPaletteEx((u32)species, form, 0U));
        ov81_02242D88(app->monIconSprites[app->unk_480], 1);
        app->unk_480 = (s32)(1 ^ app->unk_480);
        ov81_02242D88(app->monIconSprites[app->unk_480], 0);
        ov81_022409B0(app, &app->windows[8], (u8)slot, 4, 0, 0xF, 2, 0, 0, party);
    }
}
