// TwlSDK's CARD library, card_common.c: Black's main 0x02075ed4-0x02076100. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "card_internal.h"
#include "nitro/mi.h"
#include "nitro/pxi.h"

CARDiCommon cardi_common ATTRIBUTE_ALIGN(32);
static CARDiCommandArg cardi_arg ATTRIBUTE_ALIGN(32);

void CARDi_LockResource(CARDiOwner owner, CARDTargetMode target) {
    CARDiCommon *p = &cardi_common;
    OSIntrMode bak_psr = OS_DisableInterrupts();

    if (p->lock_owner == owner) {
        if (p->lock_target != target) {
            OS_Terminate();
        }
    } else {
        while (p->lock_owner != OS_LOCK_ID_ERROR) {
            OS_SleepThread(p->lock_queue);
        }
        p->lock_owner = owner;
        p->lock_target = target;
    }

    ++p->lock_ref;
    OS_RestoreInterrupts(bak_psr);
}

void CARDi_UnlockResource(CARDiOwner owner, CARDTargetMode target) {
    CARDiCommon *p = &cardi_common;
    OSIntrMode bak_psr = OS_DisableInterrupts();

    if (p->lock_owner != owner || !p->lock_ref) {
        OS_Terminate();
    } else {
        if (p->lock_target != target) {
            OS_Terminate();
        }
        if (!--p->lock_ref) {
            p->lock_owner = OS_LOCK_ID_ERROR;
            p->lock_target = CARD_TARGET_NONE;
            OS_WakeupThread(p->lock_queue);
        }
    }
    OS_RestoreInterrupts(bak_psr);
}

CARDAccessLevel CARDi_GetAccessLevel(void) {
    CARDAccessLevel level = CARD_ACCESS_LEVEL_NONE;

    if (OS_GetBootType() == 1) {
        level = CARD_ACCESS_LEVEL_FULL;
    } else if (!OS_IsRunOnTwl()) {
        level = CARD_ACCESS_LEVEL_FULL;
    } else {
        const CARDRomHeaderTWL *header = CARD_GetOwnRomHeaderTWL();
        BOOL backupPowerOn = FALSE;

        if (header->access_control.game_card_nitro_mode) {
            level |= CARD_ACCESS_LEVEL_ROM;
            backupPowerOn = TRUE;
        } else if (header->access_control.game_card_on) {
            backupPowerOn = TRUE;
        }
        if (backupPowerOn) {
            if (header->access_control.backup_access_read) {
                level |= CARD_ACCESS_LEVEL_BACKUP_R;
            }
            if (header->access_control.backup_access_write) {
                level |= CARD_ACCESS_LEVEL_BACKUP_W;
            }
        }
    }
    return level;
}

BOOL CARDi_WaitAsync(void) {
    return CARDi_WaitForTask(&cardi_common, FALSE, NULL, NULL);
}

BOOL CARDi_TryWaitAsync(void) {
    CARDiCommon *p = &cardi_common;

    return !(p->flag & CARD_STAT_BUSY);
}

void CARDi_InitResourceLock(void) {
    CARDiCommon *p = &cardi_common;

    p->lock_owner = OS_LOCK_ID_ERROR;
    p->lock_ref = 0;
    p->lock_target = CARD_TARGET_NONE;
    OS_InitThreadQueue(p->lock_queue);
}

void CARDi_InitCommand(void) {
    CARDiCommon *p = &cardi_common;

    p->cmd = &cardi_arg;
    MIi_CpuClearFast(0, &cardi_arg, sizeof(cardi_arg));
    DC_FlushRange(&cardi_arg, sizeof(cardi_arg));
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_FS, CARDi_OnFifoRecv);
}
