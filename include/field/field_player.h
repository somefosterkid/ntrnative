// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_player.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_FIELD_PLAYER_H
#define BWPORT_FIELD_FIELD_PLAYER_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "field/field_effect.h"
#include "gfl/heap.h"

void FieldPlayer_GetWPosInDir(FieldPlayer *player, u16 direction, VecFx32 *position);
void FieldPlayer_SetSpecialSeq(FieldPlayer *player, u32 seq);
void FieldPlayer_SetSpecialState(FieldPlayer *player, u32 state);
BOOL func_ov021_021a2f54(FieldPlayer *player);
// pokebw2's func_ov036_0219a834 and func_ov036_0219a870
BOOL func_ov021_021a3208(FieldPlayer *player);
BOOL func_ov021_021a3244(FieldPlayer *player);
void FieldPlayer_ForceBrake(FieldPlayer *player);
void FieldPlayer_SetRailPos(FieldPlayer *player, const RailPosition *pos);
void FieldPlayer_GetRailWorldPos(FieldPlayer *player, VecFx32 *pos);
u32 FieldPlayer_DeriveExState(FieldPlayer *player);

// The player's grid and rail movement
void FieldPlayer_InitGrid(FieldPlayer *player, HeapID heapId);
BOOL FieldPlayer_CheckRunningShoesFlag(FieldPlayer *player);
void FieldPlayer_UpdateGridMovement(FieldPlayer *player, u32 pressedKeys, u32 heldKeys, u32 flags);
void FieldPlayer_UpdateRailMovement(FieldPlayer *player, u32 pressedKeys, u32 heldKeys);
FieldActor *FieldPlayer_GetActorInFrontEx(FieldPlayer *player, fx32 maxHeightDiff);
void FieldPlayer_GetGPosInFront(FieldPlayer *player, s16 *x, s16 *y, s16 *z);

void func_ov021_021a3664(FieldPlayer *player, HeapID heapId);

// The direction that stands for the player's own tile, and for no direction held
#define PLAYER_DIR_NONE 9

FieldActor *FieldPlayer_GetActorInFront(FieldPlayer *player);
BOOL CheckCanInteractWaterfall(FieldPlayer *player, u32 tileUnder, u32 tileInFront);
FieldPlayer *FieldPlayer_Create(PlayerState *state, Field *field, const VecFx32 *pos, u32 sex, HeapID heapId);
void FieldPlayer_Free(FieldPlayer *player);
void FieldPlayer_SyncState(FieldPlayer *player);
// The event of walking into an actor, or NULL. Only on the grid
GameEvent *FieldPlayer_CheckObjContactEvent(FieldPlayer *player, u32 pressedKeys, u32 heldKeys, u32 flags);
void FieldPlayer_UpdateActionStatusObserver(FieldPlayer *player);
// The form with the object code, 10 if it isn't one of the player's
u32 FieldPlayer_GetExStateByObjCode(u32 objCode);
void FieldPlayer_InterruptCatwalkBalance(FieldPlayer *player, BOOL keep);
BOOL FieldPlayer_GetVerticalMoveOnlyFlag(FieldPlayer *player);
void func_ov021_021a2fd0(FieldPlayer *player);
void func_ov021_021a356c(FieldPlayer *player);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL Field_ToggleCycling(Field *field);
u32 FieldPlayer_GetExState(FieldPlayer *player);
u32 FieldPlayer_GetTileTypeUnder(FieldPlayer *player);
u16 FieldPlayer_GetMoveDirByKey(FieldPlayer *player, u32 heldKeys);
u32 FieldPlayer_GetTileTypeInDir(FieldPlayer *player, u16 direction);
GameSystem *FieldPlayer_GetGameSystem(FieldPlayer *player);
BOOL func_ov021_021a34f8(FieldPlayer *player);
BOOL func_ov021_021a2fb0(FieldPlayer *player);
BOOL func_ov021_021a3698(FieldPlayer *player);
void func_ov021_021a368c(FieldPlayer *player);
void func_ov021_021a3680(FieldPlayer *player, RailPosition *pos);
RailUnit *func_ov021_021a36a4(FieldPlayer *player);
BOOL CheckSurfHeightAllow(FieldPlayer *player, u32 tileUnder, u32 frontTileType);
void FieldPlayer_ForceObjCode(FieldPlayer *player, u16 objCode);
void FieldPlayer_ClearObjCode(FieldPlayer *player);
void func_ov021_021a2f60(FieldPlayer *player);
// The core's terrain effect task: set and get
void func_ov021_021a2f8c(FieldPlayer *player, FieldEffectTask *task);
FieldEffectTask *func_ov021_021a2f98(FieldPlayer *player);
u32 func_ov021_021a3238(FieldPlayer *player);
void func_ov021_021a35a0(FieldPlayer *player, BOOL keep);

Field *FieldPlayer_GetField(FieldPlayer *player);
#endif // BWPORT_FIELD_FIELD_PLAYER_H
