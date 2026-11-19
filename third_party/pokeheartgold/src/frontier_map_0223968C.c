#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "overlay_42.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "frontier_graphics.h"

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

void ov80_02239708(FrontierGraphics *param0, u16 param1, int param2);

ManagedSprite *ov80_02239BB8(SpriteSystem *param0, SpriteManager *param1, u16 param2);

ManagedSprite *ov80_0223968C(FrontierGraphics *param0, u16 param1, u16 param2);

void ov80_02239BE8(ManagedSprite *param0);
void ov80_022396D8(FrontierGraphics *param0, u16 param1);
ManagedSprite *ov80_02239700(FrontierGraphics *param0, u16 param1);

ManagedSprite *ov80_0223968C(FrontierGraphics *param0, u16 param1, u16 param2) {
    ManagedSprite *v0;

    GF_ASSERT(param1 < 8);
    GF_ASSERT(param0->unk_3C.unk_00[param1] == NULL);

    v0 = ov80_02239BB8(param0->spriteSystem, param0->spriteMan, param2);

    param0->unk_3C.unk_00[param1] = v0;
    param0->unk_3C.unk_20[param1] = param2;

    ov80_02239708(param0, param1, 0);
    return v0;
}

void ov80_022396D8(FrontierGraphics *param0, u16 param1) {
    ((param1 < 8) ? (void)0 : GF_AssertFail());
    ((param0->unk_3C.unk_00[param1] != 0L) ? (void)0 : GF_AssertFail());

    ov80_02239BE8(param0->unk_3C.unk_00[param1]);
    param0->unk_3C.unk_00[param1] = 0L;
}

ManagedSprite *ov80_02239700(FrontierGraphics *param0, u16 param1) {
    return param0->unk_3C.unk_00[param1];
}
