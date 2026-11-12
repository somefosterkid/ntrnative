// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/high_link_savedata.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/high_link.h"
#include "field/survey.h"
#include "gfl/std.h"
#include "save/playtime.h"
#include "save/save_control.h"

// The Entralink's save block: the player's levels, the three pass powers in use, the special pass powers received
// and the time played while a pass power ran. The ROM has no string for the file, so the name is a guess.
// Block 0x35.

#define SAVE_BLOCK_HIGH_LINK 0x35
#define HIGH_LINK_LEVEL_MAX 999

// The play time, in seconds, when the count of HighLinkSave_CountPlaySeconds started
static u64 sStartSeconds;

u32 getSizeofProbabilityTable(void) {
    return sizeof(HighLinkSave);
}

void probability(HighLinkSave *save) {
    MI_CpuFill8(save, 0, sizeof(HighLinkSave));
    setupProbabilityTable(&save->levels);
    save->powers[0] = HIGH_LINK_POWER_NONE;
}

void setupProbabilityTable(PassPowerLevel *levels) {
    MI_CpuFill8(levels, 0, sizeof(PassPowerLevel));
    probabilityLoop((SurveyProbabilityState *)levels);
}

u16 func_0200bdf8(void *levels) {
    return ((PassPowerLevel *)levels)->level1;
}

u16 func_0200bdfc(void *levels) {
    return ((PassPowerLevel *)levels)->level2;
}

// Adds to the White level, up to 999
// pokebw2's HighLinkSave_AddWhiteLevel
void func_0200be00(PassPowerLevel *levels, int add) {
    u16 old = levels->level1;

    if (old + add > HIGH_LINK_LEVEL_MAX) {
        levels->level1 = HIGH_LINK_LEVEL_MAX;
    } else {
        levels->level1 += add;
    }
}

// pokebw2's HighLinkSave_AddBlackLevel
void func_0200be24(PassPowerLevel *levels, int add) {
    u16 old = levels->level2;

    if (old + add > HIGH_LINK_LEVEL_MAX) {
        levels->level2 = HIGH_LINK_LEVEL_MAX;
    } else {
        levels->level2 += add;
    }
}

HighLinkSave *getHighLinkBlockAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_HIGH_LINK);
}

void func_0200be54(SaveControl *save, const PassPowerLevel *levels) {
    getHighLinkBlockAddress(save)->levels = *levels;
}

void func_0200be6c(SaveControl *save, PassPowerLevel *levels) {
    *levels = getHighLinkBlockAddress(save)->levels;
}

