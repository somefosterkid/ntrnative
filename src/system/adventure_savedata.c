// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/adventure_savedata.c, ported by tools/port_file.py.
#include "types.h"
#include "save/adventure.h"
#include "gfl/rtc_cache.h"
#include "gfl/std.h"
#include "gfl/net.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "save/save_control.h"
#include "nitro/mi.h"

// The save block with the game's OS_GetTick: what the RTC was at, the DS owner's birthday, the day's countdown and when
// the adventure began. The ROM has no string for the file, so the name is a guess. Block 0x25.

#define SAVE_BLOCK_ADVENTURE 0x25

u32 getSizeofAdventureDataBlk(void) {
    return sizeof(AdventureSave);
}

void initAdventureDataBlk(AdventureSave *adventure) {
    MIi_CpuClearFast(0, adventure, sizeof(AdventureSave));
    buildAdventureTimeBlk(&adventure->time);
}

AdventureSave *getSaveAdventureDataBlk(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_ADVENTURE);
}

AdventureTime *getSaveAdventureTimeBlock(SaveControl *save) {
    AdventureSave *adventure = SaveControl_GetBlockPtr(save, SAVE_BLOCK_ADVENTURE);
    return &adventure->time;
}

void setAdvTimeBlkRtcOffsetOwnerMacBdayMonthDay(AdventureSave *adventure) {
    OSOwnerInfo info;

    adventure->rtcOffset = OS_GetOwnerRtcOffset();
    OS_GetMacAddress(adventure->ownerMac);
    OS_GetOwnerInfo(&info);
    adventure->birthdayMonth = info.birthday.month;
    adventure->birthdayDay = info.birthday.day;
}

BOOL func_02008b4c(AdventureSave *adventure) {
    u8 mac[6];
    int i;

    OS_GetMacAddress(mac);
    for (i = 0; i < 6; i++) {
        if (mac[i] != adventure->ownerMac[i]) {
            return FALSE;
        }
    }
    return TRUE;
}


BOOL hasClockNotBeenTampered(AdventureSave *adventure) {
    if (OS_GetOwnerRtcOffset() == adventure->rtcOffset) {
        return TRUE;
    }
    return FALSE;
}

u8 getOSBirthdayMonth(AdventureSave *adventure) {
    return adventure->birthdayMonth;
}

u8 getOSBirthdayDay(AdventureSave *adventure) {
    return adventure->birthdayDay;
}

void buildAdventureTimeBlk(AdventureTime *time) {
    time->unk0 = 1;
    RTC_GetCachedDateTime(&time->date, &time->time);
    time->day = RTC_ConvertDateToDay(&time->date);
    time->startSeconds = RTC_ConvertDateTimeToSecond(&time->date, &time->time);
    time->seconds = 0;
    time->dayMinutes = 0;
}

BOOL hasFullDayMinutesPassed(AdventureTime *time) {
    if (time->dayMinutes != 0) {
        return TRUE;
    }
    return FALSE;
}

void dayCountdownTest(AdventureTime *time, u32 minutes) {
    if (time->dayMinutes > MINUTES_PER_DAY) {
        time->dayMinutes = MINUTES_PER_DAY;
    }
    if (time->dayMinutes < minutes) {
        time->dayMinutes = 0;
    } else {
        time->dayMinutes -= minutes;
    }
}

void setNewDayForCountdown(AdventureTime *time) {
    time->dayMinutes = MINUTES_PER_DAY;
    RTC_GetCachedDateTime(&time->date, &time->time);
    time->day = RTC_ConvertDateToDay(&time->date);
}
