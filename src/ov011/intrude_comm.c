// Overlay 11: the Entralink's communication with other players' games (intrude_comm.c, named by the ROM's string).
// Black's own: Black 2 kept only stubs of it.
#include "types.h"
#include "constants/version.h"
#include "battle/battle_proc.h"
#include "battle/btl_net.h"
#include "battle/btl_setup.h"
#include "field/city_state.h"
#include "field/comm_player.h"
#include "field/event_sound.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_status.h"
#include "field/player_state.h"
#include "gfl/ui.h"
#include "field/intrude_comm.h"
#include "field/intrude_work.h"
#include "field/symbol_map.h"
#include "field/symbol_save_field.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_system.h"
#include "gfl/net_whpipe.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/rtc_cache.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/wih.h"
#include "pml/poke_party.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "system/game_beacon.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/comm_player_support.h"
#include "nitro/os.h"
#include "system/game_system.h"
#include "system/wordset.h"

// The commands, from 0x200
enum {
    INTRUDE_CMD_JOIN,
    INTRUDE_CMD_LEADER,
    INTRUDE_CMD_LEAVE,
    INTRUDE_CMD_PLAYER_DATA,
    INTRUDE_CMD_DISCONNECT,
    INTRUDE_CMD_STATUS,
    INTRUDE_CMD_TIME,
    INTRUDE_CMD_EVENT_REQ,
    INTRUDE_CMD_EVENT_ANSWER,
    INTRUDE_CMD_EVENT_RESULT,
    INTRUDE_CMD_EVENT_END,
    INTRUDE_CMD_UNK_0B,
    INTRUDE_CMD_UNK_0C,
    INTRUDE_CMD_UNK_0D,
    INTRUDE_CMD_PLAYER_SET,
    INTRUDE_CMD_PLAYER,
    INTRUDE_CMD_MISSION_REQ,
    INTRUDE_CMD_MISSION_ANSWER,
    INTRUDE_CMD_MISSION,
    INTRUDE_CMD_MISSION_END,
    INTRUDE_CMD_MISSION_JOIN,
    INTRUDE_CMD_MISSION_TIME,
    INTRUDE_CMD_MISSION_ENTRUST,
    INTRUDE_CMD_MISSION_RESULT,
    INTRUDE_CMD_MISSION_REPORT,
    INTRUDE_CMD_MISSION_UNK_19,
    INTRUDE_CMD_MISSION_UNK_1A,
    INTRUDE_CMD_MISSION_UNK_1B,
    INTRUDE_CMD_PASS_POWER,
    INTRUDE_CMD_UNK_1D,
    INTRUDE_CMD_PROC_IN,
    INTRUDE_CMD_PROC_OUT,
    INTRUDE_CMD_UNK_20,
    INTRUDE_CMD_UNK_21,
    INTRUDE_CMD_HIGH_LINK,
    INTRUDE_CMD_UNK_23,
    INTRUDE_CMD_CITY_REQ,
    INTRUDE_CMD_CITY,
    INTRUDE_CMD_CITY_MSG,
    INTRUDE_CMD_CITY_REPLY,
    INTRUDE_CMD_UNK_28,
    INTRUDE_CMD_SYMBOL_MAP,
    INTRUDE_CMD_UNK_2A,
    INTRUDE_CMD_MAX,
};

// What another game's beacon carries, as the wireless layer hands it over
typedef struct {
    u8 unk_00[3];
    u8 unk_03_0 : 1;
    u8 unk_03_1 : 1;
    u8 unk_03_2 : 6;
    u32 unk_04;
    GameBeacon beacon;
} IntrudeBeaconInfo;

// The parameter the field gives the communication
typedef struct {
    GameSystem *gsys;
    GameCommSys *commSys;
    u8 mac[6];
    u8 mode;
} IntrudeCommParam;

// The parameter of the battle for a mission
typedef struct {
    GameSystem *gsys;
    u8 netId;
    u8 level;
    u8 count;
} IntrudeBattleParam;

typedef struct {
    GameSystem *gsys;
    void *heapId;
} IntrudeEventWork;

// The work of the process that runs a battle for a mission
typedef struct {
    BtlSetup setup;
    u32 unk_F0;
    GameProcManager *procMgr;
    PokeParty *party;
    BOOL overlayLoaded;
} IntrudeBattleWork;

// Where a zone is among the pairs of zones that join each other at the edges of the Entralink
typedef struct {
    u16 zoneId;
    u8 side;
    u8 index;
} IntrudeZoneLink;

// What a mission may raise a pass power level to: the kind of the mission, the least sum of the levels it takes, the
// chance in percent, and flags (bit 1 enables it)
typedef struct {
    u8 kind;
    u8 minSum;
    u8 chance;
    u8 flags;
} IntrudeProbability;


int func_ov010_02164f88(void *work);

static void func_ov011_021736e0(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173728(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov011_0217379c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_021737cc(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_021738bc(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_0217392c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_021739c8(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173a04(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173ab4(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173b20(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173b8c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173be4(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173c50(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173c9c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173d00(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173d5c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173de0(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173e5c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173eb8(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173f4c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173f9c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02173fe4(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_0217403c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_0217409c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_0217410c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02174248(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02174298(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_021742dc(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02174320(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_021743a0(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_021743e4(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_0217443c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_0217449c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02174514(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_021745b0(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02174630(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02174690(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_021746ec(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02174798(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_021747e0(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_0217482c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02174898(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov011_02174908(int netId, int size, const void *data, void *work, NetHandle *handle);
static void *func_ov011_021736a4(int netId, void *work, int size);
static void *func_ov011_021736b0(int netId, void *work, int size);
static void *func_ov011_021736c0(int netId, void *work, int size);
static void *func_ov011_021736d0(int netId, void *work, int size);
static void *func_ov011_0217358c(void *work);
static BOOL func_ov011_02173638(const void *a, const void *b);
static void func_ov011_02173644(NetHandle *handle, int a1, void *work);
static void func_ov011_02173654(void *work);

// The game's commands, from 0x200
static const NetCommand data_ov011_02176938[INTRUDE_CMD_MAX] = {
    { func_ov011_021736e0, NULL },
    { func_ov011_02173728, NULL },
    { func_ov011_0217379c, NULL },
    { func_ov011_021737cc, func_ov011_021736c0 },
    { func_ov011_021738bc, NULL },
    { func_ov011_0217392c, NULL },
    { func_ov011_021739c8, NULL },
    { func_ov011_02173a04, NULL },
    { func_ov011_02173ab4, NULL },
    { func_ov011_02173b20, NULL },
    { func_ov011_02173b8c, NULL },
    { func_ov011_02173be4, NULL },
    { func_ov011_02173c50, NULL },
    { func_ov011_02173c9c, NULL },
    { func_ov011_02173d00, NULL },
    { func_ov011_02173d5c, func_ov011_021736a4 },
    { func_ov011_02173de0, NULL },
    { func_ov011_02173e5c, NULL },
    { func_ov011_02173eb8, func_ov011_021736b0 },
    { func_ov011_02173f4c, NULL },
    { func_ov011_02173f9c, NULL },
    { func_ov011_02173fe4, NULL },
    { func_ov011_0217403c, NULL },
    { func_ov011_0217409c, NULL },
    { func_ov011_0217410c, func_ov011_021736d0 },
    { func_ov011_02174248, NULL },
    { func_ov011_02174298, NULL },
    { func_ov011_021742dc, NULL },
    { func_ov011_02174320, NULL },
    { func_ov011_021743a0, NULL },
    { func_ov011_021743e4, NULL },
    { func_ov011_0217443c, NULL },
    { func_ov011_0217449c, NULL },
    { func_ov011_02174514, NULL },
    { func_ov011_021745b0, NULL },
    { func_ov011_02174630, NULL },
    { func_ov011_02174690, NULL },
    { func_ov011_021746ec, NULL },
    { func_ov011_02174798, NULL },
    { func_ov011_021747e0, NULL },
    { func_ov011_0217482c, NULL },
    { func_ov011_02174898, NULL },
    { func_ov011_02174908, NULL },
};

static const GFLNetInitData data_ov011_021768c8 = {
    data_ov011_02176938,
    NELEMS(data_ov011_02176938),
    NULL,
    NULL,
    NULL,
    NULL,
    func_ov011_0217358c,
    func_ov010_02164f88,
    (void *)func_ov011_02173638,
    func_ov011_02173644,
    NULL,
    func_ov011_02173654,
    NULL,
    { 0 },
    NULL,
    NULL,
    0,
    { 1, 0, 0, 0, 0x80, 0x13, 0, 0 },
    HEAPID_USER,
    0xd,
    0xf,
    0xd,
    0xf0,
    0,
    3,
    0x30,
    4,
    1,
    0,
    0,
    1,
    2,
    { 0x2c, 1, 0, 0 },
    0,
    0,
};

#define INTRUDE_CMD_BASE 0x200

static void func_ov011_02173368(const void *beacon, void *work, u16 linkLevel);
static void func_ov011_02173568(void *work);
static void func_ov011_02173580(void *work);
static void func_ov011_02173590(GameData *gameData, IntrudeBeacon *beacon, s32 mode);
static BOOL func_ov011_021735fc(GameData *gameData, IntrudeBeacon *beacon);
static void func_ov011_02173658(IntrudeComm *work);
static BOOL func_ov011_021736d8(void);
static BOOL func_ov011_021736ec(IntrudeComm *work);
static BOOL func_ov011_02173758(IntrudeComm *work, u8 value);
static BOOL func_ov011_02173828(IntrudeComm *work);
static BOOL func_ov011_02173984(IntrudeComm *work, const IntrudeCommUnk8C *status);
static BOOL func_ov011_021739e0(void);
static void func_ov011_02174978(IntrudeComm *work);
static void func_ov011_02174e3c(IntrudeComm *work);
static void func_ov011_02174eac(IntrudeComm *work, u32 netId, IntrudePlayerData *data);
static void func_ov011_02174f54(IntrudeComm *work, u32 netId, const IntrudeCommUnk8C *status, BOOL a3);
void func_ov011_0217538c(IntrudeComm *work, u32 netId);
static BOOL func_ov011_02175628(void);
static void func_ov011_0217576c(IntrudeMissionWork *work);
IntrudeMissionEntrust *func_ov011_02175bcc(IntrudeMissionWork *work);
static void func_ov011_02175920(IntrudeMissionWork *work);
static void func_ov011_0217592c(IntrudeMissionWork *work);
BOOL func_ov011_02175da8(IntrudeMissionWork *work);
BOOL func_ov011_02175e90(IntrudeComm *comm, IntrudeMissionWork *work);
static BOOL func_ov011_02175eb8(IntrudeMissionWork *work);
static BOOL func_ov011_02175ed8(IntrudeMissionWork *work);

void *func_ov011_02172bc0(u32 *seq, void *heapId) {
    IntrudeCommParam *p = heapId;
    GameData *gameData = getBasePlayerBlk(p->commSys);
    PlayerInfo *info = GetGameDataPlayerInfo(gameData);
    IntrudeComm *work;
    s32 mode = 3;
    s32 i;
    u8 netId;

    if (p->mode == 1 && !func_ov010_02158590(gameData)) {
        mode = 2;
    }
    work = GFL_HeapAllocate(5, sizeof(IntrudeComm), TRUE, "intrude_comm.c", 154);
    work->gsys = p->gsys;
    work->commSys = p->commSys;
    work->unk_8FD = 1;
    work->commPlayer = func_ov010_02167518(3, p->gsys, 5, 0);
    work->unk_902 = 1;
    work->unk_901 = 1;
    func_ov011_0217538c(work, INTRUDE_NET_ID_NONE);
    func_ov011_0217576c(&work->unk_1AC);
    func_ov011_02173590(gameData, &work->beacon, mode);
    func_ov010_02161f54(&work->unk_35C);
    for (i = 0; i < 2; i++) {
        work->unk_928[i] = GFL_StrBufCreate(8, 5);
    }
    func_02012b64(gameData, 0);
    func_02012b8c(gameData, 0);
    GameData_SetLastSubscreen(gameData, 1);
    for (netId = 0; netId < INTRUDE_PLAYER_MAX; netId++) {
        func_02008494(info, func_02012950(gameData, netId));
    }
    return work;
}

BOOL func_ov011_02172cb0(u32 *seq, void *heapId, void *work) {
    IntrudeCommParam *p = heapId;
    IntrudeComm *comm = work;
    GameData *gameData = getBasePlayerBlk(p->commSys);

    if (GameData_CheckEventsPaused(gameData) == TRUE) {
        return FALSE;
    }
    switch (*seq) {
    case 0: {
        GFLNetInitData *init = GFL_HeapAllocate(0x8005, sizeof(GFLNetInitData), FALSE, "intrude_comm.c", 207);

        *init = data_ov011_021768c8;
        if (p->mode == 1 && !func_ov010_02158590(gameData)) {
            init->maxConnectNum = 2;
        }
        func_0203cbac(init, func_ov011_02173568, comm);
        GFL_HeapFree(init);
        (*seq)++;
        break;
    }
    case 1:
        if (comm->unk_8FD < 2) {
            break;
        }
        if (p->mode == 1) {
            func_0203d10c(NULL);
            comm->unk_8FD = 3;
        } else {
            func_0203cff0(p->mac);
            comm->unk_8FD = 4;
        }
        return TRUE;
    }
    return FALSE;
}

void func_ov011_02172d70(u32 *seq, void *heapId, void *work) {
    IntrudeCommParam *p = heapId;
    IntrudeComm *comm = work;
    GameData *gameData = getBasePlayerBlk(p->commSys);
    PlayerInfo *info;
    s32 start;
    s32 now;

    if (comm != NULL && comm->unk_8FD >= 7) {
        func_ov010_02161f74(&comm->unk_35C, func_020127c4(gameData), comm->unk_8FB);
    }
    if (func_0200fb24() || comm->unk_93F_1 == 1) {
        if (!comm->unk_93F_6) {
            comm->unk_8FD = 11;
            comm->unk_93F_1 = 1;
            func_02026400(comm->commSys);
        }
        return;
    }
    if (func_02035390() == TRUE && GetIsShouldUICallback() == TRUE && !comm->unk_93F_6) {
        func_02026400(comm->commSys);
        return;
    }
    if (comm->unk_8FD == 7 && comm->unk_8FA == 1) {
        func_02026400(comm->commSys);
        return;
    }
    func_ov011_021735fc(gameData, &comm->beacon);
    func_ov011_02173658(comm);
    switch (*seq) {
    case 0:
        if (func_020392b8() > 1 && comm->unk_8FD < 5) {
            comm->unk_8FD = 5;
            func_0203d7bc(TRUE);
        }
        if (comm->unk_8FD == 5) {
            if (func_02039acc() == TRUE) {
                func_02012b64(gameData, func_02039338());
                info = func_02012950(gameData, func_02039338());
                func_02008494(GetGameDataPlayerInfo(gameData), info);
                func_ov011_02175710(comm, func_02039338());
                comm->unk_070.unk_1B_0 = GAME_VERSION;
                comm->unk_070.unk_1A_6 = GameData_GetSeason(gameData);
                if (!func_ov010_02158590(gameData)) {
                    comm->unk_070.unk_1B_6 = 1;
                }
                func_0203d7bc(TRUE);
                (*seq)++;
                return;
            }
        } else if (comm->unk_8FD == 4) {
            comm->unk_90E++;
            if (comm->unk_90E > 600) {
                comm->unk_90E = 0;
                func_02026400(comm->commSys);
                return;
            }
        }
        break;
    case 1:
        if (func_020399b8(func_020399a8()) == TRUE) {
            comm->unk_8FD = 6;
            (*seq)++;
            return;
        }
        break;
    case 2:
        func_ov010_02161f54(&comm->unk_35C);
        func_ov011_02174e3c(comm);
        func_ov011_02174eac(comm, func_02039338(), &comm->unk_160);
        (*seq)++;
        return;
    case 3:
        if (func_ov011_02173828(comm) == TRUE) {
            comm->unk_8FD = 7;
            comm->unk_940_0 = RTC_ConvertDaySecondsCached();
            (*seq)++;
            return;
        }
        break;
    case 4:
        if (!GameData_IsForceSeasonSync(gameData) && comm->unk_070.unk_1A_6 != GameData_GetSeason(gameData) &&
            GSYS_CheckField(p->gsys) == TRUE) {
            func_02026400(comm->commSys);
            return;
        }
        func_ov011_02174978(comm);
        if (func_ov011_02175eb8(&comm->unk_1AC) == TRUE) {
            func_02026400(comm->commSys);
            return;
        }
        if (func_0203d378() == TRUE && !func_ov011_02175b14(&comm->unk_1AC)) {
            start = comm->unk_940_0;
            now = RTC_ConvertDaySecondsCached();
            if (now < start) {
                now += 24 * 60 * 60 - 1;
            }
            now -= start;
            if (now >= 180) {
                func_02026400(comm->commSys);
                return;
            }
            if (now > 170 && !comm->unk_940_31 && func_ov011_021739e0() == TRUE) {
                comm->unk_940_31 = 1;
            }
        } else if (!comm->unk_93F_6 && func_0203d378() == TRUE && func_ov011_02175ed8(&comm->unk_1AC) == TRUE) {
            func_02026400(comm->commSys);
        } else {
            comm->unk_940_31 = 0;
        }
        if (!func_ov011_02175628()) {
            comm->unk_90A++;
            if (comm->unk_90A > 180) {
                func_02026400(comm->commSys);
                return;
            }
        } else {
            comm->unk_90A = 0;
        }
        break;
    }
}

BOOL func_ov011_021730ac(u32 *seq, void *heapId, void *work) {
    IntrudeCommParam *p = heapId;
    IntrudeComm *comm = work;
    GameData *gameData = getBasePlayerBlk(p->commSys);

    if (GameData_CheckEventsPaused(gameData) == TRUE) {
        return FALSE;
    }
    if (comm->unk_93F_1 == 1 || func_0200fb24()) {
        comm->unk_8F8_6 = 1;
        func_0203ceac(func_ov011_02173580);
        return TRUE;
    }
    switch (*seq) {
    case 0:
        comm->unk_8FD = 9;
        if (func_0203d1b4() <= 1) {
            *seq = 4;
        } else if (func_ov011_021736ec(comm) == TRUE) {
            (*seq)++;
        }
        break;
    case 1:
        comm->unk_90E = 0;
        func_02039c68(func_020399a8(), 20, 2);
        (*seq)++;
        break;
    case 2:
        comm->unk_90E++;
        if (func_02039cb4(func_020399a8(), 20, 2) == TRUE || comm->unk_90E > 300) {
            func_0203d7bc(FALSE);
            (*seq)++;
        }
        break;
    case 3:
        comm->unk_90E++;
        if (func_0203d378() == TRUE) {
            if (func_0203d1b4() <= 1 || comm->unk_90E > 300) {
                (*seq)++;
            }
        } else {
            (*seq)++;
        }
        break;
    case 4:
        comm->unk_8F8_6 = 1;
        func_0203ceac(func_ov011_02173580);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov011_021731c8(u32 *seq, void *heapId, void *work) {
    IntrudeCommParam *p = heapId;
    IntrudeComm *comm = work;
    GameCommSys *commSys = p->commSys;
    GameData *gameData = getBasePlayerBlk(commSys);
    BOOL done = FALSE;
    IntrudeMissionEntrust *entrust;
    u32 netId;
    s32 i;

    if (GameData_CheckEventsPaused(gameData) == TRUE) {
        return done;
    }
    if (comm->unk_8FD == 10 || func_0203cdbc() == TRUE) {
        if (!GameData_IsForceSeasonSync(gameData)) {
            func_0200fba0();
            done = TRUE;
        } else if (IsZoneEntralinkHub(PlayerState_GetZoneID(GameData_GetPlayerState(gameData))) == TRUE &&
                   GSYS_CheckField(p->gsys) == TRUE && Field_CheckMapLoadFinished(GSYS_GetField(p->gsys)) == TRUE) {
            done = TRUE;
        }
    }
    if (done == TRUE) {
        func_ov010_02161f68(&comm->unk_35C);
        func_0201281c(gameData);
        func_ov010_0216754c(p->gsys, comm->commPlayer);
        if (comm->unk_93F_1 == 1 || func_0200fb24()) {
            func_02026480(commSys, 5);
        } else if (func_ov011_02175da8(&comm->unk_1AC) == TRUE) {
            if (comm->unk_904_0 == 1) {
                func_02026480(commSys, 4);
            } else if (func_ov011_02175e90(comm, &comm->unk_1AC) == TRUE) {
                func_02026480(commSys, 2);
            } else {
                entrust = func_ov011_02175bcc(&comm->unk_1AC);
                netId = func_02012b58(gameData);
                for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
                    if (entrust->netIds[i] != INTRUDE_NET_ID_NONE && entrust->netIds[i] != netId) {
                        break;
                    }
                }
                if (i < INTRUDE_PLAYER_MAX) {
                    func_02026480(commSys, 3);
                } else {
                    func_02026480(commSys, 4);
                }
            }
        } else if (comm->unk_8FB > 1) {
            func_02026480(commSys, 1);
        }
        if (!GameData_IsForceSeasonSync(gameData)) {
            func_02012b64(gameData, 0);
            func_02012b8c(gameData, 0);
            GameData_SetLastSubscreen(gameData, 1);
        } else {
            GameData_SetLastSubscreen(gameData, 1);
        }
        if ((comm->unk_93F_1 == 1 || func_0200fb24()) && GameData_IsForceSeasonSync(gameData) == TRUE) {
            func_02012124(p->gsys, 1);
        }
        func_02026680(commSys);
        for (i = 0; i < 2; i++) {
            GFL_StrBufFree(comm->unk_928[i]);
        }
        GFL_HeapFree(comm);
        GFL_HeapFree(p);
        return TRUE;
    }
    return FALSE;
}

static void func_ov011_02173368(const void *beacon, void *work, u16 linkLevel) {
    IntrudeComm *comm = work;
    const WMBssDesc *bssDesc = beacon;
    IntrudeBeaconInfo *info;
    u8 gameCommandBase;
    s32 i;

    if (comm->unk_925 >= 2) {
        return;
    }
    if (func_0203d1b4() > 1) {
        return;
    }
    if (comm->unk_93F_1 == 1) {
        return;
    }
    if (func_0200fb24()) {
        return;
    }
    if (!func_ov014_0217873c(bssDesc)) {
        return;
    }
    if (!func_ov014_02177494(beacon, ~0x10)) {
        return;
    }
    info = (IntrudeBeaconInfo *)func_ov014_02177bfc((NetWhpipeBeacon *)beacon, &gameCommandBase);
    if (gameCommandBase != 3) {
        return;
    }
    if (info->unk_03_1) {
        return;
    }
    for (i = 0; i < comm->unk_925; i++) {
        if (info->unk_04 == comm->unk_930[i]) {
            return;
        }
    }
    if (IsZoneEntralinkHub(info->beacon.zoneId) == TRUE) {
        return;
    }
    if (!func_ov010_02158570(info->beacon.zoneId)) {
        return;
    }
    func_ov010_02165c34(&info->beacon, comm->unk_928[comm->unk_925]);
    comm->unk_938[comm->unk_925] = func_ov010_02165d08(&info->beacon);
    comm->unk_93A[comm->unk_925] = func_ov010_02165c50(&info->beacon);
    comm->unk_930[comm->unk_925] = info->unk_04;
    comm->unk_926 = 0;
    comm->unk_925++;
}

void func_ov011_02173464(void *heapId, void *work, Field *field) {
    IntrudeCommParam *p = heapId;
    IntrudeComm *comm = work;
    GameData *gameData;
    int netId;
    s32 i;

    if (comm == NULL) {
        return;
    }
    gameData = getBasePlayerBlk(p->commSys);
    netId = func_02012b58(gameData);
    if (!GameData_IsForceSeasonSync(gameData)) {
        if (func_ov018_0217c11c(Field_GetPlayerStateZoneID(field)) == TRUE) {
            func_ov010_021676e4(comm->commPlayer);
        } else {
            func_ov010_02167794(comm->commPlayer);
        }
    } else {
        func_ov010_021676e4(comm->commPlayer);
    }
    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        if (i == netId) {
            continue;
        }
        if (func_ov010_021677e4(comm->commPlayer, i) == TRUE) {
            CommPlayerStatus status;

            comm->unk_08C[i].status.unk0 = comm->unk_08C[i].unk_1B_7;
            status = comm->unk_08C[i].status;
            status.unk0 = TRUE;
            func_ov010_02167848(comm->commPlayer, i, &status);
        }
    }
    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        if (i == netId) {
            continue;
        }
        if (comm->unk_8FB & (1 << i)) {
            BOOL flag = comm->unk_08C[i].unk_1B_7;

            func_ov011_02174f54(comm, i, &comm->unk_08C[i], FALSE);
            comm->unk_08C[i].unk_1B_7 = flag;
        }
    }
}

void func_ov011_02173558(void *heapId, void *work, Field *field) {
    IntrudeComm *comm = work;

    if (comm != NULL) {
        func_ov010_02167688(comm->commPlayer);
    }
}

static void func_ov011_02173568(void *work) {
    IntrudeComm *comm = work;

    comm->unk_8FD = 2;
    func_ov014_0217963c(func_ov011_02173368);
}

static void func_ov011_02173580(void *work) {
    IntrudeComm *comm = work;

    comm->unk_8FD = 10;
}

static void *func_ov011_0217358c(void *work) {
    IntrudeComm *comm = work;

    return &comm->beacon;
}

static void func_ov011_02173590(GameData *gameData, IntrudeBeacon *beacon, s32 mode) {
    beacon->unk_00 = 2;
    beacon->unk_01_0 = func_0203d1b4();
    if (mode != -1) {
        beacon->unk_01_4 = mode;
    }
    beacon->unk_02_0 = func_02039414();
    beacon->unk_02_1 = 1;
    func_02008494(GetGameDataPlayerInfo(gameData), &beacon->player);
}

static BOOL func_ov011_021735fc(GameData *gameData, IntrudeBeacon *beacon) {
    if (beacon->unk_01_0 != func_0203d1b4() || beacon->unk_02_0 != func_02039414()) {
        func_ov011_02173590(gameData, beacon, -1);
        func_ov014_0217778c();
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov011_02173638(const void *a, const void *b) {
    if (a == b) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov011_02173644(NetHandle *handle, int a1, void *work) {
    IntrudeComm *comm = work;

    comm->unk_93F_1 = 1;
}

static void func_ov011_02173654(void *work) {
}

static void func_ov011_02173658(IntrudeComm *work) {
    s32 i;

    if (work->unk_925 != 0) {
        work->unk_926++;
        if (work->unk_926 > 150) {
            for (i = 0; i < work->unk_925; i++) {
                work->unk_930[i] = 0;
            }
            work->unk_926 = 0;
            work->unk_925 = 0;
        }
    }
}

static void *func_ov011_021736a4(int netId, void *work, int size) {
    IntrudeComm *comm = work;

    return &comm->unk_648;
}

static void *func_ov011_021736b0(int netId, void *work, int size) {
    IntrudeComm *comm = work;

    return &comm->unk_754[netId];
}

static void *func_ov011_021736c0(int netId, void *work, int size) {
    IntrudeComm *comm = work;

    return &comm->unk_670[netId];
}

static void *func_ov011_021736d0(int netId, void *work, int size) {
    IntrudeComm *comm = work;

    return &comm->unk_7F0;
}

static BOOL func_ov011_021736d8(void) {
    return func_ov011_02175628();
}

static void func_ov011_021736e0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    comm->unk_8FA = 1;
}

static BOOL func_ov011_021736ec(IntrudeComm *work) {
    if (func_ov011_021736d8() == FALSE) {
        func_ov011_021736e0(func_02039338(), 0, NULL, work, func_020399a8());
        return TRUE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_JOIN, 0, NULL);
}

static void func_ov011_02173728(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const u8 *count = data;
    GameData *gameData = getBasePlayerBlk(comm->commSys);

    if (netId != func_0203d1a4(func_020399a8())) {
        comm->unk_902 = *count;
        GameData_SetLastSubscreen(gameData, *count);
    }
}

static BOOL func_ov011_02173758(IntrudeComm *work, u8 value) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d3f8(func_020399a8(), 0xff, INTRUDE_CMD_BASE + INTRUDE_CMD_LEADER, 1, &value, 1, FALSE, FALSE);
}

void func_ov011_0217379c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    comm->unk_8FE = 1;
}

static BOOL func_ov011_021737a8(void) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_LEAVE, 0, NULL);
}

static void func_ov011_021737cc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    IntrudePlayerData *playerData = (IntrudePlayerData *)data;
    GameData *gameData = getBasePlayerBlk(comm->commSys);

    if (netId >= comm->unk_902) {
        return;
    }
    func_ov011_02174eac(comm, netId, playerData);
    if (netId == func_02012b58(gameData)) {
        return;
    }
    if (func_0203d378()) {
        return;
    }
    if (netId != 0) {
        return;
    }
    if (playerData->status.unk_1B_6 == 1) {
        comm->unk_93F_0 = 1;
    }
}

static BOOL func_ov011_02173828(IntrudeComm *work) {
    BOOL sent;

    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    if (func_0203d578(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_PLAYER_DATA) == TRUE) {
        return FALSE;
    }
    MI_CpuCopy8(&work->unk_160, &work->unk_3A4, sizeof(IntrudePlayerData));
    sent = func_0203d3f8(func_020399a8(), 0xff, INTRUDE_CMD_BASE + INTRUDE_CMD_PLAYER_DATA, sizeof(IntrudePlayerData),
                         &work->unk_3A4, 0, FALSE, TRUE);
    if (sent == TRUE) {
        work->unk_8FE = 0;
        if (func_0203d378() == TRUE) {
            work->unk_903 = 1;
            func_ov011_02175920(&work->unk_1AC);
            func_ov011_0217592c(&work->unk_1AC);
        }
    }
    return sent;
}

static void func_ov011_021738bc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const s32 *id = data;
    u32 bit = 1 << *id;

    if (comm->unk_8FB & bit) {
        comm->unk_8FB ^= bit;
        func_ov010_02167658(comm->commPlayer, *id);
    }
}

static BOOL func_ov011_021738e4(IntrudeComm *work, s32 netId) {
    if (func_ov011_021736d8() == FALSE) {
        func_ov011_021738bc(func_02039338(), 4, &netId, work, func_020399a8());
        return TRUE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_DISCONNECT, 4, &netId);
}

static void func_ov011_0217392c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const IntrudeCommUnk8C *status = data;

    if (netId == func_0203d1a4(func_020399a8())) {
        func_020264b4(comm->commSys, netId, status->zoneId, status->unk_16_0, GAME_VERSION, FALSE);
    } else if (comm->unk_8FB & (1 << netId)) {
        func_ov011_02174f54(comm, netId, status, FALSE);
    }
}

static BOOL func_ov011_02173984(IntrudeComm *work, const IntrudeCommUnk8C *status) {
    BOOL sent;

    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    sent = func_0203d3f8(func_020399a8(), 0xff, INTRUDE_CMD_BASE + INTRUDE_CMD_STATUS, sizeof(IntrudeCommUnk8C), status,
                         0, TRUE, FALSE);
    if (sent == TRUE) {
        work->unk_900 = 0;
    }
    return sent;
}

static void func_ov011_021739c8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    comm->unk_940_31 = 1;
}

static BOOL func_ov011_021739e0(void) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_TIME, 0, NULL);
}

static BOOL func_ov011_021753d0(IntrudeComm *work, u32 netId, u8 kind);
static void func_ov011_021754e8(IntrudeComm *work, u32 netId, u8 answer, u8 kind);
static void func_ov011_02175508(IntrudeComm *work, u32 netId, u8 kind);
void func_ov011_02175648(GameData *gameData, IntrudeHighLinkData *data);

static void func_ov011_02173a04(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const IntrudeMissionRequest *req = data;

    if (func_ov011_021753d0(comm, netId, req->unk_01) == TRUE) {
        comm->unk_11C = *req;
    }
}

BOOL func_ov011_02173a2c(IntrudeComm *work, u32 netId, const IntrudeMissionEntry *entry, u8 kind) {
    IntrudeMissionRequest req;
    BOOL sent;

    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    if (work->unk_944 == 0) {
        work->unk_944++;
    }
    MI_CpuFill8(&req, 0, sizeof(req));
    req.unk_00 = kind;
    req.entry = *entry;
    req.unk_01 = work->unk_944;
    sent = func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_EVENT_REQ, sizeof(req), &req, 0, FALSE,
                         FALSE);
    if (sent == TRUE) {
        work->unk_154.unk_04 = work->unk_944;
        work->unk_154.unk_05 = work->unk_944;
    }
    return sent;
}

static void func_ov011_02173ab4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const u8 *answer = data;
    u8 kind = answer[1];

    if (kind == comm->unk_154.unk_05) {
        func_ov011_021754e8(comm, netId, answer[0], kind);
    } else {
        comm->unk_948[netId] = kind;
    }
}

BOOL func_ov011_02173ae0(IntrudeComm *work, u32 netId, u8 answer, u8 kind) {
    u8 buf[4];

    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    buf[0] = answer;
    buf[1] = kind;
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_EVENT_ANSWER, sizeof(buf), buf, 0, FALSE,
                         FALSE);
}

static void func_ov011_02173b20(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const u8 *result = data;

    if (comm->unk_154.netId == netId && comm->unk_154.unk_04 == result[0]) {
        func_ov011_021754e8(comm, netId, 3, result[0]);
    }
}

static BOOL func_ov011_02173b44(IntrudeComm *work, u32 netId, u8 kind) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_EVENT_RESULT, 1, &kind, 0, FALSE, FALSE);
}

static void func_ov011_02173b8c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const u8 *kind = data;

    func_ov011_02175508(work, netId, kind[0]);
}

BOOL func_ov011_02173b9c(u32 netId, u8 kind) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_EVENT_END, 1, &kind, 0, FALSE, FALSE);
}

static void func_ov011_02173be4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const IntrudeUnk15C *src = data;

    comm->unk_15C = *src;
}

BOOL func_ov011_02173c04(u32 netId, u8 value) {
    u8 buf[4];

    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    MI_CpuFill8(buf, 0, sizeof(buf));
    buf[0] = 1;
    buf[1] = value;
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_UNK_0B, sizeof(buf), buf, 0, FALSE,
                         FALSE);
}

static void func_ov011_02173c50(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    comm->unk_924 |= 1 << netId;
}

BOOL func_ov011_02173c6c(IntrudeComm *work, u8 netId) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_UNK_0C, 0, NULL, 0, FALSE, FALSE);
}

static void func_ov011_02173c9c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const IntrudeHighLinkData *src = data;

    comm->unk_914 = *src;
}

static BOOL func_ov011_02173cb0(IntrudeComm *work, u32 netId) {
    GameData *gameData = getBasePlayerBlk(work->commSys);
    getHighLinkBlockAddress(GameData_GetSaveControl(gameData));
    IntrudeHighLinkData data;

    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    func_ov011_02175648(gameData, &data);
    return func_0203d4d0(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_UNK_0D, sizeof(data), &data, 0, FALSE,
                         FALSE);
}

static void func_ov011_02175914(IntrudeMissionWork *work, u32 index);
static void func_ov011_02175938(IntrudeMissionWork *work, const IntrudeCommPlayer *player, u32 index);
static BOOL func_ov011_02175954(IntrudeComm *comm, IntrudeMissionWork *work, const IntrudeMissionEntry *entry);
static void func_ov011_021759b0(IntrudeMissionWork *work);
static void func_ov011_021759c0(IntrudeMissionWork *work, u8 netId);
static void func_ov011_021759f8(IntrudeMissionWork *work, u32 time);
static void func_ov011_02175c04(IntrudeMissionWork *work, u32 netId);
static void func_ov011_02175c58(IntrudeMissionWork *work, u32 value);
static BOOL func_ov011_02175c88(IntrudeMissionWork *work, const IntrudeMissionEntry *entry, u32 netId);
static void func_ov011_02175d2c(IntrudeMissionWork *work, u32 result);
static void func_ov011_02175d7c(IntrudeComm *comm, IntrudeMissionWork *work, const IntrudeMissionEntrust *entrust);
static void func_ov011_02175e30(GameData *gameData, IntrudeComm *comm, const IntrudeMissionEntrust *entrust);
static void func_ov011_02175f34(IntrudeComm *work, IntrudeCommPlayer *players, u32 value, u32 index);
static BOOL func_ov011_02176048(IntrudeComm *comm, IntrudeMissionWork *work, const IntrudeMissionEntry *entry, u32 netId);
static void func_ov011_021761c4(IntrudeMissionWork *work, const IntrudeMissionRequest *answer);
void insideProbabilityLoop(PassPowerLevel *levels, u32 kind);
static BOOL func_ov011_02176424(PassPowerLevel *levels, u8 *up, u8 *down);

static void func_ov011_02173d00(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const u32 *index = data;

    if (func_0203d378()) {
        func_ov011_02175f34(comm, comm->unk_1AC.players, netId, *index);
        func_ov011_02175914(&comm->unk_1AC, *index);
    }
}

BOOL func_ov011_02173d2c(IntrudeComm *work, u32 index) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_PLAYER_SET, 4, &index);
}

static void func_ov011_02173d5c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const IntrudeCommPlayer *player = data;
    GameData *gameData = getBasePlayerBlk(comm->commSys);
    u8 index = player->player.unk_10;

    func_ov011_02175938(&comm->unk_1AC, player, index);
    if (index != func_0203d1a4(func_020399a8())) {
        MI_CpuCopy8(player, func_020127ac(gameData, index), sizeof(PassPowerLevel));
    }
}

static BOOL func_ov011_02173da0(IntrudeComm *work, const IntrudeCommPlayer *players, u32 index) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d3f8(func_020399a8(), 0xff, INTRUDE_CMD_BASE + INTRUDE_CMD_PLAYER, sizeof(IntrudeCommPlayer),
                         &players[index], 0, FALSE, TRUE);
}

static void func_ov011_02173de0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const IntrudeMissionEntry *entry = data;

    if (func_0203d378()) {
        if (func_ov011_02176048(comm, &comm->unk_1AC, entry, netId) == TRUE) {
            if (entry->mission.kind == 0 || entry->mission.kind == 3) {
                func_0203d1e8(func_020399a8(), 0);
                func_ov014_021780fc(1);
                comm->unk_93C = 1;
            }
        }
    }
}

BOOL func_ov011_02173e2c(IntrudeComm *work, const IntrudeMissionEntry *entry) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    func_ov011_02176200(&work->unk_1AC);
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_REQ, 0x30, entry);
}

static void func_ov011_02173e5c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    func_ov011_021761c4(&comm->unk_1AC, data);
}

static BOOL func_ov011_02173e6c(IntrudeComm *work, const IntrudeMissionRequest *req, u32 netId) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    if (func_0203d1c0(netId) == FALSE) {
        return TRUE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_ANSWER,
                         sizeof(IntrudeMissionRequest), req, 0, FALSE, FALSE);
}

static void func_ov011_02173eb8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    GameData *gameData = getBasePlayerBlk(comm->commSys);

    PlayerState_GetZoneID(GameData_GetPlayerState(gameData));
    if (func_ov011_02175954(comm, &comm->unk_1AC, data) == TRUE) {
        comm->unk_90D = 1;
    }
}

static BOOL func_ov011_02173eec(IntrudeComm *work, IntrudeMissionWork *missionWork) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    if (func_0203d578(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION) == TRUE) {
        return FALSE;
    }
    MI_CpuCopy8(&missionWork->entry, &work->unk_3F0, sizeof(IntrudeMissionEntry));
    return func_0203d3f8(func_020399a8(), 0xff, INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION, sizeof(IntrudeMissionEntry),
                         &work->unk_3F0, 0, FALSE, TRUE);
}

static void func_ov011_02173f4c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    IntrudeMissionEntry *entry = func_ov011_02175b34(&comm->unk_1AC);

    if (entry != NULL && GFL_STD_MemCmp(entry, data, sizeof(IntrudeMissionEntry)) == 0) {
        func_ov011_021759b0(&comm->unk_1AC);
    }
}

static BOOL func_ov011_02173f74(IntrudeComm *work, IntrudeMissionWork *missionWork) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_END, sizeof(IntrudeMissionEntry),
                         &missionWork->entry);
}

static void func_ov011_02173f9c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    func_ov011_02175b34(&comm->unk_1AC);
    if (func_0203d378()) {
        func_ov011_021759c0(&comm->unk_1AC, netId);
    }
}

BOOL func_ov011_02173fc0(IntrudeComm *work, IntrudeMissionWork *missionWork) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_JOIN, sizeof(IntrudeMissionEntry),
                         &missionWork->entry);
}

static void func_ov011_02173fe4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const u32 *time = data;

    func_ov011_021759f8(&comm->unk_1AC, *time);
}

static BOOL func_ov011_02173ff4(IntrudeComm *work, s32 time, u32 netId) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d4d0(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_TIME, 4, &time, 0, FALSE, FALSE);
}

static void func_ov011_0217403c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    if (func_0203d378()) {
        func_ov011_02175c88(&comm->unk_1AC, data, netId);
    }
}

BOOL func_ov011_0217405c(IntrudeComm *work, IntrudeMissionWork *missionWork) {
    BOOL sent;

    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    func_ov011_02175d20(&work->unk_1AC);
    sent = func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_ENTRUST, sizeof(IntrudeMissionEntry),
                         &missionWork->entry);
    if (sent == TRUE) {
        func_ov011_02175bac(&work->unk_1AC);
    }
    return sent;
}

static void func_ov011_0217409c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const u32 *result = data;

    func_ov011_02175d2c(&comm->unk_1AC, *result);
}

static BOOL func_ov011_021740ac(IntrudeComm *work, u32 result, u32 netId) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    if (func_0203d1c0(netId) == FALSE) {
        return TRUE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_RESULT, 4, &result, 0, FALSE,
                         FALSE);
}

static void func_ov011_0217410c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const IntrudeMissionEntrust *entrust = data;
    GameData *gameData = getBasePlayerBlk(comm->commSys);
    PassPowerLevel *power = func_020127b8(gameData);
    BOOL counted = FALSE;
    u8 up = 0;
    u8 down = 0;
    int a = 0;
    int b = 0;

    if (entrust->entry.netId == INTRUDE_NET_ID_NONE) {
        return;
    }
    if (func_ov011_02175da8(&comm->unk_1AC) == TRUE) {
        return;
    }
    if (entrust->entry.player.unk_10 == func_0203d1a4(func_020399a8())) {
        if (entrust->unk_37 == 1) {
            insideProbabilityLoop(power, entrust->entry.mission.kind);
            comm->unk_907 = 1;
        } else {
            func_ov011_02175e30(gameData, comm, entrust);
            comm->unk_907 = 1;
        }
        if (func_ov011_02176424(power, &up, &down) == TRUE) {
            counted = TRUE;
            comm->unk_907 = TRUE;
        }
        if (entrust->entry.unk_2E == 1) {
            b = 1;
        } else {
            a = 1;
        }
        if (counted == TRUE) {
            b += down;
            a += up;
        }
        comm->unk_354.unk_00 = a;
        comm->unk_354.unk_01 = b;
        comm->unk_354.unk_02 = 1;
        comm->unk_93E = 1;
    }
    func_ov011_02175d7c(comm, &comm->unk_1AC, entrust);
}

static BOOL func_ov011_021741e8(IntrudeComm *work, IntrudeMissionWork *missionWork) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    if (func_0203d578(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_REPORT) == TRUE) {
        return FALSE;
    }
    MI_CpuCopy8(&missionWork->unk_0AC, &work->unk_424, sizeof(IntrudeMissionEntrust));
    return func_0203d3f8(func_020399a8(), 0xff, INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_REPORT,
                         sizeof(IntrudeMissionEntrust), &work->unk_424, 0, FALSE, TRUE);
}

static void func_ov011_02174248(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    func_ov011_02175c04(&comm->unk_1AC, netId);
}

BOOL func_ov011_02174258(IntrudeComm *work) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    func_ov011_02175c58(&work->unk_1AC, 0);
    return func_0203d3f8(func_020399a8(), 0, INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_UNK_19, 0, NULL, 0, FALSE, FALSE);
}

static void func_ov011_02174298(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    func_ov011_02175c58(&comm->unk_1AC, 1);
}

static BOOL func_ov011_021742a8(u8 netId) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d4d0(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_UNK_1A, 0, NULL, 0, FALSE, FALSE);
}

static void func_ov011_021742dc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    func_ov011_02175c58(&comm->unk_1AC, 2);
}

static BOOL func_ov011_021742ec(u8 netId) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d4d0(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_MISSION_UNK_1B, 0, NULL, 0, FALSE, FALSE);
}

static void func_ov011_02174320(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    GameData *gameData = getBasePlayerBlk(comm->commSys);

    if (netId == func_0203d1a4(func_020399a8())) {
        return;
    }
    if (!(comm->unk_8FB & (1 << netId))) {
        return;
    }
    MI_CpuCopy8(data, func_020127ac(gameData, netId), sizeof(PassPowerLevel));
    if (netId == comm->unk_070.unk_16_0) {
        comm->unk_906 = 1;
    }
}

static BOOL func_ov011_02174374(IntrudeComm *work) {
    GameData *gameData = getBasePlayerBlk(work->commSys);
    PassPowerLevel *power = func_020127b8(gameData);

    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_PASS_POWER, sizeof(PassPowerLevel), power);
}

static void func_ov011_021743a0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    MI_CpuCopy8(data, comm->unk_358, 4);
}

static BOOL func_ov011_021743b4(IntrudeComm *work) {
    getBasePlayerBlk(work->commSys);

    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_UNK_1D, 4, &work->unk_354);
}

static void func_ov011_021743e4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    comm->unk_93D++;
    if (comm->unk_93C == 0) {
        func_0203d1e8(func_020399a8(), 0);
        func_ov014_021780fc(1);
    }
}

BOOL func_ov011_0217440c(void) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3f8(func_020399a8(), 0, INTRUDE_CMD_BASE + INTRUDE_CMD_PROC_IN, 0, NULL, 0, FALSE, FALSE);
}

static void func_ov011_0217443c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    comm->unk_93D--;
    if (comm->unk_93D == 0 && comm->unk_93C == 0) {
        func_0203d1e8(func_020399a8(), 1);
        func_ov014_021780fc(0);
    }
}

BOOL func_ov011_0217446c(void) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3f8(func_020399a8(), 0, INTRUDE_CMD_BASE + INTRUDE_CMD_PROC_OUT, 0, NULL, 0, FALSE, FALSE);
}

static void func_ov011_0217449c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const u8 *flags = data;

    if (netId != func_0203d1a4(func_020399a8())) {
        comm->unk_93F_4 |= *flags;
    }
}

BOOL func_ov011_021744d0(IntrudeComm *work, u8 netId, u8 value) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_UNK_20, 1, &value, 0, FALSE, FALSE);
}

static void func_ov011_02174514(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const u32 *type = data;
    GameData *gameData = getBasePlayerBlk(comm->commSys);
    CommPlayerSupport *support = func_02012f20(gameData);
    u32 state;

    if (!(comm->unk_8FB & (1 << netId))) {
        return;
    }
    if (FieldStatus_GetBusyFlag(GameData_GetFieldStatus(gameData)) != TRUE) {
        return;
    }
    state = func_0202e7e0(support);
    if (state != 0 && state != 3) {
        return;
    }
    func_0202e7c4(support, *type, func_02012950(gameData, netId));
}

BOOL func_ov011_0217456c(IntrudeComm *work, u8 netId, u32 type) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_UNK_21, 4, &type, 0, FALSE, FALSE);
}

static void func_ov011_021745b0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    getBasePlayerBlk(comm->commSys);

    if (!(comm->unk_8FB & (1 << netId))) {
        return;
    }
    if (comm->unk_8F8_0) {
        return;
    }
    MI_CpuCopy8(data, &comm->unk_8E4, sizeof(IntrudeUnk8E4));
    comm->unk_8F8_0 = 1;
}

BOOL func_ov011_021745f8(u8 netId, const void *data) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_HIGH_LINK, 0x14, data, 0, FALSE, FALSE);
}

static void func_ov011_02174630(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const u8 *value = data;

    if (netId != func_0203d1a4(func_020399a8()) && (comm->unk_8FB & (1 << netId))) {
        func_020266ac(comm->commSys, netId, *value);
    }
}

BOOL func_ov011_02174660(u8 value) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_UNK_23, 1, &value);
}

static void func_ov011_02174690(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    comm->unk_908 |= 1 << netId;
}

BOOL func_ov011_021746ac(IntrudeComm *work, u8 netId) {
    BOOL sent;

    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    sent = func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_CITY_REQ, 0, NULL, 0, FALSE, FALSE);
    if (sent == TRUE) {
        work->unk_90C = 0;
    }
    return sent;
}

static void func_ov011_021746ec(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    CityState *city = (CityState *)data;
    GameData *gameData = getBasePlayerBlk(comm->commSys);
    CityState *own = func_020127d0(gameData, 0);

    func_ov020_021861f4(city, func_ov010_021583ec(gameData, netId));
    MI_CpuCopy8(city, own, sizeof(CityState));
    func_02012800(gameData, city);
    comm->unk_90C = 1;
}

static BOOL func_ov011_02174734(IntrudeComm *work, u32 netId, CityState *city) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    if (func_0203d578(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_CITY) == TRUE) {
        return FALSE;
    }
    MI_CpuCopy8(city, &work->unk_460, sizeof(CityState));
    return func_0203d4d0(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_CITY, sizeof(CityState), city, 0, FALSE,
                         TRUE);
}

static void func_ov011_02174798(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    func_ov010_02161ff0(&comm->unk_35C, data);
}

static BOOL func_ov011_021747a8(const CityCommMessage *message, u8 netId) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_CITY_MSG, sizeof(CityCommMessage),
                         message, 0, FALSE, FALSE);
}

static void func_ov011_021747e0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;

    func_ov010_02162008(&comm->unk_35C, netId, data);
}

static BOOL func_ov011_021747f4(const CityCommMessage *message, u8 netId) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_CITY_REPLY, sizeof(CityCommMessage),
                         message, 0, FALSE, FALSE);
}

static void func_ov011_0217482c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const IntrudeUnk15C *src = data;

    comm->unk_8D4[netId] = *src;
}

static BOOL func_ov011_02174854(IntrudeComm *work, u8 netId, const void *data) {
    if (func_ov011_021736d8() == FALSE) {
        return FALSE;
    }
    work->unk_8F8_1 = 0;
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_UNK_28, 4, data, 0, FALSE, FALSE);
}

static void func_ov011_02174898(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    const SymbolMapList *list = data;

    comm->unk_82C = *list;
    comm->unk_8F8_1 = 1;
    comm->unk_8F8_2 = 0;
}

static BOOL func_ov011_021748cc(IntrudeComm *work, u8 netId) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3f8(func_020399a8(), netId, INTRUDE_CMD_BASE + INTRUDE_CMD_SYMBOL_MAP, sizeof(SymbolMapList),
                         &work->unk_880, 0, FALSE, FALSE);
}

static void func_ov011_02174908(int netId, int size, const void *data, void *work, NetHandle *handle) {
    IntrudeComm *comm = work;
    GameData *gameData = getBasePlayerBlk(comm->commSys);

    if (netId != func_0203d1a4(func_020399a8()) && comm->unk_070.unk_16_0 == netId &&
        GetZoneIsEntreeForest(PlayerState_GetZoneID(GameData_GetPlayerState(gameData)))) {
        comm->unk_8F8_2 = 1;
    }
}

static BOOL func_ov011_02174954(const void *data) {
    if (func_ov011_021736d8() == FALSE) {
        return TRUE;
    }
    return func_0203d3a8(func_020399a8(), INTRUDE_CMD_BASE + INTRUDE_CMD_UNK_2A, 4, data);
}

static void func_ov011_02174ae0(IntrudeComm *work);
static void func_ov011_02174b2c(IntrudeComm *work);
static void func_ov011_02174b7c(IntrudeComm *work);
static void func_ov011_02174bc4(IntrudeComm *work);
static void func_ov011_02174c38(IntrudeComm *work);
static void func_ov011_02174ccc(IntrudeComm *work);
static void func_ov011_02174d5c(IntrudeComm *work, const IntrudeUnk15C *req);
static void func_ov011_02174df4(IntrudeComm *work);
void func_ov011_02174e14(IntrudeComm *work, u32 value);
// The trainer views another player may show
static const u16 data_ov011_02176a90[4] = { 0x0a, 0x0b, 0x0c, 0x0d };
static const u16 data_ov011_02176a98[4] = { 0x0e, 0x0f, 0x10, 0x11 };

const IntrudeZonePair data_ov011_02176aa0[8] = {
#include "asset/intrude_comm_data_ov011_02176aa0.inc"
};

// The zones of the Entralink, which may be the zone of a pass power
static const u16 data_ov011_02176ad0[0x1c] = {
#include "asset/intrude_comm_data_ov011_02176ad0.inc"
};

// The trainer views a player may take on
static const u16 data_ov011_02176b08[0xa2] = {
#include "asset/intrude_comm_data_ov011_02176b08.inc"
};

static const IntrudeProbability data_ov011_02176c4c[0xe0] = {
#include "asset/intrude_comm_data_ov011_02176c4c.inc"
};

static BOOL func_ov011_02175144(u16 zoneId, IntrudeZoneLink *link);
static u32 func_ov011_02175184(const IntrudeZoneLink *a, const IntrudeZoneLink *b);
static s32 func_ov011_021751a8(IntrudeComm *work, s32 netId);
static void func_ov011_021751d4(IntrudeComm *work, u16 zoneId, s32 x, IntrudeCommUnk8C *status);
static BOOL func_ov011_021752dc(IntrudeComm *work);
static void func_ov011_0217546c(IntrudeComm *work);
BOOL func_ov011_021756d0(u16 value);
static void func_ov011_021757c4(IntrudeComm *comm, IntrudeMissionWork *work);

static void func_ov011_02174978(IntrudeComm *work) {
    GameData *gameData = getBasePlayerBlk(work->commSys);
    HighLinkSave *save;
    s32 count;

    if (func_0203d378() == TRUE) {
        count = func_0203d1b4();
        if (count > work->unk_902) {
            work->unk_902 = count;
            work->unk_903 = 1;
            GameData_SetLastSubscreen(gameData, work->unk_902);
        }
    }
    if (work->unk_903 == 1 && func_ov011_02173758(work, work->unk_902) == TRUE) {
        work->unk_903 = 0;
    }
    func_ov011_02174ae0(work);
    func_ov011_0217546c(work);
    if (work->unk_8FE == 1) {
        func_ov011_02174e3c(work);
        func_ov011_02173828(work);
    }
    func_ov011_02174b2c(work);
    func_ov011_02174b7c(work);
    func_ov011_02174bc4(work);
    if (work->unk_93E == 1 && func_ov011_021743b4(work) == TRUE) {
        work->unk_93E = 0;
    }
    func_ov011_021757c4(work, &work->unk_1AC);
    if (work->unk_907 == 1 && func_ov011_02174374(work) == TRUE) {
        work->unk_907 = 0;
    }
    func_ov011_02174c38(work);
    func_ov011_02174ccc(work);
    func_ov011_02174df4(work);
    if (func_ov011_021752dc(work) == TRUE) {
        work->unk_900 = 1;
    }
    if (work->unk_900 == 1 && !work->unk_904_1 && func_ov011_02173984(work, &work->unk_070) == TRUE) {
        work->unk_900 = 0;
    }
    if (!GameData_CheckEventsPaused(gameData) && work->unk_8F8_0 == 1) {
        save = getHighLinkBlockAddress(GameData_GetSaveControl(gameData));
        if (!func_ov011_021756d0(work->unk_8E4.unk_10)) {
            work->unk_8E4.unk_10 = 0x11;
        }
        func_0200be84(save, &work->unk_8E4);
        work->unk_8F8_0 = 0;
    }
}

static void func_ov011_02174ae0(IntrudeComm *work) {
    GameData *gameData = getBasePlayerBlk(work->commSys);

    switch (FieldStatus_GetBusyFlag(GameData_GetFieldStatus(gameData))) {
    case 0:
        if (work->unk_070.unk_16_9 == 2) {
            func_ov011_02174e14(work, 0);
        }
        break;
    case 1:
        if (work->unk_070.unk_16_9 == 0) {
            func_ov011_02174e14(work, 2);
        }
        break;
    }
}

static void func_ov011_02174b2c(IntrudeComm *work) {
    s32 i;

    if (work->unk_8FF != 0) {
        work->unk_8FF--;
        return;
    }
    for (i = 0; i < work->unk_902; i++) {
        if (func_0203d1c0(i) == TRUE && !(work->unk_8FB & (1 << i))) {
            func_ov011_021737a8();
            work->unk_8FF = 30;
            return;
        }
    }
}

static void func_ov011_02174b7c(IntrudeComm *work) {
    s32 i;

    if (func_0203d378()) {
        for (i = 0; i < work->unk_902; i++) {
            if (!func_0203d1c0(i) && (work->unk_8FB & (1 << i))) {
                func_ov011_021738e4(work, i);
            }
        }
    }
}

static void func_ov011_02174bc4(IntrudeComm *work) {
    s32 i;

    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        if (work->unk_945[i] != 0) {
            if (func_0203d1c0(i) == TRUE) {
                if (func_ov011_02173ae0(work, i, 2, work->unk_945[i]) == TRUE) {
                    work->unk_945[i] = 0;
                }
            } else {
                work->unk_945[i] = 0;
            }
        }
    }
    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        if (work->unk_948[i] != 0) {
            if (func_0203d1c0(i) == TRUE) {
                if (func_ov011_02173b44(work, i, work->unk_948[i]) == TRUE) {
                    work->unk_948[i] = 0;
                }
            } else {
                work->unk_948[i] = 0;
            }
        }
    }
}

static void func_ov011_02174c38(IntrudeComm *work) {
    GameData *gameData = getBasePlayerBlk(work->commSys);
    CityCommMessage reply;
    CityCommMessage message;
    u8 index;

    if (work->unk_908 != 0 && func_ov011_02174734(work, work->unk_908, func_020127c4(gameData)) == TRUE) {
        work->unk_908 = 0;
    }
    for (index = 0; index < INTRUDE_PLAYER_MAX; index++) {
        if (func_ov010_02162024(&work->unk_35C, index, &reply) == TRUE && func_ov011_021747a8(&reply, index) == TRUE) {
            func_ov010_02162038(&work->unk_35C, index);
        }
    }
    if (func_ov010_0216202c(&work->unk_35C, &message, &index) == TRUE && func_ov011_021747f4(&message, index) == TRUE) {
        func_ov010_02162040(&work->unk_35C);
    }
}

static void func_ov011_02174ccc(IntrudeComm *work) {
    GameData *gameData = getBasePlayerBlk(work->commSys);
    u8 i;

    if (work->unk_908 != 0 && func_ov011_02174734(work, work->unk_908, func_020127c4(gameData)) == TRUE) {
        work->unk_908 = 0;
    }
    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        if (work->unk_8D4[i].unk_01 == 1) {
            func_ov011_02174d5c(work, &work->unk_8D4[i]);
            if (func_ov011_021748cc(work, i) == TRUE) {
                work->unk_8D4[i].unk_01 = 0;
            }
        }
    }
    if (work->unk_8E1 == 1 && func_ov011_02174954(&work->unk_8E0) == TRUE) {
        work->unk_8E1 = 0;
    }
}

static void func_ov011_02174d5c(IntrudeComm *work, const IntrudeUnk15C *req) {
    SymbolMapList *list = &work->unk_880;
    GameData *gameData = getBasePlayerBlk(work->commSys);
    AreaNPCSave *npcData = getAreaNPCData(GameData_GetSaveControl(gameData));
    u8 count;

    func_ov010_0216749c(npcData, list->pokemon, 20, req->unk_00, &count);
    list->count = count;
    list->unk50_6 = func_ov010_021673c4(npcData);
    list->unk50_10 = func_ov010_021673a8(npcData);
    list->unk50_14 = func_02039338();
    list->unk50_18 = req->unk_00;
}

static void func_ov011_02174df4(IntrudeComm *work) {
    if (work->unk_924 != 0 && func_ov011_02173cb0(work, work->unk_924) == TRUE) {
        work->unk_924 = 0;
    }
}

void func_ov011_02174e14(IntrudeComm *work, u32 value) {
    work->unk_070.unk_16_9 = value;
    work->unk_900 = 1;
}

static void func_ov011_02174e3c(IntrudeComm *work) {
    GameData *gameData = getBasePlayerBlk(work->commSys);
    PlayerInfo *info = GetGameDataPlayerInfo(gameData);

    MI_CpuCopy8(info, &work->unk_160.info, func_02008468());
    MI_CpuCopy8(func_020127b8(gameData), &work->unk_160.power, sizeof(PassPowerLevel));
    func_ov011_021752dc(work);
    if (work->unk_070.unk_16_0 == 0x80) {
        func_ov011_02175710(work, func_02039338());
    }
    work->unk_160.status = work->unk_070;
}

static void func_ov011_02174eac(IntrudeComm *work, u32 netId, IntrudePlayerData *data) {
    GameData *gameData = getBasePlayerBlk(work->commSys);
    PlayerInfo *info;
    BOOL changed = FALSE;

    info = func_02012950(gameData, netId);
    if (!(work->unk_8FB & (1 << netId)) || getIDAsUInt(info) != getIDAsUInt(&data->info)) {
        changed = TRUE;
    }
    func_02008494(&data->info, info);
    MI_CpuCopy8(&data->power, func_020127ac(gameData, netId), sizeof(PassPowerLevel));
    func_02012ba4(gameData, netId, data->status.unk_1A_6);
    func_ov011_02174f54(work, netId, &data->status, TRUE);
    work->unk_8FB |= 1 << netId;
    if (changed == TRUE && netId != func_02039338()) {
        func_020266d4(work->commSys, netId);
    }
}

static void func_ov011_02174f54(IntrudeComm *work, u32 netId, const IntrudeCommUnk8C *status, BOOL a3) {
    IntrudeCommUnk8C *dest = &work->unk_08C[netId];
    u32 unk0 = dest->status.unk0;

    MI_CpuCopy8(status, dest, sizeof(IntrudeCommUnk8C));
    dest->status.unk0 = unk0;
    dest->unk_1B_7 = status->status.unk0;
    if (status->status.unk0 == 1) {
        dest->status.unk0 = 1;
    }
    func_020264b4(work->commSys, netId, dest->zoneId, dest->unk_16_0, dest->unk_1B_0, a3);
    if (netId != func_02039338()) {
        work->unk_0E0[netId] = dest->status;
        func_ov011_021751d4(work, work->unk_070.zoneId, work->unk_070.status.pos.x, dest);
    }
    work->unk_927 |= 1 << netId;
}

void func_ov011_02175004(IntrudeComm *work) {
    GameData *gameData = getBasePlayerBlk(work->commSys);
    u8 i;
    u32 bit;

    if (GameData_CheckEventsPaused(gameData) == TRUE) {
        return;
    }
    if (work->unk_904_1 == 1) {
        return;
    }
    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        bit = 1 << i;
        if (work->unk_927 & bit) {
            func_ov011_02175060(work, i);
            work->unk_927 ^= bit;
        }
    }
}

void func_ov011_02175060(IntrudeComm *work, u32 netId) {
    IntrudeCommUnk8C *status = &work->unk_08C[netId];
    IntrudeCommUnk8C *own;
    u16 view;
    u32 hidden;

    if (netId != func_02039338()) {
        own = &work->unk_070;
        if (status->unk_1B_7 == 1) {
            hidden = 1;
        } else if (status->status.exState != 0 && status->status.exState != 1) {
            hidden = 1;
        } else if (IsZoneEntralinkHub(status->zoneId) == TRUE && IsZoneEntralinkHub(own->zoneId) == TRUE) {
            hidden = 0;
        } else if (!(status->unk_16_0 != own->unk_16_0)) {
            if (GetZoneIsEntreeForest(status->zoneId) == TRUE && GetZoneIsEntreeForest(own->zoneId) == TRUE) {
                if (!(status->unk_16_3 != own->unk_16_3)) {
                    hidden = 0;
                } else {
                    hidden = 1;
                }
            } else if (func_02013e90(own->zoneId, status->zoneId) == TRUE) {
                hidden = 0;
            } else {
                hidden = 1;
            }
        } else {
            hidden = 1;
        }
        status->status.unk0 = hidden;
    }
    if (func_ov010_021677e4(work->commPlayer, netId) == TRUE) {
        view = func_ov011_02175598(status, func_ov010_021583ec(getBasePlayerBlk(work->commSys), netId));
        if (view != func_ov010_02167810(work->commPlayer, netId)) {
            func_ov010_02167658(work->commPlayer, netId);
            if (status->unk_18 != 0) {
                func_ov010_021675d0(work->commPlayer, netId, view, &status->status);
            }
        }
    }
}

static BOOL func_ov011_02175144(u16 zoneId, IntrudeZoneLink *link) {
    s32 i;
    u8 side;

    link->zoneId = zoneId;
    for (i = 0; i < 8; i++) {
        if (zoneId == data_ov011_02176aa0[i].zoneA) {
            side = 0;
        } else if (zoneId == data_ov011_02176aa0[i].zoneB) {
            side = 1;
        } else {
            continue;
        }
        link->side = side;
        link->index = i;
        return TRUE;
    }
    link->index = 0xff;
    return FALSE;
}

static u32 func_ov011_02175184(const IntrudeZoneLink *a, const IntrudeZoneLink *b) {
    if (a->zoneId == b->zoneId) {
        return 0;
    }
    if (a->side != b->side && a->index == b->index) {
        return 1;
    }
    return 2;
}

static s32 func_ov011_021751a8(IntrudeComm *work, s32 netId) {
    s32 order;

    if (netId >= work->unk_902) {
        return 0;
    }
    order = netId - func_02012b58(getBasePlayerBlk(work->commSys));
    if (order < 0) {
        order += work->unk_902;
    }
    return order;
}

static void func_ov011_021751d4(IntrudeComm *work, u16 zoneId, s32 x, IntrudeCommUnk8C *status) {
    IntrudeZoneLink own;
    IntrudeZoneLink other;
    s32 wrapped;
    s32 order;

    func_ov011_02175144(zoneId, &own);
    func_ov011_02175144(status->zoneId, &other);
    switch (func_ov011_02175184(&own, &other)) {
    case 0:
        if (IsZoneEntralinkHub(zoneId)) {
            wrapped = status->status.pos.x % 0x400000;
            order = func_ov011_021751a8(work, status->unk_16_0);
            if (order == work->unk_902 - 1 && wrapped > 0x200000 && x < 0x200000) {
                status->status.pos.x = wrapped - 0x400000;
                return;
            }
            if (order == 0 && wrapped < 0x200000 && x > (work->unk_902 << 22) - 0x200000) {
                status->status.pos.x = wrapped + (work->unk_902 << 22);
                return;
            }
            status->status.pos.x = wrapped + (order << 22);
        }
        break;
    case 1:
        break;
    case 2:
        break;
    }
}

void func_ov011_02175274(IntrudeComm *work, s32 x) {
    GameData *gameData = getBasePlayerBlk(work->commSys);
    IntrudeCommUnk8C *others = work->unk_08C;
    u32 netId = func_02012b58(gameData);
    IntrudeCommUnk8C *own = &work->unk_070;
    IntrudeCommUnk8C *status;
    s32 i;

    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        others = work->unk_08C;
        if ((work->unk_8FB & (1 << i)) && i != netId) {
            status = &others[i];
            status->status.pos = work->unk_0E0[i].pos;
            func_ov011_021751d4(work, own->zoneId, x, status);
        }
    }
}

static BOOL func_ov011_021752dc(IntrudeComm *work) {
    GameData *gameData;
    BOOL result;
    u16 zoneID;
    IntrudeCommUnk8C *own;
    BOOL folded;

    gameData = getBasePlayerBlk(work->commSys);
    zoneID = PlayerState_GetZoneID(GameData_GetPlayerState(gameData));
    own = &work->unk_070;
    result = func_ov010_0216786c(work->commPlayer, &own->status);
    if (own->zoneId != zoneID) {
        own->zoneId = zoneID;
        result = TRUE;
    }
    if (own->unk_16_3 != GameData_Is30FPSMode(gameData)) {
        own->unk_16_3 = GameData_Is30FPSMode(gameData);
        result = TRUE;
    }
    folded = PAD_DetectFold();
    if (own->unk_16_15 != folded) {
        result = TRUE;
        own->unk_16_15 = folded;
    }
    return result;
}

void func_ov011_021753b8(IntrudeComm *work);
BOOL func_ov011_021754b4(IntrudeComm *work, u32 *netId);
static void func_ov011_0217544c(IntrudeComm *work);
void func_ov011_0217545c(IntrudeComm *work);
static BOOL func_ov011_021756b0(u16 view);
static void func_ov011_02175ee8(IntrudePlayer *player);

void func_ov011_02175368(IntrudeComm *work) {
    MI_CpuFill8(&work->unk_15C, 0, sizeof(IntrudeUnk15C));
}

IntrudeUnk15C *func_ov011_0217537c(IntrudeComm *work) {
    if (work->unk_15C.unk_00 == 1) {
        return &work->unk_15C;
    }
    return NULL;
}

void func_ov011_0217538c(IntrudeComm *work, u32 netId) {
    MI_CpuFill8(&work->unk_154, 0, sizeof(IntrudeEventReq));
    work->unk_154.netId = netId;
    work->unk_154.unk_01 = INTRUDE_NET_ID_NONE;
    work->unk_154.unk_04 = 0;
    func_ov011_021753b8(work);
}

void func_ov011_021753b8(IntrudeComm *work) {
    work->unk_944++;
    work->unk_154.unk_05 = work->unk_944;
}

static BOOL func_ov011_021753d0(IntrudeComm *work, u32 netId, u8 kind) {
    if (!(work->unk_8FB & (1 << netId))) {
        work->unk_945[netId] = kind;
        return FALSE;
    }
    if (work->unk_070.unk_16_9 == 0) {
        if (work->unk_154.netId == INTRUDE_NET_ID_NONE && work->unk_154.unk_04 == 0 &&
            !GSYS_GetEventRunningFlag(work->gsys)) {
            work->unk_154.netId = netId;
            work->unk_154.unk_02 = 1;
            work->unk_154.unk_04 = kind;
            work->unk_154.unk_05 = kind;
            func_ov011_0217544c(work);
            return TRUE;
        }
        work->unk_945[netId] = kind;
    } else {
        work->unk_945[netId] = kind;
    }
    return FALSE;
}

static void func_ov011_0217544c(IntrudeComm *work) {
    work->unk_8F8_4 = 1;
}

void func_ov011_0217545c(IntrudeComm *work) {
    work->unk_8F8_4 = 0;
}

static void func_ov011_0217546c(IntrudeComm *work) {
    u32 netId;

    if (func_ov011_021754b4(work, &netId) == TRUE && GSYS_GetEventRunningFlag(work->gsys) == TRUE) {
        if (work->unk_945[work->unk_154.netId] == 0) {
            work->unk_945[work->unk_154.netId] = work->unk_154.unk_04;
        }
        func_ov011_0217538c(work, INTRUDE_NET_ID_NONE);
        func_ov011_0217545c(work);
    }
}

BOOL func_ov011_021754b4(IntrudeComm *work, u32 *netId) {
    if (work == NULL || work->commPlayer == NULL) {
        return FALSE;
    }
    if (work->unk_154.netId != INTRUDE_NET_ID_NONE && work->unk_8F8_4 == 1) {
        *netId = work->unk_154.netId;
        return TRUE;
    }
    return FALSE;
}

static void func_ov011_021754e8(IntrudeComm *work, u32 netId, u8 answer, u8 kind) {
    if (work->unk_154.netId == netId && work->unk_154.unk_04 == kind) {
        work->unk_154.unk_01 = netId;
        work->unk_154.unk_03 = answer;
    }
}

static void func_ov011_02175508(IntrudeComm *work, u32 netId, u8 kind) {
    func_ov011_021754e8(work, netId, 3, kind);
}

u32 func_ov011_02175514(IntrudeComm *work) {
    u8 answered;

    if (work->unk_154.unk_01 != INTRUDE_NET_ID_NONE && work->unk_154.unk_03 != 0) {
        answered = work->unk_154.unk_03;
        work->unk_154.unk_03 = 0;
        return answered;
    }
    return 0;
}

void func_ov011_0217553c(IntrudeComm *work) {
    work->unk_154.unk_03 = 0;
}

void func_ov011_02175548(PlayerInfo *info, u16 *view, u8 *a, u8 *b) {
    u32 id = getIDAsUInt(info);
    u32 gender = getTrainerGender(info);
    u32 index = ((id >> 16) + (id & 0xffff)) & 3;

    if (gender == 0) {
        *view = data_ov011_02176a90[index];
        *a = 3;
        *b = 0;
    } else {
        *view = data_ov011_02176a98[index];
        *a = 4;
        *b = 1;
    }
}

u16 func_ov011_02175598(const IntrudeCommUnk8C *status, PlayerInfo *info) {
    u16 view;
    u8 a;
    u8 b;

    if (status->unk_18 == 0) {
        if (status->status.exState == 1) {
            view = getTrainerGender(info) == 0 ? 2 : 5;
        } else {
            view = getTrainerGender(info) == 0 ? 1 : 4;
        }
    } else if (status->unk_18 == 0xffff) {
        func_ov011_02175548(info, &view, &a, &b);
    } else {
        view = status->unk_18;
        if (!func_ov011_021756b0(view)) {
            view = getTrainerGender(info) == 0 ? 10 : 14;
        }
    }
    if ((view >= 0xdf && view < 0x1000) || view >= 0x1236) {
        view = 10;
    }
    return view;
}

static BOOL func_ov011_02175628(void) {
    if (func_0200fb24()) {
        return FALSE;
    }
    if (func_0203d1b4() > 1) {
        return TRUE;
    }
    return FALSE;
}

void func_ov011_02175648(GameData *gameData, IntrudeHighLinkData *data) {
    HighLinkSave *save = getHighLinkBlockAddress(GameData_GetSaveControl(gameData));
    u64 value;

    MI_CpuFill8(data, 0, sizeof(IntrudeHighLinkData));
    data->unk_00 = func_0200bf70(save);
    if (GameData_IsForceSeasonSync(gameData) == TRUE) {
        value = func_0200bfa4(save, func_02012fcc(gameData), 2);
    } else {
        value = func_0200bf94(save);
    }
    data->unk_08 = value >> 32;
    data->unk_04 = value;
    func_0200bf54(save, data->unk_0C, 2);
    data->unk_0E = 1;
}

u8 func_ov011_021756a4(IntrudeComm *work) {
    return work->unk_358[2];
}

static BOOL func_ov011_021756b0(u16 view) {
    u32 i;
    for (i = 0; i < NELEMS(data_ov011_02176b08) ? TRUE : FALSE; i++) {
        if (view == data_ov011_02176b08[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov011_021756d0(u16 value) {
    u32 i;
    for (i = 0; i < NELEMS(data_ov011_02176ad0) ? TRUE : FALSE; i++) {
        if (value == data_ov011_02176ad0[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

void func_ov011_021756f0(IntrudeComm *work, BOOL flag) {
    work->unk_93F_6 = flag;
}

void func_ov011_02175710(IntrudeComm *work, u32 netId) {
    GameData *gameData = GSYS_GetGameData(work->gsys);

    work->unk_070.unk_16_0 = netId;
    func_02012b8c(gameData, netId);
    func_ov010_02161fec(&work->unk_35C, netId);
}

void func_ov011_0217574c(IntrudeComm *work, BOOL flag) {
    work->unk_904_1 = flag;
}

static void func_ov011_0217576c(IntrudeMissionWork *work) {
    s32 i;

    MI_CpuFill8(work, 0, sizeof(IntrudeMissionWork));
    work->entry.netId = INTRUDE_NET_ID_NONE;
    func_ov011_02175ee8(&work->entry.player);
    work->unk_0AC.entry.netId = INTRUDE_NET_ID_NONE;
    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        work->unk_0AC.netIds[i] = INTRUDE_NET_ID_NONE;
    }
    func_ov011_021757a8(work);
}

void func_ov011_021757a8(IntrudeMissionWork *work) {
    s32 i;

    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        work->players[i].unk_10 = 0;
    }
}

static s32 func_ov011_02175894(IntrudeMissionWork *work);
static s32 func_ov011_021758b4(IntrudeMissionWork *work);
static void func_ov011_02175a34(IntrudeComm *comm, IntrudeMissionWork *work);
static void func_ov011_02175ae8(IntrudeComm *comm, IntrudeMissionWork *work);
static void func_ov011_02175d34(IntrudeComm *comm, IntrudeMissionWork *work, u32 netId);
static void func_ov011_02176158(IntrudeComm *comm, IntrudeMissionWork *work);
static void func_ov011_02176194(IntrudeComm *comm, IntrudeMissionWork *work);

static void func_ov011_021757c4(IntrudeComm *comm, IntrudeMissionWork *work) {
    GameData *gameData;

    if (func_0203d378() == TRUE && func_ov011_02175b14(work) == TRUE) {
        if (work->unk_0AC.entry.netId == INTRUDE_NET_ID_NONE) {
            if (work->entry.unk_30 != 0) {
                if (work->unk_19C == 0) {
                    if (func_ov011_02175894(work) > work->entry.unk_30) {
                        work->unk_19C = 1;
                    }
                }
            } else if (work->unk_1A5_1 == 1) {
                if (func_ov011_02175894(work) > work->entry.unk_32) {
                    gameData = getBasePlayerBlk(comm->commSys);
                    if (work->unk_0AC.unk_37 == 0) {
                        func_ov011_02175d34(comm, work, func_02012b58(gameData));
                    }
                }
            }
        } else if (work->unk_1A5_1 == 1) {
            if (func_ov011_02175894(work) > work->entry.unk_32) {
                work->unk_1A5_4 = 1;
            }
        }
    }
    func_ov011_02176158(comm, work);
    func_ov011_02176194(comm, work);
    func_ov011_02175a34(comm, work);
    func_ov011_02175ae8(comm, work);
}

static s32 func_ov011_02175894(IntrudeMissionWork *work) {
    s32 now = RTC_ConvertDaySecondsCached();

    if (now < work->unk_190) {
        now += 24 * 60 * 60;
    }
    return now - work->unk_190;
}

static s32 func_ov011_021758b4(IntrudeMissionWork *work) {
    u16 limit = work->entry.unk_32;
    s32 left = limit - func_ov011_02175894(work);

    if (left < 0) {
        left = 0;
    }
    return left;
}

s32 func_ov011_021758c8(IntrudeMissionWork *work) {
    s32 left;
    u16 limit;

    if (func_ov011_02175b14(work) == TRUE && work->unk_0AC.entry.netId == INTRUDE_NET_ID_NONE) {
        limit = work->entry.unk_30;
        if (limit != 0 || !work->unk_1A5_1) {
            left = limit - func_ov011_02175894(work);
        } else {
            left = func_ov011_021758b4(work);
        }
        if (left < 0) {
            return 0;
        }
        return left;
    }
    return 0;
}

static void func_ov011_02175914(IntrudeMissionWork *work, u32 index) {
    work->unk_194[index] = 1;
}

static void func_ov011_02175920(IntrudeMissionWork *work) {
    work->unk_19A = 1;
}

static void func_ov011_0217592c(IntrudeMissionWork *work) {
    work->unk_19B = 1;
}

static void func_ov011_02175938(IntrudeMissionWork *work, const IntrudeCommPlayer *player, u32 index) {
    if (work->players[index].unk_10 != 1) {
        work->players[index] = *player;
    }
}

static BOOL func_ov011_02175954(IntrudeComm *comm, IntrudeMissionWork *work, const IntrudeMissionEntry *entry) {
    IntrudeMissionEntry *dest = &work->entry;
    BOOL ret = FALSE;

    if (entry->mission.unk_00_15 > 0x14 || entry->mission.unk_00_22 > 0xb4) {
        return FALSE;
    }
    if (work->unk_1A1 == 0 && entry->netId != INTRUDE_NET_ID_NONE) {
        work->unk_1A1 = TRUE;
        ret = TRUE;
    }
    if (dest->netId != INTRUDE_NET_ID_NONE) {
        return ret;
    }
    *dest = *entry;
    return ret;
}

static void func_ov011_021759b0(IntrudeMissionWork *work) {
    IntrudeMissionEntry *entry = func_ov011_02175b34(work);

    if (entry != NULL) {
        entry->unk_30 = 0;
    }
}

static void func_ov011_021759c0(IntrudeMissionWork *work, u8 netId) {
    int bit;

    if (work->unk_19D == 0) {
        work->unk_190 = RTC_ConvertDaySecondsCached();
    }
    bit = 1 << netId;
    work->unk_19D |= bit;
    work->unk_19E |= bit;
}

static void func_ov011_021759f8(IntrudeMissionWork *work, u32 time) {
    work->unk_1A5_1 = 1;
    work->entry.unk_32 = time;
    if (!func_0203d378()) {
        work->unk_190 = RTC_ConvertDaySecondsCached();
    }
}

BOOL func_ov011_02175a24(IntrudeMissionWork *work) {
    return work->unk_1A5_1;
}

static void func_ov011_02175a34(IntrudeComm *comm, IntrudeMissionWork *work) {
    s32 i;

    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        if (work->unk_194[i] == 1 && func_ov011_02173da0(comm, work->players, i) == TRUE) {
            work->unk_194[i] = 0;
        }
    }
    if (work->unk_19A == 1 && func_ov011_02173eec(comm, work) == TRUE) {
        work->unk_19A = 0;
    }
    if (work->unk_19B == 1 && func_ov011_021741e8(comm, work) == TRUE) {
        work->unk_19B = 0;
    }
    if (work->unk_19C == 1 && func_ov011_02173f74(comm, work) == TRUE) {
        work->unk_19C = 2;
    }
    if (work->unk_19F != 0 && func_ov011_021742ec(work->unk_19F) == TRUE) {
        work->unk_19F = 0;
    }
    if (work->unk_1A0 != 0 && func_ov011_021742a8(work->unk_1A0) == TRUE) {
        work->unk_1A0 = 0;
    }
}

static void func_ov011_02175ae8(IntrudeComm *comm, IntrudeMissionWork *work) {
    u8 netId = work->unk_19E;

    if (netId != 0 && func_ov011_02173ff4(comm, func_ov011_021758b4(work), netId) == TRUE) {
        work->unk_19E = 0;
    }
}

BOOL func_ov011_02175b14(IntrudeMissionWork *work) {
    if (work->entry.netId == INTRUDE_NET_ID_NONE || work->unk_1A1 == 0) {
        return FALSE;
    }
    return TRUE;
}

IntrudeMissionEntry *func_ov011_02175b34(IntrudeMissionWork *work) {
    IntrudeMissionEntry *entry;

    if (func_ov011_02175b14(work) == FALSE) {
        entry = NULL;
    } else {
        entry = &work->entry;
    }
    return entry;
}

void func_ov011_02175b4c(IntrudeComm *comm, IntrudeMissionWork *work) {
    if (func_ov011_02175b14(work) == TRUE) {
        work->unk_1A5_2 = 1;
        work->unk_1A5_3 = 0;
        comm->unk_070.unk_1A_5 = 1;
        comm->unk_900 = 1;
    }
}

BOOL func_ov011_02175b8c(IntrudeMissionWork *work) {
    if (func_ov011_02175b14(work) == TRUE) {
        return work->unk_1A5_2;
    }
    return FALSE;
}

void func_ov011_02175bac(IntrudeMissionWork *work) {
    work->unk_1A5_3 = 1;
}

BOOL func_ov011_02175bbc(IntrudeMissionWork *work) {
    return work->unk_1A5_3;
}

IntrudeMissionEntrust *func_ov011_02175bcc(IntrudeMissionWork *work) {
    IntrudeMissionEntrust *entrust;

    if (func_ov011_02175da8(work) == FALSE) {
        entrust = NULL;
    } else {
        entrust = &work->unk_0AC;
    }
    return entrust;
}

BOOL func_ov011_02175be4(IntrudeMissionWork *work) {
    if (func_ov011_02175b8c(work) == TRUE || func_ov011_02175bcc(work) != NULL) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov011_02175c04(IntrudeMissionWork *work, u32 netId) {
    if (work->unk_0AC.netIds[0] == INTRUDE_NET_ID_NONE) {
        work->unk_19F |= 1 << netId;
    } else {
        work->unk_1A0 |= 1 << netId;
    }
}

u32 func_ov011_02175c40(IntrudeMissionWork *work) {
    u32 value = work->unk_1A5_5;

    func_ov011_02175c58(work, 0);
    return value;
}

static void func_ov011_02175c58(IntrudeMissionWork *work, u32 value) {
    work->unk_1A5_5 = value;
}

static u32 func_ov011_02175c78(IntrudeMissionWork *work, const IntrudeMissionEntry *entry, u32 netId) {
    if (work->unk_0AC.netIds[0] == INTRUDE_NET_ID_NONE) {
        return 0;
    }
    return 3;
}

static BOOL func_ov011_02175c88(IntrudeMissionWork *work, const IntrudeMissionEntry *entry, u32 netId) {
    IntrudeMissionEntrust *entrust = &work->unk_0AC;
    u32 index;

    if (GFL_STD_MemCmp(&work->entry, entry, 0x30) != 0) {
        work->unk_197[netId] = 2;
        return FALSE;
    }
    index = func_ov011_02175c78(work, entry, netId);
    if (index == 3) {
        work->unk_197[netId] = 2;
        return FALSE;
    }
    entrust->entry = *entry;
    entrust->netIds[index] = netId;
    work->unk_197[netId] = 1;
    work->unk_19B = 1;
    return TRUE;
}

BOOL func_ov011_02175cf4(IntrudeComm *comm, IntrudeMissionWork *work) {
    u8 result = work->unk_1A4;

    if (result == 1) {
        if (func_ov011_02175da8(work) == TRUE && func_ov011_021756a4(comm) == 1) {
            return TRUE;
        }
        return FALSE;
    }
    return result;
}

void func_ov011_02175d20(IntrudeMissionWork *work) {
    work->unk_1A4 = 0;
}

static void func_ov011_02175d2c(IntrudeMissionWork *work, u32 result) {
    work->unk_1A4 = result;
}

static void func_ov011_02175d34(IntrudeComm *comm, IntrudeMissionWork *work, u32 netId) {
    IntrudeMissionEntry *entry = &work->entry;
    IntrudeMissionEntrust *entrust = &work->unk_0AC;

    if (entry->netId != INTRUDE_NET_ID_NONE && entrust->entry.netId == INTRUDE_NET_ID_NONE) {
        entrust->entry = *entry;
        entrust->unk_37 = 1;
        work->unk_19B = 1;
        work->unk_19C = 0;
    }
}

static void func_ov011_02175d7c(IntrudeComm *comm, IntrudeMissionWork *work, const IntrudeMissionEntrust *entrust) {
    work->unk_0AC = *entrust;
    work->unk_1A5_0 = 1;
}

BOOL func_ov011_02175da8(IntrudeMissionWork *work) {
    IntrudeMissionEntrust *entrust = &work->unk_0AC;

    if (work->unk_1A5_0 == 1 && entrust->entry.netId != INTRUDE_NET_ID_NONE) {
        return TRUE;
    }
    return FALSE;
}

u32 func_ov011_02175dcc(IntrudeMissionWork *work) {
    if (func_ov011_02175b14(work) == FALSE) {
        return 6;
    }
    return work->entry.mission.kind;
}

s32 func_ov011_02175de4(IntrudeComm *comm, IntrudeMissionWork *work) {
    GameData *gameData = getBasePlayerBlk(comm->commSys);
    IntrudeMissionEntrust *entrust = &comm->unk_1AC.unk_0AC;
    u32 count;
    s32 i;

    if (entrust->unk_37 == 1) {
        return 0;
    }
    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        if (entrust->netIds[i] == func_02012b58(gameData)) {
            count = entrust->entry.mission.unk_00_15;
            if (count > 0x14) {
                return 1;
            }
            return count;
        }
    }
    return 0;
}

static void func_ov011_02175e30(GameData *gameData, IntrudeComm *comm, const IntrudeMissionEntrust *entrust) {
    PassPowerLevel *power = func_020127b8(gameData);
    PlayerInfo *info = func_02012950(gameData, entrust->netIds[0]);
    u32 version = comm->unk_08C[entrust->netIds[0]].unk_1B_0;
    u32 kind = func_ov010_021584f4(version, getIDAsUInt(info));

    if (kind == 0x14) {
        power->unk06[entrust->entry.mission.kind] = 1;
    } else if (kind == 0x15) {
        power->unk06[entrust->entry.mission.kind] = 2;
    }
}

BOOL func_ov011_02175e90(IntrudeComm *comm, IntrudeMissionWork *work) {
    if (func_ov011_02175da8(work) == FALSE) {
        return FALSE;
    }
    if (func_ov011_02175de4(comm, work) > 0) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov011_02175eb8(IntrudeMissionWork *work) {
    IntrudeMissionEntrust *entrust = func_ov011_02175bcc(work);

    if (entrust == NULL) {
        return FALSE;
    }
    if (entrust->unk_37 == 1) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov011_02175ed8(IntrudeMissionWork *work) {
    return work->unk_1A5_4;
}

static void func_ov011_02175ee8(IntrudePlayer *player) {
    MI_CpuFill8(player, 0, 0x14);
    player->unk_10 = 4;
}

static void func_ov011_02175efc(IntrudeComm *comm, IntrudePlayer *player, u8 netId) {
    GameData *gameData;
    PlayerInfo *info;

    player->unk_10 = netId;
    gameData = getBasePlayerBlk(comm->commSys);
    info = func_ov010_021583ec(gameData, player->unk_10);
    copyName(info->name, player->name, 8);
    player->gender = getTrainerGender(info);
}

static void func_ov011_02175f34(IntrudeComm *comm, IntrudeCommPlayer *players, u32 value, u32 index) {
    GameData *gameData = getBasePlayerBlk(comm->commSys);
    IntrudeCommPlayer *player = &players[index];
    PassPowerLevel *power;
    PlayerInfo *info;
    u8 version;
    u8 flag;

    if (player->unk_10 == 1) {
        return;
    }
    power = func_ov010_021583cc(gameData, index);
    info = func_ov010_021583ec(gameData, index);
    version = func_0200855c(info);
    flag = func_ov010_021584f4(version, getIDAsUInt(info)) == 0x15;
    player->power = *power;
    player->unk_11 = flag;
    player->unk_12 = value;
    func_ov011_02175efc(comm, &player->player, index);
    player->unk_10 = 1;
}

u8 func_ov011_02175fb4(IntrudeCommPlayer *player) {
    return player->unk_10;
}

static BOOL func_ov011_02175fb8(IntrudeMissionWork *work, const IntrudeMissionEntry *entry) {
    s32 i;

    if (entry->mission.kind >= 6 || entry->mission.unk_00_15 > 0x14 || entry->mission.unk_00_22 > 0xb4 ||
        entry->mission.unk_04_7 >= 0x2e || entry->mission.msgId >= 0x53) {
        return FALSE;
    }
    for (i = 0; i < 3; i++) {
        if (entry->mission.unk_10[i] >= 5) {
            return FALSE;
        }
        if (!func_ov011_021756b0(entry->mission.unk_0A[i])) {
            return FALSE;
        }
    }
    for (i = 0; i < 2; i++) {
        if (entry->mission.kind == 3) {
            if (!func_ov011_021756d0(entry->mission.unk_14)) {
                return FALSE;
            }
        } else if (entry->mission.kind == 4) {
            if (!func_ov011_021756d0(entry->mission.unk_16)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

static BOOL func_ov011_02176048(IntrudeComm *comm, IntrudeMissionWork *work, const IntrudeMissionEntry *entry, u32 netId) {
    IntrudeMissionEntry *dest = &work->entry;

    MI_CpuFill8(&work->unk_0E8[netId], 0, sizeof(IntrudeMissionRequest));
    if (func_ov011_02175b14(&comm->unk_1AC) == TRUE) {
        work->unk_0E8[netId].unk_00 = 2;
        return FALSE;
    }
    if (!func_ov011_02175fb8(&comm->unk_1AC, entry)) {
        work->unk_0E8[netId].unk_00 = 2;
        return FALSE;
    }
    if (entry->mission.kind == 0 || entry->mission.kind == 3) {
        if (func_020392b8() != 2 || func_0203d1b4() != 2) {
            work->unk_0E8[netId].unk_00 = 4;
            return FALSE;
        }
    }
    dest->mission = entry->mission;
    dest->player = entry->player;
    dest->netId = netId;
    dest->unk_2D = entry->player.unk_10;
    dest->unk_2E = entry->netId;
    dest->unk_30 = 10;
    dest->unk_32 = entry->mission.unk_00_22;
    work->unk_0E8[netId].entry = *dest;
    work->unk_0E8[netId].unk_00 = 1;
    func_ov011_02175920(work);
    work->unk_190 = RTC_ConvertDaySecondsCached();
    work->unk_19C = 0;
    return TRUE;
}

static void func_ov011_02176158(IntrudeComm *comm, IntrudeMissionWork *work) {
    s32 i;

    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        if (work->unk_0E8[i].unk_00 != 0 && func_ov011_02173e6c(comm, &work->unk_0E8[i], i) == TRUE) {
            MI_CpuFill8(&work->unk_0E8[i], 0, sizeof(IntrudeMissionRequest));
        }
    }
}

static void func_ov011_02176194(IntrudeComm *comm, IntrudeMissionWork *work) {
    s32 i;

    for (i = 0; i < INTRUDE_PLAYER_MAX; i++) {
        if (work->unk_197[i] != 0 && func_ov011_021740ac(comm, work->unk_197[i], i) == TRUE) {
            work->unk_197[i] = 0;
        }
    }
}

static void func_ov011_021761c4(IntrudeMissionWork *work, const IntrudeMissionRequest *answer) {
    work->unk_1A3 = answer->unk_00;
    if (answer->unk_00 == 1) {
        work->entry = answer->entry;
    }
}

u8 func_ov011_021761ec(IntrudeMissionWork *work) {
    u8 answer = work->unk_1A3;

    func_ov011_02176200(work);
    return answer;
}

void func_ov011_02176200(IntrudeMissionWork *work) {
    work->unk_1A3 = 0;
}

BOOL func_ov011_0217620c(IntrudeMissionWork *work, u32 netId) {
    if (func_ov011_02175b14(work) == FALSE) {
        return FALSE;
    }
    if (work->entry.player.unk_10 == netId) {
        return TRUE;
    }
    return FALSE;
}

u32 func_ov011_02176230(IntrudeComm *work, IntrudeMissionWork *missionWork) {
    u8 netId;

    if (func_ov011_02175b14(missionWork) == FALSE) {
        netId = func_02012b58(getBasePlayerBlk(work->commSys));
    } else {
        netId = missionWork->entry.player.unk_10;
    }
    return netId;
}

BOOL func_ov011_02176258(IntrudeComm *work) {
    if (func_ov011_0217620c(&work->unk_1AC, func_02012b58(getBasePlayerBlk(work->commSys))) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov011_02176280(WordSet *wordSet, u32 index, const u16 *name, u32 length, HeapID heapId, u32 gender) {
    StrBuf *strbuf = GFL_StrBufCreate(length, heapId);

    GFL_StrBufLoadFixedString(strbuf, name, length);
    func_0201ee0c(wordSet, index, strbuf, gender, 1, 2);
    GFL_StrBufFree(strbuf);
}

void func_ov011_021762c0(WordSet *wordSet, u32 index, const IntrudePlayer *player, HeapID heapId) {
    func_ov011_02176280(wordSet, index, player->name, 8, heapId, player->gender);
}

void func_ov011_021762d8(const IntrudeMission *mission, IntrudePlayer *player, WordSet *wordSet, HeapID heapId) {
    s32 count;
    u16 *item;

    switch (mission->kind) {
    case 0:
        func_ov011_021762c0(wordSet, 0, player, heapId);
        WordSetNumber(wordSet, 1, mission->unk_14, 3, 0, TRUE);
        break;
    case 1:
        func_ov011_021762c0(wordSet, 0, player, heapId);
        loadPassPowerToStrbuf(wordSet, 1, mission->unk_14);
        break;
    case 2:
        func_ov011_021762c0(wordSet, 0, player, heapId);
        loadPassPowerToStrbuf(wordSet, 1, mission->unk_14);
        break;
    case 3:
        item = &mission->unk_14;
        func_ov011_021762c0(wordSet, 0, player, heapId);
        loadItemNameToStrbuf(wordSet, 1, item[0]);
        WordSetNumber(wordSet, 2, item[1], 5, 0, TRUE);
        break;
    case 4:
        loadItemNameToStrbuf(wordSet, 0, mission->unk_16);
        func_ov011_021762c0(wordSet, 1, player, heapId);
        break;
    case 5:
        func_ov011_021762c0(wordSet, 0, player, heapId);
        break;
    }
    WordSetNumber(wordSet, 2, mission->unk_00_22, 4, 0, TRUE);
    count = mission->unk_00_15;
    if (count > 20) {
        count = 1;
    }
    WordSetNumber(wordSet, 3, count, 4, 0, TRUE);
}

void probabilityLoop(PassPowerLevel *levels) {
    s32 i;

    for (i = 0; i < 6; i++) {
        levels->unk06[i] = 0;
        insideProbabilityLoop(levels, i);
    }
}

void insideProbabilityLoop(PassPowerLevel *levels, u32 kind) {
    int sum = levels->level1 + levels->level2;
    u32 i;
    u8 chance;

    for (i = 0; i < NELEMS(data_ov011_02176c4c); i++) {
        if (kind != data_ov011_02176c4c[i].kind) {
            continue;
        }
        if (data_ov011_02176c4c[i].minSum > sum) {
            continue;
        }
        if (!(data_ov011_02176c4c[i].flags & GAME_VERSION_FLAG)) {
            continue;
        }
        chance = data_ov011_02176c4c[i].chance;
        if (chance != 100 && chance < GFL_RandomLC(101)) {
            continue;
        }
        levels->unk00[kind] = i;
        return;
    }
}

static BOOL func_ov011_02176424(PassPowerLevel *levels, u8 *up, u8 *down) {
    s32 i;
    u8 result;

    for (i = 0; i < 6; i++) {
        result = levels->unk06[i];
        if (result == 0) {
            return FALSE;
        }
        if (result == 1) {
            (*up)++;
        } else {
            (*down)++;
        }
    }
    probabilityLoop(levels);
    return TRUE;
}

static BOOL func_ov011_02176454(GameProc *proc, u32 *seq, void *heapId, void *work) {
    IntrudeBattleParam *p = heapId;
    s32 count;
    PokeParty *party;
    s32 i;
    s32 added = 0;
    IntrudeBattleWork *battle = GFL_ProcInitSubsystem(proc, sizeof(IntrudeBattleWork), HEAPID_GAMEEVENT);

    MI_CpuFill8(battle, 0, sizeof(IntrudeBattleWork));
    battle->procMgr = CreateGameProcManager(HEAPID_GAMEEVENT);
    party = GameData_GetParty(GSYS_GetGameData(p->gsys));
    PokeParty_RecoverAll(party);
    count = PokeParty_GetPkmCount(party);
    battle->party = PokeParty_Create(HEAPID_GAMEEVENT);
    for (i = 0; i < count; i++) {
        if (added >= p->count) {
            break;
        }
        if (func_0201ab34(party, i) == TRUE) {
            PokeParty_AddPkm(battle->party, PokeParty_GetPkm(party, i));
            setLevel(PokeParty_GetPkm(battle->party, added), p->level);
            added++;
        }
    }
    GFL_ASSERT(added > 0);
    return TRUE;
}

static BOOL func_ov011_02176504(GameProc *proc, u32 *seq, void *heapId, void *work) {
    IntrudeBattleParam *p = heapId;
    IntrudeBattleWork *battle = work;
    GameData *gameData = GSYS_GetGameData(p->gsys);
    BOOL running;
    s32 step;

    GSYS_GetGameCommSystem(p->gsys);
    running = GFL_ProcMgrUpdate(battle->procMgr);
    if (func_0200fb24() || func_0203cdbc() == TRUE) {
        step = *seq;
        if (step > 0 && step < 6) {
            *seq = 7;
        }
    }
    switch (*seq) {
    case 0:
        BtlSetup_SetNet1v1Single(&battle->setup, gameData, func_020399a8(), 1, HEAPID_GAMEEVENT);
        func_02013128(&battle->setup, battle->party, 0);
        if (func_02039338() < p->netId) {
            battle->setup.fieldSituation.unk19 = 0;
        } else {
            battle->setup.fieldSituation.unk19 = 1;
        }
        battle->setup.fieldSituation.env.terrain = 0xf;
        battle->setup.fieldSituation.bgm = 0x490;
        battle->setup.unk84 = (1 << p->netId) | (1 << func_02039338());
        battle->setup.unk76 = 0x1e;
        (*seq)++;
        break;
    case 1:
        func_0203a424(func_020399a8(), 0x15, 2, (u16)((1 << p->netId) | (1 << func_02039338())));
        (*seq)++;
        break;
    case 2:
        if (func_02039cb4(func_020399a8(), 0x15, 2) == TRUE) {
            GFL_OvlLoad(OVERLAY_ID(93));
            battle->overlayLoaded = TRUE;
            func_0203a574(0x100, data_ov093_021f03c4, 9, NULL);
            (*seq)++;
        }
        break;
    case 3:
        func_0203a424(func_020399a8(), 0x16, 2, (u16)((1 << p->netId) | (1 << func_02039338())));
        (*seq)++;
        break;
    case 4:
        if (func_02039cb4(func_020399a8(), 0x16, 2) == TRUE) {
            (*seq)++;
        }
        break;
    case 5:
        QueueGameProc(battle->procMgr, -1, &data_ov093_021efdac, &battle->setup);
        (*seq)++;
        break;
    case 6:
        if (running == FALSE) {
            (*seq)++;
        }
        break;
    case 7:
        if (battle->overlayLoaded == TRUE) {
            if (func_0203cd98() == TRUE) {
                func_0203a5e4(0x100);
            }
            GFL_OvlUnload(OVERLAY_ID(93));
        }
        func_020130dc(&battle->setup);
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov011_021766a8(GameProc *proc, u32 *seq, void *heapId, void *work) {
    IntrudeBattleWork *battle = work;

    FreeGameProcManager(battle->procMgr);
    GFL_HeapFree(battle->party);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

static const GameProcFunctions data_ov011_02176fcc = {
    func_ov011_02176454,
    func_ov011_02176504,
    func_ov011_021766a8,
};

static GameEventReturnCode func_ov011_021766f0(GameEvent *event, u32 *seq, void *data);

// The position a player starts at in another player's Entralink
static const VecFx32 data_ov011_02176fd8 = { 0x1f8000, 0x20000, 0x168000 };

GameEvent *func_ov011_021766cc(GameSystem *gsys, void *heapId) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov011_021766f0, sizeof(IntrudeEventWork));
    IntrudeEventWork *work = GameEvent_GetData(event);

    work->heapId = heapId;
    work->gsys = gsys;
    return event;
}

static GameEventReturnCode func_ov011_021766f0(GameEvent *event, u32 *seq, void *data) {
    IntrudeEventWork *work = data;
    Field *field = GSYS_GetField(work->gsys);

    switch (*seq) {
    case 0:
        GameEvent_ChainNext(event, EventBattleBGMPlay_Create(work->gsys, 0x490));
        (*seq)++;
        break;
    case 1:
        EncEff_StartEvent(Field_GetEncEff(field), event, 5);
        (*seq)++;
        break;
    case 2:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(work->gsys, field));
        (*seq)++;
        break;
    case 3:
        GSYS_QueueProcAsEvent(event, -1, &data_ov011_02176fcc, work->heapId);
        (*seq)++;
        break;
    case 4:
        GameEvent_ChainNext(event, EventBGMFadePop_Create(work->gsys));
        (*seq)++;
        break;
    case 5:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(work->gsys));
        (*seq)++;
        break;
    case 6:
        GameEvent_ChainNext(event, EventBGMFadeWait_Create(work->gsys));
        (*seq)++;
        break;
    case 7:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(work->gsys, field, 0, 0, 1, 0, 0));
        (*seq)++;
        break;
    default:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

u8 func_ov011_021767a0(GameCommSys *commSys, GameData *gameData) {
    u8 netId = func_02012b58(gameData);
    IntrudeComm *work = func_ov010_02158344(commSys);
    u8 target;

    if (func_0200fb24()) {
        target = func_02012b80(gameData);
        if (target == netId) {
            return INTRUDE_NET_ID_NONE;
        }
        return target;
    }
    if (work != NULL) {
        if (work->unk_070.unk_16_0 == netId) {
            return INTRUDE_NET_ID_NONE;
        }
        return work->unk_070.unk_16_0;
    }
    return INTRUDE_NET_ID_NONE;
}

void func_ov011_021767f0(GameCommSys *commSys, GameData *gameData, VecFx32 *pos) {
    IntrudeComm *work = func_ov010_02158344(commSys);
    s32 offset;

    if (work == NULL || work->unk_070.unk_16_0 == func_02012b58(gameData)) {
        offset = 0;
    } else {
        offset = work->unk_070.unk_16_0 - func_02012b58(gameData);
        if (offset < 0) {
            offset += work->unk_902;
        }
    }
    *pos = data_ov011_02176fd8;
    pos->x += offset << 22;
}

BOOL func_ov011_02176850(IntrudeComm *work, u8 netId) {
    IntrudeUnk15C req;

    req.unk_00 = netId;
    req.unk_01 = 1;
    return func_ov011_02174854(work, work->unk_070.unk_16_0, &req);
}

BOOL func_ov011_02176870(IntrudeComm *work) {
    return work->unk_8F8_1;
}

SymbolMapList *func_ov011_02176880(IntrudeComm *work) {
    return &work->unk_82C;
}

void func_ov011_0217688c(IntrudeComm *work, u8 value) {
    work->unk_8E0 = value;
    work->unk_8E1 = 1;
}

BOOL func_ov011_0217689c(IntrudeComm *work, u16 zoneId) {
    BOOL flag = work->unk_8F8_2;

    work->unk_8F8_2 = 0;
    if (flag == 1 && GetZoneIsEntreeForest(zoneId) == TRUE) {
        return TRUE;
    }
    return FALSE;
}
