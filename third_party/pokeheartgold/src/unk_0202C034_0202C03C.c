#include "global.h"

#include "unk_0202C034.h"
#include "unk_02037C94.h"

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

extern u32 _021D2230;

void sub_0202C03C(void *ptr) {
    WiFiList *a0 = ptr;
    s32 iVar1;
    WiFiList *pWVar2;

    MIi_CpuClearFast(0, a0, 0x8c0);
    iVar1 = 0;
    pWVar2 = a0;
    do {
        pWVar2->unk_1C0[0].unk_10[0] = 0xffff;
        pWVar2->unk_1C0[0].unk_00[0] = 0xffff;
        pWVar2->unk_1C0[0].unk_2E = 2;
        iVar1 = iVar1 + 1;
        pWVar2 = (WiFiList *)((u8 *)pWVar2 + sizeof(UnkStruct_0202B370_sub1));
    } while (iVar1 < 0x20);
    sub_0203A01C(a0);
    MI_CpuCopy8(a0, &_021D2230, 0x8c0);
    return;
}
