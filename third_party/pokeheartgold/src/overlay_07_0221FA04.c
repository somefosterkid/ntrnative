#include "global.h"
struct BattleAnimSystem;

#include "pokepic.h"
#include "overlay_manager.h"
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"

u8 ov07_0221FA04(struct BattleAnimSystem *arg0, s32 arg1);

u8 ov07_0221FA04(BattleAnimSystem *system, s32 battler) {
    return system->context->battlerTypes[battler];
}

u8 ov07_0221FA10(BattleAnimSystem *system, s32 battler);

u8 ov07_0221FA10(BattleAnimSystem *system, s32 battler) {
    return system->context->battlerGenders[battler];
}

u16 ov07_0221FA1C(BattleAnimSystem *system, s32 battler);

u16 ov07_0221FA1C(BattleAnimSystem *system, s32 battler) {
    return system->context->battlerSpecies[battler];
}

u8 ov07_0221FA2C(BattleAnimSystem *system, s32 battler);

u8 ov07_0221FA2C(BattleAnimSystem *system, s32 battler) {
    return system->context->battlerForms[battler];
}

s32 ov07_0221FA38(BattleAnimSystem *system, s32 battler);

s32 ov07_0221FA38(BattleAnimSystem *system, s32 battler) {
    return system->context->battlerPersonalities[battler];
}

Pokepic *ov07_0221FA48(BattleAnimSystem *system, int battler) {
    Pokepic *pokepic;

    pokepic = system->context->battlerSprites[battler];
    if (pokepic == NULL) {
        return NULL;
    }
    if (Pokepic_IsActive(pokepic) != 0) {
        return system->context->battlerSprites[battler];
    }
    return NULL;
}

struct PaletteData *ov07_0221FA78(BattleAnimSystem *system) {
    return system->paletteData;
}

s32 ov07_0221FA80(BattleAnimSystem *system, int battler) {
    return system->context->pokemonSpriteData[battler]->palette;
}

enum NarcId ov07_0221FA90(BattleAnimSystem *system, int battler) {
    return (enum NarcId)system->context->pokemonSpriteData[battler]->narcID;
}

s32 ov07_0221FAA0(BattleAnimSystem *system, int battler) {
    return system->context->pokemonSpriteData[battler]->yOffset;
}

s32 ov07_0221FAB0(BattleAnimSystem *system) {
    if (2 & system->context->battleType) {
        return 1;
    }
    return 0;
}

s32 ov07_0221FAC8(BattleAnimSystem *system, s32 battler);

s32 ov07_0221FAC8(BattleAnimSystem *system, s32 battler) {
    if (0x200400C0 & system->context->battlerMoveEffects[battler]) {
        return 1;
    }
    return 0;
}

s32 ov07_0221FAE8(BattleAnimSystem *system);

s32 ov07_0221FAE8(BattleAnimSystem *system) {
    return 1;
}
