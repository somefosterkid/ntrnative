#include "types.h"
#include "gfl/heap.h"
#include "gfl/heapsys.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nnsys/fnd.h"

// The heap API over heapsys.c, which stops the game when the heap system fails. The debug messages the failures printed
// are compiled out, which leaves the calls that computed their arguments

// What GFL_HeapDumpProc reports for the block it visits
typedef struct {
    u16 line;
    void *ptr;
    u32 size;
} HeapDumpState;

static void GFL_HeapHandleAllocResult(u16 result, HeapID parentHeapId, HeapID heapId);
static void GFL_HeapSetBlockDebugInfo(void *ptr, const char *file, u16 line);
static BOOL GFL_HeapDebugCopyFileName(char *dest, const char *file);
static void GFL_HeapDumpAllocFailure(HeapID heapId, u32 size, const char *file, u16 line);
static void GFL_HeapDebugNotifyTrackedAlloc(void *ptr, u32 size);
static void GFL_HeapDebugNotifyTrackedFree(void *ptr);
static void GFL_HeapDebugTraceAlloc(HeapID heapId, void *ptr, u32 size);
static void GFL_HeapDebugTraceFree(void *ptr);
static void GFL_HeapDeleteCleanCheck(HeapID heapId);
static void GFL_HeapDumpProc(void *block, NNSFndHeapHandle heap, u32 param);

// The heap whose allocations and frees the debug build reported, or -1 for none
static int sTrackHeapId = -1;

static HeapDumpState sHeapDumpState;
static char sHeapDumpFile[0x14];

#if !defined(__MWERKS__) && !defined(__arm__)
// The port's heap report (no twin on the DS): a heap that runs out prints its size, its free bytes and its blocks by
// the file and line that allocated them to stderr before the game stops; with BWPORT_HEAPLOG=1 each heap's peak use
// (its size less its least total free) is kept and printed when the heap is deleted
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT_HEAP_IDS 0x100
#define PORT_HEAP_SITES 256

typedef struct {
    char file[0x14];
    u16 line;
    u32 count;
    u32 bytes;
} PortHeapSite;

static PortHeapSite sPortHeapSites[PORT_HEAP_SITES];
static int sPortHeapSiteCount;
static int sPortHeapLog = -1;
static u32 sPortHeapPeak[PORT_HEAP_IDS];

static void PortHeap_SiteProc(void *block, NNSFndHeapHandle heap, u32 param) {
    char file[0x14];
    HeapDebugInfo *info = GFL_HeapGetBlockDebugInfoPtr((HeapBlockHeader *)block + 1);
    u32 size = NNS_FndGetSizeForMBlockExpHeap(block);
    u16 line = info->line;
    int i;

    if (!GFL_HeapDebugCopyFileName(file, info->file)) {
        MI_CpuCopy8("SYSTEM ALLOC", file, 13);
        line = 0;
    }
    for (i = 0; i < sPortHeapSiteCount; i++) {
        if (sPortHeapSites[i].line == line && strcmp(sPortHeapSites[i].file, file) == 0) {
            break;
        }
    }
    if (i == sPortHeapSiteCount) {
        if (i == PORT_HEAP_SITES) {
            return;
        }
        MI_CpuCopy8(file, sPortHeapSites[i].file, sizeof(file));
        sPortHeapSites[i].line = line;
        sPortHeapSites[i].count = 0;
        sPortHeapSites[i].bytes = 0;
        sPortHeapSiteCount++;
    }
    sPortHeapSites[i].count++;
    sPortHeapSites[i].bytes += size;
}

static int PortHeap_SiteCompare(const void *a, const void *b) {
    const PortHeapSite *x = a;
    const PortHeapSite *y = b;

    return (x->bytes < y->bytes) - (x->bytes > y->bytes);
}

static BOOL PortHeap_LogOn(void) {
    if (sPortHeapLog < 0) {
        const char *env = getenv("BWPORT_HEAPLOG");
        sPortHeapLog = env != NULL && env[0] != '\0' && env[0] != '0';
    }
    return sPortHeapLog;
}

static void PortHeap_NotePeak(HeapID heapId) {
    u32 used;

    heapId &= HEAPID_TAIL_BIT - 1;
    if (heapId >= PORT_HEAP_IDS || !PortHeap_LogOn()) {
        return;
    }
    used = GFL_HeapGetSize(heapId) - GFL_HeapGetFreeSizeCore(heapId);
    if (used > sPortHeapPeak[heapId]) {
        sPortHeapPeak[heapId] = used;
    }
}

static void PortHeap_ReportDelete(HeapID heapId) {
    u32 size;

    heapId &= HEAPID_TAIL_BIT - 1;
    if (heapId >= PORT_HEAP_IDS || !PortHeap_LogOn() || GFL_HeapGetValidHeapBase(heapId) == NULL) {
        return;
    }
    size = GFL_HeapGetSize(heapId);
    fprintf(stderr, "bwport: heap %u deleted: size 0x%x, peak use 0x%x (%u%%), %u blocks left\n", heapId, size,
        sPortHeapPeak[heapId], size != 0 ? (unsigned)((u64)sPortHeapPeak[heapId] * 100 / size) : 0,
        GFL_HeapGetAllocationCount(heapId));
    sPortHeapPeak[heapId] = 0;
}

typedef struct {
    uintptr_t start;
    uintptr_t end;
    u16 signature;
} PortHeapSpan;

static PortHeapSpan sPortHeapSpans[4096];

static int PortHeap_SpanCompare(const void *a, const void *b) {
    const PortHeapSpan *x = a;
    const PortHeapSpan *y = b;

    return (x->start > y->start) - (x->start < y->start);
}

// Every byte of an expanded heap is in a used block, a free block or the padding before a block: returns the bytes
// in neither (lost to the heap) plus one per overlap; verbose prints them and the blocks around the first
static u32 PortHeap_CheckSpans(NNSFndHeapHandle heap, BOOL verbose) {
    NNSiFndExpHeapHead *exp = (NNSiFndExpHeapHead *)((u8 *)heap + sizeof(NNSiFndHeapHead));
    NNSiFndExpHeapMBlockHead *block;
    int count = 0;
    int list;
    int i;
    int first = -1;
    uintptr_t base = (uintptr_t)heap->heapStart;
    uintptr_t at = base;
    u32 freeBytes = 0;
    u32 lost = 0;

    for (list = 0; list < 2; list++) {
        for (block = list == 0 ? exp->mbFreeList.head : exp->mbUsedList.head; block != NULL && count < 4096;
             block = block->pMBHeadNext) {
            sPortHeapSpans[count].start = (uintptr_t)block - ((block->attribute >> 8) & 0x7f);
            sPortHeapSpans[count].end = (uintptr_t)(block + 1) + block->blockSize;
            sPortHeapSpans[count].signature = block->signature;
            if (list == 0) {
                freeBytes += block->blockSize;
            }
            count++;
        }
    }
    qsort(sPortHeapSpans, count, sizeof(sPortHeapSpans[0]), PortHeap_SpanCompare);
    for (i = 0; i < count; i++) {
        if (sPortHeapSpans[i].start != at) {
            if (first < 0) {
                first = i;
            }
            if (verbose) {
                fprintf(stderr, "bwport:   heap span gap/overlap at +0x%lx: %ld bytes before a %c%c block\n",
                    (unsigned long)(at - base), (long)(sPortHeapSpans[i].start - at),
                    sPortHeapSpans[i].signature >> 8, sPortHeapSpans[i].signature & 0xff);
            }
            lost += sPortHeapSpans[i].start > at ? (u32)(sPortHeapSpans[i].start - at) : 1;
        }
        at = sPortHeapSpans[i].end;
    }
    if (at != (uintptr_t)heap->heapEnd) {
        lost += at < (uintptr_t)heap->heapEnd ? (u32)((uintptr_t)heap->heapEnd - at) : 1;
        if (first < 0) {
            first = count;
        }
        if (verbose) {
            fprintf(stderr, "bwport:   heap span gap at the end: %ld bytes\n", (long)((uintptr_t)heap->heapEnd - at));
        }
    }
    if (verbose) {
        fprintf(stderr, "bwport:   heap spans: %d blocks, free 0x%x, lost 0x%x of 0x%lx\n", count, freeBytes, lost,
            (unsigned long)((uintptr_t)heap->heapEnd - base));
        for (i = first - 4; first >= 0 && i <= first + 2; i++) {
            if (i >= 0 && i < count) {
                const PortHeapSpan *span = &sPortHeapSpans[i];
                NNSiFndExpHeapMBlockHead *mb = NULL;
                char file[0x14];

                // the NNS header sits after the padding: find it by the span's end and size
                for (list = 0; list < 2 && mb == NULL; list++) {
                    for (block = list == 0 ? exp->mbFreeList.head : exp->mbUsedList.head; block != NULL;
                         block = block->pMBHeadNext) {
                        if ((uintptr_t)(block + 1) + block->blockSize == span->end) {
                            mb = block;
                            break;
                        }
                    }
                }
                file[0] = '\0';
                if (mb != NULL && mb->signature == 0x5544) {
                    HeapDebugInfo *info = &((HeapBlockHeader *)(mb + 1))->debug;
                    if (!GFL_HeapDebugCopyFileName(file, info->file)) {
                        MI_CpuCopy8("SYSTEM ALLOC", file, 13);
                    }
                    fprintf(stderr, "bwport:     +0x%lx..+0x%lx UD size 0x%x %s:%u\n", (unsigned long)(span->start - base),
                        (unsigned long)(span->end - base), mb->blockSize, file, info->line);
                } else {
                    fprintf(stderr, "bwport:     +0x%lx..+0x%lx %c%c size 0x%x\n", (unsigned long)(span->start - base),
                        (unsigned long)(span->end - base), span->signature >> 8, span->signature & 0xff,
                        mb != NULL ? mb->blockSize : 0);
                }
            }
        }
    }
    return lost;
}

// BWPORT_HEAPLOG=2: checks heap BWPORT_HEAPCHECK (default 21) after each allocation and free and reports the first
// operation after which bytes are lost
static int sPortHeapCheckId = -1;
static BOOL sPortHeapCheckDone;

static void PortHeap_Check(HeapID heapId, const char *what, const char *file, u16 line, u32 size) {
    NNSFndHeapHandle heap;

    if (sPortHeapCheckDone || !PortHeap_LogOn() || getenv("BWPORT_HEAPLOG")[0] != '2') {
        return;
    }
    if (sPortHeapCheckId < 0) {
        const char *env = getenv("BWPORT_HEAPCHECK");
        sPortHeapCheckId = env != NULL ? atoi(env) : 21;
    }
    heapId &= HEAPID_TAIL_BIT - 1;
    if (heapId != sPortHeapCheckId || (heap = GFL_HeapGetValidHeapBase(heapId)) == NULL) {
        return;
    }
    if (PortHeap_CheckSpans(heap, FALSE) != 0) {
        sPortHeapCheckDone = TRUE;
        fprintf(stderr, "bwport: heap %u broken after %s of 0x%x for %s:%u\n", heapId, what, size, file, line);
        PortHeap_CheckSpans(heap, TRUE);
    }
}

static void PortHeap_ReportFailure(HeapID heapId, u32 size, const char *file, u16 line) {
    NNSFndHeapHandle heap;
    int i;

    heapId &= HEAPID_TAIL_BIT - 1;
    heap = GFL_HeapGetValidHeapBase(heapId);
    fprintf(stderr, "bwport: heap %u out of memory: 0x%x bytes for %s:%u; size 0x%x, free 0x%x, largest free 0x%x, %u blocks\n",
        heapId, size, file, line, GFL_HeapGetSize(heapId), GFL_HeapGetFreeSizeCore(heapId),
        GFL_HeapGetHighestAllocatableSize(heapId), GFL_HeapGetAllocationCount(heapId));
    if (heap == NULL) {
        return;
    }
    PortHeap_CheckSpans(heap, TRUE);
    sPortHeapSiteCount = 0;
    NNS_FndVisitAllocatedForExpHeap(heap, PortHeap_SiteProc, 0);
    qsort(sPortHeapSites, sPortHeapSiteCount, sizeof(sPortHeapSites[0]), PortHeap_SiteCompare);
    for (i = 0; i < sPortHeapSiteCount; i++) {
        fprintf(stderr, "bwport:   heap %u: 0x%06x bytes in %4u blocks from %s:%u\n", heapId, sPortHeapSites[i].bytes,
            sPortHeapSites[i].count, sPortHeapSites[i].file, sPortHeapSites[i].line);
    }
}
#define PORT_HEAP_NOTE_PEAK(heapId) PortHeap_NotePeak(heapId)
#define PORT_HEAP_CHECK(heapId, what, file, line, size) PortHeap_Check(heapId, what, file, line, size)
#define PORT_HEAP_REPORT_DELETE(heapId) PortHeap_ReportDelete(heapId)
#define PORT_HEAP_REPORT_FAILURE(heapId, size, file, line) PortHeap_ReportFailure(heapId, size, file, line)
#else
#define PORT_HEAP_NOTE_PEAK(heapId)
#define PORT_HEAP_CHECK(heapId, what, file, line, size)
#define PORT_HEAP_REPORT_DELETE(heapId)
#define PORT_HEAP_REPORT_FAILURE(heapId, size, file, line)
#endif

void GFL_MemInit(const HeapDef *defs, u32 rootCount, u32 maxHeapIds, u32 reserveSize) {
    BOOL ok = GFL_HeapMgrInit(defs, rootCount, maxHeapIds, reserveSize);

    OS_GetArenaLo(OS_ARENA_MAIN);
    OS_GetArenaHi(OS_ARENA_MAIN);
    if (!ok) {
        GFL_HeapGetLastResult();
        OS_Terminate();
    }
}

void GFL_HeapCreateChild(HeapID parentHeapId, HeapID heapId, u32 size) {
    if (!GFL_HeapAddChild(parentHeapId, heapId, size)) {
        GFL_HeapHandleAllocResult(GFL_HeapGetLastResult(), parentHeapId, heapId);
    }
}

void GFL_HeapCreateRoot(void *memory, u32 size, HeapID heapId) {
    if (!GFL_HeapAddRoot(memory, size, heapId)) {
        GFL_HeapHandleAllocResult(GFL_HeapGetLastResult(), 0, heapId);
    }
}

static void GFL_HeapHandleAllocResult(u16 result, HeapID parentHeapId, HeapID heapId) {
    switch (result) {
    case HEAP_RESULT_1:
        OS_Terminate();
        break;
    case HEAP_RESULT_2:
        OS_Terminate();
        break;
    case HEAP_RESULT_3:
        OS_Terminate();
        break;
    case HEAP_RESULT_4:
        OS_Terminate();
        break;
    case HEAP_RESULT_5:
        OS_Terminate();
        break;
    }
}

void GFL_HeapDelete(HeapID heapId) {
    PORT_HEAP_REPORT_DELETE(heapId);
    GFL_HeapDeleteCleanCheck(heapId);
    if (!GFL_HeapDeleteCore(heapId)) {
        switch (GFL_HeapGetLastResult()) {
        case HEAP_RESULT_1:
            OS_Terminate();
            break;
        case HEAP_RESULT_2:
            OS_Terminate();
            break;
        }
    }
}

void *GFL_HeapAllocate(HeapID heapId, u32 size, BOOL clear, const char *file, u16 line) {
    void *ptr = GFL_HeapAllocateCore(heapId, size);

    if (ptr == NULL) {
        GFL_HeapGetLastResult();
        PORT_HEAP_REPORT_FAILURE(heapId, size, file, line);
        GFL_HeapDumpAllocFailure(heapId, size, file, line);
        OS_Terminate();
    } else {
        GFL_HeapSetBlockDebugInfo(ptr, file, line);
        PORT_HEAP_NOTE_PEAK(heapId);
        PORT_HEAP_CHECK(heapId, "alloc", file, line, size);
        GFL_HeapDebugTraceAlloc(heapId, ptr, size);
        if (clear) {
            MIi_CpuClear32(0, ptr, size);
        }
    }
    return ptr;
}

void GFL_HeapFree(void *ptr) {
    PORT_HEAP_CHECK(GFL_HeapGetBlockHeapID(ptr), "before free", GFL_HeapGetBlockDebugInfoPtr(ptr)->file, GFL_HeapGetBlockDebugInfoPtr(ptr)->line, 0);
    GFL_HeapDebugTraceFree(ptr);
    GFL_HeapFreeCore(ptr);
}

void GFL_HeapCreateAllocator(NNSFndAllocator *allocator, HeapID heapId, int alignment) {
    if (!GFL_HeapCreateAllocatorCore(allocator, heapId, alignment)) {
        OS_Terminate();
    }
}

void GFL_HeapResize(void *ptr, u32 size) {
    if (!GFL_HeapResizeCore(ptr, size)) {
        switch (GFL_HeapGetLastResult()) {
        case HEAP_RESULT_1:
            OS_Terminate();
            break;
        case HEAP_RESULT_2:
            OS_Terminate();
            break;
        case HEAP_RESULT_3:
            OS_Terminate();
            break;
        case HEAP_RESULT_4:
            break;
        case HEAP_RESULT_5:
            break;
        }
    }
}

u32 GFL_HeapGetFreeSize(HeapID heapId) {
    u32 size = GFL_HeapGetFreeSizeCore(heapId);

    if (size == 0) {
        OS_Terminate();
    }
    return size;
}

BOOL GFL_HeapStatusValidate(HeapID heapId) {
    BOOL ok = GFL_HeapStatusCheck(heapId);

    if (!ok) {
        switch (GFL_HeapGetLastResult()) {
        case HEAP_RESULT_1:
            break;
        case HEAP_RESULT_2:
            break;
        case HEAP_RESULT_3:
            OS_Terminate();
            break;
        }
    }
    return ok;
}

static void GFL_HeapSetBlockDebugInfo(void *ptr, const char *file, u16 line) {
    HeapDebugInfo *info = GFL_HeapGetBlockDebugInfoPtr(ptr);
    int i;

    for (i = 0; i < (int)sizeof(info->file); i++) {
        info->file[i] = file[i];
        if (file[i] == '\0') {
            break;
        }
    }
    info->line = line;
}

// Copies a block's file name, returning whether it looks like one: printable, and with an extension unless it fills
// the name
static BOOL GFL_HeapDebugCopyFileName(char *dest, const char *file) {
    int len;
    int i;

    for (len = 0; len < (int)sizeof(((HeapDebugInfo *)NULL)->file); len++) {
        if (file[len] == '\0') {
            break;
        }
        dest[len] = file[len];
    }
    dest[len] = '\0';
    if (len == 0) {
        return FALSE;
    }
    for (i = 0; i < len; i++) {
        if ((u8)dest[i] < ' ' || (u8)dest[i] > '~') {
            return FALSE;
        }
    }
    if (len < (int)sizeof(((HeapDebugInfo *)NULL)->file)) {
        for (i = 0; i < len; i++) {
            if ((u8)dest[i] == '.') {
                break;
            }
        }
        if (i == len) {
            return FALSE;
        }
    }
    return TRUE;
}

static void GFL_HeapDumpAllocFailure(HeapID heapId, u32 size, const char *file, u16 line) {
    NNSFndHeapHandle heap = GFL_HeapGetValidHeapBase(heapId);

    NNS_FndGetTotalFreeSizeForExpHeap(heap);
    NNS_FndGetAllocatableSizeForExpHeapEx(heap, 4);
    GFL_HeapDumpOnFailure(heapId);
}

static void GFL_HeapDebugNotifyTrackedAlloc(void *ptr, u32 size) {
    char file[0x14];
    HeapDebugInfo *info = GFL_HeapGetBlockDebugInfoPtr(ptr);

    GFL_HeapGetAllocationCount(GFL_HeapGetBlockHeapID(ptr));
    GFL_HeapDebugCopyFileName(file, info->file);
}

static void GFL_HeapDebugNotifyTrackedFree(void *ptr) {
    char file[0x14];
    HeapDebugInfo *info = GFL_HeapGetBlockDebugInfoPtr(ptr);
    HeapID heapId = GFL_HeapGetBlockHeapID(ptr);

    GFL_HeapGetAllocationCount(heapId);
    GFL_HeapDebugCopyFileName(file, info->file);
    NNS_FndGetSizeForMBlockExpHeap((HeapBlockHeader *)ptr - 1);
    GFL_HeapGetFreeSizeCore(heapId);
}

static void GFL_HeapDebugTraceAlloc(HeapID heapId, void *ptr, u32 size) {
    if (sTrackHeapId >= 0 && heapId == sTrackHeapId) {
        GFL_HeapDebugNotifyTrackedAlloc(ptr, size);
    }
}

static void GFL_HeapDebugTraceFree(void *ptr) {
    HeapID heapId = GFL_HeapGetBlockHeapID(ptr);

    if (sTrackHeapId >= 0 && heapId == sTrackHeapId) {
        GFL_HeapDebugNotifyTrackedFree(ptr);
    }
}

// Reports a heap deleted with blocks still allocated
static void GFL_HeapDeleteCleanCheck(HeapID heapId) {
    if (GFL_HeapGetAllocationCount(heapId) != 0) {
        GFL_HeapDumpOnFailure(heapId);
    }
}

static void GFL_HeapDumpProc(void *block, NNSFndHeapHandle heap, u32 param) {
    char file[0x14];
    void *ptr = (HeapBlockHeader *)block + 1;
    HeapDebugInfo *info = GFL_HeapGetBlockDebugInfoPtr(ptr);
    u16 line;
    int pad;
    int i;

    sHeapDumpState.size = NNS_FndGetSizeForMBlockExpHeap(block);
    sHeapDumpState.ptr = ptr;
    if (!GFL_HeapDebugCopyFileName(file, info->file)) {
        MI_CpuCopy8("SYSTEM ALLOC", file, 13);
        line = 0;
    } else {
        line = info->line;
    }
    MI_CpuCopy8(file, sHeapDumpFile, GFL_STD_StrLen(file) + 1);
    sHeapDumpState.line = line;
    pad = 0x12 - GFL_STD_StrLen(file);
    for (i = 0; i < pad; i++) {
        file[i] = ' ';
    }
    file[i] = '\0';
}

// Prints a heap's sizes and its allocated blocks
void GFL_HeapDumpOnFailure(HeapID heapId) {
    GFL_HeapGetSize(heapId);
    GFL_HeapGetFreeSizeCore(heapId);
    GFL_HeapGetHighestAllocatableSize(heapId);
    GFL_HeapGetAllocationCount(heapId);
    exit(1);
    NNS_FndVisitAllocatedForExpHeap(GFL_HeapGetValidHeapBase(heapId), GFL_HeapDumpProc, 0);
    exit(0);
}

void GFL_HeapDTCMInit(u32 size) {
    if (!GFL_HeapDTCMInitCore(size)) {
        OS_Terminate();
    }
}

void *GFL_HeapDTCMAllocate(u32 size) {
    return GFL_HeapDTCMAllocateCore(size);
}

void _freeBlkFromDTCM(void *ptr) {
    freeBlkFromDTCM(ptr);
}
