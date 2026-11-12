// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/script_sub_event.c, ported by tools/port_file.py.
#include "types.h"
#include "field/field_script.h"
#include "field/scrcmd_camera.h"
#include "field/scrcmd_msg.h"
#include "field/scrcmd_sound.h"
#include "gfl/std.h"
#include "system/game_event.h"
#include "nitro/mi.h"

u32 g_ActiveFieldScriptSubEvents;

void FieldScriptSubEvent_ResetAll(void) {
    MIi_CpuClear32(0, &g_ActiveFieldScriptSubEvents, sizeof(g_ActiveFieldScriptSubEvents));
}

void FieldScriptSubEvent_Register(int id) {
    if (id < 14 && !FieldScriptSubEvent_IsRegisteredCore(id)) {
        g_ActiveFieldScriptSubEvents |= 1 << id;
    }
}

void FieldScriptSubEvent_Unregister(int id) {
    if (id < 14 && FieldScriptSubEvent_IsRegisteredCore(id)) {
        g_ActiveFieldScriptSubEvents &= -1 ^ (1 << id);
    }
}

BOOL FieldScriptSubEvent_IsRegistered(int id) {
    return FieldScriptSubEvent_IsRegisteredCore(id);
}

BOOL FieldScriptSubEvent_IsRegisteredCore(int id) {
    if (id >= 14) {
        return FALSE;
    }
    if (((s32)g_ActiveFieldScriptSubEvents >> id) & 1) {
        return TRUE;
    }
    return FALSE;
}

GameEventReturnCode EventFinishScriptSubEvents_Callback(GameEvent *event, u32 *state, void *data) {
    FinishScriptSubEventsWork *work = data;
    FieldScriptSubEventFinishFunc finish;
    u32 *workState = &work->state;
    s32 *index = &work->index;

    do {
        finish = FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS[work->index];
        if (finish == NULL) {
            return GAMEEVENT_DONE;
        }
        if (FieldScriptSubEvent_IsRegisteredCore(work->index)) {
            if (finish(work, workState)) {
                (*index)++;
                work->state = 0;
                continue;
            }
            return GAMEEVENT_CONTINUE;
        }
        (*index)++;
        work->state = 0;
    } while (work->index < work->count);
    return GAMEEVENT_DONE;
}

GameEvent *EventFinishScriptSubEvents_Create(FieldScriptEnv *env) {
    ScriptWork *scriptWork = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameEvent *event = GameEvent_Create(gsys, NULL, EventFinishScriptSubEvents_Callback, sizeof(FinishScriptSubEventsWork));
    FinishScriptSubEventsWork *work = GameEvent_GetData(event);

    work->gsys = gsys;
    work->env = env;
    work->scriptWork = scriptWork;
    work->index = 0;
    work->state = 0;
    work->count = NELEMS(FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS);
    return event;
}
