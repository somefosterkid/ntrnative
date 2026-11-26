#include "global.h"
#include "simple3dmodel.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_simple_renderers.h"

struct FieldEffectManager;
struct Pokemon;
struct Pokepic;
struct PokepicDrawParam;
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
void Pokepic_Delete(Pokepic *monSprite);
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
enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};
void ov41_022497A0(UnkStruct_020298D8 *param0, int *param1, int *param2);
void ov41_02249978(TouchScreenRect *rect, int param1, int param2, int param3, int param4);
void TouchscreenHitbox_TouchHeldIsIn(Simple3DModel *model);
#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL TouchscreenHitbox_PointIsIn(Simple3DModel *model);
#else
void TouchscreenHitbox_PointIsIn(Simple3DModel *model);
#endif

void ov41_022496E8(UnkStruct_020298D8 *param0);
void ov41_02249700(UnkStruct_020298D8 *param0, int param1);
int ov41_02249710(UnkStruct_020298D8 *param0);
void ov41_0224971C(UnkStruct_020298D8 *param0, int param1, int param2);
void ov41_02249768(UnkStruct_ov5_021F4DD8 *param0);
void ov41_02249774(UnkStruct_ov5_021F4DD8 *param0);
void ov41_02249780(UnkStruct_020298D8 *param0, int *xCenter, int *yCenter);
void ov41_022497A0(UnkStruct_020298D8 *param0, int *param1, int *param2);

void ov41_022496E8(UnkStruct_020298D8 *param0) {
    Pokepic_Delete(param0->unk_00);
    memset(param0, 0, sizeof(UnkStruct_020298D8));
}

void ov41_02249700(UnkStruct_020298D8 *param0, int param1) {
    Pokepic_SetAttr(param0->unk_00, MON_SPRITE_Z_CENTER, param1);
}

int ov41_02249710(UnkStruct_020298D8 *param0) {
    return Pokepic_GetAttr(param0->unk_00, MON_SPRITE_Z_CENTER);
}

void ov41_0224971C(UnkStruct_020298D8 *param0, int param1, int param2) {
    int v0, v1;

    ov41_022497A0(param0, &v0, &v1);
    Pokepic_SetAttr(param0->unk_00, MON_SPRITE_X_CENTER, param1);
    Pokepic_SetAttr(param0->unk_00, MON_SPRITE_Y_CENTER, param2);

    v1 /= 2;
    v0 /= 2;

    ov41_02249978(&param0->unk_04, param1, param2, v0, v1);
}

void ov41_02249768(UnkStruct_ov5_021F4DD8 *param0) {
    TouchscreenHitbox_TouchHeldIsIn(&param0->unk_04);
}

void ov41_02249774(UnkStruct_ov5_021F4DD8 *param0) {
    TouchscreenHitbox_PointIsIn(&param0->unk_04);
}

void ov41_02249780(UnkStruct_020298D8 *param0, int *xCenter, int *yCenter) {
    *xCenter = Pokepic_GetAttr(param0->unk_00, MON_SPRITE_X_CENTER);
    *yCenter = Pokepic_GetAttr(param0->unk_00, MON_SPRITE_Y_CENTER);
}

void ov41_022497A0(UnkStruct_020298D8 *param0, int *param1, int *param2) {
    *param1 = 80;
    *param2 = 80;
}
