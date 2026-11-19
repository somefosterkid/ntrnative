#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "sprite_transfer.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_74_main_menu_util.h"


void ov74_02235A74(void *unused);

void ov74_02235728(NarcId param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6);

void ov74_02235728(NarcId param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6) {
    SpriteResource *pSVar1;
    s32 vram;
    u32 uStack_1c;
    MainMenuUtilManager *utilMan = &ov74_0223D454;

    if (param_6 == 0) {
        vram = 1;
    } else {
        vram = 2;
    }
    uStack_1c = 1;
    if (param_1 == NARC_itemtool_itemdata_item_icon) {
        uStack_1c = 0;
    }
    if (param_2 != -1) {
        pSVar1 = AddCharResObjFromNarc(utilMan->spriteManager.resourceCollections[0], param_1, param_2, uStack_1c, param_6, vram, utilMan->heapID);
        utilMan->spriteManager.resources[param_6][0] = pSVar1;
    }
    if (param_3 != -1) {
        pSVar1 = AddPlttResObjFromNarc(utilMan->spriteManager.resourceCollections[1], param_1, param_3, 0, param_6, vram, 3, utilMan->heapID);
        utilMan->spriteManager.resources[param_6][1] = pSVar1;
    }
    if (param_4 != -1) {
        pSVar1 = AddCellOrAnimResObjFromNarc(utilMan->spriteManager.resourceCollections[2], param_1, param_4, uStack_1c, param_6, GF_GFX_RES_TYPE_CELL, utilMan->heapID);
        utilMan->spriteManager.resources[param_6][2] = pSVar1;
    }
    if (param_5 != -1) {
        pSVar1 = AddCellOrAnimResObjFromNarc(utilMan->spriteManager.resourceCollections[3], param_1, param_5, uStack_1c, param_6, GF_GFX_RES_TYPE_ANIM, utilMan->heapID);
        utilMan->spriteManager.resources[param_6][3] = pSVar1;
    }
    SpriteTransfer_CreateCharTransferTask(utilMan->spriteManager.resources[param_6][0]);
    SpriteTransfer_CreatePlttTransferTask(utilMan->spriteManager.resources[param_6][1]);
    CreateSpriteResourcesHeader(&utilMan->spriteManager.resourceHeaders[param_6], param_6, param_6, param_6, param_6, -1, -1, 0, 0, utilMan->spriteManager.resourceCollections[0], utilMan->spriteManager.resourceCollections[1], utilMan->spriteManager.resourceCollections[2], utilMan->spriteManager.resourceCollections[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    if (param_6 == 0) {
        GfGfx_EngineATogglePlanes(0x10, 1);
    } else {
        GfGfx_EngineBTogglePlanes(0x10, 1);
    }
    Main_SetVBlankIntrCB(ov74_02235A74, (void *)0x0);
    return;
}
