// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_game_manual.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_EVENT_GAME_MANUAL_H
#define BWPORT_FIELD_EVENT_GAME_MANUAL_H

#include "types.h"
#include "system/game_event.h"
#include "struct_decls.h"

struct GameManualSubwork {
    GameData *gameData;
    u32 result;
};

struct GameManualEventWork {
    GameSystem *gsys;
    Field *field;
    GameManualSubwork *subwork;
    u16 *result;
};

GameEvent *func_ov021_021d30b8(GameSystem *gsys, u16 *result);
void func_ov021_021d3164(GameManualEventWork *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEventReturnCode func_ov021_021d310c(GameEvent *event, u32 *state, void *data);

#endif // BWPORT_FIELD_EVENT_GAME_MANUAL_H
