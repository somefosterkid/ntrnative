#include "global.h"

#include "bg_window.h"
#include "constants/heap.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "string_util.h"
#include "unk_02013FDC.h"
#include "unk_0202C034.h"
#include "unk_0205B3DC.h"
#include "overlay_43_palpad.h"

void ov43_0222AA70(PalPadGfx *);

void ov43_0222DF90(PalPadDetailsSlot *, PalPadGfx *);

void ov43_0222D87C(PalPadDetails *details, PalPadGfx *gfx);

void ov43_0222AB20(PalPadGfx *, SaveData *, s32, enum HeapID);
void ov43_0222AB5C(PalPadGfx *, SaveData *, s32, enum HeapID);
s32 ov43_0222AB94(PalPadGfx *, SaveData *, s32);
u8 ov43_0222ECD4(Window *window, PalPadShared *unused, PalPadGfx *gfx, s32 entryID, u32 x, u32 y, u32 color);
void ov43_0222D8B8(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222DACC(PalPadDetails *details);

void ov43_0222D87C(PalPadDetails *details, PalPadGfx *gfx) {
    s32 iVar1;
    Window *window;
    PalPadDetailsSlot *slot;
    iVar1 = 0;
    slot = details->slots;
    do {
        ov43_0222DF90(slot, gfx);
        iVar1 = iVar1 + 1;
        slot = slot + 1;
    } while (iVar1 < 3);
    iVar1 = 0;
    window = details->profileWindows;
    do {
        ClearWindowTilemapAndScheduleTransfer(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 <= (6 - 1));
    ov43_0222AA70(gfx);
}

void ov43_0222D8B8(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    WiFiList *a0;
    s32 playerGender;
    s32 iVar1;
    s32 trclass;
    void *dest;
    Window *pWVar2;
    u32 uVar3;
    u32 uVar4;
    UnkStruct_02070D3C UStack_30;

    a0 = sub_0202C6F4(shared->saveData);
    playerGender = sub_0202C090(a0, (u32)shared->friends.slots[shared->selectedFriend], 8);
    uVar4 = 0;
    pWVar2 = details->profileWindows;
    do {
        FillWindowPixelBuffer(pWVar2, 0);
        uVar4 = uVar4 + 1;
        pWVar2 = pWVar2 + 1;
    } while (uVar4 < 8);
    BufferIntegerAsString(gfx->msgFormat, 0, shared->friends.numbers[shared->selectedFriend] + 1, 2, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(gfx->msgFormat, 1, shared->friends.numberedCount, 2, PRINTING_MODE_LEFT_ALIGN, 1);
    ov43_0222ECD4(&details->profileWindows[0], shared, gfx, 0x12, 0, 0, 0xf0200);
    if (playerGender == 0) {
        uVar3 = 0x50600;
    } else if (playerGender == 1) {
        uVar3 = 0x30400;
    } else {
        uVar3 = 0x10200;
    }
    ov43_0222AB20(gfx, shared->saveData, shared->friends.slots[shared->selectedFriend], heapID);
    ov43_0222ECD4(&details->profileWindows[2], shared, gfx, 0x11, 0, 0, uVar3);
    ov43_0222ECD4(&details->profileWindows[6], shared, gfx, 0xf, 0, 0, 0xf0200);
    ov43_0222AB5C(gfx, shared->saveData, shared->friends.slots[shared->selectedFriend], heapID);
    ov43_0222ECD4(&details->profileWindows[3], shared, gfx, 0x10, 0, 0, 0x10200);
    ov43_0222ECD4(&details->profileWindows[7], shared, gfx, 0x1b, 0, 0, 0xf0200);
    iVar1 = ov43_0222AB94(gfx, shared->saveData, shared->friends.slots[shared->selectedFriend]);
    if (iVar1 != 0) {
        ov43_0222ECD4(&details->profileWindows[4], shared, gfx, 0x1c, 0, 0, 0x10200);
    }
    trclass = sub_0202C090(a0, (u32)shared->friends.slots[shared->selectedFriend], 7);
    iVar1 = GetUnionRoomAvatarAttrBySprite(playerGender, trclass, 1);
    sub_02070D84(iVar1, 2, &UStack_30);
    dest = Heap_Alloc(heapID, 0xc80);
    sub_020141C4(UStack_30.narcId, UStack_30.ncbr_id, heapID, 0, 0, 10, 10, dest);
    BlitBitmapRectToWindow(&details->profileWindows[5], dest, 0, 0, 0x50, 0x50, 0, 0, 0x50, 0x50);
    Heap_Free(dest);
    GfGfxLoader_GXLoadPal(UStack_30.narcId, UStack_30.nclr_id, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, 0x20, heapID);
    BgTilemapRectChangePalette(gfx->bgConfig, 4, 4, 4, 0xa, 0xa, 0xf);
    uVar4 = 0;
    pWVar2 = details->profileWindows;
    do {
        ScheduleWindowCopyToVram(pWVar2);
        uVar4 = uVar4 + 1;
        pWVar2 = pWVar2 + 1;
    } while (uVar4 < 8);
}

void ov43_0222DACC(PalPadDetails *details) {
    s32 iVar1;
    Window *window;

    iVar1 = 0;
    window = details->profileWindows;
    do {
        ClearWindowTilemapAndScheduleTransfer(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 8);
}
