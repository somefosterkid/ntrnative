#ifndef POKEHEARTGOLD_FRONTIER_MAP_H
#define POKEHEARTGOLD_FRONTIER_MAP_H

#include "overlay_42.h"
#include "bg_window.h"
#include "palette.h"

typedef struct FrontierMap {
    struct BgConfig *bgConfig;
    PaletteData *paletteData;
    void *unk8;
    void *unkC;
    void *unk10;
} FrontierMap;

typedef struct UnkStruct_02239938 {
    u32 unk0;
    UnkStruct_ov42_0222903C *unk4;
} UnkStruct_02239938;

UnkStruct_02239938 *ov80_02239938(void *a0, u32 a1);

#endif
