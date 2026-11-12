// TwlSDK's OS library, os_terminate_proc.c: Black's main 0x02088234-0x020882dc. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's.

#include "types.h"
#include "nitro/os.h"

static u32 data_02151398[3];
BOOL func_02704318(void);
BOOL func_02704328(void);
void OSi_SendToPxi(s32);
void OSi_TerminateCore(void);

void OS_Terminate(void)
{
    void (*callback)(void *) = (void (*)(void *))data_02151398[1];
    if (callback != NULL) {
        data_02151398[1] = 0;
        callback((void *)data_02151398[0]);
    }
    if (data_02151398[2] == 0) {
        data_02151398[2] = 1;
        if (OS_GetProcMode() != 0x12) {
            (void)reg_OS_IME;
            reg_OS_IME = 1;
            OS_EnableInterrupts();
        }
        if (OS_IsRunOnTwl()) {
            if (func_02704318()) {
                func_02704328();
            }
        }
        OSi_SendToPxi(0x20);
    }
    OSi_TerminateCore();
}

void OSi_TerminateCore(void) {
    OS_DisableInterrupts();
    do {
        OS_Halt();
    } while( TRUE );
}

asm void OS_Halt(void)
{
    mov r0, #0
    mcr p15, 0, r0, c7, c0, 4
    bx lr
}
