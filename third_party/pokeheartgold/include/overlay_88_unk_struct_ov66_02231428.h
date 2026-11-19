#ifndef POKEHEARTGOLD_OVERLAY_88_UNK_STRUCT_OV66_02231428_H
#define POKEHEARTGOLD_OVERLAY_88_UNK_STRUCT_OV66_02231428_H

#include "global.h"

// UnkStruct_ov66_02231428: a 4-byte time value (hours, minutes, seconds in bytes 0..2); every user with a real
// layout (ov45/ov48/ov49/ov88 wifi plaza code) embeds it at a 4-byte stride, so it was never 0xc bytes.
typedef struct UnkStruct_ov66_02231428 {
    union {
        u32 unk_00_val1;
        struct {
            u8 unk_00_val2_unk_00;
            u8 unk_00_val2_unk_01;
            u8 unk_00_val2_unk_02;
            u8 unk_00_val2_unk_03;
        };
    };
} UnkStruct_ov66_02231428;

// sizeof(UnkStruct_ov66_02231428) == 0x4

#endif // POKEHEARTGOLD_OVERLAY_88_UNK_STRUCT_OV66_02231428_H
