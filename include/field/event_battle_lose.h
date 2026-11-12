// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_battle_lose.h.
#ifndef BWPORT_FIELD_EVENT_BATTLE_LOSE_H
#define BWPORT_FIELD_EVENT_BATTLE_LOSE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "system/game_event.h"

struct BattleLoseData {
    GameSystem *gsys;
    BOOL returnNonLeague;
    PlayerInfo *playerInfo;
};

GameEventReturnCode EventBattleLose_Callback(GameEvent *event, u32 *state, void *data);
GameEvent *EventBattleLose_Create(GameSystem *gsys);

// The screen of overlay 205 that a lost battle shows
extern const GameProcFunctions data_ov205_021b9ec0;

#endif // BWPORT_FIELD_EVENT_BATTLE_LOSE_H
