// GameSpy's server browsing (sb_serverbrowsing.h, sb_internal.h): the functions that lib/dwc's files call across
// files, each with its definition's signature (still Ghidra's words where the C is), or for the ones not written yet
// what their callers pass
#ifndef BWPORT_GS_SERVERBROWSING_H
#define BWPORT_GS_SERVERBROWSING_H

#include "types.h"
#include "gs/darray.h"
#include "gs/hashtable.h"
#include "gs/sb_crypt.h"

typedef enum {
    sbe_noerror,
    sbe_socketerror,
    sbe_dnserror,
    sbe_connecterror,
    sbe_dataerror,
    sbe_allocerror,
    sbe_paramerror,
} SBError;

typedef struct SBServer_ *SBServer;
// GameSpy's ServerBrowser (sb_internal.h's struct _ServerBrowser, in gs/sb_internal.h): the query engine and the server
// list with the callback to the game
typedef struct ServerBrowser_ *ServerBrowser;

// the reasons of a ServerBrowser's callback (Black's values)
typedef enum {
    sbc_serveradded,
    sbc_serverupdated,
    sbc_serverupdatefailed,
    sbc_serverdeleted,
    sbc_updatecomplete,
    sbc_queryerror,
    sbc_challengereceived,
} SBCallbackReason;
typedef void (*ServerBrowserCallback)(ServerBrowser sb, SBCallbackReason reason, SBServer server, void *instance);

// Comparison types of ServerBrowserSort
typedef enum {
    sbcm_int,
    sbcm_float,
    sbcm_strcase,
    sbcm_stricase
} SBCompareMode;
typedef enum {
    SBFalse,
    SBTrue
} SBBool;
#define SORTKEY_LENGTH 255
typedef struct _SortInfo {
    char sortkey[SORTKEY_LENGTH];
    SBCompareMode comparemode;
} SortInfo;

// The server list: the fields the code so far touches, named by offset.
typedef struct SBServerList SBServerList;
enum {
    slc_serveradded = 0, // the callback's reason for a server appended to the list (Black's value)
    slc_serverdeleted = 2, // a server removed from the list (Black's value)
    slc_serverupdated = 1, // a server updated in the list (Black's value)
    slc_initiallistcomplete = 3,
    slc_disconnected = 4,
    slc_queryerror = 5,
    slc_publicipdetermined = 6,
};
typedef void (*SBListCallback)(SBServerList *slist, int reason, SBServer server, void *instance);
typedef void (*SBValuesCallback)(SBServerList *slist, SBServer server, u32 data, int count, char **values, void *instance);
typedef void (*SBKeyCallback)(SBServerList *slist, const char *name, u32 ip, u16 port, u32 data, const char *value, void *instance);
struct SBServerList {
    s32 unk_0;
    DArray servers; // 0x4
    DArray keylist; // 0x8
    char unk_c[0x54 - 0xc];
    char unk_54[0x20]; // 0x54, a string
    char mychallenge[8]; // 0x74
    void *unk_7c; // 0x7c
    s32 unk_80; // 0x80
    char *popularValues[(0x480 - 0x84) / 4]; // 0x84
    s32 numPopularValues; // 0x480
    s32 unk_484;
    SBListCallback ListCallback; // 0x488
    SBValuesCallback unk_48c; // 0x48c
    SBKeyCallback unk_490; // 0x490
    void *instance; // 0x494
    SortInfo currsortinfo; // 0x498
    SortInfo prevsortinfo; // 0x59c
    SBBool sortascending; // 0x6a0
    u32 mypublicip; // 0x6a4
    u32 unk_6a8; // 0x6a8
    u16 unk_6ac; // 0x6ac
    u8 unk_6ae[0x6b0 - 0x6ae];
    char *lastListError; // 0x6b0
    s32 unk_6b4; // 0x6b4
    u32 unk_6b8; // 0x6b8
    u32 unk_6bc; // 0x6bc
    GOACryptState unk_6c0; // 0x6c0 the crypt state
    u8 unk_7c5[0x7c8 - 0x6c0 - sizeof(GOACryptState)];
    u32 queryoptions; // 0x7c8
    s32 unk_7cc; // 0x7cc
    u16 backendgameflags; // 0x7d0
    u8 unk_7d2[2];
    u32 unk_7d4; // 0x7d4
    SBServer deadlist; // 0x7d8
};
// sb_queryengine.c: gs/sb_queryengine.h

// sb_server.c
HashTable SBRefStrHash(SBServerList *slist);
void SBRefStrHashCleanup(SBServerList *slist);
int SBServerGetIntValueA(SBServer server, const char *key, int idefault);
const char *SBServerGetStringValueA(SBServer server, const char *keyname, const char *def);
double func_ov009_0219e1b8(SBServer server, const char *keyname, double def);

// sb_serverbrowsing.c: func_ov009_0219ec74 and func_ov009_0219ed4c are GameSpy's ServerBrowserNewA and
// ServerBrowserFree
ServerBrowser func_ov009_0219ec74(s8 *queryForGamename, s8 *queryFromGamename, s8 *queryFromKey,
                                  int queryFromVersion, int maxConcUpdates, int queryVersion, BOOL lanBrowse,
                                  ServerBrowserCallback callback, void *instance);
void func_ov009_0219ed4c(ServerBrowser sb);
SBError ServerBrowserLimitUpdateA(ServerBrowser sb, s32 async, BOOL disconnectOnComplete, const u8 *basicFields,
                                  int numBasicFields, const char *serverFilter, int maxServers);
// sb_serverbrowsing.c's functions after its range, not written yet
SBError ServerBrowserSendMessageToServerA(ServerBrowser param_1, const char *param_2, u16 param_3, u8 *param_4, s32 param_5);
SBError ServerBrowserSendNatNegotiateCookieToServerA(ServerBrowser connection, const char *str, u16 port, u32 cookie);
void ServerBrowserClear(ServerBrowser param_1);
u8 *ServerBrowserGetServer(ServerBrowser param_1, s32 param_2);
int ServerBrowserCount(ServerBrowser sb);
void ServerBrowserRemoveServer(ServerBrowser sb, SBServer server);
void ServerBrowserSortA(ServerBrowser sb, SBBool ascending, char *sortkey, SBCompareMode comparemode);
u32 ServerBrowserGetMyPublicIPAddr(ServerBrowser param_1);
int ServerBrowserThink(ServerBrowser sb);
int ServerBrowserState(ServerBrowser sb);
// sb_serverlist.c
void func_ov009_0219f9a8(SBServerList *slist, s8 *queryForGamename, s8 *queryFromGamename,
                         s8 *queryFromKey, int queryFromVersion, BOOL lanBrowse, SBListCallback callback,
                         void *instance);
void SBServerListCleanup(SBServerList *slist);
void SBServerListDisconnect(SBServerList *slist);
void SBSetLastListErrorPtr(SBServerList *slist, char *theError);
SBError ProcessLanData(SBServerList *slist);
SBError func_ov009_021a153c(SBServerList *slist);
SBError func_ov009_021a13d0(SBServerList *slist);
SBError func_ov009_021a0a24(SBServerList *slist);
SBError func_ov009_021a10d4(SBServerList *slist, char *buf, int len);
SBError func_ov009_021a0f0c(SBServerList *slist, char *buf, int len);
SBError func_ov009_021a1620(SBServerList *slist, u32 ip, u16 port, u8 *buf, int len);
int func_ov009_0219ffd4(SBServerList *slist, char *param_2, char *param_3, u32 flags, u32 param_5);
// ov009 globals the server list reads
extern SBServer SBNullServer;
// Not written yet: the sb_server.c and sb_serverlist.c callers pass the list and a string
void SBReleaseStr(SBServerList *slist, const char *str);

#endif // BWPORT_GS_SERVERBROWSING_H
