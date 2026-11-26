// "Save first" for Platinum and HeartGold / SoulSilver (port/src/save_first.c, docs/save-first.md): the start menu's own save,
// done from the field's frame hook (PortRoute4_Frame, at the end of each field frame: pokeplatinum-route.patch,
// pokeheartgold-route.patch) when the game's own menu would offer Save. Compiled into each game's module with its headers
// (port/meson.build's main_files), Platinum's half first, HeartGold's under HEARTGOLD.
//
// The save is the one the menu's Save runs, without its windows: Platinum's start menu runs common script 5, whose
// TrySaveGame is FieldSystem_Save (overlay005/save_info_window.c: the map objects and the player's place, then SaveData_Save);
// HeartGold's runs the touch save app, whose write is Field_SaveGameNormal (the same two steps). Both write at once (the
// game's two slots and footers, the port's save file), so the hook's frame ends with the save on disk.
//
// When: the field map runs, unpaused, with no field task (the start menu, a script, a warp, a battle's start are tasks: the
// field's input is not read then), and the menu would have Save: the field's input code picks the menu by the map's load type
// (field_system.c), and StartMenu_Open / StartMenu_Init hide Save in the Safari Zone, the Pal Park, the Battle Tower's
// salon or multi partner room, HeartGold's Bug-Catching Contest, the Union Room and the colosseums; the Mystery Zone opens no
// menu; the save scripts refuse a game that would overwrite another game's save. The save itself waits for the player to
// stand (the move state the menu key needs, field_control.c FieldInput_Update).
#include "port_save_first.h"

#if !defined(HEARTGOLD)   // Platinum
#include "field/field_system.h"
#include "constants/field/map_load.h"
#include "constants/fieldmap.h"
#include "constants/player_avatar.h"
#include "field_task.h"
#include "location.h"
#include "overlay005/save_info_window.h"
#include "player_avatar.h"
#include "savedata.h"
#include "start_menu.h"
#include "system_flags.h"
#include "unk_0206B9D8.h"
#include "vars_flags.h"

static int PortSave4_Place(FieldSystem *fs) {
    VarsFlags *flags = SaveData_GetVarsFlags(fs->saveData);
    int loadType = fs->location->mapHeaderID == MAP_HEADER_BATTLE_TOWER ? MAP_LOAD_TYPE_OVERWORLD : fs->mapLoadType;

    if (loadType == MAP_LOAD_TYPE_OVERWORLD) {
        if (!FieldSystem_IsInValidLocation(fs)) {   // the Mystery Zone
            return 0;
        }
    } else if (loadType != MAP_LOAD_TYPE_BATTLE_TOWER) {   // the Underground's menu, the Union Room's and the colosseums'
        return 0;
    }
    return !SystemFlag_CheckSafariGameActive(flags) && !SystemFlag_CheckInPalPark(flags) && !FieldSystem_IsInBattleTowerSalon(fs) &&
           !SaveData_OverwriteCheck(fs->saveData);
}

void PortSave4_Frame(FieldSystem *fs) {
    int up = fs != NULL && fs->runningFieldMap && fs->processManager != NULL && fs->processManager->parent != NULL &&
             fs->playerAvatar != NULL && fs->location != NULL && fs->saveData != NULL;
    int free = up && !fs->processManager->pause && !FieldSystem_IsRunningTask(fs);
    int can = free && PortSave4_Place(fs);
    int move;

    PortSaveFirst_Can(can);
    if (!can) {
        return;
    }
    move = PlayerAvatar_GetPlayerMoveState(fs->playerAvatar);
    if ((move == PLAYER_MOVE_STATE_NONE || move == PLAYER_MOVE_STATE_END) && PortSaveFirst_Take()) {
        PortSaveFirst_Done(FieldSystem_Save(fs), "the game's save failed");
    }
}

#else   // HEARTGOLD: HeartGold and SoulSilver
#include "field_system.h"
#include "constants/field/map_load.h"
#include "constants/maps.h"
#include "constants/player_avatar.h"
#include "player_avatar.h"
#include "save.h"
#include "save_vars_flags.h"
#include "start_menu.h"
#include "sys_flags.h"
#include "task.h"
#include "unk_02066EDC.h"

BOOL Field_SaveGameNormal(FieldSystem *fieldSystem);   // overlay_01_021F3F50_021F43E8_021F43E8.c (no header)

static int PortSave4_Place(FieldSystem *fs) {
    SaveVarsFlags *flags = Save_VarsFlags_Get(fs->saveData);
    int loadType = fs->location->mapId == MAP_BATTLE_TOWER ? MAP_LOAD_TYPE_OVERWORLD : fs->mapLoadType;

    if (loadType == MAP_LOAD_TYPE_UNION || loadType == MAP_LOAD_TYPE_COLOSSEUM) {
        return 0;
    }
    if (loadType != MAP_LOAD_TYPE_BATTLE_TOWER && !FieldSystem_MapIsNotMysteryZone(fs)) {
        return 0;
    }
    return !Save_VarsFlags_CheckSafariSysFlag(flags) && !Save_VarsFlags_CheckBugContestFlag(flags) &&
           !Save_VarsFlags_CheckPalParkSysFlag(flags) && !FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(fs) &&
           !Save_FileDoesNotBelongToPlayer(fs->saveData);
}

void PortSave4_Frame(FieldSystem *fs) {
    int up = fs != NULL && fs->runningFieldMap && fs->processManager != NULL && fs->processManager->parent != NULL &&
             fs->playerAvatar != NULL && fs->location != NULL && fs->saveData != NULL;
    int free = up && !fs->processManager->isPaused && !FieldSystem_TaskIsRunning(fs);
    int can = free && PortSave4_Place(fs);
    u32 move;

    PortSaveFirst_Can(can);
    if (!can) {
        return;
    }
    move = PlayerAvatar_GetPlayerMoveState(fs->playerAvatar);
    if ((move == PLAYER_MOVE_STATE_NONE || move == PLAYER_MOVE_STATE_END) && PortSaveFirst_Take()) {
        PortSaveFirst_Done(Field_SaveGameNormal(fs), "the game's save failed");
    }
}
#endif
