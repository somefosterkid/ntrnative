// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/wm/src/wm_system.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/wm.h>

#include "wm_arm9_private.h"
#include "system/wm_etc.h"

#define WM_BUF_MSG_NUM 10

// pokebw2's fifoBuf
static u32 data_02153100[WM_BUF_MSG_NUM][WM_FIFO_BUF_SIZE / sizeof(u32)] ATTRIBUTE_ALIGN(32);
static WMPortRecvCallback data_021530b8;
// pokebw2's bufMsg
static OSMessage data_02153090[WM_BUF_MSG_NUM];
// pokebw2's bufMsgQ
static OSMessageQueue data_02153070;

// The manager's state: whether WM_Init has run, the buffer it was given and the callback that ends the game if the
// console sleeps while the manager is on (TwlSDK's, as pokebw2's lib/nitro/src/wm/wm_system.c has it)
static struct {
    u16 initialized;
    WMArm9Buf *wm9buf;
    PMSleepCallbackInfo sleepCallbackInfo;
// pokebw2's sWm
} data_02153058;

static void WmClearFifoRecvFlag(void);
static u32 * WmGetCommandBuffer4Arm7(void);
static WMErrCode WmInitCore(void * wmSysBuf, u16 dmaNo, u32 size);
static void func_0208e5bc(PXIFifoTag fifoTag, uPtr data, BOOL err);
BOOL func_02086cbc(void *buf);
BOOL func_02086c70(void *buf);
void func_0208ea80(void);
static void func_0208ea94(void *arg);
// Black's, which the SDK headers here lack
void PM_InsertPreSleepCallback(PMSleepCallbackInfo *info, int priority);
s32 OS_IsAvailableWireless(void);
s32 OS_IsForceDisableWireless(void);

WMErrCode WM_Init (void * wmSysBuf, u16 dmaNo)
{
    WMErrCode result;
    result = WmInitCore(wmSysBuf, dmaNo, WM_SYSTEM_BUF_SIZE);

    if (result != WM_ERRCODE_SUCCESS) {
        return result;
    }

    data_02153058.wm9buf->scanOnlyFlag = FALSE;
    return result;
}

static WMErrCode WmInitCore (void * wmSysBuf, u16 dmaNo, u32 size)
{
    OSIntrMode e;
    int i;
    int j;

    e = OS_DisableInterrupts();

    if (OS_IsRunOnTwl() && func_02090ed8() != WM_ERRCODE_ILLEGAL_STATE) {
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    if (data_02153058.initialized) {
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    if (wmSysBuf == NULL) {
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (func_02086cbc(wmSysBuf) || (OS_IsRunOnTwl() && func_02086c70(wmSysBuf))) {
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (dmaNo > 3) {
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_INVALID_PARAM;
    }

    if ((u32)wmSysBuf & 0x1f) {
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_INVALID_PARAM;
    }

    PXI_Init();
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WM, 1)) {
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_WM_DISABLE;
    }

    DC_InvalidateRange(wmSysBuf, size);
    MIi_DmaFill32(dmaNo, wmSysBuf, 0, size, TRUE);

    data_02153058.wm9buf = wmSysBuf;
    data_02153058.wm9buf->WM7 = (WMArm7Buf *)((u8 *)wmSysBuf + WM_ARM9WM_BUF_SIZE);
    data_02153058.wm9buf->status = (WMStatus *)((u8 *)data_02153058.wm9buf->WM7 + WM_ARM7WM_BUF_SIZE);
    data_02153058.wm9buf->fifo9to7 = (u32 *)((u8 *)data_02153058.wm9buf->status + WM_STATUS_BUF_SIZE);
    data_02153058.wm9buf->fifo7to9 = (u32 *)((u8 *)data_02153058.wm9buf->fifo9to7 + WM_FIFO_BUF_SIZE);

    WmClearFifoRecvFlag();
    data_02153058.wm9buf->dmaNo = dmaNo;
    data_02153058.wm9buf->connectedAidBitmap = 0;
    data_02153058.wm9buf->myAid = 0;

    for (i = 0; i < WM_NUM_OF_PORT; i++) {
        data_02153058.wm9buf->portCallbackTable[i] = NULL;
        data_02153058.wm9buf->portCallbackArgument[i] = NULL;
    }

    OS_InitMessageQueue(&data_02153070, data_02153090, WM_BUF_MSG_NUM);
    for (j = 0; j < WM_BUF_MSG_NUM; j++) {
        *(u16 *)data_02153100[j] = WM_API_REQUEST_ACCEPTED;
        DC_StoreRange(data_02153100[j], 2);
        OS_SendMessage(&data_02153070, data_02153100[j], OS_MESSAGE_BLOCK);
    }

    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WM, func_0208e5bc);
    data_02153058.initialized = 1;
    (void)OS_RestoreInterrupts(e);
    return WM_ERRCODE_SUCCESS;
}


WMErrCode WM_Finish (void)
{
    OSIntrMode e;
    WMErrCode result;

    e = OS_DisableInterrupts();

    result = WMi_CheckInitialized();
    if (result != WM_ERRCODE_SUCCESS) {
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    result = WMi_CheckState(WM_STATE_READY);
    WM_CHECK_RESULT(result);

    WmClearFifoRecvFlag();
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WM, NULL);
    data_02153058.wm9buf = 0;

    data_02153058.initialized = 0;
    (void)OS_RestoreInterrupts(e);
    return WM_ERRCODE_SUCCESS;
}

void WMi_SetCallbackTable (WMApiid id, WMCallbackFunc callback)
{
    SDK_NULL_ASSERT(data_02153058.wm9buf);
    data_02153058.wm9buf->CallbackTable[id] = callback;
}

u32 * WmGetCommandBuffer4Arm7 (void)
{
    u32 * tmpAddr;
   

    if (FALSE == OS_ReceiveMessage(&data_02153070, (OSMessage *)&tmpAddr, OS_MESSAGE_NOBLOCK)) {
        return NULL;
    }

    DC_InvalidateRange(tmpAddr, 2);
    if ((*((u16 *)tmpAddr) & WM_API_REQUEST_ACCEPTED) == 0) {
        (void)OS_JamMessage(&data_02153070, tmpAddr, OS_MESSAGE_BLOCK);
        return NULL;
    }

    return tmpAddr;
}

WMErrCode WMi_SendCommand (WMApiid id, u16 paramNum, ...)
{
    va_list vlist;
    s32 i;
    int result;
    u32 * tmpAddr;

    SDK_NULL_ASSERT(data_02153058.wm9buf);

    tmpAddr = WmGetCommandBuffer4Arm7();
    if (tmpAddr == NULL) {
        return WM_ERRCODE_FIFO_ERROR;
    }

    *(u16 *)tmpAddr = (u16)id;

    va_start(vlist, paramNum);
    for (i = 0; i < paramNum; i++) {
        tmpAddr[i + 1] = va_arg(vlist, u32);
    }
    va_end(vlist);

    DC_StoreRange(tmpAddr, WM_FIFO_BUF_SIZE);

    result = PXI_SendWordByFifo(PXI_FIFO_TAG_WM, (u32)tmpAddr, FALSE);

    (void)OS_SendMessage(&data_02153070, tmpAddr, OS_MESSAGE_BLOCK);

    if (result < 0) {
        WM_WARNING("Failed to send command through FIFO.\n");
        return WM_ERRCODE_FIFO_ERROR;
    }

    return WM_ERRCODE_OPERATING;
}

WMErrCode WMi_SendCommandDirect (void * data, u32 length)
{
    int result;
    u32 * tmpAddr;

    SDK_NULL_ASSERT(data_02153058.wm9buf);
    SDK_ASSERT(length <= WM_FIFO_BUF_SIZE);

    tmpAddr = WmGetCommandBuffer4Arm7();
    if (tmpAddr == NULL) {
        return WM_ERRCODE_FIFO_ERROR;
    }

    MI_CpuCopy8(data, tmpAddr, length);
    DC_StoreRange(tmpAddr, length);

    result = PXI_SendWordByFifo(PXI_FIFO_TAG_WM, (u32)tmpAddr, FALSE);

    (void)OS_SendMessage(&data_02153070, tmpAddr, OS_MESSAGE_BLOCK);

    if (result < 0) {
        WM_WARNING("Failed to send command through FIFO.\n");
        return WM_ERRCODE_FIFO_ERROR;
    }

    return WM_ERRCODE_OPERATING;
}

WMArm9Buf * WMi_GetSystemWork (void)
{
    return data_02153058.wm9buf;
}

WMErrCode WMi_CheckInitialized (void)
{
    if (!data_02153058.initialized) {
        WM_WARNING("WM library is not initialized yet.\n");
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    return WM_ERRCODE_SUCCESS;
}

WMErrCode WMi_CheckIdle (void)
{
    WMErrCode result;

    result = WMi_CheckInitialized();
    WM_CHECK_RESULT(result);

    DC_InvalidateRange(&(data_02153058.wm9buf->status->state), 2);
    if ((data_02153058.wm9buf->status->state == WM_STATE_READY) || (data_02153058.wm9buf->status->state == WM_STATE_STOP)) {
        WM_WARNING("WM state is \"%d\" now. So can't execute request.\n", data_02153058.wm9buf->status->state);
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    return WM_ERRCODE_SUCCESS;
}

WMErrCode WMi_CheckStateEx (s32 paramNum, ...)
{
    WMErrCode result;
    u16 now;
    u32 temp;
    va_list vlist;

    result = WMi_CheckInitialized();
    WM_CHECK_RESULT(result);

    DC_InvalidateRange(&(data_02153058.wm9buf->status->state), 2);
    now = data_02153058.wm9buf->status->state;

    result = WM_ERRCODE_ILLEGAL_STATE;
    va_start(vlist, paramNum);
    for (; paramNum; paramNum--) {
        temp = va_arg(vlist, u32);
        if (temp == now) {
            result = WM_ERRCODE_SUCCESS;
        }
    }
    va_end(vlist);

    if (result == WM_ERRCODE_ILLEGAL_STATE) {
        WM_WARNING("WM state is \"%d\" now. So can't execute request.\n", now);
    }

    return result;
}

static void func_0208e5bc (PXIFifoTag fifoTag, uPtr data, BOOL err)
{
    WMCallback * callback = (WMCallback *)data;
    WMArm9Buf * w9b = data_02153058.wm9buf;
    WMPortRecvCallback * portRecv;
    WMCallbackFunc func;

    if (err) {
        return;
    }

    DC_InvalidateRange(w9b->fifo7to9, WM_FIFO_BUF_SIZE);
    if (!w9b->scanOnlyFlag) {
        DC_InvalidateRange(w9b->status, WM_STATUS_BUF_SIZE);
    }

    if ((u32 *)callback != w9b->fifo7to9) {
        DC_InvalidateRange(callback, WM_FIFO_BUF_SIZE);
    }

    if (callback->apiid >= WM_NUM_OF_CALLBACK) {
        if (callback->apiid == WM_APIID_INDICATION) {
            if (callback->errcode == WM_ERRCODE_FLASH_ERROR) {
                OS_Terminate();
            }
            if (w9b->indCallback != NULL) {
                w9b->indCallback(callback);
            }
        } else if (callback->apiid == WM_APIID_PORT_RECV) {
            portRecv = (WMPortRecvCallback *)callback;
            if (w9b->portCallbackTable[portRecv->port] != NULL) {
                portRecv->arg = w9b->portCallbackArgument[portRecv->port];
                portRecv->connectedAidBitmap = (u16)w9b->connectedAidBitmap;
                DC_InvalidateRange(portRecv->recvBuf, w9b->status->mp_recvBufSize);
                w9b->portCallbackTable[portRecv->port](portRecv);
            }
        } else if (callback->apiid == WM_APIID_PORT_SEND) {
            portRecv = (WMPortRecvCallback *)callback;
            callback->apiid = WM_APIID_SET_MP_DATA;
            func = (WMCallbackFunc)portRecv->arg;
            if (func != NULL) {
                func(callback);
            }
        }
    } else {
        if (callback->apiid == WM_APIID_START_MP) {
            if (((WMStartMPCallback *)callback)->state == WM_STATECODE_MPEND_IND
                || ((WMStartMPCallback *)callback)->state == WM_STATECODE_MP_IND) {
                if (callback->errcode == WM_ERRCODE_SUCCESS) {
                    DC_InvalidateRange(((WMStartMPCallback *)callback)->recvBuf, w9b->status->mp_recvBufSize);
                }
            }
        }

        if (((callback->apiid == WM_APIID_DISABLE || callback->apiid == WM_APIID_END)
                && callback->errcode == WM_ERRCODE_SUCCESS)
            || ((callback->apiid == WM_APIID_ENABLE || callback->apiid == WM_APIID_INITIALIZE)
                && callback->errcode != WM_ERRCODE_SUCCESS)) {
            func_0208ea80();
        }

        if (callback->apiid == WM_APIID_END && callback->errcode == WM_ERRCODE_SUCCESS) {
            func = w9b->CallbackTable[callback->apiid];
            WM_Finish();
            if (func != NULL) {
                func(callback);
            }
            return;
        }

        if (w9b->CallbackTable[callback->apiid] != NULL) {
            w9b->CallbackTable[callback->apiid](callback);
            if (!data_02153058.initialized) {
                return;
            }
        }

        if (callback->apiid == WM_APIID_START_PARENT || callback->apiid == WM_APIID_START_CONNECT) {
            u16 state;
            u16 aid;
            u16 myAid;
            u16 port;
            u16 reason;
            u8 *macAddress;
            u8 *ssid;
            u16 parentSize;
            u16 childSize;

            if (callback->apiid == WM_APIID_START_PARENT) {
                WMStartParentCallback *parent = (WMStartParentCallback *)callback;

                state = parent->state;
                aid = parent->aid;
                macAddress = parent->macAddress;
                ssid = parent->ssid;
                myAid = 0;
                reason = parent->reason;
                parentSize = parent->parentSize;
                childSize = parent->childSize;
            } else if (callback->apiid == WM_APIID_START_CONNECT) {
                WMStartConnectCallback *child = (WMStartConnectCallback *)callback;

                state = child->state;
                aid = 0;
                macAddress = child->macAddress;
                ssid = NULL;
                myAid = child->aid;
                reason = child->reason;
                parentSize = child->parentSize;
                childSize = child->childSize;
            }

            if (state == WM_STATECODE_CONNECTED || state == WM_STATECODE_DISCONNECTED
                || state == WM_STATECODE_DISCONNECTED_FROM_MYSELF) {
                if (state == WM_STATECODE_CONNECTED) {
                    w9b->connectedAidBitmap |= 1 << aid;
                } else {
                    w9b->connectedAidBitmap &= ~(1 << aid);
                }
                w9b->myAid = myAid;

                MI_CpuFill8(&data_021530b8, 0, sizeof(data_021530b8));
                data_021530b8.apiid = WM_APIID_PORT_RECV;
                data_021530b8.errcode = 0;
                data_021530b8.state = state;
                data_021530b8.recvBuf = NULL;
                data_021530b8.data = NULL;
                data_021530b8.length = 0;
                data_021530b8.aid = aid;
                data_021530b8.myAid = myAid;
                data_021530b8.connectedAidBitmap = w9b->connectedAidBitmap;
                data_021530b8.seqNo = 0xffff;
                data_021530b8.reason = reason;
                MI_CpuCopy8(macAddress, data_021530b8.macAddress, WM_SIZE_MACADDR);
                if (ssid != NULL) {
                    MIi_CpuCopy16(ssid, data_021530b8.ssid, WM_SIZE_CHILD_SSID);
                } else {
                    MIi_CpuClear16(0, data_021530b8.ssid, WM_SIZE_CHILD_SSID);
                }
                data_021530b8.maxSendDataSize = myAid == 0 ? parentSize : childSize;
                if (myAid != 0) {
                    childSize = parentSize;
                }
                data_021530b8.maxRecvDataSize = childSize;

                for (port = 0; port < WM_NUM_OF_PORT; port++) {
                    data_021530b8.port = port;
                    if (w9b->portCallbackTable[port] != NULL) {
                        data_021530b8.arg = w9b->portCallbackArgument[port];
                        w9b->portCallbackTable[port](&data_021530b8);
                    }
                }
            }
        }
    }

    DC_InvalidateRange(w9b->fifo7to9, WM_FIFO_BUF_SIZE);
    WmClearFifoRecvFlag();
    if ((u32 *)callback == w9b->fifo7to9) {
        return;
    }
    callback->apiid |= WM_API_REQUEST_ACCEPTED;
    DC_StoreRange(callback, WM_FIFO_BUF_SIZE);
}

static void WmClearFifoRecvFlag (void)
{
    if (OS_GetSystemWork()->wm_callback_control & WM_EXCEPTION_CB_MASK) {
        OS_GetSystemWork()->wm_callback_control &= ~WM_EXCEPTION_CB_MASK;
    }
}

const WMStatus * WMi_GetStatusAddress (void)
{
    if (WMi_CheckInitialized() != WM_ERRCODE_SUCCESS) {
        return NULL;
    }

    return data_02153058.wm9buf->status;
}

u16 WM_GetAID (void)
{
    u16 myAid;
    OSIntrMode e;

    e = OS_DisableInterrupts();

    if (data_02153058.wm9buf != NULL) {
        myAid = data_02153058.wm9buf->myAid;
    } else {
        myAid = 0;
    }

    (void)OS_RestoreInterrupts(e);

    return myAid;
}

u16 WM_GetConnectedAIDs (void)
{
    u32 connectedAidBitmap;
    OSIntrMode e;

    e = OS_DisableInterrupts();

    if (data_02153058.wm9buf != NULL) {
        connectedAidBitmap = data_02153058.wm9buf->connectedAidBitmap;
    } else {
        connectedAidBitmap = 0;
    }

    (void)OS_RestoreInterrupts(e);

#ifdef WM_DEBUG

#endif

    return (u16)connectedAidBitmap;
}

// The DSi's sleep callback and wireless check, which libntr lacks: TwlSDK's, as pokebw2's wm_system.c has them
void WMi_RegisterSleepCallback(void) {
    PM_SetSleepCallbackInfo(&data_02153058.sleepCallbackInfo, func_0208ea94, NULL);
    PM_InsertPreSleepCallback(&data_02153058.sleepCallbackInfo, 1000);
}

void func_0208ea80(void) {
    PM_DeletePreSleepCallback(&data_02153058.sleepCallbackInfo);
}

// The game ends if the console goes to sleep while the manager is on
static void func_0208ea94(void *arg) {
    OS_Terminate();
}

// Whether wireless is usable
BOOL WMi_IsWirelessAvailable(void) {
    if (OS_IsAvailableWireless() == TRUE && !OS_IsForceDisableWireless()) {
        return TRUE;
    }
    return FALSE;
}
