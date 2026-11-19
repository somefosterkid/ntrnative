#ifndef POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021E5128_H
#define POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021E5128_H

#include "global.h"
#include "linear_interpolation_task.h"

struct Sprite;

// The mugshot effect's four scale-up sprites (ov115_0225F020 creates them, ov115_0225F0B4 animates them).
typedef struct UnkStruct_ov5_021E5128 {
    s16 unk_00;
    s16 unk_02;
    struct Sprite *unk_04[4];
    LinearInterpolationTaskFX32 unk_14[4];
} UnkStruct_ov5_021E5128;

// sizeof(UnkStruct_ov5_021E5128) == 0x64 on the DS

#endif // POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021E5128_H
