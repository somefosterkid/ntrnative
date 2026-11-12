// TwlSDK's OS library, os_irqHandler.c: Black's main 0x0208459c-0x02084610. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's.

#include "types.h"
#include "nitro/os.h"

void OS_WaitIrq(BOOL clear, u32 irqFlags) {
    u32 enabled = OS_DisableInterrupts();

    if (clear) {
        *(vu32 *)HW_INTR_CHECK_BUF &= ~irqFlags;
    }

    OS_RestoreInterrupts(enabled);

    while ((irqFlags & *(vu32 *)HW_INTR_CHECK_BUF) == 0) {
        OS_SleepThread(&OSi_IrqThreadQueue);
    }
}
