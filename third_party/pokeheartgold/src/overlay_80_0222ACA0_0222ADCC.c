#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_80_0222ACA0.h"
#include "frontier_graphics.h"

void ov80_0222ADCC(FrontierGraphics *graphics, void **sceneData) {
    *sceneData = Heap_Alloc(HEAP_ID_101, 0x20);
}

struct FrontierGraphics;

void ov80_0222ADDC(FrontierGraphics *graphics, void **sceneData);

void ov80_0222ADDC(FrontierGraphics *graphics, void **sceneData) {
    Heap_Free(*sceneData);
}
