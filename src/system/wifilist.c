// Black's code at main 0x020097bc-0x02009ed4. Placed by tools/gap_files.py from 28 asserts, 20 pokebw2, 3 name.

#include "types.h"
#include "dwc/dwc.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/overlay.h"
#include "gfl/rtc_cache.h"
#include "gfl/str.h"
#include "nitro/mi.h"
#include "nitro/rtc.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
#define WIFI_LIST_RECORD_MAX 9999


struct StrBuf {
    u16 size;
    u16 length;
    u32 magic;
    u16 chars[1];
};

extern u32 OVERLAY_10_ID[];
extern u32 OVERLAY_11_ID[];
extern u32 OVERLAY_13_ID[];
extern u32 OVERLAY_9_ID[];

u32 func_020097bc(void) {
    return 0x7C4U;
}

WifiList *populateWifiList(u32 heapId) {
    WifiList *wifiList;

    wifiList = GFL_HeapAllocate((HeapID)heapId,0x7c4,1,"wifilist.c",0x39);
    initWifi(wifiList);
    return wifiList;
}

void copyWifilist(const WifiList *src, WifiList *dest) {
    MI_CpuCopy8(src, dest, 0x7c4);
}

void initWifi(WifiList *wifiList) {
    MI_CpuFill8(wifiList, 0, 0x7c4);
    GFL_OvlUnload(OVERLAY_ID(10));
    GFL_OvlUnload(OVERLAY_ID(11));
    GFL_OvlLoad(OVERLAY_ID(13));
    GFL_OvlLoad(OVERLAY_ID(9));
    func_ov009_02157138(wifiList);
    GFL_OvlUnload(OVERLAY_ID(9));
    GFL_OvlUnload(OVERLAY_ID(13));
    GFL_OvlLoad(OVERLAY_ID(10));
    GFL_OvlLoad(OVERLAY_ID(11));
}

DWCUserData *func_02009860(WifiList *wifiList) {
    return &wifiList->myUserData;
}

u32 func_02009864(WifiList *wifiList, u32 index, u32 param) {
    u32 value = 0;

    switch (param) {
    case WIFI_LIST_FRIEND_ID:
        value = wifiList->friends[index].id;
        break;
    case WIFI_LIST_FRIEND_PROFILE_ID:
        value = wifiList->friends[index].profileId;
        break;
    case WIFI_LIST_FRIEND_WINS:
        value = wifiList->friends[index].wins;
        break;
    case WIFI_LIST_FRIEND_LOSSES:
        value = wifiList->friends[index].losses;
        break;
    case WIFI_LIST_FRIEND_DRAWS:
        value = wifiList->friends[index].draws;
        break;
    case WIFI_LIST_FRIEND_LAST_YEAR:
        value = wifiList->friends[index].lastYear;
        break;
    case WIFI_LIST_FRIEND_LAST_MONTH:
        value = wifiList->friends[index].lastMonth;
        break;
    case WIFI_LIST_FRIEND_LAST_DAY:
        value = wifiList->friends[index].lastDay;
        break;
    case WIFI_LIST_FRIEND_GENDER:
        value = wifiList->friends[index].gender;
        break;
    case WIFI_LIST_FRIEND_TRAINER_VIEW:
        value = wifiList->friends[index].trainerView;
        break;
    }
    return value;
}


void func_02009918(WifiList *wifiList, u32 index, u32 param, u32 value) {
    switch (param) {
    case WIFI_LIST_FRIEND_ID:
        wifiList->friends[index].id = value;
        break;
    case WIFI_LIST_FRIEND_PROFILE_ID:
        wifiList->friends[index].profileId = value;
        break;
    case WIFI_LIST_FRIEND_LAST_YEAR:
        wifiList->friends[index].lastYear = value;
        break;
    case WIFI_LIST_FRIEND_LAST_MONTH:
        wifiList->friends[index].lastMonth = value;
        break;
    case WIFI_LIST_FRIEND_LAST_DAY:
        wifiList->friends[index].lastDay = value;
        break;
    case WIFI_LIST_FRIEND_GENDER:
        wifiList->friends[index].gender = value;
        break;
    case WIFI_LIST_FRIEND_TRAINER_VIEW:
        wifiList->friends[index].trainerView = value;
        break;
    }
}


DWCFriendData *func_020099c8(WifiList *wifiList, u32 index) {
    return wifiList->friendData + index;
}

u16 *func_020099d4(WifiList *wifiList, u32 index) {
    return wifiList->friends[index].name;
}

void func_020099e4(WifiList *wifiList, u32 index, StrBuf *dest) {
    GFL_StrBufLoadFixedString(dest, wifiList->friends[index].name, 0x10);
}

void func_02009a00(WifiList *wifiList, u32 index, const StrBuf *src) {
    GFL_StrBufStoreString(src, wifiList->friends[index].name, 0x10);
}

BOOL func_02009a1c(WifiList *wifiList, u32 index) {
    BOOL BVar1;

    if (index >= 0x20) {
        return 0;
    }
    BVar1 = func_020596d8(wifiList->friendData + index);
    return BVar1;
}

BOOL func_02009a34(WifiList *wifiList) {
    BOOL BVar1;
    u32 index;
    BOOL BVar2;

    BVar2 = 0;
    index = 0;
    do {
        BVar1 = func_02009a1c(wifiList, index);
        if (BVar1 != 0) {
            BVar2 = BVar2 + 1;
        }
        index = index + 1;
    } while ((s32)index < 0x20);
    return BVar2;
}

u32 func_02009a54(WifiList *wifiList) {
    u32 last = 0;
    s32 i;

    for (i = 0; i < WIFI_LIST_FRIEND_MAX; i++) {
        if (func_02009a1c(wifiList, i)) {
            last = i + 1;
        }
    }
    return last;
}

void func_02009a74(WifiList *wifiList, u32 index) {
    int i;
    s32 last = WIFI_LIST_FRIEND_MAX - 1;

    if (index >= WIFI_LIST_FRIEND_MAX) {
        return;
    }
    for (i = index; i < last; i++) {
        MI_CpuCopy8(&wifiList->friends[i + 1], &wifiList->friends[i], sizeof(WifiListFriend));
        MI_CpuCopy8(&wifiList->friendData[i + 1], &wifiList->friendData[i], sizeof(DWCFriendData));
    }
    MI_CpuFill8(&wifiList->friends[last], 0, sizeof(WifiListFriend));
    MI_CpuFill8(&wifiList->friendData[last], 0, sizeof(DWCFriendData));
}


void func_02009ae4(WifiList *wifiList, u32 dest, u32 src) {
    MI_CpuCopy8(&wifiList->friends[src], &wifiList->friends[dest], sizeof(WifiListFriend));
    MI_CpuCopy8(&wifiList->friendData[src], &wifiList->friendData[dest], sizeof(DWCFriendData));
    MI_CpuFill8(&wifiList->friends[src], 0, sizeof(WifiListFriend));
    MI_CpuFill8(&wifiList->friendData[src], 0, sizeof(DWCFriendData));
}

void func_02009b3c(WifiList *wifiList) {
    s32 i;
    s32 empty = -1;

    for (i = 0; i < WIFI_LIST_FRIEND_MAX; i++) {
        if (func_02009a1c(wifiList, i)) {
            if (empty != -1) {
                func_02009ae4(wifiList, empty, i);
                // Starts over from the first slot
                i = -1;
                empty = -1;
            }
        } else if (empty == -1) {
            empty = i;
        }
    }
}

void func_02009b80(WifiList *wifiList, u32 index) {
    RTCDate date;

    RTC_GetCachedDate(&date);
    wifiList->friends[index].lastYear = date.year + 2000;
    wifiList->friends[index].lastMonth = date.month;
    wifiList->friends[index].lastDay = date.day;
}

void func_02009bb8(WifiList *wifiList, s32 index, u32 wins, u32 losses, u32 draws) {
    if (index < 0 || index >= WIFI_LIST_FRIEND_MAX) {
        return;
    }
    wifiList->friends[index].wins += wins;
    if (wifiList->friends[index].wins > WIFI_LIST_RECORD_MAX) {
        wifiList->friends[index].wins = WIFI_LIST_RECORD_MAX;
    }
    wifiList->friends[index].losses += losses;
    if (wifiList->friends[index].losses > WIFI_LIST_RECORD_MAX) {
        wifiList->friends[index].losses = WIFI_LIST_RECORD_MAX;
    }
    wifiList->friends[index].draws += draws;
    if (wifiList->friends[index].draws > WIFI_LIST_RECORD_MAX) {
        wifiList->friends[index].draws = WIFI_LIST_RECORD_MAX;
    }
    func_02009b80(wifiList, index);
}

void func_02009c24(WifiList *wifiList, s32 srcIndex, s32 destIndex) {
    if ((((destIndex >= 0) && (destIndex < 0x20)) && (srcIndex >= 0)) && (srcIndex < 0x20)) {
        wifiList->friends[destIndex].wins = wifiList->friends[destIndex].wins + wifiList->friends[srcIndex].wins;
        if (9999 < wifiList->friends[destIndex].wins) {
            wifiList->friends[destIndex].wins = 9999;
        }
        wifiList->friends[destIndex].losses = wifiList->friends[destIndex].losses + wifiList->friends[srcIndex].losses;
        if (9999 < wifiList->friends[destIndex].losses) {
            wifiList->friends[destIndex].losses = 9999;
        }
        wifiList->friends[destIndex].draws = wifiList->friends[destIndex].draws + wifiList->friends[srcIndex].draws;
        if (9999 < wifiList->friends[destIndex].draws) {
            wifiList->friends[destIndex].draws = 9999;
        }
        MI_CpuFill8(wifiList->friends + srcIndex, 0, 0x30);
    }
}

void func_02009ca8(const DWCUserData *userData, DWCFriendData *token) {
    DWC_CreateExchangeToken(userData, token);
}

void func_02009cb0(WifiList *wifiList, DWCFriendData *token) {
    DWCUserData *userData;

    userData = func_02009860(wifiList);
    DWC_CreateExchangeToken(userData, token);
}

s32 func_02009cc0(WifiList *wifiList) {
    DWCUserData *userData = func_02009860(wifiList);
    DWCFriendData token;

    func_02009cb0(wifiList, &token);
    return func_020598ec(userData, &token);
}

BOOL func_02009ce4(WifiList *wifiList) {
    DWCUserData *userData;
    BOOL BVar1;

    userData = func_02009860(wifiList);
    BVar1 = DWC_CheckHasProfile(userData);
    if ((BVar1 != 0) && (BVar1 = DWC_CheckValidConsole(userData), BVar1 != 0)) {
        return 1;
    }
    return 0;
}

WifiList *func_02009d08(SaveControl *save) {
    WifiList *wifiList;

    wifiList = SaveControl_GetBlockPtr(save, 0x1e);
    func_020099d4(wifiList, 0);
    return wifiList;
}

BOOL func_02009d1c(WifiList *wifiList, PlayerInfo *info, u32 *friendIndex) {
    u32 id = getIDAsUInt(info);
    u32 gender = getTrainerGender(info);
    return func_02009d50(wifiList, GetPlayerName(info), id, gender, friendIndex);
}

BOOL func_02009d50(WifiList *wifiList, const u16 *name, u32 id, u32 gender, u32 *friendIndex) {
    u8 i;

    for (i = 0; i < WIFI_LIST_FRIEND_MAX; i++) {
        if (func_02009a1c(wifiList, i) == TRUE) {
            u32 friendId = func_02009864(wifiList, i, WIFI_LIST_FRIEND_ID);
            if (friendId == id) {
                u32 friendGender = func_02009864(wifiList, i, WIFI_LIST_FRIEND_GENDER);
                if (friendGender == gender) {
                    if (friendIndex != NULL) {
                        *friendIndex = i;
                    }
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}


u32 PalPadFriendList_GetSize(void) {
    return sizeof(PalPadFriendList);
}

void PalPadFriendList_Init(PalPadFriendList *list) {
    MI_CpuFill8(list, 0, PalPadFriendList_GetSize());
}

s32 func_02009db8(PalPadFriendList *list, u32 id) {
    s32 i;

    for (i = 0; i < PAL_PAD_PLAYER_MAX; i++) {
        if (func_0200855c(&list->players[i]) != 0 && id == getIDAsUInt(&list->players[i])) {
            return i;
        }
    }
    return -1;
}

void func_02009de8(PalPadFriendList *list, PlayerInfo *info) {
    u32 id = getIDAsUInt(info);
    u32 size = func_02008468();
    s32 index;

    if (id == 0) {
        return;
    }
    index = func_02009db8(list, id);
    if (index != -1) {
        func_02008558(&list->players[index], func_02008554(info));
        return;
    }
    sys_memcpy_ex(list, &list->players[1], (PAL_PAD_PLAYER_MAX - 1) * size);
    MI_CpuCopy8(info, list, size);
}

u32 func_02009e40(PalPadFriendList *list, u32 index) {
    if (index >= PAL_PAD_PLAYER_MAX) {
        index = 0;
    }
    return getIDAsUInt(&list->players[index]);
}

PlayerInfo *func_02009e54(PalPadFriendList *list, u32 index) {
    if (index >= PAL_PAD_PLAYER_MAX) {
        return NULL;
    }
    if (func_02009e40(list, index) == 0) {
        return NULL;
    }
    return &list->players[index];
}

BOOL func_02009e74(PalPadFriendList *list, u32 id) {
    if (func_02009db8(list, id) != -1) {
        return TRUE;
    }
    return FALSE;
}

u32 func_02009e8c(PalPadFriendList *list) {
    s32 i;

    for (i = 0; i < PAL_PAD_PLAYER_MAX; i++) {
        if (func_0200855c(&list->players[i]) == 0) {
            break;
        }
    }
    return i;
}

u32 func_02009ea8(PalPadFriendList *list) {
    return list->tradeCount;
}

void func_02009eb0(PalPadFriendList *list) {
    list->tradeCount++;
    if (list->tradeCount > PAL_PAD_TRADE_COUNT_MAX) {
        list->tradeCount = PAL_PAD_TRADE_COUNT_MAX;
    }
}

PalPadFriendList *getPalPadFriendListAddress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, 0x1f);
}
