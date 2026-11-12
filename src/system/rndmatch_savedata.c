// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/rndmatch_savedata.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/records.h"
#include "gfl/std.h"

// The results of the random matches and the competitions, which the record block (records.h) holds at 0x5a: three
// counts for each of 11 kinds of match. The ROM has no string for the file, so the name is a guess.

// pokebw2's RndMatchData_Init
void func_0200dfcc(void *results) {
    MI_CpuFill8(results, 0, sizeof(RndMatchData));
}

// pokebw2's RndMatchData_GetParam
u16 func_0200dfd8(void *results, u32 index, u32 field) {
    RndMatchData *data = results;

    return data->values[index][field];
}

// pokebw2's RndMatchData_SetParam
void func_0200dfe4(void *results, u32 index, u32 field, s32 value) {
    RndMatchData *data = results;

    if (value > RNDMATCH_VALUE_MAX) {
        value = RNDMATCH_VALUE_MAX;
    } else if (value < 0) {
        value = 0;
    }
    data->values[index][field] = value;
}

// pokebw2's RndMatchData_IncParam
void func_0200e008(void *results, u32 index, u32 field) {
    RndMatchData *data = results;

    if (data->values[index][field] < RNDMATCH_VALUE_MAX) {
        data->values[index][field]++;
    }
}
