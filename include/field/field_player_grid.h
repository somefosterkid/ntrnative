// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_player_grid.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_PLAYER_GRID_H
#define BWPORT_FIELD_FIELD_PLAYER_GRID_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Whether the player is sliding on ice
struct FieldPlayerGrid {
    u32 state;
    u32 command;
    u32 flags;
    FieldPlayerCore *core;
    Field *field;
    u16 balanceTime;
    u8 balanceKept;
    u16 stepSE;
    u16 stepCount;
    BOOL verticalMoveOnly;
};

BOOL FieldPlayerGrid_IsConstMoving(FieldPlayerGrid *grid);
// Puts the player back on the ground after the fall from a catwalk
void FieldPlayerGrid_SanitizeHeightMismatch(FieldPlayerGrid *grid);

// One frame of the player's movement. Bit 0 of the flags allows running
void FieldPlayerGrid_UpdateMovement(FieldPlayerGrid *grid, u32 pressedKeys, u32 heldKeys, u32 flags);
// Ends the animation of walking into a wall
void FieldPlayerGrid_ForceBrake(FieldPlayerGrid *grid);
// Stops the balancing on a catwalk, keeping its time if asked
void FieldPlayerGrid_InterruptCatwalkBalance(FieldPlayerGrid *grid, BOOL keep);
void FieldPlayerGrid_SetConstMoveEndSpotFlag(FieldPlayerGrid *grid);
// pokebw2's FieldPlayerGrid_IsBraking
// Whether the player is walking into a wall
BOOL func_ov021_021a5778(FieldPlayerGrid *grid);

// How a held direction continues the player's movement, which FieldPlayerGrid_GetMoveDirContType returns
#define MOVE_DIR_CONT_NONE 0

#define MOVE_DIR_CONT_MOVE 1
#define MOVE_DIR_CONT_TURN 2

u32 FieldPlayerGrid_GetMoveDirContType(FieldPlayerGrid *grid, u16 dir);
// The same, for a turn too. FALSE while the player is moving
BOOL func_ov021_021a5408(FieldPlayerGrid *grid);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Whether the player is stepping or falling off a catwalk
BOOL FieldPlayerGrid_IsCatwalkExiting(FieldPlayerGrid *grid);
FieldActor *FieldPlayerGrid_GetPlayerActor(FieldPlayerGrid *grid);
// Set when the fall from a catwalk lands, until the next frame of movement
BOOL FieldPlayerGrid_GetVerticalMoveOnlyFlag(FieldPlayerGrid *grid);
GameEvent *FieldPlayerGrid_CheckObjContactEvent(FieldPlayerGrid *grid, u32 pressedKeys, u32 heldKeys, u32 flags);
// pokebw2's FieldPlayerGrid_Free
void func_ov021_021a41a8(FieldPlayerGrid *grid);
// Ends a slide
void func_ov021_021a575c(FieldPlayerGrid *grid);
// pokebw2's FieldPlayerGrid_Create
FieldPlayerGrid *func_ov021_021a4140(FieldPlayerCore *core, HeapID heapId);
BOOL FieldPlayerGrid_IsAcmdFinished(FieldPlayerGrid *grid, u16 dir);
FieldPlayerCore *FieldPlayerGrid_GetPlayerCore(FieldPlayerGrid *grid);
// pokebw2's FieldPlayerGrid_SetVerticalMoveOnlyFlag
void func_ov021_021a5768(FieldPlayerGrid *grid, BOOL keep);

#endif // BWPORT_FIELD_FIELD_PLAYER_GRID_H
