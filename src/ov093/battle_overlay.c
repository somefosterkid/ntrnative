// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/battle_overlay.c. The overlay is 140 in
// Black.
#include "types.h"
#include "gfl/overlay.h"
#include "nitro/os.h"

extern u32 OVERLAY_140_ID[];

// Whether the overlay was loaded
static BOOL sOverlayLoaded;

// The higher of the development console's bits of the console type (OS_CONSOLE_DEV_MASK)
#define CONSOLE_TYPE_DEV_HIGH_BIT 0x2000000

// Loads the overlay, only on a development console
void func_ov093_021e88cc(void) {
    if (OS_GetConsoleType() & CONSOLE_TYPE_DEV_HIGH_BIT) {
        GFL_OvlLoad(OVERLAY_ID(140));
        sOverlayLoaded = TRUE;
    } else {
        sOverlayLoaded = FALSE;
    }
}

void func_ov093_021e88f8(void) {
    if (sOverlayLoaded) {
        GFL_OvlUnload(OVERLAY_ID(140));
        sOverlayLoaded = FALSE;
    }
}
