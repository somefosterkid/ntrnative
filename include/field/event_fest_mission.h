// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_fest_mission.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_EVENT_FEST_MISSION_H
#define BWPORT_FIELD_EVENT_FEST_MISSION_H

#include "types.h"
#include "field/festival.h"
#include "gfl/overlay.h"

// Black 2's, not checked against Black
#define OVERLAY_FUNFEST_MISSION OVERLAY_ID(157)

// Black 2's, not checked against Black
struct FestMissionEventArgs {
    FestMissionConfig config;
    // Nonzero when the player joins another's mission, 0 when the player hosts it
    u32 joined;
    // Which message EventFestMissionMessage_Create shows, from message 4
    u32 messageType;
};

#endif // BWPORT_FIELD_EVENT_FEST_MISSION_H
