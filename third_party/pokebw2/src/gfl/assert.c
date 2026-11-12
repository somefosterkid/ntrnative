#include "types.h"
#include "gfl/std.h"

// The handlers, and the callback a failed assertion calls
static AssertFailCallback sAssertFailCallback;
static AssertFinishFunc sAssertFinish;
static AssertPrintFunc sAssertPrint;
static AssertInitFunc sAssertInit;

void GFL_DebugSetAssertHandlers(AssertInitFunc init, AssertPrintFunc print, AssertFinishFunc finish) {
    sAssertInit = init;
    sAssertPrint = print;
    sAssertFinish = finish;
}

void GFL_DebugSetAssertFailCallback(AssertFailCallback callback) {
    sAssertFailCallback = callback;
}

void GFL_DebugAssertFail(const char *file, u32 line, const char *expression) {
    if (sAssertFailCallback != NULL) {
        sAssertFailCallback();
    }
}

void GFL_DebugAssertFailEx(const char *file, u32 line, const char *format, ...) {
    if (sAssertFailCallback != NULL) {
        sAssertFailCallback();
    }
}
