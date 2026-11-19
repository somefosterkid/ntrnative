#ifndef POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_CONTEXT_H

#include "global.h"
#include "battle/battler_sprite_context.h"

struct ParticleSystem;
struct Pokepic;
struct SOUND_CHATOT;
struct SPLEmitter;
struct SpriteSystem;

// BattleAnimContext: what the battle hands the move animation (BattleAnimSystem.context). One definition for the
// 78 m2c copies; the pointer slots are 4 bytes on the DS and 8 on the PC, so the accessors read it by field.
typedef struct BattleAnimContext {
    u8 command;
    u8 unk_01;

    u16 move;
    s32 damage;
    u16 power;
    u16 friendship;
    u32 fieldConditions;
    u16 effectChance;
    u16 terrain;

    u16 attacker;
    u16 defender;
    int currentParticleSystem;
    struct ParticleSystem *particleSystems[16];
    struct SPLEmitter *emitters[20];
    struct SpriteSystem *spriteSystem;

    PokemonSpriteData *pokemonSpriteData[4];
    u8 battlerTypes[4];
    struct Pokepic *battlerSprites[4];
    u32 battleType;
    u16 battlerSpecies[4];
    u8 battlerGenders[4];
    u8 battlerShinyFlags[4];
    u8 battlerForms[4];
    u32 battlerPersonalities[4];
    u32 battlerMoveEffects[4];
    struct SOUND_CHATOT *chatotCry;
    u8 *bgTiles;
    u16 *bgPaletteBuffer;
    BOOL isTransformed;
} BattleAnimContext;

// sizeof(BattleAnimContext) == 0x11C (DS)

#endif // POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_CONTEXT_H
