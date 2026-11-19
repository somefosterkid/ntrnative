#include "global.h"

#include "overlay_01_021EFB38.h"
#include "overlay_120.h"

u32 ov120_0225F08C(Ov120Tile *tile) {
    u32 uVar3;

    if (tile->active == 0) {
        return 1;
    }
    uVar3 = ov01_021EFE44(&tile->width) | ov01_021EFE44(&tile->height);
    ov01_021F0960(tile->window,
        ((s32)tile->y - tile->height.currentValue / 2) * 0x10000 >> 0x10,
        ((s32)tile->y + tile->height.currentValue / 2) * 0x10000 >> 0x10,
        (tile->x - tile->width.currentValue / 2) * 0x10000 >> 0x10,
        (tile->x + tile->width.currentValue / 2) * 0x10000 >> 0x10,
        tile->color);
    return uVar3;
}
