#ifndef POKEHEARTGOLD_OVERLAY_74_WONDER_CARD_FLIP_ANIM_H
#define POKEHEARTGOLD_OVERLAY_74_WONDER_CARD_FLIP_ANIM_H

#include "overlay_07_bg_scroll_context.h"
#include "sys_task.h"

// Wonder card flip animation (H-scroll offsets per scanline, double-buffered), shared by both ov74 wonder card app copies.
typedef struct WonderCardFlipAnimManager {
    u16 scrollsBuffers[2][192 * 4]; // 0x000
    BufferManager *bufferManager;   // 0xC00
    SysTask *sysTask;               // 0xC04
    fx32 offsetStep;                // 0xC08
    fx32 deltaOffsetStep;           // 0xC0C
    BOOL running;                   // 0xC10
    BOOL animStage;                 // 0xC14
} WonderCardFlipAnimManager;        // size 0xC18

#endif // POKEHEARTGOLD_OVERLAY_74_WONDER_CARD_FLIP_ANIM_H
