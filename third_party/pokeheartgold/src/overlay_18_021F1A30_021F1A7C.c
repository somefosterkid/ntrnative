#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "unk_02013FDC.h"

extern const UnkStruct_02014E30 ov18_021FA328;

void ov18_021F1A7C(PokedexAppData *pokedexApp, u16 species, u8 form, u8 gender, u8 facing, s32 spriteIdx, s32 mode) {
    PokepicTemplate template;
    UnkStruct_02014E30 rect;
    void *buf;
    u32 personality;
    u32 imageLoc;
    u32 palLoc;
    ManagedSprite **sprites;
    s32 idx;

    rect = ov18_021FA328;
    buf = Heap_AllocAtEnd(HEAP_ID_POKEDEX_APP, 0xC80);
    MI_CpuClearFast(&template, sizeof(template));
    if (species == 327 && facing == 2) {
        personality = Pokedex_GetSeenSpindaPersonality(pokedexApp->args->pokedex, 0);
    } else {
        personality = 0;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&template, species, gender, facing, 0, form, personality);
    sub_02014510((NarcId)(template.narcID), template.charDataID, HEAP_ID_POKEDEX_APP, &rect, buf, personality, 0, facing, species);
    sprites = pokedexApp->unk_0670;
    idx = spriteIdx;
    imageLoc = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprites[idx]->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
    DC_FlushRange(buf, 0xC80);
    GXS_LoadOBJ(buf, imageLoc, 0xC80);
    palLoc = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprites[idx]->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
    if (mode == 0) {
        GfGfxLoader_GXLoadPal((NarcId)(template.narcID), template.palDataID, GF_PAL_LOCATION_SUB_OBJ, (enum GFPalSlotOffset)palLoc, 0x20, HEAP_ID_POKEDEX_APP);
        PaletteData_LoadPaletteSlotFromHardware(pokedexApp->paletteData, PLTTBUF_SUB_OBJ, palLoc / 2, 0x20);
    } else if (mode == 1) {
        PaletteData_LoadNarc(pokedexApp->paletteData, (NarcId)(template.narcID), template.palDataID, HEAP_ID_POKEDEX_APP, PLTTBUF_SUB_OBJ, 0x20, palLoc / 2);
    } else {
        PaletteData_FillPaletteInBuffer(pokedexApp->paletteData, PLTTBUF_SUB_OBJ, PLTTSEL_BOTH, 0, palLoc / 2, palLoc / 2 + 0x10);
    }
    Heap_Free(buf);
}
