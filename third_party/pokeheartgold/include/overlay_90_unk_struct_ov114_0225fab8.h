#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225FAB8_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225FAB8_H

#include "sprite.h"
#include "sys_task_api.h"

// HBlank window effect of overlay 90 (Platinum ov114): unk_0C[0] is the per-line window-0 mask the HBlank
// callback reads, unk_0C[1] the next one, copied over on VBlank.
typedef struct UnkStruct_ov114_0225FAB8 {
    Sprite *unk_00[2];
    s16 unk_08;
    s16 unk_0A;
    u8 unk_0C[2][192];
    SysTask *unk_18C;
    GXWndPlane unk_190;
    GXWndPlane battlerType;
    int unk_194;
} UnkStruct_ov114_0225FAB8;

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225FAB8_H
