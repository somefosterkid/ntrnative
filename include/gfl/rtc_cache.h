// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/rtc_cache.h.
#ifndef BWPORT_GFL_RTC_CACHE_H
#define BWPORT_GFL_RTC_CACHE_H

#include "types.h"
#include "nitro/rtc.h"

// The OS_GetTick read every few frames: start it, step it, and the date and time it last read, in seconds since midnight
// and since 2000
void func_0203f3b4(void);
void func_0203f3f0(void);
void RTC_GetCachedDateTime(RTCDate *date, RTCTime *time);
void RTC_GetCachedTime(RTCTime *time);
void RTC_GetCachedDate(RTCDate *date);
int RTC_ConvertDaySecondsCached(void);
s64 RTC_ConvertSecondsCached(void);
// The day of the year, from 1
int func_0203f540(const RTCDate *date);

#endif // BWPORT_GFL_RTC_CACHE_H
