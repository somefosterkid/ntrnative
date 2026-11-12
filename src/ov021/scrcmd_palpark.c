// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov053/scrcmd_palpark.c, the script plugin of the
// Poke Transfer Lab. The names are Black's.
#include "types.h"
#include "app/mb_parent.h"
#include "app/startmenu.h"
#include "field/field_event.h"
#include "field/field_script.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"

enum {
    PAL_PARK_INFO_RESULT,
    PAL_PARK_INFO_HIGH_SCORE,
};

// Runs Poke Transfer, whose DS Download Play parent sends the game to the other system
u32 func_ov021_021cc570(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    Field *field = GSYS_GetField(gsys);
    MBParentParam *param = GFL_HeapAllocate(4, sizeof(MBParentParam), FALSE, "scrcmd_palpark.c", 68);

    param->startMenu = FALSE;
    param->gameData = gameData;
    ScriptWork_CallEvent(work, func_02014840(gsys, field, OVERLAY_ID(107), (void *)&MB_PARENT_PROC_FUNCTIONS, param, NULL, param));
    return TRUE;
}

// Sets a variable to the last Poke Transfer's result or the high score
u32 func_ov021_021cc5d8(VM *vm, FieldScriptEnv *env) {
    u8 info = VM_Read8(vm);
    u16 *var = ScriptReadVar(vm, env);
    TrainerCardSave *card = getTrainerGameInfoAddress(GameData_GetSaveControl(FieldScriptEnv_GetGameData(env)));

    switch (info) {
    case PAL_PARK_INFO_RESULT:
        *var = func_0200c1a4(card);
        break;
    case PAL_PARK_INFO_HIGH_SCORE:
        *var = func_0200c184(card);
        break;
    }
    return FALSE;
}
