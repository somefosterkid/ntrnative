// TwlSDK's OS library, os_event.c: Black's main 0x020882dc-0x020883d4. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's.

#include "types.h"
#include "nitro/os.h"

void OS_InitEvent(OSEvent *event) {
    (event->queue).tail = (OSThread *)0x0;
    (event->queue).head = (OSThread *)0x0;
    event->flag = 0;
}

u32 OS_WaitEventEx(OSEvent *event, u32 pattern, u32 mode, u32 clearBit)
{
    u32 result = 0;
    u32 enabled;

    enabled = OS_DisableInterrupts();

    switch (mode) {
    case 0:
        while ((event->flag & pattern) != pattern) {
            OS_SleepThread(&event->queue);
        }
        result = event->flag;
        break;
    case 1:
        while (!(event->flag & pattern)) {
            OS_SleepThread(&event->queue);
        }
        result = event->flag;
        break;
    }

    if (result) {
        event->flag &= ~clearBit;
    }

    OS_RestoreInterrupts(enabled);
    return result;
}

void OS_SignalEvent(OSEvent *event, u32 setPattern) {
    u32 mask;

    mask = OS_DisableInterrupts();
    if (setPattern != 0) {
        event->flag = event->flag | setPattern;
        OS_WakeupThread(&event->queue);
    }
    OS_RestoreInterrupts(mask);
}
