#include "global.h"

#include "battle/battle.h"
#include "library/spl_emitter.h"

#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_manager.h"
#include "overlay_07_ball_rotation.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"
#include "overlay_07_palette_fade.h"

struct BallRotation;

struct BattleMonOBJData;

struct PaletteFadeContext;

struct ParticleSystem;

struct UnkStruct_ov12_02236648_t;

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

BOOL ov07_022335B4(BallRotation *param0);

void ov07_02232F74(BallRotation *param0, int param1);

BOOL ov07_0223308C(BallRotation *param0);

struct OverlayManager;
BOOL ov07_022330E0(OverlayManager *appMan, int *state);

BOOL ov07_0223308C(BallRotation *param0) {
    BOOL v0;

    switch (param0->unk_0C) {
    case 0:
        ManagedSprite_GetPositionXY(param0->unk_30, &param0->unk_B8.unk_00, &param0->unk_B8.unk_02);
        param0->unk_B8.unk_04 = 60;
        param0->unk_B8.unk_06 = 180;
        param0->unk_B8.unk_08 = 10;
        param0->unk_B8.unk_10 = 12;
        param0->unk_0C++;
        break;
    default: {
        v0 = ov07_022335B4(param0);

        if (v0 == 0) {
            ov07_02232F74(param0, 6);
        }
    } break;
    }

    return 1;
}

BOOL ov07_022330E0(OverlayManager *appMan, int *state) {
    return 1;
}
