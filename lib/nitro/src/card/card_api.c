// TwlSDK's CARD library, card_api.c: Black's main 0x02075c38-0x02075ed4. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "card_internal.h"
#include "nitro/mi.h"

typedef struct CARDEventListener {
    OSEvent event[1];
    OSVAlarm valarm[1];
    BOOL (*Condition)(void *);
    void *userdata;
    u16 lockID;
    u8 padding[2];
} CARDEventListener;

static BOOL CARDi_EnableFlag = FALSE;

static void CARDi_PeekEventListener(void *arg) {
    CARDEventListener *el = (CARDEventListener *)arg;

    if (el->Condition(el->userdata)) {
        OS_SignalEvent(el->event, 0x00000001UL);
    } else {
        OS_SetVAlarm(el->valarm, HW_LCD_HEIGHT, OS_VALARM_DELAY_MAX, CARDi_PeekEventListener, el);
    }
}

static inline void CARDi_InitEventListener(CARDEventListener *el) {
    OS_InitEvent(el->event);
    OS_CreateVAlarm(el->valarm);
}

static inline void CARDi_SetEventListener(CARDEventListener *el, BOOL (*condition)(void *), void *userdata) {
    el->Condition = condition;
    el->userdata = userdata;
    CARDi_PeekEventListener(el);
}

static inline void CARDi_WaitForEvent(CARDEventListener *el) {
    OS_WaitEventEx(el->event, 0x00000001UL, OS_EVENT_MODE_AND, 0x00000001UL);
}

static BOOL CARDi_LockBusCondition(void *userdata) {
    u16 lockID = *(const u16 *)userdata;

    return (OS_TryLockCard(lockID) == OS_LOCK_SUCCESS);
}

void CARD_Init(void) {
    CARDiCommon *p = &cardi_common;

    if (!p->flag) {
        p->flag = CARD_STAT_INIT;

        if (OS_GetBootType() == OS_BOOTTYPE_ROM) {
            MI_CpuCopy8((const void *)HW_ROM_HEADER_BUF, (void *)HW_CARD_ROM_HEADER, HW_CARD_ROM_HEADER_SIZE);
        }

        p->src = 0;
        p->dst = 0;
        p->len = 0;
        p->dma = MI_DMA_NOT_USE;
        p->DmaCall = NULL;
        p->flush_threshold_ic = 0x400;
        p->flush_threshold_dc = 0x2400;
        cardi_rom_base = 0;
        p->priority = CARD_THREAD_PRIORITY_DEFAULT;

        CARDi_InitResourceLock();

        p->callback = NULL;
        p->callback_arg = NULL;
        OS_InitThreadQueue(p->busy_q);
        OS_CreateThread(p->thread.context, CARDi_OldTypeTaskThread, NULL, p->thread.stack + sizeof(p->thread.stack),
                        sizeof(p->thread.stack), p->priority);
        OS_WakeupThreadDirect(p->thread.context);

        CARDi_InitCommand();

        CARDi_InitRom();

        if (OS_GetBootType() == OS_BOOTTYPE_ROM) {
            CARD_Enable(TRUE);
        }

        CARD_InitPulledOutCallback();
    }
}

BOOL CARD_IsEnabled(void) {
    return CARDi_EnableFlag;
}

void CARD_CheckEnabled(void) {
    if (!CARD_IsEnabled()) {
        OS_Terminate();
    }
}

void CARD_Enable(BOOL enable) {
    CARDi_EnableFlag = enable;
}

CARDResult CARD_GetResultCode(void) {
    CARDiCommon *p = &cardi_common;

    return p->cmd->result;
}

const u8 *CARD_GetRomHeader(void) {
    return (const u8 *)HW_CARD_ROM_HEADER;
}

const CARDRomHeader *CARD_GetOwnRomHeader(void) {
    return (const CARDRomHeader *)HW_ROM_HEADER_BUF;
}

const CARDRomHeaderTWL *CARD_GetOwnRomHeaderTWL(void) {
    return (const CARDRomHeaderTWL *)HW_TWL_ROM_HEADER_BUF;
}

void CARD_LockRom(u16 lock_id) {
    CARDi_LockResource(lock_id, CARD_TARGET_ROM);

    {
        CARDEventListener el[1];
        CARDi_InitEventListener(el);
        CARDi_SetEventListener(el, CARDi_LockBusCondition, &lock_id);
        CARDi_WaitForEvent(el);
    }
}

void CARD_UnlockRom(u16 lock_id) {
    OS_UnlockCard(lock_id);

    CARDi_UnlockResource(lock_id, CARD_TARGET_ROM);
}

void CARD_LockBackup(u16 lock_id) {
    CARDi_LockResource(lock_id, CARD_TARGET_BACKUP);
}

void CARD_UnlockBackup(u16 lock_id) {
    if (!CARD_TryWaitBackupAsync()) {
        CARD_WaitBackupAsync();
    }

    CARDi_UnlockResource(lock_id, CARD_TARGET_BACKUP);
}
