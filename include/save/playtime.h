// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/playtime.h: the lines in use so far.
#ifndef BWPORT_SAVE_PLAYTIME_H
#define BWPORT_SAVE_PLAYTIME_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

struct PlayTime {
    u16 hours;
    u8 minutes;
    u8 seconds;
    // The date and time of the last save
    union {
        u32 lastSaved;
        struct {
            u32 year : 7;
            u32 month : 4;
            u32 day : 5;
            u32 hour : 5;
            u32 minute : 6;
        };
    };
};

void func_020085a0(PlayTime *time);

// Black 2's, not checked against Black
#define PLAYTIME_MAX_HOURS 999

PlayTime *setupPlaytimeCounters(HeapID heapId);
// Sets the date and time of the last save to now
void func_02008658(PlayTime *time);
u32 func_020086c8(PlayTime *time);
u32 func_020086d0(PlayTime *time);
u32 func_020086d8(PlayTime *time);
u32 func_020086e0(PlayTime *time);
u32 func_020086e8(PlayTime *time);
void func_020086f8(PlayTime *time, u32 *copy);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Adds seconds to the play time, which stops at PLAYTIME_MAX_HOURS:59:59
void updateGameTime(PlayTime *time, u32 addSeconds);
void func_0200863c(PlayTime *dest, const PlayTime *src);
u16 func_0200864c(PlayTime *time);
u8 func_02008650(PlayTime *time);
u8 func_02008654(PlayTime *time);
// Copy the word with the date and time of the last save out and back
void func_020086f0(PlayTime *time, u32 *copy);

#endif // BWPORT_SAVE_PLAYTIME_H
