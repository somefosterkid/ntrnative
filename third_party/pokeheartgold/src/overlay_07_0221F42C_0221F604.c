#include "global.h"

#include "battle/battler_sprite_context.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "camera.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "sys_task.h"
#include "overlay_07_bg_scroll_context.h"
#include "sav_chatot.h"
#include "bg_window.h"
#include "sprite_system.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_anim_tracking.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BattleAnimSystem;

struct CustomBgScrollContext;

struct Pokepic;

struct PokepicDrawParam;

struct SPLEmitter;

struct SpriteList;

struct SysTask;

struct SysTaskQueue;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;

enum BattleAnimSystemArc {
    BATTLE_ANIM_SYSTEM_ARC_BATT_BG = 0,
    BATTLE_ANIM_SYSTEM_ARC_BATT_OBJ,
    BATTLE_ANIM_SYSTEM_ARC_WECHAR,
    BATTLE_ANIM_SYSTEM_ARC_WEPLTT,
    BATTLE_ANIM_SYSTEM_ARC_WECELL,
    BATTLE_ANIM_SYSTEM_ARC_WECELLANM,

    BATTLE_ANIM_SYSTEM_ARC_COUNT,
};

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

u16 ov07_0221C470(BattleAnimSystem *system);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

void Heap_Free(void *ptr);

enum BattleAnimPositionType {
    BATTLE_ANIM_POSITION_MON_X = 0,
    BATTLE_ANIM_POSITION_MON_Y,
    BATTLE_ANIM_POSITION_PARTICLE_X,
    BATTLE_ANIM_POSITION_PARTICLE_Y,
};

s16 ov07_02221F80(BattleAnimSystem *system, int battler, enum BattleAnimPositionType posType);

typedef void (*BattleAnimScriptSpriteFunc)(struct BattleAnimSystem *, SpriteSystem *, SpriteManager *, ManagedSprite *);

BattleAnimScriptSpriteFunc ov07_0222304C(u32 id);

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

static inline void BattleAnimScript_Next(BattleAnimSystem *system);

static inline int BattleAnimScript_ReadWord(u32 *scriptPtr);

static inline void BattleAnimScript_Next(BattleAnimSystem *system) {
    system->scriptPtr += 1;
}

static inline int BattleAnimScript_ReadWord(u32 *scriptPtr) {
    return *scriptPtr;
}

void ov07_0221F604(BattleAnimSystem *system);

void ov07_0221F604(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);

    int id = BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    int funcID = BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ManagedSpriteTemplate template;
    int defender = ov07_0221C470(system);

    template.x = ov07_02221F80(system, defender, BATTLE_ANIM_POSITION_MON_X);
    template.y = ov07_02221F80(system, defender, BATTLE_ANIM_POSITION_MON_Y);
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 100;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.bgPriority = 1;
    template.vramTransfer = 0;

    int i;
    for (i = 0; i < GF_GFX_RES_TYPE_MAX; i++) {
        template.resIdList[i] = (BattleAnimScript_ReadWord(system->scriptPtr) + 5000);
        BattleAnimScript_Next(system);
    }

    system->lastSpriteTemplate = template;

    ManagedSprite *sprite = SpriteSystem_NewSprite(system->context->spriteSystem, system->spriteManagers[id], &template);

    int argCount = BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    for (i = 0; i < argCount; i++) {
        system->scriptVars[i] = BattleAnimScript_ReadWord(system->scriptPtr);
        BattleAnimScript_Next(system);
    }

    for (; i < 10; i++) {
        system->scriptVars[i] = 0;
    }

    BattleAnimScriptSpriteFunc func = ov07_0222304C(funcID);
    func(system, system->context->spriteSystem, system->spriteManagers[id], sprite);
}

void ov07_0221F710(BattleAnimSystem *system);

void ov07_0221F710(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);

    int id = BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    int spriteID = BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ManagedSpriteTemplate template;
    int defender = ov07_0221C470(system);

    template.x = ov07_02221F80(system, defender, BATTLE_ANIM_POSITION_MON_X);
    template.y = ov07_02221F80(system, defender, BATTLE_ANIM_POSITION_MON_Y);
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 100;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.bgPriority = 1;
    template.vramTransfer = 0;

    for (int i = 0; i < GF_GFX_RES_TYPE_MAX; i++) {
        template.resIdList[i] = (BattleAnimScript_ReadWord(system->scriptPtr) + 5000);
        BattleAnimScript_Next(system);
    }

    system->lastSpriteTemplate = template;

    ManagedSprite *sprite = SpriteSystem_NewSprite(system->context->spriteSystem, system->spriteManagers[id], &template);

    ((system->sprites[spriteID] == 0L) ? (void)0 : GF_AssertFail());
    system->sprites[spriteID] = sprite;
}
