// Black's code at ov021 0x021d1b8c-0x021d1c24. Placed by tools/gap_files.py from 2 pokebw2.

#include "types.h"
#include "field/field.h"
#include "field/field_actor_animation.h"
#include "field/field_script.h"
#include "field/subscreen.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "system/game_system.h"

struct GameEvent {
    GameEvent *parent;
    GameEventCallback callback;
    u32 state;
    void *data;
    GameSystem *gsys;
};

u32 func_ov021_021d1b8c(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    ScriptFieldWork *fieldWork;
    FieldSubscreen *subscreen;
    GameSystem *gsys;
    GameEvent *event;
    EventFieldSubscreenChange *data;

    work = FieldScriptEnv_GetScriptWork(env);
    fieldWork = ScriptWork_GetFieldWork(work);
    subscreen = Field_GetSubscreen(fieldWork->field);
    gsys = FieldScriptEnv_GetGameSystem(env);
    event = GameEvent_Create(gsys, NULL, EventFieldSubscreenChange_Callback, sizeof(EventFieldSubscreenChange));
    data = GameEvent_GetData(event);
    data->subscreen = subscreen;
    data->mode = 0;
    ScriptWork_CallEvent(work, event);
    FieldScriptSubEvent_Unregister(8);
    return 1;
}

BOOL func_ov021_021d1bd8(FinishScriptSubEventsWork *work, u32 *state) {
    ScriptFieldWork *fieldWork;
    FieldSubscreen *subscreen;
    u32 stateVal;

    fieldWork = ScriptWork_GetFieldWork(work->scriptWork);
    subscreen = Field_GetSubscreen(fieldWork->field);
    stateVal = *state;
    switch (stateVal) {
        return 0;
    case 0:
        if (FieldSubscreen_IsReady(subscreen) != 0) {
            FieldSubscreen_ReqChange(subscreen, 0U);
            FieldScriptSubEvent_Unregister(8);
            (*state)++;
        }
        break;
    case 1:
        if (FieldSubscreen_IsReady(subscreen) != 0) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}
