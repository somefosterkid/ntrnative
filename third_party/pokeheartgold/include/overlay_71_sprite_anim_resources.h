#ifndef POKEHEARTGOLD_OVERLAY_71_SPRITE_ANIM_RESOURCES_H
#define POKEHEARTGOLD_OVERLAY_71_SPRITE_ANIM_RESOURCES_H

// SpriteAnimResources: merged by tools/hg/promote.py from 32 m2c local copies (1 variants, base
// src/overlay_71_02246B58_02247990_02247990.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;

typedef struct SpriteAnimResources {
    void *cellBuffer;
    void *animBuffer;
    struct NNSG2dCellDataBank *cellBank;
    struct NNSG2dAnimBankData *animBank;
} SpriteAnimResources;

// sizeof(SpriteAnimResources) == 0x10

#endif // POKEHEARTGOLD_OVERLAY_71_SPRITE_ANIM_RESOURCES_H
