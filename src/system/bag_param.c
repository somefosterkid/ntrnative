// Black's code at main 0x0202e43c-0x0202e4ec. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "app/bag.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/field_status.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "nitro/mi.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/config.h"
#include "save/event_work.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/save_control.h"
#include "save/trainer_data_savedata.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/shortcut_util.h"

struct CommPlayerSupport {
    u32 type;
    u32 usedType;
    u32 lastType;
    PlayerInfo supporter;
    PlayerInfo usedSupporter;
};
struct BoxSaveAccessor {
    SaveControl *save;
};
struct GameData {
    SaveControl *saveControl;
    PlayerState playerStates[5];
    EventData *eventData;
    PlayerSave *playerSave;
    ZoneSpawnInfo *nowSpawn;
    ZoneSpawnInfo *outboundWarpSpawn;
    ZoneSpawnInfo *nextSpawn;
    ZoneSpawnInfo *escapeRopeSpawn;
    ZoneSpawnInfo entralinkParentSpawn;
    void *bagCursor;
    BagSave *bag;
    PokeParty *party;
    Config *config;
    BoxSaveAccessor *boxes;
    WifiList *wifiList;
    BGMInfo *bgmInfo;
    MMSys *mmSys;
    EventWork *eventWork;
    MapMatrix *mapMatrix;
    FieldSound *fieldSound;
    FieldStatus *fieldStatus;
    u32 lastBtlResult;
    u32 unk1C0;
    u8 season;
    u8 weather;
    u8 unk1C6;
    u8 unk1C7;
    u8 counter;
    u8 counterEnabled;
    u8 lastSubscreen;
    u8 netId;
    u8 forceSeasonSync;
    u8 savedCGearPowerRequest;
    u8 saving;
    u8 is30FPSMode;
    u8 seasons[4];
    PassPowerLevel levels[4];
    CityState cityStates[2];
    EncountState *encountState;
    ShortcutMenuPos shortcutMenuPos;
    GimmickState gimmickState;
    PleasureBoat *pleasureBoat;
    CommPlayerSupport *commPlayerSupport;
    u32 unk688;
    void *unk68C;
    Calendar *calendar;
    UnityTowerFloor unityTowerFloor;
    TrialHouseWork *trialHouseWork;
    u32 unk6A8;
    PlayTime *playTime;
};
struct BGMInfo {
    u8 count;
    u16 *bgms;
    u8 *subsystems;
};

static const u16 data_020a02a8[7] = { 0x01c2, 0x11bf, 0x05ba, 0x09d1, 0x0db5, 0x15d7, 0x19d2 };

void func_0202e43c(void *param_1) {
    MI_CpuFill8(param_1, 0, 4);
}

u32 func_0202e448(u16 item) {
    u8 *ptr;
    u16 val;
    u32 i;

    i = 0;
    loop_1:
    val = *(u16 *)((u8 *)(&data_020a02a8) + i * 2);

    ptr = (u8 *)(&data_020a02a8);
    if (item == ((u32)(*(u16 *)((u8 *)(&data_020a02a8) + i * 2) << 0x16) >> 0x16)) {
        return (u32)(*(u16 *)(ptr + i * 2) << 0x10) >> 0x1A;
    }
    i += 1;
    if (i >= 7U) {
        return 0x19U;
    }
    goto loop_1;
}

BagProcessData *func_0202e474(GameData *gameData, const PlayerActionPerms *perms, u32 mode, HeapID heapId) {
    SaveControl *save;
    BagProcessData *pBVar1;
    TrainerDataSave *pTVar2;
    PlayerInfo *pPVar3;
    BagSave *pBVar5;
    PlayerState *playerState;

    save = GameData_GetSaveControl(gameData);
    pBVar1 = GFL_HeapAllocate(heapId, sizeof(BagProcessData), 1, "bag_param.c", 0x37);
    pBVar1->gameData = gameData;
    pTVar2 = getTrainerDataBlkAddress(save);
    pBVar1->trainerData = pTVar2;
    pPVar3 = GetGameDataPlayerInfo(gameData);
    pBVar1->playerInfo = pPVar3;
    pBVar1->cursor = func_02012924(gameData);
    pBVar5 = GameData_GetBag(gameData);
    pBVar1->bag = pBVar5;
    if (perms != NULL) {
        MI_CpuCopy8(perms, &pBVar1->perms, 0x18);
    }
    pBVar1->mode = mode;
    playerState = GameData_GetPlayerState(gameData);
    pBVar1->isCycling = (u32)(FieldPlayerState_GetExState(playerState) == 1);
    return pBVar1;
}
