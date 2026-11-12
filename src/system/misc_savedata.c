// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/misc_savedata.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/misc_savedata.h"
#include "field/union_oneself.h"
#include "gfl/std.h"
#include "save/save_control.h"
#include "system/str_tool.h"

// A save block of several small things: the IDs of the trainers the player met last, a set of flags, the rival's
// name and what the Hidden Hollows hold. The ROM has no string for the file, so the name is a guess. Block 0x44 in Black.

#define SAVE_BLOCK_MISC 0x44 // Black's

u32 getSizeofHollow_RivalData(void) {
    return sizeof(RivalDataSave);
}

void clearHollowRivalData(RivalDataSave *data) {
    MI_CpuFill8(data, 0, sizeof(RivalDataSave));
}

RivalDataSave *getHollow_RivalData(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_MISC);
}

// Puts a trainer at the end of the list of the trainers met last
// pokebw2's Misc_AddMetTrainer
void func_0200ed54(RivalDataSave *rivalData, u32 id) {
    u32 i;

    for (i = 0; i < rivalData->metTrainerCount; i++) {
        if (id == rivalData->metTrainers[i]) {
            int after = rivalData->metTrainerCount - 1 - i;
            if (after > 0) {
                MI_CpuCopy8(&rivalData->metTrainers[i + 1], &rivalData->metTrainers[i], after * sizeof(u32));
                rivalData->metTrainers[rivalData->metTrainerCount - 1] = id;
            }
            return;
        }
    }
    if (rivalData->metTrainerCount < MISC_MET_TRAINER_MAX) {
        rivalData->metTrainers[rivalData->metTrainerCount] = id;
        rivalData->metTrainerCount++;
    } else {
        MI_CpuCopy8(&rivalData->metTrainers[1], &rivalData->metTrainers[0], (MISC_MET_TRAINER_MAX - 1) * sizeof(u32));
        rivalData->metTrainers[MISC_MET_TRAINER_MAX - 1] = id;
        rivalData->metTrainerCount = MISC_MET_TRAINER_MAX;
    }
}

// pokebw2's Misc_IsMetTrainer
BOOL func_0200edc4(RivalDataSave *rival, u32 trainerId) {
    u32 i;

    for (i = 0; i < rival->metTrainerCount; i++) {
        if (trainerId == rival->metTrainers[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's Misc_GetFlag
BOOL func_0200edec(RivalDataSave *data, int flag) {
    return (data->flags[flag / 32] >> (flag % 32)) & 1;
}

// pokebw2's Misc_SetFlag
void func_0200ee14(RivalDataSave *data, int flag) {
    if (flag < MISC_FLAG_COUNT) {
        data->flags[flag / 32] |= 1 << (flag % 32);
    }
}
