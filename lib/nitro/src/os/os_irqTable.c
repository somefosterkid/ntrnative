// TwlSDK's OS library, os_irqTable.c: Black's main 0x02084610-0x0208475c. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's.

#include "types.h"
#include "nitro/os.h"

void OSi_IrqCallback(s32 param_1);
typedef struct OSIrqCallbackInfo {
    void (*func)(void *arg);
    u32 enable;
    void *arg;
} OSIrqCallbackInfo;

// The callbacks of the DMA channels 0-3, the DSi's new DMA channels 0-3 and the timers 0-3 (OSi_IrqTimer0-3 run 8-11)
OSIrqCallbackInfo OSi_IrqCallbackInfo[12];
// Each callback's interrupt bit
static u16 OSi_IrqCallbackInfoIndex[12] = { 8, 9, 10, 11, 28, 29, 30, 31, 3, 4, 5, 6 };

void OS_IrqDummy(void) {
}

void OSi_IrqCallback(s32 param_1) {
    u32 imask;
    void (*callback)(void *arg);

    imask = 1 << OSi_IrqCallbackInfoIndex[param_1];
    callback = OSi_IrqCallbackInfo[param_1].func;
    OSi_IrqCallbackInfo[param_1].func = NULL;
    if (callback != NULL) {
        callback(OSi_IrqCallbackInfo[param_1].arg);
    }
    OS_SetIrqCheckFlag(imask);
    if (OSi_IrqCallbackInfo[param_1].enable == 0) {
        OS_DisableIrqMask(imask);
    }
}

void OSi_IrqDma0(void) {
    OSi_IrqCallback(0);
}

void OSi_IrqDma1(void) {
    OSi_IrqCallback(1);
}

void OSi_IrqDma2(void) {
    OSi_IrqCallback(2);
}

void OSi_IrqDma3(void) {
    OSi_IrqCallback(3);
}

void OSi_IrqTimer0(void) {
    OSi_IrqCallback(8);
}

void OSi_IrqTimer1(void) {
    OSi_IrqCallback(9);
}

void OSi_IrqTimer2(void) {
    OSi_IrqCallback(10);
}

void OSi_IrqTimer3(void) {
    OSi_IrqCallback(0xb);
}

void OSi_IrqNDma0(void) {
    OSi_IrqCallback(4);
}

void OSi_IrqNDma1(void) {
    OSi_IrqCallback(5);
}

void OSi_IrqNDma2(void) {
    OSi_IrqCallback(6);
}

void OSi_IrqNDma3(void) {
    OSi_IrqCallback(7);
}
