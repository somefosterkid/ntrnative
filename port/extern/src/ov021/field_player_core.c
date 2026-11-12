// The PC build compiles src/ov021/field_player_core.c through this file: the player's action observer, wrapped at
// the file's boundary (field_player.c calls it once a frame), also reports the end of the port's 1-frame steps (the
// 4x run and Surf, port/extern/src/ov021/fldmmdl_acmd.c). Such a step starts and ends inside one actor update, so the
// game's observer only ever sees it finished and never reports the step's end (3), which wild encounters, step
// triggers and Repel's count wait for. Every other frame keeps the game's result.
#pragma push_macro("FieldPlayerCore_UpdateActionStatusObserver")
#define FieldPlayerCore_UpdateActionStatusObserver FieldPlayerCore_UpdateActionStatusObserver_game
#include "../../../../src/ov021/field_player_core.c"
#pragma pop_macro("FieldPlayerCore_UpdateActionStatusObserver")

BOOL PortAcmd_TakeStep1F(FieldActor *actor);

void FieldPlayerCore_UpdateActionStatusObserver(FieldPlayerCore *core) {
    FieldPlayerCore_UpdateActionStatusObserver_game(core);
    if (PortAcmd_TakeStep1F(core->actor) && core->moveStatus == 1 && IsAllActorAcmdFinished(core->actor)) {
        core->observeStatus = 3;
    }
}
