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
#include "overlay_41_unk_struct_ov22_02258A48.h"
#include "touchscreen.h"
#include "constants/heap.h"
#include "overlay_41_unk_struct_ov22_022597BC.h"

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


typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

void ov41_02249C20(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, int *param3, int *param4);

void ov41_0224946C(UnkStruct_ov22_02258A48 *param0, int *param1, int *param2, int *param3, int *param4);

void ov41_0224946C(UnkStruct_ov22_02258A48 *param0, int *param1, int *param2, int *param3, int *param4) {
    UnkStruct_ov22_02258EE4 *v0 = param0->unk_00;

    ov41_02249C20(v0->unk_10, param1, param2, param3, param4);
}
