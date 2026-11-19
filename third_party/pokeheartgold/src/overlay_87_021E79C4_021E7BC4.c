#include "global.h"

#include "overlay_87_scratch_off_card_app.h"

#include "unk_0200A090.h"

void ov87_021E7BC4(ScratchOffCardsAppSpriteManager *spriteMan, int unused);

void ov87_021E7BC4(ScratchOffCardsAppSpriteManager *spriteMan, int unused) {
    spriteMan->resources[2][0] = AddCharResObjFromNarc(spriteMan->resourceCollections[0], NARC_a_2_0_5, 0xC, 0, 2, 1, HEAP_ID_122);
    spriteMan->resources[2][1] = AddPlttResObjFromNarc(spriteMan->resourceCollections[1], NARC_a_2_0_5, 0xD, 0, 2, 1, 4, HEAP_ID_122);
    spriteMan->resources[2][2] = AddCellOrAnimResObjFromNarc(spriteMan->resourceCollections[2], NARC_a_2_0_5, 0xB, 0, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_122);
    spriteMan->resources[2][3] = AddCellOrAnimResObjFromNarc(spriteMan->resourceCollections[3], NARC_a_2_0_5, 0xA, 0, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_122);
}
