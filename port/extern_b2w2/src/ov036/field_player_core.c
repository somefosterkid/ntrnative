// Black 2's and White 2's player action observer (port/extern/src/ov021/field_player_core.c's twin): pokebw2's
// src/ov036/field_player_core.c compiled through this file. The observer, wrapped at the file's boundary
// (field_player.c calls it once a frame), also reports the end of the port's 1-frame steps (the 4x run and Surf,
// port/extern_b2w2/src/ov036/field_actor_acmd.c). Such a step starts and ends inside one actor update, so the game's
// observer only ever sees it finished and never reports the step's end, which wild encounters, step triggers and Repel's
// count wait for. Every other frame keeps the game's result.
#define FieldPlayerCore_UpdateActionStatusObserver FieldPlayerCore_UpdateActionStatusObserver_game
#include "../../../../third_party/pokebw2/src/ov036/field_player_core.c"
#undef FieldPlayerCore_UpdateActionStatusObserver

BOOL PortAcmd_TakeStep1F(FieldActor *actor);

void FieldPlayerCore_UpdateActionStatusObserver(FieldPlayerCore *core) {
    FieldPlayerCore_UpdateActionStatusObserver_game(core);
    if (PortAcmd_TakeStep1F(core->actor) && core->moveStatus == PLAYER_MOVE_STATUS_MOVE &&
        IsAllActorAcmdFinished(core->actor)) {
        core->observeStatus = PLAYER_OBSERVE_END;
    }
}
