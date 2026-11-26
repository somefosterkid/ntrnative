#include "global.h"

#include "frontier_launch.h"

#include "bg_window.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"

struct BattleFrontier;
struct FrontierParticleSystem;
struct G3DPipelineBuffers;
struct UnkStruct_ov63_0222AE60_t;
struct UnkStruct_ov63_0222BB38_t;
struct UnkStruct_ov63_0222BCE8_t;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D1C0_t;
struct UnkStruct_ov63_0222D77C_t;
struct UnkStruct_ov63_0222D848_t;
typedef struct BattleFrontier BattleFrontier;
typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;
typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;
typedef struct {
    const UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov63_0222BB38 *unk_04;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
    u16 unk_10;
    u8 unk_12;
    u8 unk_13;
} UnkStruct_ov63_0222CE44;
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
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov63_0222CE44 *sprite;
    UnkStruct_ov104_0223D570 unk_08;
    UnkStruct_ov104_0223D8F0 unk_26;
    u8 pad_36[2];
    SysTask *movementTask;
} UnkStruct_ov104_0223C634;
UnkStruct_ov104_0223C634 *sub_02096868(BattleFrontier *frontier);
void ov80_02239C28(BattleFrontier *param0, UnkStruct_ov104_0223C634 *param1);
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;
typedef struct {
    UnkStruct_ov63_0222CC3C unk_00;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov63_0222CCB8;
typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
void ov42_022299C0(UnkStruct_ov63_0222D77C *param0, const UnkStruct_ov63_0222CCB8 *param1);
typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222AE60_t UnkStruct_ov63_0222AE60;
typedef struct {
    UnkStruct_ov63_0222AE60 *unk_00;
    UnkStruct_ov63_0222CE44 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov63_0222CD2C;
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222D160;
typedef struct UnkStruct_ov63_0222D1C0_t UnkStruct_ov63_0222D1C0;
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
    UnkStruct_ov63_0222D160 unk_1C;
    UnkStruct_ov63_0222CD2C *unk_20;
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
void ov80_0222EFD0(UnkStruct_ov63_0222CCB8 *param0, UnkStruct_ov63_0222BEC0 *param1, int param2, int action);

void ov80_02239C54(BattleFrontier *param0);
BOOL ov80_02239C80(BattleFrontier *param0, UnkStruct_ov104_0223C634 *param1);

void ov80_02239C54(BattleFrontier *param0) {
    UnkStruct_ov104_0223C634 *v0;
    int v1;

    v0 = sub_02096868(param0);

    for (v1 = 0; v1 < 32; v1++) {
        if ((v0->unk_00 != 0L) && (v0->movementTask == 0L)) {
            ov80_02239C28(param0, v0);
        }

        v0++;
    }
}

BOOL ov80_02239C80(BattleFrontier *param0, UnkStruct_ov104_0223C634 *param1) {
    UnkStruct_ov104_0223D8F0 *v0 = &param1->unk_26;
    UnkStruct_ov63_0222CCB8 v1;
    FrontierGraphics *v2 = sub_0209680C(param0);

    if (v0->unk_02[2] > 0) {
        v0->unk_02[2]--;
        return 0;
    }

    switch (v0->unk_01) {
    case 0:
        switch (v0->unk_02[0]) {
        case 0:
        case 1:
            if (v0->unk_02[1] == 0) {
                v0->unk_02[3] = 2;
                v0->unk_02[4] = 3;
            } else {
                v0->unk_02[3] = 3;
                v0->unk_02[4] = 2;
            }
            break;
        case 2:
        case 3:
            if (v0->unk_02[1] == 0) {
                v0->unk_02[3] = 0;
                v0->unk_02[4] = 1;
            } else {
                v0->unk_02[3] = 1;
                v0->unk_02[4] = 0;
            }
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            return 1;
        }

        v0->unk_02[5] = v0->unk_02[0];
        v0->unk_01++;
    case 1:
    case 2:
    case 3:
        ov80_0222EFD0(&v1, param1->unk_00, param1->unk_08.unk_04, v0->unk_02[3 + v0->unk_01 - 1]);
        ov42_022299C0(v2->unk_30, &v1);

        if (v0->unk_02[0] == v0->unk_02[3 + v0->unk_01 - 1]) {
            v0->unk_02[2] = 45;
            v0->unk_01 = 1;
        } else {
            v0->unk_02[2] = 30;
            v0->unk_01++;
        }
        break;
    default:
        return 1;
    }

    return 0;
}
