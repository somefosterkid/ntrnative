// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/adventure.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SAVE_ADVENTURE_H
#define BWPORT_SAVE_ADVENTURE_H

#include "types.h"
#include "struct_decls.h"
#include "nitro/rtc.h"

// Starts the OS_GetTick: the date and time now, no days since
void buildAdventureTimeBlk(AdventureTime *time);
// Takes minutes off the day's countdown, which stops at 0
void dayCountdownTest(AdventureTime *time, u32 minutes);

// The game's OS_GetTick, at 0x10 in the adventure data save block
struct AdventureTime {
    u32 unk0;
    // When the OS_GetTick was last checked
    RTCDate date;
    RTCTime time;
    // The number of the day it was last checked on
    s32 day;
    // When the adventure started, in seconds since 2000
    s64 startSeconds;
    // When the player first entered the Hall of Fame, or 0
    s64 seconds;
    // The minutes left of the day's countdown, at most MINUTES_PER_DAY
    u32 dayMinutes;
};

#define MINUTES_PER_DAY 1440

// Black 2's; twinned code confirms the offsets of rtcOffset, ownerMac, birthdayMonth, birthdayDay and time, and its
// size in Black
// The save block 0x25 (adventure_savedata.c, a guessed name)
struct AdventureSave {
    // The RTC's offset, which a changed OS_GetTick no longer matches
    s64 rtcOffset;
    u8 ownerMac[6];
    // The day and month of the DS owner's birthday
    u8 birthdayMonth;
    u8 birthdayDay;
    AdventureTime time;
    u8 unk48[0x14];
};

u32 getSizeofAdventureDataBlk(void);
void initAdventureDataBlk(AdventureSave *adventure);
void setAdvTimeBlkRtcOffsetOwnerMacBdayMonthDay(AdventureSave *adventure);
BOOL hasClockNotBeenTampered(AdventureSave *adventure);
u8 getOSBirthdayDay(AdventureSave *adventure);
BOOL hasFullDayMinutesPassed(AdventureTime *time);
void setNewDayForCountdown(AdventureTime *time);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u8 getOSBirthdayMonth(AdventureSave *adventure);

#endif // BWPORT_SAVE_ADVENTURE_H
