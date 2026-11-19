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

int ov07_02232644(int param0);

void ov07_02232F74(BallRotation *param0, int param1);

BOOL ov07_022333A8(BallRotation *param0);

struct OverlayManager;
BOOL ov07_02233498(OverlayManager *appMan, int *state);

BOOL ov07_022333A8(BallRotation *param0) {
    switch (param0->unk_08) {
    case 0:

    {
        int v0;

        v0 = ManagedSprite_GetPaletteOverrideOffset(param0->unk_30);
        PaletteData_BeginPaletteFade(param0->unk_90.paletteData, 0x4, 1 << v0, -5, 0, 10, 0x0);

        param0->unk_08++;
    } break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(param0->unk_90.paletteData) != 0) {
            break;
        }

        {
            UnkStruct_ov12_02236690 v1;

            v1.unk_04 = param0->unk_90.ball;
            v1.heapID = param0->unk_90.heapID;
            v1.unk_0C = ov07_02232644(v1.unk_04);
            v1.unk_10 = 0;

            ManagedSprite_GetPositionXY(param0->unk_30, &v1.unk_00, &v1.unk_02);
            param0->unk_D0 = ov07_02232D20(&v1);
        }
        param0->unk_08++;
        break;
    case 2:
        ov07_02232D80(param0->unk_D0);
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
        if (PaletteData_GetSelectedBuffersBitmask(param0->unk_90.paletteData) != 0) {
            break;
        }

        ov07_02232F74(param0, 22);
        break;
    }

    return 1;
}

BOOL ov07_02233498(OverlayManager *appMan, int *state) {
    return 1;
}
