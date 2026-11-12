// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/intrude_work.h, extended for Black.
#ifndef BWPORT_FIELD_INTRUDE_WORK_H
#define BWPORT_FIELD_INTRUDE_WORK_H

// Overlay 10's intrude_work.c (pokebw2's guess at the name, after Black and White's intrusion functions): what the
// field asks of the intrusion's communication about the other players. Black 2 left most of it as stubs.
// getGameOrigin and getSeasonFromPlayerData are swan's names

#include "types.h"
#include "field/intrude_comm.h"
#include "struct_decls.h"

// Whether the season may change, which it may not while it is kept in sync with another player
BOOL func_ov010_02158318(GameSystem *gsys);
u32 func_ov010_02158378(GameCommSys *commSys);
BOOL func_ov010_0215839c(IntrudeComm *work, u32 mask, u8 unused);
PassPowerLevel *func_ov010_021583cc(GameData *gameData, u32 netId);
PlayerInfo *func_ov010_021583ec(GameData *gameData, u32 netId);
IntrudeCommPlayer *func_ov010_0215840c(IntrudeComm *work, u32 index);
u8 func_ov010_0215841c(GameData *gameData);
u8 func_ov010_02158428(IntrudeComm *work);
void *func_ov010_02158434(IntrudeComm *work);
// The game the other player plays, this one's while the season isn't kept in sync
u32 getGameOrigin(GameCommSys *commSys);
u8 getSeasonFromPlayerData(GameCommSys *commSys);
BOOL func_ov010_021584a4(GameCommSys *commSys, GameData *gameData, u32 dir);
u32 func_ov010_021584f4(u8 version, u32 id);
u32 func_ov010_0215850c(GameCommSys *commSys, GameData *gameData);
BOOL func_ov010_02158570(u16 zoneId);
BOOL func_ov010_02158590(GameData *gameData);
BOOL func_ov010_021585bc(IntrudeComm *work);
u32 func_ov010_021585e0(GameCommSys *commSys);
void func_ov010_02158614(GameCommSys *commSys);
u32 func_ov010_02158630(GameCommSys *commSys);
u32 func_ov010_02158678(GameCommSys *commSys);
BOOL func_ov010_021586c8(GameCommSys *commSys, StrBuf *name);
BOOL func_ov010_0215870c(GameCommSys *commSys);
void func_ov010_0215874c(GameCommSys *commSys);

#endif // BWPORT_FIELD_INTRUDE_WORK_H
