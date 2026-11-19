#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "save.h"
#include "save_frontier.h"
#include "text.h"
#include "unk_02030A98.h"
#include "overlay_43_palpad.h"

void ov43_0222ABFC(PalPadGfx *, s32);

void ov43_0222DEF8(PalPadDetailsSlot *, u32);

void ov43_0222EC58(PalPadDetailsSlot *slot, u32 windowIndex, u32 windowOffset, PalPadShared *shared, PalPadGfx *gfx, u32 entryID, u32 xOffset, u32 yOffset, u32 color, String *string, String *fmtString, u32 align);

void ov43_0222E220(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222E220(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    u16 uVar1;
    FrontierSave *pFVar2;
    String *string;
    String *string_00;
    s32 sVar3;

    pFVar2 = Save_Frontier_GetStatic(shared->saveData);
    string = String_New(0x80, heapID);
    string_00 = String_New(0x80, heapID);
    ov43_0222EC58(slot, 3, 0, shared, gfx, 0x1e, 0, 0, 0x10200, string, string_00, 0);
    ov43_0222EC58(slot, 3, 1, shared, gfx, 0x26, 0, 0, 0x10200, string, string_00, 0);
    ov43_0222EC58(slot, 3, 1, shared, gfx, 0x28, 0xd0, 0, 0x10200, string, string_00, 2);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x66, (u32) shared->friends.slots[shared->selectedFriend]);
    if (uVar1 == 0) {
        sVar3 = 0x22;
    } else {
        sVar3 = 0x23;
    }
    ov43_0222EC58(slot, 3, 2, shared, gfx, sVar3, 0, 0, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x73, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 3, 2, shared, gfx, 0x25, 0x70, 0, 0x10200, string, string_00, 1);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x75, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 3, 2, shared, gfx, 0x2b, 0, 0, 0x10200, string, string_00, 0);
    ov43_0222EC58(slot, 3, 2, shared, gfx, 0x24, 0, 0x10, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x72, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 3, 2, shared, gfx, 0x25, 0x70, 0x10, 0x10200, string, string_00, 1);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x74, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 3, 2, shared, gfx, 0x2b, 0, 0x10, 0x10200, string, string_00, 0);
    ov43_0222EC58(slot, 3, 3, shared, gfx, 0x27, 0, 0, 0x10200, string, string_00, 0);
    ov43_0222EC58(slot, 3, 3, shared, gfx, 0x28, 0xd0, 0, 0x10200, string, string_00, 2);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x68, (u32) shared->friends.slots[shared->selectedFriend]);
    if (uVar1 == 0) {
        sVar3 = 0x22;
    } else {
        sVar3 = 0x23;
    }
    ov43_0222EC58(slot, 3, 4, shared, gfx, sVar3, 0, 0, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x77, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 3, 4, shared, gfx, 0x25, 0x70, 0, 0x10200, string, string_00, 1);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x79, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 3, 4, shared, gfx, 0x2b, 0, 0, 0x10200, string, string_00, 0);
    ov43_0222EC58(slot, 3, 4, shared, gfx, 0x24, 0, 0x10, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x76, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 3, 4, shared, gfx, 0x25, 0x70, 0x10, 0x10200, string, string_00, 1);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x78, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 3, 4, shared, gfx, 0x2b, 0, 0x10, 0x10200, string, string_00, 0);
    ov43_0222DEF8(slot, 3);
    String_Delete(string);
    String_Delete(string_00);
    return;
}

void ov43_0222E5B4(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222E5B4(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    u16 uVar1;
    FrontierSave *pFVar2;
    String *string;
    String *string_00;
    s32 sVar3;

    pFVar2 = Save_Frontier_GetStatic(shared->saveData);
    string = String_New(0x80, heapID);
    string_00 = String_New(0x80, heapID);
    ov43_0222EC58(slot, 4, 0, shared, gfx, 0x1f, 0, 0, 0x10200, string, string_00, 0);
    ov43_0222EC58(slot, 4, 0, shared, gfx, 0x29, 0x68, 0x14, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x6c, (u32) shared->friends.slots[shared->selectedFriend]);
    if (uVar1 == 0) {
        sVar3 = 0x22;
    } else {
        sVar3 = 0x23;
    }
    ov43_0222EC58(slot, 4, 0, shared, gfx, sVar3, 0, 0x30, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x87, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 4, 0, shared, gfx, 0x2d, 0x70, 0x30, 0x10200, string, string_00, 1);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x88, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 4, 0, shared, gfx, 0x2c, 0xa8, 0x30, 0x10200, string, string_00, 0);
    ov43_0222EC58(slot, 4, 0, shared, gfx, 0x24, 0, 0x48, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x86, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 4, 0, shared, gfx, 0x2d, 0x70, 0x48, 0x10200, string, string_00, 1);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x8a, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 4, 0, shared, gfx, 0x2c, 0xa8, 0x48, 0x10200, string, string_00, 0);
    ov43_0222DEF8(slot, 4);
    String_Delete(string);
    String_Delete(string_00);
    return;
}

void ov43_0222E78C(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222E78C(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    u16 uVar1;
    FrontierSave *pFVar2;
    String *string;
    String *string_00;
    String *string_01;
    s32 sVar3;

    pFVar2 = Save_Frontier_GetStatic(shared->saveData);
    string = String_New(0x80, heapID);
    string_00 = String_New(0x80, heapID);
    ov43_0222EC58(slot, 5, 0, shared, gfx, 0x20, 0, 0, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x7c, (u32) shared->friends.slots[shared->selectedFriend]);
    string_01 = GetSpeciesName(uVar1, heapID);
    AddTextPrinterParameterizedWithColor(slot->windows[5], FONT_SYSTEM, string_01, 0, 0x18, 0xff, 0x10200, (PrinterCallback_t)0x0);
    String_Delete(string_01);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x6a, (u32) shared->friends.slots[shared->selectedFriend]);
    if (uVar1 == 0) {
        sVar3 = 0x22;
    } else {
        sVar3 = 0x23;
    }
    ov43_0222EC58(slot, 5, 0, shared, gfx, sVar3, 0, 0x30, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x7b, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 5, 0, shared, gfx, 0x25, 0x70, 0x30, 0x10200, string, string_00, 1);
    ov43_0222EC58(slot, 5, 0, shared, gfx, 0x24, 0, 0x48, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x7a, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 5, 0, shared, gfx, 0x25, 0x70, 0x48, 0x10200, string, string_00, 1);
    ov43_0222DEF8(slot, 5);
    String_Delete(string);
    String_Delete(string_00);
    return;
}

void ov43_0222E904(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222E904(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    u16 uVar1;
    FrontierSave *pFVar2;
    String *string;
    String *string_00;
    s32 sVar3;

    pFVar2 = Save_Frontier_GetStatic(shared->saveData);
    string = String_New(0x80, heapID);
    string_00 = String_New(0x80, heapID);
    ov43_0222EC58(slot, 6, 0, shared, gfx, 0x21, 0, 0, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x6e, (u32) shared->friends.slots[shared->selectedFriend]);
    if (uVar1 == 0) {
        sVar3 = 0x22;
    } else {
        sVar3 = 0x23;
    }
    ov43_0222EC58(slot, 6, 0, shared, gfx, sVar3, 0, 0x18, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x8f, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 6, 0, shared, gfx, 0x2e, 0x70, 0x18, 0x10200, string, string_00, 1);
    ov43_0222EC58(slot, 6, 0, shared, gfx, 0x24, 0, 0x30, 0x10200, string, string_00, 0);
    uVar1 = FrontierSave_GetStat(pFVar2, 0x8e, (u32) shared->friends.slots[shared->selectedFriend]);
    ov43_0222ABFC(gfx, uVar1);
    ov43_0222EC58(slot, 6, 0, shared, gfx, 0x2e, 0x70, 0x30, 0x10200, string, string_00, 1);
    ov43_0222DEF8(slot, 6);
    String_Delete(string);
    String_Delete(string_00);
    return;
}
