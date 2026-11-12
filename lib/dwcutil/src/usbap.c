// libdwcutil's usbap.c (Black's ov016 0x021d0b44-0x021d1208): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include <string.h>

#include "dwcutil.h"

#include "system/wm_sync.h"

// The work of the USB AP search: the scan's buffers, the access points it found, the callback and the scan's state
typedef struct {
    u8 mac[6];
    u8 flag;
} UsbApEntry;

typedef struct {
    u16 state;
    u8 unk_02[0x7ce];
} UsbApWmStatus;

typedef void (*UsbApCallback)(s32 result);

typedef struct {
    u8 wmBuf[0xf00] ATTRIBUTE_ALIGN(32);
    u8 scanBuf[0x400] ATTRIBUTE_ALIGN(32);
    UsbApEntry aps[16];
    UsbApCallback callback;
    WMScanExParam scanParam;
    UsbApWmStatus status;
    OSTick startTick;
    s32 *task;
    volatile u8 state;  /* set by the WM callback; the wait loop re-reads it */
    u8 found;
    u8 scanning;
} UsbApWork;

static const u8 data_ov016_021da948[6] = { 0, 0, 0, 0, 0, 0 };
static const u8 data_ov016_021da94e[10] = "NWCUSBAP";
static const WMScanExParam data_ov016_021da958 = {
    NULL, 0x400, 0x3fff, 0, { 0xffff, 0xffff, 0xffff }, 2, 0x20, { 0 }, 8, { 0 }
};

static UsbApWork *data_ov016_021de654;
void cbWm(WMStartScanExCallback *param_1);
void func_ov016_021d0fcc(WMStartScanExCallback *param_1);
void func_ov016_021d0e74(WMStartScanExCallback *param_1);
void taskFirst(s32 param_1);
s32 startScan_usbap(void);

u32 DWCi_USBAPlBegin(u32 param_1) {
    OSOwnerInfo info;
    OSTick tick;

    data_ov016_021de654 = DWCi_HEAPlAllocEx(sizeof(UsbApWork), 0x20);
    data_ov016_021de654->callback = (UsbApCallback) param_1;
    tick = OS_GetTick();
    data_ov016_021de654->startTick = tick;
    if (WM_Initialize(data_ov016_021de654->wmBuf, (WMCallbackFunc) cbWm, 3) == 2) {
        do {
            WM_ReadStatus((WMStatus *) &data_ov016_021de654->status);
        } while (data_ov016_021de654->status.state != 2);
        data_ov016_021de654->scanParam = data_ov016_021da958;
        data_ov016_021de654->scanParam.scanBuf = (WMBssDesc *) data_ov016_021de654->scanBuf;
        data_ov016_021de654->scanParam.maxChannelTime = WM_GetDispersionScanPeriod();
        OS_GetOwnerInfo(&info);
        MI_CpuCopy8(data_ov016_021da94e, &data_ov016_021de654->scanParam.ssid[0], 8);
        data_ov016_021de654->scanParam.ssid[9] = 1;
        MI_CpuCopy8(info.nickName, &data_ov016_021de654->scanParam.ssid[12], info.nickNameLength * 2);
        if (startScan_usbap() != 0) {
            data_ov016_021de654->task = DWCi_TSKlForm(0, (s32) taskFirst, 0, 0x78);
            return 1;
        }
    }
    DWCi_HEAPlFree_((u32 *) &data_ov016_021de654);
    return 0;
}

s32 startScan_usbap(void) {
    if (WM_StartScanEx((WMCallbackFunc) cbWm, &data_ov016_021de654->scanParam) == 2) {
        return 1;
    }
    return 0;
}

u32 DWCi_USBAPlEnd(void) {
    data_ov016_021de654->state = 1;
    WM_ReadStatus((WMStatus *) &data_ov016_021de654->status);
    if (data_ov016_021de654->status.state != 2) {
        if (WM_Reset((WMCallbackFunc) cbWm) != 2) {
            return 0;
        }
        do {
            WM_ReadStatus((WMStatus *) &data_ov016_021de654->status);
        } while (data_ov016_021de654->status.state != 2);
    }
    if (WM_End((WMCallbackFunc) cbWm) != 2) {
        return 0;
    }
    if (data_ov016_021de654->task != NULL) {
        DWCi_TSKlDeleteEx(0, data_ov016_021de654->task);
    }
    while (data_ov016_021de654->state != 2) {
    }
    DWCi_HEAPlFree_((u32 *) &data_ov016_021de654);
    return 1;
}

void DWCi_USBAPlSetCallback(u32 callback) {
    data_ov016_021de654->callback = (UsbApCallback) callback;
}

void cbWm(WMStartScanExCallback *param_1) {
    if (param_1->errcode != 0) {
        return;
    }
    if (data_ov016_021de654->state != 0) {
        if (param_1->apiid == 2) {
            data_ov016_021de654->state = 2;
        }
        return;
    }
    if (param_1->apiid != 0x26) {
        return;
    }
    switch (param_1->state) {
    case 5:
        if (data_ov016_021de654->scanning != 0) {
            func_ov016_021d0fcc(param_1);
        } else {
            func_ov016_021d0e74(param_1);
        }
        startScan_usbap();
        break;
    case 4:
        startScan_usbap();
        break;
    default:
        OS_Terminate();
        break;
    }
}

void func_ov016_021d0e74(WMStartScanExCallback *param_1) {
    s32 i;
    s32 j;
    WMBssDesc *desc;
    UsbApWork *work;

    for (i = 0; i < param_1->bssDescCount; i++) {
        desc = param_1->bssDesc[i];
        DC_InvalidateRange(desc, 0xc0);
        if (memcmp(desc->ssid, data_ov016_021da94e, 8) == 0) {
            work = data_ov016_021de654;
            for (j = 0; j < 16; j++) {
                if (memcmp(desc->bssid, work->aps[j].mac, 6) == 0) {
                    if (work->aps[j].flag == 0 && (desc->ssid[9] & 1) != 0) {
                        if (work->callback == NULL) {
                            return;
                        }
                        work->callback(1);
                        return;
                    }
                    goto next;
                }
            }
            for (j = 0; j < 16; j++) {
                if (memcmp(data_ov016_021de654->aps[j].mac, data_ov016_021da948, 6) == 0) {
                    MI_CpuCopy8(desc->bssid, work->aps[j].mac, 6);
                    data_ov016_021de654->aps[j].flag = (desc->ssid[9] & 1) != 0;
                    break;
                }
            }
        }
    next:;
    }
}

void func_ov016_021d0fcc(WMStartScanExCallback *prm) {
    WMBssDesc *info;
    s32 i;
    s32 j;

    if (data_ov016_021de654->found) {
        if (data_ov016_021de654->callback != NULL) {
            data_ov016_021de654->callback(data_ov016_021de654->found);
        }
        return;
    }
    DC_InvalidateRange(data_ov016_021de654->scanBuf, 0x400);
    for (i = 0; i < prm->bssDescCount; i++) {
        info = prm->bssDesc[i];
        if (memcmp(info->ssid, data_ov016_021da94e, 8) == 0 && (info->ssid[9] & 1)) {
            for (j = 0; j < 16; j++) {
                if (memcmp(info->bssid, data_ov016_021de654->aps[j].mac, 6) == 0) {
                    if (data_ov016_021de654->aps[j].flag) {
                        break;
                    }
                    if (data_ov016_021de654->callback != NULL) {
                        data_ov016_021de654->callback(1);
                    } else {
                        data_ov016_021de654->found = 1;
                    }
                    return;
                }
            }
        }
    }
}

void taskFirst(s32 task) {
    s32 i;
    BOOL flagged;
    BOOL unflagged;
    UsbApWork *work;
    OSTick now;

    now = OS_GetTick();
    work = data_ov016_021de654;
    if (now < work->startTick + 0x17f898) {
        return;
    }
    unflagged = FALSE;
    flagged = FALSE;
    for (i = 0; i < 16; i++) {
        if (memcmp(work->aps[i].mac, data_ov016_021da948, 6) != 0) {
            if (work->aps[i].flag != 0) {
                flagged = TRUE;
            } else {
                unflagged = TRUE;
            }
        }
    }
    if (flagged && unflagged) {
        if (work->callback != NULL) {
            work->callback(2);
        }
    } else if (flagged) {
        if (work->callback != NULL) {
            work->callback(1);
        }
    } else if (!unflagged) {
        if (work->callback != NULL) {
            work->callback(0);
        }
    }
    data_ov016_021de654->task = NULL;
    data_ov016_021de654->scanning = 1;
    DWCi_TSKlDelete(0, task);
}
