#ifndef POKEHEARTGOLD_OVERLAY_71_PALETTE_SHIMMER_EFFECT_H
#define POKEHEARTGOLD_OVERLAY_71_PALETTE_SHIMMER_EFFECT_H

// PaletteShimmerEffect: merged by tools/hg/promote.py from 7 m2c local copies (1 variants, base
// src/overlay_71_02246B58_02246C6C_02246C6C.c) and 4 accesses; fields at every accessed offset, pads elsewhere.

struct SysTask;

typedef struct PaletteShimmerEffect {
    u16 paletteBuffer[96];
    struct SysTask *vblankTask;
    struct SysTask *sparkTasks[8];
    BOOL *isAlive;
    BOOL stopping;
    int dirty;
    int spawnTimer;
    int activeSparkCount;
    int nextSparkSlot;
    int direction;
} PaletteShimmerEffect;

// sizeof(PaletteShimmerEffect) == 0x100

#endif // POKEHEARTGOLD_OVERLAY_71_PALETTE_SHIMMER_EFFECT_H
