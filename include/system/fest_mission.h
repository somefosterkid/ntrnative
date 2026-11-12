// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/fest_mission.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_FEST_MISSION_H
#define BWPORT_SYSTEM_FEST_MISSION_H

#include "types.h"

// Black 2's, not checked against Black
#define FEST_MISSION_NONE 0x7f

// Black 2's, not checked against Black
// The part of a Funfest mission that its beacons send: the first 0x14 bytes of a FestMissionConfig, laid out as
// FestMission's (field/fest_mission_data.h)
typedef struct {
    // FEST_MISSION_NONE when there is no mission
    u32 index : 7;
    u32 level : 3;
    u32 target : 10;
    u32 unk0_20 : 10;
    u32 unk0_30 : 1;
    u32 unk0_31 : 1;
    // The item of the mission's reward
    u32 unk4_0 : 10;
    u32 unk4_10 : 7;
    // The total score the mission wants
    u32 count : 14;
    u32 unk4_31 : 1;
    u32 unk8_0 : 18;
    u32 unk8_18 : 12;
    u32 unk8_30 : 2;
    // What a log entry of the mission's beacon is about: an item, a move, a Pass Power or a species
    u32 unkC_0 : 10;
    // The mission's seconds left
    u32 seconds : 12;
    // A second item, or 0
    u32 unkC_22 : 10;
    u16 unk10_0 : 6;
    u16 unk10_6 : 3;
    // Which of FestMission_ExtraSeconds a gain of time counts
    u16 unk10_9 : 2;
    u16 unk10_11 : 5;
    u8 messageId;
    u8 resultMessageId;
} FestMissionInfo;

#endif // BWPORT_SYSTEM_FEST_MISSION_H
