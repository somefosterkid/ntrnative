#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BattleAnimContext;

struct BattleAnimSystem;

struct BattleBgAnim;

struct BgTrackingTask;

struct ManagedSprite;

struct Pokepic;

struct PokepicDrawParam;

struct SpriteTrackingTask;

struct SysTask;

struct SysTaskQueue;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);

typedef struct BattleAnimContext BattleAnimContext;

typedef struct BattleBgAnim BattleBgAnim;

typedef struct SpriteTrackingTask SpriteTrackingTask;

typedef struct BgTrackingTask BgTrackingTask;

SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

void Heap_Free(void *ptr);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

int ov07_0221D3CC(BattleAnimSystem *system, int role);

static inline void BattleAnimScript_Next(BattleAnimSystem *system);

static inline int BattleAnimScript_ReadWord(u32 *scriptPtr);

static inline void BattleAnimScript_Next(BattleAnimSystem *system) {
    system->scriptPtr += 1;
}

static inline int BattleAnimScript_ReadWord(u32 *scriptPtr) {
    return *scriptPtr;
}

void ov07_0221DAD0(SysTask *task, void *param);

void ov07_0221DAF4(BattleAnimSystem *system);

void ov07_0221DAF4(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);

    int role = BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    int slotIdx = BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    int spriteID = BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    system->pokemonSpriteDrawContexts[slotIdx].battleAnimSys = system;
    system->pokemonSpriteDrawContexts[slotIdx].spriteManager = system->pokemonSpriteManager;
    system->pokemonSpriteDrawContexts[slotIdx].sprite = system->pokemonSprites[spriteID];
    system->pokemonSpriteDrawContexts[slotIdx].active = 1;

    ManagedSprite_SetDrawFlag(system->pokemonSpriteDrawContexts[slotIdx].sprite, 0);

    if (ov07_0221FAB0(system) == 1) {
        int attackerType, defenderType;

        attackerType = ov07_02231924(system, ov07_0221C468(system));
        defenderType = ov07_02231924(system, ov07_0221C470(system));

        {
            BOOL isHidden;

            int battler = ov07_0221D3CC(system, role);
            Pokepic *battlerSprite = ov07_0221FA48(system, battler);

            if (battlerSprite != 0L) {
                isHidden = Pokepic_GetAttr(battlerSprite, MON_SPRITE_HIDE);
            } else {
                isHidden = 0;
            }

            if (isHidden == 1) {
                ManagedSprite_SetDrawFlag(system->pokemonSpriteDrawContexts[slotIdx].sprite, 0);
            } else {
                ManagedSprite_SetDrawFlag(system->pokemonSpriteDrawContexts[slotIdx].sprite, 1);
            }
        }

        switch (role) {
        case 0:
            if ((attackerType == 3) || (attackerType == 4)) {
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 1);
            } else {
                ManagedSprite_SetDrawFlag(system->pokemonSpriteDrawContexts[slotIdx].sprite, 0);
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 255);
            }
            break;
        case 2:
            if ((attackerType == 5) || (attackerType == 2)) {
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 1);
            } else {
                ManagedSprite_SetDrawFlag(system->pokemonSpriteDrawContexts[slotIdx].sprite, 0);
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 255);
            }
            break;
        case 1:
            switch (defenderType) {
            case 2:
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 255);
                break;
            case 3:
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 1);
                break;
            case 4:
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 1);
                break;
            case 5:
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 255);
                break;
            }
            break;
        case 3:
            switch (defenderType) {
            case 2:
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 1);
                break;
            case 3:
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 255);
                break;
            case 4:
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 255);
                break;
            case 5:
                ManagedSprite_SetDrawPriority(system->pokemonSpriteDrawContexts[slotIdx].sprite, 1);
                break;
            }
            break;
        }

        SysTask_CreateOnMainQueue(ov07_0221DAD0, &system->pokemonSpriteDrawContexts[slotIdx], 0x1000);
    }
}

void ov07_0221DCA8(BattleAnimSystem *system);

void ov07_0221DCA8(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);

    int slotIdx = BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);
    system->pokemonSpriteDrawContexts[slotIdx].active = 0;
}
