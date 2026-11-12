// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/game_comm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/field.h"
#include "field/musical.h"
#include "field/union_comm.h"
#include "field/union_main.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "save/player_info.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "field/mus_comm_func.h"

#define GAME_COMM_NO_UNK2  2
#define GAME_COMM_NO_UNION 3
#define GAME_COMM_NO_MUSICAL 4
#define GAME_COMM_NO_UNK5  5
#define GAME_COMM_NO_MAX   6

// The communications' callbacks, which are in the overlay that each communication is of
void *func_ov010_02164b6c(u32 *seq, void *param);
BOOL func_ov010_02164bc4(u32 *seq, void *param, void *work);
void func_ov010_02164c64(u32 *seq, void *param, void *work);
BOOL func_ov010_02164c20(u32 *seq, void *param, void *work);
BOOL func_ov010_02164c38(u32 *seq, void *param, void *work);

void *func_ov011_02172bc0(u32 *seq, void *param);
BOOL func_ov011_02172cb0(u32 *seq, void *param, void *work);
void func_ov011_02172d70(u32 *seq, void *param, void *work);
BOOL func_ov011_021730ac(u32 *seq, void *param, void *work);
BOOL func_ov011_021731c8(u32 *seq, void *param, void *work);
void func_ov011_02173464(void *param, void *work, Field *field);
void func_ov011_02173558(void *param, void *work, Field *field);

void *func_ov012_02172c4c(u32 *seq, void *param);
BOOL func_ov012_02172c78(u32 *seq, void *param, void *work);
void func_ov012_02172e48(u32 *seq, void *param, void *work);
BOOL func_ov012_02172cac(u32 *seq, void *param, void *work);
BOOL func_ov012_02172ce8(u32 *seq, void *param, void *work);

// The game's communication system: one at a time, it boots a communication (a GameCommNo), runs its callbacks through
// a sequence of states, and ends it on request. It also logs the last players met over a communication

enum {
    GAME_COMM_SEQ_INIT,
    GAME_COMM_SEQ_INIT_WAIT,
    GAME_COMM_SEQ_MAIN,
    GAME_COMM_SEQ_EXIT,
    GAME_COMM_SEQ_EXIT_WAIT,
    GAME_COMM_SEQ_END,
};

typedef struct {
    // Set to 0 at each change of seq; the callbacks get it as their sequence
    u32 subSeq;
    u8 seq;
    u8 changed;
} GameCommSeq;

// A player met over a communication
typedef struct {
    u16 beaconType;
    u8 netId;
    u8 valid : 1;
    u8 gender1 : 1;
    u8 gender2 : 1;
    StrBuf *name1;
    StrBuf *name2;
} GameCommLogEntry;

// A ring of the last players met
typedef struct {
    GameCommLogEntry entries[GAME_COMM_LOG_COUNT];
    u8 start;
    u8 end;
} GameCommLog;

typedef struct {
    u16 unk0;
    u16 unk2;
    u8 unk4;
    u8 unk5;
    u8 unk6;
} GameCommUnk24;

struct GameCommSys {
    GameData *gameData;
    // An exit requested while booting
    u8 exitReq : 1;
    u8 exitReqAgain : 1;
    u8 unkFlag2 : 1;
    u8 unkFlag3 : 1;
    u8 commNo;
    u8 lastCommNo;
    u32 unk8;
    GameCommSeq seq;
    void *param;
    void *work;
    void *exitCallbackArg;
    GameCommExitCallback exitCallback;
    GameCommUnk24 unk24[3];
    GameCommLog log;
};

// A communication's callbacks. param is what it was booted with, and work is what init returned
typedef struct {
    void *(*init)(u32 *seq, void *param);
    BOOL (*initWait)(u32 *seq, void *param, void *work);
    void (*main)(u32 *seq, void *param, void *work);
    BOOL (*exit)(u32 *seq, void *param, void *work);
    BOOL (*exitWait)(u32 *seq, void *param, void *work);
    void (*fieldCreate)(void *param, void *work, Field *field);
    void (*fieldDelete)(void *param, void *work, Field *field);
} GameCommFuncs;

void func_0202640c(GameCommSys *comm, GameCommExitCallback callback, void *callbackArg);
static void func_02026490(GameCommSeq *seq, u8 next);
void func_020264f4(GameCommSys *comm);
void func_02026504(GameCommSys *comm, u32 netId1, u32 netId2, u32 beaconType);

// pokebw2's sGameCommFuncs
static const GameCommFuncs data_0209fb5c[GAME_COMM_NO_MAX] = {
    [GAME_COMM_NO_NULL] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    [GAME_COMM_NO_BEACON_SEARCH] = {
        func_ov010_02164b6c,
        func_ov010_02164bc4,
        func_ov010_02164c64,
        func_ov010_02164c20,
        func_ov010_02164c38,
        NULL,
        NULL,
    },
    [GAME_COMM_NO_UNK2] = {
        func_ov011_02172bc0,
        func_ov011_02172cb0,
        func_ov011_02172d70,
        func_ov011_021730ac,
        func_ov011_021731c8,
        func_ov011_02173464,
        func_ov011_02173558,
    },
    [GAME_COMM_NO_UNION] = {
        func_ov012_02172c4c,
        func_ov012_02172c78,
        func_ov012_02172e48,
        func_ov012_02172cac,
        func_ov012_02172ce8,
        func_ov012_02173874,
        func_ov012_021738d4,
    },
    [GAME_COMM_NO_MUSICAL] = {
        func_ov132_021fc694,
        NULL,
        func_ov132_021fc7f8,
        func_ov132_021fc6ec,
        func_ov132_021fc7dc,
        NULL,
        NULL,
    },
    [GAME_COMM_NO_UNK5] = {
        func_ov010_02164b6c,
        func_ov010_02164bc4,
        func_ov010_02164c64,
        func_ov010_02164c20,
        func_ov010_02164c38,
        NULL,
        NULL,
    },
};

// pokebw2's GameCommSys_Create
GameCommSys *func_020261e4(HeapID heapId, GameData *gameData) {
    GameCommSys *comm = GFL_HeapAllocate(heapId, sizeof(GameCommSys), TRUE, "game_comm.c", 224);
    int i;
    PlayerInfo *info;
    GameCommLog *log;
    GameCommLogEntry *entry;

    comm->gameData = gameData;
    info = GetGameDataPlayerInfo(gameData);
    log = &comm->log;
    for (i = 0; i < GAME_COMM_LOG_COUNT; i++) {
        entry = &log->entries[i];
        entry->name1 = GFL_StrBufCreate(8, heapId);
        entry->name2 = GFL_StrBufCreate(8, heapId);
        textCopy(info->name, entry->name1);
        textCopy(info->name, entry->name2);
    }
    return comm;
}

void FreeGameComm(GameCommSys *comm) {
    int i;
    GameCommLog *log = &comm->log;
    GameCommLogEntry *entry;

    for (i = 0; i < GAME_COMM_LOG_COUNT; i++) {
        entry = &log->entries[i];
        GFL_StrBufFree(entry->name1);
        GFL_StrBufFree(entry->name2);
    }
    GFL_HeapFree(comm);
}

// pokebw2's GameCommSys_Main
void func_0202627c(GameCommSys *comm) {
    GameCommSeq *seq;
    const GameCommFuncs *funcs;
    GameCommExitCallback callback;
    void *callbackArg;
    BOOL exitReqAgain;

    if (comm == NULL || comm->commNo == GAME_COMM_NO_NULL) {
        return;
    }
    seq = &comm->seq;
    funcs = &data_0209fb5c[comm->commNo];
    if (seq->changed == TRUE) {
        seq->subSeq = 0;
        seq->changed = FALSE;
    }
    switch (seq->seq) {
    case GAME_COMM_SEQ_INIT:
        if (funcs->init != NULL) {
            comm->work = funcs->init(&seq->subSeq, comm->param);
        }
        comm->lastCommNo = comm->commNo;
        comm->unk8 = 0;
        func_02026490(seq, GAME_COMM_SEQ_INIT_WAIT);
        break;
    case GAME_COMM_SEQ_INIT_WAIT:
        if (funcs->initWait == NULL || funcs->initWait(&seq->subSeq, comm->param, comm->work) == TRUE) {
            func_02026490(seq, GAME_COMM_SEQ_MAIN);
        }
        break;
    case GAME_COMM_SEQ_MAIN:
        if (comm->exitReq == TRUE) {
            comm->exitReq = FALSE;
            func_02026490(&comm->seq, GAME_COMM_SEQ_EXIT);
        } else {
            funcs->main(&seq->subSeq, comm->param, comm->work);
        }
        break;
    case GAME_COMM_SEQ_EXIT:
        if (funcs->exit != NULL) {
            if (funcs->exit(&seq->subSeq, comm->param, comm->work) == TRUE) {
                func_02026490(seq, GAME_COMM_SEQ_EXIT_WAIT);
            }
        } else {
            func_02026490(seq, GAME_COMM_SEQ_EXIT_WAIT);
        }
        break;
    case GAME_COMM_SEQ_EXIT_WAIT:
        if (funcs->exitWait != NULL && funcs->exitWait(&seq->subSeq, comm->param, comm->work) != TRUE) {
            break;
        }
        func_02026490(seq, GAME_COMM_SEQ_END);
        // fallthrough
    case GAME_COMM_SEQ_END:
        comm->work = NULL;
        comm->commNo = GAME_COMM_NO_NULL;
        callback = comm->exitCallback;
        callbackArg = comm->exitCallbackArg;
        exitReqAgain = comm->exitReqAgain;
        comm->exitCallback = NULL;
        comm->exitCallbackArg = NULL;
        comm->exitReqAgain = FALSE;
        if (callback != NULL) {
            callback(callbackArg, exitReqAgain);
        }
        break;
    }
}

// pokebw2's GameCommSys_FieldCreate
void func_02026388(GameCommSys *comm, Field *field) {
    void (*fieldCreate)(void *param, void *work, Field *field);

    if (comm->seq.seq != GAME_COMM_SEQ_END) {
        fieldCreate = data_0209fb5c[comm->commNo].fieldCreate;
        if (fieldCreate != NULL) {
            fieldCreate(comm->param, comm->work, field);
        }
    }
}

// pokebw2's GameCommSys_FieldDelete
void func_020263b0(GameCommSys *comm, Field *field) {
    void (*fieldDelete)(void *param, void *work, Field *field);

    if (comm->seq.seq != GAME_COMM_SEQ_END) {
        fieldDelete = data_0209fb5c[comm->commNo].fieldDelete;
        if (fieldDelete != NULL) {
            fieldDelete(comm->param, comm->work, field);
        }
    }
}

// pokebw2's GameCommSys_Boot
void func_020263d8(GameCommSys *comm, u8 commNo, void *param) {
    comm->param = param;
    comm->exitReqAgain = FALSE;
    comm->commNo = commNo;
    comm->exitCallback = NULL;
    comm->exitCallbackArg = NULL;
    MI_CpuFill8(&comm->seq, 0, sizeof(GameCommSeq));
    func_020264f4(comm);
}

// pokebw2's GameCommSys_ExitReq
void func_02026400(GameCommSys *comm) {
    func_0202640c(comm, NULL, NULL);
}

// pokebw2's GameCommSys_ExitReqCallback
void func_0202640c(GameCommSys *comm, GameCommExitCallback callback, void *callbackArg) {
    if (comm->seq.seq == GAME_COMM_SEQ_MAIN) {
        func_02026490(&comm->seq, GAME_COMM_SEQ_EXIT);
        comm->exitCallback = callback;
        comm->exitCallbackArg = callbackArg;
    } else if (comm->seq.seq < GAME_COMM_SEQ_MAIN) {
        // Still booting: func_0202627c exits when it reaches GAME_COMM_SEQ_MAIN
        comm->exitReq = TRUE;
        comm->exitCallback = callback;
        comm->exitCallbackArg = callbackArg;
    } else {
        comm->exitReqAgain = TRUE;
        if (comm->exitCallback == NULL) {
            comm->exitCallback = callback;
            comm->exitCallbackArg = callbackArg;
        }
    }
}

u8 GameCommSys_BootCheck(GameCommSys *comm) {
    if (comm->commNo != GAME_COMM_NO_NULL) {
        return comm->commNo;
    }
    return GAME_COMM_NO_NULL;
}

// pokebw2's GameCommSys_IsTransitioning
BOOL func_02026460(GameCommSys *comm) {
    if (comm->commNo != GAME_COMM_NO_NULL && comm->seq.seq != GAME_COMM_SEQ_MAIN) {
        return TRUE;
    }
    return FALSE;
}

void *func_02026474(GameCommSys *comm) {
    return comm->work;
}

GameData *getBasePlayerBlk(GameCommSys *comm) {
    return comm->gameData;
}

u8 func_0202647c(GameCommSys *comm) {
    return comm->lastCommNo;
}

void func_02026480(GameCommSys *comm, u32 value) {
    comm->unk8 = value;
}

void func_02026484(GameCommSys *comm) {
    comm->unk8 = 0;
}

u32 func_0202648c(GameCommSys *comm) {
    return comm->unk8;
}

// pokebw2's GameComm_SetSeq
static void func_02026490(GameCommSeq *seq, u8 next) {
    seq->seq = next;
    seq->changed = TRUE;
}

void func_02026498(GameCommSys *comm, BOOL flag) {
    comm->unkFlag2 = flag;
}

u8 func_020264ac(GameCommSys *comm) {
    return comm->unkFlag2;
}

void func_020264b4(GameCommSys *comm, u32 netId, u16 unk0, u8 unk5, u32 unk4, BOOL noLog) {
    GameCommUnk24 *entry = &comm->unk24[netId];
    u16 old;

    entry->unk4 = unk4;
    old = comm->unk24[netId].unk0;
    if (old != unk0 || entry->unk5 != unk5) {
        if (old != unk0) {
            entry->unk2 = old;
            entry->unk0 = unk0;
        }
        if (entry->unk5 != unk5) {
            entry->unk6 = entry->unk5;
            entry->unk5 = unk5;
            if (!noLog) {
                func_020266c0(comm, netId, unk5);
            }
        }
    }
}

// pokebw2's GameCommSys_ClearUnk24
void func_020264f4(GameCommSys *comm) {
    MI_CpuFill8(comm->unk24, 0, sizeof(comm->unk24));
}

// pokebw2's GameCommSys_LogPlayers
void func_02026504(GameCommSys *comm, u32 netId1, u32 netId2, u32 beaconType) {
    GameCommLog *log = &comm->log;
    PlayerInfo *info1;
    PlayerInfo *info2;
    GameCommLogEntry *entry;

    if (netId1 == func_02012b58(comm->gameData)) {
        info1 = GetGameDataPlayerInfo(comm->gameData);
    } else {
        info1 = func_02012950(comm->gameData, netId1);
    }
    if (netId2 == func_02012b58(comm->gameData)) {
        info2 = GetGameDataPlayerInfo(comm->gameData);
    } else {
        info2 = func_02012950(comm->gameData, netId2);
    }
    if (func_020084bc(info1) == TRUE || func_020084bc(info2) == TRUE) {
        return;
    }

    entry = &log->entries[log->end];
    if (entry->valid == TRUE) {
        // The log is full: the oldest entry is overwritten
        log->start++;
    }
    entry->netId = netId1;
    entry->beaconType = beaconType;
    entry->valid = TRUE;
    textCopy(info1->name, entry->name1);
    entry->gender1 = getTrainerGender(info1);
    textCopy(info2->name, entry->name2);
    entry->gender2 = getTrainerGender(info2);
    log->end++;
    if (log->end >= GAME_COMM_LOG_COUNT) {
        log->end = 0;
    }
    if (log->start >= GAME_COMM_LOG_COUNT) {
        log->start = 0;
    }
}

u8 func_02026608(GameCommSys *comm, GameCommLogOut *out) {
    GameCommLog *log = &comm->log;
    GameCommLogEntry *entry = &log->entries[log->start];

    if (entry->valid == FALSE) {
        return 0;
    }

    MI_CpuFill8(out, 0, sizeof(GameCommLogOut));
    out->name[0] = entry->name1;
    out->unk8[0] = 0;
    out->gender[0] = entry->gender1;
    out->name[1] = entry->name2;
    out->unk8[1] = 1;
    out->gender[1] = entry->gender2;
    out->beaconType = entry->beaconType;

    entry->valid = FALSE;
    log->start++;
    if (log->start >= GAME_COMM_LOG_COUNT) {
        log->start = 0;
    }
    return 1;
}

void func_02026680(GameCommSys *comm) {
    int i;
    for (i = 0; i < GAME_COMM_LOG_COUNT; i++) {
        comm->log.entries[i].valid = FALSE;
    }
    comm->log.end = 0;
    comm->log.start = 0;
}



void func_020266ac(GameCommSys *comm, u32 netId1, u32 netId2) {
    u16 beaconType = netId1 == netId2 ? 0x27 : 0x26;

    func_02026504(comm, netId1, netId2, beaconType);
}

void func_020266c0(GameCommSys *comm, u32 netId1, u32 netId2) {
    u16 beaconType = netId1 == netId2 ? 0x1e : 0x1d;
    func_02026504(comm, netId1, netId2, beaconType);
}

// Logs a player met over the communication (pokebw2's GameCommSys_LogMissionJoined, with the ID given)
void func_020266d4(GameCommSys *comm, u32 netId) {
    func_02026504(comm, netId, netId, 0x1f);
}
