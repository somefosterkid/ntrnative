#include "global.h"

#include "poke_overlay.h"

FS_EXTERN_OVERLAY(OVY_13);

void LoadOVY13(void);

void LoadOVY13(void) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_13), OVY_LOAD_ASYNC);
}
