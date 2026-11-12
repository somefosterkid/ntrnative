// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/c_gear_phrase_savedata.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "system/pms.h"
#include "save/save_control.h"

// The C-Gear's phrases, at the start of the C-Gear's save block, which c_gear_savedata.c owns: their initial state
// and their accessors. The ROM has no string for the file, so the name is a guess. Block 0x20.

#define SAVE_BLOCK_CGEAR 0x3e

// The phrases the C-Gear keeps (pokebw2's CGEAR_PHRASE_MAX)
#define CGEAR_PHRASE_MAX 5

// The size of the phrases' save block
u32 func_0200e6b0(void) {
    return sizeof(PMSData) * CGEAR_PHRASE_MAX;
}

// pokebw2's CGearPhrases_Init
void func_0200e6b4(PMSData *phrases) {
    func_02024344(&phrases[0]);
    func_020242d8(&phrases[1], 0);
    func_020242d8(&phrases[2], 1);
    func_020242d8(&phrases[3], 2);
    func_020242d8(&phrases[4], 3);
}

// pokebw2's CGearPhrases_GetBlock
void *func_0200e6e8(SaveControl *save) {
    return getCGearDataBlkAddress(save);
}

void *getCGearDataBlkAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_CGEAR);
}

// pokebw2's CGearPhrases_Get
void func_0200e6fc(void *cgear, u32 index, PMSData *sentence) {
    PMSData *phrases = cgear;

    *sentence = phrases[index];
}

// pokebw2's CGearPhrases_Set
void func_0200e714(void *cgear, u32 index, const PMSData *sentence) {
    PMSData *phrases = cgear;

    phrases[index] = *sentence;
}
