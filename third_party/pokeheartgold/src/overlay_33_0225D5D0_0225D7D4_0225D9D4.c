#include "global.h"

#include "bg_window.h"
#include "overlay_33.h"
#include "overlay_33_ov33work.h"

extern void *ov33_0225DA50;

void ov33_0225D9D4(void *arg0, u8 palette) {
    void *v0;
    v0 = &ov33_0225DA50;
    v0 = v0 + (6 * (((((Ov33Work *)arg0)->count) - 1) * 2));
    BgTilemapRectChangePalette(*((BgConfig **)arg0), 5U, *((u8 *)(((u8 *)v0) + 8)), (u8)((*((u8 *)(((u8 *)v0) + 9))) + (((*((u16 *)(((u8 *)v0) + 0xA))) + 6) * (*((s32 *)&((Ov33Work *)arg0)->touchedHitbox)))), 0x20U, 6U, (u8)palette);
    ScheduleBgTilemapBufferTransfer(*((BgConfig **)arg0), 5U);
}
