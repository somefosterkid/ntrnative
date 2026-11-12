// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_battle_video.h. pokebw2's
// EventBattleVideo_Create and EventBattleVideo_CreateFromArgs are its own names.
#ifndef BWPORT_FIELD_EVENT_BATTLE_VIDEO_H
#define BWPORT_FIELD_EVENT_BATTLE_VIDEO_H

// Battle Video, started by the NetConnectBattleVideo script command

#include "types.h"
#include "struct_decls.h"

typedef struct {
    Field *field;
    u32 mode;
} EventBattleVideoArgs;

GameEvent *func_ov003_0215437c(GameSystem *gsys, Field *field, u32 mode);
GameEvent *func_ov003_021543bc(GameSystem *gsys, void *args);

#endif // BWPORT_FIELD_EVENT_BATTLE_VIDEO_H
