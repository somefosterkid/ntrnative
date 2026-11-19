#ifndef POKEHEARTGOLD_FRONTIER_POKEMON_H
#define POKEHEARTGOLD_FRONTIER_POKEMON_H

#include "global.h"

// Battle Frontier rental/record Pokemon, shared by the Factory, Castle and Arcade structs and the Wi-Fi Battle Tower records.
typedef struct FrontierPokemon {
    struct {
        u16 species : 11;
        u16 form : 5;
    };
    u16 item;
    u16 moves[4];
    u32 otID;
    u32 personality;
    union {
        struct {
            u32 hpIV : 5;
            u32 atkIV : 5;
            u32 defIV : 5;
            u32 speedIV : 5;
            u32 spAtkIV : 5;
            u32 spDefIV : 5;
            u32 setSpeciesAsNickname : 1;
            u32 : 1;
        };
        u32 combinedIVs;
    };
    union {
        struct {
            u8 hpEV;
            u8 atkEV;
            u8 defEV;
            u8 speedEV;
            u8 spAtkEV;
            u8 spDefEV;
        };
        u8 evList[6];
    };
    union {
        struct {
            u8 move0PPUps : 2;
            u8 move1PPUps : 2;
            u8 move2PPUps : 2;
            u8 move3PPUps : 2;
        };
        u8 combinedPPUps;
    };
    u8 language;
    u8 ability;
    u8 friendship;
    u16 nickname[10 + 1];
} FrontierPokemon;

#endif // POKEHEARTGOLD_FRONTIER_POKEMON_H
