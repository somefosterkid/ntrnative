// TwlSDK's os_pxi.c (as libntr has it, MIT, https://github.com/cybervisi0n/libntr, libraries/os/src/os_pxi.c): Black's
// main 0x02084a28-0x02084ab0, the words of the OS tag of the PXI FIFO. Black's is the ARM9 build for TWL (SDK_TWL): the
// callback takes the ARM7's terminate command too; the final ROM's OS_TPanic is OS_Terminate. SDK_TWL's
// OSi_IsTerminatePxiOccurred and OSi_SetTerminatePxiOccurred, which Black lacks, are left out. The file was pokebw2's
// os_reset_pxi.c (its guess) and held os_interrupt.c's OS_SetIrqStackChecker before and os_spinLock.c's
// OSi_SyncWithOtherProc and OS_InitLock after.

#include "types.h"
#include "nitro/pxi.h"
#include "nitro/os.h"

static vu16 OSi_IsResetOccurred = FALSE;
static vu16 OSi_IsTerminateOccurred = FALSE;

u16 OS_IsResetOccurred(void) {
    return OSi_IsResetOccurred;
}

void OSi_CommonCallback(int fifoTag, uPtr data, BOOL err) {
    u16 command = (u16)((data & OS_PXI_COMMAND_MASK) >> OS_PXI_COMMAND_SHIFT);

    if (command == OS_PXI_COMMAND_RESET) {
        OSi_IsResetOccurred = TRUE;
    } else if (command == OS_PXI_COMMAND_TERMINATE) {
        OSi_IsTerminateOccurred = TRUE;
        OS_Terminate();
    } else {
        OS_Terminate();
    }
}

void OSi_SendToPxi(s32 arg) {
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_OS, arg << OS_PXI_COMMAND_SHIFT, FALSE) != 0) {
    }
}
