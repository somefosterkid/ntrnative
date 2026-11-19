// (Black 2 / White 2) The PC build compiles pokebw2 src/ov280/name_input.c through this file (tools/port_sources.py). Test only
// (port/src/route_test.c, BWPORT_PILOT's Pk token): it tells the route pilot that the naming screen is up (gPortRouteNameIn: 1
// from the proc's init, NameEntry_ProcInit, to its exit, NameEntry_ProcExit). The proc table (NAME_ENTRY_PROC_FUNCTIONS) sits in
// the file with the functions, so the file's own references would skip a wrapper: the table is renamed too and defined again
// here over the wrappers. Pk:pw (gPortRouteNamePassword) also types the Plasma Frigate's password (White 2: the barrier
// terminal, zone 563, NAME_ENTRY_PLASMA_PASSWORD) into the name box, as the keyboard would. It changes nothing else. The DS
// build never sees this file.
#pragma push_macro("NameEntry_ProcInit")
#pragma push_macro("NameEntry_ProcMain")
#pragma push_macro("NameEntry_ProcExit")
#pragma push_macro("NAME_ENTRY_PROC_FUNCTIONS")
#define NameEntry_ProcInit NameEntry_ProcInit_game
#define NameEntry_ProcMain NameEntry_ProcMain_game
#define NameEntry_ProcExit NameEntry_ProcExit_game
#define NAME_ENTRY_PROC_FUNCTIONS NAME_ENTRY_PROC_FUNCTIONS_game
#include "../../../../third_party/pokebw2/src/ov280/name_input.c"
#pragma pop_macro("NameEntry_ProcInit")
#pragma pop_macro("NameEntry_ProcMain")
#pragma pop_macro("NameEntry_ProcExit")
#pragma pop_macro("NAME_ENTRY_PROC_FUNCTIONS")
#include "save/event_work.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include <stdio.h>

extern int gPortRouteNameIn;         // port/src/route_test.c: 1 while the naming screen's proc runs
extern int gPortRouteNamePassword;   // port/src/route_test.c: 1 while Pk:pw runs (type the frigate's password)

// The work the frigate terminal's script fills with the game's GetPasswordIndex (zone 563 script 2: GetPasswordIndex 0x400a,
// then EnterPassword); PlasmaFrigate_CheckPassword (ov064 scrcmd_plasma_frigate.c) compares the entry with that message
static int sPortPasswordTyped;

static void PortNameEntry_TypePassword(NameEntryWork *work) {
    GameData *gd = GSYS_GetGameData(g_GameSystem);
    u16 index = *EventWork_GetWkPtr(GameData_GetEventWork(gd), 0x400a);
    MsgData *msgData =
        GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_SCRCMD_PLASMA_FRIGATE_SCRCMD_BSUBWAY, HEAPID_NAME_INPUT);
    StrBuf *password = GFL_MsgDataLoadStrbufNew(msgData, index);

    NameBox_SetString(&work->box, GFL_StrBufGetStringPtr(password));   // the icon's preset-name path: AddChar per letter

    fprintf(stderr, "pilot: Pk:pw: the frigate password (message %u, %u letters) typed into the name box\n", (unsigned)index,
            (unsigned)GFL_StrBufGetCharCount(password));
    GFL_StrBufFree(password);
    GFL_MsgDataFree(msgData);
}

BOOL NameEntry_ProcInit(GameProc *proc, u32 *state, void *arg, void *unused) {
    BOOL r = NameEntry_ProcInit_game(proc, state, arg, unused);

    gPortRouteNameIn = 1;
    sPortPasswordTyped = 0;
    return r;
}

BOOL NameEntry_ProcMain(GameProc *proc, u32 *state, void *param, void *arg) {
    NameEntryParam *p = param;

    if (gPortRouteNamePassword && !sPortPasswordTyped && arg != NULL && p->mode == NAME_ENTRY_PLASMA_PASSWORD) {
        sPortPasswordTyped = 1;
        PortNameEntry_TypePassword(arg);
    }
    return NameEntry_ProcMain_game(proc, state, param, arg);
}

BOOL NameEntry_ProcExit(GameProc *proc, u32 *state, void *param, void *arg) {
    NameEntryParam *p = param;

    if (gPortRouteNamePassword && p != NULL && p->mode == NAME_ENTRY_PLASMA_PASSWORD) {   // what the script will compare
        fprintf(stderr, "pilot: Pk:pw: the naming screen closed: canceled %d, %u letters entered\n", (int)p->canceled,
                (unsigned)GFL_StrBufGetCharCount(p->name));
    }
    BOOL r = NameEntry_ProcExit_game(proc, state, param, arg);

    gPortRouteNameIn = 0;
    return r;
}

const GameProcFunctions NAME_ENTRY_PROC_FUNCTIONS = { NameEntry_ProcInit, NameEntry_ProcMain, NameEntry_ProcExit };
