// Black's code at ov010 0x021643ac-0x02164480. The name is pokebw2's file between field_actor_tool.c and
// itemuse_event.c (9 functions there, 8 here).

#include "types.h"
#include "field/encounter.h"
#include "field/field.h"
#include "field/field_status.h"
#include "field/hidden_item.h"
#include "field/move_pokemon.h"
#include "field/zone.h"
#include "field/zone_change.h"
#include "save/event_work.h"
#include "save/player_savedata.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "system/game_data.h"

void func_ov010_021643ac(GameData *param_1, Field *param_2) {
    u16 zoneId;
    SaveControl *save;
    EncountSave *save_00;
    EventWork *eventWork;

    save = GameData_GetSaveControl(param_1);
    save_00 = SaveControl_GetEncountSave(save);
    zoneId = Field_GetPlayerStateZoneID(param_2);
    func_ov010_0215e428(save_00, zoneId);
    func_ov010_0215e3a0(param_1);
    eventWork = GameData_GetEventWork(param_1);
    EventWork_FlagReset(eventWork, 0x964);
    func_ov010_0215e33c(param_2);
}

void func_ov010_021643e8(GameData *param_1, u16 param_2) {
    FieldStatus *status;
    SaveControl *pSVar2;
    EncountSave *save;
    EventWork *eventWork;
    BOOL BVar3;
    PlayerSave *playerSave;

    status = GameData_GetFieldStatus(param_1);
    if ((GetZoneFlashFlags(param_2) & 3) == 0) {
        FieldStatus_SetFlashUsed(status, 0);
    }
    pSVar2 = GameData_GetSaveControl(param_1);
    save = SaveControl_GetEncountSave(pSVar2);
    func_ov010_0215e428(save, param_2);
    func_ov010_0215e364(param_1);
    eventWork = GameData_GetEventWork(param_1);
    EventWork_FlagReset(eventWork, 0x964);
    TryClearRepeatableHiddenItemFlags(eventWork);
    BVar3 = IsZoneAbyssalRuinsInside(param_2);
    if (BVar3 == 0) {
        pSVar2 = GameData_GetSaveControl(param_1);
        playerSave = SaveControl_GetPlayerSave(pSVar2);
        PlayerSave_SetAbyssalRuinsStepCounter(playerSave, 0);
    }
}

void func_ov010_02164454(GameData *gameData) {
    func_ov010_0215e34c(gameData);
}

void func_ov010_0216445c(GameData *gameData) {
    func_ov010_0215e34c(gameData);
}

void func_ov010_02164464(GameData *gameData) {
    func_ov010_0215e34c(gameData);
}

void func_ov010_0216446c(GameData *gameData) {
    func_ov010_0215e34c(gameData);
}

void func_ov010_02164474(GameData *gameData, u16 zoneId) {
    func_ov010_0215e364(gameData);
}






void func_ov010_0216447c(void) {
}
