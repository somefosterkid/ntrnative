// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/wm/src/wm_mp.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/wm.h>

#include "wm_arm9_private.h"

WMErrCode WMi_StartMP (WMCallbackFunc callback, u16 * recvBuf, u16 recvBufSize, u16 * sendBuf, u16 sendBufSize, WMMPTmpParam * tmpParam)
{
    WMErrCode result;
    WMArm9Buf * p = WMi_GetSystemWork();
    WMStatus * status = p->status;

    result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_CHILD);
    WM_CHECK_RESULT(result);

    DC_InvalidateRange(&(status->aid), 2);
    DC_InvalidateRange(&(status->pwrMgtMode), 2);

    if (status->aid != 0 && status->pwrMgtMode != 1) {
        WM_WARNING("Could not start MP. Power save mode is illegal.\n");
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    DC_InvalidateRange(&(status->mp_flag), 4);

    if (status->mp_flag == TRUE) {
        WM_WARNING("Already started MP protocol. So can't execute request.\n");
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    if ((recvBufSize & 0x3f) != 0) {
        WM_WARNING("Parameter \"recvBufSize\" is not a multiple of 64.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }

    if ((sendBufSize & 0x1f) != 0) {
        WM_WARNING("Parameter \"sendBufSize\" is not a multiple of 32.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }

    if ((u32)recvBuf & 0x01f) {
        WM_WARNING("Parameter \"recvBuf\" is not 32-byte aligned.\n");
    }

    if ((u32)sendBuf & 0x01f) {
        WM_WARNING("Parameter \"sendBuf\" is not 32-byte aligned.\n");
    }

    DC_InvalidateRange(&(status->mp_ignoreSizePrecheckMode), sizeof(status->mp_ignoreSizePrecheckMode));

    if (status->mp_ignoreSizePrecheckMode == FALSE) {
        if (recvBufSize < WM_GetMPReceiveBufferSize()) {
            WM_WARNING("Parameter \"recvBufSize\" is not enough size.\n");
            return WM_ERRCODE_INVALID_PARAM;
        }

        if (sendBufSize < WM_GetMPSendBufferSize()) {
            WM_WARNING("Parameter \"sendBufSize\" is not enough size.\n");
            return WM_ERRCODE_INVALID_PARAM;
        }
    }

    WMi_SetCallbackTable(WM_APIID_START_MP, callback);

    {
        WMStartMPReq Req;

        MI_CpuClear32(&Req, sizeof(Req));

        Req.apiid = WM_APIID_START_MP;
        Req.recvBuf = (u32 *)recvBuf;
        Req.recvBufSize = (u32)(recvBufSize / 2);
        Req.sendBuf = (u32 *)sendBuf;
        Req.sendBufSize = (u32)sendBufSize;

        MI_CpuClear32(&Req.param, sizeof(Req.param));
        MI_CpuCopy32(tmpParam, &Req.tmpParam, sizeof(Req.tmpParam));

        result = WMi_SendCommandDirect(&Req, sizeof(Req));
        WM_CHECK_RESULT(result);
    }

    return WM_ERRCODE_OPERATING;
}

WMErrCode
WM_StartMPEx (WMCallbackFunc callback,
              u16 * recvBuf,
              u16 recvBufSize,
              u16 * sendBuf,
              u16 sendBufSize,
              u16 mpFreq,
              u16 defaultRetryCount,
              BOOL minPollBmpMode, BOOL singlePacketMode, BOOL fixFreqMode, BOOL ignoreFatalError)
{
    WMMPTmpParam tmpParam;

    MI_CpuClear32(&tmpParam, sizeof(tmpParam));

    tmpParam.mask = WM_MP_TMP_PARAM_MIN_FREQUENCY | WM_MP_TMP_PARAM_FREQUENCY | WM_MP_TMP_PARAM_DEFAULT_RETRY_COUNT
                    | WM_MP_TMP_PARAM_MIN_POLL_BMP_MODE | WM_MP_TMP_PARAM_SINGLE_PACKET_MODE |
                    WM_MP_TMP_PARAM_IGNORE_FATAL_ERROR_MODE;
    tmpParam.minFrequency = mpFreq;
    tmpParam.frequency = mpFreq;
    tmpParam.defaultRetryCount = defaultRetryCount;
    tmpParam.minPollBmpMode = (u8)minPollBmpMode;
    tmpParam.singlePacketMode = (u8)singlePacketMode;
    tmpParam.ignoreFatalErrorMode = (u8)ignoreFatalError;

    if (fixFreqMode != FALSE && mpFreq != 0) {
        tmpParam.mask |= WM_MP_TMP_PARAM_MAX_FREQUENCY;
        tmpParam.maxFrequency = mpFreq;
    }

    return WMi_StartMP(callback, recvBuf, recvBufSize, sendBuf, sendBufSize, &tmpParam);
}

WMErrCode WM_StartMP (WMCallbackFunc callback, u16 * recvBuf, u16 recvBufSize, u16 * sendBuf, u16 sendBufSize, u16 mpFreq)
{
    WMMPTmpParam tmpParam;

    MI_CpuClear32(&tmpParam, sizeof(tmpParam));

    tmpParam.mask = WM_MP_TMP_PARAM_FREQUENCY | WM_MP_TMP_PARAM_MIN_FREQUENCY;
    tmpParam.minFrequency = mpFreq;
    tmpParam.frequency = mpFreq;

    return WMi_StartMP(callback, recvBuf, recvBufSize, sendBuf, sendBufSize, &tmpParam);
}

WMErrCode WM_SetMPDataToPortEx (WMCallbackFunc callback, void * arg, const u16 * sendData, u16 sendDataSize, u16 destBitmap, u16 port, u16 prio)
{
    WMErrCode result;
    BOOL isParent;
    u16 mpReadyBitmap = 0x0001;
    u16 childBitmap = 0x0001;
    WMArm9Buf * p = WMi_GetSystemWork();
    WMStatus * status = p->status;

    result = WMi_CheckStateEx(2, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD);
    WM_CHECK_RESULT(result);

    DC_InvalidateRange(&(status->aid), 2);
    isParent = (status->aid == 0) ? TRUE : FALSE;

    if (isParent == TRUE) {
        DC_InvalidateRange(&(status->child_bitmap), 2);
        childBitmap = status->child_bitmap;
        DC_InvalidateRange(&(status->mp_readyBitmap), 2);
        mpReadyBitmap = status->mp_readyBitmap;
    }

    if (sendData == NULL) {
        WM_WARNING("Parameter \"sendData\" must not be NULL.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (childBitmap == 0) {
        WM_WARNING("There is no child connected.\n");
        return WM_ERRCODE_NO_CHILD;
    }

    if ((u32)sendData & 0x01f) {

        WM_WARNING("Parameter \"sendData\" is not 32-byte aligned.\n");
    }

    DC_InvalidateRange(&(status->mp_sendBuf), 2);

    if ((void *)sendData == (void *)status->mp_sendBuf) {
        WM_WARNING
            ("Parameter \"sendData\" must not be equal to the WM_StartMP argument \"sendBuf\".\n");
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (sendDataSize > WM_SIZE_MP_DATA_MAX) {
        WM_WARNING("Parameter \"sendDataSize\" is over limit.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (sendDataSize == 0) {
        WM_WARNING("Parameter \"sendDataSize\" must not be 0.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }

    DC_StoreRange((void *)sendData, sendDataSize);

    result = WMi_SendCommand(WM_APIID_SET_MP_DATA, 7,
                             (u32)sendData,
                             (u32)sendDataSize,
                             (u32)destBitmap, (u32)port, (u32)prio, (u32)callback, (u32)arg);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_EndMP (WMCallbackFunc callback)
{
    WMErrCode result;
    WMArm9Buf * p = WMi_GetSystemWork();

    result = WMi_CheckStateEx(2, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD);
    WM_CHECK_RESULT(result);

    DC_InvalidateRange(&(p->status->mp_flag), 4);
    if (p->status->mp_flag == FALSE) {
        WM_WARNING("It is not MP mode now.\n");
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    WMi_SetCallbackTable(WM_APIID_END_MP, callback);

    result = WMi_SendCommand(WM_APIID_END_MP, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}
