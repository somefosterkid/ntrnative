// Black's code at ov021 0x021cc544-0x021cc570. No name is known: the module and address.

#include "types.h"
#include "field/event_league_lift.h"
#include "field/field_script.h"
#include "struct_decls.h"
#include "system/game_system.h"

u32 func_ov021_021cc544(u32 param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *gsys;
    Field *field;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    field = GSYS_GetField(gsys);
    event = EventLeagueLiftWarp_Create(gsys, field);
    ScriptWork_CallEvent(work, event);
    return 1;
}
