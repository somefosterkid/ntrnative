#include "global.h"

#include "move_anim_data.h"
#include "unk_02062108.h"

s32 MovementScriptMachineSub_Init(MoveAnimData *data);

s32 MovementScriptMachineSub_Init(MoveAnimData *data) {
    data->count = 0;
    data->state = 1;
    return 1;
}

s32 MovementScriptMachineSub_WaitReady(MoveAnimData *data);

s32 MovementScriptMachineSub_WaitReady(MoveAnimData *data) {
    if (MapObject_AreBitsSetForMovementScriptInit(data->mapObj) == 0) {
        return 0;
    }
    data->state = 2;
    return 1;
}

s32 MovementScriptMachineSub_SetMovementCommand(MoveAnimData *data);

s32 MovementScriptMachineSub_SetMovementCommand(MoveAnimData *data) {
    MapObject_SetHeldMovement(data->mapObj, (u32)data->animCmd->command);
    data->state = 3;
    return 0;
}

s32 MovementScriptMachineSub_WaitMovementCommand(MoveAnimData *data);

s32 MovementScriptMachineSub_WaitMovementCommand(MoveAnimData *data) {
    if (MapObject_IsMovementPaused(data->mapObj) == 0) {
        return 0;
    }
    data->state = 4;
    return 1;
}

s32 MovementScriptMachineSub_LoopCheck(MoveAnimData *data);

s32 MovementScriptMachineSub_LoopCheck(MoveAnimData *data) {
    s32 v0;
    const MovementScriptCommand *cmd;

    cmd = data->animCmd;
    v0 = data->count + 1;
    data->count = v0;
    if (v0 < (s32)cmd->length) {
        data->state = 1;
        return 1;
    }
    data->animCmd = cmd + 1;
    if (cmd[1].command != 0xFE) {
        data->state = 0;
        return 1;
    }
    data->ended = 1;
    data->state = 5;
    return 0;
}

s32 MovementScriptMachineSub_Done(void);

s32 MovementScriptMachineSub_Done(void) {
    return 0;
}
