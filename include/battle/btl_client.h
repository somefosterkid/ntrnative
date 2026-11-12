// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_client.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_CLIENT_H
#define BWPORT_BATTLE_BTL_CLIENT_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/math.h"
#include "struct_decls.h"

BOOL func_ov093_021cd97c(BtlClient *client);
void func_ov093_021cdd94(BtlClient *client, BtlClientIDList *list);
void *func_ov093_021cd924(BtlClient *client, u32 *size);
void func_ov093_021cd984(BtlClient *client, u32 arg1);
void func_ov093_021cd9b0(BtlClient *client);
void func_ov093_021cda0c(BtlClient *client, BOOL (*proc)(BtlClient *client));

u16 func_ov093_021cd9dc(BtlClient *client);
BOOL func_ov093_021cd9e0(BtlClient *client);
u8 func_ov093_021d3d84(BtlClient *client);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov093_021cd938(BtlClient *client, void *data);
void func_ov093_021cd95c(BtlClient *client, BtlvCore *viewCore);
// pokebw2's BattleClient_Create
BtlClient *func_ov093_021cd6f0(BtlMainModule *mainModule, BtlPokeCon *pokeCon, u8 commMode, void *netHandle, u16 clientId,
                               u16 numCoverPos, u8 isAI, u32 arg7, BOOL recPlay, MATHRandContext32 *rand, HeapID heapId);
BtlAdapter *func_ov093_021cd978(BtlClient *client);
// pokebw2's BattleClient_Delete
void func_ov093_021cd8e0(BtlClient *client);
void func_ov093_021cd960(BtlClient *client, BtlServer *server);
void func_ov093_021cd914(BtlClient *client);
void func_ov093_021cd9f4(BtlClient *client, u8 value);
BOOL func_ov093_021cdda0(BtlClient *client);
BOOL func_ov093_021cddcc(BtlClient *client);
BOOL func_ov093_021cd9c8(BtlClient *client);
// pokebw2's BattleClient_GetClientId
u8 func_ov093_021d3d70(BtlClient *client);
// pokebw2's BattleClient_GetParty
BattleParty *func_ov093_021d3d7c(BtlClient *client);
// pokebw2's BattleClient_GetShooterEnergy
u8 func_ov093_021d3d90(BtlClient *client);

#endif // BWPORT_BATTLE_BTL_CLIENT_H
