#ifndef POKEHEARTGOLD_OVERLAY_07_PALETTE_FADE_H
#define POKEHEARTGOLD_OVERLAY_07_PALETTE_FADE_H

#include "palette.h"
#include "sys_task.h"

// The overlay 7 palette fade task context. sizeof == 0x1C on the DS: the task at 4, the PaletteData * at 0x18 (the
// pointer makes it larger on a 64-bit host, so allocate it with sizeof).
typedef struct PaletteFadeContext {
    BOOL active;
    SysTask *task;
    u32 bufferID;
    u16 index;
    u16 count;
    u16 color;
    u8 startFrac;
    u8 endFrac;
    u8 fraction;
    s8 step;
    s8 stepFrames;
    s8 timer;
    PaletteData *paletteData;
} PaletteFadeContext;

#endif // POKEHEARTGOLD_OVERLAY_07_PALETTE_FADE_H
