// System metrics for the Metrics window, the iOS overlay and the log: how much CPU the app uses (total, the game thread, the render
// workers, the audio thread), an estimate of the clock the game thread runs at, and the GPU's busy time. Throttling then shows as it
// happens (the clock estimate falls, the GPU ms rise, the heat state climbs), and a shared log keeps one line every 10 s.
//
// Cost: the frame hook (bwport_sys_frame, called once per frame by libntr) reads the clock and counts; once a second it runs the clock
// probe (about 30 us) and hands over the fence numbers; a sampler thread wakes once a second to read the CPU times (a few mach /
// getrusage calls) and every 10 s writes the log line. Nothing else per frame.
//
// What the numbers are (and are not):
//  - cores: the process's CPU time (user + system, every thread, dead ones included) over wall time. Exact.
//  - main / audio: the game thread (the one that calls the frame hook) and, on macOS and iOS, the SDL audio thread (found by its
//    name; the ARM7 sound driver runs inside it). other = cores - main - audio: the render worker threads (vcore starts its own every
//    frame, so they cannot be counted one by one), the driver's threads, the rest. Windows and Linux: main and other (other then
//    includes the audio thread); no audio line.
//  - clock: a chain of dependent integer adds on the game thread right after a frame, best of 3 runs of about 10 us: one add a cycle,
//    so adds / time = the clock that core runs at that moment. An estimate (a core that was idle may not have ramped up yet; the
//    system gives no public clock and no way to tell a P from an E core, so the core kind is not claimed). Only on arm64 and x86-64.
//  - gpu: GL timer queries (GL_TIME_ELAPSED, desktop GL) of a burst of frames each second; busy = ms per frame x frames a second.
//    Under Present=metal (Apple) every frame's presents' GPU time, GPUEndTime - GPUStartTime of their command buffers.
//    Where the context has no timer queries (GL ES: the iPhone) the figure is the fence latency instead (the swap call to the GPU
//    done, port/bridge's libntr-ios-lag.patch), which includes waiting behind earlier frames: an upper bound, labelled as one.
// Platform parity: Windows, Linux and macOS all get the CPU and clock lines; only the per-thread split is Apple's.
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <sys/resource.h>
#include <TargetConditionals.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#else
#include <sys/resource.h>
#endif

#include "sysmetrics.h"

int SIM_PresentMetal(void);   // libntr (libntr-ios-metal.patch): 1 = the window presents with Metal; its GPU times are the command buffers'

#if defined(__APPLE__)
// port/src/apple_thermal.m
const char *bwport_thermal_name(void) __attribute__((weak_import));
#elif defined(__ANDROID__)
const char *bwport_thermal_name(void);   // port/src/android_log.c (PowerManager's thermal status; NULL until Java told)
#endif

static bwport_sys_t gSys;                  // the published numbers (the sampler writes, the UI reads: a torn float is harmless)
static pthread_t gGameThread;
static int gGameSet;
static int gSamplerStarted;
static volatile int gGameKnown;

static double NowS(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec * 1e-9;
}

// ---- the CPU ---------------------------------------------------------------------------------------------------------------
static double ProcessCpuS(void) {
#if defined(_WIN32)
    FILETIME c, e, k, u;
    if (!GetProcessTimes(GetCurrentProcess(), &c, &e, &k, &u)) return -1.0;
    return (double)(((uint64_t)k.dwHighDateTime << 32 | k.dwLowDateTime) + ((uint64_t)u.dwHighDateTime << 32 | u.dwLowDateTime)) * 1e-7;
#else
    struct rusage r;
    if (getrusage(RUSAGE_SELF, &r) != 0) return -1.0;
    return (double)r.ru_utime.tv_sec + (double)r.ru_stime.tv_sec + (double)(r.ru_utime.tv_usec + r.ru_stime.tv_usec) * 1e-6;
#endif
}

#if defined(_WIN32)
static HANDLE gGameHandle;
#endif

// the game thread's own CPU time, -1 when unknown
static double GameCpuS(void) {
    if (!gGameSet) return -1.0;
#if defined(__APPLE__)
    return -1.0;   // from the thread list below
#elif defined(_WIN32)
    FILETIME c, e, k, u;
    if (gGameHandle == NULL || !GetThreadTimes(gGameHandle, &c, &e, &k, &u)) return -1.0;
    return (double)(((uint64_t)k.dwHighDateTime << 32 | k.dwLowDateTime) + ((uint64_t)u.dwHighDateTime << 32 | u.dwLowDateTime)) * 1e-7;
#else
    clockid_t id;
    struct timespec t;
    if (pthread_getcpuclockid(gGameThread, &id) != 0 || clock_gettime(id, &t) != 0) return -1.0;
    return (double)t.tv_sec + (double)t.tv_nsec * 1e-9;
#endif
}

#if defined(__APPLE__)
// Per-thread times by thread id; the class from the thread's name (main = the game thread, found by its pthread).
enum { kMaxThreads = 96 };
struct ThreadRec { uint64_t id; double cpu; int cls; char nm[32]; };
static struct ThreadRec gThr[kMaxThreads];
static int gThrN;

static double Time2S(time_value_t a, time_value_t b) {
    return (double)(a.seconds + b.seconds) + (double)(a.microseconds + b.microseconds) * 1e-6;
}

// the live threads now: id, CPU time, class, name ("game" for the game thread, "unnamed" for one without a name: GCD's
// workers, where Metal's completion handlers and CoreAnimation run). Any thread may call it.
static int Snap(struct ThreadRec *out, int max) {
    thread_act_array_t list;
    mach_msg_type_number_t n = 0;
    if (task_threads(mach_task_self(), &list, &n) != KERN_SUCCESS) return 0;
    uint64_t gameId = 0;
    if (gGameSet) pthread_threadid_np(gGameThread, &gameId);
    int nn = 0;
    for (mach_msg_type_number_t i = 0; i < n; i++) {
        thread_identifier_info_data_t idi;
        thread_basic_info_data_t bi;
        thread_extended_info_data_t ei;
        mach_msg_type_number_t c1 = THREAD_IDENTIFIER_INFO_COUNT, c2 = THREAD_BASIC_INFO_COUNT, c3 = THREAD_EXTENDED_INFO_COUNT;
        if (nn < max && thread_info(list[i], THREAD_IDENTIFIER_INFO, (thread_info_t)&idi, &c1) == KERN_SUCCESS &&
            thread_info(list[i], THREAD_BASIC_INFO, (thread_info_t)&bi, &c2) == KERN_SUCCESS) {
            struct ThreadRec *r = &out[nn++];
            const char *nm = "";
            if (thread_info(list[i], THREAD_EXTENDED_INFO, (thread_info_t)&ei, &c3) == KERN_SUCCESS) nm = ei.pth_name;
            r->id = idi.thread_id;
            r->cpu = Time2S(bi.user_time, bi.system_time);
            r->cls = 3;
            if (idi.thread_id == gameId && gameId != 0) {
                r->cls = 0;
                nm = "game";
            } else if (strcasestr(nm, "audio") != NULL || strcasestr(nm, "sound") != NULL || strcasestr(nm, "snd") != NULL) {
                r->cls = 2;
            } else if (strncmp(nm, "vcore", 5) == 0 || strncmp(nm, "gpu", 3) == 0 || strncmp(nm, "render", 6) == 0 ||
                       strncmp(nm, "bwr", 3) == 0 || strncmp(nm, "bwport-worker", 13) == 0) {
                r->cls = 1;
            }
            snprintf(r->nm, sizeof r->nm, "%s", nm[0] ? nm : "unnamed");
        }
        mach_port_deallocate(mach_task_self(), list[i]);
    }
    vm_deallocate(mach_task_self(), (vm_address_t)list, n * sizeof(thread_t));
    return nn;
}

// adds the CPU seconds by thread name between two snapshots to sums[0..*n) (a thread new in b counts whole)
struct NameSum { char nm[32]; double s; };
static void NameDeltas(const struct ThreadRec *a, int na, const struct ThreadRec *b, int nb, struct NameSum *sums, int *n) {
    for (int i = 0; i < nb; i++) {
        double prev = 0.0;
        for (int j = 0; j < na; j++) {
            if (a[j].id == b[i].id) { prev = a[j].cpu; break; }
        }
        double d = b[i].cpu - prev;
        if (d <= 0.0) continue;
        int k = 0;
        while (k < *n && strcmp(sums[k].nm, b[i].nm) != 0) k++;
        if (k == *n) {
            if (k == kMaxThreads) continue;
            memcpy(sums[k].nm, b[i].nm, sizeof sums[k].nm);
            sums[k].s = 0.0;
            (*n)++;
        }
        sums[k].s += d;
    }
}

// "game 1.23, bwport-worker 1.60, ..., rest 0.02": the largest 10 sums times `scale` (sorts v), the others as "rest"
static void NamesText(struct NameSum *v, int n, double scale, char *buf, int len) {
    for (int i = 1; i < n; i++) {   // insertion sort, largest first
        struct NameSum t = v[i];
        int j = i;
        for (; j > 0 && v[j - 1].s < t.s; j--) v[j] = v[j - 1];
        v[j] = t;
    }
    int k = 0;
    double rest = 0.0;
    buf[0] = 0;
    for (int i = 0; i < n; i++) {
        if (i < 10 && v[i].s * scale >= 0.005 && k < len - 48) k += snprintf(buf + k, len - k, "%s%s %.2f", k ? ", " : "", v[i].nm, v[i].s * scale);
        else rest += v[i].s * scale;
    }
    if (rest > 0.0 && k < len - 24) snprintf(buf + k, len - k, "%srest %.2f", k ? ", " : "", rest);
}

static struct NameSum gNameAcc[kMaxThreads];   // the sampler's sums by name since the last log line
static int gNameAccN;

// adds each class's CPU time since the last call to out[0..3] (main, other threads, audio, other), in seconds, and each name's
// to gNameAcc
static void ThreadDeltas(double out[4]) {
    out[0] = out[1] = out[2] = out[3] = 0.0;
    static int dump = -1;
    if (dump < 0) dump = getenv("BWPORT_SYSTHREADS") != NULL;
    struct ThreadRec nw[kMaxThreads];
    int nn = Snap(nw, kMaxThreads);
    if (nn == 0) return;
    for (int i = 0; i < nn; i++) {
        double prev = 0.0;
        for (int j = 0; j < gThrN; j++) {
            if (gThr[j].id == nw[i].id) { prev = gThr[j].cpu; break; }
        }
        double d = nw[i].cpu - prev;
        if (d > 0.0) out[nw[i].cls] += d;
        if (dump && d > 0.01) fprintf(stderr, "sysmetrics: thread %llu \"%s\" class %d +%.3f s\n", (unsigned long long)nw[i].id, nw[i].nm, nw[i].cls, d);
    }
    NameDeltas(gThr, gThrN, nw, nn, gNameAcc, &gNameAccN);
    memcpy(gThr, nw, sizeof(struct ThreadRec) * nn);
    gThrN = nn;
}

// the process's instructions and cycles retired (all threads; the CPU counters, not affected by the clock): 0 when not known
int proc_pid_rusage(int pid, int flavor, rusage_info_t *buffer);   // <libproc.h>, not in the iOS SDK
static int Counters(uint64_t *ins, uint64_t *cyc) {
    struct rusage_info_v4 ri;
    if (proc_pid_rusage(getpid(), RUSAGE_INFO_V4, (rusage_info_t *)&ri) != 0 || ri.ri_cycles == 0) return 0;
    *ins = ri.ri_instructions;
    *cyc = ri.ri_cycles;
    return 1;
}

// the process's context switches so far (every thread's wakeups and preemptions), -1 when not known
static double Switches(void) {
    task_events_info_data_t ev;
    mach_msg_type_number_t n = TASK_EVENTS_INFO_COUNT;
    if (task_info(mach_task_self(), TASK_EVENTS_INFO, (task_info_t)&ev, &n) != KERN_SUCCESS) return -1.0;
    return (double)ev.csw;
}

// a bench row's split (libntr-ios-threads.patch calls these on the game thread at the row's start and end)
static struct ThreadRec gBenchThr[kMaxThreads];
static int gBenchThrN;
static uint64_t gBenchIns, gBenchCyc;
static int gBenchCnt;
static double gBenchCsw;
void bwport_sys_threads_mark(void) {
    gBenchThrN = Snap(gBenchThr, kMaxThreads);
    gBenchCsw = Switches();
    gBenchCnt = Counters(&gBenchIns, &gBenchCyc);
}
void bwport_sys_threads_row(const char *name, double frames, double secs) {
    struct ThreadRec nw[kMaxThreads];
    int nn = Snap(nw, kMaxThreads);
    struct NameSum d[kMaxThreads];
    int nd = 0;
    NameDeltas(gBenchThr, gBenchThrN, nw, nn, d, &nd);
    double tot = 0.0;
    for (int i = 0; i < nd; i++) tot += d[i].s;
    char t[512];
    const double scale = frames > 0.0 ? 1e3 / frames : 0.0;
    NamesText(d, nd, scale, t, sizeof t);
    char c[192] = "";
    uint64_t ins, cyc;   // the counters: an A/B measure the clock does not move; cycles / CPU time = the mean clock of all threads
    if (gBenchCnt && Counters(&ins, &cyc) && frames > 0.0 && tot > 0.0)
        snprintf(c, sizeof c, "; process %.2f M instructions, %.2f M cycles a drawn frame (~%.2f GHz mean)",
                 (double)(ins - gBenchIns) / 1e6 / frames, (double)(cyc - gBenchCyc) / 1e6 / frames, (double)(cyc - gBenchCyc) / 1e9 / tot);
    double csw = Switches();
    if (gBenchCsw >= 0.0 && csw >= 0.0 && frames > 0.0) {
        int k = (int)strlen(c);
        snprintf(c + k, sizeof c - k, "; %.1f context switches a drawn frame", (csw - gBenchCsw) / frames);
    }
    fprintf(stderr, "threads: %-20s CPU ms a drawn frame (%.1f drawn/s, live threads by name): all %.2f = %s%s\n", name,
            secs > 0.0 ? frames / secs : 0.0, tot * scale, t, c);
}
#else
void bwport_sys_threads_mark(void) {}
void bwport_sys_threads_row(const char *name, double frames, double secs) { (void)name; (void)frames; (void)secs; }
#endif

// ---- the clock estimate ----------------------------------------------------------------------------------------------------
// n dependent adds: `add x, x, y` cannot be folded, one cycle each on every core this runs on. Returns the time in seconds.
#if defined(__aarch64__) || defined(__x86_64__)
#define SYS_HAVE_CLOCK 1
static double AddChain(long iters) {
    uint64_t a = 1, b = 3;
    double t0 = NowS();
    for (long i = 0; i < iters; i++) {
#if defined(__aarch64__)
        __asm__ volatile("add %0, %0, %1\n add %0, %0, %1\n add %0, %0, %1\n add %0, %0, %1\n"
                         "add %0, %0, %1\n add %0, %0, %1\n add %0, %0, %1\n add %0, %0, %1\n"
                         : "+r"(a) : "r"(b));
#else
        __asm__ volatile("add %1, %0\n add %1, %0\n add %1, %0\n add %1, %0\n"
                         "add %1, %0\n add %1, %0\n add %1, %0\n add %1, %0\n"
                         : "+r"(a) : "r"(b));
#endif
    }
    double t = NowS() - t0;
    __asm__ volatile("" : : "r"(a));
    return t;
}
// GHz of the calling thread's core now, 0 when it cannot be measured
static double ProbeGHz(void) {
    const long iters = 4000;   // 32000 adds: ~10 us at 3.2 GHz
    double best = 1e9;
    for (int k = 0; k < 3; k++) {
        double t = AddChain(iters);
        if (t < best) best = t;
    }
    if (best <= 0.0 || best > 0.01) return 0.0;   // interrupted for long: no number
    return (double)iters * 8.0 / best * 1e-9;
}
#endif

// ---- the frame hook (the game thread) --------------------------------------------------------------------------------------
static unsigned gFrames;            // frame loop turns since the second began
static double gSecStart;
static unsigned gCycle = 60;        // the burst cycle: GPU timers on for the first gBurst frames of each cycle
static unsigned gCycleI;
static const unsigned gBurst = 14;
static double gGpuFrameSum, gGpuTotal;
static unsigned gGpuN;
static int gGpuTimers;              // the context has timer queries
// fence figures of the last second (iOS and anywhere the fences are on)
static double gFenceSum;
static unsigned gFenceN;
// accumulation for the 10 s log line
static double gLogGhzSum;
static int gLogGhzN;

static void StartSampler(void);

int bwport_sys_frame(void) {
    if (!gGameSet) {
        gGameThread = pthread_self();
#if defined(_WIN32)
        DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &gGameHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
#endif
        gGameSet = 1;
        gSecStart = NowS();
        if (!gSamplerStarted) { gSamplerStarted = 1; StartSampler(); }
    }
    gFrames++;
    gCycleI = gCycleI + 1 >= gCycle ? 0 : gCycleI + 1;
    double now = NowS();
    if (now - gSecStart < 1.0) return 0;
    // once a second, after a frame's work on this thread
    double span = now - gSecStart;
    gSecStart = now;
    gSys.framesPerSec = (float)((double)gFrames / span);
    gCycle = gFrames < 30 ? 30 : gFrames;
    gFrames = 0;
#ifdef SYS_HAVE_CLOCK
    double g = ProbeGHz();
    if (g > 0.0) {
        gSys.ghz = (float)g;
        gLogGhzSum += g;
        gLogGhzN++;
    }
#endif
    return 1;
}

// libntr asks each frame (cheaply) whether GPU timer queries should run this frame: the first frames of each cycle
int bwport_sys_want_gpu(void) {
    return gGpuTimers && gCycleI < gBurst;
}

// macOS: never. Apple's GL on Metal splits its command buffers at each query and the next draw waits for the GPU (a review's sample:
// the game thread blocked 3-6 ms in a burst frame's UI draw, 0.45-0.9 s of every 10 s), so the Mac's figure is the fences' bound as
// on iOS. The Breakdown window's own per-pass queries (libntr-pf82.patch) still run while it is open.
void bwport_sys_gpu_timers(int ok) {
#if defined(__APPLE__)
    (void)ok;
    gGpuTimers = 0;
#else
    gGpuTimers = ok;
#endif
}

// a GPU time (ms) of the frame being reported (the sum of its passes, the present and the UI)
void bwport_sys_gpu_add(double ms) { gGpuFrameSum += ms; }

// the end of a reported frame
void bwport_sys_gpu_end(void) {
    if (gGpuFrameSum > 0.0) {
        gGpuTotal += gGpuFrameSum;
        gGpuN++;
    }
    gGpuFrameSum = 0.0;
}

// once a second (when bwport_sys_frame returned 1): the fence latency mean (ms), 0 = no fences
void bwport_sys_fence(float meanMs) {
    gSys.fenceMs = meanMs;
    if (meanMs > 0.0f) { gFenceSum += meanMs; gFenceN++; }
    // publish the GPU figures of the bursts since the last call
    if (gGpuN > 0) {
        gSys.gpuMs = (float)(gGpuTotal / (double)gGpuN);
        gSys.gpuBusy = fminf(100.0f, gSys.gpuMs * gSys.framesPerSec * 0.1f);
        gSys.gpuKind = 1;
        gGpuTotal = 0.0;
        gGpuN = 0;
    } else if (gGpuTimers) {
        // timers work but no burst finished this second: keep the last figures
    } else if (meanMs > 0.0f) {
        gSys.gpuMs = meanMs;
        gSys.gpuBusy = fminf(100.0f, meanMs * gSys.framesPerSec * 0.1f);
        gSys.gpuKind = 2;
    } else {
        gSys.gpuKind = 0;
    }
}

const bwport_sys_t *bwport_sys_get(void) { return &gSys; }

// ---- the sampler thread ------------------------------------------------------------------------------------------------------
static void Format(char *buf, int n, const bwport_sys_t *s);

static void *Sampler(void *arg) {
    (void)arg;
#if defined(__APPLE__)
    pthread_setname_np("bwport sysmetrics");
#endif
    double prevWall = NowS(), prevCpu = ProcessCpuS(), prevGame = GameCpuS();
    const char *pe = getenv("BWPORT_SYSPERIOD");   // seconds between the log lines (tests; default 10)
    double period = pe != NULL && atof(pe) >= 1.0 ? atof(pe) : 10.0;
    double logWall = prevWall, accMain = 0, accOther = 0, accAudio = 0, accCpu = 0;
#if defined(__APPLE__)
    double d0[4];
    ThreadDeltas(d0);
#endif
    for (;;) {
#if defined(_WIN32)
        Sleep(1000);
#else
        struct timespec ts = {1, 0};
        nanosleep(&ts, NULL);
#endif
        double wall = NowS(), cpu = ProcessCpuS();
        double dw = wall - prevWall;
        if (dw <= 0.0) continue;
        double dc = cpu >= 0.0 && prevCpu >= 0.0 ? cpu - prevCpu : -1.0;
        double mainC = -1.0, otherC = -1.0, audioC = -1.0;
#if defined(__APPLE__)
        double d[4];
        ThreadDeltas(d);
        mainC = d[0]; audioC = d[2];
#else
        double game = GameCpuS();
        if (game >= 0.0 && prevGame >= 0.0) mainC = game - prevGame;
        prevGame = game;
#endif
        if (dc >= 0.0 && mainC >= 0.0) {
            otherC = dc - mainC - (audioC > 0.0 ? audioC : 0.0);
            if (otherC < 0.0) otherC = 0.0;
        }
        if (dc >= 0.0) gSys.cores = (float)(dc / dw);
        gSys.mainCores = mainC >= 0.0 ? (float)(mainC / dw) : -1.0f;
        gSys.otherCores = otherC >= 0.0 ? (float)(otherC / dw) : -1.0f;
        gSys.audioCores = audioC >= 0.0 ? (float)(audioC / dw) : -1.0f;
        if (dc >= 0.0) accCpu += dc;
        if (mainC >= 0.0) accMain += mainC;
        if (otherC >= 0.0) accOther += otherC;
        if (audioC >= 0.0) accAudio += audioC;
        prevWall = wall; prevCpu = cpu;
        if (wall - logWall >= period && gGameSet) {
            double w = wall - logWall;
            bwport_sys_t s = gSys;   // the 10 s means
            s.cores = (float)(accCpu / w);
            s.mainCores = mainC >= 0.0 ? (float)(accMain / w) : -1.0f;
            s.otherCores = otherC >= 0.0 ? (float)(accOther / w) : -1.0f;
            s.audioCores = audioC >= 0.0 ? (float)(accAudio / w) : -1.0f;
            if (gLogGhzN > 0) { s.ghz = (float)(gLogGhzSum / gLogGhzN); gLogGhzSum = 0; gLogGhzN = 0; }
            char line[256];
            Format(line, sizeof line, &s);
            fprintf(stderr, "sys: %s\n", line);
#if defined(__APPLE__)
            char t[512];   // the threads by name, CPU ms a second (live threads)
            NamesText(gNameAcc, gNameAccN, 1e3 / w, t, sizeof t);
            gNameAccN = 0;
            fprintf(stderr, "sys threads: CPU ms/s by name: %s\n", t);
#endif
            logWall = wall; accMain = accOther = accAudio = accCpu = 0;
        }
    }
    return NULL;
}

static void StartSampler(void) {
    pthread_t t;
    pthread_attr_t at;
    pthread_attr_init(&at);
    pthread_attr_setdetachstate(&at, PTHREAD_CREATE_DETACHED);
    pthread_create(&t, &at, Sampler, NULL);
}

// ---- text ----------------------------------------------------------------------------------------------------------------------
// one part of the readout: 0 cpu, 1 clock, 2 gpu, 3 heat state (for the Metrics window and the overlay)
static void Line(const bwport_sys_t *s, int i, char *buf, int n);
static void Line(const bwport_sys_t *s, int i, char *buf, int n) {
    int k = 0;
    buf[0] = 0;
    if (i >= 4) {   // the short forms, for the overlay
        switch (i - 4) {
        case 0:
            k = snprintf(buf, n, "cpu %.1f", s->cores);
            if (s->mainCores >= 0.0f) {
                k += snprintf(buf + k, n - k, " (main %.1f", s->mainCores);
                if (s->otherCores >= 0.0f) k += snprintf(buf + k, n - k, " oth %.1f", s->otherCores);
                if (s->audioCores >= 0.0f) k += snprintf(buf + k, n - k, " aud %.1f", s->audioCores);
                snprintf(buf + k, n - k, ")");
            }
            break;
        case 1:
            if (s->ghz > 0.0f) snprintf(buf, n, "~%.1f GHz est", s->ghz);
            else snprintf(buf, n, "clock n/a");
            break;
        case 2:
            if (s->gpuKind == 1) snprintf(buf, n, "gpu %.1f ms ~%.0f%% busy", s->gpuMs, s->gpuBusy);
            else if (s->gpuKind == 2) snprintf(buf, n, "gpu <=%.1f ms <=%.0f%% (fence)", s->gpuMs, s->gpuBusy);
            else snprintf(buf, n, "gpu n/a");
            break;
        default:
            Line(s, 3, buf, n);
        }
        return;
    }
    switch (i) {
    case 0:
        k = snprintf(buf, n, "cpu %.1f cores", s->cores);
        if (s->mainCores >= 0.0f) {
            k += snprintf(buf + k, n - k, " (main %.1f", s->mainCores);
            if (s->otherCores >= 0.0f) k += snprintf(buf + k, n - k, ", other %.1f", s->otherCores);
            if (s->audioCores >= 0.0f) k += snprintf(buf + k, n - k, ", audio %.1f", s->audioCores);
            snprintf(buf + k, n - k, ")");
        }
        break;
    case 1:
        if (s->ghz > 0.0f) snprintf(buf, n, "~%.1f GHz (game thread, estimate)", s->ghz);
        else snprintf(buf, n, "clock n/a");
        break;
    case 2:
        if (s->gpuKind == 1) snprintf(buf, n, "gpu %.1f ms/frame ~%.0f%% busy (%s)", s->gpuMs, s->gpuBusy,
                                      SIM_PresentMetal() ? "Metal command buffers" : "timer queries");
        else if (s->gpuKind == 2) snprintf(buf, n, "gpu <=%.1f ms/frame <=%.0f%% busy (fence latency)", s->gpuMs, s->gpuBusy);
        else snprintf(buf, n, "gpu n/a");
        break;
    default: {
        const char *th = NULL;
#if defined(__APPLE__)
        if (bwport_thermal_name != NULL) th = bwport_thermal_name();
#elif defined(__ANDROID__)
        th = bwport_thermal_name();
#endif
        snprintf(buf, n, "thermal %s", th != NULL ? th : "n/a");
    }
    }
}

// the log line: sys: cpu 2.3 cores (main 0.9, render 0.8, audio 0.1), ~3.4 GHz (game thread, estimate), gpu 6.1 ms/frame ~72% busy
// (timer queries), thermal fair
static void Format(char *buf, int n, const bwport_sys_t *s) {
    int k = 0;
    for (int i = 0; i < 4; i++) {
        char t[96];
        Line(s, i, t, sizeof t);
        k += snprintf(buf + k, n - k, "%s%s", i ? ", " : "", t);
    }
}

void bwport_sys_line(int i, char *buf, int n) { Line(&gSys, i, buf, n); }

void bwport_sys_format(char *buf, int n) { Format(buf, n, &gSys); }
