// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/src/nwm/nwm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// TwlSDK's NWM, the DSi's wireless manager, ARM9 side, from func_02702a98 (NWM_End) on: the commands that the ARM9 sends
// the ARM7 over the PXI tag 0x16, each checked against the state that the ARM7 reports, the callback that unloads the
// device before the application exits, and the sending of the commands' buffers. ARM code in the DSi-only LTD autoload.
// The ROM names none of it; the file's name and those of the types and data are descriptive guesses, and the functions
// have the names of the SDK functions they implement where those are known, from what they do, and keep their default
// names otherwise. NWM_Init and its flag are nwm_setup.c's, and the system area and the events nwm_events.c's, as
// pokebw2 has them (Black's text and .bss have the same three files' sizes).

#include "types.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/spi/ARM9/pm.h"
#include "nitro/pxi.h"
#include "nitro/os/common/systemCall.h"
#include "nwm_private.h"
#include "twl/nwm.h"
#include <stdarg.h>

// A pointer in the DSi's shared memory to a block of its settings. The library checks the 16 bytes at 0x1e0 of it:
// a word whose upper half is the CRC-16 of the 12 bytes that follow it
#define NWM_SETTINGS_PTR 0x02fffdfc

static NWMRetCode func_02702ca0(NWMCallback callback);
static void func_02702d68(void *event);
static void func_02702d7c(void *arg);
static void func_02703764(const u8 *destAddr, const u8 *srcAddr, u8 *frame, u32 size);
static void func_027038bc(void *event);
static void func_02703a34(u32 cmd, NWMCallback callback);
static void func_02703a54(u32 index, NWMCallback callback);
static NWMRetCode func_02703a74(u32 cmd, u16 count, ...);
static NWMRetCode func_02703b24(const void *cmd, u32 size);
static NWMCmdBuf *func_02703b98(void);
static BOOL func_02703c28(void);

// The data, declared in the reverse of their order in memory, which -ipa function lays them out in: data_027341c8
// (set when the exit callback's unload is done) and data_027341cc (the block that the command 8 copies for the ARM7)
static u32 data_027341cc[0x210 / 4];
static u8 data_027341c8;

// Whether the ARM7 is running the command: it sets the command's byte in its status block while it does
static inline BOOL NWMi_IsPending(u32 cmd) {
    NWMSys *sys = func_027026dc();
    BOOL pending;

    if (sys != NULL) {
        DC_InvalidateRange(&sys->status->pending[cmd], 1);
        pending = sys->status->pending[cmd] == 1;
    } else {
        pending = FALSE;
    }
    return pending;
}

// pokebw2's NWM_End
NWMRetCode func_02702a98(void) {
    u32 irq;
    NWMRetCode result;

    result = func_027028e4(1, NWM_STATE_INITIALIZED);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    irq = OS_DisableInterrupts();
    data_027341a4->status->state = NWM_STATE_NONE;
    func_027026ec();
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_NWM, NULL);
    data_027341a4 = NULL;
    data_027341a0 = FALSE;
    OS_RestoreInterrupts(irq);
    return NWM_RETCODE_SUCCESS;
}

// pokebw2's NWM_LoadDevice
NWMRetCode func_02702b08(NWMCallback callback) {
    NWMSys *sys = func_027026dc();
    NWMRetCode result;
    PMExitCallbackInfo *info;

    result = func_027028e4(1, NWM_STATE_INITIALIZED);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (NWMi_IsPending(NWM_APIID_LOAD_DEVICE) == TRUE) {
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    func_02703c28();
    func_02703a34(NWM_APIID_LOAD_DEVICE, callback);
    func_02702a40();
    info = &data_027341a8;
    PM_SetExitCallbackInfo(info, func_02702d7c, NULL);
    PM_InsertPreExitCallback(info, NWM_PM_PRIORITY);
    result = func_02703a74(NWM_APIID_LOAD_DEVICE, 4, sys->info, sys->status, sys->event, 0);
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

// pokebw2's NWM_UnloadDevice
NWMRetCode func_02702bfc(NWMCallback callback) {
    NWMRetCode result;

    result = func_027028e4(1, NWM_STATE_LOADED);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (NWMi_IsPending(NWM_APIID_UNLOAD_DEVICE) == TRUE) {
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    func_02703a34(NWM_APIID_UNLOAD_DEVICE, callback);
    func_027026dc();
    result = func_02703a74(NWM_APIID_UNLOAD_DEVICE, 1, 0);
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

// Unloads the device in any state after the initialized one
static NWMRetCode func_02702ca0(NWMCallback callback) {
    NWMRetCode result;

    result =
        func_027028e4(5, NWM_STATE_INITIALIZED, NWM_STATE_LOADED, NWM_STATE_READY, NWM_STATE_CONNECTED, NWM_STATE_5);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (NWMi_IsPending(NWM_APIID_UNLOAD_DEVICE) == TRUE) {
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    func_02703a34(NWM_APIID_UNLOAD_DEVICE, callback);
    func_027026dc();
    result = func_02703a74(NWM_APIID_UNLOAD_DEVICE, 1, 1);
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

static void func_02702d68(void *event) {
    data_027341c8 = TRUE;
}

// Runs before the application exits: waits for the ARM7 to finish what it is doing, then unloads the device
static void func_02702d7c(void *arg) {
    s32 timeout;
    NWMSys *sys;

    if (!PMi_GetExitType()) {
        return;
    }
    sys = func_027026dc();
    timeout = NWM_EXIT_TIMEOUT;
    if (sys == NULL) {
        return;
    }
    OS_SpinWaitSysCycles(NWM_EXIT_SPIN_FIRST);
    while ((NWMi_IsPending(NWM_APIID_LOAD_DEVICE) == TRUE && func_027028b0() == 1) ||
           NWMi_IsPending(NWM_APIID_RESET) == TRUE || NWMi_IsPending(NWM_APIID_INSTALL_FIRMWARE) == TRUE) {
        OS_SpinWaitSysCycles(NWM_EXIT_SPIN_STEP);
        PXIi_HandlerRecvFifoNotEmpty();
    }
    if (func_027028e4(4, NWM_STATE_LOADED, NWM_STATE_READY, NWM_STATE_CONNECTED, NWM_STATE_5) != 0) {
        return;
    }
    data_027341c8 = FALSE;
    func_02702ca0(func_02702d68);
    do {
        OS_SpinWaitSysCycles(NWM_EXIT_SPIN_STEP);
        PXIi_HandlerRecvFifoNotEmpty();
        timeout -= NWM_EXIT_SPIN_STEP;
        if (timeout < 0) {
            return;
        }
    } while (!data_027341c8);
}

// pokebw2's NWM_Open
NWMRetCode func_02702f38(NWMCallback callback) {
    NWMRetCode result;

    if (!func_027026b4()) {
        return NWM_RETCODE_DISABLED;
    }
    result = func_027028e4(1, NWM_STATE_LOADED);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (NWMi_IsPending(NWM_APIID_OPEN) == TRUE) {
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    func_02703a34(NWM_APIID_OPEN, callback);
    func_027026dc();
    result = func_02703a74(NWM_APIID_OPEN, 0);
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

// pokebw2's NWM_Close
NWMRetCode func_02702fe8(NWMCallback callback) {
    NWMRetCode result;

    result = func_027028e4(1, NWM_STATE_READY);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (NWMi_IsPending(NWM_APIID_CLOSE) == TRUE) {
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    func_02703a34(NWM_APIID_CLOSE, callback);
    func_027026dc();
    result = func_02703a74(NWM_APIID_CLOSE, 0);
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

// The command of a scan, with the fields that it takes from the parameters
typedef struct NWMScanCmd {
    u16 cmd;
    u16 channelList;
    u16 bufferSize;
    u16 dwellTime;
    u8 *buffer;
    u16 scanType;
    u16 ssidLength;
    u16 ssidMatchLength;
    u8 ssid[32];
    u8 bssid[6];
} NWMScanCmd;

// pokebw2's NWM_StartScan
NWMRetCode func_02703088(NWMCallback callback, const NWMScanExParam *param) {
    NWMScanCmd cmd;
    NWMRetCode result;

    result = func_027028e4(1, NWM_STATE_READY);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (NWMi_IsPending(NWM_APIID_START_SCAN) == TRUE) {
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    if (param == NULL) {
        return NWM_RETCODE_INVALID_PARAM;
    }
    if (param->scanBuf == NULL) {
        return NWM_RETCODE_INVALID_PARAM;
    }
    if (param->ssidLength > 32) {
        return NWM_RETCODE_INVALID_PARAM;
    }
    if (param->ssidMatchLength > 32) {
        return NWM_RETCODE_INVALID_PARAM;
    }
    MI_CpuFill8(param->scanBuf, 0, (param->scanBufSize + 3) & ~3);
    DC_StoreRange(param->scanBuf, (param->scanBufSize + 3) & ~3);
    func_02703a34(NWM_APIID_START_SCAN, callback);
    cmd.cmd = NWM_APIID_START_SCAN;
    cmd.channelList = param->channelList;
    cmd.bufferSize = param->scanBufSize;
    cmd.dwellTime = param->channelDwellTime;
    cmd.buffer = (u8 *)param->scanBuf;
    cmd.scanType = param->scanType;
    cmd.ssidLength = param->ssidLength;
    cmd.ssidMatchLength = param->ssidMatchLength;
    MI_CpuCopy8(param->ssid, cmd.ssid, 32);
    MI_CpuCopy8(param->bssid, cmd.bssid, 6);
    result = func_02703b24(&cmd, sizeof(cmd));
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

u8 *func_02703208(NWMBssDesc *bssDesc, u32 id) {
    u8 *element = bssDesc->elements;
    u8 *end = element + bssDesc->length - NWM_BSS_DESC_ELEMENTS_OFFSET;

    while (element[0] != id && element + element[1] + 2 <= end) {
        element += element[1] + 2;
    }
    return element[0] == id ? element : NULL;
}

u8 *func_02703250(NWMBssDesc *bssDesc, u32 id, const u8 *oui) {
    u8 *element = bssDesc->elements;
    u8 *end = element + bssDesc->length - NWM_BSS_DESC_ELEMENTS_OFFSET;

    if (id != 0xdd) {
        return NULL;
    }
    while (element + element[1] + 2 <= end) {
        if (element[0] == id && sys_memcmp(element + 2, oui, 4) == 0) {
            return element;
        }
        element += element[1] + 2;
    }
    return NULL;
}

// The command that sends the block, which the ARM7 reads from data_027341cc
typedef struct NWMBlockCmd {
    u16 cmd;
    u8 index;
    void *data;
} NWMBlockCmd;

NWMRetCode func_027032e0(NWMCallback callback, u32 index, const NWMBlockParam *data) {
    NWMBlockCmd cmd;
    NWMRetCode result;

    result = func_027028e4(1, NWM_STATE_READY);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (NWMi_IsPending(NWM_APIID_CONNECT) == TRUE) {
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    if (data == NULL) {
        return NWM_RETCODE_INVALID_PARAM;
    }
    if (data->length > 0x200) {
        return NWM_RETCODE_INVALID_PARAM;
    }
    if (index >= 3) {
        return NWM_RETCODE_INVALID_PARAM;
    }
    if (data->unk_0a == 0) {
        return NWM_RETCODE_INVALID_PARAM;
    }
    // The block is cleaned for twice its length and copied for its length: one of them is not in bytes
    DC_StoreRange(data, data->length * 2);
    func_02703a34(NWM_APIID_CONNECT, callback);
    cmd.cmd = NWM_APIID_CONNECT;
    cmd.index = index;
    cmd.data = data_027341cc;
    MI_CpuCopy8(data, data_027341cc, data->length);
    DC_StoreRange(cmd.data, 0x200);
    DC_StoreRange(&cmd, 8);
    result = func_02703b24(&cmd, 8);
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

// pokebw2's NWM_Disconnect
NWMRetCode func_02703424(NWMCallback callback) {
    NWMRetCode result;

    result = func_027028e4(1, NWM_STATE_CONNECTED);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (NWMi_IsPending(NWM_APIID_DISCONNECT) == TRUE) {
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    func_02703a34(NWM_APIID_DISCONNECT, callback);
    result = func_02703a74(NWM_APIID_DISCONNECT, 0);
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

// The command that sets the WEP keys: the WEP mode becomes the key's length in bytes, and the 0x50 bytes of the keys go
// along (pokebw2's NWMWepKeyCmd)
typedef struct NWMWepKeyCmd {
    u16 cmd;
    u16 authMode;
    u16 wepkeyid;
    u16 keyLength;
    u8 wepkey[0x50];
} NWMWepKeyCmd;

// pokebw2's NWM_SetWEPKey
NWMRetCode func_027034c0(NWMCallback callback, u32 wepmode, u32 wepkeyid, const void *wepkey, u16 authMode) {
    NWMWepKeyCmd cmd;
    NWMRetCode result;
    u16 keyLength;

    result = func_027028e4(1, NWM_STATE_READY);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (NWMi_IsPending(NWM_APIID_SET_WEPKEY) == TRUE) {
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    switch (wepmode) {
    case 0:
        keyLength = 0;
        break;
    case 1:
        keyLength = 5;
        break;
    case 2:
        keyLength = 13;
        break;
    case 3:
        keyLength = 16;
        break;
    default:
        return NWM_RETCODE_INVALID_PARAM;
    }
    DC_StoreRange(wepkey, 0x50);
    func_02703a34(NWM_APIID_SET_WEPKEY, callback);
    cmd.cmd = NWM_APIID_SET_WEPKEY;
    cmd.wepkeyid = wepkeyid;
    cmd.keyLength = keyLength;
    cmd.authMode = authMode;
    MI_CpuCopy8(wepkey, cmd.wepkey, 0x50);
    result = func_02703b24(&cmd, sizeof(cmd));
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

NWMRetCode func_027035f4(NWMCallback callback, u32 arg) {
    NWMRetCode result;

    result = func_027028e4(2, NWM_STATE_READY, NWM_STATE_CONNECTED);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (NWMi_IsPending(NWM_APIID_SET_PS_MODE) == TRUE) {
        return NWM_RETCODE_ILLEGAL_STATE;
    }
    func_02703a34(NWM_APIID_SET_PS_MODE, callback);
    func_027026dc();
    result = func_02703a74(NWM_APIID_SET_PS_MODE, 1, arg);
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

// pokebw2's NWM_SendFrame
NWMRetCode func_027036a0(NWMCallback callback, const u8 *destAddr, u8 *frame, u32 size) {
    NWMSys *sys = func_027026dc();
    NWMRetCode result;
    u32 irq;

    result = func_027028e4(1, NWM_STATE_CONNECTED);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    irq = OS_DisableInterrupts();
    if (size > 0x5f2) {
        OS_RestoreInterrupts(irq);
        return NWM_RETCODE_INVALID_PARAM;
    }
    DC_StoreRange(frame, size);
    func_02703764(destAddr, sys->status->macAddr, frame, size);
    DC_StoreRange(frame, 14);
    result = func_02703a74(NWM_APIID_SEND_FRAME, 3, callback, frame, size);
    // Only the result 0 leaves without restoring the interrupts, and the command never gives it
    if (result == NWM_RETCODE_SUCCESS) {
        return NWM_RETCODE_OPERATING;
    }
    OS_RestoreInterrupts(irq);
    return result;
}

// Writes the two MAC addresses at the start of the frame
static void func_02703764(const u8 *destAddr, const u8 *srcAddr, u8 *frame, u32 size) {
    if (size < 14) {
        return;
    }
    MI_CpuCopy8(destAddr, frame, 6);
    MI_CpuCopy8(srcAddr, frame + 6, 6);
}

// pokebw2's NWM_SetReceivingFrameBuffer
NWMRetCode func_0270379c(NWMCallback receiveCallback, void *buffer, u32 size, u32 index) {
    NWMSys *sys = func_027026dc();
    NWMRetCode result;

    result = func_027028e4(2, NWM_STATE_READY, NWM_STATE_CONNECTED);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    MIi_DmaFill32(sys->dmaNo, buffer, 0, size, TRUE);
    DC_StoreRange(buffer, size);
    func_02703a34(NWM_APIID_SET_RECEIVING_BUF, func_027038bc);
    func_02703a54(index, receiveCallback);
    result = func_02703a74(NWM_APIID_SET_RECEIVING_BUF, 3, buffer, size, index);
    if (result == NWM_RETCODE_SUCCESS) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

NWMRetCode func_02703854(NWMCallback callback, u32 index) {
    NWMRetCode result;

    func_027026dc();
    result = func_027028e4(2, NWM_STATE_READY, NWM_STATE_CONNECTED);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    func_02703a34(NWM_APIID_UNSET_RECEIVING_BUF, callback);
    func_02703a54(index, NULL);
    result = func_02703a74(NWM_APIID_UNSET_RECEIVING_BUF, 1, index);
    if (result == NWM_RETCODE_SUCCESS) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

// The callback of the command that gives the ARM7 a buffer to receive in: runs the callback of the event's index
// with the event, and when the buffer is full gives it to the ARM7 again
static void func_027038bc(void *event) {
    NWMEvent *ev = event;
    NWMSys *sys = func_027026dc();
    BOOL ok;

    if (ev->result != 0) {
        if (ev->result != NWM_RESULT_BUFFER_FULL) {
            return;
        }
        ok = FALSE;
    } else {
        ok = TRUE;
    }
    if (sys->indicate[ev->u04.index] != NULL) {
        DC_StoreRange(ev, 16);
        sys->indicate[ev->u04.index](ev);
    }
    if (ok) {
        return;
    }
    func_02703a74(NWM_APIID_SET_RECEIVING_BUF, 3, ev->u0c.buffer, 0, ev->u04.index);
}

// The command that sets the key
typedef struct NWMPmkCmd {
    u16 cmd;
    u16 type;
    u8 pmk[32];
} NWMPmkCmd;

NWMRetCode func_02703960(NWMCallback callback, NWMWpaParam *param) {
    NWMPmkCmd cmd;
    NWMRetCode result;

    result = func_027028e4(1, NWM_STATE_READY);
    if (result != NWM_RETCODE_SUCCESS && result != NWM_RETCODE_OPERATING) {
        return result;
    }
    if (param == NULL) {
        return NWM_RETCODE_INVALID_PARAM;
    }
    DC_StoreRange(param, 0x22);
    switch (param->auth) {
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    default:
        return NWM_RETCODE_INVALID_PARAM;
    }
    func_02703a34(NWM_APIID_SET_WPA_PSK, callback);
    cmd.cmd = NWM_APIID_SET_WPA_PSK;
    cmd.type = param->auth;
    MI_CpuCopy8(param->psk, cmd.pmk, 32);
    result = func_02703b24(&cmd, sizeof(cmd));
    if (result == NWM_RETCODE_SUCCESS || result == NWM_RETCODE_OPERATING) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

static void func_02703a34(u32 cmd, NWMCallback callback) {
    func_027026dc()->callbacks[cmd] = callback;
}

static void func_02703a54(u32 index, NWMCallback callback) {
    func_027026dc()->indicate[index] = callback;
}

// Sends a command with its words
static NWMRetCode func_02703a74(u32 cmd, u16 count, ...) {
    int sent;
    va_list args;
    int i;
    NWMCmdBuf *buf;
    NWMSys *sys = func_027026dc();
    NWMRetCode result;

    buf = func_02703b98();
    if (buf == NULL) {
        return NWM_RETCODE_SEND_ERROR;
    }
    buf->cmd = cmd;
    va_start(args, count);
    for (i = 0; i < count; i++) {
        buf->args[i] = va_arg(args, u32);
    }
    DC_StoreRange(buf, sizeof(NWMCmdBuf));
    sent = PXI_SendWordByFifo(PXI_FIFO_TAG_NWM, (u32)buf, FALSE);
    OS_SendMessage(&sys->freeQueue, buf, OS_MESSAGE_BLOCK);
    result = NWM_RETCODE_SEND_ERROR;
    if (sent >= 0) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

// Sends a command that is a struct
static NWMRetCode func_02703b24(const void *cmd, u32 size) {
    NWMSys *sys = func_027026dc();
    NWMCmdBuf *buf;
    int sent;
    NWMRetCode result;

    buf = func_02703b98();
    if (buf == NULL) {
        return NWM_RETCODE_SEND_ERROR;
    }
    MI_CpuCopy8(cmd, buf, size);
    DC_StoreRange(buf, size);
    sent = PXI_SendWordByFifo(PXI_FIFO_TAG_NWM, (u32)buf, FALSE);
    OS_SendMessage(&sys->freeQueue, buf, OS_MESSAGE_BLOCK);
    result = NWM_RETCODE_SEND_ERROR;
    if (sent >= 0) {
        result = NWM_RETCODE_OPERATING;
    }
    return result;
}

// Takes the next command buffer that the ARM7 is done with, or NULL when the queue is empty
static NWMCmdBuf *func_02703b98(void) {
    NWMCmdBuf *buf = NULL;
    NWMSys *sys = func_027026dc();

    do {
        if (!OS_ReceiveMessage(&sys->freeQueue, (OSMessage *)&buf, OS_MESSAGE_NOBLOCK)) {
            return NULL;
        }
        DC_InvalidateRange(sys, 0x2000);
        DC_InvalidateRange(buf, 2);
        if (!(buf->cmd & NWM_CMD_BUF_FREE)) {
            OS_SendMessage(&sys->freeQueue, buf, OS_MESSAGE_BLOCK);
            buf = NULL;
        }
    } while (buf == NULL);
    return buf;
}

// Whether the CRC of the wireless settings is right (the result is not used)
static BOOL func_02703c28(void) {
    u8 *settings = *(u8 **)NWM_SETTINGS_PTR;

    return GetCRC16(0xffff, settings + 0x1e4, 12) == *(u32 *)(settings + 0x1e0) >> 16;
}
