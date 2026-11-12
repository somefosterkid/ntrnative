// What libntr's own PC objects reference and no libntr source defines for PC: TWL-only internals, reached only when
// OS_IsRunOnTwl() is TRUE, which it never is on PC (Black runs as a DS). They answer as their callers do on a DS.
#include "sdk_black.h"

// rtc/src/external.c's RTCi_GetCounter, RTCi_GetFout, RTCi_SetFout (and ...Async) call these on a DSi only; on a DS
// they return RTC_RESULT_INVALID_COMMAND themselves (libntr's rtc/include/private.h has the prototypes)
RTCResult RTCEXi_GetCounterAsync(u32 *count, RTCCallback callback, void *arg);
RTCResult RTCEXi_GetCounter(u32 *count);
RTCResult RTCEXi_GetFoutAsync(u16 *fout, RTCCallback callback, void *arg);
RTCResult RTCEXi_GetFout(u16 *fout);
RTCResult RTCEXi_SetFoutAsync(const u16 *fout, RTCCallback callback, void *arg);
RTCResult RTCEXi_SetFout(const u16 *fout);

RTCResult RTCEXi_GetCounterAsync(u32 *count, RTCCallback callback, void *arg)
{
    (void)count, (void)callback, (void)arg;
    return RTC_RESULT_INVALID_COMMAND;
}

RTCResult RTCEXi_GetCounter(u32 *count)
{
    (void)count;
    return RTC_RESULT_INVALID_COMMAND;
}

RTCResult RTCEXi_GetFoutAsync(u16 *fout, RTCCallback callback, void *arg)
{
    (void)fout, (void)callback, (void)arg;
    return RTC_RESULT_INVALID_COMMAND;
}

RTCResult RTCEXi_GetFout(u16 *fout)
{
    (void)fout;
    return RTC_RESULT_INVALID_COMMAND;
}

RTCResult RTCEXi_SetFoutAsync(const u16 *fout, RTCCallback callback, void *arg)
{
    (void)fout, (void)callback, (void)arg;
    return RTC_RESULT_INVALID_COMMAND;
}

RTCResult RTCEXi_SetFout(const u16 *fout)
{
    (void)fout;
    return RTC_RESULT_INVALID_COMMAND;
}

// os_ownerInfoEx.c's OS_GetISOCountryCodeA2 looks a DSi country code up in TwlSDK's table (os_countryCode.c) on a
// DSi only, and returns NULL on a DS. An empty table: a lookup would return NULL there too
const char *OSi_CountryCodeList[] = {NULL};
const int OSi_CountryCodeListNumEntries = 0;
