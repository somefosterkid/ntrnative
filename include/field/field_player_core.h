// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_player_core.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_PLAYER_CORE_H
#define BWPORT_FIELD_FIELD_PLAYER_CORE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "field/field_effect.h"

// Black's layouts, only the fields the code uses so far (the rest by offset)
// The field's player: the core, and the grid and rail movement that the map's controller sets up
struct FieldPlayer {
    Field *field;
    FieldPlayerCore *core;
    FieldPlayerGrid *grid;
    FieldPlayerNoGrid *nogrid;
};

struct FieldPlayerCore {
    u32 unk_0;
    GameSystem *gsys; // 0x4
    Field *field; // 0x8
    PlayerState *playerState; // 0xc
    u32 observeStatus; // 0x10
    u32 moveStatus; // 0x14
    u32 sex; // 0x18
    FieldActor *actor; // 0x1c
    u16 keyDirH; // 0x20
    u16 keyDirV; // 0x22
    u32 specialSeq; // 0x24
    FieldEffectTask *terrainEffect; // 0x28
    u16 unk_2c;
    u16 unk_2e;
    s16 seqStep; // 0x30
    u16 seqDone; // 0x32
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
FieldPlayerCore *FieldPlayerCore_Create(PlayerState *state, Field *field, const VecFx32 *pos, u32 sex, HeapID heapId);
void FieldPlayerCore_Free(FieldPlayerCore *core);
void FieldPlayerCore_UpdateActionStatusObserver(FieldPlayerCore *core);
void FieldPlayerCore_GetWPos(FieldPlayerCore *core, VecFx32 *pos);
u16 FieldPlayerCore_GetFaceDir(FieldPlayerCore *core);
FieldActor *FieldPlayerCore_GetActor(FieldPlayerCore *core);
GameSystem *FieldPlayerCore_GetGameSystem(FieldPlayerCore *core);
void FieldPlayerCore_ForceMoveObserveStatus(FieldPlayerCore *core, u32 status);
u32 FieldPlayerCore_GetExState(FieldPlayerCore *core);
PlayerState *FieldPlayerCore_GetPlayerState(FieldPlayerCore *core);
u16 FieldPlayerCore_GetMoveDirByKey(FieldPlayerCore *core, u32 heldKeys);
void FieldPlayerCore_SetSpecialSeq(FieldPlayerCore *core, u32 seq);
void FieldPlayerCore_SetMoveStatus(FieldPlayerCore *core, u32 status);
void FieldPlayerCore_InitModel(FieldPlayerCore *core);
void FieldPlayerCore_CalcSeparateKeyDirs(FieldPlayerCore *core, u32 heldKeys);
void FieldPlayerCore_ForceObjCode(FieldPlayerCore *core, u16 objCode);
void FieldPlayerCore_ClearObjCode(FieldPlayerCore *core);
u32 FieldPlayerCore_DeriveExState(FieldPlayerCore *core);
FieldEffectTask *FieldPlayerCore_GetTerrainEffectTCB(FieldPlayerCore *core);
BOOL func_ov021_021a3cc0(FieldPlayerCore *core);
void func_ov021_021a3ce8(FieldPlayerCore *core);
void func_ov021_021a3ad0(FieldPlayerCore *core, FieldEffectTask *task);
BOOL func_ov021_021a3a20(FieldPlayerCore *core);
BOOL func_ov021_021a3a3c(FieldPlayerCore *core);
BOOL func_ov021_021a39e4(FieldPlayerCore *core);
BOOL FieldPlayerCore_IsAcmdFinished(FieldPlayerCore *core);
// Changes the player's model to a form's
void func_ov021_021a39ac(FieldPlayerCore *core, s32 forme);
void func_ov021_021a392c(FieldPlayerCore *core, u16 dir);

Field *FieldPlayerCore_GetField(FieldPlayerCore *core);
FieldPlayerGrid *func_ov021_021a36e0(FieldPlayer *player);
#endif // BWPORT_FIELD_FIELD_PLAYER_CORE_H
