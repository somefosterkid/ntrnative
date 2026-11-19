#include "global.h"

#include "gf_gfx_loader.h"
#include "pokemon_icon_idx.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_020932E0.h"

void sub_02093A50(UnkStruct_020932E0 *arg0) {
    GfGfxResType type;
    s32 i;

    arg0->unk_18 = G2dRenderer_Init(0x2B, &arg0->unk_1C, arg0->unk_4);
    G2dRenderer_SetSubSurfaceCoords(&arg0->unk_1C, 0, 0x100000);
    type = GF_GFX_RES_TYPE_CHAR;
    i = 0;
    do {
        arg0->unk_144[i] = Create2DGfxResObjMan(3, type, arg0->unk_4);
        i++;
        type += 1;
    } while (type < 6);
    arg0->unk_15C = AddCharResObjFromNarc(arg0->unk_144[0], NARC_a_1_5_1, 8, 1, 0, 1, arg0->unk_4);
    arg0->unk_160 = AddPlttResObjFromNarc(arg0->unk_144[1], NARC_a_1_5_1, 5, 0, 0, 1, 4, arg0->unk_4);
    arg0->unk_164 = AddCellOrAnimResObjFromNarc(arg0->unk_144[2], NARC_a_1_5_1, 7, 1, 0, GF_GFX_RES_TYPE_CELL, arg0->unk_4);
    arg0->unk_168 = AddCellOrAnimResObjFromNarc(arg0->unk_144[3], NARC_a_1_5_1, 6, 1, 0, GF_GFX_RES_TYPE_ANIM, arg0->unk_4);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(arg0->unk_15C);
    SpriteTransfer_CreateExtPlttTransferTask(arg0->unk_160);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_8_OFFSET, 0U, arg0->unk_4);
}

void sub_02093B40(UnkStruct_020932E0 *arg0) {
    CreateSpriteResourcesHeader(&arg0->unk_174, 0, 0, 0, 0, -1, -1, 0, 0, arg0->unk_144[0], arg0->unk_144[1], arg0->unk_144[2], arg0->unk_144[3], NULL, NULL);
}
