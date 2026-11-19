#ifndef POKEHEARTGOLD_G2D_RENDERER_H
#define POKEHEARTGOLD_G2D_RENDERER_H

// G2dRenderer: the one definition (tools/hg/g2dinline.py replaced 177 inline anonymous copies whose NNSG2dRenderSurface
// was a u8[0x28] filler plus eight pointers: 416 bytes on the PC instead of 448). Kept free of other includes so any
// header can embed it by value.

#include "global.h"

typedef struct G2dRenderer {
    NNSG2dRendererInstance rendererInstance;
    NNSG2dRenderSurface renderSurface[2];
} G2dRenderer;

#endif // POKEHEARTGOLD_G2D_RENDERER_H
