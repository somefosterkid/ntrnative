#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_42.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
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
    u16 unk_00;
    u8 unk_02;
} UnkStruct_ov104_0223C688;

typedef struct {
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

UnkStruct_ov104_0223C688 *sub_02096864(BattleFrontier *frontier);

void ov80_0223947C(FrontierGraphics *param0, const UnkStruct_ov104_0223C688 *param1);

void ov80_022394D8(FrontierGraphics *param0, int param1);

void ov80_0223947C(FrontierGraphics *param0, const UnkStruct_ov104_0223C688 *param1) {
    UnkStruct_ov104_0223C688 *v0;
    int v1, v2;

    v0 = sub_02096864(param0->frontier);

    for (v1 = 0; v1 < 24; v1++) {
        if (v0[v1].unk_00 == param1->unk_00) {
            return;
        }
    }

    for (v1 = 0; v1 < 24; v1++) {
        if (v0[v1].unk_00 == 0xffff) {
            break;
        }
    }

    GF_ASSERT(v1 != 24);

    v2 = v1;
    v0[v2] = *param1;

    ov42_02228FE0(param0->unk_20, param1->unk_00, param1->unk_02, HEAP_ID_101);
}

void ov80_022394D8(FrontierGraphics *param0, int param1) {
    int v0;
    UnkStruct_ov104_0223C688 *v1 = sub_02096864(param0->frontier);

    for (v0 = 0; v0 < 24; v0++) {
        if (v1[v0].unk_00 == param1) {
            ov42_02229004(param0->unk_20, param1);
            v1[v0].unk_00 = 0xffff;
            return;
        }
    }
}
