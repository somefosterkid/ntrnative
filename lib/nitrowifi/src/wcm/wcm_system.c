// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/wcm/wcm_system.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/wcm_dsi.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/wm.h"
#include "nitrowifi/wcm.h"
#include "wcm_internal.h"

// NitroWiFi's WCM (overlay 11): its initialization, its public operations and their state machine, which hands each
// step to the DS's backend over NitroSDK's WM (wcm_wmif.c) or the DSi's over TwlSDK's NWM (wcm_dsi.c, in the LTD
// autoload). TwlSDK's libwcm names the object system.o; Black 1's decompilation (aletheia) has the same functions, in
// this order, in its wcm_system.c, and the names are the SDK's. The seven Thumb/ARM pairs after the operations that
// call the DSi backend are the linker's long-branch veneers to the LTD autoload, which the C leaves to the linker.

// The "any" BSSID and ESSID that a search takes when the caller gives none
const u8 WCM_Bssid_Any[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00 };
const u8 WCM_Essid_Any[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

// The device ID (1 for the DS's wireless, 2 for the DSi's) that func_ov009_02164d74 takes when its caller gives 0, ahead of the
// hardware's default; nothing in the ROM sets it
// pokebw2's WCMi_OverridedDeviceId
static u8 data_ov009_021aa7d4 = 0;
// WCM's work, which func_ov009_02164d74 sets and func_ov009_02164ee0 clears. The two are initialized statics, which MWCC reaches from the
// first one's address
static WCMWork *wcmw = NULL;

static BOOL func_ov009_02164f38(s32 *size, s32 mode);
static s32 func_ov009_02165164(const u8 *bssid, const u8 *essid, u32 option);
static void func_ov009_021657d8(WCMConfig *config, WCMNotify notify);
static void func_ov009_021658f8(void);
static void func_ov009_02165b5c(void);
static s32 func_ov009_02165b68(void);

// pokebw2's WCM_Init
s32 func_ov009_02164d64(void *work, s32 size) {
    return func_ov009_02164d74(work, size, 1);
}

// pokebw2's WCM_InitEx
s32 func_ov009_02164d74(void *work, s32 size, s32 requestedMode) {
    OSIntrMode irq = OS_DisableInterrupts();
    s32 requiredSize = 0;

    if (wcmw != NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (work == NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (((u32)work & 0x1f) != 0) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    MI_CpuFill8(work, 0, size);
    if (requestedMode == 0) {
        requestedMode = data_ov009_021aa7d4;
        if (requestedMode == 0) {
            requestedMode = func_ov009_02165b68();
        }
    }
    if (!func_ov009_02164f38(&requiredSize, requestedMode)) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (size < requiredSize) {
        OS_RestoreInterrupts(irq);
        return 6;
    }
    wcmw = work;
    wcmw->state = 1;
    wcmw->pendingNotify = 0;
    wcmw->scanChannelTime = 0;
    wcmw->apListLocked = 0;
    wcmw->resetPending = 0;
    wcmw->wirelessMode = requestedMode;
    func_ov009_021658f8();
    func_ov009_02166304();
    if (!OS_IsTickAvailable()) {
        OS_InitTick();
    }
    if (!OS_IsAlarmAvailable()) {
        OS_InitAlarm();
    }
    OS_CreateAlarm(&wcmw->alarm);
    OS_RestoreInterrupts(irq);
    return 0;
}

// pokebw2's WCM_Finish
s32 func_ov009_02164ee0(void) {
    OSIntrMode irq = OS_DisableInterrupts();

    if (wcmw == NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (wcmw->state != 1) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    wcmw = NULL;
    OS_RestoreInterrupts(irq);
    return 0;
}

// pokebw2's WCM_GetWorkSize
static BOOL func_ov009_02164f38(s32 *size, s32 mode) {
    if (mode == 0) {
        mode = func_ov009_02165b68();
    }
    switch (mode) {
    case 1:
        *size = WCM_WORK_SIZE;
        break;
    case 2:
        if (!OS_IsRunOnTwl()) {
            return FALSE;
        }
        *size = 0x4800;
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

// pokebw2's WCM_StartupAsync
s32 func_ov009_02164f98(WCMConfig *config, WCMNotify notify) {
    OSIntrMode irq;
    s32 result;

    irq = OS_DisableInterrupts();
    if (wcmw == NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    switch (wcmw->state) {
    case 1:
        func_ov009_021657d8(config, notify);
        break;
    case 2:
        OS_RestoreInterrupts(irq);
        return 2;
    case 3:
        OS_RestoreInterrupts(irq);
        return 0;
    default:
        OS_RestoreInterrupts(irq);
        return 1;
    }

    if (!WM_GetAllowedChannel()) {
        // BUG: Returns with interrupts still disabled
#ifdef BUGFIX
        OS_RestoreInterrupts(irq);
#endif
        return 5;
    }
    switch (wcmw->wirelessMode) {
    case 1:
        result = func_ov009_02166880();
        break;
    case 2:
        if (OS_IsRunOnTwl()) {
            result = func_0273199c_unk();
            break;
        }
        // Mode 2 without DSi hardware is unsupported, like any other mode
    default:
        result = 7;
        break;
    }
    OS_RestoreInterrupts(irq);
    return result;
}

// pokebw2's WCM_CleanupAsync
s32 func_ov009_02165088(void) {
    OSIntrMode irq;
    WCMWork *work;
    s32 result;

    irq = OS_DisableInterrupts();
    work = wcmw;
    if (work == NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    switch (work->state) {
    case 3:
        break;
    case 4:
        OS_RestoreInterrupts(irq);
        return 2;
    case 1:
        OS_RestoreInterrupts(irq);
        return 0;
    default:
        OS_RestoreInterrupts(irq);
        return 1;
    }

    switch (work->wirelessMode) {
    case 1:
        result = func_ov009_021669b8();
        break;
    case 2:
        if (OS_IsRunOnTwl()) {
            result = func_02731a84_unk();
            break;
        }
        // Mode 2 without DSi hardware is unsupported, like any other mode
    default:
        result = 7;
        break;
    }
    OS_RestoreInterrupts(irq);
    return result;
}

// pokebw2's WCM_SearchAsync
s32 func_ov009_02165144(void *bssid, void *essid, u32 option) {
    if (bssid == NULL || essid == NULL) {
        return func_ov009_02165204();
    }
    return func_ov009_02165164(bssid, essid, option);
}

// pokebw2's WCM_BeginSearchAsync
static s32 func_ov009_02165164(const u8 *bssid, const u8 *essid, u32 option) {
    OSIntrMode irq;
    WCMWork *work;
    s32 result;

    irq = OS_DisableInterrupts();
    work = wcmw;
    if (work == NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    switch (work->wirelessMode) {
    case 1:
        result = func_ov009_02166a24(bssid, essid, option);
        break;
    case 2:
        if (OS_IsRunOnTwl()) {
            result = func_02731af0_unk(bssid, essid, option);
            break;
        }
        // Mode 2 without DSi hardware is unsupported, like any other mode
    default:
        result = 7;
        break;
    }
    OS_RestoreInterrupts(irq);
    return result;
}

// pokebw2's WCM_EndSearchAsync
s32 func_ov009_02165204(void) {
    OSIntrMode irq = OS_DisableInterrupts();

    if (wcmw == NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    switch (wcmw->state) {
    case 6:
        func_ov009_02165a4c(7);
        wcmw->pendingNotify = WCM_NOTIFY_END_SEARCH;
        break;
    case 7:
        OS_RestoreInterrupts(irq);
        return 2;
    case 3:
        OS_RestoreInterrupts(irq);
        return 0;
    default:
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 3;
}

// pokebw2's WCM_ConnectAsync
s32 func_ov009_021652a4(void *bssDesc, void *wepDesc, u32 option) {
    u32 irq;
    WCMWork *work;
    s32 result;

    irq = OS_DisableInterrupts();
    work = func_ov009_021657c8();
    if (work == NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    switch (work->state) {
    case 3:
        if (bssDesc == NULL) {
            OS_RestoreInterrupts(irq);
            return 1;
        }
        if (((const WMBssDesc *)bssDesc)->gameInfoLength != 0) {
            OS_RestoreInterrupts(irq);
            return 1;
        }
        if (wepDesc != NULL) {
            const u8 *descriptor = wepDesc;

            // BUG: The original checks the previous argument byte, allowing a new length beyond the apparent 0x40
            // limit.
#ifdef BUGFIX
            if ((descriptor[0] <= 3 && descriptor[1] >= 4) || descriptor[1] > 0x40) {
#else
            if ((descriptor[0] <= 3 && descriptor[1] >= 4) || work->connectionStorage.argument > 0x40) {
#endif
                OS_RestoreInterrupts(irq);
                return 1;
            }
            work->connectionStorage.mode = descriptor[0];
            if (descriptor[0] == 0) {
                MI_CpuFill8(work->connectionStorage.data, 0, sizeof(work->connectionStorage.data));
            } else if (descriptor[0] <= 3) {
                work->connectionStorage.argument = descriptor[1];
                MI_CpuCopy8(descriptor + 2, work->connectionStorage.data, sizeof(work->connectionStorage.data));
            } else {
                work->connectionStorage.argument = descriptor[1];
                MI_CpuCopy8(descriptor + 2, work->connectionStorage.data, work->connectionStorage.argument);
            }
        } else {
            MI_CpuFill8(&work->connectionStorage, 0, sizeof(work->connectionStorage));
        }
        MI_CpuCopy8(bssDesc, &work->connectBss, sizeof(work->connectBss));
        func_ov009_021656d0(option);
        break;
    case 8:
        OS_RestoreInterrupts(irq);
        return 2;
    case 9:
        OS_RestoreInterrupts(irq);
        return 0;
    default:
        OS_RestoreInterrupts(irq);
        return 1;
    }

    switch (work->wirelessMode) {
    case 1:
        result = func_ov009_02166b2c();
        break;
    case 2:
        if (OS_IsRunOnTwl()) {
            result = func_02731bf8_unk();
            break;
        }
        // Mode 2 without DSi hardware is unsupported, like any other mode
    default:
        result = 7;
        break;
    }
    OS_RestoreInterrupts(irq);
    return result;
}

// pokebw2's WCM_DisconnectAsync
s32 func_ov009_02165478(void) {
    u32 irq = OS_DisableInterrupts();
    s32 result = 0;
    WCMWork *work = wcmw;

    if (work == NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    switch (work->state) {
    case 9:
        break;
    case 10:
        OS_RestoreInterrupts(irq);
        return 2;
    case 3:
        OS_RestoreInterrupts(irq);
        return 0;
    default:
        OS_RestoreInterrupts(irq);
        return 1;
    }

    if (work->resetPending == 1) {
        func_ov009_02165a4c(10);
        wcmw->pendingNotify = 6;
    } else {
        switch (work->wirelessMode) {
        case 1:
            result = func_ov009_02166bd8();
            break;
        case 2:
            if (OS_IsRunOnTwl()) {
                result = func_02731cfc_unk();
                break;
            }
            // Mode 2 without DSi hardware is unsupported, like any other mode
        default:
            result = 7;
            break;
        }
    }
    OS_RestoreInterrupts(irq);
    return result;
}

// pokebw2's WCM_TerminateAsync
s32 func_ov009_02165560(void) {
    u32 irq = OS_DisableInterrupts();
    s32 result = 0;

    if (wcmw == NULL) {
        OS_RestoreInterrupts(irq);
        return 1;
    }
    switch (wcmw->state) {
    case 13:
        OS_RestoreInterrupts(irq);
        return 2;
    case 1:
        OS_RestoreInterrupts(irq);
        return 0;
    case 6:
        func_ov009_02165a4c(13);
        wcmw->pendingNotify = 9;
        OS_RestoreInterrupts(irq);
        return 3;
    case 3:
    case 9:
    case 12:
        break;
    default:
        OS_RestoreInterrupts(irq);
        return 1;
    }

    if (wcmw->resetPending == 1) {
        func_ov009_02165a4c(13);
        wcmw->pendingNotify = 9;
    } else {
        switch (wcmw->wirelessMode) {
        case 1:
            result = func_ov009_02166c44();
            break;
        case 2:
            if (OS_IsRunOnTwl()) {
                result = func_02731d6c_unk();
                break;
            }
            // Mode 2 without DSi hardware is unsupported, like any other mode
        default:
            result = 7;
            break;
        }
    }
    OS_RestoreInterrupts(irq);
    return result;
}

// pokebw2's WCM_GetPhase
s32 func_ov009_021656a0(void) {
    u32 irq;
    WCMWork *work;
    s32 state;

    irq = OS_DisableInterrupts();
    work = wcmw;
    state = 0;
    if (work != NULL) {
        state = work->state;
    }
    OS_RestoreInterrupts(irq);
    return state;
}

// pokebw2's WCM_UpdateOption
u32 func_ov009_021656d0(u32 option) {
    u32 irq = OS_DisableInterrupts();
    u32 mask = 0;
    // BUG: The original reads the search option before checking whether WCM has been initialized.
#ifdef BUGFIX
    u32 old;

    if (wcmw == NULL) {
        OS_RestoreInterrupts(irq);
        return 0;
    }
    old = wcmw->searchOption;
#else
    u32 old = wcmw->searchOption;

    if (wcmw == NULL) {
        OS_RestoreInterrupts(irq);
        return 0;
    }
#endif
    if (option & 0x8000) {
        mask |= 0x3ffe;
        if (!(option & 0x3ffe)) {
            option |= 0xa082;
        }
    }
    if (option & 0x20000) {
        mask |= 0x10000;
    }
    if (option & 0x80000) {
        mask |= 0x40000;
    }
    if (option & 0x200000) {
        mask |= 0x100000;
    }
    if (option & 0x800000) {
        mask |= 0x400000;
    }
    wcmw->searchOption = (old & ~mask) | option;
    OS_RestoreInterrupts(irq);
    return old;
}

// pokebw2's WCM_GetAllowedChannel
s32 func_ov009_0216576c(void) {
    WCMWork *work = func_ov009_021657c8();

    if (work->state >= 3 && work->state <= 10) {
        switch (work->wirelessMode) {
        case 1:
            return func_ov009_02166de4();
        case 2:
            if (OS_IsRunOnTwl()) {
                return func_02731f00_unk();
            }
            break;
        }
    }
    return 0;
}

WCMWork *func_ov009_021657c8(void) {
    return wcmw;
}

// pokebw2's WcmConfigure
static void func_ov009_021657d8(WCMConfig *config, WCMNotify notify) {
    if (config == NULL) {
        wcmw->dmaNo = 3;
        wcmw->apBuffer = NULL;
        wcmw->apBufferBytes = 0;
        wcmw->scanMode = 0;
    } else {
        wcmw->dmaNo = config->dmano & 3;
        if (((4 - ((u32)config->pbdbuffer & 3)) % 4) + sizeof(WCMApListHeader) > config->nbdbuffer) {
            wcmw->apBuffer = NULL;
            wcmw->apBufferBytes = 0;
        } else {
            wcmw->apBuffer = (void *)(((u32)config->pbdbuffer + 3) & ~3);
            wcmw->apBufferBytes = config->nbdbuffer - (4 - ((u32)config->pbdbuffer & 3)) % 4;
            MI_CpuFill8(wcmw->apBuffer, 0, wcmw->apBufferBytes);
        }
        wcmw->scanMode = config->nbdmode;
    }
    wcmw->notify = notify;
}

// pokebw2's WcmInitOption
static void func_ov009_021658f8(void) {
    wcmw->searchOption = 0xaaa082;
}

// pokebw2's WCMi_GetNextScanChannel
u16 func_ov009_02165918(s32 channelIndex) {
    s32 i;
    s32 channel;

    for (i = 0; i < 13; i++) {
        channel = (channelIndex + i) % 13;
        if (wcmw->searchOption & (1 << (channel + 1))) {
            if (func_ov009_0216576c() & (1 << channel)) {
                break;
            }
        }
    }
    return (u16)((channelIndex + i) % 13 + 1);
}

// pokebw2's WCMi_Notify
void func_ov009_021659b4(s16 result, void *parameter, u32 detail, u32 line) {
    WCMWork *work = wcmw;
    s16 notify = work->pendingNotify;

    work->pendingNotify = 0;
    // The SDK transports mixed pointer/integer payloads as 32-bit words; a host port must widen this representation.
    func_ov009_021659f8(notify, result, (s32)parameter, (void *)detail, line);
}

// pokebw2's WCMi_NotifyEx
void func_ov009_021659f8(s16 notify, s16 result, s32 parameter, void *param1, s32 param2) {
    WCMWork *work = wcmw;
    WCMNotice notice;

    if (work->notify == NULL) {
        return;
    }
    notice.notify = notify;
    notice.result = result;
    notice.parameter[0].n = parameter;
    notice.parameter[1].p = param1;
    notice.parameter[2].n = param2;
    work->notify(&notice);
}

// pokebw2's WCMi_SetPhase
void func_ov009_02165a4c(s32 newState) {
    u32 irq = OS_DisableInterrupts();

    if (wcmw->state == 9 && newState != 9) {
        OS_CancelAlarm(&wcmw->alarm);
    }
    if (wcmw->state != 11) {
        wcmw->state = newState;
    }
    if (newState == 9) {
        OS_CancelAlarm(&wcmw->alarm);
        OS_SetAlarm(&wcmw->alarm, 0x22f5341, (void (*)(void *))func_ov009_02165b5c, NULL);
    }
    OS_RestoreInterrupts(irq);
}

// pokebw2's WCMi_ResetKeepAliveAlarm
void func_ov009_02165af4(void) {
    u32 irq = OS_DisableInterrupts();

    OS_CancelAlarm(&wcmw->alarm);
    if (wcmw->state == 9) {
        OS_SetAlarm(&wcmw->alarm, 0x22f5341, (void (*)(void *))func_ov009_02165b5c, NULL);
    }
    OS_RestoreInterrupts(irq);
}

// pokebw2's WcmKeepAliveAlarm
static void func_ov009_02165b5c(void) {
    func_ov009_02165af4();
}

// pokebw2's WCMi_GetDefaultDeviceId
static s32 func_ov009_02165b68(void) {
    return OS_IsRunOnTwl() ? 2 : 1;
}
