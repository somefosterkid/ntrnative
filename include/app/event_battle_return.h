// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/event_battle_return.h: the lines in use
// so far.
#ifndef BWPORT_APP_EVENT_BATTLE_RETURN_H
#define BWPORT_APP_EVENT_BATTLE_RETURN_H

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
typedef struct {
    GameData *gameData;
    BtlSetup *setup;
} EventBattleReturnParam;

#endif // BWPORT_APP_EVENT_BATTLE_RETURN_H
