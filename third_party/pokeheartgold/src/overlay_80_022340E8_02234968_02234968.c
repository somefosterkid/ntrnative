#include "global.h"
#include "overlay_80_battle_arcade.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"

struct BattleArcadeSave;

struct BattleFrontier;

struct BgConfig;

struct FieldBattleDTO;

struct FrontierParticleSystem;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct PaletteData;

struct Party;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskManager;

struct UnkStruct_ov63_0222AE60_t;

struct UnkStruct_ov63_0222BB38_t;

struct UnkStruct_ov63_0222BCE8_t;

struct UnkStruct_ov63_0222BE18_t;

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov63_0222D1C0_t;

struct UnkStruct_ov63_0222D77C_t;

struct UnkStruct_ov63_0222D848_t;

typedef struct SaveData SaveData;

typedef struct Party Party;

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

typedef struct SysTaskManager SysTaskManager;

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
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

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID, s32);

void NARC_Delete(NARC *narc);

typedef struct PaletteData PaletteData;

typedef struct ManagedSprite ManagedSprite;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

typedef struct {
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;

typedef struct BgConfig BgConfig;

void ScheduleBgTilemapBufferTransfer(BgConfig *bgConfig, u8 bgLayer);

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

void Heap_Free(void *ptr);

typedef struct FieldBattleDTO FieldBattleDTO;

typedef struct BattleArcadeSave BattleArcadeSave;

u32 GfGfxLoader_LoadCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

extern const struct {
    u16 tiles;
    u16 tilemap;
    u16 palette;
} ov80_0223BF5A[32];

void ov80_02234968(BattleArcade *arcade, FrontierGraphics *graphics, s32 arg2);

void ov80_02234968(BattleArcade *arcade, FrontierGraphics *graphics, s32 arg2) {
    if (arcade->activeEffect != 32) {
        NARC *narc = NARC_New((enum NarcId)183, HEAP_ID_101, arg2);

        GfGfxLoader_LoadCharDataFromOpenNarc(narc, ov80_0223BF5A[arcade->activeEffect].tiles, graphics->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 1, HEAP_ID_101);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, ov80_0223BF5A[arcade->activeEffect].tilemap, graphics->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 1, HEAP_ID_101);

        NNSG2dPaletteData *plttData;
        void *pltt = GfGfxLoader_GetPlttDataFromOpenNarc(narc, ov80_0223BF5A[arcade->activeEffect].palette, &plttData, HEAP_ID_101);
        DC_FlushRange(plttData->pRawData, plttData->szByte);

        GX_BeginLoadBGExtPltt();
        GX_LoadBGExtPltt(plttData->pRawData, 0x4000, 0x2000);
        GX_EndLoadBGExtPltt();

        Heap_Free(pltt);

        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 1);
        ScheduleBgTilemapBufferTransfer(graphics->bgConfig, GF_BG_LYR_MAIN_2);
        NARC_Delete(narc);
    } else {
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
    }
}
