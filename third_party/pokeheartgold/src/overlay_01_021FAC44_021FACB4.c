#include "global.h"

#include "heap.h"
#include "overlay_01_021FAC44.h"
#include "overlay_01_map_load_manager.h"

void ov01_021FACB4(MapLoadBuffers *buffers) {
    u8 i;

    i = 0;
    do {
        buffers->primary[i] = NULL;
        if (buffers->secondary[i] != NULL) {
            Heap_Free(buffers->secondary[i]);
            buffers->secondary[i] = NULL;
        }
        i += 1;
    } while ((u32)i < 4U);
    Heap_Free(buffers);
}

void ov01_021FACE4(s32 index, MapLoadBuffers *buffers, void **out) {
    *out = buffers->primary[index];
}

void ov01_021FACEC(s32 index, MapLoadBuffers *buffers, void **out) {
    *out = buffers->secondary[index];
}
