// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/game_system.h, the lines in use so
// far.
#ifndef BWPORT_SYSTEM_GAME_SYSTEM_H
#define BWPORT_SYSTEM_GAME_SYSTEM_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "system/game_event.h"

Field *GSYS_GetField(GameSystem *gsys);
GameData *GSYS_GetGameData(GameSystem *gsys);
extern GameSystem *g_GameSystem;
void SetupGameSystem(GameSystem *gsys, HeapID heapId, GameSystemProcData *procData);
BOOL GSYS_Update(GameSystem *gsys);
ISS *GameSystem_GetISS(GameSystem *gsys);
GameCommSys *GSYS_GetGameCommSystem(GameSystem *gsys);
BOOL GSYS_GetProcMgrState(GameSystem *gsys);
// pokebw2's GSYS_TryBootGameComm
BOOL func_020121cc(GameSystem *gsys);
void func_0201213c(GameSystem *gsys, u32 value);
u8 func_02012144(GameSystem *gsys);
u32 func_0201214c(GameSystem *gsys);
void func_02012158(GameSystem *gsys, u32 value);
void GSYS_QueueProc(GameSystem *gsys, s32 overlayId, const GameProcFunctions *functions, void *param);
void GSYS_QueueProcAsEvent(GameEvent *event, s32 overlayId, const GameProcFunctions *functions, void *param);

typedef enum {
    GAME_ENTRYPOINT_OPENING,
    GAME_ENTRYPOINT_FIELD_CONTINUE,
    GAME_ENTRYPOINT_DEBUG,
} GameEntryPoint;

// Black 2's; twinned code confirms the offsets of entryPoint in Black
struct GameSystemProcData {
    GameEntryPoint entryPoint;
    VecFx32 spawnPos;
    u16 zoneId;
    u16 unk12;
};

// Black 2's; matching code confirms the offsets of procManager, procManagerState, eventProvider, eventProviderData,
// event, gameData, field, comm, iss, eventRunning and unk37_0, and its size in Black
// The game itself, from the title screen's choice to the reset: the process that owns the game data, the field map,
// the running event and the processes the events start
struct GameSystem {
    HeapID heapId;
    GameSystemProcData *procData;
    GameProcManager *procManager;
    // What GFL_ProcMgrUpdate returned last: 0 with no process left
    u32 procManagerState;
    // Asked for a new event each frame that none runs
    GameEventProvider eventProvider;
    void *eventProviderData;
    GameEvent *event;
    GameData *gameData;
    // NULL while the field map is down
    Field *field;
    GameCommSys *comm;
    u32 unk28;
    ISS *iss;
    u8 unk34;
    // Whether an event ran at the start of the frame
    u8 eventRunning;
    // Nonzero keeps GSYS_TryBootGameComm from starting the field's communication
    u8 unk36;
    // Set when the console wakes with the communication on
    u8 unk37_0 : 1;
};

void GSYS_SetEventProvider(GameSystem *gsys, void *provider, void *data);
// The anti-piracy check's V-blank task: two draws of the Mersenne Twister
void func_02011d34(TCB *tcb, void *data);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Whether the field map is up, which Game Freak's asserts call GAMESYSTEM_CheckFieldMapWork
BOOL GSYS_CheckField(GameSystem *gsys);
PlayerState *GSYS_GetPlayerState(GameSystem *gsys);
GameEvent *GSYS_GetNowEvent(GameSystem *gsys);
void GSYS_SetField(GameSystem *gsys, Field *field);
void GSYS_StartFieldmapProc(GameSystem *gsys);
GameSystemProcData *GameSystem_CreateProcData(GameEntryPoint entryPoint, u16 zoneId, VecFx32 *spawnPos, s16 unk12);
BOOL GSYS_GetEventRunningFlag(GameSystem *gsys);
// Whether an event is running
BOOL GSYS_CheckNowEvent(GameSystem *gsys);
// pokebw2's func_02016b14: the byte at 0x30 of the system, which UnionMain_Update tests
u8 func_0201212c(GameSystem *gsys);
void GSYS_SetNowEvent(GameSystem *gsys, GameEvent *event);
BOOL func_02012204(GameSystem *gsys);
void func_02012210(GameSystem *gsys);
void func_02012124(GameSystem *gsys, u32 a1);

BOOL func_02012164(GameSystem *gsys);
#endif // BWPORT_SYSTEM_GAME_SYSTEM_H
