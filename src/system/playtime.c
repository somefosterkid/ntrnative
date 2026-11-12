// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/playtime.c, ported by tools/port_file.py.
#include "types.h"
#include "save/playtime.h"
#include "nitro/rtc.h"
#include "system/rtc.h"
#include "gfl/heap.h"
#include "gfl/std.h"

// The time played, and the date and time of the last save (the ROM's string for the file, "playtime.c"). The names of
// the fields are guessed.

PlayTime *setupPlaytimeCounters(HeapID heapId) {
    PlayTime *time = GFL_HeapAllocate(heapId, sizeof(PlayTime), TRUE, "playtime.c", 32);
    func_020085a0(time);
    return time;
}

void func_020085a0(PlayTime *time) {
    time->hours = 0;
    time->minutes = 0;
    time->seconds = 0;
    time->year = 0;
    time->month = 0;
    time->day = 0;
    time->hour = 0;
    time->minute = 0;
}

void updateGameTime(PlayTime *time, u32 addSeconds) {
    u32 hours = time->hours;
    u32 minutes;
    u32 seconds;

    if (hours == PLAYTIME_MAX_HOURS && time->minutes == 59 && time->seconds == 59) {
        return;
    }

    minutes = time->minutes;
    seconds = time->seconds + addSeconds;
    if (seconds > 59) {
        minutes += seconds / 60;
        seconds %= 60;
        if (minutes > 59) {
            hours += minutes / 60;
            minutes %= 60;
            if (hours >= PLAYTIME_MAX_HOURS) {
                hours = PLAYTIME_MAX_HOURS;
                minutes = 59;
                seconds = 59;
            }
        }
    }
    time->hours = hours;
    time->minutes = minutes;
    time->seconds = seconds;
}

void func_0200863c(PlayTime *dest, const PlayTime *src) {
    MI_CpuCopy8(src, dest, sizeof(PlayTime));
}

u16 func_0200864c(PlayTime *time) {
    return time->hours;
}

u8 func_02008650(PlayTime *time) {
    return time->minutes;
}

u8 func_02008654(PlayTime *time) {
    return time->seconds;
}

void func_02008658(PlayTime *time) {
    RTCDate date;
    RTCTime now;

    RTC_GetDate(&date);
    RTC_GetTime(&now);
    time->year = date.year;
    time->month = date.month;
    time->day = date.day;
    time->hour = now.hour;
    time->minute = now.minute;
}

u32 func_020086c8(PlayTime *time) {
    return time->year;
}

u32 func_020086d0(PlayTime *time) {
    return time->month;
}

u32 func_020086d8(PlayTime *time) {
    return time->day;
}

u32 func_020086e0(PlayTime *time) {
    return time->hour;
}

u32 func_020086e8(PlayTime *time) {
    return time->minute;
}

void func_020086f0(PlayTime *time, u32 *copy) {
    *copy = time->lastSaved;
}

void func_020086f8(PlayTime *time, u32 *copy) {
    time->lastSaved = *copy;
}
