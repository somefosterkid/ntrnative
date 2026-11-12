// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/player_action.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_PLAYER_ACTION_H
#define BWPORT_FIELD_PLAYER_ACTION_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"

// What the player can do where they stand, which the hidden moves check
struct PlayerActionPossibilities {
    u16 zoneId;
    u16 flags;
    u32 exState;
    GameSystem *gsys;
    FieldActor *actorInFront;
    Field *field;
};

void CalcPlayerActionPossibilities(Field *field, PlayerActionPossibilities *action);

// The number of field actions that PlayerActionPerms blocks or allows
#define PLAYER_ACTION_COUNT 10

// Black 2's; matching code confirms the offsets of exState and blocked, and its size in Black
struct PlayerActionPerms {
    u32 unk0;
    u32 exState;
    u16 paired;
    u8 unkA[10];
    // A bit for each field action blocked where the player stands
    u32 blocked;
};

// Sets the bit of the action to blocked, and reads it
void func_02028748(PlayerActionPerms *perms, u32 action, u32 blocked);
BOOL func_02028754(PlayerActionPerms *perms, u32 action);

BOOL IsNPCStrengthRock(u16 objCode);
BOOL func_ov010_0216e5b0(MMSys *actorSystem, const VecFx32 *position);

#endif // BWPORT_FIELD_PLAYER_ACTION_H
