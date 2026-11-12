// Black's code at ov010 0x0216cc30-0x0216cc58, between event_bsubway.c and fldmmdl.c (pokebw2 has nothing there).
// No name is known: the module and address. It was named scrcmd_sp_poke.c after the two of its four functions it
// shares with pokebw2's file, which is Black's code at 0x0216f8b8 (between trcard_sys.c and waza_oshie.c, as there);
// this one sets gimmick 0x34's flag, scrcmd_sp_poke.c's sets the Nuvema Town gimmick's (0x36).
#include "types.h"
#include "field/field_map.h"
#include "field/field_script.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"

static void func_ov010_0216cc40(GameSystem *gsys);

BOOL func_ov010_0216cc30(VM *vm, FieldScriptEnv *env) {
    func_ov010_0216cc40(FieldScriptEnv_GetGameSystem(env));
    return FALSE;
}

static void func_ov010_0216cc40(GameSystem *gsys) {
    u32 *state = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(gsys)), 0x34);

    *state = TRUE;
}
