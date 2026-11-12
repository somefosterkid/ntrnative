// TwlSDK's OS library, the DTCM part of os_irqTable.c: Black's dtcm .data 0x02fe0020-0x02fe00a0. The SDK puts the
// table between nitro/dtcm_begin.h and dtcm_end.h; the rest of the file is in ARM9 main (os_irqTable.c). The table is
// libntr's (MIT, https://github.com/cybervisi0n/libntr) with TwlSDK's 32 slots: the DSi's new DMA channels in 28-31.
#include "nitro/os.h"

// The handler of each of the 32 interrupts: the timers' in slots 3-6, the DMA channels' in 8-11, the new DMA
// channels' in 28-31 and OS_IrqDummy in the others, which OS_SetIrqFunction replaces
OSIrqFunction OS_IRQTable[32] = {
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OSi_IrqTimer0,
    OSi_IrqTimer1,
    OSi_IrqTimer2,
    OSi_IrqTimer3,
    OS_IrqDummy,
    OSi_IrqDma0,
    OSi_IrqDma1,
    OSi_IrqDma2,
    OSi_IrqDma3,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OS_IrqDummy,
    OSi_IrqNDma0,
    OSi_IrqNDma1,
    OSi_IrqNDma2,
    OSi_IrqNDma3,
};
