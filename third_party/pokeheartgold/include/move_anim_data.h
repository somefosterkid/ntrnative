#ifndef POKEHEARTGOLD_MOVE_ANIM_DATA_H
#define POKEHEARTGOLD_MOVE_ANIM_DATA_H

#include "global.h"

struct LocalMapObject;

typedef struct MovementScriptCommand {
    u16 command;
    u16 length;
} MovementScriptCommand;

// The task data of the movement script machine (EventObjectMovementMan_Create, sMovementScriptMachineStateFuncs, EventObjectMovementMan_Delete).
// mapObj and animCmd are pointers: DS 0xC / 0x10, host 0x10 / 0x18, so every user reads them by field.
typedef struct MoveAnimData {
    int state;
    BOOL ended;
    int count;
    struct LocalMapObject *mapObj;
    const MovementScriptCommand *animCmd;
} MoveAnimData; // size 0x14 on the DS
#endif // POKEHEARTGOLD_MOVE_ANIM_DATA_H
