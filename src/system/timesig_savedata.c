// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/timesig_savedata.c, ported by tools/port_file.py.
#include "types.h"
#include "save/timesig_savedata.h"
#include "gfl/rtc_cache.h"
#include "gfl/std.h"
#include "nitro/rtc.h"
#include "save/save_control.h"

// The save block of the trainer card's signature and when each badge was obtained. The ROM has no string for the
// file, so the name is a guess (it comes from swan's getTimeSigBlkAddress). Block 0x21.

#define SAVE_BLOCK_TIMESIG 0x21

u32 func_020089a8(void) {
    return sizeof(TimeSigSave);
}

void func_020089b0(TimeSigSave *timeSig) {
    MI_CpuFill8(timeSig, 0, sizeof(TimeSigSave));
}

void *getTimeSigBlkAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_TIMESIG);
}

void *func_020089cc(void *timeSig) {
    return timeSig;
}

BOOL func_020089d0(TimeSigSave *timeSig) {
    u32 sum = 0;
    int i;

    for (i = 0; i < 0x180; i++) {
        sum += timeSig->signature[i];
    }
    if (sum == 0) {
        return FALSE;
    }
    return TRUE;
}

u8 func_020089f4(TimeSigSave *timeSig) {
    return timeSig->unk602;
}

void func_02008a00(TimeSigSave *timeSig) {
    timeSig->unk602 = 1;
}

u16 func_02008a0c(TimeSigSave *timeSig) {
    return timeSig->unk600;
}

void func_02008a14(TimeSigSave *timeSig, u32 value) {
    timeSig->unk600 = value;
}

void func_02008a1c(TimeSigSave *timeSig, u8 value) {
    timeSig->unk603 = value;
}

BOOL func_02008a28(TimeSigSave *timeSig) {
    return timeSig->unk603;
}

void saveSecondsTime(TimeSigSave *timeSig, u32 low, u32 high) {
    timeSig->secondsLow = low;
    timeSig->secondsHigh = high;
}

s64 loadSecondsTime(TimeSigSave *timeSig) {
    return timeSig->seconds;
}

TimeSigBadgeDate func_02008a54(TimeSigSave *timeSig, u32 badgeId) {
    return timeSig->badgeDates[badgeId];
}

void setBadgeGetSecondsTime(TimeSigSave *timeSig, u32 badgeId, u32 year, u32 month, u32 day) {
    RTCDate date;
    RTCTime time;
    s64 seconds;

    RTC_GetCachedDateTime(&date, &time);
    seconds = RTC_ConvertDateTimeToSecond(&date, &time);
    timeSig->badgeDates[badgeId].year = year;
    timeSig->badgeDates[badgeId].month = month;
    timeSig->badgeDates[badgeId].day = day;
    timeSig->unk638[badgeId] = 240;
    saveSecondsTime(timeSig, seconds, seconds >> 32);
}

u16 func_02008acc(TimeSigSave *timeSig, u32 index) {
    return timeSig->unk638[index];
}

void func_02008adc(TimeSigSave *timeSig, u32 index, u32 value) {
    timeSig->unk638[index] = value;
}
