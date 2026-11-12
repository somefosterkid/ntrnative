// The in-process restart (docs/launcher.md, section 1): the game's OS_ResetSystem comes back here and the machine, the
// game's C, lib/nnsys (our NitroSystem), libntr's SDK libraries and the emulated DS (sim_vars.c), is put back as it was before the game
// first started; then the game starts again, in the same process. The host stays up: the window, GL/Metal, ImGui, the
// audio device, input, the settings, the renderers' threads.
//
// The machine's writable data is in sections of its own on Apple builds (port/machine/macos_machine_section.h for the
// game's C and lib/nnsys's; libntr-ios-reset.patch for libntr's; the overlays' own two, ovl.c), plus
// the few machine globals libntr keeps in its C++ (SIM_MachineExtra). They are copied once, at the first pass of the run
// loop below (libntr calls SIM_RunGame where it called NitroMain: every host object libntr makes before the game exists
// then and keeps its address), and put back at a reset: the initialized data in full, the zero-fill memory (the DS's
// 57 MB, libntr's 110 MB mixer) by page, only the pages in use (mincore), so memory never touched stays untouched.
//
// PORT_MachineReset (reset.c, the main thread with the game lock held): the game's other threads parked (main_threads.c),
// the PXI messages handled and the ARM7's alarm threads stopped (SIM_PxiQuiesce), then a longjmp out of the game's frames
// (C only: OS_ResetSystem's callers are the main loop's C, nothing of C++ between NitroMain and them). Back in the loop,
// with the sound callback held off: the host files the game's FSFiles held closed, NitroSystem's 2D unpack list freed,
// the ranges put back, acore made again, the overlay loader's and the thread table's state cleared, and libntr's
// renderers give back what they hold at the next frame (the next frame is drawn as the first).
//
// BWPORT_MACHINE_CHECK=FILE: one line per V-blank wait of the game's main loop, the V-blank count and a hash of each
// machine range (only its pages in use), from power-on and again after each reset: two boots in one process compare line
// by line (tools/mac_reset_check.py). Linux, Windows and Android have no sections yet (a linker script will gather the
// same objects): there SIM_RunGame runs the game once and reset.c starts the program again.
#include <nitro.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"

#include "port_game.h"

#include <fcntl.h>
#include <time.h>
#ifdef __APPLE__
#include <mach/mach.h>
#include <malloc/malloc.h>
#include <sys/mman.h>
#include <unistd.h>

extern u8 sMachData0 __asm("section$start$__DATA$__machdata");
extern u8 sMachData1 __asm("section$end$__DATA$__machdata");
extern u8 sMachBss0 __asm("section$start$__DATA$__machbss");
extern u8 sMachBss1 __asm("section$end$__DATA$__machbss");
extern u8 sOvlData0 __asm("section$start$__DATA$__ovldata");
extern u8 sOvlData1 __asm("section$end$__DATA$__ovldata");
extern u8 sOvlBss0 __asm("section$start$__DATA$__ovlbss");
extern u8 sOvlBss1 __asm("section$end$__DATA$__ovlbss");
#else
// Linux, Android, Windows: the linker script tools/port_overlays.py writes gathers the machine's objects' data and bss
// into .machdata and .machbss, after the overlays' two, and defines these symbols
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif
extern u8 __bwmach_data0[], __bwmach_data1[], __bwmach_bss0[], __bwmach_bss1[];
extern u8 __bwovl_data[], __bwovl_data_end[], __bwovl_bss[], __bwovl_bss_end[];
#define sMachData0 (*__bwmach_data0)
#define sMachData1 (*__bwmach_data1)
#define sMachBss0 (*__bwmach_bss0)
#define sMachBss1 (*__bwmach_bss1)
#define sOvlData0 (*__bwovl_data)
#define sOvlData1 (*__bwovl_data_end)
#define sOvlBss0 (*__bwovl_bss)
#define sOvlBss1 (*__bwovl_bss_end)
#endif

int SIM_MachineExtra(void **addr, size_t *size, int max);   // libntr-ios-reset.patch
void SIM_ResetHost(void);
void SIM_PxiQuiesce(void);
void SIM_OsStopThreads(void);
void SIM_OsMarkSnapshot(void);
int FS_PortCloseAll(void);
void WIN_G2dResetUnpacked(void);   // port/src/nnsys_load64.c: the 2D loaders' host copies
void PORT_NnsReset(void);          // port/src/nnsys_g3stage.c: the geometry commands held back
void PORT_ThreadsPrestart(void);   // main_threads.c
void PORT_ThreadsPark(void (*unlocked)(void));
void PORT_ThreadsReset(void);
void PortOvl_Reset(void);          // ovl.c
void PORT_BenchReset(void);        // bench_pin.c
void PORT_KeysReset(void);         // keys_script.c
void ab_lock(void);                // port/bridge/acore_bridge.c
void ab_unlock(void);
void acglue_reset(void);           // acore_glue.c (ab_lock held)

#define EXTRA_MAX 16
#define FULL_COPY_MAX (1u << 20)   // a range up to this size is copied whole; above it, it is zero-fill: by page

typedef struct Range {
    const char *name;
    u8 *start;
    size_t size;
    BOOL zero;      // zero-fill: only the pages in use are kept and put back
    u8 *copy;       // full: the bytes; zero: the kept pages' bytes, one page each
    size_t *pages;  // zero: the kept pages' indices
    size_t nPages;
} Range;

static Range sRanges[4 + EXTRA_MAX];
static int sNRanges;
static size_t sPage;
static jmp_buf sResetJmp;
static int sResets;
static FILE *sCheck;
static unsigned sCheckEvery = 1;   // BWPORT_MACHINE_CHECK_EVERY=N: a line every N V-blanks
static const char *sDumpAt;        // BWPORT_MACHINE_DUMP=V,V,...: each range's bytes at V-blank V of each boot (check file's dir)
static char sDumpDir[1024];

static void AddRange(const char *name, u8 *start, u8 *end, BOOL zero) {
    Range *r = &sRanges[sNRanges++];

    r->name = name;
    r->start = start;
    r->size = (size_t)(end - start);
    r->zero = zero && r->size > FULL_COPY_MAX;
}

// The pages of a zero-fill range that the process has touched (resident, or paged out or compressed since).
// Windows has no mincore: a page that is not in the working set may have been trimmed after the game wrote it, and
// nothing tells that from a page never touched, so every page counts (the caller reads it: 58 MB, a few ms)
static unsigned char *InUse(const Range *r, size_t *first, size_t *n) {
    uintptr_t lo = (uintptr_t)r->start & ~(uintptr_t)(sPage - 1);
    uintptr_t hi = ((uintptr_t)r->start + r->size + sPage - 1) & ~(uintptr_t)(sPage - 1);
    unsigned char *vec;

    *first = lo;
    *n = (hi - lo) / sPage;
    vec = calloc(*n ? *n : 1, 1);
#ifdef _WIN32
    if (vec != NULL) {
        memset(vec, 1, *n);
    }
#else
    if (vec != NULL && mincore((void *)lo, hi - lo, (void *)vec) != 0) {
        memset(vec, 1, *n);   // unknown: every page counts as touched
    }
#endif
    return vec;
}

// The bytes of page i of the aligned span that belong to the range
static void PageSpan(const Range *r, uintptr_t first, size_t i, u8 **p, size_t *len) {
    uintptr_t a = first + i * sPage, b = a + sPage;
    uintptr_t s = (uintptr_t)r->start, e = s + r->size;

    if (a < s) a = s;
    if (b > e) b = e;
    *p = (u8 *)a;
    *len = b > a ? b - a : 0;
}

static BOOL AllZero(const u8 *p, size_t n) {
    size_t i = 0;
    u64 acc = 0;

    for (; i + 8 <= n; i += 8) {
        u64 w;
        memcpy(&w, p + i, 8);
        acc |= w;
    }
    for (; i < n; i++) {
        acc |= p[i];
    }
    return acc == 0;
}

static void Snapshot(void) {
    void *xa[EXTRA_MAX];
    size_t xz[EXTRA_MAX];
    int nx, i;
    size_t kept = 0;

    SIM_OsMarkSnapshot();   // libntr's threads made from now on are the machine's: ended at a reset
#ifdef _WIN32
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        sPage = si.dwPageSize;
    }
#else
    sPage = (size_t)getpagesize();
#endif
    AddRange("machdata", &sMachData0, &sMachData1, FALSE);
    AddRange("machbss", &sMachBss0, &sMachBss1, TRUE);
    AddRange("ovldata", &sOvlData0, &sOvlData1, FALSE);
    AddRange("ovlbss", &sOvlBss0, &sOvlBss1, FALSE);
    nx = SIM_MachineExtra(xa, xz, EXTRA_MAX);
    for (i = 0; i < nx && i < EXTRA_MAX; i++) {
        static const char *const kNames[EXTRA_MAX] = {"sim0", "sim1", "sim2", "sim3", "sim4", "sim5", "sim6", "sim7",
                                                      "sim8", "sim9", "sim10", "sim11", "sim12", "sim13", "sim14", "sim15"};
        AddRange(kNames[i], (u8 *)xa[i], (u8 *)xa[i] + xz[i], TRUE);
    }
    for (i = 0; i < sNRanges; i++) {
        Range *r = &sRanges[i];

        if (!r->zero) {
            r->copy = malloc(r->size ? r->size : 1);
            memcpy(r->copy, r->start, r->size);
            kept += r->size;
        } else {
            size_t first, n, j;
            unsigned char *vec = InUse(r, &first, &n);

            r->pages = malloc((n ? n : 1) * sizeof(size_t));
            r->copy = malloc(sPage);
            for (j = 0; j < n; j++) {
                u8 *p;
                size_t len;

                PageSpan(r, first, j, &p, &len);
                if (vec[j] != 0 && len != 0 && !AllZero(p, len)) {
                    r->copy = realloc(r->copy, (r->nPages + 1) * sPage);
                    memcpy(r->copy + r->nPages * sPage, p, len);
                    r->pages[r->nPages++] = j;
                }
            }
            kept += r->nPages * sPage;
            free(vec);
        }
    }
    for (i = 0; i < sNRanges; i++) {
        printf("machine: range %s at %p, %zu bytes\n", sRanges[i].name, (void *)sRanges[i].start, sRanges[i].size);
    }
    printf("machine: %d ranges, %zu KB data + %zu KB zero-fill, %zu KB kept\n", sNRanges,
           (sRanges[0].size + sRanges[2].size + sRanges[3].size) / 1024, sRanges[1].size / 1024, kept / 1024);
}

// asset_load.c: a table filled from the ROM after the snapshot (a game chosen after the first): into the kept copies too
void PORT_MachineKeep(const void *p, size_t n) {
    const u8 *b = p;
    int i;

    for (i = 0; i < sNRanges; i++) {
        Range *r = &sRanges[i];

        if (!r->zero && r->copy != NULL && b >= r->start && b + n <= r->start + r->size) {
            memcpy(r->copy + (b - r->start), b, n);
        }
    }
}

static void Restore(void) {
    int i;

    for (i = 0; i < sNRanges; i++) {
        Range *r = &sRanges[i];

        if (!r->zero) {
            memcpy(r->start, r->copy, r->size);
        } else {
            size_t first, n, j, k = 0;
            unsigned char *vec = InUse(r, &first, &n);

            for (j = 0; j < n; j++) {
                u8 *p;
                size_t len;

                PageSpan(r, first, j, &p, &len);
                if (k < r->nPages && r->pages[k] == j) {
                    memcpy(p, r->copy + k * sPage, len);
                    k++;
                } else if (vec[j] != 0 && len != 0) {
                    memset(p, 0, len);
                }
            }
            free(vec);
        }
    }
}

// A hash of bytes, 8 at a time (FNV-1a's mix on words; the tail by byte)
static u64 HashBytes(u64 h, const u8 *p, size_t n) {
    size_t i = 0;

    for (; i + 8 <= n; i += 8) {
        u64 w;
        memcpy(&w, p + i, 8);
        h = (h ^ w) * 0x100000001b3ULL;
    }
    for (; i < n; i++) {
        h = (h ^ p[i]) * 0x100000001b3ULL;
    }
    return h;
}

// A range's hash; a zero-fill range's pages that are zero (or untouched) are skipped
static u64 Hash(const Range *r) {
    u64 h = 0xcbf29ce484222325ULL;
    size_t first, n, j;
    unsigned char *vec;

    if (!r->zero) {
        return HashBytes(h, r->start, r->size);
    }
    vec = InUse(r, &first, &n);
    for (j = 0; j < n; j++) {
        u8 *p;
        size_t len;

        PageSpan(r, first, j, &p, &len);
        if (vec[j] != 0 && !AllZero(p, len)) {
            h = (h ^ j) * 0x100000001b3ULL;
            h = HashBytes(h, p, len);
        }
    }
    free(vec);
    return h;
}

// main_threads.c's PORT_WaitIrq, at each V-blank wait (the game lock held)
static void Dump(unsigned vb) {
    int i;

    for (i = 0; i < sNRanges; i++) {
        char path[1100];
        FILE *f;

        if (sRanges[i].size > (64u << 20)) {
            continue;
        }
        snprintf(path, sizeof(path), "%s/dump%d_%u_%s.bin", sDumpDir, sResets, vb, sRanges[i].name);
        f = fopen(path, "wb");
        if (f != NULL) {
            fwrite(sRanges[i].start, 1, sRanges[i].size, f);
            fclose(f);
        }
    }
}

void PORT_MachineFrame(void) {
    int i;

    if (sDumpAt != NULL) {
        const char *p = sDumpAt;
        static u32 sLast = 0xffffffffu;
        u32 vb = OS_GetVBlankCount();

        while (*p && vb != sLast) {
            if ((u32)atol(p) == vb) {
                sLast = vb;
                Dump(vb);
                break;
            }
            p += strcspn(p, ",");
            p += *p == ',';
        }
    }
    if (sCheck == NULL || OS_GetVBlankCount() % sCheckEvery != 0) {
        return;
    }
    fprintf(sCheck, "%d %u", sResets, (unsigned)OS_GetVBlankCount());
    for (i = 0; i < sNRanges; i++) {
        fprintf(sCheck, " %s:%016llx", sRanges[i].name, (unsigned long long)Hash(&sRanges[i]));
    }
    fputc('\n', sCheck);
}

// With the game lock let go: the messages between the two CPUs handled, the ARM7's alarm threads and the SDK's own
// threads of this boot ended
static void Quiesce(void) {
    SIM_PxiQuiesce();
    SIM_OsStopThreads();
}

void PORT_MachineReset(void) {
    printf("machine: reset %d: parking the game's threads\n", sResets + 1);
    fflush(stdout);
    PORT_ThreadsPark(Quiesce);   // the lock let go meanwhile (PXI drained then), held again (depth 1) on return
    longjmp(sResetJmp, 1);
}

// KB of the machine's zero-fill pages in use now
static size_t InUseKB(void) {
    size_t kb = 0, first, n, j;
    int i;

    for (i = 0; i < sNRanges; i++) {
        unsigned char *vec;

        if (!sRanges[i].zero) {
            continue;
        }
        vec = InUse(&sRanges[i], &first, &n);
        for (j = 0; j < n; j++) {
            kb += vec[j] != 0 ? sPage / 1024 : 0;
        }
        free(vec);
    }
    return kb;
}

// The process: resident MB, threads, host heap in use (MB): a reset must leave them flat
#if defined(__APPLE__)
static void ProcessStats(double *rssMB, int *threads, double *heapMB) {
    mach_task_basic_info_data_t info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    thread_act_array_t list;
    mach_msg_type_number_t n = 0;
    malloc_statistics_t ms;

    *rssMB = 0;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS) {
        *rssMB = (double)info.resident_size / 1048576.0;
    }
    *threads = 0;
    if (task_threads(mach_task_self(), &list, &n) == KERN_SUCCESS) {
        unsigned i;
        for (i = 0; i < n; i++) {
            mach_port_deallocate(mach_task_self(), list[i]);
        }
        vm_deallocate(mach_task_self(), (vm_address_t)list, n * sizeof(list[0]));
        *threads = (int)n;
    }
    malloc_zone_statistics(NULL, &ms);
    *heapMB = (double)ms.size_in_use / 1048576.0;
}

static int OpenFiles(void) {
    int fd, n = 0;
    for (fd = 0; fd < 1024; fd++) {
        if (fcntl(fd, F_GETFD) != -1) {
            n++;
        }
    }
    return n;
}
#elif defined(_WIN32)
#include <tlhelp32.h>
static void ProcessStats(double *rssMB, int *threads, double *heapMB) {
    typedef struct {
        DWORD cb, PageFaultCount;
        SIZE_T PeakWorkingSetSize, WorkingSetSize, a, b, c, d, PagefileUsage, PeakPagefileUsage;
    } Counters;
    BOOL (WINAPI *get)(HANDLE, Counters *, DWORD) =
        (BOOL (WINAPI *)(HANDLE, Counters *, DWORD))(void *)GetProcAddress(GetModuleHandleA("kernel32.dll"), "K32GetProcessMemoryInfo");
    Counters c = {sizeof(c)};
    HANDLE snap;

    *rssMB = *heapMB = 0;
    if (get != NULL && get(GetCurrentProcess(), &c, sizeof(c))) {
        *rssMB = (double)c.WorkingSetSize / 1048576.0;
        *heapMB = (double)c.PagefileUsage / 1048576.0;   // (the private commit)
    }
    *threads = 0;
    snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snap != INVALID_HANDLE_VALUE) {
        THREADENTRY32 te = {sizeof(te)};
        BOOL ok = Thread32First(snap, &te);
        for (; ok; ok = Thread32Next(snap, &te)) {
            *threads += te.th32OwnerProcessID == GetCurrentProcessId();
        }
        CloseHandle(snap);
    }
}

static int OpenFiles(void) {
    DWORD n = 0;
    GetProcessHandleCount(GetCurrentProcess(), &n);
    return (int)n;
}
#else
#include <malloc.h>
static void ProcessStats(double *rssMB, int *threads, double *heapMB) {
    FILE *f = fopen("/proc/self/statm", "r");
    long pages = 0, res = 0;
    char line[256];

    *rssMB = *heapMB = 0;
    *threads = 0;
    if (f != NULL) {
        if (fscanf(f, "%ld %ld", &pages, &res) == 2) {
            *rssMB = (double)res * (double)sPage / 1048576.0;
        }
        fclose(f);
    }
    f = fopen("/proc/self/status", "r");
    while (f != NULL && fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "Threads: %d", threads) == 1) {
            break;
        }
    }
    if (f != NULL) {
        fclose(f);
    }
#if defined(__GLIBC__)
#if __GLIBC_PREREQ(2, 33)
    {
        struct mallinfo2 mi = mallinfo2();
        *heapMB = (double)(mi.uordblks + mi.hblkhd) / 1048576.0;
    }
#endif
#endif
}

static int OpenFiles(void) {
    int fd, n = 0;
    for (fd = 0; fd < 1024; fd++) {
        if (fcntl(fd, F_GETFD) != -1) {
            n++;
        }
    }
    return n;
}
#endif

static void ResetMachine(void) {
    struct timespec t0, t1;
    int closed, threads;
    double rss, heap;

    printf("machine: %zu KB of the machine's zero-fill memory in use\n", InUseKB());
    clock_gettime(CLOCK_MONOTONIC, &t0);
    ab_lock();   // the sound callback out (it writes the game's sound work): acore's state is the machine's too
    closed = FS_PortCloseAll();
    WIN_G2dResetUnpacked();
    PORT_NnsReset();
    Restore();
    acglue_reset();
    ab_unlock();
    PORT_ThreadsReset();
    PortOvl_Reset();
    PORT_BenchReset();
    PORT_KeysReset();
    SIM_ResetHost();
    clock_gettime(CLOCK_MONOTONIC, &t1);
    sResets++;
    ProcessStats(&rss, &threads, &heap);
    printf("machine: reset %d done in %.2f ms (%d game files closed, %d descriptors open; process: %.1f MB resident, "
           "%d threads, %.1f MB heap)\n", sResets, (double)(t1.tv_sec - t0.tv_sec) * 1e3 +
           (double)(t1.tv_nsec - t0.tv_nsec) / 1e6, closed, OpenFiles(), rss, threads, heap);
    fflush(stdout);
}

int PORT_AssetsLoad(const char *code);   // asset_load.c
int PORT_LauncherWanted(void);   // main_start.c
void PORT_RunLauncher(void);      // ios_launcher.m on iOS; launcher_gui.cpp on the desktop; main_start.c on Android (none yet)
volatile int PORT_LauncherUp;     // while PORT_RunLauncher runs (no game: port/src/hang_watch.c holds its watch)

// The run loop's "no game chosen yet" point: the launcher (a start with no game or Start=launcher; on Apple builds also Game >
// Games... or the settings' Games row), which returns once Play has entered a game's working directory. libntr's SIM_main read the ROM header at start
// from the working directory of then: the chosen game's header.bin goes in its place (the game's boot reads it there).
static void ChooseGame(void) {
    FILE *f;

    if (!PORT_LauncherWanted()) {
        return;
    }
    PORT_LauncherUp = 1;
    PORT_RunLauncher();
    PORT_LauncherUp = 0;
    if ((f = fopen("header.bin", "rb")) != NULL) {
        if (fread((void *)HW_ROM_HEADER_BUF, 1, 0x160, f) != 0x160) {
            printf("machine: header.bin is short\n");
        }
        fclose(f);
    }
}

// libntr's SIM_main calls this where it called NitroMain (libntr-ios-reset.patch)
void SIM_RunGame(void) {
    const char *check = getenv("BWPORT_MACHINE_CHECK");

    ChooseGame();             // before the snapshot: a first start with no game chosen boots the one Play picks
    PORT_AssetsLoad(PORT_Game->code);   // its data tables from its ROM (asset_load.c), before anything reads them
    PORT_ThreadsPrestart();   // the game threads' host threads: made now, so the snapshot holds them in libntr's tables
    Snapshot();
    if (check != NULL && *check) {
        const char *every = getenv("BWPORT_MACHINE_CHECK_EVERY");

        const char *dump = getenv("BWPORT_MACHINE_DUMP");
        const char *slash = strrchr(check, '/');

        sCheck = fopen(check, "w");
        if (dump != NULL && *dump && slash != NULL && (size_t)(slash - check) < sizeof(sDumpDir)) {
            sDumpAt = dump;
            memcpy(sDumpDir, check, (size_t)(slash - check));
        }
        if (every != NULL && atoi(every) > 0) {
            sCheckEvery = (unsigned)atoi(every);
        }
    }
    if (setjmp(sResetJmp) != 0) {
        ResetMachine();
        ChooseGame();         // the settings' Games row: the reset was flagged "then the launcher"
        PORT_AssetsLoad(PORT_Game->code);   // a game not run before in this process: its tables (once per game)
    }
    PORT_Game->main();        // the chosen game's NitroMain (port/include/port_game.h)
}
