#include "global.h"

#include "bg_window.h"
#include "frontier_map.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02026E30.h"

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

typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;

typedef struct UnkStruct_ov63_0222AE60_t UnkStruct_ov63_0222AE60;

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

typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct FrontierParticleSystem FrontierParticleSystem;

BOOL ov80_02239A38(void);

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
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov63_0222CE44 *sprite;
    UnkStruct_ov104_0223D570 unk_08;
    UnkStruct_ov104_0223D8F0 unk_26;
    u8 pad_36[2];
    SysTask *movementTask;
} UnkStruct_ov104_0223C634;

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

void ov42_022290DC(UnkStruct_ov63_0222CD2C *param0);

void ov42_02229358(UnkStruct_ov63_0222D160 *param0, const UnkStruct_ov63_0222BEC0 *param1);

UnkStruct_ov104_0223C634 *sub_0209686C(BattleFrontier *frontier, int param1);

void FrontierMap_Scroll(FrontierGraphics *param0);

void FrontierMap_Update(SysTask *param0, void *param1) {
    FrontierGraphics *v0 = param1;

    {
        UnkStruct_ov104_0223C634 *v1;

        v1 = sub_0209686C(v0->frontier, 32 - 1);

        if (v1->unk_00 != 0L) {
            ov42_02229358(&v0->unk_1C, v1->unk_00);
        }

        FrontierMap_Scroll(v0);
    }

    ov42_022290DC(v0->unk_20);

    {
        u32 v2 = v0->unk_3C.unk_30;
        int v3;

        for (v3 = 0; v3 < 8; v3++) {
            if ((v0->unk_3C.unk_00[v3] != 0L) && (v2 & 1)) {
                ManagedSprite_TickFrame(v0->unk_3C.unk_00[v3]);
            }

            v2 >>= 1;
        }
    }

    SpriteSystem_DrawSprites(v0->spriteMan);
    SpriteSystem_UpdateTransfer();

    ov80_02239A38();
    RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}
