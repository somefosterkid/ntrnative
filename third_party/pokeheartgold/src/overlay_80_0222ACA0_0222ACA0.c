#include "global.h"

#include "error_handling.h"
#include "filesystem_files_def.h"
#include "frontier_graphics.h"

struct FrontierGraphics;

struct FrontierScene;

enum BattleFrontierScene {
    FRONTIER_SCENE_0 = 0,
    FRONTIER_SCENE_1,
    FRONTIER_SCENE_2,
    FRONTIER_SCENE_FACTORY_CORRIDOR,
    FRONTIER_SCENE_FACTORY_BATTLE_ROOM,
    FRONTIER_SCENE_TOWER_CORRIDOR,
    FRONTIER_SCENE_TOWER_MULTI_CORRIDOR,
    FRONTIER_SCENE_TOWER_BATTLE_ROOM,
    FRONTIER_SCENE_TOWER_MULTI_BATTLE_ROOM,
    FRONTIER_SCENE_HALL_CORRIDOR,
    FRONTIER_SCENE_HALL_BATTLE_ROOM,
    FRONTIER_SCENE_CASTLE_CORRIDOR,
    FRONTIER_SCENE_CASTLE_BATTLE_ROOM,
    FRONTIER_SCENE_13,
    FRONTIER_SCENE_WFC_FACILITY_SELECTOR,
    FRONTIER_SCENE_ARCADE_CORRIDOR,
    FRONTIER_SCENE_ARCADE_BATTLE_ROOM,
};

enum FrontierSceneParam {
    FR_SCENE_BGMODE = 0,
    FR_SCENE_SCRIPT_ID,
    FR_SCENE_BANK_ID,
    FR_SCENE_BGM_ID,
    FR_SCENE_SCREEN_SIZE,
    FR_SCENE_NARC_ID,
    FR_SCENE_TILEMAP_IDX,
    FR_SCENE_TILES_IDX,
    FR_SCENE_PLTT_IDX,
    FR_SCENE_SUB_TILEMAP_IDX,
    FR_SCENE_SUB_TILES_IDX,
    FR_SCENE_SUB_PLTT_IDX,
    FR_SCENE_FLAG_1,
    FR_SCENE_FLAG_2,
};

typedef void (*FrontierSceneSetupDestroyFunc)(FrontierGraphics *, void **);

typedef struct FrontierScene {
    GXBGMode bgMode;
    FrontierSceneSetupDestroyFunc setupFunc;
    FrontierSceneSetupDestroyFunc destroyFunc;
    u16 scriptID;
    u16 textBankID;
    u16 bgmID;
    u16 screenSize;
    u16 narcID;
    u16 tilemapIdx;
    u16 tilesIdx;
    u16 plttIdx;
    u16 subTilemapIdx;
    u16 subTilesIdx;
    u16 subPlttIdx;
    u8 unk_22;
    u8 unk_23;
    u16 unused;
} FrontierScene;

void ov104_0222EBBC(FrontierGraphics *graphics, void **sceneData);

void ov104_0222EBCC(FrontierGraphics *graphics, void **sceneData);

void ov80_0222ADE8(FrontierGraphics *graphics, void **sceneData);

void BattleHallBattleRoomSceneDestroy(FrontierGraphics *graphics, void **sceneData);

void BattleArcadeCorridorSceneSetup(FrontierGraphics *graphics, void **sceneData);

void BattleArcadeCorridorSceneDestroy(FrontierGraphics *graphics, void **sceneData);

void ov80_0222AE30(FrontierGraphics *graphics, void **sceneData);

void BattleArcadeBattleRoomSceneDestroy(FrontierGraphics *graphics, void **sceneData);

void ov80_0222AE6C(FrontierGraphics *graphics, void **sceneData);

void BattleFactoryCorridorSceneDestroy(FrontierGraphics *graphics, void **sceneData);

void ov80_0222AE84(FrontierGraphics *graphics, void **sceneData);

void BattleFactoryBattleRoomSceneDestroy(FrontierGraphics *graphics, void **sceneData);

void WFCFacilitySelectorSceneSetup(FrontierGraphics *graphics, void **sceneData);

void ov80_0222ADDC(FrontierGraphics *graphics, void **sceneData);

void ov80_0222AEBC(FrontierGraphics *graphics, void **sceneData);

void BattleCastleBattleRoomSceneDestroy(FrontierGraphics *graphics, void **sceneData);

extern const FrontierScene ov80_0223B724[17];

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov80_0222ADCC(FrontierGraphics *, void **);
void ov80_0222AE80(FrontierGraphics *, void **);
void ov80_0222AE9C(FrontierGraphics *, void **);
void ov80_0222AE1C(FrontierGraphics *, void **);
void ov80_0222AEF4(FrontierGraphics *, void **);
void ov80_0222AEA0(FrontierGraphics *, void **);
void ov80_0222AEB0(FrontierGraphics *, void **);
void ov80_0222AE28(FrontierGraphics *, void **);
void ov80_0222AE2C(FrontierGraphics *, void **);
void ov80_0222AE68(FrontierGraphics *, void **);

const FrontierScene ov80_0223B724[17] = {
    {
        .bgMode = GX_BGMODE_0,
        .setupFunc = ov80_0222ADCC,
        .destroyFunc = ov80_0222ADDC,
        .scriptID = 4,
        .textBankID = 0x23,
        .bgmID = 0x42E,
        .screenSize = 3,
        .narcID = 0xB7,
        .tilemapIdx = 1,
        .tilesIdx = 0,
        .plttIdx = 0x94,
        .subTilemapIdx = 3,
        .subTilesIdx = 0,
        .subPlttIdx = 0x94,
        .unk_22 = 0,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_0,
        .setupFunc = NULL,
        .destroyFunc = NULL,
        .scriptID = 0,
        .textBankID = 0x23,
        .bgmID = 0x42E,
        .screenSize = 3,
        .narcID = 0xB7,
        .tilemapIdx = 2,
        .tilesIdx = 0,
        .plttIdx = 0x94,
        .subTilemapIdx = 0xFFFF,
        .subTilesIdx = 0xFFFF,
        .subPlttIdx = 0xFFFF,
        .unk_22 = 0,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_0,
        .setupFunc = NULL,
        .destroyFunc = NULL,
        .scriptID = 5,
        .textBankID = 0x24,
        .bgmID = 0x42E,
        .screenSize = 3,
        .narcID = 0xB7,
        .tilemapIdx = 1,
        .tilesIdx = 0,
        .plttIdx = 0x94,
        .subTilemapIdx = 0xFFFF,
        .subTilesIdx = 0xFFFF,
        .subPlttIdx = 0xFFFF,
        .unk_22 = 0,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = ov80_0222AE6C,
        .destroyFunc = ov80_0222AE80,
        .scriptID = 1,
        .textBankID = 0xC3,
        .bgmID = 0x477,
        .screenSize = 4,
        .narcID = 0xB7,
        .tilemapIdx = 2,
        .tilesIdx = 0,
        .plttIdx = 0x94,
        .subTilemapIdx = 3,
        .subTilesIdx = 0,
        .subPlttIdx = 0x94,
        .unk_22 = 1,
        .unk_23 = 0,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = ov80_0222AE84,
        .destroyFunc = ov80_0222AE9C,
        .scriptID = 1,
        .textBankID = 0xC3,
        .bgmID = 0x477,
        .screenSize = 1,
        .narcID = 0xB7,
        .tilemapIdx = 1,
        .tilesIdx = 0,
        .plttIdx = 0x94,
        .subTilemapIdx = 0xFFFF,
        .subTilesIdx = 0xFFFF,
        .subPlttIdx = 0xFFFF,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = NULL,
        .destroyFunc = NULL,
        .scriptID = 6,
        .textBankID = 0x60,
        .bgmID = 0x42E,
        .screenSize = 4,
        .narcID = 0xB7,
        .tilemapIdx = 0x36,
        .tilesIdx = 0x31,
        .plttIdx = 0xA0,
        .subTilemapIdx = 0x37,
        .subTilesIdx = 0x31,
        .subPlttIdx = 0xA0,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = NULL,
        .destroyFunc = NULL,
        .scriptID = 7,
        .textBankID = 0x60,
        .bgmID = 0x42E,
        .screenSize = 4,
        .narcID = 0xB7,
        .tilemapIdx = 0x33,
        .tilesIdx = 0x31,
        .plttIdx = 0xA0,
        .subTilemapIdx = 0x34,
        .subTilesIdx = 0x31,
        .subPlttIdx = 0xA0,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = NULL,
        .destroyFunc = NULL,
        .scriptID = 8,
        .textBankID = 0x61,
        .bgmID = 0x42E,
        .screenSize = 1,
        .narcID = 0xB7,
        .tilemapIdx = 0x35,
        .tilesIdx = 0x31,
        .plttIdx = 0xA0,
        .subTilemapIdx = 0xFFFF,
        .subTilesIdx = 0xFFFF,
        .subPlttIdx = 0xFFFF,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = NULL,
        .destroyFunc = NULL,
        .scriptID = 9,
        .textBankID = 0x62,
        .bgmID = 0x42E,
        .screenSize = 1,
        .narcID = 0xB7,
        .tilemapIdx = 0x32,
        .tilesIdx = 0x31,
        .plttIdx = 0xA0,
        .subTilemapIdx = 0xFFFF,
        .subTilesIdx = 0xFFFF,
        .subPlttIdx = 0xFFFF,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = NULL,
        .destroyFunc = NULL,
        .scriptID = 3,
        .textBankID = 0x1B9,
        .bgmID = 0x478,
        .screenSize = 1,
        .narcID = 0xB7,
        .tilemapIdx = 0x11,
        .tilesIdx = 0x10,
        .plttIdx = 0x97,
        .subTilemapIdx = 0xFFFF,
        .subTilesIdx = 0xFFFF,
        .subPlttIdx = 0xFFFF,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = ov80_0222ADE8,
        .destroyFunc = ov80_0222AE1C,
        .scriptID = 3,
        .textBankID = 0x1B9,
        .bgmID = 0x478,
        .screenSize = 4,
        .narcID = 0xB7,
        .tilemapIdx = 0x14,
        .tilesIdx = 0xE,
        .plttIdx = 0x96,
        .subTilemapIdx = 0x16,
        .subTilesIdx = 0x13,
        .subPlttIdx = 0x98,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = NULL,
        .destroyFunc = NULL,
        .scriptID = 0,
        .textBankID = 0x20,
        .bgmID = 0x47A,
        .screenSize = 4,
        .narcID = 0xB7,
        .tilemapIdx = 0x1A,
        .tilesIdx = 0x19,
        .plttIdx = 0x9A,
        .subTilemapIdx = 0xFFFF,
        .subTilesIdx = 0xFFFF,
        .subPlttIdx = 0xFFFF,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = ov80_0222AEBC,
        .destroyFunc = ov80_0222AEF4,
        .scriptID = 0,
        .textBankID = 0x20,
        .bgmID = 0x47A,
        .screenSize = 1,
        .narcID = 0xB7,
        .tilemapIdx = 0x1E,
        .tilesIdx = 0x1D,
        .plttIdx = 0x9F,
        .subTilemapIdx = 0x21,
        .subTilesIdx = 0x1D,
        .subPlttIdx = 0x9F,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = NULL,
        .destroyFunc = NULL,
        .scriptID = 0,
        .textBankID = 0x20,
        .bgmID = 0x47A,
        .screenSize = 1,
        .narcID = 0xB7,
        .tilemapIdx = 0x1C,
        .tilesIdx = 0x1B,
        .plttIdx = 0x9B,
        .subTilemapIdx = 0xFFFF,
        .subTilesIdx = 0xFFFF,
        .subPlttIdx = 0xFFFF,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = ov80_0222AEA0,
        .destroyFunc = ov80_0222AEB0,
        .scriptID = 0xA,
        .textBankID = 0x12,
        .bgmID = 0x42E,
        .screenSize = 1,
        .narcID = 0xB7,
        .tilemapIdx = 0x84,
        .tilesIdx = 0x83,
        .plttIdx = 0xBF,
        .subTilemapIdx = 0xFFFF,
        .subTilesIdx = 0xFFFF,
        .subPlttIdx = 0xFFFF,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = ov80_0222AE28,
        .destroyFunc = ov80_0222AE2C,
        .scriptID = 2,
        .textBankID = 0x1AA,
        .bgmID = 0x479,
        .screenSize = 1,
        .narcID = 0xB7,
        .tilemapIdx = 0x3B,
        .tilesIdx = 0x3C,
        .plttIdx = 0xA2,
        .subTilemapIdx = 0xFFFF,
        .subTilesIdx = 0xFFFF,
        .subPlttIdx = 0xFFFF,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
    {
        .bgMode = GX_BGMODE_5,
        .setupFunc = ov80_0222AE30,
        .destroyFunc = ov80_0222AE68,
        .scriptID = 2,
        .textBankID = 0x1AA,
        .bgmID = 0x479,
        .screenSize = 4,
        .narcID = 0xB7,
        .tilemapIdx = 0x38,
        .tilesIdx = 0x3A,
        .plttIdx = 0xA1,
        .subTilemapIdx = 0x3D,
        .subTilesIdx = 0x5D,
        .subPlttIdx = 0xBC,
        .unk_22 = 1,
        .unk_23 = 1,
        .unused = 0,
    },
};
#endif

void ov80_0222AD9C(FrontierGraphics *graphics, void **sceneData, enum BattleFrontierScene sceneID);

void FrontierScene_CallDestroyFunc(FrontierGraphics *graphics, void **sceneData, enum BattleFrontierScene sceneID);

int ov80_0222ACA0(enum BattleFrontierScene sceneID, enum FrontierSceneParam sceneParam);

int ov80_0222ACA0(enum BattleFrontierScene sceneID, enum FrontierSceneParam sceneParam) {
    switch (sceneParam) {
    case FR_SCENE_BGMODE:
        return ov80_0223B724[sceneID].bgMode;
    case FR_SCENE_SCRIPT_ID:
        return ov80_0223B724[sceneID].scriptID;
    case FR_SCENE_BANK_ID:
        return ov80_0223B724[sceneID].textBankID;
    case FR_SCENE_BGM_ID:
        return ov80_0223B724[sceneID].bgmID;
    case FR_SCENE_SCREEN_SIZE:
        return ov80_0223B724[sceneID].screenSize;
    case FR_SCENE_NARC_ID:
        return ov80_0223B724[sceneID].narcID;
    case FR_SCENE_TILEMAP_IDX:
        return ov80_0223B724[sceneID].tilemapIdx;
    case FR_SCENE_TILES_IDX:
        return ov80_0223B724[sceneID].tilesIdx;
    case FR_SCENE_PLTT_IDX:
        return ov80_0223B724[sceneID].plttIdx;
    case FR_SCENE_SUB_TILEMAP_IDX:
        return ov80_0223B724[sceneID].subTilemapIdx;
    case FR_SCENE_SUB_TILES_IDX:
        return ov80_0223B724[sceneID].subTilesIdx;
    case FR_SCENE_SUB_PLTT_IDX:
        return ov80_0223B724[sceneID].subPlttIdx;
    case FR_SCENE_FLAG_1:
        return ov80_0223B724[sceneID].unk_22;
    case FR_SCENE_FLAG_2:
        return ov80_0223B724[sceneID].unk_23;
    }

    GF_ASSERT(FALSE);
    return 0;
}

void ov80_0222AD9C(FrontierGraphics *graphics, void **sceneData, enum BattleFrontierScene sceneID) {
    if (ov80_0223B724[sceneID].setupFunc != NULL) {
        ov80_0223B724[sceneID].setupFunc(graphics, sceneData);
    }
}

#if !(defined(SDK_PORT) || defined(NONMATCHING))
extern const FrontierScene ov80_0223B728[17];
#endif

void ov80_0222ADB4(FrontierGraphics *graphics, void **sceneData, enum BattleFrontierScene sceneID);

void ov80_0222ADB4(FrontierGraphics *graphics, void **sceneData, enum BattleFrontierScene sceneID) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    // ov80_0223B728 is ov80_0223B724 + 4: its setupFunc slot is the destroyFunc of ov80_0223B724
    if (ov80_0223B724[sceneID].destroyFunc != NULL) {
        ov80_0223B724[sceneID].destroyFunc(graphics, sceneData);
    }
#else
    if (ov80_0223B728[sceneID].setupFunc != NULL) {
        ov80_0223B728[sceneID].setupFunc(graphics, sceneData);
    }
#endif
}
