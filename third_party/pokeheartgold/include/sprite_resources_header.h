#ifndef POKEHEARTGOLD_SPRITE_RESOURCES_HEADER_H
#define POKEHEARTGOLD_SPRITE_RESOURCES_HEADER_H

// SpriteResourcesHeader: the one definition (0x24 on the DS). tools/hg/sprhdr_local.py replaced the 721 m2c local copies and
// the 541 inline anonymous copies. Kept free of other includes so any header can embed it by value.

#include "global.h"

typedef struct SpriteResourcesHeader {
    const NNSG2dImageProxy *imageProxy;
    const NNSG2dCharacterData *charData;
    const NNSG2dImagePaletteProxy *plttProxy;
    NNSG2dCellDataBank *cellData;
    const NNSG2dCellAnimBankData *cellAnim;
    const NNSG2dMultiCellDataBank *multiCellData;
    const NNSG2dMultiCellAnimBankData *multiCellAnim;
    int flag;
    u8 priority;
} SpriteResourcesHeader;

#endif // POKEHEARTGOLD_SPRITE_RESOURCES_HEADER_H
