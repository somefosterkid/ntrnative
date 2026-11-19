#include "global.h"
#include "unk_0202C034.h"
#include "overlay_43_palpad.h"

s32 ov43_0222DF1C(PalPadDetailsSlot *, s32);

void ov43_0222EC14(PalPadGfx *, PalPadDetailsSlot *);

void ov43_0222DF90(PalPadDetailsSlot *arg0, PalPadGfx *arg1);

void ov43_0222ABFC(PalPadGfx *, s32);
s32 ov43_0222DEF8(PalPadDetailsSlot *, s32);
void ov43_0222EC58(PalPadDetailsSlot *slot, u32 windowIndex, u32 windowOffset, PalPadShared *shared, PalPadGfx *gfx, u32 entryID, u32 xOffset, u32 yOffset, u32 color, String *string, String *fmtString, u32 align);
void ov43_0222DFB0(PalPadDetailsSlot *arg0, PalPadShared *arg1, PalPadGfx *arg2);

void ov43_0222DF90(PalPadDetailsSlot *arg0, PalPadGfx *arg1) {
    s32 v0;

    v0 = 0;
    do {
        ov43_0222DF1C(arg0, v0);
        v0 += 1;
    } while (v0 < 8);
    ov43_0222EC14(arg1, arg0);
}

void ov43_0222DFB0(PalPadDetailsSlot *arg0, PalPadShared *shared, PalPadGfx *gfx) {
    WiFiList *a0;
    s32 v1;
    PalPadDetailsSlot *v0;
    u32 v3;
    u8 v2;

    v0 = arg0;
    a0 = sub_0202C6F4(shared->saveData);
    ov43_0222EC58(arg0, 0, 0, shared, gfx, 0x13, 0, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    v3 = 0;
    v1 = 0;
    do {
        v2 = (u8)v1;
        ov43_0222EC58(v0, 0, 2, shared, gfx, v3 + 0x14, 0, (s32)v2, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
        ov43_0222ABFC(gfx, sub_0202C090(a0, (s32)shared->friends.slots[shared->selectedFriend], v3 + 1));
        ov43_0222EC58(v0, 0, 2, shared, gfx, 0x16, 0, (s32)v2, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
        v1 += 0x18;
        v3 += 1;
    } while (v3 < 2U);
    ov43_0222EC58(v0, 0, 1, shared, gfx, 0x17, 0, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    ov43_0222ABFC(gfx, sub_0202C090(a0, (s32)shared->friends.slots[shared->selectedFriend], 3));
    ov43_0222EC58(v0, 0, 3, shared, gfx, 0x18, 0, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    ov43_0222DEF8(v0, 0);
}
