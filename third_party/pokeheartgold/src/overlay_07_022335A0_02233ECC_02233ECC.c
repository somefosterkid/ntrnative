#include "global.h"

#include "battle/battle.h"
#include "library/spl_emitter.h"

#include "error_handling.h"
#include "heap.h"
#include "sprite_system.h"
#include "sys_task_api.h"
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

void ov07_02233ECC(BallRotation *param0);

void ov07_02233ECC(BallRotation *param0) {
    int v0 = param0->unk_90.heapID;

    GF_ASSERT(param0 != NULL);

    SpriteSystem_FreeResourcesAndManager(param0->unk_90.spriteSystem, param0->unk_2C);
    Sprite_DeleteAndFreeResources(param0->unk_30);
    SysTask_Destroy(param0->unk_CC);
    Heap_Free(param0);
}

void ov07_02233EFC(BallRotation *param0, int param1);

void ov07_02233EFC(BallRotation *param0, int param1) {
    param0->unk_90.mode = param1;
    param0->unk_08 = 0;
    param0->unk_0C = 0;
    param0->unk_10 = 0;
    param0->unk_1C = 1;

    ManagedSprite_GetPositionXY(param0->unk_30, &param0->unk_B8.unk_00, &param0->unk_B8.unk_02);
}

int ov07_02233F20(BallRotation *param0);

int ov07_02233F20(BallRotation *param0) {
    GF_ASSERT(param0 != NULL);

    return param0->unk_90.mode;
}

void ov07_02221F04(int isDoubles, int battlerType, s16 *px, s16 *py);

void ov07_02233F30(BallRotation *param0);

void ov07_02233F30(BallRotation *param0) {
    ManagedSprite_GetPositionXY(param0->unk_30, &param0->unk_B8.unk_00, &param0->unk_B8.unk_02);

    switch (param0->unk_90.type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        ManagedSprite_GetPositionXY(param0->unk_30, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 0;
        param0->unk_B8.unk_08 = 12;
        return;
    case 6:
        ov07_02221F04(0, 0, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 48;
        param0->unk_B8.unk_06 += 32;
        break;
    case 7:
        ManagedSprite_GetPositionXY(param0->unk_30, &param0->unk_B8.unk_00, &param0->unk_B8.unk_02);
        ov07_02221F04(1, 4, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 48;
        param0->unk_B8.unk_06 += 32;
        break;
    case 8:
        ov07_02221F04(1, 2, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 48;
        param0->unk_B8.unk_06 += 32;
        break;
    case 9:
        ov07_02221F04(0, 1, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 48;
        param0->unk_B8.unk_06 += 32;
        break;
    case 10:
        ov07_02221F04(1, 3, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 48;
        param0->unk_B8.unk_06 += 32;
        break;
    case 11:
        ov07_02221F04(1, 5, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 48;
        param0->unk_B8.unk_06 += 32;
        break;
    case 12:
        ov07_02221F04(0, 1, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 16;
        param0->unk_B8.unk_06 += 16;
        param0->unk_B8.unk_08 = 12;
        return;
    case 13:
        ov07_02221F04(0, 1, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 32;
        param0->unk_B8.unk_04 -= 20;
        param0->unk_B8.unk_06 += 38;
        param0->unk_B8.unk_08 = 16;
        return;
    case 14:
        param0->unk_B8.unk_04 = 128;
        param0->unk_B8.unk_06 = (96 - 32);
        param0->unk_B8.unk_10 = 32;
        param0->unk_B8.unk_06 += 32;
        break;
    case 15:
        ov07_02221F04(0, 1, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 64;
        param0->unk_B8.unk_06 += 8;
        param0->unk_B8.unk_08 = 16;
        return;
    case 16:
        ov07_02221F04(1, 3, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 64;
        param0->unk_B8.unk_06 += 8;
        param0->unk_B8.unk_08 = 16;
        return;
    case 17:
        ov07_02221F04(1, 5, &param0->unk_B8.unk_04, &param0->unk_B8.unk_06);
        param0->unk_B8.unk_10 = 64;
        param0->unk_B8.unk_06 += 8;
        param0->unk_B8.unk_08 = 16;
        return;
    default:
        break;
    }

    param0->unk_B8.unk_08 = 20;
}
