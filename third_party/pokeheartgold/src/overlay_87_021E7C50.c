#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "filesystem_files_def.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_87_scratch_off_card_app.h"

void ov87_021E7C50(ScratchOffCardsAppSpriteManager *spriteMan, s32 resourceID, s32 resourceID2, NNS_G2D_VRAM_TYPE vRamType);

void ov87_021E7CE0(ScratchOffCardsAppSpriteManager *spriteMan, int unused);
void ov87_021E7D6C(ScratchOffCardsAppSpriteManager *spriteMan, int unused);

void ov87_021E7C50(ScratchOffCardsAppSpriteManager *spriteMan, s32 resourceID, s32 resourceID2, NNS_G2D_VRAM_TYPE vRamType) {
    SpriteResource *pSVar1;

    spriteMan->resources[resourceID][0] = AddCharResObjFromNarc(spriteMan->resourceCollections[0], NARC_a_2_0_5, 0x20, 0, resourceID2, vRamType, HEAP_ID_122);
    pSVar1 = AddPlttResObjFromNarc(spriteMan->resourceCollections[1], NARC_a_2_0_5, 0x21, 0, resourceID2, vRamType, 5, HEAP_ID_122);
    spriteMan->resources[resourceID][1] = pSVar1;
    pSVar1 = AddCellOrAnimResObjFromNarc(spriteMan->resourceCollections[2], NARC_a_2_0_5, 0x1f, 0, resourceID2, GF_GFX_RES_TYPE_CELL, HEAP_ID_122);
    spriteMan->resources[resourceID][2] = pSVar1;
    pSVar1 = AddCellOrAnimResObjFromNarc(spriteMan->resourceCollections[3], NARC_a_2_0_5, 0x1e, 0, resourceID2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_122);
    spriteMan->resources[resourceID][3] = pSVar1;
    return;
}

void ov87_021E7CE0(ScratchOffCardsAppSpriteManager *spriteMan, int unused) {
    spriteMan->resources[3][0] = AddCharResObjFromNarc(spriteMan->resourceCollections[0], NARC_a_2_0_5, 0x10, 0, 3, 1, HEAP_ID_122);
    spriteMan->resources[3][1] = AddPlttResObjFromNarc(spriteMan->resourceCollections[1], NARC_a_2_0_5, 0x11, 0, 3, 1, 4, HEAP_ID_122);
    spriteMan->resources[3][2] = AddCellOrAnimResObjFromNarc(spriteMan->resourceCollections[2], NARC_a_2_0_5, 0xF, 0, 3, GF_GFX_RES_TYPE_CELL, HEAP_ID_122);
    spriteMan->resources[3][3] = AddCellOrAnimResObjFromNarc(spriteMan->resourceCollections[3], NARC_a_2_0_5, 0xE, 0, 3, GF_GFX_RES_TYPE_ANIM, HEAP_ID_122);
}

void ov87_021E7D6C(ScratchOffCardsAppSpriteManager *spriteMan, int unused) {
    spriteMan->resources[4][0] = AddCharResObjFromNarc(spriteMan->resourceCollections[0], NARC_a_2_0_5, 0x14, 0, 4, 1, HEAP_ID_122);
    spriteMan->resources[4][1] = AddPlttResObjFromNarc(spriteMan->resourceCollections[1], NARC_a_2_0_5, 0x15, 0, 4, 1, 1, HEAP_ID_122);
    spriteMan->resources[4][2] = AddCellOrAnimResObjFromNarc(spriteMan->resourceCollections[2], NARC_a_2_0_5, 0x13, 0, 4, GF_GFX_RES_TYPE_CELL, HEAP_ID_122);
    spriteMan->resources[4][3] = AddCellOrAnimResObjFromNarc(spriteMan->resourceCollections[3], NARC_a_2_0_5, 0x12, 0, 4, GF_GFX_RES_TYPE_ANIM, HEAP_ID_122);
}
