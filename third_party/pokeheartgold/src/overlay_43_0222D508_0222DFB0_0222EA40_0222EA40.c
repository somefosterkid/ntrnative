#include "global.h"

#include "pm_string.h"
#include "unk_0202C034.h"
#include "unk_02030A98.h"
#include "overlay_43_palpad.h"

void ov43_0222ABFC(PalPadGfx *, s32);

void ov43_0222AC18(PalPadGfx *, u32);

s32 ov43_0222DEF8(PalPadDetailsSlot *, s32);

void ov43_0222EC58(PalPadDetailsSlot *slot, u32 windowIndex, u32 windowOffset, PalPadShared *shared, PalPadGfx *gfx, u32 entryID, u32 xOffset, u32 yOffset, u32 color, String *string, String *fmtString, u32 align);

void ov43_0222EA40(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222EA40(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    WiFiList *wiFiList;
    String *string;
    String *v1;
    WiFiList *a0;

    Save_Frontier_GetStatic(shared->saveData);
    a0 = sub_0202C6F4(shared->saveData);
    wiFiList = a0;
    v1 = String_New(0x80U, heapID);
    string = String_New(0x80U, heapID);
    ov43_0222EC58(slot, 1, 0, shared, gfx, 0x2F, 0, 0, 0x10200, v1, string, 0);
    ov43_0222AC18(gfx, 0);
    ov43_0222EC58(slot, 1, 0, shared, gfx, 0x30, 0, 0x18, 0x10200, v1, string, 0);
    ov43_0222ABFC(gfx, sub_0202C090(a0, (s32)shared->friends.slots[shared->selectedFriend], 0xA));
    ov43_0222EC58(slot, 1, 0, shared, gfx, 0x31, 0x78, 0x18, 0x10200, v1, string, 0);
    ov43_0222AC18(gfx, 1);
    ov43_0222EC58(slot, 1, 0, shared, gfx, 0x30, 0, 0x30, 0x10200, v1, string, 0);
    ov43_0222ABFC(gfx, sub_0202C090(a0, (s32)shared->friends.slots[shared->selectedFriend], 0xB));
    ov43_0222EC58(slot, 1, 0, shared, gfx, 0x31, 0x78, 0x30, 0x10200, v1, string, 0);
    ov43_0222AC18(gfx, 2);
    ov43_0222EC58(slot, 1, 0, shared, gfx, 0x30, 0, 0x48, 0x10200, v1, string, 0);
    ov43_0222ABFC(gfx, sub_0202C090(a0, (s32)shared->friends.slots[shared->selectedFriend], 0xC));
    ov43_0222EC58(slot, 1, 0, shared, gfx, 0x31, 0x78, 0x48, 0x10200, v1, string, 0);
    ov43_0222DEF8(slot, 1);
    String_Delete(v1);
    String_Delete(string);
}
