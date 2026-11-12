// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/sin_wave_table.h.
#ifndef BWPORT_SYSTEM_SIN_WAVE_TABLE_H
#define BWPORT_SYSTEM_SIN_WAVE_TABLE_H

#include "types.h"
#include "nitro/fx/fx.h"

// A table of a sine wave (sin_wave_table.c, a guessed name)

// Fills count entries with the whole part of amplitude times the sine, the angle starting at 0 and growing by step
// from each entry to the next, in FX_SinIdx's units (pokebw2's SinWaveTable_Make)
void func_02024230(s16 *table, u32 count, u16 step, fx32 amplitude);

#endif // BWPORT_SYSTEM_SIN_WAVE_TABLE_H
