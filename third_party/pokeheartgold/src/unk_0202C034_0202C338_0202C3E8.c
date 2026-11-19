#include "global.h"

#include "constants/save_arrays.h"

#include "assert.h"
#include "gf_rtc.h"
#include "pm_string.h"
#include "unk_0202C034.h"

struct BattleRecord;

struct DWCstAccFlag;

struct DWCstAccFriendKey;

struct DWCstAccGsProfileId;

struct DWCstAccLoginId;

struct WiFiList;

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

void sub_0202C3E8(WiFiList *a0, int a1, int a2);

void sub_0202C3E8(WiFiList *a0, int a1, int a2) {
    if (a1 >= 0 && a1 < 32 && a2 >= 0 && a2 < 32) {
        MI_CpuCopy8(&a0->unk1C0[a2], &a0->unk1C0[a1], sizeof(BattleRecord));
        MI_CpuCopy8(&a0->unk40[a2], &a0->unk40[a1], sizeof(DWCFriendData));
        MIi_CpuClearFast(0, (u32 *)&a0->unk1C0[a2], sizeof(BattleRecord));
        MIi_CpuClearFast(0, (u32 *)&a0->unk40[a2], sizeof(DWCFriendData));
        a0->unk1C0[a2].unk10[0] = 0xFFFF;
        a0->unk1C0[a2].unk00[0] = 0xFFFF;
        a0->unk1C0[a2].unk2E = 2;
    }
}
