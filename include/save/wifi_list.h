// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/wifi_list.h, the lines in use so far.
// pokebw2's func_0200a150 and func_0200a400 are Black's func_02009a34 and func_02009ce4.
#ifndef BWPORT_SAVE_WIFI_LIST_H
#define BWPORT_SAVE_WIFI_LIST_H

#include "types.h"
#include "struct_decls.h"
#include "dwc/dwc.h"
#include "gfl/heap.h"
#include "save/player_info.h"

BOOL func_02009a34(WifiList *wifiList);
// pokebw2's WifiList_GetMyGSID
s32 func_02009cc0(WifiList *wifiList);
// Whether the player's DWC user data is valid and has a friend code
BOOL func_02009ce4(WifiList *wifiList);

WifiList *populateWifiList(u32 heapId);
void copyWifilist(const WifiList *src, WifiList *dest);
WifiList *func_02009d08(SaveControl *save);

// Finds a friend by ID and gender; the name is not compared
BOOL func_02009d50(WifiList *wifiList, const u16 *name, u32 id, u32 gender, u32 *friendIndex);
#define WIFI_LIST_FRIEND_MAX 32
#define WIFI_LIST_FRIEND_ID 0
#define WIFI_LIST_FRIEND_PROFILE_ID 1
#define WIFI_LIST_FRIEND_TRAINER_VIEW 8
#define WIFI_LIST_FRIEND_GENDER 9

DWCFriendData *func_020099c8(WifiList *wifiList, u32 index);
void func_02009918(WifiList *wifiList, u32 index, u32 param, u32 value);
// Whether the slot holds a friend, how many there are and the last used slot plus one
BOOL func_02009a1c(WifiList *wifiList, u32 index);
// Adds the friend whose 12 bytes of friend data a player gave, with the player's info, to the first free place of the
// Friend Roster; TRUE if there was one, FALSE if the friend was in it already or it was full. heapId is a scratch heap
BOOL func_0200f5ec(GameData *gameData, PlayerInfo *info, const void *friendData, HeapID heapId);

// Black 2's; twinned code confirms the offsets of name, profileId, id, wins, losses, draws, lastYear, lastMonth,
// lastDay, trainerView and gender, and its size in Black
// What the Pal Pad keeps of a friend besides their DWC friend data
struct WifiListFriend {
    u16 name[8];
    u8 unk10[0xc];
    s32 profileId;
    u32 id;
    u16 wins;
    u16 losses;
    u16 draws;
    // When the player last battled or traded with the friend
    u16 lastYear;
    u8 lastMonth;
    u8 lastDay;
    u8 trainerView;
    u8 gender : 2;
    u8 unk2F_2 : 6;
}; // size: 0x30

// Black 2's; twinned code confirms the offsets of friendData and friends, and its size in Black
// Block 0x1e: the player's own DWC account and the Pal Pad's friends
struct WifiList {
    DWCUserData myUserData;
    DWCFriendData friendData[WIFI_LIST_FRIEND_MAX];
    WifiListFriend friends[WIFI_LIST_FRIEND_MAX];
    u8 unk7C0[4];
}; // size: 0x7c4
// Block 0x1f (getPalPadFriendListAddress; pokebw2's PalPadFriendList, the same layout in Black): the players traded
// with, the latest first, and how many trades
#define PAL_PAD_PLAYER_MAX 100
#define PAL_PAD_TRADE_COUNT_MAX 99999

struct PalPadFriendList {
    PlayerInfo players[PAL_PAD_PLAYER_MAX];
    u8 unkC80[0xd0];
    u32 tradeCount;
}; // size: 0xd54

PalPadFriendList *getPalPadFriendListAddress(SaveControl *save);
u32 PalPadFriendList_GetSize(void);
void PalPadFriendList_Init(PalPadFriendList *list);
// The index of the player of that ID, or -1; the ID of the player at an index
s32 func_02009db8(PalPadFriendList *list, u32 id);
u32 func_02009e40(PalPadFriendList *list, u32 index);
u32 func_02009e8c(PalPadFriendList *list);
PlayerInfo *func_02009e54(PalPadFriendList *list, u32 index);
// The number of trades
u32 func_02009ea8(PalPadFriendList *list);

// Adds to a friend's record and stamps today; and adds one friend's record to another's, clearing the first
void func_02009bb8(WifiList *wifiList, s32 index, u32 wins, u32 losses, u32 draws);

#define WIFI_LIST_FRIEND_WINS 2
#define WIFI_LIST_FRIEND_LOSSES 3
#define WIFI_LIST_FRIEND_DRAWS 4
#define WIFI_LIST_FRIEND_LAST_YEAR 5
#define WIFI_LIST_FRIEND_LAST_MONTH 6
#define WIFI_LIST_FRIEND_LAST_DAY 7

// A friend's WIFI_LIST_FRIEND_* field; the record fields can't be set
u32 func_02009864(WifiList *wifiList, u32 index, u32 param);
void func_020099e4(WifiList *wifiList, u32 index, StrBuf *dest);
void func_02009c24(WifiList *wifiList, s32 srcIndex, s32 destIndex);
// Stamps today on a friend
void func_02009b80(WifiList *wifiList, u32 index);
u32 func_02009a54(WifiList *wifiList);
// Moves the friends into the empty slots
void func_02009b3c(WifiList *wifiList);

// A gender of a friend added by friend code, whose trainer the player hasn't met
#define WIFI_LIST_GENDER_UNKNOWN 2

// A friend's name, and loading or storing it through a StrBuf
u16 *func_020099d4(WifiList *wifiList, u32 index);
// Deletes a friend and moves the later ones down
void func_02009a74(WifiList *wifiList, u32 index);

// Records a trainer the player traded with
void func_02009de8(PalPadFriendList *list, PlayerInfo *info);
void func_02009eb0(PalPadFriendList *list);
// Finds the player among the friends, and the friend's index (pokebw2's WifiList_FindFriendByPlayerInfo)
BOOL func_02009d1c(WifiList *wifiList, PlayerInfo *info, u32 *friendIndex);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The player's DWC user data
DWCUserData *func_02009860(WifiList *wifiList);
void func_02009a00(WifiList *wifiList, u32 index, const StrBuf *src);
BOOL func_02009e74(PalPadFriendList *list, u32 id);
// A token for exchanging friend data, from user data or the player's
void func_02009ca8(const DWCUserData *userData, DWCFriendData *token);
void initWifi(WifiList *wifiList);
void func_02009cb0(WifiList *wifiList, DWCFriendData *token);

#endif // BWPORT_SAVE_WIFI_LIST_H
