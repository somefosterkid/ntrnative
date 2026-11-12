// Black's code at ov021 0x021c812c-0x021c8268. No name is known: the module and address.

#include "types.h"
#include "field/encounter_effect.h"
#include "field/field_event.h"
#include "field/field_script.h"
#include "struct_decls.h"
#include "system/game_system.h"
#include "system/vm.h"

GameEvent *EventFieldEffect_CreateAlderFly(GameSystem *param_1, void *param_2, u32 param_3);

u32 func_ov021_021c812c(VM *param_1, FieldScriptEnv *param_2) {
    u16 effectId;
    GameSystem *gsys;
    Field *field;
    void *g3dCi;
    ScriptWork *work;
    GameEvent *event;

    gsys = FieldScriptEnv_GetGameSystem(param_2);
    field = GSYS_GetField(gsys);
    g3dCi = Field_Get3DCi(field);
    effectId = VM_Read16(param_1);
    work = FieldScriptEnv_GetScriptWork(param_2);
    event = EventFieldEffect_Create(gsys, g3dCi, (u8)effectId);
    if (event != (GameEvent *)0x0) {
        ScriptWork_CallEvent(work, event);
    }
    return 1;
}

u32 func_ov021_021c8170(VM *param_1, FieldScriptEnv *param_2) {
    u16 partySlot;
    GameSystem *gsys;
    Field *field;
    void *g3DCi;
    ScriptWork *work;
    GameEvent *event;

    gsys = FieldScriptEnv_GetGameSystem(param_2);
    field = GSYS_GetField(gsys);
    g3DCi = Field_Get3DCi(field);
    partySlot = ScriptReadAny(param_1, param_2);
    work = FieldScriptEnv_GetScriptWork(param_2);
    event = EventFieldEffect_CreatePokeSprite(gsys, g3DCi, (u8)partySlot);
    if (event != (GameEvent *)0x0) {
        ScriptWork_CallEvent(work, event);
    }
    return 1;
}

u32 func_ov021_021c81b4(VM *param_1, FieldScriptEnv *param_2) {
    u16 result;
    GameSystem *gsys;
    Field *field;
    void *ptr;
    ScriptWork *work;
    GameEvent *event;

    gsys = FieldScriptEnv_GetGameSystem(param_2);
    field = GSYS_GetField(gsys);
    ptr = Field_Get3DCi(field);
    result = ScriptReadAny(param_1, param_2);
    work = FieldScriptEnv_GetScriptWork(param_2);
    event = EventFieldEffect_CreateAlderFly(gsys, ptr, (u32)result);
    if (event != (GameEvent *)0x0) {
        ScriptWork_CallEvent(work, event);
    }
    return 1;
}

static const u16 data_ov021_021daae0[17][2] = {
#include "asset/ov021_021c812c_data_ov021_021daae0.inc"
};
u32 func_ov021_021c81f8(VM *param_1, FieldScriptEnv *param_2) {
    GameSystem *gsys;
    Field *field;
    void *g3dCi;
    u16 scriptId;
    BOOL found;
    u16 effectId;
    s32 i;
    ScriptWork *work;
    GameEvent *event;

    gsys = FieldScriptEnv_GetGameSystem(param_2);
    field = GSYS_GetField(gsys);
    g3dCi = Field_Get3DCi(field);
    scriptId = ScriptReadAny(param_1, param_2);
    found = FALSE;
    for (i = 0; i < 17; i++) {
        if (scriptId == data_ov021_021daae0[i][0]) {
            found = TRUE;
            effectId = data_ov021_021daae0[i][1];
            break;
        }
    }
    if (found) {
        work = FieldScriptEnv_GetScriptWork(param_2);
        event = EventFieldEffect_Create(gsys, g3dCi, (u8)effectId);
        if (event != (GameEvent *)0x0) {
            ScriptWork_CallEvent(work, event);
        }
    }
    return 1;
}

