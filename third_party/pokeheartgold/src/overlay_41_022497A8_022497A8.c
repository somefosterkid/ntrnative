#include "global.h"

#include "sys_task.h"
#include "system.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "screen_fade_internal.h"
#include "pokepic.h"

typedef struct System System;

struct Pokemon;

struct Pokepic;

struct PokepicShadow;

struct PokepicTemplate;

struct PokepicDrawParam;

struct SpriteAnimFrame;

struct SysTask;

struct SysTaskQueue;

struct System;

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

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

BOOL ov41_02249768(UnkStruct_020298D8 *param0);

void ov41_02249780(UnkStruct_020298D8 *param0, int *xCenter, int *yCenter);

void ov41_022497A0(UnkStruct_020298D8 *param0, int *param1, int *param2);

int ov41_022464BC(NNSG2dCharacterData *param0, int param1, int param2, int param3);

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};

BOOL ov41_022497A8(UnkStruct_020298D8 *param0, int *param1, int *param2, NNSG2dCharacterData *param3);

BOOL ov41_022497A8(UnkStruct_020298D8 *param0, int *param1, int *param2, NNSG2dCharacterData *param3) {
    int v0;
    int v1, v2;
    int v3, v4;

    v0 = ov41_02249768(param0);

    if (v0 == 0) {
        return 0;
    }

    ov41_02249780(param0, &v3, &v4);
    ov41_022497A0(param0, &v1, &v2);

    v3 -= (v1 / 2);
    v4 -= (v2 / 2);

    *param1 = gSystem.touchX - v3;
    *param2 = gSystem.touchY - v4;

    v0 = ov41_022464BC(param3, *param1, *param2, 0);

    if (v0 == 0) {
        return 1;
    }

    return 0;
}
