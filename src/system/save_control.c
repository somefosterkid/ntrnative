// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/save_control.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/save_control.h"
#include "gfl/backup_system.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "save/key_info.h"
#include "save/memory_link_savedata.h"
#include "save/musical_savedata.h"
#include "save/save_outside.h"
#include "save/save_table.h"
#include "save/trainer_data_savedata.h"
#include "system/box_savedata.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
void func_020115f0(SaveControl *save);
BOOL func_0203bee8(u32 dest, const void *src, u32 size);
void MI_CpuCopy8(const void *src, void *dest, u32 size);

// The game's save: loading it when the game starts, saving it a step at a time, and the extra saves. The name is the
// ROM's string, and data_02146168, ctrl, sv_extra and extra_id are the names its assertions keep. The tables of the
// saves' blocks are save_table.c's.

#define OVERLAY_SAVE_OUTSIDE OVERLAY_ID(331)

// The data outside the save that save_outside.c (overlay 331) reads: the gifts, their backup copy and the keys
#define SAVE_OUTSIDE_GIFTS_ADDR 0x7f800
#define SAVE_OUTSIDE_GIFTS_BACKUP_ADDR 0x7fc00
#define SAVE_OUTSIDE_KEYS_ADDR 0x7e800

// GFLib's GFL_STD_MemCopy, an inline over the copy. func_02006f20 reads the destination before the call that
// gives the source only through it
static inline void GFL_STD_MemCopy(const void *src, void *dest, u32 size) {
    MI_CpuCopy8(src, dest, size);
}

static void func_02007428(void);
void func_02007454(BoxMainSave *main);

// pokebw2's SaveControlWork
static SaveControl *data_02146168;


SaveControl *func_02006f20(HeapID heapId) {
    SaveOutside *work;
    BOOL giftsLoaded;
    BOOL giftsBroken;
    SaveControl *save;
    u32 result;
    u32 extraResult;
    SaveExtraID i;

    GFL_OvlLoad(OVERLAY_ID(228));
    work = func_ov228_021dfc20(heapId);
    giftsLoaded = func_ov228_021dfe90(work);
    giftsBroken = func_ov228_021dfe98(work);
    func_ov228_021dfdb8(work);
    GFL_OvlUnload(OVERLAY_ID(228));

    save = GFL_HeapAllocate(heapId, sizeof(SaveControl), TRUE, "save_control.c", 0x58);
    data_02146168 = save;
    save->newGame = TRUE;
    save->unk2 = TRUE;
    save->dataExists = FALSE;
    save->giftsLoaded = giftsLoaded;
    save->giftsBroken = giftsBroken;
    save->sv = SaveData_InitMain(&data_0209d938, heapId);
    result = func_02031ae0(save->sv, HEAPID_USER);

    if ((result == SAVE_LOAD_RESULT_OK || result == SAVE_LOAD_RESULT_OK_PARTIAL) && func_02008754(save) == TRUE) {
        result = SAVE_LOAD_RESULT_NONE;
    }
    save->errors = 0;
    if (giftsBroken == TRUE) {
        save->errors |= SAVE_CONTROL_ERROR_GIFTS_BROKEN;
    }
    switch (result) {
    case SAVE_LOAD_RESULT_OK:
        save->unk2 = FALSE;
        // fallthrough
    case SAVE_LOAD_RESULT_OK_PARTIAL:
        save->newGame = FALSE;
        save->dataExists = TRUE;
        if (result == SAVE_LOAD_RESULT_OK_PARTIAL) {
            save->errors |= SAVE_CONTROL_ERROR_PARTIAL;
        }
        // Each of the first extras, the ones before the films
        for (i = SAVE_EXTRA_ID_BATTLE_REC_0; i < SAVE_EXTRA_ID_POKEWOOD_0; i++) {
            if (func_02008784(save, i) == TRUE) {
                extraResult = func_0200725c(save, i, HEAPID_USER);
                freeIntermediateSaveExtraBlksAfterLoad(save, i);
                switch (extraResult) {
                case SAVE_LOAD_RESULT_OK:
                case SAVE_LOAD_RESULT_OK_PARTIAL:
                    if (i == SAVE_EXTRA_ID_MUSICAL_DIST) {
                        func_0200a7ec(getAddressOfMusicalDataInfo(save), TRUE);
                    }
                    break;
                case SAVE_LOAD_RESULT_NONE:
                case SAVE_LOAD_RESULT_BROKEN:
                case SAVE_LOAD_RESULT_ERROR:
                    save->errors |= SAVE_CONTROL_ERROR_EXTRA(i);
                    if (i == SAVE_EXTRA_ID_MUSICAL_DIST) {
                        func_0200a7ec(getAddressOfMusicalDataInfo(save), FALSE);
                    }
                    break;
                }
            } else if (i == SAVE_EXTRA_ID_MUSICAL_DIST) {
                func_0200a7ec(getAddressOfMusicalDataInfo(save), FALSE);
            }
        }
        break;
    case SAVE_LOAD_RESULT_BROKEN:
        save->errors |= SAVE_CONTROL_ERROR_BROKEN;
        // fallthrough
    case SAVE_LOAD_RESULT_NONE:
    case SAVE_LOAD_RESULT_ERROR:
        initSaveBlkCallbacks(save->sv);
        break;
    default:
        GFL_ASSERT(0);
    }
    return save;
}

void SaveControl_Free(void) {
    if (data_02146168 != NULL) {
        freeSaveBlocks(data_02146168->sv);
        GFL_HeapFree(data_02146168);
        data_02146168 = NULL;
    }
}

SaveControl *SaveControl_GetInstance(void) {
    GFL_ASSERT(data_02146168 != NULL);
    return data_02146168;
}

u32 func_020070c8(SaveControl *save) {
    u32 result;
    u32 extraResult;
    MusicalSave *musical;
    SaveExtraID i;

    result = func_02031ae0(save->sv, HEAPID_USER);
    switch (result) {
    case SAVE_LOAD_RESULT_OK:
    case SAVE_LOAD_RESULT_OK_PARTIAL:
        save->dataExists = TRUE;
        musical = getAddressOfMusicalDataInfo(save);
        func_0200a7ec(musical, FALSE);
        if (func_02008784(save, SAVE_EXTRA_ID_MUSICAL_DIST) == TRUE) {
            extraResult = func_0200725c(save, SAVE_EXTRA_ID_MUSICAL_DIST, HEAPID_USER);
            freeIntermediateSaveExtraBlksAfterLoad(save, SAVE_EXTRA_ID_MUSICAL_DIST);
            switch (extraResult) {
            case SAVE_LOAD_RESULT_OK:
            case SAVE_LOAD_RESULT_OK_PARTIAL:
                func_0200a7ec(musical, TRUE);
                break;
            }
        }
        if (save->errors != 0) {
            // Each of the first extras, the ones before the films
            for (i = SAVE_EXTRA_ID_BATTLE_REC_0; i < SAVE_EXTRA_ID_POKEWOOD_0; i++) {
                if (save->errors & SAVE_CONTROL_ERROR_EXTRA(i)) {
                    func_020087c0(save, i);
                }
            }
        }
        break;
    }
    return result;
}


void func_02007150(SaveControl *save) {
    save->unk3 = func_02008754(save);
    func_02008774(save);
    func_02031b8c(save->sv);
}

u32 func_02007168(SaveControl *save) {
    u32 result;

    if (save->giftsLoaded == TRUE || save->giftsBroken == TRUE) {
        func_02007428();
        save->giftsLoaded = FALSE;
        save->giftsBroken = FALSE;
        return SAVE_RESULT_CONTINUE;
    }
    result = func_02031bb4(save->sv);
    if (result == SAVE_RESULT_OK && func_02008754(save) == FALSE) {
        save->newGame = FALSE;
        save->dataExists = TRUE;
    }
    return result;
}

void func_020071a8(SaveControl *save) {
    func_02031c04(save->sv);
    if (save->unk3 == TRUE) {
        func_02008764(save);
    }
}

u32 func_020071c0(SaveControl *save) {
    return func_02031e68(save->sv);
}

void *SaveControl_GetBlockPtr(SaveControl *save, u32 block) {
    return SaveData_GetBlockPtr(save->sv, block);
}

void *func_020071d8(SaveControl *save, u32 *size) {
    return func_02031994(save->sv, size);
}

u32 SaveControl_GetStatus(SaveControl *save) {
    return save->newGame;
}

u32 func_020071e8(SaveControl *save) {
    return save->errors;
}

BOOL SaveControl_IsDataAlreadyPresent(SaveControl *save) {
    return save->dataExists;
}

BOOL func_020071f0(SaveControl *save) {
    if (SaveControl_GetStatus(save) == TRUE && SaveControl_IsDataAlreadyPresent(save) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

u8 func_02007210(SaveControl *save) {
    return save->giftsLoaded;
}


void func_02007214(SaveControl *save, u32 value) {
    save->giftsLoaded = value;
}

u8 func_02007218(SaveControl *save) {
    return save->giftsBroken;
}


void func_0200721c(SaveControl *save, u32 value) {
    save->giftsBroken = value;
}

void func_02007220(SaveControl *save) {
    save->newGame = TRUE;
    save->unk2 = TRUE;
    initSaveBlkCallbacks(save->sv);
}

void func_02007230(SaveControl *save) {
    func_020319f0(save->sv, HEAPID_USER);
}

void func_0200723c(SaveControl *save, u32 *a1, u32 *a2) {
    func_020319e4(save->sv, a1, a2);
}

BOOL func_02007248(SaveControl *save, u32 block, u32 copy, void *dest, u32 size) {
    return func_020334e8(save->sv, block, copy, dest, size);
}

u32 func_0200725c(SaveControl *save, u32 block, HeapID heapId) {
    save->sv_extra[block] = SaveData_InitMain(&sv_extra_blk_data[block], heapId);
    if (func_02008784(save, block) == FALSE) {
        return SAVE_LOAD_RESULT_NONE;
    }
    return func_02031dc0(save->sv_extra[block], heapId, *func_020087d4(save, block));
}

void freeIntermediateSaveExtraBlksAfterLoad(SaveControl *ctrl, u32 extra_id) {
    GFL_ASSERT(ctrl->sv_extra[extra_id] != NULL);
    freeSaveBlocks(ctrl->sv_extra[extra_id]);
    ctrl->sv_extra[extra_id] = NULL;
}

u32 func_020072cc(SaveControl *save, u32 block, u16 heapId, void *buffer, u32 size) {
    save->sv_extra[block] = func_02031948(&sv_extra_blk_data[block], heapId, buffer, size, TRUE);
    if (func_02008784(save, block) == FALSE) {
        return SAVE_LOAD_RESULT_NONE;
    }
    return func_02031dc0(save->sv_extra[block], heapId, *func_020087d4(save, block));
}

void func_02007324(SaveControl *save, u32 block, u32 heapId, void *buffer, u32 size) {
    save->sv_extra[block] = func_02031948(&sv_extra_blk_data[block], heapId, buffer, size, FALSE);
}

void freeIntermediateSaveExtraBlksAfterLoad2(SaveControl *ctrl, u32 extra_id) {
    GFL_ASSERT(ctrl->sv_extra[extra_id] != NULL);
    freeSaveBlocks(ctrl->sv_extra[extra_id]);
    ctrl->sv_extra[extra_id] = NULL;
}

void func_02007374(SaveControl *ctrl, u32 extra_id) {
    GFL_ASSERT(ctrl->sv_extra[extra_id] != NULL);
    func_020087a8(ctrl, extra_id);
    func_02031c10(ctrl->sv, ctrl->sv_extra[extra_id], func_020087d4(ctrl, extra_id));
}

u32 func_020073b0(SaveControl *save, u32 extra_id) {
    GFL_ASSERT(save->sv_extra[extra_id] != NULL);
    return func_02031c30(save->sv, save->sv_extra[extra_id]);
}


void *getAddressOfExtraSaveBlk(SaveControl *ctrl, u32 extra_id, u32 arg2) {
    GFL_ASSERT(ctrl->sv_extra[extra_id] != NULL);
    return SaveData_GetBlockPtr(ctrl->sv_extra[extra_id], arg2);
}

BOOL func_02007400(SaveControl *save, u32 extra_id, u32 heapId) {
    GFL_ASSERT(save->sv_extra[extra_id] != NULL);
    return func_020319f0(save->sv_extra[extra_id], heapId);
}


// Erases the gifts outside the save, by clearing the start of both copies' headers
void func_02007428(void) {
    u32 zero = 0;

    func_02033478(SAVE_OUTSIDE_GIFTS_ADDR, &zero, sizeof(zero));
    func_02033478(SAVE_OUTSIDE_GIFTS_BACKUP_ADDR, &zero, sizeof(zero));
}

// Erases the keys outside the save
void func_02007454(BoxMainSave *main) {
    BoxMainSave_Init(main);
}
