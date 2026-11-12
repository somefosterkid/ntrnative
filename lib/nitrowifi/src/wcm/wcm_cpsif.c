// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/wcm/wcm_cpsif.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/wcm_dsi.h"
#include "gfl/std.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitrowifi/wcm.h"
#include "wcm_internal.h"
#include "nitro/wm.h"

// NitroWiFi's WCM (overlay 11): its interface to CPS, the TCP/IP stack, which sends and receives frames over the
// connection (DCF). TwlSDK's libwcm names the object cpsif.o; Black 1's decompilation (aletheia) has the same
// functions, in this order, in its wcm_cpsif.c, and the names are the SDK's. The Thumb/ARM pair after
// func_ov009_02166444 is the linker's long-branch veneer to the DSi backend in the LTD autoload.

// The 802.3 header of a mode 2 frame. Only the length, big-endian, is filled in here
// Black 2's; twinned code confirms the offsets of length, and its size in Black
typedef struct WCMEthernetHeader {
    u8 destination[6];
    u8 source[6];
    u16 length;
} WCMEthernetHeader;

// pokebw2's sWcmGlobal
static WCMCpsifWork data_ov009_021aa7dc;

static s32 func_ov009_02166600(s32 mode, void *dest, const void *header, u32 headerLen, const void *data, u32 dataLen);

// pokebw2's WCMi_InitCpsif
void func_ov009_02166304(void) {
    if (!data_ov009_021aa7dc.initialized) {
        data_ov009_021aa7dc.initialized = TRUE;
        data_ov009_021aa7dc.sendStatus = 0;
        MI_CpuFill8(data_ov009_021aa7dc.ownMac, 0, 6);
        OS_InitThreadQueue(&data_ov009_021aa7dc.sendQueue);
        OS_InitMutex(&data_ov009_021aa7dc.mutex);
    }
}

// pokebw2's WCMi_CpsifRecvCallback
void func_ov009_02166358(const u8 *src, const u8 *dst, const u8 *data, s32 len) {
    if (data_ov009_021aa7dc.recvCallback != NULL) {
        data_ov009_021aa7dc.recvCallback(src, dst, data, len);
    }
}

// pokebw2's func_ov009_02166378
WCMCpsifWork *func_ov009_02166378(void) {
    return &data_ov009_021aa7dc;
}

// pokebw2's WCM_GetApMacAddress
u8 *func_ov009_02166384(void) {
    u8 *mac = NULL;
    WCMWork *work = func_ov009_021657c8();
    u32 irq = OS_DisableInterrupts();

    if (work != NULL && work->state == 9 && work->resetPending == 0) {
        mac = work->connectBss.bssid;
    }
    OS_RestoreInterrupts(irq);
    return mac;
}

// The SSID is borrowed from the connected BSS descriptor and is not necessarily NUL-terminated.
// pokebw2's WCM_GetApEssid
u8 *func_ov009_021663c8(u16 *ssidLength) {
    u8 *ssid = NULL;
    u16 length = 0;
    WCMWork *work = func_ov009_021657c8();
    u32 irq = OS_DisableInterrupts();

    if (work != NULL) {
        if (work->state == 9 && work->resetPending == 0) {
            ssid = work->connectBss.ssid;
            length = work->connectBss.ssidLength;
        }
    }
    OS_RestoreInterrupts(irq);
    if (ssidLength != NULL) {
        *ssidLength = length;
    }
    return ssid;
}

// pokebw2's WCM_SetRecvDCFCallback
void func_ov009_02166424(WCMRecvInd callback) {
    u32 irq = OS_DisableInterrupts();
    data_ov009_021aa7dc.recvCallback = callback;
    OS_RestoreInterrupts(irq);
}

// pokebw2's WCM_SendDCFDataEx
s32 func_ov009_02166444(const u8 *dst, const u8 *header, s32 headerLen, const u8 *data, s32 dataLen) {
    u32 irq;
    WCMWork *work;
    s32 sentLen;

    irq = OS_DisableInterrupts();
    work = func_ov009_021657c8();
    if (work == NULL) {
        OS_RestoreInterrupts(irq);
        return -1;
    }
    OS_LockMutex(&data_ov009_021aa7dc.mutex);
    work = func_ov009_021657c8();
    if (work == NULL) {
        OS_UnlockMutex(&data_ov009_021aa7dc.mutex);
        OS_RestoreInterrupts(irq);
        return -1;
    }
    if (work->state != 9 || work->resetPending == 1) {
        OS_UnlockMutex(&data_ov009_021aa7dc.mutex);
        OS_RestoreInterrupts(irq);
        return -4;
    }
    sentLen = func_ov009_02166600(work->wirelessMode, work, header, headerLen, data, dataLen);
    switch (work->wirelessMode) {
    case 1:
        func_ov009_02166d2c(dst, work, (u16)sentLen, NULL);
        break;
    case 2:
        if (OS_IsRunOnTwl()) {
            func_02731e50_unk(dst, work, (u16)sentLen, NULL);
        }
        break;
    default:
        break;
    }
    OS_UnlockMutex(&data_ov009_021aa7dc.mutex);
    OS_RestoreInterrupts(irq);
    return headerLen + dataLen;
}

// pokebw2's WCMi_CpsifUnlockMutexInIRQ
void func_ov009_02166594(OSMutex *mutex) {
    // The IRQ-handler address is an owner marker, not an OSThread object; this path releases no ownership-list link.
    if (mutex->thread == (OSThread *)func_01ff80f0) {
        mutex->count--;
        if (mutex->count == 0) {
            mutex->thread = NULL;
            OS_WakeupThread(&mutex->queue);
        }
    }
}

// pokebw2's WCMi_CpsifSetMacAddress
void func_ov009_021665cc(const u8 *mac) {
    sys_memcpy_fast(mac, data_ov009_021aa7dc.ownMac, 6);
}

// pokebw2's WCM_GetMacAddress
void func_ov009_021665e4(u8 *mac) {
    sys_memcpy_fast(data_ov009_021aa7dc.ownMac, mac, 6);
}

// pokebw2's WCMi_MergeToBuffer
static s32 func_ov009_02166600(s32 mode, void *dest, const void *header, u32 headerLen, const void *data, u32 dataLen) {
    void *payload;
    u32 total;

    total = headerLen + dataLen;
    switch (mode) {
    case 1:
    default:
        payload = dest;
        break;
    case 2:
        // The frame starts with an 802.3 header
        payload = (WCMEthernetHeader *)dest + 1;
        ((WCMEthernetHeader *)dest)->length = (u16)((((u16)total & 0xff00U) >> 8) | (((u16)total & 0x00ff) << 8));
        total += sizeof(WCMEthernetHeader);
        break;
    }
    sys_memcpy_fast(header, payload, headerLen);
    if (data != NULL && dataLen != 0) {
        sys_memcpy_fast(data, (u8 *)payload + headerLen, dataLen);
    }
    return total;
}
