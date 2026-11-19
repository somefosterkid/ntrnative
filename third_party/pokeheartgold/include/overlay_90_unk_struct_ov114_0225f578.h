#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225F578_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225F578_H

// UnkStruct_ov114_0225F578: screen data + 15 sprites; one of the sub-objects of UnkStruct_ov114_0225E854_t (the ov90 Wi-Fi Plaza minigame app, 0x664)

#include "global.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"

struct NNSG2dScreenData;
struct Sprite;

typedef struct UnkStruct_ov114_0225F578 {
    void *unk_00;
    struct NNSG2dScreenData *unk_04;
    struct Sprite *unk_08[15];
    UnkStruct_ov114_0225CDB4 *unk_44;
    u8 unk_48;
    u8 unk_49;
    u8 unk_4A;
    u8 unk_4B;
} UnkStruct_ov114_0225F578;

// sizeof(UnkStruct_ov114_0225F578) == 0x4c

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225F578_H
