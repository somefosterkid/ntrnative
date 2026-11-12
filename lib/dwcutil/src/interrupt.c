// libdwcutil's interrupt.c (Black's ov016 0x021d6c78-0x021d6d44): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void intrVBlank(void);

static OSIrqMask data_ov016_021de6a4;
static OSIrqFunction data_ov016_021de6a0;

#pragma thumb off
void func_ov016_021d6c78(void) {
    data_ov016_021de6a4 = reg_OS_IE;
    (void)OS_SetIrqMask(OS_IE_TIMER0 | OS_IE_TIMER1 | OS_IE_FIFO_RECV);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    data_ov016_021de6a0 = OS_GetIrqFunction(OS_IE_V_BLANK);
    OS_SetIrqFunction(OS_IE_V_BLANK, intrVBlank);
    (void)OS_ResetRequestIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
}

void func_ov016_021d6ce4(void) {
    (void)OS_DisableIrq();
    (void)OS_SetIrqMask(data_ov016_021de6a4);
    OS_SetIrqFunction(OS_IE_V_BLANK, data_ov016_021de6a0);
}

#pragma thumb reset

void intrVBlank(void) {
    DWCi_TSKlAct(1);
    *(s32 *)((u8 *)SDK_AUTOLOAD_DTCM_START + 0x3FF8) = (s32) (*(s32 *)((u8 *)SDK_AUTOLOAD_DTCM_START + 0x3FF8) | 1);
}
