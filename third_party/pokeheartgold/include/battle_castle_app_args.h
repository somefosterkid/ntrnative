#ifndef POKEHEARTGOLD_BATTLE_CASTLE_APP_ARGS_H
#define POKEHEARTGOLD_BATTLE_CASTLE_APP_ARGS_H

#include "global.h"

struct SaveData;
struct Party;

// The OverlayManager args of the ov83 Battle Castle self app (built by ov80_02231E4C, read by ov83_0223DD60).
typedef struct BattleCastleAppArgs {
    struct SaveData *saveData;
    u8 challengeType;
    u8 unused[3];
    u8 identityUnlocked[4];
    u8 levelAdjustments[4];
    u8 statsUnlocked[4];
    u8 movesUnlocked[4];
    struct Party *party;
    struct Party *opponentsParty;
    u16 selectedMonSlot;
    u8 padding[2];
    void *battleCastle;
    u16 partnersCP;
    u8 padding2[6];
} BattleCastleAppArgs;

#endif // POKEHEARTGOLD_BATTLE_CASTLE_APP_ARGS_H
