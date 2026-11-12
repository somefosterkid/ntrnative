// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov331/save_outside.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/backup_system.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/ui.h"
#include "save/mystery_gift.h"
#include "save/save_control.h"
#include "save/save_outside.h"
#include "nitro/mi.h"

// Data outside the save, at the end of the card: the gifts at GIFTS_ADDR and their backup copy at GIFTS_BACKUP_ADDR,
// then the keys. A block is a header followed by its data, whose CRC it keeps. The file's name is the ROM's

#define GIFTS_ADDR 0x7f800
#define GIFTS_BACKUP_ADDR 0x7fc00
#define KEYS_ADDR 0x7e800
#define SAVE_OUTSIDE_SIZE 0xb00
#define KEYS_SIZE 0x300

#define SAVE_OUTSIDE_MAGIC 0xa10f49ae

// The size of the key system's data: the key information and the Memory Link's
#define KEY_DATA_SIZE 0x214

enum {
    BLOCK_GIFTS,
    BLOCK_GIFTS_BACKUP,
    BLOCK_KEYS,
    BLOCK_COUNT,
};

// What the load found of the gifts' copies
enum {
    GIFTS_OK,
    GIFTS_MAIN_BROKEN,
    GIFTS_BACKUP_BROKEN,
    GIFTS_BROKEN,
};

typedef struct {
    u32 magic;
    // Counts the saves, so that the newer copy wins
    u32 count;
    u16 crc;
    // The size of the data, which isn't saved
    u16 size;
} SaveOutsideHeader;

typedef struct {
    SaveOutsideHeader header;
    SaveOutsideGifts gifts;
} SaveOutsideGiftsBlock;

typedef struct {
    SaveOutsideHeader header;
    u8 data[KEY_DATA_SIZE];
} SaveOutsideKeysBlock;

struct SaveOutside {
    SaveOutsideGiftsBlock gifts;
    u32 giftsState;
    u8 giftsLoaded;
    u8 saveSeq;
    // The copy of the gifts that is written first
    u8 saveSlot;
    u8 keysLoaded;
    u16 lockId;
    u8 keysBroken;
};

// pokebw2's SaveOutside_Free

SaveOutside *func_ov228_021dfc20(HeapID heapId) {
    SaveOutsideGiftsBlock *blocks[2];
    BOOL loaded[2];
    BOOL broken[2];
    SaveOutside *work;
    s32 latest;
    SaveOutsideGiftsBlock *backup;
    int i;

    latest = -1;
    work = GFL_HeapAllocate(heapId, sizeof(SaveOutside), TRUE, "save_outside.c", 91);
    blocks[BLOCK_GIFTS] = &work->gifts;
    blocks[BLOCK_GIFTS_BACKUP] = GFL_HeapAllocate(heapId, sizeof(SaveOutsideGiftsBlock), TRUE, "save_outside.c", 93);
    for (i = 0; i < 2; i++) {
        loaded[i] = FALSE;
        broken[i] = FALSE;
    }
    loaded[BLOCK_GIFTS] = func_020334cc(GIFTS_ADDR, blocks[BLOCK_GIFTS], sizeof(SaveOutsideGiftsBlock));
    backup = blocks[BLOCK_GIFTS_BACKUP];
    loaded[BLOCK_GIFTS_BACKUP] = func_020334cc(GIFTS_BACKUP_ADDR, backup, sizeof(SaveOutsideGiftsBlock));
    for (i = 0; i < 2; i++) {
        if (loaded[i] == TRUE) {
            if (blocks[i]->header.magic != SAVE_OUTSIDE_MAGIC) {
                loaded[i] = FALSE;
            } else if (blocks[i]->header.crc != getCRC16(&blocks[i]->gifts, sizeof(SaveOutsideGifts))) {
                broken[i] = TRUE;
            }
        }
    }

    work->giftsLoaded = FALSE;
    if (loaded[BLOCK_GIFTS] == FALSE && loaded[BLOCK_GIFTS_BACKUP] == FALSE) {
        // Nothing was saved
    } else if (loaded[BLOCK_GIFTS] == TRUE && loaded[BLOCK_GIFTS_BACKUP] == FALSE) {
        if (broken[BLOCK_GIFTS] == TRUE) {
            work->giftsState = GIFTS_BROKEN;
        } else {
            latest = BLOCK_GIFTS;
        }
    } else if (loaded[BLOCK_GIFTS] == FALSE && loaded[BLOCK_GIFTS_BACKUP] == TRUE) {
        if (broken[BLOCK_GIFTS_BACKUP] == TRUE) {
            work->giftsState = GIFTS_BROKEN;
        } else {
            latest = BLOCK_GIFTS_BACKUP;
        }
    } else if (loaded[BLOCK_GIFTS] == TRUE && loaded[BLOCK_GIFTS_BACKUP] == TRUE) {
        if (broken[BLOCK_GIFTS] == TRUE && broken[BLOCK_GIFTS_BACKUP] == TRUE) {
            work->giftsState = GIFTS_BROKEN;
        } else if (broken[BLOCK_GIFTS] == FALSE && broken[BLOCK_GIFTS_BACKUP] == FALSE) {
            u32 backupCount = backup->header.count;
            u32 count = blocks[BLOCK_GIFTS]->header.count;

            if (count >= backupCount || (count == 0 && backupCount == 0xffffffff)) {
                latest = BLOCK_GIFTS;
            } else {
                latest = BLOCK_GIFTS_BACKUP;
            }
        } else if (broken[BLOCK_GIFTS] == FALSE) {
            work->giftsState = GIFTS_BACKUP_BROKEN;
            latest = BLOCK_GIFTS;
        } else {
            latest = BLOCK_GIFTS_BACKUP;
            work->giftsState = GIFTS_MAIN_BROKEN;
        }
    }

    if (latest == BLOCK_GIFTS_BACKUP) {
        MI_CpuCopy8(backup, blocks[BLOCK_GIFTS], sizeof(SaveOutsideGiftsBlock));
    }
    GFL_HeapFree(backup);
    if (latest != -1) {
        work->giftsLoaded = TRUE;
    }
    return work;
}

void func_ov228_021dfdb8(SaveOutside *work) {
    GFL_HeapFree(work);
}

void func_ov228_021dfdc0(SaveOutside *work) {
    work->saveSeq = 0;
    if (work->giftsState == GIFTS_BACKUP_BROKEN) {
        work->saveSlot = 1;
    } else {
        work->saveSlot = 0;
    }
    work->gifts.header.magic = SAVE_OUTSIDE_MAGIC;
    work->gifts.header.count++;
    work->gifts.header.crc = getCRC16(&work->gifts.gifts, sizeof(SaveOutsideGifts));
}


BOOL func_ov228_021dfe00(SaveOutside *work) {
    BOOL result;

    switch (work->saveSeq) {
    case 0:
    case 2:
        work->lockId = func_02033494(work->saveSlot == 0 ? GIFTS_ADDR : GIFTS_BACKUP_ADDR, work, sizeof(SaveOutsideGiftsBlock));
        work->saveSeq++;
        break;
    case 1:
    case 3:
        if (func_020334b0(work->lockId, &result) == TRUE) {
            work->saveSlot ^= 1;
            work->saveSeq++;
        }
        break;
    default:
        work->giftsState = GIFTS_OK;
        work->giftsLoaded = TRUE;
        return TRUE;
    }
    return FALSE;
}



// pokebw2's SaveOutside_IsGiftsLoaded
BOOL func_ov228_021dfe90(SaveOutside *work) {
    return work->giftsLoaded;
}

// pokebw2's SaveOutside_IsGiftsBroken
BOOL func_ov228_021dfe98(SaveOutside *work) {
    if (work->giftsState == GIFTS_BROKEN) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's SaveOutside_GetGifts
SaveOutsideGifts *func_ov228_021dfeac(SaveOutside *work) {
    return &work->gifts.gifts;
}

// pokebw2's SaveOutside_CopyGiftsToSave
void func_ov228_021dfeb0(SaveOutside *work, SaveControl *save) {
    MysteryGiftSaveData *giftSave = SaveControl_GetBlockPtr(save, SAVE_BLOCK_MYSTERY_GIFT);
    SaveOutsideGifts *gifts = &work->gifts.gifts;
    int i;

    mgDecryptData(giftSave, MG_DATA_SAVE);
    mgDecryptData(gifts, MG_DATA_OUTSIDE);
    for (i = 0; i < 0x100; i++) {
        giftSave->receivedFlags[i] = gifts->receivedFlags[i];
    }
    for (i = 0; i < 3; i++) {
        giftSave->cards[i] = gifts->cards[i];
    }
    mgEncryptData(giftSave, MG_DATA_SAVE);
    mgEncryptData(gifts, MG_DATA_OUTSIDE);
}

void MIi_CpuClear32(u32 value, void *dest, u32 size);

void func_ov228_021dff18(HeapID heapId) {
    void *dest;

    dest = GFL_HeapAllocate(heapId, 0x800, FALSE, "save_outside.c", 339);
    MIi_CpuClear32(0xffffffff, dest, 0x800);
    func_02033478(GIFTS_ADDR, dest, 0x800);
    GFL_HeapFree(dest);
}


