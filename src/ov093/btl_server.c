// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btl_server.c, ported by tools/port_file.py.
#include "types.h"
#include "battle/btl_action.h"
#include "battle/btl_adapter.h"
#include "battle/btl_calc.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_rec.h"
#include "battle/btl_server.h"
#include "battle/btl_server_flow.h"
#include "constants/pokemon.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "pml/poke_party.h"
#include "system/game_beacon.h"
#include "system/game_comm.h"

BtlServer *func_ov093_021ba9c8(BtlMainModule *mainModule, const MATHRandContext32 *rand, BtlPokeCon *pokeCon,
                               u32 a3, HeapID heapId) {
    BtlServer *server = GFL_HeapAllocate(heapId, sizeof(BtlServer), TRUE, "btl_server.c", 0xad);
    int i;

    server->mainModule = mainModule;
    server->pokeCon = pokeCon;
    server->heapId = heapId;
    server->queuePtr = &server->queue;
    server->state = 0;
    server->flow = NULL;
    server->count = 0;
    server->unkCBF = 0;
    server->unk4C = a3;
    server->rand = *rand;
    server->strBuf = GFL_StrBufCreate(0x20, heapId);
    func_ov093_021d787c(rand);
    for (i = 0; i < 4; i++) {
        func_ov093_021bb828(&server->clients[i]);
    }
    server->flow = func_ov093_021bb968(server, server->mainModule, server->pokeCon, &server->queue, server->unk4C,
                                       server->heapId);
    server->clientIdList = func_ov093_021bc550(server->flow);
    func_ov093_021babec(server, func_ov093_021bac04);
    return server;
}

void func_ov093_021baa94(BtlServer *server, BtlAdapter *adapter, u8 clientId, u8 numCoverPos) {
    BattleParty *party = GetPartyData(server->pokeCon, clientId);

    func_ov093_021bb840(&server->clients[clientId], clientId, adapter, TRUE);
    func_ov093_021bb848(&server->clients[clientId], party, numCoverPos);
}

void func_ov093_021baacc(BtlServer *server, u32 mode, void *netHandle, u8 clientId, u8 numCoverPos) {
    BattleParty *party = GetPartyData(server->pokeCon, clientId);
    BtlAdapter *adapter = func_ov093_021eea58(netHandle, clientId, TRUE, server->heapId);

    func_ov093_021bb840(&server->clients[clientId], clientId, adapter, FALSE);
    func_ov093_021bb848(&server->clients[clientId], party, numCoverPos);
}

void func_ov093_021bab10(BtlServer *server, u8 clientId, u8 numCoverPos) {
    BattleParty *party = GetPartyData(server->pokeCon, clientId);

    func_ov093_021bb840(&server->clients[clientId], clientId, NULL, FALSE);
    func_ov093_021bb848(&server->clients[clientId], party, numCoverPos);
}

void func_ov093_021bab40(BtlServer *server) {
    func_ov093_021babec(server, func_ov093_021bac04);
    func_ov093_021bb7fc(server);
    func_ov093_021bb9d0(server->flow);
}

void func_ov093_021bab5c(BtlServer *server) {
    int i;

    GFL_StrBufFree(server->strBuf);
    func_ov093_021bbb48(server->flow);
    for (i = 0; i < 4; i++) {
        if (server->clients[i].clientId != 0xff && server->clients[i].adapter != NULL
            && !server->clients[i].isAttached) {
            func_ov093_021eeae0(server->clients[i].adapter);
        }
    }
    GFL_HeapFree(server);
}

BOOL func_ov093_021bab9c(BtlServer *server) {
    if (server->state != 2) {
        if (server->proc(server, &server->seq)) {
            func_ov093_021bb724(server, 9, server->clientIdList, sizeof(BtlClientIDList));
            server->state = 2;
        }
    } else if (func_ov093_021bb798(server)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021babdc(BtlServer *server, BtlClientIDList *out) {
    *out = *server->clientIdList;
}

void func_ov093_021babec(BtlServer *server, BtlServerProc proc) {
    server->proc = proc;
    server->seq = 0;
}

void func_ov093_021babf4(BtlServer *server) {
    func_ov093_021babec(server, func_ov093_021bacac);
}

BOOL func_ov093_021bac04(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021bb718(server, 1);
        (*seq)++;
    case 1:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            if (BtlSetup_GetBattleType(server->mainModule) != 4) {
                func_ov093_021bb5fc(server, func_ov093_021bbb60(server->flow));
                (*seq)++;
            } else {
                func_ov093_021babec(server, func_ov093_021bb280);
            }
        }
        break;
    case 2:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            (*seq)++;
        }
        break;
    case 3:
        func_ov093_021bb724(server, 8, server->queuePtr->buffer, server->queuePtr->writePtr);
        (*seq)++;
        break;
    case 4:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            func_ov093_021babf4(server);
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021bacac(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0:
        if (func_ov093_021bc390(server->flow)) {
            func_ov093_021bb724(server, 8, server->queuePtr->buffer, server->queuePtr->writePtr);
            (*seq)++;
        } else {
            *seq += 2;
        }
        break;
    case 1:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            (*seq)++;
        }
        break;
    case 2:
        func_ov093_021bb718(server, 3);
        (*seq)++;
        break;
    case 3:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            if (func_ov093_021ba5d8(server->mainModule)) {
                func_ov093_021babec(server, func_ov093_021bb208);
            } else if (func_ov093_021ba618(server->mainModule)) {
                func_ov093_021babec(server, func_ov093_021bb280);
            } else {
                func_ov093_021bb6a8(server);
                if (func_ov093_021bb628(server, 1, func_ov093_021bae5c(server))) {
                    (*seq)++;
                } else {
                    *seq += 2;
                }
            }
        }
        break;
    case 4:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            (*seq)++;
        }
        break;
    case 5:
        func_ov093_021bbc34(server->flow);
        func_ov093_021bb7d0(server);
        (*seq)++;
    case 6:
        server->flowResult = func_ov093_021bbc44(server->flow, &server->clientActions);
        if (func_ov093_021b86c0(server->mainModule)) {
            func_ov093_021bb56c(&server->clientActions);
        }
        func_ov093_021bb724(server, 8, server->queuePtr->buffer, server->queuePtr->writePtr);
        (*seq)++;
        break;
    case 7:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            switch (server->flowResult) {
            case 0:
                func_ov093_021babf4(server);
                break;
            case 3:
                *seq = 6;
                break;
            case 2: {
                u32 battleType = BtlSetup_GetBattleType(server->mainModule);
                u32 battleStyle = BtlSetup_GetBattleStyle(server->mainModule);

                if (battleType == 0 && battleStyle == 0) {
                    func_ov093_021babec(server, func_ov093_021bae60);
                } else {
                    func_ov093_021babec(server, func_ov093_021baef8);
                }
                break;
            }
            case 1:
                func_ov093_021babec(server, func_ov093_021bb0e0);
                break;
            case 6:
                func_ov093_021b9120(server->mainModule, func_ov093_021bc55c(server->flow));
                func_ov093_021babec(server, func_ov093_021bb280);
                return FALSE;
            case 4:
                func_ov093_021b916c(server->mainModule, func_ov093_021c84d8(server->flow));
                func_ov093_021babec(server, func_ov093_021bb280);
                return FALSE;
            case 5:
            default:
                return TRUE;
            }
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021bae5c(BtlServer *server) {
    return TRUE;
}

BOOL func_ov093_021bae60(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021bb718(server, 7);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021bb798(server)) {
            u8 clientId = GetPlayerClientID(server->mainModule);
            u8 *reply;

            func_ov093_021bb7d0(server);
            reply = func_ov093_021eeb64(server->clients[clientId].adapter, NULL);
            if (*reply == 0) {
                func_ov093_021babec(server, func_ov093_021baef8);
                break;
            }
            server->flowResult = func_ov093_021bc360(server->flow);
            func_ov093_021bb724(server, 8, server->queuePtr->buffer, server->queuePtr->writePtr);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            if (server->flowResult) {
                return TRUE;
            }
            func_ov093_021babec(server, func_ov093_021baef8);
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021baef8(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021bb724(server, 5, server->posList, server->count);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021bb798(server)) {
            if (DoesSwitchModeNeedConfirming(server)) {
                server->switchModeMonId = GetNextEnemyForSwitchMode(server);
                func_ov093_021bb768(server, 4, 0, &server->switchModeMonId, 1);
            }
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            if (func_ov093_021ba618(server->mainModule)) {
                func_ov093_021babec(server, func_ov093_021bb280);
            } else {
                func_ov093_021bb6a8(server);
                if (func_ov093_021bb628(server, 2, FALSE)) {
                    (*seq)++;
                } else {
                    *seq += 2;
                }
            }
        }
        break;
    case 3:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            (*seq)++;
        }
        break;
    case 4:
        func_ov093_021bb7d0(server);
        func_ov093_021bbd80(server->flow);
        (*seq)++;
    case 5:
        server->flowResult = func_ov093_021bbd8c(server->flow, &server->clientActions);
        func_ov093_021bb724(server, 8, server->queuePtr->buffer, server->queuePtr->writePtr);
        (*seq)++;
        break;
    case 6:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            switch (server->flowResult) {
            case 2:
                *seq = 0;
                break;
            case 3:
                *seq = 5;
                break;
            case 4:
                func_ov093_021b916c(server->mainModule, func_ov093_021c84d8(server->flow));
                func_ov093_021babec(server, func_ov093_021bb280);
                break;
            case 5:
                return TRUE;
            default:
                func_ov093_021babf4(server);
                break;
            }
        }
        break;
    }
    return FALSE;
}

// Function names from swan.
BOOL DoesSwitchModeNeedConfirming(BtlServer *server) {
    u32 i;
    BattleParty *party;

    if (IsSwitchMode(server->mainModule) && server->count != 0) {
        for (i = 0; i < server->count; i++) {
            if (func_ov093_021b8de4(server->mainModule, server->posList[i]) == 0) {
                return FALSE;
            }
        }
        party = GetPartyData(server->pokeCon, GetPlayerClientID(server->mainModule));
        if (GetAlivePartyCount(party) < 2) {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

u8 GetNextEnemyForSwitchMode(BtlServer *server) {
    BtlServerClient *client;
    BattleAction *action;
    u8 slot;

    client = &server->clients[1];
    if (IsSwitchModeEnabled(client)) {
        action = func_ov093_021eeb64(client->adapter, NULL);
        if (BattleAction_GetAction(action) == 3) {
            slot = action->change.slot;
            return GetMonID(GetClientMonData(server->pokeCon, 1, slot));
        }
    }
    return 31;
}

BOOL func_ov093_021bb0e0(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021bb724(server, 6, server->posList, server->count);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            if (func_ov093_021ba618(server->mainModule)) {
                func_ov093_021babec(server, func_ov093_021bb280);
            } else {
                func_ov093_021bb6a8(server);
                if (func_ov093_021bb628(server, 3, FALSE)) {
                    (*seq)++;
                } else {
                    *seq += 2;
                }
            }
        }
        break;
    case 2:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            (*seq)++;
        }
        break;
    case 3:
        func_ov093_021bb7d0(server);
        func_ov093_021bbd20(server->flow);
        (*seq)++;
    case 4:
        BtlServerCmdQueue_Init(server->queuePtr);
        server->flowResult = func_ov093_021bbd2c(server->flow, &server->clientActions);
        func_ov093_021bb724(server, 8, server->queuePtr->buffer, server->queuePtr->writePtr);
        (*seq)++;
        break;
    case 5:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            switch (server->flowResult) {
            case 1:
                *seq = 0;
                break;
            case 3:
                *seq = 4;
                break;
            case 2:
                func_ov093_021babec(server, func_ov093_021baef8);
                break;
            case 4:
                func_ov093_021b916c(server->mainModule, func_ov093_021c84d8(server->flow));
                func_ov093_021babec(server, func_ov093_021bb280);
                break;
            case 5:
                return TRUE;
            default:
                func_ov093_021babf4(server);
                break;
            }
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021bb208(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0: {
        u32 size;
        void *data = func_ov093_021ee9a4(&server->recTool, &size);

        func_ov093_021bb724(server, 10, data, size);
        (*seq)++;
        break;
    }
    case 1:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            func_ov093_021bb718(server, 0x10);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            func_ov093_021b916c(server->mainModule, func_ov093_021c84d8(server->flow));
            func_ov093_021babec(server, func_ov093_021bb280);
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021bb280(BtlServer *server, int *seq) {
    u32 result = func_ov093_021b917c(server->mainModule);

    server->exitData.result = result;
    server->exitData.playerClientId = GetPlayerClientID(server->mainModule);
    switch (BtlSetup_GetBattleType(server->mainModule)) {
    case 1:
        func_ov093_021babec(server, func_ov093_021bb43c);
        break;
    case 2:
        func_ov093_021babec(server, func_ov093_021bb484);
        break;
    case 3:
        func_ov093_021babec(server, func_ov093_021bb3f8);
        break;
    case 0:
        if (result == 1) {
            func_ov093_021babec(server, func_ov093_021bb368);
        } else if (result == 0 || result == 2) {
            func_ov093_021babec(server, func_ov093_021bb3b0);
        } else {
            func_ov093_021babec(server, func_ov093_021bb320);
        }
        break;
    default:
        if (result == 1) {
            GFL_SndBGMPlay(func_ov093_021b86dc(server->mainModule), 0xffff);
        }
        func_ov093_021babec(server, func_ov093_021bb320);
        break;
    }
    return FALSE;
}

BOOL func_ov093_021bb320(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0: {
        BOOL done = FALSE;

        if (!func_ov093_021b9114(server->mainModule)) {
            if ((GCTX_HIDGetPressedKeys() & 3) || func_02035c60()) {
                done = TRUE;
            }
        } else {
            done = TRUE;
        }
        if (done) {
            (*seq)++;
            return TRUE;
        }
        break;
    }
    case 1:
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov093_021bb368(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021bb724(server, 0xe, &server->exitData, sizeof(server->exitData));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            (*seq)++;
        }
        break;
    case 2:
        func_ov093_021babec(server, func_ov093_021bb320);
        break;
    }
    return FALSE;
}

BOOL func_ov093_021bb3b0(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021bb724(server, 0xf, &server->exitData, sizeof(server->exitData));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            (*seq)++;
        }
        break;
    case 2:
        func_ov093_021babec(server, func_ov093_021bb320);
        break;
    }
    return FALSE;
}

BOOL func_ov093_021bb3f8(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021bb724(server, 0xd, &server->exitData, sizeof(server->exitData));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            (*seq)++;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021bb43c(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021bb724(server, 0xb, &server->exitData, sizeof(server->exitData));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            (*seq)++;
        }
        break;
    case 2:
        func_ov093_021babec(server, func_ov093_021bb320);
        break;
    }
    return FALSE;
}

BOOL func_ov093_021bb484(BtlServer *server, int *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021bb724(server, 0xc, &server->exitData, sizeof(server->exitData));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021bb798(server)) {
            func_ov093_021bb7d0(server);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov093_021b86c0(server->mainModule)) {
            func_ov093_021babec(server, func_ov093_021bb320);
            break;
        }
        return TRUE;
    }
    return FALSE;
}

// Reads the actions of a recorded turn
void func_ov093_021bb4e0(BtlServer *server, const void *data, u32 size) {
    BattleAction actions[3];
    u32 pos;
    u8 clientId;
    u8 count;
    u8 i;

    pos = 0;
    for (i = 0; i < 4; i++) {
        server->clientActions.count[i] = 0;
    }
    func_ov093_021ee9b4(&server->recTool, data, size);
    while (func_ov093_021ee9e4(&server->recTool, &pos, &clientId, &count, actions)) {
        for (i = 0; i < count; i++) {
            server->clientActions.actions[clientId][i] = actions[i];
        }
        server->clientActions.count[clientId] = count;
    }
}

void func_ov093_021bb56c(BtlClientActions *clientActions) {
}

// Whether running the step mode writes commands different from data
BOOL func_ov093_021bb570(BtlServer *server, u32 mode, const void *data, u32 size) {
    BtlServerCmdQueue_Init(server->queuePtr);
    switch (mode) {
    case 4:
        func_ov093_021bbb60(server->flow);
        break;
    case 1:
        func_ov093_021bbc34(server->flow);
        func_ov093_021bbc44(server->flow, &server->clientActions);
        break;
    case 2:
        func_ov093_021bbd80(server->flow);
        func_ov093_021bbd8c(server->flow, &server->clientActions);
        break;
    case 3:
        func_ov093_021bbd20(server->flow);
        func_ov093_021bbd2c(server->flow, &server->clientActions);
        break;
    }
    if (server->queuePtr->writePtr == size && GFL_STD_MemCmp(server->queuePtr->buffer, data, size) == 0) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021bb5fc(BtlServer *server, u32 value) {
    u32 size;
    void *data;

    func_ov093_021ee8b4(&server->recTool, TRUE);
    data = func_ov093_021ee8d8(&server->recTool, &size, value);
    func_ov093_021bb724(server, 10, data, size);
    return TRUE;
}

BOOL func_ov093_021bb628(BtlServer *server, u8 value, BOOL flag) {
    u32 size;
    void *data = func_ov093_021bb650(server, value, flag, &size);

    if (data != NULL) {
        func_ov093_021bb724(server, 10, data, size);
        return TRUE;
    }
    return FALSE;
}

void *func_ov093_021bb650(BtlServer *server, u8 value, BOOL flag, u32 *size) {
    u32 i;

    func_ov093_021ee8b4(&server->recTool, flag);
    for (i = 0; i < 4; i++) {
        if (server->clients[i].clientId != 0xff) {
            u32 actionSize;
            void *actions = func_ov093_021eeb64(server->clients[i].adapter, &actionSize);

            func_ov093_021ee8f4(&server->recTool, i, actions, actionSize / sizeof(BattleAction));
        }
    }
    return func_ov093_021ee96c(&server->recTool, value, size);
}

void func_ov093_021bb6a8(BtlServer *server) {
    u32 i;

    for (i = 0; i < 4; i++) {
        BtlServerClient *client = &server->clients[i];

        server->clientActions.count[i] = 0;
        if (IsSwitchModeEnabled(client)) {
            u32 size;
            BattleAction *actions = func_ov093_021eeb64(client->adapter, &size);
            u32 j;
            u32 count = size / sizeof(BattleAction);

            for (j = 0; j < count; j++) {
                server->clientActions.actions[i][j] = *actions++;
                server->clientActions.count[i]++;
            }
        }
    }
}


void func_ov093_021bb718(BtlServer *server, u32 cmd) {
    func_ov093_021bb724(server, cmd, NULL, 0);
}

void func_ov093_021bb724(BtlServer *server, u32 cmd, const void *data, u32 size) {
    int i;

    func_ov093_021eeae8();
    for (i = 0; i < 4; i++) {
        if (IsSwitchModeEnabled(&server->clients[i])) {
            func_ov093_021eeb00(server->clients[i].adapter, cmd, data, size);
        }
    }
    func_ov093_021eeafc();
}

void func_ov093_021bb768(BtlServer *server, u32 cmd, u8 clientId, const void *data, u32 size) {
    BtlServerClient *client = &server->clients[clientId];

    if (IsSwitchModeEnabled(client)) {
        func_ov093_021eeb70(client->adapter);
        func_ov093_021eeb00(client->adapter, cmd, data, size);
    }
}


BOOL func_ov093_021bb798(BtlServer *server) {
    BOOL done = TRUE;
    int i;

    for (i = 0; i < 4; i++) {
        if (IsSwitchModeEnabled(&server->clients[i]) && !func_ov093_021eeb2c(server->clients[i].adapter)) {
            done = FALSE;
        }
    }
    return done;
}

void func_ov093_021bb7d0(BtlServer *server) {
    int i;

    for (i = 0; i < 4; i++) {
        if (IsSwitchModeEnabled(&server->clients[i])) {
            func_ov093_021eeb70(server->clients[i].adapter);
        }
    }
}

void func_ov093_021bb7fc(BtlServer *server) {
    int i;

    for (i = 0; i < 4; i++) {
        if (IsSwitchModeEnabled(&server->clients[i])) {
            func_ov093_021eead0(server->clients[i].adapter);
        }
    }
}

void func_ov093_021bb828(BtlServerClient *client) {
    client->clientId = 0xff;
}

// Function name from swan.
BOOL IsSwitchModeEnabled(BtlServerClient *client) {
    if (client->clientId != 0xff) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021bb840(BtlServerClient *client, u8 clientId, BtlAdapter *adapter, u8 isAttached) {
    client->clientId = clientId;
    client->adapter = adapter;
    client->isAttached = isAttached;
}

void func_ov093_021bb848(BtlServerClient *client, BattleParty *party, u8 numCoverPos) {
    client->party = party;
    client->numMons = GetNumMonsInParty(party);
    client->numCoverPos = numCoverPos;
}

BtlServerClient *func_ov093_021bb85c(BtlServer *server, u8 clientId) {
    if (clientId < 4 && server->clients[clientId].clientId != 0xff) {
        return &server->clients[clientId];
    }
    return NULL;
}

BtlServerClient *func_ov093_021bb878(BtlServer *server, u8 clientId) {
    if (server->clients[clientId].clientId != 0xff) {
        return &server->clients[clientId];
    }
    return NULL;
}

BOOL func_ov093_021bb890(BtlServer *server, u8 clientId) {
    if (clientId < 4) {
        return IsSwitchModeEnabled(&server->clients[clientId]);
    }
    return FALSE;
}

u8 func_ov093_021bb8a8(const BtlClientActions *clientActions, u8 clientId) {
    return clientActions->count[clientId];
}

BattleAction func_ov093_021bb8b0(const BtlClientActions *clientActions, u8 clientId, u8 index) {
    return clientActions->actions[clientId][index];
}

void func_ov093_021bb8bc(BtlServer *server, BattleMon *mon) {
    PokeParty_GetParam(GetSrcData(mon), PKM_PARAM_NICKNAME, server->strBuf);
    func_02027268(server->strBuf);
}

void func_ov093_021bb8dc(BtlServer *server, BattleMon *mon) {
    u16 species = PokeParty_GetParam(GetSrcData(mon), PKM_PARAM_SPECIES, NULL);
    func_020271dc(species);
    func_ov093_021ba1b4(server->mainModule, 7);
    func_ov093_021ba1b4(server->mainModule, 0x54);
}


void func_ov093_021bb908(BtlServer *server, u32 money) {
    func_ov093_021b91e4(server->mainModule, money);
}

void func_ov093_021bb914(BtlServer *server) {
    func_ov093_021b9234(server->mainModule);
}

void func_ov093_021bb920(BtlServer *server) {
    server->count = 0;
    server->unkCBF = 0;
}

// Function name from swan.
void RequestChangePokemon(BtlServer *server, u8 pos) {
    u32 i;

    for (i = 0; i < server->count; i++) {
        if (pos == server->posList[i]) {
            return;
        }
    }
    server->posList[server->count++] = pos;
}

BtlServerFlow *func_ov093_021bb964(BtlServer *server) {
    return server->flow;
}


