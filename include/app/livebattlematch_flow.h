// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/livebattlematch_flow.h: the lines in use
// so far.
#ifndef BWPORT_APP_LIVEBATTLEMATCH_FLOW_H
#define BWPORT_APP_LIVEBATTLEMATCH_FLOW_H

#include "types.h"
#include "app/wifibattlematch_graphic.h"
#include "app/wifibattlematch_util.h"
#include "app/wifibattlematch_view.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"
#include "save/player_info.h"
#include "system/pms_data.h"

// The live competition's record: clears it and gives it this console's address, adds to a result by an amount, gets and
// sets the 6-byte address of a result by its place and a byte after it, and whether an address is among the results
void func_0200e904(void *record);
void func_0200e920(void *record, u32 index, int amount);
void func_0200e958(void *record, u32 place, const u8 *mac);
void func_0200e978(void *record, u32 place, u32 byte, u8 value);
BOOL func_0200e994(void *record, const u8 *mac);

typedef struct LiveBattleMatchFlow LiveBattleMatchFlow;

// What battle_championship_flow.c gives the flow
typedef struct {
    // 0 to 3: the flow of a player who joins (0), of one who comes back to the menu (1) or after a match (2), or who
    // quits (3)
    u32 mode;
    WifiBattleMatchMsgBox *msgBox;
    Font *font;
    PrintQueue *printQueue;
    WifiBattleMatchRes *seq;
    WifiBattleMatchGraphic *net;
    GameData *gameData;
    // The player's data to send, where the other player's goes, and the result of the match (the last three of
    // overlay 290's parameters)
    LiveBattleMatchPlayerData *playerData;
    LiveBattleMatchPlayerData *partnerData;
    LiveBattleMatchResult *matchResult;
} LiveBattleMatchFlowParam;

// pokebw2's LiveBattleMatchFlow_Main
// Called every frame
void func_ov176_0217baf4(LiveBattleMatchFlow *flow);
// pokebw2's LiveBattleMatchFlow_GetResult
// 0 while the flow runs, then why it ended
u32 func_ov176_0217bb3c(LiveBattleMatchFlow *flow);
// pokebw2's LiveBattleMatchFlow_Delete
void func_ov176_0217ba9c(LiveBattleMatchFlow *flow);

// Black 2's; twinned code confirms the offsets of info, mac, record1, record2, record0, unk34, sentence, entryField2
// and unk44 in Black
// What the player sends the other player over the link: their trainer info, the last results of the live competition and
// the team picked, which comes after the LIVEBATTLEMATCH_PLAYER_HEADER_SIZE bytes of this
struct LiveBattleMatchPlayerData {
    PlayerInfo info;
    u8 mac[6];
    u8 unk26[2];
    // The record's results 1, 2 and 0 (func_0200f1d8)
    u32 record1;
    u32 record2;
    u32 record0;
    u32 unk34;
    PMSData sentence;
    // Field 2 of the player's entry in the tournament block, which the two players compare
    u32 entryField2;
    u32 unk44;
    u8 unk48[0x8c];
};

// Black 2's, not checked against Black
// What the flow leaves for battle_championship_flow.c's caller to read and the flow to show: how the last match
// ended, 0 to 4
struct LiveBattleMatchResult {
    u32 result;
    u8 unk04[4];
    u8 unk08;
    u8 unk09;
    u8 unk0A;
    // Set once the link failed, so that the flow keeps to the menus that need none
    BOOL disconnected;
};

// Whether the battle video that is loaded is whole
BOOL func_0200b45c(void);
// pokebw2's TournamentEntry_CheckCRC
// Functions of the save and battle video code that have no header yet
// Checks the CRC of a tournament entry
BOOL func_0200b1a8(void *entry);
// pokebw2's TournamentBlock_SetEntry
// Copies an entry into the block
void func_0200b1e0(void *block, u32 index, const void *entry);
// pokebw2's TournamentTeam_SetFromParty
// Fills an entry of the block's other entries with the team's species and moves
void func_0200b2a4(void *record, PokeParty *party);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's LiveBattleMatchFlow_Create
LiveBattleMatchFlow *func_ov176_0217b984(const LiveBattleMatchFlowParam *param, HeapID heapId);

#endif // BWPORT_APP_LIVEBATTLEMATCH_FLOW_H
