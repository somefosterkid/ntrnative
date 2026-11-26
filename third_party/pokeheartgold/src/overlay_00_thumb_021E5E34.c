#include "global.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

void ov00_021E5E34(void);

void ov00_021E5E34(void) {
    if (*((s32 *)&_0221A680->state) == 0xA) {
        *((s32 *)&_0221A680->state) = 0xB;
        return;
    }
    *((s32 *)&_0221A680->state) = 9;
}
