// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/src/nwm/nwm_setup.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// TwlSDK's NWM, the DSi's wireless manager, ARM9 side, its setup: func_02702434 (NWM_Init) sets the library up with the
// system area that the game gives it. ARM code in the DSi-only LTD autoload; the file's name is pokebw2's

#include "types.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/spi/ARM9/pm.h"
#include "nitro/pxi.h"
#include "nitro/wm.h"
#include "nwm_private.h"
#include "twl/nwm.h"

static NWMRetCode func_02702468(NWMSys *sys, u32 size, u32 dmaNo);

// Whether func_02702434 has set the library up. The zero initializer gives the flag a second literal for its address,
// as the original has: func_02702468 reads and stores it through one literal and passes DC_StoreRange its address
// through another
u8 data_027341a0 = FALSE;

// pokebw2's NWM_Init
NWMRetCode func_02702434(void *systemBuffer, u32 size, u8 dmaNo) {
    if (!func_027026b4()) {
        return NWM_RETCODE_UNAVAILABLE;
    }
    return func_02702468(systemBuffer, size, dmaNo);
}

static NWMRetCode func_02702468(NWMSys *sys, u32 size, u32 dmaNo) {
    u32 irq;
    int i;

    irq = OS_DisableInterrupts();
    if (WM_CheckInitialized() == 0) {
        OS_RestoreInterrupts(irq);
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    if (data_027341a0) {
        OS_RestoreInterrupts(irq);
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    if (sys == NULL) {
        OS_RestoreInterrupts(irq);
        return NWM_RETCODE_INVALID_PARAM;
    }
    if (dmaNo > 3) {
        OS_RestoreInterrupts(irq);
        return NWM_RETCODE_INVALID_PARAM;
    }
    if (((u32)sys & 0x1f) != 0) {
        OS_RestoreInterrupts(irq);
        return NWM_RETCODE_INVALID_PARAM;
    }
    PXI_Init();
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_NWM, PXI_PROC_ARM7)) {
        OS_RestoreInterrupts(irq);
        return NWM_RETCODE_UNAVAILABLE;
    }
    DC_InvalidateRange(sys, size);
    MIi_DmaFill32(dmaNo, sys, 0, size, TRUE);
    data_027341a4 = sys;
    DC_StoreRange(&data_027341a4, 4);
    data_027341a4->info = (NWMInfo *)(data_027341a4 + 1);
    data_027341a4->status = (NWMStatus *)(data_027341a4->info + 1);
    data_027341a4->event = (NWMEvent *)(data_027341a4->status + 1);
    func_027026ec();
    data_027341a4->dmaNo = dmaNo;
    OS_InitMessageQueue(&data_027341a4->freeQueue, data_027341a4->freeMsgs, NWM_CMD_BUF_COUNT);
    for (i = 0; i < NWM_CMD_BUF_COUNT; i++) {
        data_027341a4->cmdBuf[i].cmd = NWM_CMD_BUF_FREE;
        DC_StoreRange(&data_027341a4->cmdBuf[i], 2);
        OS_SendMessage(&data_027341a4->freeQueue, &data_027341a4->cmdBuf[i], OS_MESSAGE_BLOCK);
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_NWM, func_02702740);
    MI_CpuFill8(data_027341a4->unk_2120, 0, sizeof(data_027341a4->unk_2120));
    data_027341a0 = TRUE;
    data_027341a4->status->state = NWM_STATE_INITIALIZED;
    DC_StoreRange(data_027341a4, sizeof(NWMSys));
    DC_StoreRange(&data_027341a0, 1);
    OS_RestoreInterrupts(irq);
    return NWM_RETCODE_SUCCESS;
}

NWMRetCode NWMi_CheckInitialized(void) {
    return data_027341a0 ? NWM_RETCODE_SUCCESS : NWM_RETCODE_ILLEGAL_STATE;
}

// Whether the wireless is available and not turned off
BOOL func_027026b4(void) {
    if (OS_IsAvailableWireless() == TRUE && !OS_IsForceDisableWireless()) {
        return TRUE;
    }
    return FALSE;
}
