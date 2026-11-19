#include "global.h"

#include "overlay_72_wfc_state.h"
#include "system.h"
#include "unk_02037C94.h"
#include "overlay_72_wfc_connect.h"

SysTask *ov72_0223A414(void *);

void ov72_022391E8(NintendoWfcConnect *arg0);

void ov72_022391E8(NintendoWfcConnect *arg0) {
    sub_02039418(arg0->args->saveData);
    Sys_SetSleepDisableFlag(4);
    ov72_0223A414(arg0);
    DWC_InitInetEx(&arg0->inetControl, 2U, 1U, 0x14U);
    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
    DWC_ConnectInetAsync();
    arg0->state = NINTENDO_WFC_STATE_WAIT_CONNECT;
}
