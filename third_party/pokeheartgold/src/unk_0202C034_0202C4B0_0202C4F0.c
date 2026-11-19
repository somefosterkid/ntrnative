#include "global.h"

struct DWCstAccFlag;

struct DWCstAccFriendKey;

struct DWCstAccGsProfileId;

struct DWCstAccLoginId;

struct DWCstAccUserData;

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

void sub_0202C4B0(WiFiList *wiFiList, int param1);

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

void sub_0202C4F0(WiFiList *wiFiList, int param1, int param2, int param3, int param4);

void sub_0202C4F0(WiFiList *wiFiList, int param1, int param2, int param3, int param4) {
    if (param1 >= 0 && param1 < 32) {
        wiFiList->unk_1C0[param1].unk_24 += param2;

        if (wiFiList->unk_1C0[param1].unk_24 > 9999) {
            wiFiList->unk_1C0[param1].unk_24 = 9999;
        }

        wiFiList->unk_1C0[param1].unk_26 += param3;

        if (wiFiList->unk_1C0[param1].unk_26 > 9999) {
            wiFiList->unk_1C0[param1].unk_26 = 9999;
        }

        wiFiList->unk_1C0[param1].unk_28 += param4;

        if (wiFiList->unk_1C0[param1].unk_28 > 9999) {
            wiFiList->unk_1C0[param1].unk_28 = 9999;
        }

        sub_0202C4B0(wiFiList, param1);
    }
}
