// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/game_comm.h, the lines in use so
// far. pokebw2's GameCommSys_ExitReq is Black's func_02026400.
#ifndef BWPORT_SYSTEM_GAME_COMM_H
#define BWPORT_SYSTEM_GAME_COMM_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// The game's communication system (game_comm.c): it runs one communication at a time, identified by a GameCommNo
enum GameCommNo {
    GAME_COMM_NO_NULL,
    GAME_COMM_NO_BEACON_SEARCH,
    // The intrusion, whose work intrude_work.c reads; Black 2 left it without callbacks
    GAME_COMM_NO_INTRUDE,
    // The Union Room's communication (union_main.c)
    GAME_COMM_NO_UNION,
    // The musical's communication (mus_comm_func.c)
    GAME_COMM_NO_MUSICAL = 4,
};

// A met player's log entry as the greeting list reads it
typedef struct {
    StrBuf *name[2];
    u8 unk8[2];
    u8 gender[2];
    u16 beaconType;
} GameCommLogOut;

void func_02026400(GameCommSys *comm);
// The running communication's GameCommNo, or GAME_COMM_NO_NULL
u8 GameCommSys_BootCheck(GameCommSys *comm);
// Whether a communication is running but booting or exiting (pokebw2's GameCommSys_IsTransitioning)
BOOL func_02026460(GameCommSys *comm);
// The running communication's work (pokebw2's GameCommSys_GetWork)
void *func_02026474(GameCommSys *comm);
// Takes the oldest player met out of the log, or 0 when there is none
u8 func_02026608(GameCommSys *comm, GameCommLogOut *out);
u8 func_0202647c(GameCommSys *comm);
GameData *getBasePlayerBlk(GameCommSys *comm);
u32 func_0202648c(GameCommSys *comm);

void FreeGameComm(GameCommSys *comm);
// pokebw2's GameCommSys_Main
// Runs the current communication's callbacks; called once a frame
void func_0202627c(GameCommSys *comm);
// pokebw2's GameCommSys_Boot
// Starts a communication, which gets param in its callbacks
void func_020263d8(GameCommSys *comm, u8 commNo, void *param);

// pokebw2's GameCommSys_Boot
// Starts a communication, which gets param in its callbacks

// How many players GameCommSys keeps in its log of the last ones met
#define GAME_COMM_LOG_COUNT 6

// Black 2's, not checked against Black
// Called when a communication has ended. exitReqAgain is whether an exit was requested again while it was exiting
typedef void (*GameCommExitCallback)(void *arg, BOOL exitReqAgain);

// pokebw2's GameCommSys_FieldCreate
// Call the current communication's callbacks for the field being created and deleted
void func_02026388(GameCommSys *comm, Field *field);
void func_02026498(GameCommSys *comm, BOOL flag);
// Logs two players met over a communication, by net ID: as beacon type 0x27 if they are the same, else 0x26
void func_020266ac(GameCommSys *comm, u32 netId1, u32 netId2);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's GameCommSys_Create
GameCommSys *func_020261e4(HeapID heapId, GameData *gameData);
// pokebw2's GameCommSys_FieldDelete
void func_020263b0(GameCommSys *comm, Field *field);
u8 func_020264ac(GameCommSys *comm);
void func_020264b4(GameCommSys *comm, u32 netId, u16 unk0, u8 unk5, u32 unk4, BOOL noLog);
// The result the communication ends with, and the end itself
void func_02026480(GameCommSys *comm, u32 value);
// Logs a player met over the communication (func_02026504 with beacon type 0x1f)
void func_020266d4(GameCommSys *comm, u32 netId);
void func_02026680(GameCommSys *comm);
// Logs the player, as beacon type 0x37
void func_020266c0(GameCommSys *comm, u32 netId1, u32 netId2);
void func_0202640c(GameCommSys *comm, GameCommExitCallback callback, void *callbackArg);
void func_02026484(GameCommSys *comm);
void func_020264f4(GameCommSys *comm);

#endif // BWPORT_SYSTEM_GAME_COMM_H
