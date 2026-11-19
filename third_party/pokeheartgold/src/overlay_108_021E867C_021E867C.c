#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "overlay_108_ov108sprite_manager.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

s32 ov108_021E8758(Ov108SpriteManager *);

extern const u8 ov108_021EA978[4];

typedef struct Ov108SpriteCounts {
    u8 counts[4];
} Ov108SpriteCounts;

void ov108_021E867C(Ov108SpriteManager *spriteMan);

void ov108_021E867C(Ov108SpriteManager *spriteMan) {
    Ov108SpriteCounts counts = *(const Ov108SpriteCounts *)ov108_021EA978;
    u32 i, j;

    spriteMan->spriteList = G2dRenderer_Init(spriteMan->maxSprites, &spriteMan->renderer, spriteMan->heapID);
    for (i = 0; i < 4; i++) {
        spriteMan->resMan[i] = Create2DGfxResObjMan(counts.counts[i], (GfGfxResType)i, spriteMan->heapID);
        spriteMan->resList[i] = Create2DGfxResObjList(counts.counts[i], spriteMan->heapID);
        for (j = 0; j < spriteMan->resList[i]->max; j++) {
            spriteMan->resList[i]->obj[j] = NULL;
        }
    }
    ov108_021E8758(spriteMan);
}
