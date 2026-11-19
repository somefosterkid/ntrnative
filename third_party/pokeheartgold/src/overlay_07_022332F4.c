#include "global.h"

#include "battle/battle.h"
#include "library/spl_emitter.h"

#include "error_handling.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02005D10.h"
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

void ov07_02232F74(BallRotation *param0, int param1);

extern const s16 ov07_02237254[3][12];

BOOL ov07_022332F4(BallRotation *param0);

struct OverlayManager;
BOOL ov07_0223338C(OverlayManager *appMan, int *state);

BOOL ov07_022332F4(BallRotation *param0) {
    switch (param0->unk_08) {
    case 0:
        ((param0->unk_18 < 3) ? (void)0 : GF_AssertFail());
        param0->unk_08++;
    case 1: {
        s16 v0;

        v0 = ov07_02237254[param0->unk_18][param0->unk_0C];
        param0->unk_0C++;

        if ((param0->unk_0C > 11) || (v0 == 0xFF)) {
            param0->unk_08++;
            param0->unk_18++;
            ManagedSprite_SetAffineZRotation(param0->unk_30, 0);
            ov07_02232F74(param0, 18);
            return 1;
        } else {
            if (param0->unk_0C == 5) {
                sub_0200602C(1533, 117);
            }

            ManagedSprite_OffsetPositionXY(param0->unk_30, v0, 0);
            ManagedSprite_OffsetAffineZRotation(param0->unk_30, (((v0) * 2) * 0xffff) / 360);
        }
    } break;
    default:
        break;
    }

    return 1;
}

BOOL ov07_0223338C(OverlayManager *appMan, int *state) {
    return 1;
}
