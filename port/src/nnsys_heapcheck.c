// The heap checker (BWPORT_HEAPCHECK=1, docs/nnsys-port.md "Heap checker"; hc1, 2026-11-15). Off unless the env var is set: each
// wrapper in port/extern/lib/nnsys/src/fnd/expheap.c tests one int. On, it walks NitroSystem's expanded heaps (every game's
// heaps are these: Heap_Alloc of Platinum and HGSS, GFL's of Black/White) and stops the exe on the first damaged one, naming the
// block before the damage and what allocated it. Why: a block written past its end (a DS-literal size, a work with 8-byte
// pointers in a 4-byte-pointer size) cuts the heap's free list or a neighbour's header silently; the game then runs out of heap or
// crashes frames or minutes later (hg38: the rival battle's "out of memory" was two such allocs). Found by walking the lists at
// every alloc in gdb; this does it in the exe.
//
//   BWPORT_HEAPCHECK=1   the heap an alloc / free / resize is about to touch has both its lists walked first (signatures, sizes,
//                        prev / next links, every block inside the heap, the tail pointers, the free list in address order): what
//                        was written since the heap's last operation is what this finds, before the heap acts on it. And once
//                        per V-blank wait (PORT_HeapCheckFrame, from PORT_ExitFrame) every heap the game made is checked, the
//                        blocks of both lists also sorted by address: they must tile the heap with no gap and no overlap
//   BWPORT_HEAPCHECK=2   every heap at every alloc / free (slow)
//   BWPORT_HEAPCHECK=0   off (also the unset default). (Black/White's pokebw2-heapreport.patch reads BWPORT_HEAPCHECK as a GFL
//                        heap ID, but only under BWPORT_HEAPLOG=2: do not set both.)
//
// A damaged heap prints, to stderr, one line `heapcheck: kind=K frame=F heap=H ... owner=0x.. ip=0x.. ... anchor=0x..` and some
// detail lines (`heapcheck:   ...`), flushes and abort()s: under gdb the stop is at the detecting call. tools/port_run.py turns
// the line into its verdict (`heapcheck: heap overrun after FUNC (file:line), ...`: the ips resolved with the exe's nm / addr2line
// as anchor + (ip - anchor's runtime address)). Every alloc is recorded (pointer -> requested size, frame, the callers' addresses,
// with glibc's backtrace on Linux and macOS; Windows keeps the one return address) so "the block before the bad one" names its
// allocator. Only damage the heap's own structure shows is found: a write past a block into the next block's data (more than
// its 24-byte header) is not.
#ifdef BWPORT_NNSYS
#include <nitro.h>
#include <nnsys.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if (defined(__linux__) && !defined(__ANDROID__)) || defined(__APPLE__)
#include <execinfo.h>
#define HC_BACKTRACE 1
#endif

int PORT_HeapCheckLevel = -1;   // -1: the env var is not read yet (the first expanded-heap call), 0 off, 1, 2

void PORT_HeapCheckInit(void);
void PORT_HeapCheckLock(void);
void PORT_HeapCheckUnlock(void);
void PORT_HeapCheckCreated(NNSFndHeapHandle heap);
void PORT_HeapCheckDestroyed(NNSFndHeapHandle heap);
void PORT_HeapCheckBefore(NNSFndHeapHandle heap);
void PORT_HeapCheckAllocated(NNSFndHeapHandle heap, void *data, u32 size, void *caller);
void PORT_HeapCheckBeforeFree(NNSFndHeapHandle heap, void *data, void *caller);
void PORT_HeapCheckResized(NNSFndHeapHandle heap, void *data, u32 size, u32 got);
void PORT_HeapCheckFrame(void);
void PORT_HeapCheckReset(void);
void PORT_HeapCheckSetNamer(int (*namer)(NNSFndHeapHandle heap));
void PORT_HeapCheckAnchor(void);

#define HC_SIG_HEAP 0x45585048u   // 'EXPH'
#define HC_SIG_FREE 0x4652        // 'FR'
#define HC_SIG_USED 0x5544        // 'UD'
#define HC_HDR sizeof(NNSiFndExpHeapMBlockHead)
#define HC_MIN_BLOCK (HC_HDR + 4)
#define HC_MAX_HEAPS 1024
#define HC_BUCKETS (1 << 16)
#define HC_NIPS 8
#define HC_FREED_SLOTS (1 << 14)

typedef struct Rec {
    struct Rec *next;
    void *data;
    NNSFndHeapHandle heap;
    u32 size;    // as asked of the game's allocator (rounded up to 4 by the heap)
    u32 frame;   // the V-blank count when it was allocated
    int nips;
    void *ips[HC_NIPS];
} Rec;

// A freed block (direct-mapped by its pointer: the newest free of a slot wins): a second free of it, which the used list's stale
// header ('UD' still in the bytes of the free block that took it in) does not show, is said at the second call, with both callers.
typedef struct Freed {
    void *data;
    NNSFndHeapHandle heap;
    u32 frame;
    int nips;
    void *ips[HC_NIPS];
} Freed;

typedef struct Region {
    uintptr_t start, end;
    NNSiFndExpHeapMBlockHead *block;
    int used;
} Region;

static pthread_mutex_t sLock = PTHREAD_MUTEX_INITIALIZER;
static int sLocked;   // the holding thread's depth (the wrappers nest: a free inside a destroy)
static pthread_t sOwner;
static NNSFndHeapHandle sHeaps[HC_MAX_HEAPS];
static int sNHeaps;
static Rec *sTab[HC_BUCKETS];
static Freed sFreed[HC_FREED_SLOTS];
static size_t sNRec;
static int (*sNamer)(NNSFndHeapHandle);
static u32 sLastCleanFrame;
static u32 sStatsFrame;
static unsigned long long sCpuNs;   // the checker's own CPU time (the hooks below), for the stats line
static int sHadFrame;
static Region *sRegions;
static size_t sRegionsCap;
static unsigned long long sChecks, sBlocksWalked;

void PORT_HeapCheckInit(void) {
    const char *e = getenv("BWPORT_HEAPCHECK");
    int lvl = e != NULL ? atoi(e) : 0;

    PORT_HeapCheckLevel = lvl < 0 ? 0 : lvl > 2 ? 2 : lvl;
    if (PORT_HeapCheckLevel > 0) {
        fprintf(stderr, "heapcheck: on, level %d\n", PORT_HeapCheckLevel);
    }
}

void PORT_HeapCheckLock(void) {
    if (sLocked > 0 && pthread_equal(sOwner, pthread_self())) {
        sLocked++;
        return;
    }
    pthread_mutex_lock(&sLock);
    sOwner = pthread_self();
    sLocked = 1;
}

void PORT_HeapCheckUnlock(void) {
    if (--sLocked == 0) {
        pthread_mutex_unlock(&sLock);
    }
}

void PORT_HeapCheckSetNamer(int (*namer)(NNSFndHeapHandle heap)) {
    sNamer = namer;
}

static size_t Hash(const void *p) {
    uintptr_t v = (uintptr_t)p >> 3;

    return (size_t)((v ^ (v >> 16)) & (HC_BUCKETS - 1));
}

static Rec *Find(const void *data) {
    Rec *r;

    for (r = sTab[Hash(data)]; r != NULL; r = r->next) {
        if (r->data == data) {
            return r;
        }
    }
    return NULL;
}

static Freed *FreedSlot(const void *data) {
    return &sFreed[Hash(data) & (HC_FREED_SLOTS - 1)];
}

static void Drop(const void *data) {
    Rec **pp;

    for (pp = &sTab[Hash(data)]; *pp != NULL; pp = &(*pp)->next) {
        if ((*pp)->data == data) {
            Rec *r = *pp;

            *pp = r->next;
            free(r);
            sNRec--;
            return;
        }
    }
}

static void DropHeapRecords(NNSFndHeapHandle heap) {
    size_t i;

    for (i = 0; i < HC_BUCKETS; i++) {
        Rec **pp = &sTab[i];

        while (*pp != NULL) {
            if ((*pp)->heap == heap) {
                Rec *r = *pp;

                *pp = r->next;
                free(r);
                sNRec--;
            } else {
                pp = &(*pp)->next;
            }
        }
    }
}

static void Unregister(NNSFndHeapHandle heap) {
    int i;

    for (i = 0; i < sNHeaps; i++) {
        if (sHeaps[i] == heap) {
            sHeaps[i] = sHeaps[--sNHeaps];
            DropHeapRecords(heap);
            return;
        }
    }
}

void PORT_HeapCheckCreated(NNSFndHeapHandle heap) {
    int i;

    if (heap == NULL) {
        return;
    }
    for (i = 0; i < sNHeaps; i++) {
        if (sHeaps[i] == heap) {
            DropHeapRecords(heap);   // made again where one was (its memory was reused without a destroy)
            return;
        }
    }
    if (sNHeaps < HC_MAX_HEAPS) {
        sHeaps[sNHeaps++] = heap;
    }
}

void PORT_HeapCheckDestroyed(NNSFndHeapHandle heap) {
    Unregister(heap);
}

void PORT_HeapCheckReset(void) {
    size_t i;

    PORT_HeapCheckLock();
    for (i = 0; i < HC_BUCKETS; i++) {
        while (sTab[i] != NULL) {
            Rec *r = sTab[i];

            sTab[i] = r->next;
            free(r);
        }
    }
    sNRec = 0;
    sNHeaps = 0;
    sHadFrame = 0;
    memset(sFreed, 0, sizeof sFreed);
    PORT_HeapCheckUnlock();
}

// CPU time of this thread / the process, ns (0 where there is no such clock)
static unsigned long long CpuNs(int process) {
#if defined(__linux__) || defined(__APPLE__)
    struct timespec ts;

    if (clock_gettime(process ? CLOCK_PROCESS_CPUTIME_ID : CLOCK_THREAD_CPUTIME_ID, &ts) == 0) {
        return (unsigned long long)ts.tv_sec * 1000000000ull + (unsigned long long)ts.tv_nsec;
    }
#else
    (void)process;
#endif
    return 0;
}

// ---- the report

void PORT_HeapCheckAnchor(void) {}   // (its runtime address and nm's give the exe's load offset, for the ips)

static u32 Frame(void) {
    return OS_GetVBlankCount();
}

static int InHeap(const NNSiFndHeapHead *h, const void *p) {
    return (uintptr_t)p >= (uintptr_t)h->heapStart && (uintptr_t)p + HC_HDR <= (uintptr_t)h->heapEnd && ((uintptr_t)p & 3) == 0;
}

static void HeapName(NNSFndHeapHandle heap, char *buf, size_t n) {
    int id = sNamer != NULL ? sNamer(heap) : -1, i;

    for (i = 0; i < sNHeaps && sHeaps[i] != heap; i++) {
    }
    if (id >= 0) {
        snprintf(buf, n, "%d", id);
    } else {
        snprintf(buf, n, "#%d", i < sNHeaps ? i : -1);
    }
}

static void Hex(const char *what, const void *p, size_t n) {
    const u8 *b = p;
    size_t i;

    fprintf(stderr, "heapcheck:   %s %p:", what, p);
    for (i = 0; i < n; i++) {
        fprintf(stderr, "%s%02x", i % 8 == 0 ? " " : "", b[i]);
    }
    fputc('\n', stderr);
}

// The recorded live block just below addr in heap (the one a write past its end reaches addr from)
static Rec *BlockBefore(NNSFndHeapHandle heap, uintptr_t addr) {
    Rec *best = NULL, *r;
    size_t i;

    for (i = 0; i < HC_BUCKETS; i++) {
        for (r = sTab[i]; r != NULL; r = r->next) {
            if (r->heap == heap && (uintptr_t)r->data < addr && (best == NULL || r->data > best->data)) {
                best = r;
            }
        }
    }
    return best;
}

static void PrintRec(const char *what, Rec *r, uintptr_t bad) {
    int i;
    const NNSiFndExpHeapMBlockHead *h = (const NNSiFndExpHeapMBlockHead *)((u8 *)r->data - HC_HDR);

    fprintf(stderr, "heapcheck:   %s: data=%p asked=0x%x header_size=0x%x (its end is %+ld bytes from the bad block) allocated frame %u by",
            what, r->data, (unsigned)r->size, (unsigned)h->blockSize,
            (long)((long)((uintptr_t)r->data + h->blockSize) - (long)bad), (unsigned)r->frame);
    for (i = 0; i < r->nips; i++) {
        fprintf(stderr, " ip=%p", r->ips[i]);
    }
    fputc('\n', stderr);
}

// bad: the block whose header is wrong (NULL when it is not one: the heap's head), before: the block ahead of it in its list
static void Fail(NNSFndHeapHandle heap, const char *kind, const char *list, int index, NNSiFndExpHeapMBlockHead *bad,
                 NNSiFndExpHeapMBlockHead *before) {
    char name[24];
    Rec *own = NULL, *self = NULL;
    uintptr_t at = (uintptr_t)bad;
    int i;

    HeapName(heap, name, sizeof name);
    if (bad != NULL && !InHeap(heap, bad)) {
        bad = NULL;
    }
    if (bad != NULL) {
        self = Find((u8 *)bad + HC_HDR);
        own = BlockBefore(heap, at);
        if (own == NULL && before != NULL && InHeap(heap, before)) {
            own = Find((u8 *)before + HC_HDR);
        }
    }
    fprintf(stderr, "heapcheck: kind=%s frame=%u last_clean_frame=%d heap=%s handle=%p list=%s entry=%d block=%p", kind,
            (unsigned)Frame(), sHadFrame ? (int)sLastCleanFrame : -1, name, (void *)heap, list, index, (void *)bad);
    if (own != NULL) {
        fprintf(stderr, " owner=%p owner_asked=0x%x owner_frame=%u", own->data, (unsigned)own->size, (unsigned)own->frame);
        for (i = 0; i < own->nips; i++) {
            fprintf(stderr, " ip=%p", own->ips[i]);
        }
    }
    fprintf(stderr, " anchor=%p\n", (void *)&PORT_HeapCheckAnchor);
    if (bad != NULL) {
        fprintf(stderr, "heapcheck:   bad block %p: sig 0x%04x attr 0x%04x size 0x%x prev %p next %p\n", (void *)bad,
                (unsigned)bad->signature, (unsigned)bad->attribute, (unsigned)bad->blockSize, (void *)bad->pMBHeadPrev,
                (void *)bad->pMBHeadNext);
        Hex("bad block header", bad, HC_HDR);
        if (own != NULL) {
            const u8 *tail = (const u8 *)own->data + own->size;

            PrintRec("block before it (live, the last allocated below it)", own, at);
            if ((uintptr_t)tail < at && at - (uintptr_t)tail < 256) {
                Hex("bytes between its asked end and the bad block", tail, at - (uintptr_t)tail);
            }
        }
        if (self != NULL && self != own) {
            PrintRec("the bad block's own record (a write below its start?)", self, at);
        }
    }
    {
        NNSiFndHeapHead *h = heap;

        fprintf(stderr, "heapcheck:   heap %s: start %p end %p (0x%lx bytes), %d heaps and %llu blocks recorded, %llu checks\n", name,
                h->heapStart, h->heapEnd, (long)((u8 *)h->heapEnd - (u8 *)h->heapStart), sNHeaps, (unsigned long long)sNRec, sChecks);
    }
    fflush(NULL);
    abort();
}

// ---- the walk

static int RegionCmp(const void *a, const void *b) {
    uintptr_t x = ((const Region *)a)->start, y = ((const Region *)b)->start;

    return x < y ? -1 : x > y;
}

// One heap: both lists, and with tile also the blocks tiling the heap
static void CheckHeap(NNSFndHeapHandle heap, int tile) {
    NNSiFndHeapHead *h = heap;
    NNSiFndExpHeapHead *eh = (NNSiFndExpHeapHead *)((u8 *)h + sizeof(NNSiFndHeapHead));
    NNSiFndExpMBlockList *lists[2] = {&eh->mbFreeList, &eh->mbUsedList};
    const char *names[2] = {"free", "used"};
    size_t limit, n = 0, k;
    int l;

    sChecks++;
    if (h->signature != HC_SIG_HEAP) {
        Fail(heap, "heap-head-signature", "head", 0, NULL, NULL);
    }
    if ((uintptr_t)h->heapStart >= (uintptr_t)h->heapEnd) {
        Fail(heap, "heap-bounds", "head", 0, NULL, NULL);
    }
    limit = ((uintptr_t)h->heapEnd - (uintptr_t)h->heapStart) / HC_MIN_BLOCK + 2;
    for (l = 0; l < 2; l++) {
        NNSiFndExpHeapMBlockHead *b, *prev = NULL;
        size_t count = 0;
        const u16 sig = l == 0 ? HC_SIG_FREE : HC_SIG_USED;

        for (b = lists[l]->head; b != NULL; prev = b, b = b->pMBHeadNext) {
            uintptr_t end, start;

            if (++count > limit) {
                Fail(heap, "list-cycle-or-too-long", names[l], (int)count, prev, prev);
            }
            if (!InHeap(h, b)) {
                Fail(heap, "next-pointer-outside-heap", names[l], (int)count, prev, prev);   // (the pointer is prev's: its header was hit)
            }
            if (b->signature != sig) {
                Fail(heap, "block-signature", names[l], (int)count, b, prev);
            }
            if (b->pMBHeadPrev != prev) {
                Fail(heap, "prev-link", names[l], (int)count, b, prev);
            }
            end = (uintptr_t)b + HC_HDR + (uintptr_t)b->blockSize;
            if (end > (uintptr_t)h->heapEnd || end < (uintptr_t)b) {
                Fail(heap, "block-size", names[l], (int)count, b, prev);
            }
            start = (uintptr_t)b - ((b->attribute >> 8) & 0x7F);
            if (start < (uintptr_t)h->heapStart) {
                Fail(heap, "block-pad", names[l], (int)count, b, prev);
            }
            if (l == 0 && prev != NULL && (uintptr_t)b <= (uintptr_t)prev) {
                Fail(heap, "free-list-order", names[l], (int)count, b, prev);
            }
            if (tile) {
                if (n == sRegionsCap) {
                    Region *grown = realloc(sRegions, (sRegionsCap ? sRegionsCap * 2 : 4096) * sizeof *sRegions);

                    if (grown == NULL) {
                        tile = 0;
                    } else {
                        sRegions = grown;
                        sRegionsCap = sRegionsCap ? sRegionsCap * 2 : 4096;
                    }
                }
                if (tile) {
                    sRegions[n].start = start;
                    sRegions[n].end = end;
                    sRegions[n].block = b;
                    sRegions[n].used = l;
                    n++;
                }
            }
        }
        sBlocksWalked += count;
        if (lists[l]->tail != prev) {
            Fail(heap, "list-tail", names[l], (int)count, prev, prev);
        }
    }
    if (tile && n > 0) {
        qsort(sRegions, n, sizeof *sRegions, RegionCmp);
        if (sRegions[0].start != (uintptr_t)h->heapStart) {
            Fail(heap, "heap-start-not-covered", "both", 0, sRegions[0].block, NULL);
        }
        for (k = 0; k + 1 < n; k++) {
            if (sRegions[k].end != sRegions[k + 1].start) {
                Fail(heap, sRegions[k].end > sRegions[k + 1].start ? "blocks-overlap" : "blocks-gap", "both", (int)k,
                     sRegions[k + 1].block, sRegions[k].block);
            }
        }
        if (sRegions[n - 1].end != (uintptr_t)h->heapEnd) {
            Fail(heap, "heap-end-not-covered", "both", (int)n, sRegions[n - 1].block, sRegions[n - 1].block);
        }
    }
}

static void CheckAll(int tile) {
    int i;

    for (i = 0; i < sNHeaps; i++) {
        CheckHeap(sHeaps[i], tile);
    }
}

// ---- the wrappers' hooks (the caller holds the lock)

// Before the heap acts on its lists: what was written since its last operation is what this finds
static void CheckBefore(NNSFndHeapHandle heap) {
    if (PORT_HeapCheckLevel >= 2) {
        CheckAll(0);
    } else {
        CheckHeap(heap, 0);
    }
}

void PORT_HeapCheckBefore(NNSFndHeapHandle heap) {
    unsigned long long t0 = CpuNs(0);

    CheckBefore(heap);
    sCpuNs += CpuNs(0) - t0;
}

void PORT_HeapCheckAllocated(NNSFndHeapHandle heap, void *data, u32 size, void *caller) {
    Rec *r;
    size_t b;
    unsigned long long t0 = CpuNs(0);

    if (data == NULL) {
        return;
    }
    r = calloc(1, sizeof *r);
    if (r != NULL) {
        void *ips[HC_NIPS + 2];
        int n = 0, i;

        r->data = data;
        r->heap = heap;
        r->size = size;
        r->frame = Frame();
#ifdef HC_BACKTRACE
        n = backtrace(ips, HC_NIPS + 2);
        (void)caller;
#else
        ips[n++] = NULL;
        ips[n++] = NULL;
        ips[n++] = caller;   // (the wrapper's own return address: the one caller there is without a backtrace)
        (void)caller;
#endif
        for (i = 2; i < n && r->nips < HC_NIPS; i++) {   // (not this function, not the wrapper)
            r->ips[r->nips++] = ips[i];
        }
        Drop(data);   // (a stale record: the heap was made again over it)
        if (FreedSlot(data)->data == data) {
            FreedSlot(data)->data = NULL;   // (made again: a free of it now is a first free)
        }
        b = Hash(data);
        r->next = sTab[b];
        sTab[b] = r;
        sNRec++;
    }
    sCpuNs += CpuNs(0) - t0;
}

// Before the real free: a block that is not live (freed twice, a wild pointer) is said before it corrupts the lists
static void PrintIps(const char *what, void *const *ips, int n) {
    int i;

    fprintf(stderr, "%s", what);
    for (i = 0; i < n; i++) {
        fprintf(stderr, " ip=%p", ips[i]);
    }
}

void PORT_HeapCheckBeforeFree(NNSFndHeapHandle heap, void *data, void *caller) {
    NNSiFndExpHeapMBlockHead *b = (NNSiFndExpHeapMBlockHead *)((u8 *)data - HC_HDR);
    int i, n = 0, k;
    void *ips[HC_NIPS + 2];
    Freed *fr;
    unsigned long long t0 = CpuNs(0);

    CheckBefore(heap);
    if (!InHeap(heap, b) || b->signature != HC_SIG_USED) {
        Fail(heap, "free-of-a-block-that-is-not-used", "used", 0, b, NULL);
    }
#ifdef HC_BACKTRACE
    n = backtrace(ips, HC_NIPS + 2);
    (void)caller;
#else
    ips[n++] = NULL;
    ips[n++] = NULL;
    ips[n++] = caller;
#endif
    fr = FreedSlot(data);
    if (Find(data) == NULL && fr->data == data && fr->heap == heap) {
        char name[24];

        HeapName(heap, name, sizeof name);
        fprintf(stderr, "heapcheck: kind=free-of-a-block-freed-before frame=%u last_clean_frame=%d heap=%s handle=%p list=used entry=0 block=%p",
                (unsigned)Frame(), sHadFrame ? (int)sLastCleanFrame : -1, name, (void *)heap, (void *)b);
        PrintIps("", ips + 2, n > 2 ? n - 2 : 0);
        fprintf(stderr, " anchor=%p\n", (void *)&PORT_HeapCheckAnchor);
        PrintIps("heapcheck:   this free's callers:", ips + 2, n > 2 ? n - 2 : 0);
        fputc('\n', stderr);
        fprintf(stderr, "heapcheck:   the first free of the same block %p was at frame %u by", data, (unsigned)fr->frame);
        PrintIps("", fr->ips, fr->nips);
        fputc('\n', stderr);
        fflush(NULL);
        abort();
    }
    fr->data = data;
    fr->heap = heap;
    fr->frame = Frame();
    fr->nips = 0;
    for (k = 2; k < n && fr->nips < HC_NIPS; k++) {   // (not this function, not the wrapper)
        fr->ips[fr->nips++] = ips[k];
    }
    // a heap inside the block (the game's child heaps: the block is the child's memory) is gone with it
    for (i = 0; i < sNHeaps;) {
        if ((uintptr_t)sHeaps[i] >= (uintptr_t)data && (uintptr_t)sHeaps[i] < (uintptr_t)data + b->blockSize) {
            NNSFndHeapHandle gone = sHeaps[i];

            sHeaps[i] = sHeaps[--sNHeaps];
            DropHeapRecords(gone);
        } else {
            i++;
        }
    }
    Drop(data);
    sCpuNs += CpuNs(0) - t0;
}

void PORT_HeapCheckResized(NNSFndHeapHandle heap, void *data, u32 size, u32 got) {
    Rec *r = Find(data);

    (void)heap;
    if (r != NULL && got != 0) {
        r->size = size;
    }
}

// Once per V-blank wait: every heap, the tiling too
void PORT_HeapCheckFrame(void) {
    if (PORT_HeapCheckLevel <= 0) {
        return;
    }
    unsigned long long t0;

    PORT_HeapCheckLock();
    t0 = CpuNs(0);
    CheckAll(1);
    sCpuNs += CpuNs(0) - t0;
    sLastCleanFrame = Frame();
    if (!sHadFrame || sLastCleanFrame - sStatsFrame >= 4096) {   // (a sign of life in the log: what was checked so far)
        unsigned long long proc = CpuNs(1);

        sStatsFrame = sLastCleanFrame;
        fprintf(stderr,
                "heapcheck: clean at frame %u: %d heaps, %llu live blocks recorded, %llu heap checks, %llu list entries walked; "
                "checker cpu %llu ms of the process's %llu ms\n",
                (unsigned)sLastCleanFrame, sNHeaps, (unsigned long long)sNRec, sChecks, sBlocksWalked, sCpuNs / 1000000ull,
                proc / 1000000ull);
    }
    sHadFrame = 1;
    PORT_HeapCheckUnlock();
}
#endif
