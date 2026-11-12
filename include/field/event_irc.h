// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_irc.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_FIELD_EVENT_IRC_H
#define BWPORT_FIELD_EVENT_IRC_H

#include "types.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "app/pokemon_trade.h"
#include "app/ircbattlematch.h"
#include "battle/battle_proc.h"

// Black 2's; twinned code confirms the offsets of gsys, party, parties, save and bgm in Black
struct EventIRCWork {
    GameSystem *gsys;
    GameData *gameData;
    PokemonTradeParam trade;
    PokeParty *party;
    // The parties of the players, by net ID
    PokeParty *parties[4];
    SaveControl *save;
    // What the infrared menu chose
    u32 mode;
    // The music to bring back after the battle
    u32 bgm;
    BOOL useBattleBox;
    GameSystem *ov175Param;
    IrcBattleMatchParam ircMatch;
    u32 unk88;
    BtlSetup *btlSetup;
    BattlePlayers players;
    u8 unkE0[4];
    GameRecords *records;
    u32 counter;
};

void setPartyLv50(PokeParty *party);
void TrimPartyTo3Members(PokeParty *party);

void battleBoxToLv50Party(BOOL useBattleBox, EventIRCWork *work);
void func_ov010_02155224(EventIRCWork *work);
void func_ov010_02155a4c(EventIRCWork *work);
void func_ov010_02155a6c(EventIRCWork *work);
void func_ov010_02155a8c(EventIRCWork *work, u32 mode);
SaveControl *func_ov010_02155a94(EventIRCWork *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEventReturnCode EventIRC_Callback(GameEvent *event, u32 *state, void *data);
// Starts the event, as a new event or in place of event's callback
GameEvent *CallIRC(GameSystem *gsys, Field *field, GameEvent *event, BOOL create);
void func_ov010_02155244(EventIRCWork *work, GameData *gameData);
void func_ov010_02155328(EventIRCWork *work, GameSystem *gsys);
GameSystem *func_ov010_02155a90(EventIRCWork *work);

#endif // BWPORT_FIELD_EVENT_IRC_H
