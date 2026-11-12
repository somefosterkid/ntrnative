// The win64 build's console (docs/port-build.md, win64). Nothing here on Linux.
//
// - stdout and stderr unbuffered: Windows' C runtime buffers a stream that goes to a file or a pipe (bwport.bat's
//   log, a run from WSL), and a crash or an undefined-function stub's abort() loses what it held. The game's log
//   lines (port/src, the stubs) are few, so the cost doesn't matter.
// - a crash report on stderr in place of gdb's backtrace: the exception, the address it touched and the stack, each
//   frame as module+offset (`tools/port_win64.sh symbolize LOG` gives bwport.exe's their function and line). An undefined
//   function's stub (its abort(), SIGABRT) prints the same stack, so its caller shows.
#ifdef _WIN32
#include <windows.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT_CRASH_FRAMES 40

static void PrintFrame(int index, DWORD64 pc) {
    HMODULE module = NULL;
    char path[MAX_PATH], *name;

    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            (LPCSTR)(ULONG_PTR)pc, &module)
        || GetModuleFileNameA(module, path, sizeof(path)) == 0) {
        fprintf(stderr, "  #%-2d 0x%016llx\n", index, (unsigned long long)pc);
        return;
    }
    name = strrchr(path, '\\');
    name = name != NULL ? name + 1 : path;
    // the offset in the module (its RVA; the loader puts the exe anywhere): tools/port_win64.sh symbolize adds
    // bwport.exe's ImageBase and asks addr2line
    fprintf(stderr, "  #%-2d %s+0x%llx\n", index, name, (unsigned long long)(pc - (DWORD64)module));
}

// x64's unwind tables (.pdata, which mingw and clang emit for every function) walk the stack from a context
static void PrintStack(CONTEXT *context) {
    CONTEXT c = *context;
    int i;

    for (i = 0; i < PORT_CRASH_FRAMES && c.Rip != 0; i++) {
        DWORD64 imageBase, establisher;
        PVOID handlerData;
        PRUNTIME_FUNCTION fn;

        // a return address points after its call: one byte back is the call's line
        PrintFrame(i, i == 0 ? c.Rip : c.Rip - 1);
        fn = RtlLookupFunctionEntry(c.Rip, &imageBase, NULL);
        if (fn == NULL) {
            // a leaf function: the return address is on top of the stack
            if (IsBadReadPtr((void *)(ULONG_PTR)c.Rsp, sizeof(DWORD64))) {
                break;
            }
            c.Rip = *(DWORD64 *)(ULONG_PTR)c.Rsp;
            c.Rsp += 8;
        } else {
            RtlVirtualUnwind(UNW_FLAG_NHANDLER, imageBase, c.Rip, fn, &c, &handlerData, &establisher, NULL);
        }
    }
}

static LONG WINAPI PortCrash(EXCEPTION_POINTERS *e) {
    EXCEPTION_RECORD *r = e->ExceptionRecord;

    fprintf(stderr, "bwport: crash: exception 0x%08lx in thread %lu", (unsigned long)r->ExceptionCode,
            (unsigned long)GetCurrentThreadId());
    if (r->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && r->NumberParameters >= 2) {
        const char *how = r->ExceptionInformation[0] == 0 ? "reading" : r->ExceptionInformation[0] == 1 ? "writing"
                                                                                                          : "running";
        fprintf(stderr, " (access violation %s 0x%llx)", how, (unsigned long long)r->ExceptionInformation[1]);
    }
    fprintf(stderr, "\n");
    PrintStack(e->ContextRecord);
    fflush(stderr);
    // Windows ends the process as before (exit code: the exception's)
    return EXCEPTION_CONTINUE_SEARCH;
}

static void PortAbort(int sig) {
    CONTEXT c;

    (void)sig;
    fprintf(stderr, "bwport: abort() in thread %lu\n", (unsigned long)GetCurrentThreadId());
    RtlCaptureContext(&c);
    PrintStack(&c);
    fflush(stderr);
    // the C runtime's default: exit code 3
    _exit(3);
}

// BWPORT_TRACE_EXIT=1: the stack of whoever ends the process through exit()
static void PortExitTrace(void) {
    CONTEXT c;

    fprintf(stderr, "bwport: exit() in thread %lu\n", (unsigned long)GetCurrentThreadId());
    RtlCaptureContext(&c);
    PrintStack(&c);
    fflush(stderr);
}

// Before every other constructor (main_start.c's prints)
__attribute__((constructor(101))) static void PortConsoleInit(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    SetUnhandledExceptionFilter(PortCrash);
    // the handler ends the process, so msvcrt's "terminate it in an unusual way" line doesn't come
    signal(SIGABRT, PortAbort);
    if (getenv("BWPORT_TRACE_EXIT") != NULL) {
        atexit(PortExitTrace);
    }
}
#endif
