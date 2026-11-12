// Black's sciInterface.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.

#include "dwc/webservices_internal.h"

// The URL of the competition service, set by func_ov175_021df734 or once it is asked for
char scServiceURL[128];

void func_ov175_021df734(s32 param_1) {
    if ((param_1 != 0) && (param_1 == 1)) {
        OS_SNPrintf(scServiceURL, 0x80, "https://%s.secure2.atlas.gs.nintendowifi.net/CompetitionService/CompetitionService.asmx", (void *)&data_ov009_021ae01c);
        func_ov114_021bf520((u32 *)scServiceURL);
        OS_SNPrintf(wsAuthServiceURL, 0x80, "https://%s.secure2.auth.gs.nintendowifi.net/AuthService/AuthService.asmx", (void *)&data_ov009_021ae01c);
        func_ov114_021bf520((u32 *)wsAuthServiceURL);
        return;
    }
    OS_SNPrintf(scServiceURL, 0x80, "https://%s.secure.atlas.gs.nintendowifi.net/CompetitionService/CompetitionService.asmx", (void *)&data_ov009_021ae01c);
    func_ov114_021bf520((u32 *)scServiceURL);
    OS_SNPrintf(wsAuthServiceURL, 0x80, "https://%s.secure.auth.gs.nintendowifi.net/AuthService/AuthService.asmx", (void *)&data_ov009_021ae01c);
    func_ov114_021bf520((u32 *)wsAuthServiceURL);
}

u32 func_ov175_021df7fc(void **param_1) {
    void *dest;

    if (data_ov009_021acef8 == 1) {
        if (scServiceURL[0] == '\0') {
            OS_SNPrintf(scServiceURL, 0x80, "http://%s.comp.pubsvs.gs.nintendowifi.net/CompetitionService/CompetitionService.asmx", &data_ov009_021acefc);
        }
    } else {
        return 1;
    }
    dest = gsimalloc(0xac);
    *param_1 = dest;
    if (dest == NULL) {
        return 5;
    }
    memset(dest, 0, 0xac);
    return 0;
}

u32 func_ov175_021df884(Ov175_021dd1ecWork2 *param_1) {
    u32 result;

    result = func_ov175_021e06dc(param_1, param_1);
    if (result != 0) {
        return result;
    }
    memset((void *)param_1->unk_5c, 0, 0x28);
    memset((void *)&param_1->unk_84, 0, 0x28);
    param_1->unk_50 = 1;
    return 0;
}

void func_ov175_021df8d4(u32 *param_1) {
    func_ov175_021e0730(param_1);
    memset(param_1 + 0x17, 0, 0x28);
    memset(param_1 + 0x21, 0, 0x28);
    *(u8 *)(param_1 + 0x17) = 0xde;
    *(u8 *)((s32)param_1 + 0x5d) = 0xad;
    *(u8 *)(param_1 + 0x21) = 0xde;
    *(u8 *)((s32)param_1 + 0x85) = 0xad;
    param_1[0x14] = 0;
}

void func_ov175_021df928(s32 param_1, s8 *param_2) {
    if (param_2 != (s8 *)0x0) {
        strcpy((s8 *)(param_1 + 0x5c), param_2);
        return;
    }
    *(u8 *)(param_1 + 0x5c) = 0;
}

void func_ov175_021df948(s32 param_1, s8 *param_2) {
    if (param_2 != (s8 *)0x0) {
        strcpy((s8 *)(param_1 + 0x84), param_2);
        return;
    }
    *(u8 *)(param_1 + 0x84) = 0;
}

s8 *func_ov175_021df968(void) {
    if (data_ov009_021acef8 == 1) {
        if (scServiceURL[0] == '\0') {
            OS_SNPrintf(scServiceURL, 0x80, "http://%s.comp.pubsvs.gs.nintendowifi.net/CompetitionService/CompetitionService.asmx", &data_ov009_021acefc);
        }
        return scServiceURL;
    }
    gsDebugFormat(0, 3, 1, "No availability\r\n");
    return NULL;
}
