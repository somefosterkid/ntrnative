#ifndef POKEHEARTGOLD_OVERLAY_01_021F1AFC_H
#define POKEHEARTGOLD_OVERLAY_01_021F1AFC_H

#include "field/field_3d_object_task.h"
#include "mon_ride_task.h"
#include "player_avatar.h"

void Field_PlayerAvatar_OrrTransitionFlags(PlayerAvatar *playerAvatar, int flags);
void Field_PlayerAvatar_ApplyTransitionFlags(PlayerAvatar *playerAvatar);
BOOL ov01_021F1D94(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int playerEvents);
void ov01_021F2F24(PlayerAvatar *playerAvatar);

// The task data of Field_PlayerMovementSavingSet, ov01_021F3030 and Field_PlayerMovementSavingClear. The FieldSystem and
// PlayerAvatar fields are pointers: the avatar sits at DS 0xC, host 0x10, so every user reads it by field.
typedef struct PlayerMovementSavingWork {
    s32 unk_00;
    s32 playerState;
    FieldSystem *fieldSystem;
    PlayerAvatar *playerAvatar;
} PlayerMovementSavingWork; // size 0x10 on the DS

// The task data of ov01_021F23B8 (created by ov01_021F2378). Pointer fields: read them by field on the host.
typedef struct PlayerAvatarUnk34Task {
    u16 state;
    u16 timer;
    s32 direction;
    FieldSystem *fieldSystem;
    PlayerAvatar *playerAvatar;
    LocalMapObject *mapObject;
    OverworldAnimManager *animManager;
} PlayerAvatarUnk34Task; // size 0x18 on the DS

// The task data of ov01_021F2118 (created by ov01_021F202C). monRideTask holds pointers (DS 0xC, size 0x10): every user reads
// the members by field.
typedef struct SurfTaskEnv {
    int state;
    int direction;
    int timer;
    MonRideTask monRideTask;
    FieldSystem *fieldSystem;
    PlayerAvatar *playerAvatar;
    LocalMapObject *playerObject;
    OverworldAnimManager *animManager;
} SurfTaskEnv; // size 0x2C on the DS

// The task data of ov01_021F2944 (Waterfall, created by ov01_021F28B8). monRideTask holds pointers (DS 0x40, size 0x10), so
// every user reads the members by field.
typedef struct WaterfallTaskEnv {
    int state;
    int direction;
    int movementCounter;
    int targetX;
    int targetY;
    int targetZ;
    fx32 distanceMoved;
    VecFx32 deltaPos;
    VecFx32 targetPosition;
    FieldSystem *fieldSystem;
    PlayerAvatar *playerAvatar;
    LocalMapObject *playerObject;
    MonRideTask monRideTask;
    Field3dObjectTask *field3dTask;
} WaterfallTaskEnv; // size 0x54 on the DS

// The task data of ov01_021F2DD0 (Whirlpool, created by ov01_021F2D68). Same tail as WaterfallTaskEnv; only state and
// direction are set before it.
typedef struct WhirlpoolTaskEnv {
    int state;
    int direction;
    u8 unused_08[0x2C];
    FieldSystem *fieldSystem;
    PlayerAvatar *playerAvatar;
    LocalMapObject *playerObject;
    MonRideTask monRideTask;
    Field3dObjectTask *field3dTask;
} WhirlpoolTaskEnv; // size 0x54 on the DS

// The task data of ov01_021F2628 (Rock Climb, created by ov01_021F2548). nextState is the state the movement continues
// with once the rock face is known (5 with the cut-in, 6 without).
typedef struct RockClimbTaskEnv {
    u16 state;
    u16 nextState;
    int direction;
    int dummy;
    FieldSystem *fieldSystem;
    PlayerAvatar *playerAvatar;
    LocalMapObject *playerObject;
    OverworldAnimManager *animManager;
    MonRideTask monRideTask;
    Field3dObjectTask *field3dTask;
} RockClimbTaskEnv; // size 0x30 on the DS

#endif // POKEHEARTGOLD_OVERLAY_01_021F1AFC_H
