#ifndef POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225A428_H
#define POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225A428_H

#include "global.h"

#include "overlay_41_unk_struct_ov22_0225af68.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"

struct BgConfig;
struct Window;
struct SpriteList;
struct SpriteResourceCollection;
struct SpriteResource;
struct Sprite;
struct String;

typedef struct UnkStruct_ov22_0225A914 {
    struct SpriteResource *unk_00[4];
    struct Sprite *unk_10[20];
    int unk_60;
    int unk_64;
} UnkStruct_ov22_0225A914;

typedef struct UnkStruct_ov22_0225A428 {
    UnkStruct_ov22_022597BC unk_00;
    struct Window *unk_2C;
    struct SpriteList *unk_30;
    struct SpriteResourceCollection **unk_34;
    UnkStruct_ov22_0225A914 unk_38;
    UnkStruct_ov22_0225AB54 unk_A0;
    struct Window *unk_134;
    int unk_138;
    int unk_13C;
    u32 unk_140;
    struct String *unk_144;
} UnkStruct_ov22_0225A428;

// sizeof(UnkStruct_ov22_0225A428) == 0x148 on DS

#endif // POKEHEARTGOLD_OVERLAY_41_UNK_STRUCT_OV22_0225A428_H
