// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/rtc_cache.c, which is the same code in
// Black 2/White 2.
#include "types.h"
#include "gfl/rtc_cache.h"
#include "gfl/std.h"
#include "nitro/rtc.h"

// The date and time, read from the OS_GetTick every few frames so the game need not wait for it. The file's name is a guess

typedef struct {
    BOOL valid;
    // Whether a read is in progress
    BOOL busy;
    int frames;
    int result;
    RTCDate date;
    RTCTime time;
    // Where the read in progress goes
    RTCDate readDate;
    RTCTime readTime;
} RTCCache;

static const u16 data_020a08b8[12] = {
#include "asset/rtc_cache_data_020a08b8.inc"
};

static RTCCache data_021469fc;

static void func_0203f42c(RTCResult result, void *arg);
static void func_0203f46c(RTCCache *cache);

void func_0203f3b4(void) {
    RTC_Init();
    MI_CpuFill8(&data_021469fc, 0, sizeof(RTCCache));
    data_021469fc.valid = FALSE;
    data_021469fc.busy = FALSE;
    data_021469fc.frames = 0;
    func_0203f46c(&data_021469fc);
}

void func_0203f3f0(void) {
    if (data_021469fc.busy == FALSE) {
        data_021469fc.frames++;
        if (data_021469fc.frames > 10) {
            data_021469fc.frames = 0;
            func_0203f46c(&data_021469fc);
        }
    }
}

static void func_0203f42c(RTCResult result, void *arg) {
    RTCCache *cache = arg;

    cache->result = result;
    cache->valid = TRUE;
    cache->date = cache->readDate;
    cache->time = cache->readTime;
    cache->busy = FALSE;
}

static void func_0203f46c(RTCCache *cache) {
    cache->busy = TRUE;
    cache->result = RTC_GetDateTimeAsync(&cache->readDate, &cache->readTime, func_0203f42c, cache);
}

void RTC_GetCachedDateTime(RTCDate *date, RTCTime *time) {
    *date = data_021469fc.date;
    *time = data_021469fc.time;
}

void RTC_GetCachedTime(RTCTime *time) {
    *time = data_021469fc.time;
}

void RTC_GetCachedDate(RTCDate *date) {
    *date = data_021469fc.date;
}

int RTC_ConvertDaySecondsCached(void) {
    const RTCTime *time = &data_021469fc.time;

    return time->second + (time->minute * 60 + time->hour * 3600);
}

s64 RTC_ConvertSecondsCached(void) {
    return RTC_ConvertDateTimeToSecond(&data_021469fc.date, &data_021469fc.time);
}

int func_0203f540(const RTCDate *date) {
    int days;
    RTCDate firstDay;

    days = date->day;
    // Months count from 1
    days += *(data_020a08b8 + date->month - 1);
    if (date->month >= 3) {
        int year = date->year;

        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            days++;
        }
    }
    firstDay = *date;
    firstDay.month = 1;
    firstDay.day = 1;
    RTC_ConvertDateToDay(&firstDay);
    RTC_ConvertDateToDay(date);
    return days;
}

