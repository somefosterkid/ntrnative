// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/event_work.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/event_work.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "gfl/std.h"

// The event work save block: the work values and the flags of the scripts. The temporary flags, from
// EVENT_FLAG_TEMP_0, are not saved.
// The file's name is a guess, from what it does: the ROM has no string for it. The functions' names are swan's

static u8 *EventWork_GetFlagBytePtr(EventWork *eventWork, u16 flag);

// pokebw2's sTempFlags
static u8 data_02146268[EVENT_FLAG_TEMP_COUNT / 8];

u32 EventWork_GetBlockSize(void) {
    return sizeof(EventWork);
}

void EventWork_Init(EventWork *eventWork) {
    MI_CpuFill8(eventWork, 0, sizeof(EventWork));
}

BOOL EventWork_FlagGet(EventWork *eventWork, u16 flag) {
    u8 *byte = EventWork_GetFlagBytePtr(eventWork, flag);

    if (byte != NULL && (*byte & (1 << (flag % 8)))) {
        return TRUE;
    }
    return FALSE;
}

void EventWork_FlagSet(EventWork *eventWork, u16 flag) {
    u8 *byte = EventWork_GetFlagBytePtr(eventWork, flag);

    if (byte != NULL) {
        *byte |= 1 << (flag % 8);
    }
}

void EventWork_FlagReset(EventWork *eventWork, u16 flag) {
    u8 *byte = EventWork_GetFlagBytePtr(eventWork, flag);

    if (byte != NULL) {
        *byte &= 0xff ^ (1 << (flag % 8));
    }
}

void EventWork_FlagResetRange(EventWork *eventWork, u16 first, u16 last) {
    int flag;

    for (flag = first; flag <= last; flag++) {
        EventWork_FlagReset(eventWork, flag);
    }
}

static u8 *EventWork_GetFlagBytePtr(EventWork *eventWork, u16 flag) {
    if (flag == 0) {
        return NULL;
    }
    if (flag < EVENT_FLAG_TEMP_0) {
        if (flag / 8 >= (int)sizeof(eventWork->flags)) {
            return NULL;
        }
        return &eventWork->flags[flag / 8];
    }
    if ((flag - EVENT_FLAG_TEMP_0) / 8 >= (int)sizeof(data_02146268)) {
        return NULL;
    }
    return &data_02146268[(flag - EVENT_FLAG_TEMP_0) / 8];
}

u16 *EventWork_GetWkPtr(EventWork *eventWork, u16 work) {
    return &eventWork->works[work - EVENT_WORK_0x4000];
}

// Clears the work values from first up to, but not including, last
void EventWork_WorkResetRange(EventWork *eventWork, u16 first, u16 last) {
    MI_CpuFill8(EventWork_GetWkPtr(eventWork, first), 0, (last - first) * sizeof(u16));
}

void EventWork_ResetDailyFlags(EventWork *eventWork) {
    EventWork_FlagResetRange(eventWork, EVENT_FLAG_DAILY_0x0aa0, EVENT_FLAG_DAILY_0x0b5f);
    eventWork->hiddenItemRespawn = TRUE;
}

BOOL EventWork_GetHiddenItemRespawnFlag(EventWork *eventWork) {
    u8 flag = eventWork->hiddenItemRespawn;

    eventWork->hiddenItemRespawn = FALSE;
    return flag;
}
