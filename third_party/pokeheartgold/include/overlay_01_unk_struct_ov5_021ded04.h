#ifndef POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021DED04_H
#define POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021DED04_H

#include "global.h"
#include "linear_interpolation_task.h"

struct HBlankSystem;
struct HBlankTask;
struct SysTask;

// The banner (window + HBlank) effect of ov01_021F0B44 / ov01_021F0B78 / ov01_021F0B5C.
typedef struct UnkStruct_ov5_021DED04 {
    LinearInterpolationTaskS32 unk_00;
    u32 unk_14;
    u8 unk_18[192];
    struct HBlankSystem *unk_D8;
    struct HBlankTask *unk_DC;
    struct SysTask *unk_E0;
    BOOL *unk_E4;
} UnkStruct_ov5_021DED04;

// sizeof(UnkStruct_ov5_021DED04) == 0xe8 on the DS

#endif // POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021DED04_H
