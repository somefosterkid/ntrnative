#include "global.h"

#include "constants/heap.h"

#include "overlay_13_thumb_1.h"
#include "unk_020915B0.h"
#include "poke_overlay.h"

void LoadOVY13(void);

void UnloadOVY13(void);

void sub_020915F0(u32 param_1, enum HeapID param_2);

FS_EXTERN_OVERLAY(OVY_38);

void sub_020915F0(u32 param_1, enum HeapID param_2) {
    LoadDwcOverlay();
    LoadOVY13();
    ov13_0221BA00(param_2);
    UnloadOVY13();
    UnloadDwcOverlay();
    OS_ResetSystem(0);
    return;
}

void LoadOVY38(void) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_38), OVY_LOAD_ASYNC);
}
