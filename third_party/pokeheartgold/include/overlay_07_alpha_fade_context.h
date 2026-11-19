#ifndef POKEHEARTGOLD_OVERLAY_07_ALPHA_FADE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_ALPHA_FADE_CONTEXT_H

// AlphaFadeContext: merged by tools/hg/promote.py from 23 m2c local copies (1 variants, base
// src/overlay_07_022227D8_02222864_02222AC4_02222AC4.c) and 4 accesses; fields at every accessed offset, pads elsewhere.


typedef struct AlphaFadeContext {
    struct { s16 x; s16 y; s32 data[8]; } lerp;
    int done;
} AlphaFadeContext;

// sizeof(AlphaFadeContext) == 0x28

void ov07_02222AC4(AlphaFadeContext *ctx, s16 startEv1, s16 endEv1, s16 startEv2, s16 endEv2, int steps);
int ov07_02222AF4(const AlphaFadeContext *ctx);

#endif // POKEHEARTGOLD_OVERLAY_07_ALPHA_FADE_CONTEXT_H
