// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/comm_player.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_COMM_PLAYER_H
#define BWPORT_FIELD_COMM_PLAYER_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"

void func_ov010_021675d0(CommPlayerSys *cps, int index, u16 a2, const CommPlayerStatus *status);
void func_ov010_02167848(CommPlayerSys *cps, int index, const CommPlayerStatus *status);

CommPlayerSys *func_ov010_02167518(u8 a0, GameSystem *gsys, HeapID heapId, u8 a3);

#define COMM_PLAYER_MAX 4

// Where a player is, as the players send it to each other
struct CommPlayerStatus {
    u32 unk0;
    VecFx32 pos;
    u16 dir;
    u8 exState;
};

// Black 2's; twinned code confirms the offsets of pos, unkC, dir, unk12, active, hidden and actor in Black
struct CommPlayer {
    VecFx32 pos;
    u32 unkC;
    u16 dir;
    u16 unk12;
    u8 active : 1;
    // Set while the actor is taken down for the field to close
    u8 hidden : 1;
    u8 exState : 4;
    void *actor;
};

// The player's own state as it was last sent
struct CommPlayerSent {
    VecFx32 pos;
    u16 dir;
    u8 exState : 4;
    u8 busy : 1;
};

// Black 2's; twinned code confirms the offsets of gsys, act_ctrl, players, last, heapId, unk8E, unk8F and unk90, and
// its size in Black
struct CommPlayerSys {
    GameSystem *gsys;
    FldCommActSys *act_ctrl;
    CommPlayer players[COMM_PLAYER_MAX];
    CommPlayerSent last;
    u32 unk88;
    u16 heapId;
    u8 unk8E;
    u8 unk8F;
    u8 unk90;
};

void func_ov010_02167590(GameSystem *gsys, CommPlayerSys *cps);
// The player's own state, and whether it changed since it was last sent
BOOL func_ov010_0216786c(CommPlayerSys *cps, CommPlayerStatus *status);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov010_0216754c(GameSystem *gsys, CommPlayerSys *cps);
u32 func_ov010_021679a4(CommPlayerSys *cps, s16 a1, s32 a2, s16 a3, u32 *a4, u32 *a5);
void func_ov010_021676e4(CommPlayerSys *cps);
void func_ov010_02167794(CommPlayerSys *cps);
// Whether a player's actor is up, and takes it down
BOOL func_ov010_021677e4(CommPlayerSys *cps, int index);
BOOL func_ov010_021679c4(CommPlayerSys *cps, int index);
void func_ov010_02167658(CommPlayerSys *cps, int index);
u16 func_ov010_02167810(CommPlayerSys *cps, int index);
// Takes the actors down before the field closes, and puts them back after
void func_ov010_02167688(CommPlayerSys *cps);
BOOL func_ov010_02167a54(CommPlayerSys *cps, int index);
FieldActor *func_ov010_02167a14(CommPlayerSys *cps, int index);

#endif // BWPORT_FIELD_COMM_PLAYER_H
