// TwlSDK's OS library, the DTCM part of os_irqHandler.c: Black's dtcm .bss 0x02fe00a0-0x02fe00c0 (the queue's 8
// bytes, then the module's padding to 32). The SDK puts the queue between nitro/dtcm_begin.h and dtcm_end.h; the rest
// of the file is OS_WaitIrq in ARM9 main (os_irqHandler.c) and the two asm handlers in ITCM (os_irqHandler_itcm.c).
#include "nitro/os.h"

// The threads that sleep in OS_WaitIrq until an interrupt wakes them (OS_IrqHandler_ThreadSwitch)
OSThreadQueue OSi_IrqThreadQueue = { NULL, NULL };
