// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifibattlematch_link.h: the lines in use
// so far.
#ifndef BWPORT_APP_WIFIBATTLEMATCH_LINK_H
#define BWPORT_APP_WIFIBATTLEMATCH_LINK_H

#include "types.h"
#include "app/wifibattlematch.h"
#include "save/player_info.h"
#include "system/pms_data.h"
#include "struct_decls.h"

// The heap of the link, and the one that the exchange of the match results gives the Wi-Fi library
#define HEAPID_WIFIBATTLEMATCH_LINK 0x57

#define HEAPID_WIFIBATTLEMATCH_LINK_DWC 0x86

// Black 2's; twinned code confirms the values of WIFIBATTLEMATCH_LINK_ACTION_LOGOUT, WIFIBATTLEMATCH_LINK_ACTION_RESULT
// and WIFIBATTLEMATCH_LINK_ACTION_RELOGIN in Black
enum {
    WIFIBATTLEMATCH_LINK_ACTION_LOGOUT,
    WIFIBATTLEMATCH_LINK_ACTION_TEAM_LIST,
    WIFIBATTLEMATCH_LINK_ACTION_RESULT,
    WIFIBATTLEMATCH_LINK_ACTION_RELOGIN,
};

// Black 2's; twinned code confirms the offsets of info, record1, record2, unk34, sentence, entryField2, unk44, gsid,
// unk4C, checksum, signature and partyData in Black
// A player as the two machines exchange it: the info, what the records and the tournament say, and the team picked
// with the signature that the check of the Pokémon made
struct WifiBattleMatchLinkPlayer {
    PlayerInfo info;
    u8 unk20[8];
    u32 record1;
    u32 record2;
    u32 unk30;
    u32 unk34;
    PMSData sentence;
    u32 entryField2;
    u32 unk44;
    s32 gsid;
    u32 unk4C;
    u32 checksum;
    u8 signature[0x80];
    u8 partyData[];
};

// Black 2's, not checked against Black
struct WifiBattleMatchLinkReport {
    u32 data[20];
};

typedef struct {
    WifiBattleMatchLinkReport report;
    u32 unk50;
} WifiBattleMatchLinkReportBuf;

// Black 2's; twinned code confirms the offsets of result and unk14 in Black
// What the battle left
typedef struct {
    u32 result;
    u32 battleStyle;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u32 unkC;
    u32 unk10;
    u32 unk14;
} WifiBattleMatchLinkBattleResult;

// Black 2's; twinned code confirms the offsets of unk5 and registered in Black
// What the server says of the player's account: the Game Sync ID in the first word, and in the bytes after it the
// state of the competition and whether the Game Sync ID is registered
typedef struct {
    s32 profileId;
    u8 unk4;
    u8 unk5;
    u8 registered;
    u8 unk7;
} WifiBattleMatchLinkAccount;

// Black 2's; twinned code confirms the offsets of rating, unk20, results and unk54 in Black
// What the server says of the ratings, as 5 ratings, 5 pairs of results in the order that the server has the battle
// types in, and the data of the Wi-Fi competition's ranking
struct WifiBattleMatchLinkRatings {
    u32 rating[5];
    u32 unk14[3];
    u32 unk20;
    struct {
        u32 second;
        u32 first;
    } results[5];
    u32 unk4C;
    u32 unk50;
    u32 unk54;
    u32 unk58;
};

// Black 2's; twinned code confirms the offsets of mode, action, param, result, record, player, otherPlayer, unk20,
// report, unk28, battleResult, account, unk34 and ratings in Black
typedef struct {
    u32 mode;
    // What the link leaves for overlay 290 to do next
    u32 action;
    WifiBattleMatchParam *param;
    u32 result;
    void *record;
    WifiBattleMatchLinkPlayer *player;
    WifiBattleMatchLinkPlayer *otherPlayer;
    void *unk1C;
    u32 *unk20;
    WifiBattleMatchLinkReport *report;
    u32 *unk28;
    WifiBattleMatchLinkBattleResult *battleResult;
    WifiBattleMatchLinkAccount *account;
    WifiBattleMatchLinkRanking *unk34;
    WifiBattleMatchLinkRatings *ratings;
    void *unk3C;
    BOOL *unk40;
    u32 unk44;
} WifiBattleMatchLinkParam;

// Black 2's; twinned code confirms the offsets of unk8, unkC and unk10 in Black
struct WifiBattleMatchLinkRanking {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u8 body[0x534];
};

// pokebw2's WIFIBATTLEMATCH_LINK_PROC_FUNCTIONS (wifibattlematch_link.c)
extern const GameProcFunctions data_ov175_021e2c68;
// pokebw2's WIFIBATTLEMATCH_LINK_RANDOM_PROC_FUNCTIONS (wifibattlematch_link_random.c)
extern const GameProcFunctions data_ov175_021e2c74;
// pokebw2's WIFIBATTLEMATCH_LINK_COMPETITION_PROC_FUNCTIONS (wifibattlematch_link_competition.c)
extern const GameProcFunctions data_ov175_021e2c80;

#endif // BWPORT_APP_WIFIBATTLEMATCH_LINK_H
