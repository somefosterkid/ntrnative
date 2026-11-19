#include "global.h"

struct DWCstAccFlag;
struct DWCstAccFriendKey;
struct DWCstAccGsProfileId;
struct DWCstAccLoginId;
struct DWCstAccUserData;
struct String;
struct WiFiList;
typedef struct DWCstAccFlag {
    u32 flags;
    u32 reserved;
    u32 reserved1;
} DWCAccFlag;
typedef struct DWCstAccLoginId {
    u32 id_data;
    u32 userid_lo32;
    u32 playerid;
} DWCAccLoginId;
typedef struct DWCstAccFriendKey {
    u32 id_data;
    u32 friendkey_lo32;
    u32 friendkey_hi32;
} DWCAccFriendKey;
typedef struct DWCstAccGsProfileId {
    u32 id_data;
    int id;
    u32 reserved;
} DWCAccGsProfileId;
typedef union DWCstAccFriendData {
    DWCAccFlag flags;
    DWCAccLoginId login_id;
    DWCAccFriendKey friend_key;
    DWCAccGsProfileId gs_profile_id;
} DWCAccFriendData;
typedef struct DWCstAccUserData {
    int size;
    DWCAccLoginId pseudo;
    DWCAccLoginId authentic;
    int gs_profile_id;
    int flag;
    u32 gamecode;
    int reserved[5];
    u32 crc32;
} DWCAccUserData;
typedef DWCAccUserData DWCUserData;
typedef DWCAccFriendData DWCFriendData;
typedef struct WiFiList WiFiList;
typedef struct {
    u16 unk_00[8];
    u16 unk_10[8];
    u32 unk_20;
    u16 unk_24;
    u16 unk_26;
    u16 unk_28;
    u16 year;
    u8 month;
    u8 day;
    u8 unk_2E;
    u8 unk_2F;
    u16 unk_30;
    u16 unk_32;
    u16 unk_34;
    u16 unk_36;
} UnkStruct_0202B370_sub1;
struct WiFiList {
    DWCUserData userData;
    DWCFriendData friendData[32];
    UnkStruct_0202B370_sub1 unk_1C0[32];
};
typedef u16 charcode_t;
typedef struct String String;
void CopyStringToU16Array(const String *src, charcode_t *dst, u32 dstSize);
BOOL DWC_IsValidFriendData(const DWCAccFriendData *frienddata);
BOOL sub_0202C2DC(WiFiList *wiFiList, int param1);

u32 sub_0202C090(WiFiList *wiFiList, int param1, int param2);
void sub_0202C190(WiFiList *wiFiList, int param1, int param2, u32 param3);
DWCFriendData *sub_0202C23C(WiFiList *wiFiList, int param1);
u16 *sub_0202C254(WiFiList *wiFiList, int param1);
void sub_0202C270(WiFiList *wiFiList, int param1, String *param2);
u16 *sub_0202C298(WiFiList *wiFiList, int param1);
void sub_0202C2B4(WiFiList *wiFiList, int param1, String *param2);
BOOL sub_0202C2DC(WiFiList *wiFiList, int param1);
int sub_0202C2F8(WiFiList *wiFiList);
int sub_0202C318(WiFiList *wiFiList);

u32 sub_0202C090(WiFiList *wiFiList, int param1, int param2) {
    u32 v0;

    ((param1 < 32) ? (void)0 : GF_AssertFail());

    switch (param2) {
    case 0:
        v0 = wiFiList->unk_1C0[param1].unk_20;
        break;
    case 1:
        v0 = wiFiList->unk_1C0[param1].unk_24;
        break;
    case 2:
        v0 = wiFiList->unk_1C0[param1].unk_26;
        break;
    case 3:
        v0 = wiFiList->unk_1C0[param1].unk_28;
        break;
    case 4:
        v0 = wiFiList->unk_1C0[param1].year;
        break;
    case 5:
        v0 = wiFiList->unk_1C0[param1].month;
        break;
    case 6:
        v0 = wiFiList->unk_1C0[param1].day;
        break;
    case 8:
        v0 = wiFiList->unk_1C0[param1].unk_2E;
        break;
    case 7:
        v0 = wiFiList->unk_1C0[param1].unk_2F;
        break;
    case 9:
        v0 = wiFiList->unk_1C0[param1].unk_30;
        break;
    case 10:
        v0 = wiFiList->unk_1C0[param1].unk_32;
        break;
    case 11:
        v0 = wiFiList->unk_1C0[param1].unk_34;
        break;
    case 12:
        v0 = wiFiList->unk_1C0[param1].unk_36;
        break;
    }

    return v0;
}

void sub_0202C190(WiFiList *wiFiList, int param1, int param2, u32 param3) {
    ((param1 < 32) ? (void)0 : GF_AssertFail());

    switch (param2) {
    case 0:
        wiFiList->unk_1C0[param1].unk_20 = param3;
        break;
    case 1:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    case 2:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    case 3:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    case 4:
        wiFiList->unk_1C0[param1].year = param3;
        break;
    case 5:
        wiFiList->unk_1C0[param1].month = param3;
        break;
    case 6:
        wiFiList->unk_1C0[param1].day = param3;
        break;
    case 8:
        wiFiList->unk_1C0[param1].unk_2E = param3;
        break;
    case 7:
        wiFiList->unk_1C0[param1].unk_2F = param3;
        break;
    case 9:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }
}

DWCFriendData *sub_0202C23C(WiFiList *wiFiList, int param1) {
    ((param1 < 32) ? (void)0 : GF_AssertFail());
    return &(wiFiList->friendData[param1]);
}

u16 *sub_0202C254(WiFiList *wiFiList, int param1) {
    ((param1 < 32) ? (void)0 : GF_AssertFail());
    return wiFiList->unk_1C0[param1].unk_10;
}

void sub_0202C270(WiFiList *wiFiList, int param1, String *param2) {
    ((param1 < 32) ? (void)0 : GF_AssertFail());
    CopyStringToU16Array(param2, wiFiList->unk_1C0[param1].unk_10, sizeof(wiFiList->unk_1C0[param1].unk_10));
}

u16 *sub_0202C298(WiFiList *wiFiList, int param1) {
    ((param1 < 32) ? (void)0 : GF_AssertFail());
    return wiFiList->unk_1C0[param1].unk_00;
}

void sub_0202C2B4(WiFiList *wiFiList, int param1, String *param2) {
    ((param1 < 32) ? (void)0 : GF_AssertFail());
    CopyStringToU16Array(param2, wiFiList->unk_1C0[param1].unk_00, sizeof(wiFiList->unk_1C0[param1].unk_00));
}

BOOL sub_0202C2DC(WiFiList *wiFiList, int param1) {
    ((param1 < 32) ? (void)0 : GF_AssertFail());
    return DWC_IsValidFriendData(&wiFiList->friendData[param1]);
}

int sub_0202C2F8(WiFiList *wiFiList) {
    int i, validFriendsCount = 0;

    for (i = 0; i < 32; i++) {
        if (sub_0202C2DC(wiFiList, i)) {
            validFriendsCount++;
        }
    }

    return validFriendsCount;
}

int sub_0202C318(WiFiList *wiFiList) {
    int v0, v1 = 0;

    for (v0 = 0; v0 < 32; v0++) {
        if (sub_0202C2DC(wiFiList, v0)) {
            v1 = v0 + 1;
        }
    }

    return v1;
}
