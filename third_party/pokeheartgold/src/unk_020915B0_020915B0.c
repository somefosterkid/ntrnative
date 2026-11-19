#include "global.h"

#include "poke_overlay.h"
#include "unk_020915B0.h"

FS_EXTERN_OVERLAY(OVY_0);

void LoadDwcOverlay(void) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_0), OVY_LOAD_ASYNC);
}
