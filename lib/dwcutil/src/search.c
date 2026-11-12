// libdwcutil's search.c (Black's ov016 0x021cef20-0x021cf3b0): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

typedef struct SearchData {
    u8 wmBuf[0xF00];
    u8 scanBuf[0x400];
    u8 unk_1300[0x348];
    WMScanExParam scanParam;
    WMStatus status;
    u8 unk_1e5c;
} SearchData;

static SearchData *MemSearch;
WMErrCode WM_ReadStatus(WMStatus *statusBuf);
typedef struct SearchWork {
    s16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    u16 unk_08;
} SearchWork;

void callback_search(SearchWork *param_1);
s32 startScan_search(void);
void renewInfo(WMStartScanExCallback *prm);

typedef struct APInfo {
    u8 ssid[32];
    u8 bssid[6];
    u16 level;
    u8 wep;
} APInfo;

typedef struct OtherElements {
    u8 count;
    u8 rsv[3];
    struct {
        u8 id;
        u8 length;
        u8 rsv[2];
        u8 *body;
    } element[16];
} OtherElements;
OtherElements func_0208f05c(WMBssDesc *desc);

// The names of the file's data are the prebuilt library's (its DWARF)
static const u8 WPA_BODY[4] = { 0x00, 0x50, 0xf2, 0x01 };
static const u8 ESSID_EMPTY[6] = { 0, 0, 0, 0, 0, 0 };
static const WMScanExParam SCAN_PRM = { NULL, 0x400, 0x3fff, 0, { 0xffff, 0xffff, 0xffff } };

void DWCi_SEARCHlInit(void) {
    if (MemSearch != NULL) {
        return;
    }
    MemSearch = DWCi_HEAPlAllocEx(0x1e60, 0x20);
}

#pragma thumb off
void DWCi_SEARCHlEnd(void) {
    if (MemSearch == NULL) {
        return;
    }
    while (WM_GetAllowedChannel() != 0x8000) {
    }
    DWCi_HEAPlFree_((u32 *)&MemSearch);
}
#pragma thumb reset


BOOL DWCi_SEARCHlBegin(void) {
    MIi_CpuClear16(0, MemSearch->unk_1300, sizeof(MemSearch->unk_1300));
    if (WM_Initialize(MemSearch->wmBuf, (WMCallbackFunc)callback_search, 3) != WM_ERRCODE_OPERATING) {
        return FALSE;
    }
    do {
        WM_ReadStatus(&MemSearch->status);
    } while (MemSearch->status.state != 2);
    MemSearch->scanParam = SCAN_PRM;
    MemSearch->scanParam.scanBuf = (WMBssDesc *)MemSearch->scanBuf;
    MemSearch->scanParam.maxChannelTime = WM_GetDispersionScanPeriod();
    if (startScan_search()) {
        return TRUE;
    }
    return FALSE;
}

s32 startScan_search(void) {
    if (WM_StartScanEx((void (*)(void *)) callback_search, (const WMScanExParam *)((u8 *)MemSearch + 0x1648)) == 2) {
        return 1;
    }
    return 0;
}

BOOL DWCi_SEARCHlStop(void) {
    *(s8 *)((u8 *)(u8 *)MemSearch + 0x1E5C) = 1;
    WM_ReadStatus((WMStatus *)((u8 *)MemSearch + 0x168C));
    if (*(u16 *)((u8 *)(u8 *)MemSearch + 0x168C) != 2) {
        if (WM_Reset((void (*)(void *)) callback_search) != 2) {
            return 0;
        }
        do {
            WM_ReadStatus((WMStatus *)((u8 *)MemSearch + 0x168C));
        } while (*(u16 *)((u8 *)(u8 *)MemSearch + 0x168C) != 2);
        goto block_4;
    }
block_4:
    if (WM_End((void (*)(void *)) callback_search) != 2) {
        return 0;
    }
    return 1;
}

s32 func_ov016_021cf114(s32 *param_1) {
    s32 result;
    s32 i;
    s32 j;
    s32 val;

    i = 0;
    val = (s32)MemSearch->unk_1300;
    j = 0;
    *param_1 = val;
    do {
        result = memcmp((void *)(j * 0x2a + val + 0x20), ESSID_EMPTY, 6);
        j = j + 1;
        if (result != 0) {
            i = i + 1;
        }
    } while (j < 0x14);
    return i;
}

void callback_search(SearchWork *param_1) {
    if (param_1->unk_02 == 0 && *(u8 *)((u8 *)((u8 *)MemSearch) + 0x1E5C) == 0 &&
            (u16)(param_1->unk_00) == 0x26) {
        switch (param_1->unk_08) {
        case 5:
            renewInfo((WMStartScanExCallback *)param_1);
            startScan_search();
            break;
        case 4:
            startScan_search();
            break;
        default:
            OS_Terminate();
            break;
        }
    }
}

void renewInfo(WMStartScanExCallback *prm) {
    APInfo *info = (APInfo *)MemSearch->unk_1300;
    WMBssDesc *desc;
    OtherElements elems;
    s32 i;
    s32 j;
    s32 k;

    DC_InvalidateRange(MemSearch->scanBuf, sizeof(MemSearch->scanBuf));
    for (i = 0; i < prm->bssDescCount; i++) {
        s32 rs;
        desc = prm->bssDesc[i];
        if (desc->ssid[0] == 0 || desc->gameInfoLength != 0) {
            continue;
        }
        for (j = 0; j < 20; j++) {
            if (memcmp(desc->bssid, info[j].bssid, 6) == 0) {
                break;
            }
        }
        if (j == 20) {
            for (j = 0; j < 20; j++) {
                if (memcmp(info[j].bssid, ESSID_EMPTY, 6) == 0) {
                    break;
                }
            }
            if (j == 20) {
                return;
            }
        }
        MI_CpuCopy8(desc->bssid, info[j].bssid, 6);
        MI_CpuCopy8(desc->ssid, info[j].ssid, 32);
        info[j].level = prm->linkLevel[i];
        if ((desc->capaInfo & 0x10) == 0) {
            info[j].wep = 0;
            continue;
        }
        info[j].wep = 1;
        elems = func_0208f05c(desc);
        rs = elems.count;
        for (k = 0; k < rs; k++) {
            if (elems.element[k].id == 0x30) {
                info[j].wep = 2;
                break;
            }
            if (elems.element[k].id == 0xdd && elems.element[k].length >= 4
                && memcmp(elems.element[k].body, WPA_BODY, 4) == 0) {
                info[j].wep = 2;
                break;
            }
        }
    }
}

