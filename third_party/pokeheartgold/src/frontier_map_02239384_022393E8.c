#include "global.h"
#include "bg_window.h"
#include "frontier_graphics.h"

struct BattleFrontier;

struct BgConfig;

struct FrontierGraphics;

struct FrontierParticleSystem;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

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

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
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

typedef struct PaletteData PaletteData;

typedef struct ManagedSprite ManagedSprite;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

void SpriteSystem_Free(SpriteSystem *spriteSys);

BOOL SpriteManager_UnloadCharObjById(SpriteManager *spriteMan, int resourceID);

BOOL SpriteManager_UnloadPlttObjById(SpriteManager *spriteMan, int resourceID);

BOOL SpriteManager_UnloadCellObjById(SpriteManager *spriteMan, int resourceID);

BOOL SpriteManager_UnloadAnimObjById(SpriteManager *spriteMan, int resourceID);

void SpriteSystem_FreeResourcesAndManager(SpriteSystem *spriteSys, SpriteManager *spriteMan);

void Sprite_DeleteAndFreeResources(ManagedSprite *managedSprite);

typedef struct {
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void ov80_02239B7C(SpriteManager *param0, u16 param1);

extern void ov80_02239BE8(ManagedSprite *param0);

void ov80_022393E8(FrontierGraphics *graphics);

void ov80_022393E8(FrontierGraphics *graphics) {
    for (int i = 0; i < 8; i++) {
        if (graphics->unk_3C.unk_00[i] != NULL) {
            ov80_02239BE8(graphics->unk_3C.unk_00[i]);
        }
    }

    for (int i = 0; i < 8; i++) {
        if (graphics->unk_3C.unk_34[i] != 0xffff) {
            ov80_02239B7C(graphics->spriteMan, graphics->unk_3C.unk_34[i]);
        }
    }

    for (int i = 0; i < ((50000 + 3) - 50000 + 1); i++) {
        if (graphics->unk_80[i] != NULL) {
            Sprite_DeleteAndFreeResources(graphics->unk_80[i]);

            SpriteManager_UnloadCharObjById(graphics->spriteMan, 50000 + i);
            SpriteManager_UnloadPlttObjById(graphics->spriteMan, 50000 + i);
            SpriteManager_UnloadCellObjById(graphics->spriteMan, 50000 + i);
            SpriteManager_UnloadAnimObjById(graphics->spriteMan, 50000 + i);
        }
    }

    SpriteSystem_FreeResourcesAndManager(graphics->spriteSystem, graphics->spriteMan);
    SpriteSystem_Free(graphics->spriteSystem);
}
