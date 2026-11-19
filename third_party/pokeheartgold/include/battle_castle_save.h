#ifndef POKEHEARTGOLD_BATTLE_CASTLE_SAVE_H
#define POKEHEARTGOLD_BATTLE_CASTLE_SAVE_H

#include "global.h"

// Battle Castle save data of the Frontier save block: the run in progress (BattleFrontierSave's castleSave, 0x74 bytes)
// and the persistent flags (FrontierSave::castlePersistentSave, 4 bytes).
typedef struct BattleCastleSave {
    u8 challengeType : 3;
    u8 didSave : 1;
    u8 currentBattleNum;
    u8 unused;
    u8 partySlots[3];
    u16 trainerIDs[14];
    u16 monHP[4];
    u8 monPP[4][4];
    u32 monStatus[4];
    u16 heldItems[4];
    u16 monSetIDs[14];
    u16 unused2;
} BattleCastleSave; // size: 0x74

typedef struct BattleCastlePersistentSave {
    u8 streakActiveFlags;
    u8 haveHeardIntro;
    u16 unused;
} BattleCastlePersistentSave; // size: 0x4

#endif // POKEHEARTGOLD_BATTLE_CASTLE_SAVE_H
