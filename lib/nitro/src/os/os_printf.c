// TwlSDK's OS library, os_printf.c: Black's main 0x02084f40-0x02084fb4. The OS_ functions that format text, each
// passing its arguments on to STD_TVSNPrintf (std_sprintf.c), as libntr (MIT, https://github.com/cybervisi0n/libntr)
// has them for SDK 5.

#include "nitro/os.h"

int OS_SPrintf(char *dst, const char *fmt, ...) {
    int ret;
    va_list va;
    va_start(va, fmt);
    ret = OS_VSPrintf(dst, fmt, va);
    va_end(va);
    return ret;
}

int OS_VSPrintf(char *dst, const char *fmt, va_list va) {
    return OS_VSNPrintf(dst, 0x7fffffff, fmt, va);
}

int OS_SNPrintf(char *dst, u32 len, const char *fmt, ...) {
    int ret;
    va_list va;
    va_start(va, fmt);
    ret = OS_VSNPrintf(dst, len, fmt, va);
    va_end(va);
    return ret;
}

int OS_VSNPrintf(char *dst, u32 len, const char *fmt, va_list va) {
    return STD_TVSNPrintf(dst, len, fmt, va);
}
