// macOS and iOS: a crash prints a backtrace to stderr and exits quietly, so no "quit unexpectedly" dialog opens and a test run
// has the faulting stack in its log (the maintainers' notes, "Mac"). Symbols come from the exe's own symbol table. The head lines give
// the main image's load address and UUID: tools/ios_symbolicate.sh turns the addresses into functions and lines (atos) with the
// dSYM kept for that build, which a stripped or sideloaded app needs. BWPORT_CRASHTEST=1 crashes on purpose (PORT_CrashTest).
#ifdef __APPLE__
#include <execinfo.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void PutHex(uint64_t v) {
    char b[19] = "0x";
    int i;

    for (i = 0; i < 16; i++) {
        b[2 + i] = "0123456789abcdef"[(v >> (60 - 4 * i)) & 15];
    }
    b[18] = '\n';
    write(2, b, 19);
}

static void PutStr(const char *s) {
    write(2, s, strlen(s));
}

static void CrashHandler(int sig, siginfo_t *info, void *ctx) {
    void *frames[48];
    const struct mach_header_64 *mh = (const struct mach_header_64 *)_dyld_get_image_header(0);
    const struct load_command *lc = (const struct load_command *)(mh + 1);
    uint32_t i;

    PutStr("bwport: CRASH, signal ");
    PutHex((uint64_t)sig);
    PutStr("bwport: fault address ");
    PutHex((uint64_t)(uintptr_t)info->si_addr);
    PutStr("bwport: main image load address ");
    PutHex((uint64_t)(uintptr_t)mh);
    PutStr("bwport: main image slide ");
    PutHex((uint64_t)_dyld_get_image_vmaddr_slide(0));
    for (i = 0; i < mh->ncmds; i++, lc = (const struct load_command *)((const char *)lc + lc->cmdsize)) {
        if (lc->cmd == LC_UUID) {
            const uint8_t *u = ((const struct uuid_command *)lc)->uuid;
            char b[40];
            int k, n = 0;

            for (k = 0; k < 16; k++) {
                b[n++] = "0123456789ABCDEF"[u[k] >> 4];
                b[n++] = "0123456789ABCDEF"[u[k] & 15];
                if (k == 3 || k == 5 || k == 7 || k == 9) {
                    b[n++] = '-';
                }
            }
            b[n++] = '\n';
            PutStr("bwport: main image uuid ");
            write(2, b, n);
        }
    }
    PutStr("bwport: backtrace:\n");
    backtrace_symbols_fd(frames, backtrace(frames, 48), 2);
    _exit(128 + sig);
}

__attribute__((constructor)) static void CrashInit(void) {
    struct sigaction sa = {0};
    static char altStack[65536];   // a stack overflow runs the handler here, not on the exhausted stack (main thread only: it is per thread)
    stack_t ss = {.ss_sp = altStack, .ss_size = sizeof altStack};

    sigaltstack(&ss, NULL);
    sa.sa_sigaction = CrashHandler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGBUS, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
}

__attribute__((noinline)) static void CrashTestWrite(volatile int *p) {
    *p = 1;
}

// BWPORT_CRASHTEST=1: a write through a null pointer, to test the log and tools/ios_symbolicate.sh
__attribute__((noinline)) static int CrashTestRecurse(int n) {
    volatile char big[4096];

    big[0] = (char)n;
    return CrashTestRecurse(n + 1) + big[0];
}

void PORT_CrashTest(void) {
    const char *t = getenv("BWPORT_CRASHTEST");

    if (t != NULL && t[0] == '2') {   // BWPORT_CRASHTEST=2: runs off the stack
        CrashTestRecurse(0);
    }
    if (t != NULL) {
        CrashTestWrite(NULL);
    }
}
#else
void PORT_CrashTest(void) {}
#endif
