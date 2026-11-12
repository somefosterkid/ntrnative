// GameSpy's server record (sb_server.c): the functions the server list calls.
#ifndef BWPORT_GS_SB_SERVER_H
#define BWPORT_GS_SB_SERVER_H

#include "types.h"
#include "gs/serverbrowsing.h"
#include "gs/hashtable.h"

// The server record, GameSpy's struct _SBServer (sb_internal.h), named by offset: publicip 0x0, publicport 0x4,
// privateip 0x8, privateport 0xc, icmpip 0x10, state 0x14, flags 0x15, keyvals 0x18, updatetime 0x1c,
// querychallenge 0x20 (a later GameSpy's), next 0x24, splitResponseBitmap 0x28.
struct SBServer_ {
    u32 unk_0;
    u16 unk_4;
    u32 unk_8;
    u16 unk_c;
    u32 unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16[2];
    HashTable unk_18;
    s32 unk_1c;
    u32 unk_20;
    SBServer unk_24;
    u8 unk_28;
};

u32 SBServerGetPublicInetAddress(SBServer server);
u16 SBServerGetPublicQueryPort(SBServer server);
u16 SBServerGetPublicQueryPortNBO(SBServer server);
u32 SBServerGetPrivateInetAddress(SBServer server);
u16 SBServerGetPrivateQueryPort(SBServer server);
BOOL SBServerHasPrivateAddress(SBServer server);
void SBServerSetNext(SBServer server, SBServer next);
SBServer SBAllocServer(SBServerList *slist, u32 publicip, u16 publicport);
int SBIsNullServer(SBServer server);
void SBServerSetFlags(SBServer server, u8 flags);
void SBServerSetPrivateAddr(SBServer server, u32 ip, u16 port);
void SBServerSetICMPIP(SBServer server, u32 icmpip);
void SBServerAddIntKeyValue(SBServer server, const char *keyname, int value);
u8 SBServerGetState(SBServer server);
void SBServerSetState(SBServer server, u8 state);
void func_ov009_0219e01c(SBServer server, const char *keyname, const char *value);
SBServer SBServerGetNext(SBServer server);
void SBServerFree(void *elem);
void SBServerParseKeyVals(s32 param_1, s32 param_2);
// SBServerParseQR2FullKeysSingle and SBServerParseQR2FullKeysSplit
void func_ov009_0219e3cc(SBServer server, char *data, int len);
void func_ov009_0219e5b8(SBServer server, char *data, int len);

#endif // BWPORT_GS_SB_SERVER_H
