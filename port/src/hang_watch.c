// macOS and iOS (the PC and Android in the second half of this file): a hang leaves every thread's stack in the log. A thread of its own looks at the DS frame count once a second;
// when it has not moved for 10 s while the game was running (past frame 600; not while iOS's settings sheet is up, which holds
// the game, the app is in the background or the launcher runs, nor in the 5 s after), it writes "bwport: HANG" with the frame, the main image's address and UUID (as crash_bt.c, for
// tools/ios_symbolicate.sh) and then each thread's backtrace: each is sent SIGURG in turn and writes its own from the handler.
// The same when frames run but nothing has been presented for that long (a frozen picture over a running game).
// Once a stall: a frame that moves again arms it anew. The phone's "Share latest log" then says where the game stopped
// (the 2026-11-05 lock after a save with fast forward on had no log of it). BWPORT_HANGWATCH=0 turns it off; =N waits N s.
#ifdef __APPLE__
#include <nitro.h>
#include <execinfo.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach/mach.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef PORT_IOS
int bwport_ios_settings_is_open(void);   // port/src/ios_settings.m
#endif
float SIM_PresentRate(void);   // libntr's sim_main.cpp: the presents of the last second
int SIM_InBackground(void);    // libntr's sim_touch.cpp (libntr-ios-savemax.patch): the app is in the background
extern volatile int PORT_LauncherUp;   // machine.c: the launcher runs

static volatile int sDone;

static void HangStack(int sig) {
    void *frames[48];
    char name[64] = "", line[128];
    int n;

    (void)sig;
    pthread_getname_np(pthread_self(), name, sizeof name);
    if (name[0] == 0 && pthread_main_np()) {
        strcpy(name, "main");
    }
    n = snprintf(line, sizeof line, "bwport: HANG thread \"%s\" backtrace:\n", name);
    write(2, line, (size_t)n);
    backtrace_symbols_fd(frames, backtrace(frames, 48), 2);
    sDone = 1;
}

static void HangImage(void) {
    const struct mach_header_64 *mh = (const struct mach_header_64 *)_dyld_get_image_header(0);
    const struct load_command *lc = (const struct load_command *)(mh + 1);

    fprintf(stderr, "bwport: main image load address 0x%llx\nbwport: main image slide 0x%llx\n", (unsigned long long)(uintptr_t)mh,
            (unsigned long long)_dyld_get_image_vmaddr_slide(0));
    for (uint32_t i = 0; i < mh->ncmds; i++, lc = (const struct load_command *)((const char *)lc + lc->cmdsize)) {
        if (lc->cmd == LC_UUID) {
            const uint8_t *u = ((const struct uuid_command *)lc)->uuid;

            fprintf(stderr, "bwport: main image uuid %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n", u[0], u[1],
                    u[2], u[3], u[4], u[5], u[6], u[7], u[8], u[9], u[10], u[11], u[12], u[13], u[14], u[15]);
        }
    }
}

static void HangDump(const char *what, u32 frame, int secs) {
    thread_act_array_t th;
    mach_msg_type_number_t n = 0;
    mach_port_t self = mach_thread_self();

    fprintf(stderr, "bwport: HANG: %s for %d s (frame %u); the threads' stacks follow\n", what, secs, (unsigned)frame);
    HangImage();
    if (task_threads(mach_task_self(), &th, &n) == KERN_SUCCESS) {
        for (mach_msg_type_number_t i = 0; i < n; i++) {
            pthread_t p = th[i] == self ? NULL : pthread_from_mach_thread_np(th[i]);

            if (p != NULL) {
                sDone = 0;
                if (pthread_kill(p, SIGURG) == 0) {
                    for (int k = 0; k < 50 && !sDone; k++) {
                        usleep(10000);
                    }
                }
            }
            mach_port_deallocate(mach_task_self(), th[i]);
        }
        vm_deallocate(mach_task_self(), (vm_address_t)th, n * sizeof th[0]);
    }
    mach_port_deallocate(mach_task_self(), self);
    fprintf(stderr, "bwport: HANG: end of the stacks\n");
}

static void *HangWatch(void *arg) {
    int limit = (int)(intptr_t)arg, still = 0, told = 0, dark = 0, toldDark = 0, after = 0;
    u32 last = 0;

    pthread_setname_np("bwport hang watch");
    for (;;) {
        u32 f = OS_GetVBlankCount();
        int held = 0;

        sleep(1);
#ifdef PORT_IOS
        held = bwport_ios_settings_is_open();
#endif
        // nothing to watch: the launcher is up, the app is in the background or the sheet holds the game; and 5 s after
        // (the first frames back from the background or the launcher's game boot)
        held = held || PORT_LauncherUp || SIM_InBackground();
        if (held) {
            after = 5;
        } else if (after > 0) {
            after--;
            held = 1;
        }
        // the game runs but no picture reaches the screen (SIM_PresentRate: the last second's presents): a frozen picture
        dark = f != last && f >= 600 && !held && SIM_PresentRate() <= 0.0f ? dark + 1 : 0;
        if (dark == 0 && toldDark) {
            fprintf(stderr, "bwport: HANG over: pictures again at frame %u\n", (unsigned)f);
            toldDark = 0;
        }
        if (dark >= limit && !toldDark) {
            toldDark = 1;
            HangDump("frames run but nothing presented", f, dark);
        }
        if (f != last || f < 600 || held) {
            if (told && f != last) {
                fprintf(stderr, "bwport: HANG over: frames again from %u\n", (unsigned)f);
            }
            last = f;
            still = told = 0;
            continue;
        }
        if (++still >= limit && !told) {
            told = 1;
            HangDump("no frame", f, still);
        }
    }
    return NULL;
}

__attribute__((constructor)) static void HangWatchInit(void) {
    const char *e = getenv("BWPORT_HANGWATCH");
    int limit = e != NULL && *e ? atoi(e) : 10;
    struct sigaction sa = {0};
    pthread_t t;
    static int sStarted;   // (a constructor runs once a process; this file's data is the host's, an in-process reset keeps it)

    if (limit <= 0 || sStarted) {
        return;
    }
    sStarted = 1;
    sa.sa_handler = HangStack;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGURG, &sa, NULL);
    if (pthread_create(&t, NULL, HangWatch, (void *)(intptr_t)limit) == 0) {
        pthread_detach(t);
    }
}
#else
// Linux, Android, Windows: the same watch. Linux and Android also write every thread's stack (each thread of /proc/self/task is
// sent SIGURG in turn; the handler walks its own with the unwinder and prints "module+offset" lines for addr2line, with the
// module's load address first). Windows has the "HANG" line and the frame, not the stacks.
#include <nitro.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <pthread.h>
#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <unwind.h>
// (declared here: port_prefix.h's includes come before any _GNU_SOURCE this file could define; dladdr's Dl_info is the same
// five fields on glibc and bionic)
typedef struct { const char *dli_fname; void *dli_fbase; const char *dli_sname; void *dli_saddr; } Dl_info;
int dladdr(const void *addr, Dl_info *info);
int pthread_getname_np(pthread_t t, char *name, size_t len);
int pthread_setname_np(pthread_t t, const char *name);
#endif

// (libntr's sim_main.cpp / sim_touch.cpp, where the build has them: not every platform's libntr does)
float SIM_PresentRate(void) __attribute__((weak));
int SIM_InBackground(void) __attribute__((weak));
extern volatile int PORT_LauncherUp;   // machine.c: the launcher runs

#ifndef _WIN32
static volatile int sDone;

struct HwWalk {
    int n;
};

static _Unwind_Reason_Code HwFrame(struct _Unwind_Context *c, void *arg) {
    struct HwWalk *w = arg;
    uintptr_t ip = (uintptr_t)_Unwind_GetIP(c);
    Dl_info di;
    char line[300];
    int n;

    if (ip == 0 || w->n >= 48) {
        return _URC_END_OF_STACK;
    }
    w->n++;
    if (dladdr((void *)ip, &di) && di.dli_fname != NULL) {
        n = snprintf(line, sizeof line, "  %s+0x%llx (%s)\n", di.dli_fname, (unsigned long long)(ip - (uintptr_t)di.dli_fbase),
                     di.dli_sname != NULL ? di.dli_sname : "?");
    } else {
        n = snprintf(line, sizeof line, "  0x%llx\n", (unsigned long long)ip);
    }
    write(2, line, (size_t)n);
    return _URC_NO_REASON;
}

static void HangStack(int sig) {
    char name[32] = "", line[96];
    struct HwWalk w = {0};
    int n;

    (void)sig;
    pthread_getname_np(pthread_self(), name, sizeof name);
    n = snprintf(line, sizeof line, "bwport: HANG thread \"%s\" backtrace:\n", name);
    write(2, line, (size_t)n);
    _Unwind_Backtrace(HwFrame, &w);
    sDone = 1;
}

static void HangImage(void) {
    Dl_info di;

    if (dladdr((void *)HangImage, &di)) {
        fprintf(stderr, "bwport: main image %s load address 0x%llx\n", di.dli_fname, (unsigned long long)(uintptr_t)di.dli_fbase);
    }
}
#endif

static void HangDump(const char *what, u32 frame, int secs) {
    fprintf(stderr, "bwport: HANG: %s for %d s (frame %u)", what, secs, (unsigned)frame);
#ifdef _WIN32
    fprintf(stderr, "\n");
#else
    DIR *d = opendir("/proc/self/task");
    struct dirent *e;
    long self = syscall(SYS_gettid);

    fprintf(stderr, "; the threads' stacks follow\n");
    HangImage();
    while (d != NULL && (e = readdir(d)) != NULL) {
        long tid = atol(e->d_name);

        if (tid > 0 && tid != self) {
            sDone = 0;
            if (syscall(SYS_tgkill, (long)getpid(), tid, SIGURG) == 0) {
                for (int k = 0; k < 50 && !sDone; k++) {
                    usleep(10000);
                }
            }
        }
    }
    if (d != NULL) {
        closedir(d);
    }
    fprintf(stderr, "bwport: HANG: end of the stacks\n");
#endif
}

static void HwSleep(void) {
#ifdef _WIN32
    Sleep(1000);
#else
    sleep(1);
#endif
}

#ifdef _WIN32
static DWORD WINAPI HangWatch(LPVOID arg)
#else
static void *HangWatch(void *arg)
#endif
{
    int limit = (int)(intptr_t)arg, still = 0, told = 0, dark = 0, toldDark = 0, after = 0;
    u32 last = 0;

    for (;;) {
        u32 f = OS_GetVBlankCount();
        int held;

        HwSleep();
        held = PORT_LauncherUp || (SIM_InBackground != NULL && SIM_InBackground());
        if (held) {
            after = 5;
        } else if (after > 0) {
            after--;
            held = 1;
        }
        dark = SIM_PresentRate != NULL && f != last && f >= 600 && !held && SIM_PresentRate() <= 0.0f ? dark + 1 : 0;
        if (dark == 0 && toldDark) {
            fprintf(stderr, "bwport: HANG over: pictures again at frame %u\n", (unsigned)f);
            toldDark = 0;
        }
        if (dark >= limit && !toldDark) {
            toldDark = 1;
            HangDump("frames run but nothing presented", f, dark);
        }
        if (f != last || f < 600 || held) {
            if (told && f != last) {
                fprintf(stderr, "bwport: HANG over: frames again from %u\n", (unsigned)f);
            }
            last = f;
            still = told = 0;
            continue;
        }
        if (++still >= limit && !told) {
            told = 1;
            HangDump("no frame", f, still);
        }
    }
    return 0;
}

__attribute__((constructor)) static void HangWatchInit(void) {
    const char *e = getenv("BWPORT_HANGWATCH");
    int limit = e != NULL && *e ? atoi(e) : 10;
    static int sStarted;

    if (limit <= 0 || sStarted) {
        return;
    }
    sStarted = 1;
#ifdef _WIN32
    HANDLE h = CreateThread(NULL, 0, HangWatch, (LPVOID)(intptr_t)limit, 0, NULL);

    if (h != NULL) {
        CloseHandle(h);
    }
#else
    struct sigaction sa;
    pthread_t t;

    memset(&sa, 0, sizeof sa);
    sa.sa_handler = HangStack;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGURG, &sa, NULL);
    if (pthread_create(&t, NULL, HangWatch, (void *)(intptr_t)limit) == 0) {
        pthread_setname_np(t, "bwport hangwatch");
        pthread_detach(t);
    }
#endif
}
#endif
