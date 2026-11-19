#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225DFFC_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225DFFC_H

// UnkStruct_ov114_0225DFFC: HBlank scroll double buffer (two 192-line tables at +4/+0x184); one of the sub-objects of UnkStruct_ov114_0225E854_t (the ov90 Wi-Fi Plaza minigame app, 0x664)

#include "global.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"

struct BufferManager;

typedef struct UnkStruct_ov114_0225DFFC {
    UnkStruct_ov114_0225CEF0 *unk_00;
    s16 unk_04[2][192];
    struct BufferManager *bufferManager;
} UnkStruct_ov114_0225DFFC;

// sizeof(UnkStruct_ov114_0225DFFC) == 0x308

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225DFFC_H
