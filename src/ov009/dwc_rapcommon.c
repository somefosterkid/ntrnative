// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov011/dwc_rapcommon.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// dwc_rapcommon.c: the memory the Wi-Fi connection's DWC library allocates from, its start-up, and what an
// application does with the connection's error. The names are ours

#include "gfl/dwc_rapcommon.h"
#include "types.h"
#include "dwc/dwc.h"
#include "app/wfc_user_info_warning.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_state.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nnsys/fnd.h"
#include "nitro/mi.h"

typedef struct DWCRapCommonWork {
    NNSFndHeapHandle heap;
    void *heapPtr;
    // A second heap for an application, which the library allocates from while it is set
    NNSFndHeapHandle headHandleSub;
    void *heapPtrSub;
    u32 subName;
    int subCount;
    u32 subMinSize;
    int destroySub;
    u16 subHeapId;
} DWCRapCommonWork;

// pokebw2's pDwcRapWork
static DWCRapCommonWork *data_ov009_021a8268;

static void func_ov009_02156f14(void);
static int func_ov009_02157108(HeapID heapId);
void func_ov009_02157160(void);
BOOL DWC_CheckUserData(void *userData);
void func_020596fc(u32 *param_1);
int func_02059f6c(const char *gameName, u32 gameCode, void *(*alloc)(u32, u32, int), void (*free)(u32, void *, u32));
u16 NNS_FndSetAllocModeForExpHeap(NNSFndHeapHandle heap, u16 mode);
u16 NNS_FndSetGroupIDForExpHeap(NNSFndHeapHandle heap, u16 groupID);

// pokebw2's DWCRapCommon_SetSubHeap

void func_ov009_02156dfc(u32 heapId) {
    if (func_ov009_02157108(heapId) == 3) {
        GFL_OvlLoad(OVERLAY_WFC_USER_INFO_WARNING);
        func_ov017_021b6100();
        GFL_OvlUnload(OVERLAY_WFC_USER_INFO_WARNING);
    }
}

void func_ov009_02156e24(u32 name, u32 size, HeapID heapId) {
    func_ov009_02156e30(name, size, 0, heapId);
}

void func_ov009_02156e30(u32 name, u32 size, u32 minSize, HeapID heapId) {
    GFL_ASSERT(data_ov009_021a8268 != NULL);
    GFL_ASSERT(data_ov009_021a8268->heapPtrSub == NULL);
    GFL_ASSERT(data_ov009_021a8268->headHandleSub == NULL);

    data_ov009_021a8268->heapPtrSub = GFL_HeapAllocate(heapId, size - 0x80, FALSE, "dwc_rapcommon.c", 98);
    data_ov009_021a8268->headHandleSub =
        NNS_FndCreateExpHeapEx((void *)(((uintptr_t)data_ov009_021a8268->heapPtrSub + 0x1f) & ~0x1f), size - 0xc0, 0);
    data_ov009_021a8268->subName = name;
    NNS_FndSetAllocModeForExpHeap(data_ov009_021a8268->headHandleSub, 1);
    NNS_FndSetGroupIDForExpHeap(data_ov009_021a8268->headHandleSub, 0xea);
    data_ov009_021a8268->subMinSize = minSize;
}


// pokebw2's DWCRapCommon_EndSubHeap
void func_ov009_02156ed0(void) {
    GFL_ASSERT(data_ov009_021a8268);

    if (data_ov009_021a8268->subCount != 0) {
        // Blocks of the second heap are still allocated
        func_0203c9ac(0, 0, 0, 0x3ee);
        data_ov009_021a8268->destroySub = TRUE;
    } else {
        func_ov009_02156f14();
    }
}

// pokebw2's DWCRapCommon_DestroySubHeap
static void func_ov009_02156f14(void) {
    if (data_ov009_021a8268->heapPtrSub != NULL) {
        NNS_FndDestroyExpHeap(data_ov009_021a8268->headHandleSub);
        GFL_HeapFree(data_ov009_021a8268->heapPtrSub);
        data_ov009_021a8268->headHandleSub = NULL;
        data_ov009_021a8268->heapPtrSub = NULL;
        if (data_ov009_021a8268->subHeapId != 0) {
            GFL_HeapDelete(data_ov009_021a8268->subHeapId);
            data_ov009_021a8268->subHeapId = 0;
        }
    }
}

// pokebw2's DWCRapCommon_SetSubHeapOwner
void func_ov009_02156f50(HeapID heapId) {
    data_ov009_021a8268->subHeapId = heapId;
}

// pokebw2's DWCRapCommon_Alloc
void *func_ov009_02156f5c(u32 name, u32 size, int align) {
    void *ptr;
    u32 mask;

    if (data_ov009_021a8268->headHandleSub != NULL && name == data_ov009_021a8268->subName && size > data_ov009_021a8268->subMinSize) {
        mask = OS_DisableInterrupts();
        ptr = NNS_FndAllocFromExpHeapEx(data_ov009_021a8268->headHandleSub, size, align);
        OS_RestoreInterrupts(mask);
        data_ov009_021a8268->subCount++;
    } else {
        mask = OS_DisableInterrupts();
        ptr = NNS_FndAllocFromExpHeapEx(data_ov009_021a8268->heap, size, align);
        OS_RestoreInterrupts(mask);
    }

    if (ptr == NULL) {
        AssertFailErrorDisp();
        func_0203c9ac(0, 0, 0, 0x3ee);
        return NULL;
    }
    return ptr;
}

// pokebw2's DWCRapCommon_Free
void func_ov009_02156fdc(u32 name, void *ptr, u32 size) {
    u32 mask;

    if (ptr != NULL) {
        if (NNS_FndGetGroupIDForMBlockExpHeap(ptr) == 0xea) {
            if (data_ov009_021a8268->headHandleSub != NULL) {
                mask = OS_DisableInterrupts();
                NNS_FndFreeToExpHeap(data_ov009_021a8268->headHandleSub, ptr);
                OS_RestoreInterrupts(mask);
                data_ov009_021a8268->subCount--;
            } else {
                func_0203c9ac(0, 0, 0, 0x3ee);
            }
        } else {
            mask = OS_DisableInterrupts();
            NNS_FndFreeToExpHeap(data_ov009_021a8268->heap, ptr);
            OS_RestoreInterrupts(mask);
        }
    }
}

// pokebw2's DWCRapCommon_Create
void func_ov009_02157048(HeapID heapId, u32 size) {
    GFL_ASSERT(data_ov009_021a8268==NULL);

    data_ov009_021a8268 = GFL_HeapAllocate(0, sizeof(DWCRapCommonWork), TRUE, "dwc_rapcommon.c", 332);
    data_ov009_021a8268->heapPtr = GFL_HeapAllocate(heapId, size - 0x80, FALSE, "dwc_rapcommon.c", 333);
    data_ov009_021a8268->heap = NNS_FndCreateExpHeapEx((void *)(((uintptr_t)data_ov009_021a8268->heapPtr + 0x1f) & ~0x1f), size - 0xc0, 0);
    data_ov009_021a8268->subHeapId = 0;
}

// pokebw2's DWCRapCommon_Delete
void func_ov009_021570b8(void) {
    GFL_ASSERT(data_ov009_021a8268);

    if (data_ov009_021a8268->destroySub == TRUE || data_ov009_021a8268->heapPtrSub != NULL) {
        func_ov009_02156f14();
    }
    NNS_FndDestroyExpHeap(data_ov009_021a8268->heap);
    GFL_HeapFree(data_ov009_021a8268->heapPtr);
    GFL_HeapFree(data_ov009_021a8268);
    data_ov009_021a8268 = NULL;
}

static int func_ov009_02157108(HeapID heapId) {
    int result;

    func_ov009_02157048(heapId, 0x40000);
    result = func_02059f6c("syachi2ds", 0x4952414a, func_ov009_02156f5c, func_ov009_02156fdc);
    func_ov009_021570b8();
    return result;
}


void func_ov009_02157138(void *userData) {
    MIi_CpuClearFast(0, userData, 0x40);
    if (!DWC_CheckUserData(userData)) {
        func_020596fc(userData);
        func_02059870(userData);
    }
}


void func_ov009_02157160(void) {
    func_0203ca04();
    func_0203c968();
    DWC_ClearError();
}

BOOL func_ov009_02157170(u32 a0, u32 a1) {
    GFLNetErrorInfo *error;
    int type;

    if (func_0203cd98()) {
        error = func_0203ca98();
        if (func_0200fb24()) {
            switch (error->unk4) {
            case 1:
            case 2:
                if (a0 == 0) {
                    func_ov009_02157160();
                    func_0200f83c();
                    func_0200fba0();
                    return 1;
                }
                DWC_ClearError();
                // fall through
            case 3:
            case 4:
            case 5:
                func_0200fb38();
                func_0200fb4c(13);
                return 2;
            case 6:
                func_0200fb38();
                return 2;
            case 7:
                func_0200f7c0(15);
                return 3;
            }
        }

        type = error->type;
        if (type == 0x3f0 || type == 0x3f1) {
            if (a1 != 0) {
                func_0200faac();
                func_ov009_02157160();
                func_0203c554(TRUE);
                func_0200f83c();
                func_0200fba0();
                return 4;
            }
            func_0203ca04();
            func_0200fba0();
            func_0203c968();
            func_0203c554(TRUE);
            return 0;
        }
        if (type == 0x3f2 || type == 0x3f3 || type == 0x3f4 || type == 0x3f6) {
            if (a0 == 0) {
                func_ov009_02157160();
                func_0200f83c();
                func_0200fba0();
                return 1;
            }
            func_0200fb38();
            func_0200fb4c(13);
            return 2;
        }
        if (type == 0x3ee || type == 0x3f7) {
            func_0200fb38();
            func_0200fb4c(13);
            return 2;
        }
    }
    return 0;
}



