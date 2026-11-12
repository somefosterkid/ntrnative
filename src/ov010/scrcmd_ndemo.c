// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/scrcmd_ndemo.c, which is the same code in
// Black 2/White 2's overlay 12, with the face-up in overlay 82 where Black 2 has it in overlay 155.
#include "types.h"
#include "field/field_script.h"
#include "field/fld_faceup.h"
#include "gfl/overlay.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"

// The scenes with N, which overlay 82 plays (a descriptive name). Command names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), which carry Black 2's command numbers; Black's are noted.

#define SCRIPT_SUB_EVENT_NDEMO 7

static GameEventReturnCode EventNDemoEnd_Callback(GameEvent *event, u32 *state, void *data);

// Command 0x1D3 in Black
BOOL s01C9_NDemoStart(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    u8 a0 = VM_Read16(vm);
    u8 a1 = VM_Read16(vm);
    u16 a2 = VM_Read16(vm);
    GameEvent *event;

    FieldScriptSubEvent_Register(SCRIPT_SUB_EVENT_NDEMO);
    GFL_OvlLoad(OVERLAY_ID(82));
    event = func_ov082_021f5260(a0, a1, a2, gsys, env);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

// Command 0x1D5 in Black
BOOL s01CB_NDemoReadyTalkMotion(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    func_ov082_021f558c(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)));
    return FALSE;
}

// Command 0x1D4 in Black
BOOL s01CA_NDemoEnd(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameEvent *event = GameEvent_Create(FieldScriptEnv_GetGameSystem(env), NULL, EventNDemoEnd_Callback, 0);

    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL FieldScriptSubEventFinish_NDemo(FinishScriptSubEventsWork *work, u32 *state) {
    func_ov082_021f5578(GSYS_GetField(work->gsys));
    FieldScriptSubEvent_Unregister(SCRIPT_SUB_EVENT_NDEMO);
    GFL_OvlUnload(OVERLAY_ID(82));
    return TRUE;
}

static GameEventReturnCode EventNDemoEnd_Callback(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, func_ov082_021f5550(gsys));
        (*state)++;
        break;
    case 1:
        FieldScriptSubEvent_Unregister(SCRIPT_SUB_EVENT_NDEMO);
        GFL_OvlUnload(OVERLAY_ID(82));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}
