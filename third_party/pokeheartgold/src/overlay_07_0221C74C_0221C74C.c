#include "global.h"
#include "error_handling.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_context.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BattleAnimContext;

struct BattleAnimSystem;

struct BattleBgAnim;

struct BgTrackingTask;

struct SpriteTrackingTask;

struct SysTask;

struct SysTaskQueue;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);


typedef struct BattleBgAnim BattleBgAnim;

typedef struct SpriteTrackingTask SpriteTrackingTask;

typedef struct BgTrackingTask BgTrackingTask;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

s32 ov07_0221C768(BattleAnimSystem *arg0, u32 arg1);
void ov07_0221C7B8(BattleAnimSystem *system, BattlerSpriteContext *battlerSpriteCtx, u32 animParticipantMode);

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

static inline void BattleAnimScript_Next(BattleAnimSystem *system) {
    system->scriptPtr += 1;
}

void ov07_0221C74C(BattleAnimSystem *system);

void ov07_0221C74C(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);

    for (int i = 0; i < 10; i++) {
        system->scriptVars[i] = 0;
    }
}

s32 ov07_0221C768(BattleAnimSystem *arg0, u32 arg1) {
    switch (arg1) {
    case 0:
        return arg0->context->damage;
    case 1:
        return (s32)arg0->context->power;
    case 2:
        return (s32)arg0->context->friendship;
    case 3:
        return arg0->context->fieldConditions;
    case 4:
        return (s32)arg0->context->effectChance;
    case 5:
        return (s32)arg0->context->terrain;
    default:
        return 0;
    }
}

void ov07_0221C7B8(BattleAnimSystem *system, BattlerSpriteContext *battlerSpriteCtx, u32 animParticipantMode) {
    int i;

    for (i = 0; i < 4; i++) {
        battlerSpriteCtx->pokemonSpriteData[i] = system->context->pokemonSpriteData[i];
        battlerSpriteCtx->sprites[i] = system->context->battlerSprites[i];
        battlerSpriteCtx->species[i] = system->context->battlerSpecies[i];
        battlerSpriteCtx->genders[i] = system->context->battlerGenders[i];
        battlerSpriteCtx->shinyFlags[i] = system->context->battlerShinyFlags[i];
        battlerSpriteCtx->forms[i] = system->context->battlerForms[i];
        battlerSpriteCtx->personalities[i] = system->context->battlerPersonalities[i];
        battlerSpriteCtx->types[i] = system->context->battlerTypes[i];
    }
    switch (animParticipantMode) {
    case 0:
        battlerSpriteCtx->targetBattler = system->context->attacker;
        battlerSpriteCtx->sourceBattler = system->context->defender;
        return;
    case 1:
        battlerSpriteCtx->targetBattler = system->context->attacker;
        battlerSpriteCtx->sourceBattler = system->context->attacker;
        return;
    case 2:
        battlerSpriteCtx->targetBattler = system->context->attacker;
        battlerSpriteCtx->sourceBattler = system->context->attacker;
        return;
    case 3:
        battlerSpriteCtx->targetBattler = system->context->attacker;
        battlerSpriteCtx->sourceBattler = system->context->attacker;
        return;
    default:
        GF_AssertFail();
        return;
    }
}
