#include "global.h"

#include "battle/battle.h"
#include "library/spl_emitter.h"

#include "sprite_system.h"
#include "sys_task.h"

struct BallRotation;

struct BattleMonOBJData;

struct PaletteFadeContext;

struct ParticleSystem;

struct UnkStruct_ov12_02236648_t;

typedef struct BallRotation BallRotation;

typedef struct UnkStruct_ov12_02236648_t UnkStruct_ov12_02236648;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    int unk_04;
    enum HeapID heapID;
    int unk_0C;
    int unk_10;
} UnkStruct_ov12_02236690;

typedef struct BattleMonOBJData BattleMonOBJData;

typedef struct ParticleSystem ParticleSystem;

typedef struct ValueLerpContext {
    s32 value;
    s32 data[4];
} ValueLerpContext;

typedef struct XYTransformContext {
    s16 x;
    s16 y;
    s32 data[8];
} XYTransformContext;

typedef struct PaletteFadeContext PaletteFadeContext;

void ov07_022223F0(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

BOOL ov07_02222440(ValueLerpContext *ctx);

struct UnkStruct_ov12_02236648_t {
    UnkStruct_ov12_02236690 unk_00;
    SPLEmitter *unk_14;
    ParticleSystem *unk_18;
    int unk_1C;
    int unk_20;
};

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov12_02237C54_sub1;

struct BallRotation {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    BOOL unk_1C;
    u8 unk_20;
    u8 unk_21;
    s8 unk_22;
    u8 pad_23[1];
    BOOL unk_24;
    BOOL unk_28;
    SpriteManager *unk_2C;
    ManagedSprite *unk_30;
    ValueLerpContext unk_34;
    XYTransformContext unk_48[2];
    struct { s32 type; u32 heapID; s32 mode; s32 target; int ball; s32 bgPrio; s32 surface; struct SpriteSystem *spriteSystem; struct PaletteData *paletteData; struct BattleSystem *battleSystem; } unk_90;
    UnkStruct_ov12_02237C54_sub1 unk_B8;
    SysTask *unk_CC;
    UnkStruct_ov12_02236648 *unk_D0;
    PaletteFadeContext *unk_D4;
    BattleMonOBJData *unk_D8;
    int unk_DC;
};

BOOL ov07_02233C98(BallRotation *param0);

BOOL ov07_02233C98(BallRotation *param0) {
    switch (param0->unk_08) {
    case 0: {
        if (param0->unk_10 == 0) {
            ov07_022223F0(&param0->unk_34, -((45 * 0xffff) / 360), +((45 * 0xffff) / 360), 10);
        } else {
            ov07_022223F0(&param0->unk_34, +((45 * 0xffff) / 360), -((45 * 0xffff) / 360), 10);
        }

        param0->unk_10 ^= 1;
    }
        param0->unk_08++;
        break;
    case 1:
        ManagedSprite_SetAffineZRotation(param0->unk_30, param0->unk_34.value);

        if (ov07_02222440(&param0->unk_34) == 0) {
            if (param0->unk_0C >= 1) {
                param0->unk_08++;
            } else {
                param0->unk_0C++;
                param0->unk_08--;
            }
        }
        break;
    default:
        param0->unk_B8.unk_0C = 0;
        return 0;
        break;
    }

    return 1;
}

BOOL ov07_02233D24(BallRotation *param0);

BOOL ov07_02233D24(BallRotation *param0) {
    if (param0->unk_08 == 0xFF) {
        return 0;
    }

    if (param0->unk_30 != NULL) {
        s16 v0, v1;

        ManagedSprite_GetPositionXY(param0->unk_30, &v0, &v1);

        param0->unk_B8.unk_00 = v0;
        param0->unk_B8.unk_02 = v1;
    }

    return 1;
}
