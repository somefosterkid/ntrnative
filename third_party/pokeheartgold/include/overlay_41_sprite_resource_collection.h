#ifndef POKEHEARTGOLD_OVERLAY_41_SPRITE_RESOURCE_COLLECTION_H
#define POKEHEARTGOLD_OVERLAY_41_SPRITE_RESOURCE_COLLECTION_H

// SpriteResourceCollection: merged by tools/hg/promote.py from 6 m2c local copies (1 variants, base
// src/overlay_41_02246280_02246280.c) and 3 accesses; fields at every accessed offset, pads elsewhere.

struct ResourceCollection;
struct SpriteResource;

typedef struct SpriteResourceCollection {
    struct ResourceCollection *collection;
    struct GF_2DGfxResMan *resources;
    int capacity;
    int count;
    u32 type;
} SpriteResourceCollection;

// sizeof(SpriteResourceCollection) == 0x14

#endif // POKEHEARTGOLD_OVERLAY_41_SPRITE_RESOURCE_COLLECTION_H
