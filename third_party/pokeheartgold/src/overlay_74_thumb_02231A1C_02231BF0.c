#include "global.h"

#include "gf_gfx_loader.h"
#include "pokemon_icon_idx.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "overlay_74_gbamigrator.h"

void ov74_02231BF0(GBAMigrator *arg0);

void ov74_02231BF0(GBAMigrator *arg0) {
    GfGfxResType type;
    struct SpriteResourceCollection **v0;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_76);
    arg0->unk_28 = G2dRenderer_Init(0x50, &arg0->unk_2C, HEAP_ID_76);
    G2dRenderer_SetSubSurfaceCoords(&arg0->unk_2C, 0, 0x100000);
    type = GF_GFX_RES_TYPE_CHAR;
    v0 = (struct SpriteResourceCollection **)arg0;
    do {
        *(GF_2DGfxResMan **)((u8 *)v0 + offsetof(GBAMigrator, unk_1B8)) = Create2DGfxResObjMan(3, type, HEAP_ID_76);
        type += 1;
        v0 += 1;
    } while (type < 6);
    arg0->unk_1D0[0] = AddCharResObjFromNarc(((GF_2DGfxResMan *)arg0->unk_1B8[0]), NARC_a_1_1_3, 0x1E, 1, 0, 1, HEAP_ID_76);
    arg0->unk_1D0[1] = AddPlttResObjFromNarc(((GF_2DGfxResMan *)arg0->unk_1B8[1]), NARC_a_1_1_3, 0x1B, 0, 0, 1, 4, HEAP_ID_76);
    arg0->unk_1D0[2] = AddCellOrAnimResObjFromNarc(((GF_2DGfxResMan *)arg0->unk_1B8[2]), NARC_a_1_1_3, 0x1D, 1, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_76);
    arg0->unk_1D0[3] = AddCellOrAnimResObjFromNarc(((GF_2DGfxResMan *)arg0->unk_1B8[3]), NARC_a_1_1_3, 0x1C, 1, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_76);
    SpriteTransfer_CreateCharTransferTask(arg0->unk_1D0[0]);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->unk_1D0[1]);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_8_OFFSET, 0U, HEAP_ID_76);
}
