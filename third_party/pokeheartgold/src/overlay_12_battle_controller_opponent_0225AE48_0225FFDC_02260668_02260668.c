#include "global.h"
#include "constants/heap.h"
#include "palette.h"
#include "bg_window.h"
#include "overlay_07_particle_effect.h"

struct BattleSystem;

struct BgConfig;

struct PaletteAnimator;

struct PaletteData;

struct SelectedPaletteData;

struct SysTask;

struct SysTaskQueue;

struct UISetupTaskData;

typedef struct BattleSystem BattleSystem;

u16 *PaletteData_GetUnfadedBuf(PaletteData *palette, enum PaletteBufferId bufferID);

u16 *PaletteData_GetFadedBuf(PaletteData *palette, enum PaletteBufferId bufferID);

u8 PaletteData_BeginPaletteFade(PaletteData *paletteData, u16 buffersToFade, u16 palettesToFade, s8 wait, u8 cur, u8 end, u16 target);

enum BattleBackground {
    BACKGROUND_PLAIN = 0,
    BACKGROUND_WATER,
    BACKGROUND_CITY,
    BACKGROUND_FOREST,
    BACKGROUND_MOUNTAIN,
    BACKGROUND_SNOW,
    BACKGROUND_INDOORS_1,
    BACKGROUND_INDOORS_2,
    BACKGROUND_INDOORS_3,
    BACKGROUND_CAVE_1,
    BACKGROUND_CAVE_2,
    BACKGROUND_CAVE_3,
    BACKGROUND_AARON,
    BACKGROUND_BERTHA,
    BACKGROUND_FLINT,
    BACKGROUND_LUCIAN,
    BACKGROUND_CYNTHIA,
    BACKGROUND_DISTORTION_WORLD,
    BACKGROUND_BATTLE_TOWER,
    BACKGROUND_BATTLE_FACTORY,
    BACKGROUND_BATTLE_ARCADE,
    BACKGROUND_BATTLE_CASTLE,
    BACKGROUND_BATTLE_HALL,
};

enum BattleTerrain {
    TERRAIN_PLAIN = 0,
    TERRAIN_SAND = 1,
    TERRAIN_GRASS = 2,
    TERRAIN_PUDDLE = 3,
    TERRAIN_MOUNTAIN = 4,
    TERRAIN_CAVE = 5,
    TERRAIN_SNOW = 6,
    TERRAIN_WATER = 7,
    TERRAIN_ICE = 8,
    TERRAIN_BUILDING = 9,
    TERRAIN_GREAT_MARSH = 10,
    TERRAIN_BRIDGE = 11,
    TERRAIN_AARON = 12,
    TERRAIN_BERTHA = 13,
    TERRAIN_FLINT = 14,
    TERRAIN_LUCIAN = 15,
    TERRAIN_CYNTHIA = 16,
    TERRAIN_DISTORTION_WORLD = 17,
    TERRAIN_BATTLE_TOWER = 18,
    TERRAIN_BATTLE_FACTORY = 19,
    TERRAIN_BATTLE_ARCADE = 20,
    TERRAIN_BATTLE_CASTLE = 21,
    TERRAIN_BATTLE_HALL = 22,
    TERRAIN_GIRATINA = 23,
    TERRAIN_MAX = 24,
};

void SysTask_Destroy(SysTask *task);

typedef struct UISetupTaskData {
    BattleSystem *battleSys;
    UnkStruct_ov12_02237F98 *animContext;
    UnkStruct_ov12_02238004 *animObjects[3];
    u8 step;
    u8 frameCount;
    u8 fadeStep;
    u8 unused;
} UISetupTaskData;

void Heap_Free(void *ptr);

typedef struct PaletteAnimator PaletteAnimator;

typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);

struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};

void sub_0201649C(PaletteAnimator *param0, u8 param1);

BgConfig *BattleSystem_GetBgConfig(BattleSystem *battleSys);

u32 BattleSystem_GetBattleType(BattleSystem *battleSys);

PaletteData *BattleSystem_GetPaletteData(BattleSystem *battleSys);

PaletteAnimator *BattleSystem_GetMessageIcon(BattleSystem *battleSys);

enum BattleTerrain BattleSystem_GetTerrainId(BattleSystem *battleSys);

enum BattleBackground BattleSystem_GetBackgroundId(BattleSystem *battleSys);

u16 *ov12_0223BAE0(BattleSystem *battleSys);

u16 *ov12_0223BAEC(BattleSystem *battleSys);

BOOL PlaySE(u16 seqID);

extern const u8 ov12_0226D350[24];

extern const int ov12_0226D18C[23];

void ov12_02260668(SysTask *task, void *data);

void ov12_02260668(SysTask *task, void *data) {
    UISetupTaskData *uiSetupTaskData = (UISetupTaskData *)data;
    BgConfig *bgConfig = BattleSystem_GetBgConfig(uiSetupTaskData->battleSys);
    PaletteData *paletteData = BattleSystem_GetPaletteData(uiSetupTaskData->battleSys);
    int terrain = BattleSystem_GetTerrainId(uiSetupTaskData->battleSys);
    int fadeTarget = ov12_0226D18C[BattleSystem_GetBackgroundId(uiSetupTaskData->battleSys)];

    uiSetupTaskData->frameCount++;

    switch (uiSetupTaskData->step) {
    case 0:
        ParticleEffectTemplate particleEffectTemplate;

        uiSetupTaskData->animContext = ov07_0223458C(HEAP_ID_BATTLE, 0L);

        particleEffectTemplate.resourceID = ov12_0226D350[terrain];
        particleEffectTemplate.cameraProjection = 1;

        uiSetupTaskData->animObjects[0] = ov07_022345C8(uiSetupTaskData->animContext, &particleEffectTemplate);

        particleEffectTemplate.resourceID = ov12_0226D350[terrain] + 1;
        particleEffectTemplate.cameraProjection = 1;

        uiSetupTaskData->animObjects[1] = ov07_022345C8(uiSetupTaskData->animContext, &particleEffectTemplate);

        ov07_02234694(uiSetupTaskData->animObjects[0]);
        PlaySE(2127);

        uiSetupTaskData->step = 1;
    case 1:
        if (uiSetupTaskData->frameCount == 10) {
            PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_BG), 0xF3FF, 0, 0, 16, fadeTarget);
            PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_OBJ), 0x3FFF, 0, 0, 16, fadeTarget);
        }

        if (uiSetupTaskData->frameCount >= 10 && uiSetupTaskData->fadeStep < 16) {
            u16 i, v7;
            u16 *buffer;

            uiSetupTaskData->fadeStep++;

            if (uiSetupTaskData->fadeStep > 16) {
                uiSetupTaskData->fadeStep = 16;
            }

            buffer = PaletteData_GetFadedBuf(paletteData, (enum PaletteBufferId)(1));

            for (i = 0; i < 256; i++) {
                v7 = (0 + ((0x1F - 0) * uiSetupTaskData->fadeStep >> 4));
                buffer[i] = v7 | (v7 << 5) | (v7 << 10);
            }
        }

        if (uiSetupTaskData->frameCount == 20) {
            ov07_02234694(uiSetupTaskData->animObjects[1]);
        }

        if (uiSetupTaskData->frameCount == 23) {
            PlaySE(2128);
        }

        if (uiSetupTaskData->frameCount == 28) {
            MI_CpuCopy16(ov12_0223BAE0(uiSetupTaskData->battleSys), PaletteData_GetUnfadedBuf(paletteData, (enum PaletteBufferId)(0)), ((16) * sizeof(u16)) * 7);

            if (BattleSystem_GetBattleType(uiSetupTaskData->battleSys) == ((1 << 1) | (1 << 3) | (1 << 6))) {
                MI_CpuCopy16(ov12_0223BAEC(uiSetupTaskData->battleSys), PaletteData_GetUnfadedBuf(paletteData, (enum PaletteBufferId)(2)), ((16) * sizeof(u16)) * 5);
            } else if (BattleSystem_GetBattleType(uiSetupTaskData->battleSys) & (1 << 1)) {
                MI_CpuCopy16(ov12_0223BAEC(uiSetupTaskData->battleSys), PaletteData_GetUnfadedBuf(paletteData, (enum PaletteBufferId)(2)), ((16) * sizeof(u16)) * 7);
            } else if (BattleSystem_GetBattleType(uiSetupTaskData->battleSys) & (1 << 0)) {
                MI_CpuCopy16(ov12_0223BAEC(uiSetupTaskData->battleSys), PaletteData_GetUnfadedBuf(paletteData, (enum PaletteBufferId)(2)), ((16) * sizeof(u16)) * 5);
            } else {
                MI_CpuCopy16(ov12_0223BAEC(uiSetupTaskData->battleSys), PaletteData_GetUnfadedBuf(paletteData, (enum PaletteBufferId)(2)), ((16) * sizeof(u16)) * 4);
            }

            PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_BG), 0xF3FF, 0, 16, 0, fadeTarget);
            PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_OBJ), 0x3FFF, 0, 16, 0, fadeTarget);
            PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_SUB_OBJ), 0xFFFF, 0, 16, 0, fadeTarget);
        }

        if (uiSetupTaskData->frameCount >= 50) {
            PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_BG), 0xC00, 0, 16, 0, 0);
            uiSetupTaskData->step++;
        }
        break;
    case 2:
        if (ov07_022346BC(uiSetupTaskData->animObjects[1]) == 0) {
            ov07_02234604(uiSetupTaskData->animContext);
            uiSetupTaskData->step++;
        }
        break;
    case 3:
        sub_0201649C(BattleSystem_GetMessageIcon(uiSetupTaskData->battleSys), 0);
        Heap_Free(data);
        SysTask_Destroy(task);
        break;
    case 4:
        ov07_02234628(uiSetupTaskData->animContext);
        ov07_02234604(uiSetupTaskData->animContext);
        uiSetupTaskData->step++;
    case 5:
        MI_CpuCopy16(ov12_0223BAE0(uiSetupTaskData->battleSys), PaletteData_GetUnfadedBuf(paletteData, (enum PaletteBufferId)(0)), ((16) * sizeof(u16)) * 7);

        if (BattleSystem_GetBattleType(uiSetupTaskData->battleSys) == ((1 << 1) | (1 << 3) | (1 << 6))) {
            MI_CpuCopy16(ov12_0223BAEC(uiSetupTaskData->battleSys), PaletteData_GetUnfadedBuf(paletteData, (enum PaletteBufferId)(2)), ((16) * sizeof(u16)) * 5);
        } else if (BattleSystem_GetBattleType(uiSetupTaskData->battleSys) & (1 << 1)) {
            MI_CpuCopy16(ov12_0223BAEC(uiSetupTaskData->battleSys), PaletteData_GetUnfadedBuf(paletteData, (enum PaletteBufferId)(2)), ((16) * sizeof(u16)) * 7);
        } else if (BattleSystem_GetBattleType(uiSetupTaskData->battleSys) & (1 << 0)) {
            MI_CpuCopy16(ov12_0223BAEC(uiSetupTaskData->battleSys), PaletteData_GetUnfadedBuf(paletteData, (enum PaletteBufferId)(2)), ((16) * sizeof(u16)) * 5);
        } else {
            MI_CpuCopy16(ov12_0223BAEC(uiSetupTaskData->battleSys), PaletteData_GetUnfadedBuf(paletteData, (enum PaletteBufferId)(2)), ((16) * sizeof(u16)) * 4);
        }

        PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_BG), 0xFFFF, 0, 0, 0, 0x7FFF);
        PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_OBJ), 0x3FFF, 0, 0, 0, 0xFFFF);
        PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_SUB_OBJ), 0xFFFF, 0, 0, 0, 0x7FFF);
        uiSetupTaskData->step = 3;
        break;
    }
}
