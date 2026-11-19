#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "gf_3d_loader.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"
#include "overlay_41_unk_struct_ov22_02258ee4.h"

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

struct SpriteAnimFrame;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_ov22_02258A48_t;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

typedef struct UnkStruct_ov22_02258A48_t {
    void *unk_00;
    void (*unk_04)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_08)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_0C)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_10)(struct UnkStruct_ov22_02258A48_t *param0);
    s16 unk_14;
    s16 unk_16;
    u8 unk_18;
    u8 padding[3];
} UnkStruct_ov22_02258A48;

void ov41_02249C20(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, int *param3, int *param4);

void ov41_0224946C(UnkStruct_ov22_02258A48 *param0, int *param1, int *param2, int *param3, int *param4);

void ov41_0224946C(UnkStruct_ov22_02258A48 *param0, int *param1, int *param2, int *param3, int *param4) {
    UnkStruct_ov22_02258EE4 *v0 = param0->unk_00;

    ov41_02249C20(v0->unk_10, param1, param2, param3, param4);
}
