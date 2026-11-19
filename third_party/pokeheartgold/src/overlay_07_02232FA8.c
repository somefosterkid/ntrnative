#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "library/spl_emitter.h"

#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_manager.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"
#include "overlay_07_palette_fade.h"

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

UnkStruct_ov12_02236648 *ov07_02232D20(UnkStruct_ov12_02236690 *param0);

void ov07_02232D80(UnkStruct_ov12_02236648 *param0);

BOOL ov07_02232DF4(UnkStruct_ov12_02236648 *param0);

void ov07_02232E10(UnkStruct_ov12_02236648 *param0);

typedef struct BattleMonOBJData BattleMonOBJData;

BattleMonOBJData *ov07_0221FDFC(BattleSystem *battleSys, enum HeapID heapID);

void ov07_0221FE08(BattleMonOBJData *btlMonObjData);

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
    BallThrow unk_90;
    UnkStruct_ov12_02237C54_sub1 unk_B8;
    SysTask *unk_CC;
    UnkStruct_ov12_02236648 *unk_D0;
    PaletteFadeContext *unk_D4;
    BattleMonOBJData *unk_D8;
    int unk_DC;
};

void ov07_02232F74(BallRotation *param0, int param1);

BOOL ov07_02232FA8(BallRotation *param0);

struct OverlayManager;
BOOL ov07_02233088(OverlayManager *appMan, int *state);

BOOL ov07_02232FA8(BallRotation *param0) {
    switch (param0->unk_08) {
    case 0:
        ManagedSprite_SetAnim(param0->unk_30, 1);
        {
            UnkStruct_ov12_02236690 v0;

            v0.unk_04 = param0->unk_90.ball;
            v0.heapID = param0->unk_90.heapID;
            v0.unk_0C = 0xFF;
            v0.unk_10 = 0;

            ManagedSprite_GetPositionXY(param0->unk_30, &v0.unk_00, &v0.unk_02);

            param0->unk_D8 = ov07_0221FDFC(param0->unk_90.battleSystem, param0->unk_90.heapID);
            param0->unk_D0 = ov07_02232D20(&v0);
        }
        param0->unk_08++;
        break;
    case 1: {
        int v1 = ManagedSprite_GetAnimationFrame(param0->unk_30);

        if (v1 >= 2) {
            param0->unk_24 = 0;
            param0->unk_08++;
        }
    } break;
    case 2:
        ov07_02232D80(param0->unk_D0);
        ov07_0221FE08(param0->unk_D8);
        param0->unk_08++;
        break;
    case 3: {
        if (ov07_02232DF4(param0->unk_D0) == 0) {
            ManagedSprite_SetAnimationFrame(param0->unk_30, 0);
            ov07_02232E10(param0->unk_D0);
            param0->unk_08++;
        }
    } break;
    default:
        ov07_02232F74(param0, 4);
        break;
    }

    return 1;
}

BOOL ov07_02233088(OverlayManager *appMan, int *state) {
    return 1;
}
