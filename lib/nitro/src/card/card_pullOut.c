// TwlSDK's CARD library, card_pullOut.c: Black's main 0x020773dc-0x020775b8. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "card_internal.h"
#include "nitro/mi.h"
#include "nitro/pxi.h"

// PM_ForceToPowerOff, in pm.c
void PM_ForceToPowerOff(void);
// TwlSDK's PMi_ExecutePostExitCallbackList and MI_StopAllNDma, in the LTD autoload
void PMi_ExecutePostExitCallbackList(void);
void MI_StopAllNDma(void);

static CARDPulledOutCallback CARD_UserCallback;
static u32 CARDiSlotResetCount;
static BOOL CARDi_IsPulledOutFlag = FALSE;

static void CARDi_PulledOutCallback(int tag, uPtr data, BOOL err);
static void CARDi_SendtoPxi(u32 data, u32 wait);

void CARD_InitPulledOutCallback(void) {
    PXI_Init();

    CARDiSlotResetCount = 0;
    CARDi_IsPulledOutFlag = FALSE;

    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_CARD, CARDi_PulledOutCallback);

    CARD_UserCallback = NULL;
}

static void CARDi_PulledOutCallback(int tag, uPtr data, BOOL err) {
    u32 command = data & CARD_PXI_COMMAND_MASK;

    if (command == CARD_PXI_COMMAND_PULLED_OUT) {
        if (CARDi_IsPulledOutFlag == FALSE) {
            BOOL isTerminateImm = TRUE;
            CARDi_IsPulledOutFlag = TRUE;

            CARDi_NotifyEvent(CARD_EVENT_PULLEDOUT, NULL);

            if (CARD_UserCallback) {
                isTerminateImm = CARD_UserCallback();
            }

            if (isTerminateImm) {
                CARD_TerminateForPulledOut();
            }
        }
    } else if (command == CARD_PXI_COMMAND_RESET_SLOT) {
        CARDiSlotResetCount += 1;
        CARDi_IsPulledOutFlag = FALSE;
        CARDi_NotifyEvent(CARD_EVENT_SLOTRESET, NULL);
    } else {
        OS_Terminate();
    }
}

void CARD_SetPulledOutCallback(CARDPulledOutCallback callback) {
    CARD_UserCallback = callback;
}

BOOL CARD_IsPulledOut(void) {
    return CARDi_IsPulledOutFlag;
}

void CARD_TerminateForPulledOut(void) {
    if (PAD_DetectFold()) {
        PM_ForceToPowerOff();
    }

    if (OS_IsRunOnTwl()) {
        PMi_ExecutePostExitCallbackList();
    }

    CARDi_SendtoPxi(CARD_PXI_COMMAND_TERMINATE, 1);

    MI_StopAllDma();
    if (OS_IsRunOnTwl()) {
        MI_StopAllNDma();
    }

    OS_Terminate();
}

void CARDi_CheckPulledOutCore(u32 id) {
    vu32 iplCardID = *(vu32 *)(CARD_IPL_CARD_ID_BUF);

    if (id != (u32)iplCardID) {
        OSIntrMode bak_cpsr = OS_DisableInterrupts();
        CARDi_PulledOutCallback(PXI_FIFO_TAG_CARD, CARD_PXI_COMMAND_PULLED_OUT, FALSE);
        OS_RestoreInterrupts(bak_cpsr);
    }
}

static void CARDi_SendtoPxi(u32 data, u32 wait) {
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_CARD, data, FALSE) != PXI_FIFO_SUCCESS) {
        SVC_WaitByLoop(wait);
    }
}
