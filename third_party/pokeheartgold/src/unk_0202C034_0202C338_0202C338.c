#include "global.h"
#include "unk_0202C034.h"

struct DWCstAccFlag;

struct DWCstAccFriendKey;

struct DWCstAccGsProfileId;

struct DWCstAccLoginId;

struct DWCstAccUserData;

struct WiFiList;

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

void sub_0202C338(WiFiList *wiFiList, int param1);

void sub_0202C338(WiFiList *wiFiList, int param1) {
    int v0;

    if (param1 >= 0 && param1 < 32) {
        for (v0 = param1; v0 < (32 - 1); v0++) {
            MI_CpuCopy8(&wiFiList->unk_1C0[v0 + 1], &wiFiList->unk_1C0[v0], sizeof(UnkStruct_0202B370_sub1));
            MI_CpuCopy8(&wiFiList->friendData[v0 + 1], &wiFiList->friendData[v0], sizeof(DWCFriendData));
        }

        v0 = 32 - 1;

        MI_CpuClearFast(&wiFiList->unk_1C0[v0], sizeof(UnkStruct_0202B370_sub1));
        MI_CpuClearFast(&wiFiList->friendData[v0], sizeof(DWCFriendData));

        wiFiList->unk_1C0[v0].unk_10[0] = 0xffff;
        wiFiList->unk_1C0[v0].unk_00[0] = 0xffff;
        wiFiList->unk_1C0[v0].unk_2E = 2;
    }
}
