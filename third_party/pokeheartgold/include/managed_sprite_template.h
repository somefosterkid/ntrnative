#ifndef POKEHEARTGOLD_MANAGED_SPRITE_TEMPLATE_H
#define POKEHEARTGOLD_MANAGED_SPRITE_TEMPLATE_H

#include "global.h"

#include <nnsys/g2d/g2d_Image.h>

#include "struct_defs/resdat_narc_structs.h"

typedef struct ManagedSpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    int drawPriority;
    int pal;
    NNS_G2D_VRAM_TYPE vram;
    int resIdList[GF_GFX_RES_TYPE_MAX];
    int bgPriority;
    int vramTransfer;
} ManagedSpriteTemplate; // size=0x34

#endif // POKEHEARTGOLD_MANAGED_SPRITE_TEMPLATE_H
