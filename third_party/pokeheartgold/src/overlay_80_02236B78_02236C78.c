#include "global.h"

typedef struct {
    u16 trainerIDMax;
    u16 setIDMin;
    u16 setIDMax;
    u8 ivs;
    u8 canBeUpgraded;
} BattleFactoryPokemonPool;
extern const BattleFactoryPokemonPool ov80_0223C508[10];
extern const BattleFactoryPokemonPool ov80_0223C4B8[10];

const BattleFactoryPokemonPool *ov80_02236C78(int round, int isOpenLevel);

const BattleFactoryPokemonPool *ov80_02236C78(int round, int isOpenLevel) {
    if (round >= ((49 / 7) + 1)) {
        round = ((49 / 7) + 1) - 1;
    }

    if (!isOpenLevel) {
        return &ov80_0223C508[round];
    } else {
        return &ov80_0223C4B8[round];
    }
}
