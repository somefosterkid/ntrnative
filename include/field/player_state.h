// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/player_state.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_PLAYER_STATE_H
#define BWPORT_FIELD_PLAYER_STATE_H

#include "types.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"
#include "save/player_info.h"

u16 PlayerState_GetZoneID(PlayerState *playerState);

// Black 2's; twinned code confirms the offsets of zoneId, position, unk10, unk12, unk14, unk16, unk18, unk1B,
// playerInfo and exState in Black
struct PlayerState {
    u16 zoneId;
    u16 unk2;
    VecFx32 position;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk18;
    u8 unk1A;
    u8 unk1B;
    u16 objCode;
    u16 unk1E;
    PlayerInfo playerInfo;
    u32 exState;
};

// How the player is moving, which FieldPlayerState_GetExState returns. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
typedef enum {
    FLD_PLAYER_EXSTATE_NONE = 0x0,
    FLD_PLAYER_EXSTATE_CYCLING = 0x1,
    FLD_PLAYER_EXSTATE_SURF = 0x2,
    FLD_PLAYER_EXSTATE_DIVE = 0x3,
} PlayerExState;

void PlayerState_SetRotation(PlayerState *playerState, u16 angle);
// Whether the player is on a rail, which PlayerState_SetIsRail sets
u8 func_02012ac0(PlayerState *playerState);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 FieldPlayerState_GetExState(PlayerState *playerState);
u16 PlayerState_CalcDirection(PlayerState *playerState);
VecFx32 *PlayerState_GetWPos(PlayerState *playerState);
RailPosition *PlayerState_GetRailPos(PlayerState *playerState);
void PlayerState_SetIsRail(PlayerState *playerState, BOOL isRail);
void PlayerState_SetRailPos(PlayerState *playerState, const RailPosition *pos);
void PlayerState_SetRotationByDir(PlayerState *playerState, u16 dir);
void PlayerState_SetWPos(PlayerState *playerState, VecFx32 *pos);
void PlayerState_SetZoneID(PlayerState *playerState, u16 zoneId);
void SetPlayerSpecialState(PlayerState *playerState, u32 state);
u16 FieldPlayerState_GetObjCode(PlayerState *playerState);
void FieldPlayerState_SetObjCode(PlayerState *playerState, u16 objCode);
u16 func_02012ad0(PlayerState *playerState); // the rotation that PlayerState_SetRotation sets

#endif // BWPORT_FIELD_PLAYER_STATE_H
