#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

#include "overlay_45_thumb.h"

void ov45_02230390(void);

void ov45_02230390(void) {
}

void ov45_02230394(s32 arg0, void *arg1);

void ov45_02230394(s32 arg0, void *arg1) {
    if (*(u8 *)((u8 *)arg1 + 0x69E) == 1) {
        ov45_0222F154();
        return;
    }
    if (arg0 == 0) {
        _022577C0->unk_69F = 1;
    }
}
