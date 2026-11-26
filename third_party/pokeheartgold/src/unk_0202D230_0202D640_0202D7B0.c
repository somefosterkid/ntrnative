#include "global.h"

#include "unk_0202D230.h"
#include "save_frontier.h"

BOOL sub_0202D7B0(struct WifiBattleTowerDownloadData *downloadData) {
    return *((u8 *)downloadData + 0xFE);
}

u32 sub_0202D7B8(struct WifiBattleTowerDownloadData *downloadData) {
    return (u32) * ((u8 *)downloadData + 0xFF);
}

void sub_0202D7C0(WifiBattleTowerDownloadData *arg0, void *arg1, s8 arg2, s8 arg3);

void sub_0202D7C0(WifiBattleTowerDownloadData *arg0, void *src, s8 arg2, s8 arg3) {
    MI_CpuCopy8(src, arg0->trainerRecords, 0x63CU);
    arg0->storedRank = arg2;
    arg0->storedOpponentIdx = arg3;
    arg0->hasOpponentData = 1;
}

void sub_0202D7F0(WifiBattleTowerDownloadData *arg0, void *arg1);

void sub_0202D7F0(WifiBattleTowerDownloadData *arg0, void *arg1) {
    *(u8 *)arg1 = (u8) * &arg0->storedRank;
    *(u8 *)((u8 *)arg1 + 1) = (u8) * &arg0->storedOpponentIdx;
}
