// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/mystery_data.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/mystery_gift.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "save/save_control.h"
#include "save/save_outside.h"
#include "system/game_data.h"

// The mystery gift album: the cards received and the flag of each gift ID, encrypted in the save's block 0x22, or
// in the data outside the save before there is one. The file's name is the ROM's; the function names from swan
// are mysteryGiftBlock, mgEncryptData and mgDecryptData, and the statics are ours.

#define OVERLAY_SAVE_OUTSIDE OVERLAY_ID(228)

static BOOL func_02009f50(MysteryGiftSave *save, u32 slot);
static void func_02009f7c(MysteryGiftSave *save, u32 slot);
static MysteryGift *func_02009fd4(MysteryGiftSave *save, u32 slot);
static BOOL func_02009f1c(MysteryGiftSave *save);
static u32 func_0200a3fc(MysteryGiftSave *save);
static u32 func_0200a400(MysteryGiftSave *save);
static void *func_0200a418(MysteryGiftSave *save);
static u8 *func_0200a41c(MysteryGiftSave *save);
static MysteryGift *func_0200a43c(MysteryGiftSave *save, u32 slot);
static void func_0200a480(MysteryGiftSave *save);
static void func_0200a49c(MysteryGiftSave *save);

u32 func_02009ed4(void) {
    return sizeof(MysteryGiftSaveData);
}

void func_02009edc(MysteryGiftSaveData *data) {
    MysteryGiftSave save;

    MI_CpuFill8(&save, 0, sizeof(MysteryGiftSave));
    save.kind = MG_DATA_SAVE;
    save.data = data;
    MI_CpuFill8(data, 0, sizeof(MysteryGiftSaveData));
    func_0200a480(&save);
}

BOOL func_02009f0c(u32 kind) {
    if (kind != 0 && kind < 4) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's hasFreeSlot
static BOOL func_02009f1c(MysteryGiftSave *save) {
    u32 i;
    u32 max = func_0200a400(save);

    for (i = 0; i < max; i++) {
        if (!func_02009f0c(func_0200a43c(save, i)->kind)) {
            return TRUE;
        }
    }
    return FALSE;
}

// The count fetched and dropped here and below is what the code does, likely an assert compiled out
// pokebw2's isSlotUsed
static BOOL func_02009f50(MysteryGiftSave *save, u32 slot) {
    MysteryGift *card = func_0200a43c(save, slot);

    func_0200a400(save);
    // BUG: the bound is the save's twelve cards whatever the kind; the gifts outside the save have three
    if (slot < MG_CARD_MAX && func_02009f0c(card->kind)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's removeCard
static void func_02009f7c(MysteryGiftSave *save, u32 slot) {
    u32 max = func_0200a400(save);

    if (func_02009f50(save, slot)) {
        return;
    }
    for (; slot < max - 1; slot++) {
        *func_0200a43c(save, slot) = *func_0200a43c(save, slot + 1);
    }
    MI_CpuFill8(func_0200a43c(save, slot), 0, sizeof(MysteryGift));
}

// pokebw2's getUsedCard
static MysteryGift *func_02009fd4(MysteryGiftSave *save, u32 slot) {
    MysteryGift *card = func_0200a43c(save, slot);

    if (slot < func_0200a400(save) && func_02009f0c(card->kind)) {
        return card;
    }
    return NULL;
}

BOOL func_0200a000(MysteryGiftSave *save, u32 slot, MysteryGift *gift) {
    BOOL found = FALSE;
    MysteryGift *card;

    func_0200a400(save);
    func_0200a49c(save);
    card = func_02009fd4(save, slot);
    if (card != NULL) {
        MI_CpuCopy8(card, gift, sizeof(MysteryGift));
        found = TRUE;
    }
    func_0200a480(save);
    return found;
}

BOOL func_0200a034(MysteryGiftSave *save, const MysteryGift *gift) {
    BOOL saved = FALSE;
    u32 max = func_0200a400(save);
    u32 i;

    func_0200a49c(save);
    if (func_02009f1c(save) == TRUE) {
        for (i = 0; i < max; i++) {
            MysteryGift *card = func_0200a43c(save, i);
            if (!func_02009f0c(card->kind)) {
                MI_CpuCopy8(gift, card, sizeof(MysteryGift));
                saved = TRUE;
                break;
            }
        }
    }
    func_0200a480(save);
    return saved;
}

void func_0200a094(MysteryGiftSave *save, u32 slot) {
    u32 max = func_0200a400(save);

    func_0200a49c(save);
    if (slot < max) {
        func_0200a43c(save, slot)->kind = 0;
        func_02009f7c(save, slot);
    }
    func_0200a480(save);
}

BOOL func_0200a0c8(MysteryGiftSave *save) {
    BOOL free;

    func_0200a49c(save);
    free = func_02009f1c(save);
    func_0200a480(save);
    return free;
}

BOOL func_0200a0e4(MysteryGiftSave *save, u32 slot) {
    BOOL used;

    func_0200a49c(save);
    used = func_02009f50(save, slot);
    func_0200a480(save);
    return used;
}

BOOL func_0200a104(MysteryGiftSave *save, u32 slot) {
    BOOL delivered = FALSE;
    u32 max = func_0200a400(save);

    func_0200a49c(save);
    if (slot < max && func_0200a43c(save, slot)->delivered) {
        delivered = TRUE;
    }
    func_0200a480(save);
    return delivered;
}

void func_0200a13c(MysteryGiftSave *save, u32 slot) {
    u32 max = func_0200a400(save);

    func_0200a49c(save);
    if (slot < max) {
        func_0200a43c(save, slot)->delivered = 1;
    }
    func_0200a480(save);
}

BOOL func_0200a170(MysteryGiftSave *save) {
    u32 i;
    BOOL any = FALSE;
    u32 max = func_0200a400(save);

    func_0200a49c(save);
    for (i = 0; i < max; i++) {
        if (func_02009f50(save, i) == TRUE) {
            any = TRUE;
        }
    }
    func_0200a480(save);
    return any;
}

BOOL func_0200a1a8(MysteryGiftSave *save, u32 id) {
    BOOL received = FALSE;
    u8 *flags = func_0200a41c(save);

    func_0200a49c(save);
    if (id < MG_ID_MAX && ((1 << (id & 7)) & flags[id >> 3])) {
        received = TRUE;
    }
    func_0200a480(save);
    return received;
}

void func_0200a1e4(MysteryGiftSave *save, u32 id) {
    u8 *flags = func_0200a41c(save);

    func_0200a49c(save);
    if (id < MG_ID_MAX) {
        flags[id >> 3] |= (u8)(1 << (id & 7));
    }
    func_0200a480(save);
}

BOOL func_0200a21c(const MysteryGiftRecvData *data) {
    s32 i;
    s32 count = 0;

    // Counts the bytes of the data 16 at a time, to no end
    do {
        for (i = 0; i < 16; i++) {
            count++;
        }
    } while (count < sizeof(MysteryGiftRecvData));
    if (data->crc == getCRC16(data, sizeof(MysteryGiftRecvData) - sizeof(data->crc))) {
        return TRUE;
    }
    return FALSE;
}

void func_0200a254(MysteryGiftSave *save, u32 slot1, u32 slot2) {
    MysteryGift temp;

    func_0200a49c(save);
    if (func_02009f50(save, slot1) && func_02009f50(save, slot2)) {
        MysteryGift *card1 = func_02009fd4(save, slot1);
        MysteryGift *card2 = func_02009fd4(save, slot2);
        MI_CpuCopy8(card1, &temp, sizeof(MysteryGift));
        MI_CpuCopy8(card2, card1, sizeof(MysteryGift));
        MI_CpuCopy8(&temp, card2, sizeof(MysteryGift));
    }
    func_0200a480(save);
}

void func_0200a2b8(MysteryGiftSave *save, GameData *gameData) {
    if (func_0200a350(save) == MG_DATA_OUTSIDE) {
        func_ov228_021dfdc0(save->outside);
    } else {
        func_02012dac(gameData);
    }
}

u32 func_0200a2d8(MysteryGiftSave *save, GameData *gameData) {
    if (func_0200a350(save) == MG_DATA_OUTSIDE) {
        if (func_ov228_021dfe00(save->outside)) {
            return 2;
        }
        return 0;
    }
    return func_02012dd0(gameData);
}

MysteryGiftSave *mysteryGiftBlock(SaveControl *save, u32 mode, HeapID heapId) {
    MysteryGiftSave *giftSave = GFL_HeapAllocate(heapId, sizeof(MysteryGiftSave), FALSE, "mystery_data.c", 608);
    MI_CpuFill8(giftSave, 0, sizeof(MysteryGiftSave));
    func_0200a358(giftSave, save, mode, heapId);
    return giftSave;
}

void func_0200a338(MysteryGiftSave *save) {
    func_0200a3d0(save);
    GFL_HeapFree(save);
}

u32 func_0200a348(MysteryGiftSave *save) {
    return func_0200a400(save);
}

u32 func_0200a350(MysteryGiftSave *save) {
    return func_0200a3fc(save);
}

// pokebw2's func_0200aa74
void func_0200a358(MysteryGiftSave *giftSave, SaveControl *save, u32 mode, HeapID heapId) {
    BOOL useSave;

    MI_CpuFill8(giftSave, 0, sizeof(MysteryGiftSave));
    switch (mode) {
    case 0:
        useSave = TRUE;
        break;
    case 1:
        useSave = SaveControl_IsDataAlreadyPresent(save);
        break;
    default:
        useSave = TRUE;
        break;
    }
    if (useSave) {
        giftSave->data = SaveControl_GetBlockPtr(save, SAVE_BLOCK_MYSTERY_GIFT);
        giftSave->kind = MG_DATA_SAVE;
    } else {
        GFL_OvlLoad(OVERLAY_SAVE_OUTSIDE);
        giftSave->outside = func_ov228_021dfc20(heapId);
        giftSave->data = func_ov228_021dfeac(giftSave->outside);
        giftSave->kind = MG_DATA_OUTSIDE;
        if (!func_ov228_021dfe90(giftSave->outside)) {
            MI_CpuFill8(giftSave->data, 0, sizeof(SaveOutsideGifts));
            func_0200a480(giftSave);
        }
    }
}

// pokebw2's func_0200aaec
void func_0200a3d0(MysteryGiftSave *save) {
    switch (save->kind) {
    case MG_DATA_OUTSIDE:
        func_ov228_021dfdb8(save->outside);
        GFL_OvlUnload(OVERLAY_SAVE_OUTSIDE);
        break;
    case MG_DATA_SAVE:
        break;
    }
    MI_CpuFill8(save, 0, sizeof(MysteryGiftSave));
}

// pokebw2's getKind
static u32 func_0200a3fc(MysteryGiftSave *save) {
    return save->kind;
}

// pokebw2's getCardMax
static u32 func_0200a400(MysteryGiftSave *save) {
    switch (save->kind) {
    case MG_DATA_OUTSIDE:
        return MG_CARD_MAX_OUTSIDE;
    case MG_DATA_SAVE:
        return MG_CARD_MAX;
    }
    return 0;
}

// pokebw2's getData
static void *func_0200a418(MysteryGiftSave *save) {
    return save->data;
}

// pokebw2's getReceivedFlags
static u8 *func_0200a41c(MysteryGiftSave *save) {
    switch (save->kind) {
    case MG_DATA_OUTSIDE:
        return ((SaveOutsideGifts *)func_0200a418(save))->receivedFlags;
    case MG_DATA_SAVE:
        return ((MysteryGiftSaveData *)func_0200a418(save))->receivedFlags;
    }
    return NULL;
}

// pokebw2's getCard
static MysteryGift *func_0200a43c(MysteryGiftSave *save, u32 slot) {
    func_0200a400(save);
    switch (save->kind) {
    case MG_DATA_OUTSIDE:
        return &((SaveOutsideGifts *)func_0200a418(save))->cards[slot];
    case MG_DATA_SAVE:
        return &((MysteryGiftSaveData *)func_0200a418(save))->cards[slot];
    }
    return NULL;
}

// pokebw2's encrypt
static void func_0200a480(MysteryGiftSave *save) {
    void *data = func_0200a418(save);
    mgEncryptData(data, func_0200a3fc(save));
}

// pokebw2's decrypt
static void func_0200a49c(MysteryGiftSave *save) {
    void *data = func_0200a418(save);
    mgDecryptData(data, func_0200a3fc(save));
}

void mgEncryptData(void *data, u32 kind) {
    u32 size;
    u16 *tail;

    switch (kind) {
    case MG_DATA_OUTSIDE:
        size = sizeof(SaveOutsideGifts);
        tail = &((SaveOutsideGifts *)data)->checksum;
        break;
    case MG_DATA_SAVE:
        size = sizeof(MysteryGiftSaveData);
        tail = &((MysteryGiftSaveData *)data)->checksum;
        break;
    }
    tail[1] = (OS_GetVBlankCount() << 8) | OS_GetVBlankCount();
    if (tail[1] == 0) {
        tail[1] = 1;
    }
    tail[0] = addUpDataStream(data, size - 4);
    decryptData(data, size - 4, tail[0] + (tail[1] << 16));
}

void mgDecryptData(void *data, u32 kind) {
    u32 size;
    u16 *tail;

    switch (kind) {
    case MG_DATA_OUTSIDE:
        size = sizeof(SaveOutsideGifts);
        tail = &((SaveOutsideGifts *)data)->checksum;
        break;
    case MG_DATA_SAVE:
        size = sizeof(MysteryGiftSaveData);
        tail = &((MysteryGiftSaveData *)data)->checksum;
        break;
    }
    _decryptData(data, size - 4, tail[0] + (tail[1] << 16));
    tail[1] = 0;
}
