// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/gf_overlay.c.
#include "types.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "nitro/fs.h"
#include "nitro/mi.h"
#include "nitro/os.h"

// The memory an overlay is loaded to
enum {
    OVL_REGION_MAIN,
    OVL_REGION_ITCM,
    OVL_REGION_DTCM,
};

#define HW_ITCM 0x01000000
#define HW_ITCM_END 0x02000000
#define HW_DTCM_SIZE 0x4000

typedef struct {
    u32 id;
    BOOL loaded;
} OvlEntry;

// The overlays loaded to each region, with room for as many as each can hold at once
typedef struct {
    u8 mainCount;
    u8 itcmCount;
    u8 dtcmCount;
    OvlEntry *main;
    OvlEntry *itcm;
    OvlEntry *dtcm;
} OvlManager;

static void GFL_OvlEntryUnload(OvlEntry *entry);
static BOOL GFL_OvlCheckMemoryCollision(u32 id);
static OvlEntry *GFL_OvlGetList(int region);
static int GFL_OvlGetListCount(int region);
static int GFL_OvlGetMemoryRegion(u32 id);
static BOOL GFL_OvlGetMemoryRange(u32 id, uintptr_t *start, uintptr_t *end);

static OvlManager *data_021469b8;

void GFL_OvlManagerInit(HeapID heapId, u32 mainCount, u32 itcmCount, u32 dtcmCount) {
    u32 size = (mainCount + itcmCount + dtcmCount) * sizeof(OvlEntry) + sizeof(OvlManager);

    data_021469b8 = GFL_HeapAllocate(heapId, size, FALSE, "gf_overlay.c", 108);
    MIi_CpuClear32(0, data_021469b8, size);
    data_021469b8->mainCount = mainCount;
    data_021469b8->itcmCount = itcmCount;
    data_021469b8->dtcmCount = dtcmCount;
    data_021469b8->main = (OvlEntry *)(data_021469b8 + 1);
    data_021469b8->itcm = data_021469b8->main + mainCount;
    data_021469b8->dtcm = data_021469b8->itcm + itcmCount;
}

static void GFL_OvlEntryUnload(OvlEntry *entry) {
    FS_UnloadOverlay(MI_PROCESSOR_ARM9, entry->id);
    entry->loaded = FALSE;
}

void GFL_OvlUnload(u32 overlayId) {
    int region;
    OvlEntry *list;
    int count;
    int i;

    if (overlayId == OVERLAY_NONE) {
        return;
    }
    region = GFL_OvlGetMemoryRegion(overlayId);
    list = GFL_OvlGetList(region);
    count = GFL_OvlGetListCount(region);
    for (i = 0; i < count; i++) {
        if (list[i].loaded == TRUE && overlayId == list[i].id) {
            GFL_OvlEntryUnload(&list[i]);
            return;
        }
    }
}

BOOL GFL_OvlLoad(u32 overlayId) {
    int region;
    u32 dma = FS_DMA_NOT_USE;
    OvlEntry *list;
    int count;
    int i;
    BOOL loaded;

    if (overlayId == OVERLAY_NONE) {
        return TRUE;
    }
    if (!GFL_OvlCheckMemoryCollision(overlayId)) {
        return FALSE;
    }
    region = GFL_OvlGetMemoryRegion(overlayId);
    list = GFL_OvlGetList(region);
    count = GFL_OvlGetListCount(region);
    for (i = 0; i < count; i++) {
        OvlEntry *entry = &list[i];

        if (entry->loaded == FALSE) {
            entry->loaded = TRUE;
            entry->id = overlayId;
            break;
        }
    }
    if (i >= count) {
        GFL_ASSERT_AT(267, 0);
        return FALSE;
    }
    // Tightly coupled memory can't be written by DMA
    if (region == OVL_REGION_ITCM || region == OVL_REGION_DTCM) {
        dma = FS_SetDefaultDMA(FS_DMA_NOT_USE);
    }
    loaded = FS_LoadOverlay(MI_PROCESSOR_ARM9, overlayId);
    if (region == OVL_REGION_ITCM || region == OVL_REGION_DTCM) {
        FS_SetDefaultDMA(dma);
    }
    if (!loaded) {
        return FALSE;
    }
    return TRUE;
}

// Returns FALSE if the overlay would overwrite one that is loaded
static BOOL GFL_OvlCheckMemoryCollision(u32 id) {
    uintptr_t start;
    uintptr_t end;
    int region;
    OvlEntry *list;
    int count;
    int i;

    if (!GFL_OvlGetMemoryRange(id, &start, &end)) {
        return FALSE;
    }
    region = GFL_OvlGetMemoryRegion(id);
    list = GFL_OvlGetList(region);
    count = GFL_OvlGetListCount(region);
    for (i = 0; i < count; i++) {
        uintptr_t loadedStart;
        uintptr_t loadedEnd;

        if (list[i].loaded == TRUE && GFL_OvlGetMemoryRange(list[i].id, &loadedStart, &loadedEnd) == TRUE) {
            if ((start >= loadedStart && start < loadedEnd) || (end > loadedStart && end <= loadedEnd)
                || (start <= loadedStart && end >= loadedEnd)) {
                GFL_ASSERT_AT(330, 0);
                return FALSE;
            }
        }
    }
    return TRUE;
}

static OvlEntry *GFL_OvlGetList(int region) {
    switch (region) {
    case OVL_REGION_MAIN:
    default:
        return data_021469b8->main;
    case OVL_REGION_ITCM:
        return data_021469b8->itcm;
    case OVL_REGION_DTCM:
        return data_021469b8->dtcm;
    }
}

static int GFL_OvlGetListCount(int region) {
    switch (region) {
    case OVL_REGION_MAIN:
        return data_021469b8->mainCount;
    case OVL_REGION_ITCM:
        return data_021469b8->itcmCount;
    case OVL_REGION_DTCM:
        return data_021469b8->dtcmCount;
    }
    return 0;
}

static int GFL_OvlGetMemoryRegion(u32 id) {
    FSOverlayInfo info;
    uintptr_t address;

    FS_LoadOverlayInfo(&info, MI_PROCESSOR_ARM9, id);
    address = (uintptr_t)info.header.ram_address;
    if (address <= HW_ITCM_END && address >= HW_ITCM) {
        return OVL_REGION_ITCM;
    }
    if (address <= HW_DTCM + HW_DTCM_SIZE && address >= HW_DTCM) {
        return OVL_REGION_DTCM;
    }
    return OVL_REGION_MAIN;
}

static BOOL GFL_OvlGetMemoryRange(u32 id, uintptr_t *start, uintptr_t *end) {
    FSOverlayInfo info;

    if (!FS_LoadOverlayInfo(&info, MI_PROCESSOR_ARM9, id)) {
        return FALSE;
    }
    *start = (uintptr_t)info.header.ram_address;
    *end = *start + (info.header.ram_size + info.header.bss_size);
    return TRUE;
}
