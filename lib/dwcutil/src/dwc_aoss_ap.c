// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwcutil/src/dwc_aoss_ap.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "dwc_aoss_ap.h"
#include "dwc/ext.h"
#include "dwc_wcm.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/wm.h"
#include "nitrowifi/wcm.h"

// The access point layer of the AOSS client (see dwc_aoss_ap.h): the scan for the access points, the connection to one
// with a WEP key, and the disconnection, over dwc_wcm.c's functions (pokebw2's DWC_Wcm_*). A message queue turns the
// WCM's events into waits. aosslink.c in NitroDWC's prebuilt libdwcutil.a, whose object is Black's. The file reaches
// its .bss from one base (-ipa file): the definitions below are in the order that gives Black's offsets.

// The messages of the alarm
#define WIFI_ALARM_SCAN 0x13
#define WIFI_ALARM_CONNECT 0x12

static void Set_WLLA_BSS_INFO(const WMBssDesc *bss, WifiApInfo *ap);
static void Get_WLLA_SSID_PARAM(const WifiConnectParam *param, WMBssDesc *bss);
static void Set_WLLA_LINK_STAT(WifiConnectResult *result, const WMBssDesc *bss, int success);
static void aoss_apc_callback(int event, int param);
static void alarmCallback_aosslink(void *arg);

// The WEP settings for the WCM (mode, key ID, four keys of 20 bytes): a u8 array, as the prebuilt's wep_buf (DWARF), which
// MWCC puts in a .bss section of its own, after the file's other statics
static u8 data_ov016_021dcf20[0x60] ATTRIBUTE_ALIGN(32);
static WMBssDesc data_ov016_021dce60 ATTRIBUTE_ALIGN(32);
static OSMessageQueue data_ov016_021dce40;
static OSMessage data_ov016_021dce30[4];
DWCAossFreeFunc data_ov016_021dce2c;          // pokebw2's gAossApFree
DWCAossAllocFunc data_ov016_021dce20;         // pokebw2's gAossAp.alloc
static void (*data_ov016_021dce24)(int mode); // pokebw2's gAossAp.onModeChange
static void *data_ov016_021dce28;             // pokebw2's gAossAp.block

// The WM's description of an access point to ours
// pokebw2's Wifi_ConvertBss
static void Set_WLLA_BSS_INFO(const WMBssDesc *bss, WifiApInfo *ap) {
    static struct {
        u16 mask;
        u8 rate;
    } data_ov016_021dab28[12] = {
        { 0x0001, 0x02 }, { 0x0002, 0x04 }, { 0x0004, 0x0b }, { 0x0008, 0x0c }, { 0x0010, 0x12 }, { 0x0020, 0x16 },
        { 0x0040, 0x18 }, { 0x0080, 0x24 }, { 0x0100, 0x30 }, { 0x0200, 0x48 }, { 0x0400, 0x60 }, { 0x0800, 0x6c },
    };
    int i;
    int count;

    ap->ssidLength = bss->ssidLength;
    MIi_CpuCopy16(bss->ssid, ap->ssid, 0x20);
    ap->channel = bss->channel;
    MIi_CpuCopy16(bss->bssid, ap->bssid, 6);
    count = 0;
    for (i = 0; i < 12; i++) {
        if (bss->rateSet.support & data_ov016_021dab28[i].mask) {
            ap->rates[count] = data_ov016_021dab28[i].rate;
            if (bss->rateSet.basic & data_ov016_021dab28[i].mask) {
                ap->rates[count] |= 0x80;
            }
            count++;
        }
    }
    ap->rateCount = count;
    ap->beaconPeriod = bss->beaconPeriod;
    if ((bss->capaInfo & 3) == 1) {
        ap->type = 1;
    } else if ((bss->capaInfo & 3) == 2) {
        ap->type = 2;
    } else {
        ap->type = 0;
    }
}

// pokebw2's Wifi_MakeBss
static void Get_WLLA_SSID_PARAM(const WifiConnectParam *param, WMBssDesc *bss) {
    MI_CpuClear16(bss->ssid, 0x20);
    bss->ssidLength = param->ssidLength;
    MI_CpuCopy8(param->ssid, bss->ssid, bss->ssidLength);
}

// pokebw2's Wifi_ReportConnect
static void Set_WLLA_LINK_STAT(WifiConnectResult *result, const WMBssDesc *bss, int success) {
    result->success = success;
    Set_WLLA_BSS_INFO(bss, &result->ap);
}

// pokebw2's Wifi_OnEvent
static void aoss_apc_callback(int event, int param) {
    OS_SendMessage(&data_ov016_021dce40, (OSMessage)event, 0);
}

// pokebw2's Wifi_OnAlarm
static void alarmCallback_aosslink(void *arg) {
    OS_SendMessage(&data_ov016_021dce40, arg, 0);
}

// pokebw2's DWC_Aoss_Init
int AOSSi_InitLocal(DWCAossAllocFunc alloc, DWCAossFreeFunc free) {
    BOOL ok = TRUE;
    OSMessage message;
    u32 mask;

    OS_InitMessageQueue(&data_ov016_021dce40, data_ov016_021dce30, 4);
    if (alloc == NULL || free == NULL) {
        return -1;
    }
    mask = OS_DisableInterrupts();
    data_ov016_021dce20 = alloc;
    data_ov016_021dce2c = free;
    OS_RestoreInterrupts(mask);
    data_ov016_021dce28 = data_ov016_021dce20(0x5890);
    if (data_ov016_021dce28 == NULL) {
        return -1;
    }
    if (APC_Init(aoss_apc_callback, data_ov016_021dce28, 0x5890) == 0) {
        ok = FALSE;
    }
    if (ok) {
        do {
            OS_ReceiveMessage(&data_ov016_021dce40, &message, OS_MESSAGE_BLOCK);
            switch ((int)message) {
            case DWC_WCM_EVENT_STARTED:
                return 0;
            case 0:
            case 1:
            case 2:
            case 3:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            default:
                ok = FALSE;
            case 4:
            case 5:
                break;
            }
        } while (ok);
    }
    data_ov016_021dce2c(data_ov016_021dce28);
    return -1;
}

// pokebw2's DWC_Aoss_Shutdown
int AOSSi_EndLocal(void) {
    BOOL ok = TRUE;
    int result = -1;
    OSMessage message;
    u32 mask;

    if (data_ov016_021dce2c == NULL) {
        return result;
    }
    if (APC_End() == 0) {
        return -1;
    }
    do {
        OS_ReceiveMessage(&data_ov016_021dce40, &message, OS_MESSAGE_BLOCK);
        switch ((int)message) {
        case DWC_WCM_EVENT_CLEANED_UP:
            ok = FALSE;
            result = 0;
            data_ov016_021dce2c(data_ov016_021dce28);
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        default:
            ok = FALSE;
        case 4:
        case 5:
            break;
        }
    } while (ok);
    mask = OS_DisableInterrupts();
    data_ov016_021dce20 = NULL;
    data_ov016_021dce2c = NULL;
    OS_RestoreInterrupts(mask);
    return result;
}

// pokebw2's AossAp_Disconnect
int AOSSi_Disconnect(void) {
    OSMessage msg;
    BOOL loop_flag = TRUE;
    int rc = -1;

    if (APC_Disconnect()) {
        while (loop_flag) {
            OS_ReceiveMessage(&data_ov016_021dce40, &msg, OS_MESSAGE_BLOCK);
            switch ((int)msg) {
            case DWC_WCM_EVENT_DISCONNECTED:
                loop_flag = FALSE;
                rc = 0;
                break;
            default:
                loop_flag = FALSE;
                break;
            }
        }
    }
    return rc;
}

// Searches for access points and lists them; 0 if it ended well
int AOSSi_WLANGetBSSList(WifiScanList **list) {
    int result = -1;
    BOOL running = TRUE;
    BOOL started = FALSE;
    int count = 0;
    int waits = 0;
    OSMessage message;
    OSAlarm alarm;
    WMBssDesc *block;
    WMBssDesc *bss;

    if (data_ov016_021dce20 == NULL || data_ov016_021dce2c == NULL) {
        return -1;
    }
    bss = data_ov016_021dce20(0x3000);
    if (bss == NULL) {
        return -1;
    }
    block = bss;
    if (APC_SearchStart(NULL, NULL, 0, 0x30bffe)) {
        OS_CreateAlarm(&alarm);
        OS_SetAlarm(&alarm, 0x3fec42, alarmCallback_aosslink, (void *)WIFI_ALARM_SCAN);
        do {
            OS_ReceiveMessage(&data_ov016_021dce40, &message, OS_MESSAGE_BLOCK);
            switch ((int)message) {
            case WIFI_ALARM_SCAN:
                if (!started) {
                    if (waits != 0) {
                        count = APC_GetAPList(bss, 0x40);
                    }
                    if (APC_SearchEnd()) {
                        started = TRUE;
                    } else {
                        goto cancel;
                    }
                }
                break;
            case 5:
                if (!started) {
                    if (waits < 8) {
                        waits++;
                    } else {
                        count = APC_GetAPList(bss, 0x40);
                        if (APC_SearchEnd()) {
                            started = TRUE;
                        } else {
                            goto cancel;
                        }
                    }
                }
                break;
            case 10:
                running = FALSE;
                result = 0;
                break;
            case 4:
            case 8:
            case 18:
                break;
            case 0:
            case 1:
            case 2:
            case 3:
            case 6:
            case 7:
            case 9:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            default:
                goto cancel;
            }
        } while (running);
        {
            WifiScanList *scanList;
            int i;

            if (count != 0) {
                scanList = data_ov016_021dce20(sizeof(WifiScanList) + (count - 1) * sizeof(WifiApInfo));
                if (scanList != NULL) {
                    goto fill;
                }
                goto cancel;
            } else {
                scanList = data_ov016_021dce20(sizeof(WifiScanList));
                if (scanList == NULL) {
                    goto cancel;
                }
            }
        fill:
            *list = scanList;
            scanList->count = count;
            for (i = 0; i < count; i++) {
                Set_WLLA_BSS_INFO(bss++, &scanList->ap[i]);
            }
        }
    cancel:
        OS_CancelAlarm(&alarm);
        while (OS_ReceiveMessage(&data_ov016_021dce40, &message, 0) == 1) {
        }
    }
    data_ov016_021dce2c(block);
    return result;
}

// Connects to the access point; 0 if it did
int AOSSi_WLANConnect(const WifiConnectParam *param, WifiConnectResult *result) {
    OSMessage msg;
    OSAlarm alarm;
    BOOL loop_flag = TRUE;
    int ret_code = -1;
    int connect_retry_count;
    BOOL connect_lock = FALSE;
    u32 option;
    u8 wep_id = 0;

    if (param->authMode == 0) {
        option = 1 << 19;
    } else if (param->authMode == 1) {
        option = 3 << 18;
    }
    option |= 3 << 16;
    MI_CpuFill8(data_ov016_021dcf20, 0, sizeof(data_ov016_021dcf20));
    if (param->keyLength == 5) {
        data_ov016_021dcf20[0] = 1;
    } else if (param->keyLength == 13) {
        data_ov016_021dcf20[0] = 2;
    } else if (param->keyLength == 16) {
        data_ov016_021dcf20[0] = 3;
    } else {
        return -1;
    }
    data_ov016_021dcf20[1] = wep_id;
    MI_CpuCopy8(param->key, &data_ov016_021dcf20[wep_id * 20 + 2], param->keyLength);
    func_ov009_02165b80();
    if (APC_SearchStart(NULL, param->ssid, param->ssidLength, 0x30bffe)) {
        connect_retry_count = 0;
        OS_CreateAlarm(&alarm);
        OS_SetAlarm(&alarm, 0x3fec42, alarmCallback_aosslink, (void *)WIFI_ALARM_CONNECT);
        while (loop_flag) {
            OS_ReceiveMessage(&data_ov016_021dce40, &msg, OS_MESSAGE_BLOCK);
            switch ((int)msg) {
            case 18:
                if (!connect_lock) {
                    loop_flag = FALSE;
                }
                break;
            case 19:
                (void)0;
                break;
            case 5:
                if (!connect_lock) {
                    OS_CancelAlarm(&alarm);
                    if (APC_GetAPList(&data_ov016_021dce60, 1) != 1) {
                        loop_flag = FALSE;
                        break;
                    }
                    Get_WLLA_SSID_PARAM(param, &data_ov016_021dce60);
                    {
                        int i;

                        for (i = 0; i < data_ov016_021dce60.ssidLength; i++) {
                        }
                    }
                    if (!APC_Connect(&data_ov016_021dce60, data_ov016_021dcf20, option)) {
                        loop_flag = FALSE;
                    } else {
                        connect_lock = TRUE;
                    }
                }
                break;
            case 10:
                Get_WLLA_SSID_PARAM(param, &data_ov016_021dce60);
                if (!APC_Connect(&data_ov016_021dce60, data_ov016_021dcf20, option)) {
                    loop_flag = FALSE;
                }
                break;
            case 12:
                loop_flag = FALSE;
                ret_code = 0;
                break;
            case 13:
                connect_retry_count++;
                if (connect_retry_count < 3) {
                    if (!APC_Connect(&data_ov016_021dce60, data_ov016_021dcf20, option)) {
                        loop_flag = FALSE;
                    }
                } else {
                    loop_flag = FALSE;
                }
                break;
            case 0:
            case 1:
            case 2:
            case 3:
            case 6:
            case 7:
            case 9:
            case 11:
            case 14:
            case 15:
            case 16:
            case 17:
            default:
                loop_flag = FALSE;
                break;
            case 4:
            case 8:
                break;
            }
        }
        OS_CancelAlarm(&alarm);
        while (OS_ReceiveMessage(&data_ov016_021dce40, &msg, 0) == 1) {
        }
    }
    Set_WLLA_LINK_STAT(result, &data_ov016_021dce60, ret_code == 0);
    return ret_code;
}

// pokebw2's AossAp_Sleep
void AOSSi_Sleep(u32 milliseconds) {
    OS_Sleep(milliseconds);
}

// pokebw2's AossAp_NotifyModeChange
int AOSSi_Status(int mode) {
    if (data_ov016_021dce24 != NULL) {
        data_ov016_021dce24(mode);
    }
    return 0;
}
