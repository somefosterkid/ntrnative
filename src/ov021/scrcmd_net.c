// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_net.c, the same code in Black
// 2/White 2's overlay 36. Black has the Wi-Fi Club in overlay 6, where Black 2 has it in overlay 8.

// The script commands of the network: the game communication's status, the Wi-Fi friend list's state, and starting
// the Wi-Fi Club, the GTS and GTS negotiations from their overlays. The ROM has no name for the file; scrcmd_net.c
// is descriptive. Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/event_gtsnego.h"
#include "field/event_wificlub.h"
#include "field/event_worldtrade.h"
#include "field/field.h"
#include "field/field_script.h"
#include "field/scrcmd_net.h"
#include "gfl/overlay.h"
#include "save/wifi_list.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"

BOOL s013A_GameCommGetStatus(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u16 *result = ScriptReadVar(vm, env);

    *result = func_ov021_021896f4(GSYS_GetGameCommSystem(gsys));
    return TRUE;
}

BOOL func_ov021_021b4c14(VM *vm, FieldScriptEnv *env) {
    u16 *result = ScriptReadVar(vm, env);

    *result = func_02009ce4(func_0201283c(FieldScriptEnv_GetGameData(env)));
    return FALSE;
}

BOOL func_ov021_021b4c34(VM *vm, FieldScriptEnv *env) {
    u16 *result = ScriptReadVar(vm, env);

    *result = func_02009a34(func_0201283c(FieldScriptEnv_GetGameData(env)));
    return FALSE;
}

BOOL s015D_NetConnectWiFiClub(VM *vm, FieldScriptEnv *env) {
    EventWifiClubArgs args;
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    args.field = GSYS_GetField(gsys);
    args.useTransitions = FALSE;
    ScriptWork_CallEvent(work, GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(6), func_ov006_02154c20, &args));
    return TRUE;
}

BOOL s015E_NetConnectGTS(VM *vm, FieldScriptEnv *env) {
    EventWorldTradeArgs args;
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    args.field = GSYS_GetField(gsys);
    args.unused = 0;
    ScriptWork_CallEvent(work, GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(4), func_ov004_021542c0, &args));
    return TRUE;
}

BOOL s0162_NetConnectGTSNegotiation(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);

    ScriptWork_CallEvent(
        work, GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(1), func_ov001_02154694, GSYS_GetField(gsys)));
    return TRUE;
}
