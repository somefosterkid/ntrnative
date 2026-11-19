#include "global.h"

#include "constants/heap.h"

#include "heap.h"

extern enum HeapID ov74_0223D450;

void ov74_02235138(u32 arg0);

void ov74_02235138(u32 size) {
    Heap_Alloc(ov74_0223D450, size);
}
