// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov030/wih.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "gfl/wih.h"
#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/net_whpipe.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/wm.h"
#include "work_area.h"

// wih.c is named after its embedded string. It is the wireless helper of the GFL net, which grew out of NitroSDK's demo
// wh.c: the names that follow its functions are the ones it seems to be, and the variable and its fields keep the
// look of its names (the string of the assert shows data_ov014_021797a8->sScanCallback). It runs a chain of WM calls, each of
// them with a callback that starts the next, and keeps the state of the chain in sSysState

// The work of the helper, which one allocation holds (this and 0x20 bytes of cache line slack)
typedef struct {
    WMParentParam sParentParam;
    u8 unk_38[8];
    // The buffer that WM works in, WM_SYSTEM_BUF_SIZE (its structs hold pointers: libntr's is 0x1e00 on a PC target)
    u8 sWmBuf[WORK_AREA_SIZE(0xf00)];
    // The copy of the beacon that the scan is looking at
    WMBssDesc sBssDesc;
    u8 sScanBuf[0x400];
    WMScanExParam sScanParam;
    WMDataSharingInfo sDataSharing;
    WMDataSet sDataSet;
    // What runs when the start or the end of the wireless is done
    WHCallback sCallback;
    u8 *sSendBuf;
    u8 *sRecvBuf;
    int sSysState;
    // How this machine connects (WH_CONNECTMODE_*)
    int sConnectMode;
    WHReceiver sReceiver;
    NetScanFilter sScanFilter;
    // What runs with the AID of a machine that has disconnected. Nothing sets it
    void (*sDisconnectCallback)(int netId);
    WHConnectCallback sConnectCallback;
    WHScanInfoCallback sScanInfoCallback;
    u32 sSendBufSize;
    u32 sRecvBufSize;
    WHScanCallback sScanCallback;
    // The error that was reported first
    int sLastError;
    u32 sSeed;
    u8 unk_1ea0[8];
    u16 sAid;
    // The AIDs that are connected, a bit each (this machine's is 0)
    u16 sBitmap;
    u16 sChannel;
    // The least busy that a channel was, and the channels that were as little busy
    u16 sMinBusy;
    u16 sBestBitmap;
    // The channel to scan, 0 for all of them in turn
    u16 sScanChannel;
    // Whether to connect to the parent that the scan has found
    u16 sAutoConnect;
    u8 unk_1eb6[4];
    u16 sBeaconCount;
    u16 sScanCount;
    // The frames until the scan starts again
    s16 sScanWait;
    HeapID sHeapId;
    // The frames that it waits to start the scan again
    u16 sScanTime;
    u8 sSsid[0x1a];
    // Whether the scan is not to start again
    u8 sScanPaused;
    // Whether the scan has been asked to end
    u8 sScanEnding;
} WIH;

// NitroSDK's DC_InvalidateRange, which no header declares yet
void DC_InvalidateRange(void *startAddr, u32 nBytes);
static void func_ov014_02178138(int state);
static void func_ov014_0217814c(int errcode);
static BOOL func_ov014_02178168(void);
static void func_ov014_02178198(void *arg);
static BOOL func_ov014_021781bc(void);
static void func_ov014_02178204(void *arg);
static BOOL func_ov014_02178304(void);
void func_ov014_021783fc(void *arg);
static BOOL func_ov014_021784ac(void);
static void func_ov014_021784d0(void *arg);
static BOOL func_ov014_021784f0(void);
static void func_ov014_0217850c(void *arg);
static BOOL func_ov014_0217863c(void);
BOOL func_ov014_0217873c(const WMBssDesc *bssDesc);
static void func_ov014_02178788(void *arg);
BOOL func_ov014_02178964(void);
void func_ov014_02178980(void *arg);
BOOL func_ov014_021789c8(void);
void func_ov014_02178a18(void *arg);
static BOOL func_ov014_02178ab8(void);
void func_ov014_02178b98(void *arg);
static BOOL func_ov014_02178c50(void);
static void func_ov014_02178c74(void *arg);
static BOOL func_ov014_02178c98(void);
static void func_ov014_02178cc4(void *arg);
static BOOL func_ov014_02178cdc(void);
static void func_ov014_02178d00(void *arg);
static BOOL func_ov014_02178d20(const void *data, u16 size, WHSendCallback callback);
static void func_ov014_02178d74(void *arg);
static void func_ov014_02178d9c(void *arg);
static void func_ov014_02178de0(void *arg);
static u16 func_ov014_02178f1c(u16 channel);
static void func_ov014_02178f8c(void *arg);
static int func_ov014_02179010(WMCallbackFunc callback, u16 channel);
static s16 func_ov014_02179050(u16 bitmap);
void func_ov014_02179188(void *arg);
BOOL func_ov014_021791a0(WHCallback callback, u32 extended);
void func_ov014_021791ec(void *arg);
void func_ov014_021791f8(void *arg);

// pokebw2's _pWmInfo
static WIH *data_ov014_021797a8;

// pokebw2's WH_ChangeSysState
static void func_ov014_02178138(int state) {
    data_ov014_021797a8->sSysState = state;
}

// Keeps the first error, until the state is that of an error
// pokebw2's WH_ReportError
static void func_ov014_0217814c(int errcode) {
    if ((u32)(data_ov014_021797a8->sSysState - WH_SYSSTATE_ERROR) > 1) {
        data_ov014_021797a8->sLastError = errcode;
    }
}

// pokebw2's WH_StateInSetParentParam
static BOOL func_ov014_02178168(void) {
    int result;

    func_ov014_02178138(WH_SYSSTATE_BUSY);
    result = WM_SetParentParameter(func_ov014_02178198, &data_ov014_021797a8->sParentParam);
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutSetParentParam
static void func_ov014_02178198(void *arg) {
    WMCallback *cb = arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return;
    }
    if (!func_ov014_021781bc()) {
        func_ov014_02178138(WH_SYSSTATE_ERROR);
    }
}

// pokebw2's WH_StateInStartParent
static BOOL func_ov014_021781bc(void) {
    int result;

    if (data_ov014_021797a8->sSysState == WH_SYSSTATE_CONNECTED || data_ov014_021797a8->sSysState == WH_SYSSTATE_DATASHARING) {
        return TRUE;
    }
    result = WM_StartParent(func_ov014_02178204);
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    data_ov014_021797a8->sAid = 0;
    data_ov014_021797a8->sBitmap = 1;
    return TRUE;
}

// pokebw2's WH_StateOutStartParent
static void func_ov014_02178204(void *arg) {
    WMStartParentCallback *cb = arg;
    BOOL accept = TRUE;
    u16 bit = 1 << cb->aid;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_BEACON_SENT:
        data_ov014_021797a8->sBeaconCount++;
        break;
    case WM_STATECODE_CONNECTED:
        if (data_ov014_021797a8->sScanFilter != NULL) {
            accept = data_ov014_021797a8->sScanFilter(cb, func_0203d650());
        }
        if (!accept) {
            int result = WM_Disconnect(NULL, cb->aid);

            if (result != WM_ERRCODE_OPERATING) {
                func_ov014_0217814c(result);
                func_ov014_02178138(WH_SYSSTATE_ERROR);
            }
            return;
        }
        data_ov014_021797a8->sBitmap |= bit;
        if (data_ov014_021797a8->sConnectCallback != NULL) {
            data_ov014_021797a8->sConnectCallback(cb->aid);
        }
        break;
    case WM_STATECODE_DISCONNECTED:
    case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
        if (data_ov014_021797a8->sBitmap & (1 << cb->aid)) {
            data_ov014_021797a8->sBitmap &= ~bit;
            if (data_ov014_021797a8->sDisconnectCallback != NULL) {
                data_ov014_021797a8->sDisconnectCallback(cb->aid);
            }
            func_ov014_0217807c();
            func_0203a338(cb->aid);
            func_02039a2c(cb->aid);
        }
        break;
    case WM_STATECODE_PARENT_START:
        if (!func_ov014_02178304()) {
            func_ov014_02178138(WH_SYSSTATE_ERROR);
        }
        break;
    }
}

// pokebw2's WH_StateInStartParentMP
static BOOL func_ov014_02178304(void) {
    int result;

    if (data_ov014_021797a8->sSysState == WH_SYSSTATE_CONNECTED
        || data_ov014_021797a8->sSysState == WH_SYSSTATE_DATASHARING) {
        return TRUE;
    }
    func_ov014_02178138(WH_SYSSTATE_CONNECTED);
    WM_GetMPReceiveBufferSize();
    WM_GetMPSendBufferSize();
    if (data_ov014_021797a8->sRecvBuf != NULL) {
        func_0203d7fc(data_ov014_021797a8->sRecvBuf);
        func_0203d7fc(data_ov014_021797a8->sSendBuf);
    }
    data_ov014_021797a8->sRecvBufSize = WM_GetMPReceiveBufferSize();
    data_ov014_021797a8->sSendBufSize = WM_GetMPSendBufferSize();
    data_ov014_021797a8->sRecvBuf = allocConfigDSSoftwareFeature(data_ov014_021797a8->sHeapId,
                                                                 data_ov014_021797a8->sRecvBufSize + 0x20, "wih.c", 1191);
    data_ov014_021797a8->sSendBuf = allocConfigDSSoftwareFeature(data_ov014_021797a8->sHeapId,
                                                                 data_ov014_021797a8->sSendBufSize + 0x20, "wih.c", 1192);
    result = WM_StartMP(func_ov014_021783fc, data_ov014_021797a8->sRecvBuf, data_ov014_021797a8->sRecvBufSize,
                           data_ov014_021797a8->sSendBuf, data_ov014_021797a8->sSendBufSize,
                           func_0203d5d4() == 2 ? 1 : 2);
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutStartParentMP
void func_ov014_021783fc(void *arg) {
    WMStartMPCallback *cb = arg;
    u16 aidBitmap;
    int machines;
    int lastAid;
    BOOL doubleMode;
    int result;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_MP_START:
        if (data_ov014_021797a8->sConnectMode == WH_CONNECTMODE_DS_PARENT) {
            machines = func_0203d6bc();
            lastAid = func_0203d680() - 1;
            aidBitmap = (1 << (lastAid + 1)) - 1;
            doubleMode = TRUE;

            if (func_0203d5d4() == 0x2) {
                doubleMode = FALSE;
            }
            result = WM_StartDataSharing(&data_ov014_021797a8->sDataSharing, 13, aidBitmap, machines, doubleMode);
            if (result != 0) {
                func_ov014_0217814c(result);
                func_ov014_02178138(WH_SYSSTATE_ERROR);
                return;
            }
            func_ov014_02178138(WH_SYSSTATE_DATASHARING);
        } else {
            func_ov014_02178138(WH_SYSSTATE_CONNECTED);
        }
        break;
    case WM_STATECODE_MPEND_IND:
    case WM_STATECODE_MP_IND:
    case WM_STATECODE_MPACK_IND:
        break;
    }
}

// pokebw2's WH_StateInEndParentMP
static BOOL func_ov014_021784ac(void) {
    int result;

    func_ov014_02178138(WH_SYSSTATE_BUSY);
    result = WM_EndMP(func_ov014_021784d0);
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutEndParentMP
static void func_ov014_021784d0(void *arg) {
    WMCallback *cb = arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        func_ov014_0217945c();
        return;
    }
    if (!func_ov014_021784f0()) {
        func_ov014_0217945c();
    }
}

// pokebw2's WH_StateInEndParent
static BOOL func_ov014_021784f0(void) {
    int result = WM_EndParent(func_ov014_0217850c);

    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutEndParent
static void func_ov014_0217850c(void *arg) {
    WMCallback *cb = arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        return;
    }
    func_ov014_02178138(WH_SYSSTATE_IDLE);
}

// func_ov014_02178524: scans for the parent of the MAC address, and connects to it
// pokebw2's WH_ChildConnectAuto
BOOL func_ov014_02178524(u16 mode, const u8 *mac, u16 channel) {
    const u16 *bssid = (const u16 *)mac;

    func_ov014_02178138(WH_SYSSTATE_SCANNING);
    if (mac != NULL) {
        ((u16 *)data_ov014_021797a8->sScanParam.bssid)[2] = bssid[2];
        ((u16 *)data_ov014_021797a8->sScanParam.bssid)[1] = bssid[1];
        ((u16 *)data_ov014_021797a8->sScanParam.bssid)[0] = bssid[0];
    } else {
        MI_CpuFill8(data_ov014_021797a8->sScanParam.bssid, 0xff, 6);
    }
    data_ov014_021797a8->sConnectMode = mode;
    data_ov014_021797a8->sScanCallback = NULL;
    data_ov014_021797a8->sScanChannel = channel;
    data_ov014_021797a8->sScanParam.channelList = 1;
    data_ov014_021797a8->sAutoConnect = 1;
    if (!func_ov014_0217863c()) {
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

// func_ov014_021785b4: scans for parents, giving the callback each one it finds
// pokebw2's WH_StartScan
BOOL func_ov014_021785b4(WHScanCallback callback, const u8 *mac, u16 channel) {
    const u16 *bssid = (const u16 *)mac;

    func_ov014_02178138(WH_SYSSTATE_SCANNING);
    data_ov014_021797a8->sScanCallback = callback;
    data_ov014_021797a8->sScanChannel = channel;
    data_ov014_021797a8->sScanParam.channelList = 1;
    data_ov014_021797a8->sAutoConnect = 0;
    data_ov014_021797a8->sScanCount = 0;
    if (mac != NULL) {
        ((u16 *)data_ov014_021797a8->sScanParam.bssid)[2] = bssid[2];
        ((u16 *)data_ov014_021797a8->sScanParam.bssid)[1] = bssid[1];
        ((u16 *)data_ov014_021797a8->sScanParam.bssid)[0] = bssid[0];
    } else {
        MI_CpuFill8(data_ov014_021797a8->sScanParam.bssid, 0xff, 6);
    }
    if (!func_ov014_0217863c()) {
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

// Starts a scan of the next channel, or of the one the channel given
// pokebw2's WH_StateInStartScan
static BOOL func_ov014_0217863c(void) {
    u16 allowed = WM_GetAllowedChannel();
    int result;

    if (allowed == 0x8000) {
        func_ov014_0217814c(3);
        return FALSE;
    }
    if (allowed == 0) {
        func_ov014_0217814c(0x16);
        return FALSE;
    }
    if (data_ov014_021797a8->sScanChannel == 0) {
        int i;
        u32 list;
        int bit;

        list = data_ov014_021797a8->sScanParam.channelList;
        for (i = 0; i < 32; i++) {
            if (list & 1) {
                break;
            }
            list >>= 1;
        }
        list = i + 1;
        do {
            list++;
            if (list > 16) {
                list = 1;
            }
            bit = 1 << (list - 1);
        } while (!(allowed & bit));
        data_ov014_021797a8->sScanParam.channelList = bit;
    } else {
        data_ov014_021797a8->sScanParam.channelList = 1 << (data_ov014_021797a8->sScanChannel - 1);
    }
    data_ov014_021797a8->sScanParam.maxChannelTime = WM_GetDispersionScanPeriod();
    data_ov014_021797a8->sScanParam.scanBuf = (WMBssDesc *)data_ov014_021797a8->sScanBuf;
    data_ov014_021797a8->sScanParam.scanBufSize = sizeof(data_ov014_021797a8->sScanBuf);
    data_ov014_021797a8->sScanParam.scanType = 1;
    data_ov014_021797a8->sScanParam.ssidLength = 0;
    result = WM_StartScanEx(func_ov014_02178788, &data_ov014_021797a8->sScanParam);
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    return TRUE;
}

// func_ov014_02178714: sets the channel and the MAC address that the scan looks for
// pokebw2's WH_SetScanTarget
void func_ov014_02178714(u16 channel, const u8 *mac) {
    data_ov014_021797a8->sScanChannel = channel;
    MI_CpuCopy8(mac, data_ov014_021797a8->sScanParam.bssid, 6);
}

// Whether the beacon is a valid one of this game's, from the length of its data, its magic number, its GGID and its
// attributes
// pokebw2's WH_CheckBssDesc
BOOL func_ov014_0217873c(const WMBssDesc *bssDesc) {
    u16 length = bssDesc->gameInfoLength;

    if (length < 0x10 || length > 0x80 || length != bssDesc->gameInfo.userGameInfoLength + 0x10
        || bssDesc->gameInfo.magicNumber != 1 || length < 8 || bssDesc->gameInfo.ggid != data_ov014_021797a8->sParentParam.ggid) {
        return FALSE;
    }
    if ((bssDesc->gameInfo.gameNameCount_attribute & 3) == 1) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WH_StateOutStartScan
static void func_ov014_02178788(void *arg) {
    WMStartScanExCallback *cb = arg;
    u16 state = cb->state;
    BOOL found;
    int i;

    data_ov014_021797a8->sScanCount++;
    data_ov014_021797a8->sScanWait = 0;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return;
    }
    if (data_ov014_021797a8->sSysState != WH_SYSSTATE_SCANNING) {
        data_ov014_021797a8->sAutoConnect = 0;
        if (!func_ov014_02178964()) {
            func_ov014_02178138(WH_SYSSTATE_ERROR);
        }
        return;
    }
    data_ov014_021797a8->sScanWait = data_ov014_021797a8->sScanTime;
    switch (state) {
    case WM_STATECODE_PARENT_NOT_FOUND:
        func_02010884();
        return;
    case WM_STATECODE_PARENT_FOUND:
        break;
    default:
        return;
    }
    if (cb->bssDescCount != 0) {
        DC_InvalidateRange(data_ov014_021797a8->sScanBuf, sizeof(data_ov014_021797a8->sScanBuf));
    }
    found = FALSE;
    for (i = 0; i < cb->bssDescCount; i++) {
        WIH *work = data_ov014_021797a8;

        MI_CpuCopy8(cb->bssDesc[i], &work->sBssDesc, sizeof(WMBssDesc));
        func_02010864(cb->linkLevel[i]);
        if (data_ov014_021797a8->sScanInfoCallback != NULL) {
            data_ov014_021797a8->sScanInfoCallback(&work->sBssDesc, func_0203d650(), cb->linkLevel[i]);
        }
        if (func_ov014_0217873c(&work->sBssDesc)) {
            GFL_ASSERT(data_ov014_021797a8->sScanCallback);
            if (data_ov014_021797a8->sScanCallback(&work->sBssDesc)) {
                found = TRUE;
            }
        }
    }
    if (data_ov014_021797a8->sAutoConnect != 0 && found) {
        data_ov014_021797a8->sScanWait = 0;
        if (!func_ov014_02178964()) {
            func_ov014_02178138(WH_SYSSTATE_ERROR);
        }
    }
}

// func_ov014_021788fc: stops the scan, once the one under way is done. Whether there was one
// pokebw2's WH_EndScan
BOOL func_ov014_021788fc(void) {
    u32 irq;
    BOOL stopped = FALSE;

    irq = OS_DisableInterrupts();

    if (data_ov014_021797a8->sSysState == WH_SYSSTATE_SCANNING) {
        if (data_ov014_021797a8->sScanWait != 0) {
            if (!func_ov014_0217863c()) {
                func_ov014_02178138(WH_SYSSTATE_ERROR);
            }
        }
        data_ov014_021797a8->sScanWait = -1;
        data_ov014_021797a8->sAutoConnect = 0;
        stopped = TRUE;
        data_ov014_021797a8->sScanEnding = TRUE;
        func_ov014_02178138(WH_SYSSTATE_BUSY);
    }
    OS_RestoreInterrupts(irq);
    return stopped;
}

// pokebw2's WH_StateInEndScan
BOOL func_ov014_02178964(void) {
    int result = WM_EndScan(func_ov014_02178980);

    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutEndScan
void func_ov014_02178980(void *arg) {
    WMCallback *cb = arg;

    data_ov014_021797a8->sScanEnding = FALSE;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        return;
    }
    func_ov014_02178138(WH_SYSSTATE_IDLE);
    if (data_ov014_021797a8->sAutoConnect != 0) {
        data_ov014_021797a8->sAutoConnect = 0;
        if (!func_ov014_021789c8()) {
            func_ov014_02178138(WH_SYSSTATE_ERROR);
        }
    }
}

// pokebw2's WH_StateInStartChild
BOOL func_ov014_021789c8(void) {
    int result;

    if (data_ov014_021797a8->sSysState == WH_SYSSTATE_CONNECTED || data_ov014_021797a8->sSysState == WH_SYSSTATE_DATASHARING) {
        return TRUE;
    }
    func_ov014_02178138(WH_SYSSTATE_BUSY);
    result = WM_StartConnectEx(func_ov014_02178a18, &data_ov014_021797a8->sBssDesc, data_ov014_021797a8->sSsid, TRUE, 0);
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutStartChild
void func_ov014_02178a18(void *arg) {
    WMStartConnectCallback *cb = arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        if (cb->errcode == 0xc) {
            func_ov014_02178138(WH_SYSSTATE_ERROR);
        } else if (cb->errcode == 0xb) {
            func_ov014_02178138(WH_SYSSTATE_ERROR);
        } else if (cb->errcode == 1) {
            func_ov014_02178138(WH_SYSSTATE_CONNECT_FAIL);
        } else {
            func_ov014_02178138(WH_SYSSTATE_ERROR);
        }
        return;
    }
    if (cb->state == 8) {
        return;
    }
    if (cb->state == WM_STATECODE_CONNECTED) {
        func_ov014_02178138(WH_SYSSTATE_CONNECTED);
        if (!func_ov014_02178ab8()) {
            func_ov014_02178138(WH_SYSSTATE_BUSY);
            return;
        }
        data_ov014_021797a8->sAid = cb->aid;
        if (data_ov014_021797a8->sConnectCallback != NULL) {
            data_ov014_021797a8->sConnectCallback(cb->aid);
        }
    } else if (cb->state == 6) {
        return;
    } else if (cb->state == WM_STATECODE_DISCONNECTED) {
        func_ov014_0217814c(0x14);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
    } else if (cb->state == WM_STATECODE_DISCONNECTED_FROM_MYSELF) {
        return;
    } else {
        func_ov014_02178138(WH_SYSSTATE_ERROR);
    }
}

// pokebw2's WH_StateInStartChildMP
static BOOL func_ov014_02178ab8(void) {
    int result;

    WM_GetMPReceiveBufferSize();
    WM_GetMPSendBufferSize();
    if (data_ov014_021797a8->sRecvBuf != NULL) {
        func_0203d7fc(data_ov014_021797a8->sRecvBuf);
        func_0203d7fc(data_ov014_021797a8->sSendBuf);
    }
    data_ov014_021797a8->sRecvBufSize = WM_GetMPReceiveBufferSize();
    data_ov014_021797a8->sSendBufSize = WM_GetMPSendBufferSize();
    data_ov014_021797a8->sRecvBuf = allocConfigDSSoftwareFeature(data_ov014_021797a8->sHeapId,
                                                                 data_ov014_021797a8->sRecvBufSize + 0x20, "wih.c", 2042);
    data_ov014_021797a8->sSendBuf = allocConfigDSSoftwareFeature(data_ov014_021797a8->sHeapId,
                                                                 data_ov014_021797a8->sSendBufSize + 0x20, "wih.c", 2043);
    result = WM_StartMP(func_ov014_02178b98, data_ov014_021797a8->sRecvBuf, data_ov014_021797a8->sRecvBufSize,
                           data_ov014_021797a8->sSendBuf, data_ov014_021797a8->sSendBufSize,
                           func_0203d5d4() == 2 ? 1 : 2);
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutStartChildMP
void func_ov014_02178b98(void *arg) {
    WMStartMPCallback *cb = arg;
    u16 aidBitmap;
    int machines;
    int lastAid;
    BOOL doubleMode;
    int result;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        if (cb->errcode == 0xf || cb->errcode == 9 || cb->errcode == 0xd) {
            return;
        }
        func_ov014_0217814c(cb->errcode);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_MP_START:
        if (data_ov014_021797a8->sConnectMode == WH_CONNECTMODE_DS_CHILD) {
            machines = func_0203d6bc();
            lastAid = func_0203d680() - 1;
            aidBitmap = (1 << (lastAid + 1)) - 1;
            doubleMode = TRUE;

            if (func_0203d5d4() == 0x2) {
                doubleMode = FALSE;
            }
            result = WM_StartDataSharing(&data_ov014_021797a8->sDataSharing, 13, aidBitmap, machines, doubleMode);
            if (result != 0) {
                func_ov014_0217814c(result);
                func_ov014_02179498();
                return;
            }
            func_ov014_02178138(WH_SYSSTATE_DATASHARING);
        } else {
            func_ov014_02178138(WH_SYSSTATE_CONNECTED);
        }
        break;
    case WM_STATECODE_MPEND_IND:
    case WM_STATECODE_MP_IND:
    case WM_STATECODE_MPACK_IND:
        break;
    }
}

// pokebw2's WH_StateInEndChildMP
static BOOL func_ov014_02178c50(void) {
    int result;

    func_ov014_02178138(WH_SYSSTATE_BUSY);
    result = WM_EndMP(func_ov014_02178c74);
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutEndChildMP
static void func_ov014_02178c74(void *arg) {
    WMCallback *cb = arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        func_ov014_02179498();
        return;
    }
    if (!func_ov014_02178c98()) {
        func_ov014_02178138(WH_SYSSTATE_ERROR);
    }
}

// pokebw2's WH_StateInEndChild
static BOOL func_ov014_02178c98(void) {
    int result;

    func_ov014_02178138(WH_SYSSTATE_BUSY);
    result = WM_Disconnect(func_ov014_02178cc4, 0);
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        func_ov014_0217945c();
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutEndChild
static void func_ov014_02178cc4(void *arg) {
    WMCallback *cb = arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        return;
    }
    func_ov014_02178138(WH_SYSSTATE_IDLE);
}

// pokebw2's WH_StateInReset
static BOOL func_ov014_02178cdc(void) {
    int result;

    func_ov014_02178138(WH_SYSSTATE_BUSY);
    result = WM_Reset(func_ov014_02178d00);
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutReset
static void func_ov014_02178d00(void *arg) {
    WMCallback *cb = arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        func_ov014_0217814c(cb->errcode);
        return;
    }
    func_ov014_02178138(WH_SYSSTATE_IDLE);
}

// Sends the data to every machine on the port, and calls back once it is gone
// pokebw2's WH_SendMPData
static BOOL func_ov014_02178d20(const void *data, u16 size, WHSendCallback callback) {
    DC_FlushRange(data_ov014_021797a8->sSendBuf, data_ov014_021797a8->sSendBufSize);
    if (WM_SetMPDataToPortEx(func_ov014_02178d74, callback, data, size, 0xffff, 14, 2) == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WH_SendMPDataCallback
static void func_ov014_02178d74(void *arg) {
    WMPortSendCallback *cb = arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS && cb->errcode != 0xf) {
        func_ov014_0217814c(cb->errcode);
        return;
    }
    if (cb->arg != NULL) {
        ((WHSendCallback)cb->arg)(cb->errcode == WM_ERRCODE_SUCCESS);
    }
}

// pokebw2's WH_PortReceiveCallback
static void func_ov014_02178d9c(void *arg) {
    WMPortRecvCallback *cb = arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        return;
    }
    if (data_ov014_021797a8->sReceiver != NULL) {
        if (cb->state == WM_STATECODE_PORT_RECV) {
            data_ov014_021797a8->sReceiver(cb->aid, cb->data, cb->length);
        } else if (cb->state == WM_STATECODE_DISCONNECTED) {
            data_ov014_021797a8->sReceiver(cb->aid, NULL, 0);
        }
    }
}

// pokebw2's WH_StateOutEnd
static void func_ov014_02178de0(void *arg) {
    WMCallback *cb = arg;

    if (data_ov014_021797a8->sCallback != NULL) {
        data_ov014_021797a8->sCallback(cb->errcode == WM_ERRCODE_SUCCESS);
    }
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_02178138(WH_SYSSTATE_FATAL);
    } else {
        func_ov014_02178138(WH_SYSSTATE_STOP);
    }
}

// func_ov014_02178e1c
// pokebw2's WH_SetGgid
void func_ov014_02178e1c(u32 gameId) {
    data_ov014_021797a8->sParentParam.ggid = gameId;
}

// func_ov014_02178e28
// pokebw2's WH_SetUserGameInfo
void func_ov014_02178e28(const void *data, u16 size) {
    data_ov014_021797a8->sParentParam.userGameInfo = data;
    data_ov014_021797a8->sParentParam.userGameInfoLength = size;
}

// func_ov014_02178e38
// pokebw2's WH_GetBitmap
u16 func_ov014_02178e38(void) {
    return data_ov014_021797a8->sBitmap;
}

// func_ov014_02178e4c
// pokebw2's WH_GetSystemState
int func_ov014_02178e4c(void) {
    if (data_ov014_021797a8 == NULL) {
        return WH_SYSSTATE_STOP;
    }
    return data_ov014_021797a8->sSysState;
}

// func_ov014_02178e68
// pokebw2's WH_GetLastError
int func_ov014_02178e68(void) {
    if (data_ov014_021797a8 != NULL) {
        return data_ov014_021797a8->sLastError;
    }
    return 0;
}

// func_ov014_02178e84: measures how busy each channel is, one after the other
// pokebw2's WH_StartMeasureChannel
BOOL func_ov014_02178e84(void) {
    u16 mac[3];
    u16 result;

    OS_GetMacAddress((u8 *)mac);
    data_ov014_021797a8->sSeed = OS_GetVBlankCount() + mac[0] + mac[1] + mac[2];
    data_ov014_021797a8->sSeed = data_ov014_021797a8->sSeed * 69069 + 12345;
    data_ov014_021797a8->sChannel = 0;
    data_ov014_021797a8->sMinBusy = 101;
    func_ov014_02178138(WH_SYSSTATE_BUSY);
    result = func_ov014_02178f1c(1);
    if (result == 0x18) {
        func_ov014_0217814c(0x18);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

// Measures the next channel that is allowed, from the one given. 0x18 once there are none left
// pokebw2's WH_StateInMeasureChannel
static u16 func_ov014_02178f1c(u16 channel) {
    u16 allowed = WM_GetAllowedChannel();

    if (allowed == 0x8000) {
        func_ov014_0217814c(3);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return 3;
    }
    if (allowed == 0) {
        func_ov014_0217814c(0x16);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return 0x18;
    }
    if (!((1 << (channel - 1)) & allowed)) {
        do {
            channel++;
            if (channel > 16) {
                return 0x18;
            }
        } while (!((1 << (channel - 1)) & allowed));
    }
    return func_ov014_02179010(func_ov014_02178f8c, channel);
}

// pokebw2's WH_StateOutMeasureChannel
static void func_ov014_02178f8c(void *arg) {
    WMMeasureChannelCallback *cb = arg;
    u16 channel;
    u16 busy;
    u16 next;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return;
    }
    channel = cb->channel;
    busy = cb->ccaBusyRatio;
    if (data_ov014_021797a8->sMinBusy > busy) {
        data_ov014_021797a8->sMinBusy = busy;
        data_ov014_021797a8->sBestBitmap = 1 << (channel - 1);
    } else if (data_ov014_021797a8->sMinBusy == busy) {
        data_ov014_021797a8->sBestBitmap |= 1 << (channel - 1);
    }
    next = func_ov014_02178f1c(channel + 1);
    if (next == 0x18 && data_ov014_021797a8->sSysState != WH_SYSSTATE_ERROR) {
        func_ov014_02178138(WH_SYSSTATE_MEASURECHANNEL);
        return;
    }
    if (next != WM_ERRCODE_OPERATING) {
        func_ov014_02178138(WH_SYSSTATE_ERROR);
    }
}

// pokebw2's WH_MeasureChannel
static int func_ov014_02179010(WMCallbackFunc callback, u16 channel) {
    return WM_MeasureChannel(callback, 3, 0x11, channel, 30);
}

// func_ov014_02179024: the channel to use, one of the least busy ones, chosen at random
// pokebw2's WH_GetMeasureChannel
u16 func_ov014_02179024(void) {
    func_ov014_02178138(WH_SYSSTATE_IDLE);
    data_ov014_021797a8->sChannel = func_ov014_02179050(data_ov014_021797a8->sBestBitmap);
    return data_ov014_021797a8->sChannel;
}

// pokebw2's WH_ChooseChannel
static s16 func_ov014_02179050(u16 bitmap) {
    s16 channel = 0;
    s16 i;
    u16 count = 0;
    u16 pick;

    for (i = 0; i < 16; i++) {
        if (bitmap & (1 << i)) {
            channel = i + 1;
            count++;
        }
    }
    if (count > 1) {
        data_ov014_021797a8->sSeed = data_ov014_021797a8->sSeed * 69069 + 12345;
        pick = (count * (u8)data_ov014_021797a8->sSeed) >> 8;
        for (i = 0; i < 16; i++) {
            if (bitmap & 1) {
                if (pick == 0) {
                    return i + 1;
                }
                pick--;
            }
            bitmap >>= 1;
        }
        return 0;
    }
    return channel;
}

// pokebw2's WH_Initialize
BOOL func_ov014_021790e4(HeapID heapId, WHCallback callback, u32 extended) {
    if (data_ov014_021797a8 != NULL) {
        return FALSE;
    }
    data_ov014_021797a8 = allocConfigDSSoftwareFeature(heapId, sizeof(WIH) + 0x20, "wih.c", 3123);
    MI_CpuFill8(data_ov014_021797a8, 0, sizeof(WIH));
    data_ov014_021797a8->sScanTime = 1;
    data_ov014_021797a8->sBitmap = 1;
    data_ov014_021797a8->sLastError = 0;
    data_ov014_021797a8->sHeapId = heapId;
    if (!func_ov014_021791a0(callback, extended)) {
        return FALSE;
    }
    return TRUE;
}

// func_ov014_02179158: frees the work and its buffers
// pokebw2's WH_Release
void func_ov014_02179158(void) {
    if (data_ov014_021797a8 != NULL) {
        func_0203d7fc(data_ov014_021797a8->sRecvBuf);
        func_0203d7fc(data_ov014_021797a8->sSendBuf);
        func_0203d7fc(data_ov014_021797a8);
        data_ov014_021797a8 = NULL;
    }
}

// pokebw2's WH_IndicationCallback
void func_ov014_02179188(void *arg) {
    WMCallback *cb = arg;

    if (cb->errcode == 8) {
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        func_ov014_0217814c(0x19);
    }
}

// Starts WM. Its first argument is not used
// pokebw2's WH_StateInInitialize
BOOL func_ov014_021791a0(WHCallback callback, u32 extended) {
    int result;

    func_ov014_02178138(WH_SYSSTATE_BUSY);
    if (extended == 0) {
        result = WM_Initialize(data_ov014_021797a8->sWmBuf, func_ov014_021791f8, 2);
    } else {
        result = WM_InitializeForListening(data_ov014_021797a8->sWmBuf, func_ov014_021791f8, 2, 0);
    }
    if (result != WM_ERRCODE_OPERATING) {
        func_ov014_0217814c(result);
        func_ov014_02178138(WH_SYSSTATE_FATAL);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WH_StateOutSetLifeTime
void func_ov014_021791ec(void *arg) {
    func_ov014_02178138(WH_SYSSTATE_IDLE);
}

// pokebw2's WH_StateOutInitialize
void func_ov014_021791f8(void *arg) {
    WMCallback *cb = arg;
    int result;

    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(cb->errcode);
        func_ov014_02178138(WH_SYSSTATE_FATAL);
        if (data_ov014_021797a8->sCallback != NULL) {
            data_ov014_021797a8->sCallback(FALSE);
        }
        return;
    }
    result = WM_SetIndCallback(func_ov014_02179188);
    if (result != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(result);
        func_ov014_02178138(WH_SYSSTATE_FATAL);
        if (data_ov014_021797a8->sCallback != NULL) {
            data_ov014_021797a8->sCallback(FALSE);
        }
        return;
    }
    func_ov014_02178138(WH_SYSSTATE_IDLE);
    WM_SetLifeTime(func_ov014_021791ec, 0xffff, 40, 5, 40);
    if (data_ov014_021797a8->sCallback != NULL) {
        data_ov014_021797a8->sCallback(TRUE);
    }
}

// func_ov014_02179288: starts as a parent, in the mode of the connection (the multi-point mode, the keys or the data shared)
// pokebw2's WH_ParentConnect
BOOL func_ov014_02179288(u16 mode, u16 tgid, u16 channel, u16 maxEntry) {
    data_ov014_021797a8->sBeaconCount = 0;
    data_ov014_021797a8->sConnectMode = mode;
    func_ov014_02178138(WH_SYSSTATE_BUSY);
    data_ov014_021797a8->sParentParam.tgid = tgid;
    data_ov014_021797a8->sParentParam.channel = channel;
    data_ov014_021797a8->sParentParam.beaconPeriod = WM_GetDispersionBeaconPeriod();
    if (func_0203d6cc() != 0) {
        data_ov014_021797a8->sParentParam.parentMaxSize = func_0203d6cc();
    } else {
        data_ov014_021797a8->sParentParam.parentMaxSize = func_0203d6bc() * func_0203d680() + 4;
    }
    data_ov014_021797a8->sParentParam.childMaxSize = func_0203d6bc();
    data_ov014_021797a8->sParentParam.maxEntry = maxEntry;
    data_ov014_021797a8->sParentParam.CS_Flag = 0;
    data_ov014_021797a8->sParentParam.multiBootFlag = 0;
    data_ov014_021797a8->sParentParam.entryFlag = 1;
    data_ov014_021797a8->sParentParam.KS_Flag = mode == WH_CONNECTMODE_KS_PARENT;
    if (mode == WH_CONNECTMODE_MP_PARENT || mode == WH_CONNECTMODE_KS_PARENT || mode == WH_CONNECTMODE_DS_PARENT) {
        return func_ov014_02178168();
    }
    return FALSE;
}

// func_ov014_02179328
// pokebw2's WH_SetScanFilter
void func_ov014_02179328(NetScanFilter filter) {
    data_ov014_021797a8->sScanFilter = filter;
}

// func_ov014_0217933c
// pokebw2's WH_SetReceiver
void func_ov014_0217933c(WHReceiver callback) {
    data_ov014_021797a8->sReceiver = callback;
    if (WM_SetPortCallback(14, func_ov014_02178d9c, NULL) != WM_ERRCODE_SUCCESS) {
        func_ov014_02178138(WH_SYSSTATE_ERROR);
    }
}

// func_ov014_02179368: sends the data to every machine, unless this is the parent and no child is there
// pokebw2's WH_SendData
BOOL func_ov014_02179368(u8 *data, u16 size, WHSendCallback callback) {
    if (func_ov014_021795a4() == 0 && (func_ov014_02178e38() & 0xfe) == 0) {
        return FALSE;
    }
    return func_ov014_02178d20(data, size, callback);
}

// func_ov014_02179394: the data of the machine of the AID, from the last step of the data sharing
// pokebw2's WH_GetSharedDataAdr
u8 *func_ov014_02179394(u16 netId) {
    return WM_GetSharedDataAddress(&data_ov014_021797a8->sDataSharing, &data_ov014_021797a8->sDataSet, netId);
}

// func_ov014_021793b8: shares the data given, for the machines to read the next step
// pokebw2's WH_StepDS
BOOL func_ov014_021793b8(u8 *data) {
    int result = WM_StepDataSharing(&data_ov014_021797a8->sDataSharing, data, &data_ov014_021797a8->sDataSet);

    if (result == 7) {
        return FALSE;
    }
    if (result == 5) {
        return FALSE;
    }
    if (result != WM_ERRCODE_SUCCESS) {
        func_ov014_0217814c(result);
        return FALSE;
    }
    return TRUE;
}

// Run each frame: starts the scan again once its frames to wait have gone
// pokebw2's WH_UpdateScan
void func_ov014_021793f8(void) {
    if (data_ov014_021797a8 != NULL && data_ov014_021797a8->sScanWait > 0) {
        if (data_ov014_021797a8->sSysState != WH_SYSSTATE_SCANNING) {
            data_ov014_021797a8->sScanWait = 0;
        } else {
            data_ov014_021797a8->sScanWait = data_ov014_021797a8->sScanWait - 1;
        }
        if (data_ov014_021797a8->sScanWait == 0) {
            if (data_ov014_021797a8->sSysState == WH_SYSSTATE_SCANNING && data_ov014_021797a8->sScanPaused == 0) {
                if (!func_ov014_0217863c()) {
                    func_ov014_02178138(WH_SYSSTATE_ERROR);
                }
                return;
            }
            data_ov014_021797a8->sScanWait = 1;
        }
    }
}

// func_ov014_0217945c: resets the wireless, ending the data sharing first
// pokebw2's WH_Reset
void func_ov014_0217945c(void) {
    if (data_ov014_021797a8->sSysState == WH_SYSSTATE_DATASHARING) {
        int result = WM_EndDataSharing(&data_ov014_021797a8->sDataSharing);

        if (result != WM_ERRCODE_SUCCESS) {
            func_ov014_0217814c(result);
        }
    }
    if (!func_ov014_02178cdc()) {
        func_ov014_02178138(WH_SYSSTATE_FATAL);
    }
}

// func_ov014_02179498: ends whatever the machine is doing, back to idle. Whether it has
// pokebw2's WH_Finalize
BOOL func_ov014_02179498(void) {
    int state;

    if (data_ov014_021797a8 == NULL || data_ov014_021797a8->sSysState == WH_SYSSTATE_IDLE) {
        return TRUE;
    }
    state = data_ov014_021797a8->sSysState;
    if (data_ov014_021797a8->sLastError != 0) {
        func_ov014_0217945c();
        return TRUE;
    }
    if (state == WH_SYSSTATE_BUSY) {
        return FALSE;
    }
    if (state == WH_SYSSTATE_SCANNING) {
        if (!func_ov014_021788fc()) {
            func_ov014_0217945c();
        }
        return TRUE;
    }
    if (data_ov014_021797a8->sScanEnding != 0) {
        return TRUE;
    }
    if (state != WH_SYSSTATE_DATASHARING && state != WH_SYSSTATE_CONNECTED) {
        func_ov014_02178138(WH_SYSSTATE_BUSY);
        func_ov014_0217945c();
        return TRUE;
    }
    func_ov014_02178138(WH_SYSSTATE_BUSY);
    switch (data_ov014_021797a8->sConnectMode) {
    case WH_CONNECTMODE_DS_CHILD:
        if (WM_EndDataSharing(&data_ov014_021797a8->sDataSharing) != 0) {
            func_ov014_0217945c();
            break;
        }
    case WH_CONNECTMODE_MP_CHILD:
        if (!func_ov014_02178c50()) {
            func_ov014_0217945c();
        }
        break;
    case WH_CONNECTMODE_DS_PARENT:
        if (WM_EndDataSharing(&data_ov014_021797a8->sDataSharing) != 0) {
            func_ov014_0217945c();
            break;
        }
    case WH_CONNECTMODE_MP_PARENT:
        if (!func_ov014_021784ac()) {
            func_ov014_0217945c();
        }
        break;
    }
    return TRUE;
}

// func_ov014_02179570: ends the wireless, and calls the callback with whether it did
// pokebw2's WH_End
BOOL func_ov014_02179570(WHCallback callback) {
    data_ov014_021797a8->sCallback = callback;
    func_ov014_02178138(WH_SYSSTATE_BUSY);
    if (WM_End(func_ov014_02178de0) != WM_ERRCODE_OPERATING) {
        func_ov014_02178138(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

// func_ov014_021795a4
// pokebw2's WH_GetAid
u16 func_ov014_021795a4(void) {
    if (data_ov014_021797a8 != NULL) {
        return data_ov014_021797a8->sAid;
    }
    return 0;
}

// func_ov014_021795c0
// pokebw2's WH_SetScanCallback
void func_ov014_021795c0(WHScanCallback callback) {
    data_ov014_021797a8->sScanCallback = callback;
}

// func_ov014_021795d4: the parent's beacon, once the machine is connected
// pokebw2's WH_SetGameInfo
void func_ov014_021795d4(const void *data, int size, u32 gameId, int tgid) {
    if (data_ov014_021797a8 != NULL && (data_ov014_021797a8->sSysState == WH_SYSSTATE_CONNECTED || data_ov014_021797a8->sSysState == WH_SYSSTATE_DATASHARING)) {
        WM_SetGameInfo(NULL, data, size, gameId, tgid, 1);
    }
}

// func_ov014_02179614: the beacons that the parent has sent
// pokebw2's WH_GetBeaconCount
u16 func_ov014_02179614(void) {
    return data_ov014_021797a8->sBeaconCount;
}

// func_ov014_02179628
// pokebw2's WH_SetConnectCallback
void func_ov014_02179628(WHConnectCallback callback) {
    data_ov014_021797a8->sConnectCallback = callback;
}

// func_ov014_0217963c
// pokebw2's WH_SetScanInfoCallback
void func_ov014_0217963c(WHScanInfoCallback callback) {
    data_ov014_021797a8->sScanInfoCallback = callback;
}

// func_ov014_02179650
// pokebw2's WH_SetScanTime
void func_ov014_02179650(u16 time) {
    data_ov014_021797a8->sScanTime = time;
}

// func_ov014_02179664
// pokebw2's WH_SetScanPaused
void func_ov014_02179664(BOOL paused) {
    data_ov014_021797a8->sScanPaused = paused;
}
