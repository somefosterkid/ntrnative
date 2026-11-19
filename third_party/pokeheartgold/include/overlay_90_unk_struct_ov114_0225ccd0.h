#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225CCD0_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225CCD0_H

// UnkStruct_ov114_0225CCD0: the 4 sprite resource managers + resource set array; one of the sub-objects of UnkStruct_ov114_0225E854_t (the ov90 Wi-Fi Plaza minigame app, 0x664)

#include "global.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"

struct GF_2DGfxResMan;

typedef struct UnkStruct_ov114_0225CCD0 {
    struct GF_2DGfxResMan *unk_00[4];
    UnkStruct_ov114_0225CDB4 *unk_10;
    u32 unk_14;
} UnkStruct_ov114_0225CCD0;

// sizeof(UnkStruct_ov114_0225CCD0) == 0x18

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225CCD0_H
