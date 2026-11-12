// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/backup_card.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_GFL_BACKUP_CARD_H
#define BWPORT_GFL_BACKUP_CARD_H

#include "types.h"

// Called when a read fails
typedef void (*BackupReadErrorCallback)(void);

// Called when a write fails, with whether the card stopped responding
typedef void (*BackupWriteErrorCallback)(BOOL noResponse);

void cancelCardProcess(u16 lockId);
// Whether the backup is locked for a write
BOOL getLockIDStatus(void);
// Writes and waits, returning whether the write succeeded
BOOL func_020335ac(u32 dest, const void *src, u32 size, BackupWriteErrorCallback callback);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Whether the card has the 4-megabit flash the game expects
BOOL identifyGameCardBackup(void);
BOOL readFlashWaitSuccess(u32 src, void *dest, u32 size, BackupReadErrorCallback callback);
// Returns whether the write has ended, and if so whether it succeeded in *result
BOOL finishBackupGetResult(u16 lockId, BOOL *result, BackupWriteErrorCallback callback);
u16 func_020336b8(u32 dest, const void *src, u32 size, BackupWriteErrorCallback callback);

#endif // BWPORT_GFL_BACKUP_CARD_H
