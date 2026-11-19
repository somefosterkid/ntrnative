#include "global.h"

#include "bg_window.h"
#include "overlay_42.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "frontier_graphics.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct BattleFrontier;
struct FrontierParticleSystem;
struct G3DPipelineBuffers;
struct UnkStruct_ov63_0222BCE8_t;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
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
    u8 unk_00;
    u8 unk_01;
    s16 unk_02[7];
} UnkStruct_ov104_0223D8F0;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    u8 unk_0C;
    UnkStruct_ov104_0223D8F0 unk_0E;
} UnkStruct_ov104_0223D570;
typedef struct {
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
UnkStruct_ov104_0223C634 *sub_02096868(BattleFrontier *frontier);

void ov80_02239590(FrontierGraphics *param0, UnkStruct_ov63_0222BEC0 *param1);

void ov80_02239590(FrontierGraphics *param0, UnkStruct_ov63_0222BEC0 *param1) {
    int v0;
    UnkStruct_ov104_0223C634 *v1 = sub_02096868(param0->frontier);

    for (v0 = 0; v0 < 32; v0++) {
        if (v1[v0].unk_00 == param1) {
            ov42_02228100(v1[v0].unk_00);
            ov42_022290C4(v1[v0].sprite);

            ((v1[v0].movementTask == 0L) ? (void)0 : GF_AssertFail());
            MI_CpuClear8(&v1[v0], sizeof(UnkStruct_ov104_0223C634));

            v1[v0].unk_08.unk_04 = 0xffff;
            return;
        }
    }
}
