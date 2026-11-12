// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_server.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_SERVER_H
#define BWPORT_BATTLE_BTL_SERVER_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "nitro/math.h"
#include "battle/btl_action.h"
#include "battle/btl_rec.h"
#include "battle/btl_server_cmd.h"

BOOL DoesSwitchModeNeedConfirming(BtlServer *server);
u8 GetNextEnemyForSwitchMode(BtlServer *server);
BOOL IsSwitchModeEnabled(BtlServerClient *client);

void func_ov093_021baa94(BtlServer *server, BtlAdapter *adapter, u8 clientId, u8 numCoverPos);
void func_ov093_021bab40(BtlServer *server);
void func_ov093_021bab5c(BtlServer *server);
void func_ov093_021baacc(BtlServer *server, u32 mode, void *netHandle, u8 clientId, u8 numCoverPos);
void func_ov093_021bab10(BtlServer *server, u8 clientId, u8 numCoverPos);
BOOL func_ov093_021bab9c(BtlServer *server);
void func_ov093_021babdc(BtlServer *server, BtlClientIDList *out);
void func_ov093_021bb8bc(BtlServer *server, BattleMon *mon);

void func_ov093_021bb4e0(BtlServer *server, const void *data, u32 size);
BOOL func_ov093_021bb570(BtlServer *server, u32 mode, const void *data, u32 size);

// A client the server sends commands to
struct BtlServerClient {
    BtlAdapter *adapter;
    BattleParty *party;
    u8 numMons;
    u8 numCoverPos;
    // Whether the adapter was given to the server, rather than made by it
    u8 isAttached;
    // 0xff for no client
    u8 clientId;
};

void RequestChangePokemon(BtlServer *server, u8 pos);
BtlServerClient *func_ov093_021bb878(BtlServer *server, u8 clientId);
void func_ov093_021bb920(BtlServer *server);
BtlServerClient *func_ov093_021bb85c(BtlServer *server, u8 clientId);
u8 func_ov093_021bb8a8(const BtlClientActions *clientActions, u8 clientId);
BattleAction func_ov093_021bb8b0(const BtlClientActions *clientActions, u8 clientId, u8 index);
BOOL func_ov093_021bb890(BtlServer *server, u8 clientId);

// The actions each client chose
struct BtlClientActions {
    BattleAction actions[4][3];
    u8 count[4];
};

// Black 2's, not checked against Black
// A step of the server, which returns TRUE when the battle ends
typedef BOOL (*BtlServerProc)(BtlServer *server, int *seq);

// Black 2's; twinned code confirms the offsets of proc, seq, mainModule, pokeCon, clients, flow, flowResult, recTool,
// exitData, strBuf, clientIdList, clientActions, switchModeMonId, state, queuePtr, posList, count, unkCBF and heapId in
// Black
struct BtlServer {
    BtlServerProc proc;
    int seq;
    u32 unk08;
    BtlMainModule *mainModule;
    BtlPokeCon *pokeCon;
    BtlServerClient clients[4];
    BtlServerFlow *flow;
    u32 flowResult;
    u32 unk4C;
    MATHRandContext32 rand;
    BtlRecTool recTool;
    // Sent to the clients when the battle ends
    struct {
        u16 playerClientId;
        u16 result;
    } exitData;
    StrBuf *strBuf;
    BtlClientIDList *clientIdList;
    BtlClientActions clientActions;
    u8 unkE8[4];
    u8 switchModeMonId;
    u8 state;
    u8 unkEE[2];
    BtlServerCmdQueue queue;
    BtlServerCmdQueue *queuePtr;
    u8 posList[6];
    u8 count;
    u8 unkCBB[4];
    u8 unkCBF;
    u16 heapId;
    u8 unkCC2[2];
};

void func_ov093_021babec(BtlServer *server, BtlServerProc proc);
BOOL func_ov093_021bac04(BtlServer *server, int *seq);
BOOL func_ov093_021bacac(BtlServer *server, int *seq);
BOOL func_ov093_021bae60(BtlServer *server, int *seq);
BOOL func_ov093_021baef8(BtlServer *server, int *seq);
BOOL func_ov093_021bb0e0(BtlServer *server, int *seq);
BOOL func_ov093_021bb208(BtlServer *server, int *seq);
BOOL func_ov093_021bb280(BtlServer *server, int *seq);
BOOL func_ov093_021bb320(BtlServer *server, int *seq);
BOOL func_ov093_021bb368(BtlServer *server, int *seq);
BOOL func_ov093_021bb3b0(BtlServer *server, int *seq);
BOOL func_ov093_021bb3f8(BtlServer *server, int *seq);
BOOL func_ov093_021bb43c(BtlServer *server, int *seq);
BOOL func_ov093_021bb484(BtlServer *server, int *seq);
BOOL func_ov093_021bb5fc(BtlServer *server, u32 value);
BOOL func_ov093_021bb628(BtlServer *server, u8 value, BOOL flag);
void *func_ov093_021bb650(BtlServer *server, u8 value, BOOL flag, u32 *size);
void func_ov093_021bb6a8(BtlServer *server);
void func_ov093_021bb724(BtlServer *server, u32 cmd, const void *data, u32 size);
void func_ov093_021bb768(BtlServer *server, u32 cmd, u8 clientId, const void *data, u32 size);
BOOL func_ov093_021bb798(BtlServer *server);
void func_ov093_021bb7fc(BtlServer *server);
void func_ov093_021bb828(BtlServerClient *client);
void func_ov093_021bb840(BtlServerClient *client, u8 clientId, BtlAdapter *adapter, u8 isAttached);
void func_ov093_021bb848(BtlServerClient *client, BattleParty *party, u8 numCoverPos);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BtlServer *func_ov093_021ba9c8(BtlMainModule *mainModule, const MATHRandContext32 *rand, BtlPokeCon *pokeCon,
                               u32 a3, HeapID heapId);
void func_ov093_021bb8dc(BtlServer *server, BattleMon *mon);
void func_ov093_021bb908(BtlServer *server, u32 money);
void func_ov093_021bb914(BtlServer *server);
void func_ov093_021babf4(BtlServer *server);
BOOL func_ov093_021bae5c(BtlServer *server);
void func_ov093_021bb56c(BtlClientActions *clientActions);
void func_ov093_021bb718(BtlServer *server, u32 cmd);
void func_ov093_021bb7d0(BtlServer *server);
BtlServerFlow *func_ov093_021bb964(BtlServer *server);

#endif // BWPORT_BATTLE_BTL_SERVER_H
