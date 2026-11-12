// The first two functions are from pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
// src/ov012/intrude_work.c, where Black 2 has the same code. Black 2 left the rest as stubs; Black's is written from
// its assembly.
#include "types.h"
#include "constants/version.h"
#include "field/field_status.h"
#include "field/intrude_work.h"
#include "gfl/net.h"
#include "gfl/str.h"
#include "save/event_work.h"
#include "save/player_info.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_system.h"

const u16 data_ov010_021700a8[] = {0x97f, 0x980};

const u16 data_ov010_021700ac[] = {
#include "asset/intrude_work_data_ov010_021700ac.inc"
};

// The zones that intrude_field.c (ov018) looks the player's zone up in, and their count
const u16 data_ov010_02170128[] = {
#include "asset/intrude_work_data_ov010_02170128.inc"
};

const u32 data_ov010_021700a4 = NELEMS(data_ov010_02170128);

BOOL func_ov010_02158318(GameSystem *gsys) {
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);

    if (GameData_IsForceSeasonSync(GSYS_GetGameData(gsys)) == TRUE && func_0202648c(commSys)) {
        return FALSE;
    }
    return TRUE;
}

IntrudeComm *func_ov010_02158344(GameCommSys *commSys) {
    IntrudeComm *work = func_02026474(commSys);

    if (func_0200fb24() || GameCommSys_BootCheck(commSys) != GAME_COMM_NO_INTRUDE || func_02026460(commSys) == TRUE ||
        work == NULL) {
        return NULL;
    }
    return work;
}

u32 func_ov010_02158378(GameCommSys *commSys) {
    IntrudeComm *work = func_ov010_02158344(commSys);

    if (work == NULL) {
        return func_02012b58(getBasePlayerBlk(commSys));
    }
    return work->unk_911;
}

BOOL func_ov010_0215839c(IntrudeComm *work, u32 mask, u8 unused) {
    if (mask & work->unk_93F_4) {
        work->unk_93F_4 ^= 1;
        return TRUE;
    }
    return FALSE;
}

PassPowerLevel *func_ov010_021583cc(GameData *gameData, u32 netId) {
    if (netId == func_02012b58(gameData)) {
        return func_020127b8(gameData);
    }
    return func_020127ac(gameData, netId);
}

PlayerInfo *func_ov010_021583ec(GameData *gameData, u32 netId) {
    if (netId == func_02012b58(gameData)) {
        return GetGameDataPlayerInfo(gameData);
    }
    return func_02012950(gameData, netId);
}

IntrudeCommPlayer *func_ov010_0215840c(IntrudeComm *work, u32 index) {
    return work->unk_1AC.players + index;
}

u8 func_ov010_0215841c(GameData *gameData) {
    return func_02012b80(gameData);
}

u8 func_ov010_02158428(IntrudeComm *work) {
    return work->unk_90C;
}

void *func_ov010_02158434(IntrudeComm *work) {
    return &work->unk_35C;
}

u32 getGameOrigin(GameCommSys *commSys) {
    GameData *gameData = getBasePlayerBlk(commSys);
    u8 netId;

    if (GameData_IsForceSeasonSync(gameData) == FALSE) {
        return GAME_VERSION;
    }
    netId = func_02012b80(gameData);
    return func_0200855c(func_ov010_021583ec(gameData, netId));
}

u8 getSeasonFromPlayerData(GameCommSys *commSys) {
    GameData *gameData = getBasePlayerBlk(commSys);
    u8 netId;

    if (GameData_IsForceSeasonSync(gameData) == FALSE) {
        return GameData_GetSeason(gameData);
    }
    netId = func_ov010_0215841c(gameData);
    if (netId == func_02012b58(gameData)) {
        return GameData_GetSeason(gameData);
    }
    return func_02012b98(gameData, netId);
}

BOOL func_ov010_021584a4(GameCommSys *commSys, GameData *gameData, u32 dir) {
    IntrudeComm *work = func_ov010_02158344(commSys);
    s32 next;

    if (work == NULL) {
        return TRUE;
    }
    next = work->unk_070.unk_16_0;
    if (dir == 2) {
        next--;
        if (next < 0) {
            next = work->unk_901;
        }
    } else {
        next++;
        if (next >= work->unk_901 + 1) {
            next = 0;
        }
    }
    if (next == func_02012b58(gameData)) {
        return TRUE;
    }
    return FALSE;
}

// The version of a player's game, by its ID where the version isn't Black's or White's
u32 func_ov010_021584f4(u8 version, u32 id) {
    if (version != VERSION_BLACK && version != VERSION_WHITE) {
        if (id & 0x10000) {
            return VERSION_WHITE;
        }
        return VERSION_BLACK;
    }
    return version;
}

u32 func_ov010_0215850c(GameCommSys *commSys, GameData *gameData) {
    u8 netId;
    PlayerInfo *info;
    u8 version;

    func_ov010_02158344(commSys);
    if (func_0201444c(GameData_GetFieldStatus(gameData)) != TRUE) {
        return 0;
    }
    netId = func_ov010_0215841c(gameData);
    if (netId == func_02012b58(gameData)) {
        return 0;
    }
    info = func_02012950(gameData, netId);
    version = func_0200855c(info);
    switch (func_ov010_021584f4(version, getIDAsUInt(info))) {
    case VERSION_WHITE:
        return 1;
    case VERSION_BLACK:
        return 2;
    default:
        return 3;
    }
}

BOOL func_ov010_02158570(u16 zoneId) {
    u32 i;
    for (i = 0; i < NELEMS(data_ov010_021700ac) ? TRUE : FALSE; i++) {
        if (zoneId == data_ov010_021700ac[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov010_02158590(GameData *gameData) {
    EventWork *eventWork = GameData_GetEventWork(gameData);
    u32 i;

    for (i = 0; i < NELEMS(data_ov010_021700a8); i++) {
        if (!EventWork_FlagGet(eventWork, data_ov010_021700a8[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL func_ov010_021585bc(IntrudeComm *work) {
    if (work->unk_940_31 && !func_ov011_02175b14(&work->unk_1AC)) {
        return TRUE;
    }
    return FALSE;
}

u32 func_ov010_021585e0(GameCommSys *commSys) {
    IntrudeComm *work = func_ov010_02158344(commSys);

    if (work == NULL) {
        return 0;
    }
    if (!work->unk_93F_2) {
        if (func_ov010_02158630(commSys) == 3) {
            return 2;
        }
        return 1;
    }
    return 3;
}

void func_ov010_02158614(GameCommSys *commSys) {
    IntrudeComm *work = func_ov010_02158344(commSys);

    if (work != NULL) {
        work->unk_93F_2 = 1;
    }
}

u32 func_ov010_02158630(GameCommSys *commSys) {
    IntrudeComm *work = func_ov010_02158344(commSys);

    if (work == NULL) {
        return 0;
    }
    if (func_ov011_02175b14(&work->unk_1AC) == TRUE) {
        if (func_ov011_02176258(work) == TRUE) {
            return 2;
        }
        if (func_ov010_02158590(getBasePlayerBlk(commSys)) == FALSE) {
            return 1;
        }
        return 3;
    }
    return 1;
}

u32 func_ov010_02158678(GameCommSys *commSys) {
    IntrudeComm *work;
    u32 count = 0;
    u8 i;

    work = func_ov010_02158344(commSys);

    if (work == NULL) {
        return count;
    }
    if (!func_ov011_02175b14(&work->unk_1AC)) {
        return count;
    }
    for (i = 0; i < 3; i++) {
        if (work->unk_08C[i].unk_1A_5 == TRUE) {
            count++;
        }
    }
    if (count == 0) {
        count = 1;
    }
    return count;
}

BOOL func_ov010_021586c8(GameCommSys *commSys, StrBuf *name) {
    IntrudeComm *work = func_ov010_02158344(commSys);
    GameData *gameData = getBasePlayerBlk(commSys);

    if (work == NULL || !func_ov011_02175b14(&work->unk_1AC)) {
        return FALSE;
    }
    textCopy(func_02012950(gameData, func_ov011_02176230(work, &work->unk_1AC))->name, name);
    return TRUE;
}

BOOL func_ov010_0215870c(GameCommSys *commSys) {
    IntrudeComm *work;

    if (GameCommSys_BootCheck(commSys) != GAME_COMM_NO_INTRUDE) {
        return FALSE;
    }
    work = func_02026474(commSys);
    if (work == NULL) {
        return FALSE;
    }
    if (!work->unk_8F8_6) {
        return FALSE;
    }
    if (func_0203cdbc() != TRUE) {
        return TRUE;
    }
    return FALSE;
}

void func_ov010_0215874c(GameCommSys *commSys) {
    IntrudeComm *work = func_ov010_02158344(commSys);

    if (work != NULL) {
        work->unk_904_2 = 0;
    }
}
