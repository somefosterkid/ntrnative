#ifndef POKEHEARTGOLD_OVERLAY_96_APP_021EEFAC_H
#define POKEHEARTGOLD_OVERLAY_96_APP_021EEFAC_H

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_96_text_box.h"

// hk389: the 0x10-byte work object of the ov96 app run by ov96_021EEFAC (PokeathlonCourse_AllocPtr4FromHeap(data, 0x10),
// asm): show one message (ov96_021EEE0C), then fade out (ov96_021EF094).
typedef struct Ov96App_021EEFAC {
    enum HeapID heapId;   // 0x0 HEAP_ID_159
    BgConfig *bgConfig;   // 0x4
    Ov96TextBox *textBox; // 0x8
    s32 frames;           // 0xC
} Ov96App_021EEFAC; // 0x10

#endif // POKEHEARTGOLD_OVERLAY_96_APP_021EEFAC_H
