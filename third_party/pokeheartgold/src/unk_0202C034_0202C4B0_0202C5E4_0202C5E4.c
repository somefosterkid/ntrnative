#include "global.h"
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

void sub_0202C5E4(WiFiList *a0, int a1, int a2);

void sub_0202C5E4(WiFiList *a0, int a1, int a2) {
    if (a2 >= 0 && a2 < 32 && a1 >= 0 && a1 < 32) {
        a0->unk1C0[a2].unk24 += a0->unk1C0[a1].unk24;
        if (a0->unk1C0[a2].unk24 > 0x270F) {
            a0->unk1C0[a2].unk24 = 0x270F;
        }
        a0->unk1C0[a2].unk26 += a0->unk1C0[a1].unk26;
        if (a0->unk1C0[a2].unk26 > 0x270F) {
            a0->unk1C0[a2].unk26 = 0x270F;
        }
        a0->unk1C0[a2].unk28 += a0->unk1C0[a1].unk28;
        if (a0->unk1C0[a2].unk28 > 0x270F) {
            a0->unk1C0[a2].unk28 = 0x270F;
        }
        a0->unk1C0[a2].unk30 += a0->unk1C0[a1].unk30;
        if (a0->unk1C0[a2].unk30 > 0x270F) {
            a0->unk1C0[a2].unk30 = 0x270F;
        }
        a0->unk1C0[a2].unk32 += a0->unk1C0[a1].unk32;
        if (a0->unk1C0[a2].unk32 > 0x270F) {
            a0->unk1C0[a2].unk32 = 0x270F;
        }
        a0->unk1C0[a2].unk34 += a0->unk1C0[a1].unk34;
        if (a0->unk1C0[a2].unk34 > 0x270F) {
            a0->unk1C0[a2].unk34 = 0x270F;
        }
        a0->unk1C0[a2].unk36 += a0->unk1C0[a1].unk36;
        if (a0->unk1C0[a2].unk36 > 0x270F) {
            a0->unk1C0[a2].unk36 = 0x270F;
        }
        MIi_CpuCopyFast((u32 *)a0->unk1C0[a1].unk00, (u32 *)a0->unk1C0[a2].unk00, 16);
        MIi_CpuClearFast(0, (u32 *)&a0->unk1C0[a1], sizeof(BattleRecord));
        a0->unk1C0[a1].unk10[0] = 0xFFFF;
        a0->unk1C0[a1].unk00[0] = 0xFFFF;
        a0->unk1C0[a1].unk2E = 2;
    }
}
