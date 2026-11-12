// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/backup_system.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_GFL_BACKUP_SYSTEM_H
#define BWPORT_GFL_BACKUP_SYSTEM_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/backup_card.h"

// Where a save is and what it holds
typedef struct {
    const SaveBlockDef *table;
    u32 count;
    u32 mainOffset;
    // The same as mainOffset for a save without a backup
    u32 backupOffset;
    u32 size;
    // Kept in the footer, which a load checks
    u32 magic;
} SaveDataParams;

enum {
    SAVE_LOAD_RESULT_NONE,
    SAVE_LOAD_RESULT_OK,
    // Loaded, with a block that only one copy had right
    SAVE_LOAD_RESULT_OK_PARTIAL,
    SAVE_LOAD_RESULT_BROKEN,
    SAVE_LOAD_RESULT_ERROR,
};

enum {
    SAVE_RESULT_CONTINUE,
    // Writing the footer, the last step
    SAVE_RESULT_LAST,
    SAVE_RESULT_OK,
    SAVE_RESULT_NG,
};

BOOL func_020334cc(u32 src, void *dest, u32 size);
BOOL func_020334b0(u16 lockId, BOOL *result);

void mainBackupSystemInit(u32 heapId, BackupReadErrorCallback readErrorCallback,
    BackupWriteErrorCallback writeErrorCallback);

SaveData *SaveData_InitMain(const SaveDataParams *params, u32 heapId);
void *SaveData_GetBlockPtr(SaveData *sv, u32 gmdataid);
// Initializes the blocks and marks every one to be written
void initSaveBlkCallbacks(SaveData *sv);

void freeSaveBlocks(SaveData *sv);
// The save's data and its size
void *func_02031994(SaveData *sv, u32 *size);
// Loads, returning a SAVE_LOAD_RESULT_*
u32 func_02031ae0(SaveData *sv, u32 heapId);
void func_02031b8c(SaveData *sv);
u32 func_02031bb4(SaveData *sv);
void func_02031c10(SaveData *sv, SaveData *sub, u32 *count);
u32 func_02031c30(SaveData *sv, SaveData *sub);
// Loads only a save whose count is count
u32 func_02031dc0(SaveData *sv, u32 heapId, u32 count);
// The bytes the save in progress has written
u32 func_02031e68(SaveData *sv);
// Access to the card with the system's callbacks
BOOL func_02033478(u32 dest, const void *src, u32 size);
// Reads a block from the main copy (0) or the backup (1)
BOOL func_020334e8(SaveData *sv, u32 gmdataid, u32 copy, void *dest, u32 size);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 getLockIDStatus_inline_stub(void);
// A save in the caller's memory, of svwk_size bytes. init tells whether to initialize the blocks
SaveData *func_02031948(const SaveDataParams *params, u32 heapId, void *buffer, u32 svwk_size, BOOL init);
// Compares the save with the card: in *changed the size of the blocks that differ, and in *total of all of them
void func_020319e4(SaveData *sv, u32 *changed, u32 *total);
// Erases the save in the card
BOOL func_020319f0(SaveData *sv, u32 heapId);
void func_02031c04(SaveData *sv);
u16 func_02033494(u32 dest, const void *src, u32 size);

#endif // BWPORT_GFL_BACKUP_SYSTEM_H
