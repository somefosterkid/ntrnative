// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/misc_savedata.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_MISC_SAVEDATA_H
#define BWPORT_SAVE_MISC_SAVEDATA_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// pokebw2's Misc_GetFlag
BOOL func_0200edec(RivalDataSave *data, int flag);
// pokebw2's Misc_SetFlag
void func_0200ee14(RivalDataSave *data, int flag);

// Black 2's, not checked against Black
#define MISC_MET_TRAINER_MAX 32

#define MISC_FLAG_COUNT 256

// Black 2's, not checked against Black
#define MISC_RIVAL_NAME_LENGTH 8
#define HIDDEN_HOLLOW_COUNT 20

// Black 2's, not checked against Black
// What a Hidden Hollow holds
struct HiddenHollow {
    u16 filled : 1;
    u16 slot : 4;
    u16 group : 3;
    u16 gender : 7;
};

// Black 2's; twinned code confirms the offsets of metTrainers, metTrainerCount and flags in Black
struct RivalDataSave {
    // The IDs of the trainers met last, the newest last
    u32 metTrainers[MISC_MET_TRAINER_MAX];
    // The SDK's u32 (unsigned long), which MWCC indexes with differently than unsigned int
    sdk_u32 metTrainerCount;
    u32 flags[MISC_FLAG_COUNT / 32];
    u16 rivalName[MISC_RIVAL_NAME_LENGTH];
    // The data that getHollow_RivalBlk's file keeps
    u8 unkB4[0x18];
    HiddenHollow hollows[HIDDEN_HOLLOW_COUNT];
    u8 hollowNum;
    u8 paddingF5[3];
    u32 unkF8;
};

void clearHollowRivalData(RivalDataSave *data);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The block's size and its initial state
u32 getSizeofHollow_RivalData(void);

#endif // BWPORT_SAVE_MISC_SAVEDATA_H
