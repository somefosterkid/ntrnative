#ifndef POKEHEARTGOLD_POKEMON_CRY_DURATION_H
#define POKEHEARTGOLD_POKEMON_CRY_DURATION_H

#include "global.h"
#include "sys_task.h"

typedef struct PokemonCryDurationParam {
    int remainingFrames;
    SysTask *task;
} PokemonCryDurationParam;

#endif // POKEHEARTGOLD_POKEMON_CRY_DURATION_H
