#include "global.h"

#include "overlay_07_bg_scroll_context.h"

#include "filesystem.h"
#include "heap.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "battle/battle.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_bg_switch.h"
#include "gf_gfx_planes.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_anim_tracking.h"

struct BattleAnimContext;

struct BattleAnimScriptLoop;

struct BattleAnimSystem;

struct BattleBackgroundReference;

struct BattleBgAnim;

struct BattleBgSwitch;

struct BattleMonOBJData;

struct BattleSystem;

struct BgConfig;

struct BgTrackingTask;

struct BgWaveStrip;

struct CustomBgScrollContext;

struct NARC;

struct PokemonSpriteDrawContext;

struct SpriteManager;

struct SpriteTrackingTask;

struct SysTask;

struct SysTaskQueue;

struct WaveAnimContext;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;

typedef struct BattleMonOBJData BattleMonOBJData;

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

int BattleAnimSystem_GetBgID(BattleAnimSystem *system, int bg);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

CustomBgScrollContext *ov07_02222BE4(void *offsetReg, u32 initValue, enum HeapID heapID);

u32 BattleAnimUtil_MakeBgOffsetValue(u16 x, u16 y);

void *ov07_02222D90(int bgID);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

void ov07_0221E87C(SysTask *task, void *param);

BOOL ov07_0221DDB0(BattleBgSwitch *bgSwitch, BattleAnimSystem *system, int var);

extern const s16 ov07_02234C20[16];

void ov07_0221DE04(BattleBgSwitch *bgSwitch, BattleAnimSystem *system, enum GFBgLayer bgLayer, int bgID);

void ov07_0221EC7C(BattleAnimSystem *system, enum GFBgLayer bgLayer);

void ov07_0221FB30(BattleAnimSystem *system, enum GFBgLayer layer);

BattleMonOBJData *ov07_0221FDFC(BattleSystem *battleSys, enum HeapID heapID);

BOOL ov07_0221E788(BattleBgSwitch *bgSwitch);


int ov07_0221FAF8(BattleAnimSystem *system, int param);

u32 ov07_02222D88(s32 x, s32 y);

BOOL ov07_0221E788(BattleBgSwitch *bgSwitch) {
    BattleAnimSystem *system = bgSwitch->battleAnimSystem;
    BattleBgAnim *battleBgAnim = Heap_Alloc((enum HeapID)bgSwitch->battleAnimSystem->heapID, sizeof(BattleBgAnim));

    battleBgAnim->waveAnimCtx = Heap_Alloc((enum HeapID)system->heapID, sizeof(WaveAnimContext));

    ov07_0221DD38(system->bgAnim, battleBgAnim, 2);
    bgSwitch->bgWaveAnimActive = 1;

    battleBgAnim->cancel = 0;
    battleBgAnim->waveAnimCtx->bgScrollCtx = ov07_02222BE4(
        ov07_02222D90(ov07_0221FAF8(system, 2)),
        ov07_02222D88(0, 0),
        (enum HeapID)system->heapID);

    for (int i = 0; i < ((192 - 64) / 8); i++) {
        battleBgAnim->waveAnimCtx->strips[i].startScanline = (i * 8);
        battleBgAnim->waveAnimCtx->strips[i].endScanline = (battleBgAnim->waveAnimCtx->strips[i].startScanline + 8);
        battleBgAnim->waveAnimCtx->strips[i].scrollSpeed = ov07_02234C20[i];
        battleBgAnim->waveAnimCtx->strips[i].scrollAccum = 0;
        battleBgAnim->waveAnimCtx->strips[i].baseOffset = ov07_02222D88(0, 0);

        if (ov07_0221DDB0(bgSwitch, bgSwitch->battleAnimSystem, 6) == 1) {
            battleBgAnim->waveAnimCtx->strips[i].scrollSpeed *= -1;
        }
    }

    SysTask_CreateOnMainQueue(ov07_0221E87C, battleBgAnim, 0x1000);
    return 0;
}
