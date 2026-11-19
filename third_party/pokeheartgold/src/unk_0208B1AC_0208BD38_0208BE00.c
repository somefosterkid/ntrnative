#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

void sub_0208BE00(PokemonSummary *summaryScreen, void *mon, s32 spriteIdx);

void sub_0208BE00(PokemonSummary *summaryScreen, void *mon, s32 spriteIdx) {
    NNSG2dCharacterData *ppCharData;
    u32 offset;
    u32 memberNo;
    void *ptr;

    offset = NNS_G2dGetImageLocation(Sprite_GetImageProxy(summaryScreen->sprites[spriteIdx]), NNS_G2D_VRAM_TYPE_2DSUB);
    if (summaryScreen->args->unk11 == 2) {
        memberNo = Boxmon_GetIconNaix(mon);
    } else {
        memberNo = Pokemon_GetIconNaix(mon);
    }
    ptr = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, (s32)memberNo, 0, &ppCharData, HEAP_ID_19);
    DC_FlushRange(ppCharData->pRawData, 0x200U);
    GXS_LoadOBJ(ppCharData->pRawData, offset, 0x200U);
    Heap_Free(ptr);
}
