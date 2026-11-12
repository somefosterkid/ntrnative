// libdwcutil's dwc_mov_wh.c (Black's ov016 0x021d298c-0x021d3b18): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order. It is
// NitroSDK's wireless helper (demos/wireless_shared/wh) for DS Download Play, with the prebuilt's names: the work
// DWCi_MOV_WH_WORK and its fields, the statics wh_trace and wh, and ChangeSysState's statenames come from its DWARF.
#include "dwcutil.h"
#include "system/wm_etc.h"
#include "gfl/dwc_mov_wh.h"

// The helper's states
enum {
    DWCi_MOV_WH_SYSSTATE_STOP,
    DWCi_MOV_WH_SYSSTATE_IDLE,
    DWCi_MOV_WH_SYSSTATE_SCANNING,
    DWCi_MOV_WH_SYSSTATE_BUSY,
    DWCi_MOV_WH_SYSSTATE_CONNECTED,
    DWCi_MOV_WH_SYSSTATE_DATASHARING,
    DWCi_MOV_WH_SYSSTATE_KEYSHARING,
    DWCi_MOV_WH_SYSSTATE_MEASURECHANNEL,
    DWCi_MOV_WH_SYSSTATE_CONNECT_FAIL,
    DWCi_MOV_WH_SYSSTATE_ERROR,
    DWCi_MOV_WH_SYSSTATE_FATAL,
};

// How it connects (the prebuilt's sConnectMode), as NitroSDK's wh.h numbers them
enum {
    DWCi_MOV_WH_CONNECTMODE_MP_PARENT,
    DWCi_MOV_WH_CONNECTMODE_MP_CHILD,
    DWCi_MOV_WH_CONNECTMODE_KS_PARENT,
    DWCi_MOV_WH_CONNECTMODE_KS_CHILD,
    DWCi_MOV_WH_CONNECTMODE_DS_PARENT,
    DWCi_MOV_WH_CONNECTMODE_DS_CHILD,
};

// Its own errors, after the WM ones (the prebuilt's errnames end with NO_RADIO)
enum {
    DWCi_MOV_WH_ERRCODE_DISCONNECTED = 0x14,
    DWCi_MOV_WH_ERRCODE_NO_RADIO = 0x16,
    DWCi_MOV_WH_ERRCODE_NOMORE_CHANNEL = 0x18,
};

// NitroSDK's WMErrCode values that nitro/wm.h lacks
enum {
    WM_ERRCODE_ILLEGAL_STATE = 3,
    WM_ERRCODE_NO_DATASET = 5,
    WM_ERRCODE_NO_CHILD = 7,
    WM_ERRCODE_FIFO_ERROR = 8,
    WM_ERRCODE_TIMEOUT = 9,
    WM_ERRCODE_NO_ENTRY = 11,
    WM_ERRCODE_OVER_MAX_ENTRY = 12,
    WM_ERRCODE_INVALID_POLLBITMAP = 13,
    WM_ERRCODE_SEND_FAILED = 15,
};

// NitroSDK's WM_STATECODE values that nitro/wm.h lacks
enum {
    WM_STATECODE_SCAN_START = 3,
    WM_STATECODE_CONNECT_START = 6,
    WM_STATECODE_BEACON_LOST = 8,
};

// What wh_trace prints with
#define DWCi_MOV_WH_TRACE_FLAG 0x08000000

typedef WMDataSharingInfo WMKeySetBuf;

typedef struct WMScanParam {
    WMBssDesc *scanBuf;
    u16 channel;
    u16 maxChannelTime;
    u8 bssid[6];
    u16 rsv[9];
} WMScanParam;

// The scan's callback (NitroSDK's WMStartScanCallback, as the prebuilt's DWARF has it): the child-side code below,
// which Black's link drops, reads it
typedef struct WMStartScanCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;
    u8 macAddress[6];
    u16 channel;
    u16 linkLevel;
    u16 ssidLength;
    u16 ssid[16];
    u16 gameInfoLength;
    WMGameInfo gameInfo;
} WMStartScanCallback;

// NitroSDK's WM and CHT functions that only the dropped child-side code calls
WMErrCode WM_StartScan(WMCallbackFunc callback, const WMScanParam *param);
BOOL CHT_IsPictochatParent(const WMBssDesc *bssDesc);

typedef struct DWCi_MOV_WH_WORK {
    WMParentParam sParentParam;
    u8 unk_38[0x40 - sizeof(WMParentParam)];
    int sSysState;
    int sConnectMode;
    void (*sReceiverFunc)(u16 aid, u16 *data, u16 size);
    BOOL (*sJudgeAcceptFunc)(WMStartParentCallback *cb);
    u16 sMyAid;
    u16 sConnectBitmap;
    int sErrCode;
    u32 sRand;
    u16 sChannel;
    u16 sChannelBusyRatio;
    u16 sChannelBitmap;
    u8 unk_62[0x80 - 0x62];
    u8 sWmBuffer[0xf00];
    u8 sSendBuffer[0xe0];
    u8 sRecvBuffer[0x240];
    s32 sSendBufferSize;
    s32 sRecvBufferSize;
    u8 unk_12a8[0x12c0 - 0x12a8];
    WMBssDesc sBssDesc;
    WMScanParam sScanParam;
    void (*sScanCallback)(WMBssDesc *bssDesc);
    u16 sChannelIndex;
    u16 sAutoConnectFlag;
    int sPictoCatchFlag;
    u16 (*sParentWEPKeyGenerator)(u16 *wepkey, WMParentParam *parentParam);
    u16 (*sChildWEPKeyGenerator)(u16 *wepkey, WMBssDesc *bssDesc);
    u8 unk_13b4[0x13c0 - 0x13b4];
    u16 sWEPKey[10];
    u8 unk_13d4[0x13e0 - 0x13d4];
    WMDataSharingInfo sDSInfo;
    WMDataSet sDataSet;
    WMKeySetBuf sWMKeySetBuf;
} DWCi_MOV_WH_WORK;

WMErrCode WM_StartKeySharing(WMKeySetBuf *buf, u16 port);
WMErrCode WM_EndKeySharing(WMKeySetBuf *buf);

void DWCi_MOV_WH_ChangeSysState(int state);
void DWCi_MOV_WH_SetError(int code);
int DWCi_MOV_WH_StateInSetParentParam(void);
void DWCi_MOV_WH_StateOutSetParentParam(void *arg);
int DWCi_MOV_WH_StateInSetParentWEPKey(void);
void DWCi_MOV_WH_StateOutSetParentWEPKey(void *arg);
int DWCi_MOV_WH_StateInStartParent(void);
void DWCi_MOV_WH_StateOutStartParent(void *arg);
int DWCi_MOV_WH_StateInStartParentMP(void);
void DWCi_MOV_WH_StateOutStartParentMP(void *arg);
int DWCi_MOV_WH_StateInStartParentKeyShare(void);
int DWCi_MOV_WH_StateInEndParentKeyShare(void);
int DWCi_MOV_WH_StateInEndParentMP(void);
void DWCi_MOV_WH_StateOutEndParentMP(void *arg);
int DWCi_MOV_WH_StateInEndParent(void);
void DWCi_MOV_WH_StateOutEndParent(void *arg);
int DWCi_MOV_WH_StateInEndChildKeyShare(void);
int DWCi_MOV_WH_StateInEndChildMP(void);
void DWCi_MOV_WH_StateOutEndChildMP(void *arg);
int DWCi_MOV_WH_StateInEndChild(void);
void DWCi_MOV_WH_StateOutEndChild(void *arg);
int DWCi_MOV_WH_StateInReset(void);
void DWCi_MOV_WH_StateOutReset(void *arg);
void DWCi_MOV_WH_StateOutEnd(void *arg);
u16 DWCi_MOV_WH_StateInMeasureChannel(u16 channel);
void DWCi_MOV_WH_StateOutMeasureChannel(void *arg);
WMErrCode WHi_MeasureChannel(WMCallbackFunc func, u16 channel);
s16 SelectChannel(u16 bitmap);
void DWCi_MOV_WH_IndicateHandler(void *arg);
int DWCi_MOV_WH_StateInInitialize(void);
void DWCi_MOV_WH_StateOutInitialize(void *arg);
char *DWCi_MOV_WH_GetWMErrCodeName(int result);
char *DWCi_MOV_WH_GetWMStateCodeName(u16 code);
int DWCi_MOV_WH_ChildConnectAuto(s32 mode, u8 *macAddr, u16 channel);
int DWCi_MOV_WH_StateInStartScan(void);
void DWCi_MOV_WH_StateOutStartScan(void *arg);
int DWCi_MOV_WH_StateInEndScan(void);
void DWCi_MOV_WH_StateOutEndScan(void *arg);
int DWCi_MOV_WH_StateInSetChildWEPKey(void);
void DWCi_MOV_WH_StateOutSetChildWEPKey(void *arg);
int DWCi_MOV_WH_StateInStartChild(void);
void DWCi_MOV_WH_StateOutStartChild(void *arg);
int DWCi_MOV_WH_StateInStartChildMP(void);
void DWCi_MOV_WH_StateOutStartChildMP(void *arg);
int DWCi_MOV_WH_StateInStartChildKeyShare(void);

// Listed with their initializers ahead of ChangeSysState's statenames, which gives Black's order of its three 26-byte
// names (STOP, IDLE, BUSY) in MWCC's size sort; the prebuilt's list, with the WM state and error name tables of
// GetWMStateCodeName and GetWMErrCodeName (whose callers Black's link drops), gives the same order either way
static DWCi_MOV_WH_WORK *wh = NULL;
static void (*wh_trace)(u32 flag, char *fmt, ...) = NULL;

void DWCi_MOV_WH_MemInit(s32 mw) {
    wh = (DWCi_MOV_WH_WORK *)mw;
    ((DWCi_MOV_WH_WORK *)mw)->sSysState = DWCi_MOV_WH_SYSSTATE_STOP;
    wh->sPictoCatchFlag = 0;
    wh->sParentWEPKeyGenerator = NULL;
    wh->sChildWEPKeyGenerator = NULL;
}

// NitroSDK's WH_GetWMErrCodeName: Black's link drops it with its only caller (StateInSetMPData, which Black lacks)
char *DWCi_MOV_WH_GetWMErrCodeName(int result) {
    static char *errnames[] = {
        "WM_ERRCODE_SUCCESS",
        "WM_ERRCODE_FAILED",
        "WM_ERRCODE_OPERATING",
        "WM_ERRCODE_ILLEGAL_STATE",
        "WM_ERRCODE_WM_DISABLE",
        "WM_ERRCODE_NO_DATASET",
        "WM_ERRCODE_INVALID_PARAM",
        "WM_ERRCODE_NO_CHILD",
        "WM_ERRCODE_FIFO_ERROR",
        "WM_ERRCODE_TIMEOUT",
        "WM_ERRCODE_SEND_QUEUE_FULL",
        "WM_ERRCODE_NO_ENTRY",
        "WM_ERRCODE_OVER_MAX_ENTRY",
        "WM_ERRCODE_INVALID_POLLBITMAP",
        "WM_ERRCODE_NO_DATA",
        "WM_ERRCODE_SEND_FAILED",
        "WM_ERRCODE_DCF_TEST",
        "WM_ERRCODE_WL_INVALID_PARAM",
        "WM_ERRCODE_WL_LENGTH_ERR",
        "WM_ERRCODE_FLASH_ERROR",
        "DWCi_MOV_WH_ERRCODE_DISCONNECTED",
        "DWCi_MOV_WH_ERRCODE_PARENT_NOT_FOUND",
        "DWCi_MOV_WH_ERRCODE_NO_RADIO",
    };

    if (result >= 0 && result < sizeof(errnames) / sizeof(errnames[0])) {
        return errnames[result];
    } else {
        return "N/A";
    }
}

// NitroSDK's WH_GetWMStateCodeName: Black's link drops it with its only caller, StateOutStartChild
char *DWCi_MOV_WH_GetWMStateCodeName(u16 code) {
    static char *statenames[] = {
        "WM_STATECODE_PARENT_START",
        "N/A",
        "WM_STATECODE_BEACON_SENT",
        "WM_STATECODE_SCAN_START",
        "WM_STATECODE_PARENT_NOT_FOUND",
        "WM_STATECODE_PARENT_FOUND",
        "WM_STATECODE_CONNECT_START",
        "WM_STATECODE_CONNECTED",
        "WM_STATECODE_BEACON_LOST",
        "WM_STATECODE_DISCONNECTED",
        "WM_STATECODE_MP_START",
        "WM_STATECODE_MPEND_IND",
        "WM_STATECODE_MP_IND",
        "WM_STATECODE_MPACK_IND",
        "WM_STATECODE_DCF_START",
        "WM_STATECODE_DCF_IND",
        "WM_STATECODE_BEACON_RECV",
        "WM_STATECODE_DISASSOCIATE",
        "WM_STATECODE_REASSOCIATE",
        "WM_STATECODE_AUTHENTICATE",
        "WM_STATECODE_PORT_SEND",
        "WM_STATECODE_PORT_RECV",
        "WM_STATECODE_FIFO_ERROR",
    };

    return (code < sizeof(statenames) / sizeof(statenames[0])) ? statenames[code] : "N/A";
}

void DWCi_MOV_WH_ChangeSysState(int state) {
    static char *statenames[] = {
        "DWCi_MOV_WH_SYSSTATE_STOP",
        "DWCi_MOV_WH_SYSSTATE_IDLE",
        "DWCi_MOV_WH_SYSSTATE_SCANNING",
        "DWCi_MOV_WH_SYSSTATE_BUSY",
        "DWCi_MOV_WH_SYSSTATE_CONNECTED",
        "DWCi_MOV_WH_SYSSTATE_DATASHARING",
        "DWCi_MOV_WH_SYSSTATE_KEYSHARING",
        "DWCi_MOV_WH_SYSSTATE_MEASURECHANNEL",
        "DWCi_MOV_WH_SYSSTATE_CONNECT_FAIL",
        "DWCi_MOV_WH_SYSSTATE_ERROR",
    };

    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "%s -> ", statenames[wh->sSysState]);
    }
    wh->sSysState = state;
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "%s\n", statenames[wh->sSysState]);
    }
}

void DWCi_MOV_WH_SetError(int code) {
    if (wh->sSysState == DWCi_MOV_WH_SYSSTATE_ERROR || wh->sSysState == DWCi_MOV_WH_SYSSTATE_FATAL) {
        return;
    }
    wh->sErrCode = code;
}

int DWCi_MOV_WH_StateInSetParentParam(void) {
    WMErrCode result;

    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    result = WM_SetParentParameter(DWCi_MOV_WH_StateOutSetParentParam, &wh->sParentParam);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    return FALSE;
}

void DWCi_MOV_WH_StateOutSetParentParam(void *arg) {
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return;
    }
    if (wh->sParentWEPKeyGenerator != NULL) {
        if (!DWCi_MOV_WH_StateInSetParentWEPKey()) {
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        }
        return;
    }
    if (!DWCi_MOV_WH_StateInStartParent()) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    }
}

int DWCi_MOV_WH_StateInSetParentWEPKey(void) {
    u16 wepmode;
    WMErrCode result;

    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    wepmode = wh->sParentWEPKeyGenerator(wh->sWEPKey, &wh->sParentParam);
    result = WM_SetWEPKey(DWCi_MOV_WH_StateOutSetParentWEPKey, wepmode, wh->sWEPKey);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    return FALSE;
}

void DWCi_MOV_WH_StateOutSetParentWEPKey(void *arg) {
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return;
    }
    if (!DWCi_MOV_WH_StateInStartParent()) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    }
}

int DWCi_MOV_WH_StateInStartParent(void) {
    WMErrCode result;

    if (wh->sSysState == DWCi_MOV_WH_SYSSTATE_CONNECTED || wh->sSysState == DWCi_MOV_WH_SYSSTATE_KEYSHARING
        || wh->sSysState == DWCi_MOV_WH_SYSSTATE_DATASHARING) {
        return TRUE;
    }
    result = WM_StartParent(DWCi_MOV_WH_StateOutStartParent);
    if (result != WM_ERRCODE_OPERATING) {
        DWCi_MOV_WH_SetError(result);
        return FALSE;
    }
    wh->sMyAid = 0;
    wh->sConnectBitmap = 1;
    return TRUE;
}

void DWCi_MOV_WH_StateOutStartParent(void *arg) {
    WMStartParentCallback *cb = arg;
    u16 target_bitmap = 1 << cb->aid;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_CONNECTED:
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "StartParent - new child (aid %x) connected\n", cb->aid);
        }
        if (wh->sJudgeAcceptFunc != NULL && !wh->sJudgeAcceptFunc(cb)) {
            WMErrCode result = WM_Disconnect(NULL, cb->aid);

            if (result != WM_ERRCODE_OPERATING) {
                DWCi_MOV_WH_SetError(result);
                DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
            }
            return;
        }
        wh->sConnectBitmap |= target_bitmap;
        return;
    case WM_STATECODE_DISCONNECTED:
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "StartParent - child (aid %x) disconnected\n", cb->aid);
        }
        wh->sConnectBitmap &= ~target_bitmap;
        return;
    case WM_STATECODE_PARENT_START:
        if (!DWCi_MOV_WH_StateInStartParentMP()) {
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        }
        return;
    case WM_STATECODE_BEACON_SENT:
        return;
    }
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "unknown indicate, state = %d\n", cb->state);
    }
}

int DWCi_MOV_WH_StateInStartParentMP(void) {
    WMErrCode result;

    if (wh->sSysState == DWCi_MOV_WH_SYSSTATE_CONNECTED || wh->sSysState == DWCi_MOV_WH_SYSSTATE_KEYSHARING
        || wh->sSysState == DWCi_MOV_WH_SYSSTATE_DATASHARING) {
        return TRUE;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_CONNECTED);
    result = WM_StartMP(DWCi_MOV_WH_StateOutStartParentMP, (u16 *)wh->sRecvBuffer, (u16)wh->sRecvBufferSize,
                        (u16 *)wh->sSendBuffer, (u16)wh->sSendBufferSize, 1);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    return FALSE;
}

void DWCi_MOV_WH_StateOutStartParentMP(void *arg) {
    WMStartMPCallback *cb = (WMStartMPCallback *)arg;
    WMErrCode result;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_MP_START:
        if (wh->sConnectMode == DWCi_MOV_WH_CONNECTMODE_KS_PARENT) {
            if (wh->sSysState == DWCi_MOV_WH_SYSSTATE_CONNECTED) {
                if (DWCi_MOV_WH_StateInStartParentKeyShare()) {
                    return;
                }
                if (wh_trace != NULL) {
                    wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StateInStartParentKeyShare failed\n");
                }
                DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
                return;
            }
            if (wh->sSysState == DWCi_MOV_WH_SYSSTATE_KEYSHARING) {
                return;
            }
        } else if (wh->sConnectMode == DWCi_MOV_WH_CONNECTMODE_DS_PARENT) {
            result = WM_StartDataSharing(&wh->sDSInfo, 0xd, 7, 0x44, TRUE);
            if (result != WM_ERRCODE_SUCCESS) {
                DWCi_MOV_WH_SetError(result);
                DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
                return;
            }
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_DATASHARING);
            return;
        }
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_CONNECTED);
        return;
    case WM_STATECODE_MPEND_IND:
        break;
    case WM_STATECODE_MP_IND:
    case WM_STATECODE_MPACK_IND:
    default:
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "unknown indicate, state = %d\n", cb->state);
        }
        break;
    }
}

int DWCi_MOV_WH_StateInStartParentKeyShare(void) {
    WMErrCode result;

    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_KEYSHARING);
    result = WM_StartKeySharing(&wh->sWMKeySetBuf, 0xd);
    if (result != WM_ERRCODE_OPERATING) {
        DWCi_MOV_WH_SetError(result);
        return FALSE;
    }
    return TRUE;
}

int DWCi_MOV_WH_StateInEndParentKeyShare(void) {
    WMErrCode result;

    result = WM_EndKeySharing(&wh->sWMKeySetBuf);
    if (result != WM_ERRCODE_OPERATING) {
        DWCi_MOV_WH_SetError(result);
        return FALSE;
    }
    return TRUE;
}

int DWCi_MOV_WH_StateInEndParentMP(void) {
    WMErrCode result;

    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    result = WM_EndMP(DWCi_MOV_WH_StateOutEndParentMP);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    return FALSE;
}

void DWCi_MOV_WH_StateOutEndParentMP(void *arg) {
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_Reset();
        return;
    }
    if (DWCi_MOV_WH_StateInEndParent()) {
        return;
    }
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StateInEndParent failed\n");
    }
    DWCi_MOV_WH_Reset();
}

int DWCi_MOV_WH_StateInEndParent(void) {
    WMErrCode result;

    result = WM_EndParent(DWCi_MOV_WH_StateOutEndParent);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    return FALSE;
}

void DWCi_MOV_WH_StateOutEndParent(void *arg) {
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        return;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_IDLE);
}

// The child side (NitroSDK's WH_ChildConnectAuto and the scan and connect states it goes through), from the prebuilt
// libdwcutil.a's code: Black's link drops all of it (nothing of Black's calls ChildConnectAuto). It is here because its
// first use of "recv buffer size = %d\n" and "send buffer size = %d\n" puts them before ParentConnect's other strings
int DWCi_MOV_WH_ChildConnectAuto(s32 mode, u8 *macAddr, u16 channel) {
    wh->sRecvBufferSize = 0x240;
    wh->sSendBufferSize = 0x60;
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "recv buffer size = %d\n", wh->sRecvBufferSize);
    }
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "send buffer size = %d\n", wh->sSendBufferSize);
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_SCANNING);
    wh->sBssDesc.channel = 1;
    *(u16 *)(&wh->sScanParam.bssid[4]) = *(u16 *)(macAddr + 4);
    *(u16 *)(&wh->sScanParam.bssid[2]) = *(u16 *)(macAddr + 2);
    *(u16 *)(&wh->sScanParam.bssid[0]) = *(u16 *)(macAddr);
    wh->sConnectMode = mode;
    wh->sScanCallback = NULL;
    wh->sChannelIndex = channel;
    wh->sScanParam.channel = 0;
    wh->sAutoConnectFlag = TRUE;
    if (!DWCi_MOV_WH_StateInStartScan()) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

// A stand-in for the prebuilt's StateInStartScan and StateOutStartScan, which Black's link drops with ChildConnectAuto
// (this code never reaches Black's ROM): the closest C found, 13 and 17 lines off the prebuilt's code under 2.0/sp2p2.
// The prebuilt has a movs r0, #0 after the assert's bl OS_Terminate (as in its ParentConnect and StartScan) and reloads
// wh at the top of the channel search loop; no spelling of the assert, the loop or the compiler version gave either
int DWCi_MOV_WH_StateInStartScan(void) {
    WMErrCode result;
    u16 chanpat;

    (void)(wh->sSysState == DWCi_MOV_WH_SYSSTATE_SCANNING || (OS_Terminate(), 0));
    chanpat = WM_GetAllowedChannel();
    if (chanpat == 0x8000) {
        DWCi_MOV_WH_SetError(WM_ERRCODE_ILLEGAL_STATE);
        return FALSE;
    }
    if (chanpat == 0) {
        DWCi_MOV_WH_SetError(DWCi_MOV_WH_ERRCODE_NO_RADIO);
        return FALSE;
    }
    if (wh->sChannelIndex == 0) {
        while (TRUE) {
            wh->sScanParam.channel++;
            if (wh->sScanParam.channel > 16) {
                wh->sScanParam.channel = 1;
            }
            if (chanpat & (1 << (wh->sScanParam.channel - 1))) {
                break;
            }
        }
    } else {
        wh->sScanParam.channel = wh->sChannelIndex;
    }
    wh->sScanParam.maxChannelTime = WM_GetDispersionScanPeriod();
    wh->sScanParam.scanBuf = &wh->sBssDesc;
    result = WM_StartScan(DWCi_MOV_WH_StateOutStartScan, &wh->sScanParam);
    if (result != WM_ERRCODE_OPERATING) {
        DWCi_MOV_WH_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void DWCi_MOV_WH_StateOutStartScan(void *arg) {
    WMStartScanCallback *cb = (WMStartScanCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return;
    }
    if (wh->sSysState != DWCi_MOV_WH_SYSSTATE_SCANNING) {
        if (!DWCi_MOV_WH_StateInEndScan()) {
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        }
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_SCAN_START:
        return;
    case WM_STATECODE_PARENT_FOUND:
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StateOutStartScan : MAC=%02x%02x%02x%02x%02x%02x ",
                     cb->macAddress[0], cb->macAddress[1], cb->macAddress[2], cb->macAddress[3],
                     cb->macAddress[4], cb->macAddress[5]);
        }
        DC_InvalidateRange(&wh->sBssDesc, sizeof(WMBssDesc));
        if (wh->sPictoCatchFlag) {
            if (CHT_IsPictochatParent(&wh->sBssDesc)) {
                if (wh_trace != NULL) {
                    wh_trace(DWCi_MOV_WH_TRACE_FLAG, "pictochat parent find\n");
                }
                if (wh->sScanCallback != NULL) {
                    wh->sScanCallback(&wh->sBssDesc);
                }
                break;
            }
        }
        if (cb->gameInfoLength < 8 || cb->gameInfo.ggid != wh->sParentParam.ggid) {
            if (wh_trace != NULL) {
                wh_trace(DWCi_MOV_WH_TRACE_FLAG, "not my parent ggid \n");
            }
            break;
        }
        if ((cb->gameInfo.gameNameCount_attribute & 3) != 1) {
            if (wh_trace != NULL) {
                wh_trace(DWCi_MOV_WH_TRACE_FLAG, "not recieve entry\n");
            }
            break;
        }
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "parent find\n");
        }
        if (wh->sScanCallback != NULL) {
            wh->sScanCallback(&wh->sBssDesc);
        }
        if (wh->sAutoConnectFlag) {
            if (!DWCi_MOV_WH_StateInEndScan()) {
                DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
            }
            return;
        }
        break;
    case WM_STATECODE_PARENT_NOT_FOUND:
        break;
    default:
        break;
    }
    if (!DWCi_MOV_WH_StateInStartScan()) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    }
}


int DWCi_MOV_WH_StateInEndScan(void) {
    WMErrCode result;

    result = WM_EndScan(DWCi_MOV_WH_StateOutEndScan);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    return FALSE;
}

void DWCi_MOV_WH_StateOutEndScan(void *arg) {
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        return;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_IDLE);
    if (!wh->sAutoConnectFlag) {
        return;
    }
    if (wh->sChildWEPKeyGenerator != NULL) {
        if (!DWCi_MOV_WH_StateInSetChildWEPKey()) {
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        }
        return;
    }
    if (!DWCi_MOV_WH_StateInStartChild()) {
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StateOutEndScan : startchild failed\n");
        }
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    }
}

int DWCi_MOV_WH_StateInSetChildWEPKey(void) {
    u16 wepmode;
    WMErrCode result;

    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    wepmode = wh->sChildWEPKeyGenerator(wh->sWEPKey, &wh->sBssDesc);
    result = WM_SetWEPKey(DWCi_MOV_WH_StateOutSetChildWEPKey, wepmode, wh->sWEPKey);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    return FALSE;
}

void DWCi_MOV_WH_StateOutSetChildWEPKey(void *arg) {
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return;
    }
    if (!DWCi_MOV_WH_StateInStartChild()) {
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StateOutSetChildWEPKey : startchild failed\n");
        }
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    }
}

int DWCi_MOV_WH_StateInStartChild(void) {
    WMErrCode result;

    if (wh->sSysState == DWCi_MOV_WH_SYSSTATE_CONNECTED || wh->sSysState == DWCi_MOV_WH_SYSSTATE_KEYSHARING
        || wh->sSysState == DWCi_MOV_WH_SYSSTATE_DATASHARING) {
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StateInStartChild : already connected?\n");
        }
        return TRUE;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    result = WM_StartConnectEx(DWCi_MOV_WH_StateOutStartChild, &wh->sBssDesc, NULL, TRUE,
                               (u16)(wh->sChildWEPKeyGenerator != NULL ? 1 : 0));
    if (result != WM_ERRCODE_OPERATING) {
        DWCi_MOV_WH_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void DWCi_MOV_WH_StateOutStartChild(void *arg) {
    WMStartConnectCallback *cb = (WMStartConnectCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        if (cb->errcode == WM_ERRCODE_OVER_MAX_ENTRY) {
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
            return;
        } else if (cb->errcode == WM_ERRCODE_NO_ENTRY) {
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
            return;
        } else if (cb->errcode == WM_ERRCODE_FAILED) {
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_CONNECT_FAIL);
            return;
        } else {
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        }
        return;
    }
    if (cb->state == WM_STATECODE_BEACON_LOST) {
        return;
    }
    if (cb->state == WM_STATECODE_CONNECTED) {
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "Connect to Parent\n");
        }
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_CONNECTED);
        if (!DWCi_MOV_WH_StateInStartChildMP()) {
            if (wh_trace != NULL) {
                wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StateInStartChildMP failed\n");
            }
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
            return;
        }
        wh->sMyAid = cb->aid;
        return;
    } else if (cb->state == WM_STATECODE_CONNECT_START) {
        return;
    } else if (cb->state == WM_STATECODE_DISCONNECTED) {
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "Disconnected from Parent\n");
        }
        DWCi_MOV_WH_SetError(DWCi_MOV_WH_ERRCODE_DISCONNECTED);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return;
    }
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "unknown state %d, %s\n", cb->state,
                 DWCi_MOV_WH_GetWMStateCodeName(cb->state));
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
}

int DWCi_MOV_WH_StateInStartChildMP(void) {
    WMErrCode result;

    result = WM_StartMP(DWCi_MOV_WH_StateOutStartChildMP, (u16 *)wh->sRecvBuffer, (u16)wh->sRecvBufferSize,
                        (u16 *)wh->sSendBuffer, (u16)wh->sSendBufferSize, 1);
    if (result != WM_ERRCODE_OPERATING) {
        DWCi_MOV_WH_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void DWCi_MOV_WH_StateOutStartChildMP(void *arg) {
    WMStartMPCallback *cb = (WMStartMPCallback *)arg;
    WMErrCode result;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        if (cb->errcode == WM_ERRCODE_SEND_FAILED) {
            return;
        } else if (cb->errcode == WM_ERRCODE_TIMEOUT) {
            return;
        } else if (cb->errcode == WM_ERRCODE_INVALID_POLLBITMAP) {
            return;
        }
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_Reset();
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_MP_START:
        if (wh->sConnectMode == DWCi_MOV_WH_CONNECTMODE_KS_CHILD) {
            if (wh->sSysState == DWCi_MOV_WH_SYSSTATE_KEYSHARING) {
                return;
            }
            if (wh->sSysState == DWCi_MOV_WH_SYSSTATE_CONNECTED) {
                if (!DWCi_MOV_WH_StateInStartChildKeyShare()) {
                    if (wh_trace != NULL) {
                        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StateInStartChildKeyShare failed\n");
                    }
                    DWCi_MOV_WH_Finalize();
                }
                return;
            }
        } else if (wh->sConnectMode == DWCi_MOV_WH_CONNECTMODE_DS_CHILD) {
            result = WM_StartDataSharing(&wh->sDSInfo, 0xd, 7, 0x44, TRUE);
            if (result != WM_ERRCODE_SUCCESS) {
                DWCi_MOV_WH_SetError(result);
                DWCi_MOV_WH_Finalize();
                return;
            }
            if (wh_trace != NULL) {
                wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StateOutStartChildMP : WM_StartDataSharing OK\n");
            }
            DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_DATASHARING);
            return;
        }
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_CONNECTED);
        break;
    case WM_STATECODE_MP_IND:
        break;
    case WM_STATECODE_MPACK_IND:
        break;
    case WM_STATECODE_MPEND_IND:
    default:
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "unknown indicate, state = %d\n", cb->state);
        }
        break;
    }
}

int DWCi_MOV_WH_StateInStartChildKeyShare(void) {
    WMErrCode result;

    if (wh->sSysState == DWCi_MOV_WH_SYSSTATE_KEYSHARING) {
        return TRUE;
    }
    if (wh->sSysState != DWCi_MOV_WH_SYSSTATE_CONNECTED) {
        return FALSE;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_KEYSHARING);
    result = WM_StartKeySharing(&wh->sWMKeySetBuf, 0xd);
    if (result != WM_ERRCODE_OPERATING) {
        DWCi_MOV_WH_SetError(result);
        return FALSE;
    }
    return TRUE;
}

int DWCi_MOV_WH_StateInEndChildKeyShare(void) {
    WMErrCode result;

    if (wh->sSysState != DWCi_MOV_WH_SYSSTATE_KEYSHARING) {
        return FALSE;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    result = WM_EndKeySharing(&wh->sWMKeySetBuf);
    if (result != WM_ERRCODE_OPERATING) {
        DWCi_MOV_WH_SetError(result);
        return FALSE;
    }
    return TRUE;
}

int DWCi_MOV_WH_StateInEndChildMP(void) {
    WMErrCode result;

    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    result = WM_EndMP(DWCi_MOV_WH_StateOutEndChildMP);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    return FALSE;
}

void DWCi_MOV_WH_StateOutEndChildMP(void *arg) {
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_Finalize();
        return;
    }
    if (!DWCi_MOV_WH_StateInEndChild()) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    }
}

int DWCi_MOV_WH_StateInEndChild(void) {
    WMErrCode result;

    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    result = WM_Disconnect(DWCi_MOV_WH_StateOutEndChild, 0);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    DWCi_MOV_WH_Reset();
    return FALSE;
}

void DWCi_MOV_WH_StateOutEndChild(void *arg) {
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        return;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_IDLE);
}

int DWCi_MOV_WH_StateInReset(void) {
    WMErrCode result;

    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    result = WM_Reset(DWCi_MOV_WH_StateOutReset);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    return FALSE;
}

void DWCi_MOV_WH_StateOutReset(void *arg) {
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        DWCi_MOV_WH_SetError(cb->errcode);
        return;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_IDLE);
}

void DWCi_MOV_WH_StateOutEnd(void *arg) {
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_FATAL);
        return;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_STOP);
}

void DWCi_MOV_WH_SetGgid(u32 ggid) {
    wh->sParentParam.ggid = ggid;
}

u16 DWCi_MOV_WH_GetBitmap(void) {
    return wh->sConnectBitmap;
}

u32 DWCi_MOV_WH_GetSystemState(void) {
    return wh->sSysState;
}

u32 DWCi_MOV_WH_StartMeasureChannel(void) {
    u16 macAddr[3];
    u16 result;

    OS_GetMacAddress((u8 *)macAddr);
    wh->sRand = macAddr[2] + (macAddr[1] + (macAddr[0] + OS_GetVBlankCount()));
    wh->sRand = wh->sRand * 0x10DCD + 0x3039;
    wh->sChannel = 0;
    wh->sChannelBusyRatio = 0x65;
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    result = DWCi_MOV_WH_StateInMeasureChannel(1);
    if (result == DWCi_MOV_WH_ERRCODE_NOMORE_CHANNEL) {
        DWCi_MOV_WH_SetError(DWCi_MOV_WH_ERRCODE_NOMORE_CHANNEL);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return FALSE;
    }
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    return FALSE;
}

u16 DWCi_MOV_WH_StateInMeasureChannel(u16 channel) {
    u16 allowedChannel;

    allowedChannel = WM_GetAllowedChannel();
    if (allowedChannel == 0x8000) {
        DWCi_MOV_WH_SetError(WM_ERRCODE_ILLEGAL_STATE);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    if (allowedChannel == 0) {
        DWCi_MOV_WH_SetError(DWCi_MOV_WH_ERRCODE_NO_RADIO);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return DWCi_MOV_WH_ERRCODE_NOMORE_CHANNEL;
    }
    while ((allowedChannel & (1 << (channel - 1))) == 0) {
        channel++;
        if (channel > 0x10) {
            return DWCi_MOV_WH_ERRCODE_NOMORE_CHANNEL;
        }
    }
    return WHi_MeasureChannel(DWCi_MOV_WH_StateOutMeasureChannel, channel);
}

void DWCi_MOV_WH_StateOutMeasureChannel(void *arg) {
    WMMeasureChannelCallback *cb = (WMMeasureChannelCallback *)arg;
    u16 channel;
    u16 result;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        return;
    }
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "channel %d bratio = %x\n", cb->channel, cb->ccaBusyRatio);
    }
    channel = cb->channel;
    if (wh->sChannelBusyRatio > cb->ccaBusyRatio) {
        wh->sChannelBusyRatio = cb->ccaBusyRatio;
        wh->sChannelBitmap = 1 << (channel - 1);
    } else if (wh->sChannelBusyRatio == cb->ccaBusyRatio) {
        wh->sChannelBitmap |= 1 << (channel - 1);
    }
    result = DWCi_MOV_WH_StateInMeasureChannel(channel + 1);
    if (result == DWCi_MOV_WH_ERRCODE_NOMORE_CHANNEL) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_MEASURECHANNEL);
        return;
    }
    if (result == WM_ERRCODE_OPERATING) {
        return;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
}

WMErrCode WHi_MeasureChannel(WMCallbackFunc func, u16 channel) {
    return WM_MeasureChannel(func, 3, 0x11, channel, 0x1e);
}

u16 DWCi_MOV_WH_GetMeasureChannel(void) {
    if (wh->sSysState != DWCi_MOV_WH_SYSSTATE_MEASURECHANNEL) {
        OS_Terminate();
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_IDLE);
    wh->sChannel = SelectChannel(wh->sChannelBitmap);
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "decided channel = %d\n", wh->sChannel);
    }
    return wh->sChannel;
}

s16 SelectChannel(u16 bitmap) {
    s16 i;
    s16 channel = 0;
    u16 num = 0;
    u16 select;

    for (i = 0; i < 16; i++) {
        if (bitmap & (1 << i)) {
            channel = (s16)(i + 1);
            num++;
        }
    }
    if (num <= 1) {
        return channel;
    }
    wh->sRand = wh->sRand * 0x10DCD + 0x3039;
    select = (num * (u8)wh->sRand) >> 8;
    for (i = 0; i < 16; i++) {
        if (bitmap & 1) {
            if (select == 0) {
                return (s16)(i + 1);
            }
            select--;
        }
        bitmap >>= 1;
    }
    return 0;
}

BOOL DWCi_MOV_WH_Initialize(void) {
    wh->sRecvBufferSize = 0;
    wh->sSendBufferSize = 0;
    wh->sReceiverFunc = NULL;
    wh->sMyAid = 0;
    wh->sConnectBitmap = 1;
    wh->sErrCode = WM_ERRCODE_SUCCESS;
    wh->sParentParam.userGameInfo = NULL;
    wh->sParentParam.userGameInfoLength = 0;
    wh->sJudgeAcceptFunc = NULL;
    // pokebw2's switch: the if forms (!, == FALSE, != FALSE, ?:) keep 0 and 1 in the other registers
    switch (DWCi_MOV_WH_StateInInitialize()) {
    case FALSE:
        return FALSE;
    default:
        return TRUE;
    }
}

void DWCi_MOV_WH_IndicateHandler(void *arg) {
    WMIndCallback *cb = (WMIndCallback *)arg;

    if (cb->errcode == WM_ERRCODE_FIFO_ERROR) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
        OS_Terminate();
    }
}

int DWCi_MOV_WH_StateInInitialize(void) {
    WMErrCode result;

    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    result = WM_Initialize(wh->sWmBuffer, DWCi_MOV_WH_StateOutInitialize, 2);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_FATAL);
    return FALSE;
}

void DWCi_MOV_WH_StateOutInitialize(void *arg) {
    WMErrCode result;
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(cb->errcode);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_FATAL);
        return;
    }
    result = WM_SetIndCallback(DWCi_MOV_WH_IndicateHandler);
    if (result != WM_ERRCODE_SUCCESS) {
        DWCi_MOV_WH_SetError(result);
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_FATAL);
        return;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_IDLE);
}

u32 func_ov016_021d376c(s32 mode, u16 tgid, u16 channel, u32 param_4) {
    if (wh->sSysState != DWCi_MOV_WH_SYSSTATE_IDLE) {
        OS_Terminate();
    }
    wh->sRecvBufferSize = 0x180;
    wh->sSendBufferSize = 0xe0;
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "recv buffer size = %d\n", wh->sRecvBufferSize);
    }
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "send buffer size = %d\n", wh->sSendBufferSize);
    }
    wh->sConnectMode = mode;
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    wh->sParentParam.tgid = tgid;
    wh->sParentParam.channel = channel;
    wh->sParentParam.beaconPeriod = WM_GetDispersionBeaconPeriod();
    wh->sParentParam.parentMaxSize = 0xd0;
    wh->sParentParam.childMaxSize = 0x44;
    wh->sParentParam.maxEntry = 2;
    wh->sParentParam.CS_Flag = 0;
    wh->sParentParam.multiBootFlag = 0;
    wh->sParentParam.entryFlag = 1;
    wh->sParentParam.KS_Flag = (mode == DWCi_MOV_WH_CONNECTMODE_KS_PARENT) ? TRUE : FALSE;
    switch (mode) {
    case DWCi_MOV_WH_CONNECTMODE_MP_PARENT:
    case DWCi_MOV_WH_CONNECTMODE_KS_PARENT:
    case DWCi_MOV_WH_CONNECTMODE_DS_PARENT:
        return DWCi_MOV_WH_StateInSetParentParam();
    default:
        break;
    }
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "unknown connect mode %d\n", mode);
    }
    return FALSE;
}

void DWCi_MOV_WH_SetJudgeAcceptFunc(u32 func) {
    wh->sJudgeAcceptFunc = (BOOL (*)(WMStartParentCallback *))func;
}

u16 *DWCi_MOV_WH_GetSharedDataAdr(u16 aid) {
    return WM_GetSharedDataAddress(&wh->sDSInfo, &wh->sDataSet, aid);
}

u32 func_ov016_021d3904(u16 *data) {
    WMErrCode result;

    result = WM_StepDataSharing(&wh->sDSInfo, data, &wh->sDataSet);
    if (result == WM_ERRCODE_NO_CHILD) {
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StepDataSharing - Warning No Child\n");
        }
        return FALSE;
    }
    if (result == WM_ERRCODE_NO_DATASET) {
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_StepDataSharing - Warning No DataSet\n");
        }
        DWCi_MOV_WH_SetError(result);
        return FALSE;
    }
    if (result == WM_ERRCODE_SUCCESS) {
        return TRUE;
    }
    DWCi_MOV_WH_SetError(result);
    return FALSE;
}

void DWCi_MOV_WH_Reset(void) {
    if (!DWCi_MOV_WH_StateInReset()) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_FATAL);
    }
}

void DWCi_MOV_WH_Finalize(void) {
    if (wh->sSysState == DWCi_MOV_WH_SYSSTATE_IDLE) {
        if (wh_trace != NULL) {
            wh_trace(DWCi_MOV_WH_TRACE_FLAG, "already DWCi_MOV_WH_SYSSTATE_IDLE\n");
        }
        return;
    }
    if (wh_trace != NULL) {
        wh_trace(DWCi_MOV_WH_TRACE_FLAG, "DWCi_MOV_WH_Finalize, state = %d\n", wh->sSysState);
    }
    if (wh->sSysState != DWCi_MOV_WH_SYSSTATE_KEYSHARING && wh->sSysState != DWCi_MOV_WH_SYSSTATE_DATASHARING
        && wh->sSysState != DWCi_MOV_WH_SYSSTATE_CONNECTED) {
        DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
        DWCi_MOV_WH_Reset();
        return;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    switch (wh->sConnectMode) {
    case DWCi_MOV_WH_CONNECTMODE_KS_CHILD:
        if (!DWCi_MOV_WH_StateInEndChildKeyShare()) {
            DWCi_MOV_WH_Reset();
        }
        return;
    case DWCi_MOV_WH_CONNECTMODE_MP_CHILD:
    case DWCi_MOV_WH_CONNECTMODE_DS_CHILD:
        if (!DWCi_MOV_WH_StateInEndChildMP()) {
            DWCi_MOV_WH_Reset();
        }
        return;
    case DWCi_MOV_WH_CONNECTMODE_KS_PARENT:
        if (!DWCi_MOV_WH_StateInEndParentKeyShare()) {
            DWCi_MOV_WH_Reset();
        }
        return;
    case DWCi_MOV_WH_CONNECTMODE_MP_PARENT:
    case DWCi_MOV_WH_CONNECTMODE_DS_PARENT:
        if (!DWCi_MOV_WH_StateInEndParentMP()) {
            DWCi_MOV_WH_Reset();
        }
        return;
    }
}

u32 DWCi_MOV_WH_End(void) {
    if (wh->sSysState != DWCi_MOV_WH_SYSSTATE_IDLE) {
        OS_Terminate();
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_BUSY);
    if (WM_End(DWCi_MOV_WH_StateOutEnd) == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    DWCi_MOV_WH_ChangeSysState(DWCi_MOV_WH_SYSSTATE_ERROR);
    return FALSE;
}
