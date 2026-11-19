#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "library/spl_emitter.h"

#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_manager.h"

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

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BOOL ov07_022222F0(XYTransformContext *ctx, ManagedSprite *sprite);

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

void ov07_02232F74(BallRotation *param0, int param1);

BOOL ov07_022331B0(BallRotation *param0);

struct ApplicationManager;
typedef struct ApplicationManager ApplicationManager;
struct ApplicationManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};
BOOL ov07_02233224(ApplicationManager *appMan, int *state);

BOOL ov07_022331B0(BallRotation *param0) {
    switch (param0->unk_08) {
    case 0:
        if ((++param0->unk_0C) < 1) {
            break;
        }

        {
            s16 v0, v1;

            ManagedSprite_GetPositionXY(param0->unk_30, &v0, &v1);
            ov07_02222268(&param0->unk_48[0], v0, v0, v1, v1 + 32, 32 / 3);

            param0->unk_08++;
        }
        break;
    case 1:
        if (ov07_022222F0(&param0->unk_48[0], param0->unk_30) == 0) {
            param0->unk_08++;

            ov07_02232F74(param0, 13);
        }
        break;
    default:
        break;
    }

    return 1;
}

BOOL ov07_02233224(ApplicationManager *appMan, int *state) {
    return 1;
}
