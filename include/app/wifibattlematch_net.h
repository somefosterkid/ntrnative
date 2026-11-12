// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifibattlematch_net.h: the lines in use
// so far.
#ifndef BWPORT_APP_WIFIBATTLEMATCH_NET_H
#define BWPORT_APP_WIFIBATTLEMATCH_NET_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef struct WifiBattleMatchNet WifiBattleMatchNet;

// What the matching of two players is looking for: the rating, the disconnects, the cup and the number of matches
typedef struct {
    s32 rating;
    s32 disconnects;
    s32 cup;
    s32 matches;
} WifiBattleMatchNetKeys;

// Black 2's; twinned code confirms the offsets of status and signature in Black
// The check of a team that the server answers: its status, the result of each Pokémon and the signature
typedef struct {
    u8 status;
    u32 result[6];
    u8 signature[0x80];
} WifiBattleMatchNetCheck;

// pokebw2's WifiBattleMatchNet_ShowIcon
void func_ov175_021dd008(WifiBattleMatchNet *p_wk, int a1);
// pokebw2's WifiBattleMatchNet_CancelMatch
void func_ov175_021da36c(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_StartCreateRecord
void func_ov175_021dbfa4(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_SendPlayerInfo
BOOL func_ov175_021dc1a0(WifiBattleMatchNet *p_wk, const void *data);
// pokebw2's WifiBattleMatchNet_StartSync
// Starts a timing sync with the number, and checks whether the other machine reached it too
void func_ov175_021d9ed4(WifiBattleMatchNet *p_wk, u32 syncId);
// pokebw2's WifiBattleMatchNet_TakeCmd2402
BOOL func_ov175_021da45c(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_CheckSync
BOOL func_ov175_021d9eec(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_SendCmd2402
BOOL func_ov175_021da434(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_SendReportValue
BOOL func_ov175_021dc2c0(WifiBattleMatchNet *p_wk, const void *data);

// pokebw2's WifiBattleMatchNet_ResetLogin
// Resets the state of the machines that follow; 0 while running, 1 when done and 2 on failure
void func_ov175_021da044(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_SendValue4
BOOL func_ov175_021dc268(WifiBattleMatchNet *p_wk, const void *data);

// The fields of the arena's table, by the number that WifiBattleMatchNet_GetArenaFieldName gives their names for
enum {
    ARENA_DISCONNECTS_COUNTER = 3,
    ARENA_COMPLETE_MATCHES_COUNTER = 4,
    ARENA_NUM_SINGLE_WIN = 9,
    ARENA_NUM_SINGLE_LOSE = 10,
    ARENA_NUM_DOUBLE_LOSE = 11,
    ARENA_NUM_DOUBLE_WIN = 12,
    ARENA_NUM_ROTATE_WIN = 13,
    ARENA_NUM_ROTATE_LOSE = 14,
    ARENA_NUM_TRIPLE_WIN = 15,
    ARENA_NUM_TRIPLE_LOSE = 16,
    ARENA_NUM_SHOOTER_WIN = 17,
    ARENA_NUM_SHOOTER_LOSE = 18,
    ARENA_ELO_RATING_1V1_SINGLE = 19,
    ARENA_ELO_RATING_1V1_DOUBLE = 20,
    ARENA_ELO_RATING_1V1_ROTATE = 21,
    ARENA_ELO_RATING_1V1_TRIPLE = 22,
    ARENA_ELO_RATING_1V1_SHOOTER = 23,
    ARENA_CHEATS_COUNTER = 24,
    ARENA_ELO_RATING_MAXIMUM_STAT = 25,
    ARENA_ELO_RATING_MINIMUM_STAT = 26,
    ARENA_SCORE_WEIGHT_NORMAL = 27,
    ARENA_FLOOR_MODIFIER = 28,
    ARENA_INTERMED_MODIFIER = 29,
    ARENA_ADVANCED_MODIFIER = 30,
    ARENA_MASTER_MODIFIER = 31,
    ARENA_RATING_DIFF_HIGH = 32,
    ARENA_RATING_DIFF_LOW = 33,
    ARENA_NUM_WINLOSE_COUNTER_MINIMUM_STAT = 34,
    ARENA_INITIAL_RATING = 35,
    ARENA_ELO_RATING_1V1_WIFICUP = 36,
    ARENA_NUM_WIFICUP_WIN = 37,
    ARENA_NUM_WIFICUP_LOSE = 38,
    ARENA_CHEATS_WIFICUP_COUNTER = 39,
    ARENA_DISCONNECTS_WIFICUP_COUNTER = 40,
};

// pokebw2's WifiBattleMatchNet_GetArenaFieldCount
int func_ov175_021dd034(void);
// pokebw2's WifiBattleMatchNet_GetArenaFieldName
const char *func_ov175_021dd044(int field);

// Black 2's, not checked against Black
// The kinds of read that WifiBattleMatchNet_StartRead starts: it fills a WifiBattleMatchNetStats, a WifiBattleMatchNetCupStats,
// a record ID, the team of the Wi-Fi cup and the date of the last login
enum {
    WBM_READ_ARENA,
    WBM_READ_WIFICUP,
    WBM_READ_RECORD_ID,
    WBM_READ_WIFICUP_PARTY,
    WBM_READ_LAST_LOGIN,
};

// Black 2's, not checked against Black
// The kinds of record that WifiBattleMatchNet_StartWrite writes, with the data it takes: the team of the Wi-Fi cup, nothing
// (the date is the current one), nothing, and a WifiBattleMatchNetRecordData
enum {
    WBM_WRITE_WIFICUP,
    WBM_WRITE_LAST_LOGIN,
    WBM_WRITE_PROFILE,
    WBM_WRITE_RECORD_DATA,
};

// Black 2's; twinned code confirms the offsets of eloRatingDouble, eloRatingRotate, eloRatingShooter, eloRatingSingle,
// eloRatingTriple, cheatsCounter, disconnectsCounter, numDoubleLose, numDoubleWin, numRotateLose, numRotateWin,
// numShooterLose, numShooterWin, numSingleLose, numSingleWin, numTripleLose, numTripleWin, initialProfileId,
// nowProfileId, recordSaveIdx and recordId in Black
// The stats of a player in the arena
struct WifiBattleMatchNetStats {
    s32 eloRatingDouble;
    s32 eloRatingRotate;
    s32 eloRatingShooter;
    s32 eloRatingSingle;
    s32 eloRatingTriple;
    s32 unk14;
    s32 cheatsCounter;
    s32 unk1C;
    s32 disconnectsCounter;
    s32 numDoubleLose;
    s32 numDoubleWin;
    s32 numRotateLose;
    s32 numRotateWin;
    s32 numShooterLose;
    s32 numShooterWin;
    s32 numSingleLose;
    s32 numSingleWin;
    s32 numTripleLose;
    s32 numTripleWin;
    s32 initialProfileId;
    s32 nowProfileId;
    s32 recordSaveIdx;
    s32 recordId;
};

// Black 2's, not checked against Black
// The stats of a player in the Wi-Fi cup, followed by the team that was registered
struct WifiBattleMatchNetCupStats {
    s32 cheatsCounter;
    s32 disconnectsCounter;
    s32 numWin;
    s32 numLose;
    s32 eloRating;
    s32 recordSaveIdx;
    u8 party[1];
};

// Black 2's, not checked against Black
// A record that the competition keeps in one of six slots
struct WifiBattleMatchNetRecordData {
    u8 data[0x50];
    u32 slot;
};

// pokebw2's WifiBattleMatchNet_SendParty
// Sends the team picked, and then is TRUE once it is sent
BOOL func_ov175_021dc200(WifiBattleMatchNet *p_wk, PokeParty *party);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WifiBattleMatchNet_Delete
void func_ov175_021d9ea0(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_Nop
// Does nothing
void func_ov175_021d9ed0(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_StartAccountRequest
void func_ov175_021dc738(WifiBattleMatchNet *p_wk, HeapID heapId);
// pokebw2's WifiBattleMatchNet_AccountRequestMain
u32 func_ov175_021dc77c(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_GetAccountValues
void func_ov175_021dc8a0(WifiBattleMatchNet *p_wk, void *out);
// pokebw2's WifiBattleMatchNet_CheckError
// Checks the network's state and the error that was found, returning how it ends (0 for none)
u32 func_ov175_021d9f04(WifiBattleMatchNet *p_wk, u32 a1, u32 a2);
// pokebw2's WifiBattleMatchNet_StartRead
// Reads the table of fields of a kind into out; 0 while it runs, 1 when done and 2 on failure
void func_ov175_021db3a4(WifiBattleMatchNet *p_wk, int recordId, int kind, void *out);
// pokebw2's WifiBattleMatchNet_ReadMain
u32 func_ov175_021db478(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_GetRecordId
u32 func_ov175_021dc194(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_CreateRecordMain
u32 func_ov175_021dbfb4(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_StartWrite
// Writes a record of a kind; 0 while it runs, 1 when done and 2 on failure
void func_ov175_021dbc04(WifiBattleMatchNet *p_wk, int kind, void *data);
// pokebw2's WifiBattleMatchNet_WriteMain
u32 func_ov175_021dbdac(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_StartPokemonCheck
void func_ov175_021dcb58(WifiBattleMatchNet *p_wk, void *data, int kind);
// pokebw2's WifiBattleMatchNet_PokemonCheckMain
u32 func_ov175_021dcc28(WifiBattleMatchNet *p_wk, WifiBattleMatchNetCheck *out);
// pokebw2's WifiBattleMatchNet_LoginMain
u32 func_ov175_021da054(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_GetGSID
s32 func_ov175_021dd020(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_StartPostRequest
void func_ov175_021dc8b4(WifiBattleMatchNet *p_wk, const void *in, HeapID heapId);
// pokebw2's WifiBattleMatchNet_PostRequestMain
u32 func_ov175_021dc908(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_InitDownload
void func_ov175_021dc2f4(WifiBattleMatchNet *p_wk, s32 profileId);
// pokebw2's WifiBattleMatchNet_DownloadMain
u32 func_ov175_021dc338(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_GetDownload
void func_ov175_021dc66c(WifiBattleMatchNet *p_wk, void *dest);
// pokebw2's WifiBattleMatchNet_StartReport
void func_ov175_021da5b0(WifiBattleMatchNet *p_wk, u32 mode, u32 kind, u32 a3, const u32 *a4, u32 a5);
// pokebw2's WifiBattleMatchNet_ReportMain
u32 func_ov175_021da6b8(WifiBattleMatchNet *p_wk, u32 *out);
// pokebw2's WifiBattleMatchNet_ResetEnd
void func_ov175_021da384(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_EndMain
BOOL func_ov175_021da390(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_GetReceivedValue
u32 func_ov175_021dadac(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_GetArenaFieldNames
// wifibattlematch_net_fields.c, which follows: the names of the Sake fields. The first two give the arena's table of
// them and its length, and the next two the Wi-Fi cup's; the last gives the name of one of the numbered fields
const char **func_ov175_021dd02c(void);
// pokebw2's WifiBattleMatchNet_GetWifiCupFieldNames
const char **func_ov175_021dd038(void);
// pokebw2's WifiBattleMatchNet_GetWifiCupFieldCount
int func_ov175_021dd040(void);
// pokebw2's WifiBattleMatchNet_Create
WifiBattleMatchNet *func_ov175_021d9dc8(void *a0, GameData *gameData, void *loginBuffer, HeapID heapId);
// pokebw2's WifiBattleMatchNet_StartMatch
// Sets the match query and its keys for the mode, and the machine that runs it; 0 while it runs, 1 once matched, 2
// on failure
void func_ov175_021da0d4(WifiBattleMatchNet *p_wk, u32 mode, u32 a2, u32 a3, const WifiBattleMatchNetKeys *keys);
// pokebw2's WifiBattleMatchNet_MatchMain
u32 func_ov175_021da1e4(WifiBattleMatchNet *p_wk);
// pokebw2's WifiBattleMatchNet_TakePlayerInfo
BOOL func_ov175_021dc1e4(WifiBattleMatchNet *p_wk, void **out);
// pokebw2's WifiBattleMatchNet_TakeParty
// Copies the other player's team into party, and then is TRUE, once it has arrived
BOOL func_ov175_021dc240(WifiBattleMatchNet *p_wk, PokeParty *party);
// pokebw2's WifiBattleMatchNet_TakeValue4
BOOL func_ov175_021dc2a0(WifiBattleMatchNet *p_wk, void *out);
// pokebw2's WifiBattleMatchNet_TakeReportValue
BOOL func_ov175_021dc2d8(WifiBattleMatchNet *p_wk, u32 *out);
// pokebw2's WifiBattleMatchNet_StartNameCheck
void func_ov175_021dca28(WifiBattleMatchNet *p_wk, PlayerInfo *info, u16 a2);
// pokebw2's WifiBattleMatchNet_NameCheckMain
BOOL func_ov175_021dcaec(WifiBattleMatchNet *p_wk, u32 *out);

#endif // BWPORT_APP_WIFIBATTLEMATCH_NET_H
