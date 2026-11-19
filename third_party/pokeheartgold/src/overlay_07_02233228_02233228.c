#include "global.h"

#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_07_ball_rotation.h"
#include "sys_task.h"
#include "battle/battle.h"
#include "library/spl_emitter.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"
#include "overlay_07_palette_fade.h"

struct BallRotation;

struct BallThrow;

struct BattleMonOBJData;

struct BattleSystem;

struct ManagedSprite;

struct PaletteData;

struct PaletteFadeContext;

struct ParticleSystem;

struct SPLEmitter;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_ov12_02236648_t;

struct ValueLerpContext;

struct XYTransformContext;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

void ov07_02232F74(BallRotation *param0, int param1);

extern const s16 ov07_0223730E[21][3];

extern const int ov07_02237200[5][2];

BOOL ov07_02233228(BallRotation *param0);

BOOL ov07_02233228(BallRotation *param0) {
    switch (param0->unk_08) {
    case 0:
        ManagedSprite_SetAnim(param0->unk_30, 1);
        ManagedSprite_SetAnimationFrame(param0->unk_30, 0);
        param0->unk_08++;
    case 1: {
        s16 v0, v1;
        int v2;

        v0 = 0;
        v1 = ov07_0223730E[param0->unk_0C][0];
        v2 = ov07_0223730E[param0->unk_0C][1];

        ManagedSprite_SetAnimationFrame(param0->unk_30, v2);

        param0->unk_0C++;

        {
            int v3;

            for (v3 = 0; v3 < 5; v3++) {
                if (ov07_02237200[v3][0] == param0->unk_0C) {
                    sub_0200602C(ov07_02237200[v3][1], 117);
                }
            }
        }

        if ((param0->unk_0C > 20) || (v0 == 0xFF) || (v1 == 0xFF)) {
            param0->unk_08++;
        } else {
            ManagedSprite_OffsetPositionXY(param0->unk_30, v0, v1);
        }
    } break;
    default:
        ov07_02232F74(param0, 14);
        break;
    }

    return 1;
}
