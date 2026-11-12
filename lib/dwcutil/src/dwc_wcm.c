// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwcutil/src/dwc_wcm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "dwc_wcm.h"
#include "dwc/wcm.h"
#include "gfl/std.h"
#include "nitro/os.h"

// The wrapper that DWC's settings put around the wireless connection manager (WCM, see wcm.h): one operation runs at a
// time, a state says which, and a callback is told how each ends. The ROM names none of it; the names are ours.

// Where the wrapper is
enum {
    STATE_OFF,         // not initialized
    STATE_INITIALIZED, // the work is set up, the WCM is not started
    STATE_CLEANING_UP,
    STATE_IDLE,     // started, doing nothing
    STATE_STOPPING, // going to idle: the start, the end of a search or a disconnect is under way
    STATE_SEARCHING,
    STATE_SEARCH_STARTING,
    STATE_CONNECTED,
    STATE_CONNECTING,
};

typedef struct {
    void *wcmWork;           // 0x00
    u32 searchOption;        // 0x04
    WCMConfig *config;       // 0x08
    u32 wepMode;             // 0x0c
    const u8 *bssid;         // 0x10
    const u8 *essid;         // 0x14
    int state;               // 0x18
    DWCWcmCallback callback; // 0x1c
    void *bssDesc;           // 0x20
    void *wepDesc;           // 0x24
} WcmWrapper;

// MWCC 2.0 lays the statics out in reverse
// The BSSID and ESSID buffers are word-aligned in the ROM, between the two wrappers
typedef union {
    u8 bytes[6];
    u16 align;
} Bssid;

typedef union {
    u8 bytes[0x20];
    u32 align;
} Essid;

static Essid data_ov016_021dcfb0;
static Bssid data_ov016_021dcfa8;
static WcmWrapper data_ov016_021dcf80;

static void APCi_Notify_apc(WCMNotice *notice);
static BOOL APCi_MoveIdleState_apc(void);

// pokebw2's WcmNotify
static void APCi_Notify_apc(WCMNotice *notice) {
    if (notice == NULL) {
        return;
    }
    switch (notice->notify) {
    case WCM_NOTIFY_STARTUP:
        if (notice->result == 0) {
            if (data_ov016_021dcf80.state == STATE_STOPPING) {
                data_ov016_021dcf80.state = STATE_IDLE;
                if (data_ov016_021dcf80.callback != NULL) {
                    data_ov016_021dcf80.callback(DWC_WCM_EVENT_STARTED, 0);
                }
            } else if (data_ov016_021dcf80.state == STATE_SEARCH_STARTING) {
                if (func_ov009_02165144(data_ov016_021dcf80.bssid, data_ov016_021dcf80.essid, data_ov016_021dcf80.searchOption) != WCM_RESULT_ACCEPT) {
                    data_ov016_021dcf80.state = STATE_IDLE;
                    if (data_ov016_021dcf80.callback != NULL) {
                        data_ov016_021dcf80.callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (data_ov016_021dcf80.state == STATE_CONNECTING) {
                if (func_ov009_021652a4(data_ov016_021dcf80.bssDesc, data_ov016_021dcf80.wepDesc, data_ov016_021dcf80.wepMode) != WCM_RESULT_ACCEPT) {
                    data_ov016_021dcf80.state = STATE_IDLE;
                    if (data_ov016_021dcf80.callback != NULL) {
                        data_ov016_021dcf80.callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            }
        } else {
            data_ov016_021dcf80.state = STATE_INITIALIZED;
            if (data_ov016_021dcf80.callback != NULL) {
                data_ov016_021dcf80.callback(DWC_WCM_EVENT_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_BEGIN_SEARCH:
        if (notice->result == 0) {
            if (data_ov016_021dcf80.state == STATE_SEARCH_STARTING) {
                data_ov016_021dcf80.state = STATE_SEARCHING;
                if (data_ov016_021dcf80.callback != NULL) {
                    data_ov016_021dcf80.callback(DWC_WCM_EVENT_SEARCHING, 0);
                }
            }
        } else {
            data_ov016_021dcf80.state = STATE_IDLE;
            if (data_ov016_021dcf80.callback != NULL) {
                data_ov016_021dcf80.callback(DWC_WCM_EVENT_SEARCH_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_CONNECT:
        if (notice->result == 0) {
            if (data_ov016_021dcf80.state == STATE_CONNECTING) {
                data_ov016_021dcf80.state = STATE_CONNECTED;
                if (data_ov016_021dcf80.callback != NULL) {
                    data_ov016_021dcf80.callback(DWC_WCM_EVENT_CONNECTED, 0);
                }
            }
        } else {
            data_ov016_021dcf80.state = STATE_IDLE;
            if (data_ov016_021dcf80.callback != NULL) {
                data_ov016_021dcf80.callback(DWC_WCM_EVENT_CONNECT_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_END_SEARCH:
        if (notice->result == 0) {
            if (data_ov016_021dcf80.state == STATE_STOPPING) {
                data_ov016_021dcf80.state = STATE_IDLE;
                if (data_ov016_021dcf80.callback != NULL) {
                    data_ov016_021dcf80.callback(DWC_WCM_EVENT_SEARCH_ENDED, 0);
                }
            } else if (data_ov016_021dcf80.state == STATE_SEARCH_STARTING) {
                if (func_ov009_02165144(data_ov016_021dcf80.bssid, data_ov016_021dcf80.essid, data_ov016_021dcf80.searchOption) != WCM_RESULT_ACCEPT) {
                    data_ov016_021dcf80.state = STATE_IDLE;
                    if (data_ov016_021dcf80.callback != NULL) {
                        data_ov016_021dcf80.callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (data_ov016_021dcf80.state == STATE_CLEANING_UP) {
                if (func_ov009_02165088() != WCM_RESULT_ACCEPT) {
                    data_ov016_021dcf80.state = STATE_IDLE;
                    if (data_ov016_021dcf80.callback != NULL) {
                        data_ov016_021dcf80.callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (data_ov016_021dcf80.state == STATE_CONNECTING) {
                if (func_ov009_021652a4(data_ov016_021dcf80.bssDesc, data_ov016_021dcf80.wepDesc, data_ov016_021dcf80.wepMode) != WCM_RESULT_ACCEPT) {
                    data_ov016_021dcf80.state = STATE_IDLE;
                    if (data_ov016_021dcf80.callback != NULL) {
                        data_ov016_021dcf80.callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            }
        } else {
            data_ov016_021dcf80.state = STATE_IDLE;
            if (data_ov016_021dcf80.callback != NULL) {
                data_ov016_021dcf80.callback(DWC_WCM_EVENT_END_SEARCH_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_DISCONNECT:
        if (notice->result == 0) {
            if (data_ov016_021dcf80.state == STATE_STOPPING) {
                data_ov016_021dcf80.state = STATE_IDLE;
                if (data_ov016_021dcf80.callback != NULL) {
                    data_ov016_021dcf80.callback(DWC_WCM_EVENT_DISCONNECTED, 0);
                }
            } else if (data_ov016_021dcf80.state == STATE_SEARCH_STARTING) {
                if (func_ov009_02165144(data_ov016_021dcf80.bssid, data_ov016_021dcf80.essid, data_ov016_021dcf80.searchOption) != WCM_RESULT_ACCEPT) {
                    data_ov016_021dcf80.state = STATE_IDLE;
                    if (data_ov016_021dcf80.callback != NULL) {
                        data_ov016_021dcf80.callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (data_ov016_021dcf80.state == STATE_CLEANING_UP) {
                if (func_ov009_02165088() != WCM_RESULT_ACCEPT) {
                    data_ov016_021dcf80.state = STATE_IDLE;
                    if (data_ov016_021dcf80.callback != NULL) {
                        data_ov016_021dcf80.callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (data_ov016_021dcf80.state == STATE_CONNECTING) {
                if (func_ov009_021652a4(data_ov016_021dcf80.bssDesc, data_ov016_021dcf80.wepDesc, data_ov016_021dcf80.wepMode) != WCM_RESULT_ACCEPT) {
                    data_ov016_021dcf80.state = STATE_IDLE;
                    if (data_ov016_021dcf80.callback != NULL) {
                        data_ov016_021dcf80.callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else {
                data_ov016_021dcf80.state = STATE_IDLE;
            }
        } else {
            data_ov016_021dcf80.state = STATE_IDLE;
            if (data_ov016_021dcf80.callback != NULL) {
                data_ov016_021dcf80.callback(DWC_WCM_EVENT_DISCONNECT_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_CLEANUP:
        if (notice->result == 0) {
            if (data_ov016_021dcf80.state == STATE_CLEANING_UP) {
                func_ov009_02164ee0();
                data_ov016_021dcf80.state = STATE_OFF;
                if (data_ov016_021dcf80.callback != NULL) {
                    data_ov016_021dcf80.callback(DWC_WCM_EVENT_CLEANED_UP, 0);
                }
            }
        } else {
            data_ov016_021dcf80.state = STATE_IDLE;
            if (data_ov016_021dcf80.callback != NULL) {
                data_ov016_021dcf80.callback(DWC_WCM_EVENT_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_FOUND_AP:
        if (data_ov016_021dcf80.state == STATE_SEARCHING) {
            if (data_ov016_021dcf80.callback != NULL) {
                data_ov016_021dcf80.callback(DWC_WCM_EVENT_FOUND_AP, 0);
            }
        }
        break;
    default:
        if (data_ov016_021dcf80.callback != NULL) {
            data_ov016_021dcf80.callback(DWC_WCM_EVENT_UNKNOWN, 0);
        }
        break;
    }
}

// Stops what the state says is under way, and reports whether the WCM took it
// pokebw2's WcmAbort
static BOOL APCi_MoveIdleState_apc(void) {
    switch (data_ov016_021dcf80.state) {
    case STATE_SEARCHING:
        if (func_ov009_02165144(NULL, NULL, 0) != WCM_RESULT_ACCEPT) {
            return FALSE;
        }
        break;
    case STATE_CONNECTED:
        if (func_ov009_02165478() != WCM_RESULT_ACCEPT) {
            return FALSE;
        }
        break;
    case STATE_INITIALIZED:
        if (func_ov009_02164f98(data_ov016_021dcf80.config, APCi_Notify_apc) != WCM_RESULT_ACCEPT) {
            return FALSE;
        }
        break;
    case STATE_OFF:
    case STATE_CLEANING_UP:
    case STATE_IDLE:
    case STATE_STOPPING:
    case STATE_SEARCH_STARTING:
    case STATE_CONNECTING:
    default:
        return FALSE;
    }
    return TRUE;
}

// pokebw2's DWC_Wcm_GetApList
int APC_GetAPList(void *dest, int max) {
    u8 *out = dest;
    int count;
    int i;

    func_ov009_02165c28(TRUE);
    count = func_ov009_02165bd0();
    if (count > 0) {
        for (i = 0; i < count; i++) {
            if (i >= max) {
                break;
            }
            MIi_CpuCopy32(func_ov009_02165ca0(i), out, 0xc0);
            out += 0xc0;
        }
    }
    func_ov009_02165c28(FALSE);
    return count;
}

// pokebw2's DWC_Wcm_Search
BOOL APC_SearchStart(const u8 *bssid, const u8 *essid, int essidLength, u32 option) {
    u32 irq = OS_DisableInterrupts();
    int i;

    data_ov016_021dcf80.searchOption = option;
    if (bssid != NULL) {
        for (i = 0; i < 6; i++) {
            data_ov016_021dcfa8.bytes[i] = *bssid++;
        }
        data_ov016_021dcf80.bssid = data_ov016_021dcfa8.bytes;
    } else {
        MI_CpuFill8(data_ov016_021dcfa8.bytes, 0xff, 6);
        data_ov016_021dcf80.bssid = WCM_Bssid_Any;
    }
    if (essid != NULL && essidLength > 0 && essidLength <= 0x20) {
        for (i = 0; i < essidLength; i++) {
            data_ov016_021dcfb0.bytes[i] = *essid++;
        }
        for (; i < 0x20; i++) {
            data_ov016_021dcfb0.bytes[i] = 0;
        }
        data_ov016_021dcf80.essid = data_ov016_021dcfb0.bytes;
    } else {
        MI_CpuFill8(data_ov016_021dcfb0.bytes, 0xff, 0x20);
        data_ov016_021dcf80.essid = WCM_Essid_Any;
    }
    if (data_ov016_021dcf80.state == STATE_IDLE) {
        if (func_ov009_02165144(data_ov016_021dcf80.bssid, data_ov016_021dcf80.essid, data_ov016_021dcf80.searchOption) == WCM_RESULT_ACCEPT) {
            data_ov016_021dcf80.state = STATE_SEARCH_STARTING;
            OS_RestoreInterrupts(irq);
            return TRUE;
        }
    } else if (APCi_MoveIdleState_apc() == TRUE) {
        data_ov016_021dcf80.state = STATE_SEARCH_STARTING;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

// pokebw2's DWC_Wcm_EndSearch
BOOL APC_SearchEnd(void) {
    u32 irq = OS_DisableInterrupts();

    if (data_ov016_021dcf80.state == STATE_SEARCHING && func_ov009_02165144(NULL, NULL, 0) == WCM_RESULT_ACCEPT) {
        data_ov016_021dcf80.state = STATE_STOPPING;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

// pokebw2's DWC_Wcm_Disconnect
BOOL APC_Disconnect(void) {
    u32 irq = OS_DisableInterrupts();

    if (data_ov016_021dcf80.state == STATE_CONNECTED && func_ov009_02165478() == WCM_RESULT_ACCEPT) {
        data_ov016_021dcf80.state = STATE_STOPPING;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

// pokebw2's DWC_Wcm_Cleanup
BOOL APC_End(void) {
    u32 irq = OS_DisableInterrupts();

    if (data_ov016_021dcf80.state == STATE_IDLE) {
        if (func_ov009_02165088() != WCM_RESULT_ACCEPT) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        data_ov016_021dcf80.state = STATE_CLEANING_UP;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    if (APCi_MoveIdleState_apc() == TRUE) {
        data_ov016_021dcf80.state = STATE_CLEANING_UP;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

// pokebw2's DWC_Wcm_Connect
BOOL APC_Connect(const void *bssDesc, const void *wepDesc, u32 wepMode) {
    u32 irq = OS_DisableInterrupts();

    data_ov016_021dcf80.wepMode = wepMode;
    if (wepDesc != NULL) {
        MI_CpuCopy8(wepDesc, data_ov016_021dcf80.wepDesc, 0x50);
    } else {
        MI_CpuFill8(data_ov016_021dcf80.wepDesc, 0, 0x50);
    }
    MIi_CpuCopy32(bssDesc, data_ov016_021dcf80.bssDesc, 0xc0);
    if (APCi_MoveIdleState_apc() == TRUE) {
        data_ov016_021dcf80.state = STATE_CONNECTING;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    if (data_ov016_021dcf80.state == STATE_IDLE &&
        func_ov009_021652a4(data_ov016_021dcf80.bssDesc, data_ov016_021dcf80.wepDesc, data_ov016_021dcf80.wepMode) == WCM_RESULT_ACCEPT) {
        data_ov016_021dcf80.state = STATE_CONNECTING;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

// pokebw2's DWC_Wcm_Start
BOOL APC_Init(DWCWcmCallback callback, void *work, u32 size) {
    u32 irq = OS_DisableInterrupts();
    u8 *buf = work;

    data_ov016_021dcf80.wepDesc = buf;
    data_ov016_021dcf80.config = (WCMConfig *)(((u32)buf + 0x53) & ~3);
    data_ov016_021dcf80.wcmWork = (void *)(((u32)data_ov016_021dcf80.config + sizeof(WCMConfig) + 0x1f) & ~0x1f);
    data_ov016_021dcf80.bssDesc = (void *)(((u32)data_ov016_021dcf80.wcmWork + WCM_WORK_SIZE + 0x1f) & ~0x1f);
    data_ov016_021dcf80.config->pbdbuffer = (void *)(((u32)data_ov016_021dcf80.bssDesc + 0xc0 + 0x1f) & ~0x1f);
    data_ov016_021dcf80.config->nbdbuffer = (s32)(buf + size) - (s32)data_ov016_021dcf80.config->pbdbuffer;
    data_ov016_021dcf80.config->nbdmode = 0;
    data_ov016_021dcf80.config->dmano = 3;
    data_ov016_021dcf80.callback = callback;
    if (data_ov016_021dcf80.state == STATE_OFF) {
        if (func_ov009_02164d64(data_ov016_021dcf80.wcmWork, WCM_WORK_SIZE) != 0) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        data_ov016_021dcf80.state = STATE_INITIALIZED;
    }
    if (data_ov016_021dcf80.state == STATE_INITIALIZED) {
        if (func_ov009_02164f98(data_ov016_021dcf80.config, APCi_Notify_apc) != WCM_RESULT_ACCEPT) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        data_ov016_021dcf80.state = STATE_STOPPING;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}
