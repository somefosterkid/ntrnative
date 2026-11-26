#include "global.h"

#include "gf_gfx_loader.h"
#include "pokemon_icon_idx.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "pokemon.h"
#include "unk_02013FDC.h"
#include "overlay_65_trade_room.h"

void ov65_0221D3E8(TradeRoom *arg0, NARC *arg1);

s32 ov65_0221D57C(s32 arg0, Pokemon *arg1, void *arg2, PokepicTemplate *arg3);

void ov65_0221D3E8(TradeRoom *arg0, NARC *narc) {
    GfGfxResType type;
    struct SpriteResourceCollection **v0;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x64, 0, 0x20, 0, 0x64, 0, 0x20, HEAP_ID_26);
    arg0->spriteList = G2dRenderer_Init(0x2D, &arg0->spriteRenderer, HEAP_ID_26);
    type = GF_GFX_RES_TYPE_CHAR;
    v0 = (struct SpriteResourceCollection **)arg0;
    do {
        *(GF_2DGfxResMan **)((u8 *)v0 + offsetof(TradeRoom, spriteResourceCollections)) = Create2DGfxResObjMan(2, type, HEAP_ID_26);
        v0 += 1;
        type += 1;
    } while (type < 4);
    arg0->spriteResources[0][0] = AddCharResObjFromOpenNarc(((GF_2DGfxResMan *)arg0->spriteResourceCollections[0]), narc, 7, 1, 0, 1, HEAP_ID_26);
    arg0->spriteResources[0][1] = AddPlttResObjFromOpenNarc(((GF_2DGfxResMan *)arg0->spriteResourceCollections[1]), narc, 0, 0, 0, 1, 9, HEAP_ID_26);
    arg0->spriteResources[1][0] = AddCellOrAnimResObjFromOpenNarc(((GF_2DGfxResMan *)arg0->spriteResourceCollections[2]), narc, 8, 1, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_26);
    arg0->spriteResources[1][1] = AddCellOrAnimResObjFromOpenNarc(((GF_2DGfxResMan *)arg0->spriteResourceCollections[3]), narc, 9, 1, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_26);
    arg0->spriteResources[2][0] = AddCharResObjFromOpenNarc(((GF_2DGfxResMan *)arg0->spriteResourceCollections[0]), narc, 0xA, 1, 1, 2, HEAP_ID_26);
    arg0->spriteResources[2][1] = AddPlttResObjFromOpenNarc(((GF_2DGfxResMan *)arg0->spriteResourceCollections[1]), narc, 0, 0, 1, 2, 0xA, HEAP_ID_26);
    arg0->spriteResources[3][0] = AddCellOrAnimResObjFromOpenNarc(((GF_2DGfxResMan *)arg0->spriteResourceCollections[2]), narc, 0xB, 1, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_26);
    arg0->spriteResources[3][1] = AddCellOrAnimResObjFromOpenNarc(((GF_2DGfxResMan *)arg0->spriteResourceCollections[3]), narc, 0xC, 1, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_26);
    SpriteTransfer_CreateCharTransferTask(arg0->spriteResources[0][0]);
    SpriteTransfer_CreateCharTransferTask(arg0->spriteResources[2][0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->spriteResources[0][1]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->spriteResources[2][1]);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_10_OFFSET, 0x80U, HEAP_ID_26);
}

s32 ov65_0221D57C(s32 arg0, Pokemon *mon, void *arg2, PokepicTemplate *pokepicTemplate) {
    u32 pid;
    PokepicTemplate *v0;
    u32 species;

    v0 = pokepicTemplate;
    GetPokemonSpriteCharAndPlttNarcIds(pokepicTemplate, mon, 2U);
    pid = GetMonData(mon, 0, NULL);
    species = GetMonData(mon, 5, NULL);
    if ((GetMonData(mon, 0x4C, NULL) == 1) && (species == 0x147)) {
        species = 0x1EE;
    }
    sub_02014494((enum NarcId)v0->narcID, (s32)v0->charDataID, HEAP_ID_26, 0, 0, 0xA, 0xA, arg2, pid, 0, 2, (s32)species);
    DC_FlushRange(arg2, 0xC80U);
    return arg0 + 1;
}
