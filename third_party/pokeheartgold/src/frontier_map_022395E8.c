#include "global.h"

#include "error_handling.h"
#include "frontier_map.h"
#include "bg_window.h"
#include "overlay_42.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "frontier_graphics.h"

void *sub_02096868(void *);

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
void ov80_0223962C(FrontierGraphics *param0, u16 param1);
void ov80_0223965C(FrontierGraphics *param0, u16 param1);

void ov80_022395E8(void *arg0, s32 arg1, s32 *arg2, s32 *arg3) {
    s32 v1;
    void *v0;
    void *v2;

    v0 = sub_02096868(*((void **)&((FrontierGraphics *)arg0)->frontier));
    v1 = 0;
    v2 = v0;
loop_1:
    if (arg1 == *(u16 *)((u8 *)v2 + 0xC)) {
        if (arg2 != NULL) {
            *arg2 = *(s32 *)(v0 + (0x3C * v1));
        }
        if (arg3 != NULL) {
            *arg3 = *(s32 *)((u8 *)(v0 + (0x3C * v1)) + 4);
        }
    } else {
        v1 += 1;
        v2 += 0x3C;
        if (v1 >= 0x20) {
            GF_AssertFail();
            return;
        }
        goto loop_1;
    }
}

void ov80_0223962C(FrontierGraphics *param0, u16 param1) {
    int v0;

    for (v0 = 0; v0 < 8; v0++) {
        if (param0->unk_3C.unk_34[v0] == 0xffff) {
            param0->unk_3C.unk_34[v0] = param1;
            return;
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
}

void ov80_0223965C(FrontierGraphics *param0, u16 param1) {
    int v0;

    for (v0 = 0; v0 < 8; v0++) {
        if (param0->unk_3C.unk_34[v0] == param1) {
            param0->unk_3C.unk_34[v0] = 0xffff;
            return;
        }
    }
}
