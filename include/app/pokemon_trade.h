// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pokemon_trade.h, the lines in use so
// far. The proc table's name, POKEMONTRADE_PROC_FUNCTIONS, is pokebw2's guess.
#ifndef BWPORT_APP_POKEMON_TRADE_H
#define BWPORT_APP_POKEMON_TRADE_H

#include "types.h"
#include "app/gtsnego.h"
#include "demo/shinka_demo.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "struct_decls.h"

#define OVERLAY_POKEMONTRADE OVERLAY_ID(119)

// What to do after the trade
#define TRADE_NEXT_EVOLVE 1
#define TRADE_NEXT_NEGOTIATE 2
// Neither: the trade is over, with no evolution and no further negotiation
#define TRADE_NEXT_END 3

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 next;
    // After TRADE_NEXT_EVOLVE, the species the Pokémon evolves into and how, which the events pass to the evolution demo
    u32 evolveSpecies;
    u32 evolveMethod;
    GameData *gameData;
    GtsNegoParam *nego;
    PokeParty *party;
    // Not used by the trade, the events keep the evolution demo's parameter here
    ShinkaDemoParam *evolution;
    void *buffer;
    // Where the Pokémon traded away is
    u16 box;
    u16 slot;
    u16 friendIndex;
    u16 unk2E;
} PokemonTradeParam;

// For GTS Negotiation
extern const GameProcFunctions data_ov119_021e759c;
// The trade demo between two players
extern const GameProcFunctions data_ov119_021e7608;

// Black 2's, not checked against Black
// The trade demo's parameter for the Global Trade Station
struct PokemonTradeGtsParam {
    PokemonTradeParam trade;
    GameData *gameData;
    PartyPkm *sendPkm;
    PartyPkm *recvPkm;
    PlayerInfo *myStatus;
    PlayerInfo *partnerStatus;
};

// The parameter of the procs that only show the trade's animation
struct PokemonTradeDemoParam {
    PokemonTradeParam trade;
    GameData *gameData;
    // The Pokémon of this player and of the other
    PartyPkm *pkm[2];
    PlayerInfo *myInfo;
    PlayerInfo *partnerInfo;
};

#endif // BWPORT_APP_POKEMON_TRADE_H
