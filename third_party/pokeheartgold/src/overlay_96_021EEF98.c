#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_96_app_021eefac.h"

void ov96_021EEF98(Ov96App_021EEFAC *arg0);

void ov96_021EEF98(Ov96App_021EEFAC *arg0) {
    FreeBgTilemapBuffer(arg0->bgConfig, 0U);
    Heap_Free(arg0->bgConfig);
}
