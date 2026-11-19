#ifndef POKEHEARTGOLD_OVERLAY_80_FRONTIER_OPPONENT_H
#define POKEHEARTGOLD_OVERLAY_80_FRONTIER_OPPONENT_H

#include "global.h"

#include "frontier_pokemon.h"

// Battle Frontier trainer/Pokemon records shared by the Factory, Castle and Arcade structs.
// These were copied into ~160 cut files; any file that includes this header (directly or through a facility header) must not
// keep its own copy.

typedef struct FrontierTrainer {
    u32 trainerID;
    u16 trainerType;
    u16 objectID;
    u16 trainerName[7 + 1];
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
} FrontierTrainer;

typedef struct FrontierOpponent {
    FrontierTrainer trainer;
    FrontierPokemon pokemon[4];
} FrontierOpponent;

#endif // POKEHEARTGOLD_OVERLAY_80_FRONTIER_OPPONENT_H
