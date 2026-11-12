// libdwcutil's libAoss.c (Black's ov016 0x021c07a8-0x021c0970): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

#include "dwc_aoss_ap.h"

typedef struct AossWork {
    u8 started;
    AossParam *param;
} AossWork;

static AossWork data_ov016_021de564;
void DWCi_SETTINGlSetAoss(void *param_1);
static const char data_ov016_021da468[] = "NINTENDO-DS";
void *aossAlloc(u32 size);
void aossFree(void *ptr);

void DWCi_AOSSlInit(void) {
    AossClientInfo info;

    data_ov016_021de564.param = DWCi_HEAPlAllocEx(0x26C, 4);
    data_ov016_021de564.started = 0;
    MIi_CpuClear16(0, &info, sizeof(info));
    info.version = 0x50;
    info.length = 0xC;
    MI_CpuCopy8(&data_ov016_021da468, info.data, 0xC);
    data_ov016_021de564.param->types = 3;
    data_ov016_021de564.param->client = info;
    data_ov016_021de564.param->scanCount = 1;
    data_ov016_021de564.param->scanDelay = -1;
    data_ov016_021de564.param->sendCount = 1;
    data_ov016_021de564.param->sendDelay = -1;
    data_ov016_021de564.param->timeout = -1;
    OS_GetMacAddress(data_ov016_021de564.param->mac);
    if (AOSSi_InitLocal(aossAlloc, aossFree) != 0) {
        OS_Terminate();
    }
}

void DWCi_AOSSlEnd(s32 save) {
    AOSSi_EndLocal();
    if (save != 0) {
        if ((*(u8 *)((u8 *)(*(void **)((u8 *)(((u8 *)&data_ov016_021de564)) + 4)) + 0x116) == 0) && (*(u8 *)(((u8 *)&data_ov016_021de564)) == 1)) {
            DWCi_SETTINGlSetAoss(*(void **)((u8 *)(((u8 *)&data_ov016_021de564)) + 4) + 0x117);
        }
    }
    DWCi_HEAPlFree_((u8 *)&data_ov016_021de564.param);
}

s32 DWCi_AOSSlStart(void) {
    if (AOSSi_Init(data_ov016_021de564.param) == 0) {
        data_ov016_021de564.started = 1;
        return 1;
    }
    if (data_ov016_021de564.param->error == 1 || data_ov016_021de564.param->error == 3 || data_ov016_021de564.param->error == 4 || data_ov016_021de564.param->error == 5) {
        return 0;
    }
    switch (data_ov016_021de564.param->error) {
    default:
        return 2;
    }
}


void *aossAlloc(u32 size) {
    return DWCi_HEAPlAllocEx(size, 0x20);
}

void aossFree(void *ptr) {
    DWCi_HEAPlFree2(ptr);
}
