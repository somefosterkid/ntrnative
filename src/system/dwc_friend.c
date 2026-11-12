// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/dwc_friend.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "system/net_define.h"
#include "dwc/dwc.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "save/player_info.h"
#include "save/wifi_list.h"
#include "system/game_data.h"

// Checks of a friend's data or friend key against the friends known, and adding a friend to the Friend Roster.
// The ROM has no string for the file, so the name is a guess.

// What func_0200f594 does with the place
#define FRIEND_ADD_NEW 0
// Replaces the data and the player's info, but keeps the name
#define FRIEND_ADD_UPDATE 1
// Only the player's appearance
#define FRIEND_ADD_VIEW 2

static int func_0200f34c(DWCFriendData *friendData, DWCUserData *userData, DWCFriendData *other);
static int func_0200f38c(DWCFriendData *friendData, int *index);
static void func_0200f520(WifiList *wifiList, PlayerInfo *info, u32 index, u32 heapId, BOOL setName);
static void func_0200f594(GameData *gameData, PlayerInfo *info, const void *friendData, u32 index, u32 heapId,
                      int mode);

// pokebw2's compareFriendData
static int func_0200f34c(DWCFriendData *friendData, DWCUserData *userData, DWCFriendData *other) {
    if (DWC_IsEqualFriendData(friendData, other)) {
        return FRIEND_CHECK_SAME;
    }
    if (func_020598ec(userData, friendData) > 0
        && func_020598ec(userData, friendData) == func_020598ec(userData, other)) {
        return FRIEND_CHECK_SAME_PROFILE;
    }
    return FRIEND_CHECK_NEW;
}

// pokebw2's findFriendData
static int func_0200f38c(DWCFriendData *friendData, int *index) {
    int i;
    DWCUserData *userData = func_0203d5ec();
    DWCFriendData *friends = func_0203d610();
    int count = func_0203d634();

    *index = -1;
    if (!func_020596d8(friendData)) {
        return FRIEND_CHECK_INVALID;
    }
    for (i = 0; i < count; i++) {
        if (DWC_IsEqualFriendData(friendData, &friends[i])) {
            *index = i;
            return FRIEND_CHECK_SAME;
        }
        if (func_020598ec(userData, friendData) > 0
            && func_020598ec(userData, friendData) == func_020598ec(userData, &friends[i])) {
            *index = i;
            return FRIEND_CHECK_SAME_PROFILE;
        }
        if (*index < 0 && !func_020596d8(&friends[i])) {
            *index = i;
        }
    }
    return FRIEND_CHECK_NEW;
}

int func_0200f43c(WifiList *wifiList, u64 friendKey, int *index) {
    DWCUserData *userData = func_02009860(wifiList);
    int i = 0;
    DWCFriendData *friends = func_020099c8(wifiList, 0);
    int count = func_02009a34(wifiList);
    DWCFriendData token;

    if (!func_02059354(userData, friendKey)) {
        return FRIEND_CHECK_INVALID;
    }
    DWC_CreateFriendKeyToken(&token, friendKey);
    if (func_020598ec(userData, &token) <= 0) {
        return FRIEND_CHECK_INVALID;
    }
    *index = -1;
    for (; i < count; i++) {
        if (func_020598ec(userData, &token) == func_020598ec(userData, &friends[i])) {
            *index = i;
            return FRIEND_CHECK_SAME;
        }
        if (*index < 0 && !func_020596d8(&friends[i])) {
            *index = i;
        }
    }
    return FRIEND_CHECK_NEW;
}

BOOL func_0200f4e4(DWCFriendData *friendData) {
    BOOL isNew = FALSE;
    DWCFriendData *friends = func_0203d610();
    int index;
    int result;

    result = func_0200f38c(friendData, &index);
    if (result != FRIEND_CHECK_SAME) {
        if (result == FRIEND_CHECK_SAME_PROFILE) {
            MI_CpuCopy8(friendData, &friends[index], sizeof(DWCFriendData));
        } else if (result == FRIEND_CHECK_NEW) {
            isNew = TRUE;
        }
    }
    return isNew;
}

// pokebw2's setFriendInfo
static void func_0200f520(WifiList *wifiList, PlayerInfo *info, u32 index, u32 heapId, BOOL setName) {
    if (setName) {
        StrBuf *name = copyTrainerNameToNewStrbuf(info->name, heapId);

        func_02009a00(wifiList, index, name);
        GFL_StrBufFree(name);
    }
    func_02009918(wifiList, index, WIFI_LIST_FRIEND_GENDER, getTrainerGender(info));
    func_02009918(wifiList, index, WIFI_LIST_FRIEND_ID, getIDAsUInt(info));
    func_02009918(wifiList, index, WIFI_LIST_FRIEND_PROFILE_ID, func_0200853c(info));
    func_02009918(wifiList, index, WIFI_LIST_FRIEND_TRAINER_VIEW, func_02008554(info));
}

static void func_0200f594(GameData *gameData, PlayerInfo *info, const void *friendData, u32 index, u32 heapId,
                      int mode) {
    WifiList *wifiList = func_0201283c(gameData);
    DWCFriendData *dest = func_020099c8(wifiList, index);

    if (mode != FRIEND_ADD_VIEW) {
        MI_CpuCopy8(friendData, dest, sizeof(DWCFriendData));
    }
    if (mode == FRIEND_ADD_NEW) {
        func_0200f520(wifiList, info, index, heapId, TRUE);
    } else if (mode == FRIEND_ADD_UPDATE) {
        func_0200f520(wifiList, info, index, heapId, FALSE);
    }
    func_02009918(wifiList, index, WIFI_LIST_FRIEND_TRAINER_VIEW, func_02008554(info));
}

BOOL func_0200f5ec(GameData *gameData, PlayerInfo *info, const void *friendData, HeapID heapId) {
    WifiList *wifiList = func_0201283c(gameData);
    int i;

    if (!func_020596d8(friendData)) {
        return FALSE;
    }
    for (i = 0; i < WIFI_LIST_FRIEND_MAX; i++) {
        DWCFriendData *other = func_020099c8(wifiList, i);
        int result;

        if (!func_02009a1c(wifiList, i)) {
            func_0200f594(gameData, info, friendData, i, heapId, FRIEND_ADD_NEW);
            return TRUE;
        }
        result = func_0200f34c((DWCFriendData *)friendData, func_02009860(wifiList), other);
        if (result == FRIEND_CHECK_SAME) {
            func_0200f594(gameData, info, friendData, i, heapId, FRIEND_ADD_VIEW);
            return FALSE;
        }
        if (result == FRIEND_CHECK_SAME_PROFILE) {
            func_0200f594(gameData, info, friendData, i, heapId, FRIEND_ADD_UPDATE);
            return FALSE;
        }
    }
    return FALSE;
}
