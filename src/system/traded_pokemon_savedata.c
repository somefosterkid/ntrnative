// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/traded_pokemon_savedata.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/traded_pokemon.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "pml/species_names.h"
#include "system/str_tool.h"

// The Pokémon that the player gave away in the in-game trades, which their new Trainers battle with later. The ROM
// has no string for the file, so the name is descriptive (swan's SetTradedPokemonData). Block 0x41.

#define TRADED_POKEMON_LEVEL_MIN 65
#define TRADED_POKEMON_LEVEL_MAX 100
#define TRADED_POKEMON_LEVEL_GAIN 5

// pokebw2's TradedPokemonSave_GetSize
u32 func_0200e730(void) {
    return sizeof(TradedPokemonSave);
}

// pokebw2's TradedPokemonSave_Init
void func_0200e734(TradedPokemonSave *block) {
    MI_CpuFill8(block, 0, sizeof(TradedPokemonSave));
}

// pokebw2's TradedPokemonSave_IsTraded
BOOL func_0200e740(TradedPokemonSave *block, u32 index) {
    return block->pokemon[index].traded;
}

// pokebw2's TradedPokemonSave_GetBattleLevel
u8 func_0200e74c(TradedPokemonSave *block, u32 index) {
    u8 level = block->pokemon[index].level;

    if (level < TRADED_POKEMON_LEVEL_MIN) {
        level = TRADED_POKEMON_LEVEL_MIN;
    } else {
        level += TRADED_POKEMON_LEVEL_GAIN;
    }
    if (level > TRADED_POKEMON_LEVEL_MAX) {
        level = TRADED_POKEMON_LEVEL_MAX;
    }
    return level;
}

// pokebw2's TradedPokemonSave_HasAbility
BOOL func_0200e76c(TradedPokemonSave *block, u32 index) {
    if (block->pokemon[index].ability != TRADED_POKEMON_NO_ABILITY) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's TradedPokemonSave_GetAbility
u8 func_0200e780(TradedPokemonSave *block, u32 index) {
    return block->pokemon[index].ability;
}

// pokebw2's TradedPokemonSave_GetSpecies
u32 func_0200e788(TradedPokemonSave *block, u32 index, u32 version) {
    if (index == 1) {
        return SPECIES_MINCCINO;
    }
    if (version == 0x15) {
        return SPECIES_COTTONEE;
    }
    return SPECIES_PETILIL;
}

// pokebw2's TradedPokemonSave_GetPersonality
u32 func_0200e7a4(TradedPokemonSave *block, u32 index) {
    return block->pokemon[index].personality;
}

// pokebw2's TradedPokemonSave_GetNickname
u16 *func_0200e7ac(TradedPokemonSave *block, u32 index) {
    return block->pokemon[index].nickname;
}

// pokebw2's TradedPokemonSave_GetId
u32 func_0200e7b4(TradedPokemonSave *block, u32 index) {
    return block->pokemon[index].id;
}

void SetTradedPokemonData(TradedPokemonSave *block, u32 index, PartyPkm *pkm) {
    u16 name[TRADED_POKEMON_NAME_LENGTH];

    block->pokemon[index].level = (u8)PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL);
    // Whether the Pokémon has its hidden ability
    if (PokeParty_GetParam(pkm, 0x71, NULL)) {
        block->pokemon[index].ability = PokeParty_GetParam(pkm, PKM_PARAM_ABILITY, NULL);
    } else {
        block->pokemon[index].ability = TRADED_POKEMON_NO_ABILITY;
    }
    block->pokemon[index].personality = PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL);
    block->pokemon[index].id = PokeParty_GetParam(pkm, PKM_PARAM_ID, NULL);
    // Whether the Pokémon has a nickname
    if (PokeParty_GetParam(pkm, 0x75, NULL)) {
        PokeParty_GetParam(pkm, PKM_PARAM_NICKNAME_RAW, block->pokemon[index].nickname);
    } else {
        GFL_MsgDataLoadRawStr(g_PMLSpeciesNamesResident, PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL), name,
                              TRADED_POKEMON_NAME_LENGTH);
        wcharsncpy(name, block->pokemon[index].nickname, TRADED_POKEMON_NAME_LENGTH);
    }
    block->pokemon[index].traded = TRUE;
}
