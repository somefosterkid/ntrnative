// Black's code at ov010 0x02164a2c-0x02164fd0. The name is the file name its asserts pass (tools/name_files.py); files
// are on both sides, and pokebw2 has it between scrcmd_ndemo.c and game_beacon_set.c too.

#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "struct_decls.h"
#include "system/dsi.h"
#include "system/game_beacon.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "gfl/std.h"
#include "gfl/net_system.h"
#include "save/player_info.h"
#include "nitro/os.h"
#include "gfl/net_whpipe.h"
#include "gfl/ui.h"
#include "field/player_state.h"
#include "field/intrude_work.h"
#include "field/game_beacon_set.h"
#include "field/field_event.h"
#include "gfl/overlay.h"
#include "field/event_sound.h"
#include "field/field.h"
#include "app/beacon_detail.h"

// pokebw2's BeaconSearchBeacon, 0x64 bytes: the beacon a search sends, with the GameBeacon at 8
typedef struct {
    u8 type;
    u8 connectNum : 4;
    u8 connectMax : 4;
    u8 unk2_0 : 1;
    u8 unk2_1 : 5;
    u8 unk2_6 : 1;
    u8 isDSi : 1;
    u8 unk3_0 : 1;
    u8 unk3_1 : 1;
    u8 unk3_2 : 6;
    u32 trainerId;
    GameBeacon data;
} BeaconSearchBeacon;

// pokebw2's BeaconSearchFound, 9 bytes: a beacon the search found, its connect counts and the MAC address it came from
typedef struct {
    u8 type;
    u8 connectNum;
    u8 connectMax;
    u8 mac[6];
} BeaconSearchFound;

// pokebw2's BeaconSearchWork, 0x84 bytes: a GameEvent's search; the fields the file reads by offset are named so
typedef struct {
    GameData *gameData;
    // 0 once the network ended, 1 once it started, 2 once the scan is set up
    int state;
    u8 unk8[2];
    u16 unkA;
    // While nonzero, beacons of type 2 are not taken
    u32 timer;
    BeaconSearchBeacon beacon;
    BeaconSearchFound found;
    void *unk80;
} BeaconSearchWork;

// pokebw2's EventBeaconDetail (event_beacon_detail.c), 0x1c bytes: the event that opens the beacon detail screen
typedef struct {
    GameSystem *gsys;
    GameData *gameData;
    SaveControl *save;
    BeaconDetailParam param;
    u8 subscreen;
} EventBeaconDetail;

GameEventReturnCode func_ov010_02164a2c(GameEvent *event, u32 *state, void *data);
extern u8 data_ov010_021729bc;
void func_ov010_02164fc4(BeaconSearchWork *work);
void func_ov010_02164c5c(void *work);
void func_ov010_02164fd0(void *req, BOOL exitReqAgain);
static void func_ov010_02164eb4(BeaconSearchBeacon *beacon, GameData *gameData);
void func_ov010_02164c04(void *work);
BeaconSearchBeacon *func_ov010_02164f9c(BeaconSearchBeacon *beacon, BeaconSearchBeacon *other);
BeaconSearchBeacon *func_ov010_02164e04(BeaconSearchWork *work, s32 *index);
BeaconSearchFound *func_ov010_02164d6c(BeaconSearchWork *work, s32 *index);
void func_ov021_021a0f40(FieldSubscreen *, u32);
void *func_ov010_02164e8c(void *work);
int func_ov010_02164f88(void *work);
BOOL func_ov010_02164f8c(u32 a, u32 b);
void func_ov010_02164f90(NetHandle *handle, int a1, void *work);
void func_ov010_02164f98(void *work);

static const GFLNetInitData data_ov010_02171c30 = {
    NULL,
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    func_ov010_02164e8c,
    func_ov010_02164f88,
    func_ov010_02164f8c,
    func_ov010_02164f90,
    NULL,
    func_ov010_02164f98,
    NULL,
    {0},
    NULL,
    NULL,
    0,
    {1, 0, 0, 0, 0x80, 0x13, 0, 0},
    HEAPID_USER | HEAPID_TAIL_BIT,
    0xd | HEAPID_TAIL_BIT,
    HEAPID_DWC_UTILITY | HEAPID_TAIL_BIT,
    0xd | HEAPID_TAIL_BIT,
    0xf0,
    0,
    4,
    0x30,
    4,
    1,
    0,
    0,
    0,
    3,
    {0x2c, 1, 0, 0},
    0,
    0,
};


GameEventReturnCode func_ov010_02164a2c(GameEvent *event, u32 *state, void *data) {
    EventBeaconDetail *work = data;
    GameSystem *gsys = work->gsys;
    Field *field;

    GSYS_GetGameData(gsys);
    field = GSYS_GetField(gsys);
    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, field));
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, func_020297ac(gsys, 0x483, 6, 0));
        (*state)++;
    case 3:
        MI_CpuFill8(&work->param, 0, sizeof(work->param));
        work->param.gsys = work->gsys;
        work->param.gameData = work->gameData;
        work->param.subscreen = work->subscreen;
        GSYS_QueueProc(gsys, OVERLAY_ID(216), &data_ov216_021bdb40, &work->param);
        (*state)++;
        break;
    case 4:
        if (GSYS_GetProcMgrState(gsys) == 0) {
            GameEvent_ChainNext(event, EventBGMPop_CreateEx(gsys, 6, 0x3c));
            (*state)++;
        }
        break;
    case 5:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 6:
        if (work->param.unk09 != 0) {
            func_ov021_021a0f40(Field_GetSubscreen(field), 0);
        }
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0));
        (*state)++;
        break;
    case 7:
        GameEvent_ChainNext(event, EventBGMFadeWait_Create(gsys));
        (*state)++;
        break;
    case 8:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov010_02164b38(GameSystem *gsys, Field *field, u8 subscreen) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov010_02164a2c, sizeof(EventBeaconDetail));
    EventBeaconDetail *work = GameEvent_GetData(event);

    work->gameData = GSYS_GetGameData(gsys);
    work->save = GameData_GetSaveControl(work->gameData);
    work->gsys = gsys;
    work->subscreen = subscreen;
    return event;
}

void *func_ov010_02164b6c(u32 *seq, void *param) {
    GameSystem *gsys = param;
    GameData *gameData = GSYS_GetGameData(gsys);
    GameCommSys *comm = GSYS_GetGameCommSystem(gsys);
    BeaconSearchWork *work = GFL_HeapAllocate(5, sizeof(BeaconSearchWork), 1, (void *)&data_ov010_021729bc, 0x9c);

    work->gameData = gameData;
    work->unkA = 0x1ab;
    if (func_0202647c(comm) == 2 && func_0202648c(comm) == 5) {
        func_ov010_02164fc4(work);
    }
    return work;
}

BOOL func_ov010_02164bc4(u32 *seq, void *param, void *work) {
    BeaconSearchWork *search = work;

    switch (*seq) {
    case 0:
        GFL_HeapDumpOnFailure(0xc);
        func_0203cbac(&data_ov010_02171c30, func_ov010_02164c04, search);
        (*seq)++;
        break;
    case 1:
        if (search->state >= 1) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}



void func_ov010_02164c04(void *work) {
    BeaconSearchWork *search = work;

    search->state = 1;
    search->unk80 = func_02010360(4, 0x8005);
}

BOOL func_ov010_02164c20(u32 *seq, void *param, void *work) {
    BeaconSearchWork *search = work;

    func_020103e4(search->unk80);
    func_0203ceac(func_ov010_02164c5c);
    return TRUE;
}

BOOL func_ov010_02164c38(u32 *seq, void *param, void *work) {
    BeaconSearchWork *search = work;

    if (search->state == 0) {
        GFL_HeapFree(search);
        if (func_0200fb24() != 0) {
            func_0200fba0();
        }
        return TRUE;
    }
    return FALSE;
}

void func_ov010_02164c5c(void *work) {
    BeaconSearchWork *search = work;

    search->state = 0;
}

void func_ov010_02164c64(u32 *seq, void *param, void *work) {
    GameSystem *gsys = param;
    BeaconSearchWork *search = work;
    GameCommSys *comm;
    GameData *gameData;
    BeaconSearchFound *found;
    GameBeaconReq *req;
    u16 zoneId;
    s32 index;

    comm = GSYS_GetGameCommSystem(gsys);
    gameData = GSYS_GetGameData(gsys);
    index = 0;
    if (GameData_CheckEventsPaused(gameData) == 1) {
        return;
    }
    if (func_0200fb24() != 0) {
        func_02026400(comm);
        return;
    }
    if (func_02035390() == 1 && GetIsShouldUICallback() == 1) {
        func_02026400(comm);
        return;
    }
    if (search->timer != 0) {
        search->timer--;
    }
    func_020267fc();
    found = func_ov010_02164d6c(search, &index);
    if (found != NULL) {
        if (found->type == 2) {
            if (PAD_DetectFold() == 0 && func_020264ac(comm) == 0 && GameData_IsForceSeasonSync(gameData) == 0) {
                zoneId = PlayerState_GetZoneID(GameData_GetPlayerState(gameData));
                if (zoneId != search->unkA) {
                    search->unkA = zoneId;
                    if (func_ov010_02158570(zoneId) == 1) {
                        req = GFL_HeapAllocate(0x8005, sizeof(GameBeaconReq), TRUE, (void *)&data_ov010_021729bc,
                                               0x154);
                        req->gsys = gsys;
                        MI_CpuCopy8(found->mac, req->addr, 6);
                        func_0202640c(comm, func_ov010_02164fd0, req);
                        goto end;
                    }
                }
            }
            MI_CpuFill8(&search->found, 0, sizeof(BeaconSearchFound));
            func_ov014_02177bf4(index);
        } else {
            found->type = 0;
        }
    }
end:
    func_02010498();
}


BeaconSearchFound *func_ov010_02164d6c(BeaconSearchWork *work, s32 *index) {
    BeaconSearchFound *found = &work->found;
    BeaconSearchBeacon *beacon;
    u8 *mac;
    s32 i;

    if (work->state == 0) {
        return NULL;
    }
    if (work->state == 1) {
        func_0203d0fc(0);
        work->state = 2;
        return NULL;
    }
    if (found->type == 0) {
        beacon = func_ov010_02164e04(work, index);
        if (beacon != NULL) {
            if (beacon->unk2_0) {
                func_ov014_02177bf4(*index);
            } else if (beacon->unk2_1 == 1) {
                MI_CpuFill8(found, 0, sizeof(BeaconSearchFound));
                found->type = beacon->type;
                found->connectNum = beacon->connectNum;
                found->connectMax = beacon->connectMax;
                mac = func_0203cf3c(*index);
                for (i = 0; i < 6; i++) {
                    found->mac[i] = mac[i];
                }
            } else if (beacon->unk2_1 == 2) {
                func_02026a60(&beacon->data);
                func_ov014_02177bf4(*index);
            }
        }
    }
    if (found->type == 0) {
        return NULL;
    }
    return found;
}


BeaconSearchBeacon *func_ov010_02164e04(BeaconSearchWork *work, s32 *index) {
    s32 best = -1;
    s32 i;
    u8 type;
    BeaconSearchBeacon *beacon;

    for (i = 0; i < 16; i++) {
        type = func_ov014_02178098(i);
        if ((u32)type - 2 > 1) {
            continue;
        }
        if (type == 2 && work->timer != 0) {
            continue;
        }
        beacon = func_0203cf10(i);
        if (beacon == NULL) {
            continue;
        }
        if (beacon->connectNum < beacon->connectMax) {
            if (best == -1) {
                best = i;
            } else if (func_ov010_02164f9c(beacon, func_0203cf10(best)) == beacon) {
                best = i;
            }
        } else {
            func_ov014_02177bf4(i);
        }
    }
    if (best != -1) {
        *index = best;
        return func_0203cf10(best);
    }
    return NULL;
}


void *func_ov010_02164e8c(void *work) {
    BeaconSearchWork *search = work;

    func_ov010_02164eb4(&search->beacon, search->gameData);
    func_020267e4(&search->beacon.data);
    search->beacon.unk2_1 = 2;
    return &search->beacon;
}


static void func_ov010_02164eb4(BeaconSearchBeacon *beacon, GameData *gameData) {
    MI_CpuFill8(beacon, 0, sizeof(BeaconSearchBeacon));
    beacon->type = 3;
    beacon->connectNum = func_0203d1b4();
    beacon->connectMax = 3;
    beacon->unk2_0 = func_02039414();
    beacon->unk2_1 = 0;
    beacon->trainerId = getIDAsUInt(GetGameDataPlayerInfo(gameData));
    beacon->isDSi = OS_IsRunOnTwl();
    beacon->unk2_6 = 0;
    beacon->unk3_1 = PAD_DetectFold();
    if (beacon->isDSi == 1) {
        beacon->unk2_6 = OS_GetParentalControlInfoPtr()->flags.isSetParentalControl &&
                         OS_GetParentalControlInfoPtr()->flags.photoExchange;
    }
}


int func_ov010_02164f88(void *work) {
    return sizeof(BeaconSearchBeacon);
}

BOOL func_ov010_02164f8c(u32 a, u32 b) {
    return TRUE;
}

void func_ov010_02164f90(NetHandle *handle, int a1, void *work) {
    BeaconSearchWork *search = work;

    search->unk8[0] = 1;
}

void func_ov010_02164f98(void *work) {
}

BeaconSearchBeacon *func_ov010_02164f9c(BeaconSearchBeacon *beacon, BeaconSearchBeacon *other) {
    if (beacon->unk2_0) {
        return other;
    }
    if (beacon->connectNum < beacon->connectMax && beacon->connectNum >= other->connectNum) {
        return beacon;
    }
    return other;
}


void func_ov010_02164fc4(BeaconSearchWork *work) {
    work->timer = 1800;
}

