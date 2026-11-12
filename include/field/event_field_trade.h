// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_field_trade.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_EVENT_FIELD_TRADE_H
#define BWPORT_FIELD_EVENT_FIELD_TRADE_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "system/game_event.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "app/pokemon_trade.h"

// Black 2's; twinned code confirms the offsets of gameSystem, gameData, party, offerIndex, partyIndex and
// evolutionParam in Black
struct EventFieldTradeWork {
    GameSystem *gameSystem;
    GameData *gameData;
    PokeParty *party;
    u8 offerIndex;
    u8 partyIndex;
    u8 paddingE[2];
    FieldTradeInput *input;
    PokemonTradeParam subprocessData;
    GameData *transitionGameData;
    PartyPkm *partyPkm;
    PartyPkm *tradePkm;
    PlayerInfo *playerInfo;
    PlayerInfo *tradeTrainer;
    void *evolutionParam;
};

GameEvent *EventFieldTrade_Create(GameSystem *gsys, u8 offerIndex, u8 partyIndex);
void EventFieldTrade_DebugLogPkm(PartyPkm *pkm);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEventReturnCode EventFieldTrade_Callback(GameEvent *event, u32 *state, void *data);
void EventFieldTrade_CreatePkm(GameData *gameData, HeapID heapId, PartyPkm *pkm, FieldTradeOfferData *offer,
                               u32 offerIndex);
void func_ov021_021cc38c(void *param);

#endif // BWPORT_FIELD_EVENT_FIELD_TRADE_H
