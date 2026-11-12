// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_net.h: the lines in use so far.
#ifndef BWPORT_BATTLE_BTL_NET_H
#define BWPORT_BATTLE_BTL_NET_H

#include "types.h"
#include "gfl/net_command.h"

// The battle's NetCommand table, of nine commands (ov093)
extern const NetCommand data_ov093_021f03c4[];

// The server's commands and the clients' replies
BOOL func_ov093_021d4b58(u8 clientId, void *data, u32 size);
BOOL func_ov093_021d4b98(void);
u16 func_ov093_021d4bd4(u8 clientId, void **data);
BOOL func_ov093_021d4c60(void);
u16 func_ov093_021d4c6c(void **data);
BOOL func_ov093_021d4c84(void *data, u32 size);

void func_ov093_021d4150(NetHandle *netHandle, u16 clientMask, HeapID heapId);
// Whether the link failed
BOOL func_ov093_021d4270(void);
BOOL func_ov093_021d4230(void);
void func_ov093_021d4254(void);
BOOL func_ov093_021d4294(u8 clientId);
BOOL func_ov093_021d4348(void);
BOOL func_ov093_021d4360(void);
u8 func_ov093_021d4378(void);
BOOL func_ov093_021d43b8(BtlMainSyncData *data);
BOOL func_ov093_021d440c(BtlMainSyncData *data);
void func_ov093_021d4af4(u8 id);
BOOL func_ov093_021d4b18(u8 id);
u8 func_ov093_021d4438(PokeParty *party);
BOOL func_ov093_021d44cc(void);
void *func_ov093_021d4d24(u32 size, HeapID heapId);
void func_ov093_021d4d64(void *data, PokeParty *party, u8 clientId);
BOOL func_ov093_021d4680(void *data);
BOOL func_ov093_021d4784(u8 clientId);
PokeParty *func_ov093_021d47a4(u8 clientId);
BOOL func_ov093_021d4808(PlayerInfo *info);
PlayerInfo *func_ov093_021d48cc(u8 clientId);
BOOL func_ov093_021d4908(BtlSetupTrainer *trainer);
BOOL func_ov093_021d49b0(void);
void *func_ov093_021d49cc(void);
void func_ov093_021d49e0(void);
BOOL func_ov093_021d450c(void *chatter);
BOOL func_ov093_021d45fc(void);
void *func_ov093_021d4648(u8 clientId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov093_021d4c58(void);
void func_ov093_021d4d5c(void *data);
void func_ov093_021d4800(void);
BOOL func_ov093_021d4898(void);
void func_ov093_021d4a04(void);
void func_ov093_021d4500(void);
void func_ov093_021d4674(void);

#endif // BWPORT_BATTLE_BTL_NET_H
