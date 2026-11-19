#ifndef POKEHEARTGOLD_SAV_SYSTEM_INFO_H
#define POKEHEARTGOLD_SAV_SYSTEM_INFO_H

#include <nitro/rtc/ARM9/api.h>

#include "save.h"
#include "save_layout.h"

typedef struct SysInfo_RTC {
    BOOL initialized;
    RTCDate date;
    RTCTime time;
    s32 days_since_nitro_epoch;
    s64_a4 seconds_since_nitro_epoch;
    s64_a4 seconds_at_game_clear;
    u32 penaltyInMinutes;
} SysInfo_RTC;
LAYOUT_CHECK(SysInfo_RTC_size, sizeof(SysInfo_RTC) == 0x38);

typedef struct SysInfo {
    s64_a4 rtc_offset;
    u8 mac_address[6];
    u8 birth_month;
    u8 birth_day;
    SysInfo_RTC rtc_info;
    u8 mysteryGiftActive;
    s32 dwcProfileId;
    u8 padding_50[0xC];
} SysInfo;
LAYOUT_CHECK(SysInfo_size, sizeof(SysInfo) == 0x5C);
LAYOUT_CHECK_OFFSET(SysInfo_rtc_info, SysInfo, rtc_info, 0x10);

u32 Save_SysInfo_sizeof(void);
void Save_SysInfo_Init(void *sys_info);
SysInfo *Save_SysInfo_Get(SaveData *saveData);
SysInfo_RTC *Save_SysInfo_RTC_Get(SaveData *saveData);
void Save_SysInfo_InitFromSystem(SysInfo *sys_info);
BOOL Save_SysInfo_MacAddressIsMine(SysInfo *sys_info);
BOOL Save_SysInfo_RTCOffsetIsMine(SysInfo *sys_info);
u8 Save_SysInfo_GetBirthMonth(SysInfo *sys_info);
u8 Save_SysInfo_GetBirthDay(SysInfo *sys_info);
u8 Save_SysInfo_GetMysteryGiftActive(SysInfo *sys_info);
void Save_SysInfo_SetMysteryGiftActive(SysInfo *sys_info, u8);
s32 Save_SysInfo_GetDwcProfileId(SysInfo *sys_info);
void Save_SysInfo_SetDwcProfileId(SysInfo *sys_info, s32);
void Save_SysInfo_RTC_Init(SysInfo_RTC *rtc_info);
BOOL Save_SysInfo_RTC_HasPenalty(SysInfo_RTC *rtc_info);
void Save_SysInfo_RTC_DecrementPenalty(SysInfo_RTC *rtc_info, u32);
void SysInfoRTC_HandleContinueOnNewConsole(SysInfo_RTC *rtc_info);

#endif
