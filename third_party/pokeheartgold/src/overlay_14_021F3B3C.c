#include "global.h"

#include "sprite_system.h"
#include "overlay_14_box_funcs.h"

void ov14_021F3B3C(void *arg0);

void ov14_021F3B3C(void *arg0) {
    u32 v0;
    void *v1;

    v1 = arg0;
    v0 = 0;
    do {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)v1 + 0x3F0), 1);
        v0 += 1;
        v1 += 4;
    } while (v0 < 8U);
}
