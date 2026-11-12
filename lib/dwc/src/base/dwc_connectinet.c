// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/base/dwc_connectinet.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "dwc/ac.h"
#include "dwc/apinfo.h"
#include "dwc/connectinet.h"
#include "dwc/error.h"
#include "dwc/memfunc.h"
#include "dwc/wds.h"
#include "dwc/wdsscan.h"
#include "nitro/math.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/wcm.h"
#include "dwc/wcm.h"

// DWC's connection to the internet (TWL-DWC 5.3's dwc_connectinet.c, a guessed name; the ROM names none of it, and the
// file is ARM code in ARM9 main like the rest of the library's base): the control that the game gives the library, the
// connection through the auto connect (AC) and the WDS scan, and the access point information that goes with it. The
// layouts are in dwc/connectinet.h and dwc/apinfo.h; the SDK's names for the functions are in the comments there.

// The kinds of access point (DWCApInfo.apType): one that DWC does not know, the AC's answer when it has none, and the
// two of Nintendo's that carry the area and spot information: a Nintendo Wi-Fi Connection spot, and a Nintendo Zone
// that the WDS scan found
#define AP_TYPE_UNKNOWN 99
#define AP_TYPE_NONE 0xff
#define AP_TYPE_NINTENDO_WFC 4
#define AP_TYPE_WDS 8
// The area of a spot that tells none
#define AP_AREA_UNKNOWN 0xff

// The sizes of an access point's names
#define BSSID_SIZE 6
#define SSID_SIZE 32

// The TCP receive windows that a DSi may ask for
#define RWIN_MIN 0x10c0
#define RWIN_MAX 0xffff

// The AC's status (DWC_AC_GetStatus): 5 once it is connected, 0 or more while it works and a negative number for an
// error. The errors from -10 up are its own failures
#define AC_STATUS_CONNECTED 5
#define AC_STATUS_OWN_ERROR_MIN -10
// The code of the fatal error that reports those failures, and a connection that cannot start
#define AC_OWN_ERROR_CODE -50199

// The codes of the errors that the connection sets with DWC_ERROR_AC_ANY and DWC_ERROR_DISCONNECTED (the loss of the
// link while WCM is not in its state for errors); the fatal error is DWC_ERROR_FATAL. The loss of the link subtracts
// the kind of the access point from its code
#define LINK_LOST_CODE -54000
#define LINK_LOST_FATAL_CODE -54900

// WCM's state in which the link is up, and the one it enters after its errors, which the connection reports as fatal
#define WCM_STATE_CONNECTED 9
#define WCM_STATE_ERROR 11

// The work that a beacon is parsed with, 0x376 bytes: the context that decrypts the game information, the checksum of
// the decrypted record with its table, and the record
struct DWCBssWork {
    CRYPTORC4Context rc4;
    u16 crc;
    MATHCRC16Table crcTable;
    WDSBeaconInfo record;
};

// What makes a beacon one of a WDS access point: the number and ggid of its game information, its size and the
// polynomial of the checksum
#define WDS_MAGIC_NUMBER 1
#define WDS_GGID 0x857
#define WDS_GAME_INFO_SIZE 0x80
#define WDS_GAME_INFO_HEADER_SIZE 0x10
#define WDS_CRC_POLY 0xa001

// The file's statics
// Black 2's, not checked against Black
typedef struct DWCInet {
    // The AC's error that the restart of the connection through the WDS scan remembers
    int lastError;
    // The control that the game gave, NULL while there is none
    DWCInetControl *control;
} DWCInet;

// pokebw2's sInet
static DWCInet data_02146ad4;

static void func_0205c710(int rwin);
static void func_0205c81c(const char *ssid, const u8 *wepKey, int wepMode, u16 mtu, const char *spotInfo,
                                  int apType);
static BOOL func_0205ccac(void);
static void func_0205cce4(const char *spotInfo, DWCApInfo *apInfo);
static BOOL func_0205ce84(const u8 *ssid, u16 ssidLength, DWCApInfo *apInfo);
static u32 func_0205cf24(void);
static BOOL func_0205cf30(DWCBssWork *work, const WMBssDesc *bss, DWCApInfo *apInfo);

// pokebw2's DWC_InitInetEx
void func_0205c6ac(DWCInetControl *control, u16 dmaNo, u16 powerMode, u16 sslPriority) {
    if (data_02146ad4.control != NULL) {
        return;
    }
    MI_CpuFill8(control, 0, sizeof(DWCInetControl));
    control->dmaNo = dmaNo;
    control->powerMode = 1;
    control->state = DWC_CONNECTINET_STATE_IDLE;
    control->watch = 0;
    control->ended = FALSE;
    control->scanStage = DWC_INET_SCAN_NONE;
    control->skipEulaCheck = 0;
    data_02146ad4.control = control;
    CPS_SetSslHandshakePriority(sslPriority);
}

// pokebw2's DWCi_Inet_SetRecvWindow
static void func_0205c710(int rwin) {
    if (data_02146ad4.control == NULL) {
        return;
    }
    if (rwin < RWIN_MIN || rwin > RWIN_MAX) {
        return;
    }
    if (OS_IsRunOnTwl()) {
        data_02146ad4.control->rwin = rwin;
    }
}

// pokebw2's DWC_ConnectInetAsync
void func_0205c758(void) {
    DWCAcParam param;

    if (data_02146ad4.control != NULL) {
        if (data_02146ad4.control->state != DWC_CONNECTINET_STATE_IDLE) {
            return;
        }
        MI_CpuFill8(&param, 0, sizeof(DWCAcParam));
        param.dmaNo = data_02146ad4.control->dmaNo;
        param.radioOn = data_02146ad4.control->powerMode;
        param.rwin = data_02146ad4.control->rwin;
        param.alloc = DWC_Alloc;
        param.free = DWC_Free;
        param.ignoreEula = data_02146ad4.control->skipEulaCheck;
        data_02146ad4.control->state = DWC_CONNECTINET_STATE_OPERATING;
        if (!func_0205b98c(&param)) {
            DWCi_SetError(DWC_ERROR_FATAL, AC_OWN_ERROR_CODE);
        }
    } else {
        DWCi_SetError(DWC_ERROR_FATAL, AC_OWN_ERROR_CODE);
    }
}

static void func_0205c81c(const char *ssid, const u8 *wepKey, int wepMode, u16 mtu, const char *spotInfo,
                                  int apType) {
    func_0205c758();
    if (data_02146ad4.control != NULL) {
        data_02146ad4.control->ended = TRUE;
    }
    if (func_0205ca20() == DWC_CONNECTINET_STATE_OPERATING) {
        DWC_AC_SetSpecifyApEx(ssid, wepKey, wepMode, mtu, spotInfo, apType);
    }
}

// pokebw2's DWC_CheckInet
BOOL func_0205c88c(void) {
    if (data_02146ad4.control == NULL) {
        return FALSE;
    }
    if (data_02146ad4.control->result != 0) {
        data_02146ad4.control->state = DWC_CONNECTINET_STATE_OPERATED;
        func_0205ca20();
        return data_02146ad4.control->ended != 0;
    }
    return FALSE;
}

// pokebw2's DWC_ProcessInet
void func_0205c8e0(void) {
    if (data_02146ad4.control == NULL) {
        return;
    }
    switch (data_02146ad4.control->scanStage) {
    case DWC_INET_SCAN_SCANNING:
        data_02146ad4.control->result = 0;
        data_02146ad4.control->scanStage = func_0205d98c();
        return;
    case DWC_INET_SCAN_FOUND:
        func_0205dc74();
        func_0205c81c(data_02146ad4.control->ap.ssid, data_02146ad4.control->ap.wepKey, data_02146ad4.control->ap.wepMode,
                              data_02146ad4.control->ap.mtu, data_02146ad4.control->ap.spotInfo, AP_TYPE_WDS);
        func_0205c018();
        data_02146ad4.control->scanStage = DWC_INET_SCAN_NONE;
        return;
    case DWC_INET_SCAN_FAILED:
        func_0205dc74();
        data_02146ad4.control->result = -1;
        data_02146ad4.control->ended = TRUE;
        return;
    case DWC_INET_SCAN_REPORTED:
    case DWC_INET_SCAN_5:
        return;
    }
    if (data_02146ad4.control->state == DWC_CONNECTINET_STATE_OPERATING) {
        data_02146ad4.control->result = DWC_AC_Process();
    } else if (data_02146ad4.control->state == DWC_CONNECTINET_STATE_CONNECTED) {
        if (data_02146ad4.control->watch != 0) {
            if (func_ov009_021656a0() != WCM_STATE_CONNECTED) {
                data_02146ad4.control->watch = 0;
                data_02146ad4.control->state = DWC_CONNECTINET_STATE_DISCONNECTED;
            }
        }
    }
}

// pokebw2's DWC_GetInetStatus
int func_0205ca20(void) {
    int status;

    if (data_02146ad4.control == NULL) {
        return DWC_CONNECTINET_STATE_NOT_INITIALIZED;
    }
    switch (data_02146ad4.control->scanStage) {
    case DWC_INET_SCAN_SCANNING:
        return DWC_CONNECTINET_STATE_OPERATING;
    case DWC_INET_SCAN_FOUND:
        return DWC_CONNECTINET_STATE_OPERATING;
    case DWC_INET_SCAN_FAILED:
        func_0205c8e0();
        DWCi_SetError(DWC_ERROR_AC_ANY, data_02146ad4.lastError);
        data_02146ad4.control->state = DWC_CONNECTINET_STATE_ERROR;
        data_02146ad4.control->scanStage = DWC_INET_SCAN_REPORTED;
        return DWC_CONNECTINET_STATE_ERROR;
    case DWC_INET_SCAN_REPORTED:
        return DWC_CONNECTINET_STATE_ERROR;
    case DWC_INET_SCAN_5:
        return DWC_CONNECTINET_STATE_DISCONNECTING;
    }
    if (data_02146ad4.control->state == DWC_CONNECTINET_STATE_IDLE) {
        return data_02146ad4.control->state;
    }
    status = DWC_AC_GetStatus();
    if (status == AC_STATUS_CONNECTED) {
        data_02146ad4.control->state = DWC_CONNECTINET_STATE_CONNECTED;
        data_02146ad4.control->watch = 1;
        data_02146ad4.control->ended = TRUE;
        return DWC_CONNECTINET_STATE_CONNECTED;
    }
    if (status < 0) {
        if (status >= AC_STATUS_OWN_ERROR_MIN) {
            data_02146ad4.control->ended = TRUE;
            DWCi_SetError(DWC_ERROR_FATAL, AC_OWN_ERROR_CODE);
            data_02146ad4.control->state = DWC_CONNECTINET_STATE_FATAL_ERROR;
            return DWC_CONNECTINET_STATE_FATAL_ERROR;
        }
        // Two failures that end the connection without a retry
        if (status == -50599 || status == -50699) {
            data_02146ad4.control->ended = TRUE;
            DWCi_SetError(DWC_ERROR_AC_ANY, status);
            data_02146ad4.control->state = DWC_CONNECTINET_STATE_ERROR;
            return DWC_CONNECTINET_STATE_ERROR;
        }
        if (!data_02146ad4.control->ended) {
            DWCInetControl *control = data_02146ad4.control;
            u16 dmaNo = control->dmaNo;
            u32 sslPriority = CPS_GetSslHandshakePriority();
            u8 skipEulaCheck = data_02146ad4.control->skipEulaCheck;
            int rwin = data_02146ad4.control->rwin;

            data_02146ad4.lastError = status;
            func_0205cc20();
            func_0205c6ac(control, dmaNo, 1, sslPriority);
            data_02146ad4.control->skipEulaCheck = skipEulaCheck;
            if (rwin != 0) {
                func_0205c710(rwin);
            }
            func_0205d91c(&data_02146ad4.control->ap, dmaNo);
            data_02146ad4.control->scanStage = DWC_INET_SCAN_SCANNING;
            data_02146ad4.control->result = 0;
            return DWC_CONNECTINET_STATE_OPERATING;
        }
        if (data_02146ad4.lastError < status) {
            status = data_02146ad4.lastError;
        }
        DWCi_SetError(DWC_ERROR_AC_ANY, status);
        data_02146ad4.control->state = DWC_CONNECTINET_STATE_ERROR;
        return DWC_CONNECTINET_STATE_ERROR;
    }
    return DWC_CONNECTINET_STATE_OPERATING;
}

// pokebw2's DWC_CleanupInet
void func_0205cc20(void) {
    if (data_02146ad4.control == NULL) {
        return;
    }
    if (data_02146ad4.control->state == DWC_CONNECTINET_STATE_IDLE) {
        if (data_02146ad4.control->scanStage == DWC_INET_SCAN_SCANNING) {
            int scan;

            func_0205dc58();
            do {
                scan = func_0205d98c();
            } while (!(scan == DWC_INET_SCAN_FOUND || scan == DWC_INET_SCAN_FAILED || scan == DWC_INET_SCAN_REPORTED));
            func_0205dc74();
        }
        data_02146ad4.control = NULL;
        return;
    }
    if (!DWC_AC_Destroy()) {
        do {
            OS_Sleep(10);
        } while (!DWC_AC_Destroy());
    }
    data_02146ad4.control = NULL;
}

// pokebw2's DWCi_Inet_IsLinkLost
static BOOL func_0205ccac(void) {
    if (data_02146ad4.control != NULL && data_02146ad4.control->state == DWC_CONNECTINET_STATE_DISCONNECTED) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's DWC_GetLinkLevel
int func_0205ccd8(void) {
    return func_ov009_021666c8();
}

// pokebw2's DWCi_ApInfo_SetSpotInfo
static void func_0205cce4(const char *spotInfo, DWCApInfo *apInfo) {
    apInfo->area = (spotInfo[0] >= '0' && spotInfo[0] <= '9') ? spotInfo[0] - '0' : AP_AREA_UNKNOWN;
    MI_CpuCopy8(spotInfo + 1, apInfo->spotInfo, 9);
}

// pokebw2's DWC_GetApInfo
BOOL func_0205cd24(DWCApInfo *apInfo) {
    u16 ssidLength;
    char spotInfo[10];
    u32 irq;
    const u8 *bssid;
    const u8 *ssid;

    MI_CpuFill8(apInfo, 0, sizeof(DWCApInfo));
    apInfo->area = AP_AREA_UNKNOWN;
    if (func_0205ca20() != DWC_CONNECTINET_STATE_CONNECTED) {
        return FALSE;
    }
    apInfo->apType = DWC_AC_GetApType();
    if (apInfo->apType == AP_TYPE_NONE || apInfo->apType > AP_TYPE_UNKNOWN) {
        apInfo->apType = AP_TYPE_UNKNOWN;
        return FALSE;
    }
    irq = OS_DisableInterrupts();
    bssid = func_ov009_02166384();
    DC_InvalidateRange(bssid, BSSID_SIZE);
    if (bssid == NULL) {
        OS_RestoreInterrupts(irq);
        return FALSE;
    }
    MI_CpuCopy8(bssid, apInfo->bssid, BSSID_SIZE);
    if (apInfo->apType == 0 || apInfo->apType == 1 || apInfo->apType == 2 ||
        (OS_IsRunOnTwl() && (apInfo->apType == 10 || apInfo->apType == 11 || apInfo->apType == 12))) {
        ssid = func_ov009_021663c8(&ssidLength);
        DC_InvalidateRange(ssid, SSID_SIZE);
        if (ssid == NULL) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        MI_CpuCopy8(ssid, apInfo->essid, ssidLength);
    }
    OS_RestoreInterrupts(irq);
    if (!DWC_AC_GetApSpotInfo(spotInfo)) {
        apInfo->area = AP_AREA_UNKNOWN;
    } else {
        func_0205cce4(spotInfo, apInfo);
    }
    apInfo->keyMode = func_0205be58();
    return TRUE;
}

// pokebw2's DWCi_ApInfo_FromNintendoSsid
static BOOL func_0205ce84(const u8 *ssid, u16 ssidLength, DWCApInfo *apInfo) {
    char spotInfo[10];

    MI_CpuFill8(spotInfo, 0, sizeof(spotInfo));
    if (ssid == NULL) {
        return FALSE;
    }
    if (DWC_AC_CheckWiFiStation(ssid, ssidLength)) {
        if (apInfo == NULL) {
            return TRUE;
        }
        if (func_0205ea70(ssid, spotInfo)) {
            apInfo->apType = AP_TYPE_NINTENDO_WFC;
            func_0205cce4(spotInfo, apInfo);
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's DWCi_ApInfo_GetWorkSize
static u32 func_0205cf24(void) {
    return sizeof(DWCBssWork);
}

// pokebw2's DWCi_ApInfo_FromWdsBeacon
static BOOL func_0205cf30(DWCBssWork *work, const WMBssDesc *bss, DWCApInfo *apInfo) {
    if (work == NULL || bss == NULL) {
        return FALSE;
    }
    MI_CpuFill8(work, 0, sizeof(DWCBssWork));
    if (bss->gameInfo.userGameInfoLength + WDS_GAME_INFO_HEADER_SIZE == WDS_GAME_INFO_SIZE &&
        bss->gameInfo.magicNumber == WDS_MAGIC_NUMBER && bss->gameInfo.ggid == WDS_GGID) {
        work->crc = 0;
        MATHi_CRC16InitTableRev(&work->crcTable, WDS_CRC_POLY);
        if (func_0205a7cc(&work->rc4, &work->crc, &work->crcTable, bss, &work->record)) {
            if (func_0205a8fc(&work->record)) {
                if (apInfo != NULL) {
                    apInfo->apType = AP_TYPE_WDS;
                    func_0205cce4((char *)work->record.spotInfo, apInfo);
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

// pokebw2's DWCi_GetBssDescParseWorkSize
u32 func_0205d010(void) {
    return func_0205cf24();
}

// pokebw2's DWCi_BssDescToApInfo
BOOL func_0205d01c(DWCBssWork *work, const WMBssDesc *bss, DWCApInfo *apInfo) {
    if (work == NULL || bss == NULL || apInfo == NULL) {
        return FALSE;
    }
    if (func_0205ce84(bss->ssid, bss->ssidLength, apInfo)) {
        return TRUE;
    }
    return func_0205cf30(work, bss, apInfo) != 0;
}

// pokebw2's DWC_UpdateConnection
BOOL func_0205d074(void) {
    u32 apType;

    func_0205c8e0();
    if (func_0205ccac()) {
        apType = DWC_AC_GetApType();
        if (apType == AP_TYPE_NONE || apType > AP_TYPE_UNKNOWN) {
            apType = AP_TYPE_UNKNOWN;
        }
        if (func_ov009_021656a0() == WCM_STATE_ERROR) {
            DWCi_SetError(DWC_ERROR_FATAL, LINK_LOST_FATAL_CODE - apType);
        } else {
            DWCi_SetError(DWC_ERROR_DISCONNECTED, LINK_LOST_CODE - apType);
        }
        return TRUE;
    }
    return FALSE;
}
