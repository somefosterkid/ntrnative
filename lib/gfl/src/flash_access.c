// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/backup_card.c, ported by tools/port_file.py.
// Black's asserts give its original name, flash_access.c.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/backup_card.h"
#include "gfl/std.h"
#include "nitro/card.h"
#include "nitro/os.h"

void setCardWrittenVerified(void *arg);
void func_020338b8(u16 lockId, BOOL noResponse, BackupWriteErrorCallback callback);

// Whether the last write has ended
// pokebw2's sWriteDone
static BOOL data_020a789c = TRUE;

// pokebw2's sLocked
static BOOL data_02146998;

// Black's asserts name flash_access.c and the expression as the ROM's strings give them, so they're written out
BOOL identifyGameCardBackup(void) {
    u32 lock_id = OS_GetLockID();
    CARDBackupType type;

    GFL_ASSERT_AT(56, lock_id != OS_LOCK_ID_ERROR);
    CARD_LockBackup(lock_id);
    type = CARD_BACKUP_TYPE_FLASH_4MBITS_EX;
    if (!CARD_IdentifyBackup(type)) {
        type = 0;
    }
    CARD_UnlockBackup(lock_id);
    OS_ReleaseLockID(lock_id);
    return type != 0;
}

BOOL func_020335ac(u32 dest, const void *src, u32 size, BackupWriteErrorCallback callback) {
    BOOL result;
    u16 lockId = func_020336b8(dest, src, size, callback);

    while (!finishBackupGetResult(lockId, &result, callback)) {
    }
    return result;
}

BOOL readFlashWaitSuccess(u32 src, void *dest, u32 size, BackupReadErrorCallback callback) {
    u32 lock_id = OS_GetLockID();
    BOOL result;

    GFL_ASSERT_AT(164, lock_id != OS_LOCK_ID_ERROR);
    CARD_LockBackup(lock_id);
    CARD_ReadBackupAsync(src, dest, size, NULL, NULL);
    result = CARD_WaitBackupAsync();
    CARD_UnlockBackup(lock_id);
    OS_ReleaseLockID(lock_id);
    if (!result && callback != NULL) {
        callback();
    }
    return result;
}

void setCardWrittenVerified(void *arg) {
    data_020a789c = TRUE;
}

u16 func_020336b8(u32 dest, const void *src, u32 size, BackupWriteErrorCallback callback) {
    u32 lock_id = OS_GetLockID();
    u32 header;

    GFL_ASSERT_AT(212, lock_id != OS_LOCK_ID_ERROR);
    CARD_LockBackup(lock_id);
    data_02146998 = TRUE;
    if (!CARD_ReadBackup(0, &header, sizeof(header))) {
        func_020338b8(lock_id, TRUE, callback);
    }
    data_020a789c = FALSE;
    CARD_WriteAndVerifyBackupAsync(dest, src, size, setCardWrittenVerified, NULL);
    return lock_id;
}

BOOL finishBackupGetResult(u16 lockId, BOOL *result, BackupWriteErrorCallback callback) {
    if (data_020a789c == TRUE) {
        CARD_UnlockBackup(lockId);
        OS_ReleaseLockID(lockId);
        data_02146998 = FALSE;
        switch (CARD_GetResultCode()) {
        case CARD_RESULT_SUCCESS:
            *result = TRUE;
            break;
        default:
        case CARD_RESULT_FAILURE:
        case CARD_RESULT_INVALID_PARAM:
        case CARD_RESULT_UNSUPPORTED:
        case CARD_RESULT_TIMEOUT:
        case CARD_RESULT_ERROR:
        case CARD_RESULT_CANCELED:
            *result = FALSE;
            func_020338b8(lockId, FALSE, callback);
            break;
        case CARD_RESULT_NO_RESPONSE:
            *result = FALSE;
            func_020338b8(lockId, TRUE, callback);
            break;
        }
        return TRUE;
    }
    return FALSE;
}

void cancelCardProcess(u16 lockId) {
    if (CARD_TryWaitBackupAsync() == FALSE) {
        CARD_CancelBackupAsync();
        CARD_WaitBackupAsync();
    }
    if (data_02146998 == TRUE) {
        CARD_UnlockBackup(lockId);
        OS_ReleaseLockID(lockId);
        data_02146998 = FALSE;
    }
}

void func_020338b8(u16 lockId, BOOL noResponse, BackupWriteErrorCallback callback) {
    CARD_UnlockBackup(lockId);
    OS_ReleaseLockID(lockId);
    data_02146998 = FALSE;
    if (callback != NULL) {
        callback(noResponse);
    }
}

BOOL getLockIDStatus(void) {
    return data_02146998;
}
