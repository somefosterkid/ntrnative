// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/net_irc.h: the lines in use so far.
#ifndef BWPORT_FIELD_NET_IRC_H
#define BWPORT_FIELD_NET_IRC_H

#include "types.h"
#include "gfl/net.h"

// pokebw2's NetIrc_Init
void func_ov015_0217993c(u32 timeout, u8 gameCode);
// pokebw2's NetIrc_Exit
void func_ov015_02179998(void);
// pokebw2's NetIrc_SetRecvCallback
void func_ov015_021799b4(GFLNetRecvFunc callback);
// pokebw2's NetIrc_IsInitialized
BOOL func_ov015_021799f0(void);
// pokebw2's NetIrc_GetSendTurn
u8 func_ov015_021799fc(void);
// pokebw2's NetIrc_GetUnkCF0
u8 func_ov015_02179a08(void);
// pokebw2's NetIrc_CanSend
BOOL func_ov015_02179a18(void);
// pokebw2's NetIrc_SendPending
void func_ov015_02179aac(void);
// pokebw2's NetIrc_GetNetId
BOOL func_ov015_02179b24(void);
// pokebw2's NetIrc_IsError
BOOL func_ov015_02179cac(void);
// pokebw2's NetIrc_StartLink
void func_ov015_02179ce4(void);
// pokebw2's NetIrc_GetPeerSeen
BOOL func_ov015_02179d68(void);
// pokebw2's NetIrc_Update
void func_ov015_02179d80(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's NetIrc_SetInitialized
void func_ov015_021799cc(void);
// pokebw2's NetIrc_Send
void func_ov015_02179a4c(const u8 *data, u8 size, u8 cmd);
// pokebw2's NetIrc_IsLinked
BOOL func_ov015_02179d78(void);

#endif // BWPORT_FIELD_NET_IRC_H
