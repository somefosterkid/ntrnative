// Black's code at ov021 0x021d2468-0x021d2524. No name is known: the module and address.


#include "types.h"
#include "field/field_script.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "field/city_state.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_player.h"
#include "field/field_player_core.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/key.h"
#include "gfl/random.h"
#include "pml/poke_graphic.h"
#include "save/adventure.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "struct_decls.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

BOOL func_ov021_021d2468(VM *vm, FieldScriptEnv *env) {
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    u16 mode = ScriptReadAny(vm, env);
    u16 *outA = ScriptReadVar(vm, env);
    u16 *outB = ScriptReadVar(vm, env);
    u16 *outC = ScriptReadVar(vm, env);
    u16 table[31] = {
#include "asset/ov021_021d2468_table.inc"
};
    AdventureSave *adventure;
    u32 month;
    u32 day;
    u32 birthMonth;
    u32 rnd;

    adventure = getSaveAdventureDataBlk(GameData_GetSaveControl(gameData));
    month = GameData_GetMonth(gameData);
    day = GameData_GetDay(gameData);
    birthMonth = getOSBirthdayMonth(adventure);
    if (day == getOSBirthdayDay(adventure) && month == birthMonth) {
        *outA = 0x34;
        *outB = 0x53;
        *outC = table[30];
    } else {
        rnd = GFL_RandomLC(mode ? 0x1e : 0x16);
        *outA = rnd + 0x16;
        *outB = rnd + 0x35;
        *outC = table[rnd];
    }
    return FALSE;
}
