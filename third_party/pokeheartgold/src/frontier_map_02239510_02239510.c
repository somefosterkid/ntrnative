#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "gf_3d_vramman.h"
#include "overlay_42.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02014DA0.h"
#include "particle_system.h"
#include "frontier_graphics.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

typedef struct BattleFrontier BattleFrontier;

typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

typedef struct UnkStruct_ov63_0222D1C0_t UnkStruct_ov63_0222D1C0;

typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct FrontierParticleSystem FrontierParticleSystem;

struct FrontierParticleSystem {
    ParticleSystem *particleSys[8];
    u16 heapID;
};

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

void ov80_02239914(BattleFrontier *, s32, UnkStruct_ov42_02228110 *, UnkStruct_ov42_0222903C *, UnkStruct_ov104_0223D570 *);

void *sub_02096868(BattleFrontier *);

UnkStruct_ov63_0222BEC0 *ov80_02239510(FrontierGraphics *param0, UnkStruct_ov104_0223D570 *param1, s32 param2);

UnkStruct_ov63_0222BEC0 *ov80_02239510(FrontierGraphics *param0, UnkStruct_ov104_0223D570 *param1, s32 param2) {
    s32 *piVar1;
    UnkStruct_ov42_02228110 *arg1;
    UnkStruct_ov42_0222903C *arg0;
    UnkStruct_ov42_02122667 UStack_20;

    piVar1 = (s32 *)sub_02096868(param0->frontier);
    if (param2 == -1) {
        param2 = 0;
        do {
            if (*piVar1 == 0) {
                break;
            }
            param2 = param2 + 1;
            piVar1 = piVar1 + 0xf;
        } while (param2 < 0x20);
        if (param2 == 0x20) {
            GF_AssertFail();
        }
    }
    UStack_20.unk0 = param1->unk_06;
    UStack_20.unk2 = param1->unk_08;
    UStack_20.unk4 = param1->unk_04;
    UStack_20.unk6 = param1->unk_02;
    UStack_20.unk8 = (u16)param1->unk_0A;
    UStack_20.unkA = param1->unk_00;
    arg1 = ov42_022280B8((UnkStruct_ov42_022280A8 *)param0->unk_14, &UStack_20);
    arg0 = ov42_0222903C((UnkStruct_ov42_02228EDC *)param0->unk_20, arg1, 0, HEAP_ID_101);
    ov42_02229200(arg0, (u16)param1->unk_0B);
    ov80_02239914(param0->frontier, param2, arg1, arg0, param1);
    return (UnkStruct_ov63_0222BEC0 *)arg1;
}
