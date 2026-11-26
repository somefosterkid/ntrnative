#include "global.h"

#include "overlay_00_thumb.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

void ov00_021E7130(void) {
    *((s8 *)&_0221A680->matchmakingCancelState) = 0;
}
