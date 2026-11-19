#include "global.h"
#include "unk_0202C034.h"

struct DWCstAccFlag;
struct DWCstAccFriendKey;
struct DWCstAccGsProfileId;
struct DWCstAccLoginId;
struct DWCstAccUserData;
struct WiFiList;
BOOL sub_0202C2DC(WiFiList *wiFiList, int param1);
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
void sub_0202C3E8(WiFiList *wiFiList, int param1, int param2);

void sub_0202C46C(WiFiList *wiFiList);

void sub_0202C46C(WiFiList *wiFiList) {
    int i, v1 = -1;

    for (i = 0; i < 32; i++) {
        if (sub_0202C2DC(wiFiList, i)) {
            if (v1 != -1) {
                sub_0202C3E8(wiFiList, v1, i);

                i = -1;
                v1 = -1;
            }
        } else if (v1 == -1) {
            v1 = i;
        }
    }
}
