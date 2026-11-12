// TwlSDK's OS library, os_reset.c: Black's main 0x02087b78-0x02087c6c. Its functions are twins of libntr's (MIT,
// https://github.com/cybervisi0n/libntr) libraries/os/src/os_reset.c for SDK 5, whose TWL headers lib/nitro/sdk
// doesn't have yet. The game calls OS_ResetSystem to restart.
#include "nitro/os.h"

// Black's functions in order (Black's size), to write below:
// OS_InitReset: 0x54
// OS_ResetSystem: 0xa0

#include "nitro/pxi.h"
#include "system/os_spinLock.h"

u16 data_02151368;

void OSi_CommonCallback(int fifoTag, uPtr data, BOOL err);
void CARD_LockRom(u16 lock_id);

void OS_InitReset(void) {
    if (data_02151368 != 0) {
        return;
    }
    data_02151368 = 1;
    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_OS, PXI_PROC_ARM7)) {
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_OS, OSi_CommonCallback);
}

// Fixed RAM of the reset: the state of the reset, the parameter it passes on, and the TWL title ID's high word
#define OS_RESET_STATE_ADDR 0x02fffc40
#define OS_RESET_PARAM_ADDR 0x02fffc20
#define HW_TWL_TITLE_ID_HI 0x02ffe234

void PMi_ExecutePostExitCallbackList(void);
void MI_StopAllDma(void);
void MI_StopAllNDma(void);
void OSi_SendToPxi(s32 arg);
void OSi_DoResetSystem(void);

void OS_ResetSystem(u32 param) {
    u32 lockId;

    if (*(u16 *)OS_RESET_STATE_ADDR == 2 || (OS_IsRunOnTwl() == 1 && (*(u32 *)HW_TWL_TITLE_ID_HI & 4) != 0)) {
        OS_Terminate();
    }
    if (OS_IsRunOnTwl() != 0) {
        PMi_ExecutePostExitCallbackList();
    }
    lockId = OS_GetLockID();
    CARD_LockRom(lockId);
    OS_SetIrqMask(0x40000);
    OS_ResetRequestIrqMask(0xffffffff);
    MI_StopAllDma();
    if (OS_IsRunOnTwl() != 0) {
        MI_StopAllNDma();
    }
    *(u32 *)OS_RESET_PARAM_ADDR = param;
    OSi_SendToPxi(0x10);
    OSi_DoResetSystem();
}


