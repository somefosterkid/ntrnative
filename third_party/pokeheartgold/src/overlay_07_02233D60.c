#include "global.h"

#include "battle/battle.h"
#include "library/spl_emitter.h"

#include "sprite_system.h"
#include "sys_task.h"
#include "error_handling.h"
#include "heap.h"
#include "math_util.h"
#include "sys_task_api.h"

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

extern BOOL (*const ov07_022371A0[6])(BallRotation *);

void ov07_02233D60(SysTask *param0, void *param1);

void ov07_0221C69C(void);
void ov07_022342E4(BallRotation *param0);
void ov07_0223441C(BallRotation *param0);
void ov07_02233F30(BallRotation *param0);
BOOL ov07_022325BC(int param0);
BallRotation *ov07_02233DB8(BallThrow *param0);
BOOL ov07_02233E88(BallRotation *param0);
BOOL ov07_02233EA0(BallRotation *param0);

void ov07_02233EB8(BallRotation *arg0, s32 arg1);
s32 ov07_02233EBC(BallRotation *arg0, s32 arg1);

void ov07_02233D60(SysTask *param0, void *param1) {
    BOOL v0;
    BallRotation *v1 = (BallRotation *)param1;

    if (v1->unk_1C == 0) {
        return;
    }

    if (v1->unk_DC > 0) {
        v1->unk_DC--;
        SpriteSystem_DrawSprites(v1->unk_2C);
        return;
    }

    v0 = ov07_022371A0[v1->unk_90.mode](v1);

    if (v0 == 0) {
        v1->unk_1C = 0;
    }

    if (v1->unk_24 == 1) {
        ManagedSprite_TickFrame(v1->unk_30);
    }

    SpriteSystem_DrawSprites(v1->unk_2C);
}

BallRotation *ov07_02233DB8(BallThrow *param0) {
    BallRotation *v0 = NULL;

    v0 = Heap_Alloc(param0->heapID, sizeof(BallRotation));
    MI_CpuClear8(v0, sizeof(BallRotation));

    GF_ASSERT(v0 != NULL);

    v0->unk_90 = (*param0);
    v0->unk_08 = 0;
    v0->unk_10 = 0;
    v0->unk_0C = 0;
    v0->unk_14 = 0;
    v0->unk_18 = 0;
    v0->unk_00 = 0;
    v0->unk_04 = 0;
    v0->unk_2C = SpriteManager_New(v0->unk_90.spriteSystem);
    v0->unk_DC = 0;
    v0->unk_24 = ov07_022325BC(v0->unk_90.type);
    v0->unk_B8.unk_0C = 0;
    v0->unk_20 = 16;
    v0->unk_21 = 0;

    ov07_0221C69C();

    {
        int v1;

        v1 = LCRandom() % 2;

        if (v1) {
            v0->unk_22 = +1;
        } else {
            v0->unk_22 = -1;
        }
    }

    ov07_022342E4(v0);
    ov07_0223441C(v0);
    ov07_02233F30(v0);

    v0->unk_1C = 1;
    v0->unk_28 = 0;
    v0->unk_CC = SysTask_CreateOnMainQueue(ov07_02233D60, v0, 1000);

    return v0;
}

BOOL ov07_02233E88(BallRotation *param0) {
    GF_ASSERT(param0 != NULL);
    return param0->unk_1C ? 1 : 0;
}

BOOL ov07_02233EA0(BallRotation *param0) {
    GF_ASSERT(param0 != NULL);
    return param0->unk_28 ? 1 : 0;
}

void ov07_02233EB8(BallRotation *arg0, s32 arg1) {
    arg0->unk_14 = arg1;
}

s32 ov07_02233EBC(BallRotation *arg0, s32 arg1) {
    if (arg0->unk_14 == arg1) {
        return 1;
    }
    return 0;
}
