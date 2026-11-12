// Black's code at ov018 0x02180980-0x02182988. No name is known: the module and address.

#include "types.h"
#include "field/intrude_comm.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/wordset.h"
#include "field/field_actor.h"
#include "field/field.h"
#include "field/ov018_0217da54.h"
#include "field/event_comm_common.h"
#include "field/field_script_event.h"
#include "field/field_script.h"
#include "field/field_pass_power.h"
#include "field/ov018_0217c458.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/key.h"
#include "field/intrude_work.h"
#include "system/game_comm.h"
#include "field/survey.h"
#include "save/save_control.h"
#include "save/bag.h"
#include "system/comm_player_support.h"

// Black's layouts of the work areas these functions pass around, from the assembly. Every event's work starts with
// event_comm_common.h's EventCommCommonWork (0x98 bytes), what event_comm_common.c's func_ov018_0217fda0 fills in and
// the functions that start an event copy into their own work: the heap, the player's and the other player's actors,
// the field, the message windows, the other player's net ID, the mission entry and the other player
// The work of func_ov018_021821b0's event (0x54 bytes)
typedef struct Ov018Event {
    Ov018MsgWork msg;
    u16 name[8];
    u16 unk_40;
    u8 unk_42;
    u8 unk_43;
    u16 unk_44;
    u8 unk_46[2];
    u32 unk_48;
    u32 unk_4c;
    u32 unk_50;
} Ov018Event;

typedef struct Ov018EventWork {
    EventCommCommonWork common;
    u32 unk_98;
    u32 unk_9c;
    u32 unk_a0;
} Ov018EventWork;

// The work of func_ov018_02181d8c's event (0x34 bytes)
typedef struct Ov018Script {
    Ov018MsgWork msg;
    PlayerInfo *player;
} Ov018Script;

// What func_ov011_021766cc's battle reads (intrude_comm.c's IntrudeBattleParam): the system, the other player's net
// ID, the level and the count of the Pokemon
typedef struct Ov018BattleParam {
    GameSystem *gsys;
    u8 netId;
    u8 level;
    u8 count;
} Ov018BattleParam;

// The work of the battle events (func_ov018_0218123c, func_ov018_02181584; 0xb0 bytes)
typedef struct Ov018WorkB {
    EventCommCommonWork common;
    u32 unk_98;
    Ov018BattleParam battle;
    u32 unk_a4;
    u32 unk_a8;
    u32 unk_ac;
} Ov018WorkB;

// The work of func_ov018_02182920's event (0xac bytes): the message it shows
typedef struct Ov018WorkC {
    EventCommCommonWork common;
    u32 unk_98;
    u16 msgId;
    u8 unk_9e[0xe];
} Ov018WorkC;

u32 func_ov018_021809a4(GameEvent *param_1, s32 *param_2, Ov018EventWork *param_3);
GameEventReturnCode func_ov018_02180b38(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_02180c00(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_02180cc8(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_02180e0c(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_02180ed4(GameEvent *param_1, u32 *param_2, void *data);
u32 func_ov018_02180fcc(GameEvent *param_1, s32 *param_2, Ov018EventWork *param_3);
GameEventReturnCode func_ov018_02181140(GameEvent *param_1, u32 *param_2, void *data);
u32 func_ov018_0218123c(GameEvent *param_1, s32 *param_2, Ov018WorkB *param_3);
u32 func_ov018_02181584(GameEvent *param_1, s32 *param_2, Ov018WorkB *param_3);
GameEventReturnCode func_ov018_02181940(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_021819f4(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_02181a98(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_02181b68(GameEvent *param_1, u32 *param_2, void *data);
u32 func_ov018_02181c0c(GameEvent *param_1, s32 *param_2, Ov018EventWork *param_3);
GameEventReturnCode func_ov018_02181dd4(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_02181e90(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_02181f34(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_02181fd8(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_021820a4(GameEvent *param_1, u32 *param_2, void *data);
u32 func_ov018_021823a8(GameEvent *param_1, s32 *param_2, Ov018EventWork *param_3);
BOOL func_ov011_021756d0(u16 value);
u32 func_ov018_0218261c(GameEvent *param_1, s32 *param_2, Ov018EventWork *param_3);
GameEventReturnCode func_ov018_02182920(GameEvent *param_1, u32 *param_2, void *data);
GameEventReturnCode func_ov018_02182150(GameEvent *param_1, u32 *param_2, void *param_3);
GameEvent *func_ov018_021821b0(GameSystem *gsys, IntrudeMissionEntry *arg);
GameEventReturnCode func_ov018_02182214(GameEvent *param_1, s32 *param_2, void *param_3);

void func_ov018_02180980(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, (void *)func_ov018_021809a4, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

const u16 data_ov018_02183848[5] = { 0x008e, 0x008f, 0x0090, 0x0091, 0x0092 };
const u16 data_ov018_02183852[5] = { 0x0082, 0x0083, 0x0084, 0x0085, 0x0086 };
const u16 data_ov018_0218385c[5] = { 0x0098, 0x0099, 0x009a, 0x009b, 0x009c };
const u16 data_ov018_02183866[5] = { 0x0093, 0x0094, 0x0095, 0x0096, 0x0097 };
const u16 data_ov018_02183870[5] = { 0x0087, 0x0088, 0x0089, 0x008a, 0x008c };
const u16 data_ov018_0218387a[5] = { 0x0087, 0x0088, 0x0089, 0x008b, 0x008d };
const u16 data_ov018_02183884[6] = { 0x009d, 0x009e, 0x009f, 0x00a0, 0x00a1, 0x0000 };
const u16 data_ov018_02183890[5] = { 0x0032, 0x0033, 0x0034, 0x0035, 0x0036 };
const u16 data_ov018_0218389a[5] = { 0x003c, 0x003d, 0x003e, 0x003f, 0x0040 };
const u16 data_ov018_021838a4[5] = { 0x0037, 0x0038, 0x0039, 0x003a, 0x003b };
const u16 data_ov018_021838ae[5] = { 0x004b, 0x004c, 0x004d, 0x004e, 0x004f };
const u16 data_ov018_021838b8[6] = { 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0000 };
const u16 data_ov018_021838c4[5] = { 0x00b6, 0x00b7, 0x00b8, 0x00b9, 0x00ba };
const u16 data_ov018_021838ce[5] = { 0x00ac, 0x00ad, 0x00ae, 0x00af, 0x00b0 };
const u16 data_ov018_021838d8[5] = { 0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4 };
const u16 data_ov018_021838e2[5] = { 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce };
const u16 data_ov018_021838ec[5] = { 0x00d4, 0x00d5, 0x00d6, 0x00d7, 0x00d8 };
const u16 data_ov018_021838f6[5] = { 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5 };
const u16 data_ov018_02183900[5] = { 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf };
const u16 data_ov018_0218390a[5] = { 0x00c5, 0x00c6, 0x00c7, 0x00c8, 0x00c9 };
const u16 data_ov018_02183914[5] = { 0x00cf, 0x00d0, 0x00d1, 0x00d2, 0x00d3 };
const u16 data_ov018_0218391e[5] = { 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd };
const u16 data_ov018_02183928[5] = { 0x010b, 0x010c, 0x010d, 0x010e, 0x010f };
const u16 data_ov018_02183932[5] = { 0x0101, 0x0102, 0x0103, 0x0104, 0x0105 };
const u16 data_ov018_0218393c[5] = { 0x00fc, 0x00fd, 0x00fe, 0x00ff, 0x0100 };
const u16 data_ov018_02183946[5] = { 0x0106, 0x0107, 0x0108, 0x0109, 0x010a };
const u16 data_ov018_02183950[5] = { 0x00ed, 0x00ee, 0x00ef, 0x00f0, 0x00f1 };
const u16 data_ov018_0218395a[5] = { 0x00f7, 0x00f8, 0x00f9, 0x00fa, 0x00fb };
const u16 data_ov018_02183964[6] = { 0x006e, 0x006f, 0x0070, 0x0071, 0x0072, 0x0000 };
const u16 data_ov018_02183970[5] = { 0x011f, 0x0120, 0x0121, 0x0122, 0x0123 };
const u16 data_ov018_0218397a[5] = { 0x0129, 0x012a, 0x012b, 0x012c, 0x012d };
const u16 data_ov018_02183984[5] = { 0x0124, 0x0125, 0x0126, 0x0127, 0x0128 };
const u16 data_ov018_0218398e[5] = { 0x011a, 0x011b, 0x011c, 0x011d, 0x011e };
const u16 data_ov018_02183998[6] = { 0x0133, 0x0134, 0x0135, 0x0136, 0x0137, 0x0000 };
const u16 data_ov018_021839a4[5] = { 0x0138, 0x0139, 0x013a, 0x013b, 0x013d };
const u16 data_ov018_021839ae[5] = { 0x0138, 0x0139, 0x013a, 0x013c, 0x013e };
const u16 data_ov018_021839b8[5] = { 0x014e, 0x014f, 0x0150, 0x0151, 0x0152 };
const u16 data_ov018_021839c2[5] = { 0x0158, 0x0159, 0x015a, 0x015b, 0x015c };
const u16 data_ov018_021839cc[5] = { 0x0162, 0x0163, 0x0164, 0x0165, 0x0166 };
const u16 data_ov018_021839d6[5] = { 0x0144, 0x0145, 0x0146, 0x0147, 0x0148 };
const u16 data_ov018_021839e0[5] = { 0x013f, 0x0140, 0x0141, 0x0142, 0x0143 };
const u16 data_ov018_021839ea[5] = { 0x0153, 0x0154, 0x0155, 0x0156, 0x0157 };
const u16 data_ov018_021839f4[5] = { 0x015d, 0x015e, 0x015f, 0x0160, 0x0161 };
const u16 data_ov018_021839fe[5] = { 0x0167, 0x0168, 0x0169, 0x016a, 0x016b };
const u16 data_ov018_02183a08[6] = { 0x0149, 0x014a, 0x014b, 0x014c, 0x014d, 0x0000 };

u32 func_ov018_021809a4(GameEvent *param_1, s32 *param_2, Ov018EventWork *param_3) {
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameData *gameData;
    PlayerInfo *playerInfo;
    GameEvent *next;

    if (intrude == NULL && *param_2 <= 1) {
        if (func_ov018_0217f40c(&param_3->common.msg) == 0) {
            return 0;
        }
        if (*param_2 < 5) {
            func_ov018_0217f3c4(&param_3->common.msg, 4);
            *param_2 = 5;
            param_3->unk_98 = 1;
            return 0;
        }
    }

    switch (*param_2) {
    case 0:
        if (func_ov011_0217405c(intrude, &intrude->unk_1AC) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 1:
        if (func_ov011_02175cf4(intrude, &intrude->unk_1AC) != 0) {
            param_3->unk_9c = 1;
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        gameData = GSYS_GetGameData(gsys);
        playerInfo = GetGameDataPlayerInfo(gameData);
        copyVarForText(param_3->common.msg.wordSet, 1, playerInfo);
        loadPassPowerToStrbuf(param_3->common.msg.wordSet, 2, param_3->common.entry.mission.unk_14);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183870[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 3:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            func_ov018_0217f3c4(&param_3->common.msg, data_ov018_0218387a[param_3->common.unk_42]);
            *param_2 = *param_2 + 1;
        }
        break;
    case 4:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183884[param_3->common.unk_42]);
            GFL_SndSEPlay(0x5e0);
            *param_2 = *param_2 + 1;
        }
        break;
    case 5:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 6;
        }
        break;
    case 6:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 7;
        }
        break;
    case 7:
        func_ov018_0217fe38(intrude, &param_3->common);
        next = func_ov018_021804f8(gsys, param_3->unk_9c);
        GameEvent_Replace(param_1, next);
        return 0;
    default:
        break;
    }
    return 0;
}


void func_ov018_02180b14(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02180b38, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02180b38(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameData *gameData;
    PlayerInfo *playerInfo;

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        gameData = GSYS_GetGameData(gsys);
        playerInfo = GetGameDataPlayerInfo(gameData);
        copyVarForText(param_3->common.msg.wordSet, 1, playerInfo);
        loadPassPowerToStrbuf(param_3->common.msg.wordSet, 2, param_3->common.entry.mission.unk_14);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183848[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_02180bdc(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02180c00, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02180c00(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameData *gameData;
    PlayerInfo *playerInfo;

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        gameData = GSYS_GetGameData(gsys);
        playerInfo = GetGameDataPlayerInfo(gameData);
        copyVarForText(param_3->common.msg.wordSet, 1, playerInfo);
        loadPassPowerToStrbuf(param_3->common.msg.wordSet, 2, param_3->common.entry.mission.unk_14);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183866[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_02180ca4(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02180cc8, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02180cc8(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameData *gameData;
    PlayerInfo *playerInfo;
    u32 args[2];
    GameEvent *event;

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        gameData = GSYS_GetGameData(gsys);
        playerInfo = GetGameDataPlayerInfo(gameData);
        copyVarForText(param_3->common.msg.wordSet, 1, playerInfo);
        loadPassPowerToStrbuf(param_3->common.msg.wordSet, 2, param_3->common.entry.mission.unk_14);
        param_3->unk_a0 = param_3->common.unk_42;
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183852[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        func_ov018_0217f318(&param_3->common.msg);
        args[0] = param_3->common.entry.mission.unk_14;
        args[1] = 0;
        event = GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(83), func_ov083_021f5260, args);
        GameEvent_ChainNext(param_1, event);
        *param_2 = *param_2 + 1;
        break;
    case 3:
        *param_2 = 4;
        break;
    case 4:
        func_ov018_0217f2b4(&param_3->common.msg, gsys);
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_0218385c[param_3->unk_a0]);
        *param_2 = *param_2 + 1;
        break;
    case 5:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1 && func_ov018_0217f420() == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 6:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_02180de8(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02180e0c, offsetof(Ov018EventWork, unk_a0));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02180e0c(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameData *gameData;
    PlayerInfo *playerInfo;

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        gameData = GSYS_GetGameData(gsys);
        playerInfo = GetGameDataPlayerInfo(gameData);
        copyVarForText(param_3->common.msg.wordSet, 1, playerInfo);
        loadPassPowerToStrbuf(param_3->common.msg.wordSet, 2, param_3->common.entry.mission.unk_14);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021838b8[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_02180eb0(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02180ed4, offsetof(Ov018EventWork, unk_a0));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02180ed4(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameData *gameData;
    PlayerInfo *playerInfo;
    u32 args[2];
    GameEvent *event;

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        gameData = GSYS_GetGameData(gsys);
        playerInfo = GetGameDataPlayerInfo(gameData);
        copyVarForText(param_3->common.msg.wordSet, 1, playerInfo);
        loadPassPowerToStrbuf(param_3->common.msg.wordSet, 2, param_3->common.entry.mission.unk_14);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183890[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        func_ov018_0217f318(&param_3->common.msg);
        args[0] = param_3->common.entry.mission.unk_14;
        args[1] = 0;
        event = GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(83), func_ov083_021f5260, args);
        GameEvent_ChainNext(param_1, event);
        *param_2 = *param_2 + 1;
        break;
    case 3:
        *param_2 = 4;
        break;
    case 4:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_02180fa8(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, (void *)func_ov018_02180fcc, offsetof(Ov018EventWork, unk_a0));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

u32 func_ov018_02180fcc(GameEvent *param_1, s32 *param_2, Ov018EventWork *param_3) {
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameData *gameData;
    PlayerInfo *playerInfo;
    GameEvent *next;

    if (intrude == NULL && *param_2 <= 1) {
        if (func_ov018_0217f40c(&param_3->common.msg) == 0) {
            return 0;
        }
        if (*param_2 < 4) {
            func_ov018_0217f3c4(&param_3->common.msg, 4);
            *param_2 = 4;
            param_3->unk_98 = 1;
            return 0;
        }
    }

    switch (*param_2) {
    case 0:
        if (func_ov011_0217405c(intrude, &intrude->unk_1AC) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 1:
        if (func_ov011_02175cf4(intrude, &intrude->unk_1AC) != 0) {
            param_3->unk_9c = 1;
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        gameData = GSYS_GetGameData(gsys);
        playerInfo = GetGameDataPlayerInfo(gameData);
        copyVarForText(param_3->common.msg.wordSet, 1, playerInfo);
        loadPassPowerToStrbuf(param_3->common.msg.wordSet, 2, param_3->common.entry.mission.unk_14);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021838a4[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 3:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021838ae[param_3->common.unk_42]);
            GFL_SndSEPlay(0x5e0);
            *param_2 = *param_2 + 1;
        }
        break;
    case 4:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 5;
        }
        break;
    case 5:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 6;
        }
        break;
    case 6:
        func_ov018_0217fe38(intrude, &param_3->common);
        next = func_ov018_021804f8(gsys, param_3->unk_9c);
        GameEvent_Replace(param_1, next);
        return 0;
    default:
        break;
    }
    return 0;
}


void func_ov018_0218111c(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02181140, offsetof(Ov018EventWork, unk_a0));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02181140(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameData *gameData;
    PlayerInfo *playerInfo;

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        gameData = GSYS_GetGameData(gsys);
        playerInfo = GetGameDataPlayerInfo(gameData);
        copyVarForText(param_3->common.msg.wordSet, 1, playerInfo);
        loadPassPowerToStrbuf(param_3->common.msg.wordSet, 2, param_3->common.entry.mission.unk_14);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_0218389a[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_021811e4(GameEvent *param_1, void *param_2) {
    GameSystem *pGVar1;
    Ov018WorkB *dest;
    Ov018EventWork *src = param_2;

    pGVar1 = GameEvent_GetGameSystem(param_1);
    GameEvent_Transplant(param_1, (void *)func_ov018_0218123c, sizeof(Ov018WorkB));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(src, dest, sizeof(EventCommCommonWork));
    dest->unk_a4 = 0;
    dest->battle.gsys = pGVar1;
    dest->battle.netId = src->common.netId;
    dest->battle.level = src->common.entry.mission.unk_14;
    dest->battle.count = 2;
}

u32 func_ov018_0218123c(GameEvent *param_1, s32 *param_2, Ov018WorkB *param_3) {
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameEvent *next;
    u32 msg;
    u32 result;
    u32 keys;

    if (intrude == NULL) {
        if (func_ov018_0217f40c(&param_3->common.msg) == 0) {
            return 0;
        }
        if (*param_2 < 0xd) {
            func_ov018_0217f2b4(&param_3->common.msg, gsys);
            func_ov018_0217f3c4(&param_3->common.msg, 4);
            *param_2 = 0xd;
            param_3->unk_98 = 1;
            return 0;
        }
    }

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        WordSetNumber(param_3->common.msg.wordSet, 1, param_3->battle.level, 3, 0, 1);
        if (param_3->unk_a4 == 0) {
            msg = data_ov018_021838f6[param_3->common.unk_42];
        } else {
            msg = data_ov018_02183900[param_3->common.unk_42];
        }
        func_ov018_0217f3c4(&param_3->common.msg, msg);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            result = func_ov011_02175514(intrude);
            switch (result) {
            case 4:
                *param_2 = 2;
                break;
            case 2:
            case 3:
                *param_2 = 0xb;
                break;
            default:
                keys = GCTX_HIDGetPressedKeys();
                if ((keys & 2) != 0) {
                    if (func_ov011_02173b9c(intrude->unk_154.netId, intrude->unk_154.unk_04) == 1) {
                        func_ov011_021753b8(intrude);
                        *param_2 = 0xc;
                    }
                }
                break;
            }
        }
        break;
    case 2:
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_0218390a[param_3->common.unk_42]);
        *param_2 = 3;
        break;
    case 3:
        if (func_ov011_0217405c(intrude, &intrude->unk_1AC) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 4:
        if (func_ov011_02175cf4(intrude, &intrude->unk_1AC) != 0) {
            func_ov011_021756f0(intrude, 1);
            *param_2 = 5;
        }
        break;
    case 5:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            if (func_ov011_021744d0(intrude, param_3->common.netId, 1) == 1) {
                *param_2 = *param_2 + 1;
            }
        }
        break;
    case 6:
        if (func_ov010_0215839c(intrude, 1, param_3->common.netId) == 1) {
            *param_2 = 7;
        } else {
            keys = GCTX_HIDGetPressedKeys();
            if ((keys & 2) != 0) {
                if (func_ov011_02173b9c(intrude->unk_154.netId, intrude->unk_154.unk_04) == 1) {
                    func_ov011_021753b8(intrude);
                    *param_2 = 0xc;
                }
            }
        }
        break;
    case 7:
        if (func_ov011_021744d0(intrude, param_3->common.netId, 2) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 8:
        if (func_ov010_0215839c(intrude, 2, param_3->common.netId) == 1) {
            func_ov018_0217f318(&param_3->common.msg);
            *param_2 = 9;
        } else {
            if (func_ov011_02175514(intrude) == 3) {
                func_ov018_0217f3c4(&param_3->common.msg, 4);
                *param_2 = 0xd;
            }
        }
        break;
    case 9:
        param_3->unk_a8 = 1;
        next = func_ov011_021766cc(gsys, &param_3->battle);
        GameEvent_ChainNext(param_1, next);
        *param_2 = *param_2 + 1;
        break;
    case 10:
        func_ov018_0217f2b4(&param_3->common.msg, gsys);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183914[param_3->common.unk_42]);
        *param_2 = 0xd;
        break;
    case 11:
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_0218391e[param_3->common.unk_42]);
        *param_2 = 0xd;
        break;
    case 12:
        param_3->unk_ac = 1;
        *param_2 = 0xf;
        break;
    case 13:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 0xe;
        }
        break;
    case 14:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 0xf;
        }
        break;
    case 15:
        func_ov018_0217fe38(intrude, &param_3->common);
        if (GameCommSys_BootCheck(commSys) == 2) {
            if (func_02026474(commSys) != NULL) {
                intrude = func_02026474(commSys);
                func_ov011_021756f0(intrude, 0);
            }
        }
        if (param_3->unk_a8 == 1) {
            next = func_ov018_021804f8(gsys, param_3->unk_a8);
        } else if (param_3->unk_ac == 0) {
            next = func_ov018_0217ca84(gsys, 1);
        } else {
            next = func_ov018_0217ca84(gsys, 0);
        }
        GameEvent_Replace(param_1, next);
        return 0;
    default:
        break;
    }
    return 0;
}


void func_ov018_0218152c(GameEvent *param_1, void *param_2) {
    GameSystem *pGVar1;
    Ov018WorkB *dest;
    Ov018EventWork *src = param_2;

    pGVar1 = GameEvent_GetGameSystem(param_1);
    GameEvent_Transplant(param_1, (void *)func_ov018_02181584, sizeof(Ov018WorkB));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(src, dest, sizeof(EventCommCommonWork));
    dest->unk_a4 = 0;
    dest->battle.gsys = pGVar1;
    dest->battle.netId = dest->common.netId;
    dest->battle.level = src->common.entry.mission.unk_14;
    dest->battle.count = 2;
}

u32 func_ov018_02181584(GameEvent *param_1, s32 *param_2, Ov018WorkB *param_3) {
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameEvent *next;
    u32 msg;
    SurveyProbabilityState *survey;
    GameData *gameData;
    u32 keys;

    if (intrude == NULL) {
        if (func_ov018_0217f40c(&param_3->common.msg) == 0) {
            return 0;
        }
        if (*param_2 < 0xe && *param_2 != 1 && *param_2 != 2) {
            func_ov018_0217f2b4(&param_3->common.msg, gsys);
            func_ov018_0217f3c4(&param_3->common.msg, 4);
            *param_2 = 0xe;
            param_3->unk_98 = 1;
            return 0;
        }
    }

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        WordSetNumber(param_3->common.msg.wordSet, 1, param_3->battle.level, 3, 0, 1);
        if (param_3->unk_a4 == 0) {
            msg = data_ov018_021838c4[param_3->common.unk_42];
        } else {
            msg = data_ov018_021838ce[param_3->common.unk_42];
        }
        func_ov018_0217f3c4(&param_3->common.msg, msg);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            func_ov018_0217f4d8(&param_3->common.msg, gsys);
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        switch (func_ov018_0217f508(&param_3->common.msg)) {
        case 0:
            func_ov018_0217f4c4(&param_3->common.msg);
            *param_2 = 3;
            break;
        case 1:
            func_ov018_0217f4c4(&param_3->common.msg);
            *param_2 = 0xb;
            break;
        }
        break;
    case 3:
        if (func_ov011_02173ae0(intrude, intrude->unk_154.netId, 4, intrude->unk_154.unk_04) == 1) {
            func_ov011_021756f0(intrude, 1);
            *param_2 = 4;
        }
        break;
    case 4:
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021838d8[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 5:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            if (func_ov011_021744d0(intrude, param_3->common.netId, 1) == 1) {
                *param_2 = *param_2 + 1;
            }
        }
        break;
    case 6:
        if (func_ov010_0215839c(intrude, 1, param_3->common.netId) == 1) {
            *param_2 = 7;
        } else {
            keys = GCTX_HIDGetPressedKeys();
            if ((keys & 2) != 0) {
                if (func_ov011_02173b9c(intrude->unk_154.netId, intrude->unk_154.unk_04) == 1) {
                    func_ov011_021753b8(intrude);
                    *param_2 = 0xd;
                    func_02026400(commSys);
                }
            } else if (func_ov011_02175514(intrude) == 3) {
                func_ov018_0217f3c4(&param_3->common.msg, 4);
                *param_2 = 0xe;
            }
        }
        break;
    case 7:
        if (func_ov011_021744d0(intrude, param_3->common.netId, 2) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 8:
        if (func_ov010_0215839c(intrude, 2, param_3->common.netId) == 1) {
            func_ov018_0217f318(&param_3->common.msg);
            *param_2 = 9;
        } else if (func_ov011_02175514(intrude) == 3) {
            func_ov018_0217f3c4(&param_3->common.msg, 4);
            *param_2 = 0xe;
        }
        break;
    case 9:
        next = func_ov011_021766cc(gsys, &param_3->battle);
        GameEvent_ChainNext(param_1, next);
        *param_2 = *param_2 + 1;
        break;
    case 10:
        func_ov018_0217f2b4(&param_3->common.msg, gsys);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021838e2[param_3->common.unk_42]);
        *param_2 = 0xe;
        break;
    case 11:
        if (func_ov011_02173ae0(intrude, intrude->unk_154.netId, 2, intrude->unk_154.unk_04) == 1) {
            gameData = GSYS_GetGameData(gsys);
            survey = (SurveyProbabilityState *)func_020127b8(gameData);
            insideProbabilityLoop(survey, 0);
            *param_2 = 0xc;
        }
        break;
    case 12:
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021838ec[param_3->common.unk_42]);
        *param_2 = 0xe;
        break;
    case 13:
        *param_2 = 0x10;
        break;
    case 14:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 0xf;
        }
        break;
    case 15:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 0x10;
        }
        break;
    case 16:
        func_ov018_0217fe38(intrude, &param_3->common);
        if (GameCommSys_BootCheck(commSys) == 2) {
            if (func_02026474(commSys) != NULL) {
                intrude = func_02026474(commSys);
                func_ov011_021756f0(intrude, 0);
            }
        }
        return 1;
    default:
        break;
    }
    return 0;
}


void func_ov018_0218186c(GameEvent *param_1, void *param_2) {
    GameSystem *pGVar1;
    Ov018WorkB *dest;
    Ov018EventWork *src = param_2;

    pGVar1 = GameEvent_GetGameSystem(param_1);
    GameEvent_Transplant(param_1, (void *)func_ov018_0218123c, sizeof(Ov018WorkB));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(src, dest, sizeof(EventCommCommonWork));
    dest->unk_a4 = 1;
    dest->battle.gsys = pGVar1;
    dest->battle.netId = dest->common.netId;
    dest->battle.level = src->common.entry.mission.unk_14;
    dest->battle.count = 2;
}

void func_ov018_021818c4(GameEvent *param_1, void *param_2) {
    GameSystem *pGVar1;
    Ov018WorkB *dest;
    Ov018EventWork *src = param_2;

    pGVar1 = GameEvent_GetGameSystem(param_1);
    GameEvent_Transplant(param_1, (void *)func_ov018_02181584, sizeof(Ov018WorkB));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(src, dest, sizeof(EventCommCommonWork));
    dest->unk_a4 = 1;
    dest->battle.gsys = pGVar1;
    dest->battle.netId = dest->common.netId;
    dest->battle.level = src->common.entry.mission.unk_14;
    dest->battle.count = 2;
}

void func_ov018_0218191c(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02181940, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02181940(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183928[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 4;
        }
        break;
    case 4:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_021819d0(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_021819f4, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_021819f4(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_0218393c[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_02181a74(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02181a98, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02181a98(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameEvent *next;

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183932[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        if (func_ov010_02158344(commSys) != NULL) {
            GFL_SndSEPlay(0x737);
            next = func_ov018_0217ca1c(gsys);
            GameEvent_Replace(param_1, next);
            return 0;
        }
        return 1;
    }
    return 0;
}


void func_ov018_02181b44(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02181b68, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02181b68(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183946[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_02181be8(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, (void *)func_ov018_02181c0c, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

u32 func_ov018_02181c0c(GameEvent *param_1, s32 *param_2, Ov018EventWork *param_3) {
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameEvent *next;
    u32 result;

    if (intrude == NULL && *param_2 <= 2 && *param_2 < 6) {
        func_ov018_0217f3c4(&param_3->common.msg, 4);
        *param_2 = 6;
        param_3->unk_98 = 1;
        return 0;
    }

    switch (*param_2) {
    case 0:
        if (func_ov011_0217405c(intrude, &intrude->unk_1AC) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 1:
        result = func_ov011_02175cf4(intrude, &intrude->unk_1AC);
        param_3->unk_a0 = result;
        switch (result) {
        case 1:
            *param_2 = 2;
            break;
        case 2:
            *param_2 = 5;
            break;
        }
        break;
    case 2:
        if (func_ov011_0217456c(intrude, param_3->common.netId, 2) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 3:
        param_3->unk_9c = 1;
        func_ov011_021762d8(&param_3->common.entry.mission, &param_3->common.entry.player, param_3->common.msg.wordSet, param_3->common.heapId);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183950[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 4:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            func_ov018_0217f3c4(&param_3->common.msg, data_ov018_0218395a[param_3->common.unk_42]);
            GFL_SndSEPlay(0x5e0);
            *param_2 = 6;
        }
        break;
    case 5:
        param_3->unk_9c = 1;
        func_ov011_021762d8(&param_3->common.entry.mission, &param_3->common.entry.player, param_3->common.msg.wordSet, param_3->common.heapId);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183964[param_3->common.unk_42]);
        *param_2 = 6;
        break;
    case 6:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 7;
        }
        break;
    case 7:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 8;
        }
        break;
    case 8:
        func_ov018_0217fe38(intrude, &param_3->common);
        if (param_3->unk_a0 == 1) {
            next = func_ov018_021804f8(gsys, param_3->unk_9c);
            GameEvent_Replace(param_1, next);
            return 0;
        }
        return 1;
    default:
        break;
    }
    return 0;
}


GameEvent *func_ov018_02181d8c(GameSystem *param_1, u32 param_2) {
    GameData *gameData;
    CommPlayerSupport *pCVar1;
    GameEvent *event;
    Ov018Script *data;

    gameData = GSYS_GetGameData(param_1);
    pCVar1 = func_02012f20(gameData);
    event = GameEvent_Create(param_1, (GameEvent *)0x0, func_ov018_02181dd4, sizeof(Ov018Script));
    data = GameEvent_GetData(event);
    data->player = func_0200846c(param_2);
    func_02008494(func_0202e82c(pCVar1), data->player);
    func_ov018_0217f2b4(&data->msg, param_1);
    return event;
}

GameEventReturnCode func_ov018_02181dd4(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018Script *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameEvent *event;
    ScriptWork *work;

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->msg.wordSet, 0, param_3->player);
        func_ov018_0217f3c4(&param_3->msg, 0x1c);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->msg) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        func_ov018_0217f318(&param_3->msg);
        event = EventScriptCall_Create(gsys, 0xafd, NULL, 4);
        work = EventScriptCall_GetWork(event);
        ScriptWork_SetParams(work, 0x1c, 1, 0, 0);
        GameEvent_ChainNext(param_1, event);
        *param_2 = *param_2 + 1;
        break;
    case 3:
        *param_2 = 4;
        break;
    case 4:
        GFL_HeapFree(param_3->player);
        return 1;
    }
    return 0;
}


void func_ov018_02181e6c(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02181e90, 0x9c);
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02181e90(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183970[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_02181f10(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02181f34, 0x9c);
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02181f34(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183984[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


void func_ov018_02181fb4(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_02181fd8, 0x9c);
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_02181fd8(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameEvent *next;

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_0218397a[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        if (intrude != NULL) {
            GFL_SndSEPlay(0x737);
            next = func_ov018_0217ca1c(gsys);
            GameEvent_Replace(param_1, next);
            return 0;
        }
        return 1;
    }
    return 0;
}


void func_ov018_02182080(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, func_ov018_021820a4, 0x9c);
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

GameEventReturnCode func_ov018_021820a4(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018EventWork *param_3 = data;
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_0218398e[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 2;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    }
    return 0;
}


// Its event's work is a copy of the mission entry (0x34 bytes)
GameEvent *func_ov018_02182124(GameSystem *gsys, void *param_2, IntrudeMissionEntry *param_3) {
    GameEvent *event;
    event = GameEvent_Create(gsys, (GameEvent *)0x0, func_ov018_02182150, sizeof(IntrudeMissionEntry));
    *(IntrudeMissionEntry *)GameEvent_GetData(event) = *param_3;
    return event;
}

GameEventReturnCode func_ov018_02182150(GameEvent *param_1, u32 *param_2, void *param_3) {
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    Field *field = GSYS_GetField(gsys);
    GameEvent *next;

    switch (*param_2) {
    case 0:
        DisableAllActorsMovement(Field_GetActorSystem(field));
        next = func_ov018_021821b0(gsys, param_3);
        GameEvent_ChainNext(param_1, next);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (field != NULL && Field_CheckMapLoadFinished(field) == TRUE) {
            EnableAllActorsMovement(Field_GetActorSystem(field));
            return 1;
        }
        break;
    }
    return 0;
}

GameEvent *func_ov018_021821b0(GameSystem *gsys, IntrudeMissionEntry *arg) {
    GameData *gameData = GSYS_GetGameData(gsys);
    u16 *item = &arg->mission.unk_14;
    PlayerInfo *player = GetGameDataPlayerInfo(gameData);
    GameEvent *event = GameEvent_Create(gsys, NULL, (void *)func_ov018_02182214, sizeof(Ov018Event));
    Ov018Event *work = GameEvent_GetData(event);

    copyName(player->name, work->name, 8);
    work->unk_40 = item[1];
    work->unk_42 = item[0];
    work->unk_44 = arg->player.unk_10;
    func_ov018_0217f2b4(&work->msg, gsys);
    return event;
}

GameEventReturnCode func_ov018_02182214(GameEvent *param_1, s32 *param_2, void *param_3) {
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    GameData *gameData = GSYS_GetGameData(gsys);
    IntrudeComm *comm;
    Ov018Event *work = param_3;
    PlayerInfo *player;
    GameEvent *next;
    u32 value;

    GSYS_GetField(gsys);
    comm = func_ov010_02158344(commSys);
    if (comm == NULL) {
        if (func_ov018_0217f40c(&work->msg) == 0) {
            return 0;
        }
        if (*param_2 < 4) {
            func_ov018_0217f3c4(&work->msg, 6);
            *param_2 = 4;
            work->unk_48 = 1;
            return 0;
        }
    }

    switch (*param_2) {
    case 0:
        if (func_ov011_0217405c(comm, &comm->unk_1AC) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 1:
        work->unk_50 = func_ov011_02175cf4(comm, &comm->unk_1AC);
        if (work->unk_50 != 0) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        player = GetGameDataPlayerInfo(gameData);
        copyVarForText(work->msg.wordSet, 0, player);
        loadItemNameToStrbuf(work->msg.wordSet, 1, work->unk_40);
        if (work->unk_50 == 1) {
            int index = func_ov018_0217f5f0(comm, (u8)func_02012b58(gameData));
            work->unk_4c = 1;
            value = data_ov018_02183998[index];
        } else {
            value = 5;
        }
        func_ov018_0217f3c4(&work->msg, value);
        *param_2 = *param_2 + 1;
        break;
    case 3:
        if (work->unk_4c == 1) {
            if (func_ov011_021745f8((u8)work->unk_44, work->name) == 1) {
                *param_2 = *param_2 + 1;
            }
        } else {
            *param_2 = *param_2 + 1;
        }
        break;
    case 4:
        if (func_ov018_0217f40c(&work->msg) == 1) {
            *param_2 = 5;
        }
        break;
    case 5:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 6;
        }
        break;
    case 6:
        func_ov018_0217f318(&work->msg);
        next = func_ov018_021804f8(gsys, work->unk_4c);
        GameEvent_Replace(param_1, next);
        return 0;
    default:
        break;
    }
    return 0;
}





void func_ov018_0218236c(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, (void *)func_ov018_021823a8, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
    // The item as an address and an offset: dest->entry.mission.unk_14 computes its address once, Black twice
    if (func_ov011_021756d0(*(u16 *)((uintptr_t)dest + offsetof(Ov018EventWork, common.entry.mission.unk_14))) == 0) {
        *(u16 *)((uintptr_t)dest + offsetof(Ov018EventWork, common.entry.mission.unk_14)) = 0x11;
    }
}

u32 func_ov018_021823a8(GameEvent *param_1, s32 *param_2, Ov018EventWork *param_3) {
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude = func_ov010_02158344(commSys);
    GameEvent *next;
    GameData *gameData;
    u16 *item;
    u32 result;
    u32 keys;

    if (intrude == NULL && *param_2 <= 4) {
        if (func_ov018_0217f40c(&param_3->common.msg) == 0) {
            return 0;
        }
        if (*param_2 < 9) {
            func_ov018_0217f3c4(&param_3->common.msg, 4);
            *param_2 = 9;
            param_3->unk_98 = 1;
            return 0;
        }
    }

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        item = &param_3->common.entry.mission.unk_14;
        loadItemNameToStrbuf(param_3->common.msg.wordSet, 1, item[0]);
        WordSetNumber(param_3->common.msg.wordSet, 2, item[1], 5, 0, 1);
        gameData = GSYS_GetGameData(gsys);
        copyVarForText(param_3->common.msg.wordSet, 3, GetGameDataPlayerInfo(gameData));
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021839e0[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            result = func_ov011_02175514(intrude);
            switch (result) {
            case 7:
                *param_2 = 2;
                break;
            case 3:
            case 8:
                *param_2 = 5;
                break;
            case 9:
                *param_2 = 6;
                break;
            case 10:
                *param_2 = 7;
                break;
            default:
                keys = GCTX_HIDGetPressedKeys();
                if ((keys & 2) != 0) {
                    if (func_ov011_02173b9c(intrude->unk_154.netId, intrude->unk_154.unk_04) == 1) {
                        func_ov011_021753b8(intrude);
                        param_3->unk_a0 = 1;
                        *param_2 = 8;
                    }
                }
                break;
            }
        }
        break;
    case 2:
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_02183a08[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 3:
        if (func_ov011_0217405c(intrude, &intrude->unk_1AC) == 1) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 4:
        if (func_ov011_02175cf4(intrude, &intrude->unk_1AC) != 0) {
            param_3->unk_9c = 1;
            *param_2 = 9;
        }
        break;
    case 5:
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021839ea[param_3->common.unk_42]);
        *param_2 = 9;
        break;
    case 6:
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021839f4[param_3->common.unk_42]);
        *param_2 = 9;
        break;
    case 7:
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021839fe[param_3->common.unk_42]);
        *param_2 = 9;
        break;
    case 8:
        *param_2 = 0xb;
        break;
    case 9:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 0xa;
        }
        break;
    case 10:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 0xb;
        }
        break;
    case 11:
        func_ov018_0217fe38(intrude, &param_3->common);
        if (param_3->unk_9c == 1) {
            next = func_ov018_021804f8(gsys, param_3->unk_9c);
            GameEvent_Replace(param_1, next);
            return 0;
        }
        if (param_3->unk_a0 == 0) {
            next = func_ov018_0217ca84(gsys, 2);
            GameEvent_Replace(param_1, next);
            return 0;
        }
        next = func_ov018_0217ca84(gsys, 0);
        GameEvent_Replace(param_1, next);
        return 0;
    default:
        break;
    }
    return 0;
}


void func_ov018_021825f8(GameEvent *param_1, void *param_2) {
    void *dest;

    GameEvent_Transplant(param_1, (void *)func_ov018_0218261c, sizeof(Ov018EventWork));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(param_2, dest, sizeof(EventCommCommonWork));
}

u32 func_ov018_0218261c(GameEvent *param_1, s32 *param_2, Ov018EventWork *param_3) {
    GameSystem *gsys = GameEvent_GetGameSystem(param_1);
    GameData *gameData = GSYS_GetGameData(gsys);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    IntrudeComm *intrude;
    TrainerCardSave *trainerCard;
    BagSave *bag;
    SurveyProbabilityState *survey;
    BOOL show;
    u32 result;
    u16 *item = &param_3->common.entry.mission.unk_14;

    intrude = func_ov010_02158344(commSys);
    if (intrude == NULL) {
        if (func_ov018_0217f40c(&param_3->common.msg) == 0) {
            return 0;
        }
        if (*param_2 < 8 && *param_2 != 2 && *param_2 != 3) {
            func_ov018_0217f3c4(&param_3->common.msg, 4);
            *param_2 = 8;
            param_3->unk_98 = 1;
            return 0;
        }
    }

    switch (*param_2) {
    case 0:
        copyVarForText(param_3->common.msg.wordSet, 0, &param_3->common.player);
        loadItemNameToStrbuf(param_3->common.msg.wordSet, 1, item[0]);
        WordSetNumber(param_3->common.msg.wordSet, 2, item[1], 5, 0, 1);
        copyVarForText(param_3->common.msg.wordSet, 3, GetGameDataPlayerInfo(GSYS_GetGameData(gsys)));
        func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021839a4[param_3->common.unk_42]);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021839ae[param_3->common.unk_42]);
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            func_ov018_0217f4d8(&param_3->common.msg, gsys);
            *param_2 = *param_2 + 1;
        }
        break;
    case 3:
        result = func_ov018_0217f508(&param_3->common.msg);
        show = FALSE;
        switch (result) {
        case 0:
            func_ov018_0217f4c4(&param_3->common.msg);
            trainerCard = getTrainerCardDataBlkAddress(gameData);
            if (item[1] > getCash(trainerCard)) {
                show = TRUE;
                *param_2 = 5;
            } else {
                bag = GameData_GetBag(gameData);
                if (BagSave_CheckAvailItemSpace(bag, item[0], 1, 0x15) == 0) {
                    show = TRUE;
                    *param_2 = 6;
                } else {
                    *param_2 = 4;
                }
            }
            break;
        case 1:
            func_ov018_0217f4c4(&param_3->common.msg);
            show = TRUE;
            *param_2 = 7;
            break;
        }
        if (show == TRUE) {
            survey = (SurveyProbabilityState *)func_020127b8(gameData);
            insideProbabilityLoop(survey, 3);
        }
        break;
    case 4:
        if (func_ov011_02173ae0(intrude, intrude->unk_154.netId, 7, intrude->unk_154.unk_04) == 1) {
            trainerCard = getTrainerCardDataBlkAddress(gameData);
            subCashFromTotal(trainerCard, item[1]);
            bag = GameData_GetBag(gameData);
            BagSave_AddItem(bag, item[0], 1, 0x15);
            func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021839d6[param_3->common.unk_42]);
            *param_2 = 8;
        }
        break;
    case 5:
        if (func_ov011_02173ae0(intrude, intrude->unk_154.netId, 9, intrude->unk_154.unk_04) == 1) {
            func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021839c2[param_3->common.unk_42]);
            *param_2 = 8;
        }
        break;
    case 6:
        if (func_ov011_02173ae0(intrude, intrude->unk_154.netId, 10, intrude->unk_154.unk_04) == 1) {
            func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021839cc[param_3->common.unk_42]);
            *param_2 = 8;
        }
        break;
    case 7:
        if (func_ov011_02173ae0(intrude, intrude->unk_154.netId, 8, intrude->unk_154.unk_04) == 1) {
            func_ov018_0217f3c4(&param_3->common.msg, data_ov018_021839b8[param_3->common.unk_42]);
            *param_2 = 8;
        }
        break;
    case 8:
        if (func_ov018_0217f40c(&param_3->common.msg) == 1) {
            *param_2 = 9;
        }
        break;
    case 9:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 10;
        }
        break;
    case 10:
        func_ov018_0217fe38(intrude, &param_3->common);
        return 1;
    default:
        break;
    }
    return 0;
}


void func_ov018_021828a8(GameEvent *param_1, void *param_2) {
    func_ov018_0218236c(param_1, param_2);
}

void func_ov018_021828b0(GameEvent *param_1, void *param_2) {
    func_ov018_021825f8(param_1, param_2);
}



void func_ov018_021828b8(GameEvent *param_1, void *param_2, const u16 *msgIds) {
    GameSystem *gsys;
    GameCommSys *commSys;
    IntrudeComm *pIVar1;
    Ov018WorkC *dest;
    Ov018EventWork *src = param_2;

    gsys = GameEvent_GetGameSystem(param_1);
    commSys = GSYS_GetGameCommSystem(gsys);
    pIVar1 = func_ov010_02158344(commSys);
    GameEvent_Transplant(param_1, func_ov018_02182920, sizeof(Ov018WorkC));
    dest = GameEvent_GetData(param_1);
    MI_CpuCopy8(src, dest, sizeof(EventCommCommonWork));
    if (pIVar1 != (IntrudeComm *)0x0) {
        dest->msgId = msgIds[src->common.unk_42];
        copyVarForText(src->common.msg.wordSet, 0, &src->common.player);
        return;
    }
    GFL_DebugAssertFail("", 0, "");
}

GameEventReturnCode func_ov018_02182920(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018WorkC *param_3 = data;
    GameSystem *gsys;
    GameCommSys *commSys;
    IntrudeComm *pIVar1;
    BOOL BVar2;
    gsys = GameEvent_GetGameSystem(param_1);
    commSys = GSYS_GetGameCommSystem(gsys);
    pIVar1 = func_ov010_02158344(commSys);
    switch (*param_2) {
    case 0:
        func_ov018_0217f3c4(&param_3->common.msg, param_3->msgId);
        (*param_2)++;
        break;
    case 1:
        BVar2 = func_ov018_0217f40c(&param_3->common.msg);
        if (BVar2 == 1) {
            (*param_2)++;
        }
        break;
    case 2:
        if (func_ov018_0217f420() == 1) {
            *param_2 = 3;
        }
        break;
    case 3:
        func_ov018_0217fe38(pIVar1, &param_3->common);
        return 1;
    }
    return 0;
}

