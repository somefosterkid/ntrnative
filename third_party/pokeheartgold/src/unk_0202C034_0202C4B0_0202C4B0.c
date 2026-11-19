#include "global.h"
#include "unk_0202C034.h"

struct BattleRecord;

struct DWCstAccFlag;

struct DWCstAccFriendKey;

struct DWCstAccGsProfileId;

struct DWCstAccLoginId;

struct WiFiList;

void GF_RTC_CopyDate(RTCDate *date);

typedef struct BattleRecord {
    u16 unk00[8];
    u16 unk10[8];
    u32 unk20;
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u16 unk30;
    u16 unk32;
    u16 unk34;
    u16 unk36;
} BattleRecord;

struct WiFiList {
    u8 unk00[0x40];
    DWCFriendData unk40[32];
    BattleRecord unk1C0[32];
};

void sub_0202C4B0(WiFiList *a0, s32 a1);

void sub_0202C4B0(WiFiList *a0, s32 a1) {
    RTCDate date;
    GF_RTC_CopyDate(&date);
    if (a1 >= 0 && a1 < 32) {
        a0->unk1C0[a1].unk2A = date.year + 2000;
        a0->unk1C0[a1].unk2C = date.month;
        a0->unk1C0[a1].unk2D = date.day;
    }
}
