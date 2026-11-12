// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/sin_wave_table.c, which is the same code
// in Black 2/White 2. The name is Black's dsd name; pokebw2's guess is in the comment.
#include "types.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_trig.h"
#include "system/sin_wave_table.h"

// A table of a sine wave. The file's name is a guess: the ROM has no string for it

// SinWaveTable_Make
void func_02024230(s16 *table, u32 count, u16 step, fx32 amplitude) {
    u32 i;
    u16 angle = 0;

    for (i = 0; i < count; i++) {
        table[i] = FX_Whole(FX_Mul(FX_SinIdx(angle), amplitude));
        angle += step;
    }
}
