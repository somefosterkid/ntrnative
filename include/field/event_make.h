// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_make.h, the lines in use so far.
// eventMakeFunc name from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
#ifndef BWPORT_FIELD_EVENT_MAKE_H
#define BWPORT_FIELD_EVENT_MAKE_H

#include "types.h"
#include "battle/battle_proc.h"
#include "struct_decls.h"
#include "system/game_event.h"

// The event that runs overlay 8's battle proc
struct EventMakeArgs {
    BtlSetup *setup;
    BattlePlayers *players;
    u32 unk08;
};

// A GameEventProvider, for GameEvent_CreateOverlayDelegate
GameEvent *eventMakeFunc(GameSystem *gsys, void *data);

struct EventMakeWork {
    GameSystem *gsys;
    BtlSetup *setup;
    BattlePlayers *players;
    u32 unk0C;
    BattleParam param;
};

GameEvent *func_ov008_021550b0(GameSystem *gsys, BtlSetup *setup, BattlePlayers *players, u32 a3);
GameEventReturnCode func_ov008_021550dc(GameEvent *event, u32 *state, void *data);

#endif // BWPORT_FIELD_EVENT_MAKE_H
