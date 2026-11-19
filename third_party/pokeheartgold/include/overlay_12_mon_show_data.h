#ifndef POKEHEARTGOLD_OVERLAY_12_MON_SHOW_DATA_H
#define POKEHEARTGOLD_OVERLAY_12_MON_SHOW_DATA_H

#include "pokepic.h"

// MonShowData: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/overlay_12_battle_controller_opponent_02259BA8.c) and 343 accesses; fields at every accessed offset, pads elsewhere.

struct BallCapsuleSealEffect_t;
struct UnkBallData;
struct BattleAnimSystem;
struct BattleMonOBJData;
struct BattleSystem;
struct OpponentData;

typedef struct MonShowData {
    struct BattleSystem *battleSys;
    struct OpponentData *battlerData;
    struct BallCapsuleSealEffect_t *ballCapsuleSealEffect;
    struct UnkBallData *ballRotation;
    struct BattleMonOBJData *btlMonObjData;
    PokepicTemplate spriteTemplate;
    struct BattleAnimSystem *battleAnimSys;
    struct { u8 command; u8 unk_01; u16 move; s32 damage; u16 power; u16 effectChance; u16 friendship; u16 isSubstitute : 1; u16 isTransformed : 1; u16 unk_0E_2 : 1; u16 unk_0E_3 : 1; u16 unk_0E_4 : 12; u32 fieldConditions; u16 attacker; u16 defender; u16 species[4]; u8 genders[4]; u8 isShiny[4]; u8 formNums[4]; u32 personalities[4]; u32 moveEffectMasks[4]; int animMode; int secondaryAnimID; int terrain; } moveAnim;
    u8 command;
    u8 battler;
    u8 battlerType;
    u8 state;
    u8 face;
    u8 yOffset;
    u16 species;
    u32 cryMod;
    u8 selectedPartySlot;
    u8 nature;
    u16 capturedBall;
    s8 height;
    s8 shadowXOffset;
    u8 isShiny;
    u8 shadowSize;
    u16 isQuickSendOut;
    u8 delay;
    u8 formNum;
    int isSubstitute;
} MonShowData;

// sizeof(MonShowData) == 0x9c

#endif // POKEHEARTGOLD_OVERLAY_12_MON_SHOW_DATA_H
