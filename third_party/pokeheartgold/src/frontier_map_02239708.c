#include "global.h"

#include "bg_window.h"
#include "overlay_42.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"

struct BattleFrontier;

struct FrontierParticleSystem;

struct G3DPipelineBuffers;

struct UnkStruct_ov63_0222BCE8_t;

struct UnkStruct_ov63_0222BE18_t;

struct UnkStruct_ov63_0222D1C0_t;

struct UnkStruct_ov63_0222D77C_t;

struct UnkStruct_ov63_0222D848_t;

typedef struct BattleFrontier BattleFrontier;

typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

typedef struct UnkStruct_ov63_0222D1C0_t UnkStruct_ov63_0222D1C0;

typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct FrontierParticleSystem FrontierParticleSystem;

typedef struct {
    SysTask *unk_00;
    s16 unk_04;
    s16 unk_06;
    s8 unk_08;
    s8 unk_09;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_0223319C;

typedef struct {
    SysTask *unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    s16 unk_08;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_022331E8;

typedef struct {
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct FrontierGraphics {
    BgConfig *bgConfig;
    PaletteData *plttData;
    BattleFrontier *frontier;
    G3DPipelineBuffers *g3dPipeline;
    FrontierParticleSystem *particleSys;
    UnkStruct_ov63_0222BE18 *unk_14;
    UnkStruct_ov63_0222BCE8 *unk_18;
    UnkStruct_ov44_02232914 unk_1C;
    UnkStruct_ov42_02228EDC *unk_20;
    UnkStruct_ov63_0222D1C0 *unk_24;
    UnkStruct_ov63_0222D1C0 *unk_28;
    UnkStruct_ov63_0222D848 *unk_2C;
    UnkStruct_ov63_0222D77C *unk_30;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteMan;
    UnkStruct_ov104_0223D3B0_1 unk_3C;
    ManagedSprite *unk_80[4];
    void *unk_90;
    SysTask *unk_94;
    SysTask *unk_98;
    SysTask *unk_9C;
    SysTask *dummyExecuteOnVBlank;
    UnkStruct_ov104_0223319C unk_A4;
    UnkStruct_ov104_022331E8 unk_B0;
    u16 unk_BC;
    u16 unk_BE;
    u8 unk_C0;
    u8 sceneID;
    u8 unk_C2;
    u8 pad_C3[1];
} FrontierGraphics;

void ov80_02239708(FrontierGraphics *param0, u16 param1, int param2);

u32 ov80_02239734(FrontierGraphics *param0, u16 param1);

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
    u16 unk_06_0 : 13;
    u16 unk_06_13 : 1;
    u16 unk_06_14 : 1;
    u16 unk_06_15 : 1;
} UnkStruct_ov104_0223D3B0_sub1;
typedef struct {
    u16 unk_00[8];
    UnkStruct_ov104_0223D3B0_sub1 unk_10[8];
} UnkStruct_ov104_0223D3B0;
UnkStruct_ov104_0223D3B0 *sub_02096878(BattleFrontier *frontier);
void ov80_02239740(FrontierGraphics *param0);

void ov80_02239708(FrontierGraphics *param0, u16 param1, int param2) {
    if (param2 == 1) {
        param0->unk_3C.unk_30 |= 1 << param1;
    } else {
        param0->unk_3C.unk_30 &= 0xffffffff ^ (1 << param1);
    }
}

u32 ov80_02239734(FrontierGraphics *param0, u16 param1) {
    return (param0->unk_3C.unk_30 >> param1) & 1;
}

void ov80_02239740(FrontierGraphics *param0) {
    int v0;
    UnkStruct_ov104_0223D3B0 *v1 = sub_02096878(param0->frontier);
    UnkStruct_ov104_0223D3B0_1 *v2 = &param0->unk_3C;

    for (v0 = 0; v0 < 8; v0++) {
        if (v2->unk_34[v0] != 0xffff) {
            v1->unk_00[v0] = v2->unk_34[v0];
            v0++;
        }
    }

    v0 = 0;

    for (v0 = 0; v0 < 8; v0++) {
        if (v2->unk_00[v0] != NULL) {
            v1->unk_10[v0].unk_05 = ManagedSprite_GetActiveAnim(v2->unk_00[v0]);
            v1->unk_10[v0].unk_06_0 = ManagedSprite_GetAnimationFrame(v2->unk_00[v0]);
            v1->unk_10[v0].unk_06_13 = ov80_02239734(param0, v0);
            v1->unk_10[v0].unk_06_14 = ManagedSprite_GetDrawFlag(v2->unk_00[v0]);
            v1->unk_10[v0].unk_04 = v2->unk_20[v0];
            ManagedSprite_GetPositionXY(v2->unk_00[v0], &v1->unk_10[v0].unk_00, &v1->unk_10[v0].unk_02);
            v1->unk_10[v0].unk_06_15 = 1;
        }
    }
}
