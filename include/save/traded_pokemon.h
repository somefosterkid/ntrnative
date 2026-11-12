// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/traded_pokemon.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_TRADED_POKEMON_H
#define BWPORT_SAVE_TRADED_POKEMON_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's TradedPokemonSave_IsTraded
// Whether the trade of the index is done
BOOL func_0200e740(TradedPokemonSave *block, u32 index);
// pokebw2's TradedPokemonSave_GetBattleLevel
// The level, from 65 to 100
u8 func_0200e74c(TradedPokemonSave *block, u32 index);
// pokebw2's TradedPokemonSave_GetId
u32 func_0200e7b4(TradedPokemonSave *block, u32 index);
// pokebw2's TradedPokemonSave_GetPersonality
u32 func_0200e7a4(TradedPokemonSave *block, u32 index);
// pokebw2's TradedPokemonSave_HasAbility
// Whether it has its own ability, and the ability
BOOL func_0200e76c(TradedPokemonSave *block, u32 index);
// pokebw2's TradedPokemonSave_GetAbility
u8 func_0200e780(TradedPokemonSave *block, u32 index);
// pokebw2's TradedPokemonSave_GetSpecies
// The species, which can be the version's
u32 func_0200e788(TradedPokemonSave *block, u32 index, u32 version);
// pokebw2's TradedPokemonSave_GetNickname
// Its nickname, as characters
u16 *func_0200e7ac(TradedPokemonSave *block, u32 index);

// Black has two trades: the block is 0x40 bytes
#define TRADED_POKEMON_COUNT 2

#define TRADED_POKEMON_NAME_LENGTH 11
#define TRADED_POKEMON_NO_ABILITY 0xff

// A Pokémon the player gave away, which its new Trainer battles with later
struct TradedPokemon {
    u32 personality;
    u16 nickname[TRADED_POKEMON_NAME_LENGTH];
    u8 traded : 1;
    u8 level : 7;
    // TRADED_POKEMON_NO_ABILITY when the Pokémon doesn't keep its own
    u8 ability;
    u32 id;
}; // size: 0x20

struct TradedPokemonSave {
    TradedPokemon pokemon[TRADED_POKEMON_COUNT];
}; // size: 0x40

// Records the Pokémon given away in the trade of the index
void SetTradedPokemonData(TradedPokemonSave *block, u32 index, PartyPkm *pkm);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's TradedPokemonSave_GetSize
// The block's size and its initial state
u32 func_0200e730(void);
// pokebw2's TradedPokemonSave_Init
void func_0200e734(TradedPokemonSave *block);

#endif // BWPORT_SAVE_TRADED_POKEMON_H
