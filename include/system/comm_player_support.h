// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/comm_player_support.h: the lines in
// use so far.
#ifndef BWPORT_SYSTEM_COMM_PLAYER_SUPPORT_H
#define BWPORT_SYSTEM_COMM_PLAYER_SUPPORT_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The support given
#define COMM_PLAYER_SUPPORT_NONE 0

// The battle has used it
#define COMM_PLAYER_SUPPORT_USED 3

// pokebw2's CommPlayerSupport_Create
CommPlayerSupport *func_0202e770(HeapID heapId);
// pokebw2's CommPlayerSupport_Init
void func_0202e79c(CommPlayerSupport *support);
// pokebw2's CommPlayerSupport_SetUsed
// Marks the support used, keeping what it was and who gave it
void func_0202e7e4(CommPlayerSupport *support);
// Whether a support is set or was given
BOOL func_0202e818(CommPlayerSupport *support);
// pokebw2's CommPlayerSupport_GetSupporter
// The player who gave the support the battle used, else the one who gave the support set
PlayerInfo *func_0202e82c(CommPlayerSupport *support);
// Sets a support another player gives, and the state of the support
void func_0202e7c4(CommPlayerSupport *support, u32 type, PlayerInfo *info);
u32 func_0202e7e0(CommPlayerSupport *support);
// pokebw2's CommPlayerSupport_EndBattle
// Clears the support after a battle, keeping what it was
void func_0202e80c(CommPlayerSupport *support);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CommPlayerSupport_Free
void func_0202e794(CommPlayerSupport *support);
// pokebw2's CommPlayerSupport_GetType

#endif // BWPORT_SYSTEM_COMM_PLAYER_SUPPORT_H
