// Black's code at ov010 0x0216f8b8-0x0216f918: pokebw2's ov012 scrcmd_sp_poke.c, between trcard_sys.c and
// waza_oshie.c as there, its four functions (0x60 bytes in both).
// Script commands that drive overlay 65's gimmick (gimmick_nuvema.c). The name is pokebw2's guess
#include "types.h"
#include "field/field.h"
#include "field/field_map.h"
#include "field/field_script.h"
#include "field/gimmick_nuvema.h"
#include "field/scrcmd_sp_poke.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"

static void func_ov010_0216f900(GameSystem *gsys);

BOOL func_ov010_0216f8b8(VM *vm, FieldScriptEnv *env) {
    func_ov010_0216f900(FieldScriptEnv_GetGameSystem(env));
    return FALSE;
}

// s023C_NuvemaGimmickPlay
BOOL func_ov010_0216f8c8(VM *vm, FieldScriptEnv *env) {
    func_ov065_021f385c(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)));
    return FALSE;
}

// s023D_NuvemaGimmickWaitAnim
BOOL func_ov010_0216f8dc(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);

    ScriptWork_CallEvent(work, func_ov065_021f38bc(FieldScriptEnv_GetGameSystem(env)));
    return TRUE;
}

static void func_ov010_0216f900(GameSystem *gsys) {
    u32 *state = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(gsys)), GIMMICK_NUVEMA);

    *state = TRUE;
}
