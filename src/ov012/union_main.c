// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov028/union_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Union Room's field side (the file's name is from its allocations' strings): it boots the communication and runs
// each frame from the field. The net commands are union_receive.c's and the commands that run as field events are
// union_subproc.c's: the ROM lays out each of the three with an assert file name ("") of its own, which MWCC shares
// across one file
#include "field/union_main.h"
#include "types.h"
#include "app/p_status.h"
#include "app/pokelist.h"
#include "app/pokemon_trade.h"
#include "constants/zones.h"
#include "demo/shinka_demo.h"
#include "field/colosseum.h"
#include "field/event_colosseum_battle.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/field_controller.h"
#include "field/field_event.h"
#include "field/field_sound.h"
#include "field/union_actor.h"
#include "field/union_app.h"
#include "field/union_comm.h"
#include "field/union_msg.h"
#include "field/union_oneself.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_sync.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "field/comm_player.h"

// pokebw2's UnionMain_Boot
void func_ov012_02173744(GameSystem *gsys) {
    GameCommSys *comm = GSYS_GetGameCommSystem(gsys);
    GameData *gameData = GSYS_GetGameData(gsys);
    UnionBootParam *param;

    GFL_ASSERT(GameCommSys_BootCheck(comm) == GAME_COMM_NO_NULL);
    param = GFL_HeapAllocate(5, sizeof(UnionBootParam), TRUE, "union_main.c", 55);
    param->playerInfo = GetGameDataPlayerInfo(gameData);
    param->comm = comm;
    param->gameData = gameData;
    param->gsys = gsys;
    func_020263d8(comm, GAME_COMM_NO_UNION, param);
    func_ov010_021653c4();
}

// pokebw2's UnionMain_Update
void func_ov012_021737a4(GameCommSys *comm, Field *field) {
    UnionSystem *unisys;
    u8 activity;

    if (GameCommSys_BootCheck(comm) == GAME_COMM_NO_UNION) {
        unisys = func_02026474(comm);
        GFL_ASSERT(unisys != NULL);
        if (func_0201212c(Field_GetGameSystem(field)) == TRUE) {
            return;
        }
        if (GSYS_CheckNowEvent(unisys->param->gsys) == TRUE) {
            return;
        }
        if (func_ov012_02173840(unisys) == TRUE) {
            activity = unisys->situ.activity;
            if (activity >= 4 && activity <= 0x17) {
                if (func_ov012_02174268(unisys)) {
                    return;
                }
                func_ov019_0217b840(unisys, field);
                return;
            }
            switch (activity) {
            case 0:
            case 1:
                if (func_ov012_02174268(unisys)) {
                    return;
                }
                func_ov019_0217b75c(unisys);
                func_ov019_0217b840(unisys, field);
                func_ov019_0217b264(unisys, unisys->param->gameData, field);
                break;
            }
        }
    }
}


// pokebw2's UnionMain_IsFieldReady
BOOL func_ov012_02173840(UnionSystem *unisys) {
    if (unisys->fieldActive == TRUE && GSYS_CheckField(unisys->param->gsys) == TRUE &&
        Field_CheckMapLoadFinished(GSYS_GetField(unisys->param->gsys)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionMain_OnFieldIn
void func_ov012_02173874(void *param, void *work, Field *field) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    unisys->fieldActive = TRUE;
    if (unisys->updateDisabled == TRUE) {
        FieldmapCtrlGrid_SetUpdateDisable(field, TRUE);
    }
    if (GetZoneIsUnionRoom(Field_GetPlayerStateZoneID(field)) == TRUE) {
        func_ov019_0217b440(unisys, GSYS_GetGameData(unisys->param->gsys), field);
    }
    if (colosseum != NULL && colosseum->unkC8_0 == TRUE) {
        func_ov010_021676e4(colosseum->commPlayer);
    }
}

// pokebw2's UnionMain_OnFieldOut
void func_ov012_021738d4(void *param, void *work, Field *field) {
    UnionSystem *unisys = work;
    UnionColosseum *colosseum = unisys->colosseum_sys;

    if (colosseum != NULL) {
        if (colosseum->unkC9 == 1) {
            func_ov012_0217505c(colosseum, unisys->param->gsys);
            unisys->colosseum_sys = NULL;
        } else if (colosseum->unkC8_0 == TRUE) {
            func_ov010_02167688(colosseum->commPlayer);
        }
    }
    unisys->fieldActive = FALSE;
}

u32 func_ov012_0217391c(UnionSystem *unisys) {
    return unisys->unk3568;
}

u32 func_ov012_02173928(UnionSystem *unisys) {
    return unisys->unk356C;
}

void func_ov012_02173934(UnionSystem *unisys, u8 value) {
    unisys->situ.unk02 = value;
}

u8 func_ov012_02173940(UnionSystem *unisys) {
    return unisys->situ.unk02;
}

// pokebw2's UnionMain_IsIdle
BOOL func_ov012_0217394c(GameSystem *gsys) {
    UnionSystem *unisys = func_02026474(GSYS_GetGameCommSystem(gsys));

    if (unisys == NULL) {
        return FALSE;
    }
    if ((unisys->situ.step == 0 || unisys->situ.step == 0x2a) && unisys->updateDisabled == 0) {
        return TRUE;
    }
    return FALSE;
}
