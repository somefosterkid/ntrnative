#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"

struct BattleAnimContext;
struct BattleAnimSystem;
struct BattleBgAnim;
struct BgConfig;
struct BgTrackingTask;
struct ManagedSprite;
struct PaletteData;
struct SpriteManager;
struct SpriteTrackingTask;
struct SysTask;
struct SysTaskManager;
typedef struct BgConfig BgConfig;
typedef struct PaletteData PaletteData;
void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);
typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;
typedef struct ManagedSprite ManagedSprite;
typedef struct SpriteManager SpriteManager;
typedef struct ManagedSpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animIdx;
    int priority;
    int plttIdx;
    enum NNS_G2D_VRAM_TYPE vramType;
    int resources[GF_GFX_RES_TYPE_MAX];
    int bgPriority;
    BOOL vramTransfer;
} ManagedSpriteTemplate;
typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;
typedef struct SysTask SysTask;
typedef struct SysTaskManager SysTaskManager;
typedef void (*SysTaskFunc)(SysTask *, void *);
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};
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
enum BattleAnimSystemArc {
    BATTLE_ANIM_SYSTEM_ARC_BATT_BG = 0,
    BATTLE_ANIM_SYSTEM_ARC_BATT_OBJ,
    BATTLE_ANIM_SYSTEM_ARC_WECHAR,
    BATTLE_ANIM_SYSTEM_ARC_WEPLTT,
    BATTLE_ANIM_SYSTEM_ARC_WECELL,
    BATTLE_ANIM_SYSTEM_ARC_WECELLANM,

    BATTLE_ANIM_SYSTEM_ARC_COUNT,
};
typedef struct BattleAnimContext BattleAnimContext;
typedef struct BattleAnimScriptLoop {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
} BattleAnimScriptLoop;
typedef struct PokemonSpriteDrawContext {
    struct BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
} PokemonSpriteDrawContext;
typedef struct BattleBgAnim BattleBgAnim;
typedef struct SpriteTrackingTask SpriteTrackingTask;
typedef struct BgTrackingTask BgTrackingTask;
typedef struct BattleAnimSystem {
    enum HeapID heapID;
    enum NarcId moveArcID;
    BOOL isContest;
    BOOL isActive;
    BOOL moveActive;
    void *scriptData;
    u32 *scriptPtr;
    u32 *callStack[3];
    BattleAnimScriptLoop loopStack[3];
    PokemonSpriteDrawContext pokemonSpriteDrawContexts[2];
    u8 cameraProjections[16];
    u8 particleSystemCameraFlip[16];
    s8 unused_88;
    u8 scriptDelay;
    u16 activeAnimTasks;
    u16 activeSoundTasks;
    u8 pad_92[2];
    s32 scriptVars[10];
    void *executeAnimScriptFunc;
    BattleAnimContext *context;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    SpriteManager *spriteManagers[4];
    ManagedSprite *sprites[10];
    ManagedSpriteTemplate lastSpriteTemplate;
    SpriteManager *pokemonSpriteManager;
    ManagedSprite *pokemonSprites[5];
    BOOL usedPokemonSprites[5];
    SpriteTrackingTask *spriteTrackingTasks[5];
    BgTrackingTask *bgTrackingTask;
    u8 bgSwitchState;
    u8 soundEffectWaitTimer;
    u8 endDelayFrames;
    u8 unused_17B;
    BattleBgAnim *bgAnim;
    BattleBackgroundReference battleBgRefs;
    int baseBgPalettes;
    u8 bgLayerPriorities[4];
    NARC *arcs[BATTLE_ANIM_SYSTEM_ARC_COUNT];
    u8 pad_1BC[12];
} BattleAnimSystem;
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
void Heap_Free(void *ptr);
void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);
typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}
static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}
static inline void BattleAnimScript_Next(BattleAnimSystem *system);
static inline int BattleAnimScript_ReadWord(u32 *scriptPtr);
static inline void BattleAnimScript_Next(BattleAnimSystem *system) {
    system->scriptPtr += 1;
}
static inline int BattleAnimScript_ReadWord(u32 *scriptPtr) {
    return *scriptPtr;
}

s8 ov07_0221F980(s8 start, s8 end, s8 step);
extern BOOL ov07_0221F9A8(BattleAnimSystem *system, int params[], int count);
ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

s8 ov07_0221F980(s8 start, s8 end, s8 step) {
    s8 adjustedStep;

    if (start < end) {
        adjustedStep = +abs(step);
    } else if (start > end) {
        adjustedStep = -abs(step);
    } else {
        adjustedStep = 0;
    }

    return adjustedStep;
}

BOOL ov07_0221F9A8(BattleAnimSystem *system, int params[], int count) {
    BattleAnimScript_Next(system);

    int available = BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    if (available != count) {
        ((available == count) ? (void)0 : GF_AssertFail());
        return 0;
    }

    for (int i = 0; i < count; i++) {
        params[i] = BattleAnimScript_ReadWord(system->scriptPtr);
        BattleAnimScript_Next(system);
    }

    return 1;
}

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system) {
    return system->lastSpriteTemplate;
}
