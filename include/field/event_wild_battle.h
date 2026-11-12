// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_wild_battle.h.
#ifndef BWPORT_FIELD_EVENT_WILD_BATTLE_H
#define BWPORT_FIELD_EVENT_WILD_BATTLE_H

#include "types.h"
#include "struct_decls.h"

GameEvent *EventWildBattleCall_CreateRandom(EncountSystem *encountSystem, u32 mode);

#endif // BWPORT_FIELD_EVENT_WILD_BATTLE_H
