#include "global.h"
#include "bg_window.h"
#include "pm_string.h"
#include "overlay_43_palpad.h"

s32 ov43_0222DEF8(PalPadDetailsSlot *, s32);

void ov43_0222EC58(PalPadDetailsSlot *slot, u32 windowIndex, u32 windowOffset, PalPadShared *shared, PalPadGfx *gfx, u32 entryID, u32 xOffset, u32 yOffset, u32 color, String *string, String *fmtString, u32 align);

void ov43_0222EBD8(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx);

void ov43_0222EC14(PalPadGfx *gfx, PalPadDetailsSlot *slot);

void ov43_0222EBD8(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx) {
    ov43_0222EC58(slot, 7, 0, shared, gfx, 0xE, 0, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    ov43_0222DEF8(slot, 7);
}

void ov43_0222EC14(PalPadGfx *gfx, PalPadDetailsSlot *slot) {
    s32 iVar1;
    s32 iVar2;

    iVar1 = (s32)slot->layout.y;
    if (iVar1 < 0) {
        iVar1 = -iVar1;
    }
    iVar2 = (s32)slot->layout.x;
    if (iVar2 < 0) {
        iVar2 = -iVar2;
    }
    FillBgTilemapRect(gfx->bgConfig, 3, 0, (u8)iVar2, (u8)iVar1, 0x20, 0x18, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 3);
}
