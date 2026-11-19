#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_80_022384D8.h"

WFCFacilitySelectorEffects *ov80_022384D8(BgConfig *bgConfig) {
    WFCFacilitySelectorEffects *effects;

    effects = Heap_Alloc(HEAP_ID_101, sizeof(WFCFacilitySelectorEffects));
    MI_CpuFill8(effects, 0U, sizeof(WFCFacilitySelectorEffects));
    effects->flicker = ov80_022384FC(bgConfig);
    return effects;
}
