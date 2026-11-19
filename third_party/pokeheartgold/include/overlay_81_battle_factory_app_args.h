#ifndef POKEHEARTGOLD_OVERLAY_81_BATTLE_FACTORY_APP_ARGS_H
#define POKEHEARTGOLD_OVERLAY_81_BATTLE_FACTORY_APP_ARGS_H

#include "global.h"

struct SaveData;
struct Party;

// Args of the Battle Factory app (overlay 81), allocated by FrtCmd_095/098 in overlay 80.
typedef struct BattleFactoryAppArgs {
    struct SaveData *saveData;
    u8 challengeType;
    u8 isOpenLevel;
    u8 isExchangeMode;
    u8 padding_07;
    struct Party *personalParty;
    struct Party *receivableParty;
    u16 selectedIndices[6];
    void *battleFactory;
    u8 padding_20[4];
} BattleFactoryAppArgs;

#endif // POKEHEARTGOLD_OVERLAY_81_BATTLE_FACTORY_APP_ARGS_H
