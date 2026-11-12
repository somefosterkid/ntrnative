// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/wm/src/wm_standard.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/wm.h>

#include "wm_arm9_private.h"

static BOOL WmCheckParentParameter(const WMParentParam * param);
BOOL WMi_IsWirelessAvailable(void);
void WMi_RegisterSleepCallback(void);
WMErrCode WM_Init(void * wmSysBuf, u16 dmaNo);

WMErrCode WM_Enable (WMCallbackFunc callback)
{
    return WMi_EnableEx(callback, 0);
}

WMErrCode WMi_EnableEx(WMCallbackFunc callback, u32 miscFlags)
{
    WMErrCode result;
    WMArm9Buf *sys;

    result = WMi_CheckStateEx(1, WM_STATE_READY);
    WM_CHECK_RESULT(result);

    WMi_SetCallbackTable(WM_APIID_ENABLE, callback);

    WMi_RegisterSleepCallback();

    sys = WMi_GetSystemWork();
    result = WMi_SendCommand(WM_APIID_ENABLE, 4, sys->WM7, sys->status, sys->fifo7to9, miscFlags);
    if (result == WM_ERRCODE_SUCCESS) {
        return WM_ERRCODE_OPERATING;
    }
    return result;
}


WMErrCode WM_Disable (WMCallbackFunc callback)
{
    WMErrCode result;

    result = WMi_CheckState(WM_STATE_STOP);
    WM_CHECK_RESULT(result);

    WMi_SetCallbackTable(WM_APIID_DISABLE, callback);

    result = WMi_SendCommand(WM_APIID_DISABLE, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_PowerOn (WMCallbackFunc callback)
{
    WMErrCode result;

    result = WMi_CheckState(WM_STATE_STOP);
    WM_CHECK_RESULT(result);

    WMi_SetCallbackTable(WM_APIID_POWER_ON, callback);

    result = WMi_SendCommand(WM_APIID_POWER_ON, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_PowerOff (WMCallbackFunc callback)
{
    WMErrCode result;

    result = WMi_CheckState(WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    WMi_SetCallbackTable(WM_APIID_POWER_OFF, callback);

    result = WMi_SendCommand(WM_APIID_POWER_OFF, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_Initialize (void * wmSysBuf, WMCallbackFunc callback, u16 dmaNo)
{
    return WMi_InitializeEx(wmSysBuf, callback, dmaNo, 0);
}

WMErrCode WM_InitializeForListening (void * wmSysBuf, WMCallbackFunc callback, u16 dmaNo, BOOL blink)
{
    u32 miscFlags = WM_MISC_FLAG_LISTEN_ONLY;

    if (!blink) {
        miscFlags |= WM_MISC_FLAG_NO_BLINK;
    }

    return WMi_InitializeEx(wmSysBuf, callback, dmaNo, miscFlags);
}

WMErrCode WMi_InitializeEx(void * wmSysBuf, WMCallbackFunc callback, u16 dmaNo, u32 miscFlags)
{
    WMErrCode result;
    WMArm9Buf *sys;

    result = WM_Init(wmSysBuf, dmaNo);
    WM_CHECK_RESULT(result);

    WMi_SetCallbackTable(WM_APIID_INITIALIZE, callback);

    WMi_RegisterSleepCallback();

    sys = WMi_GetSystemWork();
    result = WMi_SendCommand(WM_APIID_INITIALIZE, 4, sys->WM7, sys->status, sys->fifo7to9, miscFlags);
    if (result == WM_ERRCODE_SUCCESS) {
        return WM_ERRCODE_OPERATING;
    }
    return result;
}


WMErrCode WM_Reset (WMCallbackFunc callback)
{
    WMErrCode result;

    result = WMi_CheckIdle();
    WM_CHECK_RESULT(result);

    WMi_SetCallbackTable(WM_APIID_RESET, callback);

    result = WMi_SendCommand(WM_APIID_RESET, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_End (WMCallbackFunc callback)
{
    WMErrCode result;

    result = WMi_CheckState(WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    WMi_SetCallbackTable(WM_APIID_END, callback);

    result = WMi_SendCommand(WM_APIID_END, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_SetParentParameter(WMCallbackFunc callback, const WMParentParam *param)
{
    WMErrCode result;

    result = WMi_CheckStateEx(1, WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    if (OS_IsRunOnTwl()) {
        if (WMi_IsWirelessAvailable() == 0) {
            return WM_ERRCODE_WM_DISABLE;
        }
    }

    if (param == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    if ((param->userGameInfoLength != 0) && (param->userGameInfo == NULL)) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    if ((param->parentMaxSize + (param->KS_Flag != 0 ? 0x2a : 0) > 0x200)
        || (param->childMaxSize + (param->KS_Flag != 0 ? 6 : 0) > 0x200)) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    WmCheckParentParameter(param);

    WMi_SetCallbackTable(WM_APIID_SET_P_PARAM, callback);

    DC_StoreRange(param, 0x40);
    if (param->userGameInfoLength != 0) {
        DC_StoreRange(param->userGameInfo, param->userGameInfoLength);
    }

    result = WMi_SendCommand(WM_APIID_SET_P_PARAM, 1, param);
    if (result == WM_ERRCODE_SUCCESS) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}


static BOOL WmCheckParentParameter(const WMParentParam * param)
{
    if (param->userGameInfoLength > WM_SIZE_USER_GAMEINFO) {
        return FALSE;
    }

    if ((param->beaconPeriod < 10) || (param->beaconPeriod > 1000)) {
        return FALSE;
    }

    if ((param->channel < 1) || (param->channel > 14)) {
        return FALSE;
    }

    return TRUE;
}

WMErrCode WMi_StartParentEx(WMCallbackFunc callback, BOOL param)
{
    WMErrCode result;
    WMArm9Buf *sys;

    result = WMi_CheckStateEx(1, WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    if (OS_IsRunOnTwl()) {
        if (WMi_IsWirelessAvailable() == 0) {
            return WM_ERRCODE_WM_DISABLE;
        }
    }

    sys = WMi_GetSystemWork();
    sys->myAid = 0;
    sys->connectedAidBitmap = 0;

    WMi_SetCallbackTable(WM_APIID_START_PARENT, callback);

    result = WMi_SendCommand(WM_APIID_START_PARENT, 1, param);
    if (result == WM_ERRCODE_SUCCESS) {
        return WM_ERRCODE_OPERATING;
    }
    return result;
}


WMErrCode WM_StartParent (WMCallbackFunc callback)
{
    return WMi_StartParentEx(callback, TRUE);
}

WMErrCode WM_EndParent (WMCallbackFunc callback)
{
    WMErrCode result;

    result = WMi_CheckState(WM_STATE_PARENT);
    WM_CHECK_RESULT(result);

    WMi_SetCallbackTable(WM_APIID_END_PARENT, callback);

    result = WMi_SendCommand(WM_APIID_END_PARENT, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

// libntr's: Black's link drops it, as only dwc_mov_wh.c's child-side code calls it, which Black lacks. TwlSDK's may
// test WMi_IsWirelessAvailable as its WM_StartScanEx does; no source of it says so, and this is libntr's
WMErrCode WM_StartScan (WMCallbackFunc callback, const WMScanParam * param)
{
    WMErrCode result;

    result = WMi_CheckStateEx(3, WM_STATE_IDLE, WM_STATE_CLASS1, WM_STATE_SCAN);
    WM_CHECK_RESULT(result);

    if (param == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (param->scanBuf == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    if ((param->channel < 1) || (param->channel > 14)) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    WMi_SetCallbackTable(WM_APIID_START_SCAN, callback);

    {
        WMStartScanReq Req;

        Req.apiid = WM_APIID_START_SCAN;
        Req.channel = param->channel;
        Req.scanBuf = param->scanBuf;
        Req.maxChannelTime = param->maxChannelTime;
        Req.bssid[0] = param->bssid[0];
        Req.bssid[1] = param->bssid[1];
        Req.bssid[2] = param->bssid[2];
        Req.bssid[3] = param->bssid[3];
        Req.bssid[4] = param->bssid[4];
        Req.bssid[5] = param->bssid[5];
        result = WMi_SendCommandDirect(&Req, sizeof(Req));
        WM_CHECK_RESULT(result);
    }

    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_StartScanEx(WMCallbackFunc callback, const WMScanExParam *param)
{
    WMErrCode result;
    WMStartScanExReq req;

    result = WMi_CheckStateEx(3, WM_STATE_IDLE, WM_STATE_CLASS1, WM_STATE_SCAN);
    WM_CHECK_RESULT(result);

    if (OS_IsRunOnTwl()) {
        if (WMi_IsWirelessAvailable() == 0) {
            return WM_ERRCODE_WM_DISABLE;
        }
    }

    if (param == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (param->scanBuf == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (param->scanBufSize > 0x400) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (param->ssidLength > 0x20) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (param->scanType != 0 && param->scanType != 1 && param->scanType != 2 && param->scanType != 3) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    if ((u16)(param->scanType + 0xfffe) <= 1 && param->ssidMatchLength > 0x20) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    WMi_SetCallbackTable(WM_APIID_START_SCAN_EX, callback);

    req.apiid = WM_APIID_START_SCAN_EX;
    req.channelList = param->channelList;
    req.scanBuf = param->scanBuf;
    req.scanBufSize = param->scanBufSize;
    req.maxChannelTime = param->maxChannelTime;
    MI_CpuCopy8(param->bssid, req.bssid, sizeof(req.bssid));
    req.scanType = param->scanType;
    req.ssidMatchLength = param->ssidMatchLength;
    req.ssidLength = param->ssidLength;
    MI_CpuCopy8(param->ssid, req.ssid, sizeof(req.ssid));

    result = WMi_SendCommandDirect(&req, sizeof(req));
    if (result == WM_ERRCODE_SUCCESS) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}


WMErrCode WM_EndScan (WMCallbackFunc callback)
{
    WMErrCode result;

    result = WMi_CheckState(WM_STATE_SCAN);
    WM_CHECK_RESULT(result);

    WMi_SetCallbackTable(WM_APIID_END_SCAN, callback);

    result = WMi_SendCommand(WM_APIID_END_SCAN, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_StartConnectEx(WMCallbackFunc callback, const WMBssDesc *bssDesc, const u8 *ssid, BOOL powerSave, u16 authMode)
{
    WMErrCode result;
    WMStartConnectReq req;
    WMArm9Buf *sys;

    result = WMi_CheckStateEx(1, WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    if (OS_IsRunOnTwl()) {
        if (WMi_IsWirelessAvailable() == 0) {
            return WM_ERRCODE_WM_DISABLE;
        }
    }

    if (bssDesc == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }

    DC_StoreRange(bssDesc, bssDesc->length * 2);

    sys = WMi_GetSystemWork();
    sys->myAid = 0;
    sys->connectedAidBitmap = 0;

    WMi_SetCallbackTable(WM_APIID_START_CONNECT, callback);

    req.apiid = WM_APIID_START_CONNECT;
    req.pInfo = bssDesc;
    if (ssid != NULL) {
        MI_CpuCopy8(ssid, req.ssid, sizeof(req.ssid));
    } else {
        MI_CpuFill8(req.ssid, 0, sizeof(req.ssid));
    }
    req.powerSave = powerSave;
    req.authMode = authMode;

    result = WMi_SendCommandDirect(&req, sizeof(req));
    if (result == WM_ERRCODE_SUCCESS) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}


WMErrCode WM_Disconnect (WMCallbackFunc callback, u16 aid)
{
    WMErrCode result;
    WMArm9Buf * p = WMi_GetSystemWork();

    result = WMi_CheckStateEx(5,
                              WM_STATE_PARENT, WM_STATE_MP_PARENT,
                              WM_STATE_CHILD, WM_STATE_MP_CHILD, WM_STATE_DCF_CHILD);
    WM_CHECK_RESULT(result);

    if (
        (p->status->state == WM_STATE_PARENT) || (p->status->state == WM_STATE_MP_PARENT)) {

        if ((aid < 1) || (aid > WM_NUM_MAX_CHILD)) {
            WM_WARNING("Parameter \"aid\" must be between 1 and %d.\n", WM_NUM_MAX_CHILD);
            return WM_ERRCODE_INVALID_PARAM;
        }
        DC_InvalidateRange(&(p->status->child_bitmap), 2);
        if (!(p->status->child_bitmap & (0x0001 << aid))) {
            WM_WARNING("There is no child that have aid %d.\n", aid);
            return WM_ERRCODE_NO_CHILD;
        }
    } else {
        if (aid != 0) {
            WM_WARNING("Now child mode , so aid must be 0.\n");
            return WM_ERRCODE_INVALID_PARAM;
        }
    }

    WMi_SetCallbackTable(WM_APIID_DISCONNECT, callback);

    result = WMi_SendCommand(WM_APIID_DISCONNECT, 1, (u32)(0x0001 << aid));
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}
