// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/wih_dwc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "system/wih_dwc.h"
#include "app/comm_tvt_field.h"
#include "dwc/bm.h"
#include "gfl/net.h"
#include "gfl/net_whpipe.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "gfl/wih.h"
#include "nitro/os.h"
#include "nitro/wm.h"
#include "system/ctvt_beacon.h"

// What the wireless scan sees around the player, for the icons of the C-Gear and the other screens that show the
// state of the wireless: the access points the console is set up for, the hotspots, the other access points with and
// without a key, the games that can be joined and the signal of the DS Download Play. The name is the ROM's string.

#define _ALLBEACON_MAX 4
// Frames that an access point stays shown after its last beacon
#define AP_KNOWN_FRAMES 600
#define AP_OTHER_FRAMES 400
#define LINK_LEVEL_FRAMES 400
#define LINK_LEVEL_MAX 4
#define WIFI_SLOT_COUNT 3
#define WIFI_SLOT_SSID_COUNT 2
#define WIFI_CONFIG_SIZE 0xa00

enum {
    AP_TIMER_KNOWN,
    AP_TIMER_HOTSPOT,
    AP_TIMER_OPEN,
    AP_TIMER_LOCKED,
    AP_TIMER_COUNT
};

// One of the console's Wi-Fi connection settings
typedef struct {
    u8 unk0[0x40];
    char ssid[WIFI_SLOT_SSID_COUNT][0x20];
    u8 unk80[0x67];
    u8 type;
    u8 unkE8[0x18];
} WifiSlot;

// One of the DSi's own settings, of two parts
typedef struct {
    WifiSlot base;
    WifiSlot ex;
} WifiSlotEx;

typedef struct {
    WifiSlotEx slotsEx[WIFI_SLOT_COUNT];
    WifiSlot slots[WIFI_SLOT_COUNT];
    u8 unk900[0x100];
} WifiConfig;

typedef struct {
    WMBssDesc bss[_ALLBEACON_MAX];
    void *parseWork;
    WifiConfig *config;
    u32 unk308;
    u16 paused;
    // Frames until the link level may change again
    u16 linkLevelWait;
    u16 num;
    u16 linkLevel;
    u16 bestLinkLevel;
    u16 count;
    int timers[AP_TIMER_COUNT];
} WihDwcWork;

struct DWCApInfo {
    u32 apType;
    u8 unk4[0x3c];
};

void func_0201045c(const void *beacon, void *work, u16 linkLevel);
BOOL func_020104f8(WMBssDesc *bss);
BOOL func_0201054c(WMBssDesc *bss);
BOOL func_0201057c(WMBssDesc *bss);

static WifiConfig *_localcfg;
// pokebw2's _localWork
static WihDwcWork *data_021461e0;




u32 func_0205d010(void);
u32 func_0205d010(void);

void *func_02010360(int num, HeapID heapId) {
    WihDwcWork *work;

    GFL_ASSERT(num <= _ALLBEACON_MAX);
    GFL_ASSERT(data_021461e0 == NULL);
    work = allocConfigDSSoftwareFeature(heapId, sizeof(WihDwcWork), "wih_dwc.c", 73);
    work->num = num;
    work->parseWork = allocConfigDSSoftwareFeature(heapId, func_0205d010(), "wih_dwc.c", 76);
    func_ov014_0217963c(func_0201045c);
    data_021461e0 = work;
    work->config = _localcfg;
    GFL_OvlLoad(OVERLAY_ID(15));
    return work;
}

void func_020103e4(void *work) {
    WihDwcWork *wk = work;

    GFL_ASSERT(data_021461e0);
    func_ov014_0217963c(NULL);
    func_0203d7fc(wk->parseWork);
    func_0203d7fc(wk);
    data_021461e0 = NULL;
    GFL_OvlUnload(OVERLAY_ID(15));
}

void func_0201042c(void) {
    if (data_021461e0 != NULL) {
        data_021461e0->paused = TRUE;
    }
}

void func_02010444(void) {
    if (data_021461e0 != NULL) {
        data_021461e0->paused = FALSE;
    }
}

// pokebw2's scanInfoCallback
void func_0201045c(const void *beacon, void *work, u16 linkLevel) {
    if (data_021461e0->num != 0 && data_021461e0->count < data_021461e0->num && data_021461e0->count < _ALLBEACON_MAX) {
        MI_CpuCopy8(beacon, &data_021461e0->bss[data_021461e0->count], sizeof(WMBssDesc));
        data_021461e0->count++;
    }
}

void func_02010498(void) {
    if (data_021461e0 != NULL && data_021461e0->paused) {
        return;
    }
    if (data_021461e0 != NULL) {
        if (data_021461e0->timers[AP_TIMER_KNOWN] > 0) {
            data_021461e0->timers[AP_TIMER_KNOWN]--;
        }
        if (data_021461e0->timers[AP_TIMER_HOTSPOT] > 0) {
            data_021461e0->timers[AP_TIMER_HOTSPOT]--;
        }
        if (data_021461e0->timers[AP_TIMER_OPEN] > 0) {
            data_021461e0->timers[AP_TIMER_OPEN]--;
        }
        if (data_021461e0->timers[AP_TIMER_LOCKED] > 0) {
            data_021461e0->timers[AP_TIMER_LOCKED]--;
        }
    }
}

// Whether the beacon is of an access point that the console has settings for
// pokebw2's isConfiguredAP
BOOL func_020104f8(WMBssDesc *bss) {
    int i;
    int length;

    for (i = 0; i < WIFI_SLOT_COUNT; i++) {
        length = GFL_STD_StrLen(data_021461e0->config->slots[i].ssid[0]);
        if (length > 0 && GFL_STD_MemCmp(data_021461e0->config->slots[i].ssid[0], bss->ssid, length) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

// Whether the beacon is of a FREESPOT hotspot
// BUG: the comparison is the wrong way round, so that it is every open access point with a name of eight letters
// but FREESPOT
// pokebw2's isFreeSpot
BOOL func_0201054c(WMBssDesc *bss) {
#ifdef BUGFIX
    if (bss->ssidLength == 8 && GFL_STD_MemCmp("FREESPOT", bss->ssid, 8) == 0 && !(bss->capaInfo & 0x10)) {
#else
    if (bss->ssidLength == 8 && GFL_STD_MemCmp("FREESPOT", bss->ssid, 8) != 0 && !(bss->capaInfo & 0x10)) {
#endif
        return TRUE;
    }
    return FALSE;
}

// Whether the access point asks for a key
// pokebw2's isLocked
BOOL func_0201057c(WMBssDesc *bss) {
    if (bss->capaInfo & 0x10) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_0205d01c(void *work, const void *bssDesc, DWCApInfo *apInfo);

u32 func_0201058c(WifiList *wifiList) {
    u32 status = 0;
    GFLNetInitData *init = func_0203d798();
    int i;
    DWCApInfo apInfo;

    if (data_021461e0 == NULL) {
        return status;
    }
    if (func_020107fc(wifiList) != -1) {
        status |= 0x8;
    }
    if (init->gameCommandBase == 2 && func_0203d1b4() > 1) {
        status |= 0x4;
    }
    for (i = 0; i < init->unk62; i++) {
        if (func_0203cf10(i) != NULL) {
            u8 service = func_ov014_02178098(i);

            if (service >= 0x14 && service <= 0x1b) {
                status |= 0x10;
            } else if (service == 0x9) {
                status |= 0x20;
            }
        }
    }
    for (i = 0; i < data_021461e0->count; i++) {
        WMBssDesc *bss = &data_021461e0->bss[i];

        if (func_020104f8(bss)) {
            data_021461e0->timers[AP_TIMER_KNOWN] = AP_KNOWN_FRAMES;
        } else if (func_0201054c(bss)) {
            data_021461e0->timers[AP_TIMER_HOTSPOT] = AP_KNOWN_FRAMES;
        } else if (func_0205d01c(data_021461e0->parseWork, bss, &apInfo)) {
            switch (apInfo.apType) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 10:
            case 11:
            case 12:
                data_021461e0->timers[AP_TIMER_KNOWN] = AP_KNOWN_FRAMES;
                break;
            case 4:
            case 5:
            case 6:
            case 8:
                data_021461e0->timers[AP_TIMER_HOTSPOT] = AP_KNOWN_FRAMES;
                break;
            }
        } else if (bss->ssidLength != 0) {
            if (!func_0201057c(bss)) {
                data_021461e0->timers[AP_TIMER_OPEN] = AP_OTHER_FRAMES;
            } else {
                data_021461e0->timers[AP_TIMER_LOCKED] = AP_OTHER_FRAMES;
            }
        }
    }
    if (data_021461e0->timers[AP_TIMER_KNOWN] > 0) {
        status |= 0x40;
    }
    if (data_021461e0->timers[AP_TIMER_HOTSPOT] > 0) {
        status |= 0x80;
    }
    if (data_021461e0->timers[AP_TIMER_OPEN] > 0) {
        status |= 0x100;
    }
    if (data_021461e0->timers[AP_TIMER_LOCKED] > 0) {
        status |= 0x200;
    }
    if (data_021461e0->linkLevelWait != 0) {
        data_021461e0->linkLevelWait--;
    }
    if (status == 0) {
        data_021461e0->linkLevel = 0;
    } else if (data_021461e0->bestLinkLevel != 0 && data_021461e0->linkLevelWait == 0) {
        data_021461e0->linkLevel = data_021461e0->bestLinkLevel;
        data_021461e0->linkLevelWait = LINK_LEVEL_FRAMES;
        data_021461e0->bestLinkLevel = 0;
    }
    data_021461e0->count = 0;
    return status | 0x2;
}


void setupDownloadPlayConfig(HeapID heapId) {
    GFL_ASSERT(_localcfg == NULL);
    _localcfg = allocConfigDSSoftwareFeature(heapId, WIFI_CONFIG_SIZE, "wih_dwc.c", 448);
}

void downloadPlayConfigForWifi(void) {
    GFL_ASSERT(_localcfg != NULL);
    func_0205faa8((DWCMemPage *)_localcfg->slots, 0);
}


int func_020107fc(WifiList *wifiList) {
    int i;
    u8 mac[6];

    if (func_0203cd98()) {
        func_0203d798();
        OS_GetMacAddress(mac);
        for (i = 0; i < 16; i++) {
            if (func_ov014_02177b80(i) == 0x20 && func_0202e8d8(func_0203cf10(i), wifiList, mac)) {
                return i;
            }
        }
    }
    return -1;
}

u16 func_02010848(void) {
    if (data_021461e0 != NULL) {
        return data_021461e0->linkLevel;
    }
    return 0;
}

void func_02010864(u8 linkLevel) {
    if (data_021461e0 != NULL && data_021461e0->bestLinkLevel < linkLevel && linkLevel < LINK_LEVEL_MAX) {
        data_021461e0->bestLinkLevel = linkLevel;
    }
}

void func_02010884(void) {
}
