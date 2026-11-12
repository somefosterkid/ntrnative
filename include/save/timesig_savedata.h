// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/timesig_savedata.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SAVE_TIMESIG_SAVEDATA_H
#define BWPORT_SAVE_TIMESIG_SAVEDATA_H

#include "types.h"
#include "struct_decls.h"

void *getTimeSigBlkAddress(SaveControl *save);
void saveSecondsTime(TimeSigSave *timeSig, u32 low, u32 high);
void setBadgeGetSecondsTime(TimeSigSave *timeSig, u32 badgeId, u32 year, u32 month, u32 day);

// Black 2's, not checked against Black
#define TIMESIG_BADGE_COUNT 8

// Black 2's; twinned code confirms the offsets of year, month and day in Black
// The year, month and day a badge was obtained on
typedef struct {
    u8 year;
    u8 month;
    u8 day;
    u8 unk3;
} TimeSigBadgeDate;

// Black 2's; matching code confirms the offsets of signature, unk600, unk602, unk603, badgeDates, seconds, secondsLow,
// secondsHigh and unk638, and its size in Black
// The save block 0x21 (timesig_savedata.c, a guessed name): the trainer card's signature, when each badge was
// obtained and the time it was last saved at
struct TimeSigSave {
    u32 signature[0x180];
    u16 unk600;
    u8 unk602;
    u8 unk603;
    TimeSigBadgeDate badgeDates[TIMESIG_BADGE_COUNT];
    union {
        s64 seconds;
        struct {
            u32 secondsLow;
            u32 secondsHigh;
        };
    };
    u8 unk62C[0xc];
    u16 unk638[TIMESIG_BADGE_COUNT];
    u8 unk648[0x10];
};

s64 loadSecondsTime(TimeSigSave *timeSig);
u32 func_020089a8(void);
void func_020089b0(TimeSigSave *timeSig);
BOOL func_020089d0(TimeSigSave *timeSig);
u8 func_020089f4(TimeSigSave *timeSig);
void func_02008a00(TimeSigSave *timeSig);
u16 func_02008a0c(TimeSigSave *timeSig);
void func_02008a14(TimeSigSave *timeSig, u32 value);
void func_02008a1c(TimeSigSave *timeSig, u8 value);
BOOL func_02008a28(TimeSigSave *timeSig);
u16 func_02008acc(TimeSigSave *timeSig, u32 index);
void func_02008adc(TimeSigSave *timeSig, u32 index, u32 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The block's first 0x600 bytes, which the trainer card copies
void *func_020089cc(void *timeSig);
TimeSigBadgeDate func_02008a54(TimeSigSave *timeSig, u32 badgeId);

#endif // BWPORT_SAVE_TIMESIG_SAVEDATA_H
