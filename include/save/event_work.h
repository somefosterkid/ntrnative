// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/event_work.h, the lines in use so far.
#ifndef BWPORT_SAVE_EVENT_WORK_H
#define BWPORT_SAVE_EVENT_WORK_H

#include "types.h"
#include "struct_decls.h"

BOOL EventWork_FlagGet(EventWork *eventWork, u16 flag);
void EventWork_FlagReset(EventWork *eventWork, u16 flag);
// Whether the hidden items that come back may come back today
BOOL EventWork_GetHiddenItemRespawnFlag(EventWork *eventWork);
void EventWork_FlagSet(EventWork *eventWork, u16 flag);

// Black's, which event_work.c's code confirms (Black 2 has 431 and 3064): the saved event work values,
// EVENT_WORK_0x4000 to EVENT_WORK_0x413d, and the saved event flags
#define EVENT_WORK_COUNT 318
#define EVENT_FLAG_SAVED_COUNT 2912
// The flags from EVENT_FLAG_TEMP_0, which the save does not hold
#define EVENT_FLAG_TEMP_COUNT 64

struct EventWork {
    u16 works[EVENT_WORK_COUNT];
    u8 flags[EVENT_FLAG_SAVED_COUNT / 8];
    // Set with the day's flags reset, cleared when read
    u8 hiddenItemRespawn;
}; // 0x3ea

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void EventWork_FlagResetRange(EventWork *eventWork, u16 first, u16 last);
u16 *EventWork_GetWkPtr(EventWork *eventWork, u16 work);
void EventWork_WorkResetRange(EventWork *eventWork, u16 first, u16 last);
u32 EventWork_GetBlockSize(void);
void EventWork_Init(EventWork *eventWork);
// Clears the flags and work values that last a day
void EventWork_ResetDailyFlags(EventWork *eventWork);

#endif // BWPORT_SAVE_EVENT_WORK_H
