#include "global.h"

#include "gf_gfx_loader.h"
#include "overlay_96_ov96_icon_work.h"
#include "pokemon_icon_idx.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

void ov96_021FC320(Ov96IconWork *work) {
    s32 i;

    work->spriteList = G2dRenderer_Init(0x24, &work->renderer, work->heapID);
    for (i = GF_GFX_RES_TYPE_CHAR; i < 6; i++) {
        work->resMan[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, work->heapID);
    }
    work->charRes = AddCharResObjFromNarc(work->resMan[0], NARC_a_1_5_6, 0x18, 0, 0x80, 2, work->heapID);
    work->plttRes = AddPlttResObjFromNarc(work->resMan[1], NARC_a_1_5_6, 0x15, 0, 0x80, 2, 1, work->heapID);
    work->cellRes = AddCellOrAnimResObjFromNarc(work->resMan[2], NARC_a_1_5_6, 0x17, 0, 0x80, GF_GFX_RES_TYPE_CELL, work->heapID);
    work->animRes = AddCellOrAnimResObjFromNarc(work->resMan[3], NARC_a_1_5_6, 0x16, 0, 0x80, GF_GFX_RES_TYPE_ANIM, work->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(work->charRes);
    SpriteTransfer_CreateExtPlttTransferTask(work->plttRes);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_3_OFFSET, 0U, work->heapID);
}

void ov96_021FC404(Ov96IconWork *work) {
    CreateSpriteResourcesHeader(&work->resHeader, 0x80, 0x80, 0x80, 0x80, -1, -1, 0, 0, work->resMan[0], work->resMan[1], work->resMan[2], work->resMan[3], NULL, NULL);
}
