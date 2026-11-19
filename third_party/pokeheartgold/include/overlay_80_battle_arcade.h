#ifndef POKEHEARTGOLD_OVERLAY_80_BATTLE_ARCADE_H
#define POKEHEARTGOLD_OVERLAY_80_BATTLE_ARCADE_H

#include "overlay_80_frontier_opponent.h"

// BattleArcade: merged by tools/hg/promote.py from 16 m2c local copies (1 variants, base
// src/overlay_80_0222AEF8_0222B7E4.c) and 52 accesses; fields at every accessed offset, pads elsewhere.

struct BattleArcadeSave;
struct FieldBattleDTO;
struct ManagedSprite;
struct Party;
struct SaveData;

typedef struct BattleArcade {
    int unused;
    struct SaveData *saveData;
    struct BattleArcadeSave *arcadeSave;
    struct FieldBattleDTO *dto;
    u8 challengeType;
    u8 currentBattle;
    u8 cursorRandomized;
    u8 activeEffect;
    int weather;
    u16 currentStreak;
    u16 currentRound;
    u8 rouletteSpeed;
    u8 performance;
    u8 seenArcadeStarIntro;
    u8 immuneToEffect;
    u16 randomIndex;
    u8 pad_22[0x2];
    u32 unused2;
    int wonBattle;
    u8 partySlots[3];
    u8 saveStreak;
    struct ManagedSprite *playerMonSprites[4];
    struct ManagedSprite *opponentMonSprites[4];
    struct ManagedSprite *playerItemSprites[4];
    struct ManagedSprite *opponentItemSprites[4];
    struct Party *playersParty;
    struct Party *opponentsParty;
    u16 trainerIDs[14];
    u32 monHP[4];
    u32 monAtk[4];
    u32 monDef[4];
    u32 monSpeed[4];
    u32 monSpAtk[4];
    u32 monSpDef[4];
    FrontierOpponent opponents[2];
    u16 monSetIDs[4];
    u8 opponentMonIVs[4];
    u32 opponentMonPersonalities[4];
    FrontierPokemon opponentMons[4];
    u16 unused3;
    u16 savedHeldItems[3];
    u16 appCursorPos[6];
    u16 commBuffer[40];
    u8 commHugeBuffer[512];
    u8 unk_674[2][512];
    u8 unk_A74;
    u8 unk_A75;
    u16 unk_A76;
    u16 unk_A78;
    u8 unk_A7A;
    u8 unk_A7B;
    u8 msgsReceived;
    u8 unk_A7D;
    u8 pad_A7E[0x2];
    u16 *unk_A80;
    u32 unk_A84;
} BattleArcade;

// sizeof(BattleArcade) == 0xa88

#endif // POKEHEARTGOLD_OVERLAY_80_BATTLE_ARCADE_H
