// libdwcutil's rakuraku.c (Black's ov016 0x021c1488-0x021c1630): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

s32 ATERMi_ApConfigStart(u32 param_1, s32 param_2, u32 param_3, u8 *param_4, u32 param_5, u32 param_6);
void cbConfig(u32 *param_1);
void memAlloc(u32 param_1);
void memFree(void *param_1);
static void *data_ov016_021de594;
void DWCi_SETTINGlSetRaku(void *param_1);

typedef struct { u32 b[3]; } Copycx4;

void DWCi_RAKUlSetResult(void);

void DWCi_RAKUlInit(void) {
    data_ov016_021de594 = DWCi_HEAPlAllocEx(0xc, -4);
    if (ATERMi_ApConfigStart(0xf, 0x40, (u32)cbConfig, (u8 *)memAlloc, (u32)memFree,
                                                            0x800) != 1) {
        OS_Terminate();
    }
    OS_Sleep(10);
}

void DWCi_RAKUlEnd(void) {
    if (ATERMi_ApConfigEnd() != 1) {
        OS_Terminate();
    }
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de594);
}

typedef struct _ATERMApConfigParam {
    u8 ssid[32];
    s32 auth_mode;
    s32 wepkeyid;
    u8 wepkey[4][32];
    u8 wpakey[64];
} ATERMApConfigParam;
u32 DWCi_RAKUlGetState(void) {
    ATERMApConfigParam prm;
    s32 st;
    s32 rs;

    switch (*(s32 *)data_ov016_021de594) {
    case 0:
    case 1:
    case 3:
    case 5:
        st = 0;
        break;
    case 2:
        st = 1;
        break;
    case 4:
        st = 2;
        break;
    case 6:
        rs = ATERMi_ApConfigGetResult(&prm);
        if (rs != 1) {
            OS_Terminate();
        }
        if (prm.auth_mode >= 0 && prm.auth_mode <= 3 && prm.wepkeyid == 1) {
            st = 3;
        } else {
            st = 5;
        }
        break;
    case 7:
        st = 4;
        break;
    }
    return st;
}

void DWCi_RAKUlSetResult(void) {
    u8 auStack_f0 [232];

    if (ATERMi_ApConfigGetResult(auStack_f0) != 1) {
        OS_Terminate();
    }
    DWCi_SETTINGlSetRaku(auStack_f0);
}

void cbConfig(u32 *arg) {
    *(Copycx4 *)((u8 *)data_ov016_021de594) = *(Copycx4 *)((u8 *)arg);
}

void memAlloc(u32 size) {
    DWCi_HEAPlAllocEx(size, 0x20);
}

void memFree(void *ptr) {
    DWCi_HEAPlFree2(ptr);
}
