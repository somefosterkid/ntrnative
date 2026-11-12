// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/wcm/wcm_aplist.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "gfl/std.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitrowifi/wcm.h"
#include "twl/nwm.h"
#include "wcm_internal.h"
#include <stddef.h>
#include "dwc/wcm.h"
#include "nitro/wm.h"

// NitroWiFi's WCM (overlay 11): the list of the access points that searches found, in the buffer that
// func_ov009_02164f98's caller gives. TwlSDK's libwcm names the object aplist.o; Black 1's decompilation (aletheia) has
// the same functions, in this order, in its wcm_aplist.c, and the names are the SDK's. The ARM word pair after
// func_ov009_021661f0 is the linker's long-branch veneer to func_02703250 in the LTD autoload.

// A vendor OUI and the type byte after it, which func_ov009_021661f0 copies as a whole
typedef struct WCMVendorOui {
    u8 bytes[4];
} WCMVendorOui;

// Nintendo's vendor OUI (00:09:BF) and type 0, which func_ov009_021661f0 looks for among a beacon's vendor-specific
// information elements
// pokebw2's sNintendoOui
static const WCMVendorOui data_ov009_021a19b0 = { { 0x00, 0x09, 0xbf, 0x00 } };

static WMBssDesc *func_ov009_02165cac(u16 index);
static WCMApList *func_ov009_02165dcc(void);
static WCMApList *func_ov009_02165e98(void);
static WCMApList *func_ov009_02165ec8(const u8 *bssid);
static WCMApList *func_ov009_02165f34(u32 id);
static void func_ov009_02165f88(WCMApList *entry);
static void *func_ov009_021661f0(const WMBssDesc *bss);

// pokebw2's WCM_ClearApList
void func_ov009_02165b80(void) {
    u32 irq = OS_DisableInterrupts();
    WCMWork *work = func_ov009_021657c8();

    if (work == NULL) {
        OS_RestoreInterrupts(irq);
        return;
    }
    if (work->apBuffer != NULL && (s32)work->apBufferBytes > 0) {
        MI_CpuFill8(work->apBuffer, 0, work->apBufferBytes);
    }
    OS_RestoreInterrupts(irq);
}

// pokebw2's WCM_CountApList
s32 func_ov009_02165bd0(void) {
    u32 irq = OS_DisableInterrupts();
    WCMWork *work = func_ov009_021657c8();
    s32 count = 0;
    WCMApListHeader *header;

    if (work == NULL) {
        OS_RestoreInterrupts(irq);
        return 0;
    }
    header = (WCMApListHeader *)work->apBuffer;
    if (header != NULL && work->apBufferBytes > sizeof(WCMApListHeader)) {
        count = header->count;
    }
    OS_RestoreInterrupts(irq);
    return count;
}

// pokebw2's WCM_LockApList
s32 func_ov009_02165c28(s32 lock) {
    u32 irq = OS_DisableInterrupts();
    WCMWork *work = func_ov009_021657c8();

    if (work == NULL) {
        OS_RestoreInterrupts(irq);
        return 0;
    }
    if (lock != 0) {
        lock = work->apListLocked ? 1 : 0;
        work->apListLocked = 1;
    } else {
        lock = work->apListLocked ? 1 : 0;
        work->apListLocked = 0;
    }
    OS_RestoreInterrupts(irq);
    return lock;
}

// pokebw2's WCM_PointApList
WMBssDesc *func_ov009_02165ca0(u16 index) {
    return func_ov009_02165cac(index);
}

// pokebw2's WCM_PointApListEx
static WMBssDesc *func_ov009_02165cac(u16 index) {
    u32 irq = OS_DisableInterrupts();
    WCMWork *work = func_ov009_021657c8();
    WCMApList *entry;

    if (work == NULL) {
        OS_RestoreInterrupts(irq);
        return NULL;
    }
    entry = func_ov009_02165f34(index);
    if (entry == NULL) {
        OS_RestoreInterrupts(irq);
        return NULL;
    }
    OS_RestoreInterrupts(irq);
    return (WMBssDesc *)entry->data;
}

// pokebw2's WCMi_EntryApList
void func_ov009_02165d04(const WMBssDesc *bss, u16 scanLinkLevel) {
    WCMWork *work = func_ov009_021657c8();
    WCMApList *entry;

    if (work == NULL || work->apListLocked != 0) {
        return;
    }
    if (OS_IsRunOnTwl() && work->wirelessMode == 2 && func_ov009_021661f0(bss) != NULL) {
        return;
    }
    if (work->wirelessMode == 1 && bss->gameInfoLength != 0) {
        return;
    }
    entry = func_ov009_02165ec8(bss->bssid);
    if (entry == NULL) {
        entry = func_ov009_02165dcc();
    }
    if (entry == NULL && work->scanMode == 1) {
        entry = func_ov009_02165e98();
    }
    if (entry != NULL) {
        func_ov009_02166040(bss, (WMBssDesc *)entry->data);
        entry->scanLinkLevel = scanLinkLevel;
        func_ov009_02165f88(entry);
    }
}

// pokebw2's WcmAllocApList
static WCMApList *func_ov009_02165dcc(void) {
    WCMWork *work = func_ov009_021657c8();
    WCMApListHeader *header = (WCMApListHeader *)work->apBuffer;
    WCMApList *entry = NULL;

    if (header != NULL && work->apBufferBytes > sizeof(WCMApListHeader)) {
        u32 max = (work->apBufferBytes - sizeof(WCMApListHeader)) / sizeof(WCMApList);

        if (max > 0) {
            s32 i;

            if (max > header->count) {
                for (i = 0; i < max; i++) {
                    entry = (WCMApList *)((u32)header + sizeof(WCMApListHeader) + i * sizeof(WCMApList));
                    if (entry->inUse == FALSE) {
                        break;
                    }
                }
                if (i < max) {
                    entry->inUse = TRUE;
                    entry->id = header->count;
                    entry->next = NULL;
                    entry->prev = header->tail;
                    header->tail = entry;
                    if (entry->prev != NULL) {
                        entry->prev->next = entry;
                    } else {
                        header->head = entry;
                    }
                    header->count++;
                }
            }
        }
    }
    return entry;
}

// pokebw2's WcmGetOldestApList
static WCMApList *func_ov009_02165e98(void) {
    WCMWork *work = func_ov009_021657c8();
    WCMApListHeader *header = (WCMApListHeader *)work->apBuffer;

    if (header != NULL && work->apBufferBytes > sizeof(WCMApListHeader)) {
        return header->head;
    }
    return NULL;
}

// pokebw2's WcmSearchApList
static WCMApList *func_ov009_02165ec8(const u8 *bssid) {
    WCMWork *work = func_ov009_021657c8();
    WCMApListHeader *header = (WCMApListHeader *)work->apBuffer;
    WCMApList *entry = NULL;

    if (bssid == NULL) {
        return NULL;
    }
    if (header != NULL && work->apBufferBytes > sizeof(WCMApListHeader)) {
        for (entry = header->head; entry != NULL; entry = entry->next) {
            if (func_ov009_021666b8(((WMBssDesc *)entry->data)->bssid, bssid)) {
                break;
            }
        }
    }
    return entry;
}

// pokebw2's WcmSearchIndexedApList
static WCMApList *func_ov009_02165f34(u32 id) {
    WCMWork *work = func_ov009_021657c8();
    WCMApListHeader *header = (WCMApListHeader *)work->apBuffer;
    WCMApList *entry = NULL;

    if (header != NULL && work->apBufferBytes > sizeof(WCMApListHeader)) {
        for (entry = header->head; entry != NULL; entry = entry->next) {
            if (entry->id == id) {
                break;
            }
        }
    }
    return entry;
}

// pokebw2's WcmAppendApList
static void func_ov009_02165f88(WCMApList *entry) {
    WCMWork *work = func_ov009_021657c8();
    WCMApListHeader *header = (WCMApListHeader *)work->apBuffer;
    WCMApList *walk;

    if (entry == NULL) {
        return;
    }
    if (header == NULL || work->apBufferBytes <= sizeof(WCMApListHeader)) {
        return;
    }
    walk = header->head;
    if (walk != NULL) {
        do {
            if (walk == entry) {
                if (walk->prev != NULL) {
                    walk->prev->next = walk->next;
                } else {
                    header->head = walk->next;
                }
                if (walk->next != NULL) {
                    walk->next->prev = walk->prev;
                } else {
                    header->tail = walk->prev;
                }
                break;
            }
            walk = walk->next;
        } while (walk != NULL);
    }
    // Append (or re-append) at the tail
    entry->next = NULL;
    entry->prev = header->tail;
    header->tail = entry;
    if (entry->prev != NULL) {
        entry->prev->next = entry;
    } else {
        header->head = entry;
    }
    // A fresh entry, never in the list, gets a new id; one found and moved keeps its id
    if (walk == NULL) {
        entry->id = header->count;
        header->count++;
    }
}

// Copies the descriptor prefix and retains selected IEs in the destination's raw 0x40..0xbf tail.
// pokebw2's WCMi_BssDescCopy
void func_ov009_02166040(const WMBssDesc *source, WMBssDesc *destination) {
    WCMWork *work;
    const u8 *ie;
    u8 *output;
    u8 total;
    u16 n;
    u8 tag;
    u8 len;

    work = func_ov009_021657c8();
    MIi_CpuClearFast(0, destination, sizeof(WMBssDesc));
    MIi_CpuCopyFast(source, destination, offsetof(WMBssDesc, gameInfo));
    destination->otherElementCount = 0;
    if (work->wirelessMode == 1) {
        destination->length = 0x20;
    } else {
        destination->length = 0x40;
    }
    total = 0;
    {
        // Microsoft's vendor OUI (00:50:F2) with the WPA type 1, and with the WMM type 2 and subtype 0
        u8 wpaOui[4] = { 0x00, 0x50, 0xf2, 0x01 };
        u8 wmmOui[5] = { 0x00, 0x50, 0xf2, 0x02, 0x00 };

        ie = (const u8 *)&source->gameInfo;
        output = (u8 *)&destination->gameInfo;
        for (n = 0; n < source->otherElementCount; n++) {
            tag = ie[0];
            len = ie[1];
            if (tag == 0x32 || tag == 0x2a || tag == 0x2f || tag == 7 || tag == 0x30) {
                sys_memcpy_fast(ie, output, (u8)(len + 2));
                output += len + 2;
                total += (u8)(len + 2);
                destination->otherElementCount++;
            } else if (tag == 0xdd) {
                if (sys_memcmp(ie + 2, wpaOui, 4) == 0 || sys_memcmp(ie + 2, wmmOui, 5) == 0) {
                    sys_memcpy_fast(ie, output, (u8)(len + 2));
                    output += len + 2;
                    total += (u8)(len + 2);
                    destination->otherElementCount++;
                }
            }
            ie += len + 2;
        }
    }
    // The original adds the elements' length in a branch per mode, the DS one through a 16-bit mask
    if (work->wirelessMode == 1) {
        destination->length += (u16)(total & 0xffff);
    } else {
        destination->length += total;
    }
}

// pokebw2's WCMi_GetGameInfo
static void *func_ov009_021661f0(const WMBssDesc *bss) {
    WCMWork *work = func_ov009_021657c8();
    WCMVendorOui oui = data_ov009_021a19b0;

    if (bss == NULL) {
        return NULL;
    }
    if (OS_IsRunOnTwl()) {
        if (work->wirelessMode == 2) {
            NWMInfoElements *ie = NULL;

            if (bss->otherElementCount != 0) {
                ie = (NWMInfoElements *)func_02703250((NWMBssDesc *)bss, 0xdd, oui.bytes);
            }
            if (ie != NULL) {
                // The game info starts 8 bytes into the element's data
                const WMGameInfo *info = (const WMGameInfo *)&ie->element[4];

                if (info->magicNumber == 1) {
                    return (void *)info;
                }
            }
        }
    } else if (work->wirelessMode == 1) {
        u16 len = bss->gameInfoLength;

        if (len != 0) {
            BOOL valid = FALSE;

            if (len >= 0x10 && len <= 0x80 && len == bss->gameInfo.userGameInfoLength + 0x10 &&
                bss->gameInfo.magicNumber == 1 && bss->length == (len + 1 + offsetof(WMBssDesc, gameInfo)) / 2) {
                valid = TRUE;
            }
            if (valid == TRUE) {
                return (void *)&bss->gameInfo;
            }
        }
    }
    return NULL;
}
