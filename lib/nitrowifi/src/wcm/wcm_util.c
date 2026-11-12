// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/wcm/wcm_util.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitro/os.h"
#include "nitro/wm.h"
#include "nitrowifi/wcm.h"
#include "wcm_internal.h"
#include "dwc/wcm.h"

// NitroWiFi's WCM (overlay 11): the comparison of BSSIDs and the link level, from a running average of the signal.
// TwlSDK's libwcm names the object util.o; Black 1's decompilation (aletheia) has the same functions, in this order, in
// its wcm_util.c, and the names are the SDK's.

// The last 16 signal samples, a ring, and the count of samples until it is full, then 16 plus the next index
// pokebw2's sWcmRssiSamples
static u8 data_ov009_021aa815[16];
// pokebw2's sWcmRssiCursor
static u8 data_ov009_021aa814 = 0;

static BOOL func_ov009_02166680(const void *a, const void *b, u32 size);
static WMLinkLevel func_ov009_02166710(u32 rssi);
static u8 func_ov009_02166784(void);

// pokebw2's WcmCompareMemory
static BOOL func_ov009_02166680(const void *a, const void *b, u32 size) {
    u32 i;

    for (i = 0; i < size; i++) {
        if (((const u8 *)a)[i] != ((const u8 *)b)[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's WCM_CompareBssID
BOOL func_ov009_021666b8(const u8 *a, const u8 *b) {
    return func_ov009_02166680(a, b, 6);
}

// pokebw2's WCM_GetLinkLevel
WMLinkLevel func_ov009_021666c8(void) {
    u32 irq = OS_DisableInterrupts();
    WCMWork *work = func_ov009_021657c8();
    WMLinkLevel quality = WM_LINK_LEVEL_0;

    if (work != NULL && work->state == 9) {
        quality = func_ov009_02166710(func_ov009_02166784());
    }
    OS_RestoreInterrupts(irq);
    return quality;
}

// pokebw2's WCM_CalcLinkLevel
static WMLinkLevel func_ov009_02166710(u32 rssi) {
    switch (func_ov009_021657c8()->wirelessMode) {
    case 1:
    default:
        if (rssi >= 0x1c) {
            return WM_LINK_LEVEL_3;
        }
        if (rssi >= 0x16) {
            return WM_LINK_LEVEL_2;
        }
        if (rssi >= 0x10) {
            return WM_LINK_LEVEL_1;
        }
        return WM_LINK_LEVEL_0;
    case 2:
        if (rssi >= 0x16) {
            return WM_LINK_LEVEL_3;
        }
        if (rssi >= 0x11) {
            return WM_LINK_LEVEL_2;
        }
        if (rssi >= 0xc) {
            return WM_LINK_LEVEL_1;
        }
        return WM_LINK_LEVEL_0;
    }
}

// pokebw2's WCMi_GetRssiAverage
static u8 func_ov009_02166784(void) {
    s32 sum = 0;
    s32 i;

    if (data_ov009_021aa814 > 16) {
        for (i = 0; i < 16; i++) {
            sum += data_ov009_021aa815[i];
        }
        sum /= 16;
    } else if (data_ov009_021aa814 > 0) {
        for (i = 0; i < data_ov009_021aa814; i++) {
            sum += data_ov009_021aa815[i];
        }
        sum /= data_ov009_021aa814;
    }
    return (u8)sum;
}

// Adds a signal-level sample to the running RSSI average's circular buffer
// Decodes the receive record's high signal byte. The original arithmetic is mode-dependent;
// no physical units for the encoded input or the resulting sample have been established.
// pokebw2's WCMi_ShelterRssi
void func_ov009_02166804(s32 rssi) {
    switch (func_ov009_021657c8()->wirelessMode) {
    case 2:
        break;
    case 1:
    default:
        if (rssi & 2) {
            rssi = (u8)(rssi >> 2);
        } else {
            rssi = (u8)(25 + (rssi >> 2));
        }
        break;
    }
    data_ov009_021aa815[data_ov009_021aa814 % 16] = rssi;
    if (data_ov009_021aa814 >= 16) {
        data_ov009_021aa814 = (u8)(((1 + data_ov009_021aa814) % 16) + 16);
    } else {
        data_ov009_021aa814++;
    }
}
