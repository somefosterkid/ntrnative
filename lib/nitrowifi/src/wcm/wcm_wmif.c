// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/wcm/wcm_wmif.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitro/math.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/wm.h"
#include "nitrowifi/wcm.h"
#include "wcm_internal.h"

// NitroWiFi's WCM (overlay 11): its backend for the DS's wireless, which takes the steps of wcm_system.c's operations
// through NitroSDK's WM, and WM's callbacks, which move WCM's state machine and notify WCM's user. TwlSDK's libwcm
// names the object wmif.o; Black 1's decompilation (aletheia) has the same functions, in this order, in its
// wcm_wmif.c, and the names are the SDK's.

static void func_ov009_02166e5c(void);
static void func_ov009_02166eb0(void *ind);
static void func_ov009_02166f24(void *ind);
static void func_ov009_021671c0(void *ind);
static void func_ov009_02167448(void *ind);
static void func_ov009_021674bc(void *ind);
static void func_ov009_021676e4(void *ind);
static void func_ov009_021677a4(void *ind);
static void func_ov009_021678ac(void *ind);
static void func_ov009_021679c0(void *ind);
static void func_ov009_02167cb4(const u8 *bssid, const u8 *essid, u32 option);
static void func_ov009_02167de0(void *ind);
static void func_ov009_02167e20(void *ind);

// The startup FSM's WM_Initialize-adjacent first step: brings up the hardware and reads its MAC address
// pokebw2's WCMi_WmifStartupAsync
s32 func_ov009_02166880(void) {
    WCMWork *work = func_ov009_021657c8();

    switch (WM_Init(work->startupBuffer, (u16)work->dmaNo)) {
    case 0:
        break;
    case 3:
        func_ov009_02165a4c(11);
        return 7;
    case 4:
        return 5;
    // WM_ERRCODE_INVALID_PARAM, which the SDK lists with the default
    case 6:
    default:
        func_ov009_02165a4c(11);
        return 7;
    }
    if (WM_SetIndCallback((WMCallbackFunc)func_ov009_02166eb0) != 0) {
        func_ov009_02165a4c(11);
        return 7;
    }
    {
        u8 mac[6];

        OS_GetMacAddress(mac);
        func_ov009_021665cc(mac);
    }
    switch (WM_Enable((WMCallbackFunc)func_ov009_02166f24)) {
    case 2:
        func_ov009_02165a4c(2);
        work->pendingNotify = 1;
        break;
    case 8:
        func_ov009_02165a4c(12);
        return 1;
    // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
    case 3:
    default:
        func_ov009_02165a4c(11);
        return 7;
    }
    return 3;
}

// pokebw2's WCMi_WmifCleanupAsync
s32 func_ov009_021669b8(void) {
    WCMWork *work = func_ov009_021657c8();

    switch (WM_PowerOff((WMCallbackFunc)func_ov009_02166f24)) {
    case 2:
        func_ov009_02165a4c(4);
        work->pendingNotify = 2;
        break;
    case 8:
        return 4;
    // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
    case 3:
    default:
        func_ov009_02165a4c(0xb);
        return 7;
    }
    return 3;
}

// pokebw2's WCMi_WmifBeginSearchAsync
s32 func_ov009_02166a24(const u8 *bssid, const u8 *essid, u32 option) {
    WCMWork *work = func_ov009_021657c8();

    switch (work->state) {
    case 5:
        func_ov009_02167cb4(bssid, essid, option);
        return 2;
    case 6:
        func_ov009_02167cb4(bssid, essid, option);
        return 0;
    case 3:
        break;
    default:
        return 1;
    }
    func_ov009_02167cb4(bssid, essid, option);
    DC_InvalidateRange(work->scanRequest.scanBuf, work->scanRequest.scanBufSize);
    work->scanCount++;
    switch (WM_StartScanEx((WMCallbackFunc)func_ov009_021671c0, &work->scanRequest)) {
    case 2:
        func_ov009_02165a4c(5);
        work->pendingNotify = 3;
        break;
    case 8:
        return 4;
    // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
    case 3:
    default:
        func_ov009_02165a4c(0xb);
        return 7;
    }
    return 3;
}

// pokebw2's WCMi_WmifConnectAsync
s32 func_ov009_02166b2c(void) {
    WCMWork *work = func_ov009_021657c8();

    if (work->state == 3) {
        work->connectBss.rateSet.support = work->connectBss.rateSet.basic | 3;
    }
    switch (WM_SetLifeTime((WMCallbackFunc)func_ov009_02166f24, 0xffff, 0x50, 0xffff, 0xffff)) {
    case 2:
        func_ov009_02165a4c(8);
        work->pendingNotify = 5;
        break;
    case 8:
        return 4;
    // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
    case 3:
    default:
        func_ov009_02165a4c(0xb);
        return 7;
    }
    return 3;
}

// pokebw2's WCMi_WmifDisconnectAsync
s32 func_ov009_02166bd8(void) {
    WCMWork *work = func_ov009_021657c8();

    switch (WM_EndDCF((WMCallbackFunc)func_ov009_021678ac)) {
    case 2:
        func_ov009_02165a4c(0xa);
        work->pendingNotify = 6;
        break;
    case 8:
        return 4;
    // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
    case 3:
    default:
        func_ov009_02165a4c(0xb);
        return 7;
    }
    return 3;
}

// pokebw2's WCMi_WmifTerminateAsync
s32 func_ov009_02166c44(void) {
    WCMWork *work = func_ov009_021657c8();
    const WMStatus *status = WMi_GetStatusAddress();
    s32 result;

    DC_InvalidateRange((void *)status, 2);
    switch (status->state) {
    case 0:
        result = WM_Finish();
        if (result == 0) {
            func_ov009_02165a4c(1);
            work->pendingNotify = 0;
            return 0;
        }
        break;
    case 1:
        result = WM_Disable((WMCallbackFunc)func_ov009_02166f24);
        break;
    case 2:
        result = WM_PowerOff((WMCallbackFunc)func_ov009_02166f24);
        break;
    default:
        work->resetPending = 1;
        result = WM_Reset((WMCallbackFunc)func_ov009_021679c0);
        break;
    }
    switch (result) {
    case 2:
        func_ov009_02165a4c(0xd);
        work->pendingNotify = 9;
        break;
    case 8:
        return 4;
    // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
    case 3:
    default:
        func_ov009_02165a4c(0xb);
        return 7;
    }
    return 3;
}

// pokebw2's func_ov009_02166d2c
s32 func_ov009_02166d2c(const u8 *dst, const void *buffer, u16 byteLen, void (*completion)(s32)) {
    WCMCpsifWork *global = func_ov009_02166378();
    WMErrCode result;

    if (completion == NULL) {
        result = WM_SetDCFData(func_ov009_02167de0, dst, buffer, byteLen);
    } else {
        global->sendCompletion = completion;
        result = WM_SetDCFData(func_ov009_02167e20, dst, buffer, byteLen);
    }
    switch (result) {
    case 2:
        break;
    // WM_ERRCODE_ILLEGAL_STATE, WM_ERRCODE_INVALID_PARAM and WM_ERRCODE_FIFO_ERROR, which the SDK lists with the
    // default
    case 3:
    case 6:
    case 8:
    default:
        return -5;
    }
    if (completion == NULL) {
        OS_SleepThread(&global->sendQueue);
        switch (global->sendStatus) {
        case 0:
            break;
        // WM_ERRCODE_FAILED, which the SDK lists with the default
        case 1:
        default:
            return -5;
        }
    }
    return byteLen;
}

// pokebw2's WCMi_WmifGetAllowedChannel
u16 func_ov009_02166de4(void) {
    u16 allowed = WM_GetAllowedChannel();
    u16 lowerMask = 1;
    u16 upperMask = 0x8000;

    if (allowed == 0) {
        return 0;
    }
    // Keep the inclusive span, including any gaps in the hardware's allowed-channel mask.
    while (!(upperMask & allowed)) {
        upperMask = (upperMask >> 1) | 0x8000;
    }
    while (!(lowerMask & allowed)) {
        lowerMask = (lowerMask << 1) | 1;
    }
    upperMask <<= 1;
    lowerMask >>= 1;
    return (u16) ~(upperMask | lowerMask);
}

// pokebw2's WcmWmReset
static void func_ov009_02166e5c(void) {
    WCMWork *work = func_ov009_021657c8();

    if (work->resetPending == 0) {
        work->resetPending = 1;
        if (WM_Reset((WMCallbackFunc)func_ov009_021679c0) != 2) {
            func_ov009_02165a4c(0xb);
            func_ov009_021659b4(7, NULL, 0, 0x24f);
        }
    }
}

// WM's indication callback, installed in func_ov009_02166880: dispatches disconnect indications by the current state
// pokebw2's WcmWmcbIndication
static void func_ov009_02166eb0(void *ind) {
    const WMIndCallback *indication = ind;
    WCMWork *work = func_ov009_021657c8();

    if (indication->errcode == 8) {
        if (indication->state == 0x16 && indication->reason == 0x25) {
            switch (work->state) {
            case 8:
                func_ov009_02165a4c(0xc);
                break;
            case 9:
            case 12:
                func_ov009_02166e5c();
                break;
            case 10:
                func_ov009_02165a4c(0xc);
                break;
            }
        }
    }
}

// pokebw2's WcmWmcbCommon
static void func_ov009_02166f24(void *ind) {
    const WMCallback *callback = (const WMCallback *)ind;
    int result = 20;
    WCMWork *work = func_ov009_021657c8();

    switch (callback->errcode) {
    case 0:
        switch (callback->apiid) {
        case 3:
            result = WM_PowerOn((WMCallbackFunc)func_ov009_02166f24);
            break;
        case 4:
            result = WM_Finish();
            switch (result) {
            case 0:
                func_ov009_02165a4c(1);
                func_ov009_021659b4(0, NULL, 0, 0x2a4);
                break;
            // WM_ERRCODE_WM_DISABLE, whose warning the SDK leaves out of this build
            case 4:
            default:
                func_ov009_02165a4c(11);
                func_ov009_021659b4(7, NULL, 0, 0x2ad);
                break;
            }
            return;
        case 5:
            func_ov009_02165a4c(3);
            func_ov009_021659b4(0, NULL, 0, 0x2b4);
            return;
        case 6:
            result = WM_Disable((WMCallbackFunc)func_ov009_02166f24);
            break;
        case 29:
            result = WM_SetBeaconIndication((WMCallbackFunc)func_ov009_02166f24, 0);
            break;
        case 25:
            result = WM_SetWEPKeyEx((WMCallbackFunc)func_ov009_02166f24, work->connectionStorage.mode,
                                    work->connectionStorage.argument, work->connectionStorage.data);
            break;
        case 39:
            result = WM_StartConnectEx(
                (WMCallbackFunc)func_ov009_021674bc, &work->connectBss, NULL,
                (work->searchOption & WCM_OPTION_POWER_ACTIVE) == WCM_OPTION_POWER_ACTIVE ? FALSE : TRUE,
                (work->searchOption & WCM_OPTION_AUTH_SHAREDKEY) == WCM_OPTION_AUTH_SHAREDKEY ? (u16)1 : (u16)0);
            break;
        }
        switch (result) {
        case 2:
            break;
        case 8:
            func_ov009_02165a4c(12);
            func_ov009_021659b4(1, work->pendingNotify == WCM_NOTIFY_CONNECT ? &work->connectBss : NULL, 0, 0x2f0);
            break;
        // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
        case 3:
        default:
            func_ov009_02165a4c(11);
            func_ov009_021659b4(7, work->pendingNotify == WCM_NOTIFY_CONNECT ? &work->connectBss : NULL, 0, 0x2f9);
            break;
        }
        break;
    case 1:
        func_ov009_02165a4c(12);
        func_ov009_021659b4(1, work->pendingNotify == WCM_NOTIFY_CONNECT ? &work->connectBss : NULL, 0, 0x31f);
        break;
    // WM_ERRCODE_ILLEGAL_STATE and WM_ERRCODE_WM_DISABLE, whose warnings the SDK leaves out of this build
    case 3:
    case 4:
    default:
        func_ov009_02165a4c(11);
        func_ov009_021659b4(7, work->pendingNotify == WCM_NOTIFY_CONNECT ? &work->connectBss : NULL, 0, 0x329);
        break;
    }
}

// pokebw2's WcmWmcbScanEx
static void func_ov009_021671c0(void *ind) {
    const WMStartScanExCallback *callback = ind;
    int result = 0x14;
    WCMWork *work = func_ov009_021657c8();

    switch (callback->errcode) {
    case 0:
        if (work->state == 5) {
            func_ov009_02165a4c(6);
            func_ov009_021659b4(0, NULL, 0, 0x346);
        }
        switch (work->state) {
        case 6:
            work->pendingNotify = 7;
            if (callback->state == WM_STATECODE_PARENT_FOUND) {
                int i;

                DC_InvalidateRange(work->scanRequest.scanBuf, work->scanRequest.scanBufSize);
                for (i = 0; i < callback->bssDescCount; i++) {
                    func_ov009_02165d04(callback->bssDesc[i], callback->linkLevel[i]);
                    func_ov009_021659f8(7, 0, (s32)callback->bssDesc[i], ind, 0x35c);
                }
            }
            if ((work->searchOption & 0xc00000) == 0xc00000) {
                u32 count = MATH_CountPopulation(work->searchOption & 0x3ffe);

                if (count > 0) {
                    if (work->scanCount % count == 0) {
                        func_ov009_021659f8(8, 0, work->scanCount, NULL, 0x36a);
                    }
                }
            }
            work->scanRequest.channelList =
                (u16)((1 << func_ov009_02165918((u16)(32 - MATH_CountLeadingZeros(callback->channelList)))) >>
                      1);
            DC_StoreRange(work->scanRequest.scanBuf, work->scanRequest.scanBufSize);
            work->scanCount++;
            result = WM_StartScanEx((WMCallbackFunc)func_ov009_021671c0, &work->scanRequest);
            break;
        case 7:
            result = WM_EndScan((WMCallbackFunc)func_ov009_02167448);
            break;
        case 0xd:
            func_ov009_02166e5c();
            return;
        }
        switch (result) {
        case 2:
            break;
        case 8:
            func_ov009_02165a4c(0xc);
            func_ov009_021659b4(1, NULL, 0, 0x395);
            break;
        // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
        case 3:
        default:
            func_ov009_02165a4c(0xb);
            func_ov009_021659b4(7, NULL, 0, 0x39e);
            break;
        }
        break;
    case 1:
        func_ov009_02166e5c();
        break;
    // WM_ERRCODE_ILLEGAL_STATE and WM_ERRCODE_WM_DISABLE, whose warnings the SDK leaves out of this build
    case 3:
    case 4:
    default:
        func_ov009_02165a4c(0xb);
        func_ov009_021659b4(7, NULL, 0, 0x3af);
        break;
    }
}

// pokebw2's WcmWmcbEndScan
static void func_ov009_02167448(void *ind) {
    switch (((const WMCallback *)ind)->errcode) {
    case 0:
        func_ov009_02165a4c(3);
        func_ov009_021659b4(0, NULL, 0, 0x3c5);
        return;
    case 1:
        func_ov009_02166e5c();
        return;
    // WM_ERRCODE_ILLEGAL_STATE and WM_ERRCODE_WM_DISABLE, whose warnings the SDK leaves out of this build
    case 3:
    case 4:
    default:
        func_ov009_02165a4c(11);
        func_ov009_021659b4(7, NULL, 0, 0x3d5);
        return;
    }
}

// pokebw2's WcmWmcbConnect
static void func_ov009_021674bc(void *ind) {
    const WMStartConnectCallback *callback = ind;
    WCMWork *work = func_ov009_021657c8();
    int result;

    switch (callback->errcode) {
    case 0:
        switch (callback->state) {
        case 8:
        case 9:
            switch (work->state) {
            case 10:
                work->associationId = 0;
            case 8:
                func_ov009_02165a4c(0xc);
                break;
            case 9:
                work->associationId = 0;
                work->pendingNotify = 6;
            case 12:
                func_ov009_02166e5c();
                break;
            default:
                break;
            }
            break;
        case 6:
        case 0x1a:
            break;
        case 7:
            if (work->state == 0xc) {
                func_ov009_02165a4c(8);
                func_ov009_02166e5c();
            } else if (callback->aid >= 1 && callback->aid <= 0x7d7) {
                work->associationId = callback->aid;
                result = WM_StartDCF((WMCallbackFunc)func_ov009_021677a4, (WMDcfRecvBuf *)work->ioBuffer.receive,
                                     sizeof(work->ioBuffer.receive));
                switch (result) {
                case 2:
                    break;
                case 8:
                    func_ov009_02165a4c(0xc);
                    func_ov009_021659b4(1, &work->connectBss, 0, 0x430);
                    break;
                // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
                case 3:
                default:
                    func_ov009_02165a4c(0xb);
                    func_ov009_021659b4(7, &work->connectBss, 0, 0x439);
                    break;
                }
            } else {
                func_ov009_02166e5c();
            }
            break;
        default:
            func_ov009_02165a4c(0xb);
            func_ov009_021659b4(7, &work->connectBss, callback->state, 0x447);
            break;
        }
        break;
    case 1:
        work->connectionFailureReason = callback->wlStatus;
    case 6:
    case 11:
    case 12:
        func_ov009_02165a4c(8);
        func_ov009_02166e5c();
        break;
    // WM_ERRCODE_ILLEGAL_STATE and WM_ERRCODE_WM_DISABLE, whose warnings the SDK leaves out of this build
    case 3:
    case 4:
    default:
        func_ov009_02165a4c(0xb);
        func_ov009_021659b4(7, &work->connectBss, 0, 0x45e);
        break;
    }
}

// pokebw2's WcmWmcbDisconnect
static void func_ov009_021676e4(void *ind) {
    const WMCallback *callback = ind;
    WCMWork *work = func_ov009_021657c8();

    switch (callback->errcode) {
    case 0:
        if (work->state == 0xc) {
            func_ov009_02165a4c(0xa);
            func_ov009_02166e5c();
        } else {
            work->associationId = 0;
            func_ov009_02165a4c(3);
            func_ov009_021659b4(0, &work->connectBss, 0, 0x481);
        }
        break;
    case 1:
    case 3:
        func_ov009_02165a4c(0xa);
        func_ov009_02166e5c();
        break;
    // WM_ERRCODE_WM_DISABLE, whose warning the SDK leaves out of this build
    case 4:
    default:
        func_ov009_02165a4c(0xb);
        func_ov009_021659b4(7, &work->connectBss, 0, 0x493);
        break;
    }
}

// pokebw2's WcmWmcbStartDCF
static void func_ov009_021677a4(void *ind) {
    const WMStartDCFCallback *callback = ind;
    WCMWork *work = func_ov009_021657c8();

    switch (callback->errcode) {
    case 0:
        switch (callback->state) {
        case 0xe:
            if (work->state == 0xc) {
                func_ov009_02165a4c(8);
                func_ov009_02166e5c();
            } else {
                func_ov009_02165a4c(9);
                func_ov009_021659b4(0, &work->connectBss, 0, 0x4b7);
            }
            break;
        case 0xf:
            func_ov009_02166804((u8)(callback->recvBuf->rate_rssi >> 8));
            DC_InvalidateRange(callback->recvBuf, 0x620);
            func_ov009_02166358(callback->recvBuf->srcAdrs, callback->recvBuf->destAdrs,
                                   (u8 *)callback->recvBuf->data, callback->recvBuf->length);
            break;
        default:
            func_ov009_02165a4c(0xb);
            func_ov009_021659b4(7, &work->connectBss, callback->state, 0x4c6);
            break;
        }
        break;
    // WM_ERRCODE_WM_DISABLE, whose warning the SDK leaves out of this build
    case 4:
    default:
        func_ov009_02165a4c(0xb);
        func_ov009_021659b4(7, &work->connectBss, 0, 0x4d1);
        break;
    }
}

// pokebw2's WcmWmcbEndDCF
static void func_ov009_021678ac(void *ind) {
    const WMCallback *callback = ind;
    WCMWork *work = func_ov009_021657c8();

    switch (callback->errcode) {
    case 0:
        if (work->state == 0xc) {
            func_ov009_02165a4c(0xa);
            func_ov009_02166e5c();
        } else {
            switch (WM_Disconnect((WMCallbackFunc)func_ov009_021676e4, 0)) {
            case 2:
                break;
            case 8:
                func_ov009_02165a4c(0xc);
                func_ov009_021659b4(1, &work->connectBss, 0, 0x4fa);
                break;
            case 3:
                func_ov009_02165a4c(0xa);
                func_ov009_02166e5c();
                break;
            default:
                func_ov009_02165a4c(0xb);
                func_ov009_021659b4(7, &work->connectBss, 0, 0x506);
                break;
            }
        }
        break;
    case 1:
    case 3:
        func_ov009_02165a4c(0xa);
        func_ov009_02166e5c();
        break;
    // WM_ERRCODE_WM_DISABLE, whose warning the SDK leaves out of this build
    case 4:
    default:
        func_ov009_02165a4c(0xb);
        func_ov009_021659b4(7, &work->connectBss, 0, 0x519);
        break;
    }
}

// pokebw2's WcmWmcbReset
static void func_ov009_021679c0(void *ind) {
    const WMCallback *callback = ind;
    WCMWork *work = func_ov009_021657c8();
    int result;

    switch (callback->errcode) {
    case 0:
        work->resetPending = 0;
        work->associationId = 0;
        switch (work->state) {
        case 5:
        case 6:
            func_ov009_02165a4c(3);
            func_ov009_021659b4(1, NULL, 0, 0x53c);
            break;
        case 7:
            func_ov009_02165a4c(3);
            func_ov009_021659b4(0, NULL, 0, 0x542);
            break;
        case 8: {
            u16 reason = work->connectionFailureReason;

            work->connectionFailureReason = 0;
            if (reason == 0x12) {
                if ((work->connectBss.rateSet.support & 0x24) != 0x24) {
                    work->connectBss.rateSet.support |= 0x24;
                    result = WM_StartConnectEx(
                        (WMCallbackFunc)func_ov009_021674bc, &work->connectBss, NULL,
                        (work->searchOption & WCM_OPTION_POWER_ACTIVE) == WCM_OPTION_POWER_ACTIVE ? FALSE : TRUE,
                        (work->searchOption & WCM_OPTION_AUTH_SHAREDKEY) == WCM_OPTION_AUTH_SHAREDKEY ? (u16)1
                                                                                                      : (u16)0);
                    switch (result) {
                    case 2:
                        break;
                    case 8:
                        func_ov009_02165a4c(0xc);
                        func_ov009_021659b4(1, &work->connectBss, reason, 0x563);
                        break;
                    // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
                    case 3:
                    default:
                        func_ov009_02165a4c(0xb);
                        func_ov009_021659b4(7, &work->connectBss, reason, 0x56c);
                        break;
                    }
                    return;
                }
            }
            func_ov009_02165a4c(3);
            func_ov009_021659b4(1, &work->connectBss, reason, 0x574);
            break;
        }
        case 9:
        case 12:
            func_ov009_02165a4c(3);
            func_ov009_021659b4(0, &work->connectBss, 1, 0x57c);
            break;
        case 10:
            func_ov009_02165a4c(3);
            func_ov009_021659b4(0, &work->connectBss, 0, 0x583);
            break;
        case 13:
            result = WM_PowerOff((WMCallbackFunc)func_ov009_02166f24);
            switch (result) {
            case 2:
                break;
            case 8:
                func_ov009_02165a4c(0xc);
                func_ov009_021659b4(1, NULL, 0, 0x591);
                break;
            // WM_ERRCODE_ILLEGAL_STATE, whose warning the SDK leaves out of this build
            case 3:
            default:
                func_ov009_02165a4c(0xb);
                func_ov009_021659b4(7, NULL, 0, 0x59a);
                break;
            }
            break;
        default:
            func_ov009_02165a4c(0xb);
            func_ov009_021659b4(7, NULL, work->state, 0x5a0);
            break;
        }
        break;
    default:
        func_ov009_02165a4c(0xb);
        func_ov009_021659b4(7, NULL, 0, 0x5a7);
        break;
    }
}

// pokebw2's WcmEditScanExParam
static void func_ov009_02167cb4(const u8 *bssid, const u8 *essid, u32 option) {
    WCMWork *work = func_ov009_021657c8();
    u16 channelTime;

    func_ov009_021656d0(option);
    work->scanRequest.scanBuf = (WMBssDesc *)work->ioBuffer.scan;
    work->scanRequest.scanBufSize = 0x400;
    work->scanRequest.channelList = (u16)((1 << func_ov009_02165918(0)) >> 1);
    channelTime = work->scanChannelTime;
    if (channelTime == 0) {
        channelTime = WM_GetDispersionScanPeriod();
    }
    work->scanRequest.maxChannelTime = channelTime;
    work->scanRequest.scanType = ((work->searchOption & 0x300000) != 0x300000);
    if (bssid == NULL) {
        MI_CpuCopy8(WCM_Bssid_Any, work->scanRequest.bssid, 6);
    } else {
        MI_CpuCopy8(bssid, work->scanRequest.bssid, 6);
    }
    if (essid == NULL || essid == WCM_Essid_Any) {
        MI_CpuCopy8(WCM_Essid_Any, work->scanRequest.ssid, 0x20);
        work->scanRequest.ssidLength = 0;
    } else {
        s32 i;

        MI_CpuCopy8(essid, work->scanRequest.ssid, 0x20);
        for (i = 0; i < 0x20; i++) {
            if (essid[i] == 0) {
                break;
            }
        }
        work->scanRequest.ssidLength = (u16)i;
    }
    work->scanCount = 0;
}

// pokebw2's WcmWmcbSetDCFData
static void func_ov009_02167de0(void *ind) {
    WCMCpsifWork *global = func_ov009_02166378();
    const WMCallback *result = ind;

    if (result->apiid != 0x12) {
        return;
    }
    global->sendStatus = result->errcode;
    if (result->errcode == 0) {
        func_ov009_02165af4();
    }
    OS_WakeupThread(&global->sendQueue);
}

// pokebw2's WcmWmcbSetDCFDataAsync
static void func_ov009_02167e20(void *ind) {
    WCMCpsifWork *global = func_ov009_02166378();
    const WMCallback *result = ind;
    s32 status;
    void (*completion)(s32);

    if (result->apiid != 0x12) {
        return;
    }
    global->sendStatus = result->errcode;
    if (result->errcode == 0) {
        func_ov009_02165af4();
    }
    switch (global->sendStatus) {
    case 0:
        status = 0;
        break;
    // WM_ERRCODE_FAILED, which the SDK lists with the default
    case 1:
    default:
        status = -5;
        break;
    }
    completion = global->sendCompletion;
    global->sendCompletion = NULL;
    func_ov009_02166594(&global->mutex);
    completion(status);
}
