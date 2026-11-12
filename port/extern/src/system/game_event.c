// The PC build compiles src/system/game_event.c through this file (tools/port_sources.py). The port's transition window
// (the maintainers' notes, 5. and 7.): while a door, warp or map change, the Pokemon Center heal or the Waterfall climb runs,
// libntr's tick timeline runs at the Screen fades setting's speed (2x, 4x; Instant = 4x), as Battle speed does for a battle
// (libntr-ios-transitions.patch, SIM_SetTransition). The game system runs the event chain once a frame
// (src/system/game_system.c, GSYS_Update); the wrapper lets the game's GSYS_UpdateEvents run, then looks at the chain left
// (the event now running and the events it returns to): the window is open while one of them has a callback below. Each of
// those events ends with its sequence (the door's walk out, the heal's last ball, the map's fade in), and its parent (a
// script, or nothing: normal walking) goes on at the player's own speed. The Waterfall climb is a field task, not an event:
// port/extern/src/ov021/scrcmd_field_move.c sets PortTransition_waterfall from its start to its end. The DS build never
// sees this file.
#pragma push_macro("GSYS_UpdateEvents")
#define GSYS_UpdateEvents GSYS_UpdateEvents_game
#include "../../../../src/system/game_event.c"
#pragma pop_macro("GSYS_UpdateEvents")

#include <stdio.h>

#include "system/game_data.h"
#include "field/player_state.h"
#include "port_route.h"
#include <string.h>
#include "port_route_map_game.h"
#include "port_route_story_game.h"

extern int SIM_UISettingGet(const char *key);
extern void SIM_SetTransition(int speed);
extern void SIM_SetSave(int speed);

// src/ov020/event_mapchange.c: every map change's core (the field's close, the map load, the open; :488), and the
// sequences that run it: doors, warp tiles and mats (EventMapChangeWarp_Callback :580, func_ov020_02184410 :613, their
// door and walk in EventWarpSequence_CreateOut/In, src/ov021/ov021_021bd2b8.c), the script's and the grid's map changes
// (EventMapChange_Callback :672), the fake warp, quicksand, Escape Rope, Dig, Teleport, diving, warp pads, the Union Room,
// whiting out and the Entralink's warps. Not the game's start (EventGameOpening/FieldFirst/FieldContinue) and not the
// ending (func_ov020_0218466c, EventMapChangeEnding_Create :1442: the credits keep their pace).
GameEventReturnCode EventMapChangeCore_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeWarp_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov020_02184410(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChange_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeFakeWarp_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeQuicksand_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeEscapeRope_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeDig_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeTeleport_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeDiveOut_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeDiveIn_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeWarpPad_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeUnionRoomExit_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventUnionRoomWarp_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventMapChangeBlackout_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventEntralinkWarp_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventEntralinkWarpIn_Callback(GameEvent *event, u32 *state, void *data);
// src/ov021/event_pokemon_center.c:35: the nurse's balls, the machine and its jingle (the event waits for the jingle, music
// on the real-time bus, so that part keeps its length)
GameEventReturnCode EventPokeCenHeal_Callback(GameEvent *event, u32 *state, void *data);

static const GameEventCallback sTransitionCallbacks[] = {
    EventMapChangeCore_Callback, EventMapChangeWarp_Callback, func_ov020_02184410, EventMapChange_Callback,
    EventMapChangeFakeWarp_Callback, EventMapChangeQuicksand_Callback, EventMapChangeEscapeRope_Callback,
    EventMapChangeDig_Callback, EventMapChangeTeleport_Callback, EventMapChangeDiveOut_Callback,
    EventMapChangeDiveIn_Callback, EventMapChangeWarpPad_Callback, EventMapChangeUnionRoomExit_Callback,
    EventUnionRoomWarp_Callback, EventMapChangeBlackout_Callback, EventEntralinkWarp_Callback,
    EventEntralinkWarpIn_Callback, EventPokeCenHeal_Callback,
};

BOOL PortTransition_waterfall;

static BOOL PortTransition_InChain(GameEvent *event) {
    for (; event != NULL; event = event->parent) {
        for (u32 i = 0; i < sizeof(sTransitionCallbacks) / sizeof(sTransitionCallbacks[0]); i++) {
            if (event->callback == sTransitionCallbacks[i]) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// The save window (libntr-ios-savespeed.patch, SIM_SetSave): while the game saves, libntr's tick timeline runs at the Save
// speed setting (2x, 4x). The field menu's Save is src/ov010/event_save.c's event (EventSave_Callback, chained by the field
// menu's proc link, event_field_proclink.c), run by src/ov010/report_event.c's EventSave_Update: states 0-4 are the bottom
// screen's change, the save question and its Yes/No (the player's speed: a 2x or 4x Yes/No menu would be
// hard to use), 5 on are after Yes: the bottom screen's save animation, the saving message, the write (8-9), the save
// sound (10-12), the saved message held 150 frames or until A (16) and the close (19-20); 17-18 are No's close.
// Any other save (a script's, src/ov010/scrcmd_vm.c's func_ov010_02159a64, the game clear's, ov205) is covered while it
// writes: GameData's saving flag (src/system/game_data.c, func_02012dac sets it, func_02012dd0 clears it at the end).
GameEventReturnCode EventSave_Callback(GameEvent *event, u32 *state, void *data);

// *state: the field menu save's state, -1 when it is not in the chain (each change is logged: where a save stops shows in the log)
static BOOL PortSave_InChain(GameSystem *gsys, int *state) {
    GameEvent *event;

    *state = -1;
    for (event = GSYS_GetNowEvent(gsys); event != NULL; event = event->parent) {
        if (event->callback == EventSave_Callback) {
            *state = (int)event->state;
            break;
        }
    }
    if (GameData_CheckEventsPaused(GSYS_GetGameData(gsys))) {
        return TRUE;
    }
    return *state >= 5 && *state != 17 && *state != 18;
}

BOOL GSYS_UpdateEvents(GameSystem *gsys) {
    static BOOL sOn, sSaveOn;
    static int sSaveState = -1;
    int saveState;
    BOOL ret = GSYS_UpdateEvents_game(gsys);
    BOOL save = PortSave_InChain(gsys, &saveState);
    // savespeed 1 = off, 2, 4
    int saveSpeed = save ? SIM_UISettingGet("savespeed") : 0;

    if (save != sSaveOn) {
        fprintf(stderr, "bwport: save %s (frame %u, speed %d)\n", save ? "start" : "end", (unsigned)OS_GetVBlankCount(),
                saveSpeed > 1 ? saveSpeed : 1);
        sSaveOn = save;
    }
    if (saveState != sSaveState) {
        fprintf(stderr, "bwport: save state %d (frame %u)\n", saveState, (unsigned)OS_GetVBlankCount());
        sSaveState = saveState;
    }
    SIM_SetSave(saveSpeed > 1 ? saveSpeed : 0);
    BOOL on = PortTransition_waterfall || PortTransition_InChain(GSYS_GetNowEvent(gsys));
    // fadespeed 0 = the game's, 1 = 2x, 2 = 4x, 3 = Instant (4x here)
    int fade = SIM_UISettingGet("fadespeed");
    int speed = !on || fade < 1 ? 0 : fade == 1 ? 2 : 4;

    if (on != sOn) {
        fprintf(stderr, "bwport: transition %s (frame %u, speed %d)\n", on ? "start" : "end", (unsigned)OS_GetVBlankCount(),
                speed ? speed : 1);
        sOn = on;
    }
    SIM_SetTransition(speed);
    // the first frame of a boot with no event running: the field's start (EventFieldContinue's fade in) is over and the
    // player can move. Tests count their keys from it (tools/mac_run.sh --save prints it); the static is the machine's, so
    // a reset clears it
    static BOOL sFieldUp;
    if (!sFieldUp && GSYS_GetNowEvent(gsys) == NULL) {
        fprintf(stderr, "bwport: field: up at V-blank %u (no event running)\n", (unsigned)OS_GetVBlankCount());
        sFieldUp = TRUE;
    }
    // the route test hooks (port/src/route_test.c): the position log and the pilot; both off unless their variable is set
    PlayerState *ps = GSYS_GetPlayerState(gsys);
    if (ps != NULL) {
        VecFx32 *pos = PlayerState_GetWPos(ps);
        PortRouteState rs = {PORT_ROUTE_GAME, PlayerState_GetZoneID(ps), pos->x / (16 << 12), pos->z / (16 << 12),
                             GSYS_GetNowEvent(gsys) != NULL, saveState, -1, pos->x >> 12, pos->z >> 12};

        sPortMapGsys = gsys;   // the pilot's map (port/include/port_route_map_game.h)
        PortRouteMap_SetAdapter(&kPortRouteGen5);
        PortStory_Hook(gsys);   // the story readout (port/include/port_route_story_game.h)
        PortRoute_Frame(&rs);
    }
    return ret;
}
