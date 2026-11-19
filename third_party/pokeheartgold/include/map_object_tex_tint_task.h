#ifndef POKEHEARTGOLD_MAP_OBJECT_TEX_TINT_TASK_H
#define POKEHEARTGOLD_MAP_OBJECT_TEX_TINT_TASK_H

#include "global.h"

// Heap block of ov01_0220589C: a map model texture whose palette is tinted and
// transferred to VRAM; ov01_0220596C frees it once the transfers are done.
typedef struct MapObjectTexTintTask {
    NNSG3dResFileHeader *resFile;
    void *texData;
    u16 pltt[16];
} MapObjectTexTintTask;

#endif // POKEHEARTGOLD_MAP_OBJECT_TEX_TINT_TASK_H
