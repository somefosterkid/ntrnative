// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/rtc.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_RTC_H
#define BWPORT_SYSTEM_RTC_H

#include "types.h"
#include "nitro/rtc.h"

// The hour a season's day period starts at
u8 GetLightChangeHoursForSeasons(u32 season, u32 period);
// The day period the time is in, by the season's hours. pokebw2 returns a u16, but move_pokemon.c converts the
// result to one
u32 GetRealTimeDayPeriod(u8 season);

// Black 2's; twinned code confirms the values of DAY_PERIOD_NIGHT and DAY_PERIOD_MIDNIGHT in Black
// The periods of a day
enum {
    DAY_PERIOD_MORNING,
    DAY_PERIOD_DAY,
    DAY_PERIOD_EVENING,
    DAY_PERIOD_NIGHT,
    DAY_PERIOD_MIDNIGHT,
    DAY_PERIOD_COUNT
};

// Whether it is night or midnight in the season. Not from swan
BOOL IsDayPeriodNight(u8 season);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The period of the day at the hour, in the season
u16 GetDayPeriod(u8 season, u32 hour);
// The OS_GetTick as GFL caches it is in gfl/rtc_cache.h
RTCResult RTC_GetDate(RTCDate *date);
// NitroSDK's RTC_GetTime
RTCResult RTC_GetTime(RTCTime *time);

#endif // BWPORT_SYSTEM_RTC_H
