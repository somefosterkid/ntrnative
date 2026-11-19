#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "pokepic.h"

struct Pokemon;

struct Pokepic;

struct PokepicShadow;

struct PokepicTemplate;

struct PokepicDrawParam;

struct SoftwareSprite;

struct SpriteAnimFrame;

struct UnkStruct_ov22_02259560_t;


typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;

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

typedef struct {
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;

void ov41_02246014(UnkStruct_ov22_02255040 *param0, int param1);

void ov41_02249700(UnkStruct_020298D8 *param0, int param1);

void ov41_02249A90(UnkStruct_ov22_02259560 *param0, int param1);

void ov41_02249A90(UnkStruct_ov22_02259560 *param0, int param1) {
    if (param0->unk_04 < 3) {
        UnkStruct_ov22_02255040 *v0 = param0->unk_00;
        ov41_02246014(v0, param1);
    } else {
        UnkStruct_020298D8 *v1 = param0->unk_00;
        ov41_02249700(v1, param1);
    }
}
